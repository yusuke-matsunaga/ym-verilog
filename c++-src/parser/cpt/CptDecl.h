#ifndef CPTDECL_H
#define CPTDECL_H

/// @file CptDecl.h
/// @brief CptDecl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.

#include "ym/pt/PtDecl.h"
#include "ym/FileRegion.h"
#include "parser/PtiArray.h"
#include "parser/PtiDecl.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// 宣言要素のヘッダの基底クラス
//////////////////////////////////////////////////////////////////////
class CptDeclHead :
  public PtiDeclHead
{
protected:

  /// @brief コンストラクタ
  CptDeclHead(
    const FileRegion& file_region ///< [in] ファイル位置の情報
  );

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
  bool
  is_signed() const override;

  /// @brief 範囲のMSBの取得
  /// @retval 範囲のMSB 範囲を持つとき
  /// @retval nullptr 範囲を持たないとき
  const PtExpr*
  left_range() const override;

  /// @brief 範囲のLSBの取得
  /// @retval 範囲のLSB 範囲を持つとき
  /// @retval nullptr 範囲を持たないとき
  const PtExpr*
  right_range() const override;

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

  /// @brief strength の取得
  /// @retval strength
  /// @retval nullptr strength の指定なし
  const PtStrength*
  strength() const override;

  /// @brief delay の取得
  /// @retval delay
  /// @retval nullptr delay の指定なし
  const PtDelay*
  delay() const override;

  /// @brief 要素数の取得
  SizeType
  item_num() const override;

  /// @brief 要素の取得
  const PtDeclItem*
  item(
    SizeType pos ///< [in] 位置 ( 0 <= pos < item_num() )
  ) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // PtiDeclHead の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素リストの設定
  void
  set_elem(
    PtiDeclItemArray&& elem_array ///< [in] 要素の配列
  ) override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 要素の配列
  PtiDeclItemArray mItemArray;

};


