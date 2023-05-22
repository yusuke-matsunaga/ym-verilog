
/// @file Elaborator.cc
/// @brief Elaborator の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.


#include "elaborator/Elaborator.h"

#include "UdpGen.h"
#include "ModuleGen.h"
#include "DeclGen.h"
#include "ItemGen.h"
#include "StmtGen.h"
#include "ExprGen.h"
#include "ExprEval.h"
#include "AttrGen.h"
#include "DefParamStub.h"
#include "ElbStub.h"

#include "ym/pt/PtModule.h"
#include "ym/pt/PtItem.h"
#include "ym/pt/PtMisc.h"
#include "ym/vl/VlTaskFunc.h"

#include "ym/ClibCellLibrary.h"

#include "parser/PtMgr.h"

#include "elaborator/ElbModule.h"
#include "elaborator/ElbModuleArray.h"
#include "elaborator/ElbGfRoot.h"
#include "elaborator/ElbDecl.h"
#include "elaborator/ElbParameter.h"
#include "elaborator/ElbPrimitive.h"
#include "elaborator/ElbProcess.h"
#include "elaborator/ElbGenvar.h"

#include "ym/MsgMgr.h"


BEGIN_NAMESPACE_YM_VERILOG

#define DOUT cerr

const ymuint debug_none       = 0x00000000;
const ymuint debug_objdict    = 0x00000001;
const ymuint debug_find_scope = 0x00000010;
const ymuint debug_all        = 0xFFFFFFFF;

#if 1
const ymuint debug = debug_none;
#else
const ymuint debug = debug_all;
#endif


// @brief トップレベルのスコープを生成する
const VlScope*
Elaborator::new_Toplevel()
{
  auto toplevel{mMgr.new_Toplevel()};
  mMgr.reg_toplevel(toplevel);
  return toplevel;
}

// @brief module を生成する．
// @param[in] parent 親のスコープ
// @param[in] pt_module モジュールテンプレート
// @param[in] pt_head パース木のヘッダ定義
// @param[in] pt_inst パース木のインスタンス定義
// @return 生成したオブジェクトを返す．
ElbModule*
Elaborator::new_Module(const VlScope* parent,
		       const PtModule* pt_module,
		       const PtItem* pt_head,
		       const PtInst* pt_inst)
{
  auto obj{mMgr.new_Module(parent, pt_module, pt_head, pt_inst)};
  if ( debug & debug_objdict ) {
    DOUT << "reg_module( " << obj->name() << " @ "
	 << parent->full_name()
	 << " ["
	 << hex << reinterpret_cast<PtrIntType>(parent) << dec
	 << "] )" << endl << endl;
  }
  mObjDict.add(obj);
  mModuleDefDict.add(obj);
  return obj;
}

// @brief module array を生成する．
// @param[in] parent 親のスコープ
// @param[in] pt_module モジュールテンプレート
// @param[in] pt_head パース木のヘッダ定義
// @param[in] pt_inst パース木のインスタンス定義
// @param[in] left 範囲の MSB の式
// @param[in] right 範囲の LSB の式
// @param[in] left_val 範囲の MSB の値
// @param[in] right_val 範囲の LSB の値
// @return 生成したオブジェクトを返す．
ElbModuleArray*
Elaborator::new_ModuleArray(const VlScope* parent,
			    const PtModule* pt_module,
			    const PtItem* pt_head,
			    const PtInst* pt_inst,
			    const PtExpr* left,
			    const PtExpr* right,
			    int left_val,
			    int right_val)
{
  auto obj{mMgr.new_ModuleArray(parent, pt_module, pt_head, pt_inst,
				left, right, left_val, right_val)};
  if ( debug & debug_objdict ) {
    DOUT << "reg_modulearray( " << obj->name() << " @ "
	 << parent->full_name()
	 << " ["
	 << hex << reinterpret_cast<PtrIntType>(parent) << dec
	 << "] )" << endl << endl;
  }
  mObjDict.add(obj);
  return obj;
}

// @brief continuous assignment のヘッダを生成する．
// @param[in] module 親のモジュール
// @param[in] pt_head パース木のヘッダ定義
// @param[in] delay 遅延値
// @note 遅延を持たないときは nullptr を与える．
ElbCaHead*
Elaborator::new_CaHead(const VlModule* module,
		       const PtItem* pt_head,
		       const VlDelay* delay)
{
  auto cahead{mMgr.new_CaHead(module, pt_head, delay)};
  return cahead;
}

