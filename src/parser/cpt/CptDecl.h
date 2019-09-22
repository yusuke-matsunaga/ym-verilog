#ifndef CPTDECL_H
#define CPTDECL_H

/// @file CptDecl.h
/// @brief CptDecl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym/pt/PtDecl.h"
#include "ym/pt/PtArray.h"

#include "ym/FileRegion.h"

#include "PtiDecl.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// 宣言要素のヘッダの基底クラス
//////////////////////////////////////////////////////////////////////
class CptDeclHead :
  public PtiDeclHead
{
protected:

  /// @brief コンストラクタ
  /// @param[in] file_region ファイル位置の情報
  CptDeclHead(const FileRegion& file_region);

  /// デストラクタ
  ~CptDeclHead();


public:
  //////////////////////////////////////////////////////////////////////
  // PtDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置の取得
  /// @return ファイル位置
  FileRegion
  file_region() const override;

  /// @brief 符号の取得
  /// @retval true 符号つき
  /// @retval false 符号なし
  /// @note このクラスでは false を返す．
  bool
  is_signed() const override;

  /// @brief 範囲のMSBの取得
  /// @retval 範囲のMSB 範囲を持つとき
  /// @retval nullptr 範囲を持たないとき
  /// @note このクラスでは nullptr を返す．
  const PtExpr*
  left_range() const override;

  /// @brief 範囲のLSBの取得
  /// @retval 範囲のLSB 範囲を持つとき
  /// @retval nullptr 範囲を持たないとき
  /// @note このクラスでは nullptr を返す．
  const PtExpr*
  right_range() const override;

  /// @brief データ型の取得
  /// @retval データ型 kParam, kLocalParam, kVar の場合
  /// @retval kVpiVarNone 上記以外
  /// @note このクラスでは kVpiVarNone を返す．
  tVpiVarType
  data_type() const override;

  /// @brief net 型の取得
  /// @retval net 型 net 型の要素の場合
  /// @retval kVpiNone net 型の要素でない場合
  /// @note このクラスでは kVpiNone を返す．
  tVpiNetType
  net_type() const override;

  /// @brief vectored|scalared 属性の取得
  /// @retval kVpiVsNone vectored|scalared 指定なし
  /// @retval kVpiVectored vectored 指定あり
  /// @retval kVpiScalared scalared 指定あり
  /// @note このクラスでは kVpiVsNone を返す．
  tVpiVsType
  vs_type() const override;

  /// @brief strength の取得
  /// @retval strength
  /// @retval nullptr strength の指定なし
  /// @note このクラスでは nullptr を返す．
  const PtStrength*
  strength() const override;

  /// @brief delay の取得
  /// @retval delay
  /// @retval nullptr delay の指定なし
  /// @note このクラスでは nullptr を返す．
  const PtDelay*
  delay() const override;

  /// @brief 要素のリストの取得
  PtDeclItemArray
  item_list() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // PtiDeclHead の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素リストの設定
  /// @param[in] elem_array 要素リスト(配列)
  void
  set_elem(PtDeclItemArray elem_array) override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 要素の配列
  PtDeclItemArray mItemArray;

};


//////////////////////////////////////////////////////////////////////
/// parameter 宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////
class CptParamH :
  public CptDeclHead
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] file_region ファイル位置の情報
  CptParamH(const FileRegion& file_region);

  /// @brief デストラクタ
  ~CptParamH();


public:
  //////////////////////////////////////////////////////////////////////
  // PtDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスを識別するための型を返す．
  /// @return 宣言要素の型
  tPtDeclType
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// localparam 宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////
class CptLocalParamH :
  public CptDeclHead
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] file_region ファイル位置の情報
  CptLocalParamH(const FileRegion& file_region);

  /// @brief デストラクタ
  ~CptLocalParamH();


