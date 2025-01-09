
/// @file CptStmt.cc
/// @brief CptStmt の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.

#include "CptStmt.h"
#include "parser/CptFactory.h"
#include "parser/PuHierName.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// statement の共通の親クラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptStmt::CptStmt(
  const FileRegion& file_region
) : mFileRegion{file_region}
{
}

// デストラクタ
CptStmt::~CptStmt()
{
}

// ファイル位置の取得
FileRegion
CptStmt::file_region() const
{
  return mFileRegion;
}

// @brief ステートメントの種類を表す文字列の取得
const char*
CptStmt::stmt_name() const
{
  switch ( type() ) {
  case PtStmtType::Disable:       return "disable statment";
  case PtStmtType::Enable:        return "task enable statement";
  case PtStmtType::SysEnable:     return "system task enable statement";
  case PtStmtType::DelayControl:  return "delay control statement";
  case PtStmtType::EventControl:  return "event control statement";
  case PtStmtType::Assign:        return "assignment";
  case PtStmtType::NbAssign:      return "nonblocking assignment";
  case PtStmtType::Event:         return "event statement";
  case PtStmtType::Null:          return "null statement";
  case PtStmtType::If:            return "if statement";
  case PtStmtType::Case:          return "case statement";
  case PtStmtType::CaseX:         return "casex statement";
  case PtStmtType::CaseZ:         return "casez statement";
  case PtStmtType::Wait:          return "wait statement";
  case PtStmtType::Forever:       return "forever statement";
  case PtStmtType::Repeat:        return "repeat statement";
  case PtStmtType::White:         return "while statement";
  case PtStmtType::For:           return "for-loop statement";
  case PtStmtType::PcAssign:      return "procedural continuous assignment";
  case PtStmtType::Deassign:      return "deassign statement";
  case PtStmtType::Force:         return "force statement";
  case PtStmtType::Release:       return "release statement";
  case PtStmtType::NamedParBlock:
  case PtStmtType::ParBlock:      return "parallel block";
  case PtStmtType::NamedSeqBlock:
  case PtStmtType::SeqBlock:      return "sequential block";
  }
  return "";
}

// @brief 階層ブランチの要素数の取得
SizeType
CptStmt::namebranch_num() const
{
  return 0;
}

// @brief 階層ブランチの取得
const PtNameBranch*
CptStmt::namebranch(
  SizeType pos
) const
{
  ASSERT_NOT_REACHED;
  return nullptr;
}

// @brief 名前の取得
const char*
CptStmt::name() const
{
  return nullptr;
}

// @brief 引数の数の取得
SizeType
CptStmt::arg_num() const
{
  return 0;
}

// @brief 引数の取得
const PtExpr*
CptStmt::arg(
  SizeType pos
) const
{
  ASSERT_NOT_REACHED;
  return nullptr;
}

// @brief コントロールの取得
const PtControl*
CptStmt::control() const
{
  return nullptr;
}

// @brief 本体のステートメントの取得
const PtStmt*
CptStmt::body() const
{
  return nullptr;
}

// @brief 式の取得
const PtExpr*
CptStmt::expr() const
{
  return nullptr;
}

// @brief 左辺式の取得
const PtExpr*
CptStmt::lhs() const
{
  return nullptr;
}

// @brief 右辺式の取得
const PtExpr*
CptStmt::rhs() const
{
  return nullptr;
}

// @brief イベントプライマリの取得
const PtExpr*
CptStmt::primary() const
{
  return nullptr;
}

// @brief 条件が成り立たなかったとき実行されるステートメントの取得
const PtStmt*
CptStmt::else_body() const
{
  return nullptr;
}

// @brief case item のリストの要素数の取得
SizeType
CptStmt::caseitem_num() const
{
  return 0;
}

// @brief case item の取得
const PtCaseItem*
CptStmt::caseitem(
  SizeType pos
) const
{
  ASSERT_NOT_REACHED;
  return nullptr;
}

// @brief 初期化代入文の取得
const PtStmt*
CptStmt::init_stmt() const
{
  return nullptr;
}

// @brief 繰り返し代入文の取得
const PtStmt*
CptStmt::next_stmt() const
{
  return nullptr;
}

// @brief 宣言ヘッダ配列の要素数の取得
SizeType
CptStmt::declhead_num() const
{
  return 0;
}

// @brief 宣言ヘッダの取得
const PtDeclHead*
CptStmt::declhead(
  SizeType pos
) const
{
  ASSERT_NOT_REACHED;
  return nullptr;
}

// @brief 子供のステートメント配列の要素数の取得
SizeType
CptStmt::stmt_num() const
{
  return 0;
}

// @brief 子供のステートメントの取得
const PtStmt*
CptStmt::stmt(
  SizeType pos
) const
{
  ASSERT_NOT_REACHED;
  return nullptr;
}


//////////////////////////////////////////////////////////////////////
// disable 文 を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptDisable::CptDisable(
  const FileRegion& file_region,
  const char* name
) : CptStmt{file_region},
    mName{name}
{
}

// デストラクタ
CptDisable::~CptDisable()
{
}

