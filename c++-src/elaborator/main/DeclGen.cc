
/// @file DeclGen.cc
/// @brief ElbMgr の実装ファイル(宣言要素の elaboration)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "DeclGen.h"
#include "ElbEnv.h"

#include "ym/pt/PtDecl.h"
#include "ym/pt/PtExpr.h"
#include "ym/pt/PtMisc.h"

#include "ym/vl/VlModule.h"

#include "elb/ElbContAssign.h"
#include "elb/ElbDecl.h"
#include "elb/ElbParameter.h"
#include "elb/ElbGenvar.h"
#include "elb/ElbModule.h"
#include "elb/ElbTaskFunc.h"
#include "elb/ElbRange.h"
#include "elb/ElbExpr.h"

#include "ym/MsgMgr.h"

#define dout cout

BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス DeclGen
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] elab 生成器
// @param[in] elb_mgr Elbオブジェクトを管理するクラス
// @param[in] elb_factory Elbオブジェクトを生成するファクトリクラス
DeclGen::DeclGen(Elaborator& elab,
		 ElbMgr& elb_mgr,
		 ElbFactory& elb_factory) :
  ElbProxy(elab, elb_mgr, elb_factory)
{
}

// @brief デストラクタ
DeclGen::~DeclGen()
{
}

// @brief parameter と genvar を実体化する．
// @param[in] parent 親のスコープ
// @param[in] pt_head_array 宣言ヘッダの配列
// @param[in] force_to_local true なら parameter を localparam にする．
void
DeclGen::phase1_decl(const VlNamedObj* parent,
		     const vector<const PtDeclHead*>& pt_head_array,
		     bool force_to_local)
{
  for ( auto pt_head: pt_head_array ) {
    switch ( pt_head->type() ) {
    case PtDeclType::Param:
      instantiate_param_head(parent, pt_head, force_to_local);
      break;

    case PtDeclType::LocalParam:
      instantiate_param_head(parent, pt_head, true);
      break;

    case PtDeclType::Genvar:
      instantiate_genvar_head(parent, pt_head);
      break;

    default:
      break;
    }
  }
}

