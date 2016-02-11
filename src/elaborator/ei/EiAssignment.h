﻿#ifndef EIASSIGNMENT_H
#define EIASSIGNMENT_H

/// @file EiAssignment.h
/// @brief 代入文を表すクラスのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.

// ステートメントを表すクラス
// IEEE Std 1364-2001 26.6.28 Assignment
// IEEE Std 1364-2001 26.6.29 Delay control
// IEEE Std 1364-2001 26.6.30 Event control
// IEEE Std 1364-2001 26.6.37 Assign statement, deassign, force, release


#include "EiStmt.h"
#include "ElbFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiAssignBase EiAssignment.h "EiAssignment.h"
/// @brief 代入文の基底クラス
//////////////////////////////////////////////////////////////////////
class EiAssignBase :
  public EiStmtBase
{
protected:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] lhs 左辺の式
  /// @param[in] rhs 右辺の式
  EiAssignBase(const VlNamedObj* parent,
	       ElbProcess* process,
	       const PtStmt* pt_stmt,
	       ElbExpr* lhs,
	       ElbExpr* rhs);

  /// @brief デストラクタ
  virtual
  ~EiAssignBase();


public:
  //////////////////////////////////////////////////////////////////////
  // VlStmt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 左辺を返す．
  virtual
  const VlExpr*
  lhs() const;

  /// @brief 右辺を返す．
  virtual
  const VlExpr*
  rhs() const;


protected:

  /// @brief 左辺を返す．
  ElbExpr*
  _lhs() const;

  /// @brief 右辺を返す．
  ElbExpr*
  _rhs() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 左辺
  ElbExpr* mLhs;

  // 右辺
  ElbExpr* mRhs;

};


//////////////////////////////////////////////////////////////////////
/// @class EiNbAssignment EiAssignment.h "EiAssignment.h"
/// @brief non-blocking assignment を表すクラス
/// IEEE Std 1364-2001 26.6.28 Assignment
//////////////////////////////////////////////////////////////////////
class EiNbAssignment :
  public EiAssignBase
{
  friend class EiFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] lhs 左辺の式
  /// @param[in] rhs 右辺の式
  /// @param[in] control コントロール
  EiNbAssignment(const VlNamedObj* parent,
		 ElbProcess* process,
		 const PtStmt* pt_stmt,
		 ElbExpr* lhs,
		 ElbExpr* rhs,
		 ElbControl* control);

  /// @brief デストラクタ
  virtual
  ~EiNbAssignment();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  virtual
  tVpiObjType
  type() const;


public:
  //////////////////////////////////////////////////////////////////////
  // VlStmt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief control を返す．nullptr の場合もありうる．
  virtual
  const VlControl*
  control() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // コントロール
  ElbControl* mControl;

};


//////////////////////////////////////////////////////////////////////
/// @class EiAssignment EiAssignment.h "EiAssignment.h"
/// @brief blocking assignment を表すクラス
/// IEEE Std 1364-2001 26.6.28 Assignment
//////////////////////////////////////////////////////////////////////
class EiAssignment :
  public EiNbAssignment
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] lhs 左辺の式
  /// @param[in] rhs 右辺の式
  /// @param[in] control コントロール
  EiAssignment(const VlNamedObj* parent,
	       ElbProcess* process,
	       const PtStmt* pt_stmt,
	       ElbExpr* lhs,
	       ElbExpr* rhs,
	       ElbControl* control);

  /// @brief デストラクタ
  virtual
  ~EiAssignment();


public:
  //////////////////////////////////////////////////////////////////////
  // VlStmt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 代入のブロッキング/ノンブロッキングの区別の取得
  /// @note このクラスでは true を返す．
  virtual
  bool
  is_blocking() const;

};


//////////////////////////////////////////////////////////////////////
/// @class EiAssignStmt EiAssignment.h "EiAssignment.h"
/// @brief assign statement を表すクラス
/// assignment とは別物
/// IEEE Std 1364-2001 26.6.37 Assign statement, deassign, force, release
//////////////////////////////////////////////////////////////////////
class EiAssignStmt :
  public EiAssignBase
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] lhs 左辺の式
  /// @param[in] rhs 右辺の式
  EiAssignStmt(const VlNamedObj* parent,
	       ElbProcess* process,
	       const PtStmt* pt_stmt,
	       ElbExpr* lhs,
	       ElbExpr* rhs);

  /// @brief デストラクタ
  virtual
  ~EiAssignStmt();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  virtual
  tVpiObjType
  type() const;

};


//////////////////////////////////////////////////////////////////////
/// @class EiForceStmt EiAssignment.h "EiAssignment.h"
/// @brief force statement を表すクラス
/// IEEE Std 1364-2001 26.6.37 Assign statement, deassign, force, release
//////////////////////////////////////////////////////////////////////
class EiForceStmt :
  public EiAssignBase
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] lhs 左辺の式
  /// @param[in] rhs 右辺の式
  EiForceStmt(const VlNamedObj* parent,
	      ElbProcess* process,
	      const PtStmt* pt_stmt,
	      ElbExpr* lhs,
	      ElbExpr* rhs);

  /// @brief デストラクタ
  virtual
  ~EiForceStmt();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  virtual
  tVpiObjType
  type() const;

};


//////////////////////////////////////////////////////////////////////
/// @class ElbDeassignBase EiAssignment.h "EiAssignment.h"
/// @brief deassign/release の基底クラス
//////////////////////////////////////////////////////////////////////
class EiDeassignBase :
  public EiStmtBase
{
protected:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] lhs 左辺の式
  EiDeassignBase(const VlNamedObj* parent,
		 ElbProcess* process,
		 const PtStmt* pt_stmt,
		 ElbExpr* lhs);

  /// @brief デストラクタ
  virtual
  ~EiDeassignBase();


public:
  //////////////////////////////////////////////////////////////////////
  // VlStmt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 左辺を返す．
  virtual
  const VlExpr*
  lhs() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 左辺
  ElbExpr* mLhs;

};


//////////////////////////////////////////////////////////////////////
/// @class EiDeassignStmt EiAssignment.h "EiAssignment.h"
/// @brief deassign statement を表すクラス
/// IEEE Std 1364-2001 26.6.37 Assign statement, deassign, force, release
//////////////////////////////////////////////////////////////////////
class EiDeassignStmt :
  public EiDeassignBase
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] lhs 左辺の式
  EiDeassignStmt(const VlNamedObj* parent,
		 ElbProcess* process,
		 const PtStmt* pt_stmt,
		 ElbExpr* lhs);

  /// @brief デストラクタ
  virtual
  ~EiDeassignStmt();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  virtual
  tVpiObjType
  type() const;

};


//////////////////////////////////////////////////////////////////////
/// @class EiReleaseStmt EiAssignment.h "EiAssignment.h"
/// @brief release statement を表すクラス
/// IEEE Std 1364-2001 26.6.37 Assign statement, deassign, force, release
//////////////////////////////////////////////////////////////////////
class EiReleaseStmt :
  public EiDeassignBase
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] lhs 左辺の式
  EiReleaseStmt(const VlNamedObj* parent,
		ElbProcess* process,
		const PtStmt* pt_stmt,
		ElbExpr* lhs);

  /// @brief デストラクタ
  virtual
  ~EiReleaseStmt();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  virtual
  tVpiObjType
  type() const;

};

END_NAMESPACE_YM_VERILOG

#endif // EIASSIGNMENT_H
