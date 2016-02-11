﻿
/// @file SptStmt.cc
/// @brief SptStmt の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "SptStmt.h"
#include "SptFactory.h"


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
		 tPtStmtType type,
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
  mFileRegion(file_region),
  mType(type),
  mNbArray(nb_array),
  mName(name),
  mControl(control),
  mBody1(body1),
  mBody2(body2),
  mBody3(body3),
  mExpr1(expr1),
  mExpr2(expr2),
  mCaseItemArray(caseitem_array),
  mDeclArray(decl_array),
  mStmtArray(stmt_array),
  mArgArray(arg_array)
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
tPtStmtType
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
  case kPtDisableStmt:   return "disable statment";
  case kPtEnableStmt:    return "task enable statement";
  case kPtSysEnableStmt: return "system task enable statement";
  case kPtDcStmt:        return "delay control statement";
  case kPtEcStmt:        return "event control statement";
  case kPtAssignStmt:    return "assignment";
  case kPtNbAssignStmt:  return "nonblocking assignment";
  case kPtEventStmt:     return "event statement";
  case kPtNullStmt:      return "null statement";
  case kPtIfStmt:        return "if statement";
  case kPtCaseStmt:      return "case statement";
  case kPtCaseXStmt:     return "casex statement";
  case kPtCaseZStmt:     return "casez statement";
  case kPtWaitStmt:      return "wait statement";
  case kPtForeverStmt:   return "forever statement";
  case kPtRepeatStmt:    return "repeat statement";
  case kPtWhileStmt:     return "while statement";
  case kPtForStmt:       return "for-loop statement";
  case kPtPcAssignStmt:  return "procedural continuous assignment";
  case kPtDeassignStmt:  return "deassign statement";
  case kPtForceStmt:     return "force statement";
  case kPtReleaseStmt:   return "release statement";
  case kPtNamedParBlockStmt:
  case kPtParBlockStmt:  return "parallel block";
  case kPtNamedSeqBlockStmt:
  case kPtSeqBlockStmt:  return "sequential block";
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

// @brief 引数の数の取得
// @return 引数の数
// @note kEnable/kSysEnable で意味のある関数
ymuint
SptStmt::arg_num() const
{
  return mArgArray.size();
}

