#ifndef YM_VLTIME_H
#define YM_VLTIME_H

/// @file ym/VlTime.h
/// @brief VlTime のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym/verilog.h"
#include "ym/VlScalarVal.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class VlTime VlTime.h "ym/VlTime.h"
/// @brief シミュレーション時刻を表すクラス
//////////////////////////////////////////////////////////////////////
class VlTime
{
public:

  /// @brief 空のコンストラクタ
  ///
  /// 値は不定
  VlTime() = default;

  /// @brief 2つの値を指定するコンストラクタ
  VlTime(
    PLI_UINT32 l, ///< [in] 下位32ビットの値
    PLI_UINT32 h  ///< [in] 上位32ビットの値
  )
  {
    ymuint64 tmp = h;
    mValue = (tmp << 32) | l;
  }

  /// @brief 符号なし整数からの変換コンストラクタ
  explicit
  VlTime(
    unsigned int val ///< [in] 値
  ) : mValue(val)
  {
  }

  /// @brief double からの変換コンストラクタ
  explicit
  VlTime(
    double val ///< [in] 値
  )
  {
    set(val);
  }

  /// @brief コピーコンストラクタ
  VlTime(
    const VlTime& src ///< [in] コピー元の値
  ) = default;

  /// @brief 代入演算子
  VlTime&
  operator=(
    const VlTime& src ///< [in] コピー元の値
  ) = default;

  /// @brief 加算つき代入演算子
  const VlTime&
  operator+=(
    const VlTime& src ///< [in] オペランド
  )
  {
    mValue += src.mValue;
    return *this;
  }

  /// @brief デストラクタ
  ~VlTime() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 値を設定する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 2つの値を指定する．
  void
  set(
    PLI_UINT32 l, ///< [in] 下位32ビットの値
    PLI_UINT32 h  ///< [in] 上位32ビットの値
  )
  {
    ymuint64 tmp = h;
    mValue = (tmp << 32) | l;
  }

  /// @brief 符号なし整数の値を設定する．
  void
  set(
    unsigned int val ///< [in] 値
  )
  {
    mValue = val;
  }

  /// @brief double の値を設定する．
  void
  set(
    double val ///< [in] 値
  )
  {
    if ( val < 0.0 ) {
      // 負数は0にする．
      mValue = 0UL;
    }
    else {
      double val1 = rint(val);
      mValue = static_cast<ymuint64>(val1);
    }
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 値を取り出す関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 64ビットの値を取り出す．
  ymuint64
  value() const { return mValue; }

  /// @brief 下位32ビットの値を取り出す．
  PLI_UINT32
  low() const { return mValue & 0xFFFFFFFFUL; }

  /// @brief 上位32ビットの値を取り出す．
  PLI_UINT32
  high() const { return (mValue >> 32); }

  /// @brief 符号なし整数への変換
  ///
  /// 符号なし整数に入りきらない値の時はおかしな値になる．
  unsigned int
  to_uint() const { return static_cast<unsigned int>(mValue); }

  /// @brief 論理値への変換
  /// @retval 0 値が 0
  /// @retval 1 値が 0 以外
  VlScalarVal
  to_logic() const
  {
    if ( mValue != 0UL ) {
      return VlScalarVal::one();
    }
    else {
      return VlScalarVal::zero();
    }
  }

  /// @brief double への変換
  double
  to_real() const
  {
    return static_cast<double>(mValue);
  }


public:
  //////////////////////////////////////////////////////////////////////
  // その他の関数
  //////////////////////////////////////////////////////////////////////

  // ハッシュ関数
  SizeType
  hash() const
  {
    return static_cast<SizeType>((mValue * mValue) >> 24);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 64ビットの値
  ymuint64 mValue{0UL};

};


//////////////////////////////////////////////////////////////////////
/// @name VlTime に関連する関数
/// @{

/// @brief 加算
/// @return op1 + op2
inline
VlTime
operator+(
  VlTime op1, ///< [in] 第1オペランド
  VlTime op2  ///< [in] 第2オペランド
)
{
  return VlTime(op1).operator+=(op2);
}

/// @brief 等価比較
/// @return op1 と op2 が等しいとき true を返す．
inline
bool
operator==(
  VlTime op1, ///< [in] 第1オペランド
  VlTime op2  ///< [in] 第2オペランド
)
{
  if ( op1.value() == op2.value() ) {
    return true;
  }
  else {
    return false;
  }
}

/// @brief 小なり
/// @return op1 < op2 のとき true を返す．
inline
bool
operator<(
  VlTime op1, ///< [in] 第1オペランド
  VlTime op2  ///< [in] 第2オペランド
)
{
  if ( op1.value() < op2.value() ) {
    return true;
  }
  else {
    return false;
  }
}

/// @brief 小なりまたは等しい
/// @return op1 <= op2 のとき true を返す．
inline
bool
operator<=(
  VlTime op1, ///< [in] 第1オペランド
  VlTime op2  ///< [in] 第2オペランド
)
{
  return !(op2 < op1);
}

/// @brief 大なり
/// @return op1 > op2 のとき true を返す．
inline
bool
operator>(
  VlTime op1, ///< [in] 第1オペランド
  VlTime op2  ///< [in] 第2オペランド
)
{
  return op2 < op1;
}

/// @brief 大なりまたは等しい
/// @return op1 >= op2 のとき true を返す．
inline
bool
operator>=(
  VlTime op1, ///< [in] 第1オペランド
  VlTime op2  ///< [in] 第2オペランド
)
{
  return !(op1 < op2);
}

/// @}
//////////////////////////////////////////////////////////////////////

END_NAMESPACE_YM_VERILOG

BEGIN_NAMESPACE_STD

// VlTimeをキーにしたハッシュ関数クラスの定義
template <>
struct hash<YM_NAMESPACE::nsVerilog::VlTime>
{
  SizeType
  operator()(YM_NAMESPACE::nsVerilog::VlTime time) const
  {
    return time.hash();
  }
};

END_NAMESPACE_STD

#endif // YM_VLTIME_H
