﻿#ifndef EIDECLARRAY_H
#define EIDECLARRAY_H

/// @file EiDeclArray.h
/// @brief EiDeclArray のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym/vl/VlDeclArray.h"
#include "EiRange.h"

#include "ym/BitVector.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiDeclArray EiDeclArray.h "EiDeclArray.h"
/// @brief 配列型の ElbDecl
//////////////////////////////////////////////////////////////////////
class EiDeclArray :
  public VlDeclArray
{
  friend class EiFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] head ヘッダ
  /// @param[in] pt_item パース木の宣言要素
  /// @param[in] dim_size 次元数
  /// @param[in] range_array 範囲の配列
  EiDeclArray(ElbDeclHead* head,
	      const PtNamedBase* pt_item,
	      SizeType dim_size,
	      EiRange* range_array);

  /// @brief デストラクタ
  ~EiDeclArray();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  VpiObjType
  type() const override;

  /// @brief ファイル位置を返す．
  FileRegion
  file_region() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // VlNamedObj の派生クラスに共通な仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief このオブジェクトの属しているスコープを返す．
  const VlNamedObj*
  parent() const override;

  /// @brief 名前の取得
  const char*
  name() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // VlDecl の関数
  //////////////////////////////////////////////////////////////////////

  /// @breif 値の型を返す．
  /// @note 値を持たないオブジェクトの場合には kVpiValueNone を返す．
  VlValueType
  value_type() const override;

  /// @brief 符号の取得
  /// @retval true 符号つき
  /// @retval false 符号なし
  bool
  is_signed() const override;

  /// @brief 範囲指定を持つとき true を返す．
  bool
  has_range() const override;

  /// @brief 範囲の MSB の値を返す．
  ///
  /// 範囲を持たないときの値は不定
  int
  left_range_val() const override;

  /// @brief 範囲の LSB の値を返す．
  ///
  /// 範囲を持たないときの値は不定
  int
  right_range_val() const override;

  /// @brief 範囲のMSBを表す文字列の取得
  ///
  /// 範囲を持たない時の値は不定
  string
  left_range_string() const override;

  /// @brief 範囲のLSBを表す文字列の取得
  ///
  /// 範囲を持たない時の値は不定
  string
  right_range_string() const override;

  /// @brief left_range >= right_range の時に true を返す．
  bool
  is_big_endian() const override;

  /// @brief left_range <= right_range の時に true を返す．
  bool
  is_little_endian() const override;

  /// @brief ビット幅を返す．
  SizeType
  bit_size() const override;

  /// @brief オフセット値の取得
  /// @param[in] index インデックス
  /// @param[out] offset インデックスに対するオフセット値
  /// @retval true インデックスが範囲内に入っている時
  /// @retval false インデックスが範囲外の時
  bool
  calc_bit_offset(int index,
		  SizeType& offset) const override;

  /// @brief データ型の取得
  /// @retval データ型 kParam, kLocalParam, kVar の場合
  /// @retval kVpiVarNone 上記以外
  VpiVarType
  data_type() const override;

  /// @brief net 型の取得
  /// @retval net 型 net 型の要素の場合
  /// @retval kVpiNone net 型の要素でない場合
  VpiNetType
  net_type() const override;

  /// @brief vectored|scalared 属性の取得
  /// @retval kVpiVsNone vectored|scalared 指定なし
  /// @retval kVpiVectored vectored 指定あり
  /// @retval kVpiScalared scalared 指定あり
  VpiVsType
  vs_type() const override;

  /// @brief drive0 strength の取得
  /// @retval 0 の強度
  /// @retval kVpiNoStrength strength の指定なし
  VpiStrength
  drive0() const override;

  /// @brief drive1 strength の取得
  /// @retval 1 の強度
  /// @retval kVpiNoStrength strength の指定なし
  VpiStrength
  drive1() const override;

  /// @brief charge strength の取得
  /// @retval 電荷の強度
  /// @retval kVpiNoStrength strength の指定なし
  VpiStrength
  charge() const override;

  /// @brief delay の取得
  /// @retval delay
  /// @retval nullptr delay の指定なし
  const VlDelay*
  delay() const override;

  /// @brief 配列型オブジェクトの時に true を返す．
  bool
  is_array() const override;

  /// @brief 多次元の配列型オブジェクトの時に true を返す．
  bool
  is_multi_array() const override;

  /// @brief 配列型オブジェクトの場合の次元数の取得
  SizeType
  dimension() const override;

  /// @brief 範囲の取得
  /// @param[in] pos 位置 ( 0 <= pos < dimension() )
  const VlRange*
  range(SizeType pos) const override;

  /// @brief 配列の要素数の取得
  SizeType
  array_size() const override;

  /// @brief 1次元配列の場合にインデックスからオフセットを計算する．
  /// @param[in] index インデックス
  /// @param[out] offset index に対するオフセット値
  /// @retval true index が範囲内だった．
  /// @retval false index が範囲外だった．
  bool
  calc_array_offset(int index,
		    SizeType& offset) const override;

  /// @brief 他次元配列の場合にインデックスのリストからオフセットを計算する．
  /// @param[in] index_list インデックスのリスト
  /// @param[out] offset index_list に対するオフセット値
  /// @retval true オフセットが正しく計算できた．
  /// @retval false index_list のいずれかの値が範囲外だった．
  bool
  calc_array_offset(const vector<int>& index_list,
		    SizeType& offset) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ヘッダ
  ElbDeclHead* mHead;

  // パース木の宣言要素
  const PtNamedBase* mPtItem;

  // dimension の配列
  EiRangeArray mRangeList;

};

END_NAMESPACE_YM_VERILOG

#endif // EIDECLARRAY_H
