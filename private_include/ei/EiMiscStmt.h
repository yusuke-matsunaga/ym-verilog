#ifndef EIMISCSTMT_H
#define EIMISCSTMT_H

/// @file EiMiscStmt.h
/// @brief EiMiscStmt のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.

// ステートメントを表すクラス
// IEEE Std 1364-2001 26.6.27 Process, block, statement, event statement
// IEEE Std 1364-2001 26.6.38 Disable

#include "EiStmt.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiEventStmt ElbMiscStmt.h "ElbMiscStmt.h"
/// @brief event statement を表すクラス
/// IEEE Std 1364-2001 26.6.27 Process, block, statement, event statement
//////////////////////////////////////////////////////////////////////
class EiEventStmt :
  public EiStmtBase
{
public:

  /// @brief コンストラクタ
  EiEventStmt(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in]	親のプロセス (or nullptr)
    const PtStmt* pt_stmt,    ///< [in]	パース木のステートメント定義
    ElbExpr* named_event      ///< [in]	対象のイベント
  );

  /// @brief デストラクタ
  ~EiEventStmt();


public:
  //////////////////////////////////////////////////////////////////////
  // ElbObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  VpiObjType
  type() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbStmt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief named event を返す．
  const VlExpr*
  named_event() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // イベントを表す式
  ElbExpr* mEvent;

};


//////////////////////////////////////////////////////////////////////
/// @class EiNullStmt ElbMiscStmt.h "ElbMiscStmt.h"
/// @brief null statement を表すクラス
//////////////////////////////////////////////////////////////////////
class EiNullStmt :
  public EiStmtBase
{
public:

  /// @brief コンストラクタ
  EiNullStmt(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in]	親のプロセス (or nullptr)
    const PtStmt* pt_stmt     ///< [in]	パース木のステートメント定義
  );

  /// @brief デストラクタ
  ~EiNullStmt();


