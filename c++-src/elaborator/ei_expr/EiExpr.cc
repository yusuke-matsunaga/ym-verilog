
/// @file EiExpr.cc
/// @brief EiExpr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.

#include "ei/EiExpr.h"
#include "ei/EiOperation.h"

#include "ym/BitVector.h"
#include "ym/VlTime.h"
#include "ym/VlValue.h"
#include "ym/pt/PtExpr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス EiExpr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiExpr::EiExpr()
{
}

// @brief デストラクタ
EiExpr::~EiExpr()
{
}

// @brief ファイル位置を返す．
FileRegion
EiExpr::file_region() const
{
  return pt_obj()->file_region();
}

// @brief 定数の時 true を返す．
bool
EiExpr::is_const() const
{
  return false;
}

// @brief 固定選択子の時 true を返す．
bool
EiExpr::is_constant_select() const
{
  return false;
}

// @brief プライマリ(net/reg/variables/parameter)の時に true を返す．
bool
EiExpr::is_primary() const
{
  return false;
}

// @brief ビット指定の時に true を返す．
bool
EiExpr::is_bitselect() const
{
  return false;
}

// @brief 範囲指定の時に true を返す．
bool
EiExpr::is_partselect() const
{
  return false;
}

// @brief 演算子の時に true を返す．
bool
EiExpr::is_operation() const
{
  return false;
}

// @brief 関数呼び出しの時に true を返す．
bool
EiExpr::is_funccall() const
{
  return false;
}

// @brief システム関数よびあどい時に true を返す．
bool
EiExpr::is_sysfunccall() const
{
  return false;
}

// @brief 宣言要素もしくは配列型宣言要素への参照を返す．
const VlDeclBase*
EiExpr::decl_base() const
{
  return nullptr;
}

// @brief 対象のオブジェクトを返す．
const VlDecl*
EiExpr::decl_obj() const
{
  return nullptr;
}

// @brief 宣言要素の配列への参照の場合，対象のオブジェクトを返す．
const VlDeclArray*
EiExpr::declarray_obj() const
{
  return nullptr;
}

// @brief 配列型宣言要素への参照の場合，配列の次元を返す．
SizeType
EiExpr::declarray_dimension() const
{
  return 0;
}

// @brief 配列型宣言要素への参照の場合，配列のインデックスを返す．
const VlExpr*
EiExpr::declarray_index(
  SizeType pos
) const
{
  return nullptr;
}

// @brief 配列型宣言要素への参照のオフセットを返す．
SizeType
EiExpr::declarray_offset() const
{
  return 0;
}

// @brief 対象のオブジェクトを返す．
const VlScope*
EiExpr::scope_obj() const
{
  return nullptr;
}

// @brief 対象のオブジェクトを返す．
const VlPrimitive*
EiExpr::primitive_obj() const
{
  return nullptr;
}

// @brief インデックス式を返す．
const VlExpr*
EiExpr::index() const
{
  return nullptr;
}

// @brief インデックス値を返す．
int
EiExpr::index_val() const
{
  return 0;
}

// @brief 範囲指定のモードを返す．
VpiRangeMode
EiExpr::range_mode() const
{
  return VpiRangeMode::No;
}

// @brief 範囲の MSB を返す．
const VlExpr*
EiExpr::left_range() const
{
  return nullptr;
}

// @brief 範囲の MSB の値を返す．
int
EiExpr::left_range_val() const
{
  return 0;
}

// @brief 範囲の LSB を返す．
const VlExpr*
EiExpr::right_range() const
{
  return nullptr;
}

// @brief 範囲の LSB の値を返す．
int
EiExpr::right_range_val() const
{
  return 0;
}

// @brief 範囲のベースを表す式を返す．
const VlExpr*
EiExpr::base() const
{
  return nullptr;
}

// @brief 範囲のビット幅を返す．
SizeType
EiExpr::range_width() const
{
  return 0;
}

// @brief 親の式を返す．
const VlExpr*
EiExpr::parent_expr() const
{
  return nullptr;
}

// @brief 演算子のタイプを返す．
VpiOpType
EiExpr::op_type() const
{
  return VpiOpType::Null;
}

// @brief オペランド数を返す．
SizeType
EiExpr::operand_num() const
{
  return 0;
}

// @brief オペランドを返す．
const VlExpr*
EiExpr::operand(
  SizeType pos
) const
{
  return nullptr;
}

// @brief オペランドのリストを返す．
vector<const VlExpr*>
EiExpr::operand_list() const
{
  return {};
}

// @brief 繰り返し数を返す．
SizeType
EiExpr::rep_num() const
{
  return 0;
}

// @brief 定数の型を返す．
VpiConstType
EiExpr::constant_type() const
{
  // とは言っても何か返さなければならない．
  return VpiConstType::Dec;
}

// @brief 定数値を返す．
VlValue
EiExpr::constant_value() const
{
  return VlValue();
}

// @brief 対象の関数を返す．
const VlTaskFunc*
EiExpr::function() const
{
  return nullptr;
}

// @brief 対象のシステム関数を返す．
const VlUserSystf*
EiExpr::user_systf() const
{
  return nullptr;
}

// @brief 引数の数を返す．
SizeType
EiExpr::argument_num() const
{
  return 0;
}

// @brief 引数を返す．
const VlExpr*
EiExpr::argument(
  SizeType pos
) const
{
  return nullptr;
}

// @brief 引数のリストを返す．
vector<const VlExpr*>
EiExpr::argument_list() const
{
  return {};
}

// @brief 左辺式の要素数の取得
SizeType
EiExpr::lhs_elem_num() const
{
  return 0;
}

// @brief 左辺式の要素の取得
const VlExpr*
EiExpr::lhs_elem(
  SizeType pos
) const
{
  return nullptr;
}

// @brief 左辺式の要素のリストの取得
vector<const VlExpr*>
EiExpr::lhs_elem_list() const
{
  return {};
}


//////////////////////////////////////////////////////////////////////
// クラス EiExprBase
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiExprBase::EiExprBase(
  const PtExpr* pt_expr
) : mPtExpr{pt_expr}
{
}

// @brief デストラクタ
EiExprBase::~EiExprBase()
{
}

// @brief Verilog-HDL の文字列を得る．
string
EiExprBase::decompile() const
{
  return pt_expr()->decompile();
}

/// @brief パース木の定義要素を返す．
const PtBase*
EiExprBase::pt_obj() const
{
  return pt_expr();
}


//////////////////////////////////////////////////////////////////////
// クラス EiOperation
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiOperation::EiOperation(
  const PtExpr* pt_expr
) : EiExprBase{pt_expr}
{
}

// @brief デストラクタ
EiOperation::~EiOperation()
{
}

// @brief 型の取得
VpiObjType
EiOperation::type() const
{
  return VpiObjType::Operation;
}

// @brief 演算子の時に true を返す．
bool
EiOperation::is_operation() const
{
  return true;
}

// @brief 演算子のタイプを返す．
VpiOpType
EiOperation::op_type() const
{
  return pt_expr()->op_type();
}

END_NAMESPACE_YM_VERILOG
