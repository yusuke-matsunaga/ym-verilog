
/// @file RawLex.cc
/// @brief RawLex の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014, 2019 Yusuke Matsunaga
/// All rights reserved.


#include "scanner/RawLex.h"
#include "LexPluginDict.h"

#include "LexPlugin.h"
#include "LexState.h"
#include "LexCondState.h"
#include "LexCondPlugin.h"
#include "LexMacroPlugin.h"
#include "StdLexPlugin.h"
#include "InputMgr.h"
#include "RsrvWordDic.h"
#include "MacroSource.h"
#include "TokenInfo.h"
#include "common.h"
#include "print_token.h"

#include "ym/VlLineWatcher.h"

#include "ym/MsgMgr.h"


BEGIN_NAMESPACE_YM_VERILOG

BEGIN_NONAMESPACE

// @brief c が識別子の先頭で用いられる文字種の時に true を返す．
inline
bool
is_strchar1(int c)
{
  // make_table.py strchar1 で作成
  // c++ コードの等価記述: isalpha(c) || c == '_' || c == '$'
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
}

// 識別子で使える文字
inline
bool
is_strchar(int c)
{
  // make_table.py strchar で生成
  // c++ の等価記述
  // isalnum(c) || c == '_' || c == '$'
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
}

// 2進数モードで使える文字
inline
bool
is_binchar(int c)
{
  // make_table.py binchar で作成
  // c++ の等価記述
  // c == '0' || c == '1' ||
  // c == 'x' || c == 'X' ||
  // c == 'z' || c == 'Z' ||
  // c == '?'
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
}

// 8進数モードで使える文字
inline
bool
is_octchar(int c)
{
  // make_table.py octchar で生成
  // c++ の等価記述
  //  ('0' <= c && c <= '7') ||
  //  c == 'x' || c == 'X' ||
  //  c == 'z' || c == 'Z' ||
  //  c == '?'
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
}

// 10進数モードで使える文字
inline
bool
is_decchar(int c)
{
  // make_table.py dechar で生成
  // c++ の等価記述
  // '0' <= c && c <= '9'
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
}

// 16進数モードで使える文字
inline
bool
is_hexchar(int c)
{
  // make_table.py hexchar で生成
  // c++ の等価記述
  // ('0' <= c && c <= '9') ||
  // ('a' <= c && c <= 'f') ||
  // ('A' <= c && c <= 'F') ||
  //  c == 'x' || c == 'X' ||
  //  c == 'z' || c == 'Z' ||
  //  c == '?'
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
}

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// RawLex のパブリックなメンバ関数
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
RawLex::RawLex() :
  mInputMgr{new InputMgr()},
  mDic{RsrvWordDic::the_dic()},
  mCondState{new LexCondState(*this)},
  mPluginDict{new LexPluginDict},
  mDebug{false}
{
  new LpIfdef(*this, "ifdef", mCondState);
  new LpIfdef(*this, "ifndef", mCondState);
  new LpElse(*this, "else", mCondState);
  new LpElsif(*this, "elsif", mCondState);
  new LpEndif(*this, "endif", mCondState);

  new LpInclude(*this, "include", mInputMgr.get());
  new LpLine(*this, "line", mInputMgr.get());

  new LpDefine(*this, "define");
  new LpUndef(*this, "undef");

  new LpResetAll(*this, "resetall");

  clear();
}

// @brief デストラクタ
RawLex::~RawLex()
{
  clear();

  // LexPlugin は LexPluginDict のデストラクタで削除される．
}

// @brief 初期状態に戻す．
// 読み込みの途中でこれを呼ぶと大変なことになる．
// 普通は正常に読み終わったあとか, エラーの後で呼ぶ．
void
RawLex::clear()
{
  mModuleMode = false;

  mMacroStack.clear();

  resetall(FileRegion());

  mInputMgr->clear();

  mContext = kNormal;
}

// @brief サーチパスリストを設定する．
// @param[in] searchpath セットするサーチパス
void
RawLex::set_searchpath(const SearchPathList& searchpath)
{
  mInputMgr->set_searchpath(searchpath);
}

// @brief ファイルをオープンする．
// @param[in] filename ファイル名
// @retval true オープンに成功した．
// @retval false ファイルが開けなかった
bool
RawLex::open_file(const string& filename)
{
  return mInputMgr->open_file(filename);
}


//////////////////////////////////////////////////////////////////////
// トークンの読み出し関係
//////////////////////////////////////////////////////////////////////