// @brief IO宣言要素をインスタンス化する．
// @param[in] module 親のモジュール
// @param[in] taskfunc 親のタスク/関数
// @param[in] pt_head IO宣言ヘッダ
// @note module, taskfunc は1つのみが値を持つ．残りは nullptr
void
DeclGen::instantiate_iodecl(ElbModule* module,
			    ElbTaskFunc* taskfunc,
			    const vector<const PtIOHead*>& pt_head_array)
{
  // module と taskfunc は排他的
  ASSERT_COND( module != nullptr || taskfunc != nullptr );
  ASSERT_COND( module == nullptr || taskfunc == nullptr );

  VlNamedObj* namedobj = nullptr;
  if ( module ) {
    namedobj = module;
  }
  else if ( taskfunc ) {
    namedobj = taskfunc;
  }
  else {
    // 冗長
    ASSERT_NOT_REACHED;
  }

  int index = 0;
  for ( auto pt_head: pt_head_array ) {
    VpiAuxType def_aux_type = pt_head->aux_type();
    bool sign = pt_head->is_signed();
    const PtExpr* pt_left = pt_head->left_range();
    const PtExpr* pt_right = pt_head->right_range();
    bool has_range = (pt_left != nullptr) && (pt_right != nullptr);

    // 範囲指定を持っている場合には範囲を計算する．
    int left_val = 0;
    int right_val = 0;
    if ( !evaluate_range(namedobj,
			 pt_left, pt_right,
			 left_val, right_val) ) {
      // エラーが起きたのでスキップする．
      // エラーメッセージは evaluate_range() 内で出力されている．
      continue;
    }

    // ヘッダ情報の生成
    // ちなみに IOHead は範囲の情報を持たない．
    ElbIOHead* head = nullptr;
    if ( module ) {
      head = factory().new_ModIOHead(module, pt_head);
    }
    else if ( taskfunc->type() == VpiObjType::Task ) {
      head = factory().new_TaskIOHead(taskfunc, pt_head);
    }
    else {
      head = factory().new_FunctionIOHead(taskfunc, pt_head);
    }
    ASSERT_COND( head != nullptr );

    for ( SizeType i = 0; i < pt_head->item_num(); ++ i ) {
      auto pt_item = pt_head->item(i);
      // IO定義と変数/ネット定義が一致しているか調べる．
      auto handle = find_obj(namedobj, pt_item->name());
      ElbDecl* decl = nullptr;
      if ( handle ) {
	// 同名の要素が見つかった．
	if ( def_aux_type != VpiAuxType::None ) {
	  // なのに IO 宣言の aux_type もある．
	  ostringstream buf;
	  buf << pt_item->name() << " : has an aux-type declaration"
	      << ", while it also has another declaration in "
	      << handle->file_region() << ".";
	  MsgMgr::put_msg(__FILE__, __LINE__,
			  pt_item->file_region(),
			  MsgType::Error,
			  "ELAB",
			  buf.str());
	  continue;
	}
	decl = handle->decl();
	if ( decl ) {
	  // 対象が宣言要素だった場合．
	  VpiObjType type = decl->type();
	  if ( (module == nullptr || type != VpiObjType::Net) &&
	       type != VpiObjType::Reg &&
	       type != VpiObjType::IntegerVar &&
	       type != VpiObjType::TimeVar) {
	    // ちょっと論理式が分かりにくいが，上の式を否定すると，
	    // - module かつネットは OK
	    // - reg/integer/time は OK
	    // ということになる．
	    // それ以外は nullptr にしておく．
	    decl = nullptr;
	  }
	}
	if ( !decl ) {
	  if ( handle->declarray() ) {
	    // 対象が配列だった場合．
	    ostringstream buf;
	    buf << pt_item->name()
		<< ": Array object shall not be connected to IO port.";
	    MsgMgr::put_msg(__FILE__, __LINE__,
			    decl->file_region(),
			    MsgType::Error,
			    "ELAB",
			    buf.str());
	  }
	  else {
	    // 不適切な型だった場合．
	    // 上の decl = nullptr にした時もここに来る．
	    ostringstream buf;
	    buf << handle->full_name()
		<< ": Should be a ";
	    if ( module ) {
	      buf << "net, ";
	    }
	    buf << "reg or integer/time variable.";
	    MsgMgr::put_msg(__FILE__, __LINE__,
			    pt_item->file_region(),
			    MsgType::Error,
			    "ELAB",
			    buf.str());
	  }
	  continue;
	}

	// ここに来たら decl != nullptr

	// decl と型が一致しているか調べる．
	// IEEE 1364-2001 12.3.3 Port declarations
	if ( decl->has_range() ) {
	  int left_val2 = decl->left_range_val();
	  int right_val2 = decl->right_range_val();
	  if ( !has_range ) {
	    // decl は範囲を持っているが IO は持っていない．
	    // これはエラーにしなくてよいのだろうか？
	    // たぶんコンパイルオプションで制御すべき
	    if ( allow_empty_io_range() ) {
	      left_val = left_val2;
	      right_val = right_val2;
	    }
	    else {
	      ostringstream buf;
	      buf << "Conflictive range declaration of \""
		  << pt_item->name() << "\".";
	      MsgMgr::put_msg(__FILE__, __LINE__,
			      pt_item->file_region(),
			      MsgType::Error,
			      "ELAB",
			      buf.str());
	      continue;
	    }
	  }
	  else if ( left_val != left_val2 || right_val != right_val2 ) {
	    // 範囲が異なっていた．
	    ostringstream buf;
	    buf << "Conflictive range declaration of \""
		<< pt_item->name() << "\".";
	    MsgMgr::put_msg(__FILE__, __LINE__,
			    pt_item->file_region(),
			    MsgType::Error,
			    "ELAB",
			    buf.str());
	    dout << "IO range: [" << left_val << ":" << right_val << "]"
		 << endl
		 << "Decl range: [" << left_val2 << ":" << right_val2 << "]"
		 << endl;
	    continue;
	  }
	}
	else if ( has_range ) {
	  // decl は範囲を持っていないが IO は持っている．
	  // エラーとする．
	  ostringstream buf;
	  buf << "Conflictive range declaration of \""
	      << pt_item->name() << "\".";
	  MsgMgr::put_msg(__FILE__, __LINE__,
			  pt_item->file_region(),
			  MsgType::Error,
			  "ELAB",
			  buf.str());
	  continue;
	}
	// どちらか一方でも符号付きなら両方符号付きにする．
	// ちょっと ad-hoc な仕様
	if ( !decl->is_signed() && sign ) {
	  decl->set_signed();
	}
      }
      else {
	// 同名の要素が見つからなかったので作る必要がある．
	VpiAuxType aux_type = def_aux_type;
	if ( aux_type == VpiAuxType::None ) {
	  if ( module ) {
	    // モジュール IO の場合は `default_net_type を参照する．
	    VpiNetType net_type = module->def_net_type();
	    if ( net_type == VpiNetType::None ) {
	      ostringstream buf;
	      buf << pt_item->name() << " : Implicit declaration is inhibited "
		  << " because default_nettype = \"none\".";
	      MsgMgr::put_msg(__FILE__, __LINE__,
			      pt_item->file_region(),
			      MsgType::Error,
			      "ELAB",
			      buf.str());
	      continue;
	    }
	    aux_type = VpiAuxType::Net;
	  }
	  else {
	    // task/function の場合，型指定が無い時は reg 型となる．
	    aux_type = VpiAuxType::Reg;
	  }
	}

	// ヘッダを生成する．
	ElbDeclHead* head = nullptr;
	if ( module ) {
	  if ( has_range ) {
	    head = factory().new_DeclHead(module, pt_head, aux_type,
					  pt_left, pt_right,
					  left_val, right_val);
	  }
	  else {
	    head = factory().new_DeclHead(module, pt_head, aux_type);
	  }
	}
	if ( taskfunc ) {
	  if ( has_range ) {
	    head = factory().new_DeclHead(taskfunc, pt_head, aux_type,
					  pt_left, pt_right,
					  left_val, right_val);
	  }
	  else {
	    head = factory().new_DeclHead(taskfunc, pt_head, aux_type);
	  }
	}
	ASSERT_COND( head != nullptr );

	// 初期値を生成する．
	const PtExpr* pt_init = pt_item->init_value();
	ElbExpr* init = nullptr;
	if ( module ) {
	  if ( pt_init ) {
	    // 初期値を持つ場合
	    if ( aux_type == VpiAuxType::Net ) {
	      // net 型の場合(ここに来るのは暗黙宣言のみ)は初期値を持てない．
	      ostringstream buf;
	      buf << pt_item->name()
		  << " : Implicit net declaration cannot have initial value.";
	      MsgMgr::put_msg(__FILE__, __LINE__,
			      pt_item->file_region(),
			      MsgType::Error,
			      "ELAB",
			      buf.str());
	      continue;
	    }
	    // これは verilog_grammer.yy の list_of_variable_port_identifiers
	    // に対応するので必ず constant_expression である．
	    init = instantiate_constant_expr(module, pt_init);
	    // エラーの場合には init = nullptr となるが処理は続ける．
	  }
	}
	else {
	  // task/function の IO 宣言には初期値はない．
	  ASSERT_COND( pt_init == nullptr );
	}

	decl = factory().new_Decl(head, pt_item, init);
	int tag = 0;
	switch ( aux_type ) {
	case VpiAuxType::Net: tag = vpiNet; break;
	case VpiAuxType::Reg: tag = vpiReg; break;
	case VpiAuxType::Var: tag = vpiVariables; break;
	default:
	  ASSERT_NOT_REACHED;
	}
	reg_decl(tag, decl);
      }

      if ( module ) {
	module->init_iodecl(index, head, pt_item, decl);
      }
      else if ( taskfunc ) {
	taskfunc->init_iodecl(index, head, pt_item, decl);
      }
      else {
	// かなりしつこく冗長
	ASSERT_NOT_REACHED;
      }
      ++ index;

      ostringstream buf;
      buf << "IODecl(" << pt_item->name() << ")@"
	  << namedobj->full_name() << " created.";
      MsgMgr::put_msg(__FILE__, __LINE__,
		      pt_head->file_region(),
		      MsgType::Info,
		      "ELAB",
		      buf.str());
    }
  }
}

