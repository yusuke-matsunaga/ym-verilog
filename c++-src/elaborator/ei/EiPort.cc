﻿
/// @file EiPort.cc
/// @brief EiPort の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.

#include "ei/EiPort.h"

#include "elaborator/ElbModule.h"
#include "elaborator/ElbExpr.h"

#include "ym/pt/PtPort.h"



BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス EiPort
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiPort::EiPort()
{
}

// @brief 初期設定を行う．
void
EiPort::init(
  const VlModule* module,
  const PtPort* pt_port,
  SizeType index,
  ElbExpr* low_conn,
  VpiDir dir
)
{
  mModule = module;
  mPtPort = pt_port;
  mIndex = index;
  mHighConn = nullptr;
  mLowConn = low_conn;
  mDirection = dir;
  mConnByName = false;
}

// @brief high_conn を接続する．
void
EiPort::set_high_conn(
  ElbExpr* high_conn,
  bool conn_by_name
)
{
  mHighConn = high_conn;
  mConnByName = conn_by_name;
}

// @brief 型の取得
VpiObjType
EiPort::type() const
{
  return VpiObjType::Port;
}

// @brief ファイル位置の取得
FileRegion
EiPort::file_region() const
{
  return mPtPort->file_region();
}

// @brief 入出力の区別を得る．
VpiDir
EiPort::direction() const
{
  return mDirection;
}

// @brief ビット幅を返す．
SizeType
EiPort::bit_size() const
{
  return mLowConn->bit_size();
}

// @brief 名前による接続を持つとき true を返す．
bool
EiPort::is_conn_by_name() const
{
  return mConnByName;
}

// @brief 明示的に名前がついているとき true を返す．
bool
EiPort::is_explicit_name() const
{
  return mPtPort->ext_name() != nullptr;
}

// @brief 名前を返す．
string
EiPort::name() const
{
  return mPtPort->ext_name();
}

// @brief 親のモジュールを取出す
const VlModule*
EiPort::module() const
{
  return mModule;
}

// @brief ポートリストの何番目のポートかを表すインデックスを返す．
SizeType
EiPort::port_index() const
{
  return mIndex;
}

// @brief 上位の接続先を返す．
const VlExpr*
EiPort::high_conn() const
{
  return mHighConn;
}

// @brief 下位の接続先を返す．
const VlExpr*
EiPort::low_conn() const
{
  return mLowConn;
}

END_NAMESPACE_YM_VERILOG
