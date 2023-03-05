
/// @file ItemGen.cc
/// @brief ElbMgr の実装ファイル(要素の実体化)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.

#include "ItemGen.h"
#include "DeclGen.h"
#include "DefParamStub.h"
#include "ElbEnv.h"
#include "ElbStub.h"
#include "ElbError.h"
#include "ErrorGen.h"

#include "ym/BitVector.h"

#include "ym/pt/PtItem.h"
#include "ym/pt/PtExpr.h"
#include "ym/pt/PtMisc.h"

#include "ym/vl/VlModule.h"

#include "elaborator/ElbParameter.h"
#include "elaborator/ElbProcess.h"
#include "elaborator/ElbGfRoot.h"
#include "elaborator/ElbGenvar.h"
#include "elaborator/ElbExpr.h"


#include "ym/MsgMgr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// item 関係の生成を行うクラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ItemGen::ItemGen(
  Elaborator& elab,
  ElbMgr& elb_mgr
) : ElbProxy{elab, elb_mgr}
{
}

// @brief デストラクタ
ItemGen::~ItemGen()
{
}

// @brief スコープに関係する要素を実体化する．
void
ItemGen::phase1_items(
  const VlScope* parent,
  const vector<const PtItem*>& pt_item_array
)
{
  for ( auto pt_item: pt_item_array ) {
    phase1_item(parent, pt_item);
  }
}

// @brief スコープに関係する要素を実体化する．
void
ItemGen::phase1_item(
  const VlScope* parent,
  const PtItem* pt_item
)
{
  try {
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
      ASSERT_NOT_REACHED;
      break;
    }
  }
  catch ( const ElbError& error ) {
    put_error(error);
  }
}

// defparam 文によるパラメータ割り当てを行う．
bool
ItemGen::defparam_override(
  const DefParamStub& stub,
  const VlScope* ulimit
)
{
  auto module = stub.mModule;
  auto pt_header = stub.mPtHeader;
  auto pt_defparam = stub.mPtDefparam;

  auto handle = mgr().find_obj_up(module, pt_defparam, ulimit);
  if ( !handle ) {
    // 見つからなかった．
    return false;
  }

  auto param = handle->parameter();
  if ( !param ) {
    // 対象がパラメータではなかった．
    try {
      ErrorGen::not_a_parameter(__FILE__, __LINE__, pt_defparam);
    }
    // ただし無視する．
    catch ( const ElbError& error ) {
      put_error(error);
    }

    // もうこれ以降は処理したくないので true を返す．
    return true;
  }

  if ( param->is_local_param() ) {
    // 対象が localparam だった(書き換えできない)
    try {
      ErrorGen::is_a_localparam(__FILE__, __LINE__, pt_defparam);
    }
    // ただし無視する．
    catch ( const ElbError& error ) {
      put_error(error);
    }

    // もうこれ以降は処理したくないので true を返す．
    return true;
  }

  auto pt_rhs_expr = pt_defparam->expr();
  auto value = evaluate_expr(module, pt_rhs_expr);

  {
    ostringstream buf;
    buf << "instantiating defparam: " << param->full_name()
	<< " = " << pt_rhs_expr->decompile() << ".";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    pt_defparam->file_region(),
		    MsgType::Info,
		    "ELAB",
		    buf.str());
  }

  param->set_init_expr(pt_rhs_expr, value);

  auto dp = mgr().new_DefParam(module,
			       pt_header,
			       pt_defparam,
			       param, pt_rhs_expr, value);

  return true;
}

