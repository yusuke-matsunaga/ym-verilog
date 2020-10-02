
/// @file ElbError.cc
/// @brief ElbError の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2020 Yusuke Matsunaga
/// All rights reserved.


#include "ElbError.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス ElbError
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] file ファイル名
// @param[in] line 行番号
// @param[in] loc ファイル上の位置
// @param[in] label ラベル
// @param[in] msg メッセージ
ElbError::ElbError(const char* file,
		   int line,
		   const FileRegion& loc,
		   const string& label,
		   const string& msg) :
  mFile{file},
  mLine{line},
  mFileRegion{loc},
  mLabel{label},
  mMessage{msg}
{
}

// @brief デストラクタ
ElbError::~ElbError()
{
}

// @brief エラーを送出したソースファイル名を返す．
const char*
ElbError::file() const
{
  return mFile;
}

// @brief エラーを送出したソースファイル上の行番号を返す．
int
ElbError::line() const
{
  return mLine;
}

// @brief ファイル上の位置を返す．
const FileRegion&
ElbError::file_region() const
{
  return mFileRegion;
}

// @brief エラーラベルを返す．
string
ElbError::label() const
{
  return mLabel;
}

// @brief エラーメッセージを返す．
string
ElbError::message() const
{
  return mMessage;
}


//////////////////////////////////////////////////////////////////////
// クラス ElbConstError
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] file ファイル名
// @param[in] line 行番号
// @param[in] loc ファイル上の位置
// @param[in] label ラベル
// @param[in] msg メッセージ
ElbConstError::ElbConstError(const char* file,
			     int line,
			     const FileRegion& loc,
			     const string& label,
			     const string& msg) :
  ElbError(file, line, loc, label, msg)
{
}

// @brief デストラクタ
ElbConstError::~ElbConstError()
{
}


//////////////////////////////////////////////////////////////////////
// クラス ELbIntError
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] file ファイル名
// @param[in] line 行番号
// @param[in] loc ファイル上の位置
ElbIntError::ElbIntError(const char* file,
			 int line,
			 const FileRegion& loc) :
  ElbError(file, line, loc, "ELABXXX", "Integer value required.")
{
}

// @brief デストラクタ
ElbIntError::~ElbIntError()
{
}


//////////////////////////////////////////////////////////////////////
// クラス ElbBvError
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] file ファイル名
// @param[in] line 行番号
// @param[in] loc ファイル上の位置
ElbBvError::ElbBvError(const char* file,
		       int line,
		       const FileRegion& loc) :
  ElbError(file, line, loc, "ELABXXX", "Bit-Vector value required.")
{
}

// @brief デストラクタ
ElbBvError::~ElbBvError()
{
}


//////////////////////////////////////////////////////////////////////
// クラス ElbNameError
//////////////////////////////////////////////////////////////////////

BEGIN_NONAMESPACE

inline
string
make_message(const string& name,
	     const string& msg)
{
  ostringstream buf;
  buf << name << " : " << msg;
  return buf.str();
}

END_NONAMESPACE


// @brief コンストラクタ
// @param[in] file ファイル名
// @param[in] line 行番号
// @param[in] loc ファイル上の位置
// @param[in] label ラベル
// @param[in] name 名前
// @param[in] msg メッセージ
ElbNameError::ElbNameError(const char* file,
			   int line,
			   const FileRegion& loc,
			   const string& label,
			   const string& name,
			   const string& msg) :
  ElbError(file, line, loc, label, make_message(name, msg))
{
}

// @brief デストラクタ
ElbNameError::~ElbNameError()
{
}

END_NAMESPACE_YM_VERILOG
