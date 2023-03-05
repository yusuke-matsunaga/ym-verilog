#ifndef YM_PT_PTEXPR_H
#define YM_PT_PTEXPR_H

/// @file ym/pt/PtExpr.h
/// @brief PtExpr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym/pt/PtBase.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class PtExpr PtExpr.h <ym/pt/PtExpr.h>
/// @ingroup VlParser
/// @ingroup PtGroup
/// @brief expression を表すクラスの共通の親クラス
//////////////////////////////////////////////////////////////////////
class PtExpr :
  public PtHierNamedBase
{
public:
  //////////////////////////////////////////////////////////////////////
  // PtExpr の継承クラスが実装しなければならない仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 式の型の取得
  /// @return 式の型
  virtual
  PtExprType
  type() const = 0;

  /// @brief 演算子の種類の取得
  /// @return 演算子の種類
  virtual
  VpiOpType
  op_type() const = 0;

  /// @brief オペランドの数の取得
  /// @return 子供の数
  virtual
  SizeType
  operand_num() const = 0;

  /// @brief 0番目のオペランドの取得
  virtual
  const PtExpr*
  operand0() const = 0;

  /// @brief 1番目のオペランドの取得
  virtual
  const PtExpr*
  operand1() const = 0;

  /// @brief 2番目のオペランドの取得
  virtual
  const PtExpr*
  operand2() const = 0;

  /// @brief オペランドの取得
  /// @return pos 番目のオペランド
  virtual
  const PtExpr*
  operand(
    SizeType pos ///< [in] 取り出すオペランンドの位置(最初の位置は 0)
  ) const = 0;

  /// @brief 定数インデックスのチェック
  /// @retval true インデックスもしくは範囲が定数にならなければならないとき
  /// @retval false 上記以外
  virtual
  bool
  is_const_index() const = 0;

  /// @brief インデックスリストのサイズの取得
  /// @return インデックスリストのサイズ
  virtual
  SizeType
  index_num() const = 0;

  /// @brief インデックスの取得
  virtual
  const PtExpr*
  index(
    SizeType pos ///< [in] 位置番号 ( 0 <= pos < index_num() )
  ) const = 0;

  /// @brief 範囲指定モードの取得
  /// @return 範囲指定モード
  virtual
  VpiRangeMode
  range_mode() const = 0;

  /// @brief 範囲の左側の式の取得
  /// @return 範囲の左側の式
  virtual
  const PtExpr*
  left_range() const = 0;

  /// @brief 範囲の右側の式の取得
  /// @return 範囲の右側の式
  virtual
  const PtExpr*
  right_range() const = 0;

  /// @brief 定数の種類の取得
  /// @return 定数の種類
  virtual
  VpiConstType
  const_type() const = 0;

  /// @brief 整数型の定数のサイズの取得
  /// @return サイズ
  ///
  /// サイズ指定の無い場合と整数型の定数でない場合には 0 を返す．
  virtual
  SizeType
  const_size() const = 0;

  /// @brief 整数型の値の取得
  /// @return 値
  virtual
  ymuint32
  const_uint32() const = 0;

  /// @brief 整数型および文字列型の定数の文字列表現の取得
  /// @return 値の文字列表現
  ///
  /// 整数型時のサイズと基数は含まない．
  virtual
  const char*
  const_str() const = 0;

  /// @brief 実数型の値の取得
  /// @return 値
  virtual
  double
  const_real() const = 0;

  /// @brief インデックスとして使える式のチェック
  /// @retval true 階層名の添字として使える式
  /// @retval false 使えない式
  virtual
  bool
  is_index_expr() const = 0;

  /// @brief インデックスの値の取得
  /// @return 階層名の添字として使える式の時にその値を返す．
  virtual
  int
  index_value() const = 0;

  /// @brief simple primary のチェック
  /// @retval true index_list も range も持たないとき
  /// @retval false 上記以外
  virtual
  bool
  is_simple() const = 0;

  /// @brief 式を表す文字列表現の取得
  virtual
  string
  decompile() const = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // YM_PT_PTEXPR_H
