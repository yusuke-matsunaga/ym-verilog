﻿/// @file Parser_item.cc
/// @brief Parser の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "Parser_int.h"
#include "parser/PtiFactory.h"

#include "ym/pt/PtItem.h"

#include "ym/MsgMgr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// generate 文の生成
//////////////////////////////////////////////////////////////////////

// @brief generate block の開始
void
Parser::init_generate()
{
  push_declhead_list(nullptr);
  push_item_list(nullptr);
}

// @brief generate block の終了
void
Parser::end_generate()
{
  mCurDeclArray = get_decl_array();
  mCurItemArray = get_item_array();

  pop_declhead_list(true);
  pop_item_list(true);
}

// @brief generate 文の生成
// @param[in] fr ファイル位置の情報
const PtItem*
Parser::new_Generate(const FileRegion& fr)
{
  return mFactory.new_Generate(fr, mCurDeclArray, mCurItemArray);
}

// @brief generate block 文の生成
// @param[in] fr ファイル位置の情報
void
Parser::new_GenBlock(const FileRegion& fr)
{
  const PtItem* item = mFactory.new_GenBlock(fr, mCurDeclArray, mCurItemArray);
  mCurItemList->push_back(item);
}

// @brief 名前付き generate block 文の生成
// @param[in] fr ファイル位置の情報
// @param[in] name 名前
void
Parser::new_GenBlock(const FileRegion& fr,
		     const char* name)
{
  const PtItem* item = mFactory.new_GenBlock(fr, name,
					     mCurDeclArray, mCurItemArray);
  mCurItemList->push_back(item);
}

// @brief generate-if の then 節の開始
void
Parser::init_genif()
{
  push_declhead_list(nullptr);
  push_item_list(nullptr);
}

// @brief generate-if の終了
void
Parser::end_genif()
{
  mGenThenDeclArray = get_decl_array();
  mGenThenItemArray = get_item_array();

  pop_declhead_list(true);
  pop_item_list(true);
}

// @brief generate-if の else 節の開始
void
Parser::init_genelse()
{
  push_declhead_list(nullptr);
  push_item_list(nullptr);
}

// @brief generate-if-else の終了
void
Parser::end_genelse()
{
  mGenElseDeclArray = get_decl_array();
  mGenElseItemArray = get_item_array();

  pop_declhead_list(true);
  pop_item_list(true);
}

// @brief generate if 文の生成
// @param[in] fr ファイル位置の情報
// @param[in] cond 条件を表す式
void
Parser::new_GenIf(const FileRegion& fr,
		  const PtExpr* cond)
{
  const PtItem* item = mFactory.new_GenIf(fr, cond,
					  mGenThenDeclArray,
					  mGenThenItemArray,
					  PtDeclHeadArray(),
					  PtItemArray());
  mCurItemList->push_back(item);
}

// @brief generate if 文の生成
// @param[in] fr ファイル位置の情報
// @param[in] cond 条件を表す式
void
Parser::new_GenIfElse(const FileRegion& fr,
		      const PtExpr* cond)
{
  const PtItem* item = mFactory.new_GenIf(fr, cond,
					  mGenThenDeclArray,
					  mGenThenItemArray,
					  mGenElseDeclArray,
					  mGenElseItemArray);
  mCurItemList->push_back(item);
}

// @brief generate case 文の生成
// @param[in] fr ファイル位置の情報
// @param[in] expr 選択式
// @param[in] item_list generate case item のリスト
void
Parser::new_GenCase(const FileRegion& fr,
		    const PtExpr* expr,
		    PtrList<const PtGenCaseItem>* item_list)
{
  const PtItem* item = mFactory.new_GenCase(fr, expr, to_array(item_list));
  mCurItemList->push_back(item);
}

// @brief generate case の要素の生成
// @param[in] fr ファイル位置の情報
// @param[in] label_list 比較式のリスト
// @return 生成された generate case item
const PtGenCaseItem*
Parser::new_GenCaseItem(const FileRegion& fr,
			PtrList<const PtExpr>* label_list)
{
  const PtGenCaseItem* item = mFactory.new_GenCaseItem(fr,
						       to_array(label_list),
						       mCurDeclArray,
						       mCurItemArray);
  return item;
}

// @brief generate for 文の生成
// @param[in] fr ファイル位置の情報
// @param[in] loop_var ループ変数
// @param[in] init_expr 初期化式
// @param[in] cond ループ条件式
// @param[in] inc_var 増加式の左辺の変数
// @param[in] inc_expr 増加式
// @param[in] block_name ブロック名
// @param[in] decl_array 宣言のリスト
// @param[in] item_array 要素のリスト
void
Parser::new_GenFor(const FileRegion& fr,
		   const char* loop_var,
		   const PtExpr* init_expr,
		   const PtExpr* cond,
		   const char* inc_var,
		   const PtExpr* inc_expr,
		   const char* block_name)
{
  if ( strcmp(loop_var, inc_var) == 0 ) {
    const PtItem* item = mFactory.new_GenFor(fr, loop_var,
					     init_expr, cond, inc_expr,
					     block_name,
					     mCurDeclArray,
					     mCurItemArray);
    mCurItemList->push_back(item);
  }
  else {
    ostringstream buf;
    buf << "Lhs of the increment statement ("
	<< inc_var
	<< ") does not match with Lhs of the initial statement ("
	<< loop_var
	<< ")";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    fr,
		    MsgType::Error,
		    "PARSER",
		    buf.str());
  }
}

END_NAMESPACE_YM_VERILOG
