
/// @file ExprGen_main.cc
/// @brief ExprGen の実装ファイル(メイン)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ExprGen.h"
#include "ElbEnv.h"

#include "ym/BitVector.h"
#include "ym/pt/PtItem.h"
#include "ym/pt/PtExpr.h"
#include "ym/pt/PtMisc.h"

#include "elb/ElbExpr.h"

#include "ym/MsgMgr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス ExprGen
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] elab 生成器
// @param[in] elb_mgr Elbオブジェクトを管理するクラス
// @param[in] elb_factory Elbオブジェクトを生成するファクトリクラス
ExprGen::ExprGen(Elaborator& elab,
		 ElbMgr& elb_mgr,
		 ElbFactory& elb_factory) :
  ElbProxy(elab, elb_mgr, elb_factory)
{
}

// @brief デストラクタ
ExprGen::~ExprGen()
{
}

// @brief PtExpr から ElbiExpr を生成する
// @param[in] parent 親のスコープ
// @param[in] env 生成時の環境
// @param[in] pt_expr 式を表すパース木
// @return 生成された ElbExpr のポインタを返す．
// @note 不適切な式ならばエラーメッセージを出力し nullptr を返す．
ElbExpr*
ExprGen::instantiate_expr(const VlNamedObj* parent,
			  const ElbEnv& env,
			  const PtExpr* pt_expr)
{
  // '(' expression ')' の時の対応
  while ( pt_expr->type() == PtExprType::Opr &&
	  pt_expr->op_type() == VpiOpType::Null ) {
    pt_expr = pt_expr->operand0();
  }

  switch ( pt_expr->type() ) {
  case PtExprType::Opr:
    return instantiate_opr(parent, env, pt_expr);

  case PtExprType::Const:
    return factory().new_Constant(pt_expr);

  case PtExprType::FuncCall:
    return instantiate_funccall(parent, env, pt_expr);

  case PtExprType::SysFuncCall:
    if ( env.inside_constant_function() ) {
      error_illegal_sysfunccall_in_cf(pt_expr);
      return nullptr;
    }
    if ( env.is_constant() ) {
      error_illegal_sysfunccall_in_ce(pt_expr);
      return nullptr;
    }
    return instantiate_sysfunccall(parent, env, pt_expr);

  case PtExprType::Primary:
    return instantiate_primary(parent, env, pt_expr);

  default:
    ASSERT_NOT_REACHED;
  }

  return nullptr;
}

// @brief PtExpr から定数式の ElbExpr を生成する
// @param[in] parent 親のスコープ
// @param[in] pt_expr 式を表すパース木
// @return 生成された ElbExpr のポインタを返す．
// @note 不適切な式ならばエラーメッセージを出力し nullptr を返す．
ElbExpr*
ExprGen::instantiate_constant_expr(const VlNamedObj* parent,
				   const PtExpr* pt_expr)
{
  ElbConstantEnv env;
  return instantiate_expr(parent, env, pt_expr);
}

