
/// @file ExprGen_opration.cc
/// @brief ElbMgr の実装ファイル(式の実体化)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ExprGen.h"
#include "ElbEnv.h"

#include "ym/BitVector.h"
#include "ym/pt/PtExpr.h"
#include "ym/pt/PtMisc.h"

#include "elaborator/ElbExpr.h"


BEGIN_NAMESPACE_YM_VERILOG

// @brief PtOpr から ElbExpr を生成する．
// @param[in] parent 親のスコープ
// @param[in] pt_expr 式を表すパース木
// @param[in] env 生成時の環境
ElbExpr*
ExprGen::instantiate_opr(const VlNamedObj* parent,
			 const ElbEnv& env,
			 const PtExpr* pt_expr)
{
  VpiOpType op_type = pt_expr->op_type();
  SizeType opr_size = pt_expr->operand_num();

  ElbExpr* opr0 = nullptr;
  ElbExpr* opr1 = nullptr;
  ElbExpr* opr2 = nullptr;
  ElbExpr* expr = nullptr;
  bool real_check = false;
  switch ( op_type ) {
  case VpiOpType::Posedge:
  case VpiOpType::Negedge:
    ASSERT_COND(opr_size == 1 );
    error_illegal_edge_descriptor(pt_expr);
    return nullptr;

  case VpiOpType::BitNeg:
  case VpiOpType::UnaryAnd:
  case VpiOpType::UnaryNand:
  case VpiOpType::UnaryOr:
  case VpiOpType::UnaryNor:
  case VpiOpType::UnaryXor:
  case VpiOpType::UnaryXNor: real_check = true;
  case VpiOpType::Plus:
  case VpiOpType::Minus:
  case VpiOpType::Not:
    ASSERT_COND(opr_size == 1 );
    opr0 = instantiate_expr(parent, env, pt_expr->operand0());
    if ( !opr0 ) {
      return nullptr;
    }
    if ( real_check && opr0->value_type().is_real_type() ) {
      error_illegal_real_type(pt_expr->operand0());
      return nullptr;
    }
    return factory().new_UnaryOp(pt_expr, op_type, opr0);

  case VpiOpType::BitAnd:
  case VpiOpType::BitOr:
  case VpiOpType::BitXNor:
  case VpiOpType::BitXor:
  case VpiOpType::LShift:
  case VpiOpType::RShift:
  case VpiOpType::ArithLShift:
  case VpiOpType::ArithRShift: real_check = true;
  case VpiOpType::Add:
  case VpiOpType::Sub:
  case VpiOpType::Mult:
  case VpiOpType::Div:
  case VpiOpType::Mod:
  case VpiOpType::Power:
  case VpiOpType::LogAnd:
  case VpiOpType::LogOr:
  case VpiOpType::CaseEq:
  case VpiOpType::CaseNeq:
  case VpiOpType::Eq:
  case VpiOpType::Neq:
  case VpiOpType::Ge:
  case VpiOpType::Gt:
  case VpiOpType::Le:
  case VpiOpType::Lt:
    ASSERT_COND(opr_size == 2 );
    opr0 = instantiate_expr(parent, env, pt_expr->operand0());
    opr1 = instantiate_expr(parent, env, pt_expr->operand1());
    if ( !opr0 || !opr1 ) {
      return nullptr;
    }
    if ( real_check ) {
      if ( opr0->value_type().is_real_type() ) {
	error_illegal_real_type(pt_expr->operand0());
	return nullptr;
      }
      if ( opr1->value_type().is_real_type() ) {
	error_illegal_real_type(pt_expr->operand1());
	return nullptr;
      }
    }
    expr = factory().new_BinaryOp(pt_expr, op_type, opr0, opr1);
    break;

  case VpiOpType::Condition:
  case VpiOpType::MinTypMax:
    ASSERT_COND(opr_size == 3 );
    opr0 = instantiate_expr(parent, env, pt_expr->operand0());
    opr1 = instantiate_expr(parent, env, pt_expr->operand1());
    opr2 = instantiate_expr(parent, env, pt_expr->operand2());
    if ( !opr0 || !opr1 || !opr2 ) {
      return nullptr;
    }
    expr = factory().new_TernaryOp(pt_expr, op_type, opr0, opr1, opr2);
    break;

  case VpiOpType::Concat:
    {
      ElbExpr** opr_list = factory().new_ExprList(opr_size);
      for ( SizeType i = 0; i < opr_size; ++ i ) {
	const PtExpr* pt_expr1 = pt_expr->operand(i);
	ElbExpr* expr1 = instantiate_expr(parent, env, pt_expr1);
	if ( !expr1 ) {
	  return nullptr;
	}
	if ( expr1->value_type().is_real_type() ) {
	  error_illegal_real_type(pt_expr1);
	  return nullptr;
	}
	opr_list[i] = expr1;
      }

      expr = factory().new_ConcatOp(pt_expr, opr_size, opr_list);
      expr->set_selfsize();
    }
    break;

  case VpiOpType::MultiConcat:
    {
      const PtExpr* pt_expr0 = pt_expr->operand(0);
      int rep_num;
      bool stat = evaluate_int(parent, pt_expr0, rep_num, true);
      if ( !stat ) {
	return nullptr;
      }
      ElbExpr* rep_expr = instantiate_expr(parent, env, pt_expr0);
      ElbExpr** opr_list = factory().new_ExprList(opr_size - 1);
      for ( SizeType i = 1; i < opr_size; ++ i ) {
	const PtExpr* pt_expr1 = pt_expr->operand(i);
	ElbExpr* expr1 = instantiate_expr(parent, env, pt_expr1);
	if ( !expr1 ) {
	  return nullptr;
	}
	if ( expr1->value_type().is_real_type() ) {
	  error_illegal_real_type(pt_expr1);
	  return nullptr;
	}
	opr_list[i - 1] = expr1;
      }
      expr = factory().new_MultiConcatOp(pt_expr, rep_num, rep_expr,
					 opr_size - 1, opr_list);
      expr->set_selfsize();
    }
    break;

  default:
    ASSERT_NOT_REACHED;
    return nullptr;
  }

#if 0
  // attribute instance の生成
  instantiate_attribute(pt_expr->attr_top(), false, expr);
#else
#warning "TODO:2011-02-09-01"
#endif

  return expr;
}

