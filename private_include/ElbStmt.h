#ifndef ELBSTMT_H
#define ELBSTMT_H

/// @file ElbStmt.h
/// @brief ElbStmt のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym/vl/VlStmt.h"
#include "ym/pt/PtP.h"
#include "ElbFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ElbStmt ElbStmt.h "ElbStmt.h"
/// @brief エラボレーション中の statement を表す基底クラス
/// IEEE Std 1364-2001 26.6.27 Process, block, statement, event statement
/// IEEE Std 1364-2001 26.6.28 Assignment
/// IEEE Std 1364-2001 26.6.29 Delay control
/// IEEE Std 1364-2001 26.6.30 Event control
/// IEEE Std 1364-2001 26.6.31 Repeat control
/// IEEE Std 1364-2001 26.6.32 While, repeat, wait
/// IEEE Std 1364-2001 26.6.33 For
/// IEEE Std 1364-2001 26.6.34 Forever
/// IEEE Std 1364-2001 26.6.35 If, if-else
/// IEEE Std 1364-2001 26.6.36 Case
/// IEEE Std 1364-2001 26.6.37 Assign statement, deassign, force, release
/// IEEE Std 1364-2001 26.6.38 Disable
//////////////////////////////////////////////////////////////////////
class ElbStmt :
  public VlStmt
{
protected:

  /// @brief コンストラクタ
  ElbStmt();

  /// @brief デストラクタ
  ~ElbStmt();


public:
  //////////////////////////////////////////////////////////////////////
  // VlStmt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 対象のスコープの取得
  /// @note この関数が意味を持つオブジェクトの型
  ///  - kVpiBegin
  ///  - kVpiDisable
  ///  - kVpiFork
  ///  - kVpiNamedBegin
  ///  - kVpiNamedFork
  /// @note このクラスでは nullptr を返す．
  const VlNamedObj*
  scope() const override;

  /// @brief task の実体を返す．
  /// @note この関数が意味を持つオブジェクトの型
  ///  - kVpiTaskCall
  /// @note このクラスでは nullptr を返す．
  const VlTaskFunc*
  task() const override;

  /// @brief user systf クラスへのポインタを返す．
  /// @note この関数が意味を持つオブジェクトの型
  ///  - kVpiSysTaskCall
  /// @note このクラスでは nullptr を返す．
  const VlUserSystf*
  user_systf() const override;

  /// @brief 引数の数の取得
  /// @note この関数が意味を持つオブジェクトの型
  ///  - kVpiSysTaskCall
  ///  - kVpiTaskCall
  /// @note このクラスでは 0 を返す．
  SizeType
  arg_num() const override;

  /// @brief 引数の取得
  /// @param[in] pos 位置 (0 <= pos < arg_num())
  /// @note この関数が意味を持つオブジェクトの型
  ///  - kVpiSysTaskCall
  ///  - kVpiTaskCall
  /// @note このクラスでは nullptr を返す．
  const VlExpr*
  arg(SizeType pos) const override;

  /// @brief control の取得
  /// @note この関数が意味を持つオブジェクトの型
  ///  - kVpiAssignment
  ///  - kVpiDelayControl
  ///  - kVpiEventControl
  /// @note このクラスでは nullptr を返す．
  const VlControl*
  control() const override;

  /// @brief 本体のステートメントの取得
  /// @note この関数が意味を持つオブジェクトの型
  ///  - kVpiDelayControl
  ///  - kVpiEventControl
  ///  - kVpiFor
  ///  - kVpiForever
  ///  - kVpiIf
  ///  - kVpiIfElse
  ///  - kVpiRepeat
  ///  - kVpiWait
  ///  - kVpiWhile
  /// @note このクラスでは nullptr を返す．
  const VlStmt*
  body_stmt() const override;

  /// @brief 式の取得
  /// @note この関数が意味を持つオブジェクトの型
  ///  - kVpiCase
  ///  - kVpiFor
  ///  - kVpiIf
  ///  - kVpiIfElse
  ///  - kVpiRepeat
  ///  - kVpiWait
  ///  - kVpiWhile
  /// @note このクラスでは nullptr を返す．
  const VlExpr*
  expr() const override;

  /// @brief 代入のブロッキング/ノンブロッキングの区別の取得
  /// @retval true ブロッキング代入文を表す．
  /// @retval false ノンブロッキング代入文もしくは他のステートメント
  /// @note この関数が意味を持つオブジェクトの型
  ///  - kVpiAssignment
  /// @note このクラスでは false を返す．
  bool
  is_blocking() const override;

  /// @brief 左辺式の取得
  /// @note この関数が意味を持つオブジェクトの型
  ///  - kVpiAssignStmt
  ///  - kVpiAssignment
  ///  - kVpiDeassign
  ///  - kVpiForce
  ///  - kVpiRelease
  /// @note このクラスでは nullptr を返す．
  const VlExpr*
  lhs() const override;

  /// @brief 右辺式の取得
  /// @note この関数が意味を持つオブジェクトの型
  ///  - kVpiAssignStmt
  ///  - kVpiAssignment
  ///  - kVpiForce
  /// @note このクラスでは nullptr を返す．
  const VlExpr*
  rhs() const override;

  /// @brief イベントプライマリの取得
  /// @note この関数が意味を持つオブジェクトの型
  ///  - kVpiEvent
  /// @note 返されるオブジェクトの型は kVpiNamedEvent のはず．
  /// @note このクラスでは nullptr を返す．
  const VlExpr*
  named_event() const override;

  /// @brief 条件が成り立たなかったとき実行されるステートメントの取得
  /// @note この関数が意味を持つオブジェクトの型
  ///  - kVpiIfElse
  /// @note このクラスでは nullptr を返す．
  const VlStmt*
  else_stmt() const override;

  /// @brief case type の取得
  /// @return case type
  /// @note この関数が意味を持つオブジェクトの型
  ///  - kVpiCase
  /// @note このクラスでは kVpiCaseExact を返す．
  VpiCaseType
  case_type() const override;

  /// @brief case item の要素数の取得
  /// @return case item の要素数
  /// @note この関数が意味を持つオブジェクトの型
  ///  - kVpiCase
  /// @note このクラスでは 0 を返す．
  SizeType
  caseitem_num() const override;

  /// @brief case item の取得
  /// @param[in] pos 位置番号 (0 <= pos < caseitem_num())
  /// @note この関数が意味を持つオブジェクトの型
  ///  - kVpiCase
  /// @note このクラスでは nullptr を返す．
  const VlCaseItem*
  caseitem(SizeType pos) const override;

  /// @brief 初期化代入文の取得
  /// @note この関数が意味を持つオブジェクトの型
  ///  - kVpiFor
  /// @note このクラスでは nullptr を返す．
  const VlStmt*
  init_stmt() const override;

  /// @brief 繰り返し代入文の取得
  /// @note この関数が意味を持つオブジェクトの型
  ///  - kVpiFor
  /// @note このクラスでは nullptr を返す．
  const VlStmt*
  inc_stmt() const override;

  /// @brief 子供のステートメントの数の取得
  /// @note この関数が意味を持つオブジェクトの型
  ///  - kVpiBegin
  ///  - kVpiFork
  ///  - kVpiNamedBegin
  ///  - kVpiNamedFork
  /// @note このクラスでは 0 を返す．
  SizeType
  child_stmt_num() const override;

  /// @brief 子供のステートメントの取得
  /// @param[in] pos 位置番号 (0 <= pos < stmt_num())
  /// @note この関数が意味を持つオブジェクトの型
  ///  - kVpiBegin
  ///  - kVpiFork
  ///  - kVpiNamedBegin
  ///  - kVpiNamedFork
  const VlStmt*
  child_stmt(SizeType pos) const override;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbStmt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief caseitem を設定する．
  /// @param[in] pos 位置番号
  /// @param[in] pt_caseitem パース木の caseitem 定義
  /// @param[in] expr_array 条件式を納める配列
  /// @param[in] stmt 本体のステートメント
  /// @note このクラスでは何もしない．
  virtual
  void
  set_caseitem(SizeType pos,
	       const PtCaseItem* pt_caseitem,
	       ElbExpr** expr_array,
	       ElbStmt* stmt);

  /// @brief 子供のステートメントの取得
  /// @note このクラスでは nullptr を返す．
  virtual
  ElbStmt*
  _child_stmt(SizeType pos) const;


public:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 次の要素を指すリンク
  ElbStmt* mNext;

};


//////////////////////////////////////////////////////////////////////
/// @class ElbCaseItem ElbStmt.h "ElbStmt.h"
/// @brief caes item を表すクラス
/// IEEE Std 1364-2001 26.6.36 Case
//////////////////////////////////////////////////////////////////////
class ElbCaseItem :
  public VlCaseItem
{
protected:

  /// @brief コンストラクタ
  ElbCaseItem() { }

  /// @brief デストラクタ
  ~ElbCaseItem() { }

};

END_NAMESPACE_YM_VERILOG

#endif // ELBSTMT_H
