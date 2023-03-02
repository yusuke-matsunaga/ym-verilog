#ifndef ALLOC_ALLOC_H
#define ALLOC_ALLOC_H

/// @file alloc/Alloc.h
/// @brief Alloc のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2018, 2019, 2020 Yusuke Matsunaga
/// All rights reserved.


#include "ym/verilog.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class Alloc Alloc.h "alloc/Alloc.h"
/// @brief メモリの管理を行うクラスの基底クラス
//////////////////////////////////////////////////////////////////////
class Alloc
{
public:

public:
  //////////////////////////////////////////////////////////////////////
  /// @name コンストラクタ/デストラクタ
  /// @{

  /// @brief コンストラクタ
  Alloc() = default;

  /// @brief デストラクタ
  virtual
  ~Alloc() = default;

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name メモリの確保/開放を行う関数
  /// @{

  /// @brief n バイトの領域を確保する．
  void*
  get_memory(
    SizeType n ///< [in] 確保するメモリ量(単位はバイト)
  );

  /// @brief n バイトの領域を開放する．
  void
  put_memory(
    SizeType n, ///< [in] 確保したメモリ量(単位はバイト)
    void* blk   ///< [in] 開放するメモリ領域の先頭番地
  );

  /// @brief 今までに確保した全ての領域を破棄する．
  ///
  /// 個々のオブジェクトのデストラクタなどは起動されない
  /// ので使用には注意が必要
  void
  destroy();

  /// @brief 配列用の領域を確保する(テンプレート)関数
  template<typename T>
  T*
  get_array(
    SizeType num ///< [in] 要素数
  )
  {
    if ( num <= 0 ) {
      return nullptr;
    }
    void* p = get_memory(sizeof(T) * num);
    return new (p) T[num];
  }

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name メモリ量の制限値に関する関数
  /// @{

  /// @brief メモリ量の制限値を設定する．
  ///
  /// limit が 0 の時は制限なし
  void
  set_mem_limit(
    SizeType limit ///< [in] 制限値(単位はバイト)
  )
  {
    mMemLimit = limit;
  }

  /// @brief メモリ量の制限値を返す．
  SizeType
  mem_limit() const
  {
    return mMemLimit;
  }

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 統計情報を返す関数
  /// @{

  /// @brief 使用されているメモリ量を返す．
  SizeType
  used_size() const
  {
    return mUsedSize;
  }

  /// @brief used_size() の今までの最大値を返す．
  SizeType
  max_used_size() const
  {
    return mMaxUsedSize;
  }

  /// @brief 実際に確保したメモリ量を返す．
  SizeType
  allocated_size() const
  {
    return mAllocSize;
  }

  /// @brief 実際に確保した回数を返す．
  SizeType
  allocated_count() const
  {
    return mAllocCount;
  }

  /// @brief 内部状態を出力する．
  void
  print_stats(
    ostream& s ///< [in] 出力先のストリーム
  ) const;

  /// @}
  //////////////////////////////////////////////////////////////////////


protected:
  //////////////////////////////////////////////////////////////////////
  /// @name 継承クラスから用いられる関数
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief 真のアロケート関数
  ///
  /// 確保した総量が制限値を越えていたら 0 を返す．
  void*
  alloc(
    SizeType n ///< [in] 確保するメモリ量(単位はバイト)
  );

  /// @brief 新のフリー関数
  void
  free(
    SizeType n, ///< [in] 解放するメモリ量(単位はバイト)
    void* blk   ///< [in] 解放するメモリ領域
  );

  /// @}
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  // 継承クラスで実装する必要がある．
  //////////////////////////////////////////////////////////////////////

  /// @brief 実際にメモリ領域の確保を行う関数
  virtual
  void*
  _get_memory(
    SizeType n ///< [in] 確保するメモリ量(単位はバイト)
  ) = 0;

  /// @brief 実際にメモリ領域の開放を行う関数
  virtual
  void
  _put_memory(
    SizeType n, ///< [in] 確保したメモリ量(単位はバイト)
    void* blk   ///< [in] 開放するメモリ領域の先頭番地
  ) = 0;

  /// @brief 実際に destory() の処理を行う関数
  virtual
  void
  _destroy() = 0;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // メモリ量の制限値
  SizeType mMemLimit{0};

  // 使用中のメモリサイズ
  SizeType mUsedSize{0};

  // 使用した最大のメモリサイズ
  SizeType mMaxUsedSize{0};

  // 確保したメモリサイズ
  SizeType mAllocSize{0};

  // 確保した回数
  SizeType mAllocCount{0};

};

END_NAMESPACE_YM_VERILOG

#endif // YM_PT_PTALLOC_H