// クラスの型を返す仮想関数
PtStmtType
CptDisable::type() const
{
  return PtStmtType::Disable;
}

// 末尾の名前を返す．
const char*
CptDisable::name() const
{
  return mName;
}


//////////////////////////////////////////////////////////////////////
// 階層つき名前を持つ disable 文 を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptDisableH::CptDisableH(
  const FileRegion& file_region,
  PtiNameBranchArray&& nb_array,
  const char* tail_name
) : CptDisable{file_region, tail_name},
    mNbArray{std::move(nb_array)}
{
}

// デストラクタ
CptDisableH::~CptDisableH()
{
}

// @brief 階層ブランチの要素数の取得
SizeType
CptDisableH::namebranch_num() const
{
  return mNbArray.size();
}

// @brief 階層ブランチの取得
const PtNameBranch*
CptDisableH::namebranch(
  SizeType pos
) const
{
  return mNbArray[pos];
}


//////////////////////////////////////////////////////////////////////
// task enable 文 / system task enable 文に共通な基底クラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptEnableBase::CptEnableBase(
  const FileRegion& file_region,
  const char* name,
  PtiExprArray&& arg_array
) : CptStmt{file_region},
    mName{name},
    mArgArray{std::move(arg_array)}
{
}

// デストラクタ
CptEnableBase::~CptEnableBase()
{
}

// 末尾の名前を返す．
const char*
CptEnableBase::name() const
{
  return mName;
}

// @brief 引数の数の取得
SizeType
CptEnableBase::arg_num() const
{
  return mArgArray.size();
}

// @brief 引数の取得
const PtExpr*
CptEnableBase::arg(
  SizeType pos
) const
{
  return mArgArray[pos];
}


//////////////////////////////////////////////////////////////////////
// 階層なしの名前を持つ task enable 文を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptEnable::CptEnable(
  const FileRegion& file_region,
  const char* name,
  PtiExprArray&& arg_array
) : CptEnableBase{file_region, name, std::move(arg_array)}
{
}

// デストラクタ
CptEnable::~CptEnable()
{
}

// クラスの型を返す仮想関数
PtStmtType
CptEnable::type() const
{
  return PtStmtType::Enable;
}


//////////////////////////////////////////////////////////////////////
// 階層つき名前を持つ enable 文 を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptEnableH::CptEnableH(
  const FileRegion& file_region,
  PtiNameBranchArray&& nb_array,
  const char* tail_name,
  PtiExprArray&& arg_array
) : CptEnable{file_region, tail_name, std::move(arg_array)},
    mNbArray{std::move(nb_array)}
{
}

// デストラクタ
CptEnableH::~CptEnableH()
{
}

// @brief 階層ブランチの要素数の取得
SizeType
CptEnableH::namebranch_num() const
{
  return mNbArray.size();
}

// @brief 階層ブランチの取得
const PtNameBranch*
CptEnableH::namebranch(
  SizeType pos
) const
{
  return mNbArray[pos];
}


//////////////////////////////////////////////////////////////////////
// system task enable 文 を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptSysEnable::CptSysEnable(
  const FileRegion& file_region,
  const char* task_name,
  PtiExprArray&& arg_array
) : CptEnableBase{file_region, task_name, std::move(arg_array)}
{
}

// デストラクタ
CptSysEnable::~CptSysEnable()
{
}

// クラスの型を返す仮想関数
PtStmtType
CptSysEnable::type() const
{
  return PtStmtType::SysEnable;
}


//////////////////////////////////////////////////////////////////////
// CtrlStmt の基底クラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptCtrlStmt::CptCtrlStmt(
  const FileRegion& file_region,
  const PtControl* delay,
  const PtStmt* body
) : CptStmt{file_region},
    mDelay{delay},
    mBody{body}
{
  ASSERT_COND( delay );
  ASSERT_COND( body );
}

// デストラクタ
CptCtrlStmt::~CptCtrlStmt()
{
}

// コントロールを返す．
const PtControl*
CptCtrlStmt::control() const
{
  return mDelay;
}

// 本体を取り出す．
const PtStmt*
CptCtrlStmt::body() const
{
  return mBody;
}


//////////////////////////////////////////////////////////////////////
// delay control statement 文 を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptDcStmt::CptDcStmt(
  const FileRegion& file_region,
  const PtControl* delay,
  const PtStmt* body
) : CptCtrlStmt{file_region, delay, body}
{
}

// デストラクタ
CptDcStmt::~CptDcStmt()
{
}

// クラスの型を返す仮想関数
PtStmtType
CptDcStmt::type() const
{
  return PtStmtType::DelayControl;
}


//////////////////////////////////////////////////////////////////////
// event control statement 文 を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptEcStmt::CptEcStmt(
  const FileRegion& file_region,
  const PtControl* event,
  const PtStmt* body
) : CptCtrlStmt{file_region, event, body}
{
}

// デストラクタ
CptEcStmt::~CptEcStmt()
{
}

// クラスの型を返す仮想関数
PtStmtType
CptEcStmt::type() const
{
  return PtStmtType::EventControl;
}


