
/// @file EiBinaryOp.cc
/// @brief EiBinaryOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.

#include "ei/EiFactory.h"
#include "ei/EiBinaryOp.h"

#include "ym/BitVector.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief 2項演算子を生成する．
ElbExpr*
EiFactory::new_BinaryOp(
  const PtExpr* pt_expr,
  VpiOpType op_type,
  ElbExpr* opr0,
  ElbExpr* opr1
)
{
  ElbExpr* expr = nullptr;
  switch( op_type ) {
  case VpiOpType::BitAnd:
  case VpiOpType::BitOr:
  case VpiOpType::BitXNor:
  case VpiOpType::BitXor:
    expr = new EiBinaryBitOp{pt_expr, opr0, opr1};
    break;

  case VpiOpType::Add:
  case VpiOpType::Sub:
  case VpiOpType::Mult:
  case VpiOpType::Div:
  case VpiOpType::Mod:
    expr = new EiBinaryArithOp{pt_expr, opr0, opr1};
    break;

  case VpiOpType::Power:
    expr = new EiPowerOp{pt_expr, opr0, opr1};
    break;

  case VpiOpType::LShift:
  case VpiOpType::RShift:
  case VpiOpType::ArithLShift:
  case VpiOpType::ArithRShift:
    expr = new EiShiftOp{pt_expr, opr0, opr1};
    break;

  case VpiOpType::LogAnd:
  case VpiOpType::LogOr:
    expr = new EiBinaryLogOp{pt_expr, opr0, opr1};
    break;

  case VpiOpType::CaseEq:
  case VpiOpType::CaseNeq:
  case VpiOpType::Eq:
  case VpiOpType::Neq:
  case VpiOpType::Ge:
  case VpiOpType::Gt:
  case VpiOpType::Le:
  case VpiOpType::Lt:
    expr = new EiCompareOp{pt_expr, opr0, opr1};
    break;

  default:
    ASSERT_NOT_REACHED;
    break;
  }
  return expr;
}


//////////////////////////////////////////////////////////////////////
// クラス EIBinaryOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiBinaryOp::EiBinaryOp(
  const PtExpr* pt_expr,
  ElbExpr* opr1,
  ElbExpr* opr2
) : EiOperation{pt_expr},
    mOpr{opr1, opr2}
{
}

// @brief デストラクタ
EiBinaryOp::~EiBinaryOp()
{
}

// @brief 定数の時 true を返す．
bool
EiBinaryOp::is_const() const
{
  return mOpr[0]->is_const() && mOpr[1]->is_const();
}

// @brief オペランド数を返す．
SizeType
EiBinaryOp::operand_num() const
{
  return 2;
}

// @brief オペランドを返す．
const VlExpr*
EiBinaryOp::operand(
  SizeType pos
) const
{
  ASSERT_COND( 0 <= pos && pos < 2 );

  return mOpr[pos];
}


//////////////////////////////////////////////////////////////////////
// クラス EiCompareOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiCompareOp::EiCompareOp(
  const PtExpr* pt_expr,
  ElbExpr* opr1,
  ElbExpr* opr2
) : EiBinaryOp{pt_expr, opr1, opr2}
{
  // 比較演算は大きい方の型を用いる．
  auto type1 = opr1->value_type();
  auto type2 = opr2->value_type();

  mOprType = calc_type(type1, type2);

  opr1->set_reqsize(mOprType);
  opr2->set_reqsize(mOprType);
}

// @brief デストラクタ
EiCompareOp::~EiCompareOp()
{
}

// @brief 式のタイプを返す．
VlValueType
EiCompareOp::value_type() const
{
  // 常に1ビット符号なし
  return VlValueType(false, true, 1);
}

// @brief 要求される式の型を計算してセットする．
void
EiCompareOp::_set_reqsize(
  const VlValueType& type
)
{
  // なにもしない．
}


//////////////////////////////////////////////////////////////////////
// クラス EiBinaryLogOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiBinaryLogOp::EiBinaryLogOp(
  const PtExpr* pt_expr,
  ElbExpr* opr1,
  ElbExpr* opr2
) : EiBinaryOp{pt_expr, opr1, opr2}
{
  // 論理演算の場合はオペランドも1ビットスカラーのはずだが
  // 仕様書には max(L(i), L(j)) なんて書いてある．
  // これは == などと一緒に表記しているからだと思う．
  // 実際には self determined で計算しておいて
  // 評価時に true/false/X に分類する．
  opr1->set_selfsize();
  opr2->set_selfsize();
}

// @brief デストラクタ
EiBinaryLogOp::~EiBinaryLogOp()
{
}

