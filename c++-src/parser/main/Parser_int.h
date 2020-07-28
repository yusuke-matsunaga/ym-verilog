#ifndef PARSER_INT_H
#define PARSER_INT_H

/// @file Parser_int.h
/// @brief Parser の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "parser/Parser.h"


BEGIN_NAMESPACE_YM_VERILOG

// @brief 現在の declhead リストをスタックに積む．
// @param[in] new_declhead 新しく設定する declhead
inline
void
Parser::push_declhead_list()
{
#if 0
  mDeclHeadListStack.push_back(mCurDeclHeadList);
  if ( new_declhead == nullptr ) {
    new_declhead = new vector<PtiDeclHead*>();
  }
  mCurDeclHeadList = new_declhead;
#else
  mDeclHeadListStack.push_back(vector<PtiDeclHead*>());
#endif
}

#if 0
// @brief スタックの末尾を declhead リストに戻す．
// @param[in] delete_top true なら昔の declhead を削除する．
inline
void
Parser::pop_declhead_list(bool delete_top)
{
  if ( delete_top ) {
    delete mCurDeclHeadList;
  }
  mCurDeclHeadList = mDeclHeadListStack.back();
}
#else
// @brief スタックのトップを取り出す．
inline
vector<const PtDeclHead*>
Parser::pop_declhead_list()
{
  auto vec{convert<const PtDeclHead*, PtiDeclHead*>(cur_declhead_list())};
  mDeclHeadListStack.pop_back();
  return vec;
}
#endif

// @brief 現在の item リストをスタックに積む．
// @param[in] new_item 新しく設定する item リスト
inline
void
Parser::push_item_list()
{
#if 0
  mItemListStack.push_back(mCurItemList);
  if ( new_item == nullptr ) {
    new_item = new vector<const PtItem*>();
  }
  mCurItemList = new_item;
#else
  mItemListStack.push_back(vector<const PtItem*>());
#endif
}

#if 0
// @brief スタックの末尾を item リストに戻す．
// @param[in] delete_top true なら昔の item を削除する．
inline
void
Parser::pop_item_list(bool delete_top)
{
  if ( delete_top ) {
    delete mCurItemList;
  }
  mCurItemList = mItemListStack.back();
  mItemListStack.pop_back();
}
#else
inline
// @brief スタックのトップを取り出す．
vector<const PtItem*>
Parser::pop_item_list()
{
  auto vec{cur_item_list()};
  mItemListStack.pop_back();
  return vec;
}
#endif

#if 0
// @brief 宣言リストを配列に変換する．
inline
vector<const PtDeclHead*>
Parser::get_decl_array()
{
  return convert<const PtDeclHead*, PtiDeclHead*>(*mCurDeclHeadList);
}
#endif

#if 0
// @brief item リストを配列に変換する．
inline
vector<const PtItem*>
Parser::get_item_array()
{
  return *mCurItemList;
}
#endif

END_NAMESPACE_YM_VERILOG

#endif // PARSER_INT_H
