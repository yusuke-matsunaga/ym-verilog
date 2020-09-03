
/// @file ExprGen_main.cc
/// @brief ExprGen の実装ファイル(メイン)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.


#include "ExprGen.h"
#include "ElbEnv.h"

#include "ym/BitVector.h"
#include "ym/pt/PtItem.h"
#include "ym/pt/PtExpr.h"
#include "ym/pt/PtMisc.h"

#include "elaborator/ElbExpr.h"

#include "ym/MsgMgr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス ExprGen
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] elab 生成器
// @param[in] elb_mgr Elbオブジェクトを管理するクラス
ExprGen::ExprGen(Elaborator& elab,
		 ElbMgr& elb_mgr) :
  ElbProxy(elab, elb_mgr)
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
ExprGen::instantiate_expr(const VlScope* parent,
			  const ElbEnv& env,
			  const PtExpr* pt_expr)
{
  // '(' expression ')' の時の対応
  while ( pt_expr->type() == PtExprType::Opr &&
	  pt_expr->op_type() == VpiOpType::Null ) {
    pt_expr = pt_expr->operand0();
  }

  // ここでは式の種類に応じたサブルーティンをディスパッチするだけ．
  switch ( pt_expr->type() ) {
  case PtExprType::Opr:
    return instantiate_opr(parent, env, pt_expr);

  case PtExprType::Const:
    return mgr().new_Constant(pt_expr);

  case PtExprType::FuncCall:
    return instantiate_funccall(parent, env, pt_expr);

  case PtExprType::SysFuncCall:
    if ( env.inside_constant_function() ) {
      // constant_function 内では system function は使えない．
      error_illegal_sysfunccall_in_cf(pt_expr);
      return nullptr;
    }
    if ( env.is_constant() ) {
      // 定数式内では system function は使えない．
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
ExprGen::instantiate_constant_expr(const VlScope* parent,
				   const PtExpr* pt_expr)
{
  if ( pt_expr == nullptr ) {
    return nullptr;
  }

  ElbConstantEnv env;
  return instantiate_expr(parent, env, pt_expr);
}

// @brief PtExpr からイベント式を生成する．
// @param[in] parent 親のスコープ
// @param[in] env 生成時の環境
// @param[in] pt_expr 式を表すパース木
// @note 不適切な式ならばエラーメッセージを出力し nullptr を返す．
ElbExpr*
ExprGen::instantiate_event_expr(const VlScope* parent,
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
	auto opr0 = instantiate_expr(parent, env, pt_expr->operand0());
	if ( !opr0 ) {
	  return nullptr;
	}
	auto expr = mgr().new_UnaryOp(pt_expr,
					  pt_expr->op_type(), opr0);

	// attribute instance の生成
	auto attr_list{attribute_list(pt_expr)};
	mgr().reg_attr(expr, attr_list);

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
ExprGen::instantiate_arg(const VlScope* parent,
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
ExprGen::instantiate_lhs(const VlScope* parent,
			 const ElbEnv& env,
			 const PtExpr* pt_expr)
{
  switch ( pt_expr->type() ) {
  case PtExprType::Opr:
    // 左辺では concatination しか適当でない．
    if ( pt_expr->op_type() == VpiOpType::Concat ) {
      vector<ElbExpr*> elem_array;
      SizeType opr_size{pt_expr->operand_num()};
      vector<ElbExpr*> opr_list(opr_size);
      for ( SizeType i = 0; i < opr_size; ++ i ) {
	// 現れた順は上位ビットからなので位置が逆になる．
	SizeType pos{opr_size - i - 1};
	auto pt_expr1{pt_expr->operand(pos)};
	auto expr1{instantiate_lhs_sub(parent, env, pt_expr1, elem_array)};
	if ( !expr1 ) {
	  return nullptr;
	}
	opr_list[pos] = expr1;
      }
      auto expr{mgr().new_Lhs(pt_expr, opr_list, elem_array)};

      // attribute instance の生成
      auto attr_list{attribute_list(pt_expr)};
      mgr().reg_attr(expr, attr_list);

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
ExprGen::instantiate_lhs_sub(const VlScope* parent,
			     const ElbEnv& env,
			     const PtExpr* pt_expr,
			     vector<ElbExpr*>& elem_array)
{
  switch ( pt_expr->type() ) {
  case PtExprType::Opr:
    // 左辺では concatination しか適当でない．
    if ( pt_expr->op_type() == VpiOpType::Concat ) {
      SizeType opr_size{pt_expr->operand_num()};
      vector<ElbExpr*> opr_list(opr_size);
      for ( SizeType i = 0; i < opr_size; ++ i ) {
	SizeType pos{opr_size - i - 1};
	auto pt_expr1{pt_expr->operand(pos)};
	auto expr1{instantiate_lhs_sub(parent, env, pt_expr1, elem_array)};
	if ( !expr1 ) {
	  return nullptr;
	}
	opr_list[pos] = expr1;
      }
      auto expr{mgr().new_ConcatOp(pt_expr, opr_list)};
      expr->set_selfsize();

      // attribute instance の生成
      auto attr_list{attribute_list(pt_expr)};
      mgr().reg_attr(expr, attr_list);

      return expr;
    }
    else {
      // それ以外の演算子はエラー
      error_illegal_operator_in_lhs(pt_expr);
      return nullptr;
    }


  case PtExprType::Primary:
    {
      auto expr{instantiate_primary(parent, env, pt_expr)};
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

// @brief PtDelay から ElbExpr を生成する．
// @param[in] parent 親のスコープ
// @param[in] pt_delay 遅延を表すパース木
const VlDelay*
ExprGen::instantiate_delay(const VlScope* parent,
			   const PtDelay* pt_delay)
{
  if ( pt_delay == nullptr ) {
    return nullptr;
  }

  SizeType n = 0;
  const PtExpr* expr_array[3];
  for ( ; n < 3; ++ n) {
    auto expr{pt_delay->value(n)};
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
ExprGen::instantiate_delay(const VlScope* parent,
			   const PtItem* pt_header)
{
  if ( pt_header == nullptr ) {
    return nullptr;
  }

  SizeType n{pt_header->paramassign_num()};
  ASSERT_COND( n == 1 );

  const PtExpr* expr_array[1];
  auto pt_con{pt_header->paramassign(0)};
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
ExprGen::instantiate_delay_sub(const VlScope* parent,
			       const PtBase* pt_obj,
			       SizeType n,
			       const PtExpr* expr_array[])
{
  ASSERT_COND( n <= 3 );

  // TODO : 環境の条件をチェック
  ElbEnv env;
  vector<ElbExpr*> expr_list(n);
  for ( SizeType i = 0; i < n; ++ i ) {
    auto pt_expr{expr_array[i]};
    auto expr{instantiate_expr(parent, env, pt_expr)};
    if ( !expr ) {
      return nullptr;
    }
    expr_list[i] = expr;
  }

  auto delay{mgr().new_Delay(pt_obj, expr_list)};

  return delay;
}

END_NAMESPACE_YM_VERILOG
