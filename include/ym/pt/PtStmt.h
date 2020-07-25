#ifndef YM_PT_PTSTMT_H
#define YM_PT_PTSTMT_H

/// @file ym/pt/PtStmt.h
/// @brief PtStmt のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym/pt/PtBase.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class PtStmt PtStmt.h <ym/pt/PtStmt.h>
/// @ingroup VlParser
/// @ingroup PtGroup
/// @brief statement の共通の親クラス
//////////////////////////////////////////////////////////////////////
class PtStmt :
  public PtBase
{
public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型の取得
  /// @return 型
  virtual
  PtStmtType
  type() const = 0;

  /// @brief ステートメントの種類を表す文字列の取得
  /// @return ステートメントの種類を表す文字列
  virtual
  const char*
  stmt_name() const = 0;

  /// @brief 階層ブランチの先頭の取得
  /// @return 階層ブランチの先頭
  /// @note kDisable/kEnable/kSysEnable で意味のある関数
  virtual
  const PtNameBranchArray*
  namebranch_array() const = 0;

  /// @brief 名前の取得
  /// @return 名前
  /// @note kDisable/kEnable/kSysEnable/kParBlock/kSeqBlock で意味のある関数
  virtual
  const char*
  name() const = 0;

  /// @brief 引数のリストの取得
  virtual
  const PtExprArray*
  arg_list() const = 0;

  /// @brief コントロールの取得
  /// @return ディレイ/イベントコントロール
  /// @note kDc/kEc で意味のある関数
  virtual
  const PtControl*
  control() const = 0;

  /// @brief 本体のステートメントの取得
  /// @return 本体のステートメント
  /// @note kDc/kEc/kWait/kForever/kRepeat/kWhile/kFor/kIf で意味のある関数
  virtual
  const PtStmt*
  body() const = 0;

  /// @brief 式の取得
  /// @return 式
  /// @note kWait/kRepeat/kWhile/kFor/kIf/kCase/kCaseX/kCaseZ で意味のある関数
  virtual
  const PtExpr*
  expr() const = 0;

  /// @brief 左辺式の取得
  /// @return 左辺式
  /// @note kAssign/kForce/kPcAssign/kRelease/kDeassign で意味のある関数
  virtual
  const PtExpr*
  lhs() const = 0;

  /// @brief 右辺式の取得
  /// @return 右辺式
  /// @note kAssign/kForce/kPcAssign で意味のある関数
  virtual
  const PtExpr*
  rhs() const = 0;

  /// @brief イベントプライマリの取得
  /// @return イベントプライマリ
  /// @note kEvent で意味のある関数
  virtual
  const PtExpr*
  primary() const = 0;

  /// @brief 条件が成り立たなかったとき実行されるステートメントの取得
  /// @return 条件が成り立たなかったとき実行されるステートメント
  /// @note kIf で意味のある関数
  virtual
  const PtStmt*
  else_body() const = 0;

  /// @brief case item のリストの取得
  virtual
  const PtCaseItemArray*
  caseitem_list() const = 0;

  /// @brief 初期化代入文の取得
  /// @return 初期化代入文
  /// @note kFor で意味のある関数
  virtual
  const PtStmt*
  init_stmt() const = 0;

  /// @brief 繰り返し代入文の取得
  /// @return 繰り返し代入文
  /// @note kFor で意味のある関数
  virtual
  const PtStmt*
  next_stmt() const = 0;

  /// @brief 宣言ヘッダ配列の取得
  /// @note kNamedParBlock/kNamedSeqBlock で意味のある関数
  virtual
  const PtDeclHeadArray*
  declhead_array() const = 0;

  /// @brief 子供のステートメント配列の取得
  /// @note kParBlock/kSeqBlock で意味のある関数
  virtual
  const PtStmtArray*
  stmt_array() const = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class PtCaseItem PtStmt.h <ym/pt/PtStmt.h>
/// @ingroup VlParser
/// @ingroup PtGroup
/// @brief caseitemを表すクラス
//////////////////////////////////////////////////////////////////////
class PtCaseItem :
  public PtBase
{
public:
  //////////////////////////////////////////////////////////////////////
  // PtCaseItem の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ラベルのリストの取得
  virtual
  const PtExprArray*
  label_list() const = 0;

  /// @brief 本体のステートメントの取得
  /// @return 本体のステートメント
  virtual
  const PtStmt*
  body() const = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // YM_PT_PTSTMT_H
