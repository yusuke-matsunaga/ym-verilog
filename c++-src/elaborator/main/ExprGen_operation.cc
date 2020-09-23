
/// @file ExprGen_opration.cc
/// @brief ElbMgr の実装ファイル(式の実体化)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014, 2020 Yusuke Matsunaga
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
ExprGen::instantiate_opr(const VlScope* parent,
			 const ElbEnv& env,
			 const PtExpr* pt_expr)
{
  auto op_type{pt_expr->op_type()};
  SizeType opr_size{pt_expr->operand_num()};

  ElbExpr* opr0{nullptr};
  ElbExpr* opr1{nullptr};
  ElbExpr* opr2{nullptr};
  ElbExpr* expr{nullptr};
  bool real_check{false};
  switch ( op_type ) {
  case VpiOpType::Posedge:
  case VpiOpType::Negedge:
    ASSERT_COND( opr_size == 1 );
    error_illegal_edge_descriptor(pt_expr);
    return nullptr;

  case VpiOpType::BitNeg:
  case VpiOpType::UnaryAnd:
  case VpiOpType::UnaryNand:
  case VpiOpType::UnaryOr:
  case VpiOpType::UnaryNor:
  case VpiOpType::UnaryXor:
  case VpiOpType::UnaryXNor:
    real_check = true;
    // わざと次の case に続く．
  case VpiOpType::Plus:
  case VpiOpType::Minus:
  case VpiOpType::Not:
    ASSERT_COND( opr_size == 1 );
    opr0 = instantiate_expr(parent, env, pt_expr->operand0());
    if ( !opr0 ) {
      return nullptr;
    }
    if ( real_check && opr0->value_type().is_real_type() ) {
      error_illegal_real_type(pt_expr->operand0());
      return nullptr;
    }
    return mgr().new_UnaryOp(pt_expr, op_type, opr0);

  case VpiOpType::BitAnd:
  case VpiOpType::BitOr:
  case VpiOpType::BitXNor:
  case VpiOpType::BitXor:
  case VpiOpType::LShift:
  case VpiOpType::RShift:
  case VpiOpType::ArithLShift:
  case VpiOpType::ArithRShift:
    real_check = true;
    // わざと次の case に続く．
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
    ASSERT_COND( opr_size == 2 );
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
    expr = mgr().new_BinaryOp(pt_expr, op_type, opr0, opr1);
    break;

  case VpiOpType::Condition:
  case VpiOpType::MinTypMax:
    ASSERT_COND( opr_size == 3 );
    opr0 = instantiate_expr(parent, env, pt_expr->operand0());
    opr1 = instantiate_expr(parent, env, pt_expr->operand1());
    opr2 = instantiate_expr(parent, env, pt_expr->operand2());
    if ( !opr0 || !opr1 || !opr2 ) {
      return nullptr;
    }
    expr = mgr().new_TernaryOp(pt_expr, op_type, opr0, opr1, opr2);
    break;

  case VpiOpType::Concat:
    {
      vector<ElbExpr*> opr_list(opr_size);
      for ( SizeType i = 0; i < opr_size; ++ i ) {
	auto pt_expr1{pt_expr->operand(i)};
	auto expr1{instantiate_expr(parent, env, pt_expr1)};
	if ( !expr1 ) {
	  return nullptr;
	}
	auto type1{expr1->value_type()};
	if ( type1.is_real_type() ) {
	  error_illegal_real_type(pt_expr1);
	  return nullptr;
	}
	opr_list[i] = expr1;
      }

      expr = mgr().new_ConcatOp(pt_expr, opr_list);
    }
    break;

  case VpiOpType::MultiConcat:
    {
      auto pt_expr0{pt_expr->operand(0)};

      int rep_num;
      bool stat{evaluate_int(parent, pt_expr0, rep_num, true)};
      if ( !stat ) {
	return nullptr;
      }

      auto rep_expr{instantiate_expr(parent, env, pt_expr0)};
      vector<ElbExpr*> opr_list(opr_size - 1);
      for ( SizeType i = 1; i < opr_size; ++ i ) {
	auto pt_expr1{pt_expr->operand(i)};
	auto expr1{instantiate_expr(parent, env, pt_expr1)};
	if ( !expr1 ) {
	  return nullptr;
	}
	auto type1{expr1->value_type()};
	if ( type1.is_real_type() ) {
	  error_illegal_real_type(pt_expr1);
	  return nullptr;
	}
	opr_list[i - 1] = expr1;
      }
      expr = mgr().new_MultiConcatOp(pt_expr, rep_num, rep_expr, opr_list);
    }
    break;

  default:
    ASSERT_NOT_REACHED;
    return nullptr;
  }

  // attribute instance の生成
  auto attr_list{attribute_list(pt_expr)};
  mgr().reg_attr(expr, attr_list);

  return expr;
}

END_NAMESPACE_YM_VERILOG
