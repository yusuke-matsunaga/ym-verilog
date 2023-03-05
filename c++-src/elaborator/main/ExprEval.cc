
/// @file ExprEval.cc
/// @brief ExprEval の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2020 Yusuke Matsunaga
/// All rights reserved.

#include "ExprEval.h"
#include "FuncEval.h"
#include "ErrorGen.h"
#include "ElbError.h"
#include "elaborator/ElbGenvar.h"
#include "elaborator/ElbParameter.h"
#include "ym/pt/PtItem.h"
#include "ym/pt/PtExpr.h"
#include "ym/vl/VlStmt.h"
#include "ym/vl/VlTaskFunc.h"
#include "ym/vl/VlIODecl.h"
#include "ym/MsgMgr.h"


BEGIN_NAMESPACE_YM_VERILOG

// @brief コンストラクタ
ExprEval::ExprEval(
  Elaborator& elab,
  ElbMgr& elb_mgr
) : ElbProxy{elab, elb_mgr}
{
}

// @brief デストラクタ
ExprEval::~ExprEval()
{
}

// @brief 定数式を評価し int 値を返す．
int
ExprEval::evaluate_int(
  const VlScope* parent,
  const PtExpr* pt_expr
)
{
  auto val = evaluate_expr(parent, pt_expr);
  if ( !val.is_int_compat() ) {
    ErrorGen::int_required(__FILE__, __LINE__, pt_expr->file_region());
  }

  return val.int_value();
}

// @brief 定数式ならばを評価し int 値を返す．
int
ExprEval::evaluate_int_if_const(
  const VlScope* parent,
  const PtExpr* pt_expr,
  bool& is_const
)
{
  try {
    auto val = evaluate_expr(parent, pt_expr);
    if ( !val.is_int_compat() ) {
      ErrorGen::int_required(__FILE__, __LINE__, pt_expr->file_region());
    }
    is_const = true;
    return val.int_value();
  }
  catch ( ElbConstError ) {
    is_const = false;
  }
  catch ( ... ) {
    throw;
  }
  return 0;
}

// @brief 定数式を評価しスカラー値を返す．
VlScalarVal
ExprEval::evaluate_scalar(
  const VlScope* parent,
  const PtExpr* pt_expr
)
{
  auto val = evaluate_expr(parent, pt_expr);
  // この変換は失敗しない．
  return val.scalar_value();
}

// @brief 定数式を評価し bool 値を返す．
bool
ExprEval::evaluate_bool(
  const VlScope* parent,
  const PtExpr* pt_expr
)
{
  auto val = evaluate_expr(parent, pt_expr);
  // この変換は失敗しない．
  return val.logic_value().to_bool();
}

// @brief 定数式を評価しビットベクタ値を返す．
BitVector
ExprEval::evaluate_bitvector(
  const VlScope* parent,
  const PtExpr* pt_expr
)
{
  auto val = evaluate_expr(parent, pt_expr);
  if ( !val.is_bitvector_compat() ) {
    ErrorGen::bv_required(__FILE__, __LINE__, pt_expr->file_region());
  }

  return val.bitvector_value();
}

// @brief 範囲を表す式を評価する．
pair<int, int>
ExprEval::evaluate_range(
  const VlScope* parent,
  const PtExpr* pt_left,
  const PtExpr* pt_right
)
{
  int left_val = evaluate_int(parent, pt_left);
  int right_val = evaluate_int(parent, pt_right);
  return make_pair(left_val, right_val);
}

// @brief 式の値を評価する．
VlValue
ExprEval::evaluate_expr(
  const VlScope* parent,
  const PtExpr* pt_expr
)
{
  // '(' expression ')' の時の対応
  while ( pt_expr->type() == PtExprType::Opr &&
	  pt_expr->op_type() == VpiOpType::Null ) {
    pt_expr = pt_expr->operand0();
  }

  switch ( pt_expr->type() ) {
  case PtExprType::Opr:
    return evaluate_opr(parent, pt_expr);

  case PtExprType::Const:
    return evaluate_const(parent, pt_expr);

  case PtExprType::FuncCall:
    return evaluate_funccall(parent, pt_expr);

  case PtExprType::SysFuncCall:
    ErrorGen::illegal_sysfunccall_in_ce(__FILE__, __LINE__, pt_expr);

  case PtExprType::Primary:
    return evaluate_primary(parent, pt_expr);

  default:
    ASSERT_NOT_REACHED;
  }

  return VlValue{};
}

