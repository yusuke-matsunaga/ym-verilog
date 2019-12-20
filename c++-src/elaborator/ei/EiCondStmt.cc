
/// @file EiCondStmt.cc
/// @brief EiCondStmt の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "EiFactory.h"
#include "EiCondStmt.h"

#include "elb/ElbExpr.h"

#include "ym/BitVector.h"
#include "ym/pt/PtStmt.h"
#include "ym/pt/PtArray.h"



BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief while 文を生成する．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] cond 条件式
// @param[in] stmt 本体のステートメント
ElbStmt*
EiFactory::new_WhileStmt(const VlNamedObj* parent,
			 ElbProcess* process,
			 const PtStmt* pt_stmt,
			 ElbExpr* cond,
			 ElbStmt* stmt)
{
  ElbStmt* stmt1 = new EiWhileStmt(parent, process, pt_stmt,
				   cond, stmt);

  return stmt1;
}

// @brief repeat 文を生成する．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] cond 条件式
// @param[in] stmt 本体のステートメント
ElbStmt*
EiFactory::new_RepeatStmt(const VlNamedObj* parent,
			  ElbProcess* process,
			  const PtStmt* pt_stmt,
			  ElbExpr* cond,
			  ElbStmt* stmt)
{
  ElbStmt* stmt1 = new EiRepeatStmt(parent, process, pt_stmt,
				    cond, stmt);

  return stmt1;
}

// @brief wait 文を生成する．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] cond 条件式
// @param[in] stmt 本体のステートメント
ElbStmt*
EiFactory::new_WaitStmt(const VlNamedObj* parent,
			ElbProcess* process,
			const PtStmt* pt_stmt,
			ElbExpr* cond,
			ElbStmt* stmt)
{
  ElbStmt* stmt1 = new EiWaitStmt(parent, process, pt_stmt,
				  cond, stmt);

  return stmt1;
}

// @brief for 文を生成する．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] cond 条件式
// @param[in] init_stmt 初期化式
// @param[in] inc_stmt 増加式
// @param[in] stmt 本体のステートメント
ElbStmt*
EiFactory::new_ForStmt(const VlNamedObj* parent,
		       ElbProcess* process,
		       const PtStmt* pt_stmt,
		       ElbExpr* cond,
		       ElbStmt* init_stmt,
		       ElbStmt* inc_stmt,
		       ElbStmt* stmt)
{
  ElbStmt* stmt1 = new EiForStmt(parent, process, pt_stmt,
				 cond, init_stmt, inc_stmt, stmt);

  return stmt1;
}

// @brief forever 文を生成する．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] stmt 本体のステートメント
ElbStmt*
EiFactory::new_ForeverStmt(const VlNamedObj* parent,
			   ElbProcess* process,
			   const PtStmt* pt_stmt,
			   ElbStmt* stmt)
{
  ElbStmt* stmt1 = new EiForeverStmt(parent, process, pt_stmt,
				     stmt);

  return stmt1;
}

// @brief if 文を生成する．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] cond 条件式
// @param[in] stmt 本体のステートメント
// @param[in] else_stmt else節のステートメント
// @note else_stmt は nullptr もありうる．
ElbStmt*
EiFactory::new_IfStmt(const VlNamedObj* parent,
		      ElbProcess* process,
		      const PtStmt* pt_stmt,
		      ElbExpr* cond,
		      ElbStmt* stmt,
		      ElbStmt* else_stmt)
{
  ElbStmt* stmt1;
  if ( else_stmt ) {
    stmt1 = new EiIfElseStmt(parent, process, pt_stmt,
				 cond, stmt, else_stmt);
  }
  else {
    stmt1 = new EiIfStmt(parent, process, pt_stmt,
			     cond, stmt);
  }
  return stmt1;
}

// @brief case 文を生成する．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] expr 条件式
ElbStmt*
EiFactory::new_CaseStmt(const VlNamedObj* parent,
			ElbProcess* process,
			const PtStmt* pt_stmt,
			ElbExpr* expr)
{
  int caseitem_num = pt_stmt->caseitem_list().size();

  EiCaseItem* array = new EiCaseItem[caseitem_num];
  EiCaseStmt* stmt1 = new EiCaseStmt(parent, process, pt_stmt,
				     expr,
				     caseitem_num, array);

  return stmt1;
}


//////////////////////////////////////////////////////////////////////
// クラス EiLoopStmt
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] cond 条件式
// @param[in] stmt 本体のステートメント
EiLoopStmt::EiLoopStmt(const VlNamedObj* parent,
		       ElbProcess* process,
		       const PtStmt* pt_stmt,
		       ElbExpr* cond,
		       ElbStmt* stmt) :
  EiStmtBase(parent, process, pt_stmt),
  mCondition(cond),
  mBodyStmt(stmt)
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

ElbStmt*
EiLoopStmt::_body_stmt() const
{
  return mBodyStmt;
}


