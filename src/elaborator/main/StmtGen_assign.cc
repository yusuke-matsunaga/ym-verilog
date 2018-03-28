﻿
/// @file StmtGen_assign.cc
/// @brief ElbMgr の実装ファイル(assign statement の実体化)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "StmtGen.h"
#include "ElbEnv.h"

#include "ym/pt/PtStmt.h"

#include "ym/MsgMgr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// 代入系ステートメント関係の instantiate 関数
//////////////////////////////////////////////////////////////////////

// 代入文のインスタンス化を行う．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] env 生成時の環境
// @param[in] pt_stmt 対象のステートメント
// @param[in] block ブロッキング代入の時 true
ElbStmt*
StmtGen::instantiate_assign(const VlNamedObj* parent,
			    ElbProcess* process,
			    const ElbEnv& env,
			    const PtStmt* pt_stmt,
			    bool block)
{
  const PtExpr* pt_lhs = pt_stmt->lhs();
  const PtExpr* pt_rhs = pt_stmt->rhs();
  const PtControl* pt_control = pt_stmt->control();

  ElbVarLhsEnv env1(env);
  ElbExpr* lhs = instantiate_lhs(parent, env1, pt_lhs);
  if ( !lhs ) {
    return nullptr;
  }
  ElbExpr* rhs = instantiate_rhs(parent, env, pt_rhs, lhs);
  if ( !rhs ) {
    return nullptr;
  }

  ElbControl* control = nullptr;
  if ( pt_control ) {
    if ( env.inside_function() ) {
      // function 内のインスタンス化なのでコントロールは付いていないはず
      ostringstream buf;
      MsgMgr::put_msg(__FILE__, __LINE__,
		      pt_stmt->file_region(),
		      MsgType::Error,
		      "ELAB",
		      "assignment in a constant function cannot have"
		      " timing/event specification.");
      return nullptr;
    }
    else {
      // 通常のインスタンス化
      control = instantiate_control(parent, env, pt_control);
    }
  }

  ElbStmt* stmt = factory().new_Assignment(parent, process, pt_stmt,
					   lhs, rhs, block, control);

  return stmt;
}

// @brief procedural continuous assign 文のインスタンス化を行う．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] env 生成時の環境
// @param[in] pt_stmt 対象のステートメント
ElbStmt*
StmtGen::instantiate_pca(const VlNamedObj* parent,
			 ElbProcess* process,
			 const ElbEnv& env,
			 const PtStmt* pt_stmt)
{
  const PtExpr* pt_lhs = pt_stmt->lhs();
  const PtExpr* pt_rhs = pt_stmt->rhs();
  ElbPcaLhsEnv env1(env);
  ElbExpr* lhs = instantiate_lhs(parent, env1, pt_lhs);
  if ( !lhs ) {
    return nullptr;
  }
  ElbExpr* rhs = instantiate_rhs(parent, env, pt_rhs, lhs);
  if ( !rhs ) {
    return nullptr;
  }

  ElbStmt* stmt = factory().new_AssignStmt(parent, process, pt_stmt,
					   lhs, rhs);

  return stmt;
}

// @brief deassign 文のインスタンス化を行う．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] env 生成時の環境
// @param[in] pt_stmt 対象のステートメント
ElbStmt*
StmtGen::instantiate_deassign(const VlNamedObj* parent,
			      ElbProcess* process,
			      const ElbEnv& env,
			      const PtStmt* pt_stmt)
{
  const PtExpr* pt_lhs = pt_stmt->lhs();
  ElbPcaLhsEnv env1(env);
  ElbExpr* lhs = instantiate_lhs(parent, env1, pt_lhs);
  if ( !lhs ) {
    return nullptr;
  }

  ElbStmt* stmt = factory().new_DeassignStmt(parent, process, pt_stmt,
					     lhs);

  return stmt;
}

// @brief force 文のインスタンス化を行う．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] env 生成時の環境
// @param[in] pt_stmt 対象のステートメント
ElbStmt*
StmtGen::instantiate_force(const VlNamedObj* parent,
			   ElbProcess* process,
			   const ElbEnv& env,
			   const PtStmt* pt_stmt)
{
  const PtExpr* pt_lhs = pt_stmt->lhs();
  const PtExpr* pt_rhs = pt_stmt->rhs();
  ElbForceLhsEnv env1(env);
  ElbExpr* lhs = instantiate_lhs(parent, env1, pt_lhs);
  if ( !lhs ) {
    return nullptr;
  }
  ElbExpr* rhs = instantiate_rhs(parent, env, pt_rhs, lhs);
  if ( !rhs ) {
    return nullptr;
  }

  ElbStmt* stmt = factory().new_ForceStmt(parent, process, pt_stmt,
					  lhs, rhs);

  return stmt;
}

// @brief release 文のインスタンス化を行う．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] env 生成時の環境
// @param[in] pt_stmt 対象のステートメント
ElbStmt*
StmtGen::instantiate_release(const VlNamedObj* parent,
			     ElbProcess* process,
			     const ElbEnv& env,
			     const PtStmt* pt_stmt)
{
  const PtExpr* pt_lhs = pt_stmt->lhs();
  ElbForceLhsEnv env1(env);
  ElbExpr* lhs = instantiate_lhs(parent, env1, pt_lhs);
  if ( !lhs ) {
    return nullptr;
  }

  ElbStmt* stmt = factory().new_ReleaseStmt(parent, process, pt_stmt,
					    lhs);

  return stmt;
}

END_NAMESPACE_YM_VERILOG
