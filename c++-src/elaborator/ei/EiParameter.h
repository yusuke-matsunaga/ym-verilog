#ifndef EIPARAMETER_H
#define EIPARAMETER_H

/// @file EiParameter.h
/// @brief EiParameter のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "elaborator/ElbParameter.h"
#include "EiRange.h"

#include "ym/VlValue.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiParamHead.h EiParameter.h "EiParameter.h"
/// @brief parameter 宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////
class EiParamHead :
  public ElbParamHead
{
  friend class EiFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_head パース木の宣言ヘッダ
  EiParamHead(const VlNamedObj* parent,
	      const PtDeclHead* pt_head);

  /// @brief デストラクタ
  ~EiParamHead();


public:
  //////////////////////////////////////////////////////////////////////
  // ElbParamHead の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  VpiObjType
  type() const override;

  /// @brief このオブジェクトの属しているスコープを返す．
  const VlNamedObj*
  parent() const override;

  /// @brief 符号の取得
  /// @param[in] val 値
  /// @retval true 符号つき
  /// @retval false 符号なし
  /// @note ヘッダに型指定がない時は値から情報を得る．
  bool
  is_signed(const VlValue& val) const override;

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
  /// @param[in] val 値
  /// @note ヘッダに型指定がない時は値から情報を得る．
  SizeType
  bit_size(const VlValue& val) const override;

  /// @brief オフセット値の取得
  /// @param[in] index インデックス
  /// @param[out] offset インデックスに対するオフセット値
  /// @param[in] val 値
  /// @retval true インデックスが範囲内に入っている時
  /// @retval false インデックスが範囲外の時
  /// @note ヘッダに型指定がない時は値から情報を得る．
  bool
  calc_bit_offset(int index,
		  SizeType& offset,
		  const VlValue& val) const override;

  /// @breif 値の型を返す．
  /// @param[in] val 値
  /// @note ヘッダに型指定がない時は値から情報を得る．
  VlValueType
  value_type(const VlValue& val) const override;

  /// @brief データ型の取得
  VpiVarType
  data_type() const override;


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief パース木の宣言ヘッダを返す．
  const PtDeclHead*
  pt_head() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親のスコープ
  const VlNamedObj* mParent;

  // パース木の宣言ヘッダ
  const PtDeclHead* mPtHead;

};


//////////////////////////////////////////////////////////////////////
/// @clacc EiParamHeadV EiParameter.h "EiParameter.h"
/// @brief 範囲付きの EiParamHead
//////////////////////////////////////////////////////////////////////
class EiParamHeadV :
  public EiParamHead
{
  friend class EiFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_head パース木の宣言ヘッダ
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @param[in] left_val 範囲の左側の値
  /// @param[in] right_val 範囲の右側の値
  EiParamHeadV(const VlNamedObj* parent,
	       const PtDeclHead* pt_head,
	       const PtExpr* left,
	       const PtExpr* right,
	       int left_val,
	       int right_val);

  /// @brief デストラクタ
  ~EiParamHeadV();


public:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスに共通な仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 符号の取得
  /// @param[in] val 値
  /// @retval true 符号つき
  /// @retval false 符号なし
  /// @note ヘッダに型指定がない時は値から情報を得る．
  bool
  is_signed(const VlValue& val) const override;

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
  /// @param[in] val 値
  /// @note ヘッダに型指定がない時は値から情報を得る．
  SizeType
  bit_size(const VlValue& val) const override;

  /// @brief オフセット値の取得
  /// @param[in] index インデックス
  /// @param[out] offset インデックスに対するオフセット値
  /// @param[in] val 値
  /// @retval true インデックスが範囲内に入っている時
  /// @retval false インデックスが範囲外の時
  /// @note ヘッダに型指定がない時は値から情報を得る．
  bool
  calc_bit_offset(int index,
		  SizeType& offset,
		  const VlValue& val) const override;

  /// @breif 値の型を返す．
  /// @param[in] val 値
  /// @note ヘッダに型指定がない時は値から情報を得る．
  VlValueType
  value_type(const VlValue& val) const override;


protected:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 範囲
  EiRangeImpl mRange;

};


//////////////////////////////////////////////////////////////////////
/// @class EiParameter EiParameter.h "EiParameter.h"
/// @brief ElbParameter の実装クラス
//////////////////////////////////////////////////////////////////////
class EiParameter :
  public ElbParameter
{
  friend class EiFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] head ヘッダ
  /// @param[in] pt_item パース木の宣言要素
  EiParameter(ElbParamHead* head,
	      const PtNamedBase* pt_item);

  /// @brief デストラクタ
  ~EiParameter();


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
  /// @note このクラスでは kVpiNone を返す．
  VpiNetType
  net_type() const override;

  /// @brief vectored|scalared 属性の取得
  /// @retval kVpiVsNone vectored|scalared 指定なし
  /// @retval kVpiVectored vectored 指定あり
  /// @retval kVpiScalared scalared 指定あり
  /// @note このクラスでは kVpiVsNone を返す．
  VpiVsType
  vs_type() const override;

  /// @brief drive0 strength の取得
  /// @retval 0 の強度
  /// @retval kVpiNoStrength strength の指定なし
  /// @note このクラスでは kVpiNoStrength を返す．
  VpiStrength
  drive0() const override;

  /// @brief drive1 strength の取得
  /// @retval 1 の強度
  /// @retval kVpiNoStrength strength の指定なし
  /// @note kVpiNoStrength を返す．
  VpiStrength
  drive1() const override;

  /// @brief charge strength の取得
  /// @retval 電荷の強度
  /// @retval kVpiNoStrength strength の指定なし
  /// @note このクラスでは kVpiNoStrength を返す．
  VpiStrength
  charge() const override;

  /// @brief delay の取得
  /// @retval delay
  /// @retval nullptr delay の指定なし
  /// @note このクラスでは nullptr を返す．
  const VlDelay*
  delay() const override;

  /// @brief 定数値を持つ型のときに true を返す．
  /// @note このクラスは true を返す．
  bool
  is_consttype() const override;

  /// @brief 初期値の取得
  /// @retval 初期値
  /// @retval nullptr 設定がない場合
  const VlExpr*
  init_value() const override;

  /// @brief localparam のときに true 返す．
  /// @note このクラスでは false を返す．
  bool
  is_local_param() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbParameter の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値の取得
  VlValue
  get_value() const override;

  /// @brief 値の設定
  /// @param[in] expr 値を表す式
  /// @param[in] value 値
  void
  set_expr(const PtExpr* expr,
	   const VlValue& value) override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ヘッダ
  ElbParamHead* mHead;

  // パース木の宣言要素
  const PtNamedBase* mPtItem;

  // 式
  const PtExpr* mExpr;

  // 値
  VlValue mValue;

};


//////////////////////////////////////////////////////////////////////
/// @class EiLocalParam EiParameter.h "EiParameter.h"
/// @brief ElbParameter の実装クラス (localparam版)
//////////////////////////////////////////////////////////////////////
class EiLocalParam :
  public EiParameter
{
  friend class EiFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] head ヘッダ
  /// @param[in] pt_item パース木の宣言要素
  EiLocalParam(ElbParamHead* head,
	       const PtNamedBase* pt_item);

  /// @brief デストラクタ
  ~EiLocalParam();


public:
  //////////////////////////////////////////////////////////////////////
  // VlDecl の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief localparam のときに true 返す．
  /// @note このクラスでは true を返す．
  bool
  is_local_param() const override;

};

END_NAMESPACE_YM_VERILOG

#endif // EIPARAMETER_H
