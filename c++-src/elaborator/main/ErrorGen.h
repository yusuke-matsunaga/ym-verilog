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
  /// @param[in] file ファイル名
  /// @param[in] line 行番号
  /// @param[in] param_con_list パラメータポートの割り当てリスト
  static
  void
  too_many_param(const char* file,
		 int line,
		 const vector<ElbParamCon>& param_con_list);

  /// @brief パラメータポートに現れるパラメータが存在しない．
  /// @param[in] file ファイル名
  /// @param[in] line 行番号
  /// @param[in] pt_con パラメータポート割り当てのパース木
  /// @param[in] name パラメータ名
  static
  void
  no_param(const char* file,
	   int line,
	   const PtConnection* pt_con,
	   const char* name);

  /// @brief 対象の要素が見つからない．
  /// @param[in] file ファイル名
  /// @param[in] line 行番号
  /// @param[in] loc エラー箇所
  /// @param[in] name 名前
  static
  void
  not_found(const char* file,
	    int line,
	    const FileRegion& loc,
	    const char* name);

  /// @brief ポートに配列が使われている．
  /// @param[in] file ファイル名
  /// @param[in] line 行番号
  /// @param[in] loc エラー箇所
  /// @param[in] array 対象の配列
  static
  void
  port_array(const char* file,
	     int line,
	     const FileRegion& loc,
	     const VlDeclArray* array);

  /// @brief ポートに使われている要素が宣言要素でなかった．
  /// @param[in] file ファイル名
  /// @param[in] line 行番号
  /// @param[in] loc エラー箇所
  /// @param[in] name 名前
  static
  void
  illegal_port(const char* file,
	       int line,
	       const FileRegion& loc,
	       const char* name);


public:
  //////////////////////////////////////////////////////////////////////
  // 宣言要素の生成に関するエラー
  //////////////////////////////////////////////////////////////////////

  /// @brief 暗黙の宣言が禁止されている．
  /// @param[in] file ファイル名
  /// @param[in] line 行番号
  /// @param[in] pt_item 対象の構文木要素
  static
  void
  no_impnet(const char* file,
	    int line,
	    const PtIOItem* pt_item);

  /// @brief 暗黙ネットが初期値を持っている．
  /// @param[in] file ファイル名
  /// @param[in] line 行番号
  /// @param[in] pt_item 対象の構文木要素
  static
  void
  impnet_with_init(const char* file,
		   int line,
		   const PtIOItem* pt_item);

  /// @brief IO 宣言に aux_type と宣言が重複している．
  /// @param[in] file ファイル名
  /// @param[in] line 行番号
  /// @param[in] pt_item IO宣言のパース木の要素
  /// @param[in] prev_loc 元の要素が宣言されていた箇所
  static
  void
  duplicate_type(const char* file,
		 int line,
		 const PtIOItem* pt_item,
		 const FileRegion& prev_loc);

  /// @brief 配列要素が IO 宣言として現れていた．
  /// @param[in] file ファイル名
  /// @param[in] line 行番号
  /// @param[in] pt_item IO宣言のパース木の要素
  static
  void
  array_io(const char* file,
	   int line,
	   const PtIOItem* pt_item);

  /// @brief IO 宣言に不適切な宣言要素が使われていた．
  /// @param[in] file ファイル名
  /// @param[in] line 行番号
  /// @param[in] pt_item IO宣言のパース木の要素
  /// @param[in] name 名前
  /// @param[in] is_module モジュールのIOの時 true とする．
  static
  void
  illegal_io(const char* file,
	     int line,
	     const PtIOItem* pt_item,
	     const string& name,
	     bool is_module);

  /// @brief IO 宣言と宣言要素の範囲指定が異なる．
  /// @param[in] file ファイル名
  /// @param[in] line 行番号
  /// @param[in] pt_item 対象の構文木要素
  static
  void
  conflict_io_range(const char* file,
		    int line,
		    const PtIOItem* pt_item);


