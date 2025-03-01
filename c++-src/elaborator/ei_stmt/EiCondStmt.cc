﻿
/// @file EiCondStmt.cc
/// @brief EiCondStmt の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.

#include "ei/EiFactory.h"
#include "ei/EiCondStmt.h"

#include "elaborator/ElbExpr.h"

#include "ym/BitVector.h"
#include "ym/pt/PtStmt.h"



BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief while 文を生成する．
const VlStmt*
EiFactory::new_WhileStmt(
  const VlScope* parent,
  const VlProcess* process,
  const PtStmt* pt_stmt,
  const VlExpr* cond,
  const VlStmt* stmt
)
{
  auto stmt1 = new EiWhileStmt{parent, process, pt_stmt,
			       cond, stmt};
  return stmt1;
}

// @brief repeat 文を生成する．
const VlStmt*
EiFactory::new_RepeatStmt(
  const VlScope* parent,
  const VlProcess* process,
  const PtStmt* pt_stmt,
  const VlExpr* cond,
  const VlStmt* stmt
)
{
  auto stmt1 = new EiRepeatStmt{parent, process, pt_stmt,
				cond, stmt};
  return stmt1;
}

// @brief wait 文を生成する．
const VlStmt*
EiFactory::new_WaitStmt(
  const VlScope* parent,
  const VlProcess* process,
  const PtStmt* pt_stmt,
  const VlExpr* cond,
  const VlStmt* stmt
)
{
  auto stmt1 = new EiWaitStmt{parent, process, pt_stmt,
			      cond, stmt};
  return stmt1;
}

// @brief for 文を生成する．
const VlStmt*
EiFactory::new_ForStmt(
  const VlScope* parent,
  const VlProcess* process,
  const PtStmt* pt_stmt,
  const VlExpr* cond,
  const VlStmt* init_stmt,
  const VlStmt* inc_stmt,
  const VlStmt* stmt
)
{
  auto stmt1 = new EiForStmt{parent, process, pt_stmt,
			     cond, init_stmt, inc_stmt, stmt};
  return stmt1;
}

// @brief forever 文を生成する．
const VlStmt*
EiFactory::new_ForeverStmt(
  const VlScope* parent,
  const VlProcess* process,
  const PtStmt* pt_stmt,
  const VlStmt* stmt
)
{
  auto stmt1 = new EiForeverStmt{parent, process, pt_stmt, stmt};

  return stmt1;
}

// @brief if 文を生成する．
const VlStmt*
EiFactory::new_IfStmt(
  const VlScope* parent,
  const VlProcess* process,
  const PtStmt* pt_stmt,
  const VlExpr* cond,
  const VlStmt* stmt,
  const VlStmt* else_stmt
)
{
  const VlStmt* stmt1;
  if ( else_stmt ) {
    stmt1 = new EiIfElseStmt{parent, process, pt_stmt,
			     cond, stmt, else_stmt};
  }
  else {
    stmt1 = new EiIfStmt{parent, process, pt_stmt,
			 cond, stmt};
  }
  return stmt1;
}

// @brief case 文を生成する．
const VlStmt*
EiFactory::new_CaseStmt(
  const VlScope* parent,
  const VlProcess* process,
  const PtStmt* pt_stmt,
  const VlExpr* expr,
  const vector<const VlCaseItem*>& caseitem_list
)
{
  auto stmt1 = new EiCaseStmt{parent, process, pt_stmt,
			      expr, caseitem_list};
  return stmt1;
}

// @brief caseitem を生成する．
const VlCaseItem*
EiFactory::new_CaseItem(
  const PtCaseItem* pt_item,
  const vector<ElbExpr*>& label_list,
  const VlStmt* body
)
{
  auto caseitem = new EiCaseItem{pt_item, label_list, body};
  return caseitem;
}


//////////////////////////////////////////////////////////////////////
// クラス EiLoopStmt
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiLoopStmt::EiLoopStmt(
  const VlScope* parent,
  const VlProcess* process,
  const PtStmt* pt_stmt,
  const VlExpr* cond,
  const VlStmt* stmt
) : EiStmtBase{parent, process, pt_stmt},
    mCondition{cond},
    mBodyStmt{stmt}
{
}

// @brief デストラクタ
EiLoopStmt::~EiLoopStmt()
{
}

