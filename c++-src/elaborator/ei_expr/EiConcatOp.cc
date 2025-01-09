
/// @file EiConcatOp.cc
/// @brief EiConcatOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.

#include "ei/EiFactory.h"
#include "ei/EiConcatOp.h"

#include "ym/pt/PtExpr.h"
#include "ym/BitVector.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief 連結演算子を生成する．
ElbExpr*
EiFactory::new_ConcatOp(
  const PtExpr* pt_expr,
  const vector<ElbExpr*>& opr_list
)
{
  auto op = new EiConcatOp{pt_expr, opr_list};
  return op;
}

// @brief 反復連結演算子を生成する．
ElbExpr*
EiFactory::new_MultiConcatOp(
  const PtExpr* pt_expr,
  SizeType rep_num,
  ElbExpr* rep_expr,
  const vector<ElbExpr*>& opr_list
)
{
  auto op = new EiMultiConcatOp{pt_expr, rep_num, rep_expr, opr_list};
  return op;
}


//////////////////////////////////////////////////////////////////////
// クラス EiConcatOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiConcatOp::EiConcatOp(
  const PtExpr* pt_expr,
  const vector<ElbExpr*>& opr_list
) : EiOperation{pt_expr},
    mOprList{opr_list}
{
  mSize = 0;
  for ( auto opr: mOprList ) {
    auto type1 = opr->value_type();
    ASSERT_COND( !type1.is_real_type() );

    SizeType size1 = type1.size();
    mSize += size1;
    // オペランドのサイズは self determined
    opr->set_selfsize();
  }
  // 自分自身のサイズも self determined
  set_selfsize();
}

// @brief デストラクタ
EiConcatOp::~EiConcatOp()
{
}

// @brief 式のタイプを返す．
VlValueType
EiConcatOp::value_type() const
{
  return VlValueType{false, true, mSize};
}

// @brief 定数の時 true を返す．
bool
EiConcatOp::is_const() const
{
  for ( const auto& opr: mOprList ) {
    if ( !opr->is_const() ) {
      return false;
    }
  }
  return true;
}

// @brief 要求される式の型を計算してセットする．
void
EiConcatOp::_set_reqsize(
  const VlValueType& type
)
{
  // なにもしない．
}

// @brief オペランド数を返す．
SizeType
EiConcatOp::operand_num() const
{
  return mOprList.size();
}

// @brief オペランドを返す．
const VlExpr*
EiConcatOp::operand(
  SizeType pos
) const
{
  ASSERT_COND( 0 <= pos && pos < operand_num() );

  return mOprList[pos];
}

// @brief オペランドのリストを返す．
vector<const VlExpr*>
EiConcatOp::operand_list() const
{
  return vector<const VlExpr*>{mOprList.begin(), mOprList.end()};
}

// @brief ビット幅を返す．
SizeType
EiConcatOp::bit_size() const
{
  return mSize;
}


//////////////////////////////////////////////////////////////////////
// クラス EiMultiConcatOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiMultiConcatOp::EiMultiConcatOp(
  const PtExpr* pt_expr,
  SizeType rep_num,
  ElbExpr* rep_expr,
  const vector<ElbExpr*>& opr_list
) : EiConcatOp{pt_expr, opr_list},
    mRepNum{rep_num},
    mRepExpr{rep_expr}
{
}

// @brief デストラクタ
EiMultiConcatOp::~EiMultiConcatOp()
{
}

// @brief 式のタイプを返す．
VlValueType
EiMultiConcatOp::value_type() const
{
  return VlValueType{false, true, bit_size() * mRepNum};
}

// @brief オペランド数を返す．
SizeType
EiMultiConcatOp::operand_num() const
{
  return EiConcatOp::operand_num() + 1;
}
// @brief オペランドを返す．
const VlExpr*
EiMultiConcatOp::operand(
  SizeType pos
) const
{
  if ( pos == 0 ) {
    return mRepExpr;
  }
  return EiConcatOp::operand(pos - 1);
}

// @brief オペランドのリストを返す．
vector<const VlExpr*>
EiMultiConcatOp::operand_list() const
{
  vector<const VlExpr*> ans_list;
  auto n = EiConcatOp::operand_num();
  ans_list.reserve(n);
  ans_list.push_back(mRepExpr);
  for ( SizeType i = 0; i < n; ++ i ) {
    auto expr = EiConcatOp::operand(i);
    ans_list.push_back(expr);
  }
  return ans_list;
}

// @brief 繰り返し数を返す．
SizeType
EiMultiConcatOp::rep_num() const
{
  return mRepNum;
}

END_NAMESPACE_YM_VERILOG
