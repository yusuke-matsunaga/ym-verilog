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
  ExprEval(
    Elaborator& elab, ///< [in] 生成器
    ElbMgr& elb_mgr   ///< [in] Elbオブジェクトを管理するクラス
  );

  /// @brief デストラクタ
  ~ExprEval();


public:
  //////////////////////////////////////////////////////////////////////
  // ExprEval の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 定数式の値を評価する．
  /// @return 評価した値を返す．
  ///
  /// * 定数式でなければ EvalConstError 例外を送出する．
  VlValue
  evaluate_expr(
    const VlScope* parent, ///< [in] 親のスコープ
    const PtExpr* pt_expr  ///< [in] 式を表すパース木
  );

  /// @brief 定数式を評価し int 値を返す．
  /// @return 評価した値を返す．
  ///
  /// * 定数式でなければ EvalConstError 例外を送出する．
  /// * 評価結果が int でなければ EvalIntError 例外を送出する．
  int
  evaluate_int(
    const VlScope* parent, ///< [in] 親のスコープ
    const PtExpr* pt_expr  ///< [in] 式を表すパース木
  );

  /// @brief 定数式ならばを評価し int 値を返す．
  /// @return 評価した値を返す．
  ///
  /// * 評価結果が int でなければ EvalIntError 例外を送出する．
  int
  evaluate_int_if_const(
    const VlScope* parent, ///< [in] 親のスコープ
    const PtExpr* pt_expr, ///< [in] 式を表すパース木
    bool& is_const         ///< [out] 定数式の時に true を返す．
  );

  /// @brief 定数式を評価しスカラー値を返す．
  /// @return 評価した値を返す．
  ///
  /// * 定数式でなければ EvalConstError 例外を送出する．
  /// * いかなる型でもスカラー値に変換可能
  VlScalarVal
  evaluate_scalar(
    const VlScope* parent, ///< [in] 親のスコープ
    const PtExpr* pt_expr  ///< [in] 式を表すパース木
  );

  /// @brief 定数式を評価し bool 値を返す．
  /// @return 評価した値を返す．
  ///
  /// * 定数式でなければ EvalConstError 例外を送出する．
  /// * いかなる型でも bool 値に変換可能
  bool
  evaluate_bool(
    const VlScope* parent, ///< [in] 親のスコープ
    const PtExpr* pt_expr  ///< [in] 式を表すパース木
  );

  /// @brief 定数式を評価しビットベクタ値を返す．
  /// @return 評価した値を返す．
  ///
  /// * 定数式でなければ EvalConstError 例外を送出する．
  /// * 評価結果がビットベクタ型でなければ EvalError 例外を送出する．
  BitVector
  evaluate_bitvector(
    const VlScope* parent, ///< [in] 親のスコープ
    const PtExpr* pt_expr  ///< [in] 式を表すパース木
  );

  /// @brief 範囲を表す式を評価する．
  /// @return 範囲の MSB と LSB の値のペアを返す．
  ///
  /// * 定数式でなければ EvalConstError 例外を送出する．
  /// * 評価結果が int でなければ EvalIntError 例外を送出する．
  pair<int, int>
  evaluate_range(
    const VlScope* parent, ///< [in] 親のスコープ
    const PtExpr* pt_left, ///< [in] 範囲のMSBを表すパース木
    const PtExpr* pt_right ///< [in] 範囲のLSBを表すパース木
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 演算子に対して式の値を評価する．
  VlValue
  evaluate_opr(
    const VlScope* parent, ///< [in] 親のスコープ
    const PtExpr* pt_expr  ///< [in] 式を表すパース木
  );

  /// @brief 定数に対して式の値を評価する．
  VlValue
  evaluate_const(
    const VlScope* parent, ///< [in] 親のスコープ
    const PtExpr* pt_expr  ///< [in] 式を表すパース木
  );

  /// @brief 関数呼び出しに対して式の値を評価する．
  VlValue
  evaluate_funccall(
    const VlScope* parent, ///< [in] 親のスコープ
    const PtExpr* pt_expr  ///< [in] 式を表すパース木
  );

  /// @brief プライマリに対して式の値を評価する．
  VlValue
  evaluate_primary(
    const VlScope* parent, ///< [in] 親のスコープ
    const PtExpr* pt_expr  ///< [in] 式を表すパース木
  );

};

END_NAMESPACE_YM_VERILOG

#endif // EXPREVAL_H
