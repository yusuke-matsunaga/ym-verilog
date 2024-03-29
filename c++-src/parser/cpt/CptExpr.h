﻿#ifndef CPTEXPR_H
#define CPTEXPR_H

/// @file CptExpr.h
/// @brief CptExpr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.

#include "parser/PtiExpr.h"
#include "parser/PtiArray.h"
#include "parser/PtiFwd.h"
#include "ym/FileRegion.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// expression を表す基底クラス
//////////////////////////////////////////////////////////////////////
class CptExpr :
  public PtiExpr
{
public:
  //////////////////////////////////////////////////////////////////////
  // PtExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 階層ブランチの要素数の取得
  SizeType
  namebranch_num() const override;

  /// @brief 階層ブランチの取得
  const PtNameBranch*
  namebranch(
    SizeType pos ///< [in] 位置 ( 0 <= pos < namebranch_num() )
  ) const override;

  // 末尾の名前を返す．
  const char*
  name() const override;

  /// @brief 演算子の種類の取得
  /// @return 演算子の種類
  VpiOpType
  op_type() const override;

  /// @brief オペランドの数の取得
  /// @return 子供の数
  SizeType
  operand_num() const override;

  /// @brief 0番目のオペランドの取得
  const PtExpr*
  operand0() const override;

  /// @brief 1番目のオペランドの取得
  const PtExpr*
  operand1() const override;

  /// @brief 2番目のオペランドの取得
  const PtExpr*
  operand2() const override;

  /// @brief オペランドの取得
  /// @return pos 番目のオペランド
  const PtExpr*
  operand(
    SizeType pos ///< [in] 取り出すオペランンドの位置(最初の位置は 0)
  ) const override;

  /// @brief 定数インデックスのチェック
  /// @retval true インデックスもしくは範囲が定数にならなければならないとき
  /// @retval false 上記以外
  bool
  is_const_index() const override;

  /// @brief インデックスリストのサイズの取得
  /// @return インデックスリストのサイズ
  SizeType
  index_num() const override;

  /// @brief インデックスの取得
  const PtExpr*
  index(
    SizeType pos ///< [in] 位置番号 ( 0 <= pos < index_num() )
  ) const override;

  /// @brief 範囲指定モードの取得
  /// @return 範囲指定モード
  VpiRangeMode
  range_mode() const override;

  /// @brief 範囲の左側の式の取得
  /// @return 範囲の左側の式
  const PtExpr*
  left_range() const override;

  /// @brief 範囲の右側の式の取得
  /// @return 範囲の右側の式
  const PtExpr*
  right_range() const override;

  /// @brief 定数の種類の取得
  /// @return 定数の種類
  VpiConstType
  const_type() const override;

  /// @brief 整数型の定数のサイズの取得
  /// @return サイズ\n
  /// サイズ指定の無い場合と整数型の定数でない場合には 0 を返す．
  SizeType
  const_size() const override;

  /// @brief 整数型の値の取得
  /// @return 値
  std::uint32_t
  const_uint32() const override;

  /// @brief 整数型および文字列型の定数の文字列表現の取得
  /// @return 値の文字列表現
  ///
  /// 整数型時のサイズと基数は含まない．
  const char*
  const_str() const override;

  /// @brief 実数型の値の取得
  /// @return 値
  double
  const_real() const override;

  /// @brief インデックスとして使える式のチェック
  /// @retval true 階層名の添字として使える式
  /// @retval false 使えない式
  bool
  is_index_expr() const override;

  /// @brief インデックスの値の取得
  /// @return 階層名の添字として使える式の時にその値を返す．
  int
  index_value() const override;

  /// @brief simple primary のチェック
  /// @retval true index_list も range も持たないとき
  /// @retval false 上記以外
  bool
  is_simple() const override;

};