//////////////////////////////////////////////////////////////////////
// wait 文 を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptWait::CptWait(
  const FileRegion& file_region,
  const PtExpr* expr,
  const PtStmt* body
) : CptStmt{file_region},
    mExpr{expr},
    mBody{body}
{
  ASSERT_COND( expr );
  ASSERT_COND( body );
}

// デストラクタ
CptWait::~CptWait()
{
}

// クラスの型を返す仮想関数
PtStmtType
CptWait::type() const
{
  return PtStmtType::Wait;
}

// 条件を返す．
const PtExpr*
CptWait::expr() const
{
  return mExpr;
}

// 実行すべき本体を返す．
const PtStmt*
CptWait::body() const
{
  return mBody;
}


//////////////////////////////////////////////////////////////////////
// 代入文系の基底クラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptAssignBase::CptAssignBase(
  const FileRegion& file_region,
  const PtExpr* lhs
) : CptStmt{file_region},
    mLhs{lhs}
{
  ASSERT_COND( lhs );
}

// デストラクタ
CptAssignBase::~CptAssignBase()
{
}

// lhs を得る．
const PtExpr*
CptAssignBase::lhs() const
{
  return mLhs;
}


//////////////////////////////////////////////////////////////////////
// 代入文 を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptAssign::CptAssign(
  const FileRegion& file_region,
  const PtExpr* lhs,
  const PtExpr* rhs
) : CptAssignBase{file_region, lhs},
    mRhs{rhs}
{
  ASSERT_COND( rhs );
}

// デストラクタ
CptAssign::~CptAssign()
{
}

// クラスの型を返す仮想関数
PtStmtType
CptAssign::type() const
{
  return PtStmtType::Assign;
}

// rhs を得る．
const PtExpr*
CptAssign::rhs() const
{
  return mRhs;
}


//////////////////////////////////////////////////////////////////////
// コントロールつき代入文 を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptAssignC::CptAssignC(
  const FileRegion& file_region,
  const PtExpr* lhs,
  const PtExpr* rhs,
  const PtControl* control
) : CptAssign{file_region, lhs, rhs},
    mControl{control}
{
  ASSERT_COND( control );
}

// デストラクタ
CptAssignC::~CptAssignC()
{
}

// コントロールを返す．
const PtControl*
CptAssignC::control() const
{
  return mControl;
}


//////////////////////////////////////////////////////////////////////
// ノンブロッキング代入文 を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptNbAssign::CptNbAssign(
  const FileRegion& file_region,
  const PtExpr* lhs,
  const PtExpr* rhs
) : CptAssign{file_region, lhs, rhs}
{
}

// デストラクタ
CptNbAssign::~CptNbAssign()
{
}

// クラスの型を返す仮想関数
PtStmtType
CptNbAssign::type() const
{
  return PtStmtType::NbAssign;
}


//////////////////////////////////////////////////////////////////////
// コントロールつきノンブロッキング代入文 を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptNbAssignC::CptNbAssignC(
  const FileRegion& file_region,
  const PtExpr* lhs,
  const PtExpr* rhs,
  const PtControl* control
) : CptAssignC{file_region, lhs, rhs, control}
{
}

// デストラクタ
CptNbAssignC::~CptNbAssignC()
{
}

// クラスの型を返す仮想関数
PtStmtType
CptNbAssignC::type() const
{
  return PtStmtType::NbAssign;
}


//////////////////////////////////////////////////////////////////////
// procedural continuous assignment 文 を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptPcAssign::CptPcAssign(
  const FileRegion& file_region,
  const PtExpr* lhs,
  const PtExpr* rhs
) : CptAssignBase{file_region, lhs},
    mRhs{rhs}
{
  ASSERT_COND( rhs );
}

// デストラクタ
CptPcAssign::~CptPcAssign()
{
}

// クラスの型を返す仮想関数
PtStmtType
CptPcAssign::type() const
{
  return PtStmtType::PcAssign;
}

// 右辺式を返す．
const PtExpr*
CptPcAssign::rhs() const
{
  return mRhs;
}


//////////////////////////////////////////////////////////////////////
// deassign 文 を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptDeassign::CptDeassign(
  const FileRegion& file_region,
  const PtExpr* lhs
) : CptAssignBase{file_region, lhs}
{
}

// デストラクタ
CptDeassign::~CptDeassign()
{
}

// クラスの型を返す仮想関数
PtStmtType
CptDeassign::type() const
{
  return PtStmtType::Deassign;
}


//////////////////////////////////////////////////////////////////////
// force 文 を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptForce::CptForce(
  const FileRegion& file_region,
  const PtExpr* lhs,
  const PtExpr* rhs
) : CptPcAssign{file_region, lhs, rhs}
{
}

// デストラクタ
CptForce::~CptForce()
{
}

// クラスの型を返す仮想関数
PtStmtType
CptForce::type() const
{
  return PtStmtType::Force;
}


//////////////////////////////////////////////////////////////////////
// release 文 を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptRelease::CptRelease(
  const FileRegion& file_region,
  const PtExpr* lhs
) : CptDeassign{file_region, lhs}
{
}