// @brief continuous assignment を生成する．
// @param[in] head ヘッダ
// @param[in] pt_obj 対応するパース木中の定義要素
// @param[in] lhs 左辺式
// @param[in] rhs 右辺式
const VlContAssign*
Elaborator::new_ContAssign(ElbCaHead* head,
			   const PtBase* pt_obj,
			   const VlExpr* lhs,
			   const VlExpr* rhs)
{
  auto ca{mMgr.new_ContAssign(head, pt_obj, lhs, rhs)};
  return ca;
}

// @brief net 宣言中の continuous assignment を生成する．
// @param[in] module 親のモジュール
// @param[in] pt_obj パース木の定義要素
// @param[in] lhs 左辺式
// @param[in] rhs 右辺式
const VlContAssign*
Elaborator::new_ContAssign(const VlModule* module,
			   const PtBase* pt_obj,
			   const VlExpr* lhs,
			   const VlExpr* rhs)
{
  auto ca{mMgr.new_ContAssign(module, pt_obj, lhs, rhs)};
  return ca;
}

// @brief パラメータ割り当て文を生成する．
// @param[in] module 親のモジュール
// @param[in] param 対象の parameter
// @param[in] rhs_expr 割り当て式の右辺
// @param[in] rhs_value 右辺の値
const VlParamAssign*
Elaborator::new_ParamAssign(const VlModule* module,
			    const PtBase* pt_obj,
			    ElbParameter* param,
			    const PtExpr* rhs_expr,
			    const VlValue& rhs_value)
{
  auto pa{mMgr.new_ParamAssign(module, pt_obj, param,
			       rhs_expr, rhs_value)};
  return pa;
}

// @brief 名前によるパラメータ割り当て文を生成する．
// @param[in] module 親のモジュール
// @param[in] param 対象の parameter
// @param[in] rhs_expr 割り当て式の右辺
// @param[in] rhs_value 右辺の値
const VlParamAssign*
Elaborator::new_NamedParamAssign(const VlModule* module,
				 const PtBase* pt_obj,
				 ElbParameter* param,
				 const PtExpr* rhs_expr,
				 const VlValue& rhs_value)
{
  auto pa{mMgr.new_NamedParamAssign(module, pt_obj, param,
				    rhs_expr, rhs_value)};
  return pa;
}

// @brief defparam 文を生成する．
// @param[in] module 親のモジュール
// @param[in] pt_header パース木の defparam ヘッダ
// @param[in] pt_item パース木の defparam 定義
// @param[in] param 対象の parameter
// @param[in] rhs_expr 割り当て式の右辺式
// @param[in] rhs_value 右辺の値
const VlDefParam*
Elaborator::new_DefParam(const VlModule* module,
			 const PtItem* pt_header,
			 const PtDefParam* pt_defparam,
			 ElbParameter* param,
			 const PtExpr* rhs_expr,
			 const VlValue& rhs_value)
{
  auto defparam{mMgr.new_DefParam(module, pt_header, pt_defparam,
				  param, rhs_expr, rhs_value)};
  return defparam;
}

// @brief ゲートプリミティブのヘッダを生成する．
// @param[in] parent 親のスコープ
// @param[in] pt_header パース木の定義
// @param[in] has_delay 遅延値を持つとき true
ElbPrimHead*
Elaborator::new_PrimHead(const VlScope* parent,
			 const PtItem* pt_header,
			 bool has_delay)
{
  auto phead{mMgr.new_PrimHead(parent, pt_header, has_delay)};
  return phead;
}

// @brief UDPプリミティブのヘッダを生成する．
// @param[in] parent 親のスコープ
// @param[in] pt_header パース木の定義
// @param[in] udp UDP 定義
// @param[in] has_delay 遅延値を持つとき true
ElbPrimHead*
Elaborator::new_UdpHead(const VlScope* parent,
			const PtItem* pt_header,
			const VlUdpDefn* udp,
			bool has_delay)
{
  auto uhead{mMgr.new_UdpHead(parent, pt_header, udp, has_delay)};
  return uhead;
}

// @brief セルプリミティブのヘッダを生成する．
// @param[in] parent 親のスコープ
// @param[in] pt_header パース木の定義
// @param[in] cell_id セル番号
ElbPrimHead*
Elaborator::new_CellHead(const VlScope* parent,
			 const PtItem* pt_header,
			 int cell_id)
{
  auto chead{mMgr.new_CellHead(parent, pt_header, cell_id)};
  return chead;
}

// @brief プリミティブインスタンスを生成する．
// @param[in] head ヘッダ
// @param[in] pt_inst インスタンス定義
ElbPrimitive*
Elaborator::new_Primitive(ElbPrimHead* head,
			  const PtInst* pt_inst)
{
  auto prim{mMgr.new_Primitive(head, pt_inst)};
  return prim;
}