public:
  //////////////////////////////////////////////////////////////////////
  // item 要素生成に関するエラー
  //////////////////////////////////////////////////////////////////////

  /// @brief 対象が parameter ではなかった．
  /// @param[in] file ファイル名
  /// @param[in] line 行番号
  /// @param[in] pt_item 対象の構文木要素
  static
  void
  not_a_parameter(const char* file,
		  int line,
		  const PtDefParam* pt_item);

  /// @brief 対象が localparam だった．
  /// @param[in] file ファイル名
  /// @param[in] line 行番号
  /// @param[in] pt_item 対象の構文木要素
  static
  void
  is_a_localparam(const char* file,
		  int line,
		  const PtDefParam* pt_item);

  /// @brief 重複した generate case ラベル
  /// @param[in] file ファイル名
  /// @param[in] line 行番号
  /// @param[in] pt_item 対象の構文木要素
  static
  void
  duplicate_gencase_labels(const char* file,
			   int line,
			   const PtItem* pt_item);

  /// @brief generate for 文の変数が見つからない．
  /// @param[in] file ファイル名
  /// @param[in] line 行番号
  /// @param[in] pt_item 対象の構文木要素
  static
  void
  genvar_not_found(const char* file,
		   int line,
		   const PtItem* pt_item);

  /// @brief genvar でなかった．
  /// @param[in] file ファイル名
  /// @param[in] line 行番号
  /// @param[in] pt_item 対象の構文木要素
  static
  void
  not_a_genvar(const char* file,
	       int line,
	       const PtItem* pt_item);

  /// @brief 他の generate for loop ですでに用いられている．
  /// @param[in] file ファイル名
  /// @param[in] line 行番号
  /// @param[in] pt_item 対象の構文木要素
  static
  void
  genvar_in_use(const char* file,
		int line,
		const PtItem* pt_item);

  /// @brief genvar の値が負の値だった．
  /// @param[in] file ファイル名
  /// @param[in] line 行番号
  /// @param[in] pt_item 対象の構文木要素
  static
  void
  genvar_negative(const char* file,
		  int line,
		  const PtItem* pt_item);

  /// @brief モジュールの依存関係が循環している．
  /// @param[in] file ファイル名
  /// @param[in] line 行番号
  /// @param[in] pt_module 対象の構文木要素
  static
  void
  cyclic_dependency(const char* file,
		    int line,
		    const PtModule* pt_module);

  /// @brief 名無しのモジュール定義
  /// @param[in] file ファイル名
  /// @param[in] line 行番号
  /// @param[in] pt_inst 対象の構文木要素
  static
  void
  noname_module(const char* file,
		int line,
		const PtInst* pt_inst);

  /// @brief UDP インスタンスに名前付きのparameter割り当てがあった．
  /// @param[in] file ファイル名
  /// @param[in] line 行番号
  /// @param[in] pt_item 対象の構文木要素
  static
  void
  udp_with_named_paramassign(const char* file,
			     int line,
			     const PtItem* pt_item);

  /// @brief UDP インスタンスにparameter割り当てがあった．
  /// @param[in] file ファイル名
  /// @param[in] line 行番号
  /// @param[in] pt_item 対象の構文木要素
  static
  void
  udp_with_ordered_paramassign(const char* file,
			       int line,
			       const PtItem* pt_item);

  /// @brief セルインスタンスにparameter割り当てがあった．
  /// @param[in] file ファイル名
  /// @param[in] line 行番号
  /// @param[in] pt_item 対象の構文木要素
  static
  void
  cell_with_paramassign(const char* file,
			int line,
			const PtItem* pt_item);

  /// @brief インスタンスが見つからなかった．
  /// @param[in] file ファイル名
  /// @param[in] line 行番号
  /// @param[in] pt_item 対象の構文木要素
  static
  void
  instance_not_found(const char* file,
		     int line,
		     const PtItem* pt_item);

  /// @brief モジュールインスタンスのポート数が合わない．
  /// @param[in] file ファイル名
  /// @param[in] line 行番号
  /// @param[in] pt_inst 対象の構文木要素
  static
  void
  too_many_items_in_port_list(const char* file,
			      int line,
			      const PtInst* pt_inst);

  /// @brief ポート名が存在しなかった．
  /// @param[in] file ファイル名
  /// @param[in] line 行番号
  /// @param[in] pt_con 対象の構文木要素
  static
  void
  illegal_port_name(const char* file,
		    int line,
		    const PtConnection* pt_con);

  /// @brief ポートに real 型の式を接続していた．
  /// @param[in] file ファイル名
  /// @param[in] line 行番号
  /// @param[in] expr 対象の式
  static
  void
  real_type_in_port_list(const char* file,
			 int line,
			 const VlExpr* expr);

  /// @brief ポートサイズが合わない．
  /// @param[in] file ファイル名
  /// @param[in] line 行番号
  /// @param[in] pt_expr 対象の式
  /// @param[in] name モジュール名
  /// @param[in] index ポート番号(0から始まる)
  static
  void
  port_size_mismatch(const char* file,
		     int line,
		     const PtExpr* pt_expr,
		     const string& name,
		     int index);

  /// @brief UDP instance に名前付きポート割り当てがあった．
  /// @param[in] file ファイル名
  /// @param[in] line 行番号
  /// @param[in] pt_inst 対象の構文木要素
  static
  void
  named_port_in_udp_instance(const char* file,
			     int line,
			     const PtInst* pt_inst);

  /// @brief UDP instance のポート数が合わない．
  /// @param[in] file ファイル名
  /// @param[in] line 行番号
  /// @param[in] pt_inst 対象の構文木要素
  static
  void
  port_num_mismatch(const char* file,
		    int line,
		    const PtInst* pt_inst);

  /// @brief cell instance のピン名が合わない．
  /// @param[in] file ファイル名
  /// @param[in] line 行番号
  /// @param[in] pt_con 対象の構文木要素
  static
  void
  illegal_pin_name(const char* file,
		   int line,
		   const PtConnection* pt_con);

  /// @brief 空のポート式
  /// @param[in] file ファイル名
  /// @param[in] line 行番号
  /// @param[in] pt_con 対象の構文木要素
  static
  void
  empty_port_expression(const char* file,
			int line,
			const PtConnection* pt_con);


