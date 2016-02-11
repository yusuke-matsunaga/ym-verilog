﻿#ifndef EIDECLHEAD_H
#define EIDECLHEAD_H

/// @file EiDeclHead.h
/// @brief EiDeclHead のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ElbDecl.h"
#include "EiRange.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiDeclHead.h EiDeclHead.h "EiDeclHead.h"
/// @brief 宣言要素のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////
class EiDeclHead :
  public ElbDeclHead
{
protected:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  EiDeclHead(const VlNamedObj* parent);

  /// @brief デストラクタ
  virtual
  ~EiDeclHead();


public:
  //////////////////////////////////////////////////////////////////////
  // ElbDeclHead の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief このオブジェクトの属しているスコープを返す．
  const VlNamedObj*
  parent() const;



private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親のスコープ
  const VlNamedObj* mParent;

};


//////////////////////////////////////////////////////////////////////
/// @class EiDeclHeadPt.h EiDeclHead.h "EiDeclHead.h"
/// @brief PtDeclH を用いた宣言要素のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////
class EiDeclHeadPt :
  public EiDeclHead
{
  friend class EiFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_head パース木の宣言ヘッダ
  EiDeclHeadPt(const VlNamedObj* parent,
	       const PtDeclHead* pt_head);

  /// @brief デストラクタ
  virtual
  ~EiDeclHeadPt();


public:
  //////////////////////////////////////////////////////////////////////
  // EiDeclHead の派生クラスに共通の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  virtual
  tVpiObjType
  type() const;

  /// @brief 符号の取得
  /// @retval true 符号つき
  /// @retval false 符号なし
  virtual
  bool
  is_signed() const;

  /// @brief 範囲指定を持つとき true を返す．
  virtual
  bool
  has_range() const;

  /// @brief 範囲の MSB の値を返す．
  /// @note 範囲を持たないときの値は不定
  virtual
  int
  left_range_val() const;

  /// @brief 範囲の LSB の値を返す．
  /// @note 範囲を持たないときの値は不定
  virtual
  int
  right_range_val() const;

  /// @brief 範囲のMSBを表す文字列の取得
  /// @note 範囲を持たない時の値は不定
  virtual
  string
  left_range_string() const;

  /// @brief 範囲のLSBを表す文字列の取得
  /// @note 範囲を持たない時の値は不定
  virtual
  string
  right_range_string() const;

  /// @brief left_range >= right_range の時に true を返す．
  virtual
  bool
  is_big_endian() const;

  /// @brief left_range <= right_range の時に true を返す．
  virtual
  bool
  is_little_endian() const;

  /// @brief ビット幅を返す．
  virtual
  ymuint32
  bit_size() const;

  /// @brief オフセット値の取得
  /// @param[in] index インデックス
  /// @param[out] offset インデックスに対するオフセット値
  /// @retval true インデックスが範囲内に入っている時
  /// @retval false インデックスが範囲外の時
  virtual
  bool
  calc_bit_offset(int index,
		  ymuint& offset) const;

  /// @brief データ型の取得
  /// @retval データ型 kParam, kLocalParam, kVar の場合
  /// @retval kVpiVarNone 上記以外
  virtual
  tVpiVarType
  data_type() const;

  /// @brief net 型の取得
  /// @retval net 型 net 型の要素の場合
  /// @retval kVpiNone net 型の要素でない場合
  virtual
  tVpiNetType
  net_type() const;

  /// @brief vectored|scalared 属性の取得
  /// @retval kVpiVsNone vectored|scalared 指定なし
  /// @retval kVpiVectored vectored 指定あり
  /// @retval kVpiScalared scalared 指定あり
  virtual
  tVpiVsType
  vs_type() const;

  /// @brief drive0 strength の取得
  /// @retval 0 の強度
  /// @retval kVpiNoStrength strength の指定なし
  virtual
  tVpiStrength
  drive0() const;

  /// @brief drive1 strength の取得
  /// @retval 1 の強度
  /// @retval kVpiNoStrength strength の指定なし
  virtual
  tVpiStrength
  drive1() const;

  /// @brief charge strength の取得
  /// @retval 電荷の強度
  /// @retval kVpiNoStrength strength の指定なし
  virtual
  tVpiStrength
  charge() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // パース木の宣言ヘッダ
  const PtDeclHead* mPtHead;

};


//////////////////////////////////////////////////////////////////////
/// @clacc EiDeclHeadPtD EiDeclHead.h "EiDeclHead.h"
/// @brief delay 付きの EiDeclHeadPt
//////////////////////////////////////////////////////////////////////
class EiDeclHeadPtD :
  public EiDeclHeadPt
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_head パース木の宣言ヘッダ
  EiDeclHeadPtD(const VlNamedObj* parent,
		const PtDeclHead* pt_head);

  /// @brief デストラクタ
  virtual
  ~EiDeclHeadPtD();


public:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスに共通な仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 遅延式の取得
  /// @retval delay
  /// @retval nullptr delay の指定なし
  virtual
  ElbDelay*
  delay() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 設定用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 遅延式の設定
  virtual
  void
  set_delay(ElbDelay* delay);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 遅延式
  ElbDelay* mDelay;

};


//////////////////////////////////////////////////////////////////////
/// @clacc EiDeclHeadPtV EiDeclHead.h "EiDeclHead.h"
/// @brief 範囲付きの EiDeclHeadPt
//////////////////////////////////////////////////////////////////////
class EiDeclHeadPtV :
  public EiDeclHeadPt
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
  EiDeclHeadPtV(const VlNamedObj* parent,
		const PtDeclHead* pt_head,
		const PtExpr* left,
		const PtExpr* right,
		int left_val,
		int right_val);

  /// @brief デストラクタ
  virtual
  ~EiDeclHeadPtV();


public:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスに共通な仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 範囲指定を持つとき true を返す．
  virtual
  bool
  has_range() const;

  /// @brief 範囲の MSB の値を返す．
  /// @note 範囲を持たないときの値は不定
  virtual
  int
  left_range_val() const;

  /// @brief 範囲の LSB の値を返す．
  /// @note 範囲を持たないときの値は不定
  virtual
  int
  right_range_val() const;

  /// @brief 範囲のMSBを表す文字列の取得
  /// @note 範囲を持たない時の値は不定
  virtual
  string
  left_range_string() const;

  /// @brief 範囲のLSBを表す文字列の取得
  /// @note 範囲を持たない時の値は不定
  virtual
  string
  right_range_string() const;

  /// @brief left_range >= right_range の時に true を返す．
  virtual
  bool
  is_big_endian() const;

  /// @brief left_range <= right_range の時に true を返す．
  virtual
  bool
  is_little_endian() const;

  /// @brief ビット幅を返す．
  virtual
  ymuint32
  bit_size() const;

  /// @brief オフセット値の取得
  /// @param[in] index インデックス
  /// @param[out] offset インデックスに対するオフセット値
  /// @retval true インデックスが範囲内に入っている時
  /// @retval false インデックスが範囲外の時
  virtual
  bool
  calc_bit_offset(int index,
		  ymuint& offset) const;


protected:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 範囲
  EiRangeImpl mRange;

};


//////////////////////////////////////////////////////////////////////
/// @clacc EiDeclHeadPtVD EiDeclHead.h "EiDeclHead.h"
/// @brief 範囲付き，delay 付きの EiDeclHeadPt
//////////////////////////////////////////////////////////////////////
class EiDeclHeadPtVD :
  public EiDeclHeadPtV
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_head パース木の宣言ヘッダ
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @param[in] left_val 範囲の左側の値
  /// @param[in] right_val 範囲の右側の値
  EiDeclHeadPtVD(const VlNamedObj* parent,
		 const PtDeclHead* pt_head,
		 const PtExpr* left,
		 const PtExpr* right,
		 int left_val,
		 int right_val);

  /// @brief デストラクタ
  virtual
  ~EiDeclHeadPtVD();


public:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスに共通な仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief delay の取得
  /// @retval delay
  /// @retval nullptr delay の指定なし
  virtual
  ElbDelay*
  delay() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 設定用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 遅延式の設定
  virtual
  void
  set_delay(ElbDelay* delay);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 遅延式
  ElbDelay* mDelay;

};


