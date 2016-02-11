﻿#ifndef EIDECL_H
#define EIDECL_H

/// @file EiDecl.h
/// @brief EiDecl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ElbDecl.h"

#include "ym/BitVector.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiDecl EiDecl.h "EiDecl.h"
/// @brief ElbDecl の実装クラス
//////////////////////////////////////////////////////////////////////
class EiDecl :
  public ElbDecl
{
  friend class EiFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] head ヘッダ
  /// @param[in] pt_item パース木の宣言要素
  EiDecl(ElbDeclHead* head,
	 const PtNamedBase* pt_item);

  /// @brief デストラクタ
  virtual
  ~EiDecl();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  virtual
  tVpiObjType
  type() const;

  /// @brief ファイル位置を返す．
  virtual
  FileRegion
  file_region() const;


public:
  //////////////////////////////////////////////////////////////////////
  // VlNamedObj の派生クラスに共通な仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief このオブジェクトの属しているスコープを返す．
  virtual
  const VlNamedObj*
  parent() const;

  /// @brief 名前の取得
  virtual
  const char*
  name() const;


public:
  //////////////////////////////////////////////////////////////////////
  // VlDecl の関数
  //////////////////////////////////////////////////////////////////////

  /// @breif 値の型を返す．
  /// @note 値を持たないオブジェクトの場合には kVpiValueNone を返す．
  virtual
  VlValueType
  value_type() const;

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
  ymuint
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

  /// @brief delay の取得
  /// @retval delay
  /// @retval nullptr delay の指定なし
  virtual
  const VlDelay*
  delay() const;

  /// @brief 初期値の取得
  /// @retval 初期値
  /// @retval nullptr 設定がない場合
  virtual
  const VlExpr*
  init_value() const;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 符号付きに補正する．
  virtual
  void
  set_signed();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ヘッダ
  ElbDeclHead* mHead;

  // パース木の宣言要素
  const PtNamedBase* mPtItem;

  // 符号付き属性の補正値
  bool mAuxSign;

};


//////////////////////////////////////////////////////////////////////
/// @class EiDeclN EiDecl.h "EiDecl.h"
/// @brief 値を持たない EiDecl
//////////////////////////////////////////////////////////////////////
class EiDeclN :
  public EiDecl
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] head ヘッダ
  /// @param[in] pt_item パース木の宣言要素
  EiDeclN(ElbDeclHead* head,
	  const PtNamedBase* pt_item);

  /// @brief デストラクタ
  virtual
  ~EiDeclN();


public:
  //////////////////////////////////////////////////////////////////////
  // ElbDecl の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief スカラー値を返す．
  virtual
  VlScalarVal
  get_scalar() const;

  /// @brief スカラー値を設定する．
  /// @param[in] val 値
  virtual
  void
  set_scalar(const VlScalarVal& val);

  /// @brief 論理値を返す．
  virtual
  VlScalarVal
  get_logic() const;

  /// @brief real 型の値を返す．
  virtual
  double
  get_real() const;

  /// @brief real 型の値を設定する．
  /// @param[in] val 値
  virtual
  void
  set_real(double val);

  /// @brief bitvector 型の値を返す．
  /// @param[out] val 値
  /// @param[in] req_type 要求される型
  virtual
  void
  get_bitvector(BitVector& val,
		const VlValueType& req_type = VlValueType()) const;

  /// @brief bitvector 型の値を設定する．
  /// @param[in] val 値
  virtual
  void
  set_bitvector(const BitVector& val);

  /// @brief ビット選択値を返す．
  /// @param[in] index ビット位置
  virtual
  VlScalarVal
  get_bitselect(int index) const;

  /// @brief ビット値を設定する．
  /// @param[in] index ビット位置
  /// @param[in] val 値
  virtual
  void
  set_bitselect(int index,
		const VlScalarVal& val);

  /// @brief 範囲選択値を返す．
  /// @param[in] left 範囲の MSB
  /// @param[in] right 範囲の LSB
  /// @param[out] val 値
  virtual
  void
  get_partselect(int left,
		 int right,
		 BitVector& val) const;

  /// @brief 範囲値を設定する．
  /// @param[in] left 範囲の MSB
  /// @param[in] right 範囲の LSB
  /// @param[in] val 値
  virtual
  void
  set_partselect(int left,
		 int right,
		 const BitVector& val);

};


//////////////////////////////////////////////////////////////////////
/// @class EiDeclS EiDecl.h "EiDecl.h"
/// @brief スカラー値を持つ EiDecl
//////////////////////////////////////////////////////////////////////
class EiDeclS :
  public EiDecl
{
  friend class EiFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] head ヘッダ
  /// @param[in] pt_item パース木の宣言要素
  EiDeclS(ElbDeclHead* head,
	  const PtNamedBase* pt_item);

  /// @brief デストラクタ
  virtual
  ~EiDeclS();


