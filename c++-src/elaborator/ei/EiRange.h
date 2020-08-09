#ifndef EIRANGE_H
#define EIRANGE_H

/// @file EiRange.h
/// @brief EiRange のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "elaborator/ElbRange.h"
#include "ym/pt/PtDecl.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiRange EiRange.h "EiRange.h"
/// @brief ElbRange の実装クラス
//////////////////////////////////////////////////////////////////////
class EiRange :
  public ElbRange
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  EiRange();

  /// @brief デストラクタ
  ~EiRange();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  VpiObjType
  type() const override;

  /// @brief ファイル位置を返す．
  FileRegion
  file_region() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // VlRange の派生クラスに共通な仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素数(ビット幅)を返す．
  SizeType
  size() const override;

  /// @brief MSB の値を返す．
  /// @retval MSB の値 値が確定しているとき
  /// @retval -1 値が確定していない
  int
  left_range_val() const override;

  /// @brief LSB の値を返す．
  /// @retval LSB の値 値が確定しているとき
  /// @retval -1 値が確定していない
  int
  right_range_val() const override;

  /// @brief MSB を表す文字列を返す．
  string
  left_range_string() const override;

  /// @brief LSB を表す文字列を返す．
  string
  right_range_string() const override;

  /// @brief 範囲のチェック
  /// @param[in] index インデックス
  /// @retval true index が範囲内に入っている．
  /// @retval false index が範囲外
  bool
  is_in(int index) const override;

  /// @brief LSB からのオフセット値の取得
  /// @param[in] index インデックス
  /// @param[out] offset index の LSB からのオフセット
  /// @retval true index が範囲内に入っている．
  /// @retval false index が範囲外
  bool
  calc_offset(int index,
	      SizeType& offset) const override;

  /// @brief MSB からのオフセット値の取得
  /// @param[in] index インデックス
  /// @param[out] offset index の MSB からのオフセット
  /// @retval true index が範囲内に入っている．
  /// @retval false index が範囲外
  bool
  calc_roffset(int index,
	       SizeType& offset) const override;

  /// @brief offset の逆関数
  /// @param[in] offset LSB からのオフセット値
  /// @return offset に対応したインデックスを返す．
  int
  index(SizeType offset) const override;

  /// @brief roffset の逆関数
  /// @param[in] roffset MSB からのオフセット値
  /// @return roffset に対応したインデックスを返す．
  int
  rindex(SizeType roffset) const override;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbRange の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値を設定する．
  /// @param[in] src 元となる情報
  void
  set(const ElbRangeSrc& src) override;


public:
  //////////////////////////////////////////////////////////////////////
  // クラスメソッド
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素数(ビット幅)を返す．
  /// @param[in] left 範囲の MSB
  /// @param[in] right 範囲の LSB
  static
  SizeType
  calc_size(int left,
	    int right);

  /// @brief 範囲のチェック
  /// @param[in] left 範囲の MSB
  /// @param[in] right 範囲の LSB
  /// @param[in] index インデックス
  /// @retval true index が範囲内に入っている．
  /// @retval false index が範囲外
  static
  bool
  is_in(int left,
	int right,
	int index);

  /// @brief LSB からのオフセット値の取得
  /// @param[in] left 範囲の MSB
  /// @param[in] right 範囲の LSB
  /// @param[in] index インデックス
  /// @param[out] offset index の LSB からのオフセット
  /// @retval true index が範囲内に入っている．
  /// @retval false index が範囲外
  static
  bool
  calc_offset(int left,
	      int right,
	      int index,
	      SizeType& offset);

  /// @brief MSB からのオフセット値の取得
  /// @param[in] left 範囲の MSB
  /// @param[in] right 範囲の LSB
  /// @param[in] index インデックス
  /// @param[out] offset index の MSB からのオフセット
  /// @retval true index が範囲内に入っている．
  /// @retval false index が範囲外
  static
  bool
  calc_roffset(int left,
	       int right,
	       int index,
	       SizeType& offset);

  /// @brief offset の逆関数
  /// @param[in] left 範囲の MSB
  /// @param[in] right 範囲の LSB
  /// @param[in] offset LSB からのオフセット値
  /// @return offset に対応したインデックスを返す．
  static
  int
  index(int left,
	int right,
	SizeType offset);

  /// @brief roffset の逆関数
  /// @param[in] left 範囲の MSB
  /// @param[in] right 範囲の LSB
  /// @param[in] roffset MSB からのオフセット値
  /// @return roffset に対応したインデックスを返す．
  static
  int
  rindex(int left,
	 int right,
	 SizeType roffset);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // パース木の範囲定義
  const PtRange* mPtRange;

  // 範囲の MSB
  const PtExpr* mLeftRange;

  // 範囲の LSB
  const PtExpr* mRightRange;

  // MSB の値
  int mLeftVal;

  // LSB の値
  int mRightVal;

};