//////////////////////////////////////////////////////////////////////
// 演算子のベース実装クラス
//////////////////////////////////////////////////////////////////////
class CptOpr :
  public CptExpr
{
protected:

  // コンストラクタ
  CptOpr(
    VpiOpType op_type ///< [in] 演算子の型
  );

  // デストラクタ
  ~CptOpr();


public:
  //////////////////////////////////////////////////////////////////////
  // PtExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  // クラスの型を返す．
  // このクラスの場合は kOpr を返す．
  PtExprType
  type() const override;

  // 演算子のトークン番号を得る．
  VpiOpType
  op_type() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 演算子を表すトークン番号
  VpiOpType mOpType;

};


//////////////////////////////////////////////////////////////////////
// 単項演算子を表すクラス
//////////////////////////////////////////////////////////////////////
class CptOpr1 :
  public CptOpr
{
public:

  // コンストラクタ
  CptOpr1(
    const FileRegion& file_region,
    VpiOpType op_type,
    const PtExpr* opr
  );

  // デストラクタ
  ~CptOpr1();


public:
  //////////////////////////////////////////////////////////////////////
  // PtExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  // ファイル位置を返す．
  FileRegion
  file_region() const override;

  // 階層名の添字として使える式の時に true を返す．
  bool
  is_index_expr() const override;

  // 階層名の添字として使える式の時にその値を返す．
  int
  index_value() const override;

  /// @brief オペランドの数の取得
  /// @return 子供の数
  SizeType
  operand_num() const override;

  /// @brief 0番目のオペランドの取得
  const PtExpr*
  operand0() const override;

  /// @brief 1番目のオペランドの取得
  const PtExpr*
  operand1() const override;

  /// @brief 2番目のオペランドの取得
  const PtExpr*
  operand2() const override;

  /// @brief オペランドの取得
  /// @return pos 番目のオペランド
  const PtExpr*
  operand(
    SizeType pos ///< [in] 取り出すオペランンドの位置(最初の位置は 0)
  ) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // オペランド
  const PtExpr* mOpr;

};


//////////////////////////////////////////////////////////////////////
// 二項演算子を表すクラス
//////////////////////////////////////////////////////////////////////
class CptOpr2 :
  public CptOpr
{
public:

  // コンストラクタ
  CptOpr2(
    VpiOpType op_type,
    const PtExpr* opr1,
    const PtExpr* opr2
  );

  // デストラクタ
  ~CptOpr2();


public:
  //////////////////////////////////////////////////////////////////////
  // PtExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  // ファイル位置を返す．
  FileRegion
  file_region() const override;

  /// @brief オペランドの数の取得
  /// @return 子供の数
  SizeType
  operand_num() const override;

  /// @brief 0番目のオペランドの取得
  const PtExpr*
  operand0() const override;

  /// @brief 1番目のオペランドの取得
  const PtExpr*
  operand1() const override;

  /// @brief 2番目のオペランドの取得
  const PtExpr*
  operand2() const override;

  /// @brief オペランドの取得
  /// @return pos 番目のオペランド
  const PtExpr*
  operand(
    SizeType pos ///< [in] 取り出すオペランンドの位置(最初の位置は 0)
  ) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // オペランド
  const PtExpr* mOpr[2];

};


//////////////////////////////////////////////////////////////////////
// 三項演算子を表すクラス
//////////////////////////////////////////////////////////////////////
class CptOpr3 :
  public CptOpr
{
public:

  // コンストラクタ
  CptOpr3(
    VpiOpType op_type,
    const PtExpr* opr1,
    const PtExpr* opr2,
    const PtExpr* opr3
  );

  // デストラクタ
  ~CptOpr3();


public:
  //////////////////////////////////////////////////////////////////////
  // PtExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  // ファイル位置を返す．
  FileRegion
  file_region() const override;

  /// @brief オペランドの数の取得
  /// @return 子供の数
  SizeType
  operand_num() const override;

  /// @brief 0番目のオペランドの取得
  const PtExpr*
  operand0() const override;

  /// @brief 1番目のオペランドの取得
  const PtExpr*
  operand1() const override;

  /// @brief 2番目のオペランドの取得
  const PtExpr*
  operand2() const override;

  /// @brief オペランドの取得
  /// @return pos 番目のオペランド
  const PtExpr*
  operand(
    SizeType pos ///< [in] 取り出すオペランンドの位置(最初の位置は 0)
  ) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // オペランド
  const PtExpr* mOpr[3];

};


