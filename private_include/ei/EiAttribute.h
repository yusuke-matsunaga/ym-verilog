#ifndef EIATTRIBUTE_H
#define EIATTRIBUTE_H

/// @file EiAttribute.h
/// @brief EiAttribute のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/VlAttribute.h"
#include "ym/pt/PtP.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiAttribute EiAttribute.h "EiAttribute.h"
/// @brief genvar を表すクラス
//////////////////////////////////////////////////////////////////////
class EiAttribute :
  public VlAttribute
{
public:

  /// @brief コンストラクタ
  EiAttribute(
    const PtAttrSpec* pt_attr, ///< [in] パース木の定義要素
    const VlExpr* expr,        ///< [in] 値
    bool def                   ///< [in] 定義側の属性の時 true
  );

  /// @brief デストラクタ
  ~EiAttribute();


public:
  //////////////////////////////////////////////////////////////////////
  // ElbObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  VpiObjType
  type() const override;

  /// @brief ファイル位置を返す．
  FileRegion
  file_region() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // VlAttribute の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 属性名を返す．
  const char*
  name() const override;

  /// @brief def_attribute なら true を返す．
  bool
  def_attribute() const override;

  /// @brief 値を表す式を返す．
  const VlExpr*
  expr() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // パース木の定義要素
  const PtAttrSpec* mPtAttrSpec;

  // 値
  const VlExpr* mExpr;

  // 定義側の属性の時 true をなるフラグ
  bool mDef;

};

END_NAMESPACE_YM_VERILOG

#endif // EIATTRIBUTE_H
