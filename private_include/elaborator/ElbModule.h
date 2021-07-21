#ifndef ELBMODULE_H
#define ELBMODULE_H

/// @file ElbModule.h
/// @brief ElbModule のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/VlModule.h"
#include "ym/pt/PtP.h"
#include "ElbFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ElbModule ElbModule.h "ElbModule.h"
/// @brief elaboration 中の module を表すクラス
//////////////////////////////////////////////////////////////////////
class ElbModule :
  public VlModule
{
public:
  //////////////////////////////////////////////////////////////////////
  // ElbModule の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入出力を追加する．
  virtual
  void
  add_iodecl(
    ElbIOHead* head,         ///< [in] ヘッダ
    const PtIOItem* pt_item, ///< [in] パース木のIO宣言要素
    const VlDecl* decl       ///< [in] 対応する宣言要素
  ) = 0;

  /// @brief ポートの初期設定を行う．
  virtual
  void
  init_port(
    SizeType index,        ///< [in] ポート番号
    const PtPort* pt_port, ///< [in] パース木のポート定義
    ElbExpr* low_conn,     ///< [in] 下位の接続
    VpiDir dir             ///< [in] 向き
  ) = 0;

  /// @brief ポートの high_conn を接続する．
  virtual
  void
  set_port_high_conn(
    SizeType index,     ///< [in] ポート番号
    ElbExpr* high_conn, ///< [in] 上位の接続の式
    bool conn_by_name   ///< [in] 名前による割り当て時に true とするフラグ
  ) = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // ELBMODULE_H