//////////////////////////////////////////////////////////////////////
// concatenation を表すクラス
//////////////////////////////////////////////////////////////////////
class CptConcat :
  public CptExpr
{
public:

  // コンストラクタ
  CptConcat(
    const FileRegion& file_region,
    PtiExprArray&& expr_array
  );

  // デストラクタ
  ~CptConcat();


public:
  //////////////////////////////////////////////////////////////////////
  // PtExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  // ファイル位置を返す．
  FileRegion
  file_region() const override;

  // クラスの型を返す．
  PtExprType
  type() const override;

  ///演算子の種類の取得
  VpiOpType
  op_type() const override;

  /// @brief オペランドの数の取得
  /// @return 子供の数
  SizeType
  operand_num() const override;

  /// @brief 0番目のオペランドの取得
  const PtExpr*
  operand0() const override;

  /// @brief 1番目のオペランドの取得
  const PtExpr*
  operand1() const override;

  /// @brief 2番目のオペランドの取得
  const PtExpr*
  operand2() const override;

  /// @brief オペランドの取得
  /// @return pos 番目のオペランド
  const PtExpr*
  operand(
    SizeType pos ///< [in] 取り出すオペランンドの位置(最初の位置は 0)
  ) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 結合する式の配列
  PtiExprArray mExprArray;

};


//////////////////////////////////////////////////////////////////////
// multiple concatenation を表すクラス
//////////////////////////////////////////////////////////////////////
class CptMultiConcat :
  public CptConcat
{
public:

  // コンストラクタ
  CptMultiConcat(
    const FileRegion& file_region,
    PtiExprArray&& expr_array
  );

  // デストラクタ
  ~CptMultiConcat();


public:
  //////////////////////////////////////////////////////////////////////
  // PtExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  ///演算子の種類の取得
  VpiOpType
  op_type() const override;

};


//////////////////////////////////////////////////////////////////////
// min/typ/max delayを表すクラス
//////////////////////////////////////////////////////////////////////
class CptMinTypMax :
  public CptExpr
{
public:

  // コンストラクタ
  CptMinTypMax(
    const PtExpr* val0,
    const PtExpr* val1,
    const PtExpr* val2
  );

  // デストラクタ
  ~CptMinTypMax();


public:
  //////////////////////////////////////////////////////////////////////
  // PtExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  // ファイル位置を返す．
  FileRegion
  file_region() const override;

  // クラスの型を返す．
  PtExprType
  type() const override;

  ///演算子の種類の取得
  VpiOpType
  op_type() const override;

  /// @brief オペランドの数の取得
  /// @return 子供の数
  SizeType
  operand_num() const override;

  /// @brief 0番目のオペランドの取得
  const PtExpr*
  operand0() const override;

  /// @brief 1番目のオペランドの取得
  const PtExpr*
  operand1() const override;

  /// @brief 2番目のオペランドの取得
  const PtExpr*
  operand2() const override;

  /// @brief オペランドの取得
  /// @return pos 番目のオペランド
  const PtExpr*
  operand(
    SizeType pos ///< [in] 取り出すオペランンドの位置(最初の位置は 0)
  ) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値
  const PtExpr* mValue[3];

};