// @brief 引数の取得
// @param[in] pos 位置番号 ( 0 <= pos < arg_num() )
// @note kEnable/kSysEnable で意味のある関数
const PtExpr*
SptStmt::arg(ymuint pos) const
{
  return mArgArray[pos];
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

// case item の要素数の取得
// @return case item の要素数
// kCase/kCaseX/kCaseZ で意味のある関数
ymuint
SptStmt::caseitem_num() const
{
  return mCaseItemArray.size();
}

// case item の取得
// kCase/kCaseX/kCaseZ で意味のある関数
const PtCaseItem*
SptStmt::caseitem(ymuint pos) const
{
  return mCaseItemArray[pos];
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
  mFileRegion(file_region),
  mLabelArray(label_array),
  mBody(body)
{
  ASSERT_COND(body );
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

// @brief ラベルの数の取得
// @retval ラベルの数 通常の case ラベルの場合
// @retval 0 default の場合
ymuint
SptCaseItem::label_num() const
{
  return mLabelArray.size();
}

// @brief ラベルの取得
// @param[in] pos 位置番号 ( 0 <= pos < label_num() )
const PtExpr*
SptCaseItem::label(ymuint pos) const
{
  return mLabelArray[pos];
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
  void* p = alloc().get_memory(sizeof(SptStmt));
  return new (p) SptStmt(file_region,
			 kPtDisableStmt,
			 nullptr, nullptr, nullptr,
			 nullptr, nullptr,
			 nullptr,
			 name);
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
  void* p = alloc().get_memory(sizeof(SptStmt));
  return new (p) SptStmt(file_region,
			 kPtDisableStmt,
			 nullptr, nullptr, nullptr,
			 nullptr, nullptr,
			 nullptr,
			 name,
			 nb_array);
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
  void* p = alloc().get_memory(sizeof(SptStmt));
  return new (p) SptStmt(file_region,
			 kPtEnableStmt,
			 nullptr, nullptr, nullptr,
			 nullptr, nullptr,
			 nullptr,
			 name,
			 PtNameBranchArray(),
			 PtCaseItemArray(),
			 PtDeclHeadArray(),
			 PtStmtArray(),
			 arg_array);
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
  void* p = alloc().get_memory(sizeof(SptStmt));
  return new (p) SptStmt(file_region,
			 kPtEnableStmt,
			 nullptr, nullptr, nullptr,
			 nullptr, nullptr,
			 nullptr,
			 name,
			 nb_array,
			 PtCaseItemArray(),
			 PtDeclHeadArray(),
			 PtStmtArray(),
			 arg_array);
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
  void* p = alloc().get_memory(sizeof(SptStmt));
  return new (p) SptStmt(file_region,
			 kPtSysEnableStmt,
			 nullptr, nullptr, nullptr,
			 nullptr, nullptr,
			 nullptr,
			 name,
			 PtNameBranchArray(),
			 PtCaseItemArray(),
			 PtDeclHeadArray(),
			 PtStmtArray(),
			 arg_array);
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
  void* p = alloc().get_memory(sizeof(SptStmt));
  return new (p) SptStmt(file_region,
			 kPtDcStmt,
			 body, nullptr, nullptr,
			 nullptr, nullptr,
			 delay);
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
  void* p = alloc().get_memory(sizeof(SptStmt));
  return new (p) SptStmt(file_region,
			 kPtEcStmt,
			 body, nullptr, nullptr,
			 nullptr, nullptr,
			 event);
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
  void* p = alloc().get_memory(sizeof(SptStmt));
  return new (p) SptStmt(file_region,
			 kPtWaitStmt,
			 body, nullptr, nullptr,
			 cond, nullptr,
			 nullptr);
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
  void* p = alloc().get_memory(sizeof(SptStmt));
  return new (p) SptStmt(file_region,
			 kPtAssignStmt,
			 nullptr, nullptr, nullptr,
			 lhs, rhs);
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
  void* p = alloc().get_memory(sizeof(SptStmt));
  return new (p) SptStmt(file_region,
			 kPtAssignStmt,
			 nullptr, nullptr, nullptr,
			 lhs, rhs,
			 control);
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
  void* p = alloc().get_memory(sizeof(SptStmt));
  return new (p) SptStmt(file_region,
			 kPtNbAssignStmt,
			 nullptr, nullptr, nullptr,
			 lhs, rhs);
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
  void* p = alloc().get_memory(sizeof(SptStmt));
  return new (p) SptStmt(file_region,
			 kPtNbAssignStmt,
			 nullptr, nullptr, nullptr,
			 lhs, rhs,
			 control);
}

// @brief event 文の生成
// @param[in] file_region ファイル位置の情報
// @param[in] event イベント名
// @return 生成された event 文
const PtStmt*
SptFactory::new_EventStmt(const FileRegion& file_region,
			  const PtExpr* event)
{
  void* p = alloc().get_memory(sizeof(SptStmt));
  return new (p) SptStmt(file_region,
			 kPtEventStmt,
			 nullptr, nullptr, nullptr,
			 event, nullptr);
}

// @brief null 文の生成
// @param[in] file_region ファイル位置の情報
// @return 生成された null 文
const PtStmt*
SptFactory::new_NullStmt(const FileRegion& file_region)
{
  void* p = alloc().get_memory(sizeof(SptStmt));
  return new (p) SptStmt(file_region,
			 kPtNullStmt);
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
  void* p = alloc().get_memory(sizeof(SptStmt));
  return new (p) SptStmt(file_region,
			 kPtIfStmt,
			 then_body, nullptr, nullptr,
			 expr, nullptr);
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
  void* p = alloc().get_memory(sizeof(SptStmt));
  return new (p) SptStmt(file_region,
			 kPtIfStmt,
			 then_body, else_body, nullptr,
			 expr, nullptr);
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
  void* p = alloc().get_memory(sizeof(SptStmt));
  return new (p) SptStmt(file_region,
			 kPtCaseStmt,
			 nullptr, nullptr, nullptr,
			 expr, nullptr,
			 nullptr,
			 nullptr,
			 PtNameBranchArray(),
			 caseitem_array);
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
  void* p = alloc().get_memory(sizeof(SptStmt));
  return new (p) SptStmt(file_region,
			 kPtCaseXStmt,
			 nullptr, nullptr, nullptr,
			 expr, nullptr,
			 nullptr,
			 nullptr,
			 PtNameBranchArray(),
			 caseitem_array);
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
  void* p = alloc().get_memory(sizeof(SptStmt));
  return new (p) SptStmt(file_region,
			 kPtCaseZStmt,
			 nullptr, nullptr, nullptr,
			 expr, nullptr,
			 nullptr,
			 nullptr,
			 PtNameBranchArray(),
			 caseitem_array);
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
  void* p = alloc().get_memory(sizeof(SptCaseItem));
  return new (p) SptCaseItem(file_region, label_array, body);
}

// @brief forever 文の生成
// @param[in] file_region ファイル位置の情報
// @param[in] body 本体のステートメント
// @return 生成された forever 文
const PtStmt*
SptFactory::new_Forever(const FileRegion& file_region,
			const PtStmt* body)
{
  void* p = alloc().get_memory(sizeof(SptStmt));
  return new (p) SptStmt(file_region,
			 kPtForeverStmt,
			 body, nullptr, nullptr);
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
  void* p = alloc().get_memory(sizeof(SptStmt));
  return new (p) SptStmt(file_region,
			 kPtRepeatStmt,
			 body, nullptr, nullptr,
			 expr, nullptr);
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
  void* p = alloc().get_memory(sizeof(SptStmt));
  return new (p) SptStmt(file_region,
			 kPtWhileStmt,
			 body, nullptr, nullptr,
			 cond, nullptr);
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
  void* p = alloc().get_memory(sizeof(SptStmt));
  return new (p) SptStmt(file_region,
			 kPtForStmt,
			 body, init, next,
			 cond, nullptr);
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
  void* p = alloc().get_memory(sizeof(SptStmt));
  return new (p) SptStmt(file_region,
			 kPtPcAssignStmt,
			 nullptr, nullptr, nullptr,
			 lhs, rhs);
}

// @brief deassign 文の生成
// @param[in] file_region ファイル位置の情報
// @param[in] lhs 左辺式
// @return 生成された deassign 文
const PtStmt*
SptFactory::new_Deassign(const FileRegion& file_region,
			 const PtExpr* lhs)
{
  void* p = alloc().get_memory(sizeof(SptStmt));
  return new (p) SptStmt(file_region,
			 kPtDeassignStmt,
			 nullptr, nullptr, nullptr,
			 lhs, nullptr);
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
  void* p = alloc().get_memory(sizeof(SptStmt));
  return new (p) SptStmt(file_region,
			 kPtForceStmt,
			 nullptr, nullptr, nullptr,
			 lhs, rhs);
}

// @brief release 文の生成
// @param[in] file_region ファイル位置の情報
// @param[in] lhs 左辺式
// @return 生成された release 文
const PtStmt*
SptFactory::new_Release(const FileRegion& file_region,
			const PtExpr* lhs)
{
  void* p = alloc().get_memory(sizeof(SptStmt));
  return new (p) SptStmt(file_region,
			 kPtReleaseStmt,
			 nullptr, nullptr, nullptr,
			 lhs, nullptr);
}

// @brief parallel block の生成
// @param[in] file_region ファイル位置の情報
// @param[in] stmt_top 本体のステートメントのリスト
// @return 生成された parallel block
const PtStmt*
SptFactory::new_ParBlock(const FileRegion& file_region,
			 PtStmtArray stmt_array)
{
  void* p = alloc().get_memory(sizeof(SptStmt));
  return new (p) SptStmt(file_region,
			 kPtParBlockStmt,
			 nullptr, nullptr, nullptr,
			 nullptr, nullptr,
			 nullptr,
			 nullptr,
			 PtNameBranchArray(),
			 PtCaseItemArray(),
			 PtDeclHeadArray(),
			 stmt_array);
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
  void* p = alloc().get_memory(sizeof(SptStmt));
  return new (p) SptStmt(file_region,
			 kPtNamedParBlockStmt,
			 nullptr, nullptr, nullptr,
			 nullptr, nullptr,
			 nullptr,
			 name,
			 PtNameBranchArray(),
			 PtCaseItemArray(),
			 decl_array,
			 stmt_array);
}

// @brief sequential block の生成
// @param[in] file_region ファイル位置の情報
// @param[in] stmt_top 本体のステートメントのリスト
// @return 生成された sequential block
const PtStmt*
SptFactory::new_SeqBlock(const FileRegion& file_region,
			 PtStmtArray stmt_array)
{
  void* p = alloc().get_memory(sizeof(SptStmt));
  return new (p) SptStmt(file_region,
			 kPtSeqBlockStmt,
			 nullptr, nullptr, nullptr,
			 nullptr, nullptr,
			 nullptr,
			 nullptr,
			 PtNameBranchArray(),
			 PtCaseItemArray(),
			 PtDeclHeadArray(),
			 stmt_array);
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
  void* p = alloc().get_memory(sizeof(SptStmt));
  return new (p) SptStmt(file_region,
			 kPtNamedSeqBlockStmt,
			 nullptr, nullptr, nullptr,
			 nullptr, nullptr,
			 nullptr,
			 name,
			 PtNameBranchArray(),
			 PtCaseItemArray(),
			 decl_array,
			 stmt_array);
}

END_NAMESPACE_YM_VERILOG
