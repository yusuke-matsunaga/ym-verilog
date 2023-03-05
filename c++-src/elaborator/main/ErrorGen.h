#ifndef ERRORGEN_H
#define ERRORGEN_H

/// @file ErrorGen.h
/// @brief ErrorGen のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.

#include "ym/verilog.h"
#include "ym/pt/PtP.h"
#include "ym/vl/VlFwd.h"
#include "ym/FileRegion.h"


BEGIN_NAMESPACE_YM_VERILOG

class ElbParamCon;
class VlDeclArray;

//////////////////////////////////////////////////////////////////////
/// @class ErrorGen ErrorGen.h "ErrorGen.h"
/// @brief エラー用の例外を送出するクラス
///
/// このクラスは静的メソッドしかもたない．
//////////////////////////////////////////////////////////////////////
class ErrorGen
{
public:

  /// @brief コンストラクタ
  ErrorGen() = delete;

  /// @brief デストラクタ
  ~ErrorGen() = delete;


public:
  //////////////////////////////////////////////////////////////////////
  // モジュール生成に関するエラー
  //////////////////////////////////////////////////////////////////////

  /// @brief パラメータポートの割り当て数が多すぎる．
  static
  void
  too_many_param(
    const char* file,                         ///< [in] ファイル名
    int line,                                 ///< [in] 行番号
    const vector<ElbParamCon>& param_con_list ///< [in] パラメータポートの割り当てリスト
  );

  /// @brief パラメータポートに現れるパラメータが存在しない．
  static
  void
  no_param(
    const char* file,           ///< [in] ファイル名
    int line,                   ///< [in] 行番号
    const PtConnection* pt_con, ///< [in] パラメータポート割り当てのパース木
    const char* name            ///< [in] パラメータ名
  );

  /// @brief 対象の要素が見つからない．
  static
  void
  not_found(
    const char* file,      ///< [in] ファイル名
    int line,              ///< [in] 行番号
    const FileRegion& loc, ///< [in] エラー箇所
    const char* name       ///< [in] 名前
  );

  /// @brief ポートに配列が使われている．
  static
  void
  port_array(
    const char* file,        ///< [in] ファイル名
    int line,                ///< [in] 行番号
    const FileRegion& loc,   ///< [in] エラー箇所
    const VlDeclArray* array ///< [in] 対象の配列
  );

  /// @brief ポートに使われている要素が宣言要素でなかった．
  static
  void
  illegal_port(
    const char* file,      ///< [in] ファイル名
    int line,              ///< [in] 行番号
    const FileRegion& loc, ///< [in] エラー箇所
    const char* name       ///< [in] 名前
  );


public:
  //////////////////////////////////////////////////////////////////////
  // 宣言要素の生成に関するエラー
  //////////////////////////////////////////////////////////////////////

  /// @brief 暗黙の宣言が禁止されている．
  static
  void
  no_impnet(
    const char* file,       ///< [in] ファイル名
    int line,               ///< [in] 行番号
    const PtIOItem* pt_item ///< [in] 対象の構文木要素
  );

  /// @brief 暗黙ネットが初期値を持っている．
  static
  void
  impnet_with_init(
    const char* file,       ///< [in] ファイル名
    int line,               ///< [in] 行番号
    const PtIOItem* pt_item ///< [in] 対象の構文木要素
  );

  /// @brief IO 宣言に aux_type と宣言が重複している．
  static
  void
  duplicate_type(
    const char* file,          ///< [in] ファイル名
    int line,                  ///< [in] 行番号
    const PtIOItem* pt_item,   ///< [in] IO宣言のパース木の要素
    const FileRegion& prev_loc ///< [in] 元の要素が宣言されていた箇所
  );

  /// @brief 配列要素が IO 宣言として現れていた．
  static
  void
  array_io(
    const char* file,       ///< [in] ファイル名
    int line,               ///< [in] 行番号
    const PtIOItem* pt_item ///< [in] IO宣言のパース木の要素
  );

  /// @brief IO 宣言に不適切な宣言要素が使われていた．
  static
  void
  illegal_io(
    const char* file,        ///< [in] ファイル名
    int line,                ///< [in] 行番号
    const PtIOItem* pt_item, ///< [in] IO宣言のパース木の要素
    const string& name,      ///< [in] 名前
    bool is_module           ///< [in] モジュールのIOの時 true とする．
  );

