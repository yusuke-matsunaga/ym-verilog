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
public:

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
  /// @retval true index が範囲内に入っている．
  /// @retval false index が範囲外
  bool
  is_in(
    int index ///< [in] インデックス
  ) const override;

  /// @brief LSB からのオフセット値の取得
  /// @retval true index が範囲内に入っている．
  /// @retval false index が範囲外
  bool
  calc_offset(
    int index,       ///< [in] インデックス
    SizeType& offset ///< [out] index の LSB からのオフセット
  ) const override;

  /// @brief MSB からのオフセット値の取得
  /// @retval true index が範囲内に入っている．
  /// @retval false index が範囲外
  bool
  calc_roffset(
    int index,       ///< [in] インデックス
    SizeType& offset ///< [out] index の MSB からのオフセット
  ) const override;

  /// @brief offset の逆関数
  /// @return offset に対応したインデックスを返す．
  int
  index(
    SizeType offset ///< [in] LSB からのオフセット値
  ) const override;

  /// @brief roffset の逆関数
  /// @return roffset に対応したインデックスを返す．
  int
  rindex(
    SizeType roffset ///< [in] MSB からのオフセット値
  ) const override;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbRange の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値を設定する．
  void
  set(
    const ElbRangeSrc& src ///< [in] 元となる情報
  ) override;


public:
  //////////////////////////////////////////////////////////////////////
  // クラスメソッド
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素数(ビット幅)を返す．
  static
  SizeType
  calc_size(
    int left, ///< [in] 範囲の MSB
    int right ///< [in] 範囲の LSB
  )
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

  /// @brief 範囲のチェック
  /// @retval true index が範囲内に入っている．
  /// @retval false index が範囲外
  static
  bool
  is_in(
    int left,  ///< [in] 範囲の MSB
    int right, ///< [in] 範囲の LSB
    int index  ///< [in] インデックス
  )
  {
    if ( left >= right ) {
      return right <= index && index <= left;
    }
    else {
      return right >= index && index >= left;
    }
  }

  /// @brief LSB からのオフセット値の取得
  /// @retval true index が範囲内に入っている．
  /// @retval false index が範囲外
  static
  bool
  calc_offset(
    int left,        ///< [in]  範囲の MSB
    int right,       ///< [in]	範囲の LSB
    int index,       ///< [in]	インデックス
    SizeType& offset ///< [out] index の LSB からのオフセット
  )
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

  /// @brief MSB からのオフセット値の取得
  /// @retval true index が範囲内に入っている．
  /// @retval false index が範囲外
  static
  bool
  calc_roffset(
    int left,        ///< [in]  範囲の MSB
    int right,       ///< [in]	範囲の LSB
    int index,       ///< [in]	インデックス
    SizeType& offset ///< [out]	index の MSB からのオフセット
  )
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

  /// @brief offset の逆関数
  /// @return offset に対応したインデックスを返す．
  static
  int
  index(
    int left,       ///< [in] 範囲の MSB
    int right,      ///< [in] 範囲の LSB
    SizeType offset ///< [in] LSB からのオフセット値
  )
  {
    if ( left >= right ) {
      return offset + right;
    }
    else {
      return right - offset;
    }
  }

  /// @brief roffset の逆関数
  /// @return roffset に対応したインデックスを返す．
  static
  int
  rindex(
    int left,        ///< [in] 範囲の MSB
    int right,       ///< [in] 範囲の LSB
    SizeType roffset ///< [in] MSB からのオフセット値
  )
  {
    if ( left >= right ) {
      return left - roffset;
    }
    else {
      return roffset + left;
    }
  }


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
  void
  set(
    const PtExpr* left,  ///< [in] 範囲の MSB の式
    const PtExpr* right, ///< [in] 範囲の LSB の式
    int left_val,        ///< [in] 範囲の MSB の値
    int right_val        ///< [in] 範囲の LSB の値
  );


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
  is_big_endian() const
  {
    return mLeftVal >= mRightVal;
  }

  /// @brief left_range <= right_range の時に true を返す．
  bool
  is_little_endian() const
  {
    return mRightVal >= mLeftVal;
  }

  /// @brief 範囲のチェック
  /// @retval true index が範囲内に入っている．
  /// @retval false index が範囲外
  bool
  is_in(
    int index ///< [in] インデックス
  ) const;

  /// @brief LSB からのオフセット値の取得
  /// @retval true index が範囲内に入っている．
  /// @retval false index が範囲外
  bool
  calc_offset(
    int index,       ///< [in] インデックス
    SizeType& offset ///< [out] index の LSB からのオフセット
  ) const;

  /// @brief MSB からのオフセット値の取得
  /// @retval true index が範囲内に入っている．
  /// @retval false index が範囲外
  bool
  calc_roffset(
    int index,       ///< [in] インデックス
    SizeType& offset ///< [out] index の MSB からのオフセット
  ) const;

  /// @brief offset の逆関数
  /// @return offset に対応したインデックスを返す．
  int
  index(
    SizeType offset ///< [in] LSB からのオフセット値
  ) const;

  /// @brief roffset の逆関数
  /// @return roffset に対応したインデックスを返す．
  int
  rindex(
    SizeType roffset ///< [in] MSB からのオフセット値
  ) const;


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
  EiRangeArray(
    const vector<EiRange>& array ///< [in] 範囲のリスト
  );

  // デストラクタ
  ~EiRangeArray();


public:
  //////////////////////////////////////////////////////////////////////
  // 内容にアクセスする関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 次元数を得る．
  SizeType
  size() const
  {
    return mArray.size();
  }

  /// @brief 要素数を計算する
  /// @return サイズを返す．
  SizeType
  elem_size() const
  {
    return mElemSize;
  }

  /// @brief pos 番めの範囲を返す．
  const EiRange*
  range(
    SizeType pos ///< [in] 位置番号
  ) const
  {
    ASSERT_COND( 0 <= pos && pos < mArray.size() );
    return &mArray[pos];
  }

  /// @brief アドレス(オフセット)からインデックスのリストを作る．
  void
  index(
    SizeType offset,        ///< [in] オフセット
    vector<int>& index_list ///< [out] インデックスのリスト
  ) const;

  /// @brief インデックスのリストからオフセットを得る．
  /// @retval true オフセットの計算が正しく行えた．
  /// @retval false index_list のいずれかの値が範囲外だった．
  bool
  calc_offset(
    const vector<int>& index_list, ///< [in] インデックスのリスト
    SizeType& offset               ///< [out] index_list の値に対応したオフセット値
  ) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 範囲の配列
  vector<EiRange> mArray;

  // 要素数
  SizeType mElemSize;

};

END_NAMESPACE_YM_VERILOG

#endif // EIRANGE_H
