#ifndef YM_PT_PTUDP_H
#define YM_PT_PTUDP_H

/// @file ym/pt/PtUdp.h
/// @brief PtUdp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014, 2020, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym/pt/PtBase.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class PtUdp PtUdp.h "ym/pt/PtUdp.h"
/// @ingroup VlParser
/// @ingroup PtGroup
/// @brief UDP を表すノード
//////////////////////////////////////////////////////////////////////
class PtUdp :
  public PtNamedBase
{
public:
  //////////////////////////////////////////////////////////////////////
  // PtUdp の継承クラスが実装しなければならない仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief primitive type を返す．
  virtual
  VpiPrimType
  prim_type() const = 0;

  /// @brief ポート数を取り出す．
  virtual
  SizeType
  port_num() const = 0;

  /// @brief ポートを取り出す．
  virtual
  const PtPort*
  port(SizeType pos ///< [in] 位置 ( 0 <= pos < port_num() )
  ) const = 0;

  /// @brief ポートのリストを取り出す．
  vector<const PtPort*>
  port_list() const;

  /// @brief 入出力宣言ヘッダ配列の要素数の取得
  virtual
  SizeType
  iohead_num() const = 0;

  /// @brief 入出力宣言ヘッダの取得
  virtual
  const PtIOHead*
  iohead(SizeType pos ///< [in] 位置 ( 0 <= pos < iohead_num() )
  ) const = 0;

  /// @brief 入出力のリストの取得
  vector<const PtIOHead*>
  iohead_list() const;

  /// @brief 初期値を取出す．
  virtual
  const PtExpr*
  init_value() const = 0;

  /// @brief テーブルの要素数を取り出す．
  virtual
  SizeType
  table_num() const = 0;

  /// @brief テーブルの要素を取り出す．
  virtual
  const PtUdpEntry*
  table(SizeType pos ///< [in] 位置 ( 0 <= pos < table_num() )
  ) const = 0;

  /// @brief テーブルのリストを返す．
  vector<const PtUdpEntry*>
  table_list() const;

};


//////////////////////////////////////////////////////////////////////
/// @class PtUdpEntry PtUdp.h "ym/pt/PtUdp.h"
/// @ingroup VlParser
/// @ingroup PtGroup
/// @brief UDP の中身のテーブルを表すクラス
//////////////////////////////////////////////////////////////////////
class PtUdpEntry :
  public PtBase
{
public:
  //////////////////////////////////////////////////////////////////////
  // PtUdpEntry の継承クラスが実装しなければならない仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力値の配列の要素数を取り出す．
  virtual
  SizeType
  input_num() const = 0;

  /// @brief 入力値を取り出す．
  virtual
  const PtUdpValue*
  input(SizeType pos ///< [in] 位置 ( 0 <= pos < input_num() )
  ) const = 0;

  /// @brief 入力値のリストを取り出す．
  vector<const PtUdpValue*>
  input_list() const;

  /// @brief 現状態の値を取り出す．
  virtual
  const PtUdpValue*
  current() const = 0;

  /// @brief 出力の値を取り出す．
  virtual
  const PtUdpValue*
  output() const = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class PtUdpValue PtUdp.h "ym/pt/PtUdp.h"
/// @ingroup VlParser
/// @ingroup PtGroup
/// @brief UDP のテーブルの値を表すクラス
//////////////////////////////////////////////////////////////////////
class PtUdpValue :
  public PtBase
{
public:
  //////////////////////////////////////////////////////////////////////
  // PtUdpValue の継承クラスが実装しなければならない仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値を取り出す．
  virtual
  VlUdpVal
  symbol() const = 0;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief ポートのリストを取り出す．
inline
vector<const PtPort*>
PtUdp::port_list() const
{
  SizeType n = port_num();
  vector<const PtPort*> vec(n);
  for ( SizeType i = 0; i < n; ++ i ) {
    vec[i] = port(i);
  }
  return vec;
}

// @brief 入出力のリストの取得
inline
vector<const PtIOHead*>
PtUdp::iohead_list() const
{
  SizeType n = iohead_num();
  vector<const PtIOHead*> vec(n);
  for ( SizeType i = 0; i < n; ++ i ) {
    vec[i] = iohead(i);
  }
  return vec;
}

// @brief テーブルのリストを返す．
inline
vector<const PtUdpEntry*>
PtUdp::table_list() const
{
  SizeType n = table_num();
  vector<const PtUdpEntry*> vec(n);
  for ( SizeType i = 0; i < n; ++ i ) {
    vec[i] = table(i);
  }
  return vec;
}

// @brief 入力値のリストを取り出す．
inline
vector<const PtUdpValue*>
PtUdpEntry::input_list() const
{
  SizeType n = input_num();
  vector<const PtUdpValue*> vec(n);
  for ( SizeType i = 0; i < n; ++ i ) {
    vec[i] = input(i);
  }
  return vec;
}

END_NAMESPACE_YM_VERILOG

#endif // YM_PT_PTUDP_H