// デストラクタ
CptRelease::~CptRelease()
{
}

// クラスの型を返す仮想関数
PtStmtType
CptRelease::type() const
{
  return PtStmtType::Release;
}


//////////////////////////////////////////////////////////////////////
// event statement を表すノード
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptEventStmt::CptEventStmt(
  const FileRegion& file_region,
  const PtExpr* event
) : CptStmt{file_region},
    mPrimary{event}
{
  ASSERT_COND( event );
}

// デストラクタ
CptEventStmt::~CptEventStmt()
{
}

// クラスの型を返す仮想関数
PtStmtType
CptEventStmt::type() const
{
  return PtStmtType::Event;
}

// イベントプライマリを返す．
const PtExpr*
CptEventStmt::primary() const
{
  return mPrimary;
}


//////////////////////////////////////////////////////////////////////
// null statement 文 を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptNullStmt::CptNullStmt(
  const FileRegion& file_region
) : CptStmt{file_region}
{
}

// デストラクタ
CptNullStmt::~CptNullStmt()
{
}

// クラスの型を返す仮想関数
PtStmtType
CptNullStmt::type() const
{
  return PtStmtType::Null;
}


//////////////////////////////////////////////////////////////////////
// if 文 を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptIf::CptIf(
  const FileRegion& file_region,
  const PtExpr* expr,
  const PtStmt* then_body
) : CptStmt{file_region},
    mCond{expr},
    mThen{then_body}
{
  ASSERT_COND( expr );
}

// デストラクタ
CptIf::~CptIf()
{
}

// クラスの型を返す仮想関数
PtStmtType
CptIf::type() const
{
  return PtStmtType::If;
}

// 条件式を返す．
const PtExpr*
CptIf::expr() const
{
  return mCond;
}

// 成り立ったとき実行されるステートメント
const PtStmt*
CptIf::body() const
{
  return mThen;
}


//////////////////////////////////////////////////////////////////////
// else 節付きの if 文 を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptIfElse::CptIfElse(
  const FileRegion& file_region,
  const PtExpr* expr,
  const PtStmt* then_body,
  const PtStmt* else_body
) : CptIf{file_region, expr, then_body},
    mElse{else_body}
{
  ASSERT_COND( expr );
}

// デストラクタ
CptIfElse::~CptIfElse()
{
}

// 成り立たなかったとき実行されるステートメント
const PtStmt*
CptIfElse::else_body() const
{
  return mElse;
}


//////////////////////////////////////////////////////////////////////
// case 文 を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptCase::CptCase(
  const FileRegion& file_region,
  const PtExpr* expr,
  PtiCaseItemArray&& caseitem_array
) : CptStmt{file_region},
    mExpr{expr},
    mCaseItemArray{std::move(caseitem_array)}
{
  ASSERT_COND( expr );
}

// デストラクタ
CptCase::~CptCase()
{
}

// クラスの型を返す仮想関数
PtStmtType
CptCase::type() const
{
  return PtStmtType::Case;
}

// 比較される式を返す．
const PtExpr*
CptCase::expr() const
{
  return mExpr;
}

// @brief case item のリストの要素数の取得
SizeType
CptCase::caseitem_num() const
{
  return mCaseItemArray.size();
}

// @brief case item の取得
const PtCaseItem*
CptCase::caseitem(
  SizeType pos
) const
{
  return mCaseItemArray[pos];
}


//////////////////////////////////////////////////////////////////////
// casex 文 を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptCaseX::CptCaseX(
  const FileRegion& file_region,
  const PtExpr* expr,
  PtiCaseItemArray&& caseitem_array
) : CptCase{file_region, expr, std::move(caseitem_array)}
{
}

// デストラクタ
CptCaseX::~CptCaseX()
{
}

// クラスの型を返す仮想関数
PtStmtType
CptCaseX::type() const
{
  return PtStmtType::CaseX;
}


//////////////////////////////////////////////////////////////////////
// casez 文 を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptCaseZ::CptCaseZ(
  const FileRegion& file_region,
  const PtExpr* expr,
  PtiCaseItemArray&& caseitem_array
) : CptCase{file_region, expr, std::move(caseitem_array)}
{
}

// デストラクタ
CptCaseZ::~CptCaseZ()
{
}

// クラスの型を返す仮想関数
PtStmtType
CptCaseZ::type() const
{
  return PtStmtType::CaseZ;
}


//////////////////////////////////////////////////////////////////////
// caseitem を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptCaseItem::CptCaseItem(
  const FileRegion& file_region,
  PtiExprArray&& label_array,
  const PtStmt* body
) : mFileRegion{file_region},
    mLabelArray{std::move(label_array)},
    mBody{body}
{
  ASSERT_COND( body );
}

// デストラクタ
CptCaseItem::~CptCaseItem()
{
}

// ファイル位置を返す．
FileRegion
CptCaseItem::file_region() const
{
  return mFileRegion;
}

// @brief ラベルのリストの要素数の取得
SizeType
CptCaseItem::label_num() const
{
  return mLabelArray.size();
}

// @brief ラベルの取得
const PtExpr*
CptCaseItem::label(
  SizeType pos
) const
{
  return mLabelArray[pos];
}

