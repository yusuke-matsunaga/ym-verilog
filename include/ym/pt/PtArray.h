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

  /// @brief 空のコンストラクタ
  PtArray();

  /// @brief コンストラクタ
  /// @param[in] num 要素数
  /// @param[in] array 配列本体
  PtArray(int num,
	  T** array);

  /// @brief デストラクタ
  /// @note このクラスではメモリの開放は行わない．
  ~PtArray();


public:

  /// @brief 要素数の取得
  /// @return 要素数
  int
  size() const;

  /// @brief 要素の取得
  /// @param[in] pos 位置番号 ( 0 <= pos < size() )
  T*
  operator[](int pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 要素数
  int mNum;

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
  mNum(0),
  mArray(nullptr)
{
}

// @brief コンストラクタ
// @param[in] num 要素数
// @param[in] array 配列本体
template <typename T>
inline
PtArray<T>::PtArray(int num,
		    T** array) :
  mNum(num),
  mArray(array)
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
int
PtArray<T>::size() const
{
  return mNum;
}

// @brief 要素の取得
// @param[in] pos 位置番号 ( 0 <= pos < size() )
template <typename T>
inline
T*
PtArray<T>::operator[](int pos) const
{
  return mArray[pos];
}

END_NAMESPACE_YM_VERILOG

#endif // YM_PT_PTARRAY_H
