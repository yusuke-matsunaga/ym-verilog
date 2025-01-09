#ifndef EIEXPR_H
#define EIEXPR_H

/// @file EiExpr.h
/// @brief EiExpr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2020 Yusuke Matsunaga
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
  bool
  is_const() const override;

  /// @brief 固定選択子の時 true を返す．
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
  const VlDeclBase*
  decl_base() const override;

  /// @brief 宣言要素への参照の場合，対象のオブジェクトを返す．
  const VlDecl*
  decl_obj() const override;

  /// @brief 宣言要素の配列への参照の場合，対象のオブジェクトを返す．
  const VlDeclArray*
  declarray_obj() const override;

  /// @brief 配列型宣言要素への参照の場合，配列の次元を返す．
  SizeType
  declarray_dimension() const override;

  /// @brief 配列型宣言要素への参照の場合，配列のインデックスを返す．
  const VlExpr*
  declarray_index(
    SizeType pos ///< [in] 位置番号 ( 0 <= pos < declarray_dimension() )
  ) const override;

  /// @brief 配列型宣言要素への参照のオフセットを返す．
  SizeType
  declarray_offset() const override;

  /// @brief スコープへの参照の場合，対象のオブジェクトを返す．
  const VlScope*
  scope_obj() const override;

  /// @brief primitive への参照の場合，対象のオブジェクトを返す．
  const VlPrimitive*
  primitive_obj() const override;

  /// @brief 親の式を返す．
  const VlExpr*
  parent_expr() const override;

  /// @brief インデックス式を返す．
  const VlExpr*
  index() const override;

  /// @brief インデックス値を返す．
  int
  index_val() const override;

  /// @brief 範囲指定のモードを返す．
  VpiRangeMode
  range_mode() const override;

  /// @brief 範囲の MSB を返す．
  const VlExpr*
  left_range() const override;

  /// @brief 範囲の MSB の値を返す．
  int
  left_range_val() const override;

  /// @brief 範囲の LSB を返す．
  const VlExpr*
  right_range() const override;

  /// @brief 範囲の LSB の値を返す．
  int
  right_range_val() const override;

  /// @brief 範囲のベースを表す式を返す．
  const VlExpr*
  base() const override;

  /// @brief 範囲のビット幅を返す．
  SizeType
  range_width() const override;

  /// @brief 演算子のタイプを返す．
  VpiOpType
  op_type() const override;

  /// @brief オペランド数を返す．
  SizeType
  operand_num() const override;

  /// @brief オペランドを返す．
  const VlExpr*
  operand(
    SizeType pos ///< [in] 位置番号
  ) const override;

  /// @brief オペランドのリストを返す．
  vector<const VlExpr*>
  operand_list() const override;

  /// @brief 繰り返し数を返す．
  SizeType
  rep_num() const override;

  /// @brief 定数の型を返す．
  VpiConstType
  constant_type() const override;

  /// @brief 定数値を返す．
  VlValue
  constant_value() const override;

  /// @brief 対象の関数を返す．
  const VlTaskFunc*
  function() const override;

  /// @brief 対象のシステム関数を返す．
  const VlUserSystf*
  user_systf() const override;

  /// @brief 引数の数を返す．
  SizeType
  argument_num() const override;

  /// @brief 引数を返す．
  const VlExpr*
  argument(
    SizeType pos ///< [in] 位置番号 ( 0 <= pos < argument_num() )
  ) const override;

  /// @brief 引数のリストを返す．
  vector<const VlExpr*>
  argument_list() const override;

  /// @brief 左辺式の要素数の取得
  SizeType
  lhs_elem_num() const override;

  /// @brief 左辺式の要素の取得
  const VlExpr*
  lhs_elem(
    SizeType pos ///< [in] 位置 ( 0 <= pos < lhs_elem_num() )
  ) const override;

  /// @brief 左辺式の要素のリストの取得
  vector<const VlExpr*>
  lhs_elem_list() const override;


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
  EiExprBase(
    const PtExpr* pt_expr ///< [in] パース木の定義要素
  );

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
