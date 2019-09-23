﻿#ifndef EIBLOCKSTMT_H
#define EIBLOCKSTMT_H

/// @file EiBlockStmtStmt.h
/// @brief EiBlockStmtStmt のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.

// ステートメントを表すクラス
// IEEE Std 1364-2001 26.6.27 Process, block, statement, event statement


#include "EiStmt.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiBlockStmt EiBlockStmtStmt.h "EiBlockStmtStmt.h"
/// @brief ブロックを表すクラス
/// IEEE Std 1364-2001 26.6.27 Process, block, statement, event statement
//////////////////////////////////////////////////////////////////////
class EiBlockStmt :
  public EiStmtBase
{
  friend class EiFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] stmt_num ステートメントのリストの要素数
  /// @param[in] array ステートメントのリスト用配列
  EiBlockStmt(const VlNamedObj* parent,
	      ElbProcess* process,
	      const PtStmt* pt_stmt,
	      SizeType stmt_num,
	      ElbStmt** array);

  /// @brief デストラクタ
  ~EiBlockStmt();


public:
  //////////////////////////////////////////////////////////////////////
  // VlStmt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 子供のステートメントのリストの要素数を返す．
  SizeType
  child_stmt_num() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // EiStmt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 子供のステートメントを返す．
  /// @param[in] pos 位置番号
  ElbStmt*
  _child_stmt(SizeType pos) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ステートメント数
  SizeType mStmtNum;

  // ステートメントのリスト
  ElbStmt** mStmtList;

};


//////////////////////////////////////////////////////////////////////
/// @class EiBegin EiBlockStmtStmt.h "EiBlockStmtStmt.h"
/// @brief sequential blockstatement を表すクラス
/// IEEE Std 1364-2001 26.6.27 Process, block, statement, event statement
//////////////////////////////////////////////////////////////////////
class EiBegin :
  public EiBlockStmt
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] stmt_num ステートメントのリストの要素数
  /// @param[in] array ステートメントのリスト用配列
  EiBegin(const VlNamedObj* parent,
	  ElbProcess* process,
	  const PtStmt* pt_stmt,
	  SizeType stmt_num,
	  ElbStmt** array);

  /// @brief デストラクタ
  ~EiBegin();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  VpiObjType
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// @class EiFork EiBlockStmtStmt.h "EiBlockStmtStmt.h"
/// @brief parallel blockstatement を表すクラス
/// IEEE Std 1364-2001 26.6.27 Process, block, statement, event statement
//////////////////////////////////////////////////////////////////////
class EiFork :
  public EiBlockStmt
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] stmt_num ステートメントのリストの要素数
  /// @param[in] array ステートメントのリスト用配列
  EiFork(const VlNamedObj* parent,
	 ElbProcess* process,
	 const PtStmt* pt_stmt,
	 SizeType stmt_num,
	 ElbStmt** array);

  /// @brief デストラクタ
  ~EiFork();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  VpiObjType
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// @class EiNamedBlockStmt EiBlockStmtStmt.h "EiBlockStmtStmt.h"
/// @brief 名前つきのブロックを表すクラス
/// IEEE Std 1364-2001 26.6.27 Process, block, statement, event statement
//////////////////////////////////////////////////////////////////////
class EiNamedBlockStmt :
  public ElbStmt
{
  friend class EiFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] block 自分自身に対応するスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] stmt_num ステートメントのリストの要素数
  /// @param[in] array ステートメントのリスト用配列
  EiNamedBlockStmt(const VlNamedObj* block,
		   ElbProcess* process,
		   SizeType stmt_num,
		   ElbStmt** array);

  /// @brief デストラクタ
  ~EiNamedBlockStmt();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  FileRegion
  file_region() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // VlStmt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 親のスコープを返す．
  const VlNamedObj*
  parent() const override;

  /// @brief 対応するスコープを返す．
  const VlNamedObj*
  scope() const override;

  /// @brief 子供ののステートメントのリストの要素数を返す．
  SizeType
  child_stmt_num() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // EiStmt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 子供のステートメントを返す．
  /// @param[in] pos 位置番号
  ElbStmt*
  _child_stmt(SizeType pos) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 自身に対応するスコープ
  const VlNamedObj* mBlockScope;

  // 親のプロセス
  ElbProcess* mProcess;

  // ステートメント数
  SizeType mStmtNum;

  // ステートメントのリスト
  ElbStmt** mStmtList;

};


//////////////////////////////////////////////////////////////////////
/// @class EiNamedBegin EiBlockStmtStmt.h "EiBlockStmtStmt.h"
/// @brief 名前付き sequential blockstatement を表すクラス
/// IEEE Std 1364-2001 26.6.27 Process, block, statement, event statement
//////////////////////////////////////////////////////////////////////
class EiNamedBegin :
  public EiNamedBlockStmt
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] block 自分自身に対応するスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] stmt_num ステートメントのリストの要素数
  /// @param[in] array ステートメントのリスト用配列
  EiNamedBegin(const VlNamedObj* block,
	       ElbProcess* process,
	       SizeType stmt_num,
	       ElbStmt** array);

  /// @brief デストラクタ
  ~EiNamedBegin();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  VpiObjType
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// @class EiNamedFork ElbStmt.h "ElbStmt.h"
/// @brief 名前付き parallel blockstatement を表すクラス
/// IEEE Std 1364-2001 26.6.27 Process, block, statement, event statement
//////////////////////////////////////////////////////////////////////
class EiNamedFork :
  public EiNamedBlockStmt
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] block 自分自身に対応するスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] stmt_num ステートメントのリストの要素数
  /// @param[in] array ステートメントのリスト用配列
  EiNamedFork(const VlNamedObj* block,
	      ElbProcess* process,
	      SizeType stmt_num,
	      ElbStmt** array);

  /// @brief デストラクタ
  ~EiNamedFork();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  VpiObjType
  type() const override;

};

END_NAMESPACE_YM_VERILOG

#endif // EIBLOCKSTMT_H