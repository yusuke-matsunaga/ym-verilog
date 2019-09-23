﻿
/// @file EiBlockScope.cc
/// @brief EiBlockScope の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "EiFactory.h"
#include "EiBlockScope.h"
#include "ym/pt/PtStmt.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @breif statement block を生成する．
// @param[in] parent 親のスコープ環境
// @param[in] pt_stmt 対応するパース木の要素
ElbScope*
EiFactory::new_StmtScope(const VlNamedObj* parent,
			 const PtStmt* pt_stmt)
{
  void* p = mAlloc.get_memory(sizeof(EiBlockScope));
  EiBlockScope* scope = new (p) EiBlockScope(parent, pt_stmt);

  return scope;
}


//////////////////////////////////////////////////////////////////////
// クラス EiBlockScope
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のスコープ環境
// @param[in] pt_stmt 対応するパース木の要素
EiBlockScope::EiBlockScope(const VlNamedObj* parent,
			   const PtStmt* pt_stmt) :
  mParent(parent),
  mPtStmt(pt_stmt)
{
}

// @brief デストラクタ
EiBlockScope::~EiBlockScope()
{
}

// @brief 型の取得
VpiObjType
EiBlockScope::type() const
{
  switch ( mPtStmt->type() ) {
  case kPtNamedParBlockStmt: return VpiObjType::NamedFork;
  case kPtNamedSeqBlockStmt: return VpiObjType::NamedBegin;
  default:
    break;
  }
  ASSERT_NOT_REACHED;
  return VpiObjType::Scope;
}

// @brief ファイル位置を返す．
FileRegion
EiBlockScope::file_region() const
{
  return mPtStmt->file_region();
}

// @brief このオブジェクトの属しているスコープを返す．
const VlNamedObj*
EiBlockScope::parent() const
{
  return mParent;
}

// @brief 名前の取得
const char*
EiBlockScope::name() const
{
  return mPtStmt->name();
}

END_NAMESPACE_YM_VERILOG