// @brief 式のタイプを返す．
VlValueType
EiBinaryLogOp::value_type() const
{
  // 常に1ビット符号なし
  return VlValueType{false, true, 1};
}

// @brief 要求される式の型を計算してセットする．
void
EiBinaryLogOp::_set_reqsize(
  const VlValueType& type
)
{
  // なにもしない．
}


//////////////////////////////////////////////////////////////////////
// クラス EiBinaryBitOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiBinaryBitOp::EiBinaryBitOp(
  const PtExpr* pt_expr,
  ElbExpr* opr1,
  ElbExpr* opr2
) : EiBinaryOp{pt_expr, opr1, opr2}
{
  // オペランドのサイズの大きい方で決まる．
  auto type1 = opr1->value_type();
  auto type2 = opr2->value_type();

  mType = calc_type(type1, type2);

  ASSERT_COND( !mType.is_real_type() );
}

// @brief デストラクタ
EiBinaryBitOp::~EiBinaryBitOp()
{
}

// @brief 式のタイプを返す．
VlValueType
EiBinaryBitOp::value_type() const
{
  return mType;
}

// @brief 要求される式の型を計算してセットする．
void
EiBinaryBitOp::_set_reqsize(
  const VlValueType& type
)
{
  mType = update_size(mType, type);
  mOpr[0]->set_reqsize(mType);
  mOpr[1]->set_reqsize(mType);
}


//////////////////////////////////////////////////////////////////////
// クラス EiBinaryArithOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiBinaryArithOp::EiBinaryArithOp(
  const PtExpr* pt_expr,
  ElbExpr* opr1,
  ElbExpr* opr2
) : EiBinaryOp{pt_expr, opr1, opr2}
{
  // オペランドのサイズの大きい方で決まる．
  auto type1 = opr1->value_type();
  auto type2 = opr2->value_type();

  mType = calc_type(type1, type2);
}

// @brief デストラクタ
EiBinaryArithOp::~EiBinaryArithOp()
{
}

// @brief 式のタイプを返す．
VlValueType
EiBinaryArithOp::value_type() const
{
  return mType;
}

// @brief 要求される式の型を計算してセットする．
void
EiBinaryArithOp::_set_reqsize(
  const VlValueType& type
)
{
  mType = update_size(mType, type);
  mOpr[0]->set_reqsize(mType);
  mOpr[1]->set_reqsize(mType);
}


//////////////////////////////////////////////////////////////////////
// クラス EiPowerOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiPowerOp::EiPowerOp(
  const PtExpr* pt_expr,
  ElbExpr* opr1,
  ElbExpr* opr2
) : EiBinaryOp{pt_expr, opr1, opr2}
{
  // 巾乗演算の場合, どちらかのオペランドが real, integer, signed
  // なら結果は real, どちらも unsigned の時のみ unsigned となる．
  auto type1 = opr1->value_type();
  auto type2 = opr2->value_type();
  mType = calc_type2(type1, type2);

  // ただし opr2 は self-determined
  opr2->set_reqsize(type2);
}

// @brief デストラクタ
EiPowerOp::~EiPowerOp()
{
}

// @brief 式のタイプを返す．
VlValueType
EiPowerOp::value_type() const
{
  return mType;
}

// @brief 要求される式の型を計算してセットする．
void
EiPowerOp::_set_reqsize(
  const VlValueType& type
)
{
  mType = update_size(mType, type);

  // 第1オペランドは type をそのまま伝える．
  mOpr[0]->set_reqsize(mType);

  // 第2オペランドは不変
}


//////////////////////////////////////////////////////////////////////
// クラス EiShiftOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiShiftOp::EiShiftOp(
  const PtExpr* pt_expr,
  ElbExpr* opr1,
  ElbExpr* opr2
) : EiBinaryOp{pt_expr, opr1, opr2}
{
  // シフト演算子は第1オペランドの型とサイズをそのまま引き継ぐ
  mType = opr1->value_type();
  ASSERT_COND( !mType.is_real_type() );

  ASSERT_COND( !opr2->value_type().is_real_type() );

  // 第2オペランドのサイズは self determined
  opr2->set_selfsize();
}

// @brief デストラクタ
EiShiftOp::~EiShiftOp()
{
}

// @brief 式のタイプを返す．
VlValueType
EiShiftOp::value_type() const
{
  return mType;
}

// @brief 要求される式の型を計算してセットする．
void
EiShiftOp::_set_reqsize(
  const VlValueType& type
)
{
  mType = update_size(mType, type);

  // 第1オペランドは type をそのまま伝える．
  mOpr[0]->set_reqsize(mType);

  // 第2オペランドは不変
}

END_NAMESPACE_YM_VERILOG
