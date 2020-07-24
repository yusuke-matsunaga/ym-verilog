#ifndef YM_PT_PTALLOC_H
#define YM_PT_PTALLOC_H

/// @file ym/pt/PtAlloc.h
/// @brief PtAlloc のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2018, 2019 Yusuke Matsunaga
/// All rights reserved.


#include "ym/verilog.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class PtAlloc PtAlloc.h "ym/PtAlloc.h"
/// @brief メモリの管理を行うクラスの基底クラス
//////////////////////////////////////////////////////////////////////
class PtAlloc
{
public:

public:
  //////////////////////////////////////////////////////////////////////
  /// @name コンストラクタ/デストラクタ
  /// @{

  /// @brief コンストラクタ
  PtAlloc() = default;

  /// @brief デストラクタ
  virtual
  ~PtAlloc() = default;

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name メモリの確保/開放を行う関数
  /// @{

  /// @brief n バイトの領域を確保する．
  /// @param[in] n 確保するメモリ量(単位はバイト)
  void*
  get_memory(SizeType n);

  /// @brief n バイトの領域を開放する．
  /// @param[in] n 確保したメモリ量(単位はバイト)
  /// @param[in] blk 開放するメモリ領域の先頭番地
  void
  put_memory(SizeType n,
	     void* blk);

  /// @brief 今までに確保した全ての領域を破棄する．
  /// 個々のオブジェクトのデストラクタなどは起動されない
  /// ので使用には注意が必要
  void
  destroy();

  /// @brief 配列用の領域を確保する(テンプレート)関数
  /// @param[in] num 要素数
  template<typename T>
  T*
  get_array(SizeType num);

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name メモリ量の制限値に関する関数
  /// @{

  /// @brief メモリ量の制限値を設定する．
  /// @param[in] limit 制限値(単位はバイト)
  /// @note limit が 0 の時は制限なし
  void
  set_mem_limit(SizeType limit);

  /// @brief メモリ量の制限値を返す．
  SizeType
  mem_limit() const;

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 統計情報を返す関数
  /// @{

  /// @brief 使用されているメモリ量を返す．
  SizeType
  used_size() const;

  /// @brief used_size() の今までの最大値を返す．
  SizeType
  max_used_size() const;

  /// @brief 実際に確保したメモリ量を返す．
  SizeType
  allocated_size() const;

  /// @brief 実際に確保した回数を返す．
  SizeType
  allocated_count() const;

  /// @brief 内部状態を出力する．
  void
  print_stats(ostream& s) const;

  /// @}
  //////////////////////////////////////////////////////////////////////


protected:
  //////////////////////////////////////////////////////////////////////
  /// @name 継承クラスから用いられる関数
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief 真のアロケート関数
  /// @param[in] n 確保するメモリ量(単位はバイト)
  /// @note 確保した総量が制限値を越えていたら 0 を返す．
  void*
  alloc(SizeType n);

  /// @brief 新のフリー関数
  /// @param[in] n 解放するメモリ量(単位はバイト)
  /// @param[in] blk 解放するメモリ領域
  void
  free(SizeType n,
       void* blk);

  /// @}
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  // 継承クラスで実装する必要がある．
  //////////////////////////////////////////////////////////////////////

  /// @brief 実際にメモリ領域の確保を行う関数
  /// @param[in] n 確保するメモリ量(単位はバイト)
  virtual
  void*
  _get_memory(SizeType n) = 0;

  /// @brief 実際にメモリ領域の開放を行う関数
  /// @param[in] n 確保したメモリ量(単位はバイト)
  /// @param[in] blk 開放するメモリ領域の先頭番地
  virtual
  void
  _put_memory(SizeType n,
	      void* blk) = 0;

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


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

/// @brief 配列用の領域を確保する(テンプレート)関数
/// @param[in] num 要素数
template<typename T>
inline
T*
PtAlloc::get_array(SizeType num)
{
  if ( num <= 0 ) {
    return nullptr;
  }
  void* p = get_memory(sizeof(T) * num);
  return new (p) T[num];
}

// @brief メモリ量の制限値を設定する．
// @param[in] limit 制限値(単位はバイト)
// @note limit が 0 の時は制限なし
inline
void
PtAlloc::set_mem_limit(SizeType limit)
{
  mMemLimit = limit;
}

// @brief メモリ量の制限値を返す．
inline
SizeType
PtAlloc::mem_limit() const
{
  return mMemLimit;
}

// @brief 使用されているメモリ量を返す．
inline
SizeType
PtAlloc::used_size() const
{
  return mUsedSize;
}

// @brief used_size() の今までの最大値を返す．
inline
SizeType
PtAlloc::max_used_size() const
{
  return mMaxUsedSize;
}

// @brief 実際に確保したメモリ量を返す．
inline
SizeType
PtAlloc::allocated_size() const
{
  return mAllocSize;
}

// @brief 実際に確保した回数を返す．
inline
SizeType
PtAlloc::allocated_count() const
{
  return mAllocCount;
}

END_NAMESPACE_YM_VERILOG

#endif // YM_PT_PTALLOC_H
