/// @file Parser_stmt.cc
/// @brief Parser の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2020 Yusuke Matsunaga
/// All rights reserved.

#include "parser/Parser.h"
#include "parser/PtiFactory.h"


BEGIN_NAMESPACE_YM_VERILOG

// @brief disable 文の生成
const PtStmt*
Parser::new_Disable(
  const FileRegion& fr,
  const char* name
)
{
  auto stmt = mFactory->new_Disable(fr, name);
  return stmt;
}

// @brief disable 文の生成 (階層付き識別子)
const PtStmt*
Parser::new_Disable(
  const FileRegion& fr,
  PuHierName* hname
)
{
  auto stmt = mFactory->new_Disable(fr, hname);
  return stmt;
}

// @brief enable 文の生成
const PtStmt*
Parser::new_Enable(
  const FileRegion& fr,
  const char* name,
  PtrList<const PtExpr>* arg_list
)
{
  auto stmt = mFactory->new_Enable(fr, name, arg_list->to_vector());
  return stmt;
}

// @brief enable 文の生成 (階層付き識別子)
const PtStmt*
Parser::new_Enable(
  const FileRegion& fr,
  PuHierName* hname,
  PtrList<const PtExpr>* arg_list
)
{
  auto stmt = mFactory->new_Enable(fr, hname, arg_list->to_vector());
  return stmt;
}

// @brief system task enable 文の生成
const PtStmt*
Parser::new_SysEnable(
  const FileRegion& fr,
  const char* name,
  PtrList<const PtExpr>* arg_list
)
{
  auto stmt = mFactory->new_SysEnable(fr, name, arg_list->to_vector());
  return stmt;
}

// @brief delay control 文の生成
const PtStmt*
Parser::new_DcStmt(
  const FileRegion& fr,
  const PtControl* delay,
  const PtStmt* body
)
{
  auto stmt = mFactory->new_DcStmt(fr, delay, body);
  return stmt;
}

// @brief event control 文の生成
const PtStmt*
Parser::new_EcStmt(
  const FileRegion& fr,
  const PtControl* event,
  const PtStmt* body
)
{
  auto stmt = mFactory->new_EcStmt(fr, event, body);
  return stmt;
}

// @brief wait 文の生成
const PtStmt*
Parser::new_Wait(
  const FileRegion& fr,
  const PtExpr* cond,
  const PtStmt* body
)
{
  auto stmt = mFactory->new_Wait(fr, cond, body);
  return stmt;
}

// @brief assign 文の生成
const PtStmt*
Parser::new_Assign(
  const FileRegion& fr,
  const PtExpr* lhs,
  const PtExpr* rhs
)
{
  auto stmt = mFactory->new_Assign(fr, lhs, rhs);
  return stmt;
}

// @brief control 付き assign 文の生成
const PtStmt*
Parser::new_Assign(
  const FileRegion& fr,
  const PtExpr* lhs,
  const PtExpr* rhs,
  const PtControl* control
)
{
  auto stmt = mFactory->new_Assign(fr, lhs, rhs, control);
  return stmt;
}

// @brief nonblocking assign 文の生成
const PtStmt*
Parser::new_NbAssign(
  const FileRegion& fr,
  const PtExpr* lhs,
  const PtExpr* rhs
)
{
  auto stmt = mFactory->new_NbAssign(fr, lhs, rhs);
  return stmt;
}

// @brief control 付き nonblocking assign 文の生成
const PtStmt*
Parser::new_NbAssign(
  const FileRegion& fr,
  const PtExpr* lhs,
  const PtExpr* rhs,
  const PtControl* control
)
{
  auto stmt = mFactory->new_NbAssign(fr, lhs, rhs, control);
  return stmt;
}

// @brief event 文の生成
const PtStmt*
Parser::new_EventStmt(
  const FileRegion& fr,
  const PtExpr* event
)
{
  auto stmt = mFactory->new_EventStmt(fr, event);
  return stmt;
}

// @brief null 文の生成
const PtStmt*
Parser::new_NullStmt(
  const FileRegion& fr
)
{
  auto stmt = mFactory->new_NullStmt(fr);
  return stmt;
}

// @brief if 文の生成
const PtStmt*
Parser::new_If(
  const FileRegion& fr,
  const PtExpr* expr,
  const PtStmt* then_body
)
{
  auto stmt = mFactory->new_If(fr, expr, then_body);
  return stmt;
}

// @brief if 文の生成
const PtStmt*
Parser::new_If(
  const FileRegion& fr,
  const PtExpr* expr,
  const PtStmt* then_body,
  const PtStmt* else_body
)
{
  auto stmt = mFactory->new_If(fr, expr, then_body, else_body);
  return stmt;
}

