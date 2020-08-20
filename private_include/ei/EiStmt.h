#ifndef EISTMT_H
#define EISTMT_H

/// @file EiStmt.h
/// @brief EiStmt のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.

// ステートメントを表すクラス
// IEEE Std 1364-2001 26.6.28 Assignment
// IEEE Std 1364-2001 26.6.29 Delay control
// IEEE Std 1364-2001 26.6.30 Event control
// IEEE Std 1364-2001 26.6.37 Assign statement, deassign, force, release


#include "ym/vl/VlStmt.h"
#include "ym/pt/PtP.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiStmt EiStmt.h "EiStmt.h"
/// @brief VlStmt の実装クラス
///
/// 仮想関数のデフォルト実装を行っている．
//////////////////////////////////////////////////////////////////////
class EiStmt :
  public VlStmt
{
protected:

  /// @brief コンストラクタ
  /// @param[in] process 親のプロセス (or nullptr)
  EiStmt(const VlProcess* process);

  /// デストラクタ
  ~EiStmt() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // VlStmt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 対象のスコープの取得
  ///
  /// この関数が意味を持つオブジェクトの型
  ///  - kVpiNamedBegin
  ///  - kVpiNamedFork
  ///
  /// このクラスでは nullptr を返す．
  const VlScope*
  scope() const override;

  /// @brief task の実体を返す．
  ///
  /// この関数が意味を持つオブジェクトの型
  ///  - kVpiTaskCall
  ///
  /// このクラスでは nullptr を返す．
  const VlTaskFunc*
  task() const override;

  /// @brief user systf クラスへのポインタを返す．
  ///
  /// この関数が意味を持つオブジェクトの型
  ///  - kVpiSysTaskCall
  ///
  /// このクラスでは nullptr を返す．
  const VlUserSystf*
  user_systf() const override;

  /// @brief 引数の数の取得
  ///
  /// この関数が意味を持つオブジェクトの型
  ///  - kVpiSysTaskCall
  ///  - kVpiTaskCall
  ///
  /// このクラスでは 0 を返す．
  SizeType
  arg_num() const override;

  /// @brief 引数の取得
  /// @param[in] pos 位置 (0 <= pos < arg_num())
  ///
  /// この関数が意味を持つオブジェクトの型
  ///  - kVpiSysTaskCall
  ///  - kVpiTaskCall
  ///
  /// このクラスでは nullptr を返す．
  const VlExpr*
  arg(SizeType pos) const override;

  /// @brief control の取得
  ///
  /// この関数が意味を持つオブジェクトの型
  ///  - kVpiAssignment
  ///  - kVpiDelayControl
  ///  - kVpiEventControl
  ///
  /// このクラスでは nullptr を返す．
  const VlControl*
  control() const override;

  /// @brief 本体のステートメントの取得
  ///
  /// この関数が意味を持つオブジェクトの型
  ///  - kVpiDelayControl
  ///  - kVpiEventControl
  ///  - kVpiFor
  ///  - kVpiForever
  ///  - kVpiIf
  ///  - kVpiIfElse
  ///  - kVpiRepeat
  ///  - kVpiWait
  ///  - kVpiWhile
  ///
  /// このクラスでは nullptr を返す．
  const VlStmt*
  body_stmt() const override;

  /// @brief 式の取得
  ///
  /// この関数が意味を持つオブジェクトの型
  ///  - kVpiCase
  ///  - kVpiFor
  ///  - kVpiIf
  ///  - kVpiIfElse
  ///  - kVpiRepeat
  ///  - kVpiWait
  ///  - kVpiWhile
  ///
  /// このクラスでは nullptr を返す．
  const VlExpr*
  expr() const override;

  /// @brief 代入のブロッキング/ノンブロッキングの区別の取得
  /// @retval true ブロッキング代入文を表す．
  /// @retval false ノンブロッキング代入文もしくは他のステートメント
  ///
  /// この関数が意味を持つオブジェクトの型
  ///  - kVpiAssignment
  ///
  /// このクラスでは false を返す．
  bool
  is_blocking() const override;

  /// @brief 左辺式の取得
  ///
  /// この関数が意味を持つオブジェクトの型
  ///  - kVpiAssignStmt
  ///  - kVpiAssignment
  ///  - kVpiDeassign
  ///  - kVpiForce
  ///  - kVpiRelease
  ///
  /// このクラスでは nullptr を返す．
  const VlExpr*
  lhs() const override;

  /// @brief 右辺式の取得
  ///
  /// この関数が意味を持つオブジェクトの型
  ///  - kVpiAssignStmt
  ///  - kVpiAssignment
  ///  - kVpiForce
  ///
  /// このクラスでは nullptr を返す．
  const VlExpr*
  rhs() const override;

  /// @brief イベントプライマリの取得
  ///
  /// この関数が意味を持つオブジェクトの型
  ///  - kVpiEvent
  /// 返されるオブジェクトの型は kVpiNamedEvent のはず．
  ///
  /// このクラスでは nullptr を返す．
  const VlExpr*
  named_event() const override;

  /// @brief 条件が成り立たなかったとき実行されるステートメントの取得
  ///
  /// この関数が意味を持つオブジェクトの型
  ///  - kVpiIfElse
  ///
  /// このクラスでは nullptr を返す．
  const VlStmt*
  else_stmt() const override;

  /// @brief case type の取得
  /// @return case type
  ///
  /// この関数が意味を持つオブジェクトの型
  ///  - kVpiCase
  ///
  /// このクラスでは kVpiCaseExact を返す．
  VpiCaseType
  case_type() const override;

  /// @brief case item の要素数の取得
  /// @return case item の要素数
  ///
  /// この関数が意味を持つオブジェクトの型
  ///  - kVpiCase
  ///
  /// このクラスでは 0 を返す．
  SizeType
  caseitem_num() const override;

  /// @brief case item の取得
  /// @param[in] pos 位置番号 (0 <= pos < caseitem_num())
  ///
  /// この関数が意味を持つオブジェクトの型
  ///  - kVpiCase
  ///
  /// このクラスでは nullptr を返す．
  const VlCaseItem*
  caseitem(SizeType pos) const override;

  /// @brief 初期化代入文の取得
  ///
  /// この関数が意味を持つオブジェクトの型
  ///  - kVpiFor
  ///
  /// このクラスでは nullptr を返す．
  const VlStmt*
  init_stmt() const override;

  /// @brief 繰り返し代入文の取得
  ///
  /// この関数が意味を持つオブジェクトの型
  ///  - kVpiFor
  ///
  /// このクラスでは nullptr を返す．
  const VlStmt*
  inc_stmt() const override;

  /// @brief 子供のステートメントの数の取得
  ///
  /// この関数が意味を持つオブジェクトの型
  ///  - kVpiBegin
  ///  - kVpiFork
  ///  - kVpiNamedBegin
  ///  - kVpiNamedFork
  ///
  /// このクラスでは 0 を返す．
  SizeType
  child_stmt_num() const override;

  /// @brief 子供のステートメントの取得
  /// @param[in] pos 位置番号 (0 <= pos < stmt_num())
  ///
  /// この関数が意味を持つオブジェクトの型
  /// このクラスではなにもしない．
  ///  - kVpiBegin
  ///  - kVpiFork
  ///  - kVpiNamedBegin
  ///  - kVpiNamedFork
  const VlStmt*
  child_stmt(SizeType pos) const override;

  /// @brief disable 対象のスコープを得る．
  const VlScope*
  target_scope() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親のプロセス
  const VlProcess* mProcess;

};


//////////////////////////////////////////////////////////////////////
/// @class EiStmtBase EiStmt.h "EiStmt.h"
/// @brief VlStmt の実装クラス
///
/// EiStmt に親のスコープとパース木の要素をもたせたもの
//////////////////////////////////////////////////////////////////////
class EiStmtBase :
  public EiStmt
{
protected:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  EiStmtBase(const VlScope* parent,
	     const VlProcess* process,
	     const PtStmt* pt_stmt);

  /// デストラクタ
  ~EiStmtBase() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // VlStmt(VlObj) の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  FileRegion
  file_region() const override;

  /// @brief 親のスコープを返す．
  const VlScope*
  parent_scope() const override;


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから参照可能な関数
  //////////////////////////////////////////////////////////////////////

  /// @brief パース木の定義要素を得る．
  const PtStmt*
  pt_stmt() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親のスコープ
  const VlScope* mParent;

  // パース木のステートメント定義
  const PtStmt* mPtStmt;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief パース木の定義要素を得る．
inline
const PtStmt*
EiStmtBase::pt_stmt() const
{
  return mPtStmt;
}

END_NAMESPACE_YM_VERILOG

#endif // EISTMT_H
