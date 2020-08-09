
/// @file EiGenBlock.cc
/// @brief EiGenBlock の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "EiFactory.h"
#include "EiGenBlock.h"
#include "ym/pt/PtItem.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief generate block を生成する．
// @param[in] parent 親のスコープ環境
// @param[in] pt_item 対応するパース木の要素
const VlNamedObj*
EiFactory::new_GenBlock(const VlNamedObj* parent,
			const PtItem* pt_item)
{
  auto scope = new EiGenBlock(parent, pt_item);

  return scope;
}

// @brief generate for block を生成する．
// @param[in] parent 親のスコープ環境
// @param[in] pt_item 対応するパース木の要素
// @param[in] int gvi 対応する genvar の値
const VlNamedObj*
EiFactory::new_GfBlock(const VlNamedObj* parent,
		       const PtItem* pt_item,
		       int gvi)
{
  auto scope = new EiGfBlock(parent, pt_item, gvi);

  return scope;
}


//////////////////////////////////////////////////////////////////////
// クラス EiGenBlock
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のスコープ環境
// @param[in] pt_item 対応するパース木の要素
EiGenBlock::EiGenBlock(const VlNamedObj* parent,
		       const PtItem* pt_item) :
  mParent{parent},
  mPtItem{pt_item}
{
}

// @brief デストラクタ
EiGenBlock::~EiGenBlock()
{
}

// @brief 型の取得
VpiObjType
EiGenBlock::type() const
{
  return VpiObjType::Scope;
}

// @brief ファイル位置の取得
FileRegion
EiGenBlock::file_region() const
{
  return mPtItem->file_region();
}

// @brief このオブジェクトの属しているスコープを返す．
const VlNamedObj*
EiGenBlock::parent() const
{
  return mParent;
}

// @brief 名前の取得
const char*
EiGenBlock::name() const
{
  return mPtItem->name();
}


//////////////////////////////////////////////////////////////////////
// クラス EiGfBlock
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のスコープ環境
// @param[in] pt_item 対応するパース木の要素
// @param[in] index 対応する genvar の値
EiGfBlock::EiGfBlock(const VlNamedObj* parent,
		     const PtItem* pt_item,
		     int index) :
  EiGenBlock(parent, pt_item)
{
  ostringstream buf;
  buf << pt_item->name() << "[" << index << "]";
#warning "TODO 2020-08-08: ここのメモリアロケーションどうする．"

  mName = buf.str();
}

// @brief デストラクタ
EiGfBlock::~EiGfBlock()
{
}

// @brief 名前の取得
const char*
EiGfBlock::name() const
{
  return mName.c_str();
}

END_NAMESPACE_YM_VERILOG