  /// @brief IO 宣言と宣言要素の範囲指定が異なる．
  static
  void
  conflict_io_range(
    const char* file,       ///< [in] ファイル名
    int line,               ///< [in] 行番号
    const PtIOItem* pt_item ///< [in] 対象の構文木要素
  );


public:
  //////////////////////////////////////////////////////////////////////
  // item 要素生成に関するエラー
  //////////////////////////////////////////////////////////////////////

  /// @brief 対象が parameter ではなかった．
  static
  void
  not_a_parameter(
    const char* file,         ///< [in] ファイル名
    int line,                 ///< [in] 行番号
    const PtDefParam* pt_item ///< [in] 対象の構文木要素
  );

  /// @brief 対象が localparam だった．
  static
  void
  is_a_localparam(
    const char* file,         ///< [in] ファイル名
    int line,                 ///< [in] 行番号
    const PtDefParam* pt_item ///< [in] 対象の構文木要素
  );

  /// @brief 重複した generate case ラベル
  static
  void
  duplicate_gencase_labels(
    const char* file,     ///< [in] ファイル名
    int line,             ///< [in] 行番号
    const PtItem* pt_item ///< [in] 対象の構文木要素
  );

  /// @brief generate for 文の変数が見つからない．
  static
  void
  genvar_not_found(
    const char* file,     ///< [in] ファイル名
    int line,             ///< [in] 行番号
    const PtItem* pt_item ///< [in] 対象の構文木要素
  );

  /// @brief genvar でなかった．
  static
  void
  not_a_genvar(
    const char* file,     ///< [in] ファイル名
    int line,             ///< [in] 行番号
    const PtItem* pt_item ///< [in] 対象の構文木要素
  );

  /// @brief 他の generate for loop ですでに用いられている．
  static
  void
  genvar_in_use(
    const char* file,     ///< [in] ファイル名
    int line,             ///< [in] 行番号
    const PtItem* pt_item ///< [in] 対象の構文木要素
  );

  /// @brief genvar の値が負の値だった．
  static
  void
  genvar_negative(
    const char* file,     ///< [in] ファイル名
    int line,		  ///< [in] 行番号
    const PtItem* pt_item ///< [in] 対象の構文木要素
  );

  /// @brief モジュールの依存関係が循環している．
  static
  void
  cyclic_dependency(
    const char* file,         ///< [in] ファイル名
    int line,		      ///< [in] 行番号
    const PtModule* pt_module ///< [in] 対象の構文木要素
  );

  /// @brief 名無しのモジュール定義
  static
  void
  noname_module(
    const char* file,     ///< [in] ファイル名
    int line,		  ///< [in] 行番号
    const PtInst* pt_inst ///< [in] 対象の構文木要素
  );

  /// @brief UDP インスタンスに名前付きのparameter割り当てがあった．
  static
  void
  udp_with_named_paramassign(
    const char* file,     ///< [in] ファイル名
    int line,		  ///< [in] 行番号
    const PtItem* pt_item ///< [in] 対象の構文木要素
  );

  /// @brief UDP インスタンスにparameter割り当てがあった．
  static
  void
  udp_with_ordered_paramassign(
    const char* file,      ///< [in] ファイル名
    int line,		   ///< [in] 行番号
    const PtItem* pt_item  ///< [in] 対象の構文木要素
  );

  /// @brief セルインスタンスにparameter割り当てがあった．
  static
  void
  cell_with_paramassign(
    const char* file,     ///< [in] ファイル名
    int line,		  ///< [in] 行番号
    const PtItem* pt_item ///< [in] 対象の構文木要素
  );

  /// @brief インスタンスが見つからなかった．
  static
  void
  instance_not_found(
    const char* file,     ///< [in] ファイル名
    int line,		  ///< [in] 行番号
    const PtItem* pt_item ///< [in] 対象の構文木要素
  );

  /// @brief モジュールインスタンスのポート数が合わない．
  static
  void
  too_many_items_in_port_list(
    const char* file,     ///< [in] ファイル名
    int line,		  ///< [in] 行番号
    const PtInst* pt_inst ///< [in] 対象の構文木要素
  );

  /// @brief ポート名が存在しなかった．
  static
  void
  illegal_port_name(
    const char* file,          ///< [in] ファイル名
    int line,		       ///< [in] 行番号
    const PtConnection* pt_con ///< [in] 対象の構文木要素
  );

  /// @brief ポートに real 型の式を接続していた．
  static
  void
  real_type_in_port_list(
    const char* file,   ///< [in] ファイル名
    int line,	        ///< [in] 行番号
    const VlExpr* expr  ///< [in] 対象の式
  );

