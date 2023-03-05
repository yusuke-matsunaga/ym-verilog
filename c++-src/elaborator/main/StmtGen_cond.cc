
/// @file StmtGen_cond.cc
/// @brief ElbMgr の実装ファイル(conditional statement の実体化)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.

#include "StmtGen.h"
#include "ElbEnv.h"

#include "elaborator/ElbExpr.h"

#include "ym/pt/PtStmt.h"
#include "ym/pt/PtExpr.h"

#include "ym/MsgMgr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// 条件ステートメント関係の instantiate 関数
//////////////////////////////////////////////////////////////////////

// @brief if 文の生成を行う．
const VlStmt*
StmtGen::instantiate_if(
  const VlScope* parent,
  const VlProcess* process,
  const ElbEnv& env,
  const PtStmt* pt_stmt
)
{
  auto pt_cond = pt_stmt->expr();
  auto cond = instantiate_expr(parent, env, pt_cond);

  auto pt_then = pt_stmt->body();
  auto then_stmt = instantiate_stmt(parent, process, env, pt_then);

  auto pt_else = pt_stmt->else_body();
  auto else_stmt = instantiate_stmt(parent, process, env, pt_else);

  if ( !cond || !then_stmt || ( pt_else && !else_stmt ) ) {
    // たぶんエラー
    return nullptr;
  }

  auto stmt = mgr().new_IfStmt(parent, process, pt_stmt,
			       cond, then_stmt, else_stmt);
  return stmt;
}

// @brief case 文の実体化を行う．
const VlStmt*
StmtGen::instantiate_case(
  const VlScope* parent,
  const VlProcess* process,
  const ElbEnv& env,
  const PtStmt* pt_stmt
)
{
  // 条件式の生成
  auto pt_cond = pt_stmt->expr();
  auto cond = instantiate_expr(parent, env, pt_cond);
  if ( !cond ) {
    // たぶんエラー
    return nullptr;
  }

  // この case 文に関係する全ての式のリスト
  // あとでサイズ調整をするために用いる．
  vector<ElbExpr*> expr_list;
  SizeType ne{0};
  for ( auto pt_item: pt_stmt->caseitem_list() ) {
    ne += pt_item->label_num();
  }
  expr_list.reserve(ne);

  // default caseitem を末尾にするために順序づけを行う．
  // Parser::check_default_label() で default が高々1個しかないことは確認済み．
  vector<const PtCaseItem*> pt_caseitem_list;
  {
    SizeType nc = pt_stmt->caseitem_num();
    pt_caseitem_list.reserve(nc);
    const PtCaseItem* default_caseitem{nullptr};
    for ( auto pt_item: pt_stmt->caseitem_list() ) {
      if ( pt_item->label_num() > 0 ) {
	pt_caseitem_list.push_back(pt_item);
      }
      else {
	default_caseitem = pt_item;
      }
    }
    if ( default_caseitem ) {
      // default caseitem を末尾に置く．
      pt_caseitem_list.push_back(default_caseitem);
    }
  }

  // case-item の生成
  vector<const VlCaseItem*> caseitem_list;
  caseitem_list.reserve(pt_caseitem_list.size());
  for ( auto pt_item: pt_caseitem_list ) {
    auto pt_body = pt_item->body();
    auto body = instantiate_stmt(parent, process, env, pt_body);
    if ( pt_body && !body ) {
      // たぶんエラー
      return nullptr;
    }
    // pt_body が空の場合はあり．

    // ラベルの生成と設定
    SizeType n = pt_item->label_num();
    vector<ElbExpr*> label_list;
    label_list.reserve(n);
    for ( auto pt_expr: pt_item->label_list() ) {
      auto expr = instantiate_expr(parent, env, pt_expr);
      if ( !expr ) {
	// たぶんエラー
	return nullptr;
      }
      label_list.push_back(expr);
      expr_list.push_back(expr);
    }

    // caseitem の生成
    auto caseitem = mgr().new_CaseItem(pt_item, label_list, body);
    caseitem_list.push_back(caseitem);
  }

  // expr_list のサイズを合わせる．
  // ルールは
  // - どれか一つでも符号付きならすべて符号付き
  // - サイズは各要素の最大サイズ
  // 初期値は cond のものを用いる．
  auto value_type0 = cond->value_type();
  if ( value_type0.is_real_type() ) {
    // real 型は駄目
    MsgMgr::put_msg(__FILE__, __LINE__,
		    cond->file_region(),
		    MsgType::Error,
		    "ELAB",
		    "Case expression should not be real-type.");
    return nullptr;
  }
  bool sign = value_type0.is_signed();
  SizeType size = value_type0.size();

  for ( auto expr: expr_list ) {
    auto value_type1 = expr->value_type();
    if ( value_type1.is_real_type() ) {
      // real 型は駄目
      MsgMgr::put_msg(__FILE__, __LINE__,
		      expr->file_region(),
		      MsgType::Error,
		      "ELAB",
		      "Case-item expression should not be real-type.");
      return nullptr;
    }

    if ( value_type1.is_signed() ) {
      // 符号付き
      sign = true;
    }

    SizeType size1 = value_type1.size();
    if ( size < size1 ) {
      // 最大サイズの更新
      size = size1;
    }
  }

  VlValueType value_type{sign, true, size};
  cond->set_reqsize(value_type);
  for ( auto expr: expr_list ) {
    expr->set_reqsize(value_type);
  }

  // case statement の生成
  auto stmt = mgr().new_CaseStmt(parent, process, pt_stmt,
				 cond, caseitem_list);
  return stmt;
}