//////////////////////////////////////////////////////////////////////
// function call / system function call に共通の基底クラス
//////////////////////////////////////////////////////////////////////
class CptFuncCallBase :
  public CptExpr
{
public:

  // コンストラクタ
  CptFuncCallBase(
    const FileRegion& file_region,
    const char* name,
    PtiExprArray&& arg_array
  );

  // デストラクタ
  ~CptFuncCallBase();


public:
  //////////////////////////////////////////////////////////////////////
  // PtExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  // ファイル位置を返す．
  FileRegion
  file_region() const override;

  // 末尾の名前を返す．
  const char*
  name() const override;

  /// @brief オペランドの数の取得
  /// @return 子供の数
  SizeType
  operand_num() const override;

  /// @brief 0番目のオペランドの取得
  const PtExpr*
  operand0() const override;

  /// @brief 1番目のオペランドの取得
  const PtExpr*
  operand1() const override;

  /// @brief 2番目のオペランドの取得
  const PtExpr*
  operand2() const override;

  /// @brief オペランドの取得
  /// @return pos 番目のオペランド
  const PtExpr*
  operand(
    SizeType pos ///< [in] 取り出すオペランンドの位置(最初の位置は 0)
  ) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 末尾の名前
  const char* mName;

  // 引数の配列
  PtiExprArray mArgArray;

};


//////////////////////////////////////////////////////////////////////
// 階層なし名前を持つ function call を表すクラス
//////////////////////////////////////////////////////////////////////
class CptFuncCall :
  public CptFuncCallBase
{
public:

  // コンストラクタ
  CptFuncCall(
    const FileRegion& file_region,
    const char* name,
    PtiExprArray&& arg_array
  );

  // デストラクタ
  ~CptFuncCall();


public:
  //////////////////////////////////////////////////////////////////////
  // PtExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  // クラスの型を返す．
  // このクラスの場合は kPtFunCallExpr を返す．
  PtExprType
  type() const override;

};


//////////////////////////////////////////////////////////////////////
// 階層つき名前を持つ function call を表すクラス
//////////////////////////////////////////////////////////////////////
class CptFuncCallH :
  public CptFuncCall
{
public:

  // コンストラクタ
  CptFuncCallH(
    const FileRegion& file_region,
    PtiNameBranchArray&& nb_array,
    const char* tail_name,
    PtiExprArray&& arg_array
  );

  // デストラクタ
  ~CptFuncCallH();


public:
  //////////////////////////////////////////////////////////////////////
  // PtExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 階層ブランチの要素数の取得
  SizeType
  namebranch_num() const override;

  /// @brief 階層ブランチの取得
  const PtNameBranch*
  namebranch(
    SizeType pos ///< [in] 位置 ( 0 <= pos < namebranch_num() )
  ) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 階層ブランチのリスト
  PtiNameBranchArray mNbArray;

};


//////////////////////////////////////////////////////////////////////
// system function call を表すクラス
//////////////////////////////////////////////////////////////////////
class CptSysFuncCall :
  public CptFuncCallBase
{
public:

  // コンストラクタ
  CptSysFuncCall(
    const FileRegion& file_region,
    const char* name,
    PtiExprArray&& arg_array
  );

  // デストラクタ
  ~CptSysFuncCall();


public:
  //////////////////////////////////////////////////////////////////////
  // PtExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  // クラスの型を返す．
  PtExprType
  type() const override;

};


// 解説
// もとは generic な一つのクラスだったがメモリ使用量を減らすために
// カスタマイズした．
// 直交した2つの属性で分類している．
// - 階層の有無
// - インデックス/範囲指定の有無, 定数式か否か
// この2つを多重継承で組み合わせると仮想関数テーブルがその数だけ必要に
// なってメモリ節約の意味がなくなるのでかっこ悪さを承知で階層の有無に関
// してはクラスごとに仮想関数を書く．ただし PtSnameHolder もしくは
// PtHnameHolder を private 継承(俗に言う「実装のための継承」)しているので
// それらの関数を呼び出すだけで良い．
// また，これらは全ての組合わせがあるわけではないので実際に使われている
// クラスだけ考える．具体的には以下の組み合わせ．
//
//                             simple             hierarchical
// none                          X                     X
// index                                               X
// constant index                X                     X
// range                                               X
// constant range                X
// index and range                                     X
// constant index and range