public:
  //////////////////////////////////////////////////////////////////////
  // 定数式が求められるコンテキストでのエラー
  // ElbConstError (もしくはその継承クラス) を例外として送出する．
  //////////////////////////////////////////////////////////////////////

  /// @brief constant function 中にシステム関数呼び出し
  /// @param[in] file ファイル名
  /// @param[in] line 行番号
  /// @param[in] pt_expr 式を表すパース木
  static
  void
  illegal_sysfunccall_in_cf(const char* file,
			    int line,
			    const PtExpr* pt_expr);

  /// @brief constant expression 中にシステム関数呼び出し
  /// @param[in] file ファイル名
  /// @param[in] line 行番号
  /// @param[in] pt_expr 式を表すパース木
  static
  void
  illegal_sysfunccall_in_ce(const char* file,
			    int line,
			    const PtExpr* pt_expr);

  /// @brief 定数関数は自己再帰できない．
  /// @param[in] file ファイル名
  /// @param[in] line 行番号
  /// @param[in] pt_expr 式を表すパース木
  static
  void
  uses_itself(const char* file,
	      int line,
	      const PtExpr* pt_expr);

  /// @brief 定数関数ではない．
  /// @param[in] file ファイル名
  /// @param[in] line 行番号
  /// @param[in] pt_expr 式を表すパース木
  static
  void
  not_a_constant_function(const char* file,
			  int line,
			  const PtExpr* pt_expr);

  /// @brief オブジェクトの型が constant function 用として不適切
  /// @param[in] file ファイル名
  /// @param[in] line 行番号
  /// @param[in] pt_expr 式を表すパース木
  static
  void
  illegal_object_cf(const char* file,
		    int line,
		    const PtExpr* pt_expr);

  /// @brief 階層名が constant expression 中にあった
  /// @param[in] file ファイル名
  /// @param[in] line 行番号
  /// @param[in] pt_expr 式を表すパース木
  static
  void
  hname_in_ce(const char* file,
	      int line,
	      const PtExpr* pt_expr);

  /// @brief 階層名が constant function 中にあった
  /// @param[in] file ファイル名
  /// @param[in] line 行番号
  /// @param[in] pt_expr 式を表すパース木
  static
  void
  hname_in_cf(const char* file,
	      int line,
	      const PtExpr* pt_expr);

  /// @brief オブジェクトが parameter でなかった
  /// @param[in] file ファイル名
  /// @param[in] line 行番号
  /// @param[in] pt_expr 式を表すパース木
  static
  void
  not_a_parameter(const char* file,
		  int line,
		  const PtExpr* pt_expr);