// @brief 一語を読み込む
// @return 読み込んだトークンの id を返す．
int
RawLex::get_token()
{
  // 本当の処理は get_raw_token() で行っている．
  // ここでの主要な処理は compiler directive の処理
  for ( ; ; ) {
    int id = get_raw_token();
    if ( mCondState->cond() ) {
      switch ( id ) {
      case EOF:
	if ( mDebug ) {
	  MsgMgr::put_msg(__FILE__, __LINE__,
			  cur_token_loc(),
			  MsgType::Debug,
			  "LEX",
			  "Reached to the EOF.");
	}
	return EOF;

      case SPACE:
      case NL:
      case COMMENT1:
      case COMMENT2:
	continue;

      case CD_SYMBOL:
	{
	  // 先頭の '`' をスキップする．
	  const char* macroname = cur_string() + 1;
	  LexPlugin* plugin = mPluginDict->find_plugin(macroname);
	  if ( plugin == nullptr ) {
	    ostringstream buf;
	    buf << "macro `" << macroname << " is not defined.";
	    MsgMgr::put_msg(__FILE__, __LINE__,
			    cur_token_loc(),
			    MsgType::Error,
			    "LEX",
			    buf.str());
	    goto error;
	  }
	  if ( !plugin->parse() ) {
	    goto error;
	  }
	}
	continue;

      case MODULE:
      case MACROMODULE:
	// module 内に入ったことを記録しておく
	mModuleMode = true;
	break;

      case ENDMODULE:
	// module 定義から抜けたことを記録しておく．
	mModuleMode = false;
	break;

      case UNUM_INT:
	{
	  // 小さい整数か大きい整数か判断する．
	  const char* tmp = cur_string();
	  ymuint end = strlen(tmp);
	  const ymuint sft = sizeof(ymuint32) * 8 - 4;
	  mCurUint = 0;
	  bool overflow = false;
	  for (ymuint pos = 0; pos < end; ++ pos) {
	    char c = tmp[pos];
	    ymuint32 v = c - '0';
	    ymuint32 u = mCurUint >> sft;
	    ymuint32 l = mCurUint - (u << sft);
	    ymuint32 u10 = u * 10;
	    ymuint32 l10 = l * 10;
	    mCurUint = v + l10 + ((u10 % 16) << sft);
	    if ( u10 / 16 > 0 ) {
	      // 桁あふれ
	      overflow = true;
	      break;
	    }
	  }
	  if ( overflow ) {
	    // 大きい整数型
	    id = UNUM_BIG;
	  }
	  // そうでなければ mCurUint に正しい値が入っている．
	}
	break;

      default:
	break;
      }

      if ( mDebug ) {
	ostringstream buf;
	print_token(buf, id, cur_string());
	MsgMgr::put_msg(__FILE__, __LINE__,
			cur_token_loc(),
			MsgType::Debug,
			"LEX",
			buf.str());
      }
      return id;
    }
    else {
      // 条件が成り立っていない場合は `ifdef 関係のディレクティブのみ
      // パーズすればよい
      switch ( id ) {
      case EOF:
	MsgMgr::put_msg(__FILE__, __LINE__,
			cur_token_loc(),
			MsgType::Error,
			"LEX",
			"Unexpected EOF.");
	return ERROR;

      case CD_SYMBOL:
	{
	  // 先頭の '`' をスキップする．
	  const char* macroname = cur_string() + 1;
	  LexPlugin* plugin = mPluginDict->find_plugin(macroname);
	  if ( plugin && plugin->is_cond_plugin() ) {
	    if ( !plugin->parse() ) {
	      goto error;
	    }
	  }
	}
	continue;

      default:
	continue;
      }
    }
  }

 error:
  if ( mDebug ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    cur_token_loc(),
		    MsgType::Debug,
		    "LEX",
		    "Error exit.");
  }
  return ERROR;
}

