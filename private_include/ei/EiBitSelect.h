#ifndef EIBITSELECT_H
#define EIBITSELECT_H

/// @file EiBitSelect.h
/// @brief EiBitSelect のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.

#include "EiExpr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiBitSelect EiBitSelect.h "EiBitSelect.h"
/// @brief ビット選択式を表す基底クラス
//////////////////////////////////////////////////////////////////////
class EiBitSelect :
  public EiExprBase
{
protected:

  /// @brief コンストラクタ
  EiBitSelect(
    const PtExpr* pt_expr, ///< [in] パース木の定義要素
    ElbExpr* base_expr     ///< [in] 対象の式
  );

  /// @brief デストラクタ
  ~EiBitSelect();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  VpiObjType
  type() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 式のタイプを返す．
  VlValueType
  value_type() const override;

  /// @brief 定数の時 true を返す．
  bool
  is_const() const override;

  /// @brief ビット指定の時に true を返す．
  bool
  is_bitselect() const override;

  /// @brief 宣言要素もしくは配列型宣言要素への参照を返す．
  const VlDeclBase*
  decl_base() const override;

  /// @brief 宣言要素への参照の場合，対象のオブジェクトを返す．
  const VlDecl*
  decl_obj() const override;

  /// @brief 宣言要素への参照の場合，対象のオブジェクトを返す．
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

  /// @brief 親の式を返す．
  const VlExpr*
  parent_expr() const override;

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


public:
  //////////////////////////////////////////////////////////////////////
  // ElbExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要求される式の型を計算してセットする．
  void
  _set_reqsize(
    const VlValueType& type ///< [in] 要求される式の型
  ) override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象の式
  ElbExpr* mBaseExpr;

};


//////////////////////////////////////////////////////////////////////
/// @class EiConstBitSelect EiBitSelect.h "EiBitSelect.h"
/// @brief 固定ビット選択式を表すクラス
//////////////////////////////////////////////////////////////////////
class EiConstBitSelect :
  public EiBitSelect
{
public:

  /// @brief コンストラクタ
  EiConstBitSelect(
    const PtExpr* pt_expr,    ///< [in] パース木の定義要素
    ElbExpr* base_expr,       ///< [in] 対象の式
    const PtExpr* index_expr, ///< [in] ビット選択式
    int index_val             ///< [in] ビット選択式の値
  );

  /// @brief デストラクタ
  ~EiConstBitSelect();


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 固定選択子の時 true を返す．
  bool
  is_constant_select() const override;

  /// @brief インデックス式を返す．
  const VlExpr*
  index() const override;

  /// @brief インデックス値を返す．
  int
  index_val() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // インデックス式
  const PtExpr* mIndexExpr;

  // インデックス値
  int mIndexVal;

};


//////////////////////////////////////////////////////////////////////
/// @class EiVarBitSelect EiBitSelect.h "EiBitSelect.h"
/// @brief 可変ビット選択式を表すクラス
//////////////////////////////////////////////////////////////////////
class EiVarBitSelect :
  public EiBitSelect
{
public:

  /// @brief コンストラクタ
  EiVarBitSelect(
    const PtExpr* pt_expr, ///< [in] パース木の定義要素
    ElbExpr* base_expr,    ///< [in] 対象の式
    ElbExpr* index_expr    ///< [in] ビット選択式
  );

  /// @brief デストラクタ
  ~EiVarBitSelect();


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 固定選択子の時 true を返す．
  bool
  is_constant_select() const override;

  /// @brief インデックス式を返す．
  const VlExpr*
  index() const override;

  /// @brief インデックス値を返す．
  int
  index_val() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // インデックス式
  ElbExpr* mIndexExpr;

};

END_NAMESPACE_YM_VERILOG

#endif // EIBITSELECT_H
