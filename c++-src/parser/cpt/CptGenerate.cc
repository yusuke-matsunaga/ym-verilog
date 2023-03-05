
/// @file CptGenerate.cc
/// @brief CptGenerate の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.

#include "CptGenerate.h"
#include "alloc/Alloc.h"
#include "parser/CptFactory.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// generate 文で生成される要素を持つ本体
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptGenBody::CptGenBody(
  PtiDeclHeadArray&& declhead_array,
  PtiItemArray&& item_array
) : mDeclHeadArray{move(declhead_array)},
    mItemArray{move(item_array)}
{
}

// デストラクタ
CptGenBody::~CptGenBody()
{
}


//////////////////////////////////////////////////////////////////////
// generate ブロックの基底クラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptGenBase::CptGenBase(
  const FileRegion& file_region,
  PtiDeclHeadArray&& declhead_array,
  PtiItemArray&& item_array
) : mFileRegion{file_region},
    mBody{move(declhead_array), move(item_array)}
{
}

// デストラクタ
CptGenBase::~CptGenBase()
{
}

// ファイル位置を返す．
FileRegion
CptGenBase::file_region() const
{
  return mFileRegion;
}

// @brief 宣言ヘッダの要素数の取得
SizeType
CptGenBase::declhead_num() const
{
  return mBody.declhead_num();
}

// @brief 宣言ヘッダの取得
const PtDeclHead*
CptGenBase::declhead(
  SizeType pos
) const
{
  return mBody.declhead(pos);
}

// @brief item リストの要素数の取得
SizeType
CptGenBase::item_num() const
{
  return mBody.item_num();
}

// @brief item の取得
const PtItem*
CptGenBase::item(
  SizeType pos
) const
{
  return mBody.item(pos);
}


//////////////////////////////////////////////////////////////////////
// generate ブロックを表すノード
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptGenerate::CptGenerate(
  const FileRegion& file_region,
  PtiDeclHeadArray&& declhead_array,
  PtiItemArray&& item_array
) : CptGenBase{file_region, move(declhead_array), move(item_array)}
{
}

// デストラクタ
CptGenerate::~CptGenerate()
{
}

// 型を返す．
PtItemType
CptGenerate::type() const
{
  return PtItemType::Generate;
}


//////////////////////////////////////////////////////////////////////
// generate ブロックの中のブロック
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptGenBlock::CptGenBlock(
  const FileRegion& file_region,
  PtiDeclHeadArray&& declhead_array,
  PtiItemArray&& item_array
) : CptGenBase{file_region, move(declhead_array), move(item_array)}
{
}

// デストラクタ
CptGenBlock::~CptGenBlock()
{
}

// 型を返す．
PtItemType
CptGenBlock::type() const
{
  return PtItemType::GenBlock;
}


//////////////////////////////////////////////////////////////////////
// 名前つきの GenBlock
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptGenBlockN::CptGenBlockN(
  const FileRegion& file_region,
  const char* name,
  PtiDeclHeadArray&& declhead_array,
  PtiItemArray&& item_array
) : CptGenBlock{file_region, move(declhead_array), move(item_array)},
    mName{name}
{
}

// デストラクタ
CptGenBlockN::~CptGenBlockN()
{
}

// 名前を返す．
const char*
CptGenBlockN::name() const
{
  return mName;
}


//////////////////////////////////////////////////////////////////////
// gen_if 文 を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptGenIf::CptGenIf(
  const FileRegion& file_region,
  const PtExpr* cond,
  PtiDeclHeadArray&& then_declhead_array,
  PtiItemArray&& then_item_array,
  PtiDeclHeadArray&& else_declhead_array,
  PtiItemArray&& else_item_array
) : mFileRegion{file_region},
    mCond{cond},
    mThenBody{move(then_declhead_array), move(then_item_array)},
    mElseBody{move(else_declhead_array), move(else_item_array)}
{
}

// デストラクタ
CptGenIf::~CptGenIf()
{
}

// ファイル位置を返す．
FileRegion
CptGenIf::file_region() const
{
  return mFileRegion;
}

// 型を返す．
PtItemType
CptGenIf::type() const
{
  return PtItemType::GenIf;
}

// 条件式を返す．
const PtExpr*
CptGenIf::expr() const
{
  return mCond;
}

// @brief 条件が成り立ったときに生成される宣言ヘッダの要素数の取得
SizeType
CptGenIf::then_declhead_num() const
{
  return mThenBody.declhead_num();
}

// @brief 条件が成り立ったときに生成される宣言ヘッダの取得
const PtDeclHead*
CptGenIf::then_declhead(
  SizeType pos
) const
{
  return mThenBody.declhead(pos);
}

// @brief 条件が成り立ったときに生成される item リストの要素数の取得
SizeType
CptGenIf::then_item_num() const
{
  return mThenBody.item_num();
}

// @brief 条件が成り立ったときに生成される item の取得
const PtItem*
CptGenIf::then_item(
  SizeType pos
) const
{
  return mThenBody.item(pos);
}

