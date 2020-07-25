
/// @file SptGenerate.cc
/// @brief SptGenerate の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "SptItem.h"
#include "parser/SptFactory.h"


BEGIN_NAMESPACE_YM_VERILOG


//////////////////////////////////////////////////////////////////////
// generate 文で生成される要素を持つ本体
//////////////////////////////////////////////////////////////////////

// コンストラクタ
SptGenBody::SptGenBody(const PtDeclHeadArray* declhead_array,
		       const PtItemArray* item_array) :
  mDeclArray{declhead_array},
  mItemArray{item_array}
{
}

// デストラクタ
SptGenBody::~SptGenBody()
{
}


//////////////////////////////////////////////////////////////////////
// generate ブロックの基底クラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
SptGenerate::SptGenerate(const FileRegion& file_region,
			 PtItemType type,
			 const char* name,
			 const PtDeclHeadArray* declhead_array,
			 const PtItemArray* item_array) :
  SptItem{file_region, type},
  mName{name},
  mBody{declhead_array, item_array}
{
}

// デストラクタ
SptGenerate::~SptGenerate()
{
}

// 名前を返す．
const char*
SptGenerate::name() const
{
  return mName;
}

// @brief 宣言のリストの取得
const PtDeclHeadArray*
SptGenerate::declhead_array() const
{
  return mBody.declhead_array();
}

// @brief item リストの取得
const PtItemArray*
SptGenerate::item_array() const
{
  return mBody.item_array();
}


//////////////////////////////////////////////////////////////////////
// gen_if 文 を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
SptGenIf::SptGenIf(const FileRegion& file_region,
		   const PtExpr* cond,
		   const PtDeclHeadArray* then_declhead_array,
		   const PtItemArray* then_item_array,
		   const PtDeclHeadArray* else_declhead_array,
		   const PtItemArray* else_item_array) :
  SptItem{file_region, PtItemType::GenIf},
  mCond{cond},
  mThenBody{then_declhead_array, then_item_array},
  mElseBody{else_declhead_array, else_item_array}
{
}

// デストラクタ
SptGenIf::~SptGenIf()
{
}

// 条件式を返す．
const PtExpr*
SptGenIf::expr() const
{
  return mCond;
}

// @brief 条件が成り立ったときに生成される宣言ヘッダ配列の取得
const PtDeclHeadArray*
SptGenIf::then_declhead_array() const
{
  return mThenBody.declhead_array();
}

// @brief 条件が成り立ったときに生成される item 配列の取得
const PtItemArray*
SptGenIf::then_item_array() const
{
  return mThenBody.item_array();
}

// @brief 条件が成り立たなかったときに生成される宣言ヘッダ配列の取得
const PtDeclHeadArray*
SptGenIf::else_declhead_array() const
{
  return mElseBody.declhead_array();
}

// @brief 条件が成り立たなかったときに生成される item 配列の取得
const PtItemArray*
SptGenIf::else_item_array() const
{
  return mElseBody.item_array();
}


//////////////////////////////////////////////////////////////////////
// caseitem を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
SptGenCaseItem::SptGenCaseItem(const FileRegion& file_region,
			       const PtExprArray* label_array,
			       const PtDeclHeadArray* declhead_array,
			       const PtItemArray* item_array) :
  mFileRegion{file_region},
  mLabelArray{label_array},
  mBody{declhead_array, item_array}
{
}

// デストラクタ
SptGenCaseItem::~SptGenCaseItem()
{
}

// ファイル位置を返す．
FileRegion
SptGenCaseItem::file_region() const
{
  return mFileRegion;
}

// @brief ラベルのリストの取得
const PtExprArray*
SptGenCaseItem::label_list() const
{
  return mLabelArray;
}

// @brief 宣言のリストの取得
const PtDeclHeadArray*
SptGenCaseItem::declhead_array() const
{
  return mBody.declhead_array();
}

// @brief item リストの取得
const PtItemArray*
SptGenCaseItem::item_array() const
{
  return mBody.item_array();
}


//////////////////////////////////////////////////////////////////////
// case 文 を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
SptGenCase::SptGenCase(const FileRegion& file_region,
		       const PtExpr* expr,
		       const PtGenCaseItemArray* item_array) :
  SptItem{file_region, PtItemType::GenCase},
  mExpr{expr},
  mCaseItemArray{item_array}
{
}

// デストラクタ
SptGenCase::~SptGenCase()
{
}

// 比較式を返す．
const PtExpr*
SptGenCase::expr() const
{
  return mExpr;
}

// case item のリストを返す．
const PtGenCaseItemArray*
SptGenCase::caseitem_list() const
{
  return mCaseItemArray;
}


//////////////////////////////////////////////////////////////////////
// gen_for 文 を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
SptGenFor::SptGenFor(const FileRegion& file_region,
		     const char* loop_var,
		     const PtExpr* init_expr,
		     const PtExpr* cond,
		     const PtExpr* next_expr,
		     const char* block_name,
		     const PtDeclHeadArray* declhead_array,
		     const PtItemArray* item_array) :
  SptItem{file_region, PtItemType::GenFor},
  mName{block_name},
  mLoopVar{loop_var},
  mInitExpr{init_expr},
  mCond{cond},
  mNextExpr{next_expr},
  mBody{declhead_array, item_array}
{
}

// デストラクタ
SptGenFor::~SptGenFor()
{
}

// 名前を返す．
const char*
SptGenFor::name() const
{
  return mName;
}