// @brief プリミティブ配列インスタンスを生成する．
// @param[in] head ヘッダ
// @param[in] pt_inst インスタンス定義
// @param[in] left 範囲の MSB の式
// @param[in] right 範囲の LSB の式
// @param[in] left_val 範囲の MSB の値
// @param[in] right_val 範囲の LSB の値
ElbPrimArray*
Elaborator::new_PrimitiveArray(ElbPrimHead* head,
			       const PtInst* pt_inst,
			       const PtExpr* left,
			       const PtExpr* right,
			       int left_val,
			       int right_val)
{
  auto primarray{mMgr.new_PrimitiveArray(head, pt_inst,
					 left, right,
					 left_val, right_val)};
  return primarray;
}

// @brief セルプリミティブインスタンスを生成する．
// @param[in] head ヘッダ
// @param[in] cell セル
// @param[in] pt_inst インスタンス定義
ElbPrimitive*
Elaborator::new_CellPrimitive(ElbPrimHead* head,
			      const ClibCell& cell,
			      const PtInst* pt_inst)
{
  auto prim{mMgr.new_CellPrimitive(head, cell, pt_inst)};
  return prim;
}

// @brief セルプリミティブ配列インスタンスを生成する．
// @param[in] head ヘッダ
// @param[in] cell セル
// @param[in] pt_inst インスタンス定義
// @param[in] left 範囲の MSB の式
// @param[in] right 範囲の LSB の式
// @param[in] left_val 範囲の MSB の値
// @param[in] right_val 範囲の LSB の値
ElbPrimArray*
Elaborator::new_CellPrimitiveArray(ElbPrimHead* head,
				   const ClibCell& cell,
				   const PtInst* pt_inst,
				   const PtExpr* left,
				   const PtExpr* right,
				   int left_val,
				   int right_val)
{
  auto primarray{mMgr.new_CellPrimitiveArray(head, cell, pt_inst,
					     left, right,
					     left_val, right_val)};
  return primarray;
}

// @brief function を生成する．
// @param[in] parent 親のスコープ
// @param[in] pt_item パース木の定義
// @param[in] const_func 定数関数フラグ
ElbTaskFunc*
Elaborator::new_Function(const VlScope* parent,
			 const PtItem* pt_item,
			 bool const_func)
{
  auto func{mMgr.new_Function(parent, pt_item, const_func)};
  return func;
}

// @brief function を生成する．
// @param[in] parent 親のスコープ
// @param[in] pt_item パース木の定義
// @param[in] left 範囲の MSB の式
// @param[in] right 範囲の LSB の式
// @param[in] left_val 範囲の MSB の値
// @param[in] right_val 範囲の LSB の値
// @param[in] const_func 定数関数フラグ
ElbTaskFunc*
Elaborator::new_Function(const VlScope* parent,
			 const PtItem* pt_item,
			 const PtExpr* left,
			 const PtExpr* right,
			 int left_val,
			 int right_val,
			 bool const_func)
{
  auto func{mMgr.new_Function(parent, pt_item,
			      left, right,
			      left_val, right_val,
			      const_func)};
  return func;
}

// @brief task を生成する．
// @param[in] parent 親のスコープ
// @param[in] pt_item パース木の定義
ElbTaskFunc*
Elaborator::new_Task(const VlScope* parent,
		     const PtItem* pt_item)
{
  auto task{mMgr.new_Task(parent, pt_item)};
  return task;
}

// @brief プロセス文を生成する．
// @param[in] parent 親のスコープ
// @param[in] pt_item パース木の要素定義
ElbProcess*
Elaborator::new_Process(const VlScope* parent,
			const PtItem* pt_item)
{
  auto process{mMgr.new_Process(parent, pt_item)};
  return process;
}

// @brief 代入文を生成する．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] lhs 左辺の式
// @param[in] rhs 右辺の式
// @param[in] block ブロッキング代入の時 true
// @param[in] control コントロール
const VlStmt*
Elaborator::new_Assignment(const VlScope* parent,
			   const VlProcess* process,
			   const PtStmt* pt_stmt,
			   const VlExpr* lhs,
			   const VlExpr* rhs,
			   bool block,
			   const VlControl* control)
{
  auto stmt{mMgr.new_Assignment(parent, process, pt_stmt,
				lhs, rhs, block, control)};
  return stmt;
}

// @brief assign ステートメントを生成する．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] lhs 左辺の式
// @param[in] rhs 右辺の式
const VlStmt*
Elaborator::new_AssignStmt(const VlScope* parent,
			   const VlProcess* process,
			   const PtStmt* pt_stmt,
			   const VlExpr* lhs,
			   const VlExpr* rhs)
{
  auto stmt{mMgr.new_AssignStmt(parent, process, pt_stmt, lhs, rhs)};
  return stmt;
}

