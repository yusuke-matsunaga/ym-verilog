﻿#ifndef YM_PT_PTDECL_H
#define YM_PT_PTDECL_H

/// @file ym/pt/PtDecl.h
/// @brief 宣言要素関係の部品クラスのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym/pt/PtBase.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class PtIOHead PtDecl.h <ym/pt/PtDecl.h>
/// @ingroup VlParser
/// @ingroup PtGroup
/// @brief IO宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////
class PtIOHead :
  public PtBase
{
public:
  //////////////////////////////////////////////////////////////////////
  // PtIOH の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 方向の取得
  /// @return 方向(Input, Output, Inout)
  virtual
  VpiDir
  direction() const = 0;

  /// @brief 補助的な型の取得
  /// @return 補助的な型
  virtual
  VpiAuxType
  aux_type() const = 0;

  /// @brief 補助的なネット型の取得
  /// @return ネット型
  virtual
  VpiNetType
  net_type() const = 0;

  /// @brief 補助的な変数型の取得
  /// @return 変数型
  virtual
  VpiVarType
  var_type() const = 0;

  /// @brief 符号の取得
  /// @retval true 符号付き
  /// @retval false 符号なし
  virtual
  bool
  is_signed() const = 0;

  /// @brief 範囲のMSBの取得
  /// @retval 範囲のMSB
  /// @retval nullptr 範囲を持たないとき
  virtual
  const PtExpr*
  left_range() const = 0;

  /// @brief 範囲のLSBの取得
  /// @retval 範囲のLSB
  /// @retval nullptr 範囲を持たないとき
  virtual
  const PtExpr*
  right_range() const = 0;

  /// @brief 要素数の取得
  virtual
  SizeType
  item_num() const = 0;

  /// @brief 要素の取得
  virtual
  const PtIOItem*
  item(
    SizeType pos ///< [in] 位置 ( 0 <= pos < item_num() )
  ) const = 0;

  /// @brief 要素のリストを返す．
  vector<const PtIOItem*>
  item_list() const
  {
    SizeType n = item_num();
    vector<const PtIOItem*> vec(n);
    for ( SizeType i = 0; i < n; ++ i ) {
      vec[i] = item(i);
    }
    return vec;
  }

};


//////////////////////////////////////////////////////////////////////
/// @class PtIOItem PtDecl.h <ym/pt/PtDecl.h>
/// @ingroup VlParser
/// @ingroup PtGroup
/// @brief IO宣言要素の基底クラス
//////////////////////////////////////////////////////////////////////
class PtIOItem :
  public PtNamedBase
{
public:
  //////////////////////////////////////////////////////////////////////
  // PtIOItem の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期値の取得
  /// @retval 初期値
  /// @retval nullptr 初期値を持たないとき
  virtual
  const PtExpr*
  init_value() const = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class PtDeclHead PtDecl.h <ym/pt/PtDecl.h>
/// @ingroup VlParser
/// @ingroup PtGroup
/// @brief 宣言要素のヘッダの基底クラス
//////////////////////////////////////////////////////////////////////
class PtDeclHead :
  public PtBase
{
public:
  //////////////////////////////////////////////////////////////////////
  // PtDeclH の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 宣言要素の型の取得
  /// @return 宣言要素の型
  virtual
  PtDeclType
  type() const = 0;

  /// @brief 符号の取得
  /// @retval true 符号つき
  /// @retval false 符号なし
  virtual
  bool
  is_signed() const = 0;

  /// @brief 範囲のMSBの取得
  /// @retval 範囲のMSB 範囲を持つとき
  /// @retval nullptr 範囲を持たないとき
  virtual
  const PtExpr*
  left_range() const = 0;

  /// @brief 範囲のLSBの取得
  /// @retval 範囲のLSB 範囲を持つとき
  /// @retval nullptr 範囲を持たないとき
  virtual
  const PtExpr*
  right_range() const = 0;

  /// @brief データ型の取得
  /// @retval データ型 kParam, kLocalParam, kVar の場合
  /// @retval kVpiVarNone 上記以外
  virtual
  VpiVarType
  data_type() const = 0;

  /// @brief net 型の取得
  /// @retval net 型 net 型の要素の場合
  /// @retval kVpiNone net 型の要素でない場合
  virtual
  VpiNetType
  net_type() const = 0;

  /// @brief vectored|scalared 属性の取得
  /// @retval kVpiVsNone vectored|scalared 指定なし
  /// @retval kVpiVectored vectored 指定あり
  /// @retval kVpiScalared scalared 指定あり
  virtual
  VpiVsType
  vs_type() const = 0;

  /// @brief strength の取得
  /// @retval strength
  /// @retval nullptr strength の指定なし
  virtual
  const PtStrength*
  strength() const = 0;

  /// @brief delay の取得
  /// @retval delay
  /// @retval nullptr delay の指定なし
  virtual
  const PtDelay*
  delay() const = 0;

  /// @brief 要素数の取得
  virtual
  SizeType
  item_num() const = 0;

  /// @brief 要素の取得
  virtual
  const PtDeclItem*
  item(
    SizeType pos ///< [in] 位置 ( 0 <= pos < item_num() )
  ) const = 0;

  /// @brief 要素のリストを返す．
  vector<const PtDeclItem*>
  item_list() const
  {
    SizeType n = item_num();
    vector<const PtDeclItem*> vec(n);
    for ( SizeType i = 0; i < n; ++ i ) {
      vec[i] = item(i);
    }
    return vec;
  }

};


//////////////////////////////////////////////////////////////////////
/// @class PtDeclItem PtDecl.h <ym/pt/PtDecl.h>
/// @ingroup VlParser
/// @ingroup PtGroup
/// @brief 宣言要素のクラス
//////////////////////////////////////////////////////////////////////
class PtDeclItem :
  public PtNamedBase
{
public:
  //////////////////////////////////////////////////////////////////////
  // PtDeclItem の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief dimension list のサイズの取得
  virtual
  SizeType
  range_num() const = 0;

  /// @brief 範囲の取得
  virtual
  const PtRange*
  range(
    SizeType pos ///< [in] 位置 ( 0 <= pos < range_num() )
  ) const = 0;

  /// @brief 要素のリストを返す．
  vector<const PtRange*>
  range_list() const
  {
    SizeType n = range_num();
    vector<const PtRange*> vec(n);
    for ( SizeType i = 0; i < n; ++ i ) {
      vec[i] = range(i);
    }
    return vec;
  }

  /// @brief 初期値の取得
  /// @retval 初期値
  /// @retval nullptr 設定がない場合
  virtual
  const PtExpr*
  init_value() const = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class PtRange PtDecl.h <ym/pt/PtDecl.h>
/// @ingroup VlParser
/// @ingroup PtGroup
/// @brief 範囲を表すクラス
//////////////////////////////////////////////////////////////////////
class PtRange :
  public PtBase
{
public:
  //////////////////////////////////////////////////////////////////////
  // PtRange の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 範囲の MSB を取り出す．
  virtual
  const PtExpr*
  left() const = 0;

  /// @brief 範囲の LSB を取り出す．
  virtual
  const PtExpr*
  right() const = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // YM_PT_PTDECL_H