public:
  //////////////////////////////////////////////////////////////////////
  // PtDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスを識別するための型を返す．
  /// @return 宣言要素の型
  tPtDeclType
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// 符号なし範囲つき(ビットベクタタイプ)の parameter ヘッダのクラス
//////////////////////////////////////////////////////////////////////
class CptParamHV :
  public CptParamH
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  CptParamHV(const FileRegion& file_region,
	     const PtExpr* left,
	     const PtExpr* right);

  /// @brief デストラクタ
  ~CptParamHV();


public:
  //////////////////////////////////////////////////////////////////////
  // PtDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 符号の取得
  /// @retval true 符号つき
  /// @retval false 符号なし
  bool
  is_signed() const override;

  /// @brief 範囲のMSBの取得
  /// @return 範囲のMSB
  const PtExpr*
  left_range() const override;

  /// @brief 範囲のLSBの取得
  /// @return 範囲のLSB
  const PtExpr*
  right_range() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 範囲のMSB
  const PtExpr* mLeftRange;

  // 範囲のLSB
  const PtExpr* mRightRange;

};


//////////////////////////////////////////////////////////////////////
/// 符号付き範囲つき(ビットベクタタイプ)の parameter ヘッダのクラス
//////////////////////////////////////////////////////////////////////
class CptParamHSV :
  public CptParamHV
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  CptParamHSV(const FileRegion& file_region,
	      const PtExpr* left,
	      const PtExpr* right);

  /// @brief デストラクタ
  ~CptParamHSV();


public:
  //////////////////////////////////////////////////////////////////////
  // PtDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 符号の取得
  /// @retval true 符号つき
  /// @retval false 符号なし
  bool
  is_signed() const override;

};


//////////////////////////////////////////////////////////////////////
/// 符号なし範囲つき(ビットベクタタイプ)の localparam ヘッダのクラス
//////////////////////////////////////////////////////////////////////
class CptLocalParamHV :
  public CptParamHV
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  CptLocalParamHV(const FileRegion& file_region,
		  const PtExpr* left,
		  const PtExpr* right);

  /// @brief デストラクタ
  ~CptLocalParamHV();


public:
  //////////////////////////////////////////////////////////////////////
  // PtDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスを識別するための型を返す．
  /// @return 宣言要素の型
  tPtDeclType
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// 符号付き範囲つき(ビットベクタタイプ)の localparam ヘッダのクラス
//////////////////////////////////////////////////////////////////////
class CptLocalParamHSV :
  public CptParamHSV
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  CptLocalParamHSV(const FileRegion& file_region,
		   const PtExpr* left,
		   const PtExpr* right);

  /// @brief デストラクタ
  ~CptLocalParamHSV();


public:
  //////////////////////////////////////////////////////////////////////
  // PtDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスを識別するための型を返す．
  /// @return 宣言要素の型
  tPtDeclType
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// 組み込み型 parameter ヘッダのクラス
//////////////////////////////////////////////////////////////////////
class CptParamHT :
  public CptDeclHead
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] var_type データ型
  CptParamHT(const FileRegion& file_region,
	     tVpiVarType var_type);

  /// @brief デストラクタ
  ~CptParamHT();


public:
  //////////////////////////////////////////////////////////////////////
  // PtDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスを識別するための型を返す．
  /// @return 宣言要素の型
  tPtDeclType
  type() const override;

  /// @brief 符号の取得
  /// @retval true 符号つき
  /// @retval false 符号なし
  bool
  is_signed() const override;

  /// @brief データ型の取得
  /// @retval データ型 kParam, kLocalParam, kVar の場合
  /// @retval kVpiVarNone 上記以外
  tVpiVarType
  data_type() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // データ型
  tVpiVarType mVarType;

};


//////////////////////////////////////////////////////////////////////
/// 組み込み型 localparam ヘッダのクラス
//////////////////////////////////////////////////////////////////////
class CptLocalParamHT :
  public CptParamHT
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] var_type データ型
  CptLocalParamHT(const FileRegion& file_region,
		  tVpiVarType var_type);

  /// @brief デストラクタ
  ~CptLocalParamHT();