//////////////////////////////////////////////////////////////////////
// PtPrimary のベース実装クラス
//////////////////////////////////////////////////////////////////////
class CptPrimaryBase :
  public CptExpr
{
public:

  // コンストラクタ
  CptPrimaryBase(
    const char* name
  );

  // デストラクタ
  ~CptPrimaryBase();


public:
  //////////////////////////////////////////////////////////////////////
  // PtExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  // クラスの型を返す．
  PtExprType
  type() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // PtPrimary の仮想関数
  //////////////////////////////////////////////////////////////////////

  // 末尾の名前を取り出す．
  const char*
  name() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 末尾の名前
  const char* mName;

};


//////////////////////////////////////////////////////////////////////
// PtPrimary のベース実装クラス
//////////////////////////////////////////////////////////////////////
class CptPrimary :
  public CptPrimaryBase
{
public:

  // コンストラクタ
  CptPrimary(
    const FileRegion& file_region,
    const char* name
  );

  // デストラクタ
  ~CptPrimary();


public:
  //////////////////////////////////////////////////////////////////////
  // PtExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  // ファイル位置を返す．
  FileRegion
  file_region() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // PtPrimary の仮想関数
  //////////////////////////////////////////////////////////////////////

  // index_list も range も持たないとき true を返す．
  bool
  is_simple() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

};


//////////////////////////////////////////////////////////////////////
// インデックスつきの primary を表すクラス
//////////////////////////////////////////////////////////////////////
class CptPrimaryI :
  public CptPrimaryBase
{
public:

  // コンストラクタ
  CptPrimaryI(
    const FileRegion& file_region,
    const char* name,
    PtiExprArray&& index_array
  );

  // デストラクタ
  ~CptPrimaryI();


public:
  //////////////////////////////////////////////////////////////////////
  // PtPrimary の仮想関数
  //////////////////////////////////////////////////////////////////////

  // ファイル位置を返す．
  FileRegion
  file_region() const override;

  /// @brief インデックスリストのサイズの取得
  /// @return インデックスリストのサイズ
  SizeType
  index_num() const override;

  /// @brief インデックスの取得
  const PtExpr*
  index(
    SizeType pos ///< [in] 位置番号 ( 0 <= pos < index_num() )
  ) const override;

  // index_list も range も持たないとき true を返す．
  bool
  is_simple() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // インデックスの配列
  PtiExprArray mIndexArray;

};


//////////////////////////////////////////////////////////////////////
// 定数インデックスつきの primary を表すクラス
//////////////////////////////////////////////////////////////////////
class CptPrimaryCI :
  public CptPrimaryI
{
public:

  // コンストラクタ
  CptPrimaryCI(
    const FileRegion& file_region,
    const char* name,
    PtiExprArray&& index_array
  );

  // デストラクタ
  ~CptPrimaryCI();


public:
  //////////////////////////////////////////////////////////////////////
  // PtPrimary の仮想関数
  //////////////////////////////////////////////////////////////////////

  // インデックスもしくは範囲が定数にならなければならないとき true を返す．
  bool
  is_const_index() const override;

};


//////////////////////////////////////////////////////////////////////
// 範囲指定つきの primary を表すクラス
//////////////////////////////////////////////////////////////////////
class CptPrimaryR :
  public CptPrimaryBase
{
public:

  // コンストラクタ
  CptPrimaryR(
    const FileRegion& file_region,
    const char* name,
    VpiRangeMode mode,
    const PtExpr* left,
    const PtExpr* right
  );

  // デストラクタ
  ~CptPrimaryR();


public:
  //////////////////////////////////////////////////////////////////////
  // PtPrimary の仮想関数
  //////////////////////////////////////////////////////////////////////

  // ファイル位置を返す．
  FileRegion
  file_region() const override;

  // 範囲指定モードの取得
  VpiRangeMode
  range_mode() const override;

  // range の MSB を取出す．
  const PtExpr*
  left_range() const override;

  // range の LSB を取出す．
  const PtExpr*
  right_range() const override;

  // index_list も range も持たないとき true を返す．
  bool
  is_simple() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 範囲のモード
  VpiRangeMode mMode;

  // 範囲のMSB
  const PtExpr* mLeftRange;

  // 範囲のLSB
  const PtExpr* mRightRange;

};


