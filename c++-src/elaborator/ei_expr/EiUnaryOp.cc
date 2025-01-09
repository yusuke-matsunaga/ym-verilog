
/// @file EiUnaryOp.cc
/// @brief EiUnaryOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.

#include "ei/EiFactory.h"
#include "ei/EiUnaryOp.h"

#include "ym/BitVector.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief 単項演算子を生成する．
ElbExpr*
EiFactory::new_UnaryOp(
  const PtExpr* pt_expr,
  VpiOpType op_type,
  ElbExpr* opr1
)
{
  ElbExpr* expr = nullptr;
  switch ( op_type ) {
  case VpiOpType::Posedge:
  case VpiOpType::Negedge:
    expr = new EiEventEdgeOp{pt_expr, opr1};
    break;

  case VpiOpType::BitNeg:
    expr = new EiBitNegOp{pt_expr, opr1};
    break;

  case VpiOpType::Plus:
  case VpiOpType::Minus:
    expr = new EiUnaryArithOp{pt_expr, opr1};
    break;

  case VpiOpType::UnaryAnd:
  case VpiOpType::UnaryNand:
  case VpiOpType::UnaryOr:
  case VpiOpType::UnaryNor:
  case VpiOpType::UnaryXor:
  case VpiOpType::UnaryXNor:
    expr = new EiReductionOp{pt_expr, opr1};
    break;

  case VpiOpType::Not:
    expr = new EiNotOp{pt_expr, opr1};
    break;

  default:
    ASSERT_NOT_REACHED;
    break;
  }
  return expr;
}


//////////////////////////////////////////////////////////////////////
// クラス EiUnaryOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiUnaryOp::EiUnaryOp(
  const PtExpr* pt_expr,
  ElbExpr* opr1
) : EiOperation{pt_expr},
    mOpr1{opr1}
{
}

// @brief デストラクタ
EiUnaryOp::~EiUnaryOp()
{
}

// @brief 定数の時 true を返す．
bool
EiUnaryOp::is_const() const
{
  return mOpr1->is_const();
}

// @brief オペランド数を返す．
SizeType
EiUnaryOp::operand_num() const
{
  return 1;
}

// @brief オペランドを返す．
const VlExpr*
EiUnaryOp::operand(
  SizeType pos
) const
{
  return mOpr1;
}

// @brief オペランドのリストを返す．
vector<const VlExpr*>
EiUnaryOp::operand_list() const
{
  return vector<const VlExpr*>{mOpr1};
}


//////////////////////////////////////////////////////////////////////
// クラス EiNotOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiNotOp::EiNotOp(
  const PtExpr* pt_expr,
  ElbExpr* opr1
) : EiUnaryOp{pt_expr, opr1}
{
  // オペランドのサイズは self determined
  opr1->set_selfsize();
}

// @brief デストラクタ
EiNotOp::~EiNotOp()
{
}

// @brief 式のタイプを返す．
VlValueType
EiNotOp::value_type() const
{
  // 結果は常に符号無し1ビット
  return VlValueType(false, true, 1);
}

// @brief 要求される式の型を計算してセットする．
void
EiNotOp::_set_reqsize(
  const VlValueType& type
)
{
  // この演算子は型が固定なので何もしない．
}


//////////////////////////////////////////////////////////////////////
// クラス EiBitNeg
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiBitNegOp::EiBitNegOp(
  const PtExpr* pt_expr,
  ElbExpr* opr1
) : EiUnaryOp{pt_expr, opr1}
{
  // オペランドの型とサイズをそのまま使う．
  mType = opr1->value_type();

  ASSERT_COND( !mType.is_real_type() );
}

// @brief デストラクタ
EiBitNegOp::~EiBitNegOp()
{
}

// @brief 式のタイプを返す．
VlValueType
EiBitNegOp::value_type() const
{
  return mType;
}

// @brief 要求される式の型を計算してセットする．
void
EiBitNegOp::_set_reqsize(
  const VlValueType& type
)
{
  mType = update_size(mType, type);
  mOpr1->set_reqsize(mType);
}


//////////////////////////////////////////////////////////////////////
// クラス EiReductionOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiReductionOp::EiReductionOp(
  const PtExpr* pt_expr,
  ElbExpr* opr1
) : EiUnaryOp{pt_expr, opr1}
{
  ASSERT_COND( !opr1->value_type().is_real_type() );

  // オペランドのサイズは self determined
  opr1->set_selfsize();
}

// @brief デストラクタ
EiReductionOp::~EiReductionOp()
{
}

// @brief 式のタイプを返す．
VlValueType
EiReductionOp::value_type() const
{
  // 結果は常に符号無し1ビット
  return VlValueType{false, true, 1};
}

// @brief 要求される式の型を計算してセットする．
void
EiReductionOp::_set_reqsize(
  const VlValueType& type
)
{
  // この演算子は型が固定なので何もしない．
}


//////////////////////////////////////////////////////////////////////
// クラス EiUnaryArithOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiUnaryArithOp::EiUnaryArithOp(
  const PtExpr* pt_expr,
  ElbExpr* opr1
) : EiUnaryOp{pt_expr, opr1}
{
}

// @brief デストラクタ
EiUnaryArithOp::~EiUnaryArithOp()
{
}

// @brief 式のタイプを返す．
VlValueType
EiUnaryArithOp::value_type() const
{
  // オペランドの型とサイズをそのまま使う．
  return mOpr1->value_type();
}

// @brief 要求される式の型を計算してセットする．
void
EiUnaryArithOp::_set_reqsize(
  const VlValueType& type
)
{
  mOpr1->set_reqsize(type);
}


//////////////////////////////////////////////////////////////////////
// クラス EiEventEdgeOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiEventEdgeOp::EiEventEdgeOp(
  const PtExpr* pt_expr,
  ElbExpr* opr1
) : EiUnaryOp{pt_expr, opr1}
{
}

// @brief デストラクタ
EiEventEdgeOp::~EiEventEdgeOp()
{
}

// @brief 式のタイプを返す．
VlValueType
EiEventEdgeOp::value_type() const
{
  return VlValueType();
}

// @brief 要求される式の型を計算してセットする．
void
EiEventEdgeOp::_set_reqsize(
  const VlValueType& type
)
{
  // なにもしない．
}

END_NAMESPACE_YM_VERILOG