public:
  //////////////////////////////////////////////////////////////////////
  // PtDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスを識別するための型を返す．
  /// @return 宣言要素の型
  tPtDeclType
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// reg 宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////
class CptRegH :
  public CptDeclHead
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] file_region ファイル位置の情報
  CptRegH(const FileRegion& file_region);

  /// @brief デストラクタ
  ~CptRegH();


public:
  //////////////////////////////////////////////////////////////////////
  // PtDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスを識別するための型を返す．
  /// @return 宣言要素の型
  tPtDeclType
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// 1ビット符号つき？？？ の reg 宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////
class CptRegHS :
  public CptRegH
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] file_region ファイル位置の情報
  CptRegHS(const FileRegion& file_region);

  /// @brief デストラクタ
  ~CptRegHS();


public:
  //////////////////////////////////////////////////////////////////////
  // PtDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 符号の取得
  /// @return このクラスでは常に true を返す．
  bool
  is_signed() const override;

};


//////////////////////////////////////////////////////////////////////
/// ビットベクタ型の reg 宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////
class CptRegHV :
  public CptRegH
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  CptRegHV(const FileRegion& file_region,
	   const PtExpr* left,
	   const PtExpr* right);

  /// @brief デストラクタ
  ~CptRegHV();


public:
  //////////////////////////////////////////////////////////////////////
  // PtDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 符号の有無の取得
  /// @return このクラスでは常に false を返す．
  bool
  is_signed() const override;

  /// @brief 範囲のMSBの取得
  /// @return 範囲のMSB
  const PtExpr*
  left_range() const override;

  /// @brief 範囲のLSBの取得
  /// @return 範囲のLSB
  const PtExpr*
  right_range() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 範囲のMSB
  const PtExpr* mLeftRange;

  // 範囲のLSB
  const PtExpr* mRightRange;

};


//////////////////////////////////////////////////////////////////////
/// 符号つきビットベクタ型の reg 宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////
class CptRegHSV :
  public CptRegHV
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  CptRegHSV(const FileRegion& file_region,
	    const PtExpr* left,
	    const PtExpr* right);

  /// @brief デストラクタ
  ~CptRegHSV();


public:
  //////////////////////////////////////////////////////////////////////
  // PtDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 符号の有無の取得
  /// @return このクラスでは常に true を返す．
  bool
  is_signed() const override;

};


//////////////////////////////////////////////////////////////////////
/// 変数宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////
class CptVarH :
  public CptDeclHead
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] var_type データ型
  CptVarH(const FileRegion& file_region,
	  tVpiVarType var_type);

  /// @brief デストラクタ
  ~CptVarH();


public:
  //////////////////////////////////////////////////////////////////////
  // PtDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスを識別するための型を返す．
  /// @return 宣言要素の型
  tPtDeclType
  type() const override;

  /// @brief 符号の取得
  /// @retval true 符号つき
  /// @retval false 符号なし
  bool
  is_signed() const override;

  /// @brief データ型の取得
  /// @retval データ型 kParam, kLocalParam, kVar の場合
  /// @retval kVpiVarNone 上記以外
  tVpiVarType
  data_type() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // データ型
  tVpiVarType
  mVarType;

};


//////////////////////////////////////////////////////////////////////
/// genvar宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////
class CptGenvarH :
  public CptDeclHead
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] elem_top 要素のリスト
  CptGenvarH(const FileRegion& file_region);

  /// @brief デストラクタ
  ~CptGenvarH();


public:
  //////////////////////////////////////////////////////////////////////
  // PtDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスを識別するための型を返す．
  /// @return 宣言要素の型
  tPtDeclType
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// net宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////
class CptNetH :
  public CptDeclHead
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] net_type net の型
  /// @param[in] sign 符号付きのとき true となるフラグ
  CptNetH(const FileRegion& file_region,
	  tVpiNetType net_type,
	  bool sign);

  /// @brief デストラクタ
  ~CptNetH();


public:
  //////////////////////////////////////////////////////////////////////
  // PtDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスを識別するための型を返す．
  /// @return 宣言要素の型
  tPtDeclType
  type() const override;

  /// @brief net type を返す．(vpiWire など)
  /// @return net 型
  tVpiNetType
  net_type() const override;

  /// @brief 符号の有無の取得
  bool
  is_signed() const override;


