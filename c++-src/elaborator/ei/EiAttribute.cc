
/// @file EiAttribute.cc
/// @brief EiAttribute の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.


#include "ei/EiFactory.h"
#include "ei/EiAttribute.h"
#include "elaborator/ElbExpr.h"

#include "ym/pt/PtMisc.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief attribute instance のリストを生成する．
// @param[in] pt_attr パース木の定義要素
// @param[in] expr 値
// @param[in] def 定義側の属性の時 true
const VlAttribute*
EiFactory::new_Attribute(const PtAttrInst* pt_attr,
			 bool def)
{
  //auto attr{new EiAttribute(pt_attr, def)};
  //return attr;
  return nullptr;
}


//////////////////////////////////////////////////////////////////////
// attribute instance を表すクラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_attr パース木の定義要素
// @param[in] expr 値
// @param[in] def 定義側の属性の時 true
EiAttribute::EiAttribute(const PtAttrSpec* pt_attr,
			 const VlExpr* expr,
			 bool def) :
  mPtAttrSpec{pt_attr},
  mExpr{expr},
  mDef{def}
{
}

// @brief デストラクタ
EiAttribute::~EiAttribute()
{
}

// @brief 型の取得
VpiObjType
EiAttribute::type() const
{
  return VpiObjType::Attribute;
}

// @brief ファイル位置を返す．
FileRegion
EiAttribute::file_region() const
{
  return mPtAttrSpec->file_region();
}

// @brief 属性名を返す．
const char*
EiAttribute::name() const
{
  return mPtAttrSpec->name();
}

// @brief def_attribute なら true を返す．
bool
EiAttribute::def_attribute() const
{
  return mDef;
}

// @brief 値を表す式を返す．
const VlExpr*
EiAttribute::expr() const
{
  return mExpr;
}

END_NAMESPACE_YM_VERILOG
