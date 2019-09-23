#ifndef SPTDECL_H
#define SPTDECL_H

/// @file SptDecl.h
/// @brief SptDecl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym/FileRegion.h"
#include "ym/pt/PtArray.h"

#include "parser/PtiDecl.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// IO宣言のヘッダのベース実装クラス
//////////////////////////////////////////////////////////////////////
class SptIOHead :
  public PtiIOHead
{
  friend class SptFactory;

private:

  /// コンストラクタ
  SptIOHead(const FileRegion& file_region,
	    tPtIOType type,
	    VpiAuxType aux_type,
	    VpiNetType net_type,
	    VpiVarType var_type,
	    bool sign,
	    const PtExpr* left,
	    const PtExpr* right);

  /// デストラクタ
  ~SptIOHead();


public:
  //////////////////////////////////////////////////////////////////////
  // PtIOH の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// ファイル位置の取得
  FileRegion
  file_region() const override;

  /// 型の取得
  tPtIOType
  type() const override;

  /// 補助的な型の取得
  VpiAuxType
  aux_type() const override;

  /// 補助的なネット型の取得
  VpiNetType
  net_type() const override;

  /// 補助的な変数型の取得
  VpiVarType
  var_type() const override;

  /// 符号の取得
  bool
  is_signed() const override;

  /// 範囲のMSBの取得
  const PtExpr*
  left_range() const override;

  /// 範囲のLSBの取得
  const PtExpr*
  right_range() const override;

  /// @brief 要素のリストの取得
  PtIOItemArray
  item_list() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // PtiIOH の仮想関数
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

  // IO の種類
  tPtIOType mType;

  // 補助的な型
  VpiAuxType mAuxType;

  // 補助的なネット型
  VpiNetType mNetType;

  // 補助的な変数型
  VpiVarType mVarType;

  // 符号の有無
  bool mSigned;

  // 範囲のMSB
  const PtExpr* mLeftRange;

  // 範囲のLSB
  const PtExpr* mRightRange;

  // 要素の配列
  PtIOItemArray mItemArray;

};


//////////////////////////////////////////////////////////////////////
/// IO宣言要素のベース実装クラス
//////////////////////////////////////////////////////////////////////
class SptIOItem :
  public PtIOItem
{
  friend class SptFactory;
  friend class SptIOHead;

private:

  /// コンストラクタ
  SptIOItem(const FileRegion& file_region,
	    const char* name,
	    const PtExpr* init_value);

  /// デストラクタ
  ~SptIOItem();


public:
  //////////////////////////////////////////////////////////////////////
  // PtIOItem の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// ファイル位置の取得
  FileRegion
  file_region() const override;

  /// 名前の取得
  const char*
  name() const override;

  /// 初期値の取得
  const PtExpr*
  init_value() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 名前
  const char* mName;

  // 初期値
  const PtExpr* mInitValue;

};


//////////////////////////////////////////////////////////////////////
/// 宣言要素のヘッダの基底クラス
//////////////////////////////////////////////////////////////////////
class SptDeclHead :
  public PtiDeclHead
{
  friend class SptFactory;

private:

  /// コンストラクタ
  SptDeclHead(const FileRegion& file_region,
	      tPtDeclType type,
	      bool sign,
	      const PtExpr* left,
	      const PtExpr* right,
	      VpiVarType var_type,
	      VpiNetType net_type,
	      VpiVsType vs_type,
	      const PtStrength* strength,
	      const PtDelay* delay);

  /// デストラクタ
  ~SptDeclHead();


public:
  //////////////////////////////////////////////////////////////////////
  // PtDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// ファイル位置の取得
  FileRegion
  file_region() const override;

  /// 宣言要素の型の取得
  tPtDeclType
  type() const override;

  /// 符号の取得
  bool
  is_signed() const override;

  /// 範囲のMSBの取得
  const PtExpr*
  left_range() const override;

  /// 範囲のLSBの取得
  const PtExpr*
  right_range() const override;

  /// データ型の取得
  VpiVarType
  data_type() const override;

  /// net 型の取得
  VpiNetType
  net_type() const override;

  /// vectored|scalared 属性の取得
  VpiVsType
  vs_type() const override;

  /// strength の取得
  const PtStrength*
  strength() const override;

  /// delay の取得
  const PtDelay*
  delay() const override;

  /// @brief 要素のリストの取得
  PtDeclItemArray
  item_list() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // PtiDeclH の継承クラスが実装する仮想関数
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

  // クラスの型
  tPtDeclType mType;

  // 符号の有無
  bool mSigned;

  // 範囲のMSB
  const PtExpr* mLeftRange;

  // 範囲のLSB
  const PtExpr* mRightRange;

  // データ型
  VpiVarType mVarType;

  // net type
  VpiNetType mNetType;

  // vs_type
  VpiVsType mVsType;

  // strength
  const PtStrength* mStrength;

  // delay
  const PtDelay* mDelay;

  // 要素の配列
  PtDeclItemArray mItemArray;

};


//////////////////////////////////////////////////////////////////////
/// 宣言要素のクラス
//////////////////////////////////////////////////////////////////////
class SptDeclItem :
  public PtDeclItem
{
  friend class SptFactory;
  friend class SptDeclHead;

private:

  /// コンストラクタ
  SptDeclItem(const FileRegion& file_region,
	      const char* name,
	      PtRangeArray range_array,
	      const PtExpr* init_value);

  /// デストラクタ
  ~SptDeclItem();


public:
  //////////////////////////////////////////////////////////////////////
  // PtDeclItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// ファイル位置の取得
  FileRegion
  file_region() const override;

  /// 名前の取得
  const char*
  name() const override;

  /// dimension list のサイズの取得
  int
  dimension_list_size() const override;

  /// 範囲のリストの取得
  PtRangeArray
  range_list() const override;

  /// 初期値の取得
  const PtExpr*
  init_value() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 名前
  const char* mName;

  // 範囲の配列
  PtRangeArray mRangeArray;

  // 初期値
  const PtExpr* mInitValue;

};


//////////////////////////////////////////////////////////////////////
/// @class SptRange SptDecl.h "SptDecl.h"
/// @brief 範囲を表すクラス
//////////////////////////////////////////////////////////////////////
class SptRange :
  public PtRange
{
public:

  /// @brief コンストラクタ
  SptRange(const FileRegion& file_region,
	   const PtExpr* msb,
	   const PtExpr* lsb);

  /// @brief デストラクタ
  ~SptRange();


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

#endif // SPTDECL_H
