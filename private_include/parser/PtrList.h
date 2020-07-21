#ifndef PTRLIST_H
#define PTRLIST_H

/// @file PtrList.h
/// @brief PtrList のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "PtiFwd.h"
#include "ym/pt/PtArray.h"


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
  PtrListIterator();

  /// @brief コピーコンストラクタ
  PtrListIterator(const PtrListIterator& src);

  /// @brief PtrList が用いるコンストラクタ
  PtrListIterator(PtrListCell<T>* cell);

  /// @brief デストラクタ
  ~PtrListIterator();


public:

  /// @brief 内容を取り出す演算子
  T*
  operator*() const;

  /// @brief 次の要素を指す．
  const PtrListIterator&
  operator++();

  /// @brief 等価比較演算子
  bool
  operator==(const PtrListIterator& right) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // セル
  PtrListCell<T>* mCell;

};


//////////////////////////////////////////////////////////////////////
/// @class PtrList PtrList.h "PtrList.h"
/// @brief リスト構造のテンプレートクラス
/// @note T2 は T1 の親クラス
//////////////////////////////////////////////////////////////////////
template <typename T1,
	  typename T2 = T1>
class PtrList
{
public:

  typedef PtrListIterator<T1> const_iterator;
  typedef PtrListCell<T1> Cell;

public:

  /// @brief コンストラクタ
  /// @note 空のリストを作る
  PtrList();

  /// @brief デストラクタ
  ~PtrList();


public:

  /// @brief リストのクリア
  void
  clear();

  /// @brief 要素の追加
  /// @param[in] elem 追加する要素
  void
  push_back(T1* elem);

  /// @brief 要素数の取得
  /// @return 要素数
  SizeType
  size() const;

  /// @brief 空の時に true を返す．
  bool
  empty() const;

  /// @brief 先頭の反復子を返す．
  PtrListIterator<T1>
  begin() const;

  /// @brief 末尾の反復子を返す．
  PtrListIterator<T1>
  end() const;

  /// @brief 先頭の要素を返す．
  T1*
  front() const;

  /// @brief 末尾の要素を返す．
  T1*
  back() const;


public:
  //////////////////////////////////////////////////////////////////////
  /// 特別な関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を配列にコピーする．
  /// @note この処理の後ではリストは空になる．
  PtArray<T2>
  to_array();

  /// @brief 内容をvectorにコピーする．
  /// @note この処理の後ではリストは空になる．
  vector<T2*>
  to_vector();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 先頭の要素
  Cell* mTop;

  // 末尾の要素
  Cell* mEnd;

  // 要素数
  SizeType mNum;

};


//////////////////////////////////////////////////////////////////////
// PtrListIterator のインライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
template <typename T>
inline
PtrListIterator<T>::PtrListIterator() :
  mCell(nullptr)
{
}

// @brief コピーコンストラクタ
template <typename T>
inline
PtrListIterator<T>::PtrListIterator(const PtrListIterator& src) :
  mCell(src.mCell)
{
}

// @brief PtrList が用いるコンストラクタ
template <typename T>
inline
PtrListIterator<T>::PtrListIterator(PtrListCell<T>* cell) :
  mCell(cell)
{
}

// @brief デストラクタ
template <typename T>
inline
PtrListIterator<T>::~PtrListIterator()
{
}

// @brief 内容を取り出す演算子
template <typename T>
inline
T*
PtrListIterator<T>::operator*() const
{
  if ( mCell ) {
    return mCell->mPtr;
  }
  else {
    return nullptr;
  }
}

// @brief 等価比較演算子
template <typename T>
inline
bool
PtrListIterator<T>::operator==(const PtrListIterator<T>& right) const
{
  return mCell == right.mCell;
}

// @brief 非等価比較演算子
template <typename T>
inline
bool
operator!=(const PtrListIterator<T>& left,
	   const PtrListIterator<T>& right)
{
  return !left.operator==(right);
}

// @brief 次の要素を指す．
template <typename T>
inline
const PtrListIterator<T>&
PtrListIterator<T>::operator++()
{
  if ( mCell ) {
    mCell = mCell->mLink;
  }
  return *this;
}


//////////////////////////////////////////////////////////////////////
// PtrList のインライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @note 空のリストを作る
template <typename T1,
	  typename T2>
inline
PtrList<T1, T2>::PtrList() :
  mTop(nullptr),
  mEnd(nullptr),
  mNum(0)
{
}

// @brief デストラクタ
template <typename T1,
	  typename T2>
inline
PtrList<T1, T2>::~PtrList()
{
  clear();
}

// @brief リストのクリア
template <typename T1,
	  typename T2>
inline
void
PtrList<T1, T2>::clear()
{
  for ( auto cell = mTop; cell; ) {
    auto next = cell->mLink;
    delete cell;
    cell = next;
  }
  mTop = nullptr;
  mEnd = nullptr;
  mNum = 0;
}

// @brief 要素の追加
// @param[in] elem 追加する要素
template <typename T1,
	  typename T2>
inline
void
PtrList<T1, T2>::push_back(T1* elem)
{
  Cell* cell = new Cell;
  cell->mPtr = elem;
  cell->mLink = nullptr;
  if ( mEnd ) {
    mEnd->mLink = cell;
  }
  else {
    mTop = cell;
  }
  mEnd = cell;
  ++ mNum;
}

// @brief 要素数の取得
// @return 要素数
template <typename T1,
	  typename T2>
inline
SizeType
PtrList<T1, T2>::size() const
{
  return mNum;
}

// @brief 空の時に true を返す．
template <typename T1,
	  typename T2>
inline
bool
PtrList<T1, T2>::empty() const
{
  return mNum == 0;
}

// @brief 先頭の反復子を返す．
template <typename T1,
	  typename T2>
inline
PtrListIterator<T1>
PtrList<T1, T2>::begin() const
{
  return PtrListIterator<T1>(mTop);
}

// @brief 先頭の反復子を返す．
template <typename T1,
	  typename T2>
inline
PtrListIterator<T1>
PtrList<T1, T2>::end() const
{
  return PtrListIterator<T1>(nullptr);
}

// @brief 先頭の要素を返す．
template <typename T1,
	  typename T2>
inline
T1*
PtrList<T1, T2>::front() const
{
  if ( mTop ) {
    return mTop->mPtr;
  }
  else {
    return nullptr;
  }
}

// @brief 末尾の要素を返す．
template <typename T1,
	  typename T2>
inline
T1*
PtrList<T1, T2>::back() const
{
  if ( mEnd ) {
    return mEnd->mPtr;
  }
  else {
    return nullptr;
  }
}

// @brief 内容を配列にコピーする．
// @param[in] array 対象の配列
// @note この処理の後ではリストは空になる．
template <typename T1,
	  typename T2>
inline
PtArray<T2>
PtrList<T1, T2>::to_array()
{
  return PtArray<T2>{to_vector()};
}

// @brief 内容を配列にコピーする．
// @param[in] array 対象の配列
// @note この処理の後ではリストは空になる．
template <typename T1,
	  typename T2>
inline
vector<T2*>
PtrList<T1, T2>::to_vector()
{
  vector<T2*> vec(mNum);
  SizeType i = 0;
  for ( Cell* cell = mTop; cell; cell = cell->mLink, ++ i ) {
    vec[i] = cell->mPtr;
  }
  clear();
  return vec;
}

END_NAMESPACE_YM_VERILOG

#endif // PTRLIST_H
