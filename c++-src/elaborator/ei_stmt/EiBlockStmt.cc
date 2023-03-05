
/// @file EiBlockStmt.cc
/// @brief EiBlockStmt の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.

#include "ei/EiFactory.h"
#include "ei/EiBlockStmt.h"
#include "ym/vl/VlScope.h"
#include "ym/pt/PtStmt.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief begin ブロックを生成する．
const VlStmt*
EiFactory::new_Begin(
  const VlScope* parent,
  const VlProcess* process,
  const PtStmt* pt_stmt,
  const vector<const VlStmt*>& stmt_list
)
{
  auto stmt = new EiBegin{parent, process, pt_stmt, stmt_list};
  return stmt;
}

// @brief fork ブロックを生成する．
const VlStmt*
EiFactory::new_Fork(
  const VlScope* parent,
  const VlProcess* process,
  const PtStmt* pt_stmt,
  const vector<const VlStmt*>& stmt_list
)
{
  auto stmt = new EiFork{parent, process, pt_stmt, stmt_list};
  return stmt;
}

// @brief 名前付き begin ブロックを生成する．
const VlStmt*
EiFactory::new_NamedBegin(
  const VlScope* block,
  const VlProcess* process,
  const PtStmt* pt_stmt,
  const vector<const VlStmt*>& stmt_list
)
{
  auto stmt = new EiNamedBegin{block, process, pt_stmt, stmt_list};
  return stmt;
}

// @brief 名前付き fork ブロックを生成する．
const VlStmt*
EiFactory::new_NamedFork(
  const VlScope* block,
  const VlProcess* process,
  const PtStmt* pt_stmt,
  const vector<const VlStmt*>& stmt_list
)
{
  auto stmt = new EiNamedFork{block, process, pt_stmt, stmt_list};
  return stmt;
}


//////////////////////////////////////////////////////////////////////
// クラス EiBlockStmt
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiBlockStmt::EiBlockStmt(
  const VlScope* parent,
  const VlProcess* process,
  const PtStmt* pt_stmt,
  const vector<const VlStmt*>& stmt_list
) : EiStmtBase{parent, process, pt_stmt},
    mStmtList{stmt_list}
{
}

// @brief デストラクタ
EiBlockStmt::~EiBlockStmt()
{
}

// @brief 中身のステートメントのリストの要素数を返す．
SizeType
EiBlockStmt::child_stmt_num() const
{
  return mStmtList.size();
}

// @brief 子供のステートメントの取得
const VlStmt*
EiBlockStmt::child_stmt(
  SizeType pos
) const
{
  ASSERT_COND( 0 <= pos && pos < child_stmt_num() );

  return mStmtList[pos];
}


//////////////////////////////////////////////////////////////////////
// クラス EiBegin
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiBegin::EiBegin(
  const VlScope* parent,
  const VlProcess* process,
  const PtStmt* pt_stmt,
  const vector<const VlStmt*>& stmt_list
) : EiBlockStmt{parent, process, pt_stmt, stmt_list}
{
}

// @brief デストラクタ
EiBegin::~EiBegin()
{
}

// @brief 型の取得
VpiObjType
EiBegin::type() const
{
  return VpiObjType::Begin;
}


//////////////////////////////////////////////////////////////////////
// クラス EiFork
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiFork::EiFork(
  const VlScope* parent,
  const VlProcess* process,
  const PtStmt* pt_stmt,
  const vector<const VlStmt*>& stmt_list
) : EiBlockStmt{parent, process, pt_stmt, stmt_list}
{
}

// @brief デストラクタ
EiFork::~EiFork()
{
}

// @brief 型の取得
VpiObjType
EiFork::type() const
{
  return VpiObjType::Fork;
}


//////////////////////////////////////////////////////////////////////
// クラス EiNamedBlockStmt
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiNamedBlockStmt::EiNamedBlockStmt(
  const VlScope* scope,
  const VlProcess* process,
  const PtStmt* pt_stmt,
  const vector<const VlStmt*>& stmt_list
) : EiBlockStmt{scope->parent_scope(), process, pt_stmt, stmt_list},
    mScope{scope}
{
}

// @brief デストラクタ
EiNamedBlockStmt::~EiNamedBlockStmt()
{
}

// @brief 対応するスコープを返す．
const VlScope*
EiNamedBlockStmt::scope() const
{
  return mScope;
}


//////////////////////////////////////////////////////////////////////
// クラス EiNamedBegin
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiNamedBegin::EiNamedBegin(
  const VlScope* block,
  const VlProcess* process,
  const PtStmt* pt_stmt,
  const vector<const VlStmt*>& stmt_list
) : EiNamedBlockStmt{block, process, pt_stmt, stmt_list}
{
}

// @brief デストラクタ
EiNamedBegin::~EiNamedBegin()
{
}

// @brief 型の取得
VpiObjType
EiNamedBegin::type() const
{
  return VpiObjType::NamedBegin;
}


//////////////////////////////////////////////////////////////////////
// クラス EiNamedFork
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiNamedFork::EiNamedFork(
  const VlScope* block,
  const VlProcess* process,
  const PtStmt* pt_stmt,
  const vector<const VlStmt*>& stmt_list
) : EiNamedBlockStmt{block, process, pt_stmt, stmt_list}
{
}

// @brief デストラクタ
EiNamedFork::~EiNamedFork()
{
}

// @brief 型の取得
VpiObjType
EiNamedFork::type() const
{
  return VpiObjType::NamedFork;
}

END_NAMESPACE_YM_VERILOG
