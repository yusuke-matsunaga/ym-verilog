
/// @file ItemGen_taskfunc.cc
/// @brief ElbMgr の実装ファイル(タスク/関数の実体化)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.

#include "ItemGen.h"
#include "ElbEnv.h"
#include "ElbStub.h"

#include "ym/pt/PtItem.h"
#include "elaborator/ElbDecl.h"
#include "elaborator/ElbTaskFunc.h"

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
void
ItemGen::phase1_tf(
  const VlScope* parent,
  const PtItem* pt_item
)
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

  ElbTaskFunc* taskfunc{nullptr};
  if ( pt_item->type() == PtItemType::Task ) {
    taskfunc = mgr().new_Task(parent, pt_item);
  }
  else {
    ASSERT_COND( pt_item->type() == PtItemType::Func );

    auto pt_left = pt_item->left_range();
    auto pt_right = pt_item->right_range();
    if ( pt_left && pt_right ) {
      int left_val;
      int right_val;
      tie(left_val, right_val) = evaluate_range(parent, pt_left, pt_right);
      taskfunc = mgr().new_Function(parent, pt_item,
				    pt_left, pt_right,
				    left_val, right_val,
				    false);
    }
    else {
      taskfunc = mgr().new_Function(parent, pt_item, false);
    }
    ASSERT_COND( taskfunc != nullptr );
  }

  // 宣言要素の生成(phase1 では parameter と genvar のみ)
  phase1_decl(taskfunc, pt_item->declhead_list(), false);

  // attribute instance の生成
  auto attr_list = attribute_list(pt_item);
  mgr().reg_attr(taskfunc, attr_list);

  {
    ostringstream buf;
    buf << "instantiating task/func : " << taskfunc->full_name() << ".";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    pt_item->file_region(),
		    MsgType::Info,
		    "ELAB",
		    buf.str());
  }

  // 本体のステートメント内部のスコープの生成
  auto pt_body = pt_item->body();
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
void
ItemGen::phase2_tf(
  ElbTaskFunc* taskfunc,
  const PtItem* pt_item
)
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
  instantiate_decl(taskfunc, pt_item->declhead_list());

  // 入出力の生成
  instantiate_iodecl(taskfunc, pt_item->iohead_list());

  if ( taskfunc->type() == VpiObjType::Function ) {
    // 関数名と同名の変数の生成
    int left_val = taskfunc->left_range_val();
    int right_val = taskfunc->right_range_val();
    ElbDeclHead* head{nullptr};
    if ( taskfunc->has_range() ) {
      head = mgr().new_DeclHead(taskfunc, pt_item,
				pt_item->left_range(), pt_item->right_range(),
				left_val, right_val);
    }
    else {
      head = mgr().new_DeclHead(taskfunc, pt_item);
    }
    ASSERT_COND( head );

    int tag{ (pt_item->data_type() == VpiVarType::None) ? vpiReg : vpiVariables };
    auto decl = mgr().new_Decl(tag, head, pt_item);

    taskfunc->set_ovar(decl);
  }

  if ( debug ) {
    dout << "phase2_tf end" << endl
	 << endl;
  }
}

// @param[in] task/function 内のステートメントの生成を行う．
void
ItemGen::phase3_tf(
  ElbTaskFunc* taskfunc,
  const PtItem* pt_item
)
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
  auto pt_body = pt_item->body();
  auto body = instantiate_stmt(taskfunc, nullptr, env, pt_body);
  if ( body ) {
    taskfunc->set_stmt(body);
  }

  if ( debug ) {
    dout << "phase3_tf end" << endl
	 << endl;
  }
}

// @brief constant function の生成を行う．
const VlTaskFunc*
ItemGen::instantiate_constant_function(
  const VlScope* parent,
  const PtItem* pt_function
)
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

  auto pt_left = pt_function->left_range();
  auto pt_right = pt_function->right_range();

  ElbTaskFunc* func{nullptr};
  const VlScope* scope{nullptr};
  ElbDeclHead* head{nullptr};
  if ( pt_left && pt_right ) {
    int left_val;
    int right_val;
    tie(left_val, right_val) = evaluate_range(parent, pt_left, pt_right);
    func = mgr().new_Function(parent, pt_function,
			      pt_left, pt_right,
			      left_val, right_val,
			      true);
    head = mgr().new_DeclHead(func, pt_function,
			      pt_left, pt_right,
			      left_val, right_val);
  }
  else {
    func = mgr().new_Function(parent, pt_function, true);
    head = mgr().new_DeclHead(func, pt_function);
  }
  ASSERT_COND( func );
  ASSERT_COND( head );

  // 登録しておく．
  reg_constant_function(func);

  // parameter の生成
  phase1_decl(func, pt_function->declhead_list(), false);

  // 宣言要素の生成
  instantiate_decl(func, pt_function->declhead_list());

  // 関数名と同名の変数の生成
  int tag{ (pt_function->data_type() == VpiVarType::None) ? vpiReg : vpiVariables };
  auto decl = mgr().new_Decl(tag, head, pt_function);

  func->set_ovar(decl);

  // 入出力の生成
  instantiate_iodecl(func, pt_function->iohead_list());

  // 本体のステートメント内部のスコープの生成
  auto pt_body = pt_function->body();
  phase1_stmt(func, pt_body, true);

  // 本体のステートメントの生成
  ElbConstantFunctionEnv env(scope);
  auto body = instantiate_stmt(scope, nullptr, env, pt_body);
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
