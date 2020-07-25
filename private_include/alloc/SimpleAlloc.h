#ifndef YM_SIMPLEALLOC_H
#define YM_SIMPLEALLOC_H

/// @file SimpleAlloc.h
/// @brief SimpleAlloc のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2018, 2019 Yusuke Matsunaga
/// All rights reserved.


#include "ym/pt/PtAlloc.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class SimpleAlloc SimpleAlloc.h "ym/SimpleAlloc.h"
/// @brief 単純なメモリ管理
//////////////////////////////////////////////////////////////////////
class SimpleAlloc :
  public PtAlloc
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
  // 内部で用いられる下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief アラインメントを考慮してサイズを調節する．
  static
  SizeType
  align(SizeType req_size);

  /// @brief 利用可能なブロックを見つける．
  void*
  find_block(SizeType alloc_size);

  /// @brief 通常ページの実際のメモリサイズ
  SizeType
  page_size() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ構造
  //////////////////////////////////////////////////////////////////////

  // mPageSize のメモリブロックを管理するための構造体
  // 実際には mPageSize + sizeof(Page) のメモリを確保する．
  struct Page
  {
    // size のメモリを確保する．
    void*
    alloc(SizeType size)
    {
      auto p{static_cast<void*>(&mDummy[mNextPos])};
      mNextPos += size;
      return p;
    }

    // 利用可能な先頭番地
    SizeType mNextPos;

    // 実際にはこの後に確保したメモリ領域がある．
    char mDummy[1];
  };

  // mPageSize 以上のメモリブロックを管理するための構造体
  // size のメモリブロックを確保する場合，実際には
  // sizeof(BigBlock) + size - 1 のメモリを確保する．
  struct BigBlock
  {
    // 実際に確保したメモリサイズを返す．
    SizeType
    alloc_size() const
    {
      return sizeof(BigBlock) + mSize - 1;
    }

    // サイズ
    SizeType mSize;

    // 実際にはこの後に確保したメモリ領域がある．
    char mDummy[1];
  };


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 一度に確保するメモリの単位
  SizeType mPageSize;

  // ページのリスト
  vector<Page*> mPageList;

  // 確保されたメモリブロックのリスト
  vector<char*> mUsedList;

  // BigBlock のリスト
  vector<BigBlock*> mBigBlockList;

};

END_NAMESPACE_YM_VERILOG

#endif // YM_SIMPLEALLOC_H
