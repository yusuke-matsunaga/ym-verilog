#ifndef INPUTFILE_H
#define INPUTFILE_H

/// @file InputFile.h
/// @brief InputFile のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2013-2014, 2019 Yusuke Matsunaga
/// All rights reserved.


#include "ym/verilog.h"

#include "ym/Scanner.h"
#include "ym/FileRegion.h"
#include "ym/FileInfo.h"
#include "ym/StrBuff.h"

#include "scanner/RawLex.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class InputFile InputFile.h "InputFile.h"
/// @ingroup VlParser
/// @brief 入力ファイルを表すクラス
///
/// このクラスはファイルを一文字単位で読み出す．\n
/// ただし, 改行コードの処理系依存部分を吸収して常に '\\n' だけを
/// 返すようにしている．
/// @sa FileInfo RawLex
//////////////////////////////////////////////////////////////////////
class InputFile :
  public Scanner
{
public:

  /// @brief コンストラクタ
  /// @param[in] lex 親の Lex
  /// @param[in] filename ファイル名
  /// @param[in] parent_loc インクルードされた場合の親のファイル位置
  InputFile(RawLex& lex,
	    const string& filename,
	    const FileLoc& parent_file);

  /// @brief デストラクタ
  ~InputFile();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 正常な状態の時 true を返す．
  bool
  is_valid() const;

  /// @brief インクルード元のファイル位置を返す．
  FileLoc
  parent_loc();

  /// @brief ファイルの末尾の時 true を返す．
  bool
  is_eof();


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief read_token() の下請け関数
  /// @param[out] buff 結果の文字列を格納するバッファ
  int
  _read_token(StrBuff& buff);


protected:
  //////////////////////////////////////////////////////////////////////
  // Scanner の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 改行を読み込んだ時に起動する関数
  /// @param[in] line 行番号
  void
  check_line(int line) override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力のファイルストリーム
  ifstream mIn;

  // 親の Lex
  RawLex& mLex;

  // 状態
  bool mValid{false};

};

//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 正常な状態の時 true を返す．
inline
bool
InputFile::is_valid() const
{
  return mValid;
}

// @brief トークンの読み出しを行う．
// @param[out] buff 結果の文字列を格納するバッファ
// @param[out] token_loc トークンの位置情報
inline
int
InputFile::read_token(StrBuff& buff,
		      FileRegion& token_loc)
{
  int id = _read_token(buff);
  token_loc = cur_loc();
  return id;
}

// @brief インクルード元のファイル位置を返す．
inline
FileLoc
InputFile::parent_loc()
{
  return FileLoc{file_info(), cur_loc().end_line(), 1};
}

// @brief ファイルの末尾の時 true を返す．
inline
bool
InputFile::is_eof()
{
  int c = peek();
  return c == EOF;
}

END_NAMESPACE_YM_VERILOG

#endif // INPUTFILE_H
