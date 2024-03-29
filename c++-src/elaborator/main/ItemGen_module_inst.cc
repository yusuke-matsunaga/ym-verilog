﻿
/// @file ItemGen_module_inst.cc
/// @brief ElbMgr の実装ファイル(モジュールインスタンスの実体化)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.

#include "ItemGen.h"
#include "ElbEnv.h"
#include "ElbParamCon.h"
#include "ErrorGen.h"

#include "ym/BitVector.h"

#include "ym/pt/PtModule.h"
#include "ym/pt/PtPort.h"
#include "ym/pt/PtItem.h"
#include "ym/pt/PtExpr.h"
#include "ym/pt/PtMisc.h"

#include "ym/vl/VlPort.h"

#include "elaborator/ElbModule.h"
#include "elaborator/ElbModuleArray.h"
#include "elaborator/ElbExpr.h"

#include "ym/MsgMgr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// インスタンス関係の instantiate 関数 (変な日本語)
//////////////////////////////////////////////////////////////////////

// @brief module instance のスコープ生成を行う．
void
ItemGen::phase1_muheader(
  const VlScope* parent,
  const PtItem* pt_head
)
{
  auto defname = pt_head->name();

  // モジュールを探す
  auto pt_module = find_moduledef(defname);
  if ( pt_module ) {
    // モジュール定義が見つかった．
    phase1_module(parent, pt_head, pt_module);
    return;
  }

  // 次に udp を探す．
  auto udpdefn = mgr().find_udp(defname);
  if ( udpdefn ) {
    phase1_udp(parent, pt_head, udpdefn);
    return;
  }

  // 正式な仕様にはないが，セルライブラリを探す．
  auto cell = find_cell(defname);
  if ( cell.is_valid() ) {
    phase1_cell(parent, pt_head, cell);
    return;
  }

  // どれもなければエラー
  ErrorGen::instance_not_found(__FILE__, __LINE__, pt_head);
}

// @brief module instance の生成を行う．
void
ItemGen::phase1_module(
  const VlScope* parent,
  const PtItem* pt_head,
  const PtModule* pt_module
)
{
  if ( pt_module->is_in_use() ) {
    // 依存関係が循環している．
    ErrorGen::cyclic_dependency(__FILE__, __LINE__, pt_module);
  }

  for ( auto pt_inst: pt_head->inst_list() ) {
    auto name = pt_inst->name();
    if ( name == nullptr ) {
      // 名無しのモジュールインスタンスはない
      ErrorGen::noname_module(__FILE__, __LINE__, pt_inst);
    }

    auto pt_left = pt_inst->left_range();
    auto pt_right = pt_inst->right_range();
    if ( pt_left && pt_right ) {
      // 配列型は今すぐにはインスタンス化できない．
      add_phase1stub(make_stub(this, &ItemGen::phase1_module_array,
			       parent, pt_module, pt_head, pt_inst));
    }
    else {
      // 単一の要素
      auto module1 = mgr().new_Module(parent,
				      pt_module,
				      pt_head,
				      pt_inst);

      // attribute instance の生成
      auto attr_list = attribute_list(pt_module, pt_head);
      mgr().reg_attr(module1, attr_list);

      {
	ostringstream buf;
	buf << "\"" << module1->full_name() << "\" has been created.";
	MsgMgr::put_msg(__FILE__, __LINE__,
			pt_inst->file_region(),
			MsgType::Info,
			"ELAB",
			buf.str());
      }

      // パラメータ割り当て式の生成
      auto param_con_list = gen_param_con_list(parent, pt_head);
      phase1_module_item(module1, pt_module, param_con_list);

      add_phase3stub(make_stub(this, &ItemGen::link_module,
			       module1, pt_module, pt_inst));
    }
  }
}

// @brief module array のインスタンス化を行う．
void
ItemGen::phase1_module_array(
  const VlScope* parent,
  const PtModule* pt_module,
  const PtItem* pt_head,
  const PtInst* pt_inst
)
{
  auto defname = pt_head->name();

  auto name = pt_inst->name();
  auto pt_left = pt_inst->left_range();
  auto pt_right = pt_inst->right_range();

  int left_val;
  int right_val;
  tie(left_val, right_val) = evaluate_range(parent, pt_left, pt_right);
  auto module_array = mgr().new_ModuleArray(parent,
					    pt_module,
					    pt_head,
					    pt_inst,
					    pt_left, pt_right,
					    left_val, right_val);

  {
    ostringstream buf;
    buf << "instantiating module array \"" << name << "\" of \""
	<< defname << "\" [" << left_val << " : " << right_val << "].";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    pt_head->file_region(),
		    MsgType::Info,
		    "ELAB",
		    buf.str());
  }

  add_phase3stub(make_stub(this, &ItemGen::link_module_array,
			   module_array, pt_module, pt_inst));

  // パラメータ割り当て式の生成
  auto param_con_list = gen_param_con_list(parent, pt_head);
  SizeType n = module_array->elem_num();
  auto attr_list = attribute_list(pt_module, pt_head);
  for ( SizeType i = 0; i < n; ++ i ) {
    auto module = module_array->elem(i);

    {
      ostringstream buf;
      buf << "\"" << module->full_name() << "\" has been created.";
      MsgMgr::put_msg(__FILE__, __LINE__,
		      module_array->file_region(),
		      MsgType::Info,
		      "ELAB",
		      buf.str());
    }

    // モジュール要素を作る．
    phase1_module_item(module, pt_module, param_con_list);
    // attribute instance の登録
    mgr().reg_attr(module, attr_list);
  }
}

