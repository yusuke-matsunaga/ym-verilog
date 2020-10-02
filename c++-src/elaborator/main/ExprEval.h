#ifndef EXPREVAL_H
#define EXPREVAL_H

/// @file ExprEval.h
/// @brief ExprEval のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.


#include "ym/verilog.h"
#include "ym/VlValue.h"
#include "ym/pt/PtP.h"
#include "ElbProxy.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ExprEval ExprEval.h "ExprEval.h"
/// @brief 定数式を表す PtExpr の評価を行うクラス
//////////////////////////////////////////////////////////////////////
class ExprEval :
  public ElbProxy
{
public:

  /// @brief コンストラクタ
  /// @param[in] elab 生成器
  /// @param[in] elb_mgr Elbオブジェクトを管理するクラス
  ExprEval(Elaborator& elab,
	   ElbMgr& elb_mgr);

  /// @brief デストラクタ
  ~ExprEval();


public:
  //////////////////////////////////////////////////////////////////////
  // ExprEval の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 定数式の値を評価する．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_expr 式を表すパース木
  /// @return 評価した値を返す．
  ///
  /// * 定数式でなければ EvalConstError 例外を送出する．
  VlValue
  evaluate_expr(const VlScope* parent,
		const PtExpr* pt_expr);

  /// @brief 定数式を評価し int 値を返す．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_expr 式を表すパース木
  /// @return 評価した値を返す．
  ///
  /// * 定数式でなければ EvalConstError 例外を送出する．
  /// * 評価結果が int でなければ EvalIntError 例外を送出する．
  int
  evaluate_int(const VlScope* parent,
	       const PtExpr* pt_expr);

  /// @brief 定数式ならばを評価し int 値を返す．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_expr 式を表すパース木
  /// @param[out] is_const 定数式の時に true を返す．
  /// @return 評価した値を返す．
  ///
  /// * 評価結果が int でなければ EvalIntError 例外を送出する．
  int
  evaluate_int_if_const(const VlScope* parent,
			const PtExpr* pt_expr,
			bool& is_const);

  /// @brief 定数式を評価しスカラー値を返す．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_expr 式を表すパース木
  /// @return 評価した値を返す．
  ///
  /// * 定数式でなければ EvalConstError 例外を送出する．
  /// * いかなる型でもスカラー値に変換可能
  VlScalarVal
  evaluate_scalar(const VlScope* parent,
		  const PtExpr* pt_expr);

  /// @brief 定数式を評価し bool 値を返す．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_expr 式を表すパース木
  /// @return 評価した値を返す．
  ///
  /// * 定数式でなければ EvalConstError 例外を送出する．
  /// * いかなる型でも bool 値に変換可能
  bool
  evaluate_bool(const VlScope* parent,
		const PtExpr* pt_expr);

  /// @brief 定数式を評価しビットベクタ値を返す．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_expr 式を表すパース木
  /// @return 評価した値を返す．
  ///
  /// * 定数式でなければ EvalConstError 例外を送出する．
  /// * 評価結果がビットベクタ型でなければ EvalError 例外を送出する．
  BitVector
  evaluate_bitvector(const VlScope* parent,
		     const PtExpr* pt_expr);

  /// @brief 範囲を表す式を評価する．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_left 範囲のMSBを表すパース木
  /// @param[in] pt_right 範囲のLSBを表すパース木
  /// @param[return] 範囲の MSB と LSB の値のペアを返す．
  ///
  /// * 定数式でなければ EvalConstError 例外を送出する．
  /// * 評価結果が int でなければ EvalIntError 例外を送出する．
  pair<int, int>
  evaluate_range(const VlScope* parent,
		 const PtExpr* pt_left,
		 const PtExpr* pt_right);


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 演算子に対して式の値を評価する．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_expr 式を表すパース木
  VlValue
  evaluate_opr(const VlScope* parent,
	       const PtExpr* pt_expr);

  /// @brief 定数に対して式の値を評価する．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_expr 式を表すパース木
  VlValue
  evaluate_const(const VlScope* parent,
		 const PtExpr* pt_expr);

  /// @brief 関数呼び出しに対して式の値を評価する．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_expr 式を表すパース木
  VlValue
  evaluate_funccall(const VlScope* parent,
		    const PtExpr* pt_expr);

  /// @brief プライマリに対して式の値を評価する．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_expr 式を表すパース木
  VlValue
  evaluate_primary(const VlScope* parent,
		   const PtExpr* pt_expr);

};

END_NAMESPACE_YM_VERILOG

#endif // EXPREVAL_H