// @brief deassign ステートメントを生成する．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] lhs 左辺の式
const VlStmt*
Elaborator::new_DeassignStmt(const VlScope* parent,
			     const VlProcess* process,
			     const PtStmt* pt_stmt,
			     const VlExpr* lhs)
{
  auto stmt{mMgr.new_DeassignStmt(parent, process, pt_stmt, lhs)};
  return stmt;
}

// @brief force ステートメントを生成する．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] lhs 左辺の式
// @param[in] rhs 右辺の式
const VlStmt*
Elaborator::new_ForceStmt(const VlScope* parent,
			  const VlProcess* process,
			  const PtStmt* pt_stmt,
			  const VlExpr* lhs,
			  const VlExpr* rhs)
{
  auto stmt{mMgr.new_ForceStmt(parent, process, pt_stmt,
			       lhs, rhs)};
  return stmt;
}

// @brief release ステートメントを生成する．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] lhs 左辺の式
const VlStmt*
Elaborator::new_ReleaseStmt(const VlScope* parent,
			    const VlProcess* process,
			    const PtStmt* pt_stmt,
			    const VlExpr* lhs)
{
  auto stmt{mMgr.new_ReleaseStmt(parent, process, pt_stmt, lhs)};
  return stmt;
}

// @brief begin ブロックを生成する．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] stmt_list 子のステートメントリスト
const VlStmt*
Elaborator::new_Begin(const VlScope* parent,
		      const VlProcess* process,
		      const PtStmt* pt_stmt,
		      const vector<const VlStmt*>& stmt_list)
{
  auto stmt{mMgr.new_Begin(parent, process, pt_stmt, stmt_list)};
  return stmt;
}

// @brief fork ブロックを生成する．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] stmt_list 子のステートメントリスト
const VlStmt*
Elaborator::new_Fork(const VlScope* parent,
		     const VlProcess* process,
		     const PtStmt* pt_stmt,
		     const vector<const VlStmt*>& stmt_list)
{
  auto stmt{mMgr.new_Fork(parent, process, pt_stmt, stmt_list)};
  return stmt;
}

// @brief 名前付き begin ブロックを生成する．
// @param[in] block 自分自身に対応するスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] stmt_list 子のステートメントリスト
const VlStmt*
Elaborator::new_NamedBegin(const VlScope* block,
			   const VlProcess* process,
			   const PtStmt* pt_stmt,
			   const vector<const VlStmt*>& stmt_list)
{
  auto stmt{mMgr.new_NamedBegin(block, process, pt_stmt, stmt_list)};
  return stmt;
}

// @brief 名前付き fork ブロックを生成する．
// @param[in] block 自分自身に対応するスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] stmt_list 子のステートメントリスト
const VlStmt*
Elaborator::new_NamedFork(const VlScope* block,
			  const VlProcess* process,
			  const PtStmt* pt_stmt,
			  const vector<const VlStmt*>& stmt_list)
{
  auto stmt{mMgr.new_NamedFork(block, process, pt_stmt, stmt_list)};
  return stmt;
}

// @brief while 文を生成する．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] cond 条件式
// @param[in] body 本体のステートメント
const VlStmt*
Elaborator::new_WhileStmt(const VlScope* parent,
			  const VlProcess* process,
			  const PtStmt* pt_stmt,
			  const VlExpr* cond,
			  const VlStmt* body)
{
  auto stmt{mMgr.new_WhileStmt(parent, process, pt_stmt, cond, body)};
  return stmt;
}

// @brief repeat 文を生成する．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] cond 条件式
// @param[in] body 本体のステートメント
const VlStmt*
Elaborator::new_RepeatStmt(const VlScope* parent,
			   const VlProcess* process,
			   const PtStmt* pt_stmt,
			   const VlExpr* cond,
			   const VlStmt* body)
{
  auto stmt{mMgr.new_RepeatStmt(parent, process, pt_stmt, cond, body)};
  return stmt;
}

// @brief wait 文を生成する．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] cond 条件式
// @param[in] body 本体のステートメント
const VlStmt*
Elaborator::new_WaitStmt(const VlScope* parent,
			 const VlProcess* process,
			 const PtStmt* pt_stmt,
			 const VlExpr* cond,
			 const VlStmt* body)
{
  auto stmt{mMgr.new_WaitStmt(parent, process, pt_stmt, cond, body)};
  return stmt;
}

