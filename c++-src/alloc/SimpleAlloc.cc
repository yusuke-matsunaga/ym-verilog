
/// @file SimpleAlloc.cc
/// @brief SimpleAlloc の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2018, 2019, 2020 Yusuke Matsunaga
/// All rights reserved.


#include "alloc/SimpleAlloc.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス SimpleAlloc
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SimpleAlloc::SimpleAlloc(SizeType page_size) :
  mPageSize{align(page_size)}
{
  ASSERT_COND( page_size > 0 );
}

// デストラクタ
SimpleAlloc::~SimpleAlloc()
{
  destroy();
}

// @brief n バイトの領域を確保する．
void*
SimpleAlloc::_get_memory(SizeType n)
{
  if ( n > mPageSize ) {
    // mPageSize を越えるものは普通にアロケートする．
    SizeType alloc_size = align(n + sizeof(BigBlock) - sizeof(char));
    auto p = alloc(alloc_size);
    auto block = new (p) BigBlock();
    block->mSize = n;
    mBigBlockList.push_back(block);
    return &block->mDummy[0];
  }
  else {
    // ワード境界に乗るようにサイズに整える．
    SizeType alloc_size = align(n);

    // alloc_size 以上の余りがあるブロックを探す．
    return find_block(alloc_size);
  }
}

// @brief n バイトの領域を開放する．
void
SimpleAlloc::_put_memory(SizeType n,
			 void* block)
{
  // このクラスでは領域の再利用はしない．
  return;
}

// @brief 今までに確保した全ての領域を破棄する．
void
SimpleAlloc::_destroy()
{
  for ( auto chunk: mUsedList ) {
    free(page_size(), chunk);
  }
  mUsedList.clear();

  mPageList.clear();

  for ( auto block: mBigBlockList ) {
    free(block->alloc_size(), block);
  }
  mBigBlockList.clear();
}

// @brief アラインメントを考慮してサイズを調節する．
inline
SizeType
SimpleAlloc::align(SizeType req_size)
{
  // double 型の整列境界値
#if defined(WIN32)
  const SizeType ALIGNOF_DOUBLE = __alignof(double);
#else
  const SizeType ALIGNOF_DOUBLE = __alignof__(double);
#endif
  return ((req_size + ALIGNOF_DOUBLE - 1) / ALIGNOF_DOUBLE) * ALIGNOF_DOUBLE;
}

// @brief 利用可能なブロックを見つける．
void*
SimpleAlloc::find_block(SizeType alloc_size)
{

  for ( auto i = mPageList.begin(); i != mPageList.end(); ++ i ) {
    auto page = *i;
    if ( page->mNextPos + alloc_size <= mPageSize ) {
      // 見つけた．
      auto p = page->alloc(alloc_size);
      if ( page->mNextPos + align(1) > mPageSize ) {
	// ページが一杯になったので削除する．
	mPageList.erase(i);
      }
      return p;
    }
  }

  // 適当なページがなければ新しいページを確保する．
  void* chunk = alloc(page_size());
  mUsedList.push_back(static_cast<char*>(chunk));

  auto page = static_cast<Page*>(chunk);
  page->mNextPos = 0;
  auto p = page->alloc(alloc_size);
  if ( page->mNextPos + align(1) <= mPageSize ) {
    mPageList.push_back(page);
  }

  return p;
}

// @brief 通常ページの実際のメモリサイズ
inline
SizeType
SimpleAlloc::page_size() const
{
  return sizeof(Page) + mPageSize - 1;
}

END_NAMESPACE_YM_VERILOG
