
/// @file VlDumperImpl_stmt.cc
/// @brief VlDumperImpl の実装ファイル (stmt担当)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2009, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "VlDumperImpl.h"

#include "ym/VlMgr.h"
#include "ym/vl/VlDecl.h"
#include "ym/vl/VlProcess.h"
#include "ym/vl/VlStmt.h"
#include "ym/vl/VlControl.h"
#include "ym/vl/VlModule.h"
#include "ym/vl/VlScope.h"
#include "ym/vl/VlTaskFunc.h"
#include "ym/vl/VlUserSystf.h"
#include "ym/vl/VlExpr.h"


BEGIN_NAMESPACE_YM_VERILOG

// initial/always の内容を出力する関数
// IEEE 1364-2001 p.656
void
VlDumperImpl::put_process(const char* label,
			  const VlMgr& mgr,
			  const VlProcess* process)
{
  const char* nm = nullptr;
  switch ( process->type() ) {
  case VpiObjType::Initial: nm = "Initial"; break;
  case VpiObjType::Always:  nm = "Always"; break;
  default: ASSERT_NOT_REACHED;
  }
  VlDumpHeader x(this, label, nm);

  put("FileRegion", process->file_region() );
  put("vpiModule", process->parent_scope()->parent_module()->full_name() );
  put_stmt("vpiStmt", mgr, process->stmt() );
}

// @brief initial/always のリストの内容を出力する関数
void
VlDumperImpl::put_process_list(const char* label,
			       const VlMgr& mgr,
			       const vector<const VlProcess*>& process_list)
{
  VlDumpHeader x(this, label, "ProcessList");

  for ( auto process: process_list ) {
    put_process(label, mgr, process);
  }
}

// statement の内容を出力する関数
void
VlDumperImpl::put_stmt(const char* label,
		       const VlMgr& mgr,
		       const VlStmt* stmt)
{
  const char* nm = nullptr;
  switch ( stmt->type() ) {
  case VpiObjType::Begin:           nm = "Begin"; break;
  case VpiObjType::Fork:            nm = "Fork"; break;
  case VpiObjType::NamedBegin:      nm = "NamedBegin"; break;
  case VpiObjType::NamedFork:       nm = "NamedFork"; break;
  case VpiObjType::NullStmt:        nm = "NullStmt"; break;
  case VpiObjType::EventStmt:       nm = "EventStmt"; break;
  case VpiObjType::Assignment:      nm = "Assignment"; break;
  case VpiObjType::While:           nm = "While"; break;
  case VpiObjType::Repeat:          nm = "Repeat"; break;
  case VpiObjType::Wait:            nm = "Wait"; break;
  case VpiObjType::For:             nm = "For"; break;
  case VpiObjType::Forever:         nm = "Forever"; break;
  case VpiObjType::If:              nm = "If"; break;
  case VpiObjType::IfElse:          nm = "IfElse"; break;
  case VpiObjType::Case:            nm = "Case"; break;
  case VpiObjType::Force:           nm = "Force"; break;
  case VpiObjType::AssignStmt:      nm = "AssignStmt"; break;
  case VpiObjType::Deassign:        nm = "Deassign"; break;
  case VpiObjType::Release:         nm = "Release"; break;
  case VpiObjType::TaskCall:        nm = "TaskCall"; break;
  case VpiObjType::SysTaskCall:     nm = "SysTaskCall"; break;
  case VpiObjType::Disable:         nm = "Disable"; break;
  case VpiObjType::DelayControl:    nm = "DelayControl"; break;
  case VpiObjType::EventControl:    nm = "EventControl"; break;
  default: assert_not_reached( __FILE__, __LINE__);
  }

  VlDumpHeader x(this, label, nm);

  put("FileRegion", stmt->file_region());
  put("vpiScope", stmt->parent_scope()->full_name() );

  switch ( stmt->type() ) {
  case VpiObjType::NamedBegin:
  case VpiObjType::NamedFork:
    // スコープとしての内容は別に出力されている．
  case VpiObjType::Begin:
  case VpiObjType::Fork:
    put_child_stmt_list("vpiStmt", mgr, stmt);
    break;

  case VpiObjType::EventStmt:
    put("vpiNamedEvent", stmt->named_event()->decompile());
    break;

  case VpiObjType::Assignment:
    put("vpiBlocking", stmt->is_blocking() );
    put_lhs("vpiLhs", mgr, stmt->lhs() );
    put_expr("vpiRhs", mgr, stmt->rhs() );
    put_control("control", mgr, stmt->control() );
    break;

  case VpiObjType::While:
    put_expr("vpiCondition", mgr, stmt->expr() );
    put_stmt("vpiStmt", mgr, stmt->body_stmt() );
    break;

  case VpiObjType::Repeat:
    put_expr("vpiCondition", mgr, stmt->expr() );
    put_stmt("vpiStmt", mgr, stmt->body_stmt() );
    break;

  case VpiObjType::Wait:
    put_expr("vpiCondition", mgr, stmt->expr() );
    put_stmt("vpiStmt", mgr, stmt->body_stmt() );
    break;

  case VpiObjType::For:
    put_stmt("vpiForInitStmt", mgr, stmt->init_stmt() );
    put_expr("vpiCondition", mgr, stmt->expr() );
    put_stmt("vpiForIncStmt", mgr, stmt->inc_stmt() );
    put_stmt("vpiStmt", mgr, stmt->body_stmt() );
    break;

  case VpiObjType::Forever:
    put_stmt("vpiStmt", mgr, stmt->body_stmt() );
    break;

  case VpiObjType::If:
    put_expr("vpiCondition", mgr, stmt->expr() );
    put_stmt("vpiStmt", mgr, stmt->body_stmt() );
    break;

  case VpiObjType::IfElse:
    put_expr("vpiCondition", mgr, stmt->expr() );
    put_stmt("vpiStmt", mgr, stmt->body_stmt() );
    put_stmt("vpiElseStmt", mgr, stmt->else_stmt() );
    break;

  case VpiObjType::Case:
    put("vpiCaseType", stmt->case_type() );
    put_expr("vpiCondition", mgr, stmt->expr() );
    for ( int i = 0; i < stmt->caseitem_num(); ++ i ) {
      auto ci = stmt->caseitem(i);
      VlDumpHeader x(this, "vpiCaseItem", "CaseItem");
      put("FileRegion", ci->file_region() );
      for ( int i = 0; i < ci->expr_num(); ++ i ) {
	auto expr = ci->expr(i);
	put_expr("vpiExpr", mgr, expr);
      }
      put_stmt("vpiStmt", mgr, ci->body_stmt() );
    }
    break;

  case VpiObjType::AssignStmt:
  case VpiObjType::Force:
    put_lhs("vpiLhs", mgr, stmt->lhs() );
    put_expr("vpiRhs", mgr, stmt->rhs() );
    break;

  case VpiObjType::Deassign:
  case VpiObjType::Release:
    put_lhs("vpiLhs", mgr, stmt->lhs() );
    break;

  case VpiObjType::TaskCall:
    put("vpiTask", stmt->task()->full_name() );
    put_argument_list("vpiArgument", mgr, stmt);
    break;

  case VpiObjType::SysTaskCall:
#if 0
    put("vpiUserDefn", handle.get_bool(vpiUserDefn));
#else
#warning "TODO: SystemTaskCall の userDefn"
#endif
    put("vpiUserSystf", stmt->user_systf()->name() );
    put_argument_list("vpiArgument", mgr, stmt);
    break;

  case VpiObjType::Disable:
    put("vpiExpr", stmt->scope()->full_name() );
    break;

  case VpiObjType::DelayControl:
  case VpiObjType::EventControl:
    put_control("control", mgr, stmt->control() );
    put_stmt("vpiStmt", mgr, stmt->body_stmt() );
    break;

  default:
    ASSERT_NOT_REACHED;
  }
}

