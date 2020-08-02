#ifndef ELBCONTASSIGN_H
#define ELBCONTASSIGN_H

/// @file ElbContAssign.h
/// @brief ElbContAssign のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym/vl/VlContAssign.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ElbCaHead ElbContAssign.h "ElbContAssign.h"
/// @brief 継続的代入文のヘッダ
//////////////////////////////////////////////////////////////////////
class ElbCaHead
{
protected:

  /// @brief コンストラクタ
  ElbCaHead() = default;

  /// @brief デストラクタ
  virtual
  ~ElbCaHead() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbCaHead の仮想関数
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

};

END_NAMESPACE_YM_VERILOG

#endif // ELBCONTASSIGN_H
