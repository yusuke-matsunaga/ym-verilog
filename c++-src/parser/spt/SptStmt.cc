
/// @file SptStmt.cc
/// @brief SptStmt の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "SptStmt.h"
#include "parser/SptFactory.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// ステートメントを表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
// @param file_region ファイル位置の情報
// @param type 型
// @param nblist 階層ブランチのリスト
// @param name 名前
// @param control コントロール
// @param body1 ステートメント1
// @param body2 ステートメント2
// @param body3 ステートメント3
// @param expr1 式1
// @param expr2 式2
// @param caseitem_array case item のリスト
// @param decl_array 宣言のリスト
// @param stmt_array ステートメントのリスト
// @param arg_array 引数のリスト
SptStmt::SptStmt(const FileRegion& file_region,
		 PtStmtType type,
		 const PtStmt* body1,
		 const PtStmt* body2,
		 const PtStmt* body3,
		 const PtExpr* expr1,
		 const PtExpr* expr2,
		 const PtControl* control,
		 const char* name,
		 PtNameBranchArray nb_array,
		 PtCaseItemArray caseitem_array,
		 PtDeclHeadArray decl_array,
		 PtStmtArray stmt_array,
		 PtExprArray arg_array) :
  mFileRegion{file_region},
  mType{type},
  mNbArray{nb_array},
  mName{name},
  mControl{control},
  mBody1{body1},
  mBody2{body2},
  mBody3{body3},
  mExpr1{expr1},
  mExpr2{expr2},
  mCaseItemArray{caseitem_array},
  mDeclArray{decl_array},
  mStmtArray{stmt_array},
  mArgArray{arg_array}
{
}

// デストラクタ
SptStmt::~SptStmt()
{
}

// ファイル位置の取得
// @return ファイル位置の情報
FileRegion
SptStmt::file_region() const
{
  return mFileRegion;
}

// クラスの型の取得
// @return 型
PtStmtType
SptStmt::type() const
{
  return mType;
}

// @brief ステートメントの種類を表す文字列の取得
// @return ステートメントの種類を表す文字列
const char*
SptStmt::stmt_name() const
{
  switch ( type() ) {
  case PtStmtType::Disable:   return "disable statment";
  case PtStmtType::Enable:    return "task enable statement";
  case PtStmtType::SysEnable: return "system task enable statement";
  case PtStmtType::DelayControl:        return "delay control statement";
  case PtStmtType::EventControl:        return "event control statement";
  case PtStmtType::Assign:    return "assignment";
  case PtStmtType::NbAssign:  return "nonblocking assignment";
  case PtStmtType::Event:     return "event statement";
  case PtStmtType::Null:      return "null statement";
  case PtStmtType::If:        return "if statement";
  case PtStmtType::Case:      return "case statement";
  case PtStmtType::CaseX:     return "casex statement";
  case PtStmtType::CaseZ:     return "casez statement";
  case PtStmtType::Wait:      return "wait statement";
  case PtStmtType::Forever:   return "forever statement";
  case PtStmtType::Repeat:    return "repeat statement";
  case PtStmtType::White:     return "while statement";
  case PtStmtType::For:       return "for-loop statement";
  case PtStmtType::PcAssign:  return "procedural continuous assignment";
  case PtStmtType::Deassign:  return "deassign statement";
  case PtStmtType::Force:     return "force statement";
  case PtStmtType::Release:   return "release statement";
  case PtStmtType::NamedParBlock:
  case PtStmtType::ParBlock:  return "parallel block";
  case PtStmtType::NamedSeqBlock:
  case PtStmtType::SeqBlock:  return "sequential block";
  }
  return "";
}

// 階層ブランチの取得
// kDisable/kEnable/kSysEnable で意味のある関数
PtNameBranchArray
SptStmt::namebranch_array() const
{
  return mNbArray;
}

// 名前の取得
// @return 名前
// kDisable/kEnable/kSysEnable/kParBlock/kSeqBlock で意味のある関数
const char*
SptStmt::name() const
{
  return mName;
}

// @brief 引数のリストの取得
PtExprArray
SptStmt::arg_list() const
{
  return mArgArray;
}

// コントロールの取得
// @return ディレイ/イベントコントロール
// kDc/kEc で意味のある関数
const PtControl*
SptStmt::control() const
{
  return mControl;
}

