#ifndef YM_PT_PTITEM_H
#define YM_PT_PTITEM_H

/// @file ym/pt/PtItem.h
/// @brief PtItem のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym/pt/PtBase.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class PtItem PtItem.h <ym/pt/PtItem.h>
/// @ingroup VlParser
/// @ingroup PtGroup
/// @brief ブロック内要素の共通の親クラス
//////////////////////////////////////////////////////////////////////
class PtItem :
  public PtNamedBase
{
public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  /// @return 型
  virtual
  PtItemType
  type() const = 0;

  /// @brief プリミティブタイプの取得
  /// @return プリミティブタイプ
  virtual
  VpiPrimType
  prim_type() const = 0;

  /// @brief strength の取得
  /// @return 信号強度
  virtual
  const PtStrength*
  strength() const = 0;

  /// @brief delay の取得
  /// @return 遅延
  virtual
  const PtDelay*
  delay() const = 0;

  /// @brief パラメータ割り当て数の取得
  virtual
  SizeType
  paramassign_num() const = 0;

  /// @brief パラメータ割り当ての取得
  virtual
  const PtConnection*
  paramassign(SizeType pos) ///< [in] 位置 ( 0 <= pos < paramassign_num() )
    const = 0;

  /// @brief パラメータ割り当てのリストの取得
  vector<const PtConnection*>
  paramassign_list() const;

  /// @brief defparam の要素数の取得
  virtual
  SizeType
  defparam_num() const = 0;

  /// @brief defparam の取得
  virtual
  const PtDefParam*
  defparam(SizeType pos) ///< [in] 位置 ( 0 <= pos < defparam_num() )
    const = 0;

  /// @brief defparam リストの取得
  vector<const PtDefParam*>
  defparam_list() const;

  /// @brief continuous assign の要素数の取得
  virtual
  SizeType
  contassign_num() const = 0;

  /// @brief continuous assign の取得
  virtual
  const PtContAssign*
  contassign(SizeType pos) ///< [in] 位置 ( 0 <= pos < contassign_num() )
    const = 0;

  /// @brief continuous assign リストの取得
  vector<const PtContAssign*>
  contassign_list() const;

  /// @brief module/UDP/gate instance の要素数の取得
  virtual
  SizeType
  inst_num() const = 0;

  /// @brief module/UDP/gate instance の取得
  virtual
  const PtInst*
  inst(SizeType pos) ///< [in] 位置 ( 0 <= pos < inst_num() )
    const = 0;

  /// @brief module/UDP/gate instance リストの取得
  vector<const PtInst*>
  inst_list() const;

  /// @brief automatic 情報の取得
  /// @retval true automatic 宣言された task/function
  /// @retval false 上記以外
  virtual
  bool
  automatic() const = 0;

  /// @brief IO宣言の要素数の取得
  virtual
  SizeType
  ioitem_num() const = 0;

  /// @brief IO宣言ヘッダリストの要素数の取得
  virtual
  SizeType
  iohead_num() const = 0;

  /// @brief IO宣言ヘッダの取得
  virtual
  const PtIOHead*
  iohead(SizeType pos) ///< [in] 位置 ( 0 <= pos < iohead_num() )
    const = 0;

  /// @brief IO宣言ヘッダリストの取得
  vector<const PtIOHead*>
  iohead_list() const;

  /// @brief 宣言ヘッダの要素数の取得
  virtual
  SizeType
  declhead_num() const = 0;

  /// @brief 宣言ヘッダの取得
  virtual
  const PtDeclHead*
  declhead(SizeType pos) ///< [in] 位置 ( 0 <= pos < declhead_num() )
    const = 0;

  /// @brief 宣言ヘッダリストの取得
  vector<const PtDeclHead*>
  declhead_list() const;

  /// @brief item リストの要素数の取得
  virtual
  SizeType
  item_num() const = 0;

  /// @brief item の取得
  virtual
  const PtItem*
  item(SizeType pos) ///< [in] 位置 ( 0 <= pos < item_num() )
    const = 0;

  /// @brief item リストの取得
  vector<const PtItem*>
  item_list() const;

  /// @brief 本体のステートメントの取得
  /// @return 本体のステートメント
  virtual
  const PtStmt*
  body() const = 0;

  /// @brief 符号の取得
  /// @retval true 符号つき
  /// @retval false 符号なし
  virtual
  bool
  is_signed() const = 0;

  /// @brief 範囲の左側の式の取得
  /// @return 範囲の左側の式
  virtual
  const PtExpr*
  left_range() const = 0;

  /// @brief 範囲の右側の式の取得
  /// @return 範囲の右側の式
  virtual
  const PtExpr*
  right_range() const = 0;

  /// @brief 戻値のデータ型の取得
  /// @return 戻値のデータ型
  virtual
  VpiVarType
  data_type() const = 0;

  /// @brief constant function の展開中の印をつける．
  virtual
  void
  set_in_use() const = 0;

  /// @brief constant function の展開中の印を消す．
  virtual
  void
  clear_in_use() const = 0;

  /// @brief 使用中(constant function として展開中)のチェック
  /// @return 使用中の時 true を返す．
  virtual
  bool
  is_in_use() const = 0;

  /// @brief specify block item の種類の取得
  /// @return specify block item の種類
  virtual
  VpiSpecItemType
  specitem_type() const = 0;

  /// @brief specify block path の種類の取得
  /// @return specify block path の種類
  virtual
  VpiSpecPathType
  specpath_type() const = 0;

  /// @brief ターミナルの要素数の取得
  virtual
  SizeType
  terminal_num() const = 0;

  /// @brief ターミナルの取得
  virtual
  const PtExpr*
  terminal(SizeType pos) ///< [in] 位置 ( 0 <= pos < terminal_num() )
    const = 0;

  /// @brief ターミナルリストの取得
  vector<const PtExpr*>
  terminal_list() const;

  /// @brief パス記述の取得
  /// @return パス記述
  virtual
  const PtPathDecl*
  path_decl() const = 0;

  /// @brief 条件式の取得
  /// @return 条件式
  virtual
  const PtExpr*
  expr() const = 0;

  /// @brief 条件が成り立ったときに生成される宣言ヘッダ配列の要素数の取得
  virtual
  SizeType
  then_declhead_num() const = 0;

  /// @brief 条件が成り立ったときに生成される宣言ヘッダの取得
  virtual
  const PtDeclHead*
  then_declhead(SizeType pos) ///< [in] 位置 ( 0 <= pos < then_declhead_num() )
    const = 0;

  /// @brief 条件が成り立った時に生成される宣言ヘッダリストの取得
  vector<const PtDeclHead*>
  then_declhead_list() const;

  /// @brief 条件が成り立ったときに生成される item 配列の要素数の取得
  virtual
  SizeType
  then_item_num() const = 0;

  /// @brief 条件が成り立ったときに生成される item の取得
  virtual
  const PtItem*
  then_item(SizeType pos) ///< [in] 位置 ( 0 <= pos < then_item_num() )
    const = 0;

  /// @brief 条件が成り立った時に生成されるitemリストの取得
  vector<const PtItem*>
  then_item_list() const;

  /// @brief 条件が成り立たなかったときに生成される宣言ヘッダ配列の要素数の取得
  virtual
  SizeType
  else_declhead_num() const = 0;

  /// @brief 条件が成り立たなかったときに生成される宣言ヘッダの取得
  virtual
  const PtDeclHead*
  else_declhead(SizeType pos) ///< [in] 位置 ( 0 <= pos < else_declhead_num() )
    const = 0;

  /// @brief 条件が成り立たなかった時に生成される宣言ヘッダリストの取得
  vector<const PtDeclHead*>
  else_declhead_list() const;

  /// @brief 条件が成り立たなかったときに生成される item 配列の要素数の取得
  virtual
  SizeType
  else_item_num() const = 0;

  /// @brief 条件が成り立たなかったときに生成される item の取得
  virtual
  const PtItem*
  else_item(SizeType pos) ///< [in] 位置 ( 0 <= pos < else_item_num() )
    const = 0;

  /// @brief 条件が成り立たなかった時に生成されるitemリストの取得
  vector<const PtItem*>
  else_item_list() const;

  /// @brief case item のリストの要素数の取得
  virtual
  SizeType
  caseitem_num() const = 0;

  /// @brief case item の取得
  virtual
  const PtGenCaseItem*
  caseitem(SizeType pos) ///< [in] 位置 ( 0 <= pos < caseitem_num() )
    const = 0;

  /// @brief case item リストの取得
  vector<const PtGenCaseItem*>
  caseitem_list() const;

  /// @brief 繰り返し制御用の変数名の取得
  /// @return 繰り返し制御用の変数名
  virtual
  const char*
  loop_var() const = 0;

  /// @brief 初期化文の右辺の取得
  /// @return 初期化文の右辺
  virtual
  const PtExpr*
  init_expr() const = 0;

  /// @brief 増加文の右辺の取得
  /// @return 増加文の右辺
  virtual
  const PtExpr*
  next_expr() const = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class PtDefParam PtItem.h <ym/pt/PtItem.h>
/// @ingroup VlParser
/// @ingroup PtGroup
/// @brief defparam 文を表すクラス
//////////////////////////////////////////////////////////////////////
class PtDefParam :
  public PtHierNamedBase
{
public:
  //////////////////////////////////////////////////////////////////////
  // PtDefParam の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値の取得
  /// @return 値を表す式
  virtual
  const PtExpr*
  expr() const = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class PtContAssign PtItem.h <ym/pt/PtItem.h>
/// @ingroup VlParser
/// @ingroup PtGroup
/// @brief continuous assign 文
//////////////////////////////////////////////////////////////////////
class PtContAssign :
  public PtBase
{
public:
  //////////////////////////////////////////////////////////////////////
  // PtContAssign の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 左辺式の取得
  /// @return 左辺式
  virtual
  const PtExpr*
  lhs() const = 0;

  /// @brief 右辺式の取得
  /// @return 右辺式
  virtual
  const PtExpr*
  rhs() const = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class PtInst PtItem.h <ym/pt/PtItem.h>
/// @ingroup VlParser
/// @ingroup PtGroup
/// @brief module instance/UDP/gate instance を表すクラス
//////////////////////////////////////////////////////////////////////
class PtInst :
  public PtNamedBase
{
public:
  //////////////////////////////////////////////////////////////////////
  // PtInst の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 範囲の左側の式の取得
  /// @return 範囲の左側の式
  virtual
  const PtExpr*
  left_range() const = 0;

  /// @brief 範囲の右側の式の取得
  /// @return 範囲の右側の式
  virtual
  const PtExpr*
  right_range() const = 0;

  /// @brief ポートの要素数の取得
  virtual
  SizeType
  port_num() const = 0;

  /// @brief ポートの取得
  virtual
  const PtConnection*
  port(SizeType pos) ///< [in] 位置 ( 0 <= pos < port_num() )
    const = 0;

  /// @brief ポートリストの取得
  vector<const PtConnection*>
  port_list() const;

};


//////////////////////////////////////////////////////////////////////
/// @class PtGenCaseItem PtItem.h <ym/pt/PtItem.h>
/// @ingroup VlParser
/// @ingroup PtGroup
/// @brief gencaseitemを表すクラス
//////////////////////////////////////////////////////////////////////
class PtGenCaseItem :
  public PtBase
{
public:
  //////////////////////////////////////////////////////////////////////
  // PtGenCaseItem の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ラベルの要素数の取得
  virtual
  SizeType
  label_num() const = 0;

  /// @brief ラベルの取得
  virtual
  const PtExpr*
  label(SizeType pos) ///< [in] 位置 ( 0 <= pos < label_num() )
    const = 0;

  /// @brief ラベルリストの取得
  vector<const PtExpr*>
  label_list() const;

  /// @brief 宣言の要素数の取得
  virtual
  SizeType
  declhead_num() const = 0;

  /// @brief 宣言の取得
  virtual
  const PtDeclHead*
  declhead(SizeType pos) const = 0;

  /// @brief 宣言リストの取得
  vector<const PtDeclHead*>
  declhead_list() const;

  /// @brief item の要素数の取得
  virtual
  SizeType
  item_num() const = 0;

  /// @brief item の取得
  virtual
  const PtItem*
  item(SizeType pos) const = 0;

  /// @brief item リストの取得
  vector<const PtItem*>
  item_list() const;

};


//////////////////////////////////////////////////////////////////////
/// @class PtPathDecl PtItem.h <ym/pt/PtItem.h>
/// @ingroup VlParser
/// @ingroup PtGroup
/// @brief path_delay_declaration を表すクラス
//////////////////////////////////////////////////////////////////////
class PtPathDecl :
  public PtBase
{
public:
  //////////////////////////////////////////////////////////////////////
  // PtPathDecl の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief edge_descriptor の取得
  /// @return edge_descriptor\n
  /// 0の場合もある．
  virtual
  int
  edge() const = 0;

  /// @brief 入力のリストの要素数の取得
  virtual
  SizeType
  input_num() const = 0;

  /// @brief 入力の取得
  virtual
  const PtExpr*
  input(SizeType pos) ///< [in] 位置 ( 0 <= pos < input_num() )
    const = 0;

  /// @brief 入力のリストの取得
  vector<const PtExpr*>
  input_list() const;

  /// @brief 入力の極性の取得
  /// @return 入力の極性\n
  /// 0の場合もありうる．
  virtual
  int
  input_pol() const = 0;

  /// @brief パス記述子(?)の取得
  /// @return vpiParallel か vpiFull
  virtual
  VpiPathType
  op() const = 0;

  /// @brief 出力のリストの要素数の取得
  virtual
  SizeType
  output_num() const = 0;

  /// @brief 出力の取得
  virtual
  const PtExpr*
  output(SizeType pos) ///< [in] 位置 ( 0 <= pos < output_num() )
    const = 0;

  /// @brief 出力リストの取得
  vector<const PtExpr*>
  output_list() const;

  /// @brief 出力の極性の取得
  /// @return 出力の極性\n
  /// 0の場合もありうる．
  virtual
  int
  output_pol() const = 0;

  /// @brief 式の取得
  /// @return 式\n
  /// nullptr の場合もありうる．
  virtual
  const PtExpr*
  expr() const = 0;

  /// @brief path_delay_value の取得
  /// @return path_delay_value
  virtual
  const PtPathDelay*
  path_delay() const = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class PtPathDelay PtItem.h <ym/pt/PtItem.h>
/// @ingroup VlParser
/// @ingroup PtGroup
/// @brief path_delay_value を表すクラス
//////////////////////////////////////////////////////////////////////
class PtPathDelay :
  public PtBase
{
public:
  //////////////////////////////////////////////////////////////////////
  // PtPathDecl の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値を取り出す．
  /// 0の場合もある．
  virtual
  const PtExpr*
  value(SizeType pos) const = 0;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief パラメータ割り当てのリストの取得
inline
vector<const PtConnection*>
PtItem::paramassign_list() const
{
  SizeType n = paramassign_num();
  vector<const PtConnection*> vec(n);
  for ( SizeType i = 0; i < n; ++ i ) {
    vec[i] = paramassign(i);
  }
  return vec;
}

// @brief defparam リストの取得
inline
vector<const PtDefParam*>
PtItem::defparam_list() const
{
  SizeType n = defparam_num();
  vector<const PtDefParam*> vec(n);
  for ( SizeType i = 0; i < n; ++ i ) {
    vec[i] = defparam(i);
  }
  return vec;
}

// @brief continuous assign リストの取得
inline
vector<const PtContAssign*>
PtItem::contassign_list() const
{
  SizeType n = contassign_num();
  vector<const PtContAssign*> vec(n);
  for ( SizeType i = 0; i < n; ++ i ) {
    vec[i] = contassign(i);
  }
  return vec;
}

// @brief module/UDP/gate instance リストの取得
inline
vector<const PtInst*>
PtItem::inst_list() const
{
  SizeType n = inst_num();
  vector<const PtInst*> vec(n);
  for ( SizeType i = 0; i < n; ++ i ) {
    vec[i] = inst(i);
  }
  return vec;
}

// @brief IO宣言ヘッダリストの取得
inline
vector<const PtIOHead*>
PtItem::iohead_list() const
{
  SizeType n = iohead_num();
  vector<const PtIOHead*> vec(n);
  for ( SizeType i = 0; i < n; ++ i ) {
    vec[i] = iohead(i);
  }
  return vec;
}

// @brief 宣言ヘッダリストの取得
inline
vector<const PtDeclHead*>
PtItem::declhead_list() const
{
  SizeType n = declhead_num();
  vector<const PtDeclHead*> vec(n);
  for ( SizeType i = 0; i < n; ++ i ) {
    vec[i] = declhead(i);
  }
  return vec;
}

// @brief item リストの取得
inline
vector<const PtItem*>
PtItem::item_list() const
{
  SizeType n = item_num();
  vector<const PtItem*> vec(n);
  for ( SizeType i = 0; i < n; ++ i ) {
    vec[i] = item(i);
  }
  return vec;
}

// @brief ターミナルリストの取得
inline
vector<const PtExpr*>
PtItem::terminal_list() const
{
  SizeType n = terminal_num();
  vector<const PtExpr*> vec(n);
  for ( SizeType i = 0; i < n; ++ i ) {
    vec[i] = terminal(i);
  }
  return vec;
}

// @brief 条件が成り立った時に生成される宣言ヘッダリストの取得
inline
vector<const PtDeclHead*>
PtItem::then_declhead_list() const
{
  SizeType n = then_declhead_num();
  vector<const PtDeclHead*> vec(n);
  for ( SizeType i = 0; i < n; ++ i ) {
    vec[i] = then_declhead(i);
  }
  return vec;
}

// @brief 条件が成り立った時に生成されるitemリストの取得
inline
vector<const PtItem*>
PtItem::then_item_list() const
{
  SizeType n = then_item_num();
  vector<const PtItem*> vec(n);
  for ( SizeType i = 0; i < n; ++ i ) {
    vec[i] = then_item(i);
  }
  return vec;
}

// @brief 条件が成り立たなかった時に生成される宣言ヘッダリストの取得
inline
vector<const PtDeclHead*>
PtItem::else_declhead_list() const
{
  SizeType n = else_declhead_num();
  vector<const PtDeclHead*> vec(n);
  for ( SizeType i = 0; i < n; ++ i ) {
    vec[i] = else_declhead(i);
  }
  return vec;
}

// @brief 条件が成り立たなかった時に生成されるitemリストの取得
inline
vector<const PtItem*>
PtItem::else_item_list() const
{
  SizeType n = then_item_num();
  vector<const PtItem*> vec(n);
  for ( SizeType i = 0; i < n; ++ i ) {
    vec[i] = else_item(i);
  }
  return vec;
}

// @brief case item リストの取得
inline
vector<const PtGenCaseItem*>
PtItem::caseitem_list() const
{
  SizeType n = caseitem_num();
  vector<const PtGenCaseItem*> vec(n);
  for ( SizeType i = 0; i < n; ++ i ) {
    vec[i] = caseitem(i);
  }
  return vec;
}

// @brief ポートリストの取得
inline
vector<const PtConnection*>
PtInst::port_list() const
{
  SizeType n = port_num();
  vector<const PtConnection*> vec(n);
  for ( SizeType i = 0; i < n; ++ i ) {
    vec[i] = port(i);
  }
  return vec;
}

// @brief ラベルリストの取得
inline
vector<const PtExpr*>
PtGenCaseItem::label_list() const
{
  SizeType n = label_num();
  vector<const PtExpr*> vec(n);
  for ( SizeType i = 0; i < n; ++ i ) {
    vec[i] = label(i);
  }
  return vec;
}

// @brief 宣言リストの取得
inline
vector<const PtDeclHead*>
PtGenCaseItem::declhead_list() const
{
  SizeType n = declhead_num();
  vector<const PtDeclHead*> vec(n);
  for ( SizeType i = 0; i < n; ++ i ) {
    vec[i] = declhead(i);
  }
  return vec;
}

// @brief item リストの取得
inline
vector<const PtItem*>
PtGenCaseItem::item_list() const
{
  SizeType n = item_num();
  vector<const PtItem*> vec(n);
  for ( SizeType i = 0; i < n; ++ i ) {
    vec[i] = item(i);
  }
  return vec;
}

// @brief 入力のリストの取得
inline
vector<const PtExpr*>
PtPathDecl::input_list() const
{
  SizeType n = input_num();
  vector<const PtExpr*> vec(n);
  for ( SizeType i = 0; i < n; ++ i ) {
    vec[i] = input(i);
  }
  return vec;
}

// @brief 出力リストの取得
inline
vector<const PtExpr*>
PtPathDecl::output_list() const
{
  SizeType n = output_num();
  vector<const PtExpr*> vec(n);
  for ( SizeType i = 0; i < n; ++ i ) {
    vec[i] = output(i);
  }
  return vec;
}

END_NAMESPACE_YM_VERILOG

#endif // YM_PT_PTITEM_H
