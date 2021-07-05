#ifndef YM_PT_PTSTMT_H
#define YM_PT_PTSTMT_H

/// @file ym/pt/PtStmt.h
/// @brief PtStmt のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014, 2021 Yusuke Matsunaga
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
  public PtHierNamedBase
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

  /// @brief 引数の数の取得
  virtual
  SizeType
  arg_num() const = 0;

  /// @brief 引数の取得
  virtual
  const PtExpr*
  arg(
    SizeType pos ///< [in] 位置 ( 0 <= pos < arg_num() )
  ) const = 0;

  /// @brief 引数のリストの取得
  vector<const PtExpr*>
  arg_list() const
  {
    SizeType n = arg_num();
    vector<const PtExpr*> vec(n);
    for ( SizeType i = 0; i < n; ++ i ) {
      vec[i] = arg(i);
    }
    return vec;
  }

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

  /// @brief case item のリストの要素数の取得
  virtual
  SizeType
  caseitem_num() const = 0;

  /// @brief case item の取得
  virtual
  const PtCaseItem*
  caseitem(
    SizeType pos ///< [in] 位置 ( 0 <= pos < caseitem_num() )
  ) const = 0;

  /// @brief case item のリストの取得
  vector<const PtCaseItem*>
  caseitem_list() const
  {
    SizeType n = caseitem_num();
    vector<const PtCaseItem*> vec(n);
    for ( SizeType i = 0; i < n; ++ i ) {
      vec[i] = caseitem(i);
    }
    return vec;
  }

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

  /// @brief 宣言ヘッダ配列の要素数の取得
  /// @note kNamedParBlock/kNamedSeqBlock で意味のある関数
  virtual
  SizeType
  declhead_num() const = 0;

  /// @brief 宣言ヘッダの取得
  /// @note kNamedParBlock/kNamedSeqBlock で意味のある関数
  virtual
  const PtDeclHead*
  declhead(
    SizeType pos ///< [in] 位置 ( 0 <= pos < declhead_num() )
  ) const = 0;

  /// @brief 宣言ヘッダのリストの取得
  vector<const PtDeclHead*>
  declhead_list() const
  {
    SizeType n = declhead_num();
    vector<const PtDeclHead*> vec(n);
    for ( SizeType i = 0; i < n; ++ i ) {
      vec[i] = declhead(i);
    }
    return vec;
  }

  /// @brief 子供のステートメント配列の要素数の取得
  /// @note kParBlock/kSeqBlock で意味のある関数
  virtual
  SizeType
  stmt_num() const = 0;

  /// @brief 子供のステートメントの取得
  /// @note kParBlock/kSeqBlock で意味のある関数
  virtual
  const PtStmt*
  stmt(
    SizeType pos ///< [in] 位置 ( 0 <= pos < stmt_num() )
  ) const = 0;

  /// @brief 子供のステートメントのリストの取得
  vector<const PtStmt*>
  stmt_list() const
  {
    SizeType n = stmt_num();
    vector<const PtStmt*> vec(n);
    for ( SizeType i = 0; i < n; ++ i ) {
      vec[i] = stmt(i);
    }
    return vec;
  }

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

  /// @brief ラベルのリストの要素数の取得
  virtual
  SizeType
  label_num() const = 0;

  /// @brief ラベルの取得
  virtual
  const PtExpr*
  label(
    SizeType pos ///< [in] 位置 ( 0 <= pos < label_num() )
  ) const = 0;

  /// @brief ラベルリストの取得
  vector<const PtExpr*>
  label_list() const
  {
    SizeType n = label_num();
    vector<const PtExpr*> vec(n);
    for ( SizeType i = 0; i < n; ++ i ) {
      vec[i] = label(i);
    }
    return vec;
  }

  /// @brief 本体のステートメントの取得
  /// @return 本体のステートメント
  virtual
  const PtStmt*
  body() const = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // YM_PT_PTSTMT_H