// 本体のステートメントの取得
// @return 本体のステートメント
// kDc/kEc/kWait/kForever/kRepeat/kWhile/kFor/kIf で意味のある関数
const PtStmt*
SptStmt::body() const
{
  return mBody1;
}

// 式の取得
// @return 式
// kWait/kRepeat/kWhile/kFor/kIf/kCase/kCaseX/kCaseZ で意味のある関数
const PtExpr*
SptStmt::expr() const
{
  return mExpr1;
}

// 左辺式の取得
// @return 左辺式
// kAssign/kForce/kPcAssign/kRelease/kDeassign で意味のある関数
const PtExpr*
SptStmt::lhs() const
{
  return mExpr1;
}

// 右辺式の取得
// @return 右辺式
// kAssign/kForce/kPcAssign で意味のある関数
const PtExpr*
SptStmt::rhs() const
{
  return mExpr2;
}

// イベントプライマリの取得
// @return イベントプライマリ
// kEvent で意味のある関数
const PtExpr*
SptStmt::primary() const
{
  return mExpr1;
}

// 条件が成り立たなかったとき実行されるステートメントの取得
// @return 条件が成り立たなかったとき実行されるステートメント
// kIf で意味のある関数
const PtStmt*
SptStmt::else_body() const
{
  return mBody2;
}

// @brief case item のリストの取得
PtCaseItemArray
SptStmt::caseitem_list() const
{
  return mCaseItemArray;
}

// 初期化代入文の取得
// @return 初期化代入文
// kFor で意味のある関数
const PtStmt*
SptStmt::init_stmt() const
{
  return mBody2;
}

// 繰り返し代入文の取得
// @return 繰り返し代入文
// kFor で意味のある関数
const PtStmt*
SptStmt::next_stmt() const
{
  return mBody3;
}

// @brief 宣言ヘッダ配列の取得
// @note kNamedParBlock/kNamedSeqBlock で意味のある関数
PtDeclHeadArray
SptStmt::declhead_array() const
{
  return mDeclArray;
}

// @brief 子供のステートメント配列の取得
// @note kParBlock/kSeqBlock で意味のある関数
PtStmtArray
SptStmt::stmt_array() const
{
  return mStmtArray;
}


//////////////////////////////////////////////////////////////////////
// caseitem を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
SptCaseItem::SptCaseItem(const FileRegion& file_region,
			 PtExprArray label_array,
			 const PtStmt* body) :
  mFileRegion{file_region},
  mLabelArray{label_array},
  mBody{body}
{
  ASSERT_COND( body );
}

// デストラクタ
SptCaseItem::~SptCaseItem()
{
}

// ファイル位置を返す．
FileRegion
SptCaseItem::file_region() const
{
  return mFileRegion;
}

// @brief ラベルのリストの取得
PtExprArray
SptCaseItem::label_list() const
{
  return mLabelArray;
}

// 本体のステートメント得る．
const PtStmt*
SptCaseItem::body() const
{
  return mBody;
}


//////////////////////////////////////////////////////////////////////
// statement 関係
//////////////////////////////////////////////////////////////////////

// @brief disable 文の生成
// @param[in] file_region ファイル位置の情報
// @param[in] name 停止するスコープ名
// @return 生成された disable 文
const PtStmt*
SptFactory::new_Disable(const FileRegion& file_region,
			const char* name)
{
  auto node = new SptStmt(file_region,
			  PtStmtType::Disable,
			  nullptr, nullptr, nullptr,
			  nullptr, nullptr,
			  nullptr,
			  name);
  return node;
}

// @brief disable 文の生成 (階層付き識別子)
// @param[in] file_region ファイル位置の情報
// @param[in] nb_top 階層名のリスト
// @param[in] name 末尾の名前
// @return 生成された disable 文
const PtStmt*
SptFactory::new_Disable(const FileRegion& file_region,
			PtNameBranchArray nb_array,
			const char* name)
{
  auto node = new SptStmt(file_region,
			  PtStmtType::Disable,
			  nullptr, nullptr, nullptr,
			  nullptr, nullptr,
			  nullptr,
			  name,
			  nb_array);
  return node;
}