public:
  //////////////////////////////////////////////////////////////////////
  // ElbDecl の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief スカラー値を返す．
  virtual
  VlScalarVal
  get_scalar() const;

  /// @brief スカラー値を設定する．
  /// @param[in] val 値
  virtual
  void
  set_scalar(const VlScalarVal& val);

  /// @brief 論理値を返す．
  virtual
  VlScalarVal
  get_logic() const;

  /// @brief real 型の値を返す．
  virtual
  double
  get_real() const;

  /// @brief real 型の値を設定する．
  /// @param[in] val 値
  virtual
  void
  set_real(double val);

  /// @brief bitvector 型の値を返す．
  /// @param[out] val 値
  /// @param[in] req_type 要求される型
  virtual
  void
  get_bitvector(BitVector& val,
		const VlValueType& req_type = VlValueType()) const;

  /// @brief bitvector 型の値を設定する．
  virtual
  void
  set_bitvector(const BitVector& val);

  /// @brief ビット選択値を返す．
  /// @param[in] index ビット位置
  virtual
  VlScalarVal
  get_bitselect(int index) const;

  /// @brief ビット値を設定する．
  /// @param[in] index ビット位置
  /// @param[in] val 値
  virtual
  void
  set_bitselect(int index,
		const VlScalarVal& val);

  /// @brief 範囲選択値を返す．
  /// @param[in] left 範囲の MSB
  /// @param[in] right 範囲の LSB
  /// @param[out] val 値
  virtual
  void
  get_partselect(int left,
		 int right,
		 BitVector& val) const;

  /// @brief 範囲値を設定する．
  /// @param[in] left 範囲の MSB
  /// @param[in] right 範囲の LSB
  /// @param[in] val 値
  virtual
  void
  set_partselect(int left,
		 int right,
		 const BitVector& val);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値
  VlScalarVal mVal;

};


//////////////////////////////////////////////////////////////////////
/// @class EiDeclR EiDecl.h "EiDecl.h"
/// @brief 実数値を持つ EiDecl
//////////////////////////////////////////////////////////////////////
class EiDeclR :
  public EiDecl
{
  friend class EiFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] head ヘッダ
  /// @param[in] pt_item パース木の宣言要素
  EiDeclR(ElbDeclHead* head,
	  const PtNamedBase* pt_item);

  /// @brief デストラクタ
  virtual
  ~EiDeclR();


public:
  //////////////////////////////////////////////////////////////////////
  // ElbDecl の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief スカラー値を返す．
  virtual
  VlScalarVal
  get_scalar() const;

  /// @brief スカラー値を設定する．
  /// @param[in] val 値
  virtual
  void
  set_scalar(const VlScalarVal& val);

  /// @brief 論理値を返す．
  virtual
  VlScalarVal
  get_logic() const;

  /// @brief real 型の値を返す．
  virtual
  double
  get_real() const;

  /// @brief real 型の値を設定する．
  /// @param[in] val 値
  virtual
  void
  set_real(double val);

  /// @brief bitvector 型の値を返す．
  /// @param[out] val 値
  /// @param[in] req_type 要求される型
  virtual
  void
  get_bitvector(BitVector& val,
		const VlValueType& req_type = VlValueType()) const;

  /// @brief bitvector 型の値を設定する．
  /// @param[in] val 値
  virtual
  void
  set_bitvector(const BitVector& val);

  /// @brief ビット選択値を返す．
  /// @param[in] index ビット位置
  virtual
  VlScalarVal
  get_bitselect(int index) const;

  /// @brief ビット値を設定する．
  /// @param[in] index ビット位置
  /// @param[in] val 値
  virtual
  void
  set_bitselect(int index,
		const VlScalarVal& val);

  /// @brief 範囲選択値を返す．
  /// @param[in] left 範囲の MSB
  /// @param[in] right 範囲の LSB
  /// @param[out] val 値
  virtual
  void
  get_partselect(int left,
		 int right,
		 BitVector& val) const;

  /// @brief 範囲値を設定する．
  /// @param[in] left 範囲の MSB
  /// @param[in] right 範囲の LSB
  /// @param[in] val 値
  virtual
  void
  set_partselect(int left,
		 int right,
		 const BitVector& val);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値
  double mVal;

};


//////////////////////////////////////////////////////////////////////
/// @class EiDeclV EiDecl.h "EiDecl.h"
/// @brief ビットベクタ値を持つ EiDecl
//////////////////////////////////////////////////////////////////////
class EiDeclV :
  public EiDecl
{
  friend class EiFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] head ヘッダ
  /// @param[in] pt_item パース木の宣言要素
  EiDeclV(ElbDeclHead* head,
	  const PtNamedBase* pt_item);

  /// @brief デストラクタ
  virtual
  ~EiDeclV();