//////////////////////////////////////////////////////////////////////
// 定数の範囲指定つきの primary を表すクラス
//////////////////////////////////////////////////////////////////////
class CptPrimaryCR :
  public CptPrimaryR
{
public:

  // コンストラクタ
  CptPrimaryCR(
    const FileRegion& file_region,
    const char* name,
    VpiRangeMode mode,
    const PtExpr* left,
    const PtExpr* right
  );

  // デストラクタ
  ~CptPrimaryCR();


public:
  //////////////////////////////////////////////////////////////////////
  // PtPrimary の仮想関数
  //////////////////////////////////////////////////////////////////////

  // インデックスもしくは範囲が定数にならなければならないとき true を返す．
  bool
  is_const_index() const override;

};


//////////////////////////////////////////////////////////////////////
// インデックスと範囲指定つきの primary を表すクラス
//////////////////////////////////////////////////////////////////////
class CptPrimaryIR :
  public CptPrimaryI
{
public:

  // コンストラクタ
  CptPrimaryIR(
    const FileRegion& file_region,
    const char* name,
    PtiExprArray&& index_array,
    VpiRangeMode mode,
    const PtExpr* left,
    const PtExpr* right
  );

  // デストラクタ
  ~CptPrimaryIR();


public:
  //////////////////////////////////////////////////////////////////////
  // PtPrimary の仮想関数
  //////////////////////////////////////////////////////////////////////

  // 範囲指定モードの取得
  VpiRangeMode
  range_mode() const override;

  // range の MSB を取出す．
  const PtExpr*
  left_range() const override;

  // range の LSB を取出す．
  const PtExpr*
  right_range() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 範囲のモード
  VpiRangeMode mMode;

  // 範囲のMSB
  const PtExpr* mLeftRange;

  // 範囲のLSB
  const PtExpr* mRightRange;

};


//////////////////////////////////////////////////////////////////////
// 階層名を持つ primary operatorを表すクラス
//////////////////////////////////////////////////////////////////////
class CptPrimaryH :
  public CptPrimaryBase
{
public:

  // コンストラクタ
  CptPrimaryH(
    const FileRegion& file_region,
    PtiNameBranchArray&& nb_array,
    const char* tail_name
  );

  // デストラクタ
  ~CptPrimaryH();


public:
  //////////////////////////////////////////////////////////////////////
  // PtPrimary の仮想関数
  //////////////////////////////////////////////////////////////////////

  // ファイル位置を返す．
  FileRegion
  file_region() const override;

  /// @brief 階層ブランチの要素数の取得
  SizeType
  namebranch_num() const override;

  /// @brief 階層ブランチの取得
  const PtNameBranch*
  namebranch(
    SizeType pos ///< [in] 位置 ( 0 <= pos < namebranch_num() )
  ) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 階層ブランチのリスト
  PtiNameBranchArray mNbArray;

};


//////////////////////////////////////////////////////////////////////
// 階層名を持つインデックスつき primary を表すクラス
//////////////////////////////////////////////////////////////////////
class CptPrimaryHI :
  public CptPrimaryI
{
public:

  // コンストラクタ
  CptPrimaryHI(
    const FileRegion& file_region,
    PtiNameBranchArray&& nb_array,
    const char* tail_name,
    PtiExprArray&& index_array
  );

  // デストラクタ
  ~CptPrimaryHI();


public:
  //////////////////////////////////////////////////////////////////////
  // PtPrimary の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 階層ブランチの要素数の取得
  SizeType
  namebranch_num() const override;

  /// @brief 階層ブランチの取得
  const PtNameBranch*
  namebranch(
    SizeType pos ///< [in] 位置 ( 0 <= pos < namebranch_num() )
  ) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 階層ブランチのリスト
  PtiNameBranchArray mNbArray;

};


