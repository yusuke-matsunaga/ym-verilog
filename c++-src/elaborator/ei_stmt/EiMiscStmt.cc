
/// @file EiMiscStmt.cc
/// @brief EiMiscStmt の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.


#include "ei/EiFactory.h"
#include "ei/EiMiscStmt.h"

#include "elaborator/ElbDecl.h"
#include "elaborator/ElbExpr.h"

#include "ym/vl/VlUserSystf.h"
#include "ym/vl/VlTaskFunc.h"
#include "ym/vl/VlControl.h"

#include "ym/pt/PtStmt.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief イベント文を生成する．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] named_event 対象のイベント
const VlStmt*
EiFactory::new_EventStmt(const VlScope* parent,
			 const VlProcess* process,
			 const PtStmt* pt_stmt,
			 ElbExpr* named_event)
{
  auto stmt = new EiEventStmt(parent, process, pt_stmt,
			      named_event);

  return stmt;
}

// @brief nullptr ステートメントを生成する．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] pt_stmt パース木のステートメント定義
const VlStmt*
EiFactory::new_NullStmt(const VlScope* parent,
			const VlProcess* process,
			const PtStmt* pt_stmt)
{
  auto stmt = new EiNullStmt(parent, process, pt_stmt);

  return stmt;
}

// @brief タスクコール文を生成する．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] task 対象のタスク
// @param[in] arg_array 引数の配列
const VlStmt*
EiFactory::new_TaskCall(const VlScope* parent,
			const VlProcess* process,
			const PtStmt* pt_stmt,
			const VlTaskFunc* task,
			const vector<ElbExpr*>& arg_array)
{
  auto stmt = new EiTaskCall(parent, process, pt_stmt,
			     task, arg_array);

  return stmt;
}

// @brief システムタスクコール文を生成する．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] user_systf システムタスク
// @param[in] arg_array 引数の配列
const VlStmt*
EiFactory::new_SysTaskCall(const VlScope* parent,
			   const VlProcess* process,
			   const PtStmt* pt_stmt,
			   const VlUserSystf* user_systf,
			   const vector<ElbExpr*>& arg_array)
{
  auto stmt = new EiSysTaskCall(parent, process, pt_stmt,
				user_systf, arg_array);

  return stmt;
}

// @brief disable 文を生成する．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] target 対象のスコープ
const VlStmt*
EiFactory::new_DisableStmt(const VlScope* parent,
			   const VlProcess* process,
			   const PtStmt* pt_stmt,
			   const VlScope* target)
{
  auto stmt = new EiDisableStmt(parent, process, pt_stmt,
				target);

  return stmt;
}

// @brief コントロール文を生成する．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] control コントロール
// @param[in] stmt 本体のステートメント
const VlStmt*
EiFactory::new_CtrlStmt(const VlScope* parent,
			const VlProcess* process,
			const PtStmt* pt_stmt,
			const VlControl* control,
			const VlStmt* stmt)
{
  auto stmt1 = new EiCtrlStmt(parent, process, pt_stmt,
			      control, stmt);

  return stmt1;
}


//////////////////////////////////////////////////////////////////////
// クラス EiEventStmt
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] named_event 対象のイベント
EiEventStmt::EiEventStmt(const VlScope* parent,
			 const VlProcess* process,
			 const PtStmt* pt_stmt,
			 ElbExpr* named_event) :
  EiStmtBase(parent, process, pt_stmt),
  mEvent{named_event}
{
}

// @brief デストラクタ
EiEventStmt::~EiEventStmt()
{
}

// @brief 型の取得
VpiObjType
EiEventStmt::type() const
{
  return VpiObjType::EventStmt;
}

// @brief named event を返す．
const VlExpr*
EiEventStmt::named_event() const
{
  return mEvent;
}


//////////////////////////////////////////////////////////////////////
// クラス EiNullStmt
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] pt_stmt パース木のステートメント定義
EiNullStmt::EiNullStmt(const VlScope* parent,
		       const VlProcess* process,
		       const PtStmt* pt_stmt) :
  EiStmtBase(parent, process, pt_stmt)
{
}

// @brief デストラクタ
EiNullStmt::~EiNullStmt()
{
}

// @brief 型の取得
VpiObjType
EiNullStmt::type() const
{
  return VpiObjType::NullStmt;
}


