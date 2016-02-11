﻿#ifndef YM_VL_VLNAMEDOBJ_H
#define YM_VL_VLNAMEDOBJ_H

/// @file ym/vl/VlNamedObj.h
/// @brief VlNamedObj のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym/vl/VlObj.h"
#include "ym/vl/VlFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class VlNamedObj VlNamedObj.h "ym/vl/VlNamedObj.h"
/// @brief エラボレーション中の名前付きオブジェクトを表す基底クラス
//////////////////////////////////////////////////////////////////////
class VlNamedObj :
  public VlObj
{
protected:

  /// @brief デストラクタ
  virtual
  ~VlNamedObj() { }


public:
  //////////////////////////////////////////////////////////////////////
  // VlNamedObj の派生クラスに共通な仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief このオブジェクトの属している scope を返す．
  virtual
  const VlNamedObj*
  parent() const = 0;

  /// @brief このオブジェクトの属している module を返す．
  /// @note ただし module の場合には自分自身を返す．
  virtual
  const VlModule*
  parent_module() const;

  /// @brief 名前の取得
  virtual
  const char*
  name() const = 0;

  /// @brief fullname の取得
  virtual
  string
  full_name() const;

};

END_NAMESPACE_YM_VERILOG

#endif // YM_VL_VLNAMEDOBJ_H
