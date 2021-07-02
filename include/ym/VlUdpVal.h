﻿#ifndef YM_VLUDPVAL_H
#define YM_VLUDPVAL_H

/// @file ym/VlUdpVal.h
/// @brief VlUdpVal のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym/verilog.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class VlUdpVal VlUdpVal.h "ym/VlUdpVal.h"
/// @brief UDP のテーブルで使われる値を表すクラス
/// 基本的には
/// - 0
/// - 1
/// - X
/// - B = (0 | 1)
/// - Q = (0 | 1 | X)
/// の5種類の値からなる．
/// ただし，val_a -> val_b という形の遷移シンボルがある．
/// また，変化なし，というシンボルもある．
/// やっかいなのが，
/// p = 01 | 0X | X1
/// n = 10 | 1X | X0
/// という特殊シンボル
/// なので 00, 01, 0X, 10, 11, 1X, X0, X1, XX の9つのビットの組み合わせ
/// で表す．
//////////////////////////////////////////////////////////////////////
class VlUdpVal
{
public:

  /// @brief 値を表す文字からのコンストラクタ
  explicit
  VlUdpVal(char symbol = '-'); ///< [in] 値を表すシンボル

  /// @brief 値を表す2つの文字からのコンストラクタ
  VlUdpVal(char symbol1,  ///< [in] 変化前のシンボル
	   char symbol2); ///< [in] 変化後のシンボル

  /// @brief デストラクタ
  ~VlUdpVal() = default;


public:

  /// @brief レベルシンボルの時に true を返す．
  bool
  is_level_symbol() const;

  /// @brief 遷移シンボル(エッジシンボル)の時に true を返す．
  bool
  is_edge_symbol() const;

  /// @brief No Change シンボルの時に true を返す．
  bool
  is_nc_symbol() const;

  /// @brief 複合値の時に true を返す．
  /// @note レベルシンボルの時のみ意味を持つ．
  bool
  is_composite_symbol() const;

  /// @brief 文字列を返す．
  string
  to_string() const;

  /// @brief 等価比較
  bool
  operator==(const VlUdpVal& right) const ///< [in] オペランド
  {
    return mData == right.mData;
  }

  /// @brief 包含性チェック
  bool
  operator<=(const VlUdpVal& right) const ///< [in] オペランド
  {
    return (mData & right.mData) == mData;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値を直接指定するコンストラクタ
  VlUdpVal(ymuint16 val);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実際の値
  ymuint16 mData{0UL};

};

/// @brief 非等価比較
inline
bool
operator!=(const VlUdpVal& left,  ///< [in] 第1オペランド
	   const VlUdpVal& right) ///< [in] 第2オペランド
{
  return !left.operator==(right);
}

/// @brief 包含性チェック
inline
bool
operator>=(const VlUdpVal& left,  ///< [in] 第1オペランド
	   const VlUdpVal& right) ///< [in] 第2オペランド
{
  return right.operator<=(left);
}

/// @brief ストリーム出力
/// @relates VlUdpVal
/// @param[in] s 出力先のストリーム
/// @param[in] val 値
ostream&
operator<<(ostream& s,
	   const VlUdpVal& val);


END_NAMESPACE_YM_VERILOG

#endif // YM_VLUDPVAL_H