// 宣言要素をインスタンス化する．
// @param[in] parent 親のスコープ
// @param[in] pt_head_array 宣言ヘッダの配列
void
DeclGen::instantiate_decl(const VlNamedObj* parent,
			  const vector<const PtDeclHead*>& pt_head_array)
{
  for ( auto pt_head: pt_head_array ) {
    switch ( pt_head->type() ) {
    case PtDeclType::Param:
    case PtDeclType::LocalParam:
      // すでに処理済みのはず．
      break;

    case PtDeclType::Reg:
      instantiate_reg_head(parent, pt_head);
      break;

    case PtDeclType::Var:
      instantiate_var_head(parent, pt_head);
      break;

    case PtDeclType::Genvar:
      // すでに処理済みのはず．
      break;

    case PtDeclType::Net:
      instantiate_net_head(parent, pt_head);
      break;

    case PtDeclType::Event:
      instantiate_event_head(parent, pt_head);
      break;

    case PtDeclType::SpecParam:
      // 未対応
      break;

    default:
      break;
    }
  }
}

// @brief パラメータ用の instantiate 関数
// @param[in] parent 親のスコープ
// @param[in] pt_head_array 宣言ヘッダの配列
// @param[in] is_local local_param にする時 true
void
DeclGen::instantiate_param_head(const VlNamedObj* parent,
				const PtDeclHead* pt_head,
				bool is_local)
{
  const VlModule* module = parent->parent_module();

  const PtExpr* pt_left = pt_head->left_range();
  const PtExpr* pt_right = pt_head->right_range();
  ElbParamHead* param_head = nullptr;
  if ( pt_left && pt_right ) {
    int left_val = 0;
    int right_val = 0;
    if ( !evaluate_range(parent, pt_left, pt_right,
			 left_val, right_val) ) {
      return;
    }
    param_head = factory().new_ParamHead(module, pt_head,
					 pt_left, pt_right,
					 left_val, right_val);
  }
  else {
    param_head = factory().new_ParamHead(module, pt_head);
  }


  for ( auto pt_item: pt_head->item_list() ) {
    const FileRegion& file_region = pt_item->file_region();

    ElbParameter* param = factory().new_Parameter(param_head,
						  pt_item,
						  is_local);
    ASSERT_COND(param );
    reg_parameter(vpiParameter, param);

#if 0
    // attribute instance の生成
    instantiate_attribute(pt_head->attr_top(), false, param);
#else
#warning "TODO:2011-02-09-01"
#endif

    ostringstream buf;
    buf << "Parameter(" << param->full_name() << ") created.";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    file_region,
		    MsgType::Info,
		    "ELAB",
		    buf.str());

    // 右辺の式は constant expression のはずなので今つくる．
    auto pt_init_expr = pt_item->init_value();
    auto value = evaluate_expr(parent, pt_init_expr, true);
    param->set_expr(pt_init_expr, value);

    // ダブっている感じがするけど同じことを表す parameter assign 文
    // をつくってモジュールに追加する．
    auto pa = factory().new_NamedParamAssign(module, pt_item,
					     param, pt_init_expr,
					     value);
    reg_paramassign(pa);
  }
}

