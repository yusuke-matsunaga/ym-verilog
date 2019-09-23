#ifndef EIIMPNET_H
#define EIIMPNET_H

/// @file EiImpNet.h
/// @brief EiImpNet のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ElbDecl.h"
#include "ym/VlScalarVal.h"
#include "ym/pt/PtP.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiImpNet EiImpNet.h "EiImpNet.h"
/// @brief 暗黙のネットを表すクラス
//////////////////////////////////////////////////////////////////////
class EiImpNet :
  public ElbDecl
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] pt_expr パース木のプライマリ式
  EiImpNet(const VlNamedObj* parent,
	   const PtExpr* pt_expr,
	   VpiNetType net_type);

  /// @brief デストラクタ
  ~EiImpNet();


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
  /// @note このクラスでは false を返す．
  bool
  is_signed() const override;

  /// @brief 範囲指定を持つとき true を返す．
  bool
  has_range() const override;

  /// @brief 範囲の MSB の値を返す．
  /// @note 範囲を持たないときの値は不定
  int
  left_range_val() const override;

  /// @brief 範囲の LSB の値を返す．
  /// @note 範囲を持たないときの値は不定
  int
  right_range_val() const override;

  /// @brief 範囲のMSBを表す文字列の取得
  /// @note 範囲を持たない時の値は不定
  string
  left_range_string() const override;

  /// @brief 範囲のLSBを表す文字列の取得
  /// @note 範囲を持たない時の値は不定
  string
  right_range_string() const override;

  /// @brief left_range >= right_range の時に true を返す．
  bool
  is_big_endian() const override;

  /// @brief left_range <= right_range の時に true を返す．
  bool
  is_little_endian() const override;

  /// @brief ビット幅を返す．
  /// @note このクラスは 1 を返す．
  SizeType
  bit_size() const override;

  /// @brief オフセット値の取得
  /// @param[in] index インデックス
  /// @param[out] offset インデックスに対するオフセット値
  /// @retval true インデックスが範囲内に入っている時
  /// @retval false インデックスが範囲外の時
  bool
  calc_bit_offset(int index,
		  int& offset) const override;

  /// @brief データ型の取得
  /// @retval データ型 パラメータや変数の場合
  /// @retval kVpiVarNone 上記以外
  /// @note デフォルト値としてこのクラスでは kVpiVarNone を返す．
  VpiVarType
  data_type() const override;

  /// @brief net 型の取得
  /// @retval net 型 net 型の要素の場合
  /// @retval kVpiNone net 型の要素でない場合
  /// @note デフォルト値としてこのクラスでは kVpiNone を返す．
  VpiNetType
  net_type() const override;

  /// @brief vectored|scalared 属性の取得
  /// @retval kVpiVsNone vectored|scalared 指定なし
  /// @retval kVpiVectored vectored 指定あり
  /// @retval kVpiScalared scalared 指定あり
  /// @note デフォルト値としてこのクラスでは kVpiVsNone を返す．
  VpiVsType
  vs_type() const override;

  /// @brief drive0 strength の取得
  /// @retval 0 の強度
  /// @retval kVpiNoStrength strength の指定なし
  /// @note デフォルト値としてこのクラスでは kVpiNoStrength を返す．
  VpiStrength
  drive0() const override;

  /// @brief drive1 strength の取得
  /// @retval 1 の強度
  /// @retval kVpiNoStrength strength の指定なし
  /// @note デフォルト値としてこのクラスでは kVpiNoStrength を返す．
  VpiStrength
  drive1() const override;

  /// @brief charge strength の取得
  /// @retval 電荷の強度
  /// @retval kVpiNoStrength strength の指定なし
  /// @note デフォルト値としてこのクラスでは kVpiNoStrength を返す．
  VpiStrength
  charge() const override;

  /// @brief delay の取得
  /// @retval delay
  /// @retval nullptr delay の指定なし
  /// @note デフォルト値としてこのクラスでは nullptr を返す．
  const VlDelay*
  delay() const override;

  /// @brief 初期値の取得
  /// @retval 初期値
  /// @retval nullptr 設定がない場合
  const VlExpr*
  init_value() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 符号付きに補正する．
  void
  set_signed() override;

  /// @brief スカラー値を返す．
  VlScalarVal
  get_scalar() const override;

  /// @brief スカラー値を設定する．
  /// @param[in] val 値
  void
  set_scalar(const VlScalarVal& val) override;

  /// @brief 論理値を返す．
  VlScalarVal
  get_logic() const override;

  /// @brief real 型の値を返す．
  double
  get_real() const override;

  /// @brief real 型の値を設定する．
  /// @param[in] val 値
  void
  set_real(double val) override;

  /// @brief bitvector 型の値を返す．
  /// @param[out] val 値
  /// @param[in] req_type 要求される型
  void
  get_bitvector(BitVector& val,
		const VlValueType& req_type = VlValueType()) const override;

  /// @brief bitvector 型の値を設定する．
  /// @param[in] val 値
  void
  set_bitvector(const BitVector& val) override;

  /// @brief ビット選択値を返す．
  /// @param[in] index ビット位置
  VlScalarVal
  get_bitselect(int index) const override;

  /// @brief ビット値を設定する．
  /// @param[in] index ビット位置
  /// @param[in] val 値
  void
  set_bitselect(int index,
		const VlScalarVal& val) override;

  /// @brief 範囲選択値を返す．
  /// @param[in] left 範囲の MSB
  /// @param[in] right 範囲の LSB
  /// @param[out] val 値
  void
  get_partselect(int left,
		 int right,
		 BitVector& val) const override;

  /// @brief 範囲値を設定する．
  /// @param[in] left 範囲の MSB
  /// @param[in] right 範囲の LSB
  /// @param[in] val 値
  void
  set_partselect(int left,
		 int right,
		 const BitVector& val) override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親のスコープ
  const VlNamedObj* mParent;

  // パース木のプライマリ式
  const PtExpr* mPtExpr;

  // ネット型
  VpiNetType mNetType;

  // 値
  VlScalarVal mVal;

};

END_NAMESPACE_YM_VERILOG

#endif // EIIMPNET_H
