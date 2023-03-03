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
  ElbError(
    const char* file,      ///< [in] ファイル名
    int line,              ///< [in] 行番号
    const FileRegion& loc, ///< [in] ファイル上の位置
    const string& label,   ///< [in] ラベル
    const string& msg      ///< [in] メッセージ
  );

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
  ElbConstError(
    const char* file,      ///< [in] ファイル名
    int line,              ///< [in] 行番号
    const FileRegion& loc, ///< [in] ファイル上の位置
    const string& label,   ///< [in] ラベル
    const string& msg      ///< [in] メッセージ
  );

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
  ElbIntError(
    const char* file,     ///< [in] ファイル名
    int line,             ///< [in] 行番号
    const FileRegion& loc ///< [in] ファイル上の位置
  );

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
  ElbBvError(
    const char* file,     ///< [in] ファイル名
    int line,             ///< [in] 行番号
    const FileRegion& loc ///< [in] ファイル上の位置
  );

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
  ElbNameError(
    const char* file,      ///< [in] ファイル名
    int line,              ///< [in] 行番号
    const FileRegion& loc, ///< [in] ファイル上の位置
    const string& label,   ///< [in] ラベル
    const string& name,    ///< [in] 名前
    const string& msg      ///< [in] メッセージ
  );

  /// @brief デストラクタ
  ~ElbNameError();

};

END_NAMESPACE_YM_VERILOG

#endif // ELBERROR_H
