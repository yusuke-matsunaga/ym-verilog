
/// @file EiBitSelect.cc
/// @brief EiBitSelect の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.

#include "ei/EiFactory.h"
#include "ei/EiBitSelect.h"

#include "elaborator/ElbDecl.h"

#include "ym/BitVector.h"
#include "ym/pt/PtExpr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief 固定ビット選択式を生成する．
ElbExpr*
EiFactory::new_BitSelect(
  const PtExpr* pt_expr,
  ElbExpr* base_expr,
  const PtExpr* index_expr,
  int index_val
)
{
  auto expr = new EiConstBitSelect{pt_expr, base_expr, index_expr, index_val};
  return expr;
}

// @brief 固定ビット選択式を生成する．
ElbExpr*
EiFactory::new_BitSelect(
  const PtExpr* pt_expr,
  ElbExpr* base_expr,
  int index_val
)
{
  auto expr = new EiConstBitSelect{pt_expr, base_expr, nullptr, index_val};
  return expr;
}

// @brief 可変ビット選択式を生成する．
ElbExpr*
EiFactory::new_BitSelect(
  const PtExpr* pt_expr,
  ElbExpr* base_expr,
  ElbExpr* index_expr
)
{
  auto expr = new EiVarBitSelect{pt_expr, base_expr, index_expr};
  return expr;
}


//////////////////////////////////////////////////////////////////////
// クラス EiBitSelect
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiBitSelect::EiBitSelect(
  const PtExpr* pt_expr,
  ElbExpr* base_expr
) : EiExprBase{pt_expr},
    mBaseExpr{base_expr}
{
}

// @brief デストラクタ
EiBitSelect::~EiBitSelect()
{
}

// @brief 型の取得
VpiObjType
EiBitSelect::type() const
{
  switch ( parent_expr()->type() ) {
  case VpiObjType::Net: return VpiObjType::NetBit;
  case VpiObjType::Reg: return VpiObjType::RegBit;
  default: break;
  }
  return VpiObjType::BitSelect;
}

// @brief 式のタイプを返す．
VlValueType
EiBitSelect::value_type() const
{
  return VlValueType{false, true, 1};
}

// @brief 定数の時 true を返す．
bool
EiBitSelect::is_const() const
{
  return parent_expr()->is_const() && is_constant_select();
}

// @brief ビット指定の時に true を返す．
bool
EiBitSelect::is_bitselect() const
{
  return true;
}

// @brief 宣言要素もしくは配列型宣言要素への参照を返す．
const VlDeclBase*
EiBitSelect::decl_base() const
{
  return parent_expr()->decl_base();
}

// @brief 宣言要素への参照の場合，対象のオブジェクトを返す．
const VlDecl*
EiBitSelect::decl_obj() const
{
  return parent_expr()->decl_obj();
}

// @brief 宣言要素への参照の場合，対象のオブジェクトを返す．
const VlDeclArray*
EiBitSelect::declarray_obj() const
{
  return parent_expr()->declarray_obj();
}

// @brief 配列型宣言要素への参照の場合，配列の次元を返す．
SizeType
EiBitSelect::declarray_dimension() const
{
  return parent_expr()->declarray_dimension();
}

// @brief 配列型宣言要素への参照の場合，配列のインデックスを返す．
const VlExpr*
EiBitSelect::declarray_index(
  SizeType pos
) const
{
  return parent_expr()->declarray_index(pos);
}

// @brief 親の式を返す．
const VlExpr*
EiBitSelect::parent_expr() const
{
  return mBaseExpr;
}

// @brief 左辺式の要素数の取得
SizeType
EiBitSelect::lhs_elem_num() const
{
  return 1;
}

// @brief 左辺式の要素の取得
const VlExpr*
EiBitSelect::lhs_elem(
  SizeType pos
) const
{
  ASSERT_COND( pos == 0 );

  return this;
}

// @brief 左辺式の要素のリストの取得
vector<const VlExpr*>
EiBitSelect::lhs_elem_list() const
{
  return {this};
}

// @brief 要求される式の型を計算してセットする．
void
EiBitSelect::_set_reqsize(
  const VlValueType& type
)
{
  // なにもしない．
}


//////////////////////////////////////////////////////////////////////
// クラス EiConstBitSelect
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiConstBitSelect::EiConstBitSelect(
  const PtExpr* pt_expr,
  ElbExpr* base_expr,
  const PtExpr* index_expr,
  int index_val
) : EiBitSelect{pt_expr, base_expr},
    mIndexExpr{index_expr},
    mIndexVal{index_val}
{
}

// @brief デストラクタ
EiConstBitSelect::~EiConstBitSelect()
{
}

// @brief 固定選択子の時 true を返す．
bool
EiConstBitSelect::is_constant_select() const
{
  return true;
}

// @brief インデックス式を返す．
const VlExpr*
EiConstBitSelect::index() const
{
  return nullptr;
}

// @brief インデックス値を返す．
int
EiConstBitSelect::index_val() const
{
  return mIndexVal;
}


//////////////////////////////////////////////////////////////////////
// クラス EiVarBitSelect
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiVarBitSelect::EiVarBitSelect(
  const PtExpr* pt_expr,
  ElbExpr* base_expr,
  ElbExpr* index_expr
) : EiBitSelect{pt_expr, base_expr},
    mIndexExpr{index_expr}
{
}

// @brief デストラクタ
EiVarBitSelect::~EiVarBitSelect()
{
}

// @brief 固定選択子の時 true を返す．
bool
EiVarBitSelect::is_constant_select() const
{
  return mIndexExpr->is_const();
}

// @brief インデックス式を返す．
const VlExpr*
EiVarBitSelect::index() const
{
  return mIndexExpr;
}

// @brief インデックス値を返す．
int
EiVarBitSelect::index_val() const
{
  return 0;
}

END_NAMESPACE_YM_VERILOG