// @brief enable 文の生成
// @param[in] file_region ファイル位置の情報
// @param[in] name 起動するタスク名
// @param[in] arg_array 引数のリスト
// @return 生成された enable 文
const PtStmt*
SptFactory::new_Enable(const FileRegion& file_region,
		       const char* name,
		       PtExprArray arg_array)
{
  auto node = new SptStmt(file_region,
			  PtStmtType::Enable,
			  nullptr, nullptr, nullptr,
			  nullptr, nullptr,
			  nullptr,
			  name,
			  PtNameBranchArray(),
			  PtCaseItemArray(),
			  PtDeclHeadArray(),
			  PtStmtArray(),
			  arg_array);
  return node;
}

// @brief enable 文の生成 (階層付き識別子)
// @param[in] file_region ファイル位置の情報
// @param[in] nb_top 階層名のリスト
// @param[in] name 末尾の名前
// @param[in] arg_array 引数のリスト
// @return 生成された enable 文
const PtStmt*
SptFactory::new_Enable(const FileRegion& file_region,
		       PtNameBranchArray nb_array,
		       const char* name,
		       PtExprArray arg_array)
{
  auto node = new SptStmt(file_region,
			  PtStmtType::Enable,
			  nullptr, nullptr, nullptr,
			  nullptr, nullptr,
			  nullptr,
			  name,
			  nb_array,
			  PtCaseItemArray(),
			  PtDeclHeadArray(),
			  PtStmtArray(),
			  arg_array);
  return node;
}

// @brief system task enable 文の生成
// @param[in] file_region ファイル位置の情報
// @param[in] name 起動するシステムタスク名
// @param[in] arg_array 引数のリスト
// @return 生成された system task enable 文
const PtStmt*
SptFactory::new_SysEnable(const FileRegion& file_region,
			  const char* name,
			  PtExprArray arg_array)
{
  auto node = new SptStmt(file_region,
			  PtStmtType::SysEnable,
			  nullptr, nullptr, nullptr,
			  nullptr, nullptr,
			  nullptr,
			  name,
			  PtNameBranchArray(),
			  PtCaseItemArray(),
			  PtDeclHeadArray(),
			  PtStmtArray(),
			  arg_array);
  return node;
}

// @brief delay control 文の生成
// @param[in] file_region ファイル位置の情報
// @param[in] delay delay control (delay control 文とは別物)
// @param[in] body 本体のステートメント
// @return 生成された delay control 文
const PtStmt*
SptFactory::new_DcStmt(const FileRegion& file_region,
		       const PtControl* delay,
		       const PtStmt* body)
{
  auto node = new SptStmt(file_region,
			  PtStmtType::DelayControl,
			  body, nullptr, nullptr,
			  nullptr, nullptr,
			  delay);
  return node;
}

// @brief event control 文の生成
// @param[in] file_region ファイル位置の情報
// @param[in] event delay control (event control 文とは別物)
// @param[in] body 本体のステートメント
// @return 生成された event control 文
const PtStmt*
SptFactory::new_EcStmt(const FileRegion& file_region,
		       const PtControl* event,
		       const PtStmt* body)
{
  auto node = new SptStmt(file_region,
			  PtStmtType::EventControl,
			  body, nullptr, nullptr,
			  nullptr, nullptr,
			  event);
  return node;
}

// @brief wait 文の生成
// @param[in] file_region ファイル位置の情報
// @param[in] cond 条件式
// @param[in] body 本体のステートメント
// @return 生成された wait 文
const PtStmt*
SptFactory::new_Wait(const FileRegion& file_region,
		     const PtExpr* cond,
		     const PtStmt* body)
{
  auto node = new SptStmt(file_region,
			  PtStmtType::Wait,
			  body, nullptr, nullptr,
			  cond, nullptr,
			  nullptr);
  return node;
}

// @brief assign 文の生成
// @param[in] file_region ファイル位置の情報
// @param[in] lhs 左辺式
// @param[in] rhs 右辺式
// @return 生成された assign 文
const PtStmt*
SptFactory::new_Assign(const FileRegion& file_region,
		       const PtExpr* lhs,
		       const PtExpr* rhs)
{
  auto node = new SptStmt(file_region,
			  PtStmtType::Assign,
			  nullptr, nullptr, nullptr,
			  lhs, rhs);
  return node;
}

