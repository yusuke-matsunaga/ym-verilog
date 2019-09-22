﻿
/// @file StmtGen_main.cc
/// @brief ElbMgr の実装ファイル(statement の実体化)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "StmtGen.h"
#include "ElbEnv.h"

#include "ym/pt/PtStmt.h"
#include "ym/pt/PtExpr.h"
#include "ym/pt/PtMisc.h"

#include "ElbStmt.h"
#include "ElbTaskFunc.h"
#include "ElbScope.h"

#include "ElbStub.h"

#include "ym/MsgMgr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス StmtGen
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] elab 生成器
// @param[in] elb_mgr Elbオブジェクトを管理するクラス
// @param[in] elb_factory Elbオブジェクトを生成するファクトリクラス
StmtGen::StmtGen(Elaborator& elab,
		 ElbMgr& elb_mgr,
		 ElbFactory& elb_factory) :
  ElbProxy(elab, elb_mgr, elb_factory)
{
}

// @brief デストラクタ
StmtGen::~StmtGen()
{
}

// @brief スコープに関係するステートメントの実体化を行う．
// @param[in] parent 親のスコープ
// @param[in] pt_stmt 対象のステートメント
// 具体的には以下の処理を行う．
//
// 1. 内部にステートメントを持つステートメントは再帰する．
// 2. 自身がスコープとなるもの (named-begin, named-fork) はスコープ
//    を生成し，phase2 用のキューに登録す．
void
StmtGen::phase1_stmt(const VlNamedObj* parent,
		     const PtStmt* pt_stmt,
		     bool cf)
{
  ASSERT_COND(pt_stmt != nullptr );

  switch ( pt_stmt->type() ) {
  case kPtDisableStmt:
  case kPtEnableStmt:
  case kPtSysEnableStmt:
  case kPtAssignStmt:
  case kPtNbAssignStmt:
  case kPtEventStmt:
  case kPtNullStmt:
  case kPtPcAssignStmt:
  case kPtDeassignStmt:
  case kPtForceStmt:
  case kPtReleaseStmt:
    // 無視
    break;

  case kPtDcStmt:
  case kPtEcStmt:
  case kPtWaitStmt:
  case kPtForeverStmt:
  case kPtRepeatStmt:
  case kPtWhileStmt:
  case kPtForStmt:
    phase1_stmt(parent, pt_stmt->body());
    break;

  case kPtIfStmt:
    phase1_stmt(parent, pt_stmt->body());
    if ( pt_stmt->else_body() ) {
      phase1_stmt(parent, pt_stmt->else_body());
    }
    break;

  case kPtCaseStmt:
  case kPtCaseXStmt:
  case kPtCaseZStmt:
    for ( auto pt_item: pt_stmt->caseitem_list() ) {
      phase1_stmt(parent, pt_item->body());
    }
    break;

  case kPtParBlockStmt:
  case kPtSeqBlockStmt:
    for (ymuint i = 0; i < pt_stmt->stmt_array().size(); ++ i) {
      const PtStmt* pt_stmt1 = pt_stmt->stmt_array()[i];
      phase1_stmt(parent, pt_stmt1);
    }
    break;

  case kPtNamedParBlockStmt:
  case kPtNamedSeqBlockStmt:
    {
      ElbScope* block_scope = factory().new_StmtScope(parent, pt_stmt);
      reg_internalscope(block_scope);

      for (ymuint i = 0; i < pt_stmt->stmt_array().size(); ++ i) {
	const PtStmt* pt_stmt1 = pt_stmt->stmt_array()[i];
	phase1_stmt(block_scope, pt_stmt1);
      }
      if ( cf ) {
	phase2_namedblock(block_scope, pt_stmt->declhead_array());
      }
      else {
	add_phase2stub(make_stub(this, &StmtGen::phase2_namedblock,
				 static_cast<const VlNamedObj*>(block_scope),
				 pt_stmt->declhead_array()));
      }
    }
    break;

  default:
    ASSERT_NOT_REACHED;
  }
}