// @brief wait 文の実体化を行う．
const VlStmt*
StmtGen::instantiate_wait(
  const VlScope* parent,
  const VlProcess* process,
  const ElbEnv& env,
  const PtStmt* pt_stmt
)
{
  auto pt_cond = pt_stmt->expr();
  auto cond = instantiate_expr(parent, env, pt_cond);

  auto pt_body = pt_stmt->body();
  auto body = instantiate_stmt(parent, process, env, pt_body);

  if ( !cond || !body ) {
    return nullptr;
  }

  auto stmt = mgr().new_WaitStmt(parent, process, pt_stmt, cond, body);
  return stmt;
}

// @brief forever 文のインスタンス化を行う．
const VlStmt*
StmtGen::instantiate_forever(
  const VlScope* parent,
  const VlProcess* process,
  const ElbEnv& env,
  const PtStmt* pt_stmt
)
{
  auto pt_body = pt_stmt->body();
  auto body = instantiate_stmt(parent, process, env, pt_body);

  if ( !body ) {
    return nullptr;
  }

  auto stmt = mgr().new_ForeverStmt(parent, process, pt_stmt, body);
  return stmt;
}

// @brief repeat 文のインスタンス化を行う．
const VlStmt*
StmtGen::instantiate_repeat(
  const VlScope* parent,
  const VlProcess* process,
  const ElbEnv& env,
  const PtStmt* pt_stmt
)
{
  auto pt_expr = pt_stmt->expr();
  auto expr = instantiate_expr(parent, env, pt_expr);

  auto pt_body = pt_stmt->body();
  auto body = instantiate_stmt(parent, process, env, pt_body);

  if ( !expr || !body ) {
    return nullptr;
  }

  auto stmt = mgr().new_RepeatStmt(parent, process, pt_stmt, expr, body);
  return stmt;
}

// @brief while 文のインスタンス化を行う．
const VlStmt*
StmtGen::instantiate_while(
  const VlScope* parent,
  const VlProcess* process,
  const ElbEnv& env,
  const PtStmt* pt_stmt
)
{
  auto pt_cond = pt_stmt->expr();
  auto cond = instantiate_expr(parent, env, pt_cond);

  auto pt_body = pt_stmt->body();
  auto body = instantiate_stmt(parent, process, env, pt_body);

  if ( !cond || !body ) {
    return nullptr;
  }

  auto stmt = mgr().new_WhileStmt(parent, process, pt_stmt, cond, body);
  return stmt;
}

// @brief for 文のインスタンス化を行う．
const VlStmt*
StmtGen::instantiate_for(
  const VlScope* parent,
  const VlProcess* process,
  const ElbEnv& env,
  const PtStmt* pt_stmt
)
{
  auto pt_cond = pt_stmt->expr();
  auto cond = instantiate_expr(parent, env, pt_stmt->expr());

  auto pt_init = pt_stmt->init_stmt();
  auto init = instantiate_stmt(parent, process, env, pt_init);

  auto pt_next = pt_stmt->next_stmt();
  auto next = instantiate_stmt(parent, process, env, pt_next);

  auto pt_body = pt_stmt->body();
  auto body = instantiate_stmt(parent, process, env, pt_body);

  if ( !cond || !init || !next || !body ) {
    return nullptr;
  }

  auto stmt = mgr().new_ForStmt(parent, process, pt_stmt, cond, init, next, body);
  return stmt;
}

END_NAMESPACE_YM_VERILOG
