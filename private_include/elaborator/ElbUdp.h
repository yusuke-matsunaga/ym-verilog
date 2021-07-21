#ifndef ELBUDP_H
#define ELBUDP_H

/// @file ElbUdp.h
/// @brief ElbUdp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/VlUdp.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ElbUdpDefn ElbUdp.h "ElbUdp.h"
/// @brief UDP定義を表すクラス
/// IEEE Std 1364-2001 26.6.14 UDP
//////////////////////////////////////////////////////////////////////
class ElbUdpDefn :
  public VlUdpDefn
{
public:

  /// @brief コンストラクタ
  ElbUdpDefn() = default;

  /// @brief デストラクタ
  ~ElbUdpDefn() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbUdpDefn の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入出力オブジェクトの内容を設定する．
  virtual
  void
  set_io(
    SizeType pos,              ///< [in] ポート中の位置
    const PtIOHead* pt_header, ///< [in] パース木のヘッダ
    const PtIOItem* pt_item    ///< [in] パース木の要素
  ) = 0;

  /// @brief 初期値を設定する．
  virtual
  void
  set_initial(
    const PtExpr* init_expr,    ///< [in] 初期値を表す式
    const VlScalarVal& init_val ///< [in] 初期値
  ) = 0;

  /// @brief table entry の内容を設定する．
  virtual
  void
  set_tableentry(
    SizeType pos,                   ///< [in] 行番号
    const PtUdpEntry* pt_udp_entry, ///< [in] パース木の一行分の定義
    const vector<VlUdpVal>& vals    ///< [in] シンボル値の配列
  ) = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class ElbTableEntry ElbUdp.h "ElbUdp.h"
/// @brief UDP の table entry を表すクラス
//////////////////////////////////////////////////////////////////////
class ElbTableEntry :
  public VlTableEntry
{
public:

  /// @brief コンストラクタ
  ElbTableEntry() = default;

  /// @brief デストラクタ
  ~ElbTableEntry() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbTableEntry の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 設定する．
  virtual
  void
  set(
    const PtUdpEntry* pt_entry,  ///< [in] パース木の要素
    const vector<VlUdpVal>& vals ///< [in] 値の配列
  ) = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // ELBUDP_H