// @brief PtExpr からイベント式を生成する．
// @param[in] parent 親のスコープ
// @param[in] env 生成時の環境
// @param[in] pt_expr 式を表すパース木
// @note 不適切な式ならばエラーメッセージを出力し nullptr を返す．
ElbExpr*
ExprGen::instantiate_event_expr(const VlNamedObj* parent,
				const ElbEnv& env,
				const PtExpr* pt_expr)
{
  // '(' expression ')' の時の対応
  while ( pt_expr->type() == PtExprType::Opr &&
	  pt_expr->op_type() == VpiOpType::Null ) {
    pt_expr = pt_expr->operand0();
  }

  switch ( pt_expr->type() ) {
  case PtExprType::Opr:
    switch ( pt_expr->op_type() ) {
    case VpiOpType::Posedge:
    case VpiOpType::Negedge:
      { // これのみがイベント式の特徴
	ASSERT_COND(pt_expr->operand_num() == 1 );
	ElbExpr* opr0 = instantiate_expr(parent, env, pt_expr->operand0());
	if ( !opr0 ) {
	  return nullptr;
	}
	ElbExpr* expr = factory().new_UnaryOp(pt_expr,
					      pt_expr->op_type(), opr0);

#if 0
	// attribute instance の生成
	instantiate_attribute(pt_expr->attr_top(), false, expr);
#else
#warning "TODO:2011-02-09-01"
#endif

	return expr;
      }

    default:
      // それ以外の場合は通常の処理を行う．
      return instantiate_opr(parent, env, pt_expr);
    }
    break;

  case PtExprType::Primary:
    // 通常の識別子に加えて named event も扱う．
    {
      ElbEventExprEnv env1(env);
      return instantiate_primary(parent, env1, pt_expr);
    }

  case PtExprType::Const:
    // イベント式の根元には定数は使えない．
    error_illegal_constant_in_event_expression(pt_expr);
    return nullptr;

  case PtExprType::FuncCall:
    // イベント式の根元には関数呼び出しは使えない．
    error_illegal_funccall_in_event_expression(pt_expr);
    return nullptr;

  case PtExprType::SysFuncCall:
    // イベント式の根元にはシステム関数呼び出しは使えない．
    error_illegal_sysfunccall_in_event_expression(pt_expr);
    return nullptr;

  default:
    break;
  }

  ASSERT_NOT_REACHED;

  return nullptr;
}

// @brief PtExpr からシステム関数の引数を生成する．
// @param[in] parent 親のスコープ
// @param[in] env 生成時の環境
// @param[in] pt_expr 式を表すパース木
// @return 生成された ElbExpr のポインタを返す．
// @note 不適切な式ならばエラーメッセージを出力し nullptr を返す．
ElbExpr*
ExprGen::instantiate_arg(const VlNamedObj* parent,
			 const ElbEnv& env,
			 const PtExpr* pt_expr)
{
  // '(' expression ')' の時の対応
  while ( pt_expr->type() == PtExprType::Opr &&
	  pt_expr->op_type() == VpiOpType::Null ) {
    pt_expr = pt_expr->operand0();
  }

  if ( pt_expr->type() == PtExprType::Primary ) {
    // システム関数の引数用の特別処理はここだけ．
    ElbSystemTfArgEnv env1(env);
    return instantiate_primary(parent, env1, pt_expr);
  }
  return instantiate_expr(parent, env, pt_expr);
}

// @brief PtExpr から左辺式を生成する
// @param[in] parent 親のスコープ
// @param[in] env 生成時の環境
// @param[in] pt_expr 式を表すパース木
// @return 生成された ElbExpr のポインタを返す．
// @note 不適切な式ならばエラーメッセージを出力し nullptr を返す．
ElbExpr*
ExprGen::instantiate_lhs(const VlNamedObj* parent,
			 const ElbEnv& env,
			 const PtExpr* pt_expr)
{
  switch ( pt_expr->type() ) {
  case PtExprType::Opr:
    // 左辺では concatination しか適当でない．
    if ( pt_expr->op_type() == VpiOpType::Concat ) {
      vector<ElbExpr*> elem_array;
      SizeType opr_size = pt_expr->operand_num();
      ElbExpr** opr_list = factory().new_ExprList(opr_size);
      for ( SizeType i = 0; i < opr_size; ++ i ) {
	SizeType pos = opr_size - i - 1;
	const PtExpr* pt_expr1 = pt_expr->operand(pos);
	vector<ElbExpr*> tmp_array;
	ElbExpr* expr1 = instantiate_lhs_sub(parent, env, pt_expr1, tmp_array);
	if ( !expr1 ) {
	  return nullptr;
	}
	opr_list[pos] = expr1;
	elem_array.insert(elem_array.end(), tmp_array.begin(), tmp_array.end());
      }
      SizeType n = elem_array.size();
      ElbExpr** lhs_elem_array = factory().new_ExprList(n);
      for ( SizeType i = 0; i < n; ++ i ) {
	lhs_elem_array[i] = elem_array[i];
      }
      ElbExpr* expr = factory().new_Lhs(pt_expr, opr_size, opr_list,
					n, lhs_elem_array);
      expr->set_selfsize();

#if 0
      // attribute instance の生成
      instantiate_attribute(pt_expr->attr_top(), false, expr);
#else
#warning "TODO:2011-02-09-01"
#endif

      return expr;
    }
    // それ以外の演算子はエラー
    error_illegal_operator_in_lhs(pt_expr);
    return nullptr;


  case PtExprType::Primary:
    return instantiate_primary(parent, env, pt_expr);

  case PtExprType::Const:
    error_illegal_constant_in_lhs(pt_expr);
    return nullptr;

  case PtExprType::FuncCall:
    error_illegal_funccall_in_lhs(pt_expr);
    return nullptr;

  case PtExprType::SysFuncCall:
    error_illegal_sysfunccall_in_lhs(pt_expr);
    return nullptr;

  default:
    break;
  }

  ASSERT_NOT_REACHED;
  return nullptr;
}