protected:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // net type + sign (+ vstype)
  ymuint32 mFlags;

};


//////////////////////////////////////////////////////////////////////
/// strength つきの net宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////
class CptNetHS :
  public CptNetH
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] net_type net の型
  /// @param[in] sign 符号付きのとき true となるフラグ
  /// @param[in] strength 信号強度
  CptNetHS(const FileRegion& file_region,
	   tVpiNetType net_type,
	   bool sign,
	   const PtStrength* strength);

  /// @brief デストラクタ
  ~CptNetHS();


public:
  //////////////////////////////////////////////////////////////////////
  // PtDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief strength を返す．
  /// @return strength
  const PtStrength*
  strength() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  /// strength
  const PtStrength* mStrength;

};


//////////////////////////////////////////////////////////////////////
/// delay つきの net宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////
class CptNetHD :
  public CptNetH
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] net_type net の型
  /// @param[in] sign 符号付きのとき true となるフラグ
  /// @param[in] delay 遅延
  CptNetHD(const FileRegion& file_region,
	   tVpiNetType net_type,
	   bool sign,
	   const PtDelay* delay);

  /// @brief デストラクタ
  ~CptNetHD();


public:
  //////////////////////////////////////////////////////////////////////
  // PtDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief delay を返す．
  /// @return delay
  const PtDelay*
  delay() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // delay
  const PtDelay* mDelay;

};


//////////////////////////////////////////////////////////////////////
/// strength と delay つきの net宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////
class CptNetHSD :
  public CptNetH
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] net_type net の型
  /// @param[in] strength 信号強度
  /// @param[in] sign 符号付きのとき true となるフラグ
  /// @param[in] delay 遅延
  CptNetHSD(const FileRegion& file_region,
	    tVpiNetType net_type,
	    bool sign,
	    const PtStrength* strength,
	    const PtDelay* delay);

  /// @brief デストラクタ
  ~CptNetHSD();


public:
  //////////////////////////////////////////////////////////////////////
  // PtDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief strength を返す．
  /// @return strength
  const PtStrength*
  strength() const override;

  /// @brief delay を返す．
  /// @return delay
  const PtDelay*
  delay() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // strength
  const PtStrength* mStrength;

  // delay
  const PtDelay* mDelay;

};


//////////////////////////////////////////////////////////////////////
/// ビットベクタ型の net宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////
class CptNetHV :
  public CptNetH
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] net_type net の型
  /// @param[in] vstype vectored/scalared 属性を表す値
  /// @param[in] sign 符号の有無を表すフラグ
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  CptNetHV(const FileRegion& file_region,
	   tVpiNetType net_type,
	   tVpiVsType vstype,
	   bool sign,
	   const PtExpr* left,
	   const PtExpr* right);

  /// @brief デストラクタ
  ~CptNetHV();


public:
  //////////////////////////////////////////////////////////////////////
  // PtDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief vectored/scalared 属性を返す．
  /// @retval kVpiVsNone vectored|scalared 指定なし
  /// @retval kVpiVectored vectored 指定あり
  /// @retval kVpiScalared scalared 指定あり
  tVpiVsType
  vs_type() const override;

  /// @brief 範囲のMSBを取り出す．
  /// @return 範囲のMSB
  const PtExpr*
  left_range() const override;

  /// @brief 範囲のLSBを取り出す．
  /// @return 範囲のLSB
  const PtExpr*
  right_range() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 範囲のMSB
  const PtExpr* mLeftRange;

  // 範囲のLSB
  const PtExpr* mRightRange;

};


//////////////////////////////////////////////////////////////////////
/// strength つきの PtNetHV
//////////////////////////////////////////////////////////////////////
class CptNetHVS :
  public CptNetHV
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] net_type net の型
  /// @param[in] vstype vectored/scalared 属性を表す値
  /// @param[in] sign 符号の有無を表すフラグ
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @param[in] strength 信号強度
  CptNetHVS(const FileRegion& file_region,
	    tVpiNetType net_type,
	    tVpiVsType vstype,
	    bool sign,
	    const PtExpr* left,
	    const PtExpr* right,
	    const PtStrength* strength);

  /// @brief デストラクタ
  ~CptNetHVS();