// @brief 演算子に対して int 型の値を評価する．
VlValue
ExprEval::evaluate_opr(
  const VlScope* parent,
  const PtExpr* pt_expr
)
{
  auto op_type = pt_expr->op_type();
  SizeType op_size{pt_expr->operand_num()};

  // オペランドの値の評価を行う．
  vector<VlValue> val(op_size);
  for ( SizeType i = 0; i < op_size; ++ i ) {
    val[i] = evaluate_expr(parent, pt_expr->operand(i));
  }

  // 結果の型のチェックを行う．
  switch ( op_type ) {
  case VpiOpType::Posedge:
  case VpiOpType::Negedge:
    // この演算は使えない．
    ErrorGen::illegal_edge_descriptor(__FILE__, __LINE__, pt_expr);
    break;

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
	ErrorGen::illegal_real_type(__FILE__, __LINE__, pt_expr->operand(i));
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
  return VlValue{};
}

// @brief プライマリに対して式の値を評価する．
VlValue
ExprEval::evaluate_primary(
  const VlScope* parent,
  const PtExpr* pt_expr
)
{
  // 識別子の階層
  if ( pt_expr->namebranch_num() > 0 ) {
    // 階層つき識別子はだめ
    ErrorGen::hname_in_ce(__FILE__, __LINE__, pt_expr);
  }

  SizeType isize = pt_expr->index_num();
  bool has_bit_select = (isize == 1);
  bool has_range_select = (pt_expr->left_range() && pt_expr->right_range());

  if (  isize > 1 || (isize == 1 && has_range_select) ) {
    // 配列型ではない．
    ErrorGen::dimension_mismatch(__FILE__, __LINE__, pt_expr);
  }

  int index1{0};
  int index2{0};
  if ( has_bit_select ) {
    index1 = evaluate_int(parent, pt_expr->index(0));
  }
  if ( has_range_select ) {
    auto pt_left = pt_expr->left_range();
    index1 = evaluate_int(parent, pt_left);
    auto pt_right = pt_expr->right_range();
    index2 = evaluate_int(parent, pt_right);
  }

  // モジュール内の識別子を探索する．
  auto handle = mgr().find_obj_up(parent, pt_expr, parent->parent_module());
  if ( !handle ) {
    // 見つからなかった．
    ErrorGen::not_found(__FILE__, __LINE__, pt_expr);
  }

  // そのオブジェクトが genvar の場合
  auto genvar = handle->genvar();
  if ( genvar ) {
    if ( has_bit_select ) {
      // ビット選択
      BitVector bv{genvar->value()};
      return VlValue{bv.bit_select_op(index1)};
    }
    else if ( has_range_select ) {
      // 範囲選択
      BitVector bv{genvar->value()};
      if ( index1 < index2 ) {
	ErrorGen::range_order(__FILE__, __LINE__, pt_expr);
      }
      return VlValue{bv.part_select_op(index1, index2)};
    }
    else {
      return VlValue{genvar->value()};
    }
  }

  // それ以外の宣言要素の場合
  // しかしこの場合には parameter でなければならない．
  auto param = handle->parameter();
  if ( !param ) {
    ErrorGen::not_a_parameter(__FILE__, __LINE__, pt_expr);
    return VlValue();
  }

  auto pt_init_expr = param->init_expr();
  auto val = evaluate_expr(parent, pt_init_expr);
  if ( param->value_type().is_real_type() ) {
    if ( has_bit_select || has_range_select ) {
      ErrorGen::illegal_real_type(__FILE__, __LINE__, pt_expr);
      return VlValue{};
    }
  }
  else {
    if ( has_bit_select ) {
      // ビット選択
      if ( !val.is_bitvector_compat() ) {
	ErrorGen::illegal_real_type(__FILE__, __LINE__, pt_expr);
      }
      SizeType offset{0};
      if ( !param->calc_bit_offset(index1, offset) ) {
	// インデックスが範囲外だった．
	// エラーではなく X になる．
	return VlValue{VlScalarVal::x()};
      }
      return VlValue{val.bitvector_value().bit_select_op(offset)};
    }
    else if ( has_range_select ) {
      if ( !val.is_bitvector_compat() ) {
	ErrorGen::illegal_real_type(__FILE__, __LINE__, pt_expr);
      }
      switch ( pt_expr->range_mode() ) {
      case VpiRangeMode::Const:
	{
	  bool big = (index1 >= index2);
	  if ( big ^ param->is_big_endian() ) {
	    ErrorGen::range_order(__FILE__, __LINE__, pt_expr);
	  }
	}
	break;

      case VpiRangeMode::Plus:
	if ( param->is_big_endian() ) {
	  int range{index2};
	  index2 = index1;
	  index1 = index1 + range - 1;
	}
	else {
	  index2 = index1 + index2 - 1;
	}
	break;

      case VpiRangeMode::Minus:
	if ( param->is_big_endian() ) {
	  index2 = index1 - index2 + 1;
	}
	else {
	  int range{index2};
	  index2 = index1;
	  index1 = index1 - range + 1;
	}
	ASSERT_NOT_REACHED;
	break;

      case VpiRangeMode::No:
	ASSERT_NOT_REACHED;
	break;
      }

      SizeType msb_offset;
      SizeType lsb_offset;
      bool stat1 = param->calc_bit_offset(index1, msb_offset);
      bool stat2 = param->calc_bit_offset(index2, lsb_offset);
      if ( stat1 && stat2 ) {
	return VlValue(val.bitvector_value().part_select_op(msb_offset, lsb_offset));
      }

      SizeType bw;
      if ( index1 < index2 ) {
	bw = index2 - index1 + 1;
      }
      else {
	bw = index1 - index2 + 1;
      }
      return VlValue{BitVector{VlScalarVal::x(), bw}};
    }
  }

  return val;
}

// @brief 定数に対して式の値を評価する．
VlValue
ExprEval::evaluate_const(
  const VlScope* parent,
  const PtExpr* pt_expr
)
{
  SizeType size{pt_expr->const_size()};
  bool is_signed{false};
  SizeType base{0};
  switch ( pt_expr->const_type() ) {
  case VpiConstType::Int:
    if ( pt_expr->const_str() == nullptr ) {
      auto val = static_cast<int>(pt_expr->const_uint32());
      return VlValue{val};
    }
    break;

  case VpiConstType::Real:
    return VlValue{pt_expr->const_real()};

  case VpiConstType::SignedBinary:
    is_signed = true;
    // わざと次の case につづく
  case VpiConstType::Binary:
    base = 2;
    break;

  case VpiConstType::SignedOct:
    is_signed = true;
    // わざと次の case につづく
  case VpiConstType::Oct:
    base = 8;
    break;

  case VpiConstType::SignedDec:
    is_signed = true;
    // わざと次の case につづく
  case VpiConstType::Dec:
    base = 10;
    break;

  case VpiConstType::SignedHex:
    is_signed = true;
    // わざと次の case につづく
  case VpiConstType::Hex:
    base = 16;
    break;

  case VpiConstType::String:
    return VlValue{BitVector{pt_expr->const_str()}};

  default:
    ASSERT_NOT_REACHED;
    break;
  }

  // ここに来たということはビットベクタ型
  return VlValue{BitVector{size, is_signed, base, pt_expr->const_str()}};
}

// @brief PtFuncCall から式の値を評価する．
VlValue
ExprEval::evaluate_funccall(
  const VlScope* parent,
  const PtExpr* pt_expr
)
{
  if ( pt_expr->namebranch_num() > 0 ) {
    // 階層名は使えない．
    ErrorGen::hname_in_ce(__FILE__, __LINE__, pt_expr);
  }

  // 関数名
  auto name = pt_expr->name();

  // 関数本体を探し出す．
  // constant function はモジュール直下にしかあり得ない
  // <- generated scope 内の関数は constant function ではない．
  auto module = parent->parent_module();
  auto pt_func = find_funcdef(module, name);
  if ( !pt_func ) {
    // 関数が見つからなかった．
    ErrorGen::no_such_function(__FILE__, __LINE__, pt_expr);
  }

  if ( pt_func->is_in_use() ) {
    // 再帰的な呼び出しも行えない．
    ErrorGen::uses_itself(__FILE__, __LINE__, pt_expr);
  }

  // 定数関数を探し出す．
  auto child_func = find_constant_function(module, name);
  if ( child_func == nullptr ) {
    pt_func->set_in_use();
    // なかったので作る．
    child_func = instantiate_constant_function(parent, pt_func);
    pt_func->clear_in_use();
  }
  if ( !child_func ) {
    // instantiate_constant_function が失敗した．
    // たぶん constant function ではなかった．
    ErrorGen::not_a_constant_function(__FILE__, __LINE__, pt_expr);
  }

  // 引数の生成
  SizeType n{pt_expr->operand_num()};
  if ( n != child_func->io_num() ) {
    // 引数の数が合わなかった．
    ErrorGen::n_of_arguments_mismatch(__FILE__, __LINE__, pt_expr);
  }

  vector<VlValue> arg_list(n);
  for ( SizeType i = 0; i < n; ++ i ) {
    auto pt_expr1 = pt_expr->operand(i);
    auto val1 = evaluate_expr(parent, pt_expr1);
    auto io_decl = child_func->io(i);
    auto decl = io_decl->decl();
    auto decl_type = decl->value_type();
    if ( decl_type.is_real_type() ) {
      if ( !val1.is_real_compat() ) {
	// 型が異なる．
	ErrorGen::illegal_argument_type(__FILE__, __LINE__, pt_expr1);
      }
    }
    else if ( decl_type.is_bitvector_type() ) {
      if ( !val1.is_bitvector_compat() ) {
	// 型が異なる．
	ErrorGen::illegal_argument_type(__FILE__, __LINE__, pt_expr1);
      }
    }
    arg_list[i] = val1;
  }

  // 関数の評価を行う．
  FuncEval eval{child_func};
  auto val = eval(arg_list);
  return val;
}

END_NAMESPACE_YM_VERILOG
