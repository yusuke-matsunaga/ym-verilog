
/// @file InputFile.cc
/// @brief InputFile の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "InputFile.h"
#include <fcntl.h>

#include "ym/MsgMgr.h"

#include "common.h"


// ファイル末尾に改行がなくても warning としない時に 1
#define ALLOW_EOF_WITHOUT_NL 1


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// 入力ファイルを表すためのクラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] lex 親の Lex
// @param[in] filename ファイル名
// @param[in] parent_loc インクルードされた場合の親のファイル位置
InputFile::InputFile(RawLex& lex,
		     const string& filename,
		     const FileLoc& parent_file) :
  Scanner(mIn, FileInfo{filename, parent_file}),
  mIn(filename),
  mLex(lex)
{
  mValid = mIn.operator bool();
}

// @brief デストラクタ
InputFile::~InputFile()
{
}

// @brief 改行を読み込んだ時に起動する関数
// @param[in] line 行番号
void
InputFile::check_line(int line)
{
  mLex.check_line(line);
}

END_NAMESPACE_YM_VERILOG