// 本体のステートメント得る．
const PtStmt*
CptCaseItem::body() const
{
  return mBody;
}


//////////////////////////////////////////////////////////////////////
// LoopStmt のベースクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptLoopStmt::CptLoopStmt(
  const FileRegion& file_region,
  const PtStmt* body
) : CptStmt{file_region},
    mBody{body}
{
  ASSERT_COND( body );
}

// デストラクタ
CptLoopStmt::~CptLoopStmt()
{
}

// 実行すべき本体を返す．
const PtStmt*
CptLoopStmt::body() const
{
  return mBody;
}


//////////////////////////////////////////////////////////////////////
// forever 文 を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptForever::CptForever(
  const FileRegion& file_region,
  const PtStmt* body
) : CptLoopStmt{file_region, body}
{
}

// デストラクタ
CptForever::~CptForever()
{
}

// クラスの型を返す仮想関数
PtStmtType
CptForever::type() const
{
  return PtStmtType::Forever;
}


//////////////////////////////////////////////////////////////////////
// repeat 文 を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptRepeat::CptRepeat(
  const FileRegion& file_region,
  const PtExpr* expr,
  const PtStmt* body
) : CptLoopStmt{file_region, body},
    mExpr{expr}
{
  ASSERT_COND( expr );
}

// デストラクタ
CptRepeat::~CptRepeat()
{
}

// クラスの型を返す仮想関数
PtStmtType
CptRepeat::type() const
{
  return PtStmtType::Repeat;
}

// 繰り返し式を返す．
const PtExpr*
CptRepeat::expr() const
{
  return mExpr;
}


//////////////////////////////////////////////////////////////////////
// while 文 を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptWhile::CptWhile(
  const FileRegion& file_region,
  const PtExpr* expr,
  const PtStmt* body
) : CptRepeat{file_region, expr, body}
{
}

// デストラクタ
CptWhile::~CptWhile()
{
}

// クラスの型を返す仮想関数
PtStmtType
CptWhile::type() const
{
  return PtStmtType::White;
}


//////////////////////////////////////////////////////////////////////
// for 文 を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptFor::CptFor(
  const FileRegion& file_region,
  const PtStmt* init,
  const PtExpr* cond,
  const PtStmt* next,
  const PtStmt* body
) : CptRepeat{file_region, cond, body},
    mInit{init},
    mNext{next}
{
  ASSERT_COND( init );
  ASSERT_COND( next );
}

// デストラクタ
CptFor::~CptFor()
{
}

// クラスの型を返す仮想関数
PtStmtType
CptFor::type() const
{
  return PtStmtType::For;
}

// 初期化代入式を取出す
const PtStmt*
CptFor::init_stmt() const
{
  return mInit;
}

// 繰り返し代入式を取出す
const PtStmt*
CptFor::next_stmt() const
{
  return mNext;
}


//////////////////////////////////////////////////////////////////////
// StmtBlock の基底クラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptStmtBlock::CptStmtBlock(
  const FileRegion& file_region,
  PtiStmtArray&& stmt_array
) : CptStmt{file_region},
    mStmtArray{std::move(stmt_array)}
{
}

// デストラクタ
CptStmtBlock::~CptStmtBlock()
{
}

// @brief 子供のステートメント配列の要素数の取得
SizeType
CptStmtBlock::stmt_num() const
{
  return mStmtArray.size();
}

// @brief 子供のステートメントの取得
const PtStmt*
CptStmtBlock::stmt(
  SizeType pos
) const
{
  return mStmtArray[pos];
}


//////////////////////////////////////////////////////////////////////
// 名前と宣言要素を持つ StmtBlock
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptStmtBlockN::CptStmtBlockN(
  const FileRegion& file_region,
  const char* name,
  PtiDeclHeadArray&& declhead_array,
  PtiStmtArray&& stmt_array
) : CptStmtBlock{file_region, std::move(stmt_array)},
    mName{name},
    mDeclHeadArray{std::move(declhead_array)}
{
}

// デストラクタ
CptStmtBlockN::~CptStmtBlockN()
{
}

// 名前を取り出す．
const char*
CptStmtBlockN::name() const
{
  return mName;
}

// @brief 宣言ヘッダ配列の要素数の取得
SizeType
CptStmtBlockN::declhead_num() const
{
  return mDeclHeadArray.size();
}

// @brief 宣言ヘッダの取得
const PtDeclHead*
CptStmtBlockN::declhead(
  SizeType pos
) const
{
  return mDeclHeadArray[pos];
}


//////////////////////////////////////////////////////////////////////
// parallel block を表すノード
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptParBlock::CptParBlock(
  const FileRegion& file_region,
  PtiStmtArray&& stmt_array
) : CptStmtBlock{file_region, std::move(stmt_array)}
{
}

// デストラクタ
CptParBlock::~CptParBlock()
{
}

// クラスの型を返す仮想関数
PtStmtType
CptParBlock::type() const
{
  return PtStmtType::ParBlock;
}