// @brief net をインスタンス化する．
// @param[in] parent 親のスコープ
// @param[in] pt_head 宣言のヘッダ
void
DeclGen::instantiate_net_head(const VlNamedObj* parent,
			      const PtDeclHead* pt_head)
{
  auto pt_left = pt_head->left_range();
  auto pt_right = pt_head->right_range();
  auto pt_delay = pt_head->delay();

  bool has_delay = (pt_delay != nullptr);

  ElbDeclHead* net_head = nullptr;
  if ( pt_left && pt_right ) {
    int left_val = 0;
    int right_val = 0;
    if ( !evaluate_range(parent, pt_left, pt_right,
			 left_val, right_val) ) {
      return;
    }
    net_head = factory().new_DeclHead(parent, pt_head,
				      pt_left, pt_right,
				      left_val, right_val,
				      has_delay);
  }
  else {
    net_head = factory().new_DeclHead(parent, pt_head);
  }
  ASSERT_COND( net_head );

  if ( pt_delay ) {
    add_phase3stub(make_stub(this, &DeclGen::link_net_delay,
			     net_head, pt_delay));
  }

  for ( SizeType i = 0; i < pt_head->item_num(); ++ i ) {
    auto pt_item = pt_head->item(i);
    // init_value() が 0 でなければ初期割り当てを持つということ．
    auto pt_init = pt_item->init_value();

    SizeType dim_size = pt_item->range_num();
    if ( dim_size > 0 ) {
      // 配列

      // 初期割り当ては構文規則上持てないはず
      ASSERT_COND( !pt_init );

      // 範囲の配列を作る．
      vector<ElbRangeSrc> range_src;
      if ( !instantiate_dimension_list(parent, pt_item, range_src) ) {
	continue;
      }

      auto net_array = factory().new_DeclArray(net_head, pt_item, range_src);
      reg_declarray(vpiNetArray, net_array);

#if 0
      // attribute instance の生成
      instantiate_attribute(pt_head->attr_top(), false, net_array);
#else
#warning "TODO:2011-02-09-01"
#endif

      ostringstream buf;
      buf << "NetArray(" << net_array->full_name() << ") created.";
      MsgMgr::put_msg(__FILE__, __LINE__,
		      pt_item->file_region(),
		      MsgType::Info,
		      "ELAB",
		      buf.str());
    }
    else {
      // 単一の要素
      ElbDecl* net = factory().new_Decl(net_head,
					pt_item);
      reg_decl(vpiNet, net);

      if ( pt_init ) {
	// 初期割り当てつき
	add_phase3stub(make_stub(this, &DeclGen::link_net_assign,
				 net, pt_item));
      }

#if 0
      // attribute instance の生成
      instantiate_attribute(pt_head->attr_top(), false, net);
#else
#warning "TODO:2011-02-09-01"
#endif

      ostringstream buf;
      buf << "Net(" << net->full_name() << ") created.";
      MsgMgr::put_msg(__FILE__, __LINE__,
		      pt_item->file_region(),
		      MsgType::Info,
		      "ELAB",
		      buf.str());
    }
  }
}

