#ifndef CPTGENERATE_H
#define CPTGENERATE_H

/// @file CptGenerate.h
/// @brief CptGenerate のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.

#include "CptItem.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @brief generate 文で生成される要素を持つ本体
//////////////////////////////////////////////////////////////////////
class CptGenBody
{
public:

  /// @brief コンストラクタ
  CptGenBody(
    PtiDeclHeadArray&& declhead_array,
    PtiItemArray&& item_array
  );

  /// @brief デストラクタ
  ~CptGenBody();


public:

  /// @brief 宣言ヘッダの要素数の取得
  SizeType
  declhead_num() const
  {
    return mDeclHeadArray.size();
  }

  /// @brief 宣言ヘッダの取得
  const PtDeclHead*
  declhead(
    SizeType pos ///< [in] 位置 ( 0 <= pos < declhead_num() )
  ) const
  {
    return mDeclHeadArray[pos];
  }

  /// @brief item リストの要素数の取得
  SizeType
  item_num() const
  {
    return mItemArray.size();
  }

  /// @brief item の取得
  const PtItem*
  item(
    SizeType pos ///< [in] 位置 ( 0 <= pos < item_num() )
  ) const
  {
    return mItemArray[pos];
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 宣言の配列
  PtiDeclHeadArray mDeclHeadArray;

  // 要素の配列
  PtiItemArray mItemArray;

};


//////////////////////////////////////////////////////////////////////
/// @brief generate ブロックの基底クラス
//////////////////////////////////////////////////////////////////////
class CptGenBase :
  public CptItem
{
public:

  /// @brief コンストラクタ
  CptGenBase(
    const FileRegion& file_region,
    PtiDeclHeadArray&& declhead_array,
    PtiItemArray&& item_array
  );

  /// @brief デストラクタ
  ~CptGenBase();


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  FileRegion
  file_region() const override;

  /// @brief 宣言ヘッダの要素数の取得
  SizeType
  declhead_num() const override;

  /// @brief 宣言ヘッダの取得
  const PtDeclHead*
  declhead(
    SizeType pos ///< [in] 位置 ( 0 <= pos < declhead_num() )
  ) const override;

  /// @brief item リストの要素数の取得
  SizeType
  item_num() const override;

  /// @brief item の取得
  const PtItem*
  item(
    SizeType pos ///< [in] 位置 ( 0 <= pos < item_num() )
  ) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 本体
  CptGenBody mBody;

};


//////////////////////////////////////////////////////////////////////
/// @brief generate ブロックを表すノード
//////////////////////////////////////////////////////////////////////
class CptGenerate :
  public CptGenBase
{
public:

  /// @brief コンストラクタ
  CptGenerate(
    const FileRegion& file_region,
    PtiDeclHeadArray&& declhead_array,
    PtiItemArray&& item_array
  );

  /// @brief デストラクタ
  ~CptGenerate();


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  PtItemType
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// @brief generate ブロックの中のブロック
//////////////////////////////////////////////////////////////////////
class CptGenBlock :
  public CptGenBase
{
public:

  /// @brief コンストラクタ
  CptGenBlock(
    const FileRegion& file_region,
    PtiDeclHeadArray&& declhead_array,
    PtiItemArray&& item_array
  );

  /// @brief デストラクタ
  ~CptGenBlock();


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  PtItemType
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// @brief 名前つきの GenBlock
//////////////////////////////////////////////////////////////////////
class CptGenBlockN :
  public CptGenBlock
{
public:

  /// @brief コンストラクタ
  CptGenBlockN(
    const FileRegion& file_region,
    const char* name,
    PtiDeclHeadArray&& declhead_array,
    PtiItemArray&& item_array
  );

  /// @brief デストラクタ
  ~CptGenBlockN();


public:
  //////////////////////////////////////////////////////////////////////
  // PtGenerate の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を返す．
  const char*
  name() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  const char* mName;

};


//////////////////////////////////////////////////////////////////////
/// @brief gen_if 文 を表すクラス
//////////////////////////////////////////////////////////////////////
class CptGenIf :
  public CptItem
{
public:

  /// @brief コンストラクタ
  CptGenIf(
    const FileRegion& file_region,
    const PtExpr* cond,
    PtiDeclHeadArray&& then_declhead_array,
    PtiItemArray&& then_item_array,
    PtiDeclHeadArray&& else_declhead_array,
    PtiItemArray&& else_item_array
  );

  /// @brief デストラクタ
  ~CptGenIf();


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  FileRegion
  file_region() const override;

  /// @brief 型を返す．
  PtItemType
  type() const override;

  /// @brief 条件式を返す．
  const PtExpr*
  expr() const override;

  /// @brief 条件が成り立ったときに生成される宣言ヘッダの要素数の取得
  SizeType
  then_declhead_num() const override;

  /// @brief 条件が成り立ったときに生成される宣言ヘッダの取得
  const PtDeclHead*
  then_declhead(
    SizeType pos ///< [in] 位置 ( 0 <= pos < declhead_num() )
  ) const override;

  /// @brief 条件が成り立ったときに生成される item リストの要素数の取得
  SizeType
  then_item_num() const override;

  /// @brief 条件が成り立ったときに生成される item の取得
  const PtItem*
  then_item(
    SizeType pos ///< [in] 位置 ( 0 <= pos < item_num() )
  ) const override;

  /// @brief 条件が成り立たなかったときに生成される宣言ヘッダの要素数の取得
  SizeType
  else_declhead_num() const override;

  /// @brief 条件が成り立たなかったときに生成される宣言ヘッダの取得
  const PtDeclHead*
  else_declhead(
    SizeType pos ///< [in] 位置 ( 0 <= pos < declhead_num() )
  ) const override;

  /// @brief 条件が成り立たなかったときに生成される item リストの要素数の取得
  SizeType
  else_item_num() const override;

  /// @brief 条件が成り立たなかったときに生成される item の取得
  const PtItem*
  else_item(
    SizeType pos ///< [in] 位置 ( 0 <= pos < item_num() )
  ) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 条件式
  const PtExpr* mCond;

  // 成り立ったとき生成される本体
  CptGenBody mThenBody;

  // 成り立たなかったとき生成される本体
  CptGenBody mElseBody;

};


//////////////////////////////////////////////////////////////////////
/// @brief gen_case 文 を表すクラス
//////////////////////////////////////////////////////////////////////
class CptGenCase :
  public CptItem
{
public:

  /// @brief コンストラクタ
  CptGenCase(
    const FileRegion& file_region,
    const PtExpr* expr,
    PtiGenCaseItemArray&& item_array
  );

  /// @brief デストラクタ
  ~CptGenCase();


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  FileRegion
  file_region() const override;

  /// @brief 型を返す．
  PtItemType
  type() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // PtGenCase の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 比較式を返す．
  const PtExpr*
  expr() const override;

  /// @brief case item のリストの要素数の取得
  SizeType
  caseitem_num() const override;

  /// @brief case item の取得
  const PtGenCaseItem*
  caseitem(
    SizeType pos ///< [in] 位置 ( 0 <= pos < caseitem_num() )
  ) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 比較式
  const PtExpr* mExpr;

  // case item の配列
  PtiGenCaseItemArray mCaseItemArray;

};


//////////////////////////////////////////////////////////////////////
/// @brief gencaseitemを表すクラス
//////////////////////////////////////////////////////////////////////
class CptGenCaseItem :
  public PtGenCaseItem
{
public:

  /// @brief コンストラクタ
  CptGenCaseItem(
    const FileRegion& file_region,
    PtiExprArray&& label_array,
    PtiDeclHeadArray&& declhead_array,
    PtiItemArray&& item_array
  );

  /// @brief デストラクタ
  ~CptGenCaseItem();


public:
  //////////////////////////////////////////////////////////////////////
  // PtGenCaseItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  FileRegion
  file_region() const override;

  /// @brief ラベルの要素数の取得
  SizeType
  label_num() const override;

  /// @brief ラベルの取得
  const PtExpr*
  label(
    SizeType pos ///< [in] 位置 ( 0 <= pos < label_num() )
  ) const override;

  /// @brief 宣言ヘッダの要素数の取得
  SizeType
  declhead_num() const override;

  /// @brief 宣言ヘッダの取得
  const PtDeclHead*
  declhead(
    SizeType pos ///< [in] 位置 ( 0 <= pos < declhead_num() )
  ) const override;

  /// @brief item リストの要素数の取得
  SizeType
  item_num() const override;

  /// @brief item の取得
  const PtItem*
  item(
    SizeType pos ///< [in] 位置 ( 0 <= pos < item_num() )
  ) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // ラベルの配列
  PtiExprArray mLabelArray;

  // 生成される本体
  CptGenBody mBody;

};


//////////////////////////////////////////////////////////////////////
/// @brief gen_for 文 を表すクラス
//////////////////////////////////////////////////////////////////////
class CptGenFor :
  public CptItem
{
public:

  /// @brief コンストラクタ
  CptGenFor(
    const FileRegion& file_region,
    const char* loop_var,
    const PtExpr* init_expr,
    const PtExpr* cond,
    const PtExpr* next_expr,
    const char* block_name,
    PtiDeclHeadArray&& declhead_array,
    PtiItemArray&& item_array
  );

  /// @brief デストラクタ
  ~CptGenFor();


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  FileRegion
  file_region() const override;

  /// @brief 型を返す．
  PtItemType
  type() const override;

  /// @brief 名前を返す．
  const char*
  name() const override;

  /// @brief 宣言ヘッダの要素数の取得
  SizeType
  declhead_num() const override;

  /// @brief 宣言ヘッダの取得
  const PtDeclHead*
  declhead(
    SizeType pos ///< [in] 位置 ( 0 <= pos < declhead_num() )
  ) const override;

  /// @brief item リストの要素数の取得
  SizeType
  item_num() const override;

  /// @brief item の取得
  const PtItem*
  item(
    SizeType pos ///< [in] 位置 ( 0 <= pos < item_num() )
  ) const override;

  /// @brief 繰り返し制御用の変数名を返す．
  const char*
  loop_var() const override;

  /// @brief 初期化文の右辺を返す．
  const PtExpr*
  init_expr() const override;

  /// @brief 繰り返し条件を返す．
  const PtExpr*
  expr() const override;

  /// @brief 増加文の右辺を返す．
  const PtExpr*
  next_expr() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 名前
  const char* mName;

  // 繰り返し制御用の変数名
  const char* mLoopVar;

  // 初期化文の右辺
  const PtExpr* mInitExpr;

  // 繰り返し条件
  const PtExpr* mCond;

  // 増加文の右辺
  const PtExpr* mNextExpr;

  // 生成される本体
  CptGenBody mBody;

};

END_NAMESPACE_YM_VERILOG

#endif // CPTITEM_H