// @brief 条件が成り立たなかったときに生成される宣言ヘッダの要素数の取得
SizeType
CptGenIf::else_declhead_num() const
{
  return mElseBody.declhead_num();
}

// @brief 条件が成り立たなかったときに生成される宣言ヘッダの取得
const PtDeclHead*
CptGenIf::else_declhead(
  SizeType pos
) const
{
  return mElseBody.declhead(pos);
}

// @brief 条件が成り立たなかったときに生成される item リストの要素数の取得
SizeType
CptGenIf::else_item_num() const
{
  return mElseBody.item_num();
}

// @brief 条件が成り立たなかったときに生成される item の取得
const PtItem*
CptGenIf::else_item(
  SizeType pos
) const
{
  return mElseBody.item(pos);
}


//////////////////////////////////////////////////////////////////////
// caseitem を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptGenCaseItem::CptGenCaseItem(
  const FileRegion& file_region,
  PtiExprArray&& label_array,
  PtiDeclHeadArray&& declhead_array,
  PtiItemArray&& item_array
) : mFileRegion{file_region},
    mLabelArray{move(label_array)},
    mBody{move(declhead_array), move(item_array)}
{
}

// デストラクタ
CptGenCaseItem::~CptGenCaseItem()
{
}

// ファイル位置を返す．
FileRegion
CptGenCaseItem::file_region() const
{
  return mFileRegion;
}

// @brief ラベルの要素数の取得
SizeType
CptGenCaseItem::label_num() const
{
  return mLabelArray.size();
}

// @brief ラベルの取得
const PtExpr*
CptGenCaseItem::label(
  SizeType pos
) const
{
  return mLabelArray[pos];
}

// @brief 宣言ヘッダの要素数の取得
SizeType
CptGenCaseItem::declhead_num() const
{
  return mBody.declhead_num();
}

// @brief 宣言ヘッダの取得
const PtDeclHead*
CptGenCaseItem::declhead(
  SizeType pos
) const
{
  return mBody.declhead(pos);
}

// @brief item リストの要素数の取得
SizeType
CptGenCaseItem::item_num() const
{
  return mBody.item_num();
}

// @brief item の取得
const PtItem*
CptGenCaseItem::item(
  SizeType pos
) const
{
  return mBody.item(pos);
}


//////////////////////////////////////////////////////////////////////
// case 文 を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptGenCase::CptGenCase(
  const FileRegion& file_region,
  const PtExpr* expr,
  PtiGenCaseItemArray&& item_array
) : mFileRegion{file_region},
    mExpr{expr},
    mCaseItemArray{move(item_array)}
{
}

// デストラクタ
CptGenCase::~CptGenCase()
{
}

// ファイル位置を返す．
FileRegion
CptGenCase::file_region() const
{
  return mFileRegion;
}

// 型を返す．
PtItemType
CptGenCase::type() const
{
  return PtItemType::GenCase;
}

// 比較式を返す．
const PtExpr*
CptGenCase::expr() const
{
  return mExpr;
}

// @brief case item のリストの要素数の取得
SizeType
CptGenCase::caseitem_num() const
{
  return mCaseItemArray.size();
}

// @brief case item の取得
const PtGenCaseItem*
CptGenCase::caseitem(
  SizeType pos
) const
{
  return mCaseItemArray[pos];
}


//////////////////////////////////////////////////////////////////////
// gen_for 文 を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptGenFor::CptGenFor(
  const FileRegion& file_region,
  const char* loop_var,
  const PtExpr* init_expr,
  const PtExpr* cond,
  const PtExpr* next_expr,
  const char* block_name,
  PtiDeclHeadArray&& declhead_array,
  PtiItemArray&& item_array
) : mFileRegion{file_region},
    mName{block_name},
    mLoopVar{loop_var},
    mInitExpr{init_expr},
    mCond{cond},
    mNextExpr{next_expr},
    mBody{move(declhead_array), move(item_array)}
{
}

// デストラクタ
CptGenFor::~CptGenFor()
{
}

// ファイル位置を返す．
FileRegion
CptGenFor::file_region() const
{
  return mFileRegion;
}

// 型を返す．
PtItemType
CptGenFor::type() const
{
  return PtItemType::GenFor;
}

// 名前を返す．
const char*
CptGenFor::name() const
{
  return mName;
}

// @brief 宣言ヘッダの要素数の取得
SizeType
CptGenFor::declhead_num() const
{
  return mBody.declhead_num();
}

// @brief 宣言ヘッダの取得
const PtDeclHead*
CptGenFor::declhead(
  SizeType pos
) const
{
  return mBody.declhead(pos);
}

// @brief item リストの要素数の取得
SizeType
CptGenFor::item_num() const
{
  return mBody.item_num();
}

// @brief item の取得
const PtItem*
CptGenFor::item(
  SizeType pos
) const
{
  return mBody.item(pos);
}

// 繰り返し制御用の変数名を返す．
const char*
CptGenFor::loop_var() const
{
  return mLoopVar;
}

