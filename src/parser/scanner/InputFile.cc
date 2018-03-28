﻿
/// @file InputFile.cc
/// @brief InputFile の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "InputFile.h"
#include <fcntl.h>

#include "ym/MsgMgr.h"

#include "parser_common.h"


// ファイル末尾に改行がなくても warning としない時に 1
#define ALLOW_EOF_WITHOUT_NL 1


BEGIN_NAMESPACE_YM_VERILOG

BEGIN_NONAMESPACE

// @brief c が識別子の先頭で用いられる文字種の時に true を返す．
inline
bool
is_strchar1(int c)
{
#if 0
  if ( isalpha(c) || c == '_' || c == '$' ) return true;
  return false;
#else
  static char table[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1,
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
  };
  return table[c];
#endif
}

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// 入力ファイルを表すためのクラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] lex 親の Lex
InputFile::InputFile(RawLex& lex) :
  Scanner(mIDO),
  mLex(lex)
{
}

// @brief デストラクタ
InputFile::~InputFile()
{
}

// @brief ファイルを開く
// @param[in] filename ファイル名
// @param[in] parent_loc インクルードされた場合の親のファイル位置
// @return 成功したら true を返す．
bool
InputFile::open(const string& filename,
		const FileLoc& parent_file)
{
  return mIDO.open(filename, parent_file);
}

