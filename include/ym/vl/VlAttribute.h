#ifndef YM_VL_VLATTRIBUTE_H
#define YM_VL_VLATTRIBUTE_H

/// @file ym/vl/VlAttribute.h
/// @brief VlAttribute のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/VlObj.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class VlAttribute VlAttribute.h "ym/vl/VlAttribute.h"
/// @brief attribute instance を表すクラス
//////////////////////////////////////////////////////////////////////
class VlAttribute :
  public VlObj
{
public:
  //////////////////////////////////////////////////////////////////////
  // VlAttribute の派生クラスに共通な仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 属性名を返す．
  virtual
  const char*
  name() const = 0;

  /// @brief def_attribute なら true を返す．
  virtual
  bool
  def_attribute() const = 0;

  /// @brief 値を表す式を返す．
  virtual
  const VlExpr*
  expr() const = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // YM_VL_VLATTRIBUTE_H
