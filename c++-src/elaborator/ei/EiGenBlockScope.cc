
/// @file EiGenBlockScope.cc
/// @brief EiGenBlockScope の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2020 Yusuke Matsunaga
/// All rights reserved.

#include "ei/EiFactory.h"
#include "ei/EiGenBlockScope.h"
#include "ym/pt/PtItem.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief generate block を生成する．
const VlScope*
EiFactory::new_GenBlock(
  const VlScope* parent,
  const PtItem* pt_item
)
{
  auto scope = new EiGenBlockScope{parent, pt_item};
  return scope;
}

// @brief generate for block を生成する．
const VlScope*
EiFactory::new_GfBlock(
  const VlScope* parent,
  const PtItem* pt_item,
  int gvi
)
{
  auto scope = new EiGfBlockScope{parent, pt_item, gvi};
  return scope;
}


//////////////////////////////////////////////////////////////////////
// クラス EiGenBlockScope
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiGenBlockScope::EiGenBlockScope(
  const VlScope* parent,
  const PtItem* pt_item
) : EiScope{parent},
    mPtItem{pt_item}
{
}

// @brief デストラクタ
EiGenBlockScope::~EiGenBlockScope()
{
}

// @brief ファイル位置の取得
FileRegion
EiGenBlockScope::file_region() const
{
  return mPtItem->file_region();
}

// @brief 名前の取得
string
EiGenBlockScope::name() const
{
  return mPtItem->name();
}


//////////////////////////////////////////////////////////////////////
// クラス EiGfBlockScope
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiGfBlockScope::EiGfBlockScope(
  const VlScope* parent,
  const PtItem* pt_item,
  int index
) : EiGenBlockScope{parent, pt_item},
    mIndex{index}
{
}

// @brief デストラクタ
EiGfBlockScope::~EiGfBlockScope()
{
}

// @brief 名前の取得
string
EiGfBlockScope::name() const
{
  ostringstream buf;
  buf << EiGenBlockScope::name() << "[" << mIndex << "]";
  return buf.str();
}

END_NAMESPACE_YM_VERILOG