//////////////////////////////////////////////////////////////////////
// 階層名を持つ定数インデックスつき primary を表すクラス
//////////////////////////////////////////////////////////////////////
class CptPrimaryHCI :
  public CptPrimaryHI
{
public:

  // コンストラクタ
  CptPrimaryHCI(
    const FileRegion& file_region,
    PtiNameBranchArray&& nb_array,
    const char* tail_name,
    PtiExprArray&& index_array
  );

  // デストラクタ
  ~CptPrimaryHCI();


public:
  //////////////////////////////////////////////////////////////////////
  // PtPrimary の仮想関数
  //////////////////////////////////////////////////////////////////////

  // インデックスもしくは範囲が定数にならなければならないとき true を返す．
  bool
  is_const_index() const override;

};


//////////////////////////////////////////////////////////////////////
// 階層名を持つ範囲指定つき primary を表すクラス
//////////////////////////////////////////////////////////////////////
class CptPrimaryHR :
  public CptPrimaryR
{
public:

  // コンストラクタ
  CptPrimaryHR(
    const FileRegion& file_region,
    PtiNameBranchArray&& nb_array,
    const char* tail_name,
    VpiRangeMode mode,
    const PtExpr* left,
    const PtExpr* right
  );

  // デストラクタ
  ~CptPrimaryHR();


public:
  //////////////////////////////////////////////////////////////////////
  // PtPrimary の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 階層ブランチの要素数の取得
  SizeType
  namebranch_num() const override;

  /// @brief 階層ブランチの取得
  const PtNameBranch*
  namebranch(
    SizeType pos ///< [in] 位置 ( 0 <= pos < namebranch_num() )
  ) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 階層ブランチのリスト
  PtiNameBranchArray mNbArray;

};


//////////////////////////////////////////////////////////////////////
// 階層名を持つインデックスと範囲指定つき primary を表すクラス
//////////////////////////////////////////////////////////////////////
class CptPrimaryHIR :
  public CptPrimaryIR
{
public:

  // コンストラクタ
  CptPrimaryHIR(
    const FileRegion& file_region,
    PtiNameBranchArray&& nb_array,
    const char* tail_name,
    PtiExprArray&& index_array,
    VpiRangeMode mode,
    const PtExpr* left,
    const PtExpr* right
  );

  // デストラクタ
  ~CptPrimaryHIR();


public:
  //////////////////////////////////////////////////////////////////////
  // PtPrimary の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 階層ブランチの要素数の取得
  SizeType
  namebranch_num() const override;

  /// @brief 階層ブランチの取得
  const PtNameBranch*
  namebranch(
    SizeType pos ///< [in] 位置 ( 0 <= pos < namebranch_num() )
  ) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 階層ブランチのリスト
  PtiNameBranchArray mNbArray;

};


//////////////////////////////////////////////////////////////////////
// PtConstant のベース実装クラス
//////////////////////////////////////////////////////////////////////
class CptConstant :
  public CptExpr
{
protected:

  // コンストラクタ
  CptConstant(
    const FileRegion& file_region
  );

  // デストラクタ
  ~CptConstant();


public:
  //////////////////////////////////////////////////////////////////////
  // PtExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  // ファイル位置を返す．
  FileRegion
  file_region() const override;

  // クラスの型を返す．
  // このクラスの場合は kPtConstantExpr を返す．
  PtExprType
  type() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

};


