﻿
/// @file EiStmt.cc
/// @brief EiStmt の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "EiStmt.h"
#include "ym/pt/PtStmt.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス EiStmtBase
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] pt_stmt パース木のステートメント定義
EiStmtBase::EiStmtBase(const VlNamedObj* parent,
		       ElbProcess* process,
		       const PtStmt* pt_stmt) :
  mParent(parent),
  mProcess(process),
  mPtStmt(pt_stmt)
{
}

// デストラクタ
EiStmtBase::~EiStmtBase()
{
}

// @brief ファイル位置を返す．
FileRegion
EiStmtBase::file_region() const
{
  return mPtStmt->file_region();
}

// @brief 親のスコープを返す．
const VlNamedObj*
EiStmtBase::parent() const
{
  return mParent;
}

#if 0
// @brief 親の process を返す．
ElbProcess*
EiStmtBase::process() const
{
  return mProcess;
}
#endif

END_NAMESPACE_YM_VERILOG
