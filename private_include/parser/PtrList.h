#ifndef PTRLIST_H
#define PTRLIST_H

/// @file PtrList.h
/// @brief PtrList のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014, 2020, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "PtiFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

template <typename T1,
	  typename T2>
class PtrList;

//////////////////////////////////////////////////////////////////////
/// @class PtrListCell PtrList.h "PtrList.h"
/// @brief PtrList 用のセル
//////////////////////////////////////////////////////////////////////
template <typename T>
struct PtrListCell
{
  // 本体を指すポインタ
  T* mPtr;

  // リンクポインタ
  PtrListCell* mLink;

};


//////////////////////////////////////////////////////////////////////
/// @class PtrListIterator PtrList.h "PtrList.h"
/// @brief PtrList 用の反復子
//////////////////////////////////////////////////////////////////////
template <typename T>
class PtrListIterator
{
public:

  /// @brief コンストラクタ
  PtrListIterator() = default;

  /// @brief コピーコンストラクタ
  PtrListIterator(
    const PtrListIterator& src ///< [in] ソース
  ) = default;

  /// @brief PtrList が用いるコンストラクタ
  explicit
  PtrListIterator(
    PtrListCell<T>* cell ///< [in] 要素
  ) : mCell{cell}
  {
  }

  /// @brief デストラクタ
  ~PtrListIterator() = default;


public:

  /// @brief 内容を取り出す演算子
  T*
  operator*() const
  {
    if ( mCell ) {
      return mCell->mPtr;
    }
    else {
      return nullptr;
    }
  }

  /// @brief 次の要素を指す．
  PtrListIterator&
  operator++()
  {
    if ( mCell ) {
      mCell = mCell->mLink;
    }
    return *this;
  }

  /// @brief 等価比較演算子
  bool
  operator==(
    const PtrListIterator& right ///< [in] 相手のオペランド
  ) const
  {
    return mCell == right.mCell;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // セル
  PtrListCell<T>* mCell{nullptr};

};


/// @brief 非等価比較演算子
template <typename T>
inline
bool
operator!=(
  const PtrListIterator<T>& left,
  const PtrListIterator<T>& right
)
{
  return !left.operator==(right);
}


//////////////////////////////////////////////////////////////////////
/// @class PtrList PtrList.h "PtrList.h"
/// @brief リスト構造のテンプレートクラス
/// @note T2 は T1 の親クラス
///
/// 単純なリンクトリストの実装
//////////////////////////////////////////////////////////////////////
template <typename T1,
	  typename T2 = T1>
class PtrList
{
public:

  using const_iterator = PtrListIterator<T1>;
  using Cell = PtrListCell<T1>;

public:

  /// @brief コンストラクタ
  PtrList() = default;

  /// @brief デストラクタ
  ~PtrList()
  {
    clear();
  }


public:

  /// @brief リストのクリア
  void
  clear()
  {
    for ( auto cell = mTop; cell; ) {
      auto next{cell->mLink};
      delete cell;
      cell = next;
    }
    mTop = nullptr;
    mEnd = nullptr;
    mNum = 0;
  }

  /// @brief 要素を先頭に追加
  void
  push_front(
    T1* elem ///< [in] 追加する要素
  )
  {
    auto cell = new Cell{elem, nullptr};
    cell->mLink = mTop;
    mTop = cell;
    if ( mEnd == nullptr ) {
      mEnd = cell;
    }
    ++ mNum;
  }

  /// @brief 要素を末尾に追加
  void
  push_back(
    T1* elem ///< [in] 追加する要素
  )
  {
    auto cell = new Cell{elem, nullptr};
    if ( mEnd ) {
      mEnd->mLink = cell;
    }
    else {
      mTop = cell;
    }
    mEnd = cell;
    ++ mNum;
  }

  /// @brief 要素数の取得
  /// @return 要素数
  SizeType
  size() const
  {
    return mNum;
  }

  /// @brief 空の時に true を返す．
  bool
  empty() const
  {
    return mNum == 0;
  }

  /// @brief 先頭の反復子を返す．
  const_iterator
  begin() const
  {
    return PtrListIterator<T1>(mTop);
  }

  /// @brief 末尾の反復子を返す．
  const_iterator
  end() const
  {
    return PtrListIterator<T1>(nullptr);
  }

  /// @brief 先頭の要素を返す．
  T1*
  front() const
  {
    if ( mTop ) {
      return mTop->mPtr;
    }
    else {
      return nullptr;
    }
  }

  /// @brief 末尾の要素を返す．
  T1*
  back() const
  {
    if ( mEnd ) {
      return mEnd->mPtr;
    }
    else {
      return nullptr;
    }
  }


public:
  //////////////////////////////////////////////////////////////////////
  /// 特別な関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容をvectorにコピーする．
  /// @note この処理の後ではリストは空になる．
  vector<T2*>
  to_vector()
  {
    vector<T2*> vec; vec.reserve(mNum);
    for ( auto elem: *this ) {
      vec.push_back(elem);
    }
    clear();
    delete this;
    return vec;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 先頭の要素
  Cell* mTop{nullptr};

  // 末尾の要素
  Cell* mEnd{nullptr};

  // 要素数
  SizeType mNum{0};

};

END_NAMESPACE_YM_VERILOG

#endif // PTRLIST_H
