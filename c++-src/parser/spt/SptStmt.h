#ifndef SPTSTMT_H
#define SPTSTMT_H

/// @file SptStmt.h
/// @brief SptStmt のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym/FileRegion.h"

#include "ym/pt/PtStmt.h"
#include "ym/pt/PtArray.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// ステートメントを表すクラス
//////////////////////////////////////////////////////////////////////
class SptStmt :
  public PtStmt
{
  friend class SptFactory;

private:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ/デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// コンストラクタ
  SptStmt(const FileRegion& file_region,
	  PtStmtType type,
	  const PtStmt* body1 = nullptr,
	  const PtStmt* body2 = nullptr,
	  const PtStmt* body3 = nullptr,
	  const PtExpr* expr1 = nullptr,
	  const PtExpr* expr2 = nullptr,
	  const PtControl* control = nullptr,
	  const char* name = nullptr,
	  const PtNameBranchArray* nb_array = nullptr,
	  const PtCaseItemArray* caseitem_array = nullptr,
	  const PtDeclHeadArray* decl_array = nullptr,
	  const PtStmtArray* stmt_array = nullptr,
	  const PtExprArray* expr_array = nullptr);

  /// デストラクタ
  ~SptStmt();


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// ファイル位置の取得
  FileRegion
  file_region() const override;

  /// クラスの型の取得
  PtStmtType
  type() const override;

  /// @brief ステートメントの種類を表す文字列の取得
  /// @return ステートメントの種類を表す文字列
  const char*
  stmt_name() const override;

  /// 階層ブランチの取得
  const PtNameBranchArray*
  namebranch_array() const override;

  /// 名前の取得
  const char*
  name() const override;

  /// @brief 引数のリストの取得
  const PtExprArray*
  arg_list() const override;

  /// コントロールの取得
  const PtControl*
  control() const override;

  /// 本体のステートメントの取得
  const PtStmt*
  body() const override;

  /// 式の取得
  const PtExpr*
  expr() const override;

  /// 左辺式の取得
  const PtExpr*
  lhs() const override;

  /// 右辺式の取得
  const PtExpr*
  rhs() const override;

  /// イベントプライマリの取得
  const PtExpr*
  primary() const override;

  /// 条件が成り立たなかったとき実行されるステートメントの取得
  const PtStmt*
  else_body() const override;

  /// @brief case item のリストの取得
  const PtCaseItemArray*
  caseitem_list() const override;

  /// 初期化代入文の取得
  const PtStmt*
  init_stmt() const override;

  /// 繰り返し代入文の取得
  const PtStmt*
  next_stmt() const override;

  /// @brief 宣言ヘッダ配列の取得
  /// @note kNamedParBlock/kNamedSeqBlock で意味のある関数
  const PtDeclHeadArray*
  declhead_array() const override;

  /// @brief 子供のステートメント配列の取得
  /// @note kParBlock/kSeqBlock で意味のある関数
  const PtStmtArray*
  stmt_array() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // クラスの型
  PtStmtType mType;

  // 階層ブランチの配列
  const PtNameBranchArray* mNbArray;

  // 名前
  const char* mName;

  // コントロール
  const PtControl* mControl;

  // ステートメント1
  const PtStmt* mBody1;

  // ステートメント2
  const PtStmt* mBody2;

  // ステートメント3
  const PtStmt* mBody3;

  // 式1
  const PtExpr* mExpr1;

  // 式2
  const PtExpr* mExpr2;

  // case item の配列
  const PtCaseItemArray* mCaseItemArray;

  // 宣言の配列
  const PtDeclHeadArray* mDeclArray;

  // ステートメントの配列
  const PtStmtArray* mStmtArray;

  // 式の配列
  const PtExprArray* mArgArray;

};


//////////////////////////////////////////////////////////////////////
// caseitemを表すクラス
//////////////////////////////////////////////////////////////////////
class SptCaseItem :
  public PtCaseItem
{
  friend class SptFactory;

private:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ/デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// コンストラクタ
  SptCaseItem(const FileRegion& file_region,
	      const PtExprArray* label_array,
	      const PtStmt* body);

  /// デストラクタ
  ~SptCaseItem();


public:
  //////////////////////////////////////////////////////////////////////
  // PtCaseItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// ファイル位置の取得
  FileRegion
  file_region() const override;

  /// @brief ラベルのリストの取得
  const PtExprArray*
  label_list() const override;

  /// 本体のステートメントの取得
  const PtStmt*
  body() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // ラベルのリスト
  const PtExprArray* mLabelArray;

  // ラベルが一致したときに実行されるステートメント
  const PtStmt* mBody;

};

END_NAMESPACE_YM_VERILOG

#endif // SPTSTMT_H