// @brief 条件式を返す．
const VlExpr*
EiLoopStmt::expr() const
{
  return mCondition;
}

// @brief 本体のステートメントを返す．
const VlStmt*
EiLoopStmt::body_stmt() const
{
  return mBodyStmt;
}


//////////////////////////////////////////////////////////////////////
// クラス EiWhileStmt
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiWhileStmt::EiWhileStmt(
  const VlScope* parent,
  const VlProcess* process,
  const PtStmt* pt_stmt,
  const VlExpr* cond,
  const VlStmt* stmt
) : EiLoopStmt{parent, process, pt_stmt, cond, stmt}
{
}

// @brief デストラクタ
EiWhileStmt::~EiWhileStmt()
{
}

// @brief 型の取得
VpiObjType
EiWhileStmt::type() const
{
  return VpiObjType::While;
}


//////////////////////////////////////////////////////////////////////
// クラス EiRepeatStmt
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiRepeatStmt::EiRepeatStmt(
  const VlScope* parent,
  const VlProcess* process,
  const PtStmt* pt_stmt,
  const VlExpr* cond,
  const VlStmt* stmt
) : EiLoopStmt{parent, process, pt_stmt, cond, stmt}
{
}

// @brief デストラクタ
EiRepeatStmt::~EiRepeatStmt()
{
}

// @brief 型の取得
VpiObjType
EiRepeatStmt::type() const
{
  return VpiObjType::Repeat;
}


//////////////////////////////////////////////////////////////////////
// クラス EiWaitStmt
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiWaitStmt::EiWaitStmt(
  const VlScope* parent,
  const VlProcess* process,
  const PtStmt* pt_stmt,
  const VlExpr* cond,
  const VlStmt* stmt
) : EiLoopStmt{parent, process, pt_stmt, cond, stmt}
{
}

// @brief デストラクタ
EiWaitStmt::~EiWaitStmt()
{
}

// @brief 型の取得
VpiObjType
EiWaitStmt::type() const
{
  return VpiObjType::Wait;
}


//////////////////////////////////////////////////////////////////////
// クラス EiForStmt
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiForStmt::EiForStmt(
  const VlScope* parent,
  const VlProcess* process,
  const PtStmt* pt_stmt,
  const VlExpr* cond,
  const VlStmt* init_stmt,
  const VlStmt* inc_stmt,
  const VlStmt* stmt
) : EiLoopStmt{parent, process, pt_stmt, cond, stmt},
    mInitStmt{init_stmt},
    mIncStmt{inc_stmt}
{
}

// @brief デストラクタ
EiForStmt::~EiForStmt()
{
}

// @brief 型の取得
VpiObjType
EiForStmt::type() const
{
  return VpiObjType::For;
}

// 初期化文を返す．
const VlStmt*
EiForStmt::init_stmt() const
{
  return mInitStmt;
}

// 繰り返し文を返す．
const VlStmt*
EiForStmt::inc_stmt() const
{
  return mIncStmt;
}


//////////////////////////////////////////////////////////////////////
// クラス EiForeverStmt
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiForeverStmt::EiForeverStmt(
  const VlScope* parent,
  const VlProcess* process,
  const PtStmt* pt_stmt,
  const VlStmt* stmt
) : EiStmtBase{parent, process, pt_stmt},
    mBodyStmt{stmt}
{
}

// @brief デストラクタ
EiForeverStmt::~EiForeverStmt()
{
}

// @brief 型の取得
VpiObjType
EiForeverStmt::type() const
{
  return VpiObjType::Forever;
}

// @brief 本体のステートメントを返す．
const VlStmt*
EiForeverStmt::body_stmt() const
{
  return mBodyStmt;
}


//////////////////////////////////////////////////////////////////////
// クラス IfStmt
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiIfStmt::EiIfStmt(
  const VlScope* parent,
  const VlProcess* process,
  const PtStmt* pt_stmt,
  const VlExpr* cond,
  const VlStmt* stmt
) : EiStmtBase{parent, process, pt_stmt},
    mCondition{cond},
    mBodyStmt{stmt}
{
}

// @brief デストラクタ
EiIfStmt::~EiIfStmt()
{
}

// @brief 型の取得
VpiObjType
EiIfStmt::type() const
{
  return VpiObjType::If;
}