  /// @brief ポートサイズが合わない．
  static
  void
  port_size_mismatch(
    const char* file,      ///< [in] ファイル名
    int line,		   ///< [in] 行番号
    const PtExpr* pt_expr, ///< [in] 対象の構文木要素
    const string& name,    ///< [in] モジュール名
    int index              ///< [in] ポート番号(0から始まる)
  );

  /// @brief UDP instance に名前付きポート割り当てがあった．
  static
  void
  named_port_in_udp_instance(
    const char* file,     ///< [in] ファイル名
    int line,		  ///< [in] 行番号
    const PtInst* pt_inst ///< [in] 対象の構文木要素
  );

  /// @brief UDP instance のポート数が合わない．
  static
  void
  port_num_mismatch(
    const char* file,     ///< [in] ファイル名
    int line,		  ///< [in] 行番号
    const PtInst* pt_inst ///< [in] 対象の構文木要素
  );

  /// @brief cell instance のピン名が合わない．
  static
  void
  illegal_pin_name(
    const char* file,          ///< [in] ファイル名
    int line,		       ///< [in] 行番号
    const PtConnection* pt_con ///< [in] 対象の構文木要素
  );

  /// @brief 空のポート式
  static
  void
  empty_port_expression(
    const char* file,          ///< [in] ファイル名
    int line,		       ///< [in] 行番号
    const PtConnection* pt_con ///< [in] 対象の構文木要素
  );


public:
  //////////////////////////////////////////////////////////////////////
  // 定数式が求められるコンテキストでのエラー
  // ElbConstError (もしくはその継承クラス) を例外として送出する．
  //////////////////////////////////////////////////////////////////////

  /// @brief constant function 中にシステム関数呼び出し
  static
  void
  illegal_sysfunccall_in_cf(
    const char* file,     ///< [in] ファイル名
    int line,		  ///< [in] 行番号
    const PtExpr* pt_expr ///< [in] 対象の構文木要素
  );

  /// @brief constant expression 中にシステム関数呼び出し
  static
  void
  illegal_sysfunccall_in_ce(
    const char* file,     ///< [in] ファイル名
    int line,		  ///< [in] 行番号
    const PtExpr* pt_expr ///< [in] 対象の構文木要素
  );

  /// @brief 定数関数は自己再帰できない．
  static
  void
  uses_itself(
    const char* file,     ///< [in] ファイル名
    int line,		  ///< [in] 行番号
    const PtExpr* pt_expr ///< [in] 対象の構文木要素
  );

  /// @brief 定数関数ではない．
  static
  void
  not_a_constant_function(
    const char* file,     ///< [in] ファイル名
    int line,		  ///< [in] 行番号
    const PtExpr* pt_expr ///< [in] 対象の構文木要素
  );

  /// @brief オブジェクトの型が constant function 用として不適切
  static
  void
  illegal_object_cf(
    const char* file,     ///< [in] ファイル名
    int line,		  ///< [in] 行番号
    const PtExpr* pt_expr ///< [in] 対象の構文木要素
  );

  /// @brief 階層名が constant expression 中にあった
  static
  void
  hname_in_ce(
    const char* file,     ///< [in] ファイル名
    int line,		  ///< [in] 行番号
    const PtExpr* pt_expr ///< [in] 対象の構文木要素
  );

  /// @brief 階層名が constant function 中にあった
  static
  void
  hname_in_cf(
    const char* file,     ///< [in] ファイル名
    int line,		  ///< [in] 行番号
    const PtExpr* pt_expr ///< [in] 対象の構文木要素
  );

  /// @brief オブジェクトが parameter でなかった
  static
  void
  not_a_parameter(
    const char* file,     ///< [in] ファイル名
    int line,		  ///< [in] 行番号
    const PtExpr* pt_expr ///< [in] 対象の構文木要素
  );


public:
  //////////////////////////////////////////////////////////////////////
  // イベント式が求められるコンテキストでのエラー
  //////////////////////////////////////////////////////////////////////


  /// @brief イベント式の根元に定数
  static
  void
  illegal_constant_in_event_expression(
    const char* file,     ///< [in] ファイル名
    int line,		  ///< [in] 行番号
    const PtExpr* pt_expr ///< [in] 対象の構文木要素
  );

  /// @brief イベント式の根元に関数呼び出し
  static
  void
  illegal_funccall_in_event_expression(
    const char* file,     ///< [in] ファイル名
    int line,		  ///< [in] 行番号
    const PtExpr* pt_expr ///< [in] 対象の構文木要素
  );

