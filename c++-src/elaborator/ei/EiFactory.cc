﻿
/// @file EiFactory.cc
/// @brief EiFactory の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "EiFactory.h"
#include "EiExpr.h"

#include "ym/pt/PtUdp.h"
#include "ym/pt/PtDecl.h"


BEGIN_NAMESPACE_YM_VERILOG

// @brief オブジェクトを生成する関数
ElbFactory*
ElbFactory::new_obj(Alloc& alloc)
{
  return new EiFactory(alloc);
}

//////////////////////////////////////////////////////////////////////
// クラス EiFactory
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] alloc メモリ確保用のアロケータ
EiFactory::EiFactory(Alloc& alloc) :
  mAlloc(alloc)
{
}

// @brief デストラクタ
EiFactory::~EiFactory()
{
}

// @brief 内訳を表示する．
void
EiFactory::dump_prof(ostream& s)
{
}

// @brief ステートメントの配列を生成する．
// @param[in] stmt_num 要素数
ElbStmt**
EiFactory::new_StmtList(SizeType stmt_num)
{
  void* q = mAlloc.get_memory(sizeof(ElbStmt*) * stmt_num);
  ElbStmt** array = new (q) ElbStmt*[stmt_num];

  return array;
}

// @brief 式の配列を生成する．
// @param[in] elem_num 要素数
ElbExpr**
EiFactory::new_ExprList(SizeType elem_num)
{
  void* p = mAlloc.get_memory(sizeof(ElbExpr*) * elem_num);
  ElbExpr** expr_array = new (p) ElbExpr*[elem_num];

  return expr_array;
}

END_NAMESPACE_YM_VERILOG
