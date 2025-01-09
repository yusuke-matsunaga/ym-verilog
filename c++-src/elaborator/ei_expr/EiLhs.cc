
/// @file EiLhs.cc
/// @brief EiLhs の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.

#include "ei/EiFactory.h"
#include "ei/EiLhs.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief 連結演算子の左辺式を生成する．
ElbExpr*
EiFactory::new_Lhs(
  const PtExpr* pt_expr,
  const vector<ElbExpr*>& opr_array,
  const vector<ElbExpr*>& lhs_elem_array
)
{
  auto lhs = new EiLhs{pt_expr, opr_array, lhs_elem_array};
  return lhs;
}


//////////////////////////////////////////////////////////////////////
// クラス EiLhs
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiLhs::EiLhs(
  const PtExpr* pt_expr,
  const vector<ElbExpr*>& opr_array,
  const vector<ElbExpr*>& lhs_elem_array
) : EiConcatOp{pt_expr, opr_array},
    mArray{lhs_elem_array}
{
}

// @brief デストラクタ
EiLhs::~EiLhs()
{
  // mArray は別のオブジェクトが管理している．
}

// @brief 左辺式の要素数の取得
SizeType
EiLhs::lhs_elem_num() const
{
  return mArray.size();
}

// @brief 左辺式の要素の取得
const VlExpr*
EiLhs::lhs_elem(
  SizeType pos
) const
{
  ASSERT_COND( 0 <= pos && pos < lhs_elem_num() );
  return mArray[pos];
}

// @brief 左辺式の要素のリストの取得
vector<const VlExpr*>
EiLhs::lhs_elem_list() const
{
  return vector<const VlExpr*>{mArray.begin(), mArray.end()};
}

END_NAMESPACE_YM_VERILOG
