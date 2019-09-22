﻿#ifndef YM_PT_PTITEM_H
#define YM_PT_PTITEM_H

/// @file ym/pt/PtItem.h
/// @brief PtItem のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
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

  /// @brief 仮想デストラクタ
  virtual
  ~PtItem() { }


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  /// @return 型
  virtual
  tPtItemType
  type() const = 0;

  /// @brief プリミティブタイプの取得
  /// @return プリミティブタイプ
  virtual
  tVpiPrimType
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

  /// @brief パラメータ割り当てリストの取得
  virtual
  PtConnectionArray
  paramassign_array() const = 0;

  /// @brief defparam のリストの取得
  virtual
  PtDefParamArray
  defparam_list() const = 0;

  /// @brief continuous assign のリストの取得
  virtual
  PtContAssignArray
  contassign_list() const = 0;

  /// @brief module/UDP/gate instance リストの取得
  virtual
  PtInstArray
  inst_list() const = 0;

  /// @brief automatic 情報の取得
  /// @retval true automatic 宣言された task/function
  /// @retval false 上記以外
  virtual
  bool
  automatic() const = 0;

  /// @brief IO宣言の要素数の取得
  virtual
  int
  ioitem_num() const = 0;

  /// @brief IO宣言リストの配列の取得
  virtual
  PtIOHeadArray
  iohead_array() const = 0;

  /// @brief 宣言ヘッダ配列の取得
  virtual
  PtDeclHeadArray
  declhead_array() const = 0;

  /// @brief item 配列の取得
  virtual
  PtItemArray
  item_array() const = 0;

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
  tVpiVarType
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
  tVpiSpecItemType
  specitem_type() const = 0;

  /// @brief specify block path の種類の取得
  /// @return specify block path の種類
  virtual
  tVpiSpecPathType
  specpath_type() const = 0;

  /// @brief ターミナルのリストの取得
  virtual
  PtExprArray
  terminal_list() const = 0;

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

  /// @brief 条件が成り立ったときに生成される宣言ヘッダ配列の取得
  virtual
  PtDeclHeadArray
  then_declhead_array() const = 0;

  /// @brief 条件が成り立ったときに生成される item 配列の取得
  virtual
  PtItemArray
  then_item_array() const = 0;

  /// @brief 条件が成り立たなかったときに生成される宣言ヘッダ配列の取得
  virtual
  PtDeclHeadArray
  else_declhead_array() const = 0;

  /// @brief 条件が成り立たなかったときに生成される item 配列の取得
  virtual
  PtItemArray
  else_item_array() const = 0;

  /// @brief case item のリストの取得
  virtual
  PtGenCaseItemArray
  caseitem_list() const = 0;

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
  public PtBase
{
public:

  /// @brief 仮想デストラクタ
  virtual
  ~PtDefParam() { }


public:
  //////////////////////////////////////////////////////////////////////
  // PtDefParam の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 階層ブランチの取得
  virtual
  PtNameBranchArray
  namebranch_array() const = 0;

  /// @brief 末尾の名前の取得
  /// @return 末尾の名前
  virtual
  const char*
  name() const = 0;

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

  /// @brief 仮想デストラクタ
  virtual
  ~PtContAssign() { }


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

  /// @brief 仮想デストラクタ
  virtual
  ~PtInst() { }


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

  /// @brief ポートのリストの取得
  virtual
  PtConnectionArray
  port_list() const = 0;


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

  /// @brief 仮想デストラクタ
  virtual
  ~PtGenCaseItem() { }


public:
  //////////////////////////////////////////////////////////////////////
  // PtGenCaseItem の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ラベルのリストの取得
  virtual
  PtExprArray
  label_list() const = 0;

  /// @brief 宣言のリストの取得
  virtual
  PtDeclHeadArray
  declhead_array() const = 0;

  /// @brief item リストの取得
  virtual
  PtItemArray
  item_array() const = 0;

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

  /// @brief 仮想デストラクタ
  virtual
  ~PtPathDecl() { }


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

  /// @brief 入力のリストの取得
  virtual
  PtExprArray
  input_list() const = 0;

  /// @brief 入力の極性の取得
  /// @return 入力の極性\n
  /// 0の場合もありうる．
  virtual
  int
  input_pol() const = 0;

  /// @brief パス記述子(?)の取得
  /// @return vpiParallel か vpiFull
  virtual
  int
  op() const = 0;

  /// @brief 出力のリストの取得
  virtual
  PtExprArray
  output_list() const = 0;

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

  /// @brief 仮想デストラクタ
  virtual
  ~PtPathDelay() { }


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

END_NAMESPACE_YM_VERILOG

#endif // YM_PT_PTITEM_H
