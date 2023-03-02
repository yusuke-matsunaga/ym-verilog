/// @file Parser_item.cc
/// @brief Parser の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.

#include "parser/Parser.h"
#include "parser/PtiFactory.h"

#include "ym/MsgMgr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// generate 文の生成
//////////////////////////////////////////////////////////////////////

// @brief generate block の開始
void
Parser::init_generate()
{
  push_declhead_list();
  push_item_list();
}

// @brief generate block の終了
void
Parser::end_generate()
{
  mCurDeclArray = pop_declhead_list();
  mCurItemArray = pop_item_list();
}

// @brief generate 文の生成
const PtItem*
Parser::new_Generate(
  const FileRegion& fr
)
{
  auto item = mFactory->new_Generate(fr, mCurDeclArray, mCurItemArray);
  return item;
}

// @brief generate block 文の生成
void
Parser::new_GenBlock(
  const FileRegion& fr
)
{
  auto item = mFactory->new_GenBlock(fr, mCurDeclArray, mCurItemArray);
  add_item(item);
}

// @brief 名前付き generate block 文の生成
void
Parser::new_GenBlock(
  const FileRegion& fr,
  const char* name
)
{
  auto item = mFactory->new_GenBlock(fr, name, mCurDeclArray, mCurItemArray);
  add_item(item);
}

// @brief generate-if の then 節の開始
void
Parser::init_genif()
{
  push_declhead_list();
  push_item_list();
}

// @brief generate-if の終了
void
Parser::end_genif()
{
  mGenThenDeclArray = pop_declhead_list();
  mGenThenItemArray = pop_item_list();
}

// @brief generate-if の else 節の開始
void
Parser::init_genelse()
{
  push_declhead_list();
  push_item_list();
}

// @brief generate-if-else の終了
void
Parser::end_genelse()
{
  mGenElseDeclArray = pop_declhead_list();
  mGenElseItemArray = pop_item_list();
}

// @brief generate if 文の生成
void
Parser::new_GenIf(
  const FileRegion& fr,
  const PtExpr* cond
)
{
  auto item = mFactory->new_GenIf(fr, cond,
				  mGenThenDeclArray,
				  mGenThenItemArray,
				  vector<const PtDeclHead*>{},
				  vector<const PtItem*>{});
  add_item(item);
}

// @brief generate if 文の生成
void
Parser::new_GenIfElse(
  const FileRegion& fr,
  const PtExpr* cond
)
{
  auto item = mFactory->new_GenIf(fr, cond,
				  mGenThenDeclArray,
				  mGenThenItemArray,
				  mGenElseDeclArray,
				  mGenElseItemArray);
  add_item(item);
}

// @brief generate case 文の生成
void
Parser::new_GenCase(
  const FileRegion& fr,
  const PtExpr* expr,
  PtrList<const PtGenCaseItem>* item_list
)
{
  auto item = mFactory->new_GenCase(fr, expr, item_list->to_vector());
  add_item(item);
}

// @brief generate case の要素の生成
const PtGenCaseItem*
Parser::new_GenCaseItem(
  const FileRegion& fr,
  PtrList<const PtExpr>* label_list
)
{
  auto item = mFactory->new_GenCaseItem(fr, label_list->to_vector(),
					mCurDeclArray, mCurItemArray);
  return item;
}

// @brief generate for 文の生成
void
Parser::new_GenFor(
  const FileRegion& fr,
  const char* loop_var,
  const PtExpr* init_expr,
  const PtExpr* cond,
  const char* inc_var,
  const PtExpr* inc_expr,
  const char* block_name
)
{
  if ( strcmp(loop_var, inc_var) == 0 ) {
    auto item = mFactory->new_GenFor(fr, loop_var,
				     init_expr, cond, inc_expr, block_name,
				     mCurDeclArray, mCurItemArray);
    add_item(item);
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