// @brief ステートメントの実体化を行う．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] env 生成時の環境
// @param[in] pt_stmt 対象のステートメント
ElbStmt*
StmtGen::instantiate_stmt(const VlNamedObj* parent,
			  ElbProcess* process,
			  const ElbEnv& env,
			  const PtStmt* pt_stmt)
{
  ASSERT_COND(pt_stmt != nullptr );

  ElbStmt* stmt = nullptr;
  switch ( pt_stmt->type() ) {
  case kPtDisableStmt:
    stmt = instantiate_disable(parent, process,
			       pt_stmt);
    break;

  case kPtEnableStmt:
    if ( env.inside_function() ) {
      goto error;
    }
    stmt = instantiate_enable(parent, process, env,
			      pt_stmt);
    break;

  case kPtSysEnableStmt:
    if ( env.inside_constant_function() ) {
      // 無視する．
      // といっても nullptr を返すとまずいので NULL_STMT を返す．
      stmt = instantiate_nullstmt(parent, process,
				  pt_stmt);
    }
    else {
      stmt = instantiate_sysenable(parent, process, env,
				   pt_stmt);
    }
    break;

  case kPtAssignStmt:
    stmt = instantiate_assign(parent, process, env,
			      pt_stmt, true);
    break;

  case kPtNbAssignStmt:
    ASSERT_COND(!env.inside_function() );
    stmt = instantiate_assign(parent, process, env,
			      pt_stmt, false);
    break;

  case kPtEventStmt:
    if ( env.inside_function() ) {
      goto error;
    }
    stmt = instantiate_eventstmt(parent, process,
				 pt_stmt);
    break;

  case kPtNullStmt:
    stmt = instantiate_nullstmt(parent, process,
				pt_stmt);
    break;

  case kPtPcAssignStmt:
    if ( env.inside_function() ) {
      goto error;
    }
    stmt = instantiate_pca(parent, process, env,
			   pt_stmt);
    break;

  case kPtDeassignStmt:
    if ( env.inside_function() ) {
      goto error;
    }
    stmt = instantiate_deassign(parent, process, env,
				pt_stmt);
    break;

  case kPtForceStmt:
    if ( env.inside_function() ) {
      goto error;
    }
    stmt = instantiate_force(parent, process, env,
			     pt_stmt);
    break;

  case kPtReleaseStmt:
    if ( env.inside_function() ) {
      goto error;
    }
    stmt = instantiate_release(parent, process, env,
			       pt_stmt);
    break;

  case kPtDcStmt:
  case kPtEcStmt:
    if ( env.inside_function() ) {
      goto error;
    }
    stmt = instantiate_ctrlstmt(parent, process, env,
				pt_stmt);
    break;

  case kPtWaitStmt:
    if ( env.inside_function() ) {
      goto error;
    }
    stmt = instantiate_wait(parent, process, env,
			    pt_stmt);
    break;

  case kPtForeverStmt:
    stmt = instantiate_forever(parent, process, env,
			       pt_stmt);
    break;

  case kPtRepeatStmt:
    stmt = instantiate_repeat(parent, process, env,
			      pt_stmt);
    break;

  case kPtWhileStmt:
    stmt = instantiate_while(parent, process, env,
			     pt_stmt);
    break;

  case kPtForStmt:
    stmt = instantiate_for(parent, process, env,
			   pt_stmt);
    break;

  case kPtIfStmt:
    stmt = instantiate_if(parent, process, env,
			  pt_stmt);
    break;

  case kPtCaseStmt:
  case kPtCaseXStmt:
  case kPtCaseZStmt:
    stmt = instantiate_case(parent, process, env,
			    pt_stmt);
    break;

  case kPtParBlockStmt:
    if ( env.inside_function() ) {
      goto error;
    }
    stmt = instantiate_parblock(parent, process, env,
				pt_stmt);
    break;

  case kPtSeqBlockStmt:
    stmt = instantiate_seqblock(parent, process, env,
				pt_stmt);
    break;

  case kPtNamedParBlockStmt:
    if ( env.inside_function() ) {
      goto error;
    }
    stmt = instantiate_namedparblock(parent, process, env,
				     pt_stmt);
    break;

  case kPtNamedSeqBlockStmt:
    stmt = instantiate_namedseqblock(parent, process, env,
				     pt_stmt);
    break;

  default:
    ASSERT_NOT_REACHED;
  }
  if ( stmt ) {
#if 0
    // attribute instance の生成
    instantiate_attribute(pt_stmt->attr_top(), false, stmt);
#else
#warning "TODO:2011-02-09-01"
#endif
  }

  return stmt;

 error:
  // 使えません．
  {
    ostringstream buf;
    buf << pt_stmt->stmt_name()
	<< " : cannot be used in a constant function.";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    pt_stmt->file_region(),
		    MsgType::Error,
		    "ELAB",
		    buf.str());
  }
  return nullptr;
}


