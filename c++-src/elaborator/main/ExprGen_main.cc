
/// @file ExprGen_main.cc
/// @brief ExprGen の実装ファイル(メイン)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.


#include "ExprGen.h"
#include "ElbEnv.h"
#include "ErrorGen.h"
#include "ElbError.h"

#include "ym/BitVector.h"
#include "ym/pt/PtItem.h"
#include "ym/pt/PtExpr.h"
#include "ym/pt/PtMisc.h"

#include "elaborator/ElbExpr.h"


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
      ErrorGen::illegal_sysfunccall_in_cf(__FILE__, __LINE__, pt_expr);
    }
    if ( env.is_constant() ) {
      // 定数式内では system function は使えない．
      ErrorGen::illegal_sysfunccall_in_ce(__FILE__, __LINE__, pt_expr);
    }
    return instantiate_sysfunccall(parent, env, pt_expr);

  case PtExprType::Primary:
    return instantiate_primary(parent, env, pt_expr);

  default:
    ASSERT_NOT_REACHED;
  }

  // コンパイラを騙すためのダミー
  return nullptr;
}

// @brief PtExpr から定数式の ElbExpr を生成する
// @param[in] parent 親のスコープ
// @param[in] pt_expr 式を表すパース木
// @return 生成された ElbExpr のポインタを返す．
ElbExpr*
ExprGen::instantiate_constant_expr(const VlScope* parent,
				   const PtExpr* pt_expr)
{
  ASSERT_COND( pt_expr != nullptr );

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
	auto expr = mgr().new_UnaryOp(pt_expr, pt_expr->op_type(), opr0);

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
    ErrorGen::illegal_constant_in_event_expression(__FILE__, __LINE__, pt_expr);
    break;

  case PtExprType::FuncCall:
    // イベント式の根元には関数呼び出しは使えない．
    ErrorGen::illegal_funccall_in_event_expression(__FILE__, __LINE__, pt_expr);
    break;

  case PtExprType::SysFuncCall:
    // イベント式の根元にはシステム関数呼び出しは使えない．
    ErrorGen::illegal_sysfunccall_in_event_expression(__FILE__, __LINE__, pt_expr);
    break;

  default:
    break;
  }

  ASSERT_NOT_REACHED;
  // コンパイラの不平をごまかすためのダミー
  return nullptr;
}

// @brief PtExpr からシステム関数の引数を生成する．
// @param[in] parent 親のスコープ
// @param[in] env 生成時の環境
// @param[in] pt_expr 式を表すパース木
// @return 生成された ElbExpr のポインタを返す．
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
	opr_list[pos] = expr1;
      }
      auto expr{mgr().new_Lhs(pt_expr, opr_list, elem_array)};

      // attribute instance の生成
      auto attr_list{attribute_list(pt_expr)};
      mgr().reg_attr(expr, attr_list);

      return expr;
    }
    else {
      // それ以外の演算子はエラー
      ErrorGen::illegal_operator_in_lhs(__FILE__, __LINE__, pt_expr);
    }
    break;


  case PtExprType::Primary:
    return instantiate_primary(parent, env, pt_expr);

  case PtExprType::Const:
    ErrorGen::illegal_constant_in_lhs(__FILE__, __LINE__, pt_expr);
    break;

  case PtExprType::FuncCall:
    ErrorGen::illegal_funccall_in_lhs(__FILE__, __LINE__, pt_expr);
    break;

  case PtExprType::SysFuncCall:
    ErrorGen::illegal_sysfunccall_in_lhs(__FILE__, __LINE__, pt_expr);
    break;

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
	opr_list[pos] = instantiate_lhs_sub(parent, env, pt_expr1, elem_array);
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
      ErrorGen::illegal_operator_in_lhs(__FILE__, __LINE__, pt_expr);
    }
    break;


  case PtExprType::Primary:
    {
      auto expr{instantiate_primary(parent, env, pt_expr)};
      elem_array.push_back(expr);
      return expr;
    }

  case PtExprType::Const:
    ErrorGen::illegal_constant_in_lhs(__FILE__, __LINE__, pt_expr);
    break;

  case PtExprType::FuncCall:
    ErrorGen::illegal_funccall_in_lhs(__FILE__, __LINE__, pt_expr);
    break;

  case PtExprType::SysFuncCall:
    ErrorGen::illegal_sysfunccall_in_lhs(__FILE__, __LINE__, pt_expr);
    break;

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
    // もともと遅延式を持たない場合は nullptr を返す．
    // エラーではない．
    return nullptr;
  }

  SizeType n = 0;
  vector<const PtExpr*> expr_array;
  expr_array.reserve(3);
  for ( SizeType n = 0; n < 3; ++ n) {
    auto expr{pt_delay->value(n)};
    if ( expr == nullptr ) break;
    expr_array.push_back(expr);
  }
  ASSERT_COND( !expr_array.empty() );

  return instantiate_delay_sub(parent, pt_delay, expr_array);
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
    // もともと遅延式を持たない場合は nullptr を返す．
    // エラーではない．
    return nullptr;
  }

  SizeType n{pt_header->paramassign_num()};
  ASSERT_COND( n == 1 );

  auto pt_con{pt_header->paramassign(0)};
  vector<const PtExpr*> expr_array{pt_con->expr()};

  return instantiate_delay_sub(parent, pt_header, expr_array);
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
			       const vector<const PtExpr*>& pt_expr_array)
{
  ASSERT_COND( pt_expr_array.size() <= 3 );

  try {
    // TODO : 環境の条件をチェック
    ElbEnv env;
    vector<ElbExpr*> expr_list;
    expr_list.reserve(pt_expr_array.size());
    for ( auto pt_expr: pt_expr_array ) {
      expr_list.push_back(instantiate_expr(parent, env, pt_expr));
    }

    auto delay{mgr().new_Delay(pt_obj, expr_list)};

    return delay;
  }
  catch ( const ElbError& error ) {
    put_error(error);
    return nullptr;
  }
}

END_NAMESPACE_YM_VERILOG
