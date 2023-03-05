
/// @file ElbStubList.cc
/// @brief ElbStubList の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2020 Yusuke Matsunaga
/// All rights reserved.

#include "elaborator/ElbStubList.h"
#include "ElbStub.h"


BEGIN_NAMESPACE_YM_VERILOG

// @brief デストラクタ
ElbStubList::~ElbStubList()
{
  clear();
}

// @brief 末尾に要素を追加する．
void
ElbStubList::push_back(
  ElbStub* elem
)
{
  mList.push_back(elem);
}

// @brief 空の時 true を返す．
bool
ElbStubList::empty() const
{
  return mList.empty();
}

// @brief 要素の stub を評価する．
void
ElbStubList::eval()
{
  for ( auto stub: mList ) {
    stub->eval();
    delete stub;
  }
  mList.clear();
}

// @brief 内容を空にする．
void
ElbStubList::clear()
{
  for ( auto stub: mList ) {
    delete stub;
  }
  mList.clear();
}

END_NAMESPACE_YM_VERILOG
