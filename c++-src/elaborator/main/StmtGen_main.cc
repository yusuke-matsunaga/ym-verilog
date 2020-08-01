
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

#include "elb/ElbStmt.h"
#include "elb/ElbTaskFunc.h"

#include "elb/ElbStub.h"

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
  case PtStmtType::Disable:
  case PtStmtType::Enable:
  case PtStmtType::SysEnable:
  case PtStmtType::Assign:
  case PtStmtType::NbAssign:
  case PtStmtType::Event:
  case PtStmtType::Null:
  case PtStmtType::PcAssign:
  case PtStmtType::Deassign:
  case PtStmtType::Force:
  case PtStmtType::Release:
    // 無視
    break;

  case PtStmtType::DelayControl:
  case PtStmtType::EventControl:
  case PtStmtType::Wait:
  case PtStmtType::Forever:
  case PtStmtType::Repeat:
  case PtStmtType::White:
  case PtStmtType::For:
    phase1_stmt(parent, pt_stmt->body());
    break;

  case PtStmtType::If:
    phase1_stmt(parent, pt_stmt->body());
    if ( pt_stmt->else_body() ) {
      phase1_stmt(parent, pt_stmt->else_body());
    }
    break;

  case PtStmtType::Case:
  case PtStmtType::CaseX:
  case PtStmtType::CaseZ:
    for ( auto pt_item: pt_stmt->caseitem_list() ) {
      phase1_stmt(parent, pt_item->body());
    }
    break;

  case PtStmtType::ParBlock:
  case PtStmtType::SeqBlock:
    for ( auto pt_stmt1: pt_stmt->stmt_list() ) {
      phase1_stmt(parent, pt_stmt1);
    }
    break;

  case PtStmtType::NamedParBlock:
  case PtStmtType::NamedSeqBlock:
    {
      const VlNamedObj* block_scope = factory().new_StmtScope(parent, pt_stmt);
      reg_internalscope(block_scope);

      for ( auto pt_stmt1: pt_stmt->stmt_list() ) {
	phase1_stmt(block_scope, pt_stmt1);
      }
      if ( cf ) {
	phase2_namedblock(block_scope, pt_stmt->declhead_list());
      }
      else {
	auto stub{make_stub<StmtGen,
		  const VlNamedObj*,
		  const vector<const PtDeclHead*>&>(this,
						    &StmtGen::phase2_namedblock,
						    static_cast<const VlNamedObj*>(block_scope),
						    pt_stmt->declhead_list())};
	add_phase2stub(stub);
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
  case PtStmtType::Disable:
    stmt = instantiate_disable(parent, process,
			       pt_stmt);
    break;

  case PtStmtType::Enable:
    if ( env.inside_function() ) {
      goto error;
    }
    stmt = instantiate_enable(parent, process, env,
			      pt_stmt);
    break;

  case PtStmtType::SysEnable:
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

  case PtStmtType::Assign:
    stmt = instantiate_assign(parent, process, env,
			      pt_stmt, true);
    break;

  case PtStmtType::NbAssign:
    ASSERT_COND(!env.inside_function() );
    stmt = instantiate_assign(parent, process, env,
			      pt_stmt, false);
    break;

  case PtStmtType::Event:
    if ( env.inside_function() ) {
      goto error;
    }
    stmt = instantiate_eventstmt(parent, process,
				 pt_stmt);
    break;

  case PtStmtType::Null:
    stmt = instantiate_nullstmt(parent, process,
				pt_stmt);
    break;

  case PtStmtType::PcAssign:
    if ( env.inside_function() ) {
      goto error;
    }
    stmt = instantiate_pca(parent, process, env,
			   pt_stmt);
    break;

  case PtStmtType::Deassign:
    if ( env.inside_function() ) {
      goto error;
    }
    stmt = instantiate_deassign(parent, process, env,
				pt_stmt);
    break;

  case PtStmtType::Force:
    if ( env.inside_function() ) {
      goto error;
    }
    stmt = instantiate_force(parent, process, env,
			     pt_stmt);
    break;

  case PtStmtType::Release:
    if ( env.inside_function() ) {
      goto error;
    }
    stmt = instantiate_release(parent, process, env,
			       pt_stmt);
    break;

  case PtStmtType::DelayControl:
  case PtStmtType::EventControl:
    if ( env.inside_function() ) {
      goto error;
    }
    stmt = instantiate_ctrlstmt(parent, process, env,
				pt_stmt);
    break;

  case PtStmtType::Wait:
    if ( env.inside_function() ) {
      goto error;
    }
    stmt = instantiate_wait(parent, process, env,
			    pt_stmt);
    break;

  case PtStmtType::Forever:
    stmt = instantiate_forever(parent, process, env,
			       pt_stmt);
    break;

  case PtStmtType::Repeat:
    stmt = instantiate_repeat(parent, process, env,
			      pt_stmt);
    break;

  case PtStmtType::White:
    stmt = instantiate_while(parent, process, env,
			     pt_stmt);
    break;

  case PtStmtType::For:
    stmt = instantiate_for(parent, process, env,
			   pt_stmt);
    break;

  case PtStmtType::If:
    stmt = instantiate_if(parent, process, env,
			  pt_stmt);
    break;

  case PtStmtType::Case:
  case PtStmtType::CaseX:
  case PtStmtType::CaseZ:
    stmt = instantiate_case(parent, process, env,
			    pt_stmt);
    break;

  case PtStmtType::ParBlock:
    if ( env.inside_function() ) {
      goto error;
    }
    stmt = instantiate_parblock(parent, process, env,
				pt_stmt);
    break;

  case PtStmtType::SeqBlock:
    stmt = instantiate_seqblock(parent, process, env,
				pt_stmt);
    break;

  case PtStmtType::NamedParBlock:
    if ( env.inside_function() ) {
      goto error;
    }
    stmt = instantiate_namedparblock(parent, process, env,
				     pt_stmt);
    break;

  case PtStmtType::NamedSeqBlock:
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

  ObjHandle* handle = nullptr;

  // disable はモジュール境界を越えない？ 要チェック ##TODO##TODO##
  // 仕様書には何も書いていないのでたぶん越えられる．
  handle = find_obj_up(parent, pt_stmt, nullptr);
  if ( !handle ) {
    ostringstream buf;
    buf << pt_stmt->fullname() << " : Not found.";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    fr,
		    MsgType::Error,
		    "ELAB",
		    buf.str());
    return nullptr;
  }

  VpiObjType type = handle->type();
  if ( type != VpiObjType::NamedBegin &&
       type != VpiObjType::NamedFork &&
       type != VpiObjType::Task ) {
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

  // タスクを探し出して設定する．
  // タスク名の探索はモジュール境界を越える．
  ObjHandle* cell = find_obj_up(parent, pt_stmt, nullptr);
  if ( !cell ) {
    ostringstream buf;
    buf << pt_stmt->fullname() << " : Not found.";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    fr,
		    MsgType::Error,
		    "ELAB",
		    buf.str());
    return nullptr;
  }
  if ( cell->type() != VpiObjType::Task ) {
    ostringstream buf;
    buf << pt_stmt->fullname() << " : Not a task.";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    fr,
		    MsgType::Error,
		    "ELAB",
		    buf.str());
  }
  ElbTaskFunc* task = cell->taskfunc();
  ASSERT_COND( task != nullptr );

  // 引数を生成する．
  ElbExpr** arg_list = factory().new_ExprList(pt_stmt->arg_num());
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
  ElbExpr** arg_list = factory().new_ExprList(pt_stmt->arg_num());
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
  if ( pt_control->type() == PtCtrlType::Delay ) {
    ElbExpr* delay = instantiate_expr(parent, env, pt_control->delay());
    if ( delay ) {
      return factory().new_DelayControl(pt_control, delay);
    }
    return nullptr;
  }

  // イベントリストの生成を行う．
  SizeType event_num = pt_control->event_num();
  ElbExpr** event_list = factory().new_ExprList(event_num);
  for ( SizeType i = 0; i < event_num; ++ i ) {
    const PtExpr* pt_expr = pt_control->event(i);
    ElbExpr* expr = instantiate_event_expr(parent, env, pt_expr);
    if ( !expr ) {
      return nullptr;
    }
    event_list[i] = expr;
  }

  if ( pt_control->type() == PtCtrlType::Event ) {
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
