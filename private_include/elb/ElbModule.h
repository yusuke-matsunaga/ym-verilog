﻿#ifndef ELBMODULE_H
#define ELBMODULE_H

/// @file ElbModule.h
/// @brief ElbModule のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym/vl/VlModule.h"
#include "ym/pt/PtP.h"
#include "ElbFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ElbModuleArray ElbModule.h "ElbModule.h"
/// @brief elaboration 中の module 配列を表すクラス
//////////////////////////////////////////////////////////////////////
class ElbModuleArray :
  public VlModuleArray
{
protected:

  /// @brief コンストラクタ
  ElbModuleArray() = default;

  /// @brief デストラクタ
  ~ElbModuleArray() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 設定用の関数 (ElbMgr が使う)
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素を返す．
  virtual
  ElbModule*
  _module(int offset) = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class ElbModule ElbModule.h "ElbModule.h"
/// @brief elaboration 中の module を表すクラス
//////////////////////////////////////////////////////////////////////
class ElbModule :
  public VlModule
{
protected:

  /// @brief コンストラクタ
  ElbModule() = default;

  /// @brief デストラクタ
  ~ElbModule() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbModule の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入出力の初期設定を行う．
  /// @param[in] pos 位置番号
  /// @param[in] head ヘッダ
  /// @param[in] pt_item パース木のIO宣言要素
  /// @param[in] decl 対応する宣言要素
  virtual
  void
  init_iodecl(int pos,
	      ElbIOHead* head,
	      const PtIOItem* pt_item,
	      ElbDecl* decl) = 0;

  /// @brief ポートの初期設定を行う．
  /// @param[in] index ポート番号
  /// @param[in] pt_port パース木のポート定義
  /// @param[in] low_conn 下位の接続
  /// @param[in] dir 向き
  virtual
  void
  init_port(int index,
	    const PtPort* pt_port,
	    ElbExpr* low_conn,
	    VpiDir dir) = 0;

  /// @brief ポートの high_conn を接続する．
  /// @param[in] index ポート番号
  /// @param[in] high_conn 上位の接続の式
  /// @param[in] conn_by_name 名前による割り当て時に true とするフラグ
  virtual
  void
  set_port_high_conn(int index,
		     ElbExpr* high_conn,
		     bool conn_by_name) = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // ELBMODULE_H
