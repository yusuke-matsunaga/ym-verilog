﻿
/// @file ItemGen.cc
/// @brief ElbMgr の実装ファイル(要素の実体化)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ItemGen.h"
#include "DeclGen.h"
#include "ElbEnv.h"

#include "ym/BitVector.h"

#include "ym/pt/PtItem.h"
#include "ym/pt/PtExpr.h"
#include "ym/pt/PtMisc.h"

#include "ym/vl/VlModule.h"

#include "elb/ElbParameter.h"
#include "elb/ElbContAssign.h"
#include "elb/ElbProcess.h"
#include "elb/ElbGfRoot.h"
#include "elb/ElbGenvar.h"
#include "elb/ElbExpr.h"

#include "elb/ElbStub.h"

#include "ym/MsgMgr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// item 関係の生成を行うクラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] elab 生成器
// @param[in] elb_mgr Elbオブジェクトを管理するクラス
// @param[in] elb_factory Elbオブジェクトを生成するファクトリクラス
ItemGen::ItemGen(Elaborator& elab,
		 ElbMgr& elb_mgr,
		 ElbFactory& elb_factory) :
  ElbProxy(elab, elb_mgr, elb_factory)
{
}

// @brief デストラクタ
ItemGen::~ItemGen()
{
}

// @brief スコープに関係する要素を実体化する．
// @param[in] parent 親のスコープ
// @param[in] pt_item_array 要素定義の配列
void
ItemGen::phase1_item(const VlNamedObj* parent,
		     const vector<const PtItem*>& pt_item_array)
{
  for ( auto pt_item: pt_item_array ) {
    switch ( pt_item->type() ) {
    case PtItemType::DefParam:
      // 実際には登録するだけ
      add_defparamstub(parent->parent_module(), pt_item);
      break;

    case PtItemType::ContAssign:
      // phase3 で処理する．
      add_phase3stub(make_stub(this, &ItemGen::instantiate_cont_assign,
			       parent, pt_item));
      break;

    case PtItemType::Initial:
    case PtItemType::Always:
      phase1_stmt(parent, pt_item->body());
      // 本体の生成は phase3 で処理する．
      add_phase3stub(make_stub(this, &ItemGen::instantiate_process,
			       parent, pt_item));
      break;

    case PtItemType::Task:
    case PtItemType::Func:
      phase1_tf(parent, pt_item);
      break;

    case PtItemType::GateInst:
      // 今すぐには処理できないのでキューに積む．
      add_phase2stub(make_stub(this, &ItemGen::instantiate_gateheader,
			       parent, pt_item));
      break;

    case PtItemType::MuInst:
      phase1_muheader(parent, pt_item);
      break;

    case PtItemType::Generate:
      // 実際にはキューに積まれるだけ
      add_phase1stub(make_stub(this, &ItemGen::phase1_generate,
			       parent, pt_item));
      break;

    case PtItemType::GenBlock:
      phase1_genblock(parent, pt_item);
      break;

    case PtItemType::GenIf:
      phase1_genif(parent, pt_item);
      break;

    case PtItemType::GenCase:
      phase1_gencase(parent, pt_item);
      break;

    case PtItemType::GenFor:
      phase1_genfor(parent, pt_item);
      break;

    case PtItemType::SpecItem:
      // 未対応
      break;

    case PtItemType::SpecPath:
      // 未対応
      break;

    default:
      break;
    }
  }
}

// defparam 文によるパラメータ割り当てを行う．
// 該当するパラメータが見つかったら true を返す．
bool
ItemGen::defparam_override(const VlModule* module,
			   const PtItem* pt_header,
			   const PtDefParam* pt_defparam,
			   const VlNamedObj* ulimit)
{
  const FileRegion& fr = pt_defparam->file_region();

  ObjHandle* handle = find_obj_up(module, pt_defparam, ulimit);
  if ( !handle ) {
    return false;
  }
  ElbParameter* param = handle->parameter();
  if ( !param ) {
    ostringstream buf;
    buf << "\"" << pt_defparam->fullname() << "\" is not a parameter.";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    fr,
		    MsgType::Error,
		    "ELAB",
		    buf.str());

    // もうこれ以降は処理したくないので true を返す．
    return true;
  }

  if ( param->is_local_param() ) {
    ostringstream buf;
    buf << "\"" << param->name()
	<< "\" is a localparam, which shall not be override.";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    fr,
		    MsgType::Error,
		    "ELAB",
		    buf.str());

    // もうこれ以降は処理したくないので true を返す．
    return true;
  }

  const PtExpr* rhs_expr = pt_defparam->expr();

  ostringstream buf;
  buf << "instantiating defparam: " << param->full_name()
      << " = " << rhs_expr->decompile() << ".";
  MsgMgr::put_msg(__FILE__, __LINE__,
		  fr,
		  MsgType::Info,
		  "ELAB",
		  buf.str());

  auto value = evaluate_expr(module, rhs_expr, true);
  param->set_expr(rhs_expr, value);


  auto dp = factory().new_DefParam(module,
				   pt_header,
				   pt_defparam,
				   param, rhs_expr,
				   value);
  reg_defparam(dp);

  return true;
}

