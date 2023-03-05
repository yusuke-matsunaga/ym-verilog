#ifndef EXPRGEN_H
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
  ExprGen(
    Elaborator& elab, ///< [in] 生成器
    ElbMgr& elb_mgr   ///< [in] Elbオブジェクトを管理するクラス
  );

  /// @brief デストラクタ
  ~ExprGen();


public:
  //////////////////////////////////////////////////////////////////////
  // ExprGen の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief PtExpr から通常の ElbiExpr を生成する
  /// @return 生成された ElbExpr のポインタを返す．
  ///
  /// 不適切な式ならば ElbError 例外を送出する．
  ElbExpr*
  instantiate_expr(
    const VlScope* parent, ///< [in] 親のスコープ
    const ElbEnv& env,     ///< [in] 生成時の環境
    const PtExpr* pt_expr  ///< [in] 式を表すパース木
  );

  /// @brief PtExpr から定数式の ElbExpr を生成する
  /// @return 生成された ElbExpr のポインタを返す．
  ///
  /// 不適切な式ならば ElbError 例外を送出する．
  ElbExpr*
  instantiate_constant_expr(
    const VlScope* parent, ///< [in] 親のスコープ
    const PtExpr* pt_expr  ///< [in] 式を表すパース木
  );

  /// @brief PtExpr からイベント式の ElbiExpr を生成する
  /// @return 生成された ElbExpr のポインタを返す．
  ///
  /// 不適切な式ならば ElbError 例外を送出する．
  ElbExpr*
  instantiate_event_expr(
    const VlScope* parent, ///< [in] 親のスコープ
    const ElbEnv& env,     ///< [in] 生成時の環境
    const PtExpr* pt_expr  ///< [in] 式を表すパース木
  );

  /// @brief PtExpr からシステム関数の引数を生成する．
  /// @return 生成された ElbExpr のポインタを返す．
  ///
  /// 不適切な式ならば ElbError 例外を送出する．
  ElbExpr*
  instantiate_arg(
    const VlScope* parent, ///< [in] 親のスコープ
    const ElbEnv& env,	   ///< [in] 生成時の環境
    const PtExpr* pt_expr  ///< [in] 式を表すパース木
  );

  /// @brief PtExpr から左辺式を生成する
  /// @return 生成された ElbExpr のポインタを返す．
  ///
  /// 不適切な式ならば ElbError 例外を送出する．
  ElbExpr*
  instantiate_lhs(
    const VlScope* parent, ///< [in] 親のスコープ
    const ElbEnv& env,	   ///< [in] 生成時の環境
    const PtExpr* pt_expr  ///< [in] 式を表すパース木
  );

  /// @brief PtExpr(primary) から named_event を生成する．
  ///
  /// 不適切な式ならば ElbError 例外を送出する．
  ElbExpr*
  instantiate_namedevent(
    const VlScope* parent, ///< [in] 親のスコープ
    const PtExpr* pt_expr  ///< [in] 式を表すパース木
  );

  /// @brief PtDelay から ElbExpr を生成する．
  ///
  /// 不適切な式ならば内部でエラーメッセージを出力して nullptr を返す．
  /// 例外は送出しない
  const VlDelay*
  instantiate_delay(
    const VlScope* parent,  ///< [in] 親のスコープ
    const PtDelay* pt_delay ///< [in] 遅延を表すパース木
  );

  /// @brief PtOrderedCon から ElbExpr を生成する．
  ///
  /// これは PtInst の前にある # つきの式がパラメータ割り当てなのか
  /// 遅延なのかわからないので PtOrderedCon で表していることによる．
  ///
  /// 不適切な式ならば内部でエラーメッセージを出力して nullptr を返す．
  /// 例外は送出しない
  const VlDelay*
  instantiate_delay(
    const VlScope* parent, ///< [in] 親のスコープ
    const PtItem* pt_head  ///< [in] 順序付き割り当て式
  );

  /// @brief instantiate_delay の下請け関数
  ///
  /// * pt_obj は PtDelay か PtItem のどちらか
  /// * 配列の要素数は最大で 3
  ///
  /// 不適切な式ならば内部でエラーメッセージを出力して nullptr を返す．
  const VlDelay*
  instantiate_delay_sub(
    const VlScope* parent,                     ///< [in] 親のスコープ
    const PtBase* pt_obj,                      ///< [in] 遅延式を表すパース木
    const vector<const PtExpr*>& pt_expr_array ///< [in] 遅延式の配列
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief PtExpr から左辺式を生成する
  /// @return 生成した式を返す．
  ///
  /// 不適切な式ならば ElbError 例外を送出する．
  ElbExpr*
  instantiate_lhs_sub(
    const VlScope* parent,       ///< [in] 親のスコープ
    const ElbEnv& env,           ///< [in] 生成時の環境
    const PtExpr* pt_expr,       ///< [in] 式を表すパース木
    vector<ElbExpr*>& elem_array ///< [out] 生成した左辺式の要素を格納するベクタ
  );

  /// @brief PtPrimary から ElbExpr を生成する．
  /// @return 生成された式を返す．
  ///
  /// 不適切な式ならば ElbError 例外を送出する．
  ElbExpr*
  instantiate_primary(
    const VlScope* parent, ///< [in] 親のスコープ
    const ElbEnv& env,	   ///< [in] 生成時の環境
    const PtExpr* pt_expr  ///< [in] 式を表すパース木
  );

  /// @brief PtOpr から ElbExpr を生成する．
  /// @return 生成された式を返す．
  ///
  /// 不適切な式ならば ElbError 例外を送出する．
  ElbExpr*
  instantiate_opr(
    const VlScope* parent, ///< [in] 親のスコープ
    const ElbEnv& env,	   ///< [in] 生成時の環境
    const PtExpr* pt_expr  ///< [in] 式を表すパース木
  );

  /// @brief PtFuncCall から ElbExpr を生成する．
  /// @return 生成された式を返す．
  ///
  /// 不適切な式ならば ElbError 例外を送出する．
  ElbExpr*
  instantiate_funccall(
    const VlScope* parent, ///< [in] 親のスコープ
    const ElbEnv& env,	   ///< [in] 生成時の環境
    const PtExpr* pt_expr  ///< [in] 式を表すパース木
  );


  /// @brief PtSysFuncCall から引数を生成する．
  /// @return 生成された式を返す．
  ///
  /// 不適切な式ならば ElbError 例外を送出する．
  ElbExpr*
  instantiate_sysfunccall(
    const VlScope* parent, ///< [in] 親のスコープ
    const ElbEnv& env,	   ///< [in] 生成時の環境
    const PtExpr* pt_expr  ///< [in] 式を表すパース木
  );

  /// @brief 定数識別子を探す．
  ObjHandle*
  find_const_handle(
    const VlScope* parent, ///< [in] 親のスコープ
    const PtExpr* pt_expr  ///< [in] 式を表すパース木
  );

  /// @brief genvar に対応した定数を生成する．
  ///
  /// pt_expr に添字が付いていたらエラーとなる．
  ElbExpr*
  instantiate_genvar(
    const VlScope* parent, ///< [in] 親のスコープ
    const PtExpr* pt_expr, ///< [in] 式を表すパース木
    int val                ///< [in] 値
  );

  /// @brief 宣言要素のインスタンス化を行う．
  ElbExpr*
  instantiate_primary_sub(
    ObjHandle* handle,      ///< [in] オブジェクトハンドル
    const VlScope* parent,  ///< [in] 親のスコープ
    const ElbEnv& env,      ///< [in] インスタンス化している環境
    const PtExpr* pt_expr,  ///< [in] 式を表すパース木
    bool& is_array,         ///< [out] 対象が配列の時 true を返す．
    bool& has_range_select, ///< [out] 範囲指定を持っていたら true を返す．
    bool& has_bit_select    ///< [out] ビット指定を持っていたら true を返す．
  );

  /// @brief decl の型が適切がチェックする．
  ///
  /// 不適切な場合，例外が送出される．
  void
  check_decl(
    const ElbEnv& env,     ///< [in] インスタンス化している環境
    const PtExpr* pt_expr, ///< [in] 式を表すパース木
    VpiObjType decl_type,  ///< [in] 対象の宣言要素の型
    bool is_array,         ///< [in] 対象が配列の時 true を渡す．
    bool has_select        ///< [in] ビット/範囲指定を持つ時 true を渡す．
  );

};

END_NAMESPACE_YM_VERILOG

#endif // EXPRGEN_H