// @brief 演算子に対して int 型の値を評価する．
// @param[in] parent 親のスコープ
// @param[in] pt_expr 式を表すパース木
// @param[in] put_error エラーを出力する時，true にする．
VlValue
ExprGen::evaluate_opr(const VlNamedObj* parent,
		      const PtExpr* pt_expr,
		      bool put_error)
{
  VpiOpType op_type = pt_expr->op_type();
  SizeType op_size = pt_expr->operand_num();
  vector<VlValue> val(3);

  // オペランドの値の評価を行う．
  for ( SizeType i = 0; i < op_size; ++ i ) {
    val[i] = evaluate_expr(parent, pt_expr->operand(i), put_error);
    if ( val[i].is_error() ) {
      return VlValue();
    }
  }

  // 結果の型のチェックを行う．
  switch ( op_type ) {
  case VpiOpType::Posedge:
  case VpiOpType::Negedge:
    // この演算は使えない．
    error_illegal_edge_descriptor(pt_expr);
    return VlValue();

  case VpiOpType::BitNeg:
  case VpiOpType::UnaryAnd:
  case VpiOpType::UnaryNand:
  case VpiOpType::UnaryOr:
  case VpiOpType::UnaryNor:
  case VpiOpType::UnaryXor:
  case VpiOpType::UnaryXNor:
  case VpiOpType::BitAnd:
  case VpiOpType::BitOr:
  case VpiOpType::BitXNor:
  case VpiOpType::BitXor:
  case VpiOpType::LShift:
  case VpiOpType::RShift:
  case VpiOpType::ArithLShift:
  case VpiOpType::ArithRShift:
  case VpiOpType::Mod:
  case VpiOpType::Concat:
  case VpiOpType::MultiConcat:
    // この演算はビットベクタ型に変換できなければならない．
    for ( SizeType i = 0; i < op_size; ++ i ) {
      if ( !val[i].is_bitvector_compat() ) {
	if ( put_error ) {
	  error_illegal_real_type(pt_expr->operand(i));
	}
	return VlValue();
      }
    }
    break;

  case VpiOpType::Plus:
  case VpiOpType::Minus:
  case VpiOpType::Add:
  case VpiOpType::Sub:
  case VpiOpType::Mult:
  case VpiOpType::Div:
  case VpiOpType::Power:
  case VpiOpType::CaseEq:
  case VpiOpType::CaseNeq:
  case VpiOpType::Eq:
  case VpiOpType::Neq:
  case VpiOpType::Ge:
  case VpiOpType::Gt:
  case VpiOpType::Le:
  case VpiOpType::Lt:
  case VpiOpType::LogAnd:
  case VpiOpType::LogOr:
  case VpiOpType::Condition:
  case VpiOpType::MinTypMax:
    // この演算はどの型でもOK
    break;

  default:
    ASSERT_NOT_REACHED;
    return VlValue();
  }

  // 二回目は値の評価を行う．
  switch ( op_type ) {
  case VpiOpType::UnaryAnd:
    return reduction_and(val[0]);

  case VpiOpType::UnaryNand:
    return reduction_nand(val[0]);

  case VpiOpType::UnaryOr:
    return reduction_or(val[0]);

  case VpiOpType::UnaryNor:
    return reduction_nor(val[0]);

  case VpiOpType::UnaryXor:
    return reduction_xor(val[0]);

  case VpiOpType::UnaryXNor:
    return reduction_xor(val[0]);

  case VpiOpType::Plus:
    return val[0];

  case VpiOpType::Minus:
    return - val[0];

  case VpiOpType::BitNeg:
    return bit_negate(val[0]);

  case VpiOpType::BitAnd:
    return bit_and(val[0], val[1]);

  case VpiOpType::BitOr:
    return bit_or(val[0], val[1]);

  case VpiOpType::BitXNor:
    return bit_xnor(val[0], val[1]);

  case VpiOpType::BitXor:
    return bit_xor(val[0], val[1]);

  case VpiOpType::LShift:
    return val[0] << val[1];

  case VpiOpType::RShift:
    return val[0] >> val[1];

  case VpiOpType::ArithLShift:
    return alshift(val[0], val[1]);

  case VpiOpType::ArithRShift:
    return arshift(val[0], val[1]);

  case VpiOpType::Add:
    return val[0] + val[1];

  case VpiOpType::Sub:
    return val[0] - val[1];

  case VpiOpType::Mult:
    return val[0] * val[1];

  case VpiOpType::Div:
    return val[0] / val[1];

  case VpiOpType::Mod:
    return val[0] % val[1];

  case VpiOpType::Power:
    return power(val[0], val[1]);

  case VpiOpType::Not:
    return log_not(val[0]);

  case VpiOpType::LogAnd:
    return log_and(val[0], val[1]);

  case VpiOpType::LogOr:
    return log_or(val[0], val[1]);

  case VpiOpType::CaseEq:
    return eq_with_x(val[0], val[1]);

  case VpiOpType::CaseNeq:
    return log_not(eq_with_x(val[0], val[1]));

  case VpiOpType::Eq:
    return eq(val[0], val[1]);

  case VpiOpType::Neq:
    return log_not(eq(val[0], val[1]));

  case VpiOpType::Ge:
    return ge(val[0], val[1]);

  case VpiOpType::Gt:
    return log_not(ge(val[1], val[0]));

  case VpiOpType::Le:
    return ge(val[1], val[0]);

  case VpiOpType::Lt:
    return log_not(ge(val[0], val[1]));

  case VpiOpType::Condition:
    return ite(val[0], val[1], val[2]);

  case VpiOpType::MinTypMax:
    // 本当はエラー
    ASSERT_NOT_REACHED;
    break;

  case VpiOpType::Concat:
    return concat(val);

  case VpiOpType::MultiConcat:
    return multi_concat(val);

  default:
    ASSERT_NOT_REACHED;
    break;
  }

  // ダミー
  return VlValue();
}

END_NAMESPACE_YM_VERILOG
