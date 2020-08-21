﻿#ifndef EXPRGEN_H
#define EXPRGEN_H

/// @file ExprGen.h
/// @brief ExprGen のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.


#include "ym/verilog.h"
#include "ym/VlValue.h"
#include "ym/pt/PtP.h"
#include "ElbProxy.h"


BEGIN_NAMESPACE_YM_VERILOG

class ElbEnv;

//////////////////////////////////////////////////////////////////////
/// @class ExprGen ExprGen.h "ExprGen.h"
/// @brief ElbExpr を生成するクラス
//////////////////////////////////////////////////////////////////////
class ExprGen :
  public ElbProxy
{
public:

  /// @brief コンストラクタ
  /// @param[in] elab 生成器
  /// @param[in] elb_mgr Elbオブジェクトを管理するクラス
  ExprGen(Elaborator& elab,
	  ElbMgr& elb_mgr);

  /// @brief デストラクタ
  ~ExprGen();


public:
  //////////////////////////////////////////////////////////////////////
  // ExprGen の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief PtExpr から通常の ElbiExpr を生成する
  /// @param[in] parent 親のスコープ
  /// @param[in] env 生成時の環境
  /// @param[in] pt_expr 式を表すパース木
  /// @return 生成された ElbExpr のポインタを返す．
  /// @note 不適切な式ならばエラーメッセージを出力し nullptr を返す．
  ElbExpr*
  instantiate_expr(const VlScope* parent,
		   const ElbEnv& env,
		   const PtExpr* pt_expr);

  /// @brief PtExpr から定数式の ElbExpr を生成する
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_expr 式を表すパース木
  /// @return 生成された ElbExpr のポインタを返す．
  /// @note 不適切な式ならばエラーメッセージを出力し nullptr を返す．
  ElbExpr*
  instantiate_constant_expr(const VlScope* parent,
			    const PtExpr* pt_expr);

  /// @brief PtExpr からイベント式の ElbiExpr を生成する
  /// @param[in] parent 親のスコープ
  /// @param[in] env 生成時の環境
  /// @param[in] pt_expr 式を表すパース木
  /// @note 不適切な式ならばエラーメッセージを出力し nullptr を返す．
  ElbExpr*
  instantiate_event_expr(const VlScope* parent,
			 const ElbEnv& env,
			 const PtExpr* pt_expr);

  /// @brief PtExpr からシステム関数の引数を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] env 生成時の環境
  /// @param[in] pt_expr 式を表すパース木
  /// @return 生成された ElbExpr のポインタを返す．
  /// @note 不適切な式ならばエラーメッセージを出力し nullptr を返す．
  ElbExpr*
  instantiate_arg(const VlScope* parent,
		  const ElbEnv& env,
		  const PtExpr* pt_expr);

  /// @brief PtExpr から左辺式を生成する
  /// @param[in] parent 親のスコープ
  /// @param[in] env 生成時の環境
  /// @param[in] pt_expr 式を表すパース木
  /// @return 生成された ElbExpr のポインタを返す．
  /// @note 不適切な式ならばエラーメッセージを出力し nullptr を返す．
  ElbExpr*
  instantiate_lhs(const VlScope* parent,
		  const ElbEnv& env,
		  const PtExpr* pt_expr);

  /// @brief PtExpr(primary) から named_event を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_expr 式を表すパース木
  ElbExpr*
  instantiate_namedevent(const VlScope* parent,
			 const PtExpr* pt_expr);

  /// @brief 式の値を評価する．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_expr 式を表すパース木
  /// @param[in] put_error エラーを出力する時，true にする．
  VlValue
  evaluate_expr(const VlScope* parent,
		const PtExpr* pt_expr,
		bool put_error);

  /// @brief PtExpr を評価し int 値を返す．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_expr 式を表すパース木
  /// @param[out] value 評価値を格納する変数
  /// @param[in] put_error エラーを出力する時，true にする．
  /// @note 定数でなければエラーメッセージを出力し false を返す．
  bool
  evaluate_int(const VlScope* parent,
	       const PtExpr* pt_expr,
	       int& value,
	       bool put_error);

  /// @brief PtExpr を評価しスカラー値を返す．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_expr 式を表すパース木
  /// @param[out] value 評価値を格納する変数
  /// @param[in] put_error エラーを出力する時，true にする．
  /// @note 定数でなければエラーメッセージを出力し false を返す．
  bool
  evaluate_scalar(const VlScope* parent,
		  const PtExpr* pt_expr,
		  VlScalarVal& value,
		  bool put_error);

  /// @brief PtExpr を評価し bool 値を返す．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_expr 式を表すパース木
  /// @param[out] value 評価値を格納する変数
  /// @param[in] put_error エラーを出力する時，true にする．
  /// @note 定数でなければエラーメッセージを出力し false を返す．
  bool
  evaluate_bool(const VlScope* parent,
		const PtExpr* pt_expr,
		bool& value,
		bool put_error);

  /// @brief PtExpr を評価しビットベクタ値を返す．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_expr 式を表すパース木
  /// @param[out] value 評価値を格納する変数
  /// @param[in] put_error エラーを出力する時，true にする．
  /// @note 定数でなければエラーメッセージを出力し false を返す．
  bool
  evaluate_bitvector(const VlScope* parent,
		     const PtExpr* pt_expr,
		     BitVector& value,
		     bool put_error);

  /// @brief PtDelay から ElbExpr を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_delay 遅延を表すパース木
  const VlDelay*
  instantiate_delay(const VlScope* parent,
		    const PtDelay* pt_delay);

  /// @brief PtOrderedCon から ElbExpr を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_head 順序付き割り当て式
  /// これは PtInst の前にある # つきの式がパラメータ割り当てなのか
  /// 遅延なのかわからないので PtOrderedCon で表していることによる．
  const VlDelay*
  instantiate_delay(const VlScope* parent,
		    const PtItem* pt_head);

  /// @brief instantiate_delay の下請け関数
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_obj 遅延式を表すパース木
  /// @param[in] n 要素数
  /// @param[in] expr_array 遅延式の配列
  /// @note pt_obj は PtDelay か PtItem のどちらか
  /// @note n は最大で 3
  const VlDelay*
  instantiate_delay_sub(const VlScope* parent,
			const PtBase* pt_obj,
			SizeType n,
			const PtExpr* expr_array[]);


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief PtExpr から左辺式を生成する
  /// @param[in] parent 親のスコープ
  /// @param[in] env 生成時の環境
  /// @param[in] pt_expr 式を表すパース木
  /// @param[out] elem_array 生成した左辺式の要素を格納するベクタ
  /// @return 生成した式を返す．
  /// @note 不適切な式ならばエラーメッセージを出力し nullptr を返す．
  ElbExpr*
  instantiate_lhs_sub(const VlScope* parent,
		      const ElbEnv& env,
		      const PtExpr* pt_expr,
		      vector<ElbExpr*>& elem_array);

  /// @brief PtPrimary から ElbExpr を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] env 生成時の環境
  /// @param[in] pt_expr 式を表すパース木
  /// @return 生成された式を返す．
  /// @note エラーが起きたらエラーメッセージを出力し，nullptr を返す．
  ElbExpr*
  instantiate_primary(const VlScope* parent,
		      const ElbEnv& env,
		      const PtExpr* pt_expr);

  /// @brief PtOpr から ElbExpr を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] env 生成時の環境
  /// @param[in] pt_expr 式を表すパース木
  /// @return 生成された式を返す．
  /// @note エラーが起きたらエラーメッセージを出力し，nullptr を返す．
  ElbExpr*
  instantiate_opr(const VlScope* parent,
		  const ElbEnv& env,
		  const PtExpr* pt_expr);

  /// @brief PtFuncCall から ElbExpr を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] env 生成時の環境
  /// @param[in] pt_expr 式を表すパース木
  /// @return 生成された式を返す．
  /// @note エラーが起きたらエラーメッセージを出力し，nullptr を返す．
  ElbExpr*
  instantiate_funccall(const VlScope* parent,
		       const ElbEnv& env,
		       const PtExpr* pt_expr);

  /// @brief PtSysFuncCall から引数を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] env 生成時の環境
  /// @param[in] pt_expr 式を表すパース木
  /// @return 生成された式を返す．
  /// @note エラーが起きたらエラーメッセージを出力し，nullptr を返す．
  ElbExpr*
  instantiate_sysfunccall(const VlScope* parent,
			  const ElbEnv& env,
			  const PtExpr* pt_expr);

  /// @brief 定数識別子を探す．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_expr 式を表すパース木
  ObjHandle*
  find_const_handle(const VlScope* parent,
		    const PtExpr* pt_expr);

  /// @brief genvar に対応した定数を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_expr 式を表すパース木
  /// @param[in] val 値
  /// @note pt_expr に添字が付いていたらエラーとなる．
  ElbExpr*
  instantiate_genvar(const VlScope* parent,
		     const PtExpr* pt_expr,
		     int val);

  /// @brief 宣言要素のインスタンス化を行う．
  /// @param[in] handle オブジェクトハンドル
  /// @param[in] parent 親のスコープ
  /// @param[in] env インスタンス化している環境
  /// @param[in] pt_expr 式を表すパース木
  /// @param[out] is_array 対象が配列の時 true を返す．
  /// @param[out] has_range_select 範囲指定を持っていたら true を返す．
  /// @param[out] has_bit_select ビット指定を持っていたら true を返す．
  ElbExpr*
  instantiate_primary_sub(ObjHandle* handle,
			  const VlScope* parent,
			  const ElbEnv& env,
			  const PtExpr* pt_expr,
			  bool& is_array,
			  bool& has_range_select,
			  bool& has_bit_select);

  /// @brief decl の型が適切がチェックする．
  /// @param[in] env インスタンス化している環境
  /// @param[in] pt_expr 式を表すパース木
  /// @param[in] decl_type 対象の宣言要素の型
  /// @param[in] is_array 対象が配列の時 true を渡す．
  /// @param[in] has_select ビット/範囲指定を持つ時 true を渡す．
  bool
  check_decl(const ElbEnv& env,
	     const PtExpr* pt_expr,
	     VpiObjType decl_type,
	     bool is_array,
	     bool has_select);

  /// @brief 演算子に対して式の値を評価する．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_expr 式を表すパース木
  /// @param[in] put_error エラーを出力する時，true にする．
  VlValue
  evaluate_opr(const VlScope* parent,
	       const PtExpr* pt_expr,
	       bool put_error);

  /// @brief 定数に対して式の値を評価する．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_expr 式を表すパース木
  VlValue
  evaluate_const(const VlScope* parent,
		 const PtExpr* pt_expr);

  /// @brief 関数呼び出しに対して式の値を評価する．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_expr 式を表すパース木
  /// @param[in] put_error エラーを出力する時，true にする．
  VlValue
  evaluate_funccall(const VlScope* parent,
		    const PtExpr* pt_expr,
		    bool put_error);

  /// @brief プライマリに対して式の値を評価する．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_expr 式を表すパース木
  /// @param[in] put_error エラーを出力する時，true にする．
  VlValue
  evaluate_primary(const VlScope* parent,
		   const PtExpr* pt_expr,
		   bool put_error);


private:
  //////////////////////////////////////////////////////////////////////
  // エラーメッセージ生成用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief constant function 中にシステム関数呼び出し
  /// @param[in] pt_expr 式を表すパース木
  void
  error_illegal_sysfunccall_in_cf(const PtExpr* pt_expr);

  /// @brief constant expression 中にシステム関数呼び出し
  /// @param[in] pt_expr 式を表すパース木
  void
  error_illegal_sysfunccall_in_ce(const PtExpr* pt_expr);

  /// @brief イベント式の根元に定数
  /// @param[in] pt_expr 式を表すパース木
  void
  error_illegal_constant_in_event_expression(const PtExpr* pt_expr);

  /// @brief イベント式の根元に関数呼び出し
  /// @param[in] pt_expr 式を表すパース木
  void
  error_illegal_funccall_in_event_expression(const PtExpr* pt_expr);

  /// @brief イベント式の根元にシステム関数呼び出し
  /// @param[in] pt_expr 式を表すパース木
  void
  error_illegal_sysfunccall_in_event_expression(const PtExpr* pt_expr);

  /// @brief 左辺式で用いることのできない演算子
  /// @param[in] pt_expr 式を表すパース木
  void
  error_illegal_operator_in_lhs(const PtExpr* pt_expr);

  /// @brief 左辺式に定数
  /// @param[in] pt_expr 式を表すパース木
  void
  error_illegal_constant_in_lhs(const PtExpr* pt_expr);

  /// @brief 左辺式に関数呼び出し
  /// @param[in] pt_expr 式を表すパース木
  void
  error_illegal_funccall_in_lhs(const PtExpr* pt_expr);

  /// @brief 左辺式にシステム関数呼び出し
  /// @param[in] pt_expr 式を表すパース木
  void
  error_illegal_sysfunccall_in_lhs(const PtExpr* pt_expr);

  /// @brief 通常の式中に edge descriptor
  /// @param[in] pt_expr 式を表すパース木
  void
  error_illegal_edge_descriptor(const PtExpr* pt_expr);

  /// @brief real 型のオペランドをとれない
  /// @param[in] pt_expr 式を表すパース木
  void
  error_illegal_real_type(const PtExpr* pt_expr);

  /// @brief 該当する関数が存在しない．
  /// @param[in] pt_expr 式を表すパース木
  void
  error_no_such_function(const PtExpr* pt_expr);

  /// @brief 該当するシステム関数が存在しない．
  /// @param[in] pt_expr 式を表すパース木
  void
  error_no_such_sysfunction(const PtExpr* pt_expr);

  /// @brief 定数関数は自己再帰できない．
  /// @param[in] pt_expr 式を表すパース木
  void
  error_uses_itself(const PtExpr* pt_expr);

  /// @brief 定数関数ではない．
  /// @param[in] pt_expr 式を表すパース木
  void
  error_not_a_constant_function(const PtExpr* pt_expr);

  /// @brief 関数ではない．
  /// @param[in] pt_expr 式を表すパース木
  void
  error_not_a_function(const PtExpr* pt_expr);

  /// @brief 引数の数が合わない．
  /// @param[in] pt_expr 式を表すパース木
  void
  error_n_of_arguments_mismatch(const PtExpr* pt_expr);

  /// @brief 引数の型が合わない．
  /// @param[in] pt_expr 式を表すパース木
  void
  error_illegal_argument_type(const PtExpr* pt_expr);

  /// @brief オブジェクトが存在しない
  /// @param[in] pt_expr 式を表すパース木
  void
  error_not_found(const PtExpr* pt_expr);

  /// @brief オブジェクトの型が不適切
  /// @param[in] pt_expr 式を表すパース木
  void
  error_illegal_object(const PtExpr* pt_expr);

  /// @brief オブジェクトの型が constant function 用として不適切
  /// @param[in] pt_expr 式を表すパース木
  void
  error_illegal_object_cf(const PtExpr* pt_expr);

  /// @brief 階層名が constant expression 中にあった
  /// @param[in] pt_expr 式を表すパース木
  void
  error_hname_in_ce(const PtExpr* pt_expr);

  /// @brief 階層名が constant function 中にあった
  /// @param[in] pt_expr 式を表すパース木
  void
  error_hname_in_cf(const PtExpr* pt_expr);

  /// @brief オブジェクトが parameter でなかった
  /// @param[in] pt_expr 式を表すパース木
  void
  error_not_a_parameter(const PtExpr* pt_expr);

  /// @brief オブジェクトが named-event でなかった
  /// @param[in] pt_expr 式を表すパース木
  void
  error_not_a_namedevent(const PtExpr* pt_expr);

  /// @brief 要素の範囲の順番と範囲指定の順番が異なる．
  void
  error_range_order(const PtExpr* pt_expr);

  /// @brief named-event に対する範囲指定
  void
  error_select_for_namedevent(const PtExpr* pt_expr);

  /// @brief assign/deassign に不適切なビット/範囲指定
  /// @param[in] pt_expr 式を表すパース木
  void
  error_select_in_pca(const PtExpr* pt_expr);

  /// @brief force/release に不適切なビット/範囲指定
  /// @param[in] pt_expr 式を表すパース木
  void
  error_select_in_force(const PtExpr* pt_expr);

  /// @brief assign/deassign に不適切な配列要素
  /// @param[in] pt_expr 式を表すパース木
  void
  error_array_in_pca(const PtExpr* pt_expr);

  /// @brief force/release に不適切な配列要素
  /// @param[in] pt_expr 式を表すパース木
  void
  error_array_in_force(const PtExpr* pt_expr);

  /// @brief 配列の次元が合わない
  /// @param[in] pt_expr 式を表すパース木
  void
  error_dimension_mismatch(const PtExpr* pt_expr);

  /// @brief real 型に対するビット選択あるいは部分選択があった
  /// @param[in] pt_expr 式を表すパース木
  void
  error_select_for_real(const PtExpr* pt_expr);

  /// @brief 階層名付きのエラーメッセージを生成する共通部分
  /// @param[in] pt_expr 式を表すパース木
  /// @param[in] label エラーラベル
  /// @param[in] msg エラーメッセージ
  void
  error_with_hname(const PtExpr* pt_expr,
		   const char* label,
		   const char* msg);

  /// @brief エラーメッセージを生成する共通部分
  /// @param[in] pt_expr 式を表すパース木
  /// @param[in] label エラーラベル
  /// @param[in] msg エラーメッセージ
  void
  error_common(const PtExpr* pt_expr,
	       const char* label,
	       const char* msg);

};

END_NAMESPACE_YM_VERILOG

#endif // EXPRGEN_H
