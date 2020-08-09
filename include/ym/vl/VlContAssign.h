﻿#ifndef YM_VL_VLCONTASSIGN_H
#define YM_VL_VLCONTASSIGN_H

/// @file ym/vl/VlContAssign.h
/// @brief VlContAssign のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym/vl/VlObj.h"
#include "ym/vl/VlFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class VlContAssign VlContAssign.h "ym/vl/VlContAssign.h"
/// @brief 継続的代入文を表すクラス
/// IEEE Std 1364-2001 26.6.24 Continuous assignment
//////////////////////////////////////////////////////////////////////
class VlContAssign :
  public VlObj
{
public:
  //////////////////////////////////////////////////////////////////////
  // VlContAssign の派生クラスに共通な仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 親のモジュールを返す．
  virtual
  const VlModule*
  module() const = 0;

  /// @brief 0の強さを返す．
  virtual
  VpiStrength
  drive0() const = 0;

  /// @brief 1の強さを返す．
  virtual
  VpiStrength
  drive1() const = 0;

  /// @brief 遅延を表す式を返す．
  virtual
  const VlDelay*
  delay() const = 0;

  /// @brief ネット宣言中の assignment の時に true を返す．
  virtual
  bool
  has_net_decl_assign() const = 0;

  /// @brief ビット幅を返す．
  virtual
  int
  bit_size() const = 0;

  /// @brief 左辺を返す．
  virtual
  const VlExpr*
  lhs() const = 0;

  /// @brief 右辺を返す．
  virtual
  const VlExpr*
  rhs() const = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // YM_VL_VLCONTASSIGN_H
