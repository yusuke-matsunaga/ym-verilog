
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
SptGenBody::SptGenBody(PtiDeclHeadArray&& declhead_array,
		       PtiItemArray&& item_array) :
  mDeclArray{move(declhead_array)},
  mItemArray{move(item_array)}
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
			 PtiDeclHeadArray&& declhead_array,
			 PtiItemArray&& item_array) :
  SptItem{file_region, type},
  mName{name},
  mBody{move(declhead_array), move(item_array)}
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

// @brief 宣言ヘッダの要素数の取得
SizeType
SptGenerate::declhead_num() const
{
  return mBody.declhead_num();
}

// @brief 宣言ヘッダの取得
// @param[in] pos 位置 ( 0 <= pos < declhead_num() )
const PtDeclHead*
SptGenerate::declhead(SizeType pos) const
{
  return mBody.declhead(pos);
}

// @brief item リストの要素数の取得
SizeType
SptGenerate::item_num() const
{
  return mBody.item_num();
}

// @brief item の取得
// @param[in] pos 位置 ( 0 <= pos < item_num() )
const PtItem*
SptGenerate::item(SizeType pos) const
{
  return mBody.item(pos);
}


//////////////////////////////////////////////////////////////////////
// gen_if 文 を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
SptGenIf::SptGenIf(const FileRegion& file_region,
		   const PtExpr* cond,
		   PtiDeclHeadArray&& then_declhead_array,
		   PtiItemArray&& then_item_array,
		   PtiDeclHeadArray&& else_declhead_array,
		   PtiItemArray&& else_item_array) :
  SptItem{file_region, PtItemType::GenIf},
  mCond{cond},
  mThenBody{move(then_declhead_array), move(then_item_array)},
  mElseBody{move(else_declhead_array), move(else_item_array)}
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

// @brief 条件が成り立ったときに生成される宣言ヘッダの要素数の取得
SizeType
SptGenIf::then_declhead_num() const
{
  return mThenBody.declhead_num();
}

// @brief 条件が成り立ったときに生成される宣言ヘッダの取得
// @param[in] pos 位置 ( 0 <= pos < declhead_num() )
const PtDeclHead*
SptGenIf::then_declhead(SizeType pos) const
{
  return mThenBody.declhead(pos);
}

// @brief 条件が成り立ったときに生成される item リストの要素数の取得
SizeType
SptGenIf::then_item_num() const
{
  return mThenBody.item_num();
}

// @brief 条件が成り立ったときに生成される item の取得
// @param[in] pos 位置 ( 0 <= pos < item_num() )
const PtItem*
SptGenIf::then_item(SizeType pos) const
{
  return mThenBody.item(pos);
}

// @brief 条件が成り立たなかったときに生成される宣言ヘッダの要素数の取得
SizeType
SptGenIf::else_declhead_num() const
{
  return mElseBody.declhead_num();
}

// @brief 条件が成り立たなかったときに生成される宣言ヘッダの取得
// @param[in] pos 位置 ( 0 <= pos < declhead_num() )
const PtDeclHead*
SptGenIf::else_declhead(SizeType pos) const
{
  return mElseBody.declhead(pos);
}

// @brief 条件が成り立たなかったときに生成される item リストの要素数の取得
SizeType
SptGenIf::else_item_num() const
{
  return mElseBody.item_num();
}

// @brief 条件が成り立たなかったときに生成される item の取得
// @param[in] pos 位置 ( 0 <= pos < item_num() )
const PtItem*
SptGenIf::else_item(SizeType pos) const
{
  return mElseBody.item(pos);
}


//////////////////////////////////////////////////////////////////////
// caseitem を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
SptGenCaseItem::SptGenCaseItem(const FileRegion& file_region,
			       PtiExprArray&& label_array,
			       PtiDeclHeadArray&& declhead_array,
			       PtiItemArray&& item_array) :
  mFileRegion{file_region},
  mLabelArray{move(label_array)},
  mBody{move(declhead_array), move(item_array)}
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

// @brief ラベルの要素数の取得
SizeType
SptGenCaseItem::label_num() const
{
  return mLabelArray.size();
}

// @brief ラベルの取得
// @param[in] pos 位置 ( 0 <= pos < label_num() )
const PtExpr*
SptGenCaseItem::label(SizeType pos) const
{
  return mLabelArray[pos];
}

// @brief 宣言ヘッダの要素数の取得
SizeType
SptGenCaseItem::declhead_num() const
{
  return mBody.declhead_num();
}

// @brief 宣言ヘッダの取得
// @param[in] pos 位置 ( 0 <= pos < declhead_num() )
const PtDeclHead*
SptGenCaseItem::declhead(SizeType pos) const
{
  return mBody.declhead(pos);
}

// @brief item リストの要素数の取得
SizeType
SptGenCaseItem::item_num() const
{
  return mBody.item_num();
}