// @brief continous assignment に関連した式の名前解決を行う．
// @param[in] parent 親のスコープ
// @param[in] pt_header ヘッダ
void
ItemGen::instantiate_cont_assign(const VlNamedObj* parent,
				 const PtItem* pt_header)
{
  const PtDelay* pt_delay = pt_header->delay();
  const VlDelay* delay = nullptr;
  if ( pt_delay ) {
    delay = instantiate_delay(parent, pt_delay);
  }

  const VlModule* module = parent->parent_module();
  ElbCaHead* ca_head = factory().new_CaHead(module, pt_header, delay);

  ElbEnv env;
  ElbNetLhsEnv env1(env);
  for ( auto pt_elem: pt_header->contassign_list() ) {
    // 左辺式の生成
    const PtExpr* pt_lhs = pt_elem->lhs();
    ElbExpr* lhs = instantiate_lhs(parent, env1, pt_lhs);
    if ( !lhs ) {
      return;
    }

    // 右辺式の生成
    const PtExpr* pt_rhs = pt_elem->rhs();
    ElbExpr* rhs = instantiate_rhs(parent, env, pt_rhs, lhs);
    if ( !rhs ) {
      return;
    }

    auto ca = factory().new_ContAssign(ca_head, pt_elem, lhs, rhs);
    reg_contassign(ca);

    ostringstream buf;
    buf << "instantiating continuous assign: "
	<< lhs->decompile() << " = " << rhs->decompile() << ".";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    pt_elem->file_region(),
		    MsgType::Info,
		    "ELAB",
		    buf.str());
  }
}

// @brief process 文の生成を行う．
// @param[in] parent 親のスコープ
// @param[in] pt_item パース木の定義
void
ItemGen::instantiate_process(const VlNamedObj* parent,
			     const PtItem* pt_item)
{
  ElbProcess* process = factory().new_Process(parent, pt_item);
  reg_process(process);

  ElbEnv env;
  ElbStmt* body = instantiate_stmt(parent, process, env,
				   pt_item->body());
  process->set_stmt(body);
}

// @brief generate block を実際にインスタンス化を行う．
// @param[in] parent 親のスコープ
// @param[in] pt_generate generate block 定義
void
ItemGen::phase1_generate(const VlNamedObj* parent,
			 const PtItem* pt_generate)
{
  phase1_genitem(parent,
		 pt_generate->declhead_list(),
		 pt_generate->item_list());
}

// @brief PtGenBlock に対応するインスタンスの生成を行う
// @param[in] parent 親のスコープ
// @param[in] pt_genblock generate block 定義
void
ItemGen::phase1_genblock(const VlNamedObj* parent,
			 const PtItem* pt_genblock)
{
  const char* name = pt_genblock->name();
  if ( name != nullptr ) {
    const VlNamedObj* genblock = factory().new_GenBlock(parent, pt_genblock);
    reg_internalscope(genblock);

    parent = genblock;
  }
  phase1_generate(parent, pt_genblock);
}

// @brief generate if に対応するインスタンスの生成を行う
// @param[in] parent 親のスコープ
// @parma[in] pt_genif generate if 定義
void
ItemGen::phase1_genif(const VlNamedObj* parent,
		      const PtItem* pt_genif)
{
  bool cond;
  if ( !evaluate_bool(parent, pt_genif->expr(), cond, true) ) {
    return;
  }
  if ( cond ) {
    phase1_genitem(parent,
		   pt_genif->then_declhead_list(),
		   pt_genif->then_item_list());
  }
  else {
    phase1_genitem(parent,
		   pt_genif->else_declhead_list(),
		   pt_genif->else_item_list());
  }
}