// @brief net の遅延値を生成する．
// @param[in] net_head ネットのヘッダ
// @param[in] pt_delay パース木の遅延式定義
void
DeclGen::link_net_delay(ElbDeclHead* net_head,
			const PtDelay* pt_delay)
{
  auto parent = net_head->parent();
  auto delay = instantiate_delay(parent, pt_delay);
  if ( delay ) {
    net_head->set_delay(delay);
  }
}

// @brief net の初期値を生成する．
// @param[in] net ネット
// @param[in] pt_item パース木のネット定義要素
void
DeclGen::link_net_assign(ElbDecl* net,
			 const PtDeclItem* pt_item)
{
  // 実際には対応する continuous assign 文を作る．
  auto lhs = factory().new_Primary(pt_item, net);

  auto parent = net->parent();
  auto pt_init = pt_item->init_value();
  auto rhs = instantiate_rhs(parent, ElbEnv(), pt_init, lhs);
  if ( !rhs ) {
    return;
  }

  auto module = parent->parent_module();
  auto ca = factory().new_ContAssign(module, pt_item, lhs, rhs);
  reg_contassign(ca);
}

// @brief reg をインスタンス化する．
// @param[in] parent 親のスコープ
// @param[in] pt_head 宣言のヘッダ
void
DeclGen::instantiate_reg_head(const VlNamedObj* parent,
			      const PtDeclHead* pt_head)
{
  const PtExpr* pt_left = pt_head->left_range();
  const PtExpr* pt_right = pt_head->right_range();

  ElbDeclHead* reg_head = nullptr;
  if ( pt_left && pt_right ) {
    int left_val = 0;
    int right_val = 0;
    if ( !evaluate_range(parent, pt_left, pt_right,
			 left_val, right_val) ) {
      return;
    }
    reg_head = factory().new_DeclHead(parent, pt_head,
				      pt_left, pt_right,
				      left_val, right_val);
  }
  else {
    reg_head = factory().new_DeclHead(parent, pt_head);
  }
  ASSERT_COND( reg_head != nullptr );

  for ( SizeType i = 0; i < pt_head->item_num(); ++ i ) {
    auto pt_item = pt_head->item(i);
    const PtExpr* pt_init = pt_item->init_value();
    SizeType dim_size = pt_item->range_num();
    if ( dim_size > 0 ) {
      // 配列の場合

      // 配列は初期値を持たない．
      ASSERT_COND( !pt_init );

      // 範囲の配列を作る．
      vector<ElbRangeSrc> range_src;
      if ( !instantiate_dimension_list(parent, pt_item, range_src) ) {
	continue;
      }

      ElbDeclArray* reg_array = factory().new_DeclArray(reg_head,
							pt_item,
							range_src);
      reg_declarray(vpiRegArray, reg_array);

#if 0
      // attribute instance の生成
      instantiate_attribute(pt_head->attr_top(), false, reg_array);
#else
#warning "TODO:2011-02-09-01"
#endif

      ostringstream buf;
      buf << "RegArray(" << reg_array->full_name() << ") created.";
      MsgMgr::put_msg(__FILE__, __LINE__,
		      pt_item->file_region(),
		      MsgType::Info,
		      "ELAB",
		      buf.str());
    }
    else {
      // 単独の要素
      ElbExpr* init = nullptr;
      if ( pt_init ) {
	// 初期値を持つ場合
	// 初期値は constant_expression なので今作る．
	init = instantiate_constant_expr(parent, pt_init);
	// エラーの時には init = nullptr となるがそれでも処理は続ける．
	// もちろんエラーは記録されている．
      }

      ElbDecl* reg = factory().new_Decl(reg_head, pt_item, init);
      reg_decl(vpiReg, reg);

#if 0
      // attribute instance の生成
      instantiate_attribute(pt_head->attr_top(), false, reg);
#else
#warning "TODO:2011-02-09-01"
#endif

      ostringstream buf;
      buf << "Reg(" << reg->full_name() << ") created.";
      MsgMgr::put_msg(__FILE__, __LINE__,
		      pt_item->file_region(),
		      MsgType::Info,
		      "ELAB",
		      buf.str());
    }
  }
}

