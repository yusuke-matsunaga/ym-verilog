
/// @file EiStmt.cc
/// @brief EiStmt の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.

#include "ei/EiStmt.h"
#include "ym/pt/PtStmt.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス EiStmt
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiStmt::EiStmt(
  const VlProcess* process
) : mProcess{process}
{
}

// @brief 対象のスコープの取得
const VlScope*
EiStmt::scope() const
{
  return nullptr;
}

// @brief task の実体を返す．
const VlTaskFunc*
EiStmt::task() const
{
  return nullptr;
}

// @brief user systf クラスへのポインタを返す．
const VlUserSystf*
EiStmt::user_systf() const
{
  return nullptr;
}

// @brief 引数の数の取得
SizeType
EiStmt::argument_num() const
{
  return 0;
}

// @brief 引数の取得
const VlExpr*
EiStmt::argument(
  SizeType pos
) const
{
  return nullptr;
}

// @brief 引数のリストの取得
vector<const VlExpr*>
EiStmt::argument_list() const
{
  return {};
}

// @brief control の取得
const VlControl*
EiStmt::control() const
{
  return nullptr;
}

// @brief 本体のステートメントの取得
const VlStmt*
EiStmt::body_stmt() const
{
  return nullptr;
}

// @brief 式の取得
const VlExpr*
EiStmt::expr() const
{
  return nullptr;
}

// @brief 左辺式の取得
const VlExpr*
EiStmt::lhs() const
{
  return nullptr;
}

// @brief 右辺式の取得
const VlExpr*
EiStmt::rhs() const
{
  return nullptr;
}

// @brief 代入のブロッキング/ノンブロッキングの区別の取得
bool
EiStmt::is_blocking() const
{
  return false;
}

// @brief イベントプライマリの取得
const VlExpr*
EiStmt::named_event() const
{
  return nullptr;
}

// @brief 条件が成り立たなかったとき実行されるステートメントの取得
const VlStmt*
EiStmt::else_stmt() const
{
  return nullptr;
}

// @brief case type の取得
VpiCaseType
EiStmt::case_type() const
{
  return VpiCaseType::Exact;
}

// @brief case item の要素数の取得
SizeType
EiStmt::caseitem_num() const
{
  return 0;
}

// @brief case item の取得
const VlCaseItem*
EiStmt::caseitem(
  SizeType pos
) const
{
  ASSERT_NOT_REACHED;
  return nullptr;
}

// @brief case item のリストの取得
vector<const VlCaseItem*>
EiStmt::caseitem_list() const
{
  ASSERT_NOT_REACHED;
  return {};
}

// @brief 初期化代入文の取得
const VlStmt*
EiStmt::init_stmt() const
{
  return nullptr;
}

// @brief 繰り返し代入文の取得
const VlStmt*
EiStmt::inc_stmt() const
{
  return nullptr;
}

// @brief 子供のステートメントの数の取得
SizeType
EiStmt::child_stmt_num() const
{
  return 0;
}

// @brief 子供のステートメントの取得
const VlStmt*
EiStmt::child_stmt(
  SizeType pos
) const
{
  return nullptr;
}

// @brief 子供のステートメントのリストの取得
vector<const VlStmt*>
EiStmt::child_stmt_list() const
{
  return {};
}

// @brief disable 対象のスコープを得る．
const VlScope*
EiStmt::target_scope() const
{
  return nullptr;
}


//////////////////////////////////////////////////////////////////////
// クラス EiStmtBase
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiStmtBase::EiStmtBase(
  const VlScope* parent,
  const VlProcess* process,
  const PtStmt* pt_stmt
) : EiStmt{process},
    mParent{parent},
    mPtStmt{pt_stmt}
{
}

// @brief 親のスコープを返す．
const VlScope*
EiStmtBase::parent_scope() const
{
  return mParent;
}

// @brief ファイル位置を返す．
FileRegion
EiStmtBase::file_region() const
{
  return mPtStmt->file_region();
}

END_NAMESPACE_YM_VERILOG
