
/// @file EiStmtBlockScope.cc
/// @brief EiStmtBlockScope の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2020 Yusuke Matsunaga
/// All rights reserved.


#include "ei/EiFactory.h"
#include "ei/EiStmtBlockScope.h"
#include "ym/pt/PtStmt.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @breif statement block を生成する．
// @param[in] parent 親のスコープ環境
// @param[in] pt_stmt 対応するパース木の要素
const VlScope*
EiFactory::new_StmtBlockScope(const VlScope* parent,
			      const PtStmt* pt_stmt)
{
  auto scope{new EiStmtBlockScope(parent, pt_stmt)};

  return scope;
}

//////////////////////////////////////////////////////////////////////
// クラス EiStmtBlockScope
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のスコープ環境
// @param[in] pt_stmt 対応するパース木の要素
EiStmtBlockScope::EiStmtBlockScope(const VlScope* parent,
				   const PtStmt* pt_stmt) :
  EiScope(parent),
  mPtStmt{pt_stmt}
{
}

// @brief デストラクタ
EiStmtBlockScope::~EiStmtBlockScope()
{
}

// @brief ファイル位置の取得
FileRegion
EiStmtBlockScope::file_region() const
{
  return mPtStmt->file_region();
}

// @brief 名前の取得
string
EiStmtBlockScope::name() const
{
  return mPtStmt->name();
}

END_NAMESPACE_YM_VERILOG
