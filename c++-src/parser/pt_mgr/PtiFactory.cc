
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

// @brief module instance/UDP/gate instance の要素の生成
const PtInst*
PtiFactory::new_Inst(
  const FileRegion& fr,
  const PtExpr* expr1
)
{
  return new_Inst(fr, {new_OrderedCon(expr1)});
}

// @brief module instance/UDP/gate instance の要素の生成
const PtInst*
PtiFactory::new_Inst(
  const FileRegion& fr,
  const PtExpr* expr1,
  const PtExpr* expr2
)
{
  return new_Inst(fr, {new_OrderedCon(expr1), new_OrderedCon(expr2)});
}

// @brief module instance/UDP/gate instance の要素の生成
const PtInst*
PtiFactory::new_Inst(
  const FileRegion& fr,
  const PtExpr* expr1,
  const PtExpr* expr2,
  const PtExpr* expr3
)
{
  return new_Inst(fr, {new_OrderedCon(expr1),
		       new_OrderedCon(expr2),
		       new_OrderedCon(expr3)});
}

// @brief module instance/UDP/gate instance の要素の生成
const PtInst*
PtiFactory::new_Inst(
  const FileRegion& fr,
  const PtExpr* expr1,
  const PtExpr* expr2,
  const PtExpr* expr3,
  const PtExpr* expr4
)
{
  return new_Inst(fr, {new_OrderedCon(expr1),
		       new_OrderedCon(expr2),
		       new_OrderedCon(expr3),
		       new_OrderedCon(expr4)});
}

// @brief 名前付き module instance/UDP/gate instance の要素の生成
const PtInst*
PtiFactory::new_InstN(
  const FileRegion& fr,
  const char* name,
  const PtExpr* expr1
)
{
  return new_InstN(fr, name, {new_OrderedCon(expr1)});

}

// @brief 名前付き module instance/UDP/gate instance の要素の生成
const PtInst*
PtiFactory::new_InstN(
  const FileRegion& fr,
  const char* name,
  const PtExpr* expr1,
  const PtExpr* expr2
)
{
  return new_InstN(fr, name, {new_OrderedCon(expr1),
			      new_OrderedCon(expr2)});
}

// @brief 名前付き module instance/UDP/gate instance の要素の生成
const PtInst*
PtiFactory::new_InstN(
  const FileRegion& fr,
  const char* name,
  const PtExpr* expr1,
  const PtExpr* expr2,
  const PtExpr* expr3
)
{
  return new_InstN(fr, name, {new_OrderedCon(expr1),
			      new_OrderedCon(expr2),
			      new_OrderedCon(expr3)});
}

// @brief 名前付き module instance/UDP/gate instance の要素の生成
const PtInst*
PtiFactory::new_InstN(
  const FileRegion& fr,
  const char* name,
  const PtExpr* expr1,
  const PtExpr* expr2,
  const PtExpr* expr3,
  const PtExpr* expr4
)
{
  return new_InstN(fr, name, {new_OrderedCon(expr1),
			      new_OrderedCon(expr2),
			      new_OrderedCon(expr3),
			      new_OrderedCon(expr4)});
}

// @brief 名前と範囲付き module instance/UDP/gate instance の要素の生成
const PtInst*
PtiFactory::new_InstV(
  const FileRegion& fr,
  const char* name,
  const PtExpr* left,
  const PtExpr* right,
  const PtExpr* expr1
)
{
  return new_InstV(fr, name, left, right,
		   {new_OrderedCon(expr1)});
}

// @brief 名前と範囲付き module instance/UDP/gate instance の要素の生成
const PtInst*
PtiFactory::new_InstV(
  const FileRegion& fr,
  const char* name,
  const PtExpr* left,
  const PtExpr* right,
  const PtExpr* expr1,
  const PtExpr* expr2
)
{
  return new_InstV(fr, name, left, right,
		   {new_OrderedCon(expr1),
		    new_OrderedCon(expr2)});
}

// @brief 名前と範囲付き module instance/UDP/gate instance の要素の生成
const PtInst*
PtiFactory::new_InstV(
  const FileRegion& fr,
  const char* name,
  const PtExpr* left,
  const PtExpr* right,
  const PtExpr* expr1,
  const PtExpr* expr2,
  const PtExpr* expr3
)
{
  return new_InstV(fr, name, left, right,
		   {new_OrderedCon(expr1),
		    new_OrderedCon(expr2),
		    new_OrderedCon(expr3)});

}

// @brief 名前と範囲付き module instance/UDP/gate instance の要素の生成
const PtInst*
PtiFactory::new_InstV(
  const FileRegion& fr,
  const char* name,
  const PtExpr* left,
  const PtExpr* right,
  const PtExpr* expr1,
  const PtExpr* expr2,
  const PtExpr* expr3,
  const PtExpr* expr4
)
{
  return new_InstV(fr, name, left, right,
		   {new_OrderedCon(expr1),
		    new_OrderedCon(expr2),
		    new_OrderedCon(expr3),
		    new_OrderedCon(expr4)});
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
