﻿#ifndef YM_VL_VLATTRIBUTE_H
#define YM_VL_VLATTRIBUTE_H

/// @file ym/vl/VlAttribute.h
/// @brief VlAttribute のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
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
protected:

  /// @brief デストラクタ
  virtual
  ~VlAttribute() { }


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
  VlExpr*
  expr() const = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class VlAttrList VlAttribute.h "ym/vl/VlAttribute.h"
/// @brief VlAttribute のリスト
//////////////////////////////////////////////////////////////////////
class VlAttrList
{
protected:

  /// @brief デストラクタ
  virtual
  ~VlAttrList() { }


public:
  //////////////////////////////////////////////////////////////////////
  // VlAttrList の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素数を返す．
  virtual
  ymuint
  size() const = 0;

  /// @brief 内容を返す．
  /// @param[in] pos 位置番号 (0 <= pos < size() )
  virtual
  VlAttribute*
  elem(ymuint pos) const = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // YM_VL_VLATTRIBUTE_H