public:
  //////////////////////////////////////////////////////////////////////
  // イベント式が求められるコンテキストでのエラー
  //////////////////////////////////////////////////////////////////////


  /// @brief イベント式の根元に定数
  /// @param[in] file ファイル名
  /// @param[in] line 行番号
  /// @param[in] pt_expr 式を表すパース木
  static
  void
  illegal_constant_in_event_expression(const char* file,
				       int line,
				       const PtExpr* pt_expr);

  /// @brief イベント式の根元に関数呼び出し
  /// @param[in] file ファイル名
  /// @param[in] line 行番号
  /// @param[in] pt_expr 式を表すパース木
  static
  void
  illegal_funccall_in_event_expression(const char* file,
				       int line,
				       const PtExpr* pt_expr);

  /// @brief イベント式の根元にシステム関数呼び出し
  /// @param[in] file ファイル名
  /// @param[in] line 行番号
  /// @param[in] pt_expr 式を表すパース木
  static
  void
  illegal_sysfunccall_in_event_expression(const char* file,
					  int line,
					  const PtExpr* pt_expr);


public:
  //////////////////////////////////////////////////////////////////////
  // 左辺式が求められるコンテキストでのエラー
  //////////////////////////////////////////////////////////////////////

  /// @brief 左辺式で用いることのできない演算子
  /// @param[in] file ファイル名
  /// @param[in] line 行番号
  /// @param[in] pt_expr 式を表すパース木
  static
  void
  illegal_operator_in_lhs(const char* file,
			  int line,
			  const PtExpr* pt_expr);

  /// @brief 左辺式に定数
  /// @param[in] file ファイル名
  /// @param[in] line 行番号
  /// @param[in] pt_expr 式を表すパース木
  static
  void
  illegal_constant_in_lhs(const char* file,
			  int line,
			  const PtExpr* pt_expr);

  /// @brief 左辺式に関数呼び出し
  /// @param[in] file ファイル名
  /// @param[in] line 行番号
  /// @param[in] pt_expr 式を表すパース木
  static
  void
  illegal_funccall_in_lhs(const char* file,
			  int line,
			  const PtExpr* pt_expr);

  /// @brief 左辺式にシステム関数呼び出し
  /// @param[in] file ファイル名
  /// @param[in] line 行番号
  /// @param[in] pt_expr 式を表すパース木
  static
  void
  illegal_sysfunccall_in_lhs(const char* file,
			     int line,
			     const PtExpr* pt_expr);


