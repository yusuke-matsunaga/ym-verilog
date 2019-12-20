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
  /// @param[in] size サイズ
  /// @param[in] body 配列本体
  PtArray(SizeType size,
	  T** body);

  /// コピーコンストラクタ，コピー代入演算子はデフォルトでOK
  PtArray(const PtArray& src) = default;
  PtArray&
  operator=(const PtArray& src) = default;

  // ムーブ系は意味がないのでどうでもいい．

  /// @brief デストラクタ
  /// @note このクラスではメモリの開放は行わない．
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
// @param[in] size サイズ
// @param[in] body 配列本体
template <typename T>
inline
PtArray<T>::PtArray(SizeType size,
		    T** body) :
  mNum{size},
  mArray{body}
{
}

// @brief デストラクタ
template <typename T>
inline
PtArray<T>::~PtArray()
{
  // mArray は他で確保されていると仮定する．
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