public:
  //////////////////////////////////////////////////////////////////////
  // ElbObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  VpiObjType
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// @class EiTcBase ElbMiscStmt.h "ElbMiscStmt.h"
/// @brief task call / system task call の基底クラス
//////////////////////////////////////////////////////////////////////
class EiTcBase :
  public EiStmtBase
{
public:

  /// @brief コンストラクタ
  EiTcBase(
    const VlScope* parent,            ///< [in] 親のスコープ
    const VlProcess* process,         ///< [in]	親のプロセス (or nullptr)
    const PtStmt* pt_stmt,            ///< [in]	パース木のステートメント定義
    const vector<ElbExpr*>& arg_array ///< [in] 引数を納める配列
  );

  /// @brief デストラクタ
  ~EiTcBase() override;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbStmt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 引数の数を返す．
  SizeType
  argument_num() const override;

  /// @brief 引数の取得
  const VlExpr*
  argument(
    SizeType pos ///< [in] 位置番号 ( 0 <= pos < argument_num() )
  ) const override;

  /// @brief 引数のリストの取得
  vector<const VlExpr*>
  argument_list() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // 設定用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 引数の設定
  void
  set_argument(
    SizeType pos, ///< [in] 位置番号
    ElbExpr* arg  ///< [in] 設定する引数
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 引数のリスト
  vector<ElbExpr*> mArgumentList;

};


//////////////////////////////////////////////////////////////////////
/// @class EiTaskCall ElbMiscStmt.h "ElbMiscStmt.h"
/// @brief task call を表すクラス
//////////////////////////////////////////////////////////////////////
class EiTaskCall :
  public EiTcBase
{
public:

  /// @brief コンストラクタ
  EiTaskCall(
    const VlScope* parent,            ///< [in] 親のスコープ
    const VlProcess* process,         ///< [in]	親のプロセス (or nullptr)
    const PtStmt* pt_stmt,            ///< [in]	パース木のステートメント定義
    const VlTaskFunc* task,           ///< [in]	対象のタスク
    const vector<ElbExpr*>& arg_array ///< [in]	引数を納める配列
  );

  /// @brief デストラクタ
  ~EiTaskCall();


public:
  //////////////////////////////////////////////////////////////////////
  // ElbObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  VpiObjType
  type() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbStmt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief task の実体を返す．
  const VlTaskFunc*
  task() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // タスク
  const VlTaskFunc* mTask;

};


//////////////////////////////////////////////////////////////////////
/// @class EiSysTaskCall ElbMiscStmt.h "ElbMiscStmt.h"
/// @brief system task call を表すクラス
//////////////////////////////////////////////////////////////////////
class EiSysTaskCall :
  public EiTcBase
{
public:

  /// @brief コンストラクタ
  EiSysTaskCall(
    const VlScope* parent,            ///< [in] 親のスコープ
    const VlProcess* process,         ///< [in]	親のプロセス (or nullptr)
    const PtStmt* pt_stmt,            ///< [in]	パース木のステートメント定義
    const VlUserSystf* user_systf,    ///< [in]	システムタスク
    const vector<ElbExpr*>& arg_array ///< [in]	引数を納める配列
  );

  /// @brief デストラクタ
  ~EiSysTaskCall();


public:
  //////////////////////////////////////////////////////////////////////
  // ElbObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  VpiObjType
  type() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbStmt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief user systf クラスへのポインタを返す．
  const VlUserSystf*
  user_systf() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // user systf へのポインタ
  const VlUserSystf* mUserSystf;

};


//////////////////////////////////////////////////////////////////////
/// @class EiDisableStmt ElbMiscStmt.h "ElbMiscStmt.h"
/// @brief disable statement を表すクラス
/// IEEE Std 1364-2001 26.6.38 Disable
//////////////////////////////////////////////////////////////////////
class EiDisableStmt :
  public EiStmtBase
{
public:

  /// @brief コンストラクタ
  EiDisableStmt(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in]	親のプロセス (or nullptr)
    const PtStmt* pt_stmt,    ///< [in]	パース木のステートメント定義
    const VlScope* target     ///< [in] 対象のスコープ
  );

  /// @brief デストラクタ
  ~EiDisableStmt();


public:
  //////////////////////////////////////////////////////////////////////
  // ElbObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  VpiObjType
  type() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbStmt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief expr を返す．
  ///
  /// 型が VlScope になっているが実際には
  /// ElbFunction/ElbTask/EiNamedFork/EiNamedBegin
  /// のいずれか
  const VlScope*
  target_scope() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象の式
  const VlScope* mExpr;

};


//////////////////////////////////////////////////////////////////////
/// @class EiCtrlStmt EiMisc.h "EiMisc.h"
/// @brief delay / event control 付きの statement を表すクラス
/// IEEE Std 1364-2001 26.6.29 Delay control
/// IEEE Std 1364-2001 26.6.30 Event control
//////////////////////////////////////////////////////////////////////
class EiCtrlStmt :
  public EiStmtBase
{
public:

  /// @brief コンストラクタ
  EiCtrlStmt(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in]	親のプロセス (or nullptr)
    const PtStmt* pt_stmt,    ///< [in]	パース木のステートメント定義
    const VlControl* control, ///< [in]	コントロール
    const VlStmt* stmt        ///< [in] 本体のステートメント
  );

  /// @brief デストラクタ
  ~EiCtrlStmt();


public:
  //////////////////////////////////////////////////////////////////////
  // ElbObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  VpiObjType
  type() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbStmt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief コントロールを返す．
  const VlControl*
  control() const override;

  /// @brief 本文のステートメントを返す．
  const VlStmt*
  body_stmt() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // コントロール
  const VlControl* mControl;

  // 本文
  const VlStmt* mBodyStmt;

};

END_NAMESPACE_YM_VERILOG

#endif // EIMISCSTMT_H
