
/// @file EiBlockStmt.cc
/// @brief EiBlockStmt の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "EiFactory.h"
#include "EiBlockStmt.h"
#include "EiBlockScope.h"

#include "ym/pt/PtStmt.h"
#include "ym/pt/PtArray.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief begin ブロックを生成する．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] stmt_list 子のステートメントリスト
ElbStmt*
EiFactory::new_Begin(const VlNamedObj* parent,
		     ElbProcess* process,
		     const PtStmt* pt_stmt,
		     ElbStmt** stmt_list)
{
  SizeType stmt_num = pt_stmt->stmt_array().size();
  EiBegin* stmt = new EiBegin(parent, process, pt_stmt,
			      stmt_num, stmt_list);

  return stmt;
}

// @brief fork ブロックを生成する．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] stmt_list 子のステートメントリスト
ElbStmt*
EiFactory::new_Fork(const VlNamedObj* parent,
		    ElbProcess* process,
		    const PtStmt* pt_stmt,
		    ElbStmt** stmt_list)
{
  SizeType stmt_num = pt_stmt->stmt_array().size();
  EiFork* stmt = new EiFork(parent, process, pt_stmt,
			    stmt_num, stmt_list);

  return stmt;
}

// @brief 名前付き begin ブロックを生成する．
// @param[in] block 自分自身に対応するスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] stmt_list 子のステートメントリスト
ElbStmt*
EiFactory::new_NamedBegin(const VlNamedObj* block,
			  ElbProcess* process,
			  const PtStmt* pt_stmt,
			  ElbStmt** stmt_list)
{
  SizeType stmt_num = pt_stmt->stmt_array().size();
  EiNamedBegin* stmt = new EiNamedBegin(block, process,
					stmt_num, stmt_list);

  return stmt;
}

// @brief 名前付き fork ブロックを生成する．
// @param[in] block 自分自身に対応するスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] stmt_list 子のステートメントリスト
ElbStmt*
EiFactory::new_NamedFork(const VlNamedObj* block,
			 ElbProcess* process,
			 const PtStmt* pt_stmt,
			 ElbStmt** stmt_list)
{
  SizeType stmt_num = pt_stmt->stmt_array().size();
  EiNamedFork* stmt = new EiNamedFork(block, process,
				      stmt_num, stmt_list);

  return stmt;
}


//////////////////////////////////////////////////////////////////////
// クラス EiBlockStmt
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] stmt_num ステートメントのリストの要素数
// @param[in] array ステートメントのリスト用配列
EiBlockStmt::EiBlockStmt(const VlNamedObj* parent,
			 ElbProcess* process,
			 const PtStmt* pt_stmt,
			 SizeType stmt_num,
			 ElbStmt** array) :
  EiStmtBase(parent, process, pt_stmt),
  mStmtNum(stmt_num),
  mStmtList(array)
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
  return mStmtNum;
}

// @brief 中身のステートメントを返す．
// @param[in] pos 位置番号
ElbStmt*
EiBlockStmt::_child_stmt(SizeType pos) const
{
  return mStmtList[pos];
}


//////////////////////////////////////////////////////////////////////
// クラス EiBegin
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] stmt_num ステートメントのリストの要素数
// @param[in] array ステートメントのリスト用配列
EiBegin::EiBegin(const VlNamedObj* parent,
		 ElbProcess* process,
		 const PtStmt* pt_stmt,
		 SizeType stmt_num,
		 ElbStmt** array) :
  EiBlockStmt(parent, process, pt_stmt, stmt_num, array)
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
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] stmt_num ステートメントのリストの要素数
// @param[in] array ステートメントのリスト用配列
EiFork::EiFork(const VlNamedObj* parent,
	       ElbProcess* process,
	       const PtStmt* pt_stmt,
	       SizeType stmt_num,
	       ElbStmt** array) :
  EiBlockStmt(parent, process, pt_stmt, stmt_num, array)
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
// @param[in] block 自分自身に対応するスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] stmt_num ステートメントのリストの要素数
// @param[in] array ステートメントのリスト用配列
EiNamedBlockStmt::EiNamedBlockStmt(const VlNamedObj* block,
				   ElbProcess* process,
				   SizeType stmt_num,
				   ElbStmt** array) :
  mBlockScope(block),
  mProcess(process),
  mStmtNum(stmt_num),
  mStmtList(array)
{
}

// @brief デストラクタ
EiNamedBlockStmt::~EiNamedBlockStmt()
{
}

// @brief ファイル位置の取得
FileRegion
EiNamedBlockStmt::file_region() const
{
  return mBlockScope->file_region();
}

// @brief 親のスコープを返す．
const VlNamedObj*
EiNamedBlockStmt::parent() const
{
  return mBlockScope->parent();
}

// @brief 対応するスコープを返す．
const VlNamedObj*
EiNamedBlockStmt::scope() const
{
  return mBlockScope;
}

// @brief 中身のステートメントのリストの要素数を返す．
SizeType
EiNamedBlockStmt::child_stmt_num() const
{
  return mStmtNum;
}

// @brief 中身のステートメントを返す．
// @param[in] pos 位置番号
ElbStmt*
EiNamedBlockStmt::_child_stmt(SizeType pos) const
{
  return mStmtList[pos];
}


//////////////////////////////////////////////////////////////////////
// クラス EiNamedBegin
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] block 自分自身に対応するスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] stmt_num ステートメントのリストの要素数
// @param[in] array ステートメントのリスト用配列
EiNamedBegin::EiNamedBegin(const VlNamedObj* block,
			   ElbProcess* process,
			   SizeType stmt_num,
			   ElbStmt** array) :
  EiNamedBlockStmt(block, process, stmt_num, array)
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
// @param[in] block 自分自身に対応するスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] stmt_num ステートメントのリストの要素数
// @param[in] array ステートメントのリスト用配列
EiNamedFork::EiNamedFork(const VlNamedObj* block,
			 ElbProcess* process,
			 SizeType stmt_num,
			 ElbStmt** array) :
  EiNamedBlockStmt(block, process, stmt_num, array)
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
