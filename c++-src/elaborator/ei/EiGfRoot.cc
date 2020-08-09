
/// @file EiGfRoot.cc
/// @brief EiGfRoot の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.


#include "EiFactory.h"
#include "EiGfRoot.h"
#include "ym/pt/PtItem.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief GfBlock 検索用の親の名前付きオブジェクトを作る．
ElbGfRoot*
EiFactory::new_GfRoot(const VlNamedObj* parent,
		      const PtItem* pt_item)
{
  auto gfroot = new EiGfRoot(parent, pt_item);

  return gfroot;
}


//////////////////////////////////////////////////////////////////////
// クラス EiGfRoot
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のスコープ環境
// @param[in] pt_item 対応するパース木の要素
EiGfRoot::EiGfRoot(const VlNamedObj* parent,
		   const PtItem* pt_item) :
  mParent{parent},
  mPtItem{pt_item}
{
}

// @brief デストラクタ
EiGfRoot::~EiGfRoot()
{
}

// @brief 型の取得
VpiObjType
EiGfRoot::type() const
{
  return VpiObjType::Scope;
}

// @brief ファイル位置の取得
FileRegion
EiGfRoot::file_region() const
{
  return mPtItem->file_region();
}

// @brief このオブジェクトの属しているスコープを返す．
const VlNamedObj*
EiGfRoot::parent() const
{
  return mParent;
}

// @brief 名前の取得
const char*
EiGfRoot::name() const
{
  return mPtItem->name();
}

// @brief 子供のスコープを追加する．
void
EiGfRoot::add(SizeType index,
	      const VlNamedObj* block)
{
  mTable[index] = block;
}


// @brief 子供のスコープを取り出す．
const VlNamedObj*
EiGfRoot::elem_by_index(SizeType index)
{
  if ( mTable.count(index) ) {
    return mTable.at(index);
  }
  else {
    return nullptr;
  }
}

END_NAMESPACE_YM_VERILOG