//////////////////////////////////////////////////////////////////////
/// @class EiRangeImpl EiRange.h "EiRange.h"
/// @brief 範囲を表す部品クラス
/// @note ElbRange の派生クラスではない
//////////////////////////////////////////////////////////////////////
class EiRangeImpl
{
public:

  /// @brief コンストラクタ
  EiRangeImpl();

  /// @brief デストラクタ
  ~EiRangeImpl();


public:
  //////////////////////////////////////////////////////////////////////
  // 値をセットする関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値を設定する．
  /// @param[in] left 範囲の MSB の式
  /// @param[in] right 範囲の LSB の式
  /// @param[in] left_val 範囲の MSB の値
  /// @param[in] right_val 範囲の LSB の値
  void
  set(const PtExpr* left,
      const PtExpr* right,
      int left_val,
      int right_val);


public:
  //////////////////////////////////////////////////////////////////////
  // EiRangeImpl の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素数(ビット幅)を返す．
  SizeType
  size() const;

  /// @brief MSB の値を返す．
  int
  left_range_val() const;

  /// @brief LSB の値を返す．
  int
  right_range_val() const;

  /// @brief MSB を表す文字列を返す．
  string
  left_range_string() const;

  /// @brief LSB を表す文字列を返す．
  string
  right_range_string() const;

  /// @brief left_range >= right_range の時に true を返す．
  bool
  is_big_endian() const;

  /// @brief left_range <= right_range の時に true を返す．
  bool
  is_little_endian() const;

  /// @brief 範囲のチェック
  /// @param[in] index インデックス
  /// @retval true index が範囲内に入っている．
  /// @retval false index が範囲外
  bool
  is_in(int index) const;

  /// @brief LSB からのオフセット値の取得
  /// @param[in] index インデックス
  /// @param[out] offset index の LSB からのオフセット
  /// @retval true index が範囲内に入っている．
  /// @retval false index が範囲外
  bool
  calc_offset(int index,
	      SizeType& offset) const;

  /// @brief MSB からのオフセット値の取得
  /// @param[in] index インデックス
  /// @param[out] offset index の MSB からのオフセット
  /// @retval true index が範囲内に入っている．
  /// @retval false index が範囲外
  bool
  calc_roffset(int index,
	       SizeType& offset) const;

  /// @brief offset の逆関数
  /// @param[in] offset LSB からのオフセット値
  /// @return offset に対応したインデックスを返す．
  int
  index(SizeType offset) const;

  /// @brief roffset の逆関数
  /// @param[in] roffset MSB からのオフセット値
  /// @return roffset に対応したインデックスを返す．
  int
  rindex(SizeType roffset) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 範囲の MSB を表す式
  const PtExpr* mLeftRange;

  // 範囲の LSB を表す式
  const PtExpr* mRightRange;

  // MSB の値
  int mLeftVal;

  // LSB の値
  int mRightVal;

};


//////////////////////////////////////////////////////////////////////
/// @class EiRangeArray EiRange.h "EiRange.h"
/// @brief EiRangeImpl の配列 + α
//////////////////////////////////////////////////////////////////////
class EiRangeArray
{
public:

  /// @brief コンストラクタ
  /// @brief dim_size 次元数
  EiRangeArray(SizeType dim_size,
	       EiRange* array);

  // デストラクタ
  ~EiRangeArray();


public:
  //////////////////////////////////////////////////////////////////////
  // 内容にアクセスする関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 次元数を得る．
  SizeType
  size() const;

