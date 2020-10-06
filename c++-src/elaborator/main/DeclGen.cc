
/// @file DeclGen.cc
/// @brief ElbMgr の実装ファイル(宣言要素の elaboration)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.


#include "DeclGen.h"
#include "ElbEnv.h"
#include "ElbError.h"
#include "ErrorGen.h"

#include "ym/pt/PtDecl.h"
#include "ym/pt/PtExpr.h"
#include "ym/pt/PtMisc.h"

#include "elaborator/ElbDecl.h"
#include "elaborator/ElbParameter.h"
#include "elaborator/ElbGenvar.h"
#include "elaborator/ElbModule.h"
#include "elaborator/ElbTaskFunc.h"
#include "elaborator/ElbRange.h"
#include "elaborator/ElbExpr.h"


#define DOUT cout

BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス DeclGen
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] elab 生成器
// @param[in] elb_mgr Elbオブジェクトを管理するクラス
DeclGen::DeclGen(Elaborator& elab,
		 ElbMgr& elb_mgr) :
  ElbProxy(elab, elb_mgr)
{
}

// @brief デストラクタ
DeclGen::~DeclGen()
{
}

// @brief parameter と genvar を実体化する．
// @param[in] scope 親のスコープ
// @param[in] pt_head_array 宣言ヘッダの配列
// @param[in] force_to_local true なら parameter を localparam にする．
void
DeclGen::phase1_decl(const VlScope* scope,
		     const vector<const PtDeclHead*>& pt_head_array,
		     bool force_to_local)
{
  for ( auto pt_head: pt_head_array ) {
    try {
      switch ( pt_head->type() ) {
      case PtDeclType::Param:
	instantiate_param_head(scope, pt_head, force_to_local);
	break;

      case PtDeclType::LocalParam:
	instantiate_param_head(scope, pt_head, true);
	break;

      case PtDeclType::Genvar:
	instantiate_genvar_head(scope, pt_head);
	break;

      default:
	// それ以外のタイプはここでは無視する．
	break;
      }
    }
    catch ( const ElbError& error ) {
      put_error(error);
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
  const VlScope* scope{nullptr};
  if ( module ) {
    scope = module;
  }
  else if ( taskfunc ) {
    scope = taskfunc;
  }
  ASSERT_COND( scope != nullptr );

  for ( auto pt_head: pt_head_array ) {
    auto def_aux_type{pt_head->aux_type()};
    bool sign{pt_head->is_signed()};
    auto pt_left{pt_head->left_range()};
    auto pt_right{pt_head->right_range()};
    bool has_range{(pt_left != nullptr) && (pt_right != nullptr)};

    // 範囲指定を持っている場合には範囲を計算する．
    int left_val;
    int right_val;
    tie(left_val, right_val) = evaluate_range(scope, pt_left, pt_right);

    // ヘッダ情報の生成
    // ちなみに IOHead は範囲の情報を持たない．
    ElbIOHead* head{nullptr};
    if ( module ) {
      head = mgr().new_IOHead(module, pt_head);
    }
    else if ( taskfunc ) {
      head = mgr().new_IOHead(taskfunc, pt_head);
    }
    ASSERT_COND( head != nullptr );

    for ( auto pt_item: pt_head->item_list() ) {
      // IO定義と変数/ネット定義が一致しているか調べる．
      auto handle{mgr().find_obj(scope, pt_item->name())};
      ElbDecl* decl{nullptr};
      if ( handle ) {
	// 同名の要素が見つかった．
	if ( def_aux_type != VpiAuxType::None ) {
	  // なのに IO 宣言の aux_type もある．
	  ErrorGen::duplicate_type(__FILE__, __LINE__, pt_item, handle->file_region());
	}
	decl = handle->decl();
	if ( decl ) {
	  // 対象が宣言要素だった場合．
	  auto type = decl->type();
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
	    ErrorGen::array_io(__FILE__, __LINE__, pt_item);
	  }

	  // 不適切な型だった場合．
	  // 上の decl = nullptr にした時もここに来る．
	  bool is_module{module != nullptr};
	  ErrorGen::illegal_io(__FILE__, __LINE__, pt_item, handle->full_name(), is_module);
	}

	// ここに来たら decl != nullptr

	// decl と型が一致しているか調べる．
	// IEEE 1364-2001 12.3.3 Port declarations
	if ( decl->has_range() ) {
	  int left_val2{decl->left_range_val()};
	  int right_val2{decl->right_range_val()};
	  if ( !has_range ) {
	    // decl は範囲を持っているが IO は持っていない．
	    // これはエラーにしなくてよいのだろうか？
	    // たぶんコンパイルオプションで制御すべき
	    if ( allow_empty_io_range() ) {
	      left_val = left_val2;
	      right_val = right_val2;
	    }
	    else {
	      ErrorGen::conflict_io_range(__FILE__, __LINE__, pt_item);
	    }
	  }
	  else if ( left_val != left_val2 || right_val != right_val2 ) {
	    // 範囲が異なっていた．
	    ErrorGen::conflict_io_range(__FILE__, __LINE__, pt_item);
	    DOUT << "IO range: [" << left_val << ":" << right_val << "]"
		 << endl
		 << "Decl range: [" << left_val2 << ":" << right_val2 << "]"
		 << endl;
	    continue;
	  }
	}
	else if ( has_range ) {
	  // decl は範囲を持っていないが IO は持っている．
	  // エラーとする．
	  ErrorGen::conflict_io_range(__FILE__, __LINE__, pt_item);
	}
	// どちらか一方でも符号付きなら両方符号付きにする．
	// ちょっと ad-hoc な仕様
	if ( !decl->is_signed() && sign ) {
	  decl->set_signed();
	}
      }
      else {
	// 同名の要素が見つからなかったので作る必要がある．
	auto aux_type{def_aux_type};
	if ( aux_type == VpiAuxType::None ) {
	  if ( module ) {
	    // モジュール IO の場合は `default_net_type を参照する．
	    auto net_type{module->def_net_type()};
	    if ( net_type == VpiNetType::None ) {
	      ErrorGen::no_impnet(__FILE__, __LINE__, pt_item);
	    }
	    aux_type = VpiAuxType::Net;
	  }
	  else {
	    // task/function の場合，型指定が無い時は reg 型となる．
	    aux_type = VpiAuxType::Reg;
	  }
	}

	// ヘッダを生成する．
	ElbDeclHead* head{nullptr};
	if ( has_range ) {
	  head = mgr().new_DeclHead(scope, pt_head, aux_type,
				    pt_left, pt_right,
				    left_val, right_val);
	}
	else {
	  head = mgr().new_DeclHead(scope, pt_head, aux_type);
	}
	ASSERT_COND( head != nullptr );

	// 初期値を生成する．
	auto pt_init{pt_item->init_value()};
	ElbExpr* init{nullptr};
	if ( module ) {
	  if ( pt_init != nullptr ) {
	    // 初期値を持つ場合
	    if ( aux_type == VpiAuxType::Net ) {
	      // net 型の場合(ここに来るのは暗黙宣言のみ)は初期値を持てない．
	      ErrorGen::impnet_with_init(__FILE__, __LINE__, pt_item);
	    }
	    // これは verilog_grammer.yy の list_of_variable_port_identifiers
	    // に対応するので必ず constant_expression である．
	    init = instantiate_constant_expr(scope, pt_init);
	  }
	}
	else {
	  // task/function の IO 宣言には初期値はない．
	  ASSERT_COND( pt_init == nullptr );
	}

	int tag{0};
	switch ( aux_type ) {
	case VpiAuxType::Net: tag = vpiNet; break;
	case VpiAuxType::Reg: tag = vpiReg; break;
	case VpiAuxType::Var: tag = vpiVariables; break;
	default:
	  ASSERT_NOT_REACHED;
	}
	decl = mgr().new_Decl(tag, head, pt_item, init);
      }

      if ( module ) {
	module->add_iodecl(head, pt_item, decl);
      }
      else if ( taskfunc ) {
	taskfunc->add_iodecl(head, pt_item, decl);
      }
      else {
	ASSERT_NOT_REACHED;
      }

      {
	ostringstream buf;
	buf << "IODecl(" << pt_item->name() << ")@"
	    << scope->full_name() << " created.";
	put_info(__FILE__, __LINE__,
		 pt_head->file_region(),
		 "ELABXXX",
		 buf.str());
      }
    }
  }
}

// 宣言要素をインスタンス化する．
// @param[in] scope 親のスコープ
// @param[in] pt_head_array 宣言ヘッダの配列
void
DeclGen::instantiate_decl(const VlScope* scope,
			  const vector<const PtDeclHead*>& pt_head_array)
{
  for ( auto pt_head: pt_head_array ) {
    try {
      switch ( pt_head->type() ) {
      case PtDeclType::Param:
      case PtDeclType::LocalParam:
	// すでに処理済みのはず．
	break;

      case PtDeclType::Reg:
	instantiate_reg_head(scope, pt_head);
	break;

      case PtDeclType::Var:
	instantiate_var_head(scope, pt_head);
	break;

      case PtDeclType::Genvar:
	// すでに処理済みのはず．
	break;

      case PtDeclType::Net:
	instantiate_net_head(scope, pt_head);
	break;

      case PtDeclType::Event:
	instantiate_event_head(scope, pt_head);
	break;

      case PtDeclType::SpecParam:
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
}

// @brief パラメータ用の instantiate 関数
// @param[in] scope 親のスコープ
// @param[in] pt_head_array 宣言ヘッダの配列
// @param[in] is_local local_param にする時 true
void
DeclGen::instantiate_param_head(const VlScope* scope,
				const PtDeclHead* pt_head,
				bool is_local)
{
  auto module{scope->parent_module()};
  auto pt_left{pt_head->left_range()};
  auto* pt_right{pt_head->right_range()};
  ElbParamHead* param_head{nullptr};
  if ( pt_left && pt_right ) {
    int left_val;
    int right_val;
    tie(left_val, right_val) = evaluate_range(scope, pt_left, pt_right);
    param_head = mgr().new_ParamHead(scope, pt_head,
				     pt_left, pt_right,
				     left_val, right_val);
  }
  else {
    param_head = mgr().new_ParamHead(scope, pt_head);
  }

  for ( auto pt_item: pt_head->item_list() ) {
    const auto& file_region{pt_item->file_region()};
    auto param{mgr().new_Parameter(param_head,
				   pt_item,
				   is_local)};
    ASSERT_COND( param );

    // attribute instance の生成
    auto attr_list{attribute_list(pt_head)};
    mgr().reg_attr(param, attr_list);

    {
      ostringstream buf;
      buf << "Parameter(" << param->full_name() << ") created.";
      put_info(__FILE__, __LINE__,
	       file_region,
	       "ELABXXX",
	       buf.str());
    }

    // 右辺の式は constant expression のはずなので今つくる．
    auto pt_init_expr{pt_item->init_value()};
    auto value{evaluate_expr(scope, pt_init_expr)};
    param->set_init_expr(pt_init_expr, value);

    // ダブっている感じがするけど同じことを表す parameter assign 文
    // をつくってモジュールに追加する．
    auto pa{mgr().new_NamedParamAssign(module, pt_item, param,
				       pt_init_expr, value)};
  }
}

// @brief net をインスタンス化する．
// @param[in] scope 親のスコープ
// @param[in] pt_head 宣言のヘッダ
void
DeclGen::instantiate_net_head(const VlScope* scope,
			      const PtDeclHead* pt_head)
{
  auto pt_left{pt_head->left_range()};
  auto pt_right{pt_head->right_range()};
  auto pt_delay{pt_head->delay()};
  bool has_delay{(pt_delay != nullptr)};

  ElbDeclHead* net_head{nullptr};
  if ( pt_left && pt_right ) {
    int left_val;
    int right_val;
    tie(left_val, right_val) = evaluate_range(scope, pt_left, pt_right);
    net_head = mgr().new_DeclHead(scope, pt_head,
				  pt_left, pt_right,
				  left_val, right_val,
				  has_delay);
  }
  else {
    net_head = mgr().new_DeclHead(scope, pt_head);
  }
  ASSERT_COND( net_head );

  if ( pt_delay ) {
    add_phase3stub(make_stub(this, &DeclGen::link_net_delay,
			     net_head, pt_delay));
  }

  for ( auto pt_item: pt_head->item_list() ) {
    // init_value() が 0 でなければ初期割り当てを持つということ．
    auto pt_init{pt_item->init_value()};

    SizeType dim_size{pt_item->range_num()};
    if ( dim_size > 0 ) {
      // 配列

      // 初期割り当ては構文規則上持てないはず
      ASSERT_COND( !pt_init );

      // 範囲の配列を作る．
      vector<ElbRangeSrc> range_src;
      if ( !instantiate_dimension_list(scope, pt_item, range_src) ) {
	continue;
      }

      auto net_array{mgr().new_DeclArray(vpiNetArray, net_head, pt_item, range_src)};

      // attribute instance の生成
      auto attr_list{attribute_list(pt_head)};
      mgr().reg_attr(net_array, attr_list);

      {
	ostringstream buf;
	buf << "NetArray(" << net_array->full_name() << ") created.";
	put_info(__FILE__, __LINE__,
		 pt_item->file_region(),
		 "ELABXXX",
		 buf.str());
      }
    }
    else {
      // 単一の要素
      auto net{mgr().new_Decl(vpiNet, net_head, pt_item)};

      if ( pt_init ) {
	// 初期割り当てつき
	// net の初期割り当ては continuous assignment と同等なので
	// あとで作る．
	add_phase3stub(make_stub(this, &DeclGen::link_net_assign,
				 net, pt_item));
      }

      // attribute instance の生成
      auto attr_list{attribute_list(pt_head)};
      mgr().reg_attr(net, attr_list);

      {
	ostringstream buf;
	buf << "Net(" << net->full_name() << ") created.";
	put_info(__FILE__, __LINE__,
		 pt_item->file_region(),
		 "ELABXXX",
		 buf.str());
      }
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
  auto scope{net_head->parent_scope()};
  auto delay{instantiate_delay(scope, pt_delay)};
  net_head->set_delay(delay);
}

// @brief net の初期値を生成する．
// @param[in] net ネット
// @param[in] pt_item パース木のネット定義要素
void
DeclGen::link_net_assign(ElbDecl* net,
			 const PtDeclItem* pt_item)
{
  // 実体は左辺が net の代入文を作る．
  auto lhs{mgr().new_Primary(pt_item, net)};
  auto scope{net->parent_scope()};
  auto pt_init{pt_item->init_value()};
  auto rhs{instantiate_rhs(scope, ElbEnv(), pt_init, lhs)};
  if ( !rhs ) {
    return;
  }

  net->set_init(rhs);

  // 対応する continuous assign 文を作る．
  auto module{scope->parent_module()};
  auto ca{mgr().new_ContAssign(module, pt_item, lhs, rhs)};
}

// @brief reg をインスタンス化する．
// @param[in] scope 親のスコープ
// @param[in] pt_head 宣言のヘッダ
void
DeclGen::instantiate_reg_head(const VlScope* scope,
			      const PtDeclHead* pt_head)
{
  auto pt_left{pt_head->left_range()};
  auto pt_right{pt_head->right_range()};

  ElbDeclHead* reg_head{nullptr};
  if ( pt_left && pt_right ) {
    int left_val;
    int right_val;
    tie(left_val, right_val) = evaluate_range(scope, pt_left, pt_right);
    reg_head = mgr().new_DeclHead(scope, pt_head,
				  pt_left, pt_right,
				  left_val, right_val);
  }
  else {
    reg_head = mgr().new_DeclHead(scope, pt_head);
  }
  ASSERT_COND( reg_head != nullptr );

  for ( auto pt_item: pt_head->item_list() ) {
    auto pt_init{pt_item->init_value()};
    SizeType dim_size{pt_item->range_num()};
    if ( dim_size > 0 ) {
      // 配列の場合

      // 配列は初期値を持たない．
      ASSERT_COND( !pt_init );

      // 範囲の配列を作る．
      vector<ElbRangeSrc> range_src;
      if ( !instantiate_dimension_list(scope, pt_item, range_src) ) {
	continue;
      }

      auto reg_array{mgr().new_DeclArray(vpiRegArray, reg_head,
					 pt_item, range_src)};

      // attribute instance の生成
      auto attr_list{attribute_list(pt_head)};
      mgr().reg_attr(reg_array, attr_list);

      {
	ostringstream buf;
	buf << "RegArray(" << reg_array->full_name() << ") created.";
	put_info(__FILE__, __LINE__,
		 pt_item->file_region(),
		 "ELABXXX",
		 buf.str());
      }
    }
    else {
      // 単独の要素
      const VlExpr* init{nullptr};
      if ( pt_init != nullptr ) {
	// 初期値を持つ場合
	// 初期値は constant_expression なので今作る．
	init = instantiate_constant_expr(scope, pt_init);
      }

      auto reg{mgr().new_Decl(vpiReg, reg_head, pt_item, init)};

      // attribute instance の生成
      auto attr_list{attribute_list(pt_head)};
      mgr().reg_attr(reg, attr_list);

      {
	ostringstream buf;
	buf << "Reg(" << reg->full_name() << ") created.";
	put_info(__FILE__, __LINE__,
		 pt_item->file_region(),
		 "ELABXXX",
		 buf.str());
      }
    }
  }
}

// @brief variable header をインスタンス化する．
// @param[in] scope 親のスコープ
// @param[in] pt_head 宣言のヘッダ
void
DeclGen::instantiate_var_head(const VlScope* scope,
			      const PtDeclHead* pt_head)
{
  ASSERT_COND( pt_head->data_type() != VpiVarType::None );

  auto var_head{mgr().new_DeclHead(scope, pt_head)};
  for ( auto pt_item: pt_head->item_list() ) {
    auto pt_init{pt_item->init_value()};
    SizeType dim_size{pt_item->range_num()};
    if ( dim_size > 0 ) {
      // 配列の場合

      // 配列は初期値を持たない．
      ASSERT_COND( !pt_init );

      // 範囲の配列を作る．
      vector<ElbRangeSrc> range_src;
      if ( !instantiate_dimension_list(scope, pt_item, range_src) ) {
	continue;
      }

      auto var_array{mgr().new_DeclArray(vpiVariables, var_head,
					 pt_item, range_src)};

      // attribute instance の生成
      auto attr_list{attribute_list(pt_head)};
      mgr().reg_attr(var_array, attr_list);

      {
	ostringstream buf;
	buf << "VarArray(" << var_array->full_name() << ") created.";
	put_info(__FILE__, __LINE__,
		 pt_item->file_region(),
		 "ELABXXX",
		 buf.str());
      }
    }
    else {
      // 単独の変数
      const VlExpr* init{nullptr};
      if ( pt_init != nullptr ) {
	// 初期値を持つ場合
	// 初期値は constant_expression なので今作る．
	init = instantiate_constant_expr(scope, pt_init);
      }

      auto var{mgr().new_Decl(vpiVariables, var_head, pt_item, init)};

      // attribute instance の生成
      auto attr_list{attribute_list(pt_head)};
      mgr().reg_attr(var, attr_list);

      {
	ostringstream buf;
	buf << "Var(" << var->full_name() << ") created.";
	put_info(__FILE__, __LINE__,
		 pt_item->file_region(),
		 "ELABXXX",
		 buf.str());
      }
    }
  }
}

// @brief named_event をインスタンス化する．
// @param[in] scope 親のスコープ
// @param[in] pt_head 宣言のヘッダ
void
DeclGen::instantiate_event_head(const VlScope* scope,
				const PtDeclHead* pt_head)
{
  auto event_head{mgr().new_DeclHead(scope, pt_head)};
  for ( auto pt_item: pt_head->item_list() ) {
    SizeType dim_size{pt_item->range_num()};
    if ( dim_size > 0 ) {
      // 配列

      // 範囲の配列を作る．
      vector<ElbRangeSrc> range_src;
      if ( !instantiate_dimension_list(scope, pt_item, range_src) ) {
	continue;
      }

      auto ne_array{mgr().new_DeclArray(vpiNamedEventArray, event_head,
					pt_item, range_src)};

      // attribute instance の生成
      auto attr_list{attribute_list(pt_head)};
      mgr().reg_attr(ne_array, attr_list);

      {
	ostringstream buf;
	buf << "NamedEventArray(" << ne_array->full_name() << ") created.";
	put_info(__FILE__, __LINE__,
		 pt_item->file_region(),
		 "ELABXXX",
		 buf.str());
      }
    }
    else {
      // 単一の要素
      auto named_event{mgr().new_Decl(vpiNamedEvent, event_head, pt_item)};

      // attribute instance の生成
      auto attr_list{attribute_list(pt_head)};
      mgr().reg_attr(named_event, attr_list);

      {
	ostringstream buf;
	buf << "NamedEvent(" << named_event->full_name() << ") created.";
	put_info(__FILE__, __LINE__,
		 pt_item->file_region(),
		 "ELABXXX",
		 buf.str());
      }
    }
  }
}

// @brief genvar をインスタンス化する．
// @param[in] scope 親のスコープ
// @param[in] pt_head 宣言のヘッダ
void
DeclGen::instantiate_genvar_head(const VlScope* scope,
				 const PtDeclHead* pt_head)
{
  for ( auto pt_item: pt_head->item_list() ) {
    auto genvar{mgr().new_Genvar(scope, pt_item, 0)};

    {
      ostringstream buf;
      buf << "Genvar(" << genvar->full_name() << ") created.";
      put_info(__FILE__, __LINE__,
	       pt_item->file_region(),
	       "ELABXXX",
	       buf.str());
    }
  }
}

// @brief 配列の次元リストを生成する．
// @param[in] scope 親のスコープ
// @param[in] pt_item 要素定義
// @param[out] range_src 範囲の情報を設定する配列
bool
DeclGen::instantiate_dimension_list(const VlScope* scope,
				    const PtDeclItem* pt_item,
				    vector<ElbRangeSrc>& range_src)
{
  SizeType n{pt_item->range_num()};
  range_src.reserve(n);

  for ( auto pt_range: pt_item->range_list() ) {
    auto pt_left{pt_range->left()};
    auto pt_right{pt_range->right()};
    int left_val;
    int right_val;
    tie(left_val, right_val) = evaluate_range(scope, pt_left, pt_right);
    range_src.push_back(ElbRangeSrc(pt_range,
				    pt_left, pt_right,
				    left_val, right_val));
  }

  return true;
}

#if 0
// @brief IO 宣言に aux_type と宣言が重複している．
// @param[in] pt_item パース木の要素
void
DeclGen::error_duplicate_type(const PtIOItem* pt_item,
			      const ObjHandle* handle)
{
  ostringstream buf;
  buf << pt_item->name() << " : has an aux-type declaration"
      << ", while it also has another declaration in "
      << handle->file_region() << ".";
  MsgMgr::put_msg(__FILE__, __LINE__,
		  pt_item->file_region(),
		  MsgType::Error,
		  "ELAB",
		  buf.str());
}

// @brief 配列要素が IO 宣言として現れていた．
// @param[in] pt_item IO宣言のパース木の要素
// @param[in] declarray 排列要素
void
DeclGen::error_array_io(const PtIOItem* pt_item,
			const VlDeclArray* declarray)
{
  ostringstream buf;
  buf << pt_item->name()
      << ": Array object shall not be connected to IO port.";
  MsgMgr::put_msg(__FILE__, __LINE__,
		  declarray->file_region(),
		  MsgType::Error,
		  "ELAB",
		  buf.str());
}

// @brief IO 宣言に不適切な宣言要素が使われていた．
// @param[in] pt_item IO宣言のパース木の要素
// @param[in] handle 宣言要素のハンドル
void
DeclGen::error_illegal_io(const PtIOItem* pt_item,
			  const ObjHandle* handle,
			  const VlModule* module)
{
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

// @brief IO 宣言と宣言要素の範囲指定が異なる．
void
DeclGen::error_conflict_io_range(const PtIOItem* pt_item)
{
  ostringstream buf;
  buf << "Conflictive range declaration of \""
      << pt_item->name() << "\".";
  MsgMgr::put_msg(__FILE__, __LINE__,
		  pt_item->file_region(),
		  MsgType::Error,
		  "ELAB",
		  buf.str());
}
#endif

END_NAMESPACE_YM_VERILOG