// @brief variable header をインスタンス化する．
// @param[in] parent 親のスコープ
// @param[in] pt_head 宣言のヘッダ
void
DeclGen::instantiate_var_head(const VlNamedObj* parent,
			      const PtDeclHead* pt_head)
{
  ASSERT_COND(pt_head->data_type() != VpiVarType::None );

  ElbDeclHead* var_head = factory().new_DeclHead(parent, pt_head);

  for ( SizeType i = 0; i < pt_head->item_num(); ++ i ) {
    auto pt_item = pt_head->item(i);
    const PtExpr* pt_init = pt_item->init_value();
    SizeType dim_size = pt_item->range_num();
    if ( dim_size > 0 ) {
      // 配列の場合

      // 配列は初期値を持たない．
      ASSERT_COND( !pt_init );

      // 範囲の配列を作る．
      vector<ElbRangeSrc> range_src;
      if ( !instantiate_dimension_list(parent, pt_item, range_src) ) {
	continue;
      }

      ElbDeclArray* var_array = factory().new_DeclArray(var_head,
							pt_item,
							range_src);
      reg_declarray(vpiVariables, var_array);

#if 0
      // attribute instance の生成
      instantiate_attribute(pt_head->attr_top(), false, var_array);
#else
#warning "TODO:2011-02-09-01"
#endif

      ostringstream buf;
      buf << "VarArray(" << var_array->full_name() << ") created.";
      MsgMgr::put_msg(__FILE__, __LINE__,
		      pt_item->file_region(),
		      MsgType::Info,
		      "ELAB",
		      buf.str());
    }
    else {
      // 単独の変数
      ElbExpr* init = nullptr;
      if ( pt_init ) {
	// 初期値を持つ場合
	// 初期値は constant_expression なので今作る．
	init = instantiate_constant_expr(parent, pt_init);
	// エラーの時には init = nullptr となるがそれでも処理は続ける．
	// もちろんエラーは記録されている．
      }

      ElbDecl* var = factory().new_Decl(var_head, pt_item, init);
      reg_decl(vpiVariables, var);

#if 0
      // attribute instance の生成
      instantiate_attribute(pt_head->attr_top(), false, var);
#else
#warning "TODO:2011-02-09-01"
#endif

      ostringstream buf;
      buf << "Var(" << var->full_name() << ") created.";
      MsgMgr::put_msg(__FILE__, __LINE__,
		      pt_item->file_region(),
		      MsgType::Info,
		      "ELAB",
		      buf.str());
    }
  }
}