// @brief generate case に対応するインスタンスの生成を行う
// @param[in] parent 親のスコープ
// @parma[in] pt_gencase generate case 定義
void
ItemGen::phase1_gencase(const VlNamedObj* parent,
			const PtItem* pt_gencase)
{
  BitVector val;
  if ( !evaluate_bitvector(parent, pt_gencase->expr(), val, true) ) {
    return;
  }

  bool found = false;
  for ( auto pt_caseitem: pt_gencase->caseitem_list() ) {
    // default(ラベルリストが空) なら常にマッチする．
    SizeType n = pt_caseitem->label_num();
    bool match = (n == 0);
    for ( auto pt_expr: pt_caseitem->label_list() ) {
      BitVector label_val;
      if ( !evaluate_bitvector(parent, pt_expr, label_val, true) ) {
	continue;
      }
      if ( label_val == val ) {
	match = true;
	break;
      }
    }
    if ( match ) {
      if ( found ) {
	MsgMgr::put_msg(__FILE__, __LINE__,
			pt_gencase->file_region(),
			MsgType::Error,
			"ELAB",
			"Matches more than one labels.");
	return;
      }
      found = true;
      phase1_genitem(parent,
		     pt_caseitem->declhead_list(),
		     pt_caseitem->item_list());
    }
  }
}

// @brief generate for に対応するインスタンスの生成を行う
// @param[in] parent 親のスコープ
// @parma[in] pt_genfor generate for 定義
void
ItemGen::phase1_genfor(const VlNamedObj* parent,
		       const PtItem* pt_genfor)
{
  const FileRegion& fr = pt_genfor->file_region();

  const char* name0 = pt_genfor->name();
  ASSERT_COND(name0 != nullptr );

  ObjHandle* handle = find_obj(parent, pt_genfor->loop_var());
  if ( !handle ) {
    ostringstream buf;
    buf << pt_genfor->loop_var() << " : Not found.";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    fr,
		    MsgType::Error,
		    "ELAB",
		    buf.str());
    return;
  }
  ElbGenvar* genvar = handle->genvar();
  if ( !genvar ) {
    ostringstream buf;
    buf << pt_genfor->loop_var() << " : Not a genvar.";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    fr,
		    MsgType::Error,
		    "ELAB",
		    buf.str());
    return;
  }
  if ( genvar->is_inuse() ) {
    // すでに他の generate-for loop が使用中
    ostringstream buf;
    buf << pt_genfor->loop_var() << " : Already in use.";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    fr,
		    MsgType::Error,
		    "ELAB",
		    buf.str());
    return;
  }

  // 子供のスコープの検索用オブジェクト
  ElbGfRoot* gfroot = factory().new_GfRoot(parent, pt_genfor);
  reg_gfroot(gfroot);

  {
    int val;
    if ( !evaluate_int(parent, pt_genfor->init_expr(), val, true) ) {
      return;
    }
    if ( val < 0 ) {
      MsgMgr::put_msg(__FILE__, __LINE__,
		      fr,
		      MsgType::Error,
		      "ELAB",
		      "genvar should not be negative.");
      return;
    }
    genvar->set_value(val);
    genvar->set_inuse();
  }

  for ( ; ; ) {
    // 終了条件のチェック
    bool val;
    if ( !evaluate_bool(parent, pt_genfor->expr(), val, true) ) {
      break;
    }
    if ( !val ) {
      break;
    }

    // スコープ名生成のために genvar の値を取得
    {
      int gvi = genvar->value();
      const VlNamedObj* genblock = factory().new_GfBlock(parent, pt_genfor, gvi);
      gfroot->add(gvi, genblock);
      reg_internalscope(genblock);

      const PtDeclItem* pt_item = genvar->pt_item();
      ElbGenvar* genvar1 = factory().new_Genvar(genblock, pt_item, gvi);
      reg_genvar(genvar1);

      phase1_generate(genblock, pt_genfor);
    }

    // genvar の増加分の処理．
    {
      int val;
      if ( !evaluate_int(parent, pt_genfor->next_expr(), val, true) ) {
	break;
      }
      if ( val < 0 ) {
	MsgMgr::put_msg(__FILE__, __LINE__,
			fr,
			MsgType::Error,
			"ELAB",
			"genvar should not be negative.");
	break;
      }
      genvar->set_value(val);
    }
  }
  genvar->reset_inuse();
}

// @brief generate block の要素でスコープに関連するものの生成を行う．
// @note と同時の残りの処理をキューに積む．
// @param[in] parent 親のスコープ
// @param[in] pt_decl_array パース木の宣言の配列
// @param[in] pt_item_array パース木の要素の配列
void
ItemGen::phase1_genitem(const VlNamedObj* parent,
			const vector<const PtDeclHead*>& pt_decl_array,
			const vector<const PtItem*>& pt_item_array)
{
  phase1_item(parent, pt_item_array);
  auto stub{make_stub<ElbProxy,
	    const VlNamedObj*,
	    const vector<const PtDeclHead*>&>(static_cast<ElbProxy*>(this),
					      &ElbProxy::instantiate_decl,
					      parent, pt_decl_array)};
  add_phase2stub(stub);
}

END_NAMESPACE_YM_VERILOG