// @brief トークンの読み出しを行う．
// @param[out] buff 結果の文字列を格納するバッファ
// @param[out] token_loc トークンの位置情報
int
InputFile::_read_token(StrBuff& buff)
{
  buff.clear();

  int c = get();
  set_first_loc();

  if ( c == EOF ) {
    return EOF;
  }
  if ( c == '\n' || c == '\r' ) {
    return NL;
  }

  if ( c == ' ' || c == '\t' ) {
    read_space();
    return SPACE;
  }

  switch ( mLex.context() ) {
  case RawLex::kBin:
    // 2進数モード
    return read_bin_str(c, buff);

  case RawLex::kOct:
    // 8進数モード
    return read_oct_str(c, buff);

  case RawLex::kDec:
    // 10進数モード
    return read_dec_str(c, buff);

  case RawLex::kHex:
    // 16進数モード
    return read_hex_str(c, buff);

  case RawLex::kUdp:
    switch ( c ) {
    case '-': return '-';
    case '*': return '*';
    case '0': return '0';
    case '1': return '1';
    case 'X':
    case 'x': return 'x';
    case '?': return '?';
    case 'B':
    case 'b': return 'b';
    case 'R':
    case 'r': return 'r';
    case 'F':
    case 'f': return 'f';
    case 'P':
    case 'p': return 'p';
    case 'N':
    case 'n': return 'n';
    default: break;
    }

  case RawLex::kNormal:
    break;
  }

  // ここに来るのは kUdp か kNormal
  switch ( c ) {
  case ';':
  case ':':
  case '[':
  case ']':
  case '{':
  case '}':
  case ')':
  case ',':
  case '.':
  case '@':
  case '#':
  case '%':
  case '?':
    // これらの文字は単独のトークンとなる．
    return c;

  case '\'':
    // 定数
    buff.put_char(c);
    c = get();
    if ( c == 's' || c == 'S' ) {
      buff.put_char(c);
      c = get();
    }
    if ( c == 'B' || c == 'b' ) {
      return BASE_B;
    }
    if ( c == 'O' || c == 'o' ) {
      return BASE_O;
    }
    if ( c == 'D' || c == 'd' ) {
      return BASE_D;
    }
    if ( c == 'H' || c == 'h' ) {
      return BASE_H;
    }
    {
      ostringstream buf;
      buf << "illegal character \'" << static_cast<char>(c) << "\',"
	  << "only B|b|O|o|D|d|H|h is allowed here.";
      MsgMgr::put_msg(__FILE__, __LINE__,
		      cur_loc(),
		      MsgType::Error,
		      "LEX",
		      buf.str());
    }
    return ERROR;

  case '(':
    c = peek();
    if ( c == '*' ) {
      accept();
      return PRSTAR;
    }
    return '(';

  case '+':
    c = peek();
    if ( c == ':' ) {
      accept();
      return PLUSCOLON;
    }
    return '+';

  case '-':
    c = peek();
    if ( c == ':' ) {
      accept();
      return MINUSCOLON;
    }
    if ( c == '>' ) {
      accept();
      return MINUSGT;
    }
    return '-';

  case '^':
    c = peek();
    if ( c == '~' ) {
      accept();
      return TILDEXOR;
    }
    return '^';

  case '*':
    c = peek();
    if ( c == '>' ) {
      accept();
      return STARGT;
    }
    if ( c == '*' ) {
      accept();
      return STARSTAR;
    }
    if ( c == ')' ) {
      accept();
      return STARPR;
    }
    return '*';

  case '!':
    c = peek();
    if ( c == '=' ) {
      accept();
      c = peek();
      if ( c == '=' ) {
	accept();
	return NOTEQEQ;
      }
      return NOTEQ;
    }
    return '!';

  case '&':
    c = peek();
    if ( c == '&' ) {
      accept();
      c = peek();
      if ( c == '&' ) {
	accept();
	return ANDANDAND;
      }
      return ANDAND;
    }
    return '&';

  case '|':
    c = peek();
    if ( c == '|' ) {
      accept();
      return OROR;
    }
    return '|';

  case '~':
    c = peek();
    if ( c == '&' ) {
      accept();
      return TILDEAND;
    }
    if ( c == '|' ) {
      accept();
      return TILDEOR;
    }
    if ( c == '^' ) {
      accept();
      return TILDEXOR;
    }
    return '~';

  case '<':
    c = peek();
    if ( c == '<' ) {
      accept();
      c = peek();
      if ( c == '<' ) {
	accept();
	return LTLTLT;
      }
      return LTLT;
    }
    if ( c == '=' ) {
      accept();
      return LTEQ;
    }
    return '<';

  case '>':
    c = peek();
    if ( c == '>' ) {
      accept();
      c = peek();
      if ( c == '>' ) {
	accept();
	return GTGTGT;
      }
      return GTGT;
    }
    if ( c == '=' ) {
      accept();
      return GTEQ;
    }
    return '>';

  case '=':
    c = peek();
    if ( c == '=' ) {
      accept();
      c = peek();
      if ( c == '=' ) {
	accept();
	return EQEQEQ;
      }
      return EQEQ;
    }
    if ( c == '>' ) {
      accept();
      return EQGT;
    }
    return '=';

  case '\"':
    return read_dq_str(buff);

  case '\\':
    return read_esc_str(buff);

  case '`':
    buff.put_char(c);
    c = get();
    if ( is_strchar1(c) ) {
      buff.put_char(c);
      read_str(buff);
      return CD_SYMBOL;
    }
    else {
      ostringstream buf;
      buf << "illegal charactor \'" << static_cast<char>(c)
	  << "\' [" << c << " in digit code].";
      MsgMgr::put_msg(__FILE__, __LINE__,
		      cur_loc(),
		      MsgType::Error,
		      "LEX",
		      buf.str());
      return ERROR;
    }

  case '/':
    return read_comment(buff);

  default:
    break;
  }

  if ( is_strchar1(c) ) {
    // 通常の識別子
    buff.put_char(c);
    // 文字列要素が表れる限り mStringBuff につむ．
    read_str(buff);

    // buff の内容をチェックする．
    // 特例: 単独の $
    if ( strcmp(buff.c_str(), "$") == 0 ) {
      return '$';
    }

    // 取り合えずここでは予約語の検査はせずに IDENTIFIER としておく
    return IDENTIFIER;
  }

  if ( isdigit(c) ) {
    // 数字
    buff.put_char(c);
    return read_num(buff);
  }

  ostringstream buf;
  buf << "illegal charactor \'" << static_cast<char>(c)
      << "\' [" << c << " in digit code].";
  MsgMgr::put_msg(__FILE__, __LINE__,
		  cur_loc(),
		  MsgType::Error,
		  "LEX",
		  buf.str());
  return ERROR;
}


BEGIN_NONAMESPACE