// @brief トークンの読み出し
// @return 読み出されたトークン
int
RawLex::get_raw_token()
{
  // マクロ置き換え中の場合にはマクロソースから読み込みを行う．
  while ( !mMacroStack.empty() ) {
    MacroSource& macro = mMacroStack.back();
    const TokenInfo* token = macro.get();
    if ( token ) {
      mCurPos = token->loc();
      mCurString = token->str();
      if ( mDebug ) {
	ostringstream buf;
	buf << "get_raw_token(from macro) ==> ";
	print_token(buf, token->id(), mCurString);
	MsgMgr::put_msg(__FILE__, __LINE__,
			cur_token_loc(),
			MsgType::Debug,
			"LEX",
			buf.str());
      }
      return token->id();
    }
    mMacroStack.pop_back();
  }

  // 通常の読み込み
 LOOP:
  int id = _read_token();
  mCurString = mStringBuff.c_str();
  mCurPos = FileRegion{mFirstLoc, cur_loc()};

  switch ( id ) {
  case IDENTIFIER:
    // buff が予約語かどうか判定する．
    id = mDic.token(mCurString);

    // UDP のテーブル定義の中はルールが変わるのでモードを用意しておく
    if ( id == TABLE ) {
      mContext = kUdp;
    }
    else if ( id == ENDTABLE ) {
      mContext = kNormal;
    }
    break;

  case BASE_B:
    mContext = kBin;
    break;

  case BASE_O:
    mContext = kOct;
    break;

  case BASE_D:
    mContext = kDec;
    break;

  case BASE_H:
    mContext = kHex;
    break;

  case UNUMBER:
  case ERROR:
    mContext = kNormal;
    break;

  case EOF:
    if ( mInputMgr->wrap_up() ) {
      goto LOOP;
    }
    break;

  default:
    break;
  }

  if ( mDebug ) {
    ostringstream buf;
    buf << "get_raw_token(from "
	<< mInputMgr->cur_filename() << ") ==> ";
    print_token(buf, id, mCurString);
    MsgMgr::put_msg(__FILE__, __LINE__,
		    cur_token_loc(),
		    MsgType::Debug,
		    "LEX",
		    buf.str());
  }

  return id;
}