//////////////////////////////////////////////////////////////////////
// PtStmt の派生クラスから ElbStmt を生成する関数
//////////////////////////////////////////////////////////////////////

// @brief disable statement の実体化を行う．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] pt_stmt 対象のステートメント
ElbStmt*
StmtGen::instantiate_disable(const VlNamedObj* parent,
			     ElbProcess* process,
			     const PtStmt* pt_stmt)
{
  const FileRegion& fr = pt_stmt->file_region();
  PtNameBranchArray nb_array = pt_stmt->namebranch_array();
  const char* name = pt_stmt->name();

  ElbObjHandle* handle = nullptr;

  // disable はモジュール境界を越えない？ 要チェック ##TODO##TODO##
  // 仕様書には何も書いていないのでたぶん越えられる．
  handle = find_obj_up(parent, nb_array, name, nullptr);
  if ( !handle ) {
    ostringstream buf;
    buf << expand_full_name(nb_array, name) << " : Not found.";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    fr,
		    MsgType::Error,
		    "ELAB",
		    buf.str());
    return nullptr;
  }

  tVpiObjType type = handle->type();
  if ( type != kVpiNamedBegin &&
       type != kVpiNamedFork &&
       type != kVpiTask ) {
    ostringstream buf;
    buf << handle->full_name()
	<< " : Not a named block, nor a task.";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    fr,
		    MsgType::Error,
		    "ELAB",
		    buf.str());
    return nullptr;
  }
  const VlNamedObj* scope = handle->obj();
  ElbStmt* stmt = factory().new_DisableStmt(parent, process, pt_stmt,
					    scope);

  return stmt;
}

// @brief enable の実体化を行う．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] env 生成時の環境
// @param[in] pt_stmt 対象のステートメント
ElbStmt*
StmtGen::instantiate_enable(const VlNamedObj* parent,
			    ElbProcess* process,
			    const ElbEnv& env,
			    const PtStmt* pt_stmt)
{
  const FileRegion& fr = pt_stmt->file_region();
  PtNameBranchArray nb_array = pt_stmt->namebranch_array();
  const char* name = pt_stmt->name();

  // タスクを探し出して設定する．
  // タスク名の探索はモジュール境界を越える．
  ElbObjHandle* cell = find_obj_up(parent, nb_array, name, nullptr);
  if ( !cell ) {
    ostringstream buf;
    buf << expand_full_name(nb_array, name) << " : Not found.";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    fr,
		    MsgType::Error,
		    "ELAB",
		    buf.str());
    return nullptr;
  }
  if ( cell->type() != kVpiTask ) {
    ostringstream buf;
    buf << expand_full_name(nb_array, name) << " : Not a task.";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    fr,
		    MsgType::Error,
		    "ELAB",
		    buf.str());
  }
  ElbTaskFunc* task = cell->taskfunc();
  ASSERT_COND( task != nullptr );

  // 引数を生成する．
  ElbExpr** arg_list = factory().new_ExprList(pt_stmt->arg_list().size());
  SizeType wpos = 0;
  for ( auto pt_expr: pt_stmt->arg_list() ) {
    ElbExpr* expr = instantiate_expr(parent, env, pt_expr);
    if ( !expr ) {
      // エラーが起った．
      return nullptr;
    }
    arg_list[wpos] = expr;
    ++ wpos;
  }

  // task call ステートメントの生成
  ElbStmt* stmt = factory().new_TaskCall(parent, process, pt_stmt,
					 task, arg_list);

  return stmt;
}