// 2進数モードで使える文字
inline
bool
is_binchar(int c)
{
#if 0
  if ( c == '0' || c == '1' ||
       c == 'x' || c == 'X' ||
       c == 'z' || c == 'Z' ||
       c == '?' ) {
    return true;
  }
  else {
    return false;
  }
#else
  static char table[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
  };
  return table[c];
#endif
}

END_NONAMESPACE

// @brief 2進数モードの読み込みを行う．
// @param[in] c 最初の文字
// @param[out] buff 結果を格納するバッファ
// @return トークンを返す．
int
InputFile::read_bin_str(int c,
			StrBuff& buff)
{
  if ( is_binchar(c) ) {
    buff.put_char(c);
    for ( ; ; ) {
      c = peek();
      if ( is_binchar(c) ) {
	accept();
	buff.put_char(c);
      }
      else if ( c == '_' ) {
	accept();
	// ただ読み飛ばす
      }
      else {
	// c == EOF も含む．
	break;
      }
    }
    return UNUMBER;
  }

  ostringstream msg_buf;
  msg_buf << "illegal charactor \'" << static_cast<char>(c) << "\',"
	  << "only \'01xXzZ?\' are allowed here.";
  MsgMgr::put_msg(__FILE__, __LINE__,
		  cur_loc(),
		  MsgType::Error,
		  "LEX",
		  msg_buf.str());
  return ERROR;
}


BEGIN_NONAMESPACE

// 8進数モードで使える文字
inline
bool
is_octchar(int c)
{
#if 0
  if ( ('0' <= c && c <= '7') ||
       c == 'x' || c == 'X' ||
       c == 'z' || c == 'Z' ||
       c == '?' ) {
    return true;
  }
  else {
    return false;
  }
#else
  static char table[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
  };
  return table[c];
#endif
}

END_NONAMESPACE

// @brief 8進数モードの読み込みを行う．
// @param[in] c 最初の文字
// @param[out] buff 結果を格納するバッファ
// @return トークンを返す．
int
InputFile::read_oct_str(int c,
			StrBuff& buff)
{
  if ( is_octchar(c) ) {
    buff.put_char(c);
    for ( ; ; ) {
      c = peek();
      if ( is_octchar(c) ) {
	accept();
	buff.put_char(c);
      }
      else if ( c == '_' ) {
	accept();
	// ただ読み飛ばす．
      }
      else {
	// c == EOF も含む．
	break;
      }
    }
    return UNUMBER;
  }

  ostringstream msg_buf;
  msg_buf << "illegal charactor \'" << static_cast<char>(c) << "\',"
	  << "only \'0-7xXzZ?\' are allowed here.";
  MsgMgr::put_msg(__FILE__, __LINE__,
		  cur_loc(),
		  MsgType::Error,
		  "LEX",
		  msg_buf.str());
  return ERROR;
}


BEGIN_NONAMESPACE

// 10進数モードで使える文字
inline
bool
is_decchar(int c)
{
#if 0
  if ( '0' <= c && c <= '9' ) {
    return true;
  }
  else {
    return false;
  }
#else
  static char table[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
  };
  return table[c];
#endif
}

END_NONAMESPACE

// @brief 10進数モードの読み込みを行う．
// @param[in] c 最初の文字
// @param[out] buff 結果を格納するバッファ
// @return トークンを返す．
int
InputFile::read_dec_str(int c,
			StrBuff& buff)
{
  if ( is_decchar(c) ) {
    buff.put_char(c);
    for ( ; ; ) {
      c = peek();
      if ( is_decchar(c) ) {
	accept();
	buff.put_char(c);
      }
      else if ( c == '_' ) {
	accept();
	// ただ読み飛ばす．
      }
      else {
	// c == EOF も含む．
	break;
      }
    }
    return UNUMBER;
  }
  else if ( c == 'x' || c == 'X' ||
	    c == 'Z' || c == 'Z' ||
	    c == '?' ) {
    buff.put_char(c);
    // xz? と他の数字との混在はない．
    for ( ; ; ) {
      c = peek();
      if ( c == '_' ) {
	accept();
	// ただ読み飛ばす．
      }
      else {
	break;
      }
    }
    return UNUMBER;
  }

  ostringstream msg_buf;
  msg_buf << "illegal charactor \'" << static_cast<char>(c) << "\',"
	  << "only \'0-9xXzZ?\' are allowed here.";
  MsgMgr::put_msg(__FILE__, __LINE__,
		  cur_loc(),
		  MsgType::Error,
		  "LEX",
		  msg_buf.str());
  return ERROR;
}