// @brief control 付き assign 文の生成
// @param[in] file_region ファイル位置の情報
// @param[in] lhs 左辺式
// @param[in] rhs 右辺式
// @param[in] control 制御式
// @return 生成された assign 文
const PtStmt*
SptFactory::new_Assign(const FileRegion& file_region,
		       const PtExpr* lhs,
		       const PtExpr* rhs,
		       const PtControl* control)
{
  auto node = new SptStmt(file_region,
			  PtStmtType::Assign,
			  nullptr, nullptr, nullptr,
			  lhs, rhs,
			  control);
  return node;
}

// @brief nonblocking assign 文の生成
// @param[in] file_region ファイル位置の情報
// @param[in] lhs 左辺式
// @param[in] rhs 右辺式
// @return 生成された nonblocking assign 文
const PtStmt*
SptFactory::new_NbAssign(const FileRegion& file_region,
			 const PtExpr* lhs,
			 const PtExpr* rhs)
{
  auto node = new SptStmt(file_region,
			  PtStmtType::NbAssign,
			  nullptr, nullptr, nullptr,
			  lhs, rhs);
  return node;
}

// @brief control 付き nonblocking assign 文の生成
// @param[in] file_region ファイル位置の情報
// @param[in] lhs 左辺式
// @param[in] rhs 右辺式
// @param[in] control 制御式
// @return 生成された nonblocking assign 文
const PtStmt*
SptFactory::new_NbAssign(const FileRegion& file_region,
			 const PtExpr* lhs,
			 const PtExpr* rhs,
			 const PtControl* control)
{
  auto node = new SptStmt(file_region,
			  PtStmtType::NbAssign,
			  nullptr, nullptr, nullptr,
			  lhs, rhs,
			  control);
  return node;
}

// @brief event 文の生成
// @param[in] file_region ファイル位置の情報
// @param[in] event イベント名
// @return 生成された event 文
const PtStmt*
SptFactory::new_EventStmt(const FileRegion& file_region,
			  const PtExpr* event)
{
  auto node = new SptStmt(file_region,
			  PtStmtType::Event,
			  nullptr, nullptr, nullptr,
			  event, nullptr);
  return node;
}

// @brief null 文の生成
// @param[in] file_region ファイル位置の情報
// @return 生成された null 文
const PtStmt*
SptFactory::new_NullStmt(const FileRegion& file_region)
{
  auto node = new SptStmt(file_region,
			  PtStmtType::Null);
  return node;
}

// @brief if 文の生成
// @param[in] file_region ファイル位置の情報
// @param[in] expr 条件式
// @param[in] then_body 成り立ったときに実行されるステートメント
// @return 生成された if 文
const PtStmt*
SptFactory::new_If(const FileRegion& file_region,
		   const PtExpr* expr,
		   const PtStmt* then_body)
{
  auto node = new SptStmt(file_region,
			  PtStmtType::If,
			  then_body, nullptr, nullptr,
			  expr, nullptr);
  return node;
}

// @brief if 文の生成
// @param[in] file_region ファイル位置の情報
// @param[in] expr 条件式
// @param[in] then_body 成り立ったときに実行されるステートメント
// @param[in] else_body 成り立たなかったときに実行されるステートメント
// @return 生成された if 文
const PtStmt*
SptFactory::new_If(const FileRegion& file_region,
		   const PtExpr* expr,
		   const PtStmt* then_body,
		   const PtStmt* else_body)
{
  auto node = new SptStmt(file_region,
			  PtStmtType::If,
			  then_body, else_body, nullptr,
			  expr, nullptr);
  return node;
}

// @brief case 文の生成
// @param[in] file_region ファイル位置の情報
// @param[in] expr 条件式
// @param[in] caseitem_list case item のリスト
// @return 生成された case 文
const PtStmt*
SptFactory::new_Case(const FileRegion& file_region,
		     const PtExpr* expr,
		     PtCaseItemArray caseitem_array)
{
  auto node = new SptStmt(file_region,
			  PtStmtType::Case,
			  nullptr, nullptr, nullptr,
			  expr, nullptr,
			  nullptr,
			  nullptr,
			  PtNameBranchArray(),
			  caseitem_array);
  return node;
}

