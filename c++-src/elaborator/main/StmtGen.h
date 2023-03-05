#ifndef STMTGEN_H
#define STMTGEN_H

/// @file StmtGen.h
/// @brief StmtGen のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.

#include "ym/verilog.h"
#include "ym/pt/PtP.h"
#include "ElbProxy.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class StmtGen StmtGen.h "StmtGen.h"
/// @brief ElbStmt を生成するクラス
//////////////////////////////////////////////////////////////////////
class StmtGen :
  public ElbProxy
{
public:

  /// @brief コンストラクタ
  StmtGen(
    Elaborator& elab, ///< [in] 生成器
    ElbMgr& elb_mgr   ///< [in] Elbオブジェクトを管理するクラス
  );

  /// @brief デストラクタ
  ~StmtGen();


public:
  //////////////////////////////////////////////////////////////////////
  // StmtGen の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief スコープに関係するステートメントの実体化を行う．
  void
  phase1_stmt(
    const VlScope* parent, ///< [in] 親のスコープ
    const PtStmt* pt_stmt, ///< [in] 対象のステートメント
    bool cf = false        ///< [in] constant function 中のステートメントの時 true
  );

  /// @brief ステートメントの実体化を行う．
  ///
  /// pt_stmt が nullptr の時は nullptr を返す．
  const VlStmt*
  instantiate_stmt(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in] 親のプロセス
    const ElbEnv& env,        ///< [in] 生成時の環境
    const PtStmt* pt_stmt     ///< [in] 対象のステートメント
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief disable statement の実体化を行う．
  const VlStmt*
  instantiate_disable(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in] 親のプロセス (or nullptr)
    const PtStmt* pt_stmt     ///< [in] 対象のステートメント
  );

  /// @brief enable の実体化を行う．
  const VlStmt*
  instantiate_enable(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in] 親のプロセス (or nullptr)
    const ElbEnv& env,        ///< [in] 生成時の環境
    const PtStmt* pt_stmt     ///< [in] 対象のステートメント
  );

  /// @brief system enable 文の実体化を行う．
  const VlStmt*
  instantiate_sysenable(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in] 親のプロセス (or nullptr)
    const ElbEnv& env,	      ///< [in] 生成時の環境
    const PtStmt* pt_stmt     ///< [in] 対象のステートメント
  );

  /// @brief delay / event control statement の実体化を行う．
  const VlStmt*
  instantiate_ctrlstmt(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in] 親のプロセス (or nullptr)
    const ElbEnv& env,	      ///< [in] 生成時の環境
    const PtStmt* pt_stmt     ///< [in] 対象のステートメント
  );

  /// 代入文のインスタンス化を行う．
  const VlStmt*
  instantiate_assign(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in] 親のプロセス (or nullptr)
    const ElbEnv& env,	      ///< [in] 生成時の環境
    const PtStmt* pt_stmt,    ///< [in] 対象のステートメント
    bool block                ///< [in] ブロッキング代入の時 true
  );

  /// @brief コントロールを生成する．
  const VlControl*
  instantiate_control(
    const VlScope* parent,      ///< [in] 親のスコープ
    const ElbEnv& env,	        ///< [in] 親のプロセス (or nullptr)
    const PtControl* pt_control ///< [in] パース木のコントロール定義
  );

  /// @brief event statement の実体化を行う．
  const VlStmt*
  instantiate_eventstmt(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in] 親のプロセス (or nullptr)
    const PtStmt* pt_stmt     ///< [in] 対象のステートメント
  );

  /// @brief null statement の実体化を行う．
  const VlStmt*
  instantiate_nullstmt(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in] 親のプロセス (or nullptr)
    const PtStmt* pt_stmt     ///< [in] 対象のステートメント
  );

  /// @brief if 文の生成を行う．
  const VlStmt*
  instantiate_if(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in] 親のプロセス (or nullptr)
    const ElbEnv& env,	      ///< [in] 生成時の環境
    const PtStmt* pt_stmt     ///< [in] 対象のステートメント
  );

  /// @brief case 文の実体化を行う．
  const VlStmt*
  instantiate_case(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in] 親のプロセス (or nullptr)
    const ElbEnv& env,	      ///< [in] 生成時の環境
    const PtStmt* pt_stmt     ///< [in] 対象のステートメント
  );

  /// @brief wait 文の実体化を行う．
  const VlStmt*
  instantiate_wait(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in] 親のプロセス (or nullptr)
    const ElbEnv& env,	      ///< [in] 生成時の環境
    const PtStmt* pt_stmt     ///< [in] 対象のステートメント
  );

  /// @brief forever 文のインスタンス化を行う．
  const VlStmt*
  instantiate_forever(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in] 親のプロセス (or nullptr)
    const ElbEnv& env,	      ///< [in] 生成時の環境
    const PtStmt* pt_stmt     ///< [in] 対象のステートメント
  );

  /// @brief repeat 文のインスタンス化を行う．
  const VlStmt*
  instantiate_repeat(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in] 親のプロセス (or nullptr)
    const ElbEnv& env,	      ///< [in] 生成時の環境
    const PtStmt* pt_stmt     ///< [in] 対象のステートメント
  );

  /// @brief while 文のインスタンス化を行う．
  const VlStmt*
  instantiate_while(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in] 親のプロセス (or nullptr)
    const ElbEnv& env,	      ///< [in] 生成時の環境
    const PtStmt* pt_stmt     ///< [in] 対象のステートメント
  );

  /// @brief for 文のインスタンス化を行う．
  const VlStmt*
  instantiate_for(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in] 親のプロセス (or nullptr)
    const ElbEnv& env,	      ///< [in] 生成時の環境
    const PtStmt* pt_stmt     ///< [in] 対象のステートメント
  );

  /// @brief procedural continuous assign 文のインスタンス化を行う．
  const VlStmt*
  instantiate_pca(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in] 親のプロセス (or nullptr)
    const ElbEnv& env,	      ///< [in] 生成時の環境
    const PtStmt* pt_stmt     ///< [in] 対象のステートメント
  );

  /// @brief deassign 文のインスタンス化を行う．
  const VlStmt*
  instantiate_deassign(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in] 親のプロセス (or nullptr)
    const ElbEnv& env,	      ///< [in] 生成時の環境
    const PtStmt* pt_stmt     ///< [in] 対象のステートメント
  );

  /// @brief force 文のインスタンス化を行う．
  const VlStmt*
  instantiate_force(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in] 親のプロセス (or nullptr)
    const ElbEnv& env,	      ///< [in] 生成時の環境
    const PtStmt* pt_stmt     ///< [in] 対象のステートメント
  );

  /// @brief release 文のインスタンス化を行う．
  const VlStmt*
  instantiate_release(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in] 親のプロセス (or nullptr)
    const ElbEnv& env,	      ///< [in] 生成時の環境
    const PtStmt* pt_stmt     ///< [in] 対象のステートメント
  );

  /// @brief parallel block のインスタンス化を行う．
  const VlStmt*
  instantiate_parblock(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in] 親のプロセス (or nullptr)
    const ElbEnv& env,	      ///< [in] 生成時の環境
    const PtStmt* pt_stmt     ///< [in] 対象のステートメント
  );

  /// @brief sequential block のインスタンス化を行う．
  const VlStmt*
  instantiate_seqblock(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in] 親のプロセス (or nullptr)
    const ElbEnv& env,	      ///< [in] 生成時の環境
    const PtStmt* pt_stmt     ///< [in] 対象のステートメント
  );

  /// @brief ブロックスコープ内の宣言要素の生成を行う．
  void
  phase2_namedblock(
    const VlScope* parent,
    const vector<const PtDeclHead*>& pt_head_array
  );

  /// @brief 名前つき parallel block のインスタンス化を行う．
  const VlStmt*
  instantiate_namedparblock(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in] 親のプロセス (or nullptr)
    const ElbEnv& env,	      ///< [in] 生成時の環境
    const PtStmt* pt_stmt     ///< [in] 対象のステートメント
  );

  /// @brief 名前つき sequential block のインスタンス化を行う．
  const VlStmt*
  instantiate_namedseqblock(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in] 親のプロセス (or nullptr)
    const ElbEnv& env,	      ///< [in] 生成時の環境
    const PtStmt* pt_stmt     ///< [in] 対象のステートメント
  );

  /// @brief Stmt のリストのインスタンス化を行う．
  ///
  /// pt_stmt はブロック系のステートメント
  vector<const VlStmt*>
  instantiate_stmt_list(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in] 親のプロセス (or nullptr)
    const ElbEnv& env,	      ///< [in] 生成時の環境
    const PtStmt* pt_stmt     ///< [in] 対象のステートメント
  );

  /// @brief ステートメントブロックのスコープを生成する．
  const VlScope*
  new_StmtBlockScope(
    const VlScope* parent,  ///< [in] 親のスコープ
    const PtStmt* pt_stmt   ///< [in] 対応するパース木の要素
  );

};

END_NAMESPACE_YM_VERILOG

#endif // STMTGEN_H