//////////////////////////////////////////////////////////////////////
/// @class EiDeclHeadPt2.h EiDeclHead.h "EiDeclHead.h"
/// @brief PtIOH を用いた宣言要素のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////
class EiDeclHeadPt2 :
  public EiDeclHead
{
  friend class EiFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_head パース木のIO宣言ヘッダ
  /// @param[in] aux_type 補助的なデータ型
  EiDeclHeadPt2(const VlNamedObj* parent,
		const PtIOHead* pt_head,
		tVpiAuxType aux_type);

  /// @brief デストラクタ
  virtual
  ~EiDeclHeadPt2();


public:
  //////////////////////////////////////////////////////////////////////
  // EiDeclHead の派生クラスに共通の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  /// @return vpi_user.h で定義された型 (vpiModule など)
  virtual
  tVpiObjType
  type() const;

  /// @brief 符号の取得
  /// @retval true 符号つき
  /// @retval false 符号なし
  virtual
  bool
  is_signed() const;

  /// @brief 範囲指定を持つとき true を返す．
  virtual
  bool
  has_range() const;

  /// @brief 範囲の MSB の値を返す．
  /// @note 範囲を持たないときの値は不定
  virtual
  int
  left_range_val() const;

  /// @brief 範囲の LSB の値を返す．
  /// @note 範囲を持たないときの値は不定
  virtual
  int
  right_range_val() const;

  /// @brief 範囲のMSBを表す文字列の取得
  /// @note 範囲を持たない時の値は不定
  virtual
  string
  left_range_string() const;

  /// @brief 範囲のLSBを表す文字列の取得
  /// @note 範囲を持たない時の値は不定
  virtual
  string
  right_range_string() const;

  /// @brief left_range >= right_range の時に true を返す．
  virtual
  bool
  is_big_endian() const;

  /// @brief left_range <= right_range の時に true を返す．
  virtual
  bool
  is_little_endian() const;

  /// @brief ビット幅を返す．
  virtual
  ymuint32
  bit_size() const;

  /// @brief オフセット値の取得
  /// @param[in] index インデックス
  /// @param[out] offset インデックスに対するオフセット値
  /// @retval true インデックスが範囲内に入っている時
  /// @retval false インデックスが範囲外の時
  virtual
  bool
  calc_bit_offset(int index,
		  ymuint& offset) const;

  /// @brief データ型の取得
  /// @retval データ型 kParam, kLocalParam, kVar の場合
  /// @retval kVpiVarNone 上記以外
  virtual
  tVpiVarType
  data_type() const;

  /// @brief net 型の取得
  /// @retval net 型 net 型の要素の場合
  /// @retval kVpiNone net 型の要素でない場合
  virtual
  tVpiNetType
  net_type() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // パース木のIO宣言ヘッダ
  const PtIOHead* mPtHead;

  // 補助的なデータ型
  tVpiAuxType mAuxType;

};


//////////////////////////////////////////////////////////////////////
/// @clacc EiDeclHeadPt2V EiDeclHead.h "EiDeclHead.h"
/// @brief 範囲付きの EiDeclHeadPt2
//////////////////////////////////////////////////////////////////////
class EiDeclHeadPt2V :
  public EiDeclHeadPt2
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_head パース木のIO宣言ヘッダ
  /// @param[in] aux_type 補助的なデータ型
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @param[in] left_val 範囲の MSB の値
  /// @param[in] right_val 範囲の LSB の値
  EiDeclHeadPt2V(const VlNamedObj* parent,
		 const PtIOHead* pt_head,
		 tVpiAuxType aux_type,
		 const PtExpr* left,
		 const PtExpr* right,
		 int left_val,
		 int right_val);

  /// @brief デストラクタ
  virtual
  ~EiDeclHeadPt2V();


public:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスに共通な仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 範囲指定を持つとき true を返す．
  virtual
  bool
  has_range() const;

  /// @brief 範囲の MSB の値を返す．
  /// @note 範囲を持たないときの値は不定
  virtual
  int
  left_range_val() const;

  /// @brief 範囲の LSB の値を返す．
  /// @note 範囲を持たないときの値は不定
  virtual
  int
  right_range_val() const;

  /// @brief 範囲のMSBを表す文字列の取得
  /// @note 範囲を持たない時の値は不定
  virtual
  string
  left_range_string() const;

  /// @brief 範囲のLSBを表す文字列の取得
  /// @note 範囲を持たない時の値は不定
  virtual
  string
  right_range_string() const;

  /// @brief left_range >= right_range の時に true を返す．
  virtual
  bool
  is_big_endian() const;

  /// @brief left_range <= right_range の時に true を返す．
  virtual
  bool
  is_little_endian() const;

  /// @brief ビット幅を返す．
  virtual
  ymuint32
  bit_size() const;

  /// @brief オフセット値の取得
  /// @param[in] index インデックス
  /// @param[out] offset インデックスに対するオフセット値
  /// @retval true インデックスが範囲内に入っている時
  /// @retval false インデックスが範囲外の時
  virtual
  bool
  calc_bit_offset(int index,
		  ymuint& offset) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 範囲
  EiRangeImpl mRange;

};


