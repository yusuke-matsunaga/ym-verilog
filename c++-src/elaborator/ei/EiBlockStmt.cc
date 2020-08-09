
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


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief begin ブロックを生成する．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] stmt_list 子のステートメントリスト
const VlStmt*
EiFactory::new_Begin(const VlNamedObj* parent,
		     const VlProcess* process,
		     const PtStmt* pt_stmt,
		     const VlStmt** stmt_list)
{
  SizeType stmt_num = pt_stmt->stmt_num();
  {
    for ( SizeType i = 0; i < stmt_num; ++ i ) {
      ASSERT_COND( stmt_list[i] );
    }
  }
  auto stmt = new EiBegin(parent, process, pt_stmt,
			  stmt_num, stmt_list);
  {
    for ( SizeType i = 0; i < stmt_num; ++ i ) {
      ASSERT_COND( stmt->child_stmt(i) );
    }
  }

  return stmt;
}

// @brief fork ブロックを生成する．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] stmt_list 子のステートメントリスト
const VlStmt*
EiFactory::new_Fork(const VlNamedObj* parent,
		    const VlProcess* process,
		    const PtStmt* pt_stmt,
		    const VlStmt** stmt_list)
{
  SizeType stmt_num = pt_stmt->stmt_num();
  auto stmt = new EiFork(parent, process, pt_stmt,
			 stmt_num, stmt_list);

  return stmt;
}

// @brief 名前付き begin ブロックを生成する．
// @param[in] block 自分自身に対応するスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] stmt_list 子のステートメントリスト
const VlStmt*
EiFactory::new_NamedBegin(const VlNamedObj* block,
			  const VlProcess* process,
			  const PtStmt* pt_stmt,
			  const VlStmt** stmt_list)
{
  SizeType stmt_num = pt_stmt->stmt_num();
  auto stmt = new EiNamedBegin(block, process,
			       stmt_num, stmt_list);

  return stmt;
}

// @brief 名前付き fork ブロックを生成する．
// @param[in] block 自分自身に対応するスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] stmt_list 子のステートメントリスト
const VlStmt*
EiFactory::new_NamedFork(const VlNamedObj* block,
			 const VlProcess* process,
			 const PtStmt* pt_stmt,
			 const VlStmt** stmt_list)
{
  SizeType stmt_num = pt_stmt->stmt_num();
  auto stmt = new EiNamedFork(block, process,
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
			 const VlProcess* process,
			 const PtStmt* pt_stmt,
			 SizeType stmt_num,
			 const VlStmt** array) :
  EiStmtBase(parent, process, pt_stmt),
  mStmtNum{stmt_num},
  mStmtList{array}
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

// @brief 子供のステートメントの取得
// @param[in] pos 位置番号 (0 <= pos < stmt_num())
const VlStmt*
EiBlockStmt::child_stmt(SizeType pos) const
{
  ASSERT_COND( 0 <= pos && pos < child_stmt_num() );
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
		 const VlProcess* process,
		 const PtStmt* pt_stmt,
		 SizeType stmt_num,
		 const VlStmt** array) :
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
	       const VlProcess* process,
	       const PtStmt* pt_stmt,
	       SizeType stmt_num,
	       const VlStmt** array) :
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
				   const VlProcess* process,
				   SizeType stmt_num,
				   const VlStmt** array) :
  EiStmt(process),
  mBlockScope{block},
  mStmtNum{stmt_num},
  mStmtList{array}
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

// @brief 子供のステートメントの取得
// @param[in] pos 位置番号 (0 <= pos < stmt_num())
const VlStmt*
EiNamedBlockStmt::child_stmt(SizeType pos) const
{
  ASSERT_COND( 0 <= pos && pos < child_stmt_num() );
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
			   const VlProcess* process,
			   SizeType stmt_num,
			   const VlStmt** array) :
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
			 const VlProcess* process,
			 SizeType stmt_num,
			 const VlStmt** array) :
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