// @brief UDP instance の生成を行う．
void
ItemGen::phase1_udp(
  const VlScope* parent,
  const PtItem* pt_head,
  const VlUdpDefn* udpdefn
)
{
  // この場合, mParamList は空でなければならない．
  // 問題は delay が mParamList に見える場合があるということ．
  auto pa_array = pt_head->paramassign_list();
  SizeType param_size = pa_array.size();
  auto pt_delay = pt_head->delay();
  if ( param_size > 0 && pa_array[0]->name() != nullptr ) {
    ErrorGen::udp_with_named_paramassign(__FILE__, __LINE__, pt_head);
  }
  if ( (pt_delay && param_size > 0) || param_size > 1 ) {
    ErrorGen::udp_with_ordered_paramassign(__FILE__, __LINE__, pt_head);
  }

  // 今すぐには処理できないのでキューに積む．
  add_phase2stub(make_stub(this, &ItemGen::instantiate_udpheader,
			   parent, pt_head, udpdefn));
}

// @brief cell instance の生成を行う．
void
ItemGen::phase1_cell(
  const VlScope* parent,
  const PtItem* pt_head,
  ClibCell cell
)
{
  // この場合, parameter 割り当てリストは空でなければならない．
  auto pa_array = pt_head->paramassign_list();
  if ( pa_array.size() > 0 ) {
    ErrorGen::cell_with_paramassign(__FILE__, __LINE__, pt_head);
  }

  // 今すぐには処理できないのでキューに積む．
  add_phase2stub(make_stub(this, &ItemGen::instantiate_cell,
			   parent, pt_head, cell));
}

