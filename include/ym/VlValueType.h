#ifndef YM_VLVALUETYPE_H
#define YM_VLVALUETYPE_H

/// @file ym/VlValueType.h
/// @brief VlValueType のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym/verilog.h"


BEGIN_NAMESPACE_YM_VERILOG

/// @brief integer 型のサイズ
const SizeType kVpiSizeInteger = 32U;

/// @brief real 型のサイズ
const SizeType kVpiSizeReal = 64U;

/// @brief time 型のサイズ
const SizeType kVpiSizeTime = 64U;

//////////////////////////////////////////////////////////////////////
/// @class VlValueType VlValueType.h "ym/VlValueType.h"
/// @brief verilog の値の型を表すクラス
//////////////////////////////////////////////////////////////////////
class VlValueType
{
public:

  /// @brief 空のコンストラクタ
  ///
  /// 型の指定なしの意味になる．
  VlValueType() = default;

  /// @brief 内容を指定するコンストラクタ
  VlValueType(
    bool has_sign, ///< [in] 符号の有無
    bool has_size, ///< [in] サイズ指定の有無
    SizeType size  ///< [in] サイズ(ビット幅)
  )
  {
    mData = kSignMask * has_sign |
      kSizeMask * has_size |
      kBvMask |
      (size << kSizeShift);
  }

  /// @brief 整数型を返す．
  static
  VlValueType
  int_type() { return VlValueType(kIntData); }

  /// @brief 実数型を返す．
  static
  VlValueType
  real_type() { return VlValueType(kRealData); }

  /// @brief time 型を返す．
  static
  VlValueType
  time_type() { return VlValueType(kTimeData); }

  /// @brief デストラクタ
  ~VlValueType() = default;


public:

  /// @brief 型の指定がない時に true を返す．
  bool
  is_no_type() const { return (mData & (kBvMask | kRealMask)) == 0U; }

  /// @brief 整数型の時に true を返す．
  bool
  is_int_type() const { return mData == kIntData; }

  /// @brief time 型の時に true を返す．
  bool
  is_time_type() const { return mData == kTimeData; }

  /// @brief ビットベクタ型の時に true を返す．
  ///
  /// 整数型/time型も含む．
  bool
  is_bitvector_type() const { return static_cast<bool>((mData >> kBvBit) & 1U); }

  /// @brief 実数型の時に true を返す．
  bool
  is_real_type() const { return mData == kRealData; }

  /// @brief 符号付きの時に true を返す．
  bool
  is_signed() const { return static_cast<bool>((mData >> kSignBit) & 1U); }

  /// @brief サイズ指定付きの時に true を返す．
  bool
  is_sized() const { return static_cast<bool>((mData >> kSizeBit) & 1U); }

  /// @brief サイズを返す．
  SizeType
  size() const { return (mData >> kSizeShift); }

  /// @brief 等価比較演算子
  bool
  operator==(
    const VlValueType& right ///< [in] オペランド
  ) const
  {
    return mData == right.mData;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値を直接指定するコンストラクタ
  VlValueType(
    SizeType data ///< [in] 内部の値
  ) : mData{data}
  {
  }


private:
  //////////////////////////////////////////////////////////////////////
  // mData の符号化に使われる定数
  //////////////////////////////////////////////////////////////////////

  // 符号ビット
  static
  const int kSignBit = 0;

  // サイズビット
  static
  const int kSizeBit = 1;

  // ビットベクタビット
  static
  const int kBvBit = 2;

  // 実数ビット
  static
  const int kRealBit = 3;

  // サイズシフト
  static
  const int kSizeShift = 4;

  // 符号マスク
  static
  const SizeType kSignMask = 1U << kSignBit;

  // サイズマスク
  static
  const SizeType kSizeMask = 1U << kSizeBit;

  // ビットベクタマスク
  static
  const SizeType kBvMask = 1U << kBvBit;

  // 実数マスク
  static
  const SizeType kRealMask = 1U << kRealBit;

  // 整数を表すパタン
  static
  const SizeType kIntData = kSignMask | kSizeMask | kBvMask |
			   (kVpiSizeInteger << kSizeShift);

  // time 型を表すパタン
  static
  const SizeType kTimeData = kSizeMask | kBvMask |
			    (kVpiSizeTime << kSizeShift);

  // 実数を表すパタン
  static
  const SizeType kRealData = kSignMask | kSizeMask | kRealMask |
                            (kVpiSizeReal << kSizeShift);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 型を表す実体
  SizeType mData{0UL};

};

/// @brief 非等価比較演算子
inline
bool
operator!=(
  const VlValueType& left, ///< [in] 第1オペランド
  const VlValueType& right ///< [in] 第2オペランド
)
{
  return !left.operator==(right);
}

/// @brief 型の内容をストリーム出力する関数
/// @relates VlValueType
inline
ostream&
operator<<(
  ostream& s,             ///< [in] 出力先のストリーム
  const VlValueType& type ///< [in] 型
)
{
  if ( type.is_no_type() ) {
    s << "NO TYPE";
  }
  else if ( type.is_int_type() ) {
    s << "INT TYPE";
  }
  else if ( type.is_real_type() ) {
    s << "REAL TYPE";
  }
  else if ( type.is_time_type() ) {
    s << "TIME TYPE";
  }
  else if ( type.is_bitvector_type() ) {
    if ( type.is_signed() ) {
      s << "SIGNED ";
    }
    else {
      s << "UNSIGNED ";
    }
    if ( type.is_sized() ) {
      s << "SIZED ";
    }
    else {
      s << "UNSIZED ";
    }
    s << type.size() << " BITS TYPE";
  }

  return s;
}

END_NAMESPACE_YM_VERILOG

#endif // YM_VLVALUETYPE_H