  /// @brief イベント式の根元にシステム関数呼び出し
  static
  void
  illegal_sysfunccall_in_event_expression(
    const char* file,     ///< [in] ファイル名
    int line,		  ///< [in] 行番号
    const PtExpr* pt_expr ///< [in] 対象の構文木要素
  );


public:
  //////////////////////////////////////////////////////////////////////
  // 左辺式が求められるコンテキストでのエラー
  //////////////////////////////////////////////////////////////////////

  /// @brief 左辺式で用いることのできない演算子
  static
  void
  illegal_operator_in_lhs(
    const char* file,     ///< [in] ファイル名
    int line,		  ///< [in] 行番号
    const PtExpr* pt_expr ///< [in] 対象の構文木要素
  );

  /// @brief 左辺式に定数
  static
  void
  illegal_constant_in_lhs(
    const char* file,     ///< [in] ファイル名
    int line,		  ///< [in] 行番号
    const PtExpr* pt_expr ///< [in] 対象の構文木要素
  );

  /// @brief 左辺式に関数呼び出し
  static
  void
  illegal_funccall_in_lhs(
    const char* file,     ///< [in] ファイル名
    int line,		  ///< [in] 行番号
    const PtExpr* pt_expr ///< [in] 対象の構文木要素
  );

  /// @brief 左辺式にシステム関数呼び出し
  static
  void
  illegal_sysfunccall_in_lhs(
    const char* file,     ///< [in] ファイル名
    int line,		  ///< [in] 行番号
    const PtExpr* pt_expr ///< [in] 対象の構文木要素
  );


public:
  //////////////////////////////////////////////////////////////////////
  // 通常の式におけるエラー
  //////////////////////////////////////////////////////////////////////

  /// @brief int 型が要求されている所で互換性のない型があった．
  static
  void
  int_required(
    const char* file,     ///< [in] ファイル名
    int line,		  ///< [in] 行番号
    const FileRegion& loc ///< [in] エラー箇所
  );

  /// @brief ビットベクタ型が要求されている所で互換性のない型があった．
  static
  void
  bv_required(
    const char* file,     ///< [in] ファイル名
    int line,		  ///< [in] 行番号
    const FileRegion& loc ///< [in] エラー箇所
  );

  /// @brief 通常の式中に edge descriptor
  static
  void
  illegal_edge_descriptor(
    const char* file,     ///< [in] ファイル名
    int line,		  ///< [in] 行番号
    const PtExpr* pt_expr ///< [in] 対象の構文木要素
  );

  /// @brief real 型のオペランドをとれない
  static
  void
  illegal_real_type(
    const char* file,     ///< [in] ファイル名
    int line,		  ///< [in] 行番号
    const PtExpr* pt_expr ///< [in] 対象の構文木要素
  );

  /// @brief 該当する関数が存在しない．
  static
  void
  no_such_function(
    const char* file,     ///< [in] ファイル名
    int line,		  ///< [in] 行番号
    const PtExpr* pt_expr ///< [in] 対象の構文木要素
  );

  /// @brief 該当するシステム関数が存在しない．
  static
  void
  no_such_sysfunction(
    const char* file,     ///< [in] ファイル名
    int line,		  ///< [in] 行番号
    const PtExpr* pt_expr ///< [in] 対象の構文木要素
  );

  /// @brief 関数ではない．
  static
  void
  not_a_function(
    const char* file,     ///< [in] ファイル名
    int line,		  ///< [in] 行番号
    const PtExpr* pt_expr ///< [in] 対象の構文木要素
  );

  /// @brief 引数の数が合わない．
  static
  void
  n_of_arguments_mismatch(
    const char* file,     ///< [in] ファイル名
    int line,		  ///< [in] 行番号
    const PtExpr* pt_expr ///< [in] 対象の構文木要素
  );

  /// @brief 引数の型が合わない．
  static
  void
  illegal_argument_type(
    const char* file,     ///< [in] ファイル名
    int line,		  ///< [in] 行番号
    const PtExpr* pt_expr ///< [in] 対象の構文木要素
  );

  /// @brief オブジェクトが存在しない
  static
  void
  not_found(
    const char* file,     ///< [in] ファイル名
    int line,		  ///< [in] 行番号
    const PtExpr* pt_expr ///< [in] 対象の構文木要素
  );

  /// @brief オブジェクトの型が不適切
  static
  void
  illegal_object(
    const char* file,     ///< [in] ファイル名
    int line,		  ///< [in] 行番号
    const PtExpr* pt_expr ///< [in] 対象の構文木要素
  );