// @brief for 文を生成する．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] cond 条件式
// @param[in] init_stmt 初期化式
// @param[in] inc_stmt 増加式
// @param[in] body 本体のステートメント
const VlStmt*
Elaborator::new_ForStmt(const VlScope* parent,
			const VlProcess* process,
			const PtStmt* pt_stmt,
			const VlExpr* cond,
			const VlStmt* init_stmt,
			const VlStmt* inc_stmt,
			const VlStmt* body)
{
  auto stmt{mMgr.new_ForStmt(parent, process, pt_stmt, cond,
			     init_stmt, inc_stmt, body)};
  return stmt;
}

// @brief forever 文を生成する．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] body 本体のステートメント
const VlStmt*
Elaborator::new_ForeverStmt(const VlScope* parent,
			    const VlProcess* process,
			    const PtStmt* pt_stmt,
			    const VlStmt* body)
{
  auto stmt{mMgr.new_ForeverStmt(parent, process, pt_stmt, body)};
  return stmt;
}

// @brief if 文を生成する．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] cond 条件式
// @param[in] then_stmt then節のステートメント
// @param[in] else_stmt else節のステートメント
// @note else_stmt は nullptr もありうる．
const VlStmt*
Elaborator::new_IfStmt(const VlScope* parent,
		       const VlProcess* process,
		       const PtStmt* pt_stmt,
		       const VlExpr* cond,
		       const VlStmt* then_stmt,
		       const VlStmt* else_stmt)
{
  auto stmt{mMgr.new_IfStmt(parent, process, pt_stmt, cond,
			    then_stmt, else_stmt)};
  return stmt;
}

// @brief case 文を生成する．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] expr 条件式
// @param[in] caseitem_list caseitem のリスト
const VlStmt*
Elaborator::new_CaseStmt(const VlScope* parent,
			 const VlProcess* process,
			 const PtStmt* pt_stmt,
			 const VlExpr* expr,
			 const vector<const VlCaseItem*>& caseitem_list)
{
  auto stmt{mMgr.new_CaseStmt(parent, process, pt_stmt, expr,
			      caseitem_list)};
  return stmt;
}

// @brief caseitem を生成する．
// @param[in] pt_item パース木の caseitem 要素
// @param[in] label_list ラベルを表す式のリスト
// @param[in] body 本体のステートメント
const VlCaseItem*
Elaborator::new_CaseItem(const PtCaseItem* pt_item,
			 const vector<ElbExpr*>& label_list,
			 const VlStmt* body)
{
  auto caseitem{mMgr.new_CaseItem(pt_item, label_list, body)};
  return caseitem;
}

// @brief イベント文を生成する．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] named_event 対象のイベント
const VlStmt*
Elaborator::new_EventStmt(const VlScope* parent,
			  const VlProcess* process,
			  const PtStmt* pt_stmt,
			  ElbExpr* named_event)
{
  auto stmt{mMgr.new_EventStmt(parent, process, pt_stmt,
			       named_event)};
  return stmt;
}

// @brief nullptr ステートメントを生成する．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] pt_stmt パース木のステートメント定義
const VlStmt*
Elaborator::new_NullStmt(const VlScope* parent,
			 const VlProcess* process,
			 const PtStmt* pt_stmt)
{
  auto stmt{mMgr.new_NullStmt(parent, process, pt_stmt)};
  return stmt;
}

// @brief タスクコール文を生成する．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] task 対象のタスク
// @param[in] arg_array 引数の配列
const VlStmt*
Elaborator::new_TaskCall(const VlScope* parent,
			 const VlProcess* process,
			 const PtStmt* pt_stmt,
			 const VlTaskFunc* task,
			 const vector<ElbExpr*>& arg_array)
{
  auto stmt{mMgr.new_TaskCall(parent, process, pt_stmt,
			      task, arg_array)};
  return stmt;
}

// @brief システムタスクコール文を生成する．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] user_systf システムタスク
// @param[in] arg_array 引数の配列
const VlStmt*
Elaborator::new_SysTaskCall(const VlScope* parent,
			    const VlProcess* process,
			    const PtStmt* pt_stmt,
			    const VlUserSystf* user_systf,
			    const vector<ElbExpr*>& arg_array)
{
  auto stmt{mMgr.new_SysTaskCall(parent, process, pt_stmt,
				 user_systf, arg_array)};
  return stmt;
}

// @brief disable 文を生成する．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] target 対象のスコープ
const VlStmt*
Elaborator::new_DisableStmt(const VlScope* parent,
			    const VlProcess* process,
			    const PtStmt* pt_stmt,
			    const VlScope* target)
{
  auto stmt{mMgr.new_DisableStmt(parent, process, pt_stmt, target)};
  return stmt;
}