//////////////////////////////////////////////////////////////////////
// 名前と宣言要素を持つ parallel block を表すノード
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptParBlockN::CptParBlockN(
  const FileRegion& file_region,
  const char* name,
  PtiDeclHeadArray&& declhead_array,
  PtiStmtArray&& stmt_array
) : CptStmtBlockN{file_region, name, std::move(declhead_array), std::move(stmt_array)}
{
}

// デストラクタ
CptParBlockN::~CptParBlockN()
{
}

// クラスの型を返す仮想関数
PtStmtType
CptParBlockN::type() const
{
  return PtStmtType::NamedParBlock;
}


//////////////////////////////////////////////////////////////////////
// sequential block を表すノード
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptSeqBlock::CptSeqBlock(
  const FileRegion& file_region,
  PtiStmtArray&& stmt_array
) : CptStmtBlock{file_region, std::move(stmt_array)}
{
}

// デストラクタ
CptSeqBlock::~CptSeqBlock()
{
}

// クラスの型を返す仮想関数
PtStmtType
CptSeqBlock::type() const
{
  return PtStmtType::SeqBlock;
}


//////////////////////////////////////////////////////////////////////
// 名前と宣言要素を持つ sequential block を表すノード
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptSeqBlockN::CptSeqBlockN(
  const FileRegion& file_region,
  const char* name,
  PtiDeclHeadArray&& declhead_array,
  PtiStmtArray&& stmt_array
) : CptStmtBlockN{file_region, name,
		  std::move(declhead_array),
		  std::move(stmt_array)}
{
}

// デストラクタ
CptSeqBlockN::~CptSeqBlockN()
{
}

// クラスの型を返す仮想関数
PtStmtType
CptSeqBlockN::type() const
{
  return PtStmtType::NamedSeqBlock;
}


//////////////////////////////////////////////////////////////////////
// statement 関係
//////////////////////////////////////////////////////////////////////

// disable 文を生成する．
const PtStmt*
CptFactory::new_Disable(
  const FileRegion& file_region,
  const char* name
)
{
  ++ mNumDisable;
  void* p = mAlloc.get_memory(sizeof(CptDisable));
  auto obj = new (p) CptDisable{file_region, name};
  return obj;
}

const PtStmt*
CptFactory::new_Disable(
  const FileRegion& file_region,
  PuHierName* hname
)
{
  ++ mNumDisableH;
  void* p = mAlloc.get_memory(sizeof(CptDisableH));
  auto nb_array = hname->name_branch_to_vector();
  auto tail_name = hname->tail_name();
  auto obj = new (p) CptDisableH{file_region,
				 PtiArray<const PtNameBranch>{mAlloc, nb_array},
				 tail_name};
  return obj;
}

// enable 文を生成する．
const PtStmt*
CptFactory::new_Enable(
  const FileRegion& file_region,
  const char* name,
  const vector<const PtExpr*>& arg_array
)
{
  ++ mNumEnable;
  void* p = mAlloc.get_memory(sizeof(CptEnable));
  auto obj = new (p) CptEnable{file_region, name,
			       PtiArray<const PtExpr>{mAlloc, arg_array}};
  return obj;
}

const PtStmt*
CptFactory::new_Enable(
  const FileRegion& file_region,
  PuHierName* hname,
  const vector<const PtExpr*>& arg_array
)
{
  ++ mNumEnableH;
  void* p = mAlloc.get_memory(sizeof(CptEnableH));
  auto nb_array = hname->name_branch_to_vector();
  auto tail_name = hname->tail_name();
  auto obj = new (p) CptEnableH{file_region,
				PtiArray<const PtNameBranch>{mAlloc, nb_array},
				tail_name,
				PtiArray<const PtExpr>{mAlloc, arg_array}};
  return obj;
}

// system task enable 文を生成する．
const PtStmt*
CptFactory::new_SysEnable(
  const FileRegion& file_region,
  const char* task_name,
  const vector<const PtExpr*>& arg_array
)
{
  ++ mNumSysEnable;
  void* p = mAlloc.get_memory(sizeof(CptSysEnable));
  auto obj = new (p) CptSysEnable{file_region, task_name,
				  PtiArray<const PtExpr>{mAlloc, arg_array}};
  return obj;
}

// delay control 文を生成する．
const PtStmt*
CptFactory::new_DcStmt(
  const FileRegion& file_region,
  const PtControl* delay,
  const PtStmt* body
)
{
  ++ mNumDcStmt;
  void* p = mAlloc.get_memory(sizeof(CptDcStmt));
  auto obj = new (p) CptDcStmt{file_region, delay, body};
  return obj;
}

// event control 文を生成する．
const PtStmt*
CptFactory::new_EcStmt(
  const FileRegion& file_region,
  const PtControl* event,
  const PtStmt* body
)
{
  ++ mNumEcStmt;
  void* p = mAlloc.get_memory(sizeof(CptEcStmt));
  auto obj = new (p) CptEcStmt{file_region, event, body};
  return obj;
}

// wait 文を生成する．
const PtStmt*
CptFactory::new_Wait(
  const FileRegion& file_region,
  const PtExpr* cond,
  const PtStmt* body
)
{
  ++ mNumWait;
  void* p = mAlloc.get_memory(sizeof(CptWait));
  auto obj = new (p) CptWait{file_region, cond, body};
  return obj;
}

