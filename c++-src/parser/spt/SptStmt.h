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
#include "parser/PtiArray.h"
#include "parser/PtiFwd.h"


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
	  PtiNameBranchArray&& nb_array = PtiNameBranchArray(),
	  PtiCaseItemArray&& caseitem_array = PtiCaseItemArray(),
	  PtiDeclHeadArray&& decl_array = PtiDeclHeadArray(),
	  PtiStmtArray&& stmt_array = PtiStmtArray(),
	  PtiExprArray&& expr_array = PtiExprArray());

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

  /// @brief 階層ブランチの要素数の取得
  SizeType
  namebranch_num() const override;

  /// @brief 階層ブランチの取得
  /// @param[in] pos 位置 ( 0 <= pos < namebranch_num() )
  const PtNameBranch*
  namebranch(SizeType pos) const override;

  /// 名前の取得
  const char*
  name() const override;

  /// @brief 引数の数の取得
  SizeType
  arg_num() const override;

  /// @brief 引数の取得
  /// @param[in] pos 位置 ( 0 <= pos < arg_num() )
  const PtExpr*
  arg(SizeType pos) const override;

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

  /// @brief case item のリストの要素数の取得
  SizeType
  caseitem_num() const override;

  /// @brief case item の取得
  /// @param[in] pos 位置 ( 0 <= pos < caseitem_num() )
  const PtCaseItem*
  caseitem(SizeType pos) const override;

  /// 初期化代入文の取得
  const PtStmt*
  init_stmt() const override;

  /// 繰り返し代入文の取得
  const PtStmt*
  next_stmt() const override;

  /// @brief 宣言ヘッダ配列の要素数の取得
  /// @note kNamedParBlock/kNamedSeqBlock で意味のある関数
  SizeType
  declhead_num() const override;

  /// @brief 宣言ヘッダの取得
  /// @param[in] pos 位置 ( 0 <= pos < declhead_num() )
  /// @note kNamedParBlock/kNamedSeqBlock で意味のある関数
  const PtDeclHead*
  declhead(SizeType pos) const override;

  /// @brief 子供のステートメント配列の要素数の取得
  /// @note kParBlock/kSeqBlock で意味のある関数
  SizeType
  stmt_num() const override;

  /// @brief 子供のステートメントの取得
  /// @param[in] pos 位置 ( 0 <= pos < stmt_num() )
  /// @note kParBlock/kSeqBlock で意味のある関数
  const PtStmt*
  stmt(SizeType pos) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // クラスの型
  PtStmtType mType;

  // 階層ブランチの配列
  PtiNameBranchArray mNbArray;

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
  PtiCaseItemArray mCaseItemArray;

  // 宣言の配列
  PtiDeclHeadArray mDeclArray;

  // ステートメントの配列
  PtiStmtArray mStmtArray;

  // 式の配列
  PtiExprArray mArgArray;

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
	      PtiExprArray&& label_array,
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

  /// @brief ラベルのリストの要素数の取得
  SizeType
  label_num() const override;

  /// @brief ラベルの取得
  /// @param[in] pos 位置 ( 0 <= pos < label_num() )
  const PtExpr*
  label(SizeType pos) const override;

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
  PtiExprArray mLabelArray;

  // ラベルが一致したときに実行されるステートメント
  const PtStmt* mBody;

};

END_NAMESPACE_YM_VERILOG

#endif // SPTSTMT_H
