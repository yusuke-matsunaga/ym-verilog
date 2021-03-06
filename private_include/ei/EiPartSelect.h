﻿#ifndef EIPARTSELECT_H
#define EIPARTSELECT_H

/// @file EiPartSelect.h
/// @brief EiPartSelect のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.


#include "ei/EiExpr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiPartSelectPart EiPartSelect.h "EiPartSelect.h"
/// @brief パート選択式を表す基底クラス
//////////////////////////////////////////////////////////////////////
class EiPartSelect :
  public EiExprBase
{
protected:

  /// @brief コンストラクタ
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] parent_expr 対象の式
  EiPartSelect(const PtExpr* pt_expr,
	       ElbExpr* parent_expr);

  /// @brief デストラクタ
  ~EiPartSelect();


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

  /// @brief 定数の時 true を返す．
  /// @note 参照している要素の型によって決まる．
  bool
  is_const() const override;

  /// @brief 範囲指定の時に true を返す．
  bool
  is_partselect() const override;

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
  ElbExpr* mParentExpr;

};


//////////////////////////////////////////////////////////////////////
/// @class EiConstPartSelectPart EiPartSelect.h "EiPartSelect.h"
/// @brief 固定パート選択式を表すクラス
//////////////////////////////////////////////////////////////////////
class EiConstPartSelect :
  public EiPartSelect
{
public:

  /// @brief コンストラクタ
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] parent_expr 対象の式
  /// @param[in] index1, index2 パート選択式
  /// @param[in] index1_val, index2_val パート選択式の値
  EiConstPartSelect(const PtExpr* pt_expr,
		    ElbExpr* parent_expr,
		    const PtExpr* index1,
		    const PtExpr* index2,
		    int index1_val,
		    int index2_val);

  /// @brief デストラクタ
  ~EiConstPartSelect();


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 式の値のタイプを返す．
  VlValueType
  value_type() const override;

  /// @brief 固定選択子の時 true を返す．
  /// @note ビット選択，部分選択の時，意味を持つ．
  bool
  is_constant_select() const override;

  /// @brief 範囲指定のモードを返す．
  VpiRangeMode
  range_mode() const override;

  /// @brief 範囲の MSB の式を返す．
  /// @note 通常の範囲選択の時，意味を持つ．
  const VlExpr*
  left_range() const override;

  /// @brief 範囲の LSB の式を返す．
  /// @note 通常の範囲選択の時，意味を持つ．
  const VlExpr*
  right_range() const override;

  /// @brief 範囲の MSB の値を返す．
  /// @note 式に対する範囲選択の時，意味を持つ．
  int
  left_range_val() const override;

  /// @brief 範囲の LSB の値を返す．
  /// @note 式に対する範囲選択の時，意味を持つ．
  int
  right_range_val() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 範囲選択の MSB
  const PtExpr* mLeftRange;

  // 範囲選択の LSB
  const PtExpr* mRightRange;

  // 範囲選択の MSB の値
  int mLeftVal;

  // 範囲選択の LSB の値
  int mRightVal;

};


//////////////////////////////////////////////////////////////////////
/// @class EiVarPartSelect EiVarPartSelect.h "EiVarPartSelect.h"
/// @brief 可変パート選択式を表すクラス
//////////////////////////////////////////////////////////////////////
class EiVarPartSelect :
  public EiPartSelect
{
protected:

  /// @brief コンストラクタ
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] parent_expr 対象の式
  /// @param[in] base 範囲のベースアドレスを表す式
  /// @param[in] range 範囲を表す式
  /// @param[in] range_width 範囲の値
  EiVarPartSelect(const PtExpr* pt_expr,
		  ElbExpr* parent_expr,
		  ElbExpr* base,
		  const PtExpr* range,
		  SizeType range_width);

  /// @brief デストラクタ
  ~EiVarPartSelect();


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 式の値のタイプを返す．
  VlValueType
  value_type() const override;

  /// @brief 固定選択子の時 true を返す．
  /// @note ビット選択，部分選択の時，意味を持つ．
  bool
  is_constant_select() const override;

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


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 範囲のベースの式
  ElbExpr* mBaseExpr;

  // 範囲
  const PtExpr* mRangeExpr;

  // 範囲の値
  SizeType mRangeWidth;

};


//////////////////////////////////////////////////////////////////////
/// @class EiPlusPartSelect EiVarPartSelect.h "EiVarPartSelect.h"
/// @brief 可変パート選択式([ +: ])を表すクラス
//////////////////////////////////////////////////////////////////////
class EiPlusPartSelect :
  public EiVarPartSelect
{
public:

  /// @brief コンストラクタ
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] parent_expr 対象の式
  /// @param[in] obj 本体の式
  /// @param[in] base 範囲のベースアドレスを表す式
  /// @param[in] range 範囲を表す式
  /// @param[in] range_val 範囲の値
  EiPlusPartSelect(const PtExpr* pt_expr,
		   ElbExpr* parent_expr,
		   ElbExpr* base,
		   const PtExpr* range,
		   int range_val);

  /// @brief デストラクタ
  ~EiPlusPartSelect();


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 範囲指定のモードを返す．
  VpiRangeMode
  range_mode() const override;

};


//////////////////////////////////////////////////////////////////////
/// @class EiMinusPartSelect EiVarPartSelect.h "EiVarPartSelect.h"
/// @brief 可変パート選択式([ -: ])を表すクラス
//////////////////////////////////////////////////////////////////////
class EiMinusPartSelect :
  public EiVarPartSelect
{
public:

  /// @brief コンストラクタ
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] parent_expr 対象の式
  /// @param[in] base 範囲のベースアドレスを表す式
  /// @param[in] range 範囲を表す式
  /// @param[in] range_val 範囲の値
  EiMinusPartSelect(const PtExpr* pt_expr,
		    ElbExpr* parent_expr,
		    ElbExpr* base,
		    const PtExpr* range,
		    int range_val);

  /// @brief デストラクタ
  ~EiMinusPartSelect();


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 範囲指定のモードを返す．
  VpiRangeMode
  range_mode() const override;

};

END_NAMESPACE_YM_VERILOG

#endif // EIPARTSELECT_H