  /// @brief オブジェクトが named-event でなかった
  static
  void
  not_a_namedevent(
    const char* file,     ///< [in] ファイル名
    int line,		  ///< [in] 行番号
    const PtExpr* pt_expr ///< [in] 対象の構文木要素
  );

  /// @brief 要素の範囲の順番と範囲指定の順番が異なる．
  static
  void
  range_order(
    const char* file,     ///< [in] ファイル名
    int line,		  ///< [in] 行番号
    const PtExpr* pt_expr ///< [in] 対象の構文木要素
  );

  /// @brief named-event に対する範囲指定
  static
  void
  select_for_namedevent(
    const char* file,     ///< [in] ファイル名
    int line,		  ///< [in] 行番号
    const PtExpr* pt_expr ///< [in] 対象の構文木要素
  );

  /// @brief assign/deassign に不適切なビット/範囲指定
  static
  void
  select_in_pca(
    const char* file,     ///< [in] ファイル名
    int line,		  ///< [in] 行番号
    const PtExpr* pt_expr ///< [in] 対象の構文木要素
  );

  /// @brief force/release に不適切なビット/範囲指定
  static
  void
  select_in_force(
    const char* file,     ///< [in] ファイル名
    int line,		  ///< [in] 行番号
    const PtExpr* pt_expr ///< [in] 対象の構文木要素
  );

  /// @brief assign/deassign に不適切な配列要素
  static
  void
  array_in_pca(
    const char* file,     ///< [in] ファイル名
    int line,		  ///< [in] 行番号
    const PtExpr* pt_expr ///< [in] 対象の構文木要素
  );

  /// @brief force/release に不適切な配列要素
  static
  void
  array_in_force(
    const char* file,     ///< [in] ファイル名
    int line,		  ///< [in] 行番号
    const PtExpr* pt_expr ///< [in] 対象の構文木要素
  );

  /// @brief 配列の次元が合わない
  static
  void
  dimension_mismatch(
    const char* file,     ///< [in] ファイル名
    int line,		  ///< [in] 行番号
    const PtExpr* pt_expr ///< [in] 対象の構文木要素
  );

  /// @brief real 型に対するビット選択あるいは部分選択があった
  static
  void
  select_for_real(
    const char* file,     ///< [in] ファイル名
    int line,		  ///< [in] 行番号
    const PtExpr* pt_expr ///< [in] 対象の構文木要素
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 階層名付きのエラーメッセージを生成する共通部分(ElbConstError用)
  static
  void
  const_with_hname(
    const char* file,      ///< [in] ファイル名
    int line,		   ///< [in] 行番号
    const PtExpr* pt_expr, ///< [in] 対象の構文木要素
    const char* label,     ///< [in] エラーラベル
    const string& msg      ///< [in] エラーメッセージ
  );

  /// @brief ElbConstError を送出する．
  static
  void
  const_common(
    const char* file,      ///< [in] ファイル名
    int line,		   ///< [in] 行番号
    const PtExpr* pt_expr, ///< [in] 対象の構文木要素
    const char* label,     ///< [in] エラーラベル
    const string& msg	   ///< [in] エラーメッセージ
  );

  /// @brief 階層名付きのエラーメッセージを生成する共通部分
  static
  void
  expr_with_hname(
    const char* file,      ///< [in] ファイル名
    int line,		   ///< [in] 行番号
    const PtExpr* pt_expr, ///< [in] 対象の構文木要素
    const char* label,     ///< [in] エラーラベル
    const string& msg	   ///< [in] エラーメッセージ
  );

  /// @brief PtExpr に関するエラーメッセージを生成する共通部分
  static
  void
  expr_common(
    const char* file,      ///< [in] ファイル名
    int line,		   ///< [in] 行番号
    const PtExpr* pt_expr, ///< [in] 対象の構文木要素
    const char* label,     ///< [in] エラーラベル
    const string& msg	   ///< [in] エラーメッセージ
  );

  /// @brief エラーメッセージを生成する共通部分
  static
  void
  error_common(
    const char* file,      ///< [in] ファイル名
    int line,		   ///< [in] 行番号
    const FileRegion& loc, ///< [in] 対象の構文木要素
    const char* label,     ///< [in] エラーラベル
    const string& msg	   ///< [in] エラーメッセージ
  );

  /// @brief 最初に識別名を冠したメッセージを作る．
  static
  string
  make_message(
    const PtExpr* pt_expr, ///< [in] 式を表すパース木
    const string& msg      ///< [in] メッセージ
  );

};

END_NAMESPACE_YM_VERILOG

#endif // ERRORGEN_H