// @brief casex 文の生成
// @param[in] file_region ファイル位置の情報
// @param[in] expr 条件式
// @param[in] caseitem_list case item のリスト
// @return 生成された casex 文
const PtStmt*
SptFactory::new_CaseX(const FileRegion& file_region,
		      const PtExpr* expr,
		      PtCaseItemArray caseitem_array)
{
  auto node = new SptStmt(file_region,
			  PtStmtType::CaseX,
			  nullptr, nullptr, nullptr,
			  expr, nullptr,
			  nullptr,
			  nullptr,
			  PtNameBranchArray(),
			  caseitem_array);
  return node;
}

// @brief casez 文の生成
// @param[in] file_region ファイル位置の情報
// @param[in] expr 条件式
// @param[in] caseitem_list case item のリスト
// @return 生成された casez 文
const PtStmt*
SptFactory::new_CaseZ(const FileRegion& file_region,
		      const PtExpr* expr,
		      PtCaseItemArray caseitem_array)
{
  auto node = new SptStmt(file_region,
			  PtStmtType::CaseZ,
			  nullptr, nullptr, nullptr,
			  expr, nullptr,
			  nullptr,
			  nullptr,
			  PtNameBranchArray(),
			  caseitem_array);
  return node;
}

// @brief case item の生成
// @param[in] file_region ファイル位置の情報
// @param[in] label ラベルのリスト
// @param[in] body 本体のステートメント
// @return 生成された case item
const PtCaseItem*
SptFactory::new_CaseItem(const FileRegion& file_region,
			 PtExprArray label_array,
			 const PtStmt* body)
{
  auto node = new SptCaseItem(file_region, label_array, body);
  return node;
}

// @brief forever 文の生成
// @param[in] file_region ファイル位置の情報
// @param[in] body 本体のステートメント
// @return 生成された forever 文
const PtStmt*
SptFactory::new_Forever(const FileRegion& file_region,
			const PtStmt* body)
{
  auto node = new SptStmt(file_region,
			  PtStmtType::Forever,
			  body, nullptr, nullptr);
  return node;
}

// @brief repeat 文の生成
// @param[in] file_region ファイル位置の情報
// @param[in] expr 繰り返し数を表す式
// @param[in] body 本体のステートメント
// @return 生成された repeat 文
const PtStmt*
SptFactory::new_Repeat(const FileRegion& file_region,
		       const PtExpr* expr,
		       const PtStmt* body)
{
  auto node = new SptStmt(file_region,
			  PtStmtType::Repeat,
			  body, nullptr, nullptr,
			  expr, nullptr);
  return node;
}

// @brief while 文の生成
// @param[in] file_region ファイル位置の情報
// @param[in] cond 繰り返し条件式
// @param[in] body 本体のステートメント
// @return 生成された while 文
const PtStmt*
SptFactory::new_While(const FileRegion& file_region,
		      const PtExpr* cond,
		      const PtStmt* body)
{
  auto node = new SptStmt(file_region,
			  PtStmtType::White,
			  body, nullptr, nullptr,
			  cond, nullptr);
  return node;
}

// @brief for 文の生成
// @param[in] file_region ファイル位置の情報
// @param[in] init 初期化文
// @param[in] cond 繰り返し条件式
// @param[in] next 増加文
// @param[in] body 本体のステートメント
// @return 生成された for 文
const PtStmt*
SptFactory::new_For(const FileRegion& file_region,
		    const PtStmt* init,
		    const PtExpr* cond,
		    const PtStmt* next,
		    const PtStmt* body)
{
  auto node = new SptStmt(file_region,
			  PtStmtType::For,
			  body, init, next,
			  cond, nullptr);
  return node;
}

// @brief procedural assign 文の生成
// @param[in] file_region ファイル位置の情報
// @param[in] lhs 左辺式
// @param[in] rhs 右辺式
// @return 生成された procedural assign 文
const PtStmt*
SptFactory::new_PcAssign(const FileRegion& file_region,
			 const PtExpr* lhs,
			 const PtExpr* rhs)
{
  auto node = new SptStmt(file_region,
			  PtStmtType::PcAssign,
			  nullptr, nullptr, nullptr,
			  lhs, rhs);
  return node;
}