//////////////////////////////////////////////////////////////////////
// クラス EiWhileStmt
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] cond 条件式
// @param[in] stmt 本体のステートメント
EiWhileStmt::EiWhileStmt(const VlNamedObj* parent,
			 ElbProcess* process,
			 const PtStmt* pt_stmt,
			 ElbExpr* cond,
			 ElbStmt* stmt) :
  EiLoopStmt(parent, process, pt_stmt, cond, stmt)
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
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] cond 条件式
// @param[in] stmt 本体のステートメント
EiRepeatStmt::EiRepeatStmt(const VlNamedObj* parent,
			   ElbProcess* process,
			   const PtStmt* pt_stmt,
			   ElbExpr* cond,
			   ElbStmt* stmt) :
  EiLoopStmt(parent, process, pt_stmt, cond, stmt)
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
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] cond 条件式
// @param[in] stmt 本体のステートメント
EiWaitStmt::EiWaitStmt(const VlNamedObj* parent,
		       ElbProcess* process,
		       const PtStmt* pt_stmt,
		       ElbExpr* cond,
		       ElbStmt* stmt) :
  EiLoopStmt(parent, process, pt_stmt, cond, stmt)
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
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] cond 条件式
// @param[in] init_stmt 初期化式
// @param[in] inc_stmt 増加式
// @param[in] stmt 本体のステートメント
EiForStmt::EiForStmt(const VlNamedObj* parent,
		     ElbProcess* process,
		     const PtStmt* pt_stmt,
		     ElbExpr* cond,
		     ElbStmt* init_stmt,
		     ElbStmt* inc_stmt,
		     ElbStmt* stmt) :
  EiLoopStmt(parent, process, pt_stmt, cond, stmt),
  mInitStmt(init_stmt),
  mIncStmt(inc_stmt)
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
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] stmt 本体のステートメント
EiForeverStmt::EiForeverStmt(const VlNamedObj* parent,
			     ElbProcess* process,
			     const PtStmt* pt_stmt,
			     ElbStmt* stmt) :
  EiStmtBase(parent, process, pt_stmt),
  mBodyStmt(stmt)
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
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] cond 条件式
// @param[in] stmt 本体のステートメント
EiIfStmt::EiIfStmt(const VlNamedObj* parent,
		   ElbProcess* process,
		   const PtStmt* pt_stmt,
		   ElbExpr* cond,
		   ElbStmt* stmt) :
  EiStmtBase(parent, process, pt_stmt),
  mCondition(cond),
  mBodyStmt(stmt)
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


ElbStmt*
EiIfStmt::_body_stmt() const
{
  return mBodyStmt;
}


//////////////////////////////////////////////////////////////////////
// クラス EiIfElseStmt
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] cond 条件式
// @param[in] stmt 本体のステートメント
// @param[in] else_stmt else節のステートメント
EiIfElseStmt::EiIfElseStmt(const VlNamedObj* parent,
			   ElbProcess* process,
			   const PtStmt* pt_stmt,
			   ElbExpr* cond,
			   ElbStmt* stmt,
			   ElbStmt* else_stmt) :
  EiIfStmt(parent, process, pt_stmt, cond, stmt),
  mElseStmt(else_stmt)
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
EiCaseItem::EiCaseItem() :
  mExprNum(0),
  mExprList(nullptr),
  mBodyStmt(nullptr)
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
  return mExprNum;
}

// @brief 条件式を返す．
// @param[in] pos 位置番号
const VlExpr*
EiCaseItem::expr(SizeType pos) const
{
  return mExprList[pos];
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
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] expr 条件式
// @param[in] caseitem_list caseitem のリストの要素数
// @param[in] caseitem_array caseitem の配列
EiCaseStmt::EiCaseStmt(const VlNamedObj* parent,
		       ElbProcess* process,
		       const PtStmt* pt_stmt,
		       ElbExpr* expr,
		       int caseitem_num,
		       EiCaseItem* caseitem_array) :
  EiStmtBase(parent, process, pt_stmt),
  mCondition(expr),
  mCaseItemNum(caseitem_num),
  mCaseItemList(caseitem_array)
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
  case kPtCaseStmt:  return VpiCaseType::Exact;
  case kPtCaseXStmt: return VpiCaseType::X;
  case kPtCaseZStmt: return VpiCaseType::Z;
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
  return mCaseItemNum;
}

// @brief case item を返す．
// @param[in] pos 位置番号
const VlCaseItem*
EiCaseStmt::caseitem(SizeType pos) const
{
  return &mCaseItemList[pos];
}

// @brief caseitem を設定する．
// @param[in] pt_caseitem パース木の caseitem 定義
// @param[in] expr_array ラベルのリスト用配列
// @param[in] stmt 本体のステートメント
void
EiCaseStmt::set_caseitem(SizeType pos,
			 const PtCaseItem* pt_caseitem,
			 ElbExpr** expr_array,
			 ElbStmt* stmt)
{
  EiCaseItem& ci = mCaseItemList[pos];
  ci.mPtCaseItem = pt_caseitem;
  ci.mExprNum = pt_caseitem->label_list().size();
  ci.mExprList = expr_array;
  ci.mBodyStmt = stmt;
}

END_NAMESPACE_YM_VERILOG