// assign 文を生成する．
const PtStmt*
CptFactory::new_Assign(
  const FileRegion& file_region,
  const PtExpr* lhs,
  const PtExpr* rhs
)
{
  ++ mNumAssign;
  void* p = mAlloc.get_memory(sizeof(CptAssign));
  auto obj = new (p) CptAssign{file_region, lhs, rhs};
  return obj;
}

// assign 文を生成する．
const PtStmt*
CptFactory::new_Assign(
  const FileRegion& file_region,
  const PtExpr* lhs,
  const PtExpr* rhs,
  const PtControl* control
)
{
  ++ mNumAssignC;
  void* p = mAlloc.get_memory(sizeof(CptAssignC));
  auto obj = new (p) CptAssignC{file_region, lhs, rhs, control};
  return obj;
}

// nonblocking assign 文を生成する．
const PtStmt*
CptFactory::new_NbAssign(
  const FileRegion& file_region,
  const PtExpr* lhs,
  const PtExpr* rhs
)
{
  ++ mNumNbAssign;
  void* p = mAlloc.get_memory(sizeof(CptNbAssign));
  auto obj = new (p) CptNbAssign{file_region, lhs, rhs};
  return obj;
}

// nonblocking assign 文を生成する．
const PtStmt*
CptFactory::new_NbAssign(
  const FileRegion& file_region,
  const PtExpr* lhs,
  const PtExpr* rhs,
  const PtControl* control
)
{
  ++ mNumNbAssignC;
  void* p = mAlloc.get_memory(sizeof(CptNbAssignC));
  auto obj = new (p) CptNbAssignC{file_region, lhs, rhs, control};
  return obj;
}

// event 文を生成する．
const PtStmt*
CptFactory::new_EventStmt(
  const FileRegion& file_region,
  const PtExpr* event
)
{
  ++ mNumEventStmt;
  void* p = mAlloc.get_memory(sizeof(CptEventStmt));
  auto obj = new (p) CptEventStmt{file_region, event};
  return obj;
}

// null 文を生成する．
const PtStmt*
CptFactory::new_NullStmt(
  const FileRegion& file_region
)
{
  ++ mNumNullStmt;
  void* p = mAlloc.get_memory(sizeof(CptNullStmt));
  auto obj = new (p) CptNullStmt{file_region};
  return obj;
}

// if 文を生成する．
const PtStmt*
CptFactory::new_If(
  const FileRegion& file_region,
  const PtExpr* expr,
  const PtStmt* then_body
)
{
  ++ mNumIf;
  void* p = mAlloc.get_memory(sizeof(CptIf));
  auto obj = new (p) CptIf{file_region, expr, then_body};
  return obj;
}

// if 文を生成する．
const PtStmt*
CptFactory::new_If(
  const FileRegion& file_region,
  const PtExpr* expr,
  const PtStmt* then_body,
  const PtStmt* else_body
)
{
  ++ mNumIf;
  void* p = mAlloc.get_memory(sizeof(CptIfElse));
  auto obj = new (p) CptIfElse{file_region, expr, then_body, else_body};
  return obj;
}

// case 文を生成する．
const PtStmt*
CptFactory::new_Case(
  const FileRegion& file_region,
  const PtExpr* expr,
  const vector<const PtCaseItem*>& caseitem_array
)
{
  ++ mNumCase;
  void* p = mAlloc.get_memory(sizeof(CptCase));
  auto obj = new (p) CptCase{file_region, expr,
			     PtiArray<const PtCaseItem>{mAlloc, caseitem_array}};
  return obj;
}

// casex 文を生成する．
const PtStmt*
CptFactory::new_CaseX(
  const FileRegion& file_region,
  const PtExpr* expr,
  const vector<const PtCaseItem*>& caseitem_array
)
{
  ++ mNumCaseX;
  void* p = mAlloc.get_memory(sizeof(CptCaseX));
  auto obj = new (p) CptCaseX{file_region, expr,
			      PtiArray<const PtCaseItem>{mAlloc, caseitem_array}};
  return obj;
}

// casez 文を生成する．
const PtStmt*
CptFactory::new_CaseZ(
  const FileRegion& file_region,
  const PtExpr* expr,
  const vector<const PtCaseItem*>& caseitem_array
)
{
  ++ mNumCaseZ;
  void* p = mAlloc.get_memory(sizeof(CptCaseZ));
  auto obj = new (p) CptCaseZ{file_region, expr,
			      PtiArray<const PtCaseItem>{mAlloc, caseitem_array}};
  return obj;
}

// case item を生成する．
const PtCaseItem*
CptFactory::new_CaseItem(
  const FileRegion& file_region,
  const vector<const PtExpr*>& label_array,
  const PtStmt* body
)
{
  ++ mNumCaseItem;
  void* p = mAlloc.get_memory(sizeof(CptCaseItem));
  auto obj = new (p) CptCaseItem{file_region,
				 PtiArray<const PtExpr>{mAlloc, label_array},
				 body};
  return obj;
}