BEGIN_NONAMESPACE

// 16進数モードで使える文字
inline
bool
is_hexchar(int c)
{
#if 0
  if ( ('0' <= c && c <= '9') ||
       ('a' <= c && c <= 'f') ||
       ('A' <= c && c <= 'F') ||
       c == 'x' || c == 'X' ||
       c == 'z' || c == 'Z' ||
       c == '?' ) {
    return true;
  }
  else {
    return false;
  }
#else
  static char table[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1,
    0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0,
    0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
  };
  return table[c];
#endif
}

END_NONAMESPACE

// @brief 16進数モードの読み込みを行う．
// @param[in] c 最初の文字
// @param[out] buff 結果を格納するバッファ
// @return トークンを返す．
int
InputFile::read_hex_str(int c,
			StrBuff& buff)
{
  if ( is_hexchar(c) ) {
    buff.put_char(c);
    for ( ; ; ) {
      c = peek();
      if ( is_hexchar(c) ) {
	accept();
	buff.put_char(c);
      }
      else if ( c == '_' ) {
	accept();
	// ただ読み飛ばす．
      }
      else {
	// c == EOF も含む．
	break;
      }
    }
    return UNUMBER;
  }

  ostringstream msg_buf;
  msg_buf << "illegal charactor \'" << static_cast<char>(c) << "\',"
	  << "only \'0-9a-ha-HxXzZ?\' are allowed here.";
  MsgMgr::put_msg(__FILE__, __LINE__,
		  cur_loc(),
		  MsgType::Error,
		  "LEX",
		  msg_buf.str());
  return ERROR;
}

