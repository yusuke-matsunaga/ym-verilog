#ifndef EICONCATOP_H
#define EICONCATOP_H

/// @file EiConcatOp.h
/// @brief 結合演算子の派生クラスのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.

#include "EiOperation.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiConcatOp EiConcatOp.h "EiConcatOp.h"
/// @brief 結合演算子
//////////////////////////////////////////////////////////////////////
class EiConcatOp :
  public EiOperation
{
public:

  /// @brief コンストラクタ
  EiConcatOp(
    const PtExpr* pt_expr,           ///< [in] パース木の定義要素
    const vector<ElbExpr*>& opr_list ///< [in] オペランドのリスト
  );

  /// @brief デストラクタ
  ~EiConcatOp();


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

  /// @brief オペランド数を返す．
  SizeType
  operand_num() const override;

  /// @brief オペランドを返す．
  const VlExpr*
  operand(
    SizeType pos
  ) const override;

  /// @brief オペランドのリストを返す．
  vector<const VlExpr*>
  operand_list() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbExpr の設定用の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要求される式の型を計算してセットする．
  void
  _set_reqsize(
    const VlValueType& type ///< [in] 要求される式の型
  ) override;


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスで用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ビット幅を返す．
  SizeType
  bit_size() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // オペランドの配列
  vector<ElbExpr*> mOprList;

  // 結果のビット幅
  SizeType mSize;

};


//////////////////////////////////////////////////////////////////////
/// @class EiMultiConcatOp EiConcatOp.h "EiConcatOp.h"
/// @brief マルチ結合演算子
//////////////////////////////////////////////////////////////////////
class EiMultiConcatOp :
  public EiConcatOp
{
public:

  /// @brief コンストラクタ
  EiMultiConcatOp(
    const PtExpr* pt_expr,            ///< [in] パース木の定義要素
    SizeType rep_num,                 ///< [in] 繰り返し数
    ElbExpr* rep_expr,                ///< [in] 繰り返し数を表す式
    const vector<ElbExpr*>& opr_array ///< [in] オペランドのリスト
  );

  /// @brief デストラクタ
  ~EiMultiConcatOp();


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 式のタイプを返す．
  VlValueType
  value_type() const override;

  /// @brief オペランド数を返す．
  SizeType
  operand_num() const override;

  /// @brief オペランドを返す．
  const VlExpr*
  operand(
    SizeType pos
  ) const override;

  /// @brief オペランドのリストを返す．
  vector<const VlExpr*>
  operand_list() const override;

  /// @brief 繰り返し数を返す．
  /// @note multiple concatenation の時のみ意味を持つ．
  SizeType
  rep_num() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 繰り返し数
  SizeType mRepNum;

  // 繰り返し数を表す式
  ElbExpr* mRepExpr;

};

END_NAMESPACE_YM_VERILOG

#endif // EICONCATOP_H