public:
  //////////////////////////////////////////////////////////////////////
  // 通常の式におけるエラー
  //////////////////////////////////////////////////////////////////////

  /// @brief int 型が要求されている所で互換性のない型があった．
  /// @param[in] file ファイル名
  /// @param[in] line 行番号
  /// @param[in] loc エラー箇所
  static
  void
  int_required(const char* file,
	       int line,
	       const FileRegion& loc);

  /// @brief ビットベクタ型が要求されている所で互換性のない型があった．
  /// @param[in] file ファイル名
  /// @param[in] line 行番号
  /// @param[in] loc エラー箇所
  static
  void
  bv_required(const char* file,
	      int line,
	      const FileRegion& loc);

  /// @brief 通常の式中に edge descriptor
  /// @param[in] file ファイル名
  /// @param[in] line 行番号
  /// @param[in] pt_expr 式を表すパース木
  static
  void
  illegal_edge_descriptor(const char* file,
			  int line,
			  const PtExpr* pt_expr);

  /// @brief real 型のオペランドをとれない
  /// @param[in] file ファイル名
  /// @param[in] line 行番号
  /// @param[in] pt_expr 式を表すパース木
  static
  void
  illegal_real_type(const char* file,
		    int line,
		    const PtExpr* pt_expr);

  /// @brief 該当する関数が存在しない．
  /// @param[in] file ファイル名
  /// @param[in] line 行番号
  /// @param[in] pt_expr 式を表すパース木
  static
  void
  no_such_function(const char* file,
		   int line,
		   const PtExpr* pt_expr);

  /// @brief 該当するシステム関数が存在しない．
  /// @param[in] file ファイル名
  /// @param[in] line 行番号
  /// @param[in] pt_expr 式を表すパース木
  static
  void
  no_such_sysfunction(const char* file,
		      int line,
		      const PtExpr* pt_expr);

  /// @brief 関数ではない．
  /// @param[in] file ファイル名
  /// @param[in] line 行番号
  /// @param[in] pt_expr 式を表すパース木
  static
  void
  not_a_function(const char* file,
		 int line,
		 const PtExpr* pt_expr);

  /// @brief 引数の数が合わない．
  /// @param[in] file ファイル名
  /// @param[in] line 行番号
  /// @param[in] pt_expr 式を表すパース木
  static
  void
  n_of_arguments_mismatch(const char* file,
			  int line,
			  const PtExpr* pt_expr);

  /// @brief 引数の型が合わない．
  /// @param[in] file ファイル名
  /// @param[in] line 行番号
  /// @param[in] pt_expr 式を表すパース木
  static
  void
  illegal_argument_type(const char* file,
			int line,
			const PtExpr* pt_expr);

  /// @brief オブジェクトが存在しない
  /// @param[in] file ファイル名
  /// @param[in] line 行番号
  /// @param[in] pt_expr 式を表すパース木
  static
  void
  not_found(const char* file,
	    int line,
	    const PtExpr* pt_expr);

  /// @brief オブジェクトの型が不適切
  /// @param[in] file ファイル名
  /// @param[in] line 行番号
  /// @param[in] pt_expr 式を表すパース木
  static
  void
  illegal_object(const char* file,
		 int line,
		 const PtExpr* pt_expr);

  /// @brief オブジェクトが named-event でなかった
  /// @param[in] file ファイル名
  /// @param[in] line 行番号
  /// @param[in] pt_expr 式を表すパース木
  static
  void
  not_a_namedevent(const char* file,
		   int line,
		   const PtExpr* pt_expr);

  /// @brief 要素の範囲の順番と範囲指定の順番が異なる．
  /// @param[in] file ファイル名
  /// @param[in] line 行番号
  /// @param[in] pt_expr 式を表すパース木
  static
  void
  range_order(const char* file,
	      int line,
	      const PtExpr* pt_expr);

  /// @brief named-event に対する範囲指定
  /// @param[in] file ファイル名
  /// @param[in] line 行番号
  /// @param[in] pt_expr 式を表すパース木
  static
  void
  select_for_namedevent(const char* file,
			int line,
			const PtExpr* pt_expr);

  /// @brief assign/deassign に不適切なビット/範囲指定
  /// @param[in] file ファイル名
  /// @param[in] line 行番号
  /// @param[in] pt_expr 式を表すパース木
  static
  void
  select_in_pca(const char* file,
		int line,
		const PtExpr* pt_expr);

  /// @brief force/release に不適切なビット/範囲指定
  /// @param[in] file ファイル名
  /// @param[in] line 行番号
  /// @param[in] pt_expr 式を表すパース木
  static
  void
  select_in_force(const char* file,
		  int line,
		  const PtExpr* pt_expr);

  /// @brief assign/deassign に不適切な配列要素
  /// @param[in] file ファイル名
  /// @param[in] line 行番号
  /// @param[in] pt_expr 式を表すパース木
  static
  void
  array_in_pca(const char* file,
	       int line,
	       const PtExpr* pt_expr);

  /// @brief force/release に不適切な配列要素
  /// @param[in] file ファイル名
  /// @param[in] line 行番号
  /// @param[in] pt_expr 式を表すパース木
  static
  void
  array_in_force(const char* file,
		 int line,
		 const PtExpr* pt_expr);

  /// @brief 配列の次元が合わない
  /// @param[in] file ファイル名
  /// @param[in] line 行番号
  /// @param[in] pt_expr 式を表すパース木
  static
  void
  dimension_mismatch(const char* file,
		     int line,
		     const PtExpr* pt_expr);

  /// @brief real 型に対するビット選択あるいは部分選択があった
  /// @param[in] file ファイル名
  /// @param[in] line 行番号
  /// @param[in] pt_expr 式を表すパース木
  static
  void
  select_for_real(const char* file,
		  int line,
		  const PtExpr* pt_expr);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 階層名付きのエラーメッセージを生成する共通部分(ElbConstError用)
  /// @param[in] file ファイル名
  /// @param[in] line 行番号
  /// @param[in] pt_expr 式を表すパース木
  /// @param[in] label エラーラベル
  /// @param[in] msg エラーメッセージ
  static
  void
  const_with_hname(const char* file,
		   int line,
		   const PtExpr* pt_expr,
		   const char* label,
		   const string& msg);

  /// @brief ElbConstError を送出する．
  /// @param[in] file ファイル名
  /// @param[in] line 行番号
  /// @param[in] pt_expr 式を表すパース木
  /// @param[in] label エラーラベル
  /// @param[in] msg エラーメッセージ
  static
  void
  const_common(const char* file,
	       int line,
	       const PtExpr* pt_expr,
	       const char* label,
	       const string& msg);

  /// @brief 階層名付きのエラーメッセージを生成する共通部分
  /// @param[in] file ファイル名
  /// @param[in] line 行番号
  /// @param[in] pt_expr 式を表すパース木
  /// @param[in] label エラーラベル
  /// @param[in] msg エラーメッセージ
  static
  void
  expr_with_hname(const char* file,
		  int line,
		  const PtExpr* pt_expr,
		  const char* label,
		  const string& msg);

  /// @brief PtExpr に関するエラーメッセージを生成する共通部分
  /// @param[in] file ファイル名
  /// @param[in] line 行番号
  /// @param[in] loc ファイル位置
  /// @param[in] label エラーラベル
  /// @param[in] msg エラーメッセージ
  static
  void
  expr_common(const char* file,
	      int line,
	      const PtExpr* pt_expr,
	      const char* label,
	      const string& msg);

  /// @brief エラーメッセージを生成する共通部分
  /// @param[in] file ファイル名
  /// @param[in] line 行番号
  /// @param[in] pt_expr 式を表すパース木
  /// @param[in] label エラーラベル
  /// @param[in] msg エラーメッセージ
  static
  void
  error_common(const char* file,
	       int line,
	       const FileRegion& loc,
	       const char* label,
	       const string& msg);

  /// @brief 最初に識別名を冠したメッセージを作る．
  /// @param[in] pt_expr 式を表すパース木
  /// @param[in] msg メッセージ
  static
  string
  make_message(const PtExpr* pt_expr,
	       const string& msg);

};

END_NAMESPACE_YM_VERILOG

#endif // ERRORGEN_H
