﻿#ifndef YM_VLSCALARVAL_H
#define YM_VLSCALARVAL_H

/// @file ym/VlScalarVal.h
/// @brief VlScalarVal のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym/verilog.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class VlScalarVal VlScalarVal.h "ym/VlScalarVal.h"
/// @brief 1ビットの値を表すクラス
/// @note 具体的には
///  - 0
///  - 1
///  - X ( 0 か 1 か不明 )
///  - Z ( high-impedance )
///  の4種類の値を持つ．
//////////////////////////////////////////////////////////////////////
class VlScalarVal
{
public:

  /// @brief 空のコンストラクタ
  ///
  /// 不定値となる．
  VlScalarVal() = default;

  /// @brief 整数値からの変換コンストラクタ
  ///
  /// val が 0 の時のみ 0 に，それ以外は 1 にする．
  explicit
  VlScalarVal(int val) ///< [in] 値
  {
    if ( val == 0 ) {
      mData = kScalar0;
    }
    else {
      mData = kScalar1;
    }
  }

  /// @brief 符号なし整数値からの変換コンストラクタ
  ///
  /// val が 0 の時のみ 0 に，それ以外は 1 にする．
  explicit
  VlScalarVal(unsigned int val) ///< [in] 値
  {
    if ( val == 0U ) {
      mData = kScalar0;
    }
    else {
      mData = kScalar1;
    }
  }

  /// @brief 実数値からの変換コンストラクタ
  ///
  /// val が 0.0 の時のみ 0 に，それ以外は 1 にする．
  explicit
  VlScalarVal(double val) ///< [in] 値
  {
    if ( val == 0.0 ) {
      mData = kScalar0;
    }
    else {
      mData = kScalar1;
    }
  }

  /// @brief ブール値からの変換コンストラクタ
  ///
  /// val が false なら 0 に，true なら 1 にする．
  explicit
  VlScalarVal(bool val) ///< [in] 値
  {
    if ( val ) {
      mData = kScalar1;
    }
    else {
      mData = kScalar0;
    }
  }

  /// @brief デストラクタ
  ~VlScalarVal() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 値を作るクラスメンバ関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 0 を作る．
  static
  VlScalarVal
  zero() { return VlScalarVal(kScalar0); }

  /// @brief 1 を作る．
  static
  VlScalarVal
  one() { return VlScalarVal(kScalar1); }

  /// @brief X を作る．
  static
  VlScalarVal
  x() { return VlScalarVal(kScalarX); }

  /// @brief Z を作る．
  static
  VlScalarVal
  z() { return VlScalarVal(kScalarZ); }


public:
  //////////////////////////////////////////////////////////////////////
  // 値を調べる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 0 の時に true を返す．
  bool
  is_zero() const { return mData == kScalar0; }

  /// @brief 1 の時に true を返す．
  bool
  is_one() const { return mData == kScalar1; }

  /// @brief X の時に true を返す．
  bool
  is_x() const { return mData == kScalarX; }

  /// @brief Z の時に true を返す．
  bool
  is_z() const { return mData == kScalarZ; }

