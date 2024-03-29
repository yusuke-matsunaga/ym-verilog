﻿
/// @file StmtGen_assign.cc
/// @brief ElbMgr の実装ファイル(assign statement の実体化)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2020 Yusuke Matsunaga
/// All rights reserved.

#include "StmtGen.h"
#include "ElbEnv.h"
#include "elaborator/ElbExpr.h"

#include "ym/pt/PtStmt.h"

#include "ym/MsgMgr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// 代入系ステートメント関係の instantiate 関数
//////////////////////////////////////////////////////////////////////

// 代入文のインスタンス化を行う．
const VlStmt*
StmtGen::instantiate_assign(
  const VlScope* parent,
  const VlProcess* process,
  const ElbEnv& env,
  const PtStmt* pt_stmt,
  bool block
)
{
  ElbVarLhsEnv env1{env};
  auto pt_lhs = pt_stmt->lhs();
  auto lhs = instantiate_lhs(parent, env1, pt_lhs);
  if ( !lhs ) {
    return nullptr;
  }

  auto pt_rhs = pt_stmt->rhs();
  auto rhs = instantiate_rhs(parent, env, pt_rhs, lhs);
  if ( !rhs ) {
    return nullptr;
  }

  auto pt_control = pt_stmt->control();
  if ( pt_control && env.inside_function() ) {
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

  auto control = instantiate_control(parent, env, pt_control);
  auto stmt = mgr().new_Assignment(parent, process, pt_stmt,
				   lhs, rhs, block, control);

  return stmt;
}

// @brief procedural continuous assign 文のインスタンス化を行う．
const VlStmt*
StmtGen::instantiate_pca(
  const VlScope* parent,
  const VlProcess* process,
  const ElbEnv& env,
  const PtStmt* pt_stmt
)
{
  ElbPcaLhsEnv env1{env};
  auto pt_lhs = pt_stmt->lhs();
  auto lhs = instantiate_lhs(parent, env1, pt_lhs);
  if ( !lhs ) {
    return nullptr;
  }

  auto pt_rhs = pt_stmt->rhs();
  auto rhs = instantiate_rhs(parent, env, pt_rhs, lhs);
  if ( !rhs ) {
    return nullptr;
  }

  auto stmt = mgr().new_AssignStmt(parent, process, pt_stmt, lhs, rhs);

  return stmt;
}

// @brief deassign 文のインスタンス化を行う．
const VlStmt*
StmtGen::instantiate_deassign(
  const VlScope* parent,
  const VlProcess* process,
  const ElbEnv& env,
  const PtStmt* pt_stmt
)
{
  ElbPcaLhsEnv env1{env};
  auto pt_lhs = pt_stmt->lhs();
  auto lhs = instantiate_lhs(parent, env1, pt_lhs);
  if ( !lhs ) {
    return nullptr;
  }

  auto stmt = mgr().new_DeassignStmt(parent, process, pt_stmt, lhs);

  return stmt;
}

// @brief force 文のインスタンス化を行う．
const VlStmt*
StmtGen::instantiate_force(
  const VlScope* parent,
  const VlProcess* process,
  const ElbEnv& env,
  const PtStmt* pt_stmt
)
{
  ElbForceLhsEnv env1{env};
  auto pt_lhs = pt_stmt->lhs();
  auto lhs = instantiate_lhs(parent, env1, pt_lhs);
  if ( !lhs ) {
    return nullptr;
  }

  auto pt_rhs = pt_stmt->rhs();
  auto rhs = instantiate_rhs(parent, env, pt_rhs, lhs);
  if ( !rhs ) {
    return nullptr;
  }

  auto stmt = mgr().new_ForceStmt(parent, process, pt_stmt, lhs, rhs);

  return stmt;
}

// @brief release 文のインスタンス化を行う．
const VlStmt*
StmtGen::instantiate_release(
  const VlScope* parent,
  const VlProcess* process,
  const ElbEnv& env,
  const PtStmt* pt_stmt
)
{
  ElbForceLhsEnv env1{env};
  auto pt_lhs = pt_stmt->lhs();
  auto lhs = instantiate_lhs(parent, env1, pt_lhs);
  if ( !lhs ) {
    return nullptr;
  }

  auto stmt = mgr().new_ReleaseStmt(parent, process, pt_stmt, lhs);

  return stmt;
}

END_NAMESPACE_YM_VERILOG
