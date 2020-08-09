#ifndef EICONCATOP_H
#define EICONCATOP_H

/// @file EiConcatOp.h
/// @brief 結合演算子の派生クラスのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
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
  friend class EiFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] opr_size オペランド数
  /// @param[in] opr_array オペランドを格納する配列
  EiConcatOp(const PtExpr* pt_expr,
	     SizeType opr_size,
	     ElbExpr** opr_array);

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
  /// @note オペランドが定数ならこの式も定数となる．
  bool
  is_const() const override;

  /// @brief オペランド数を返す．
  SizeType
  operand_num() const override;

  /// @brief オペランドを返す．
  const VlExpr*
  operand(SizeType pos) const override;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbExpr の設定用の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要求される式の型を計算してセットする．
  /// @param[in] type 要求される式の型
  /// @note 必要であればオペランドに対して再帰的に処理を行なう．
  void
  _set_reqsize(const VlValueType& type) override;


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

  // オペランド数
  SizeType mOprNum;

  // オペランドの配列
  ElbExpr** mOprList;

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
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] rep_num 繰り返し数
  /// @param[in] rep_expr 繰り返し数を表す式
  /// @param[in] opr_size オペランド数
  /// @param[in] opr_array オペランドを格納する配列
  /// @note は opr_size は繰り返し数のオペランドは含まない．
  EiMultiConcatOp(const PtExpr* pt_expr,
		  SizeType rep_num,
		  ElbExpr* rep_expr,
		  SizeType opr_size,
		  ElbExpr** opr_array);

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
  operand(SizeType pos) const override;

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
