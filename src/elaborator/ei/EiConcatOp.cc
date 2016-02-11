﻿
/// @file EiConcatOp.cc
/// @brief EiConcatOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "EiFactory.h"
#include "EiConcatOp.h"

#include "ym/pt/PtExpr.h"
#include "ym/BitVector.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief 連結演算子を生成する．
// @param[in] pt_expr パース木の定義要素
// @param[in] opr_num オペランド数
// @param[in] opr_list オペランドのリスト
ElbExpr*
EiFactory::new_ConcatOp(const PtExpr* pt_expr,
			ymuint opr_size,
			ElbExpr** opr_list)
{
  void* p = mAlloc.get_memory(sizeof(EiConcatOp));
  EiConcatOp* op = new (p) EiConcatOp(pt_expr, opr_size, opr_list);

  return op;
}

// @brief 反復連結演算子を生成する．
// @param[in] pt_expr パース木の定義要素
// @param[in] rep_num 繰り返し数
// @param[in] rep_expr 繰り返し数を表す式
// @param[in] opr_num オペランド数
// @param[in] opr_list オペランドのリスト
ElbExpr*
EiFactory::new_MultiConcatOp(const PtExpr* pt_expr,
			     int rep_num,
			     ElbExpr* rep_expr,
			     ymuint opr_size,
			     ElbExpr** opr_list)
{
  void* p = mAlloc.get_memory(sizeof(EiMultiConcatOp));
  EiMultiConcatOp* op = new (p) EiMultiConcatOp(pt_expr, rep_num, rep_expr,
						opr_size, opr_list);

  return op;
}


//////////////////////////////////////////////////////////////////////
// クラス EiConcatOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_expr パース木の定義要素
// @param[in] opr_size オペランド数
// @param[in] opr_array オペランドを格納する配列
EiConcatOp::EiConcatOp(const PtExpr* pt_expr,
		       ymuint opr_size,
		       ElbExpr** opr_array) :
  EiOperation(pt_expr),
  mOprNum(opr_size),
  mOprList(opr_array)
{
  ymuint n = operand_num();
  mSize = 0;
  for (ymuint i = 0; i < n; ++ i) {
    ElbExpr* expr = _operand(i);
    VlValueType type1 = expr->value_type();
    ASSERT_COND( !type1.is_real_type() );
    ymuint size1 = type1.size();
    mSize += size1;

    // オペランドのサイズは self determined
    expr->set_selfsize();
  }
}

// @brief デストラクタ
EiConcatOp::~EiConcatOp()
{
}

// @brief 式のタイプを返す．
VlValueType
EiConcatOp::value_type() const
{
  return VlValueType(false, true, mSize);
}

// @brief 定数の時 true を返す．
// @note オペランドが定数ならこの式も定数となる．
bool
EiConcatOp::is_const() const
{
  for (ymuint i = 0; i < mOprNum; ++ i) {
    if ( !mOprList[i]->is_const() ) {
      return false;
    }
  }
  return true;
}

// @brief 要求される式の型を計算してセットする．
// @param[in] type 要求される式の型
// @note 必要であればオペランドに対して再帰的に処理を行なう．
void
EiConcatOp::_set_reqsize(const VlValueType& type)
{
  // なにもしない．
}

// @brief オペランド数を返す．
ymuint
EiConcatOp::operand_num() const
{
  return mOprNum;
}

// @brief オペランドを返す．
// @param[in] pos 位置番号
ElbExpr*
EiConcatOp::_operand(ymuint pos) const
{
  return mOprList[pos];
}


//////////////////////////////////////////////////////////////////////
// クラス EiMultiConcatOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_expr パース木の定義要素
// @param[in] rep_num 繰り返し数
// @param[in] rep_expr 繰り返し数を表す式
// @param[in] opr_size オペランド数
// @param[in] opr_array オペランドを格納する配列
EiMultiConcatOp::EiMultiConcatOp(const PtExpr* pt_expr,
				 int rep_num,
				 ElbExpr* rep_expr,
				 ymuint opr_size,
				 ElbExpr** opr_array) :
  EiConcatOp(pt_expr, opr_size, opr_array),
  mRepNum(rep_num),
  mRepExpr(rep_expr)
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
  return VlValueType(false, true, bit_size() * mRepNum);
}

// @brief オペランド数を返す．
ymuint
EiMultiConcatOp::operand_num() const
{
  return EiConcatOp::operand_num() + 1;
}

// @brief 繰り返し数を返す．
// @note multiple concatenation の時のみ意味を持つ．
ymuint
EiMultiConcatOp::rep_num() const
{
  return mRepNum;
}

// @brief オペランドを返す．
// @param[in] pos 位置番号
ElbExpr*
EiMultiConcatOp::_operand(ymuint pos) const
{
  if ( pos == 0 ) {
    return mRepExpr;
  }
  return EiConcatOp::_operand(pos - 1);
}

END_NAMESPACE_YM_VERILOG
