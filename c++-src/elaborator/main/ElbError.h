#ifndef ELBERROR_H
#define ELBERROR_H

/// @file ElbError.h
/// @brief ElbError のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2020 Yusuke Matsunaga
/// All rights reserved.

#include "ym/verilog.h"
#include "ym/FileRegion.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ElbError ElbError.h "ElbError.h"
/// @brief elaboration 時のエラーを表す例外クラス
//////////////////////////////////////////////////////////////////////
class ElbError
{
public:

  /// @brief コンストラクタ
  /// @param[in] file ファイル名
  /// @param[in] line 行番号
  /// @param[in] loc ファイル上の位置
  /// @param[in] label ラベル
  /// @param[in] msg メッセージ
  ElbError(const char* file,
	   int line,
	   const FileRegion& loc,
	   const string& label,
	   const string& msg);

  /// @brief デストラクタ
  ~ElbError();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief エラーを送出したソースファイル名を返す．
  const char*
  file() const;

  /// @brief エラーを送出したソースファイル上の行番号を返す．
  int
  line() const;

  /// @brief ファイル上の位置を返す．
  const FileRegion&
  file_region() const;

  /// @brief エラーラベルを返す．
  string
  label() const;

  /// @brief エラーメッセージを返す．
  string
  message() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル名
  const char* mFile;

  // 行番号
  int mLine;

  // ファイル上の位置
  FileRegion mFileRegion;

  // ラベル
  string mLabel;

  // メッセージ
  string mMessage;

};


//////////////////////////////////////////////////////////////////////
/// @class ElbConstError ElbError.h "ElbError.h"
/// @brief 定数式が要求される時に定数式でなかった場合のエラー
///
/// 中身は ElbError と同一．catch () で区別するために存在している．
//////////////////////////////////////////////////////////////////////
class ElbConstError :
  public ElbError
{
public:

  /// @brief コンストラクタ
  /// @param[in] file ファイル名
  /// @param[in] line 行番号
  /// @param[in] loc ファイル上の位置
  /// @param[in] label ラベル
  /// @param[in] msg メッセージ
  ElbConstError(const char* file,
		int line,
		const FileRegion& loc,
		const string& label,
		const string& msg);

  /// @brief デストラクタ
  ~ElbConstError();

};


//////////////////////////////////////////////////////////////////////
/// @class ElbIntError ElbError.h "ElbError.h"
/// @brief int型が要求される時にintでなかった場合のエラー
//////////////////////////////////////////////////////////////////////
class ElbIntError :
  public ElbError
{
public:

  /// @brief コンストラクタ
  /// @param[in] file ファイル名
  /// @param[in] line 行番号
  /// @param[in] loc ファイル上の位置
  ElbIntError(const char* file,
	      int line,
	      const FileRegion& loc);

  /// @brief デストラクタ
  ~ElbIntError();

};


//////////////////////////////////////////////////////////////////////
/// @class ElbBvError ElbError.h "ElbError.h"
/// @brief int型が要求される時にintでなかった場合のエラー
//////////////////////////////////////////////////////////////////////
class ElbBvError :
  public ElbError
{
public:

  /// @brief コンストラクタ
  /// @param[in] file ファイル名
  /// @param[in] line 行番号
  /// @param[in] loc ファイル上の位置
  ElbBvError(const char* file,
	     int line,
	     const FileRegion& loc);

  /// @brief デストラクタ
  ~ElbBvError();

};


//////////////////////////////////////////////////////////////////////
/// @class ElbNameError ElbError.h "ElbError.h"
/// @brief 識別子に関連したエラー
//////////////////////////////////////////////////////////////////////
class ElbNameError :
  public ElbError
{
public:

  /// @brief コンストラクタ
  /// @param[in] file ファイル名
  /// @param[in] line 行番号
  /// @param[in] loc ファイル上の位置
  /// @param[in] label ラベル
  /// @param[in] name 名前
  /// @param[in] msg メッセージ
  ElbNameError(const char* file,
	       int line,
	       const FileRegion& loc,
	       const string& label,
	       const string& name,
	       const string& msg);

  /// @brief デストラクタ
  ~ElbNameError();

};

END_NAMESPACE_YM_VERILOG

#endif // ELBERROR_H
