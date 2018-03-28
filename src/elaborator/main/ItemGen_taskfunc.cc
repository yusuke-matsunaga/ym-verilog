﻿
/// @file ItemGen_taskfunc.cc
/// @brief ElbMgr の実装ファイル(タスク/関数の実体化)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ItemGen.h"
#include "ElbEnv.h"

#include "ym/pt/PtItem.h"

#include "ElbTaskFunc.h"

#include "ElbStub.h"

#include "ym/MsgMgr.h"


BEGIN_NAMESPACE_YM_VERILOG

BEGIN_NONAMESPACE

const int debug = 0;
#define dout cout

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// task/function 関係の instantiate 関数
//////////////////////////////////////////////////////////////////////

// @brief task/function の生成を行う．
// @param[in] parent 親のヘッダ
// @param[in] pt_item タスク/関数定義
void
ItemGen::phase1_tf(const VlNamedObj* parent,
		   const PtItem* pt_item)
{
  if ( debug ) {
    dout << endl
	 << "phase1_tf( "
	 << parent->full_name()
	 << " ["
	 << hex << reinterpret_cast<ympuint>(parent) << dec
	 << "], "
	 << pt_item->name() << " )" << endl
	 << endl;
  }

  ElbTaskFunc* taskfunc = nullptr;

  if ( pt_item->type() == kPtItem_Task ) {
    taskfunc = factory().new_Task(parent, pt_item);
    reg_task(taskfunc);
  }
  else {
    ASSERT_COND( pt_item->type() == kPtItem_Func );

    const PtExpr* pt_left = pt_item->left_range();
    const PtExpr* pt_right = pt_item->right_range();

    if ( pt_left && pt_right ) {
      int left_val = 0;
      int right_val = 0;
      if ( !evaluate_range(parent,
			   pt_left, pt_right,
			   left_val, right_val) ) {
	return;
      }
      taskfunc = factory().new_Function(parent,	pt_item,
					pt_left, pt_right,
					left_val, right_val);
    }
    else {
      taskfunc = factory().new_Function(parent,	pt_item);
    }
    ASSERT_COND( taskfunc != nullptr );

    reg_function(taskfunc);
  }

  // parameter の生成
  phase1_decl(taskfunc, pt_item->declhead_array(), false);

#if 0
  // attribute instance の生成
  instantiate_attribute(pt_item->attr_top(), false, taskfunc);
#else
#warning "TODO:2011-02-09-01"
#endif

  ostringstream buf;
  buf << "instantiating task/func : " << taskfunc->full_name() << ".";
  MsgMgr::put_msg(__FILE__, __LINE__,
		  pt_item->file_region(),
		  MsgType::Info,
		  "ELAB",
		  buf.str());

  // 本体のステートメント内部のスコープの生成
  const PtStmt* pt_body = pt_item->body();
  phase1_stmt(taskfunc, pt_body);

  // 残りの仕事は phase2, phase3 で行う．
  add_phase2stub(make_stub(this, &ItemGen::phase2_tf, taskfunc, pt_item));
  add_phase3stub(make_stub(this, &ItemGen::phase3_tf, taskfunc, pt_item));

  if ( debug ) {
    dout << "phase1_tf end" << endl
	 << endl;
  }
}

// @param[in] task/function 内の宣言要素の生成を行う．
// @param[in] taskfunc タスク/関数本体
// @param[in] pt_item パース木のタスク/関数定義
void
ItemGen::phase2_tf(ElbTaskFunc* taskfunc,
		   const PtItem* pt_item)
{
  if ( debug ) {
    dout << endl
	 << "phase2_tf( "
	 << taskfunc->full_name()
	 << " ["
	 << hex << reinterpret_cast<ympuint>(taskfunc) << dec
	 << "] )" << endl
	 << endl;
  }

  // 宣言要素の生成
  instantiate_decl(taskfunc, pt_item->declhead_array());

  // 入出力の生成
  instantiate_iodecl(nullptr, taskfunc, pt_item->iohead_array());

  if ( taskfunc->type() == kVpiFunction ) {
    // 関数名と同名の変数の生成
    int left_val = taskfunc->left_range_val();
    int right_val = taskfunc->right_range_val();
    ElbDeclHead* head = nullptr;
    if ( taskfunc->has_range() ) {
      head = factory().new_DeclHead(taskfunc, pt_item,
				    pt_item->left_range(), pt_item->right_range(),
				    left_val, right_val);
    }
    else {
      head = factory().new_DeclHead(taskfunc, pt_item);
    }
    ASSERT_COND( head );

    ElbDecl* decl = factory().new_Decl(head, pt_item);
    int tag = vpiVariables;
    if ( pt_item->data_type() == kVpiVarNone ) {
      tag = vpiReg;
    }
    reg_decl(tag, decl);

    taskfunc->set_ovar(decl);
  }

  if ( debug ) {
    dout << "phase2_tf end" << endl
	 << endl;
  }
}

