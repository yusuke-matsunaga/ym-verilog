﻿
/// @file ModuleGen.cc
/// @brief ElbMgr の実装ファイル (module のインスタンス化関係)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ModuleGen.h"
#include "ElbParamCon.h"
#include "ElbStub.h"

#include "ym/pt/PtModule.h"
#include "ym/pt/PtPort.h"
#include "ym/pt/PtDecl.h"
#include "ym/pt/PtItem.h"
#include "ym/pt/PtExpr.h"
#include "ym/pt/PtMisc.h"
#include "ym/pt/PtArray.h"

#include "ElbModule.h"
#include "ElbDecl.h"
#include "ElbParameter.h"
#include "ElbParamAssign.h"
#include "ElbExpr.h"

#include "ym/MsgMgr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス ModuleGen
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] elab 生成器
// @param[in] elb_mgr Elbオブジェクトを管理するクラス
// @param[in] elb_factory Elbオブジェクトを生成するファクトリクラス
ModuleGen::ModuleGen(Elaborator& elab,
		     ElbMgr& elb_mgr,
		     ElbFactory& elb_factory) :
  ElbProxy(elab, elb_mgr, elb_factory)
{
}

// @brief デストラクタ
ModuleGen::~ModuleGen()
{
}

// top module 用の骨組みを作る．
// @param[in] toplevel トップレベルのスコープ
// @param[in] pt_module モジュール定義
void
ModuleGen::phase1_topmodule(const VlNamedObj* toplevel,
			    const PtModule* pt_module)
{
  const FileRegion& file_region = pt_module->file_region();
  const char* name = pt_module->name();

  ostringstream buf;
  buf << "instantiating top module \"" << name << "\".";
  MsgMgr::put_msg(__FILE__, __LINE__,
		  file_region,
		  MsgType::Info,
		  "ELAB",
		  buf.str());

  // モジュール本体の生成
  ElbModule* module = factory().new_Module(toplevel,
					   pt_module,
					   nullptr,
					   nullptr);
  reg_module(module);

#if 0
  // attribute instance の生成
  instantiate_attribute(pt_module->attr_top(), true, module);
#else
#warning "TODO:2011-02-09-01"
#endif

  ostringstream buf2;
  buf2 << "module \"" << module->full_name() << "\" has been created.";
  MsgMgr::put_msg(__FILE__, __LINE__,
		  file_region,
		  MsgType::Info,
		  "ELAB",
		  buf2.str());

  // 中身のうちスコープに関係する要素の生成
  phase1_module_item(module, pt_module, nullptr);
}

