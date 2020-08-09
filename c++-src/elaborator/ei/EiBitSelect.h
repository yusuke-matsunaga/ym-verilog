#ifndef EIBITSELECT_H
#define EIBITSELECT_H

/// @file EiBitSelect.h
/// @brief EiBitSelect のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
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
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] base_expr 対象の式
  /// @param[in] index_expr ビット選択式
  EiBitSelect(const PtExpr* pt_expr,
	      ElbExpr* base_expr);

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
  /// @note 参照している要素の型によって決まる．
  bool
  is_const() const override;

  /// @brief ビット指定の時に true を返す．
  bool
  is_bitselect() const override;

  /// @brief 宣言要素もしくは配列型宣言要素への参照を返す．
  /// @note それ以外では nullptr を返す．
  const VlDeclBase*
  decl_base() const override;

  /// @brief 宣言要素への参照の場合，対象のオブジェクトを返す．
  /// @note 宣言要素に対するビット選択，部分選択の場合にも意味を持つ．
  const VlDecl*
  decl_obj() const override;

  /// @brief 宣言要素への参照の場合，対象のオブジェクトを返す．
  /// @note 宣言要素に対するビット選択，部分選択の場合にも意味を持つ．
  const VlDeclArray*
  declarray_obj() const override;

  /// @brief 配列型宣言要素への参照の場合，配列の次元を返す．
  /// @note それ以外では 0 を返す．
  SizeType
  declarray_dimension() const override;

  /// @brief 配列型宣言要素への参照の場合，配列のインデックスを返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < declarray_dimension() )
  /// @note それ以外では nullptr を返す．
  const VlExpr*
  declarray_index(SizeType pos) const override;

  /// @brief 親の式を返す．
  const VlExpr*
  parent_expr() const override;

  /// @brief 左辺式の要素数の取得
  /// @note 通常は1だが，連結演算子の場合はその子供の数となる．
  /// @note ただし，連結演算の入れ子はすべて平坦化して考える．
  /// @note このクラスでは 1 を返す．
  SizeType
  lhs_elem_num() const override;

  /// @brief 左辺式の要素の取得
  /// @param[in] pos 位置 ( 0 <= pos < lhs_elem_num() )
  /// @note 連結演算子の見かけと異なり LSB 側が0番めの要素となる．
  /// @note このクラスでは pos = 0 の時，自分自身を返す．
  const VlExpr*
  lhs_elem(SizeType pos) const override;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要求される式の型を計算してセットする．
  /// @param[in] type 要求される式の型
  /// @note 必要であればオペランドに対して再帰的に処理を行なう．
  void
  _set_reqsize(const VlValueType& type) override;


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
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] base_expr 対象の式
  /// @param[in] index_expr ビット選択式
  /// @param[in] index_val ビット選択式の値
  EiConstBitSelect(const PtExpr* pt_expr,
		   ElbExpr* base_expr,
		   const PtExpr* index_expr,
		   int index_val);

  /// @brief デストラクタ
  ~EiConstBitSelect();


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 固定選択子の時 true を返す．
  /// @note ビット選択，部分選択の時，意味を持つ．
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
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] base_expr 対象の式
  /// @param[in] index_expr ビット選択式
  EiVarBitSelect(const PtExpr* pt_expr,
		 ElbExpr* base_expr,
		 ElbExpr* index_expr);

  /// @brief デストラクタ
  ~EiVarBitSelect();


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 固定選択子の時 true を返す．
  /// @note ビット選択，部分選択の時，意味を持つ．
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