// @brief item の取得
// @param[in] pos 位置 ( 0 <= pos < item_num() )
const PtItem*
SptGenCaseItem::item(SizeType pos) const
{
  return mBody.item(pos);
}


//////////////////////////////////////////////////////////////////////
// case 文 を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
SptGenCase::SptGenCase(const FileRegion& file_region,
		       const PtExpr* expr,
		       PtiGenCaseItemArray&& item_array) :
  SptItem{file_region, PtItemType::GenCase},
  mExpr{expr},
  mCaseItemArray{move(item_array)}
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

// @brief case item のリストの要素数の取得
SizeType
SptGenCase::caseitem_num() const
{
  return mCaseItemArray.size();
}

// @brief case item の取得
// @param[in] pos 位置 ( 0 <= pos < caseitem_num() )
const PtGenCaseItem*
SptGenCase::caseitem(SizeType pos) const
{
  return mCaseItemArray[pos];
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
		     PtiDeclHeadArray&& declhead_array,
		     PtiItemArray&& item_array) :
  SptItem{file_region, PtItemType::GenFor},
  mName{block_name},
  mLoopVar{loop_var},
  mInitExpr{init_expr},
  mCond{cond},
  mNextExpr{next_expr},
  mBody{move(declhead_array), move(item_array)}
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

// @brief 宣言ヘッダの要素数の取得
SizeType
SptGenFor::declhead_num() const
{
  return mBody.declhead_num();
}

// @brief 宣言ヘッダの取得
// @param[in] pos 位置 ( 0 <= pos < declhead_num() )
const PtDeclHead*
SptGenFor::declhead(SizeType pos) const
{
  return mBody.declhead(pos);
}

// @brief item リストの要素数の取得
SizeType
SptGenFor::item_num() const
{
  return mBody.item_num();
}

// @brief item の取得
// @param[in] pos 位置 ( 0 <= pos < item_num() )
const PtItem*
SptGenFor::item(SizeType pos) const
{
  return mBody.item(pos);
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
			 const vector<const PtDeclHead*>& declhead_array,
			 const vector<const PtItem*>& item_array)
{
  auto node = new SptGenerate(file_region, PtItemType::Generate, nullptr,
			      PtiDeclHeadArray(mAlloc, declhead_array),
			      PtiItemArray(mAlloc, item_array));
  return node;
}

// @brief generate block 文の生成
// @param[in] file_region ファイル位置の情報
// @param[in] declhead_array 宣言のリスト
// @param[in] item_array 要素のリスト
// @return 生成された generate block 文
const PtItem*
SptFactory::new_GenBlock(const FileRegion& file_region,
			 const vector<const PtDeclHead*>& declhead_array,
			 const vector<const PtItem*>& item_array)
{
  auto node = new SptGenerate(file_region, PtItemType::GenBlock, nullptr,
			      PtiDeclHeadArray(mAlloc, declhead_array),
			      PtiItemArray(mAlloc, item_array));
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
			 const vector<const PtDeclHead*>& declhead_array,
			 const vector<const PtItem*>& item_array)
{
  auto node = new SptGenerate(file_region, PtItemType::GenBlock, name,
			      PtiDeclHeadArray(mAlloc, declhead_array),
			      PtiItemArray(mAlloc, item_array));
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
		      const vector<const PtDeclHead*>& then_declhead_array,
		      const vector<const PtItem*>& then_item_array,
		      const vector<const PtDeclHead*>& else_declhead_array,
		      const vector<const PtItem*>& else_item_array)
{
  auto node = new SptGenIf(file_region, cond,
			   PtiDeclHeadArray(mAlloc, then_declhead_array),
			   PtiItemArray(mAlloc, then_item_array),
			   PtiDeclHeadArray(mAlloc, else_declhead_array),
			   PtiItemArray(mAlloc, else_item_array));
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
			const vector<const PtGenCaseItem*>& item_array)
{
  auto node = new SptGenCase(file_region, expr,
			     PtiGenCaseItemArray(mAlloc, item_array));
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
			    const vector<const PtExpr*>& label_array,
			    const vector<const PtDeclHead*>& declhead_array,
			    const vector<const PtItem*>& item_array)
{
  auto node = new SptGenCaseItem(file_region,
				 PtiExprArray(mAlloc, label_array),
				 PtiDeclHeadArray(mAlloc, declhead_array),
				 PtiItemArray(mAlloc, item_array));
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
		       const vector<const PtDeclHead*>& declhead_array,
		       const vector<const PtItem*>& item_array)
{
  auto node = new SptGenFor(file_region, loop_var, init_expr, cond,
			    next_expr, block_name,
			    PtiDeclHeadArray(mAlloc, declhead_array),
			    PtiItemArray(mAlloc, item_array));
  return node;
}

END_NAMESPACE_YM_VERILOG