// @brief module の中身のうちスコープに関係する要素のインスタンス化をする．
// @param[in] modle モジュール
// @param[in] pt_module モジュール定義
// @param[in] pt_inst_header インスタンス定義のヘッダ
void
ModuleGen::phase1_module_item(ElbModule* module,
			      const PtModule* pt_module,
			      const ElbParamCon* param_con)
{
  // ループチェック用のフラグを立てる．
  pt_module->set_in_use();

  // パラメータポートを実体化する．
  PtDeclHeadArray paramport_array = pt_module->paramport_array();
  bool has_paramportdecl = (paramport_array.size() > 0);
  if ( has_paramportdecl ) {
    phase1_decl(module, paramport_array, false);
  }

  // parameter と genvar を実体化する．
  PtDeclHeadArray declhead_array = pt_module->declhead_array();
  phase1_decl(module, declhead_array, has_paramportdecl);

  // パラメータの割り当てを作る．
  if ( param_con ) {
    ymuint n = param_con->elem_num();
    if ( param_con->named_con() ) {
      // 名前による割り当て
      for (ymuint i = 0; i < n; ++ i) {
	const PtConnection* pt_con = param_con->pt_con(i);
	ElbObjHandle* handle = find_obj(module, pt_con->name());
	if ( handle == nullptr || handle->type() != kVpiParameter ) {
	  ostringstream buf;
	  buf << param_con->name(i) << " : No such parameter.";
	  MsgMgr::put_msg(__FILE__, __LINE__,
			  pt_con->file_region(),
			  MsgType::Error,
			  "ELAB",
			  buf.str());
	  continue;
	}
	ElbParameter* param = handle->parameter();
	ASSERT_COND( param );

	const PtExpr* expr = param_con->expr(i);
	VlValue value = param_con->value(i);
	param->set_expr(expr, value);
	ElbParamAssign* pa = factory().new_NamedParamAssign(module, pt_con,
							    param, expr,
							    value);
	reg_paramassign(pa);
      }
    }
    else {
      // 順序による割り当て

      // パラメータポートリストの名前を現れた順番に paramport_list に入れる．
      vector<const char*> paramport_list;
      if ( has_paramportdecl ) {
	for (ymuint i = 0; i < paramport_array.size(); ++ i) {
	  const PtDeclHead* pt_param = paramport_array[i];
	  for (ymuint j = 0; j < pt_param->item_num(); ++ j) {
	    const PtDeclItem* pt_item = pt_param->item(j);
	    paramport_list.push_back(pt_item->name());
	  }
	}
      }
      else {
	for (ymuint i = 0; i < declhead_array.size(); ++ i) {
	  const PtDeclHead* pt_decl = declhead_array[i];
	  if ( pt_decl->type() == kPtDecl_Param ) {
	    for (ymuint j = 0; j < pt_decl->item_num(); ++ j) {
	      const PtDeclItem* pt_item = pt_decl->item(j);
	      paramport_list.push_back(pt_item->name());
	    }
	  }
	}
      }
      if ( paramport_list.size() < n ) {
	MsgMgr::put_msg(__FILE__, __LINE__,
			param_con->file_region(),
			MsgType::Error,
			"ELAB",
			"Too many parameters.");
      }
      else {
	for (ymuint i = 0; i < n; ++ i) {
	  const PtConnection* pt_con = param_con->pt_con(i);
	  const char* tmp_name = paramport_list[i];
	  ElbObjHandle* handle = find_obj(module, tmp_name);
	  ASSERT_COND( handle );

	  ElbParameter* param = handle->parameter();
	  ASSERT_COND( param );

	  const PtExpr* expr = param_con->expr(i);
	  VlValue value = param_con->value(i);
	  param->set_expr(expr, value);
	  ElbParamAssign* pa = factory().new_ParamAssign(module, pt_con,
							 param, expr, value);
	  reg_paramassign(pa);
	}
      }
    }
  }

  // それ以外の要素を実体化する．
  phase1_item(module, pt_module->item_array());

  // phase2 で行う処理を登録しておく．
  add_phase2stub(module, pt_module);

  // ループチェック用のフラグを下ろす．
  pt_module->reset_in_use();
}

// @brief module の中身のインスタンス化を行う．
// @param[in] modle モジュール
// @param[in] pt_module モジュール定義
void
ModuleGen::phase2_module_item(ElbModule* module,
			      const PtModule* pt_module)
{
  // 宣言要素を実体化する．
  instantiate_decl(module, pt_module->declhead_array());

  // IODecl を実体化する．
  instantiate_iodecl(module, nullptr, pt_module->iohead_array());

  // ポートを実体化する
  instantiate_port(module, pt_module);
}

