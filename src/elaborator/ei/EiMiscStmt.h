﻿#ifndef EIMISCSTMT_H
#define EIMISCSTMT_H

/// @file EiMiscStmt.h
/// @brief EiMiscStmt のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
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
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] named_event 対象のイベント
  EiEventStmt(const VlNamedObj* parent,
	      ElbProcess* process,
	      const PtStmt* pt_stmt,
	      ElbExpr* named_event);

  /// @brief デストラクタ
  virtual
  ~EiEventStmt();


public:
  //////////////////////////////////////////////////////////////////////
  // ElbObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  virtual
  tVpiObjType
  type() const;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbStmt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief named event を返す．
  virtual
  const VlExpr*
  named_event() const;


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
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  EiNullStmt(const VlNamedObj* parent,
	     ElbProcess* process,
	     const PtStmt* pt_stmt);

  /// @brief デストラクタ
  virtual
  ~EiNullStmt();


public:
  //////////////////////////////////////////////////////////////////////
  // ElbObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  virtual
  tVpiObjType
  type() const;

};


//////////////////////////////////////////////////////////////////////
/// @class EiTcBase ElbMiscStmt.h "ElbMiscStmt.h"
/// @brief task call / system task call の基底クラス
//////////////////////////////////////////////////////////////////////
class EiTcBase :
  public EiStmtBase
{
  friend class EiFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] arg_num 引数の数
  /// @param[in] arg_array 引数を納める配列
  EiTcBase(const VlNamedObj* parent,
	   ElbProcess* process,
	   const PtStmt* pt_stmt,
	   ymuint arg_num,
	   ElbExpr** arg_array);

  /// @brief デストラクタ
  virtual
  ~EiTcBase();


public:
  //////////////////////////////////////////////////////////////////////
  // ElbStmt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 引数の数を返す．
  virtual
  ymuint
  argument_num() const;

  /// @brief 引数の取得
  /// @param[in] pos 位置番号 ( 0 <= pos < argument_num() )
  virtual
  const VlExpr*
  argument(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 設定用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 引数の設定
  /// @param[in] pos 位置番号
  /// @param[in] arg 設定する引数
  void
  set_argument(ymuint pos,
	       ElbExpr* arg);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 引数の数
  ymuint32 mArgumentNum;

  // 引数のリスト
  ElbExpr** mArgumentList;

};


//////////////////////////////////////////////////////////////////////
/// @class EiTaskCall ElbMiscStmt.h "ElbMiscStmt.h"
/// @brief task call を表すクラス
//////////////////////////////////////////////////////////////////////
class EiTaskCall :
  public EiTcBase
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] task 対象のタスク
  /// @param[in] arg_num 引数の数
  /// @param[in] arg_array 引数を納める配列
  EiTaskCall(const VlNamedObj* parent,
	     ElbProcess* process,
	     const PtStmt* pt_stmt,
	     ElbTaskFunc* task,
	     ymuint arg_num,
	     ElbExpr** arg_array);

  /// @brief デストラクタ
  virtual
  ~EiTaskCall();


public:
  //////////////////////////////////////////////////////////////////////
  // ElbObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  virtual
  tVpiObjType
  type() const;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbStmt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief task の実体を返す．
  virtual
  const VlTaskFunc*
  task() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // タスク
  ElbTaskFunc* mTask;

};


//////////////////////////////////////////////////////////////////////
/// @class EiSysTaskCall ElbMiscStmt.h "ElbMiscStmt.h"
/// @brief system task call を表すクラス
//////////////////////////////////////////////////////////////////////
class EiSysTaskCall :
  public EiTcBase
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] user_systf システムタスク
  /// @param[in] arg_num 引数の数
  /// @param[in] arg_array 引数を納める配列
  EiSysTaskCall(const VlNamedObj* parent,
		ElbProcess* process,
		const PtStmt* pt_stmt,
		const ElbUserSystf* user_systf,
		ymuint arg_num,
		ElbExpr** arg_array);

  /// @brief デストラクタ
  virtual
  ~EiSysTaskCall();


public:
  //////////////////////////////////////////////////////////////////////
  // ElbObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  virtual
  tVpiObjType
  type() const;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbStmt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief user systf クラスへのポインタを返す．
  virtual
  const VlUserSystf*
  user_systf() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // user systf へのポインタ
  const ElbUserSystf* mUserSystf;

};


//////////////////////////////////////////////////////////////////////
/// @class EiDisableStmt ElbMiscStmt.h "ElbMiscStmt.h"
/// @brief disable statement を表すクラス
/// IEEE Std 1364-2001 26.6.38 Disable
//////////////////////////////////////////////////////////////////////
class EiDisableStmt :
  public EiStmtBase
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] target 対象のスコープ
  EiDisableStmt(const VlNamedObj* parent,
		ElbProcess* process,
		const PtStmt* pt_stmt,
		const VlNamedObj* target);

  /// @brief デストラクタ
  virtual
  ~EiDisableStmt();


public:
  //////////////////////////////////////////////////////////////////////
  // ElbObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  virtual
  tVpiObjType
  type() const;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbStmt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief expr を返す．
  ///
  /// 型が VlNamedObj になっているが実際には
  /// ElbFunction/ElbTask/EiNamedFork/EiNamedBegin
  /// のいずれか
  virtual
  const VlNamedObj*
  scope() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象の式
  const VlNamedObj* mExpr;

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
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] control コントロール
  /// @param[in] stmt 本体のステートメント
  EiCtrlStmt(const VlNamedObj* parent,
	     ElbProcess* process,
	     const PtStmt* pt_stmt,
	     ElbControl* control,
	     ElbStmt* stmt);

  /// @brief デストラクタ
  virtual
  ~EiCtrlStmt();


public:
  //////////////////////////////////////////////////////////////////////
  // ElbObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  virtual
  tVpiObjType
  type() const;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbStmt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief コントロールを返す．
  virtual
  const VlControl*
  control() const;

  /// @brief 本文のステートメントを返す．
  virtual
  const VlStmt*
  body_stmt() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // コントロール
  ElbControl* mControl;

  // 本文
  ElbStmt* mBodyStmt;

};

END_NAMESPACE_YM_VERILOG

#endif // EIMISCSTMT_H