// 識別子で使える文字
inline
bool
is_strchar(int c)
{
#if 0
  if ( isalnum(c) || c == '_' || c == '$' ) {
    return true;
  }
  else {
    return false;
#else
  static char table[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1,
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
  };
  return table[c];
#endif
}

// @brief 識別子に用いられる文字([a-zA-Z0-9_$])が続く限り読みつづける．
// @param[out] buf 結果を格納する文字列バッファ
void
InputFile::read_str(StrBuff& buff)
{
  for ( ; ; ) {
    int c = peek();
    if ( is_strchar(c) ) {
      accept();
      buff.put_char(c);
    }
    else {
      // [a-zA-Z0-9_$] でなければ終わり
      return;
    }
  }
}

// @brief 二重引用符用の読み込み
// @param[out] buf 結果を格納する文字列バッファ
// @return トークン番号を返す．
// @note 可能性のあるトークンは
//  - STRING
//  - ERROR
int
InputFile::read_dq_str(StrBuff& buff)
{
  // goto 文の嵐だが，全体の構造はいくつかの無限ループのブロック間の
  // 移動に goto 文を用いているだけ．
  // 各々の無限ループを状態ととらえて状態遷移だと思えば良い．

  // 8進数モードの値
  int cur_val = 0;

 INIT:
  for ( ; ; ) {
    int c = peek();
    if ( c == '\"' ) {
      accept();
      // 終わり
      return STRING;
    }
    else if ( c == '\n' || c == '\r' || c == EOF ) {
      accept();
      // 文字列が終わらないうちに改行が来てしまった．
      goto ERR_END;
    }
    else if ( c == '\\' ) {
      accept();
      goto BSLASH;
    }
    else {
      accept();
      buff.put_char(c);
    }
  }

 BSLASH:
  for ( ; ; ) {
    int c = peek();
    if ( c == 'n' || c == 't' ) {
      accept();
      buff.put_char('\\');
      buff.put_char(c);
      goto INIT;
    }
    else if ( c == '\n' ) {
      accept();
      // これでいいのか良くわからないけどスペースに変える．
      buff.put_char(' ');
      goto INIT;
    }
    else if ( c >= '0' && c <= '7' ) {
      accept();
      cur_val = c - '0';
      goto BSLASH1;
    }
    else if ( c == EOF ) {
      goto ERR_END;
    }
    else {
      accept();
      // 上記以外の文字ならバックスラッシュの意味はない
      buff.put_char(c);
      goto INIT;
    }
  }

 BSLASH1:
  for ( ; ; ) {
    int c = peek();
    if ( c >= '0' && c <= '7' ) {
      accept();
      cur_val = cur_val * 8 + (c - '0');
      goto BSLASH2;
    }
    else if ( c == EOF ) {
      goto ERR_END;
    }
    else {
      buff.put_char(static_cast<char>(cur_val));
      goto INIT;
    }
  }

 BSLASH2:
  for ( ; ; ) {
    int c = peek();
    if ( c >= '0' && c <= '7' ) {
      accept();
      cur_val = cur_val * 8 + (c - '0');
    }
    else if ( c == EOF ) {
      goto ERR_END;
    }
    buff.put_char(static_cast<char>(cur_val));
    goto INIT;
  }

 ERR_END:
  // 文字列が終わらないうちに改行が来てしまった．
  MsgMgr::put_msg(__FILE__, __LINE__,
		  cur_loc(),
		  MsgType::Error,
		  "LEX",
		  "new line in quoted string.");
  return ERROR;
}

// @brief escaped identifier 用の読み込み
// @param[out] buf 結果を格納する文字列バッファ
// @param[out] buf 結果を格納する文字列バッファ
// @return トークン番号を返す．
// @note 可能性のあるトークンは
//  - SPACE
//  - IDENTIFIER
//  - ERROR
int
InputFile::read_esc_str(StrBuff& buff)
{
  int c = peek();
  if ( c == ' ' || c == '\t' || c == '\n' ) {
    accept();
    // ただの空白に置き換える．
    return SPACE;
  }
  if ( !isascii(c) ) {
    // escaped identifier でも非 ascii 文字は違反
    MsgMgr::put_msg(__FILE__, __LINE__,
		    cur_loc(),
		    MsgType::Error,
		    "LEX",
		    "non-ascii character in escaped string.");
    return ERROR;
  }

  // escaped identifier モード
  accept();
  buff.put_char(c);
  for ( ; ; ) {
    int c = peek();
    if ( c == ' ' || c == '\t' || c == '\n' || c == '\r' ) {
      // 空白改行文字なら終わり
      return IDENTIFIER;
    }
    else if ( isascii(c) ) {
      accept();
      buff.put_char(c);
    }
    else {
      // 非 ascii 文字はエラー
      // ただしその直前までを escaped identifier とする．
      // 次の read_token() でエラーとなるはず．
      break;
    }
  }

  return IDENTIFIER;
}

// @brief 数字を読み込む．
// @param[out] buf 結果を格納する文字列バッファ
// @return トークン番号を返す．
// @note 可能性のあるトークンは
//  - UNUM_INT
//  - RNUMBER
//  - ERROR
int
InputFile::read_num(StrBuff& buff)
{
  for ( ; ; ) {
    int c = peek();
    if ( c == '_' ) {
      accept();
      // ただ読み飛ばす．
    }
    else if ( isdigit(c) ) {
      accept();
      buff.put_char(c);
    }
    else if ( c == '.' ) {
      accept();
      buff.put_char(c);
      goto AFTER_DOT;
    }
    else if ( c == 'e' || c == 'E' ) {
      accept();
      buff.put_char(c);
      goto AFTER_EXP;
    }
    else {
      break;
    }
  }
  return UNUM_INT;

 AFTER_DOT:
  for ( ; ; ) {
    int c = peek();
    if ( c == '_' ) {
      accept();
      // ただ読み飛ばす．
    }
    else if ( isdigit(c) ) {
      accept();
      buff.put_char(c);
      goto AFTER_DOT_NUM;
    }
    else {
      break;
    }
  }
  return ERROR;

 AFTER_DOT_NUM:
  for ( ; ; ) {
    int c = peek();
    if ( c == '_' ) {
      accept();
      // ただ読み飛ばす．
    }
    else if ( c == 'e' || c == 'E' ) {
      accept();
      buff.put_char(c);
      goto AFTER_EXP;
    }
    else if ( isdigit(c) ) {
      accept();
      buff.put_char(c);
    }
    else {
      break;
    }
  }
  return RNUMBER;

 AFTER_EXP:
  for ( ; ; ) {
    int c = peek();
    if ( c == '_' ) {
      accept();
      // ただ読み飛ばす．
    }
    else if ( c == '+' || c == '-' ) {
      accept();
      buff.put_char(c);
      goto AFTER_EXP_SIGN;
    }
    else if ( isdigit(c) ) {
      buff.put_char('+');
      accept();
      buff.put_char(c);
      goto AFTER_EXP_NUM;
    }
    else {
      break;
    }
  }
  return ERROR;

 AFTER_EXP_SIGN:
  for ( ; ; ) {
    int c = peek();
    if ( c == '_' ) {
      accept();
      // ただ読み飛ばす．
    }
    else if ( isdigit(c) ) {
      accept();
      buff.put_char(c);
      goto AFTER_EXP_NUM;
    }
    else {
      break;
    }
  }
  return ERROR;

 AFTER_EXP_NUM:
  for ( ; ; ) {
    int c = peek();
    if ( c == '_' ) {
      accept();
      // ただ読み飛ばす．
    }
    else if ( isdigit(c) ) {
      accept();
      buff.put_char(c);
    }
    else {
      break;
    }
  }
  return RNUMBER;
}

// @brief 空白文字を読み飛ばす
void
InputFile::read_space()
{
  for ( ; ; ) {
    int c = peek();
    if ( c == ' ' || c == '\t' ) {
      accept();
    }
    else {
      break;
    }
  }
}

// @brief '/' を読んだ直後の処理
// @param[out] buf コメントを格納する文字列バッファ
// @return トークン番号を返す．
// @note 可能性のあるトークンは
//  - COMMENT1
//  - COMMENT2
//  - '/'
//  - ERROR
int
InputFile::read_comment(StrBuff& buff)
{
  int c = peek();
  if ( c == '/' ) {
    accept();

    buff.put_char('/');
    buff.put_char('/');

    // 意味的には以下のコードと等価な処理を行う．
    // for ( ; ; ) {
    //   int c = peek();
    //   accept();
    //   if ( c == '\n' ) {
    //     break;
    //   }
    // }
    for ( ; ; ) {
      c = peek();
      if ( c == '\n' || c == '\r' ) {
	// 末尾の改行は buf に積まない．
	return COMMENT1;
      }
      else if ( c == EOF ) {
	// 末尾に NL がなく EOF の場合の処理はこれでいいの？
	return COMMENT1;
      }
      else {
	accept();
	buff.put_char(c);
      }
    }
  }
  else if ( c == '*' ) {
    accept();

    buff.put_char('/');
    buff.put_char('*');

    // 直前の文字が '*' の時 true となるフラグ
    bool star = false;
    for ( ; ; ) {
      c = peek();
      if ( c == '\n' || c == '\r' ) {
	accept();
	buff.put_char('\n');
      }
      else if ( c == '*' ) {
	accept();
	buff.put_char(c);
	star = true;
      }
      else if ( star && c == '/' ) {
	accept();
	buff.put_char(c);
	// '*/' を読み込んだ
	return COMMENT2;
      }
      else if ( c == EOF ) {
	// '*/' を読む前に EOF になってしまった．
	MsgMgr::put_msg(__FILE__, __LINE__,
			cur_loc(),
			MsgType::Error,
			"LEX",
			"unexpected end-of-file in comment block(/*).");
	return ERROR;
      }
      else {
	accept();
	buff.put_char(c);
	star = false;
      }
    }
  }
  else {
    return '/';
  }
}

// @brief 改行を読み込んだ時に起動する関数
// @param[in] line 行番号
void
InputFile::check_line(ymuint line)
{
  mLex.check_line(line);
}

END_NAMESPACE_YM_VERILOG