  /// @brief 要素数を計算する
  /// @return サイズを返す．
  SizeType
  elem_size() const;

  /// @brief pos 番めの範囲を返す．
  /// @param[in] pos 位置番号
  EiRange*
  range(SizeType pos) const;

  /// @brief アドレス(オフセット)からインデックスのリストを作る．
  /// @param[in] offset オフセット
  /// @param[out] index_list インデックスのリスト
  void
  index(SizeType offset,
	vector<int>& index_list) const;

  /// @brief インデックスのリストからオフセットを得る．
  /// @param[in] index_list インデックスのリスト
  /// @param[out] offset index_list の値に対応したオフセット値
  /// @retval true オフセットの計算が正しく行えた．
  /// @retval false index_list のいずれかの値が範囲外だった．
  bool
  calc_offset(const vector<int>& index_list,
	      SizeType& offset) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 次元数
  SizeType mDimSize;

  // 範囲の配列
  EiRange* mArray;

  // 要素数
  SizeType mElemSize;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief サイズを返す．
inline
SizeType
EiRange::calc_size(int left,
		   int right)
{
  SizeType ans = 0;
  if ( left >= right ) {
    ans = left - right + 1;
  }
  else {
    ans = right - left + 1;
  }
  return ans;
}

// index が範囲内に入っていたら true を返す．
// 範囲外の時には false を返す．
inline
bool
EiRange::is_in(int left,
	       int right,
	       int index)
{
  if ( left >= right ) {
    return right <= index && index <= left;
  }
  else {
    return right >= index && index >= left;
  }
}

// @brief LSB からのオフセット値の取得
// @param[in] left 範囲の MSB
// @param[in] right 範囲の LSB
// @param[in] index インデックス
// @param[out] offset index の LSB からのオフセット
// @retval true index が範囲内に入っている．
// @retval false index が範囲外
inline
bool
EiRange::calc_offset(int left,
		     int right,
		     int index,
		     SizeType& offset)
{
  if ( left >= right ) {
    if ( right <= index && index <= left ) {
      offset = index - right;
      return true;
    }
  }
  else {
    if ( right >= index && index >= left ) {
      offset = right - index;
      return true;
    }
  }
  return false;
}

// @brief MSB からのオフセット値の取得
// @param[in] left 範囲の MSB
// @param[in] right 範囲の LSB
// @param[in] index インデックス
// @param[out] offset index の MSB からのオフセット
// @retval true index が範囲内に入っている．
// @retval false index が範囲外
inline
bool
EiRange::calc_roffset(int left,
		      int right,
		      int index,
		      SizeType& offset)
{
  if ( left >= right ) {
    if ( right <= index && index <= left ) {
      offset = left - index;
      return true;
    }
  }
  else {
    if ( right >= index && index >= left ) {
      offset = index - left;
      return true;
    }
  }
  return false;
}

// offset の逆関数
inline
int
EiRange::index(int left,
	       int right,
	       SizeType offset)
{
  if ( left >= right ) {
    return offset + right;
  }
  else {
    return right - offset;
  }
}

// roffset の逆関数
inline
int
EiRange::rindex(int left,
		int right,
		SizeType roffset)
{
  if ( left >= right ) {
    return left - roffset;
  }
  else {
    return roffset + left;
  }
}

// @brief left_range >= right_range の時に true を返す．
inline
bool
EiRangeImpl::is_big_endian() const
{
  return mLeftVal >= mRightVal;
}

// @brief left_range <= right_range の時に true を返す．
inline
bool
EiRangeImpl::is_little_endian() const
{
  return mRightVal >= mLeftVal;
}

// @brief 次元数を得る．
inline
SizeType
EiRangeArray::size() const
{
  return mDimSize;
}

// @brief 要素数を計算する
// @return サイズを返す．
inline
SizeType
EiRangeArray::elem_size() const
{
  return mElemSize;
}

// @brief pos 番めの範囲を返す．
// @param[in] pos 位置番号
inline
EiRange*
EiRangeArray::range(SizeType pos) const
{
  return &mArray[pos];
}


END_NAMESPACE_YM_VERILOG

#endif // EIRANGE_H
