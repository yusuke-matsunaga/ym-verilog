
/// @file EiAssignment.cc
/// @brief EiAssignment の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.

#include "ei/EiFactory.h"
#include "ei/EiAssignment.h"

#include "elaborator/ElbExpr.h"

#include "ym/vl/VlControl.h"
#include "ym/BitVector.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief 代入文を生成する．
const VlStmt*
EiFactory::new_Assignment(
  const VlScope* parent,
  const VlProcess* process,
  const PtStmt* pt_stmt,
  const VlExpr* lhs,
  const VlExpr* rhs,
  bool block,
  const VlControl* control
)
{
  const VlStmt* stmt = nullptr;
  if ( block ) {
    stmt = new EiAssignment{parent, process, pt_stmt,
			    lhs, rhs, control};
  }
  else {
    stmt = new EiNbAssignment{parent, process, pt_stmt,
			      lhs, rhs, control};
  }
  return stmt;
}

// @brief assign ステートメントを生成する．
const VlStmt*
EiFactory::new_AssignStmt(
  const VlScope* parent,
  const VlProcess* process,
  const PtStmt* pt_stmt,
  const VlExpr* lhs,
  const VlExpr* rhs
)
{
  auto stmt = new EiAssignStmt{parent, process, pt_stmt,
			       lhs, rhs};
  return stmt;
}

// @brief deassign ステートメントを生成する．
const VlStmt*
EiFactory::new_DeassignStmt(
  const VlScope* parent,
  const VlProcess* process,
  const PtStmt* pt_stmt,
  const VlExpr* lhs
)
{
  auto stmt = new EiDeassignStmt{parent, process, pt_stmt,
				 lhs};

  return stmt;
}

// @brief force ステートメントを生成する．
const VlStmt*
EiFactory::new_ForceStmt(
  const VlScope* parent,
  const VlProcess* process,
  const PtStmt* pt_stmt,
  const VlExpr* lhs,
  const VlExpr* rhs
)
{
  auto stmt = new EiForceStmt{parent, process, pt_stmt,
			      lhs, rhs};
  return stmt;
}

// @brief release ステートメントを生成する．
const VlStmt*
EiFactory::new_ReleaseStmt(
  const VlScope* parent,
  const VlProcess* process,
  const PtStmt* pt_stmt,
  const VlExpr* lhs
)
{
  auto stmt = new EiReleaseStmt{parent, process, pt_stmt,
				lhs};
  return stmt;
}


//////////////////////////////////////////////////////////////////////
// クラス EiAssignBase
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiAssignBase::EiAssignBase(
  const VlScope* parent,
  const VlProcess* process,
  const PtStmt* pt_stmt,
  const VlExpr* lhs,
  const VlExpr* rhs
) : EiStmtBase{parent, process, pt_stmt},
    mLhs{lhs},
    mRhs{rhs}
{
}

// @brief デストラクタ
EiAssignBase::~EiAssignBase()
{
}

// @brief 左辺を返す．
const VlExpr*
EiAssignBase::lhs() const
{
  return mLhs;
}

// @brief 右辺を返す．
const VlExpr*
EiAssignBase::rhs() const
{
  return mRhs;
}


//////////////////////////////////////////////////////////////////////
// クラス EiNbAssignment
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiNbAssignment::EiNbAssignment(
  const VlScope* parent,
  const VlProcess* process,
  const PtStmt* pt_stmt,
  const VlExpr* lhs,
  const VlExpr* rhs,
  const VlControl* control
) : EiAssignBase{parent, process, pt_stmt, lhs, rhs},
    mControl{control}
{
}

// @brief デストラクタ
EiNbAssignment::~EiNbAssignment()
{
}

// @brief クラスの型の取得
VpiObjType
EiNbAssignment::type() const
{
  return VpiObjType::Assignment;
}

// @brief control を返す．nullptr の場合もありうる．
const VlControl*
EiNbAssignment::control() const
{
  return mControl;
}


//////////////////////////////////////////////////////////////////////
// クラス EiAssignment
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiAssignment::EiAssignment(
  const VlScope* parent,
  const VlProcess* process,
  const PtStmt* pt_stmt,
  const VlExpr* lhs,
  const VlExpr* rhs,
  const VlControl* control
) : EiNbAssignment{parent, process, pt_stmt, lhs, rhs, control}
{
}

// @brief デストラクタ
EiAssignment::~EiAssignment()
{
}

// @brief 代入のブロッキング/ノンブロッキングの区別の取得
bool
EiAssignment::is_blocking() const
{
  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス EiAssignStmt
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiAssignStmt::EiAssignStmt(
  const VlScope* parent,
  const VlProcess* process,
  const PtStmt* pt_stmt,
  const VlExpr* lhs,
  const VlExpr* rhs
) : EiAssignBase{parent, process, pt_stmt, lhs, rhs}
{
}

// @brief デストラクタ
EiAssignStmt::~EiAssignStmt()
{
}

// @brief このクラスの型を返す．
VpiObjType
EiAssignStmt::type() const
{
  return VpiObjType::AssignStmt;
}


//////////////////////////////////////////////////////////////////////
// クラス EiForceStmt
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiForceStmt::EiForceStmt(
  const VlScope* parent,
  const VlProcess* process,
  const PtStmt* pt_stmt,
  const VlExpr* lhs,
  const VlExpr* rhs
) : EiAssignBase{parent, process, pt_stmt, lhs, rhs}
{
}

// @brief デストラクタ
EiForceStmt::~EiForceStmt()
{
}

// @brief このクラスの型を返す．
VpiObjType
EiForceStmt::type() const
{
  return VpiObjType::Force;
}


//////////////////////////////////////////////////////////////////////
// クラス EiDeassignBase
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiDeassignBase::EiDeassignBase(
  const VlScope* parent,
  const VlProcess* process,
  const PtStmt* pt_stmt,
  const VlExpr* lhs
) : EiStmtBase{parent, process, pt_stmt},
    mLhs{lhs}
{
}

// @brief デストラクタ
EiDeassignBase::~EiDeassignBase()
{
}

// @brief 左辺を返す．
const VlExpr*
EiDeassignBase::lhs() const
{
  return mLhs;
}


//////////////////////////////////////////////////////////////////////
// クラス EiDeassignStmt
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiDeassignStmt::EiDeassignStmt(
  const VlScope* parent,
  const VlProcess* process,
  const PtStmt* pt_stmt,
  const VlExpr* lhs
) : EiDeassignBase{parent, process, pt_stmt, lhs}
{
}

// @brief デストラクタ
EiDeassignStmt::~EiDeassignStmt()
{
}

// @brief このクラスの型を返す．
VpiObjType
EiDeassignStmt::type() const
{
  return VpiObjType::Deassign;
}


//////////////////////////////////////////////////////////////////////
// クラス EiReleaseStmt
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiReleaseStmt::EiReleaseStmt(
  const VlScope* parent,
  const VlProcess* process,
  const PtStmt* pt_stmt,
  const VlExpr* lhs
) : EiDeassignBase{parent, process, pt_stmt, lhs}
{
}

// @brief デストラクタ
EiReleaseStmt::~EiReleaseStmt()
{
}

// @brief このクラスの型を返す．
VpiObjType
EiReleaseStmt::type() const
{
  return VpiObjType::Release;
}

END_NAMESPACE_YM_VERILOG
