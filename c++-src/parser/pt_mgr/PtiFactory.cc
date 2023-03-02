
/// @file PtiFactory.cc
/// @brief PtiFactory の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.

#include "parser/PtiFactory.h"
#include "parser/CptFactory.h"
#include "parser/SptFactory.h"
#include "parser/PuHierName.h"
#include "alloc/Alloc.h"


BEGIN_NAMESPACE_YM_VERILOG

// @brief PtiFactory の実装クラスのオブジェクトを作る関数
PtiFactory*
PtiFactory::make_obj(
  const string& type,
  Alloc& alloc
)
{
  if ( type == "spt" ) {
    return new SptFactory(alloc);
  }
  return new CptFactory(alloc);
}

// @brief コンストラクタ
PtiFactory::PtiFactory(
  Alloc& alloc
) : mAlloc{alloc}
{
}

// @brief デストラクタ
PtiFactory::~PtiFactory()
{
}

// @brief 順序つき結合子のベクタの生成
vector<const PtConnection*>
PtiFactory::new_OrderedConVector(
  const PtExpr* expr
)
{
  return vector<const PtConnection*>{new_OrderedCon(expr)};
}

// @brief 順序つき結合子のベクタの生成
vector<const PtConnection*>
PtiFactory::new_OrderedConVector(
  const PtExpr* expr1,
  const PtExpr* expr2
)
{
  return vector<const PtConnection*>{new_OrderedCon(expr1),
				     new_OrderedCon(expr2)};
}

// @brief 順序つき結合子のベクタの生成
vector<const PtConnection*>
PtiFactory::new_OrderedConVector(
  const PtExpr* expr1,
  const PtExpr* expr2,
  const PtExpr* expr3
)
{
  return vector<const PtConnection*>{new_OrderedCon(expr1),
				     new_OrderedCon(expr2),
				     new_OrderedCon(expr3)};
}

// @brief 順序つき結合子のベクタの生成
vector<const PtConnection*>
PtiFactory::new_OrderedConVector(
  const PtExpr* expr1,
  const PtExpr* expr2,
  const PtExpr* expr3,
  const PtExpr* expr4
)
{
  return vector<const PtConnection*>{new_OrderedCon(expr1),
				     new_OrderedCon(expr2),
				     new_OrderedCon(expr3),
				     new_OrderedCon(expr4)};
}

// @brief 階層名の生成
PuHierName*
PtiFactory::new_HierName(
  const PtNameBranch* nb,
  const char* name
)
{
  void* p{mAlloc.get_memory(sizeof(PuHierName))};
  auto hname = new (p) PuHierName(nb, name);
  return hname;
}

END_NAMESPACE_YM_VERILOG