// @brief named_event をインスタンス化する．
// @param[in] parent 親のスコープ
// @param[in] pt_head 宣言のヘッダ
void
DeclGen::instantiate_event_head(const VlNamedObj* parent,
				const PtDeclHead* pt_head)
{
  ElbDeclHead* event_head = factory().new_DeclHead(parent, pt_head);

  for ( SizeType i = 0; i < pt_head->item_num(); ++ i ) {
    auto pt_item = pt_head->item(i);
    SizeType dim_size = pt_item->range_num();
    if ( dim_size > 0 ) {
      // 配列

      // 範囲の配列を作る．
      vector<ElbRangeSrc> range_src;
      if ( !instantiate_dimension_list(parent, pt_item, range_src) ) {
	continue;
      }

      ElbDeclArray* ne_array = factory().new_DeclArray(event_head,
						       pt_item,
						       range_src);
      reg_declarray(vpiNamedEventArray, ne_array);

#if 0
      // attribute instance の生成
      instantiate_attribute(pt_head->attr_top(), false, ne_array);
#else
#warning "TODO:2011-02-09-01"
#endif

      ostringstream buf;
      buf << "NamedEventArray(" << ne_array->full_name() << ") created.";
      MsgMgr::put_msg(__FILE__, __LINE__,
		      pt_item->file_region(),
		      MsgType::Info,
		      "ELAB",
		      buf.str());
    }
    else {
      // 単一の要素
      ElbDecl* named_event = factory().new_Decl(event_head,
						pt_item);
      reg_decl(vpiNamedEvent, named_event);

#if 0
      // attribute instance の生成
      instantiate_attribute(pt_head->attr_top(), false, named_event);
#else
#warning "TODO:2011-02-09-01"
#endif

      ostringstream buf;
      buf << "NamedEvent(" << named_event->full_name() << ") created.";
      MsgMgr::put_msg(__FILE__, __LINE__,
		      pt_item->file_region(),
		      MsgType::Info,
		      "ELAB",
		      buf.str());
    }
  }
}

// @brief genvar をインスタンス化する．
// @param[in] parent 親のスコープ
// @param[in] pt_head 宣言のヘッダ
void
DeclGen::instantiate_genvar_head(const VlNamedObj* parent,
				 const PtDeclHead* pt_head)
{
  for ( SizeType i = 0; i < pt_head->item_num(); ++ i ) {
    auto pt_item = pt_head->item(i);
    ElbGenvar* genvar = factory().new_Genvar(parent, pt_item, 0);
    reg_genvar(genvar);

    ostringstream buf;
    buf << "Genvar(" << genvar->full_name() << ") created.";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    pt_item->file_region(),
		    MsgType::Info,
		    "ELAB",
		    buf.str());
  }
}

// @brief 配列の次元リストを生成する．
// @param[in] parent 親のスコープ
// @param[in] pt_item 要素定義
// @param[in] range_src 範囲の情報を設定する配列
bool
DeclGen::instantiate_dimension_list(const VlNamedObj*  parent,
				    const PtDeclItem* pt_item,
				    vector<ElbRangeSrc>& range_src)
{
  SizeType n = pt_item->range_num();
  range_src.reserve(n);

  bool ok = true;
  for ( SizeType i = 0; i < n; ++ i ) {
    auto pt_range = pt_item->range(i);
    const PtExpr* pt_left = pt_range->left();
    const PtExpr* pt_right = pt_range->right();
    int left_val = 0;
    int right_val = 0;
    if ( !evaluate_range(parent, pt_left, pt_right,
			 left_val, right_val) ) {
      ok = false;
      break;
    }
    range_src.push_back(ElbRangeSrc(pt_range,
				    pt_left, pt_right,
				    left_val, right_val));
  }

  return ok;
}

END_NAMESPACE_YM_VERILOG
