#ifndef ELBPARAMETER_H
#define ELBPARAMETER_H

/// @file ElbParameter.h
/// @brief ElbParameter のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.


#include "ElbDecl.h"

#include "ym/pt/PtP.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ElbParamHead.h ElbParameter.h "ElbParameter.h"
/// @brief 宣言要素のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////
class ElbParamHead
{
protected:

  /// @brief コンストラクタ
  ElbParamHead() = default;

  /// @brief デストラクタ
  virtual
  ~ElbParamHead() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbParamHead の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  virtual
  VpiObjType
  type() const = 0;

  /// @brief このオブジェクトの属しているスコープを返す．
  virtual
  const VlScope*
  parent_scope() const = 0;

  /// @brief 符号の取得
  /// @param[in] val 値
  /// @retval true 符号つき
  /// @retval false 符号なし
  /// @note ヘッダに型指定がない時は値から情報を得る．
  virtual
  bool
  is_signed(const VlValue& val) const = 0;

  /// @brief 範囲指定を持つとき true を返す．
  virtual
  bool
  has_range() const = 0;

  /// @brief 範囲の MSB の値を返す．
  /// @note 範囲を持たないときの値は不定
  virtual
  int
  left_range_val() const = 0;

  /// @brief 範囲の LSB の値を返す．
  /// @note 範囲を持たないときの値は不定
  virtual
  int
  right_range_val() const = 0;

  /// @brief 範囲のMSBを表す文字列の取得
  /// @note 範囲を持たない時の値は不定
  virtual
  string
  left_range_string() const = 0;

  /// @brief 範囲のLSBを表す文字列の取得
  /// @note 範囲を持たない時の値は不定
  virtual
  string
  right_range_string() const = 0;

  /// @brief left_range >= right_range の時に true を返す．
  virtual
  bool
  is_big_endian() const = 0;

  /// @brief left_range <= right_range の時に true を返す．
  virtual
  bool
  is_little_endian() const = 0;

  /// @brief ビット幅を返す．
  /// @param[in] val 値
  /// @note ヘッダが型指定を持たない時には値から情報を得る．
  virtual
  SizeType
  bit_size(const VlValue& val) const = 0;

  /// @brief オフセット値の取得
  /// @param[in] index インデックス
  /// @param[out] offset インデックスに対するオフセット値
  /// @param[in] val 値
  /// @retval true インデックスが範囲内に入っている時
  /// @retval false インデックスが範囲外の時
  /// @note ヘッダが型指定を持たない時には値から情報を得る．
  virtual
  bool
  calc_bit_offset(int index,
		  SizeType& offset,
		  const VlValue& val) const = 0;

  /// @breif 値の型を返す．
  /// @param[in] val 値
  /// @note ヘッダが型指定を持たない時には値から情報を得る．
  virtual
  VlValueType
  value_type(const VlValue& val) const = 0;

  /// @brief データ型の取得
  virtual
  VpiVarType
  data_type() const = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class ElbParameter ElbParameter.h "ElbParameter.h"
/// @brief parameter を表すクラス
/// IEEE Std 1364-2001 26.6.12 Parameter, specparam
///
/// 設定用仮想関数の定義を行っている．
//////////////////////////////////////////////////////////////////////
class ElbParameter :
  public VlDecl
{
public:
  //////////////////////////////////////////////////////////////////////
  // ElbParameter の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値の取得
  virtual
  const PtExpr*
  init_expr() const = 0;

  /// @brief 値の取得
  virtual
  VlValue
  value() const = 0;

  /// @brief parameter の値の設定
  /// @param[in] expr 値を表す式
  /// @param[in] value 値
  virtual
  void
  set_init_expr(const PtExpr* expr,
		const VlValue& value) = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // ELBPARAMETER_H