//////////////////////////////////////////////////////////////////////
/// parameter 宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////
class CptParamH :
  public CptDeclHead
{
public:

  /// @brief コンストラクタ
  CptParamH(
    const FileRegion& file_region ///< [in] ファイル位置の情報
  );

  /// @brief デストラクタ
  ~CptParamH();


public:
  //////////////////////////////////////////////////////////////////////
  // PtDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスを識別するための型を返す．
  /// @return 宣言要素の型
  PtDeclType
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// localparam 宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////
class CptLocalParamH :
  public CptDeclHead
{
public:

  /// @brief コンストラクタ
  CptLocalParamH(
    const FileRegion& file_region ///< [in] ファイル位置の情報
  );

  /// @brief デストラクタ
  ~CptLocalParamH();


public:
  //////////////////////////////////////////////////////////////////////
  // PtDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスを識別するための型を返す．
  /// @return 宣言要素の型
  PtDeclType
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// 符号なし範囲つき(ビットベクタタイプ)の parameter ヘッダのクラス
//////////////////////////////////////////////////////////////////////
class CptParamHV :
  public CptParamH
{
public:

  /// @brief コンストラクタ
  CptParamHV(
    const FileRegion& file_region, ///< [in] ファイル位置の情報
    const PtExpr* left,            ///< [in] 範囲の左側の式
    const PtExpr* right            ///< [in] 範囲の右側の式
  );

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
public:

  /// @brief コンストラクタ
  CptParamHSV(
    const FileRegion& file_region, ///< [in] ファイル位置の情報
    const PtExpr* left,            ///< [in] 範囲の左側の式
    const PtExpr* right            ///< [in] 範囲の右側の式
  );

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
public:

  /// @brief コンストラクタ
  CptLocalParamHV(
    const FileRegion& file_region, ///< [in] ファイル位置の情報
    const PtExpr* left,		   ///< [in] 範囲の左側の式
    const PtExpr* right		   ///< [in] 範囲の右側の式
  );

  /// @brief デストラクタ
  ~CptLocalParamHV();


public:
  //////////////////////////////////////////////////////////////////////
  // PtDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスを識別するための型を返す．
  /// @return 宣言要素の型
  PtDeclType
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// 符号付き範囲つき(ビットベクタタイプ)の localparam ヘッダのクラス
//////////////////////////////////////////////////////////////////////
class CptLocalParamHSV :
  public CptParamHSV
{
public:

  /// @brief コンストラクタ
  CptLocalParamHSV(
    const FileRegion& file_region, ///< [in] ファイル位置の情報
    const PtExpr* left,		   ///< [in] 範囲の左側の式
    const PtExpr* right		   ///< [in] 範囲の右側の式
  );

  /// @brief デストラクタ
  ~CptLocalParamHSV();


public:
  //////////////////////////////////////////////////////////////////////
  // PtDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスを識別するための型を返す．
  /// @return 宣言要素の型
  PtDeclType
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// 組み込み型 parameter ヘッダのクラス
//////////////////////////////////////////////////////////////////////
class CptParamHT :
  public CptDeclHead
{
public:

  /// @brief コンストラクタ
  CptParamHT(
    const FileRegion& file_region, ///< [in] ファイル位置の情報
    VpiVarType var_type            ///< [in] データ型
  );

  /// @brief デストラクタ
  ~CptParamHT();


public:
  //////////////////////////////////////////////////////////////////////
  // PtDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスを識別するための型を返す．
  /// @return 宣言要素の型
  PtDeclType
  type() const override;

  /// @brief 符号の取得
  /// @retval true 符号つき
  /// @retval false 符号なし
  bool
  is_signed() const override;

  /// @brief データ型の取得
  /// @retval データ型 kParam, kLocalParam, kVar の場合
  /// @retval kVpiVarNone 上記以外
  VpiVarType
  data_type() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // データ型
  VpiVarType mVarType;

};


//////////////////////////////////////////////////////////////////////
/// 組み込み型 localparam ヘッダのクラス
//////////////////////////////////////////////////////////////////////
class CptLocalParamHT :
  public CptParamHT
{
public:

  /// @brief コンストラクタ
  CptLocalParamHT(
    const FileRegion& file_region, ///< [in] ファイル位置の情報
    VpiVarType var_type            ///< [in] データ型
  );

  /// @brief デストラクタ
  ~CptLocalParamHT();


public:
  //////////////////////////////////////////////////////////////////////
  // PtDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスを識別するための型を返す．
  /// @return 宣言要素の型
  PtDeclType
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// reg 宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////
class CptRegH :
  public CptDeclHead
{
public:

  /// @brief コンストラクタ
  CptRegH(
    const FileRegion& file_region ///< [in] ファイル位置の情報
  );

  /// @brief デストラクタ
  ~CptRegH();


public:
  //////////////////////////////////////////////////////////////////////
  // PtDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスを識別するための型を返す．
  /// @return 宣言要素の型
  PtDeclType
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// 1ビット符号つき？？？ の reg 宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////
class CptRegHS :
  public CptRegH
{
public:

  /// @brief コンストラクタ
  CptRegHS(
    const FileRegion& file_region ///< [in] ファイル位置の情報
  );

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
public:

  /// @brief コンストラクタ
  CptRegHV(
    const FileRegion& file_region, ///< [in] ファイル位置の情報
    const PtExpr* left,		   ///< [in] 範囲の左側の式
    const PtExpr* right		   ///< [in] 範囲の右側の式
  );

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
public:

  /// @brief コンストラクタ
  CptRegHSV(
    const FileRegion& file_region, ///< [in] ファイル位置の情報
    const PtExpr* left,		   ///< [in] 範囲の左側の式
    const PtExpr* right		   ///< [in] 範囲の右側の式
  );

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
public:

  /// @brief コンストラクタ
  CptVarH(
    const FileRegion& file_region, ///< [in] ファイル位置の情報
    VpiVarType var_type            ///< [in] データ型
  );

  /// @brief デストラクタ
  ~CptVarH();


public:
  //////////////////////////////////////////////////////////////////////
  // PtDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスを識別するための型を返す．
  /// @return 宣言要素の型
  PtDeclType
  type() const override;

  /// @brief 符号の取得
  /// @retval true 符号つき
  /// @retval false 符号なし
  bool
  is_signed() const override;

  /// @brief データ型の取得
  /// @retval データ型 kParam, kLocalParam, kVar の場合
  /// @retval kVpiVarNone 上記以外
  VpiVarType
  data_type() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // データ型
  VpiVarType
  mVarType;

};


//////////////////////////////////////////////////////////////////////
/// genvar宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////
class CptGenvarH :
  public CptDeclHead
{
public:

  /// @brief コンストラクタ
  CptGenvarH(
    const FileRegion& file_region ///< [in] ファイル位置の情報
  );

  /// @brief デストラクタ
  ~CptGenvarH();


public:
  //////////////////////////////////////////////////////////////////////
  // PtDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスを識別するための型を返す．
  /// @return 宣言要素の型
  PtDeclType
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// net宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////
class CptNetH :
  public CptDeclHead
{
public:

  /// @brief コンストラクタ
  CptNetH(
    const FileRegion& file_region, ///< [in] ファイル位置の情報
    VpiNetType net_type,           ///< [in] net の型
    bool sign                      ///< [in] 符号付きのとき true となるフラグ
  );

  /// @brief デストラクタ
  ~CptNetH();


public:
  //////////////////////////////////////////////////////////////////////
  // PtDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスを識別するための型を返す．
  /// @return 宣言要素の型
  PtDeclType
  type() const override;

  /// @brief net type を返す．(vpiWire など)
  /// @return net 型
  VpiNetType
  net_type() const override;

  /// @brief 符号の有無の取得
  bool
  is_signed() const override;


protected:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // net type + sign (+ vstype)
  std::uint32_t mFlags;

};


//////////////////////////////////////////////////////////////////////
/// strength つきの net宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////
class CptNetHS :
  public CptNetH
{
public:

  /// @brief コンストラクタ
  CptNetHS(
    const FileRegion& file_region, ///< [in] ファイル位置の情報
    VpiNetType net_type,	   ///< [in] net の型
    bool sign,			   ///< [in] 符号付きのとき true となるフラグ
    const PtStrength* strength     ///< [in] 信号強度
  );

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
public:

  /// @brief コンストラクタ
  CptNetHD(
    const FileRegion& file_region, ///< [in] ファイル位置の情報
    VpiNetType net_type,	   ///< [in] net の型
    bool sign,			   ///< [in] 符号付きのとき true となるフラグ
    const PtDelay* delay           ///< [in] 遅延
  );

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
public:

  /// @brief コンストラクタ
  CptNetHSD(
    const FileRegion& file_region, ///< [in] ファイル位置の情報
    VpiNetType net_type,	   ///< [in] net の型
    bool sign,			   ///< [in] 符号付きのとき true となるフラグ
    const PtStrength* strength,    ///< [in] 信号強度
    const PtDelay* delay           ///< [in] 遅延
  );

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
public:

  /// @brief コンストラクタ
  CptNetHV(
    const FileRegion& file_region, ///< [in] ファイル位置の情報
    VpiNetType net_type,	   ///< [in] net の型
    VpiVsType vstype,		   ///< [in] vectored/scalared 属性を表す値
    bool sign,                     ///< [in] 符号付きのとき true となるフラグ
    const PtExpr* left,            ///< [in] 範囲の左側の式
    const PtExpr* right            ///< [in] 範囲の右側の式
  );

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
  VpiVsType
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
public:

  /// @brief コンストラクタ
  CptNetHVS(
    const FileRegion& file_region, ///< [in] ファイル位置の情報
    VpiNetType net_type,	   ///< [in] net の型
    VpiVsType vstype,		   ///< [in] vectored/scalared 属性を表す値
    bool sign,			   ///< [in] 符号付きのとき true となるフラグ
    const PtExpr* left,		   ///< [in] 範囲の左側の式
    const PtExpr* right,	   ///< [in] 範囲の右側の式
    const PtStrength* strength     ///< [in] 信号強度
  );

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
public:

  /// @brief コンストラクタ
  CptNetHVD(
    const FileRegion& file_region, ///< [in] ファイル位置の情報
    VpiNetType net_type,	   ///< [in] net の型
    VpiVsType vstype,		   ///< [in] vectored/scalared 属性を表す値
    bool sign,			   ///< [in] 符号付きのとき true となるフラグ
    const PtExpr* left,		   ///< [in] 範囲の左側の式
    const PtExpr* right,	   ///< [in] 範囲の右側の式
    const PtDelay* delay           ///< [in] 遅延
  );

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
public:

  /// @brief コンストラクタ
  CptNetHVSD(
    const FileRegion& file_region, ///< [in] ファイル位置の情報
    VpiNetType net_type,	   ///< [in] net の型
    VpiVsType vstype,		   ///< [in] vectored/scalared 属性を表す値
    bool sign,			   ///< [in] 符号付きのとき true となるフラグ
    const PtExpr* left,		   ///< [in] 範囲の左側の式
    const PtExpr* right,	   ///< [in] 範囲の右側の式
    const PtStrength* strength,    ///< [in] 信号強度
    const PtDelay* delay           ///< [in] 遅延
  );

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
public:

  /// @brief コンストラクタ
  CptEventH(
    const FileRegion& file_region ///< [in] ファイル位置の情報
  );

  /// @brief デストラクタ
  ~CptEventH();


public:
  //////////////////////////////////////////////////////////////////////
  // PtDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスを識別するための型を返す．
  /// @return 宣言要素の型
  PtDeclType
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// specparam 宣言のヘッダのベース実装クラス
//////////////////////////////////////////////////////////////////////
class CptSpecParamH :
  public CptDeclHead
{
public:

  /// @brief コンストラクタ
  CptSpecParamH(
    const FileRegion& file_region ///< [in] ファイル位置の情報
  );

  /// @brief デストラクタ
  ~CptSpecParamH();


public:
  //////////////////////////////////////////////////////////////////////
  // PtDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスを識別するための型を返す．
  /// @return 宣言要素の型
  PtDeclType
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// ビットベクタ型の specparam
//////////////////////////////////////////////////////////////////////
class CptSpecParamHV :
  public CptSpecParamH
{
public:

  /// @brief コンストラクタ
  CptSpecParamHV(
    const FileRegion& file_region, ///< [in] ファイル位置の情報
    const PtExpr* left,		   ///< [in] 範囲の左側の式
    const PtExpr* right		   ///< [in] 範囲の右側の式
  );

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
public:

  /// @brief コンストラクタ
  CptDeclItemBase(
    const char* name ///< [in] 名前
  );

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

  /// @brief 範囲リストのサイズの取得
  SizeType
  range_num() const override;

  /// @brief 範囲の取得
  const PtRange*
  range(
    SizeType pos ///< [in] 位置 ( 0 <= pos < range_num() )
  ) const override;

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
public:

  /// @brief コンストラクタ
  CptDeclItem(
    const FileRegion& file_region, ///< [in] ファイル位置の情報
    const char* name               ///< [in] 名前
  );

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
public:

  /// @brief コンストラクタ
  CptDeclItemR(
    const FileRegion& file_region, ///< [in] ファイル位置の情報
    const char* name,              ///< [in] 名前
    PtiRangeArray&& range_array    ///< [in] 範囲のリスト
  );

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

  /// @brief dimension list のサイズの取得
  SizeType
  range_num() const override;

  /// @brief 範囲の取得
  const PtRange*
  range(
    SizeType pos ///< [in] 位置 ( 0 <= pos < range_num() )
  ) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 範囲の配列
  PtiRangeArray mRangeArray;

};


//////////////////////////////////////////////////////////////////////
/// 初期値をもった宣言要素のクラス
//////////////////////////////////////////////////////////////////////
class CptDeclItemI :
  public CptDeclItem
{
public:

  /// @brief コンストラクタ
  CptDeclItemI(
    const FileRegion& file_region, ///< [in] ファイル位置の情報
    const char* name,              ///< [in] 名前
    const PtExpr* init_value       ///< [in] 初期値
  );

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
  CptRange(
    const FileRegion& fr, ///< [in] ファイル上の位置
    const PtExpr* msb,    ///< [in] 範囲の MSB
    const PtExpr* lsb     ///< [in] 範囲の LSB
  );

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