public:
  //////////////////////////////////////////////////////////////////////
  // PtDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief strength を返す．
  /// @return strength
  const PtStrength*
  strength() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // strength
  const PtStrength* mStrength;

};


//////////////////////////////////////////////////////////////////////
/// delay つきの PtNetHV
//////////////////////////////////////////////////////////////////////
class CptNetHVD :
  public CptNetHV
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] net_type net の型
  /// @param[in] vstype vectored/scalared 属性を表す値
  /// @param[in] sign 符号の有無を表すフラグ
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @param[in] delay 遅延
  CptNetHVD(const FileRegion& file_region,
	    tVpiNetType net_type,
	    tVpiVsType vstype,
	    bool sign,
	    const PtExpr* left,
	    const PtExpr* right,
	    const PtDelay* delay);

  /// @brief デストラクタ
  ~CptNetHVD();


public:
  //////////////////////////////////////////////////////////////////////
  // PtDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief delay を返す．
  /// @return delay
  const PtDelay*
  delay() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // delay
  const PtDelay* mDelay;

};


//////////////////////////////////////////////////////////////////////
/// strength と delay つきの PtNetHV
//////////////////////////////////////////////////////////////////////
class CptNetHVSD :
  public CptNetHV
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] net_type net の型
  /// @param[in] vstype vectored/scalared 属性を表す値
  /// @param[in] sign 符号の有無を表すフラグ
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @param[in] strength 信号強度
  /// @param[in] delay 遅延
  CptNetHVSD(const FileRegion& file_region,
	     tVpiNetType net_type,
	     tVpiVsType vstype,
	     bool sign,
	     const PtExpr* left,
	     const PtExpr* right,
	     const PtStrength* strength,
	     const PtDelay* delay);

  /// @brief デストラクタ
  ~CptNetHVSD();


public:
  //////////////////////////////////////////////////////////////////////
  // PtDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief strength を返す．
  /// @return strength
  const PtStrength*
  strength() const override;

  /// @brief delay を返す．
  /// @return delay
  const PtDelay*
  delay() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // strength
  const PtStrength* mStrength;

  // delay
  const PtDelay* mDelay;

};


//////////////////////////////////////////////////////////////////////
/// event 宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////
class CptEventH :
  public CptDeclHead
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] file_region ファイル位置の情報
  CptEventH(const FileRegion& file_region);

  /// @brief デストラクタ
  ~CptEventH();


public:
  //////////////////////////////////////////////////////////////////////
  // PtDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスを識別するための型を返す．
  /// @return 宣言要素の型
  tPtDeclType
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// specparam 宣言のヘッダのベース実装クラス
//////////////////////////////////////////////////////////////////////
class CptSpecParamH :
  public CptDeclHead
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] file_region ファイル位置の情報
  CptSpecParamH(const FileRegion& file_region);

  /// @brief デストラクタ
  ~CptSpecParamH();


public:
  //////////////////////////////////////////////////////////////////////
  // PtDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスを識別するための型を返す．
  /// @return 宣言要素の型
  tPtDeclType
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// ビットベクタ型の specparam
//////////////////////////////////////////////////////////////////////
class CptSpecParamHV :
  public CptSpecParamH
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  CptSpecParamHV(const FileRegion& file_region,
		 const PtExpr* left,
		 const PtExpr* right);

  /// @brief デストラクタ
  ~CptSpecParamHV();


public:
  //////////////////////////////////////////////////////////////////////
  // PtDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 範囲の MSB を返す．
  /// @return 範囲のMSB
  const PtExpr*
  left_range() const override;

  /// @brief 範囲の LSB を返す．
  /// @return 範囲のLSB
  const PtExpr*
  right_range() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 範囲のMSB
  const PtExpr* mLeftRange;

  // 範囲のLSB
  const PtExpr* mRightRange;

};


