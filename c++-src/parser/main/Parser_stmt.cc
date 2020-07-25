/// @file Parser_stmt.cc
/// @brief Parser の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2020 Yusuke Matsunaga
/// All rights reserved.


#include "parser/Parser.h"

#include "parser/PtiFactory.h"
#include "parser/PuHierName.h"
#include "ym/pt/PtStmt.h"
#include "ym/pt/PtExpr.h"


BEGIN_NAMESPACE_YM_VERILOG

// @brief disable 文の生成
// @param[in] fr ファイル位置の情報
// @param[in] name 停止するスコープ名
// @return 生成された disable 文
const PtStmt*
Parser::new_Disable(const FileRegion& fr,
		    const char* name)
{
  auto stmt{mFactory.new_Disable(fr, name)};
  return stmt;
}

// @brief disable 文の生成 (階層付き識別子)
// @param[in] fr ファイル位置の情報
// @param[in] hname 階層名
// @param[in] name 末尾の名前
// @return 生成された disable 文
const PtStmt*
Parser::new_Disable(const FileRegion& fr,
		    PuHierName* hname)
{
  auto stmt{mFactory.new_Disable(fr, hname)};
  return stmt;
}

// @brief enable 文の生成
// @param[in] fr ファイル位置の情報
// @param[in] name 起動するタスク名
// @param[in] arg_array 引数のリスト
// @return 生成された enable 文
const PtStmt*
Parser::new_Enable(const FileRegion& fr,
		   const char* name,
		   PtrList<const PtExpr>* arg_list)
{
  auto stmt{mFactory.new_Enable(fr, name, new_array(arg_list))};
  return stmt;
}

// @brief enable 文の生成 (階層付き識別子)
// @param[in] fr ファイル位置の情報
// @param[in] hname 階層名
// @param[in] name 末尾の名前
// @param[in] arg_array 引数のリスト
// @return 生成された enable 文
const PtStmt*
Parser::new_Enable(const FileRegion& fr,
		   PuHierName* hname,
		   PtrList<const PtExpr>* arg_list)
{
  auto stmt{mFactory.new_Enable(fr, hname, new_array(arg_list))};
  return stmt;
}

// @brief system task enable 文の生成
// @param[in] fr ファイル位置の情報
// @param[in] name 起動するシステムタスク名
// @param[in] arg_array 引数のリスト
const PtStmt*
Parser::new_SysEnable(const FileRegion& fr,
		      const char* name,
		      PtrList<const PtExpr>* arg_list)
{
  auto stmt{mFactory.new_SysEnable(fr, name, new_array(arg_list))};
  return stmt;
}

// @brief delay control 文の生成
// @param[in] fr ファイル位置の情報
// @param[in] delay delay control (delay control 文とは別物)
// @param[in] body 本体のステートメント
// @return 生成された delay control 文
const PtStmt*
Parser::new_DcStmt(const FileRegion& fr,
		   const PtControl* delay,
		   const PtStmt* body)
{
  auto stmt{mFactory.new_DcStmt(fr, delay, body)};
  return stmt;
}

// @brief event control 文の生成
// @param[in] fr ファイル位置の情報
// @param[in] event event control (event control 文とは別物)
// @param[in] body 本体のステートメント
// @return 生成された event control 文
const PtStmt*
Parser::new_EcStmt(const FileRegion& fr,
		   const PtControl* event,
		   const PtStmt* body)
{
  auto stmt{mFactory.new_EcStmt(fr, event, body)};
  return stmt;
}

// @brief wait 文の生成
// @param[in] fr ファイル位置の情報
// @param[in] cond 条件式
// @param[in] body 本体のステートメント
// @return 生成された wait 文
const PtStmt*
Parser::new_Wait(const FileRegion& fr,
		 const PtExpr* cond,
		 const PtStmt* body)
{
  auto stmt{mFactory.new_Wait(fr, cond, body)};
  return stmt;
}

// @brief assign 文の生成
// @param[in] fr ファイル位置の情報
// @param[in] lhs 左辺式
// @param[in] rhs 右辺式
// @return 生成された assign 文
const PtStmt*
Parser::new_Assign(const FileRegion& fr,
		   const PtExpr* lhs,
		   const PtExpr* rhs)
{
  auto stmt{mFactory.new_Assign(fr, lhs, rhs)};
  return stmt;
}

// @brief control 付き assign 文の生成
// @param[in] fr ファイル位置の情報
// @param[in] lhs 左辺式
// @param[in] rhs 右辺式
// @param[in] control 制御式
// @return 生成された assign 文
const PtStmt*
Parser::new_Assign(const FileRegion& fr,
		   const PtExpr* lhs,
		   const PtExpr* rhs,
		   const PtControl* control)
{
  auto stmt{mFactory.new_Assign(fr, lhs, rhs, control)};
  return stmt;
}

