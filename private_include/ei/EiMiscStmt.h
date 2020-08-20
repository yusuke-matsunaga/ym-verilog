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
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] named_event 対象のイベント
  EiEventStmt(const VlScope* parent,
	      const VlProcess* process,
	      const PtStmt* pt_stmt,
	      ElbExpr* named_event);

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
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  EiNullStmt(const VlScope* parent,
	     const VlProcess* process,
	     const PtStmt* pt_stmt);

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
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] arg_array 引数を納める配列
  EiTcBase(const VlScope* parent,
	   const VlProcess* process,
	   const PtStmt* pt_stmt,
	   const vector<ElbExpr*>& arg_array);

  /// @brief デストラクタ
  ~EiTcBase() override;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbStmt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 引数の数を返す．
  SizeType
  arg_num() const override;

  /// @brief 引数の取得
  /// @param[in] pos 位置番号 ( 0 <= pos < argument_num() )
  const VlExpr*
  arg(SizeType pos) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // 設定用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 引数の設定
  /// @param[in] pos 位置番号
  /// @param[in] arg 設定する引数
  void
  set_argument(SizeType pos,
	       ElbExpr* arg);


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
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] task 対象のタスク
  /// @param[in] arg_array 引数を納める配列
  EiTaskCall(const VlScope* parent,
	     const VlProcess* process,
	     const PtStmt* pt_stmt,
	     const VlTaskFunc* task,
	     const vector<ElbExpr*>& arg_array);

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
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] user_systf システムタスク
  /// @param[in] arg_array 引数を納める配列
  EiSysTaskCall(const VlScope* parent,
		const VlProcess* process,
		const PtStmt* pt_stmt,
		const VlUserSystf* user_systf,
		const vector<ElbExpr*>& arg_array);

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
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] target 対象のスコープ
  EiDisableStmt(const VlScope* parent,
		const VlProcess* process,
		const PtStmt* pt_stmt,
		const VlScope* target);

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
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] control コントロール
  /// @param[in] stmt 本体のステートメント
  EiCtrlStmt(const VlScope* parent,
	     const VlProcess* process,
	     const PtStmt* pt_stmt,
	     const VlControl* control,
	     const VlStmt* stmt);

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