// @brief continous assignment に関連した式の名前解決を行う．
void
ItemGen::instantiate_cont_assign(
  const VlScope* parent,
  const PtItem* pt_header
)
{
  // delay の実体化でエラーが置きても nullptr になっているだけで処理を続ける．
  // エラーメッセージは出力されている．
  auto module = parent->parent_module();
  auto pt_delay = pt_header->delay();
  auto delay = instantiate_delay(parent, pt_delay);
  auto ca_head = mgr().new_CaHead(module, pt_header, delay);

  ElbEnv env;
  ElbNetLhsEnv env1(env);
  for ( auto pt_elem: pt_header->contassign_list() ) {
    try {
      // 左辺式の生成
      auto pt_lhs = pt_elem->lhs();
      auto lhs = instantiate_lhs(parent, env1, pt_lhs);

      // 右辺式の生成
      auto pt_rhs = pt_elem->rhs();
      auto rhs = instantiate_rhs(parent, env, pt_rhs, lhs);

      auto ca = mgr().new_ContAssign(ca_head, pt_elem, lhs, rhs);

      {
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
    catch ( const ElbError& error ) {
      put_error(error);
    }
  }
}

// @brief process 文の生成を行う．
void
ItemGen::instantiate_process(
  const VlScope* parent,
  const PtItem* pt_item
)
{
  try {
    auto process = mgr().new_Process(parent, pt_item);

    ElbEnv env;
    auto body = instantiate_stmt(parent, process, env,
				 pt_item->body());
    process->set_stmt(body);
  }
  catch ( const ElbError& error ) {
    put_error(error);
  }
}

// @brief generate block を実際にインスタンス化を行う．
void
ItemGen::phase1_generate(
  const VlScope* parent,
  const PtItem* pt_generate
)
{
  phase1_genitem(parent,
		 pt_generate->declhead_list(),
		 pt_generate->item_list());
}

// @brief PtGenBlock に対応するインスタンスの生成を行う
void
ItemGen::phase1_genblock(
  const VlScope* parent,
  const PtItem* pt_genblock
)
{
  auto* name = pt_genblock->name();
  if ( name != nullptr ) {
    parent = mgr().new_GenBlock(parent, pt_genblock);
  }
  phase1_generate(parent, pt_genblock);
}

// @brief generate if に対応するインスタンスの生成を行う
void
ItemGen::phase1_genif(
  const VlScope* parent,
  const PtItem* pt_genif
)
{
  auto pt_cond = pt_genif->expr();
  bool cond = evaluate_bool(parent, pt_cond);
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
void
ItemGen::phase1_gencase(
  const VlScope* parent,
  const PtItem* pt_gencase
)
{
  auto pt_expr = pt_gencase->expr();
  BitVector val{evaluate_bitvector(parent, pt_expr)};

  bool already_matched = false;
  for ( auto pt_caseitem: pt_gencase->caseitem_list() ) {
    // default(ラベルリストが空) なら常にマッチする．
    SizeType n = pt_caseitem->label_num();
    bool match = (n == 0);
    for ( auto pt_expr: pt_caseitem->label_list() ) {
      BitVector label_val{evaluate_bitvector(parent, pt_expr)};
      if ( label_val == val ) {
	match = true;
	break;
      }
    }
    if ( match ) {
      if ( already_matched ) {
	ErrorGen::duplicate_gencase_labels(__FILE__, __LINE__, pt_gencase);
      }
      else {
	already_matched = true;
	phase1_genitem(parent,
		       pt_caseitem->declhead_list(),
		       pt_caseitem->item_list());
      }
    }
  }
}

// @brief generate for に対応するインスタンスの生成を行う
void
ItemGen::phase1_genfor(
  const VlScope* parent,
  const PtItem* pt_genfor
)
{
  // Genvar を使用中にするオブジェクト
  // デストラクタの起動されるタイミングで
  // 使用中のフラグを降ろす．
  // このメカニズムにより例外が送出されても正しく働く
  class GenvarHolder
  {
  public:

    GenvarHolder(ElbGenvar* genvar) :
      mGenvar{genvar}
    {
      mGenvar->set_inuse();
    }

    ~GenvarHolder()
    {
      mGenvar->reset_inuse();
    }

  private:

    ElbGenvar* mGenvar;

  };

  auto name0 = pt_genfor->name();
  ASSERT_COND( name0 != nullptr );

  auto handle = mgr().find_obj(parent, pt_genfor->loop_var());
  if ( !handle ) {
    // 見つからなかった．
    ErrorGen::genvar_not_found(__FILE__, __LINE__, pt_genfor);
  }

  auto genvar = handle->genvar();
  if ( !genvar ) {
    // genvar ではなかった．
    ErrorGen::not_a_genvar(__FILE__, __LINE__, pt_genfor);
  }
  if ( genvar->is_inuse() ) {
    // すでに他の generate-for loop が使用中
    ErrorGen::genvar_in_use(__FILE__, __LINE__, pt_genfor);
  }

  // genvar を使用中にする．
  GenvarHolder holder(genvar);

  // 子供のスコープの検索用オブジェクト
  auto gfroot = mgr().new_GfRoot(parent, pt_genfor);

  auto pt_init_expr = pt_genfor->init_expr();
  int init_val{evaluate_int(parent, pt_init_expr)};
  if ( init_val < 0 ) {
    ErrorGen::genvar_negative(__FILE__, __LINE__, pt_genfor);
  }
  genvar->set_value(init_val);

  for ( ; ; ) {
    // 終了条件のチェック
    auto pt_cond_expr = pt_genfor->expr();
    bool cond_val = evaluate_bool(parent, pt_cond_expr);
    if ( !cond_val ) {
      break;
    }

    // スコープ名生成のために genvar の値を取得
    {
      int gvi = genvar->value();
      auto genblock = mgr().new_GfBlock(parent, pt_genfor, gvi);
      gfroot->add(gvi, genblock);

      auto pt_item = genvar->pt_item();
      auto genvar1 = mgr().new_Genvar(genblock, pt_item, gvi);

      phase1_generate(genblock, pt_genfor);
    }

    // genvar の増加分の処理．
    auto pt_next_expr = pt_genfor->next_expr();
    int next_val = evaluate_int(parent, pt_next_expr);
    if ( next_val < 0 ) {
      ErrorGen::genvar_negative(__FILE__, __LINE__, pt_genfor);
      genvar->set_value(next_val);
    }
  }
}

// @brief generate block の要素でスコープに関連するものの生成を行う．
void
ItemGen::phase1_genitem(
  const VlScope* parent,
  const vector<const PtDeclHead*>& pt_decl_array,
  const vector<const PtItem*>& pt_item_array
)
{
  phase1_items(parent, pt_item_array);
  auto stub = make_stub<ElbProxy,
			const VlScope*,
			const vector<const PtDeclHead*>&>(static_cast<ElbProxy*>(this),
							  &ElbProxy::instantiate_decl,
							  parent, pt_decl_array);
  add_phase2stub(stub);
}

END_NAMESPACE_YM_VERILOG