// @param[in] task/function 内のステートメントの生成を行う．
// @param[in] taskfunc task/関数本体
// @param[in] pt_item パース木のタスク/関数定義
void
ItemGen::phase3_tf(ElbTaskFunc* taskfunc,
		   const PtItem* pt_item)
{
  if ( debug ) {
    dout << endl
	 << "phase3_tf( "
	 << taskfunc->full_name()
	 << " ["
	 << hex << reinterpret_cast<ympuint>(taskfunc) << dec
	 << "] )" << endl
	 << endl;
  }

  // 本体のステートメントの生成
  ElbTfEnv env(taskfunc);
  const PtStmt* pt_body = pt_item->body();
  ElbStmt* body = instantiate_stmt(taskfunc, nullptr, env, pt_body);
  if ( body ) {
    taskfunc->set_stmt(body);
  }

  if ( debug ) {
    dout << "phase3_tf end" << endl
	 << endl;
  }
}

// @brief constant function の生成を行う．
// @param[in] parent 親のスコープ
// @param[in] pt_function 関数定義
ElbTaskFunc*
ItemGen::instantiate_constant_function(const VlNamedObj* parent,
				       const PtItem* pt_function)
{
  // 基本的には phase1_tf(), phase2_tf(), phase3_tf() と同じことを
  // やっているが，ElbConstantFunctionEnv を用いているところが異なる．
  if ( debug ) {
    dout << endl
	 << "instantiate_constant_function( "
	 << parent->full_name()
	 << " ["
	 << hex << reinterpret_cast<ympuint>(parent) << dec
	 << "] , "
	 << pt_function->name() << " )" << endl
	 << endl;
  }

  const PtExpr* pt_left = pt_function->left_range();
  const PtExpr* pt_right = pt_function->right_range();

  ElbTaskFunc* func = nullptr;
  ElbDeclHead* head = nullptr;
  if ( pt_left && pt_right ) {
    int left_val = 0;
    int right_val = 0;
    if ( !evaluate_range(parent, pt_left, pt_right,
			 left_val, right_val) ) {
      return nullptr;
    }
    func = factory().new_Function(parent, pt_function,
				  pt_left, pt_right,
				  left_val, right_val);
    head = factory().new_DeclHead(func, pt_function,
				  pt_left, pt_right,
				  left_val, right_val);
  }
  else {
    func = factory().new_Function(parent, pt_function);
    head = factory().new_DeclHead(func, pt_function);
  }
  ASSERT_COND( func );
  ASSERT_COND( head );

  // 登録しておく．
  reg_constant_function(parent, pt_function->name(), func);

  // parameter の生成
  phase1_decl(func, pt_function->declhead_array(), false);

  // 宣言要素の生成
  instantiate_decl(func, pt_function->declhead_array());

  // 関数名と同名の変数の生成
  ElbDecl* decl = factory().new_Decl(head, pt_function);
  int tag = vpiVariables;
  if ( pt_function->data_type() == kVpiVarNone ) {
    tag = vpiReg;
  }
  reg_decl(tag, decl);

  func->set_ovar(decl);

  // 入出力の生成
  instantiate_iodecl(nullptr, func, pt_function->iohead_array());

  // 本体のステートメント内部のスコープの生成
  const PtStmt* pt_body = pt_function->body();
  phase1_stmt(func, pt_body, true);

  // 本体のステートメントの生成
  ElbConstantFunctionEnv env(func);
  ElbStmt* body = instantiate_stmt(func, nullptr, env, pt_body);
  if ( body ) {
    func->set_stmt(body);
  }

  if ( debug ) {
    dout << "instantiate_constant_function end" << endl
	 << endl;
  }

  return func;
}

END_NAMESPACE_YM_VERILOG
