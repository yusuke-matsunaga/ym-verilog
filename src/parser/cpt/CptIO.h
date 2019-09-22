#ifndef CPTIO_H
#define CPTIO_H

/// @file CptIO.h
/// @brief CptIO のヘッダファイル
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
/// @brief IO宣言のヘッダのベース実装クラス
//////////////////////////////////////////////////////////////////////
class CptIOHBase :
  public PtiIOHead
{
protected:

  /// @brief コンストラクタ
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] type IOの種類
  /// @param[in] aux_type 補助的な型
  /// @param[in] net_type 補助的なネット型
  /// @param[in] var_type 補助的な変数型
  /// @param[in] sign 符号つきの時 true にするフラグ
  CptIOHBase(const FileRegion& file_region,
	     tPtIOType type,
	     tVpiAuxType aux_type,
	     tVpiNetType net_type,
	     tVpiVarType var_type,
	     bool sign);

  /// @brief デストラクタ
  ~CptIOHBase();


public:
  //////////////////////////////////////////////////////////////////////
  // PtIOH の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置の取得
  FileRegion
  file_region() const override;

  /// @brief 型の取得
  tPtIOType
  type() const override;

  /// @brief 補助的な型の取得
  tVpiAuxType
  aux_type() const override;

  /// @brief 補助的なネット型の取得
  tVpiNetType
  net_type() const override;

  /// @brief 補助的な変数型の取得
  tVpiVarType
  var_type() const override;

  /// @brief 符号の取得
  /// @retval true 符号付き
  /// @retval false 符号なし
  bool
  is_signed() const override;

  /// @brief 範囲のMSBの取得
  /// @retval 範囲のMSB
  /// @retval nullptr 範囲を持たないとき
  /// @note このクラスでは nullptr を返す．
  const PtExpr*
  left_range() const override;

  /// @brief 範囲のLSBの取得
  /// @retval 範囲のLSB
  /// @retval nullptr 範囲を持たないとき
  /// @note このクラスでは nullptr を返す．
  const PtExpr*
  right_range() const override;

  /// @brief 要素のリストの取得
  PtIOItemArray
  item_list() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // PtiIOHead の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素リストの設定
  /// @param[in] elem_array 要素リスト
  void
  set_elem(PtIOItemArray elem_array) override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // type と aux_type と符号を格納するメンバ
  ymuint32 mAttr;

  // 要素の配列
  PtIOItemArray mItemArray;

};


//////////////////////////////////////////////////////////////////////
/// @brief 範囲を持たない IO宣言のヘッダ
//////////////////////////////////////////////////////////////////////
class CptIOH :
  public CptIOHBase
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] type IOの種類
  /// @param[in] aux_type 補助的な型
  /// @param[in] net_type 補助的なネット型
  /// @param[in] var_type 補助的な変数型
  /// @param[in] sign 符号の有無
  CptIOH(const FileRegion& file_region,
	 tPtIOType type,
	 tVpiAuxType aux_type,
	 tVpiNetType net_type,
	 tVpiVarType var_type,
	 bool sign);

  /// @brief デストラクタ
  ~CptIOH();

};


//////////////////////////////////////////////////////////////////////
/// @brief ビットベクタ型の IO宣言のヘッダ
//////////////////////////////////////////////////////////////////////
class CptIOHV :
  public CptIOHBase
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] type IOの種類
  /// @param[in] aux_type 補助的な型
  /// @param[in] net_type 補助的なネット型
  /// @param[in] sign 符号つきの時に true とするフラグ
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  CptIOHV(const FileRegion& file_region,
	  tPtIOType type,
	  tVpiAuxType aux_type,
	  tVpiNetType net_type,
	  bool sign,
	  const PtExpr* left,
	  const PtExpr* right);

  /// @brief デストラクタ
  ~CptIOHV();


public:
  //////////////////////////////////////////////////////////////////////
  // PtIOH の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 範囲のMSBの取得
  const PtExpr*
  left_range() const override;

  /// @brief 範囲のLSBの取得
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
/// @brief IO宣言要素のベース実装クラス
//////////////////////////////////////////////////////////////////////
class CptIOItem :
  public PtIOItem
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] name 名前
  CptIOItem(const FileRegion& file_region,
	    const char* name);

  /// @brief デストラクタ
  ~CptIOItem();


public:
  //////////////////////////////////////////////////////////////////////
  // PtIOItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置の取得
  FileRegion
  file_region() const override;

  /// @brief 名前の取得
  const char*
  name() const override;

  /// @brief 初期値の取得
  /// @retval 初期値
  /// @retval nullptr 初期値を持たないとき
  /// @note このクラスでは nullptr を返す．
  const PtExpr*
  init_value() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // トークンの位置
  FileRegion mLoc;

  // 名前
  const char* mName;

};


//////////////////////////////////////////////////////////////////////
/// @brief 初期値をもった IO宣言要素の基底クラス
//////////////////////////////////////////////////////////////////////
class CptIOItemI :
  public CptIOItem
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] name 名前
  /// @param[in] init_value 初期値
  CptIOItemI(const FileRegion& file_region,
	     const char* name,
	     const PtExpr* init_value);

  /// @brief デストラクタ
  ~CptIOItemI();


public:
  //////////////////////////////////////////////////////////////////////
  // PtIOItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置の取得
  FileRegion
  file_region() const override;

  /// @brief 初期値の取得
  const PtExpr*
  init_value() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 初期値
  const PtExpr* mInitValue;

};


END_NAMESPACE_YM_VERILOG

#endif // CPTIO_H