//////////////////////////////////////////////////////////////////////
// 整数型の定数(サイズ/基数の指定なし)
//////////////////////////////////////////////////////////////////////
class CptIntConstant1 :
  public CptConstant
{
public:

  // コンストラクタ
  CptIntConstant1(
    const FileRegion& file_region,
    std::uint32_t value
  );

  // デストラクタ
  ~CptIntConstant1();


public:
  //////////////////////////////////////////////////////////////////////
  // PtExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  // 階層名の添字として使える式の時に true を返す．
  bool
  is_index_expr() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // PtConstant の仮想関数
  //////////////////////////////////////////////////////////////////////

  // 定数の種類を表す型(vpiIntConst, vpiBinaryConst など) を返す．
  VpiConstType
  const_type() const override;

  // 整数型の値の取得
  std::uint32_t
  const_uint32() const override;

  /// @brief インデックスの値の取得
  /// @return 階層名の添字として使える式の時にその値を返す．
  int
  index_value() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値
  std::uint32_t mValue;

};


//////////////////////////////////////////////////////////////////////
// 整数型の定数(基数のみ指定あり)
//////////////////////////////////////////////////////////////////////
class CptIntConstant2 :
  public CptConstant
{
public:

  // コンストラクタ
  CptIntConstant2(
    const FileRegion& file_region,
    VpiConstType const_type,
    const char* value
  );

  // デストラクタ
  ~CptIntConstant2();


public:
  //////////////////////////////////////////////////////////////////////
  // PtConstant の仮想関数
  //////////////////////////////////////////////////////////////////////

  // 定数の種類を表す型(vpiIntConst, vpiBinaryConst など) を返す．
  VpiConstType
  const_type() const override;

  // 文字列型の値の取得
  const char*
  const_str() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 定数の種類
  VpiConstType mConstType;

  // 値を表す文字列
  const char* mValue;

};


//////////////////////////////////////////////////////////////////////
// 整数型の定数(サイズ/基数の指定あり)
//////////////////////////////////////////////////////////////////////
class CptIntConstant3 :
  public CptConstant
{
public:

  // コンストラクタ
  CptIntConstant3(
    const FileRegion& file_region,
    SizeType size,
    VpiConstType const_type,
    const char* value
  );

  // デストラクタ
  ~CptIntConstant3();


public:
  //////////////////////////////////////////////////////////////////////
  // PtConstant の仮想関数
  //////////////////////////////////////////////////////////////////////

  // 定数の種類を表す型(vpiIntConst, vpiBinaryConst など) を返す．
  VpiConstType
  const_type() const override;

  // 整数型の定数のサイズの取得
  SizeType
  const_size() const override;

  // 文字列型の値の取得
  const char*
  const_str() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 定数の種類
  VpiConstType mConstType;

  // サイズ
  SizeType mSize;

  // 値を表す文字列
  const char* mValue;

};


//////////////////////////////////////////////////////////////////////
// 実数型の定数
//////////////////////////////////////////////////////////////////////
class CptRealConstant :
  public CptConstant
{
public:

  // コンストラクタ
  CptRealConstant(
    const FileRegion& file_region,
    double value
  );

  // デストラクタ
  ~CptRealConstant();


public:
  //////////////////////////////////////////////////////////////////////
  // PtConstant の仮想関数
  //////////////////////////////////////////////////////////////////////

  // 定数の種類を表す型(vpiRealConst) を返す．
  VpiConstType
  const_type() const override;

  // 実数型の値の取得
  double
  const_real() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値
  double mValue;

};


//////////////////////////////////////////////////////////////////////
// 文字列型の定数
//////////////////////////////////////////////////////////////////////
class CptStringConstant :
  public CptConstant
{
public:

  // 値を表す文字列を引数にとるコンストラクタ
  CptStringConstant(
    const FileRegion& file_region,
    const char* value
  );

  // デストラクタ
  ~CptStringConstant();


public:
  //////////////////////////////////////////////////////////////////////
  // PtConstant の仮想関数
  //////////////////////////////////////////////////////////////////////

  // 定数の種類を表す型(vpiStringConst) を返す．
  VpiConstType
  const_type() const override;

  // 文字列型の値の取得
  const char*
  const_str() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値
  const char* mValue;

};

END_NAMESPACE_YM_VERILOG

#endif // CPTEXPR_H