//////////////////////////////////////////////////////////////////////
/// @class EiDeclHeadPt3.h EiDeclHead.h "EiDeclHead.h"
/// @brief PtItem を用いた宣言要素のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////
class EiDeclHeadPt3 :
  public EiDeclHead
{
  friend class EiFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_item パース木の関数定義
  EiDeclHeadPt3(const VlNamedObj* parent,
		const PtItem* pt_item);

  /// @brief デストラクタ
  virtual
  ~EiDeclHeadPt3();


public:
  //////////////////////////////////////////////////////////////////////
  // EiDeclHead の派生クラスに共通の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  /// @return vpi_user.h で定義された型 (vpiModule など)
  virtual
  tVpiObjType
  type() const;

  /// @brief 符号の取得
  /// @retval true 符号つき
  /// @retval false 符号なし
  virtual
  bool
  is_signed() const;

  /// @brief 範囲指定を持つとき true を返す．
  virtual
  bool
  has_range() const;

  /// @brief 範囲の MSB の値を返す．
  /// @note 範囲を持たないときの値は不定
  virtual
  int
  left_range_val() const;

  /// @brief 範囲の LSB の値を返す．
  /// @note 範囲を持たないときの値は不定
  virtual
  int
  right_range_val() const;

  /// @brief 範囲のMSBを表す文字列の取得
  /// @note 範囲を持たない時の値は不定
  virtual
  string
  left_range_string() const;

  /// @brief 範囲のLSBを表す文字列の取得
  /// @note 範囲を持たない時の値は不定
  virtual
  string
  right_range_string() const;

  /// @brief left_range >= right_range の時に true を返す．
  virtual
  bool
  is_big_endian() const;

  /// @brief left_range <= right_range の時に true を返す．
  virtual
  bool
  is_little_endian() const;

  /// @brief ビット幅を返す．
  virtual
  ymuint32
  bit_size() const;

  /// @brief オフセット値の取得
  /// @param[in] index インデックス
  /// @param[out] offset インデックスに対するオフセット値
  /// @retval true インデックスが範囲内に入っている時
  /// @retval false インデックスが範囲外の時
  virtual
  bool
  calc_bit_offset(int index,
		  ymuint& offset) const;

  /// @brief データ型の取得
  /// @retval データ型 kParam, kLocalParam, kVar の場合
  /// @retval kVpiVarNone 上記以外
  virtual
  tVpiVarType
  data_type() const;

  /// @brief net 型の取得
  /// @retval net 型 net 型の要素の場合
  /// @retval kVpiNone net 型の要素でない場合
  /// @note このクラスでは kVpiNone を返す．
  virtual
  tVpiNetType
  net_type() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // パース木の関数定義
  const PtItem* mPtItem;

};


//////////////////////////////////////////////////////////////////////
/// @clacc EiDeclHeadPt3V EiDeclHead.h "EiDeclHead.h"
/// @brief 範囲付きの EiDeclHeadPt3
//////////////////////////////////////////////////////////////////////
class EiDeclHeadPt3V :
  public EiDeclHeadPt3
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_item パース木の関数定義
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @param[in] left_val 範囲の MSB の値
  /// @param[in] right_val 範囲の LSB の値
  EiDeclHeadPt3V(const VlNamedObj* parent,
		 const PtItem* pt_item,
		 const PtExpr* left,
		 const PtExpr* right,
		 int left_val,
		 int right_val);

  /// @brief デストラクタ
  virtual
  ~EiDeclHeadPt3V();


public:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスに共通な仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 範囲指定を持つとき true を返す．
  virtual
  bool
  has_range() const;

  /// @brief 範囲の MSB の値を返す．
  /// @note 範囲を持たないときの値は不定
  virtual
  int
  left_range_val() const;

  /// @brief 範囲の LSB の値を返す．
  /// @note 範囲を持たないときの値は不定
  virtual
  int
  right_range_val() const;

  /// @brief 範囲のMSBを表す文字列の取得
  /// @note 範囲を持たない時の値は不定
  virtual
  string
  left_range_string() const;

  /// @brief 範囲のLSBを表す文字列の取得
  /// @note 範囲を持たない時の値は不定
  virtual
  string
  right_range_string() const;

  /// @brief left_range >= right_range の時に true を返す．
  virtual
  bool
  is_big_endian() const;

  /// @brief left_range <= right_range の時に true を返す．
  virtual
  bool
  is_little_endian() const;

  /// @brief ビット幅を返す．
  virtual
  ymuint32
  bit_size() const;

  /// @brief オフセット値の取得
  /// @param[in] index インデックス
  /// @param[out] offset インデックスに対するオフセット値
  /// @retval true インデックスが範囲内に入っている時
  /// @retval false インデックスが範囲外の時
  virtual
  bool
  calc_bit_offset(int index,
		  ymuint& offset) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 範囲
  EiRangeImpl mRange;

};

END_NAMESPACE_YM_VERILOG

#endif // EIDECLHEAD_H
