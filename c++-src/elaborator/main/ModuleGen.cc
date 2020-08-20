
/// @file ModuleGen.cc
/// @brief ElbMgr の実装ファイル (module のインスタンス化関係)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014, 2020 Yusuke Matsunaga
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

#include "elaborator/ElbModule.h"
#include "elaborator/ElbDecl.h"
#include "elaborator/ElbParameter.h"
#include "elaborator/ElbExpr.h"

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
ModuleGen::phase1_topmodule(const VlScope* toplevel,
			    const PtModule* pt_module)
{
  const auto& file_region = pt_module->file_region();
  auto name = pt_module->name();

  ostringstream buf;
  buf << "instantiating top module \"" << name << "\".";
  MsgMgr::put_msg(__FILE__, __LINE__,
		  file_region,
		  MsgType::Info,
		  "ELAB",
		  buf.str());
  // モジュール本体の生成
  auto module = factory().new_Module(toplevel,
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
  phase1_module_item(module, pt_module, vector<ElbParamCon>());
}

// @brief module の中身のうちスコープに関係する要素のインスタンス化をする．
// @param[in] modle モジュール
// @param[in] pt_module モジュール定義
// @param[in] pt_inst_header インスタンス定義のヘッダ
void
ModuleGen::phase1_module_item(ElbModule* module,
			      const PtModule* pt_module,
			      const vector<ElbParamCon>& param_con_list)
{
  // ループチェック用のフラグを立てる．
  pt_module->set_in_use();

  // パラメータポートを実体化する．
  bool has_paramportdecl = (pt_module->paramport_num() > 0);
  if ( has_paramportdecl ) {
    phase1_decl(module, pt_module->paramport_list(), false);
  }

  // parameter と genvar を実体化する．
  phase1_decl(module, pt_module->declhead_list(), has_paramportdecl);

  // パラメータの割り当てを作る．
  bool named_con = (param_con_list.size() > 0 &&
		    param_con_list[0].mPtCon->name() != nullptr);
  // パラメータポートリストの名前を現れた順番に paramport_list に入れる．
  vector<const char*> paramport_list;
  if ( named_con ) {
    // 名前による割り当て
    for ( const auto& param_con: param_con_list ) {
      auto pt_con = param_con.mPtCon;
      paramport_list.push_back(pt_con->name());
    }
  }
  else {
    // 順序による割り当て
    if ( has_paramportdecl ) {
      for ( auto pt_param: pt_module->paramport_list() ) {
	for ( auto pt_item: pt_param->item_list() ) {
	  paramport_list.push_back(pt_item->name());
	}
      }
    }
    else {
      for ( auto pt_decl: pt_module->declhead_list() ) {
	if ( pt_decl->type() == PtDeclType::Param ) {
	  for ( auto pt_item: pt_decl->item_list() ) {
	    paramport_list.push_back(pt_item->name());
	  }
	}
      }
    }
    if ( paramport_list.size() < param_con_list.size() ) {
      // 実際のパラメータの数より割り当てリストの要素数が多い．
      error_too_many_param(param_con_list);
    }
  }

  // param_con を paramport_list の名前と結びつける．
  // named_con の場合には冗長なことをやっている．
  SizeType index = 0;
  for ( const auto& param_con: param_con_list ) {
    auto pt_con = param_con.mPtCon;
    auto name = paramport_list[index]; ++ index;
    auto handle = find_obj(module, name);
    if ( handle == nullptr || handle->type() != VpiObjType::Parameter ) {
      error_no_param(pt_con, name);
      continue;
    }

    auto param = handle->parameter();
    ASSERT_COND( param );

    auto expr = param_con.mExpr;
    auto value = param_con.mValue;
    param->set_init_expr(expr, value);
    auto pa = factory().new_NamedParamAssign(module, pt_con,
					     param, expr, value);
    reg_paramassign(pa);
  }

  // それ以外の要素を実体化する．
  phase1_item(module, pt_module->item_list());

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
  instantiate_decl(module, pt_module->declhead_list());

  // IODecl を実体化する．
  instantiate_iodecl(module, pt_module->iohead_list());

  // ポートを実体化する
  instantiate_port(module, pt_module);

}

// port の生成を行う．
void
ModuleGen::instantiate_port(ElbModule* module,
			    const PtModule* pt_module)
{
  SizeType index = 0;
  for ( auto pt_port: pt_module->port_list() ) {
    // 内側の接続と向きを作る．
    SizeType n = pt_port->portref_size();

    ElbExpr* low_conn = nullptr;
    auto dir = VpiDir::NoDirection;
    auto pt_portref = pt_port->portref();
    if ( n == 1 ) {
      // 単一の要素の場合
      dir = pt_port->portref_dir(0);
      low_conn = instantiate_portref(module, pt_portref);
    }
    else if ( n > 1 ) {
      // 複数要素の結合の場合
      vector<ElbExpr*> expr_list(n);
      vector<ElbExpr*> lhs_elem_array(n);
      for ( SizeType i = 0; i < n; ++ i ) {
	auto pt_portexpr = pt_port->portref_elem(i);
	auto portexpr = instantiate_portref(module, pt_portexpr);
	if ( !portexpr ) {
	  return;
	}
	expr_list[i] = portexpr;
	lhs_elem_array[n - i - 1] = portexpr;

	auto dir1 = pt_port->portref_dir(i);
	if ( dir == VpiDir::NoDirection ) {
	  dir = dir1;
	}
	else if ( dir != dir1 ) {
	  dir = VpiDir::MixedIO;
	}
      }

      low_conn = factory().new_Lhs(pt_portref, expr_list, lhs_elem_array);
    }
    module->init_port(index, pt_port, low_conn, dir);
    ++ index;
  }
}

// PtPortRef から expression を生成する．
ElbExpr*
ModuleGen::instantiate_portref(ElbModule* module,
			       const PtExpr* pt_portref)
{
  auto name = pt_portref->name();
  auto handle = find_obj(module, name);
  if ( !handle ) {
    error_not_found(pt_portref->file_region(), name);
    return nullptr;
  }

  if ( handle->declarray() ) {
    error_port_array(pt_portref->file_region(), handle->declarray());
    return nullptr;
  }

  auto decl = handle->decl();
  if ( decl == nullptr ) {
    error_illegal_port(pt_portref->file_region(), name);
    return nullptr;
  }

  auto primary = factory().new_Primary(pt_portref, decl);

  // 添字の部分を実体化する．
  const PtExpr* pt_index = nullptr;
  if ( pt_portref->index_num() == 0 ) {
    pt_index = pt_portref->index(0);
  }
  auto pt_left = pt_portref->left_range();
  auto pt_right = pt_portref->right_range();
  if ( pt_index ) {
    int index_val;
    bool stat = evaluate_int(module, pt_index, index_val, true);
    if ( !stat ) {
      return nullptr;
    }

    SizeType offset;
    bool stat2 = decl->calc_bit_offset(index_val, offset);
    if ( !stat2 ) {
      // 添字が範囲外
      warning_index_out_of_range(pt_index->file_region());
    }
    return factory().new_BitSelect(pt_portref, primary, pt_index, index_val);
  }
  if ( pt_left && pt_right ) {
    int left_val = 0;
    int right_val = 0;
    if ( !evaluate_range(module, pt_left, pt_right, left_val, right_val) ) {
      return nullptr;
    }

    SizeType offset;
    bool stat1 = decl->calc_bit_offset(left_val, offset);
    if ( !stat1 ) {
      // 左の添字が範囲外
      warning_left_index_out_of_range(pt_left->file_region());
    }
    bool stat2 = decl->calc_bit_offset(right_val, offset);
    if ( !stat2 ) {
      // 右の添字が範囲外
      warning_right_index_out_of_range(pt_right->file_region());
    }
    return factory().new_PartSelect(pt_portref, primary,
				    pt_left, pt_right,
				    left_val, right_val);
  }
  return primary;
}

// @brief パラメータポートの割り当て数が多すぎる．
// @param[in] param_con_list パラメータポートの割り当てリスト
void
ModuleGen::error_too_many_param(const vector<ElbParamCon>& param_con_list)
{
  auto last = param_con_list.back();
  MsgMgr::put_msg(__FILE__, __LINE__,
		  last.mPtCon->file_region(),
		  MsgType::Error,
		  "ELAB",
		  "Too many parameters.");
}

// @brief パラメータポートに現れるパラメータが存在しない．
// @param[in] pt_con パラメータポート割り当てのパース木
// @param[in] name パラメータ名
void
ModuleGen::error_no_param(const PtConnection* pt_con,
			  const char* name)
{
  ostringstream buf;
  buf << name << " : No such parameter.";
  MsgMgr::put_msg(__FILE__, __LINE__,
		  pt_con->file_region(),
		  MsgType::Error,
		  "ELAB",
		  buf.str());
}

// @brief 対象の要素が見つからない．
// @param[in] file_region ファイル位置
// @param[in] name 名前
void
ModuleGen::error_not_found(const FileRegion& file_region,
			   const char* name)
{
  ostringstream buf;
  buf << name
      << ": Not found.";
  MsgMgr::put_msg(__FILE__, __LINE__,
		  file_region,
		  MsgType::Error,
		  "ELAB",
		  buf.str());
}

// @brief ポートに配列が使われている．
// @param[in] file_region ファイル位置
// @param[in] array 配列
void
ModuleGen::error_port_array(const FileRegion& file_region,
			    const VlDeclArray* array)
{
  ostringstream buf;
  buf << array->full_name()
      << ": Array shall not be connected to a module port.";
  MsgMgr::put_msg(__FILE__, __LINE__,
		  file_region,
		  MsgType::Error,
		  "ELAB",
		  buf.str());
}

// @brief ポートに使われている要素が宣言要素でなかった．
// @param[in] file_region ファイル位置
// @param[in] name 名前
void
ModuleGen::error_illegal_port(const FileRegion& file_region,
			      const char* name)
{
  ostringstream buf;
  buf << name
      << ": Illegal type for port connection.";
  MsgMgr::put_msg(__FILE__, __LINE__,
		  file_region,
		  MsgType::Error,
		  "ELAB",
		  buf.str());
}

// @brief 添字が範囲外
// @param[in] file_region ファイル位置
void
ModuleGen::warning_index_out_of_range(const FileRegion& file_region)
{
  MsgMgr::put_msg(__FILE__, __LINE__,
		  file_region,
		  MsgType::Warning,
		  "ELAB",
		  "Index is out of range.");
}

// @brief 左の範囲が範囲外
// @param[in] file_region ファイル位置
void
ModuleGen::warning_left_index_out_of_range(const FileRegion& file_region)
{
  MsgMgr::put_msg(__FILE__, __LINE__,
		  file_region,
		  MsgType::Warning,
		  "ELAB",
		  "Left index is out of range.");
}

// @brief 右の範囲が範囲外
// @param[in] file_region ファイル位置
void
ModuleGen::warning_right_index_out_of_range(const FileRegion& file_region)
{
  MsgMgr::put_msg(__FILE__, __LINE__,
		  file_region,
		  MsgType::Warning,
		  "ELAB",
		  "Right index is out of range.");
}

END_NAMESPACE_YM_VERILOG
