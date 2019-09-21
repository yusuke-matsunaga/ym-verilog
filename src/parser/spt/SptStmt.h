﻿#ifndef SPTSTMT_H
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
	  tPtStmtType type,
	  const PtStmt* body1 = nullptr,
	  const PtStmt* body2 = nullptr,
	  const PtStmt* body3 = nullptr,
	  const PtExpr* expr1 = nullptr,
	  const PtExpr* expr2 = nullptr,
	  const PtControl* control = nullptr,
	  const char* name = nullptr,
	  PtNameBranchArray nb_array = PtNameBranchArray(),
	  PtCaseItemArray caseitem_array = PtCaseItemArray(),
	  PtDeclHeadArray decl_array = PtDeclHeadArray(),
	  PtStmtArray stmt_array = PtStmtArray(),
	  PtExprArray expr_array = PtExprArray());

  /// デストラクタ
  virtual
  ~SptStmt();


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// ファイル位置の取得
  virtual
  FileRegion
  file_region() const;

  /// クラスの型の取得
  virtual
  tPtStmtType
  type() const;

  /// @brief ステートメントの種類を表す文字列の取得
  /// @return ステートメントの種類を表す文字列
  virtual
  const char*
  stmt_name() const;

  /// 階層ブランチの取得
  virtual
  PtNameBranchArray
  namebranch_array() const;

  /// 名前の取得
  virtual
  const char*
  name() const;

  /// @brief 引数の数の取得
  /// @return 引数の数
  /// @note kEnable/kSysEnable で意味のある関数
  virtual
  int
  arg_num() const;

  /// @brief 引数の取得
  /// @param[in] pos 位置番号 ( 0 <= pos < arg_num() )
  /// @note kEnable/kSysEnable で意味のある関数
  virtual
  const PtExpr*
  arg(int pos) const;

  /// コントロールの取得
  virtual
  const PtControl*
  control() const;

  /// 本体のステートメントの取得
  virtual
  const PtStmt*
  body() const;

  /// 式の取得
  virtual
  const PtExpr*
  expr() const;

  /// 左辺式の取得
  virtual
  const PtExpr*
  lhs() const;

  /// 右辺式の取得
  virtual
  const PtExpr*
  rhs() const;

  /// イベントプライマリの取得
  virtual
  const PtExpr*
  primary() const;

  /// 条件が成り立たなかったとき実行されるステートメントの取得
  virtual
  const PtStmt*
  else_body() const;

  /// case item の要素数の取得
  virtual
  int
  caseitem_num() const;

  /// case item の取得
  virtual
  const PtCaseItem*
  caseitem(int pos) const;

  /// 初期化代入文の取得
  virtual
  const PtStmt*
  init_stmt() const;

  /// 繰り返し代入文の取得
  virtual
  const PtStmt*
  next_stmt() const;

  /// @brief 宣言ヘッダ配列の取得
  /// @note kNamedParBlock/kNamedSeqBlock で意味のある関数
  virtual
  PtDeclHeadArray
  declhead_array() const;

  /// @brief 子供のステートメント配列の取得
  /// @note kParBlock/kSeqBlock で意味のある関数
  virtual
  PtStmtArray
  stmt_array() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // クラスの型
  tPtStmtType mType;

  // 階層ブランチの配列
  PtNameBranchArray mNbArray;

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
  PtCaseItemArray mCaseItemArray;

  // 宣言の配列
  PtDeclHeadArray mDeclArray;

  // ステートメントの配列
  PtStmtArray mStmtArray;

  // 式の配列
  PtExprArray mArgArray;

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
	      PtExprArray label_array,
	      const PtStmt* body);

  /// デストラクタ
  ~SptCaseItem();


public:
  //////////////////////////////////////////////////////////////////////
  // PtCaseItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// ファイル位置の取得
  virtual
  FileRegion
  file_region() const;

  /// @brief ラベルの数の取得
  /// @retval ラベルの数 通常の case ラベルの場合
  /// @retval 0 default の場合
  virtual
  int
  label_num() const;

  /// @brief ラベルの取得
  /// @param[in] pos 位置番号 ( 0 <= pos < label_num() )
  virtual
  const PtExpr*
  label(int pos) const;

  /// 本体のステートメントの取得
  virtual
  const PtStmt*
  body() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // ラベルのリスト
  PtExprArray mLabelArray;

  // ラベルが一致したときに実行されるステートメント
  const PtStmt* mBody;

};

END_NAMESPACE_YM_VERILOG

#endif // SPTSTMT_H