  /// @brief X/Z の時に true を返す．
  bool
  is_xz() const {
    // kScalar? のコーディングに注意
    return mData >= kScalarX;
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 変換用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ブール値に変換する．
  /// @retval true 値が 1 の時
  /// @retval false それ以外
  bool
  to_bool() const { return mData == kScalar1; }

  /// @brief 論理値に変換する．
  /// @retval 0 値が 0 の時
  /// @retval 1 値が 1 の時
  /// @retval X 値が X か Z の時
  VlScalarVal
  to_logic() const
  {
    switch ( mData ) {
    case kScalar0: return VlScalarVal(kScalar0);
    case kScalar1: return VlScalarVal(kScalar1);
    case kScalarX:
    case kScalarZ: return VlScalarVal(kScalarX);
    }
    ASSERT_NOT_REACHED;
    // ダミー
    return VlScalarVal::x();
  }

  /// @brief 整数値に変換する．
  /// @retval 1 値が 1 の時
  /// @retval 0 それ以外
  int
  to_int() const
  {
    if ( mData == kScalar1 ) {
      return 1;
    }
    else {
      return 0;
    }
  }

  /// @brief 実数値に変換する．
  /// @retval 1.0 値が 1 の時
  /// @retval 0.0 それ以外
  double
  to_real() const
  {
    if ( mData == kScalar1 ) {
      return 1.0;
    }
    else {
      return 0.0;
    }
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 演算子
  //////////////////////////////////////////////////////////////////////

  /// @brief 否定
  VlScalarVal
  operator!() const
  {
    switch ( mData ) {
    case kScalar0: return VlScalarVal(kScalar1);
    case kScalar1: return VlScalarVal(kScalar0);
    case kScalarX:
    case kScalarZ: return VlScalarVal(kScalarX);
    }
    ASSERT_NOT_REACHED;
    // ダミー
    return VlScalarVal::x();
  }

  /// @brief 選言(Conjunction)
  VlScalarVal
  operator&&(const VlScalarVal& right) const ///< [in] オペランド
  {
    if ( is_zero() || right.is_zero() ) {
      return VlScalarVal::zero();
    }
    if ( is_one() && right.is_one() ) {
      return VlScalarVal::one();
    }
    return VlScalarVal::x();
  }

  /// @brief 連言(Disjunction)
  VlScalarVal
  operator||(const VlScalarVal& right) const ///< [in] オペランド
  {
    if ( is_one() || right.is_one() ) {
      return VlScalarVal::one();
    }
    if ( is_zero() && right.is_zero() ) {
      return VlScalarVal::zero();
    }
    return VlScalarVal::x();
  }

  /// @brief 値が等しいときに true を返す．
  bool
  operator==(const VlScalarVal& right) const ///< [in] オペランド
  {
    return mData == right.mData;
  }

  /// @brief 値が等しくないときに true を返す．
  bool
  operator!=(const VlScalarVal& right) const ///< [in] オペランド
  {
    return mData != right.mData;
  }


public:
  //////////////////////////////////////////////////////////////////////
  // フレンド関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 等価比較
  ///
  /// どちらか一方に X/Z を含む時，答も X になる．
  /// eq と neq は相補的でないことに注意
  friend
  VlScalarVal
  eq(const VlScalarVal& left,  ///< [in] 第1オペランド
     const VlScalarVal& right) ///< [in] 第2オペランド
  {
    if ( left.is_xz() || right.is_xz() ) {
      return VlScalarVal::x();
    }
    if ( left.mData == right.mData ) {
      return VlScalarVal::one();
    }
    return VlScalarVal::zero();
  }

  /// @brief  非等価比較
  ///
  /// どちらか一方に X/Z を含む時，答も X になる．
  /// eq と neq は相補的でないことに注意
  friend
  VlScalarVal
  neq(const VlScalarVal& left,  ///< [in] 第1オペランド
      const VlScalarVal& right) ///< [in] 第2オペランド
  {
    if ( left.is_xz() || right.is_xz() ) {
      return VlScalarVal::x();
    }
    if ( left.mData != right.mData ) {
      return VlScalarVal::one();
    }
    return VlScalarVal::zero();
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値を直接指定するコンストラクタ
  explicit
  VlScalarVal(ymuint8 val)
    : mData{val}
  {
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる定数
  //////////////////////////////////////////////////////////////////////

  /// @brief 0 を表す値
  static
  const ymuint8 kScalar0 = 0U;

  /// @brief 1 を表す値
  static
  const ymuint8 kScalar1 = 1U;

  /// @brief X を表す値
  static
  const ymuint8 kScalarX = 2U;

  /// @brief high-impedance を表す値
  static
  const ymuint8 kScalarZ = 3U;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値
  ymuint8 mData{kScalarX};

};

/// @brief ストリーム出力
/// @relates VlScalarVal
inline
ostream&
operator<<(ostream& s,             ///< [in] 出力先のストリーム
	   const VlScalarVal& val) ///< [in] 値
{
  if ( val.is_zero() ) {
    s << "0";
  }
  else if ( val.is_one() ) {
    s << "1";
  }
  else if ( val.is_x() ) {
    s << "X";
  }
  else if ( val.is_z() ) {
    s << "Z";
  }
  else {
    s << "?";
  }
  return s;
}

END_NAMESPACE_YM_VERILOG

#endif // YM_VLSCALARVAL_H