// @brief PtExpr から左辺式を生成する
// @param[in] parent 親のスコープ
// @param[in] env 生成時の環境
// @param[in] pt_expr 式を表すパース木
// @param[out] elem_array 生成した左辺式の要素を格納するベクタ
// @return 生成した式を返す．
// @note 不適切な式ならばエラーメッセージを出力し nullptr を返す．
ElbExpr*
ExprGen::instantiate_lhs_sub(const VlNamedObj* parent,
			     const ElbEnv& env,
			     const PtExpr* pt_expr,
			     vector<ElbExpr*>& elem_array)
{
  elem_array.clear();
  switch ( pt_expr->type() ) {
  case PtExprType::Opr:
    // 左辺では concatination しか適当でない．
    if ( pt_expr->op_type() == VpiOpType::Concat ) {
      SizeType opr_size = pt_expr->operand_num();
      ElbExpr** opr_list = factory().new_ExprList(opr_size);
      for ( SizeType i = 0; i < opr_size; ++ i ) {
	SizeType pos = opr_size - i - 1;
	const PtExpr* pt_expr1 = pt_expr->operand(pos);
	vector<ElbExpr*> tmp_array;
	ElbExpr* expr1 = instantiate_lhs_sub(parent, env, pt_expr1, tmp_array);
	if ( !expr1 ) {
	  return nullptr;
	}
	opr_list[pos] = expr1;
	elem_array.insert(elem_array.end(), tmp_array.begin(), tmp_array.end());
      }
      ElbExpr* expr = factory().new_ConcatOp(pt_expr, opr_size, opr_list);
      expr->set_selfsize();

#if 0
      // attribute instance の生成
      instantiate_attribute(pt_expr->attr_top(), false, expr);
#else
#warning "TODO:2011-02-09-01"
#endif

      return expr;
    }
    // それ以外の演算子はエラー
    error_illegal_operator_in_lhs(pt_expr);
    return nullptr;


  case PtExprType::Primary:
    {
      ElbExpr* expr = instantiate_primary(parent, env, pt_expr);
      elem_array.push_back(expr);
      return expr;
    }

  case PtExprType::Const:
    error_illegal_constant_in_lhs(pt_expr);
    return nullptr;

  case PtExprType::FuncCall:
    error_illegal_funccall_in_lhs(pt_expr);
    return nullptr;

  case PtExprType::SysFuncCall:
    error_illegal_sysfunccall_in_lhs(pt_expr);
    return nullptr;

  default:
    break;
  }

  ASSERT_NOT_REACHED;
  return nullptr;
}