// @brief nonblocking assign 文の生成
// @param[in] fr ファイル位置の情報
// @param[in] lhs 左辺式
// @param[in] rhs 右辺式
// @return 生成された nonblocking assign 文
const PtStmt*
Parser::new_NbAssign(const FileRegion& fr,
		     const PtExpr* lhs,
		     const PtExpr* rhs)
{
  auto stmt{mFactory.new_NbAssign(fr, lhs, rhs)};
  return stmt;
}

// @brief control 付き nonblocking assign 文の生成
// @param[in] fr ファイル位置の情報
// @param[in] lhs 左辺式
// @param[in] rhs 右辺式
// @param[in] control 制御式
// @return 生成された nonblocking assign 文
const PtStmt*
Parser::new_NbAssign(const FileRegion& fr,
		     const PtExpr* lhs,
		     const PtExpr* rhs,
		     const PtControl* control)
{
  auto stmt{mFactory.new_NbAssign(fr, lhs, rhs, control)};
  return stmt;
}

// @brief event 文の生成
// @param[in] fr ファイル位置の情報
// @param[in] event イベント名
// @return 生成された event 文
const PtStmt*
Parser::new_EventStmt(const FileRegion& fr,
		      const PtExpr* event)
{
  auto stmt{mFactory.new_EventStmt(fr, event)};
  return stmt;
}

// @brief null 文の生成
// @param[in] fr ファイル位置の情報
// @return 生成された null 文
const PtStmt*
Parser::new_NullStmt(const FileRegion& fr)
{
  auto stmt{mFactory.new_NullStmt(fr)};
  return stmt;
}

// @brief if 文の生成
// @param[in] fr ファイル位置の情報
// @param[in] expr 条件式
// @param[in] then_body 成り立ったときに実行されるステートメント
// @return 生成された if 文
const PtStmt*
Parser::new_If(const FileRegion& fr,
	       const PtExpr* expr,
	       const PtStmt* then_body)
{
  auto stmt{mFactory.new_If(fr, expr, then_body)};
  return stmt;
}

// @brief if 文の生成
// @param[in] fr ファイル位置の情報
// @param[in] expr 条件式
// @param[in] then_body 成り立ったときに実行されるステートメント
// @param[in] else_body 成り立たなかったときに実行されるステートメント
// @return 生成された if 文
const PtStmt*
Parser::new_If(const FileRegion& fr,
	       const PtExpr* expr,
	       const PtStmt* then_body,
	       const PtStmt* else_body)
{
  auto stmt{mFactory.new_If(fr, expr, then_body, else_body)};
  return stmt;
}

// @brief case 文の生成
// @param[in] fr ファイル位置の情報
// @param[in] expr 条件式
// @param[in] caseitem_list case item のリスト
// @return 生成された case 文
const PtStmt*
Parser::new_Case(const FileRegion& fr,
		 const PtExpr* expr,
		 PtrList<const PtCaseItem>* caseitem_list)
{
  auto stmt{mFactory.new_Case(fr, expr, new_array(caseitem_list))};
  return stmt;
}

// @brief casex 文の生成
// @param[in] fr ファイル位置の情報
// @param[in] expr 条件式
// @param[in] caseitem_list case item のリスト
// @return 生成された case 文
const PtStmt*
Parser::new_CaseX(const FileRegion& fr,
		  const PtExpr* expr,
		  PtrList<const PtCaseItem>* caseitem_list)
{
  auto stmt{mFactory.new_CaseX(fr, expr, new_array(caseitem_list))};
  return stmt;
}

// @brief casez 文の生成
// @param[in] fr ファイル位置の情報
// @param[in] expr 条件式
// @param[in] caseitem_list case item のリスト
// @return 生成された case 文
const PtStmt*
Parser::new_CaseZ(const FileRegion& fr,
		  const PtExpr* expr,
		  PtrList<const PtCaseItem>* caseitem_list)
{
  auto stmt{mFactory.new_CaseZ(fr, expr, new_array(caseitem_list))};
  return stmt;
}

// @brief case item の生成
// @param[in] fr ファイル位置の情報
// @param[in] label_list ラベルのリスト
// @param[in] body 本体のステートメント
// @return 生成された case item
const PtCaseItem*
Parser::new_CaseItem(const FileRegion& fr,
		     PtrList<const PtExpr>* label_list,
		     const PtStmt* body)
{
  auto stmt{mFactory.new_CaseItem(fr, new_array(label_list), body)};
  return stmt;
}

// @brief forever 文の生成
// @param[in] fr ファイル位置の情報
// @param[in] body 本体のステートメント
// @return 生成された forever 文
const PtStmt*
Parser::new_Forever(const FileRegion& fr,
		    const PtStmt* body)
{
  auto stmt{mFactory.new_Forever(fr, body)};
  return stmt;
}