// @brief トークンの読み出しを行う．
// @param[out] buff 結果の文字列を格納するバッファ
int
RawLex::_read_token()
{
  mStringBuff.clear();

  int c = get();
  mFirstLoc = cur_loc();

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

  switch ( context() ) {
  case kBin:
    // 2進数モード
    return read_bin_str(c);

  case kOct:
    // 8進数モード
    return read_oct_str(c);

  case kDec:
    // 10進数モード
    return read_dec_str(c);

  case kHex:
    // 16進数モード
    return read_hex_str(c);

  case kUdp:
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

  case kNormal:
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
    mStringBuff.put_char(c);
    c = get();
    if ( c == 's' || c == 'S' ) {
      mStringBuff.put_char(c);
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
    return read_dq_str();

  case '\\':
    return read_esc_str();

  case '`':
    mStringBuff.put_char(c);
    c = get();
    if ( is_strchar1(c) ) {
      mStringBuff.put_char(c);
      read_str();
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
    return read_comment();

  default:
    break;
  }

  if ( is_strchar1(c) ) {
    // 通常の識別子
    mStringBuff.put_char(c);
    // 文字列要素が表れる限り mStringBuff につむ．
    read_str();

    // buff の内容をチェックする．
    // 特例: 単独の $
    if ( strcmp(mStringBuff.c_str(), "$") == 0 ) {
      return '$';
    }

    // 取り合えずここでは予約語の検査はせずに IDENTIFIER としておく
    return IDENTIFIER;
  }

  if ( isdigit(c) ) {
    // 数字
    mStringBuff.put_char(c);
    return read_num();
  }

  ostringstream msg_buf;
  msg_buf << "illegal charactor \'" << static_cast<char>(c)
	  << "\' [" << c << " in digit code].";
  MsgMgr::put_msg(__FILE__, __LINE__,
		  cur_loc(),
		  MsgType::Error,
		  "LEX",
		  msg_buf.str());
  return ERROR;
}

// @brief 2進数モードの読み込みを行う．
// @param[in] c 最初の文字
// @param[out] buff 結果を格納するバッファ
// @return トークンを返す．
int
RawLex::read_bin_str(int c)
{
  if ( is_binchar(c) ) {
    mStringBuff.put_char(c);
    for ( ; ; ) {
      c = peek();
      if ( is_binchar(c) ) {
	accept();
	mStringBuff.put_char(c);
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

// @brief 8進数モードの読み込みを行う．
// @param[in] c 最初の文字
// @param[out] buff 結果を格納するバッファ
// @return トークンを返す．
int
RawLex::read_oct_str(int c)
{
  if ( is_octchar(c) ) {
    mStringBuff.put_char(c);
    for ( ; ; ) {
      c = peek();
      if ( is_octchar(c) ) {
	accept();
	mStringBuff.put_char(c);
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

// @brief 10進数モードの読み込みを行う．
// @param[in] c 最初の文字
// @return トークンを返す．
int
RawLex::read_dec_str(int c)
{
  if ( is_decchar(c) ) {
    mStringBuff.put_char(c);
    for ( ; ; ) {
      c = peek();
      if ( is_decchar(c) ) {
	accept();
	mStringBuff.put_char(c);
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
    mStringBuff.put_char(c);
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

// @brief 16進数モードの読み込みを行う．
// @param[in] c 最初の文字
// @return トークンを返す．
int
RawLex::read_hex_str(int c)
{
  if ( is_hexchar(c) ) {
    mStringBuff.put_char(c);
    for ( ; ; ) {
      c = peek();
      if ( is_hexchar(c) ) {
	accept();
	mStringBuff.put_char(c);
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

// @brief 識別子に用いられる文字([a-zA-Z0-9_$])が続く限り読みつづける．
// @param[out] buf 結果を格納する文字列バッファ
void
RawLex::read_str()
{
  for ( ; ; ) {
    int c = peek();
    if ( is_strchar(c) ) {
      accept();
      mStringBuff.put_char(c);
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
RawLex::read_dq_str()
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
      mStringBuff.put_char(c);
    }
  }

 BSLASH:
  for ( ; ; ) {
    int c = peek();
    if ( c == 'n' || c == 't' ) {
      accept();
      mStringBuff.put_char('\\');
      mStringBuff.put_char(c);
      goto INIT;
    }
    else if ( c == '\n' ) {
      accept();
      // これでいいのか良くわからないけどスペースに変える．
      mStringBuff.put_char(' ');
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
      mStringBuff.put_char(c);
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
      mStringBuff.put_char(static_cast<char>(cur_val));
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
    mStringBuff.put_char(static_cast<char>(cur_val));
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
// @return トークン番号を返す．
// @note 可能性のあるトークンは
//  - SPACE
//  - IDENTIFIER
//  - ERROR
int
RawLex::read_esc_str()
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
  mStringBuff.put_char(c);
  for ( ; ; ) {
    int c = peek();
    if ( c == ' ' || c == '\t' || c == '\n' || c == '\r' ) {
      // 空白改行文字なら終わり
      return IDENTIFIER;
    }
    else if ( isascii(c) ) {
      accept();
      mStringBuff.put_char(c);
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
RawLex::read_num()
{
  for ( ; ; ) {
    int c = peek();
    if ( c == '_' ) {
      accept();
      // ただ読み飛ばす．
    }
    else if ( isdigit(c) ) {
      accept();
      mStringBuff.put_char(c);
    }
    else if ( c == '.' ) {
      accept();
      mStringBuff.put_char(c);
      goto AFTER_DOT;
    }
    else if ( c == 'e' || c == 'E' ) {
      accept();
      mStringBuff.put_char(c);
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
      mStringBuff.put_char(c);
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
      mStringBuff.put_char(c);
      goto AFTER_EXP;
    }
    else if ( isdigit(c) ) {
      accept();
      mStringBuff.put_char(c);
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
      mStringBuff.put_char(c);
      goto AFTER_EXP_SIGN;
    }
    else if ( isdigit(c) ) {
      mStringBuff.put_char('+');
      accept();
      mStringBuff.put_char(c);
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
      mStringBuff.put_char(c);
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
      mStringBuff.put_char(c);
    }
    else {
      break;
    }
  }
  return RNUMBER;
}

// @brief 空白文字を読み飛ばす
void
RawLex::read_space()
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
// @return トークン番号を返す．
// @note 可能性のあるトークンは
//  - COMMENT1
//  - COMMENT2
//  - '/'
//  - ERROR
int
RawLex::read_comment()
{
  int c = peek();
  if ( c == '/' ) {
    accept();

    mStringBuff.put_char('/');
    mStringBuff.put_char('/');

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
      if ( c == '\n' ) {
	// 末尾の改行は buf に積まない．
	return COMMENT1;
      }
      else if ( c == EOF ) {
	// 末尾に NL がなく EOF の場合の処理はこれでいいの？
	return COMMENT1;
      }
      else {
	accept();
	mStringBuff.put_char(c);
      }
    }
  }
  else if ( c == '*' ) {
    accept();

    mStringBuff.put_char('/');
    mStringBuff.put_char('*');

    // 直前の文字が '*' の時 true となるフラグ
    bool star = false;
    for ( ; ; ) {
      c = peek();
      if ( c == '\n' ) {
	accept();
	mStringBuff.put_char('\n');
      }
      else if ( c == '*' ) {
	accept();
	mStringBuff.put_char(c);
	star = true;
      }
      else if ( star && c == '/' ) {
	accept();
	mStringBuff.put_char(c);
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
	mStringBuff.put_char(c);
	star = false;
      }
    }
  }
  else {
    return '/';
  }
}


//////////////////////////////////////////////////////////////////////
// 行番号ウォッチャー関係
//////////////////////////////////////////////////////////////////////

// @brief 行番号ウオッチャーを設定する．
// @param[in] watcher 登録対象のウオッチャー
// watcher がすでに登録されていればなにもしない．
void
RawLex::reg_watcher(VlLineWatcher* watcher)
{
  mWatcherMgr.reg_binder(watcher);
}

// @brief 行番号ウオッチャーの登録を解除する．
// @param[in] watcher 登録削除対象のウオッチャー
// watcher が登録されていなければなにもしない．
void
RawLex::unreg_watcher(VlLineWatcher* watcher)
{
  mWatcherMgr.unreg_binder(watcher);
}


//////////////////////////////////////////////////////////////////////
// プラグインの設定
//////////////////////////////////////////////////////////////////////

// @brief プラグインの追加
// @param[in] name プラグインを起動するコンパイラディレクティブ名
// @param[in] plugin 登録するプラグイン
void
RawLex::add_plugin(const char* name,
		   LexPlugin* plugin)
{
  mPluginDict->reg_plugin(plugin);
}

// @brief プラグインの削除
// @param[in] name 削除するプラグイン名
// @retval true 削除が成功した．
// @retval false name という名のプラグインが存在しなかった．
bool
RawLex::erase_plugin(const char* name)
{
  return mPluginDict->unreg_plugin(name);
}

// @brief プラグインを登録できるか調べる．
// @param[in] name プラグイン名
// @return true 登録可能
// @return false 同名の定義済みプラグインがあり，登録不可能
bool
RawLex::check_pluginname(const char* name)
{
  return mPluginDict->check_name(name);
}

// @brief 内部状態の追加
// @param[in] state 登録する内部状態
void
RawLex::add_state(LexState* state)
{
  mStates.push_back(unique_ptr<LexState>{state});
}


//////////////////////////////////////////////////////////////////////
// デバッグモードの制御
//////////////////////////////////////////////////////////////////////

// @brief デバッグモードのセット
// @param[in] flag true ならすべてのトークンの情報を標準エラーに出力する
void
RawLex::set_debug(bool flag)
{
  mDebug = flag;
}

// @brief デバッグモードの取得
bool
RawLex::debug()
{
  return mDebug;
}


//////////////////////////////////////////////////////////////////////
// 内部で用いられる関数
//////////////////////////////////////////////////////////////////////

// @brief `resetall の処理
void
RawLex::resetall(const FileRegion& file_region)
{
  for ( auto& state: mStates ) {
    state->resetall(file_region);
  }
}

// @brief マクロ定義の検査
// @param[in] name 名前
// @return name という名のマクロが定義されていたら true を返す．
bool
RawLex::is_macro_defined(const char* name) const
{
  LexPlugin* plugin = mPluginDict->find_plugin(name);
  if ( plugin == nullptr ) {
    return false;
  }
  return plugin->is_macro();
}

// @brief マクロの多重展開のチェック
// @param[in] name 名前
// @return マクロ展開が自己ループしていたら true を返す．
bool
RawLex::check_macro(const char* name) const
{
  for ( auto macro: mMacroStack ) {
    if ( macro.check_name(name) ) {
      return true;
    }
  }
  return false;
}

// @brief マクロの追加
void
RawLex::push_macro(const char* name,
		   const TokenInfo* top,
		   TokenList* param_array)
{
  mMacroStack.push_back(MacroSource());
  MacroSource& macro = mMacroStack.back();
  macro.set(name, top, param_array);
}

// @brief 改行文字を読み込んだときに呼ばれる関数
void
RawLex::check_line(ymuint line)
{
  mWatcherMgr.prop_event(line);
}

// @brief 一文字読み出す．
//
// 実際には peek(); acept() と等価
int
RawLex::get()
{
  return mInputMgr->get();
}

// @brief 次の文字を読み出す．
//
// ファイル位置の情報等は変わらない
int
RawLex::peek()
{
  return mInputMgr->peek();
}

// @brief 直前の peek() を確定させる．
void
RawLex::accept()
{
  mInputMgr->accept();
}

// @brief ファイルの末尾の時にtrue を返す．
bool
RawLex::is_eof() const
{
  return mInputMgr->is_eof();
}

// @brief 現在の位置を返す．
FileLoc
RawLex::cur_loc() const
{
  return mInputMgr->cur_loc();
}

END_NAMESPACE_YM_VERILOG