// @brief PtExpr を評価し int 値を返す．
// @param[in] parent 親のスコープ
// @param[in] pt_expr 式を表すパース木
// @param[out] value 評価値を格納する変数
// @param[in] put_error エラーを出力する時，true にする．
// @note 定数でなければエラーメッセージを出力し false を返す．
// @brief PtExpr から expression を生成し int 値を返す．
bool
ExprGen::evaluate_int(const VlNamedObj* parent,
		      const PtExpr* pt_expr,
		      int& value,
		      bool put_error)
{
  VlValue val = evaluate_expr(parent, pt_expr, put_error);
  if ( !val.is_int_compat() ) {
    if ( put_error ) {
      MsgMgr::put_msg(__FILE__, __LINE__,
		      pt_expr->file_region(),
		      MsgType::Error,
		      "ELAB",
		      "Integer value required.");
    }
    return false;
  }

  value = val.int_value();
  return true;
}

// @brief PtExpr を評価しスカラー値を返す．
// @param[in] parent 親のスコープ
// @param[in] pt_expr 式を表すパース木
// @param[out] value 評価値を格納する変数
// @param[in] put_error エラーを出力する時，true にする．
// @note 定数でなければエラーメッセージを出力し false を返す．
bool
ExprGen::evaluate_scalar(const VlNamedObj* parent,
			 const PtExpr* pt_expr,
			 VlScalarVal& value,
			 bool put_error)
{
  VlValue val = evaluate_expr(parent, pt_expr, put_error);
  value = val.scalar_value();
  return true;
}

// @brief PtExpr を評価し bool 値を返す．
// @param[in] parent 親のスコープ
// @param[in] pt_expr 式を表すパース木
// @param[out] value 評価値を格納する変数
// @param[in] put_error エラーを出力する時，true にする．
// @note 定数でなければエラーメッセージを出力し false を返す．
bool
ExprGen::evaluate_bool(const VlNamedObj* parent,
		       const PtExpr* pt_expr,
		       bool& value,
		       bool put_error)
{
  VlValue val = evaluate_expr(parent, pt_expr, put_error);
  value = val.logic_value().to_bool();
  return true;
}

// @brief PtExpr を評価しビットベクタ値を返す．
// @param[in] parent 親のスコープ
// @param[in] pt_expr 式を表すパース木
// @param[out] value 評価値を格納する変数
// @param[in] put_error エラーを出力する時，true にする．
// @note 定数でなければエラーメッセージを出力し false を返す．
bool
ExprGen::evaluate_bitvector(const VlNamedObj* parent,
			    const PtExpr* pt_expr,
			    BitVector& value,
			    bool put_error)
{
  VlValue val = evaluate_expr(parent, pt_expr, put_error);
  if ( !val.is_bitvector_compat() ) {
    if ( put_error ) {
      MsgMgr::put_msg(__FILE__, __LINE__,
		      pt_expr->file_region(),
		      MsgType::Error,
		      "ELAB",
		      "Bit-vector value required.");
    }
    return false;
  }

  value = val.bitvector_value();
  return true;
}

// @brief 式の値を評価する．
// @param[in] parent 親のスコープ
// @param[in] pt_expr 式を表すパース木
// @param[in] put_error エラーを出力する時，true にする．
VlValue
ExprGen::evaluate_expr(const VlNamedObj* parent,
		       const PtExpr* pt_expr,
		       bool put_error)
{
  // '(' expression ')' の時の対応
  while ( pt_expr->type() == PtExprType::Opr &&
	  pt_expr->op_type() == VpiOpType::Null ) {
    pt_expr = pt_expr->operand0();
  }

  switch ( pt_expr->type() ) {
  case PtExprType::Opr:
    return evaluate_opr(parent, pt_expr, put_error);

  case PtExprType::Const:
    return evaluate_const(parent, pt_expr);

  case PtExprType::FuncCall:
    return evaluate_funccall(parent, pt_expr, put_error);

  case PtExprType::SysFuncCall:
    if ( put_error ) {
      error_illegal_sysfunccall_in_ce(pt_expr);
    }
    break;

  case PtExprType::Primary:
    return evaluate_primary(parent, pt_expr, put_error);

  default:
    ASSERT_NOT_REACHED;
  }

  return VlValue();
}