// 初期化文の右辺を返す．
const PtExpr*
CptGenFor::init_expr() const
{
  return mInitExpr;
}

// 繰り返し条件を返す．
const PtExpr*
CptGenFor::expr() const
{
  return mCond;
}

// 増加文の右辺を返す．
const PtExpr*
CptGenFor::next_expr() const
{
  return mNextExpr;
}


//////////////////////////////////////////////////////////////////////
// generate 文関係
//////////////////////////////////////////////////////////////////////

// generate 文を生成する．
const PtItem*
CptFactory::new_Generate(
  const FileRegion& file_region,
  const vector<const PtDeclHead*>& declhead_array,
  const vector<const PtItem*>& item_array
)
{
  ++ mNumGenerate;
  void* p = mAlloc.get_memory(sizeof(CptGenerate));
  auto obj = new (p) CptGenerate{file_region,
				 PtiArray<const PtDeclHead>{mAlloc, declhead_array},
				 PtiArray<const PtItem>{mAlloc, item_array}};
  return obj;
}

// generate block 文を生成する．
const PtItem*
CptFactory::new_GenBlock(
  const FileRegion& file_region,
  const vector<const PtDeclHead*>& declhead_array,
  const vector<const PtItem*>& item_array
)
{
  ++ mNumGenBlock;
  void* p = mAlloc.get_memory(sizeof(CptGenBlock));
  auto obj = new (p) CptGenBlock{file_region,
				 PtiArray<const PtDeclHead>{mAlloc, declhead_array},
				 PtiArray<const PtItem>{mAlloc, item_array}};
  return obj;
}

// generate block 文を生成する．
const PtItem*
CptFactory::new_GenBlock(
  const FileRegion& file_region,
  const char* name,
  const vector<const PtDeclHead*>& declhead_array,
  const vector<const PtItem*>& item_array
)
{
  ++ mNumGenBlockN;
  void* p = mAlloc.get_memory(sizeof(CptGenBlockN));
  auto obj = new (p) CptGenBlockN{file_region, name,
				  PtiArray<const PtDeclHead>{mAlloc, declhead_array},
				  PtiArray<const PtItem>{mAlloc, item_array}};
  return obj;
}

// generate if 文を生成する．
const PtItem*
CptFactory::new_GenIf(
  const FileRegion& file_region,
  const PtExpr* cond,
  const vector<const PtDeclHead*>& then_declhead_array,
  const vector<const PtItem*>& then_item_array,
  const vector<const PtDeclHead*>& else_declhead_array,
  const vector<const PtItem*>& else_item_array
)
{
  ++ mNumGenIf;
  void* p = mAlloc.get_memory(sizeof(CptGenIf));
  auto obj = new (p) CptGenIf{file_region, cond,
			      PtiArray<const PtDeclHead>{mAlloc, then_declhead_array},
			      PtiArray<const PtItem>{mAlloc, then_item_array},
			      PtiArray<const PtDeclHead>{mAlloc, else_declhead_array},
			      PtiArray<const PtItem>{mAlloc, else_item_array}};
  return obj;
}

// generate case 文を生成する．
const PtItem*
CptFactory::new_GenCase(
  const FileRegion& file_region,
  const PtExpr* expr,
  const vector<const PtGenCaseItem*>& item_array
)
{
  ++ mNumGenCase;
  void* p = mAlloc.get_memory(sizeof(CptGenCase));
  auto obj = new (p) CptGenCase{file_region, expr,
				PtiArray<const PtGenCaseItem>{mAlloc, item_array}};
  return obj;
}

// generate case の要素を生成する．
const PtGenCaseItem*
CptFactory::new_GenCaseItem(
  const FileRegion& file_region,
  const vector<const PtExpr*>& label_array,
  const vector<const PtDeclHead*>& declhead_array,
  const vector<const PtItem*>& item_array
)
{
  ++ mNumGenCaseItem;
  void* p = mAlloc.get_memory(sizeof(CptGenCaseItem));
  auto obj = new (p) CptGenCaseItem{file_region,
				    PtiArray<const PtExpr>{mAlloc, label_array},
				    PtiArray<const PtDeclHead>{mAlloc, declhead_array},
				    PtiArray<const PtItem>{mAlloc, item_array}};
  return obj;
}

// generate for 文を生成する．
const PtItem*
CptFactory::new_GenFor(
  const FileRegion& file_region,
  const char* loop_var,
  const PtExpr* init_expr,
  const PtExpr* cond,
  const PtExpr* next_expr,
  const char* block_name,
  const vector<const PtDeclHead*>& declhead_array,
  const vector<const PtItem*>& item_array
)
{
  ++ mNumGenFor;
  void* p = mAlloc.get_memory(sizeof(CptGenFor));
  auto obj = new (p) CptGenFor{file_region, loop_var, init_expr, cond,
			       next_expr, block_name,
			       PtiArray<const PtDeclHead>{mAlloc, declhead_array},
			       PtiArray<const PtItem>{mAlloc, item_array}};
  return obj;
}

END_NAMESPACE_YM_VERILOG