// port の生成を行う．
void
ModuleGen::instantiate_port(ElbModule* module,
			    const PtModule* pt_module)
{
  ymuint port_num = pt_module->port_num();
  for (ymuint index = 0; index < port_num; ++ index) {
    const PtPort* pt_port = pt_module->port(index);
    // 内側の接続と向きを作る．
    ymuint n = pt_port->portref_size();

    ElbExpr* low_conn = nullptr;
    tVlDirection dir = kVlNoDirection;

    const PtExpr* pt_portref = pt_port->portref();
    if ( n == 1 ) {
      // 単一の要素の場合
      dir = pt_port->portref_dir(0);
      low_conn = instantiate_portref(module, pt_portref);
    }
    else if ( n > 1 ) {
      // 複数要素の結合の場合
      ElbExpr** expr_list = factory().new_ExprList(n);
      ElbExpr** lhs_elem_array = factory().new_ExprList(n);

      for (ymuint i = 0; i < n; ++ i) {
	const PtExpr* pt_portexpr = pt_port->portref_elem(i);
	ElbExpr* portexpr = instantiate_portref(module, pt_portexpr);
	if ( !portexpr ) {
	  return;
	}
	expr_list[i] = portexpr;
	lhs_elem_array[n - i - 1] = portexpr;

	tVlDirection dir1 = pt_port->portref_dir(i);
	if ( dir == kVlNoDirection ) {
	  dir = dir1;
	}
	else if ( dir != dir1 ) {
	  dir = kVlMixedIO;
	}
      }

      low_conn = factory().new_Lhs(pt_portref, n, expr_list, n, lhs_elem_array);
    }
    module->init_port(index, pt_port, low_conn, dir);
  }
}

// PtPortRef から expression を生成する．
ElbExpr*
ModuleGen::instantiate_portref(ElbModule* module,
			       const PtExpr* pt_portref)
{
  const char* name = pt_portref->name();
  ElbObjHandle* handle = find_obj(module, name);
  if ( !handle ) {
    ostringstream buf;
    buf << name
	<< ": Not found.";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    pt_portref->file_region(),
		    MsgType::Error,
		    "ELAB",
		    buf.str());
    return nullptr;
  }

  if ( handle->declarray() ) {
    ostringstream buf;
    buf << handle->declarray()->full_name()
	<< ": Array shall not be connected to a module port.";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    pt_portref->file_region(),
		    MsgType::Error,
		    "ELAB",
		    buf.str());
    return nullptr;
  }
  ElbDecl* decl = handle->decl();
  if ( decl == nullptr ) {
    ostringstream buf;
    buf << name
	<< ": Illegal type for port connection.";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    pt_portref->file_region(),
		    MsgType::Error,
		    "ELAB",
		    buf.str());
    return nullptr;
  }

  ElbExpr* primary = factory().new_Primary(pt_portref, decl);

  // 添字の部分を実体化する．
  const PtExpr* pt_index = nullptr;
  if ( pt_portref->index_num() == 0 ) {
    pt_index = pt_portref->index(0);
  }
  const PtExpr* pt_left = pt_portref->left_range();
  const PtExpr* pt_right = pt_portref->right_range();
  if ( pt_index ) {
    int index_val;
    bool stat = evaluate_int(module, pt_index, index_val, true);
    if ( !stat ) {
      return nullptr;
    }
    ymuint offset;
    bool stat2 = decl->calc_bit_offset(index_val, offset);
    if ( !stat2 ) {
      // 添字が範囲外
      MsgMgr::put_msg(__FILE__, __LINE__,
		      pt_index->file_region(),
		      MsgType::Warning,
		      "ELAB",
		      "Index is out of range.");
    }
    return factory().new_BitSelect(pt_portref, primary, pt_index, index_val);
  }
  if ( pt_left && pt_right ) {
    int left_val = 0;
    int right_val = 0;
    if ( !evaluate_range(module, pt_left, pt_right, left_val, right_val) ) {
      return nullptr;
    }
    ymuint offset;
    bool stat1 = decl->calc_bit_offset(left_val, offset);
    if ( !stat1 ) {
      // 左の添字が範囲外
      MsgMgr::put_msg(__FILE__, __LINE__,
		      pt_left->file_region(),
		      MsgType::Warning,
		      "ELAB",
		      "Left index is out of range.");
    }
    bool stat2 = decl->calc_bit_offset(right_val, offset);
    if ( !stat2 ) {
      // 右の添字が範囲外
      MsgMgr::put_msg(__FILE__, __LINE__,
		      pt_right->file_region(),
		      MsgType::Warning,
		      "ELAB",
		      "Right index is out of range.");
    }
    return factory().new_PartSelect(pt_portref, primary,
				    pt_left, pt_right,
				    left_val, right_val);
  }
  return primary;
}

END_NAMESPACE_YM_VERILOG