// @brief repeat 文の生成
// @param[in] fr ファイル位置の情報
// @param[in] expr 繰り返し数を表す式
// @param[in] body 本体のステートメント
// @return 生成された repeat 文
const PtStmt*
Parser::new_Repeat(const FileRegion& fr,
		   const PtExpr* expr,
		   const PtStmt* body)
{
  auto stmt{mFactory.new_Repeat(fr, expr, body)};
  return stmt;
}

// @brief while 文の生成
// @param[in] fr ファイル位置の情報
// @param[in] cond 繰り返し条件式
// @param[in] body 本体のステートメント
// @return 生成された while 文
const PtStmt*
Parser::new_While(const FileRegion& fr,
		  const PtExpr* cond,
		  const PtStmt* body)
{
  auto stmt{mFactory.new_While(fr, cond, body)};
  return stmt;
}

// @brief for 文の生成
// @param[in] fr ファイル位置の情報
// @param[in] init 初期化文
// @param[in] cond 繰り返し条件式
// @param[in] next 増加文
// @param[in] body 本体のステートメント
// @return 生成された for 文
const PtStmt*
Parser::new_For(const FileRegion& fr,
		const PtStmt* init,
		const PtExpr* cond,
		const PtStmt* next,
		const PtStmt* body)
{
  auto stmt{mFactory.new_For(fr, init, cond, next, body)};
  return stmt;
}

// @brief procedural assign 文の生成
// @param[in] fr ファイル位置の情報
// @param[in] lhs 左辺式
// @param[in] rhs 右辺式
// @return 生成された procedural assign 文
const PtStmt*
Parser::new_PcAssign(const FileRegion& fr,
		     const PtExpr* lhs,
		     const PtExpr* rhs)
{
  auto stmt{mFactory.new_PcAssign(fr, lhs, rhs)};
  return stmt;
}

// @brief deassign 文の生成
// @param[in] fr ファイル位置の情報
// @param[in] lhs 左辺式
// @return 生成された deassign 文
const PtStmt*
Parser::new_Deassign(const FileRegion& fr,
		     const PtExpr* lhs)
{
  auto stmt{mFactory.new_Deassign(fr, lhs)};
  return stmt;
}

// @brief force 文の生成
// @param[in] fr ファイル位置の情報
// @param[in] lhs 左辺式
// @param[in] rhs 右辺式
// @return 生成された force 文
const PtStmt*
Parser::new_Force(const FileRegion& fr,
		  const PtExpr* lhs,
		  const PtExpr* rhs)
{
  auto stmt{mFactory.new_Force(fr, lhs, rhs)};
  return stmt;
}

// @brief release 文の生成
// @param[in] fr ファイル位置の情報
// @param[in] lhs 左辺式
// @return 生成された release 文
const PtStmt*
Parser::new_Release(const FileRegion& fr,
		    const PtExpr* lhs)
{
  auto stmt{mFactory.new_Release(fr, lhs)};
  return stmt;
}

// @brief parallel block の生成
// @param[in] fr ファイル位置の情報
// @param[in] stmt_list 本体のステートメントのリスト
// @return 生成された parallel block
const PtStmt*
Parser::new_ParBlock(const FileRegion& fr,
		     PtrList<const PtStmt>* stmt_list)
{
  auto stmt{mFactory.new_ParBlock(fr, new_array(stmt_list))};
  return stmt;
}

// @brief 名前付き parallel block の生成
// @param[in] fr ファイル位置の情報
// @param[in] name ブロック名
// @param[in] stmt_list 本体のステートメントのリスト
// @return 生成された parallel block
const PtStmt*
Parser::new_NamedParBlock(const FileRegion& fr,
			  const char* name,
			  PtrList<const PtStmt>* stmt_list)
{
  auto stmt{mFactory.new_NamedParBlock(fr, name, mCurDeclArray,
				       new_array(stmt_list))};
  return stmt;
}

// @brief sequential block の生成
// @param[in] fr ファイル位置の情報
// @param[in] stmt_list 本体のステートメントのリスト
// @return 生成された sequential block
const PtStmt*
Parser::new_SeqBlock(const FileRegion& fr,
		     PtrList<const PtStmt>* stmt_list)
{
  auto stmt{mFactory.new_SeqBlock(fr, new_array(stmt_list))};
  return stmt;
}

// @brief 名前付き sequential block の生成
// @param[in] fr ファイル位置の情報
// @param[in] name ブロック名
// @param[in] stmt_list 本体のステートメントのリスト
// @return 生成された sequential block
const PtStmt*
Parser::new_NamedSeqBlock(const FileRegion& fr,
			  const char* name,
			  PtrList<const PtStmt>* stmt_list)
{
  auto stmt{mFactory.new_NamedSeqBlock(fr, name, mCurDeclArray,
				       new_array(stmt_list))};
  return stmt;
}

END_NAMESPACE_YM_VERILOG