// @brief case 文の生成
const PtStmt*
Parser::new_Case(
  const FileRegion& fr,
  const PtExpr* expr,
  PtrList<const PtCaseItem>* caseitem_list
)
{
  auto stmt = mFactory->new_Case(fr, expr, caseitem_list->to_vector());
  return stmt;
}

// @brief casex 文の生成
const PtStmt*
Parser::new_CaseX(
  const FileRegion& fr,
  const PtExpr* expr,
  PtrList<const PtCaseItem>* caseitem_list
)
{
  auto stmt = mFactory->new_CaseX(fr, expr, caseitem_list->to_vector());
  return stmt;
}

// @brief casez 文の生成
const PtStmt*
Parser::new_CaseZ(
  const FileRegion& fr,
  const PtExpr* expr,
  PtrList<const PtCaseItem>* caseitem_list
)
{
  auto stmt = mFactory->new_CaseZ(fr, expr, caseitem_list->to_vector());
  return stmt;
}

// @brief case item の生成
const PtCaseItem*
Parser::new_CaseItem(
  const FileRegion& fr,
  PtrList<const PtExpr>* label_list,
  const PtStmt* body
)
{
  auto stmt = mFactory->new_CaseItem(fr, label_list->to_vector(), body);
  return stmt;
}

// @brief forever 文の生成
const PtStmt*
Parser::new_Forever(
  const FileRegion& fr,
  const PtStmt* body
)
{
  auto stmt = mFactory->new_Forever(fr, body);
  return stmt;
}

// @brief repeat 文の生成
const PtStmt*
Parser::new_Repeat(
  const FileRegion& fr,
  const PtExpr* expr,
  const PtStmt* body
)
{
  auto stmt = mFactory->new_Repeat(fr, expr, body);
  return stmt;
}

// @brief while 文の生成
const PtStmt*
Parser::new_While(
  const FileRegion& fr,
  const PtExpr* cond,
  const PtStmt* body
)
{
  auto stmt = mFactory->new_While(fr, cond, body);
  return stmt;
}

// @brief for 文の生成
const PtStmt*
Parser::new_For(
  const FileRegion& fr,
  const PtStmt* init,
  const PtExpr* cond,
  const PtStmt* next,
  const PtStmt* body
)
{
  auto stmt = mFactory->new_For(fr, init, cond, next, body);
  return stmt;
}

// @brief procedural assign 文の生成
const PtStmt*
Parser::new_PcAssign(
  const FileRegion& fr,
  const PtExpr* lhs,
  const PtExpr* rhs
)
{
  auto stmt = mFactory->new_PcAssign(fr, lhs, rhs);
  return stmt;
}

// @brief deassign 文の生成
const PtStmt*
Parser::new_Deassign(
  const FileRegion& fr,
  const PtExpr* lhs
)
{
  auto stmt = mFactory->new_Deassign(fr, lhs);
  return stmt;
}

// @brief force 文の生成
const PtStmt*
Parser::new_Force(
  const FileRegion& fr,
  const PtExpr* lhs,
  const PtExpr* rhs
)
{
  auto stmt = mFactory->new_Force(fr, lhs, rhs);
  return stmt;
}

// @brief release 文の生成
const PtStmt*
Parser::new_Release(
  const FileRegion& fr,
  const PtExpr* lhs
)
{
  auto stmt = mFactory->new_Release(fr, lhs);
  return stmt;
}

// @brief parallel block の生成
const PtStmt*
Parser::new_ParBlock(
  const FileRegion& fr,
  PtrList<const PtStmt>* stmt_list
)
{
  auto stmt = mFactory->new_ParBlock(fr, stmt_list->to_vector());
  return stmt;
}

// @brief 名前付き parallel block の生成
const PtStmt*
Parser::new_NamedParBlock(
  const FileRegion& fr,
  const char* name,
  PtrList<const PtStmt>* stmt_list
)
{
  auto stmt = mFactory->new_NamedParBlock(fr, name, mCurDeclArray,
					  stmt_list->to_vector());
  return stmt;
}

// @brief sequential block の生成
const PtStmt*
Parser::new_SeqBlock(
  const FileRegion& fr,
  PtrList<const PtStmt>* stmt_list
)
{
  auto stmt = mFactory->new_SeqBlock(fr, stmt_list->to_vector());
  return stmt;
}

// @brief 名前付き sequential block の生成
const PtStmt*
Parser::new_NamedSeqBlock(
  const FileRegion& fr,
  const char* name,
  PtrList<const PtStmt>* stmt_list
)
{
  auto stmt = mFactory->new_NamedSeqBlock(fr, name, mCurDeclArray,
					  stmt_list->to_vector());
  return stmt;
}

END_NAMESPACE_YM_VERILOG