// @brief コントロール文を生成する．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] control コントロール
// @param[in] body 本体のステートメント
const VlStmt*
Elaborator::new_CtrlStmt(const VlScope* parent,
			 const VlProcess* process,
			 const PtStmt* pt_stmt,
			 const VlControl* control,
			 const VlStmt* body)
{
  auto stmt{mMgr.new_CtrlStmt(parent, process, pt_stmt, control, body)};
  return stmt;
}

// @brief 遅延コントロールを生成する．
// @param[in] pt_control パース木の定義要素
// @param[in] delay 遅延式
const VlControl*
Elaborator::new_DelayControl(const PtControl* pt_control,
			     ElbExpr* delay)
{
  auto ctrl{mMgr.new_DelayControl(pt_control, delay)};
  return ctrl;
}

// @brief イベントコントロールを生成する．
// @param[in] pt_control パース木の定義要素
// @param[in] event_list イベントリストを表す配列
const VlControl*
Elaborator::new_EventControl(const PtControl* pt_control,
			     const vector<ElbExpr*>& event_list)
{
  auto ctrl{mMgr.new_EventControl(pt_control, event_list)};
  return ctrl;
}

// @brief リピートコントロールを生成する．
// @param[in] pt_control パース木の定義要素
// @param[in] rep 繰り返し式
// @param[in] event_list イベントリストを表す配列
const VlControl*
Elaborator::new_RepeatControl(const PtControl* pt_control,
			      ElbExpr* rep,
			      const vector<ElbExpr*>& event_list)
{
  auto ctrl{mMgr.new_RepeatControl(pt_control, rep, event_list)};
  return ctrl;
}

// @brief 単項演算子を生成する．
// @param[in] pt_expr パース木の定義要素
// @param[in] op_type 演算子のタイプ
// @param[in] opr1 オペランド
ElbExpr*
Elaborator::new_UnaryOp(const PtExpr* pt_expr,
			VpiOpType op_type,
			ElbExpr* opr1)
{
  auto expr{mMgr.new_UnaryOp(pt_expr, op_type, opr1)};
  return expr;
}

// @brief 2項演算子を生成する．
// @param[in] pt_expr パース木の定義要素
// @param[in] op_type 演算子のタイプ
// @param[in] opr1 オペランド
// @param[in] opr2 オペランド
ElbExpr*
Elaborator::new_BinaryOp(const PtExpr* pt_expr,
			 VpiOpType op_type,
			 ElbExpr* opr1,
			 ElbExpr* opr2)
{
  auto expr{mMgr.new_BinaryOp(pt_expr, op_type,
			      opr1, opr2)};
  return expr;
}

// @brief 3項演算子を生成する．
// @param[in] pt_expr パース木の定義要素
// @param[in] op_type 演算子のタイプ
// @param[in] opr1 オペランド
// @param[in] opr2 オペランド
// @param[in] opr3 オペランド
ElbExpr*
Elaborator::new_TernaryOp(const PtExpr* pt_expr,
			  VpiOpType op_type,
			  ElbExpr* opr1,
			  ElbExpr* opr2,
			  ElbExpr* opr3)
{
  auto expr{mMgr.new_TernaryOp(pt_expr, op_type,
			       opr1, opr2, opr3)};
  return expr;
}

// @brief 連結演算子を生成する．
// @param[in] pt_expr パース木の定義要素
// @param[in] opr_list オペランドのリスト
ElbExpr*
Elaborator::new_ConcatOp(const PtExpr* pt_expr,
			 const vector<ElbExpr*>& opr_list)
{
  auto expr{mMgr.new_ConcatOp(pt_expr, opr_list)};
  return expr;
}

// @brief 反復連結演算子を生成する．
// @param[in] pt_expr パース木の定義要素
// @param[in] rep_expr 繰り返し数を表す式
// @param[in] rep_num 繰り返し数
// @param[in] opr_list オペランドのリスト
ElbExpr*
Elaborator::new_MultiConcatOp(const PtExpr* pt_expr,
			      SizeType rep_num,
			      ElbExpr* rep_expr,
			      const vector<ElbExpr*>& opr_list)
{
  auto expr{mMgr.new_MultiConcatOp(pt_expr, rep_num, rep_expr,
				   opr_list)};
  return expr;
}

// @brief プライマリ式を生成する．
// @param[in] pt_expr パース木の定義要素
// @param[in] obj 本体のオブジェクト
ElbExpr*
Elaborator::new_Primary(const PtExpr* pt_expr,
			const VlDecl* obj)
{
  auto expr{mMgr.new_Primary(pt_expr, obj)};
  return expr;
}

// @brief プライマリ式を生成する(net decl の初期値用)．
// @param[in] pt_item パース木の定義要素
// @param[in] obj 本体のオブジェクト
// @param[in] index_list インデックスのリスト
ElbExpr*
Elaborator::new_Primary(const PtDeclItem* pt_item,
			const VlDecl* obj)
{
  auto expr{mMgr.new_Primary(pt_item, obj)};
  return expr;
}

