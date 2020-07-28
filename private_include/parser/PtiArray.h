#ifndef PTIARRAY_H
#define PTIARRAY_H

/// @file PtiArray.h
/// @brief PtiArray のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.


#include "ym/pt/PtP.h"
#include "ym/pt/PtAlloc.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class PtiArray PtiArray.h PtiArray.h
/// @brief ポインタ配列のテンプレートクラス
//////////////////////////////////////////////////////////////////////
template <typename T>
class PtiArray
{
public:

  // 反復子の型
  using iterator = const T**;

public:

  /// @brief 空のコンストラクタ
  ///
  /// 要素を持たない配列を表す．
  PtiArray();

  /// @breif 内容を指定したコンストラクタ
  /// @param[in] alloc メモリアロケータ
  /// @param[in] src ソース
  explicit
  PtiArray(PtAlloc& alloc,
	   const vector<T*>& src);

  /// @brief 1つの要素からなるコンストラクタ
  /// @param[in] alloc メモリアロケータ
  /// @param[in] elem1 要素1
  explicit
  PtiArray(PtAlloc& alloc,
	   T* elem1);

  /// @brief 2つの要素からなるコンストラクタ
  /// @param[in] alloc メモリアロケータ
  /// @param[in] elem1 要素1
  /// @param[in] elem2 要素2
  explicit
  PtiArray(PtAlloc& alloc,
	   T* elem1,
	   T* elem2);

  /// @brief 3つの要素からなるコンストラクタ
  /// @param[in] alloc メモリアロケータ
  /// @param[in] elem1 要素1
  /// @param[in] elem2 要素2
  /// @param[in] elem3 要素3
  explicit
  PtiArray(PtAlloc& alloc,
	   T* elem1,
	   T* elem2,
	   T* elem3);

  /// @brief 4つの要素からなるコンストラクタ
  /// @param[in] alloc メモリアロケータ
  /// @param[in] elem1 要素1
  /// @param[in] elem2 要素2
  /// @param[in] elem3 要素3
  /// @param[in] elem4 要素4
  explicit
  PtiArray(PtAlloc& alloc,
	   T* elem1,
	   T* elem2,
	   T* elem3,
	   T* elem4);

  /// コピーコンストラクタ
  /// @param[in] alloc メモリアロケータ
  /// @param[in] src ソース
  PtiArray(PtAlloc& alloc,
	   const PtiArray& src);

  /// @brief コピー代入演算子は禁止
  /// @param[in] src ソース
  PtiArray&
  operator=(const PtiArray& src) = delete;

  /// @brief ムーブコンストラクタ
  /// @param[in] src ムーブ元
  PtiArray(PtiArray&& src);

  /// @brief ムーブ代入演算子
  /// @param[in] src ムーブ元
  PtiArray&
  operator=(PtiArray&& src);

  /// @brief デストラクタ
  ~PtiArray();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素数の取得
  /// @return 要素数
  SizeType
  size() const;

  /// @brief 要素の取得
  /// @param[in] pos 位置番号 ( 0 <= pos < size() )
  T*
  operator[](SizeType pos) const;

  /// @brief 先頭の反復子を返す．
  iterator
  begin() const;

  /// @brief 末尾の反復子を返す．
  iterator
  end() const;

  /// @brief 配列本体の先頭アドレスを返す．
  T**
  _body() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 要素数
  SizeType mNum;

  // ポインタ配列本体
  T** mArray;

};


//////////////////////////////////////////////////////////////////////
// PtiArray のインライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 空のコンストラクタ
template <typename T>
inline
PtiArray<T>::PtiArray() :
  mNum{0},
  mArray{nullptr}
{
}

// @breif 内容を指定したコンストラクタ
// @param[in] alloc メモリアロケータ
// @param[in] src ソース
template <typename T>
inline
PtiArray<T>::PtiArray(PtAlloc& alloc,
		      const vector<T*>& src) :
  mNum{src.size()},
  mArray{alloc.get_array<T*>(mNum)}
{
  for ( int i = 0; i < mNum; ++ i ) {
    mArray[i] = src[i];
  }
}