// @brief 宣言のリストの取得
const PtDeclHeadArray*
SptGenFor::declhead_array() const
{
  return mBody.declhead_array();
}

// @brief item リストの取得
const PtItemArray*
SptGenFor::item_array() const
{
  return mBody.item_array();
}

// 繰り返し制御用の変数名を返す．
const char*
SptGenFor::loop_var() const
{
  return mLoopVar;
}

// 初期化文の右辺を返す．
const PtExpr*
SptGenFor::init_expr() const
{
  return mInitExpr;
}

// 繰り返し条件を返す．
const PtExpr*
SptGenFor::expr() const
{
  return mCond;
}

// 増加文の右辺を返す．
const PtExpr*
SptGenFor::next_expr() const
{
  return mNextExpr;
}


//////////////////////////////////////////////////////////////////////
// item 関係の生成
//////////////////////////////////////////////////////////////////////

// @brief generate 文の生成
// @param[in] file_region ファイル位置の情報
// @param[in] declhead_array 宣言のリスト
// @param[in] item_array 要素のリスト
// @return 生成された generate 文
const PtItem*
SptFactory::new_Generate(const FileRegion& file_region,
			 const PtDeclHeadArray* declhead_array,
			 const PtItemArray* item_array)
{
  auto node = new SptGenerate(file_region, PtItemType::Generate, nullptr,
			      declhead_array, item_array);
  return node;
}

// @brief generate block 文の生成
// @param[in] file_region ファイル位置の情報
// @param[in] declhead_array 宣言のリスト
// @param[in] item_array 要素のリスト
// @return 生成された generate block 文
const PtItem*
SptFactory::new_GenBlock(const FileRegion& file_region,
			 const PtDeclHeadArray* declhead_array,
			 const PtItemArray* item_array)
{
  auto node = new SptGenerate(file_region, PtItemType::GenBlock, nullptr,
			      declhead_array, item_array);
  return node;
}

// @brief 名前付き generate block 文の生成
// @param[in] file_region ファイル位置の情報
// @param[in] name 名前
// @param[in] declhead_array 宣言のリスト
// @param[in] item_array 要素のリスト
// @return 生成された generate block 文
const PtItem*
SptFactory::new_GenBlock(const FileRegion& file_region,
			 const char* name,
			 const PtDeclHeadArray* declhead_array,
			 const PtItemArray* item_array)
{
  auto node = new SptGenerate(file_region, PtItemType::GenBlock, name,
			      declhead_array, item_array);
  return node;
}

// @brief generate if 文の生成
// @param[in] file_region ファイル位置の情報
// @param[in] cond 条件を表す式
// @param[in] then_declhead_array 条件が成り立った時に生成する宣言のリスト
// @param[in] then_item_array 条件が成り立った時に生成する要素のリスト
// @param[in] else_declhead_array 条件が成り立たなかった時に生成する宣言のリスト
// @param[in] else_item_array 条件が成り立たなかった時に生成する要素のリスト
// @return 生成された generate if 文
const PtItem*
SptFactory::new_GenIf(const FileRegion& file_region,
		      const PtExpr* cond,
		      const PtDeclHeadArray* then_declhead_array,
		      const PtItemArray* then_item_array,
		      const PtDeclHeadArray* else_declhead_array,
		      const PtItemArray* else_item_array)
{
  auto node = new SptGenIf(file_region, cond,
			   then_declhead_array, then_item_array,
			   else_declhead_array, else_item_array);
  return node;
}

// @brief generate case 文の生成
// @param[in] file_region ファイル位置の情報
// @param[in] expr 選択式
// @param[in] item_array generate case item のリスト
// @return 生成された generate case 文
const PtItem*
SptFactory::new_GenCase(const FileRegion& file_region,
			const PtExpr* expr,
			const PtGenCaseItemArray* item_array)
{
  auto node = new SptGenCase(file_region, expr, item_array);
  return node;
}

// @brief generate case の要素の生成
// @param[in] file_region ファイル位置の情報
// @param[in] label 比較式
// @param[in] declhead_array 宣言のリスト
// @param[in] item_array 要素のリスト
// @return 生成された generate case item
const PtGenCaseItem*
SptFactory::new_GenCaseItem(const FileRegion& file_region,
			    const PtExprArray* label_array,
			    const PtDeclHeadArray* declhead_array,
			    const PtItemArray* item_array)
{
  auto node = new SptGenCaseItem(file_region, label_array,
				 declhead_array, item_array);
  return node;
}

// @brief generate for 文の生成
// @param[in] file_region ファイル位置の情報
// @param[in] loop_var ループ変数
// @param[in] init_expr 初期化式
// @param[in] cond ループ条件式
// @param[in] next_expr 増加式
// @param[in] block_name ブロック名
// @param[in] declhead_array 宣言のリスト
// @param[in] item_array 要素のリスト
// @return 生成された generate for 文
const PtItem*
SptFactory::new_GenFor(const FileRegion& file_region,
		       const char* loop_var,
		       const PtExpr* init_expr,
		       const PtExpr* cond,
		       const PtExpr* next_expr,
		       const char* block_name,
		       const PtDeclHeadArray* declhead_array,
		       const PtItemArray* item_array)
{
  auto node = new SptGenFor(file_region, loop_var, init_expr, cond,
			    next_expr, block_name,
			    declhead_array, item_array);
  return node;
}

END_NAMESPACE_YM_VERILOG
