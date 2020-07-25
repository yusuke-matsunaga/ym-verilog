#ifndef YM_PT_PTUDP_H
#define YM_PT_PTUDP_H

/// @file ym/pt/PtUdp.h
/// @brief PtUdp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
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

  /// @brief ポートのリストを取り出す．
  virtual
  const PtPortArray*
  port_list() const = 0;

  /// @brief 入出力宣言ヘッダ配列の取得
  virtual
  const PtIOHeadArray*
  iohead_array() const = 0;

  /// @brief 初期値を取出す．
  virtual
  const PtExpr*
  init_value() const = 0;

  /// @brief テーブルを取り出す．
  virtual
  const PtUdpEntryArray*
  table_array() const = 0;

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

  /// @brief 入力値の配列を取り出す．
  virtual
  const PtUdpValueArray*
  input_array() const = 0;

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

END_NAMESPACE_YM_VERILOG

#endif // YM_PT_PTUDP_H