// @brief 1つの要素からなるコンストラクタ
// @param[in] alloc メモリアロケータ
// @param[in] elem1 要素1
template <typename T>
inline
PtiArray<T>::PtiArray(PtAlloc& alloc,
		      T* elem1) :
  mNum{1},
  mArray{alloc.get_array<T*>(1)}
{
  mArray[0] = elem1;
}

// @brief 2つの要素からなるコンストラクタ
// @param[in] alloc メモリアロケータ
// @param[in] elem1 要素1
// @param[in] elem2 要素2
template <typename T>
inline
PtiArray<T>::PtiArray(PtAlloc& alloc,
		      T* elem1,
		      T* elem2) :
  mNum{2},
  mArray{alloc.get_array<T*>(2)}
{
  mArray[0] = elem1;
  mArray[1] = elem2;
}

// @brief 3つの要素からなるコンストラクタ
// @param[in] alloc メモリアロケータ
// @param[in] elem1 要素1
// @param[in] elem2 要素2
// @param[in] elem3 要素3
template <typename T>
inline
PtiArray<T>::PtiArray(PtAlloc& alloc,
		      T* elem1,
		      T* elem2,
		      T* elem3) :
  mNum{3},
  mArray{alloc.get_array<T*>(3)}
{
  mArray[0] = elem1;
  mArray[1] = elem2;
  mArray[2] = elem3;
}

// @brief 4つの要素からなるコンストラクタ
// @param[in] alloc メモリアロケータ
// @param[in] elem1 要素1
// @param[in] elem2 要素2
// @param[in] elem3 要素3
// @param[in] elem4 要素4
template <typename T>
inline
PtiArray<T>::PtiArray(PtAlloc& alloc,
		      T* elem1,
		      T* elem2,
		      T* elem3,
		      T* elem4) :
  mNum{4},
  mArray{alloc.get_array<T*>(4)}
{
  mArray[0] = elem1;
  mArray[1] = elem2;
  mArray[2] = elem3;
  mArray[3] = elem4;
}

// コピーコンストラクタ
// @param[in] alloc メモリアロケータ
// @param[in] src ソース
template <typename T>
inline
PtiArray<T>::PtiArray(PtAlloc& alloc,
		      const PtiArray& src) :
  mNum{src.mNum},
  mArray{alloc.get_array<T*>(mNum)}
{
  for ( int i = 0; i < mNum; ++ i ) {
    mArray[i] = src.mArray[i];
  }
}

// @brief ムーブコンストラクタ
// @param[in] src ムーブ元
template <typename T>
inline
PtiArray<T>::PtiArray(PtiArray&& src) :
  mNum{src.mNum},
  mArray{src.mArray}
{
  src.mNum = 0;
  src.mArray = nullptr;
}

// @brief ムーブ代入演算子
// @param[in] src ムーブ元
template <typename T>
inline
PtiArray<T>&
PtiArray<T>::operator=(PtiArray&& src)
{
  mNum = src.mNum;
  mArray = src.mArray;
  src.mNum = 0;
  src.mArray = nullptr;
  return *this;
}

// @brief デストラクタ
template <typename T>
inline
PtiArray<T>::~PtiArray()
{
  // mArray は alloc の管理下にある．
}

// @brief 要素数の取得
// @return 要素数
template <typename T>
inline
SizeType
PtiArray<T>::size() const
{
  return mNum;
}

// @brief 要素の取得
// @param[in] pos 位置番号 ( 0 <= pos < size() )
template <typename T>
inline
T*
PtiArray<T>::operator[](SizeType pos) const
{
  if ( pos >= size() ) {
    abort();
  }
  ASSERT_COND( 0 <= pos && pos < size() );
  return mArray[pos];
}

// @brief 先頭の反復子を返す．
template <typename T>
inline
typename PtiArray<T>::iterator
PtiArray<T>::begin() const
{
  return &mArray[0];
}

// @brief 末尾の反復子を返す．
template <typename T>
inline
typename PtiArray<T>::iterator
PtiArray<T>::end() const
{
  return &mArray[mNum];
}

// @brief 配列本体の先頭アドレスを返す．
template <typename T>
inline
T**
PtiArray<T>::_body() const
{
  return mArray;
}

END_NAMESPACE_YM_VERILOG

#endif // PTIARRAY_H