// @brief プライマリ式を生成する．
// @param[in] pt_expr パース木の定義要素
// @param[in] obj 本体のオブジェクト
ElbExpr*
Elaborator::new_Primary(const PtExpr* pt_expr,
			ElbParameter* obj)
{
  auto expr{mMgr.new_Primary(pt_expr, obj)};
  return expr;
}

// @brief プライマリ式を生成する(配列要素版)．
// @param[in] pt_expr パース木の定義要素
// @param[in] obj 本体のオブジェクト
// @param[in] index_list インデックスのリスト
ElbExpr*
Elaborator::new_Primary(const PtExpr* pt_expr,
			const VlDeclArray* obj,
			const vector<ElbExpr*>& index_list)
{
  auto expr{mMgr.new_Primary(pt_expr, obj, index_list)};
  return expr;
}

// @brief プライマリ式を生成する(固定インデックスの配列要素版)．
// @param[in] pt_expr パース木の定義要素
// @param[in] obj 本体のオブジェクト
// @param[in] offset オフセット
ElbExpr*
Elaborator::new_Primary(const PtExpr* pt_expr,
			const VlDeclArray* obj,
			int offset)
{
  auto expr{mMgr.new_Primary(pt_expr, obj, offset)};
  return expr;
}

// @brief 固定ビット選択式を生成する．
// @param[in] pt_expr パース木の定義要素
// @param[in] body 本体のオブジェクト
// @param[in] bit_index ビット選択式
// @param[in] bit_index_val ビット選択式の値
ElbExpr*
Elaborator::new_BitSelect(const PtExpr* pt_expr,
			  ElbExpr* body,
			  const PtExpr* bit_index,
			  int bit_index_val)
{
  auto expr{mMgr.new_BitSelect(pt_expr, body,
			       bit_index, bit_index_val)};
  return expr;
}

// @brief 固定ビット選択式を生成する．
// @param[in] pt_expr パース木の定義要素
// @param[in] body 本体の式
// @param[in] bit_index_val ビット選択式の値
ElbExpr*
Elaborator::new_BitSelect(const PtExpr* pt_expr,
			  ElbExpr* body,
			  int bit_index_val)
{
  auto expr{mMgr.new_BitSelect(pt_expr, body, bit_index_val)};
  return expr;
}

// @brief 可変ビット選択式を生成する．
// @param[in] pt_expr パース木の定義要素
// @param[in] body 本体のオブジェクト
// @param[in] bit_index ビット選択式
ElbExpr*
Elaborator::new_BitSelect(const PtExpr* pt_expr,
			  ElbExpr* body,
			  ElbExpr* bit_index)
{
  auto expr{mMgr.new_BitSelect(pt_expr, body, bit_index)};
  return expr;
}

// @brief 固定部分選択式を生成する．
// @param[in] pt_expr パース木の定義要素
// @param[in] body 本体のオブジェクト
// @param[in] index1, index2 パート選択式
// @param[in] index1_val, index2_val パート選択式の値
ElbExpr*
Elaborator::new_PartSelect(const PtExpr* pt_expr,
			   ElbExpr* body,
			   const PtExpr* index1,
			   const PtExpr* index2,
			   int index1_val,
			   int index2_val)
{
  auto expr{mMgr.new_PartSelect(pt_expr, body,
				index1, index2,
				index1_val, index2_val)};
  return expr;
}

// @brief 固定部分選択式を生成する．
// @param[in] pt_expr パース木の定義要素
// @param[in] body 本体の式
// @param[in] index1, inde2 パート選択式
ElbExpr*
Elaborator::new_PartSelect(const PtExpr* pt_expr,
			   ElbExpr* body,
			   int index1,
			   int index2)
{
  auto expr{mMgr.new_PartSelect(pt_expr, body,
				index1, index2)};
  return expr;
}

// @brief 可変部分選択式を生成する．
// @param[in] pt_expr パース木の定義要素
// @param[in] body 本体のオブジェクト
// @param[in] base 範囲のベースアドレスを表す式
// @param[in] range 範囲を表す式
// @param[in] range_val 範囲の値
ElbExpr*
Elaborator::new_PlusPartSelect(const PtExpr* pt_expr,
			       ElbExpr* body,
			       ElbExpr* base,
			       const PtExpr* range_expr,
			       int range_val)
{
  auto expr{mMgr.new_PlusPartSelect(pt_expr, body, base,
				    range_expr, range_val)};
  return expr;
}

