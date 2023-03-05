#ifndef EIPARTSELECT_H
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
  EiPartSelect(
    const PtExpr* pt_expr, ///< [in] パース木の定義要素
    ElbExpr* parent_expr   ///< [in] 対象の式
  );

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
  bool
  is_const() const override;

  /// @brief 範囲指定の時に true を返す．
  bool
  is_partselect() const override;

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
  EiConstPartSelect(
    const PtExpr* pt_expr, ///< [in] パース木の定義要素
    ElbExpr* parent_expr,  ///< [in] 対象の式
    const PtExpr* index1,  ///< [in] パート選択式1
    const PtExpr* index2,  ///< [in] パート選択式2
    int index1_val,        ///< [in] パート選択式1の値
    int index2_val         ///< [in] パート選択式2の値
  );

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
  bool
  is_constant_select() const override;

  /// @brief 範囲指定のモードを返す．
  VpiRangeMode
  range_mode() const override;

  /// @brief 範囲の MSB の式を返す．
  const VlExpr*
  left_range() const override;

  /// @brief 範囲の LSB の式を返す．
  const VlExpr*
  right_range() const override;

  /// @brief 範囲の MSB の値を返す．
  int
  left_range_val() const override;

  /// @brief 範囲の LSB の値を返す．
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
  EiVarPartSelect(
    const PtExpr* pt_expr, ///< [in] パース木の定義要素
    ElbExpr* parent_expr,  ///< [in] 対象の式
    ElbExpr* base,         ///< [in] 範囲のベースアドレスを表す式
    const PtExpr* range,   ///< [in] 範囲を表す式
    SizeType range_width   ///< [in] 範囲の値
  );

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
  bool
  is_constant_select() const override;

  /// @brief 範囲のベースを表す式を返す．
  const VlExpr*
  base() const override;

  /// @brief 範囲のビット幅を返す．
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
  EiPlusPartSelect(
    const PtExpr* pt_expr, ///< [in] パース木の定義要素
    ElbExpr* parent_expr,  ///< [in] 対象の式
    ElbExpr* base,	   ///< [in] 範囲のベースアドレスを表す式
    const PtExpr* range,   ///< [in] 範囲を表す式
    SizeType range_val	   ///< [in] 範囲の値
  );

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
  EiMinusPartSelect(
    const PtExpr* pt_expr, ///< [in] パース木の定義要素
    ElbExpr* parent_expr,  ///< [in] 対象の式
    ElbExpr* base,	   ///< [in] 範囲のベースアドレスを表す式
    const PtExpr* range,   ///< [in] 範囲を表す式
    SizeType range_val	   ///< [in] 範囲の値
  );

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
