﻿#ifndef SPTDECL_H
#define SPTDECL_H

/// @file SptDecl.h
/// @brief SptDecl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym//FileRegion.h"

#include "PtiDecl.h"



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
	    tVpiAuxType aux_type,
	    tVpiNetType net_type,
	    tVpiVarType var_type,
	    bool sign,
	    const PtExpr* left,
	    const PtExpr* right);

  /// デストラクタ
  virtual
  ~SptIOHead();


public:
  //////////////////////////////////////////////////////////////////////
  // PtIOH の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// ファイル位置の取得
  virtual
  FileRegion
  file_region() const;

  /// 型の取得
  virtual
  tPtIOType
  type() const;

  /// 補助的な型の取得
  virtual
  tVpiAuxType
  aux_type() const;

  /// 補助的なネット型の取得
  virtual
  tVpiNetType
  net_type() const;

  /// 補助的な変数型の取得
  virtual
  tVpiVarType
  var_type() const;

  /// 符号の取得
  virtual
  bool
  is_signed() const;

  /// 範囲のMSBの取得
  virtual
  const PtExpr*
  left_range() const;

  /// 範囲のLSBの取得
  virtual
  const PtExpr*
  right_range() const;

  /// @brief 要素数の取得
  virtual
  ymuint
  item_num() const;

  /// @brief 要素の取得
  /// @param[in] pos 位置番号 ( 0 <= pos < item_num() )
  virtual
  const PtIOItem*
  item(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // PtiIOH の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素リストの設定
  /// @param[in] elem_array 要素リスト
  virtual
  void
  set_elem(PtIOItemArray elem_array);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // IO の種類
  tPtIOType mType;

  // 補助的な型
  tVpiAuxType mAuxType;

  // 補助的なネット型
  tVpiNetType mNetType;

  // 補助的な変数型
  tVpiVarType mVarType;

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
  virtual
  ~SptIOItem();


public:
  //////////////////////////////////////////////////////////////////////
  // PtIOItem の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// ファイル位置の取得
  virtual
  FileRegion
  file_region() const;

  /// 名前の取得
  virtual
  const char*
  name() const;

  /// 初期値の取得
  virtual
  const PtExpr*
  init_value() const;


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
	      tVpiVarType var_type,
	      tVpiNetType net_type,
	      tVpiVsType vs_type,
	      const PtStrength* strength,
	      const PtDelay* delay);

  /// デストラクタ
  virtual
  ~SptDeclHead();


public:
  //////////////////////////////////////////////////////////////////////
  // PtDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// ファイル位置の取得
  virtual
  FileRegion
  file_region() const;

  /// 宣言要素の型の取得
  virtual
  tPtDeclType
  type() const;

  /// 符号の取得
  virtual
  bool
  is_signed() const;

  /// 範囲のMSBの取得
  virtual
  const PtExpr*
  left_range() const;

  /// 範囲のLSBの取得
  virtual
  const PtExpr*
  right_range() const;

  /// データ型の取得
  virtual
  tVpiVarType
  data_type() const;

  /// net 型の取得
  virtual
  tVpiNetType
  net_type() const;

  /// vectored|scalared 属性の取得
  virtual
  tVpiVsType
  vs_type() const;

  /// strength の取得
  virtual
  const PtStrength*
  strength() const;

  /// delay の取得
  virtual
  const PtDelay*
  delay() const;

  /// @brief 要素数の取得
  /// @return 要素数
  virtual
  ymuint
  item_num() const;

  /// @brief 要素の取得
  /// @param[in] pos 位置番号 ( 0 <= pos < item_num() )
  virtual
  const PtDeclItem*
  item(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // PtiDeclH の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素リストの設定
  /// @param[in] elem_array 要素リスト(配列)
  virtual
  void
  set_elem(PtDeclItemArray elem_array);


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
  tVpiVarType mVarType;

  // net type
  tVpiNetType mNetType;

  // vs_type
  tVpiVsType mVsType;

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
  virtual
  ~SptDeclItem();


public:
  //////////////////////////////////////////////////////////////////////
  // PtDeclItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// ファイル位置の取得
  virtual
  FileRegion
  file_region() const;

  /// 名前の取得
  virtual
  const char*
  name() const;

  /// dimension list のサイズの取得
  virtual
  ymuint
  dimension_list_size() const;

  /// 範囲の取得
  virtual
  const PtRange*
  range(ymuint pos) const;

  /// 初期値の取得
  virtual
  const PtExpr*
  init_value() const;


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
  virtual
  ~SptRange();


public:
  //////////////////////////////////////////////////////////////////////
  // PtRange の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// ファイル位置の取得
  virtual
  FileRegion
  file_region() const;

  /// @brief 範囲の MSB を取り出す．
  virtual
  const PtExpr*
  left() const;

  /// @brief 範囲の LSB を取り出す．
  virtual
  const PtExpr*
  right() const;


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