// @brief 可変部分選択式を生成する．
// @param[in] pt_expr パース木の定義要素
// @param[in] body 本体のオブジェクト
// @param[in] base 範囲のベースアドレスを表す式
// @param[in] range 範囲を表す式
// @param[in] range_val 範囲の値
ElbExpr*
Elaborator::new_MinusPartSelect(const PtExpr* pt_expr,
				ElbExpr* body,
				ElbExpr* base,
				const PtExpr* range_expr,
				int range_val)
{
  auto expr{mMgr.new_MinusPartSelect(pt_expr, body, base,
				     range_expr, range_val)};
  return expr;
}

// @brief 定数式を生成する．
// @param[in] pt_expr パース木の定義要素
ElbExpr*
Elaborator::new_Constant(const PtExpr* pt_expr)
{
  auto expr{mMgr.new_Constant(pt_expr)};
  return expr;
}

// @brief genvar 起因の定数式を生成する．
// @param[in] pt_primary パース木の定義要素
// @param[in] val 値
ElbExpr*
Elaborator::new_GenvarConstant(const PtExpr* pt_primary,
			       int val)
{
  auto expr{mMgr.new_GenvarConstant(pt_primary, val)};
  return expr;
}

// @brief 関数呼び出し式を生成する．
// @param[in] pt_expr パース木の定義要素
// @param[in] func 関数
// @param[in] arg_list 引数のリスト
ElbExpr*
Elaborator::new_FuncCall(const PtExpr* pt_expr,
			 const VlTaskFunc* func,
			 const vector<ElbExpr*>& arg_list)
{
  auto expr{mMgr.new_FuncCall(pt_expr, func, arg_list)};
  return expr;
}

// @brief システム関数呼び出し式を生成する．
// @param[in] pt_expr パース木の定義要素
// @param[in] user_systf システム関数
// @param[in] arg_list 引数のリスト
ElbExpr*
Elaborator::new_SysFuncCall(const PtExpr* pt_expr,
			    const VlUserSystf* user_systf,
			    const vector<ElbExpr*>& arg_list)
{
  auto expr{mMgr.new_SysFuncCall(pt_expr, user_systf, arg_list)};
  return expr;
}

// @brief システム関数/システムタスクの引数を生成する．
// @param[in] pt_expr パース木中で参照している要素
// @param[in] arg 引数本体
ElbExpr*
Elaborator::new_ArgHandle(const PtExpr* pt_expr,
			  const VlScope* arg)
{
  auto expr{mMgr.new_ArgHandle(pt_expr, arg)};
  return expr;
}

// @brief システム関数/システムタスクの引数を生成する．
// @param[in] pt_expr パース木中で参照している要素
// @param[in] arg 引数本体
ElbExpr*
Elaborator::new_ArgHandle(const PtExpr* pt_expr,
			  const VlPrimitive* arg)
{
  auto expr{mMgr.new_ArgHandle(pt_expr, arg)};
  return expr;
}

// @brief システム関数/システムタスクの引数を生成する．
// @param[in] pt_expr パース木中で参照している要素
// @param[in] arg 引数本体
ElbExpr*
Elaborator::new_ArgHandle(const PtExpr* pt_expr,
			  const VlDeclArray* arg)
{
  auto expr{mMgr.new_ArgHandle(pt_expr, arg)};
  return expr;
}

// @brief 連結演算子の左辺式を生成する．
// @param[in] pt_expr パース木の定義要素
// @param[in] opr_array オペランドを格納する配列
// @param[in] lhs_elem_array 左辺の要素の配列
ElbExpr*
Elaborator::new_Lhs(const PtExpr* pt_expr,
		    const vector<ElbExpr*>& opr_array,
		    const vector<ElbExpr*>& lhs_elem_array)
{
  auto expr{mMgr.new_Lhs(pt_expr, opr_array, lhs_elem_array)};
  return expr;
}

// @brief 遅延値を生成する．
// @param[in] pt_obj パース木の定義要素
// @param[in] expr_list 式の配列
const VlDelay*
Elaborator::new_Delay(const PtBase* pt_obj,
		      const vector<ElbExpr*>& expr_list)
{
  auto delay{mMgr.new_Delay(pt_obj, expr_list)};
  return delay;
}

// @brief attribute instance のリストを生成する．
// @param[in] pt_attr パース木の定義要素
// @param[in] expr 値
// @param[in] def 定義側の属性の時 true
const VlAttribute*
Elaborator::new_Attribute(const PtAttrSpec* pt_attr,
			  const VlExpr* expr,
			  bool def)
{
  auto attr{mMgr.new_Attribute(pt_attr, expr, def)};
  return attr;
}

END_NAMESPACE_YM_VERILOG
