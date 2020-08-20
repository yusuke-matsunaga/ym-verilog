
/// @file EiStmt.cc
/// @brief EiStmt の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.


#include "ei/EiStmt.h"
#include "ym/pt/PtStmt.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス EiStmt
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] process 親のプロセス (or nullptr)
EiStmt::EiStmt(const VlProcess* process) :
  mProcess{process}
{
}

// @brief 対象のスコープの取得
// @note kDisable/kParBlock/kSeqBlock で意味のある関数
// @note このクラスでは nullptr を返す．
const VlScope*
EiStmt::scope() const
{
  return nullptr;
}

// @brief task の実体を返す．
// @note kEnable で意味のある関数
// @note このクラスでは nullptr を返す．
const VlTaskFunc*
EiStmt::task() const
{
  return nullptr;
}

// @brief user systf クラスへのポインタを返す．
// @note kSysEnable で意味のある関数
// @note このクラスでは nullptr を返す．
const VlUserSystf*
EiStmt::user_systf() const
{
  return nullptr;
}

// @brief 引数の数の取得
// @note kEnable/kSysEnable で意味のある関数
// @note このクラスでは 0 を返す．
SizeType
EiStmt::arg_num() const
{
  return 0;
}

// @brief 引数の取得
// @param[in] pos 位置 (0 <= pos < arg_num())
// @note kEnable/kSysEnable で意味のある関数
// @note このクラスでは nullptr を返す．
const VlExpr*
EiStmt::arg(SizeType pos) const
{
  return nullptr;
}

// @brief control の取得
// @note このクラスでは nullptr を返す．
const VlControl*
EiStmt::control() const
{
  return nullptr;
}

// @brief 本体のステートメントの取得
// @note kDc/kEc/kWait/kForever/kRepeat/kWhile/kFor/kIf で意味のある関数
// @note このクラスでは nullptr を返す．
const VlStmt*
EiStmt::body_stmt() const
{
  return nullptr;
}

// @brief 式の取得
// @note kWait/kRepeat/kWhile/kFor/kIf/kCase/kCaseX/kCaseZ で意味のある関数
// @note このクラスでは nullptr を返す．
const VlExpr*
EiStmt::expr() const
{
  return nullptr;
}

// @brief 左辺式の取得
// @note kAssign/kForce/kPcAssign/kRelease/kDeassign で意味のある関数
// @note このクラスでは nullptr を返す．
const VlExpr*
EiStmt::lhs() const
{
  return nullptr;
}

// @brief 右辺式の取得
// @note kAssign/kForce/kPcAssign で意味のある関数
// @note このクラスでは nullptr を返す．
const VlExpr*
EiStmt::rhs() const
{
  return nullptr;
}

// @brief 代入のブロッキング/ノンブロッキングの区別の取得
// @note このクラスでは false を返す．
bool
EiStmt::is_blocking() const
{
  return false;
}

// @brief イベントプライマリの取得
// @note kEvent で意味のある関数
// @note このクラスでは nullptr を返す．
const VlExpr*
EiStmt::named_event() const
{
  return nullptr;
}

// @brief 条件が成り立たなかったとき実行されるステートメントの取得
// @note kIf で意味のある関数
// @note このクラスでは nullptr を返す．
const VlStmt*
EiStmt::else_stmt() const
{
  return nullptr;
}

// @brief case type の取得
// @return case type
// @note kCase/kCaseX/kCaseZ で意味のある関数
// @note このクラスでは kVpiCaseExact を返す．
VpiCaseType
EiStmt::case_type() const
{
  return VpiCaseType::Exact;
}

// @brief case item の要素数の取得
// @return case item の要素数
// @note kCase/kCaseX/kCaseZ で意味のある関数
// @note このクラスでは 0 を返す．
SizeType
EiStmt::caseitem_num() const
{
  return 0;
}

// @brief case item の取得
// @param[in] pos 位置番号 (0 <= pos < caseitem_num())
// @note kCase/kCaseX/kCaseZ で意味のある関数
// @note このクラスでは nullptr を返す．
const VlCaseItem*
EiStmt::caseitem(SizeType pos) const
{
  ASSERT_NOT_REACHED;
  return nullptr;
}

// @brief 初期化代入文の取得
// @note kFor で意味のある関数
// @note このクラスでは nullptr を返す．
const VlStmt*
EiStmt::init_stmt() const
{
  return nullptr;
}

// @brief 繰り返し代入文の取得
// @note kFor で意味のある関数
// @note このクラスでは nullptr を返す．
const VlStmt*
EiStmt::inc_stmt() const
{
  return nullptr;
}

// @brief 子供のステートメントの数の取得
// @note kParBlock/kSeqBlock で意味のある関数
// @note このクラスでは 0 を返す．
SizeType
EiStmt::child_stmt_num() const
{
  return 0;
}

// @brief 子供のステートメントの取得
// @param[in] pos 位置番号 (0 <= pos < stmt_num())
// @note kParBlock/kSeqBlock で意味のある関数
const VlStmt*
EiStmt::child_stmt(SizeType pos) const
{
  return nullptr;
}

// @brief disable 対象のスコープを得る．
const VlScope*
EiStmt::target_scope() const
{
  return nullptr;
}


//////////////////////////////////////////////////////////////////////
// クラス EiStmtBase
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] pt_stmt パース木のステートメント定義
EiStmtBase::EiStmtBase(const VlScope* parent,
		       const VlProcess* process,
		       const PtStmt* pt_stmt) :
  EiStmt(process),
  mParent{parent},
  mPtStmt{pt_stmt}
{
}

// @brief 親のスコープを返す．
const VlScope*
EiStmtBase::parent_scope() const
{
  return mParent;
}

// @brief ファイル位置を返す．
FileRegion
EiStmtBase::file_region() const
{
  return mPtStmt->file_region();
}

END_NAMESPACE_YM_VERILOG