// @brief system enable 文の実体化を行う．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] env 生成時の環境
// @param[in] pt_stmt 対象のステートメント
ElbStmt*
StmtGen::instantiate_sysenable(const VlNamedObj* parent,
			       ElbProcess* process,
			       const ElbEnv& env,
			       const PtStmt* pt_stmt)
{
  const FileRegion& fr = pt_stmt->file_region();

  const char* name = pt_stmt->name();

  // UserSystf を取り出す．
  const ElbUserSystf* user_systf = find_user_systf(name);
  if ( user_systf == nullptr ) {
    ostringstream buf;
    buf << name << " : No such system task.";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    fr,
		    MsgType::Error,
		    "ELAB",
		    buf.str());
    return nullptr;
  }

  // 引数を生成する．
  ElbExpr** arg_list = factory().new_ExprList(pt_stmt->arg_list().size());
  SizeType wpos = 0;
  for ( auto pt_expr: pt_stmt->arg_list() ) {
    ElbExpr* arg = nullptr;
    // 空の引数があるのでエラーと区別する．
    if ( pt_expr ) {
      arg = instantiate_arg(parent, env, pt_expr);
      if ( !arg ) {
	// エラーが起こった
	return nullptr;
      }
    }
    arg_list[wpos] = arg;
    ++ wpos;
  }

  // system task call ステートメントの生成
  ElbStmt* stmt = factory().new_SysTaskCall(parent, process, pt_stmt,
					    user_systf, arg_list);

  return stmt;
}

// @brief delay / event control statement の実体化を行う．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] env 生成時の環境
// @param[in] pt_stmt 対象のステートメント
ElbStmt*
StmtGen::instantiate_ctrlstmt(const VlNamedObj* parent,
			      ElbProcess* process,
			      const ElbEnv& env,
			      const PtStmt* pt_stmt)
{
  ElbStmt* body = instantiate_stmt(parent, process, env,
				   pt_stmt->body());
  ElbControl* control = instantiate_control(parent, env, pt_stmt->control());
  if ( !body || !control ) {
    return nullptr;
  }

  // delay / event control ステートメントの生成
  ElbStmt* stmt = factory().new_CtrlStmt(parent, process, pt_stmt,
					 control, body);

  return stmt;
}

// @brief コントロールを生成する．
// @param[in] parent 親のスコープ
// @param[in] env 生成時の環境
// @param[in] pt_control パース木のコントロール定義
ElbControl*
StmtGen::instantiate_control(const VlNamedObj* parent,
			     const ElbEnv& env,
			     const PtControl* pt_control)
{
  if ( pt_control->type() == kPtDelayControl ) {
    ElbExpr* delay = instantiate_expr(parent, env, pt_control->delay());
    if ( delay ) {
      return factory().new_DelayControl(pt_control, delay);
    }
    return nullptr;
  }

  // イベントリストの生成を行う．
  SizeType event_num = pt_control->event_list().size();
  ElbExpr** event_list = factory().new_ExprList(event_num);
  for ( SizeType i = 0; i < event_num; ++ i ) {
    const PtExpr* pt_expr = pt_control->event_list()[i];
    ElbExpr* expr = instantiate_event_expr(parent, env, pt_expr);
    if ( !expr ) {
      return nullptr;
    }
    event_list[i] = expr;
  }

  if ( pt_control->type() == kPtEventControl ) {
    return factory().new_EventControl(pt_control, event_num, event_list);
  }

  ElbExpr* rep = instantiate_expr(parent, env, pt_control->rep_expr());
  if ( !rep ) {
    return nullptr;
  }
  return factory().new_RepeatControl(pt_control, rep, event_num, event_list);
}

// @brief event statement の実体化を行う．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] pt_stmt 対象のステートメント
ElbStmt*
StmtGen::instantiate_eventstmt(const VlNamedObj* parent,
			       ElbProcess* process,
			       const PtStmt* pt_stmt)
{
  const PtExpr* pt_expr = pt_stmt->primary();
  ElbExpr* named_event = instantiate_namedevent(parent, pt_expr);
  if ( !named_event ) {
    return nullptr;
  }

  ElbStmt* stmt = factory().new_EventStmt(parent, process, pt_stmt,
					  named_event);

  return stmt;
}

// @brief null statement の実体化を行う．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] pt_stmt 対象のステートメント
ElbStmt*
StmtGen::instantiate_nullstmt(const VlNamedObj* parent,
			      ElbProcess* process,
			      const PtStmt* pt_stmt)
{
  ElbStmt* stmt = factory().new_NullStmt(parent, process, pt_stmt);

  return stmt;
}

END_NAMESPACE_YM_VERILOG