// @brief 条件式を返す．
const VlExpr*
EiIfStmt::expr() const
{
  return mCondition;
}

// @brief 本体のステートメントを返す．
const VlStmt*
EiIfStmt::body_stmt() const
{
  return mBodyStmt;
}


//////////////////////////////////////////////////////////////////////
// クラス EiIfElseStmt
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiIfElseStmt::EiIfElseStmt(
  const VlScope* parent,
  const VlProcess* process,
  const PtStmt* pt_stmt,
  const VlExpr* cond,
  const VlStmt* stmt,
  const VlStmt* else_stmt
) : EiIfStmt{parent, process, pt_stmt, cond, stmt},
    mElseStmt{else_stmt}
{
}

// @brief デストラクタ
EiIfElseStmt::~EiIfElseStmt()
{
}

// @brief 型の取得
VpiObjType
EiIfElseStmt::type() const
{
  return VpiObjType::IfElse;
}

// @brief else ステートメントを返す．
const VlStmt*
EiIfElseStmt::else_stmt() const
{
  return mElseStmt;
}


//////////////////////////////////////////////////////////////////////
// クラス EiCaseItem
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiCaseItem::EiCaseItem(
  const PtCaseItem* pt_item,
  const vector<ElbExpr*>& label_list,
  const VlStmt* body
) : mPtCaseItem{pt_item},
    mExprList{label_list},
    mBodyStmt{body}
{
}

// @brief デストラクタ
EiCaseItem::~EiCaseItem()
{
}

// @brief 型の取得
VpiObjType
EiCaseItem::type() const
{
  return VpiObjType::CaseItem;
}

// @brief ファイル位置を返す．
FileRegion
EiCaseItem::file_region() const
{
  return mPtCaseItem->file_region();
}

// @brief 条件式のリストの要素数を返す．
SizeType
EiCaseItem::expr_num() const
{
  return mExprList.size();
}

// @brief 条件式を返す．
const VlExpr*
EiCaseItem::expr(
  SizeType pos
) const
{
  ASSERT_COND( 0 <= pos && pos < expr_num() );
  return mExprList[pos];
}

// @brief 条件式のリストを返す．
vector<const VlExpr*>
EiCaseItem::expr_list() const
{
  return vector<const VlExpr*>{mExprList.begin(), mExprList.end()};
}

// @brief 本体のステートメントを返す．
const VlStmt*
EiCaseItem::body_stmt() const
{
  return mBodyStmt;
}


//////////////////////////////////////////////////////////////////////
// クラス EiCase
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiCaseStmt::EiCaseStmt(
  const VlScope* parent,
  const VlProcess* process,
  const PtStmt* pt_stmt,
  const VlExpr* expr,
  const vector<const VlCaseItem*>& caseitem_list
) : EiStmtBase{parent, process, pt_stmt},
    mCondition{expr},
    mCaseItemList{caseitem_list}
{
}

// @brief デストラクタ
EiCaseStmt::~EiCaseStmt()
{
}

// @brief 型の取得
VpiObjType
EiCaseStmt::type() const
{
  return VpiObjType::Case;
}

// @brief case type を返す．
VpiCaseType
EiCaseStmt::case_type() const
{
  switch ( pt_stmt()->type() ) {
  case PtStmtType::Case:  return VpiCaseType::Exact;
  case PtStmtType::CaseX: return VpiCaseType::X;
  case PtStmtType::CaseZ: return VpiCaseType::Z;
  default: break;
  }
  ASSERT_NOT_REACHED;
  return VpiCaseType::Exact;
}

// @brief 条件式を返す．
const VlExpr*
EiCaseStmt::expr() const
{
  return mCondition;
}

// @brief case item のリストの要素数を返す．
SizeType
EiCaseStmt::caseitem_num() const
{
  return mCaseItemList.size();
}

// @brief case item を返す．
const VlCaseItem*
EiCaseStmt::caseitem(
  SizeType pos
) const
{
  ASSERT_COND( 0 <= pos && pos < caseitem_num() );
  return mCaseItemList[pos];
}

// @brief case item のリストを返す．
vector<const VlCaseItem*>
EiCaseStmt::caseitem_list() const
{
  return mCaseItemList;
}

END_NAMESPACE_YM_VERILOG