// forever 文を生成する．
const PtStmt*
CptFactory::new_Forever(
  const FileRegion& file_region,
  const PtStmt* body
)
{
  ++ mNumForever;
  void* p = mAlloc.get_memory(sizeof(CptForever));
  auto obj = new (p) CptForever{file_region, body};
  return obj;
}

// repeat 文を生成する．
const PtStmt*
CptFactory::new_Repeat(
  const FileRegion& file_region,
  const PtExpr* expr,
  const PtStmt* body
)
{
  ++ mNumRepeat;
  void* p = mAlloc.get_memory(sizeof(CptRepeat));
  auto obj = new (p) CptRepeat{file_region, expr, body};
  return obj;
}

// while 文を生成する．
const PtStmt*
CptFactory::new_While(
  const FileRegion& file_region,
  const PtExpr* cond,
  const PtStmt* body
)
{
  ++ mNumWhile;
  void* p = mAlloc.get_memory(sizeof(CptWhile));
  auto obj = new (p) CptWhile{file_region, cond, body};
  return obj;
}

// for 文を生成する．
const PtStmt*
CptFactory::new_For(
  const FileRegion& file_region,
  const PtStmt* init,
  const PtExpr* cond,
  const PtStmt* next,
  const PtStmt* body
)
{
  ++ mNumFor;
  void* p = mAlloc.get_memory(sizeof(CptFor));
  auto obj = new (p) CptFor{file_region, init, cond, next, body};
  return obj;
}

// procedural continuous assignment 文を生成する．
const PtStmt*
CptFactory::new_PcAssign(
  const FileRegion& file_region,
  const PtExpr* lhs,
  const PtExpr* rhs
)
{
  ++ mNumPcAssign;
  void* p = mAlloc.get_memory(sizeof(CptPcAssign));
  auto obj = new (p) CptPcAssign{file_region, lhs, rhs};
  return obj;
}

// deassign 文を生成する．
const PtStmt*
CptFactory::new_Deassign(
  const FileRegion& file_region,
  const PtExpr* lhs
)
{
  ++ mNumDeassign;
  void* p = mAlloc.get_memory(sizeof(CptDeassign));
  auto obj = new (p) CptDeassign{file_region, lhs};
  return obj;
}

// force 文を生成する．
const PtStmt*
CptFactory::new_Force(
  const FileRegion& file_region,
  const PtExpr* lhs,
  const PtExpr* rhs
)
{
  ++ mNumForce;
  void* p = mAlloc.get_memory(sizeof(CptForce));
  auto obj = new (p) CptForce{file_region, lhs, rhs};
  return obj;
}

// release 文を生成する．
const PtStmt*
CptFactory::new_Release(
  const FileRegion& file_region,
  const PtExpr* lhs
)
{
  ++ mNumRelease;
  void* p = mAlloc.get_memory(sizeof(CptRelease));
  auto obj = new (p) CptRelease{file_region, lhs};
  return obj;
}

// par block を生成する．
const PtStmt*
CptFactory::new_ParBlock(
  const FileRegion& file_region,
  const vector<const PtStmt*>& stmt_array
)
{
  ++ mNumParBlock;
  void* p = mAlloc.get_memory(sizeof(CptParBlock));
  auto obj = new (p) CptParBlock{file_region,
				 PtiArray<const PtStmt>{mAlloc, stmt_array}};
  return obj;
}

// par block を生成する．
const PtStmt*
CptFactory::new_NamedParBlock(const FileRegion& file_region,
			      const char* name,
			      const vector<const PtDeclHead*>& declhead_array,
			      const vector<const PtStmt*>& stmt_array)
{
  ++ mNumParBlockN;
  void* p = mAlloc.get_memory(sizeof(CptParBlockN));
  auto obj = new (p) CptParBlockN{file_region, name,
				  PtiArray<const PtDeclHead>{mAlloc, declhead_array},
				  PtiArray<const PtStmt>{mAlloc, stmt_array}};
  return obj;
}

// seq block を生成する．
const PtStmt*
CptFactory::new_SeqBlock(
  const FileRegion& file_region,
  const vector<const PtStmt*>& stmt_array
)
{
  ++ mNumSeqBlock;
  void* p = mAlloc.get_memory(sizeof(CptSeqBlock));
  auto obj = new (p) CptSeqBlock{file_region,
				 PtiArray<const PtStmt>{mAlloc, stmt_array}};
  return obj;
}

// seq block を生成する．
const PtStmt*
CptFactory::new_NamedSeqBlock(
  const FileRegion& file_region,
  const char* name,
  const vector<const PtDeclHead*>& declhead_array,
  const vector<const PtStmt*>& stmt_array
)
{
  ++ mNumSeqBlockN;
  void* p = mAlloc.get_memory(sizeof(CptSeqBlockN));
  auto obj = new (p) CptSeqBlockN{file_region, name,
				  PtiArray<const PtDeclHead>{mAlloc, declhead_array},
				  PtiArray<const PtStmt>{mAlloc, stmt_array}};
  return obj;
}

END_NAMESPACE_YM_VERILOG