// @brief 定数に対して式の値を評価する．
// @param[in] parent 親のスコープ
// @param[in] pt_expr 式を表すパース木
VlValue
ExprGen::evaluate_const(const VlNamedObj* parent,
			const PtExpr* pt_expr)
{
  SizeType size = pt_expr->const_size();
  bool is_signed = false;
  SizeType base = 0;
  switch ( pt_expr->const_type() ) {
  case VpiConstType::Int:
    if ( pt_expr->const_str() == nullptr ) {
      return VlValue(static_cast<int>(pt_expr->const_uint()));
    }
    break;

  case VpiConstType::Real:
    return VlValue(pt_expr->const_real());

  case VpiConstType::SignedBinary:
    is_signed = true;
  case VpiConstType::Binary:
    base = 2;
    break;

  case VpiConstType::SignedOct:
    is_signed = true;
  case VpiConstType::Oct:
    base = 8;
    break;

  case VpiConstType::SignedDec:
    is_signed = true;
  case VpiConstType::Dec:
    base = 10;
    break;

  case VpiConstType::SignedHex:
    is_signed = true;
  case VpiConstType::Hex:
    base = 16;
    break;

  case VpiConstType::String:
    return VlValue(BitVector(pt_expr->const_str()));

  default:
    ASSERT_NOT_REACHED;
    break;
  }

  // ここに来たということはビットベクタ型
  return VlValue(BitVector(size, is_signed, base, pt_expr->const_str()));
}

// @brief PtDelay から ElbExpr を生成する．
// @param[in] parent 親のスコープ
// @param[in] pt_delay 遅延を表すパース木
const VlDelay*
ExprGen::instantiate_delay(const VlNamedObj* parent,
			   const PtDelay* pt_delay)
{
  SizeType n = 0;
  const PtExpr* expr_array[3];
  for ( ; n < 3; ++ n) {
    const PtExpr* expr = pt_delay->value(n);
    if ( expr == nullptr ) break;
    expr_array[n] = expr;
  }
  ASSERT_COND(n > 0 );

  return instantiate_delay_sub(parent, pt_delay, n, expr_array);
}

// @brief PtOrderedCon から ElbExpr を生成する．
// @param[in] parent 親のスコープ
// @param[in] pt_head 順序付き割り当て式
// これは PtInst の前にある # つきの式がパラメータ割り当てなのか
// 遅延なのかわからないので PtOrderedCon で表していることによる．
const VlDelay*
ExprGen::instantiate_delay(const VlNamedObj* parent,
			   const PtItem* pt_header)
{
  SizeType n = pt_header->paramassign_num();
  ASSERT_COND( n == 1 );

  const PtExpr* expr_array[1];
  const PtConnection* pt_con = pt_header->paramassign(0);
  expr_array[0] = pt_con->expr();
  return instantiate_delay_sub(parent, pt_header, 1, expr_array);
}

// @brief instantiate_delay の下請け関数
// @param[in] parent 親のスコープ
// @param[in] pt_obj 遅延式を表すパース木
// @param[in] n 要素数
// @param[in] expr_array 遅延式の配列
// @note pt_obj は PtDelay か PtItem のどちらか
// @note n は最大で 3
const VlDelay*
ExprGen::instantiate_delay_sub(const VlNamedObj* parent,
			       const PtBase* pt_obj,
			       SizeType n,
			       const PtExpr* expr_array[])
{
  ASSERT_COND( n <= 3 );

  // TODO : 環境の条件をチェック
  ElbEnv env;
  auto expr_list = factory().new_ExprList(n);
  for ( SizeType i = 0; i < n; ++ i ) {
    auto pt_expr = expr_array[i];
    auto expr = instantiate_expr(parent, env, pt_expr);
    if ( !expr ) {
      return nullptr;
    }
    expr_list[i] = expr;
  }

  auto delay = factory().new_Delay(pt_obj, n, expr_list);

  return delay;
}

END_NAMESPACE_YM_VERILOG