//////////////////////////////////////////////////////////////////////
/// 宣言要素のベースクラス
//////////////////////////////////////////////////////////////////////
class CptDeclItemBase :
  public PtDeclItem
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] name 名前
  CptDeclItemBase(const char* name);

  /// @brief デストラクタ
  ~CptDeclItemBase();


public:
  //////////////////////////////////////////////////////////////////////
  // PtDeclItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を取り出す．
  /// @return 名前
  const char*
  name() const override;

  /// @brief dimension list のサイズを取り出す．
  /// @return ここでは常に 0 を返す．
  int
  dimension_list_size() const override;

  /// 範囲のリストの取得
  PtRangeArray
  range_list() const override;

  /// @brief 初期値を取り出す．
  /// @return ここでは常に nullptr を返す．
  const PtExpr*
  init_value() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  const char* mName;

};


//////////////////////////////////////////////////////////////////////
/// 宣言要素のクラス
//////////////////////////////////////////////////////////////////////
class CptDeclItem :
  public CptDeclItemBase
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] name 名前
  CptDeclItem(const FileRegion& file_region,
	      const char* name);

  /// @brief デストラクタ
  ~CptDeclItem();


public:
  //////////////////////////////////////////////////////////////////////
  // PtDeclItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  /// @return ファイル位置
  FileRegion
  file_region() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // トークンの位置
  FileRegion mLoc;

};


//////////////////////////////////////////////////////////////////////
/// 範囲を持った宣言要素のクラス
//////////////////////////////////////////////////////////////////////
class CptDeclItemR :
  public CptDeclItemBase
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] name 名前
  /// @param[in] range_array 範囲のリスト
  CptDeclItemR(const FileRegion& file_region,
	       const char* name,
	       PtRangeArray range_array);

  /// @brief デストラクタ
  ~CptDeclItemR();


public:
  //////////////////////////////////////////////////////////////////////
  // PtDeclItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  /// @return ファイル位置
  FileRegion
  file_region() const override;

  /// @brief dimension list のサイズを取り出す．
  /// @return dimension list のサイズ
  int
  dimension_list_size() const override;

  /// 範囲のリストの取得
  PtRangeArray
  range_list() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 範囲の配列
  PtRangeArray mRangeArray;

};


//////////////////////////////////////////////////////////////////////
/// 初期値をもった宣言要素のクラス
//////////////////////////////////////////////////////////////////////
class CptDeclItemI :
  public CptDeclItem
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] name 名前
  /// @param[in] init_value 初期値
  CptDeclItemI(const FileRegion& file_region,
	       const char* name,
	       const PtExpr* init_value);

  /// @brief デストラクタ
  ~CptDeclItemI();


public:
  //////////////////////////////////////////////////////////////////////
  // PtDeclItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  /// @return ファイル位置
  FileRegion
  file_region() const override;

  /// @brief 初期値を取り出す．
  /// @retval 初期値
  /// @retval nullptr 設定がない場合
  const PtExpr*
  init_value() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 初期値
  const PtExpr* mInitValue;

};


//////////////////////////////////////////////////////////////////////
/// @class CptRange CptDecl.h "CptDecl.h"
/// @brief 範囲を表すクラス
//////////////////////////////////////////////////////////////////////
class CptRange :
  public PtRange
{
public:

  /// @brief コンストラクタ
  CptRange(const FileRegion& fr,
	   const PtExpr* msb,
	   const PtExpr* lsb);

  /// @brief デストラクタ
  ~CptRange();


public:
  //////////////////////////////////////////////////////////////////////
  // PtRange の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// ファイル位置の取得
  FileRegion
  file_region() const override;

  /// @brief 範囲の MSB を取り出す．
  const PtExpr*
  left() const override;

  /// @brief 範囲の LSB を取り出す．
  const PtExpr*
  right() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // MSB
  const PtExpr* mMsb;

  // LSB
  const PtExpr* mLsb;

};

END_NAMESPACE_YM_VERILOG

#endif // CPTDECL_H
