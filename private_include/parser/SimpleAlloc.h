#ifndef YM_SIMPLEALLOC_H
#define YM_SIMPLEALLOC_H

/// @file ym/SimpleAlloc.h
/// @brief SimpleAlloc のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2018, 2019 Yusuke Matsunaga
/// All rights reserved.


#include "parser/Alloc.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class SimpleAlloc SimpleAlloc.h "ym/SimpleAlloc.h"
/// @brief 単純なメモリ管理
//////////////////////////////////////////////////////////////////////
class SimpleAlloc :
  public Alloc
{
public:

  /// @brief コンストラクタ
  /// @param[in] page_size このオブジェクトが管理するメモリ量の単位
  ///
  /// page_size 以上のメモリ領域はデフォルトのアロケーターを使用する．
  explicit
  SimpleAlloc(SizeType page_size = 4096);

  /// @brief デストラクタ
  virtual
  ~SimpleAlloc();


private:
  //////////////////////////////////////////////////////////////////////
  // AllocBase の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief n バイトの領域を確保する．
  /// @param[in] n 確保するメモリ量(単位はバイト)
  void*
  _get_memory(SizeType n) override;

  /// @brief n バイトの領域を開放する．
  /// @param[in] n 確保したメモリ量(単位はバイト)
  /// @param[in] blk 開放するメモリ領域の先頭番地
  void
  _put_memory(SizeType n,
	      void* blk) override;

  /// @brief 今までに確保した全ての領域を破棄する．
  /// 個々のオブジェクトのデストラクタなどは起動されない
  /// ので使用には注意が必要
  void
  _destroy() override;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ構造
  //////////////////////////////////////////////////////////////////////

  struct Page
  {
    // コンストラクタ
    Page(void* p = nullptr) :
      mNextPos{0},
      mChunk{static_cast<char*>(p)}
    {
    }

    // 利用可能な先頭番地
    SizeType mNextPos;

    // メモリチャンク
    char* mChunk;

  };

  struct BigBlock
  {
    // サイズ
    SizeType mSize;

    // 次のブロックを指すリンク
    BigBlock* mNext;

    // 実際にはこの後に確保したメモリ領域がある．
    char mDummy[1];
  };


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief アラインメントを考慮してサイズを調節する．
  static
  SizeType
  align(SizeType req_size);

  /// @brief 利用可能なページを見つける．
  vector<Page>::iterator
  find_page(SizeType alloc_size);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 一度に確保するメモリの単位
  SizeType mPageSize;

  // 使用可能なメモリ領域のリスト
  vector<Page> mAvailList;

  // 確保されたメモリブロックのリスト
  vector<char*> mUsedList;

  // BigBlock の先頭
  BigBlock* mBlockTop;

};

END_NAMESPACE_YM

#endif // YM_SIMPLEALLOC_H