// @brief module array instance の入出力端子の接続を行う．
void
ItemGen::link_module_array(
  ElbModuleArray* module_array,
  const PtModule* pt_module,
  const PtInst* pt_inst
)
{
  auto parent = module_array->parent_scope();

  SizeType module_size = module_array->elem_num();
  auto module0 = module_array->elem_by_offset(0);
  SizeType port_num = module0->port_num();

  auto port_list = pt_inst->port_list();
  SizeType n = port_list.size();

  // ポートの割り当てを行う．
  // 例外: ポートを一つも取らないモジュールの場合
  // module_name instance_name ()
  // という形になって一つのポートが省略された形と区別がつかない．
  // これは Verilog-HDL の仕様がアホ
  // () を取らない形を用意しておけば良かったのに．
  if ( port_num == 0 && n == 1 ) {
    auto con = port_list[0];
    if ( /* con->attr_top() == nullptr &&*/
	 con->name() == nullptr &&
	 con->expr() == nullptr ) {
      // この要素は無視する．
      return;
    }
  }
  if ( n > port_num ) {
    ErrorGen::too_many_items_in_port_list(__FILE__, __LINE__, pt_inst);
  }
  // どうやら実際のポート数よりも少ないのはいいらしい

  // YACC の文法から一つでも named_con なら全部そう
  bool conn_by_name = (port_list[0]->name() != nullptr);
  unordered_map<string, int> port_index;
  if ( conn_by_name ) {
    // ポート名とインデックスの辞書を作る．
    SizeType index{0};
    for ( auto pt_port: pt_module->port_list() ) {
      auto name = pt_port->ext_name();
      if ( name != nullptr ) {
	port_index[string(name)] = index;
      }
      ++ index;
    }
  }

  // ポートに接続する式を生成する．
  ElbEnv env;
  SizeType pos{0};
  for ( auto pt_con: pt_inst->port_list() ) {
    auto pt_expr = pt_con->expr();
    if ( !pt_expr ) {
      continue;
    }

    // この式が対応するインデックス
    SizeType index;
    if ( conn_by_name ) {
      // 名前による割り当ての場合はポート名で探す．
      auto port_name = pt_con->name();
      ASSERT_COND( port_name != nullptr );
      if ( port_index.count(port_name) == 0 ) {
	ErrorGen::illegal_port_name(__FILE__, __LINE__, pt_con);
      }
      index = port_index.at(port_name);
      ASSERT_COND ( index < port_num );
    }
    else {
      // 順序に割り当ての場合は単純に pos
      index = pos;
      // 前にも書いたように YACC の文法で規定されているのでこれは常に真
      ASSERT_COND ( pt_con->name() == nullptr );
      ++ pos;
    }

    // 割り当てるポートを取り出す．
    auto port = module0->port(index);
    if ( port == nullptr ) {
      // このポートはダミー
      continue;
    }

    SizeType port_size = port->bit_size();
    if ( port->direction() == VpiDir::Input ) {
      // 入力ポートには任意の式を接続できる．
      auto tmp = instantiate_expr(parent, env, pt_expr);
      auto type = tmp->value_type();
      // ただし real 型は駄目
      if ( type.is_real_type() ) {
	ErrorGen::real_type_in_port_list(__FILE__, __LINE__, tmp);
      }

      SizeType expr_size = type.size();
      if ( expr_size == 0 ) {
	// もともとサイズがなければ port_size に合わせる．
	tmp->set_reqsize(VlValueType(false, true, port_size));
	expr_size = port_size;
      }

      // 配列型インスタンスの場合 expr_size に制限がある．
      if ( port_size == expr_size ) {
	// サイズが等しい場合はそのまま接続する．
	for ( SizeType i = 0; i < module_size; ++ i ) {
	  auto module = module_array->elem(i);
	  module->set_port_high_conn(index, tmp, conn_by_name);
	}
      }
      else if ( port_size * module_size == expr_size ) {
	ASSERT_COND( module_size > 1 );
	// tmp を 分割する．
	for ( SizeType i = 0; i < module_size; ++ i ) {
	  ElbExpr* tmp1{nullptr};
	  if ( port_size == 1 ) {
	    tmp1 = mgr().new_BitSelect(pt_expr, tmp, i);
	  }
	  else {
	    int lsb = i;
	    int msb = lsb + port_size - 1;
	    tmp1 = mgr().new_PartSelect(pt_expr, tmp, msb, lsb);
	  }
	  auto module = module_array->elem(i);
	  module->set_port_high_conn(index, tmp1, conn_by_name);
	}
      }
      else {
	ErrorGen::port_size_mismatch(__FILE__, __LINE__, pt_expr,
				     module_array->full_name(), index);
      }
    }
    else {
      // それ以外の場合には左辺式のみが接続できる．
      auto tmp = instantiate_lhs(parent, env, pt_expr);
      auto type = tmp->value_type();
      if ( type.is_real_type() ) {
	ErrorGen::real_type_in_port_list(__FILE__, __LINE__, tmp);
      }

      SizeType expr_size = type.size();
      if ( expr_size == port_size ) {
	// 式のサイズとポートサイズが等しければ全部のモジュールに
	// 同一の式を接続する．
	// 普通に考えていいアイデアとは思えない．
	for ( SizeType i = 0; i < module_size; ++ i ) {
	  auto module = module_array->elem(i);
	  module->set_port_high_conn(index, tmp, conn_by_name);
	}
      }
      else if ( expr_size == port_size * module_size ) {
	// 式を分割する．
	// 0番目のモジュールがLSB側になる．
	for ( SizeType i = 0; i < module_size; ++ i ) {
	  auto module = module_array->elem(i);
	  int index1 = i * port_size;
	  int index2 = index2 + port_size - 1;
	  auto expr1 = mgr().new_PartSelect(pt_expr, tmp, index1, index2);
	  module->set_port_high_conn(index, expr1, conn_by_name);
	}
      }
      else {
	// サイズが合わない．
	ErrorGen::port_size_mismatch(__FILE__, __LINE__, pt_expr,
				     module_array->full_name(), index);
      }
    }

    // attribute instance の設定
    for ( SizeType i = 0; i < module_size; ++ i ) {
      auto module = module_array->elem(i);
      auto port = module->port(index);

      // attribute instance の生成
      auto attr_list = attribute_list(pt_con);
      mgr().reg_attr(port, attr_list);
    }
  }
}

