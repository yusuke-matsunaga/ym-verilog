#ifndef EIEXPR_H
#define EIEXPR_H

/// @file EiExpr.h
/// @brief EiExpr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "elaborator/ElbExpr.h"
#include "ym/pt/PtP.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiExpr EiExpr.h "EiExpr.h"
/// @brief ExprExpr の実装クラス
//////////////////////////////////////////////////////////////////////
class EiExpr :
  public ElbExpr
{
protected:

  /// @brief コンストラクタ
  EiExpr();

  /// @brief デストラクタ
  ~EiExpr();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  FileRegion
  file_region() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 定数の時 true を返す．
  /// @note このクラスは false を返す．
  bool
  is_const() const override;

  /// @brief 固定選択子の時 true を返す．
  /// @note ビット選択，部分選択の時，意味を持つ．
  /// @note このクラスでは false を返す．
  bool
  is_constant_select() const override;

  /// @brief プライマリ(net/reg/variables/parameter)の時に true を返す．
  bool
  is_primary() const override;

  /// @brief ビット指定の時に true を返す．
  bool
  is_bitselect() const override;

  /// @brief 範囲指定の時に true を返す．
  bool
  is_partselect() const override;

  /// @brief 演算子の時に true を返す．
  bool
  is_operation() const override;

  /// @brief 関数呼び出しの時に true を返す．
  bool
  is_funccall() const override;

  /// @brief システム関数よびあどい時に true を返す．
  bool
  is_sysfunccall() const override;

  /// @brief 宣言要素もしくは配列型宣言要素への参照を返す．
  /// @note それ以外では nullptr を返す．
  const VlDeclBase*
  decl_base() const override;

  /// @brief 宣言要素への参照の場合，対象のオブジェクトを返す．
  /// @note このクラスでは nullptr を返す．
  const VlDecl*
  decl_obj() const override;

  /// @brief 宣言要素の配列への参照の場合，対象のオブジェクトを返す．
  /// @note それ以外では nullptr を返す．
  const VlDeclArray*
  declarray_obj() const override;

  /// @brief 配列型宣言要素への参照の場合，配列の次元を返す．
  /// @note このクラスでは 0 を返す．
  SizeType
  declarray_dimension() const override;

  /// @brief 配列型宣言要素への参照の場合，配列のインデックスを返す．
 /// @param[in] pos 位置番号 ( 0 <= pos < declarray_dimension() )
  /// @note このクラスでは nullptr を返す．
  const VlExpr*
  declarray_index(SizeType pos) const override;

  /// @brief 配列型宣言要素への参照のオフセットを返す．
  /// @note 固定インデックスの場合のみ意味を持つ．
  SizeType
  declarray_offset() const override;

  /// @brief スコープへの参照の場合，対象のオブジェクトを返す．
  /// @note このクラスでは nullptr を返す．
  const VlNamedObj*
  scope_obj() const override;

  /// @brief primitive への参照の場合，対象のオブジェクトを返す．
  const VlPrimitive*
  primitive_obj() const override;

  /// @brief 親の式を返す．
  /// @note 式に対するビット選択/範囲選択の時，意味を持つ．
  /// @note このクラスでは nullptr を返す．
  const VlExpr*
  parent_expr() const override;

  /// @brief インデックス式を返す．
  /// @note ビット選択の時，意味を持つ．
  /// @note このクラスでは nullptr を返す．
  const VlExpr*
  index() const override;

  /// @brief インデックス値を返す．
  /// @note 式に対するビット選択の時，意味を持つ．
  /// @note このクラスでは 0 を返す．
  int
  index_val() const override;

  /// @brief 範囲指定のモードを返す．
  VpiRangeMode
  range_mode() const override;

  /// @brief 範囲の MSB を返す．
  /// @note 部分選択の時，意味を持つ．
  /// @note このクラスでは nullptr を返す．
  const VlExpr*
  left_range() const override;

  /// @brief 範囲の MSB の値を返す．
  /// @note 式に対する範囲選択の時，意味を持つ．
  /// @note このクラスでは 0 を返す．
  int
  left_range_val() const override;

  /// @brief 範囲の LSB を返す．
  /// @note 部分選択の時，意味を持つ．
  /// @note このクラスでは nullptr を返す．
  const VlExpr*
  right_range() const override;

  /// @brief 範囲の LSB の値を返す．
  /// @note 式に対する範囲選択の時，意味を持つ．
  /// @note このクラスでは 0 を返す．
  int
  right_range_val() const override;

  /// @brief 範囲のベースを表す式を返す．
  /// @note 可変範囲選択の時，意味を持つ．
  /// @note それ以外では nullptr を返す．
  const VlExpr*
  base() const override;

  /// @brief 範囲のビット幅を返す．
  /// @note 可変範囲選択の時，意味を持つ．
  /// @note それ以外では 0 を返す．
  SizeType
  range_width() const override;

  /// @brief 演算子のタイプを返す．
  /// @note 演算子の時，意味を持つ．
  /// @note このクラスでは kVlNullOp を返す．
  VpiOpType
  op_type() const override;

  /// @brief オペランド数を返す．
  /// @note 演算子の時，意味を持つ．
  /// @note このクラスでは 0 を返す．
  SizeType
  operand_num() const override;

  /// @brief オペランドを返す．
  /// @param[in] pos 位置番号
  /// @note 演算子の時，意味を持つ．
  /// @note このクラスでは nullptr を返す．
  const VlExpr*
  operand(SizeType pos) const override;

  /// @brief 繰り返し数を返す．
  /// @note multiple concatenation の時のみ意味を持つ．
  SizeType
  rep_num() const override;

  /// @brief 定数の型を返す．
  /// @note 定数の時，意味を持つ．
  /// @note このクラスでは動作は不定
  VpiConstType
  constant_type() const override;

  /// @brief 定数値を返す．
  /// @note kVpiConstant の時，意味を持つ．
  /// @note それ以外では動作は不定
  VlValue
  constant_value() const override;

  /// @brief 対象の関数を返す．
  /// @note kVpiFuncCall の時，意味を持つ．
  /// @note このクラスでは nullptr を返す．
  const VlTaskFunc*
  function() const override;

  /// @brief 対象のシステム関数を返す．
  /// @note kVpiSysFuncCall の時，意味を持つ．
  /// @note このクラスでは nullptr を返す．
  const VlUserSystf*
  user_systf() const override;

  /// @brief 引数の数を返す．
  /// @note kVpiFuncCall/kVpiSysFuncCall の時，意味を持つ．
  /// @note このクラスでは 0 を返す．
  SizeType
  argument_num() const override;

  /// @brief 引数を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < argument_num() )
  /// @note kVpiFuncCall/kVpiSysFuncCall の時，意味を持つ．
  /// @note このクラスでは nullptr を返す．
  const VlExpr*
  argument(SizeType pos) const override;

  /// @brief 左辺式の要素数の取得
  /// @note 通常は1だが，連結演算子の場合はその子供の数となる．
  /// @note ただし，連結演算の入れ子はすべて平坦化して考える．
  /// @note このクラスでは 0 を返す．
  SizeType
  lhs_elem_num() const override;

  /// @brief 左辺式の要素の取得
  /// @param[in] pos 位置 ( 0 <= pos < lhs_elem_num() )
  /// @note 連結演算子の見かけと異なり LSB 側が0番めの要素となる．
  /// @note このクラスでは nullptr を返す．
  const VlExpr*
  lhs_elem(SizeType pos) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // 派生クラスが実装しなければならない仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief パース木の定義要素を返す．
  virtual
  const PtBase*
  pt_obj() const = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class EiExprBase EiExpr.h "EiExpr.h"
/// @brief PtExpr を持つ EiExpr の派生クラス
//////////////////////////////////////////////////////////////////////
class EiExprBase :
  public EiExpr
{
protected:

  /// @brief コンストラクタ
  /// @param[in] pt_expr パース木の定義要素
  EiExprBase(const PtExpr* pt_expr);

  /// @brief デストラクタ
  ~EiExprBase();


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief Verilog-HDL の文字列を得る．
  string
  decompile() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // EiExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief パース木の定義要素を返す．
  const PtBase*
  pt_obj() const override;


protected:
  //////////////////////////////////////////////////////////////////////
  // 派生クラスから使われる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief PtExpr を得る．
  const PtExpr*
  pt_expr() const { return mPtExpr; }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // パース木の定義要素
  const PtExpr* mPtExpr;

};

END_NAMESPACE_YM_VERILOG

#endif // EIEXPR_H
