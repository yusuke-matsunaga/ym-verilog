
/// @file StmtGen_block.cc
/// @brief ElbMgr の実装ファイル(block statement の実体化)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.


#include "StmtGen.h"
#include "ElbEnv.h"

#include "ym/pt/PtStmt.h"


BEGIN_NAMESPACE_YM_VERILOG

BEGIN_NONAMESPACE

const int debug = 0;
#define dout cout

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// ブロックステートメント関係の instantiate 関数
//////////////////////////////////////////////////////////////////////

// @brief ブロックスコープ内の宣言要素の生成を行う．
void
StmtGen::phase2_namedblock(const VlScope* parent,
			   const vector<const PtDeclHead*>& pt_head_array)
{
  if ( debug ) {
    dout << endl
	 << "phase2_namedblock( "
	 << parent->full_name()
	 << " ["
	 << hex << reinterpret_cast<ympuint>(parent) << dec
	 << "] )" << endl;
  }

  instantiate_decl(parent, pt_head_array);

  if ( debug ) {
    dout << "phase2_namedblock end" << endl
	 << endl;
  }
}

// @brief parallel block のインスタンス化を行う．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] env 生成時の環境
// @param[in] pt_stmt 対象のステートメント
const VlStmt*
StmtGen::instantiate_parblock(const VlScope* parent,
			      const VlProcess* process,
			      const ElbEnv& env,
			      const PtStmt* pt_stmt)
{
  auto stmt_list = instantiate_stmt_list(parent, process, env, pt_stmt);
  auto stmt = factory().new_Fork(parent, process, pt_stmt, stmt_list);

  return stmt;
}

// @brief sequential block のインスタンス化を行う．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] env 生成時の環境
// @param[in] pt_stmt 対象のステートメント
const VlStmt*
StmtGen::instantiate_seqblock(const VlScope* parent,
			      const VlProcess* process,
			      const ElbEnv& env,
			      const PtStmt* pt_stmt)
{
  auto stmt_list = instantiate_stmt_list(parent, process, env, pt_stmt);
  auto stmt = factory().new_Begin(parent, process, pt_stmt, stmt_list);

  return stmt;
}

// @brief 名前つき parallel block のインスタンス化を行う．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] env 生成時の環境
// @param[in] pt_stmt 対象のステートメント
const VlStmt*
StmtGen::instantiate_namedparblock(const VlScope* parent,
				   const VlProcess* process,
				   const ElbEnv& env,
				   const PtStmt* pt_stmt)
{
  auto block = find_namedobj(parent, pt_stmt->name());
  ASSERT_COND( block );

  auto stmt_list = instantiate_stmt_list(block, process, env, pt_stmt);
  auto stmt = factory().new_NamedFork(block, process, pt_stmt, stmt_list);

  return stmt;
}

// @brief 名前つき sequential block のインスタンス化を行う．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] env 生成時の環境
// @param[in] pt_stmt 対象のステートメント
const VlStmt*
StmtGen::instantiate_namedseqblock(const VlScope* parent,
				   const VlProcess* process,
				   const ElbEnv& env,
				   const PtStmt* pt_stmt)
{
  auto block = find_namedobj(parent, pt_stmt->name());
  ASSERT_COND( block );

  auto stmt_list = instantiate_stmt_list(block, process, env, pt_stmt);
  auto stmt = factory().new_NamedBegin(block, process, pt_stmt, stmt_list);

  return stmt;
}

// @brief Stmt のリストのインスタンス化を行う．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] env 生成時の環境
// @param[in] pt_stmt 対象のステートメント
//
// pt_stmt はブロック系のステートメント
vector<const VlStmt*>
StmtGen::instantiate_stmt_list(const VlScope* parent,
			       const VlProcess* process,
			       const ElbEnv& env,
			       const PtStmt* pt_stmt)
{
  SizeType stmt_num = pt_stmt->stmt_num();
  vector<const VlStmt*> stmt_list;
  stmt_list.reserve(stmt_num);
  for ( auto pt_stmt1: pt_stmt->stmt_list() ) {
    auto stmt1 = instantiate_stmt(parent, process, env, pt_stmt1);
    if ( !stmt1 ) {
      return vector<const VlStmt*>{};
    }
    stmt_list.push_back(stmt1);
  }

  return stmt_list;
}

END_NAMESPACE_YM_VERILOG