// @brief module instance の入出力端子の接続を行う．
void
ItemGen::link_module(
  ElbModule* module,
  const PtModule* pt_module,
  const PtInst* pt_inst
)
{
  auto parent = module->parent_scope();
  SizeType port_num = module->port_num();
  auto port_list = pt_inst->port_list();
  SizeType n = port_list.size();
  // ポートの割り当てを行う．
  // 例外: ポートを一つも取らないモジュールの場合
  // module_name instance_name ()
  // という形になって一つのポートが省略された形と区別がつかない．
  // これは Verilog-HDL の仕様がアホ
  // () を取らない形を用意しておけば良かったのに．
  if ( port_num == 0 && n == 1 ) {
    auto pt_con = port_list[0];
    if ( /* pt_con->attr_top() == nullptr &&*/
	 pt_con->name() == nullptr &&
	 pt_con->expr() == nullptr ) {
      // この要素は無視する．
      return;
    }
  }
  if ( n > port_num ) {
    ErrorGen::too_many_items_in_port_list(__FILE__, __LINE__, pt_inst);
  }
  // どうやら実際のポート数よりも少ないのはいいらしい

  // YACC の文法から一つでも named_con なら全部そう
  bool conn_by_name = (port_list[0]->name() != nullptr);
  unordered_map<string, SizeType> port_index;
  if ( conn_by_name ) {
    // ポート名とインデックスの辞書を作る．
    SizeType index{0};
    for ( auto pt_port: pt_module->port_list() ) {
      auto name = pt_port->ext_name();
      if ( name != nullptr ) {
	port_index[string(name)] = index;
      }
      ++ index;
    }
  }

  // ポートに接続する式を生成する．
  ElbEnv env;
  SizeType pos{0};
  for ( auto pt_con: pt_inst->port_list() ) {
    auto pt_expr = pt_con->expr();
    if ( !pt_expr ) {
      continue;
    }

    // この式に対応するポート番号を求める．
    int index{-1};
    if ( conn_by_name ) {
      // 名前による割り当ての場合はポート名で探す．
      auto port_name = pt_con->name();
      ASSERT_COND( port_name != nullptr );
      if ( port_index.count(port_name) == 0 ) {
	ErrorGen::illegal_port_name(__FILE__, __LINE__, pt_con);
      }
      index = port_index.at(port_name);
      ASSERT_COND( 0 <= index && index < port_num );
    }
    else {
      // 順序による割り当ての場合は単純に pos
      index = pos;
      // 前にも書いたように YACC の文法から下の仮定は常になりたつはず．
      ASSERT_COND( pt_con->name() == nullptr );
      ++ pos;
    }

    auto port = module->port(index);
    if ( !port ) {
      // このポートはダミー
      continue;
    }

    SizeType port_size = port->bit_size();
    if ( port->direction() == VpiDir::Input ) {
      // 入力ポートには任意の式を接続できる．
      auto tmp = instantiate_expr(parent, env, pt_expr);
      auto type = tmp->value_type();
      if ( type.is_real_type() ) {
	// ただし real 型は駄目
	ErrorGen::real_type_in_port_list(__FILE__, __LINE__, tmp);
      }

      SizeType expr_size = type.size();

      // 単独のインスタンスの場合 expr のサイズは補正される．
      // ... でいいんだよね．
      if ( port_size != expr_size ) {
	if ( expr_size != 0 ) {
	  {
	    ostringstream buf;
	    buf << "port_size: " << port_size << ", expr_size: " << expr_size;
	    MsgMgr::put_msg(__FILE__, __LINE__,
			    pt_expr->file_region(),
			    MsgType::Debug,
			    "ELAB",
			    buf.str());
	  }
	  ErrorGen::port_size_mismatch(__FILE__, __LINE__, pt_expr,
				       module->full_name(), index);
	}
	tmp->set_reqsize(VlValueType(false, true, port_size));
      }
      module->set_port_high_conn(index, tmp, conn_by_name);
    }
    else {
      // それ以外のポートに接続できるのは左辺式だけ．
      auto tmp = instantiate_lhs(parent, env, pt_expr);
      auto type = tmp->value_type();
      if ( type.is_real_type() ) {
	ErrorGen::real_type_in_port_list(__FILE__, __LINE__, tmp);
      }
      // 左辺はサイズの補正をしても意味がないのでそのまま接続する．
      module->set_port_high_conn(index, tmp, conn_by_name);
    }

    // attribute instance の生成
    auto attr_list = attribute_list(pt_con);
    mgr().reg_attr(port, attr_list);
  }
}

// @brief パラメータ割り当て情報を作る．
vector<ElbParamCon>
ItemGen::gen_param_con_list(
  const VlScope* parent,
  const PtItem* pt_head
)
{
  vector<ElbParamCon> param_con_list;
  auto pa_array = pt_head->paramassign_list();
  for ( auto pt_con: pa_array ) {
    auto pt_expr = pt_con->expr();
    auto value = evaluate_expr(parent, pt_expr);
    param_con_list.push_back({pt_con, pt_expr, value});
  }
  return param_con_list;
}

END_NAMESPACE_YM_VERILOG
