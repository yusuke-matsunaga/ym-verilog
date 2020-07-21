﻿
/// @file EiProcess.cc
/// @brief EiProcess の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "EiFactory.h"
#include "EiProcess.h"

#include "elb/ElbStmt.h"

#include "ym/pt/PtItem.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief プロセス文を生成する．
// @param[in] parent 親のスコープ
// @param[in] pt_item パース木の要素定義
ElbProcess*
EiFactory::new_Process(const VlNamedObj* parent,
		       const PtItem* pt_item)
{
  EiProcess* process = new EiProcess(parent, pt_item);

  return process;
}


//////////////////////////////////////////////////////////////////////
// クラス EiProcess
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のスコープ
// @param[in] pt_item パース木の要素定義
EiProcess::EiProcess(const VlNamedObj* parent,
		     const PtItem* pt_item) :
  mParent(parent),
  mPtItem(pt_item)
{
}

// @brief デストラクタ
EiProcess::~EiProcess()
{
}

// @brief クラスの型の取得
VpiObjType
EiProcess::type() const
{
  switch ( mPtItem->type() ) {
  case PtItemType::Initial: return VpiObjType::Initial;
  case PtItemType::Always:  return VpiObjType::Always;
  default: break;
  }
  ASSERT_NOT_REACHED;
  return VpiObjType::Initial;
}

// @brief ファイル位置を返す．
FileRegion
EiProcess::file_region() const
{
  return mPtItem->file_region();
}

// @brief 親のスコープを返す．
const VlNamedObj*
EiProcess::parent() const
{
  return mParent;
}

// @brief 本体のステートメントの取得
const VlStmt*
EiProcess::stmt() const
{
  return mStmt;
}

// @brief 本体のステートメントをセットする．
// @param[in] stmt 本体のステートメント
void
EiProcess::set_stmt(ElbStmt* stmt)
{
  mStmt = stmt;
}


END_NAMESPACE_YM_VERILOG
