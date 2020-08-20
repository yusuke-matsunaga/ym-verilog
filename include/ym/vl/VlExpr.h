﻿#ifndef YM_VL_VEXPRR_H
#define YM_VL_VEXPRR_H

/// @file ym/vl/VlExpr.h
/// @brief VlExpr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym/vl/VlObj.h"
#include "ym/vl/VlFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class VlExpr VlExpr.h "ym/vl/VlExpr.h"
/// @brief エラボレーション中の expression を表す基底クラス
//////////////////////////////////////////////////////////////////////
class VlExpr :
  public VlObj
{
public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の派生クラスに共通な仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 式の値のタイプを返す．
  /// @note このタイプで評価する．
  virtual
  VlValueType
  value_type() const = 0;

  /// @brief 要求された値のタイプを返す．
  /// @note こちらは上位の式あるいは左辺の式から決まるタイプ
  /// @note 評価後にこのタイプに変換する．
  virtual
  VlValueType
  req_type() const = 0;

  /// @brief Verilog-HDL の文字列を得る．
  virtual
  string
  decompile() const = 0;

  /// @brief 式のビット幅を返す．
  /// @note value_type().size() を同じ
  virtual
  SizeType
  bit_size() const = 0;

  /// @brief 定数式の時 true を返す．
  virtual
  bool
  is_const() const = 0;

  /// @brief 部分/ビット指定が定数の時 true を返す．
  /// @note kVpiPartSelect/kVpiBitSelect の時，意味を持つ．
  /// @note それ以外では常に false を返す．
  virtual
  bool
  is_constant_select() const = 0;

  /// @brief プライマリ(net/reg/variables/parameter)の時に true を返す．
  virtual
  bool
  is_primary() const = 0;

  /// @brief ビット指定の時に true を返す．
  virtual
  bool
  is_bitselect() const = 0;

  /// @brief 範囲指定の時に true を返す．
  virtual
  bool
  is_partselect() const = 0;

  /// @brief 演算子の時に true を返す．
  virtual
  bool
  is_operation() const = 0;

  /// @brief 関数呼び出しの時に true を返す．
  virtual
  bool
  is_funccall() const = 0;

  /// @brief システム関数呼び出しの時に true を返す．
  virtual
  bool
  is_sysfunccall() const = 0;

  /// @brief 宣言要素もしくは配列型宣言要素への参照を返す．
  /// @note それ以外では nullptr を返す．
  virtual
  const VlDeclBase*
  decl_base() const = 0;

  /// @brief 宣言要素への参照の場合，対象のオブジェクトを返す．
  /// @note それ以外では nullptr を返す．
  virtual
  const VlDecl*
  decl_obj() const = 0;

  /// @brief 宣言要素の配列への参照の場合，対象のオブジェクトを返す．
  /// @note それ以外では nullptr を返す．
  virtual
  const VlDeclArray*
  declarray_obj() const = 0;

  /// @brief 配列型宣言要素への参照の場合，配列の次元を返す．
  /// @note それ以外では 0 を返す．
  virtual
  SizeType
  declarray_dimension() const = 0;

  /// @brief 配列型宣言要素への参照の場合，配列のインデックスを返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < declarray_dimension() )
  /// @note それ以外では nullptr を返す．
  virtual
  const VlExpr*
  declarray_index(SizeType pos) const = 0;

  /// @brief 配列型宣言要素への参照のオフセットを返す．
  /// @note 固定インデックスの場合のみ意味を持つ．
  virtual
  SizeType
  declarray_offset() const = 0;

  /// @brief スコープへの参照の場合，対象のオブジェクトを返す．
  /// @note それ以外では nullptr を返す．
  /// @note スコープへの参照が式で用いられるのはシステム関数/タスクの引数だけ
  virtual
  const VlScope*
  scope_obj() const = 0;

  /// @brief primitive への参照の場合，対象のオブジェクトを返す．
  /// @note primitiveへの参照が式で用いられるのはシステム関数/タスクの引数だけ
  virtual
  const VlPrimitive*
  primitive_obj() const = 0;

  /// @brief 親の式を返す．
  /// @note 式に対するビット選択/範囲選択の時，意味を持つ．
  /// @note それ以外では nullptr を返す．
  virtual
  const VlExpr*
  parent_expr() const = 0;

  /// @brief インデックス式を返す．
  /// @note ビット選択の時，意味を持つ．
  /// @note それ以外では nullptr を返す．
  virtual
  const VlExpr*
  index() const = 0;

  /// @brief インデックス値を返す．
  /// @note 固定ビット選択の時，意味を持つ．
  /// @note それ以外では値は不定
  virtual
  int
  index_val() const = 0;

  /// @brief 範囲指定のモードを返す．
  /// @retval kVpiNoRange 範囲指定なし
  /// @retval kVpiConstRange 固定範囲
  /// @retval kVpiPlusRange +: の可動範囲
  /// @retval kVpiMinusRange -: の可動範囲
  /// @note is_partselect() == true の時のみ意味を持つ．
  virtual
  VpiRangeMode
  range_mode() const = 0;

  /// @brief 範囲の MSB の式を返す．
  /// @note 範囲選択の時，意味を持つ．
  /// @note それ以外では nullptr を返す．
  virtual
  const VlExpr*
  left_range() const = 0;

  /// @brief 範囲の MSB の値を返す．
  /// @note それ以外では値は不定
  virtual
  int
  left_range_val() const = 0;

  /// @brief 範囲の LSB の式を返す．
  /// @note 範囲選択の時，意味を持つ．
  /// @note それ以外では nullptr を返す．
  virtual
  const VlExpr*
  right_range() const = 0;

  /// @brief 範囲の LSB の値を返す．
  /// @note それ以外では値は不定
  virtual
  int
  right_range_val() const = 0;

  /// @brief 範囲のベースを表す式を返す．
  /// @note 可変範囲選択の時，意味を持つ．
  /// @note それ以外では nullptr を返す．
  virtual
  const VlExpr*
  base() const = 0;

  /// @brief 範囲のビット幅を返す．
  /// @note 可変範囲選択の時，意味を持つ．
  /// @note それ以外では値は不定
  virtual
  SizeType
  range_width() const = 0;

  /// @brief 演算子の型を返す．
  /// @note kVpiOperation の時，意味を持つ．
  /// @note それ以外では動作は不定
  virtual
  VpiOpType
  op_type() const = 0;

  /// @brief オペランド数を返す．
  /// @note kVpiOperation の時，意味を持つ．
  /// @note それ以外では 0 を返す．
  virtual
  SizeType
  operand_num() const = 0;

  /// @brief オペランドを返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < operand_num() )
  /// @note kVpiOperation の時，意味を持つ．
  /// @note それ以外では nullptr を返す．
  virtual
  const VlExpr*
  operand(SizeType pos) const = 0;

  /// @brief 繰り返し数を返す．
  /// @note multiple concatenation の時のみ意味を持つ．
  /// @note multiple concatenation の時は operand(0) と等しい．
  virtual
  SizeType
  rep_num() const = 0;

  /// @brief 定数型を返す．
  ///
  /// is_const() = true の時，意味を持つ．
  /// それ以外では動作は不定
  virtual
  VpiConstType
  constant_type() const = 0;

  /// @brief 定数値を返す．
  ///
  /// is_const() = true の時，意味を持つ．
  /// それ以外では動作は不定
  virtual
  VlValue
  constant_value() const = 0;

  /// @brief 対象の関数を返す．
  /// @note kVpiFuncCall の時，意味を持つ．
  /// @note それ以外では nullptr を返す．
  virtual
  const VlTaskFunc*
  function() const = 0;

  /// @brief 対象のシステム関数を返す．
  /// @note kVpiSysFuncCall の時，意味を持つ．
  /// @note それ以外では nullptr を返す．
  virtual
  const VlUserSystf*
  user_systf() const = 0;

  /// @brief 引数の数を返す．
  /// @note kVpiFuncCall/kVpiSysFuncCall の時，意味を持つ．
  virtual
  SizeType
  argument_num() const = 0;

  /// @brief 引数を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < argument_num() )
  /// @note kVpiFuncCall/kVpiSysFuncCall の時，意味を持つ．
  /// @note それ以外では nullptr を返す．
  virtual
  const VlExpr*
  argument(SizeType pos) const = 0;

  /// @brief 左辺式の要素数の取得
  /// @note 通常は1だが，連結演算子の場合はその子供の数となる．
  /// @note ただし，連結演算の入れ子はすべて平坦化して考える．
  virtual
  SizeType
  lhs_elem_num() const = 0;

  /// @brief 左辺式の要素の取得
  /// @param[in] pos 位置 ( 0 <= pos < lhs_elem_num() )
  /// @note 連結演算子の見かけと異なり LSB 側が0番めの要素となる．
  virtual
  const VlExpr*
  lhs_elem(SizeType pos) const = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // YM_VL_VEXPRR_H
