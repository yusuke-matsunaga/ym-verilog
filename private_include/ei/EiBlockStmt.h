#ifndef EIBLOCKSTMT_H
#define EIBLOCKSTMT_H

/// @file EiBlockStmtStmt.h
/// @brief EiBlockStmtStmt のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2020 Yusuke Matsunaga
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
public:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] stmt_list ステートメントのリスト
  EiBlockStmt(const VlScope* parent,
	      const VlProcess* process,
	      const PtStmt* pt_stmt,
	      const vector<const VlStmt*>& stmt_list);

  /// @brief デストラクタ
  ~EiBlockStmt();


public:
  //////////////////////////////////////////////////////////////////////
  // VlStmt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 子供のステートメントのリストの要素数を返す．
  SizeType
  child_stmt_num() const override;

  /// @brief 子供のステートメントの取得
  /// @param[in] pos 位置番号 (0 <= pos < stmt_num())
  const VlStmt*
  child_stmt(SizeType pos) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ステートメントのリスト
  vector<const VlStmt*> mStmtList;

};


//////////////////////////////////////////////////////////////////////
/// @class EiBegin EiBlockStmtStmt.h "EiBlockStmtStmt.h"
/// @brief sequential blockstatement を表すクラス
/// IEEE Std 1364-2001 26.6.27 Process, block, statement, event statement
//////////////////////////////////////////////////////////////////////
class EiBegin :
  public EiBlockStmt
{
public:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] stmt_list ステートメントのリスト
  EiBegin(const VlScope* parent,
	  const VlProcess* process,
	  const PtStmt* pt_stmt,
	  const vector<const VlStmt*>& stmt_list);

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
public:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] stmt_list ステートメントのリスト
  EiFork(const VlScope* parent,
	 const VlProcess* process,
	 const PtStmt* pt_stmt,
	 const vector<const VlStmt*>& stmt_list);

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
  public EiBlockStmt
{
public:

  /// @brief コンストラクタ
  /// @param[in] scope 自分自身に対応するスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] stmt_list ステートメントのリスト
  EiNamedBlockStmt(const VlScope* scope,
		   const VlProcess* process,
		   const PtStmt* pt_stmt,
		   const vector<const VlStmt*>& stmt_list);

  /// @brief デストラクタ
  ~EiNamedBlockStmt();


public:
  //////////////////////////////////////////////////////////////////////
  // VlStmt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 対応するスコープを返す．
  const VlScope*
  scope() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 自身に対応するスコープ
  const VlScope* mScope;

};


//////////////////////////////////////////////////////////////////////
/// @class EiNamedBegin EiBlockStmtStmt.h "EiBlockStmtStmt.h"
/// @brief 名前付き sequential blockstatement を表すクラス
/// IEEE Std 1364-2001 26.6.27 Process, block, statement, event statement
//////////////////////////////////////////////////////////////////////
class EiNamedBegin :
  public EiNamedBlockStmt
{
public:

  /// @brief コンストラクタ
  /// @param[in] block 自分自身に対応するスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] stmt_list ステートメントのリスト
  EiNamedBegin(const VlScope* block,
	       const VlProcess* process,
	       const PtStmt* pt_stmt,
	       const vector<const VlStmt*>& stmt_list);

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
public:

  /// @brief コンストラクタ
  /// @param[in] block 自分自身に対応するスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] stmt_list ステートメントのリスト
  EiNamedFork(const VlScope* block,
	      const VlProcess* process,
	      const PtStmt* pt_stmt,
	      const vector<const VlStmt*>& stmt_list);

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