//////////////////////////////////////////////////////////////////////
// クラス EiTcBase
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] arg_num 引数の数
// @param[in] arg_array 引数を納める配列
EiTcBase::EiTcBase(const VlScope* parent,
		   const VlProcess* process,
		   const PtStmt* pt_stmt,
		   const vector<ElbExpr*>& arg_array) :
  EiStmtBase(parent, process, pt_stmt),
  mArgumentList{arg_array}
{
}

// @brief デストラクタ
EiTcBase::~EiTcBase()
{
}

// @brief 引数の数を返す．
SizeType
EiTcBase::arg_num() const
{
  return mArgumentList.size();
}

// @brief 引数の取得
const VlExpr*
EiTcBase::arg(SizeType pos) const
{
  ASSERT_COND( 0 <= pos && pos < arg_num() );
  return mArgumentList[pos];
}


//////////////////////////////////////////////////////////////////////
// クラス EiTaskCall
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] task 対象のタスク
// @param[in] arg_num 引数の数
// @param[in] arg_array 引数を納める配列
EiTaskCall::EiTaskCall(const VlScope* parent,
		       const VlProcess* process,
		       const PtStmt* pt_stmt,
		       const VlTaskFunc* task,
		       const vector<ElbExpr*>& arg_array) :
  EiTcBase(parent, process, pt_stmt, arg_array),
  mTask{task}
{
}

// @brief デストラクタ
EiTaskCall::~EiTaskCall()
{
}

// @brief 型の取得
VpiObjType
EiTaskCall::type() const
{
  return VpiObjType::TaskCall;
}

// @brief task の実体を返す．
const VlTaskFunc*
EiTaskCall::task() const
{
  return mTask;
}


//////////////////////////////////////////////////////////////////////
// クラス SysTaskCall
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] user_systf システムタスク
// @param[in] arg_array 引数を納める配列
EiSysTaskCall::EiSysTaskCall(const VlScope* parent,
			     const VlProcess* process,
			     const PtStmt* pt_stmt,
			     const VlUserSystf* user_systf,
			     const vector<ElbExpr*>& arg_array) :
  EiTcBase(parent, process, pt_stmt, arg_array),
  mUserSystf(user_systf)
{
}

// @brief デストラクタ
EiSysTaskCall::~EiSysTaskCall()
{
}

// @brief 型の取得
VpiObjType
EiSysTaskCall::type() const
{
  return VpiObjType::SysTaskCall;
}

// @brief user systf クラスへのポインタを返す．
const VlUserSystf*
EiSysTaskCall::user_systf() const
{
  return mUserSystf;
}


//////////////////////////////////////////////////////////////////////
// クラス EiDisableStmt
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] target 対象のスコープ
EiDisableStmt::EiDisableStmt(const VlScope* parent,
			     const VlProcess* process,
			     const PtStmt* pt_stmt,
			     const VlScope* target) :
  EiStmtBase(parent, process, pt_stmt),
  mExpr{target}
{
}

// @brief デストラクタ
EiDisableStmt::~EiDisableStmt()
{
}

// @brief 型の取得
VpiObjType
EiDisableStmt::type() const
{
  return VpiObjType::Disable;
}

// @brief expr を返す．
//
// 型が VlScope になっているが実際には
// ElbFunction/ElbTask/EiNamedFork/EiNamedBegin
// のいずれか
const VlScope*
EiDisableStmt::target_scope() const
{
  return mExpr;
}


//////////////////////////////////////////////////////////////////////
// クラス EiCtrlStmt
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] control コントロール
// @param[in] stmt 本体のステートメント
EiCtrlStmt::EiCtrlStmt(const VlScope* parent,
		       const VlProcess* process,
		       const PtStmt* pt_stmt,
		       const VlControl* control,
		       const VlStmt* stmt) :
  EiStmtBase(parent, process, pt_stmt),
  mControl{control},
  mBodyStmt{stmt}
{
}

// @brief デストラクタ
EiCtrlStmt::~EiCtrlStmt()
{
}

// @brief 型の取得
VpiObjType
EiCtrlStmt::type() const
{
  return mControl->type();
}

// @brief コントロールを返す．
const VlControl*
EiCtrlStmt::control() const
{
  return mControl;
}

// @brief 本文のステートメントを返す．
const VlStmt*
EiCtrlStmt::body_stmt() const
{
  return mBodyStmt;
}

END_NAMESPACE_YM_VERILOG