// @brief deassign 文の生成
// @param[in] file_region ファイル位置の情報
// @param[in] lhs 左辺式
// @return 生成された deassign 文
const PtStmt*
SptFactory::new_Deassign(const FileRegion& file_region,
			 const PtExpr* lhs)
{
  auto node = new SptStmt(file_region,
			  PtStmtType::Deassign,
			  nullptr, nullptr, nullptr,
			  lhs, nullptr);
  return node;
}

// @brief force 文の生成
// @param[in] file_region ファイル位置の情報
// @param[in] lhs 左辺式
// @param[in] rhs 右辺式
// @return 生成された force 文
const PtStmt*
SptFactory::new_Force(const FileRegion& file_region,
		      const PtExpr* lhs,
		      const PtExpr* rhs)
{
  auto node = new SptStmt(file_region,
			  PtStmtType::Force,
			  nullptr, nullptr, nullptr,
			  lhs, rhs);
  return node;
}

// @brief release 文の生成
// @param[in] file_region ファイル位置の情報
// @param[in] lhs 左辺式
// @return 生成された release 文
const PtStmt*
SptFactory::new_Release(const FileRegion& file_region,
			const PtExpr* lhs)
{
  auto node = new SptStmt(file_region,
			  PtStmtType::Release,
			  nullptr, nullptr, nullptr,
			  lhs, nullptr);
  return node;
}

// @brief parallel block の生成
// @param[in] file_region ファイル位置の情報
// @param[in] stmt_top 本体のステートメントのリスト
// @return 生成された parallel block
const PtStmt*
SptFactory::new_ParBlock(const FileRegion& file_region,
			 PtStmtArray stmt_array)
{
  auto node = new SptStmt(file_region,
			  PtStmtType::ParBlock,
			  nullptr, nullptr, nullptr,
			  nullptr, nullptr,
			  nullptr,
			  nullptr,
			  PtNameBranchArray(),
			  PtCaseItemArray(),
			  PtDeclHeadArray(),
			  stmt_array);
  return node;
}

// @brief 名前付き parallel block の生成
// @param[in] file_region ファイル位置の情報
// @param[in] name ブロック名
// @param[in] decl_top 宣言のリスト
// @param[in] stmt_top 本体のステートメントのリスト
// @return 生成された parallel block
const PtStmt*
SptFactory::new_NamedParBlock(const FileRegion& file_region,
			      const char* name,
			      PtDeclHeadArray decl_array,
			      PtStmtArray stmt_array)
{
  auto node = new SptStmt(file_region,
			  PtStmtType::NamedParBlock,
			  nullptr, nullptr, nullptr,
			  nullptr, nullptr,
			  nullptr,
			  name,
			  PtNameBranchArray(),
			  PtCaseItemArray(),
			  decl_array,
			  stmt_array);
  return node;
}

// @brief sequential block の生成
// @param[in] file_region ファイル位置の情報
// @param[in] stmt_top 本体のステートメントのリスト
// @return 生成された sequential block
const PtStmt*
SptFactory::new_SeqBlock(const FileRegion& file_region,
			 PtStmtArray stmt_array)
{
  auto node = new SptStmt(file_region,
			  PtStmtType::SeqBlock,
			  nullptr, nullptr, nullptr,
			  nullptr, nullptr,
			  nullptr,
			  nullptr,
			  PtNameBranchArray(),
			  PtCaseItemArray(),
			  PtDeclHeadArray(),
			  stmt_array);
  return node;
}

// @brief 名前付き sequential block の生成
// @param[in] file_region ファイル位置の情報
// @param[in] name ブロック名
// @param[in] decl_top 宣言のリスト
// @param[in] stmt_top 本体のステートメントのリスト
// @return 生成された sequential block
const PtStmt*
SptFactory::new_NamedSeqBlock(const FileRegion& file_region,
			      const char* name,
			      PtDeclHeadArray decl_array,
			      PtStmtArray stmt_array)
{
  auto node = new SptStmt(file_region,
			  PtStmtType::NamedSeqBlock,
			  nullptr, nullptr, nullptr,
			  nullptr, nullptr,
			  nullptr,
			  name,
			  PtNameBranchArray(),
			  PtCaseItemArray(),
			  decl_array,
			  stmt_array);
  return node;
}

END_NAMESPACE_YM_VERILOG