public:
  //////////////////////////////////////////////////////////////////////
  // ElbDecl の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief スカラー値を返す．
  virtual
  VlScalarVal
  get_scalar() const;

  /// @brief スカラー値を設定する．
  /// @param[in] val 値
  virtual
  void
  set_scalar(const VlScalarVal& val);

  /// @brief 論理値を返す．
  virtual
  VlScalarVal
  get_logic() const;

  /// @brief real 型の値を返す．
  virtual
  double
  get_real() const;

  /// @brief real 型の値を設定する．
  /// @param[in] val 値
  virtual
  void
  set_real(double val);

  /// @brief bitvector 型の値を返す．
  /// @param[out] val 値
  /// @param[in] req_type 要求される型
  virtual
  void
  get_bitvector(BitVector& val,
		const VlValueType& req_type = VlValueType()) const;

  /// @brief bitvector 型の値を設定する．
  /// @param[in] val 値
  virtual
  void
  set_bitvector(const BitVector& val);

  /// @brief ビット選択値を返す．
  /// @param[in] index ビット位置
  virtual
  VlScalarVal
  get_bitselect(int index) const;

  /// @brief ビット値を設定する．
  /// @param[in] index ビット位置
  /// @param[in] val 値
  virtual
  void
  set_bitselect(int index,
		const VlScalarVal& val);

  /// @brief 範囲選択値を返す．
  /// @param[in] left 範囲の MSB
  /// @param[in] right 範囲の LSB
  /// @param[out] val 値
  virtual
  void
  get_partselect(int left,
		 int right,
		 BitVector& val) const;

  /// @brief 範囲値を設定する．
  /// @param[in] left 範囲の MSB
  /// @param[in] right 範囲の LSB
  /// @param[in] val 値
  virtual
  void
  set_partselect(int left,
		 int right,
		 const BitVector& val);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値
  BitVector mVal;

};


//////////////////////////////////////////////////////////////////////
/// @class EiDeclIS EiDecl.h "EiDecl.h"
/// @brief 初期値を持つ EiDeclS
//////////////////////////////////////////////////////////////////////
class EiDeclIS :
  public EiDeclS
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] head ヘッダ
  /// @param[in] pt_item パース木の宣言要素
  /// @param[in] init 初期値
  EiDeclIS(ElbDeclHead* head,
	   const PtNamedBase* pt_item,
	   ElbExpr* init);

  /// @brief デストラクタ
  virtual
  ~EiDeclIS();


public:
  //////////////////////////////////////////////////////////////////////
  // VlDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期値の取得
  /// @retval 初期値
  /// @retval nullptr 設定がない場合
  virtual
  const VlExpr*
  init_value() const;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期値の設定
  /// @param[in] expr 初期値
  virtual
  void
  set_init(ElbExpr* expr);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 初期値
  ElbExpr* mInit;

};


//////////////////////////////////////////////////////////////////////
/// @class EiDeclIR EiDecl.h "EiDecl.h"
/// @brief 初期値を持つ EiDeclR
//////////////////////////////////////////////////////////////////////
class EiDeclIR :
  public EiDeclR
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] head ヘッダ
  /// @param[in] pt_item パース木の宣言要素
  /// @param[in] init 初期値
  EiDeclIR(ElbDeclHead* head,
	   const PtNamedBase* pt_item,
	   ElbExpr* init);

  /// @brief デストラクタ
  virtual
  ~EiDeclIR();


public:
  //////////////////////////////////////////////////////////////////////
  // VlDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期値の取得
  /// @retval 初期値
  /// @retval nullptr 設定がない場合
  virtual
  const VlExpr*
  init_value() const;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期値の設定
  /// @param[in] expr 初期値
  virtual
  void
  set_init(ElbExpr* expr);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 初期値
  ElbExpr* mInit;

};


//////////////////////////////////////////////////////////////////////
/// @class EiDeclIV EiDecl.h "EiDecl.h"
/// @brief 初期値を持つ EiDeclV
//////////////////////////////////////////////////////////////////////
class EiDeclIV :
  public EiDeclV
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] head ヘッダ
  /// @param[in] pt_item パース木の宣言要素
  /// @param[in] init 初期値
  EiDeclIV(ElbDeclHead* head,
	   const PtNamedBase* pt_item,
	   ElbExpr* init);

  /// @brief デストラクタ
  virtual
  ~EiDeclIV();


public:
  //////////////////////////////////////////////////////////////////////
  // VlDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期値の取得
  /// @retval 初期値
  /// @retval nullptr 設定がない場合
  virtual
  const VlExpr*
  init_value() const;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期値の設定
  /// @param[in] expr 初期値
  virtual
  void
  set_init(ElbExpr* expr);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 初期値
  ElbExpr* mInit;

};

END_NAMESPACE_YM_VERILOG

#endif // EIDECL_H
