﻿#ifndef SPTEXPR_H
#define SPTEXPR_H

/// @file SptExpr.h
/// @brief SptExpr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.

#include "ym/FileRegion.h"
#include "parser/PtiExpr.h"
#include "parser/PtiArray.h"
#include "parser/PtiFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// expression を表す基底クラス
//////////////////////////////////////////////////////////////////////
class SptExpr :
  public PtiExpr
{
public:

  /// @brief コンストラクタ
  SptExpr(
    const FileRegion& file_region,
    PtExprType type
  );

  /// @brief デストラクタ
  ~SptExpr();


public:
  //////////////////////////////////////////////////////////////////////
  // PtExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置の取得
  FileRegion
  file_region() const override;

  /// @brief 式の型の取得
  PtExprType
  type() const override;

  /// @brief 演算子の種類の取得
  /// @return 演算子の種類
  VpiOpType
  op_type() const override;

  /// @brief 階層ブランチの要素数の取得
  SizeType
  namebranch_num() const override;

  /// @brief 階層ブランチの取得
  const PtNameBranch*
  namebranch(
    SizeType pos ///< [in] 位置 ( 0 <= pos < namebranch_num() )
  ) const override;

  /// @brief 末尾の名前の取得
  /// @return 末尾の名前
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
  /// @return サイズ
  ///
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


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 式の種類
  PtExprType mType;

};


//////////////////////////////////////////////////////////////////////
/// オペランド数が固定の演算子を表すクラス
//////////////////////////////////////////////////////////////////////
class SptOpr1 :
  public SptExpr
{
public:

  /// @brief コンストラクタ
  SptOpr1(
    const FileRegion& file_region,
    VpiOpType op_type,
    const PtExpr* opr1,
    const PtExpr* opr2 = nullptr,
    const PtExpr* opr3 = nullptr
  );

  /// @brief デストラクタ
  ~SptOpr1();


public:
  //////////////////////////////////////////////////////////////////////
  // PtExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief インデックスとして使える式のチェック
  bool
  is_index_expr() const override;

  /// @brief インデックスの値の取得
  int
  index_value() const override;

  /// @brief 演算子の種類の取得
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

  /// pos 番目のオペランドを取り出す．
  const PtExpr*
  operand(
    SizeType pos ///< [in] 位置番号 ( 0 <= pos < operand_num() )
  ) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 演算子の種類
  VpiOpType mOpType;

  // オペランド数
  SizeType mSize;

  // オペランドの配列
  const PtExpr* mExprList[3];

};


//////////////////////////////////////////////////////////////////////
/// オペランド数が可変の演算子を表すクラス
//////////////////////////////////////////////////////////////////////
class SptOpr2 :
  public SptExpr
{
public:

  /// @brief コンストラクタ
  SptOpr2(
    const FileRegion& file_region,
    VpiOpType op_type,
    PtiExprArray&& opr_array
  );

  /// @brief デストラクタ
  ~SptOpr2();


public:
  //////////////////////////////////////////////////////////////////////
  // PtExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief インデックスとして使える式のチェック
  bool
  is_index_expr() const override;

  /// @brief インデックスの値の取得
  int
  index_value() const override;

  /// @brief 演算子の種類の取得
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

  /// @brief pos 番目のオペランドを取り出す．
  const PtExpr*
  operand(
    SizeType pos
  ) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 演算子の種類
  VpiOpType mOpType;

  // オペランドのリスト
  PtiExprArray mExprArray;

};


//////////////////////////////////////////////////////////////////////
// function call / system function call に共通の基底クラス
//////////////////////////////////////////////////////////////////////
class SptFuncCall :
  public SptExpr
{
public:

  /// @brief コンストラクタ
  SptFuncCall(
    const FileRegion& file_region,
    PtExprType type,
    PtiNameBranchArray&& nb_array,
    const char* name,
    PtiExprArray&& arg_array
  );

  /// @brief デストラクタ
  ~SptFuncCall();


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

  /// 末尾の名前を返す．
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

  // 階層ブランチの配列
  PtiNameBranchArray mNbArray;

  // 末尾の名前
  const char* mName;

  // 引数リスト
  PtiExprArray mArgArray;

};


//////////////////////////////////////////////////////////////////////
// PtPrimary のベース実装クラス
//////////////////////////////////////////////////////////////////////
class SptPrimary :
  public SptExpr
{
public:

  /// @brief コンストラクタ
  SptPrimary(
    const FileRegion& file_region,
    PtiNameBranchArray&& nb_array,
    const char* tail_name,
    bool const_index,
    PtiExprArray&& index_array = PtiExprArray(),
    VpiRangeMode mode = VpiRangeMode::No,
    const PtExpr* left = nullptr,
    const PtExpr* right = nullptr
  );

  /// @brief デストラクタ
  ~SptPrimary();


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

  /// @brief 末尾の名前を取り出す．
  const char*
  name() const override;

  /// @brief インデックスもしくは範囲が定数にならなければならないとき true を返す．
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
  VpiRangeMode
  range_mode() const override;

  /// @brief range の MSB を取出す．
  const PtExpr*
  left_range() const override;

  /// @brief range の LSB を取出す．
  const PtExpr*
  right_range() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 階層ブランチのリスト
  PtiNameBranchArray mNbArray;

  // 末尾の名前
  const char* mName;

  // インデックス/範囲が定数の時 true
  bool mConstIndex;

  // インデックスの配列
  PtiExprArray mIndexArray;

  // 範囲のモード
  VpiRangeMode mMode;

  // 範囲のMSB
  const PtExpr* mLeftRange;

  // 範囲のLSB
  const PtExpr* mRightRange;

};


//////////////////////////////////////////////////////////////////////
// 定数
//////////////////////////////////////////////////////////////////////
class SptConstant :
  public SptExpr
{
public:

  /// @brief コンストラクタ
  SptConstant(
    const FileRegion& file_region,
    VpiConstType const_type,
    SizeType size,
    std::uint32_t ivalue,
    const char* svalue,
    double rvalue
  );

  /// @brief デストラクタ
  ~SptConstant();


public:
  //////////////////////////////////////////////////////////////////////
  // PtExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 階層名の添字として使える式の時に true を返す．
  bool
  is_index_expr() const override;

  /// @brief 定数の種類を表す型を返す．
  VpiConstType
  const_type() const override;

  /// @brief 整数型の定数のサイズの取得
  SizeType
  const_size() const override;

  /// @brief 整数型の値の取得
  std::uint32_t
  const_uint32() const override;

  /// @brief 文字列型の値の取得
  const char*
  const_str() const override;

  /// @brief 実数型の値の取得
  double
  const_real() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 定数の型
  VpiConstType mConstType;

  // サイズ
  // 0 の時は指定なし
  SizeType mSize;

  // 整数の値
  std::uint32_t mIntValue;

  // 文字列の値
  const char* mStrValue;

  // 実数の値
  double mRealValue;

};

END_NAMESPACE_YM_VERILOG

#endif // SPTEXPR_H