// @brief statement のリストの内容を出力する関数
void
VlDumperImpl::put_stmt_list(const char* label,
			    const VlMgr& mgr,
			    const vector<const VlStmt*>& stmt_list)
{
  VlDumpHeader x(this, label, "StmtList");

  for ( auto stmt: stmt_list ) {
    put_stmt(label, mgr, stmt);
  }
}

// @brief delay/event/repeat control の内容を出力する関数
void
VlDumperImpl::put_control(const char* label,
			  const VlMgr& mgr,
			  const VlControl* control)
{
  if ( control == nullptr ) {
    if ( !nullptr_suppress_mode() ) {
      VlDumpHeader x(this, label, "null-pointer");
    }
    return;
  }

  if ( control->delay() ) {
    VlDumpHeader x(this, label, "DelayControl");
    put("FileRegion", control->file_region() );
    put("vpiDelay", control->delay() );
  }
  else if ( control->expr() ) {
    VlDumpHeader x(this, label, "RepeatControl");
    put("FileRegion", control->file_region() );
    put_expr("vpiExpr", mgr, control->expr() );
    for (ymuint i = 0; i < control->event_num(); ++ i) {
      put_expr("vpiCondition", mgr, control->event(i) );
    }
  }
  else {
    VlDumpHeader x(this, label, "EventControl");
    put("FileRegion", control->file_region() );
    for (ymuint i = 0; i < control->event_num(); ++ i) {
      put_expr("vpiCondition", mgr, control->event(i) );
    }
  }
}

// @brief 子供のステートメントの内容を出力する関数
void
VlDumperImpl::put_child_stmt_list(const char* label,
				  const VlMgr& mgr,
				  const VlStmt* stmt)
{
  ymuint n = stmt->child_stmt_num();
  for (ymuint i = 0; i < n; ++ i) {
    put_stmt(label, mgr, stmt->child_stmt(i) );
  }
}

// @brief 引数のリストを出力する関数
void
VlDumperImpl::put_argument_list(const char* label,
				const VlMgr& mgr,
				const VlStmt* stmt)
{
  ymuint n = stmt->arg_num();
  for (ymuint i = 0; i < n; ++ i) {
    put_expr(label, mgr, stmt->arg(i) );
  }
}

END_NAMESPACE_YM_VERILOG
