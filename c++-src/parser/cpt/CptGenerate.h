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
  CptGenBody(const PtDeclHeadArray* declhead_array,
	     const PtItemArray* item_array);

  /// @brief デストラクタ
  ~CptGenBody();


public:

  /// @brief 宣言ヘッダ配列の取得
  const PtDeclHeadArray*
  declhead_array() const { return mDeclHeadArray; }

  /// @brief item 配列の取得
  const PtItemArray*
  item_array() const { return mItemArray; }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 宣言の配列
  const PtDeclHeadArray* mDeclHeadArray;

  // 要素の配列
  const PtItemArray* mItemArray;

};


//////////////////////////////////////////////////////////////////////
/// @brief generate ブロックの基底クラス
//////////////////////////////////////////////////////////////////////
class CptGenBase :
  public CptItem
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  CptGenBase(const FileRegion& file_region,
	     const PtDeclHeadArray* declhead_array,
	     const PtItemArray* item_array);

  /// @brief デストラクタ
  ~CptGenBase();


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  FileRegion
  file_region() const override;

  /// @brief 宣言ヘッダ配列の取得
  const PtDeclHeadArray*
  declhead_array() const override;

  /// @brief item 配列の取得
  const PtItemArray*
  item_array() const override;


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
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  CptGenerate(const FileRegion& file_region,
	      const PtDeclHeadArray* declhead_array,
	      const PtItemArray* item_array);

  /// @brief デストラクタ
  ~CptGenerate();


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  /// @note ここでは PtItemType::Generate を返す．
  PtItemType
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// @brief generate ブロックの中のブロック
//////////////////////////////////////////////////////////////////////
class CptGenBlock :
  public CptGenBase
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  CptGenBlock(const FileRegion& file_region,
	      const PtDeclHeadArray* declhead_array,
	      const PtItemArray* item_array);

  /// @brief デストラクタ
  ~CptGenBlock();


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  /// @note ここでは PtItemType::GenBlock を返す．
  PtItemType
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// @brief 名前つきの GenBlock
//////////////////////////////////////////////////////////////////////
class CptGenBlockN :
  public CptGenBlock
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  CptGenBlockN(const FileRegion& file_region,
	       const char* name,
	       const PtDeclHeadArray* declhead_array,
	       const PtItemArray* item_array);

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
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  CptGenIf(const FileRegion& file_region,
	   const PtExpr* cond,
	   const PtDeclHeadArray* then_declhead_array,
	   const PtItemArray* then_item_array,
	   const PtDeclHeadArray* else_declhead_array,
	   const PtItemArray* else_item_array);

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
  /// @note ここでは PtItemType::GenIf を返す．
  PtItemType
  type() const override;

  /// @brief 条件式を返す．
  const PtExpr*
  expr() const override;

  /// @brief 条件が成り立ったときに生成される宣言ヘッダ配列の取得
  const PtDeclHeadArray*
  then_declhead_array() const override;

  /// @brief 条件が成り立ったときに生成される item 配列の取得
  const PtItemArray*
  then_item_array() const override;

  /// @brief 条件が成り立たなかったときに生成される宣言ヘッダ配列の取得
  const PtDeclHeadArray*
  else_declhead_array() const override;

  /// @brief 条件が成り立たなかったときに生成される item 配列の取得
  const PtItemArray*
  else_item_array() const override;


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
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  CptGenCase(const FileRegion& file_region,
	     const PtExpr* expr,
	     const PtGenCaseItemArray* item_array);

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
  /// @note ここでは PtItemType::GenCase を返す．
  PtItemType
  type() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // PtGenCase の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 比較式を返す．
  const PtExpr*
  expr() const override;

  /// @brief case item のリストを返す．
  const PtGenCaseItemArray*
  caseitem_list() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 比較式
  const PtExpr* mExpr;

  // case item の配列
  const PtGenCaseItemArray* mCaseItemArray;

};


//////////////////////////////////////////////////////////////////////
/// @brief gencaseitemを表すクラス
//////////////////////////////////////////////////////////////////////
class CptGenCaseItem :
  public PtGenCaseItem
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  CptGenCaseItem(const FileRegion& file_region,
		 const PtExprArray* label_array,
		 const PtDeclHeadArray* declhead_array,
		 const PtItemArray* item_array);

  /// @brief デストラクタ
  ~CptGenCaseItem();


public:
  //////////////////////////////////////////////////////////////////////
  // PtGenCaseItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  FileRegion
  file_region() const override;

  /// @brief ラベルのリストの取得
  const PtExprArray*
  label_list() const override;

  /// @brief 宣言ヘッダ配列の取得
  const PtDeclHeadArray*
  declhead_array() const override;

  /// @brief item 配列の取得
  const PtItemArray*
  item_array() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // ラベルの配列
  const PtExprArray* mLabelArray;

  // 生成される本体
  CptGenBody mBody;

};


//////////////////////////////////////////////////////////////////////
/// @brief gen_for 文 を表すクラス
//////////////////////////////////////////////////////////////////////
class CptGenFor :
  public CptItem
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  CptGenFor(const FileRegion& file_region,
	    const char* loop_var,
	    const PtExpr* init_expr,
	    const PtExpr* cond,
	    const PtExpr* next_expr,
	    const char* block_name,
	    const PtDeclHeadArray* declhead_array,
	    const PtItemArray* item_array);

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
  /// @note ここでは PtItemType::GenFor を返す．
  PtItemType
  type() const override;

  /// @brief 名前を返す．
  const char*
  name() const override;

  /// @brief 宣言ヘッダ配列の取得
  const PtDeclHeadArray*
  declhead_array() const override;

  /// @brief item 配列の取得
  const PtItemArray*
  item_array() const override;

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
