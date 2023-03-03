#ifndef PTIARRAY_H
#define PTIARRAY_H

/// @file PtiArray.h
/// @brief PtiArray のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014, 2020, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym/pt/PtP.h"
#include "alloc/Alloc.h"


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
  PtiArray(
  ) : mNum{0},
      mArray{nullptr}
  {
  }

  /// @breif 内容を指定したコンストラクタ
  PtiArray(
    Alloc& alloc,         ///< [in] メモリアロケータ
    const vector<T*>& src ///< [in] ソース
  ) : mNum{src.size()},
      mArray{alloc.get_array<T*>(mNum)}
  {
    for ( int i = 0; i < mNum; ++ i ) {
      mArray[i] = src[i];
    }
  }

  /// @brief 1つの要素からなるコンストラクタ
  PtiArray(
    Alloc& alloc, ///< [in] メモリアロケータ
    T* elem1      ///< [in] 要素1
  ) : mNum{1},
      mArray{alloc.get_array<T*>(1)}
  {
    mArray[0] = elem1;
  }

  /// @brief 2つの要素からなるコンストラクタ
  PtiArray(
    Alloc& alloc, ///< [in] メモリアロケータ
    T* elem1,	  ///< [in] 要素1
    T* elem2      ///< [in] 要素2
  ) : mNum{2},
      mArray{alloc.get_array<T*>(2)}
  {
    mArray[0] = elem1;
    mArray[1] = elem2;
  }

  /// @brief 3つの要素からなるコンストラクタ
  PtiArray(
    Alloc& alloc, ///< [in] メモリアロケータ
    T* elem1,	  ///< [in] 要素1
    T* elem2,	  ///< [in] 要素2
    T* elem3      ///< [in] 要素3
  ) : mNum{3},
      mArray{alloc.get_array<T*>(3)}
  {
    mArray[0] = elem1;
    mArray[1] = elem2;
    mArray[2] = elem3;
  }

  /// @brief 4つの要素からなるコンストラクタ
  PtiArray(
    Alloc& alloc, ///< [in] メモリアロケータ
    T* elem1,	  ///< [in] 要素1
    T* elem2,	  ///< [in] 要素2
    T* elem3,	  ///< [in] 要素3
    T* elem4      ///< [in] 要素4
  ) : mNum{4},
      mArray{alloc.get_array<T*>(4)}
  {
    mArray[0] = elem1;
    mArray[1] = elem2;
    mArray[2] = elem3;
    mArray[3] = elem4;
  }

  /// コピーコンストラクタ
  PtiArray(
    Alloc& alloc,       ///< [in] メモリアロケータ
    const PtiArray& src ///< [in] ソース
  ) : mNum{src.mNum},
      mArray{alloc.get_array<T*>(mNum)}
  {
    for ( int i = 0; i < mNum; ++ i ) {
      mArray[i] = src.mArray[i];
    }
  }

  /// @brief コピー代入演算子は禁止
  PtiArray&
  operator=(
    const PtiArray& src
  ) = delete;

  /// @brief ムーブコンストラクタ
  PtiArray(
    PtiArray&& src ///< [in] ムーブ元
  ) : mNum{src.mNum},
      mArray{src.mArray}
  {
    src.mNum = 0;
    src.mArray = nullptr;
  }

  /// @brief ムーブ代入演算子
  PtiArray&
  operator=(
    PtiArray&& src ///< [in] ムーブ元
  )
  {
    mNum = src.mNum;
    mArray = src.mArray;
    src.mNum = 0;
    src.mArray = nullptr;
    return *this;
  }

  /// @brief デストラクタ
  ~PtiArray() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素数の取得
  /// @return 要素数
  SizeType
  size() const
  {
    return mNum;
  }

  /// @brief 要素の取得
  T*
  operator[](
    SizeType pos ///< [in] 位置番号 ( 0 <= pos < size() )
  ) const
  {
    if ( pos >= size() ) {
      abort();
    }
    ASSERT_COND( 0 <= pos && pos < size() );
    return mArray[pos];
  }

  /// @brief 先頭の反復子を返す．
  iterator
  begin() const
  {
    return &mArray[0];
  }

  /// @brief 末尾の反復子を返す．
  iterator
  end() const
  {
    return &mArray[mNum];
  }

  /// @brief 配列本体の先頭アドレスを返す．
  T**
  _body() const
  {
    return mArray;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 要素数
  SizeType mNum;

  // ポインタ配列本体
  T** mArray;

};

END_NAMESPACE_YM_VERILOG

#endif // PTIARRAY_H
