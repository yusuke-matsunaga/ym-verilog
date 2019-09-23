﻿
/// @file EiAttribute.cc
/// @brief EiAttribute の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "EiFactory.h"
#include "EiAttribute.h"
#include "elb/ElbExpr.h"

#include "ym/pt/PtMisc.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief attribute instance のリストを生成する．
// @param[in] n 要素数
ElbAttrList*
EiFactory::new_AttrList(SizeType n)
{
  void* q = mAlloc.get_memory(sizeof(EiAttribute) * n);
  EiAttribute* array = new (q) EiAttribute[n];

  void* p = mAlloc.get_memory(sizeof(EiAttrList));
  EiAttrList* attr_list = new (p) EiAttrList(n, array);

  return attr_list;
}


//////////////////////////////////////////////////////////////////////
// attribute instance を表すクラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiAttribute::EiAttribute()
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
VlExpr*
EiAttribute::expr() const
{
  return mExpr;
}


//////////////////////////////////////////////////////////////////////
// attribute instance のリストを表すクラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiAttrList::EiAttrList(SizeType n,
		       EiAttribute* array) :
  mSize(n),
  mArray(array)
{
}

// @brief デストラクタ
EiAttrList::~EiAttrList()
{
}

// @brief 要素数を返す．
SizeType
EiAttrList::size() const
{
  return mSize;
}

// @brief 内容を返す．
// @param[in] pos 位置番号 (0 <= pos < size() )
VlAttribute*
EiAttrList::elem(SizeType pos) const
{
  return &mArray[pos];
}

// @brief 要素を設定する．
// @param[in] pos 位置番号
// @param[in] pt_as パース木の定義要素
// @param[in] expr 値を表す式
// @param[in] def 定義側の属性のとき true とするフラグ
void
EiAttrList::set(SizeType pos,
		const PtAttrSpec* pt_as,
		ElbExpr* expr,
		bool def)
{
  EiAttribute& attr = mArray[pos];
  attr.mPtAttrSpec = pt_as;
  attr.mExpr = expr;
  attr.mDef = def;
}

END_NAMESPACE_YM_VERILOG
