#ifndef EIUSERSYSTF_H
#define EIUSERSYSTF_H

/// @file EiUserSystf.h
/// @brief EiUserSystf のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ElbUserSystf.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiUserSystf EiUserSystf.h "EiUserSystf.h"
/// @brief システムタスク/関数の定義を表すクラス
/// IEEE Std 1364-2001 26.6.14 UDP
//////////////////////////////////////////////////////////////////////
class EiUserSystf :
  public ElbUserSystf
{
public:

  /// @brief コンストラクタ
  EiUserSystf();

  /// @brief デストラクタ
  ~EiUserSystf();


public:
  //////////////////////////////////////////////////////////////////////
  // VlUserSystf に固有の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を返す．
  string
  name() const override;

  /// @brief system task の時 true を返す．
  bool
  system_task() const override;

  /// @brief system function の時 true を返す．
  bool
  system_function() const override;

  /// @brief system function の型を返す．
  tVpiFuncType
  function_type() const override;

  /// @brief compile 時のコールバック関数
  int
  on_compile() override;

  /// @brief 実行時のコールバック関数
  int
  on_call() override;

  /// @brief SizedFunc の場合にサイズを返す．
  int
  size() const override;

};


//////////////////////////////////////////////////////////////////////
/// @class EiUserSystf1 EiUserSystf.h "EiUserSystf.h"
/// @brief システムタスク/関数の定義を表すクラス
/// IEEE Std 1364-2001 26.6.14 UDP
//////////////////////////////////////////////////////////////////////
class EiUserSystf1 :
  public ElbUserSystf
{
public:

  /// @brief コンストラクタ
  /// @param[in]
  EiUserSystf1();

  /// @brief デストラクタ
  ~EiUserSystf1();


public:
  //////////////////////////////////////////////////////////////////////
  // VlUserSystf に固有の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を返す．
  string
  name() const override;

  /// @brief system task の時 true を返す．
  bool
  system_task() const override;

  /// @brief system function の時 true を返す．
  bool
  system_function() const override;

  /// @brief system function の型を返す．
  tVpiFuncType
  function_type() const override;

  /// @brief compile 時のコールバック関数
  int
  on_compile() override;

  /// @brief 実行時のコールバック関数
  int
  on_call() override;

  /// @brief SizedFunc の場合にサイズを返す．
  int
  size() const override;

};

END_NAMESPACE_YM_VERILOG

#endif // EIUSERSYSTF_H
