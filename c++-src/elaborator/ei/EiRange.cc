
/// @file EiRange.cc
/// @brief EiRange の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.

#include "ei/EiFactory.h"
#include "ei/EiRange.h"

#include "ym/pt/PtExpr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス EiRange
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiRange::EiRange()
{
}

// @brief デストラクタ
EiRange::~EiRange()
{
}

// @brief 値を設定する．
void
EiRange::set(
  const ElbRangeSrc& src
)
{
  mPtRange = src.pt_range();
  mLeftRange = src.left_range();
  mRightRange = src.right_range();
  mLeftVal = src.left_range_val();
  mRightVal = src.right_range_val();
}

// @brief 型の取得
VpiObjType
EiRange::type() const
{
  return VpiObjType::Range;
}

// @brief ファイル位置を返す．
FileRegion
EiRange::file_region() const
{
  return mPtRange->file_region();
}

// @brief 要素数(ビット幅)を返す．
SizeType
EiRange::size() const
{
  return calc_size(mLeftVal, mRightVal);
}

// @brief MSB の値を返す．
int
EiRange::left_range_val() const
{
  return mLeftVal;
}

// @brief LSB の値を返す．
int
EiRange::right_range_val() const
{
  return mRightVal;
}

// @brief MSB を表す文字列を返す．
string
EiRange::left_range_string() const
{
  return mLeftRange->decompile();
}

// @brief LSB を表す文字列を返す．
string
EiRange::right_range_string() const
{
  return mRightRange->decompile();
}

// @brief 範囲のチェック
bool
EiRange::is_in(
  int index
) const
{
  return is_in(mLeftVal, mRightVal, index);
}

// @brief LSB からのオフセット値の取得
bool
EiRange::calc_offset(
  int index,
  SizeType& offset
) const
{
  return calc_offset(mLeftVal, mRightVal, index, offset);
}

// @brief MSB からのオフセット値の取得
bool
EiRange::calc_roffset(
  int index,
  SizeType& offset
) const
{
  return calc_roffset(mLeftVal, mRightVal, index, offset);
}

// @brief offset の逆関数
int
EiRange::index(
  SizeType offset
) const
{
  return index(mLeftVal, mRightVal, offset);
}

// @brief roffset の逆関数
int
EiRange::rindex(
  SizeType roffset
) const
{
  return rindex(mLeftVal, mRightVal, roffset);
}


//////////////////////////////////////////////////////////////////////
// クラス EiRangeImpl
//////////////////////////////////////////////////////////////////////

// コンストラクタ
EiRangeImpl::EiRangeImpl(
) : mLeftRange{nullptr},
    mRightRange{nullptr}
{
}

// デストラクタ
EiRangeImpl::~EiRangeImpl()
{
}

// @brief 値を設定する．
void
EiRangeImpl::set(
  const PtExpr* left,
  const PtExpr* right,
  int left_val,
  int right_val
)
{
  mLeftRange = left;
  mRightRange = right;
  mLeftVal = left_val;
  mRightVal = right_val;
}

// @brief サイズを返す．
SizeType
EiRangeImpl::size() const
{
  return EiRange::calc_size(mLeftVal, mRightVal);
}

// @brief MSB の値を返す．
int
EiRangeImpl::left_range_val() const
{
  return mLeftVal;
}

// @brief LSB の値を返す．
int
EiRangeImpl::right_range_val() const
{
  return mRightVal;
}

// @brief MSB を表す文字列を返す．
string
EiRangeImpl::left_range_string() const
{
  return mLeftRange->decompile();
}

// @brief LSB を表す文字列を返す．
string
EiRangeImpl::right_range_string() const
{
  return mRightRange->decompile();
}

// index が範囲内に入っていたら true を返す．
bool
EiRangeImpl::is_in(
  int index
) const
{
  return EiRange::is_in(mLeftVal, mRightVal, index);
}

// @brief LSB からのオフセット値の取得
bool
EiRangeImpl::calc_offset(
  int index,
  SizeType& offset
) const
{
  return EiRange::calc_offset(mLeftVal, mRightVal, index, offset);
}

// @brief MSB からのオフセット値の取得
bool
EiRangeImpl::calc_roffset(
  int index,
  SizeType& offset
) const
{
  return EiRange::calc_roffset(mLeftVal, mRightVal, index, offset);
}

// offset の逆関数
int
EiRangeImpl::index(
  SizeType offset
) const
{
  return EiRange::index(mLeftVal, mRightVal, offset);
}

// roffset の逆関数
int
EiRangeImpl::rindex(
  SizeType roffset
) const
{
  return EiRange::rindex(mLeftVal, mRightVal, roffset);
}


//////////////////////////////////////////////////////////////////////
// EiRange の配列
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiRangeArray::EiRangeArray(
  const vector<EiRange>& array
) : mArray{array}
{
  // 要素数を計算する．
  mElemSize = 1;
  for ( const auto& range: mArray ) {
    mElemSize *= range.size();
  }
}

// デストラクタ
EiRangeArray::~EiRangeArray()
{
}

// @brief アドレス(オフセット)からインデックスの配列を作る．
void
EiRangeArray::index(
  SizeType offset,
  vector<int>& index_list
) const
{
  SizeType n = size();
  index_list.resize(n);
  for ( SizeType i = n; i -- > 0; ) {
    auto r = range(i);
    SizeType k = r->size();
    SizeType offset1 = offset % k;
    offset /= k;
    index_list[i] = r->rindex(offset1);
  }
}

// @brief インデックスのリストからオフセットを得る．
bool
EiRangeArray::calc_offset(
  const vector<int>& index_list,
  SizeType& offset
) const
{
  SizeType n = size();
  if ( index_list.size() != n ) {
    // そもそもインデックス配列のサイズが違う．
    return false;
  }

  offset = 0;
  for ( SizeType i = 0; i < n; ++ i ) {
    auto r = range(i);
    SizeType k = r->size();
    offset *= k;
    SizeType offset1;
    if ( r->calc_roffset(index_list[i], offset1) ) {
      offset += offset1;
    }
    else {
      // インデックスが範囲外だった．
      return false;
    }
  }
  return true;
}

END_NAMESPACE_YM_VERILOG
