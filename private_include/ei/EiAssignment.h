﻿#ifndef EIASSIGNMENT_H
#define EIASSIGNMENT_H

/// @file EiAssignment.h
/// @brief 代入文を表すクラスのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.

// ステートメントを表すクラス
// IEEE Std 1364-2001 26.6.28 Assignment
// IEEE Std 1364-2001 26.6.29 Delay control
// IEEE Std 1364-2001 26.6.30 Event control
// IEEE Std 1364-2001 26.6.37 Assign statement, deassign, force, release


#include "EiStmt.h"
#include "elaborator/ElbFwd.h"


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
  EiAssignBase(const VlScope* parent,
	       const VlProcess* process,
	       const PtStmt* pt_stmt,
	       const VlExpr* lhs,
	       const VlExpr* rhs);

  /// @brief デストラクタ
  ~EiAssignBase();


public:
  //////////////////////////////////////////////////////////////////////
  // VlStmt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 左辺を返す．
  const VlExpr*
  lhs() const override;

  /// @brief 右辺を返す．
  const VlExpr*
  rhs() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 左辺
  const VlExpr* mLhs;

  // 右辺
  const VlExpr* mRhs;

};


//////////////////////////////////////////////////////////////////////
/// @class EiNbAssignment EiAssignment.h "EiAssignment.h"
/// @brief non-blocking assignment を表すクラス
/// IEEE Std 1364-2001 26.6.28 Assignment
//////////////////////////////////////////////////////////////////////
class EiNbAssignment :
  public EiAssignBase
{
public:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] lhs 左辺の式
  /// @param[in] rhs 右辺の式
  /// @param[in] control コントロール
  EiNbAssignment(const VlScope* parent,
		 const VlProcess* process,
		 const PtStmt* pt_stmt,
		 const VlExpr* lhs,
		 const VlExpr* rhs,
		 const VlControl* control);

  /// @brief デストラクタ
  ~EiNbAssignment();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  VpiObjType
  type() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // VlStmt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief control を返す．nullptr の場合もありうる．
  const VlControl*
  control() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // コントロール
  const VlControl* mControl;

};


//////////////////////////////////////////////////////////////////////
/// @class EiAssignment EiAssignment.h "EiAssignment.h"
/// @brief blocking assignment を表すクラス
/// IEEE Std 1364-2001 26.6.28 Assignment
//////////////////////////////////////////////////////////////////////
class EiAssignment :
  public EiNbAssignment
{
public:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] lhs 左辺の式
  /// @param[in] rhs 右辺の式
  /// @param[in] control コントロール
  EiAssignment(const VlScope* parent,
	       const VlProcess* process,
	       const PtStmt* pt_stmt,
	       const VlExpr* lhs,
	       const VlExpr* rhs,
	       const VlControl* control);

  /// @brief デストラクタ
  ~EiAssignment();


public:
  //////////////////////////////////////////////////////////////////////
  // VlStmt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 代入のブロッキング/ノンブロッキングの区別の取得
  /// @note このクラスでは true を返す．
  bool
  is_blocking() const override;

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
public:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] lhs 左辺の式
  /// @param[in] rhs 右辺の式
  EiAssignStmt(const VlScope* parent,
	       const VlProcess* process,
	       const PtStmt* pt_stmt,
	       const VlExpr* lhs,
	       const VlExpr* rhs);

  /// @brief デストラクタ
  ~EiAssignStmt();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  VpiObjType
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// @class EiForceStmt EiAssignment.h "EiAssignment.h"
/// @brief force statement を表すクラス
/// IEEE Std 1364-2001 26.6.37 Assign statement, deassign, force, release
//////////////////////////////////////////////////////////////////////
class EiForceStmt :
  public EiAssignBase
{
public:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] lhs 左辺の式
  /// @param[in] rhs 右辺の式
  EiForceStmt(const VlScope* parent,
	      const VlProcess* process,
	      const PtStmt* pt_stmt,
	      const VlExpr* lhs,
	      const VlExpr* rhs);

  /// @brief デストラクタ
  ~EiForceStmt();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  VpiObjType
  type() const override;

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
  EiDeassignBase(const VlScope* parent,
		 const VlProcess* process,
		 const PtStmt* pt_stmt,
		 const VlExpr* lhs);

  /// @brief デストラクタ
  ~EiDeassignBase();


public:
  //////////////////////////////////////////////////////////////////////
  // VlStmt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 左辺を返す．
  const VlExpr*
  lhs() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 左辺
  const VlExpr* mLhs;

};


//////////////////////////////////////////////////////////////////////
/// @class EiDeassignStmt EiAssignment.h "EiAssignment.h"
/// @brief deassign statement を表すクラス
/// IEEE Std 1364-2001 26.6.37 Assign statement, deassign, force, release
//////////////////////////////////////////////////////////////////////
class EiDeassignStmt :
  public EiDeassignBase
{
public:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] lhs 左辺の式
  EiDeassignStmt(const VlScope* parent,
		 const VlProcess* process,
		 const PtStmt* pt_stmt,
		 const VlExpr* lhs);

  /// @brief デストラクタ
  ~EiDeassignStmt();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  VpiObjType
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// @class EiReleaseStmt EiAssignment.h "EiAssignment.h"
/// @brief release statement を表すクラス
/// IEEE Std 1364-2001 26.6.37 Assign statement, deassign, force, release
//////////////////////////////////////////////////////////////////////
class EiReleaseStmt :
  public EiDeassignBase
{
public:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] lhs 左辺の式
  EiReleaseStmt(const VlScope* parent,
		const VlProcess* process,
		const PtStmt* pt_stmt,
		const VlExpr* lhs);

  /// @brief デストラクタ
  ~EiReleaseStmt();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  VpiObjType
  type() const override;

};

END_NAMESPACE_YM_VERILOG

#endif // EIASSIGNMENT_H
