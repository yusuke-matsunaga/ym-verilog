﻿#ifndef EITERNARYOP_H
#define EITERNARYOP_H

/// @file EiTernaryOp.h
/// @brief EiTernaryOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.

//////////////////////////////////////////////////////////////////////
// expression を表すクラス
// IEEE Std 1364-2001 26.6.25 Simple expressions
// IEEE Std 1364-2001 26.6.26 Expressions
//
// operation の分類
// オペランドの数
// - 3個
//   - 返り値 第2オペランド or 第3オペランド, オペランド any
//     - ConditionOp
//   - 返り値 いずれかのオペランド, オペランド any
//     - MinTypMaxOp
//////////////////////////////////////////////////////////////////////

#include "EiOperation.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ElbTerinaryOp EiTernaryOp.h "EiTernaryOp.h"
/// @brief 三項演算子を表すクラス
//////////////////////////////////////////////////////////////////////
class EiTernaryOp :
  public EiOperation
{
public:

  /// @brief コンストラクタ
  EiTernaryOp(
    const PtExpr* pt_expr, ///< [in] パース木の定義要素
    ElbExpr* opr1,         ///< [in] オペランド1
    ElbExpr* opr2,         ///< [in] オペランド2
    ElbExpr* opr3          ///< [in] オペランド3
  );

  /// @brief デストラクタ
  ~EiTernaryOp();


public:
  //////////////////////////////////////////////////////////////////////
  // VlbExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 定数の時 true を返す．
  bool
  is_const() const override;

  /// @brief オペランド数を返す．
  SizeType
  operand_num() const override;

  /// @brief オペランドを返す．
  const VlExpr*
  operand(
    SizeType pos ///< [in] 位置番号
  ) const override;


protected:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // オペランド
  ElbExpr* mOpr[3];

};


//////////////////////////////////////////////////////////////////////
/// @class EiConditionOp EiTernaryOp.h "EiTernaryOp.h"
/// @brief 条件演算子
//////////////////////////////////////////////////////////////////////
class EiConditionOp :
  public EiTernaryOp
{
public:

  /// @brief コンストラクタ
  EiConditionOp(
    const PtExpr* pt_expr, ///< [in] パース木の定義要素
    ElbExpr* opr1,         ///< [in] オペランド1
    ElbExpr* opr2,         ///< [in] オペランド2
    ElbExpr* opr3          ///< [in] オペランド3
  );

  /// @brief デストラクタ
  ~EiConditionOp();


public:
  //////////////////////////////////////////////////////////////////////
  // ElbExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 式のタイプを返す．
  VlValueType
  value_type() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbExpr の設定用の仮想関数
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

  // 式の型
  VlValueType mType;

};


//////////////////////////////////////////////////////////////////////
/// @class EiMinTypMaxOp EiTernaryOp.h "EiTernaryOp.h"
/// @brief min-typ-max 演算子
//////////////////////////////////////////////////////////////////////
class EiMinTypMaxOp :
  public EiTernaryOp
{
public:

  /// @brief コンストラクタ
  EiMinTypMaxOp(
    const PtExpr* pt_expr, ///< [in] パース木の定義要素
    ElbExpr* opr1,         ///< [in] オペランド1
    ElbExpr* opr2,         ///< [in] オペランド2
    ElbExpr* opr3          ///< [in] オペランド3
  );

  /// @brief デストラクタ
  ~EiMinTypMaxOp();


public:
  //////////////////////////////////////////////////////////////////////
  // ElbExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 式のタイプを返す．
  VlValueType
  value_type() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbExpr の設定用の仮想関数
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

  // 式の型
  VlValueType mType;

};

END_NAMESPACE_YM_VERILOG

#endif // EITERNARYOP_H
