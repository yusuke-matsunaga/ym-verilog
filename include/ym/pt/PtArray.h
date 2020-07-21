#ifndef YM_PT_PTARRAY_H
#define YM_PT_PTARRAY_H

/// @file ym/pt/PtArray.h
/// @brief PtArray のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym/pt/PtP.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class PtArray PtArray.h <ym/pt/PtArray.h>
/// @brief ポインタ配列のテンプレートクラス
//////////////////////////////////////////////////////////////////////
template <typename T>
class PtArray
{
public:

  // 反復子の型
  using iterator = const T**;

public:

  /// @brief 空のコンストラクタ
  ///
  /// 要素を持たない配列を表す．
  PtArray();

  /// @breif 内容を指定したコンストラクタ
  /// @param[in] src ソース
  explicit
  PtArray(const vector<T*>& src);

  /// @brief 1つの要素からなるコンストラクタ
  /// @param[in] elem1 要素1
  explicit
  PtArray(T* elem1);

  /// @brief 2つの要素からなるコンストラクタ
  /// @param[in] elem1 要素1
  /// @param[in] elem2 要素2
  explicit
  PtArray(T* elem1,
	  T* elem2);

  /// @brief 3つの要素からなるコンストラクタ
  /// @param[in] elem1 要素1
  /// @param[in] elem2 要素2
  /// @param[in] elem3 要素3
  explicit
  PtArray(T* elem1,
	  T* elem2,
	  T* elem3);

  /// @brief 4つの要素からなるコンストラクタ
  /// @param[in] elem1 要素1
  /// @param[in] elem2 要素2
  /// @param[in] elem3 要素3
  /// @param[in] elem4 要素4
  explicit
  PtArray(T* elem1,
	  T* elem2,
	  T* elem3,
	  T* elem4);

  /// コピーコンストラクタ
  /// @param[in] src ソース
  PtArray(const PtArray& src);

  /// @brief コピー代入演算子
  /// @param[in] src ソース
  PtArray&
  operator=(const PtArray& src);

  /// @brief ムーブコンストラクタ
  /// @param[in] src ムーブ元
  PtArray(PtArray&& src);

  /// @brief ムーブ代入演算子
  /// @param[in] src ムーブ元
  PtArray&
  operator=(PtArray&& src);

  /// @brief デストラクタ
  ~PtArray();


public:

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
// PtArray のインライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 空のコンストラクタ
template <typename T>
inline
PtArray<T>::PtArray() :
  mNum{0},
  mArray{nullptr}
{
}

// @breif 内容を指定したコンストラクタ
// @param[in] src ソース
template <typename T>
inline
PtArray<T>::PtArray(const vector<T*>& src) :
  mNum{src.size()},
  mArray{new T*[mNum]}
{
  for ( int i = 0; i < mNum; ++ i ) {
    mArray[i] = src[i];
  }
}

// @brief 1つの要素からなるコンストラクタ
// @param[in] elem1 要素1
template <typename T>
inline
PtArray<T>::PtArray(T* elem1) :
  mNum{1},
  mArray{new T*[1]}
{
  mArray[0] = elem1;
}

// @brief 2つの要素からなるコンストラクタ
// @param[in] elem1 要素1
// @param[in] elem2 要素2
template <typename T>
inline
PtArray<T>::PtArray(T* elem1,
		    T* elem2) :
  mNum{2},
  mArray{new T*[2]}
{
  mArray[0] = elem1;
  mArray[1] = elem2;
}

// @brief 3つの要素からなるコンストラクタ
// @param[in] elem1 要素1
// @param[in] elem2 要素2
// @param[in] elem3 要素3
template <typename T>
inline
PtArray<T>::PtArray(T* elem1,
		    T* elem2,
		    T* elem3) :
  mNum{3},
  mArray{new T*[3]}
{
  mArray[0] = elem1;
  mArray[1] = elem2;
  mArray[2] = elem3;
}

// @brief 4つの要素からなるコンストラクタ
// @param[in] elem1 要素1
// @param[in] elem2 要素2
// @param[in] elem3 要素3
// @param[in] elem4 要素4
template <typename T>
inline
PtArray<T>::PtArray(T* elem1,
		    T* elem2,
		    T* elem3,
		    T* elem4) :
  mNum{4},
  mArray{new T*[4]}
{
  mArray[0] = elem1;
  mArray[1] = elem2;
  mArray[2] = elem3;
  mArray[3] = elem4;
}

// コピーコンストラクタ
// @param[in] src ソース
template <typename T>
inline
PtArray<T>::PtArray(const PtArray& src) :
  mNum{src.mNum},
  mArray{new T*[mNum]}
{
  for ( int i = 0; i < mNum; ++ i ) {
    mArray[i] = src.mArray[i];
  }
}

// @brief コピー代入演算子
// @param[in] src ソース
template <typename T>
inline
PtArray<T>&
PtArray<T>::operator=(const PtArray& src)
{
  if ( this != &src ) {
    if ( mNum != src.mNum ) {
      delete [] mArray;
      mNum = src.mNum;
      mArray = new T*[mNum];
    }
    for ( int i = 0; i < mNum; ++ i ) {
      mArray[i] = src.mArray[i];
    }
  }
  return *this;
}

// @brief ムーブコンストラクタ
// @param[in] src ムーブ元
template <typename T>
inline
PtArray<T>::PtArray(PtArray&& src) :
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
PtArray<T>&
PtArray<T>::operator=(PtArray&& src)
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
PtArray<T>::~PtArray()
{
  delete [] mArray;
}

// @brief 要素数の取得
// @return 要素数
template <typename T>
inline
SizeType
PtArray<T>::size() const
{
  return mNum;
}

// @brief 要素の取得
// @param[in] pos 位置番号 ( 0 <= pos < size() )
template <typename T>
inline
T*
PtArray<T>::operator[](SizeType pos) const
{
  ASSERT_COND( 0 <= pos && pos < size() );

  return mArray[pos];
}

// @brief 先頭の反復子を返す．
template <typename T>
inline
typename PtArray<T>::iterator
PtArray<T>::begin() const
{
  return &mArray[0];
}

// @brief 末尾の反復子を返す．
template <typename T>
inline
typename PtArray<T>::iterator
PtArray<T>::end() const
{
  return &mArray[mNum];
}

// @brief 配列本体の先頭アドレスを返す．
template <typename T>
inline
T**
PtArray<T>::_body() const
{
  return mArray;
}

END_NAMESPACE_YM_VERILOG

#endif // YM_PT_PTARRAY_H
