
/// @file ElbProxy.cc
/// @brief ElbProxy の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ElbProxy.h"
#include "ElbEnv.h"
#include "ElbError.h"
#include "ModuleGen.h"
#include "DeclGen.h"
#include "ItemGen.h"
#include "StmtGen.h"
#include "ExprGen.h"
#include "ExprEval.h"
#include "AttrGen.h"

#include "elaborator/ElbExpr.h"
#include "parser/PtDumper.h"

#include "ym/pt/PtExpr.h"
#include "ym/MsgMgr.h"


BEGIN_NAMESPACE_YM_VERILOG

// @brief コンストラクタ
// @param[in] elb 生成器
// @param[in] elb_mgr Elbオブジェクトを管理するクラス
ElbProxy::ElbProxy(Elaborator& elab,
		   ElbMgr& elb_mgr) :
  mElaborator{elab},
  mMgr{elb_mgr}
{
}

// @brief デストラクタ
ElbProxy::~ElbProxy()
{
}

// @brief 初期化を行う．
// @param[in] module_gen モジュール生成用のオブジェクト
// @param[in] decl_gen 宣言要素生成用のオブジェクト
// @param[in] item_gen 構成要素生成用のオブジェクト
// @param[in] stmt_gen ステートメント生成用のオブジェクト
// @param[in] expr_gen 式生成用のオブジェクト
// @param[in] expr_eval 定数式評価用のオブジェクト
// @param[in] attr_gen 属性生成用のオブジェクト
void
ElbProxy::init(ModuleGen* module_gen,
	       DeclGen* decl_gen,
	       ItemGen* item_gen,
	       StmtGen* stmt_gen,
	       ExprGen* expr_gen,
	       ExprEval* expr_eval,
	       AttrGen* attr_gen)
{
  mModuleGen = module_gen;
  mDeclGen = decl_gen;
  mItemGen = item_gen;
  mStmtGen = stmt_gen;
  mExprGen = expr_gen;
  mExprEval = expr_eval;
  mAttrGen = attr_gen;
}

// @brief 対応する宣言に範囲があり，IO宣言に範囲がない記述を認めるか
bool
ElbProxy::allow_empty_io_range()
{
  return mElaborator.mAllowEmptyIORange;
}

// @brief module の中身のうちスコープに関係するインスタンス化を行う．
// @param[in] modle モジュール
// @param[in] pt_module モジュール定義
// @param[in] param_con パラメータ割り当ての情報
void
ElbProxy::phase1_module_item(ElbModule* module,
			     const PtModule* pt_module,
			     const vector<ElbParamCon>& param_con_list)
{
  mModuleGen->phase1_module_item(module, pt_module, param_con_list);
}

// @brief parameter と genvar を実体化する．
// @param[in] parent 親のスコープ
// @param[in] pt_head_array 宣言ヘッダの配列
// @param[in] force_to_local true なら parameter を localparam にする．
void
ElbProxy::phase1_decl(const VlScope* parent,
		      const vector<const PtDeclHead*>& pt_head_array,
		      bool force_to_local)
{
  mDeclGen->phase1_decl(parent, pt_head_array, force_to_local);
}

// @brief IO宣言要素を実体化する．
// @param[in] module 親のモジュール
// @param[in] pt_head_array IO宣言ヘッダの配列
void
ElbProxy::instantiate_iodecl(ElbModule* module,
			     const vector<const PtIOHead*>& pt_head_array)
{
  mDeclGen->instantiate_iodecl(module, nullptr, pt_head_array);
}

// @brief IO宣言要素を実体化する．
// @param[in] pt_head_array IO宣言ヘッダの配列
// @param[in] taskfunc 親のタスク/関数
void
ElbProxy::instantiate_iodecl(ElbTaskFunc* taskfunc,
			     const vector<const PtIOHead*>& pt_head_array)
{
  mDeclGen->instantiate_iodecl(nullptr, taskfunc, pt_head_array);
}

// @brief 宣言要素のリストをインスタンス化する．
// @param[in] parent 親のスコープ
// @param[in] pt_head_array 宣言ヘッダの配列
void
ElbProxy::instantiate_decl(const VlScope* parent,
			   const vector<const PtDeclHead*>& pt_head_array)
{
  mDeclGen->instantiate_decl(parent, pt_head_array);
}

// @brief スコープに関係する要素を実体化する．
// @param[in] parent 親のスコープ
// @param[in] pt_item_array 要素定義の配列
void
ElbProxy::phase1_items(const VlScope* parent,
		       const vector<const PtItem*>& pt_item_array)
{
  mItemGen->phase1_items(parent, pt_item_array);
}

// @brief constant function の生成を行う．
// @param[in] parent 親のスコープ
// @param[in] pt_function 関数定義
const VlTaskFunc*
ElbProxy::instantiate_constant_function(const VlScope* parent,
					const PtItem* pt_function)
{
  return mItemGen->instantiate_constant_function(parent, pt_function);
}

// @brief スコープに関係するステートメントの実体化を行う．
// @param[in] parent 親のスコープ
// @param[in] pt_stmt 対象のステートメント
// @param[in] cf constant function 中のステートメントの時 true
void
ElbProxy::phase1_stmt(const VlScope* parent,
		      const PtStmt* pt_stmt,
		      bool cf)
{
  mStmtGen->phase1_stmt(parent, pt_stmt, cf);
}

// @brief ステートメントの実体化を行う．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス
// @param[in] env 生成時の環境
// @param[in] pt_stmt 対象のステートメント
const VlStmt*
ElbProxy::instantiate_stmt(const VlScope* parent,
			   const VlProcess* process,
			   const ElbEnv& env,
			   const PtStmt* pt_stmt)
{
  return mStmtGen->instantiate_stmt(parent, process, env, pt_stmt);
}

// @brief PtExpr から ElbiExpr を生成する
// @param[in] parent 親のスコープ
// @param[in] env 生成時の環境
// @param[in] pt_expr 式を表すパース木
// @return 生成された ElbExpr のポインタを返す．
// @note 不適切な式ならばエラーメッセージを出力し nullptr を返す．
ElbExpr*
ElbProxy::instantiate_expr(const VlScope* parent,
			   const ElbEnv& env,
			   const PtExpr* pt_expr)
{
  return mExprGen->instantiate_expr(parent, env, pt_expr);
}

// @brief PtExpr から定数式の ElbExpr を生成する
// @param[in] parent 親のスコープ
// @param[in] pt_expr 式を表すパース木
// @return 生成された ElbExpr のポインタを返す．
// @note 不適切な式ならばエラーメッセージを出力し nullptr を返す．
ElbExpr*
ElbProxy::instantiate_constant_expr(const VlScope* parent,
				    const PtExpr* pt_expr)
{
  return mExprGen->instantiate_constant_expr(parent, pt_expr);
}

// @brief PtExpr からイベント式の ElbiExpr を生成する
// @param[in] parent 親のスコープ
// @param[in] env 生成時の環境
// @param[in] pt_expr 式を表すパース木
// @return 生成された ElbExpr のポインタを返す．
// @note 不適切な式ならばエラーメッセージを出力し nullptr を返す．
ElbExpr*
ElbProxy::instantiate_event_expr(const VlScope* parent,
				 const ElbEnv& env,
				 const PtExpr* pt_expr)
{
  return mExprGen->instantiate_event_expr(parent, env, pt_expr);
}

// @brief PtExpr からシステム関数の引数を生成する．
// @param[in] parent 親のスコープ
// @param[in] env 生成時の環境
// @param[in] pt_expr 式を表すパース木
// @return 生成された ElbExpr のポインタを返す．
// @note 不適切な式ならばエラーメッセージを出力し nullptr を返す．
ElbExpr*
ElbProxy::instantiate_arg(const VlScope* parent,
			  const ElbEnv& env,
			  const PtExpr* pt_expr)
{
  return mExprGen->instantiate_arg(parent, env, pt_expr);
}

// @brief PtExpr から左辺式を生成する
// @param[in] parent 親のスコープ
// @param[in] env 生成時の環境
// @param[in] pt_expr 式を表すパース木
// @return 生成された ElbExpr のポインタを返す．
// @note 不適切な式ならばエラーメッセージを出力し nullptr を返す．
ElbExpr*
ElbProxy::instantiate_lhs(const VlScope* parent,
			  const ElbEnv& env,
			  const PtExpr* pt_expr)
{
  return mExprGen->instantiate_lhs(parent, env, pt_expr);
}

// @brief PtExpr から右辺式を生成する
// @param[in] parent 親のスコープ
// @param[in] env 生成時の環境
// @param[in] pt_expr 式を表すパース木
// @param[in] lhs 左辺式
// @return 生成された ElbExpr のポインタを返す．
// @note 不適切な式ならばエラーメッセージを出力し nullptr を返す．
ElbExpr*
ElbProxy::instantiate_rhs(const VlScope* parent,
			  const ElbEnv& env,
			  const PtExpr* pt_expr,
			  ElbExpr* lhs)
{
  ElbExpr* expr = mExprGen->instantiate_expr(parent, env, pt_expr);
  if ( expr ) {
    // lhs の型を expr に設定する．
    expr->set_reqsize(lhs->value_type());
  }
  return expr;
}

// @brief PtExpr(primary) から named_event を生成する．
// @param[in] parent 親のスコープ
// @param[in] pt_expr 式を表すパース木
ElbExpr*
ElbProxy::instantiate_namedevent(const VlScope* parent,
				 const PtExpr* pt_expr)
{
  return mExprGen->instantiate_namedevent(parent, pt_expr);
}

// @brief PtDelay から ElbExpr を生成する．
// @param[in] parent 親のスコープ
// @param[in] pt_delay 遅延を表すパース木
const VlDelay*
ElbProxy::instantiate_delay(const VlScope* parent,
			    const PtDelay* pt_delay)
{
  return mExprGen->instantiate_delay(parent, pt_delay);
}

// @brief PtOrderedCon から ElbExpr を生成する．
// @param[in] parent 親のスコープ
// @param[in] pt_head 順序付きわりあて式
// これは PtInst の前にある # つきの式がパラメータ割り当てなのか
// 遅延なのかわからないので PtOrderedCon で表していることによる．
const VlDelay*
ElbProxy::instantiate_delay(const VlScope* parent,
			    const PtItem* pt_head)
{
  return mExprGen->instantiate_delay(parent, pt_head);
}

// @brief 定数式の値を評価する．
// @param[in] parent 親のスコープ
// @param[in] pt_expr 式を表すパース木
// @return 評価した値を返す．
//
// * 定数式でなければ EvalError 例外を送出する．
VlValue
ElbProxy::evaluate_expr(const VlScope* parent,
			const PtExpr* pt_expr)
{
  return mExprEval->evaluate_expr(parent, pt_expr);
}

// @brief 定数式を評価し int 値を返す．
// @param[in] parent 親のスコープ
// @param[in] pt_expr 式を表すパース木
// @return 評価した値を返す．
//
// * 定数式でなければ EvalError 例外を送出する．
// * 評価結果が int でなければ EvalError 例外を送出する．
int
ElbProxy::evaluate_int(const VlScope* parent,
		       const PtExpr* pt_expr)
{
  return mExprEval->evaluate_int(parent, pt_expr);
}

// @brief 定数式ならばを評価し int 値を返す．
// @param[in] parent 親のスコープ
// @param[in] pt_expr 式を表すパース木
// @param[out] is_const 定数式の時に true を返す．
// @return 評価した値を返す．
//
// * 評価結果が int でなければ EvalError 例外を送出する．
int
ElbProxy::evaluate_int_if_const(const VlScope* parent,
				const PtExpr* pt_expr,
				bool& is_const)
{
  return mExprEval->evaluate_int_if_const(parent, pt_expr, is_const);
}

// @brief 定数式を評価しスカラー値を返す．
// @param[in] parent 親のスコープ
// @param[in] pt_expr 式を表すパース木
// @return 評価した値を返す．
//
// * 定数式でなければ EvalError 例外を送出する．
// * いかなる型でもスカラー値に変換可能
VlScalarVal
ElbProxy::evaluate_scalar(const VlScope* parent,
			  const PtExpr* pt_expr)
{
  return mExprEval->evaluate_scalar(parent, pt_expr);
}

// @brief 定数式を評価し bool 値を返す．
// @param[in] parent 親のスコープ
// @param[in] pt_expr 式を表すパース木
// @return 評価した値を返す．
//
// * 定数式でなければ EvalError 例外を送出する．
// * いかなる型でも bool 値に変換可能
bool
ElbProxy::evaluate_bool(const VlScope* parent,
			const PtExpr* pt_expr)
{
  return mExprEval->evaluate_bool(parent, pt_expr);
}

// @brief 定数式を評価しビットベクタ値を返す．
// @param[in] parent 親のスコープ
// @param[in] pt_expr 式を表すパース木
// @return 評価した値を返す．
//
// * 定数式でなければ EvalError 例外を送出する．
// * 評価結果がビットベクタ型でなければ EvalError 例外を送出する．
BitVector
ElbProxy::evaluate_bitvector(const VlScope* parent,
			     const PtExpr* pt_expr)
{
  return mExprEval->evaluate_bitvector(parent, pt_expr);
}

// @brief 範囲を表す式を評価する．
// @param[in] parent 親のスコープ
// @param[in] pt_left 範囲のMSBを表すパース木
// @param[in] pt_right 範囲のLSBを表すパース木
// @param[return] 範囲の MSB と LSB の値のペアを返す．
//
// * 定数式でなければ EvalError 例外を送出する．
// * 評価結果が int でなければ EvalError 例外を送出する．
pair<int, int>
ElbProxy::evaluate_range(const VlScope* parent,
			 const PtExpr* pt_left,
			 const PtExpr* pt_right)
{
  return mExprEval->evaluate_range(parent, pt_left, pt_right);
}

// @brief 構文木要素に対応する属性リストを返す．
// @param[in] pt_obj 元となる構文木要素
const vector<const VlAttribute*>&
ElbProxy::attribute_list(const PtBase* pt_obj)
{
  return mAttrGen->attribute_list(pt_obj);
}

// @brief 構文木要素に対応する属性リストを返す．
// @param[in] pt_obj 元となる構文木要素
vector<const VlAttribute*>
ElbProxy::attribute_list(const PtBase* pt_obj1,
			 const PtBase* pt_obj2)
{
  auto ans{mAttrGen->attribute_list(pt_obj1)};
  const auto& ans1{mAttrGen->attribute_list(pt_obj2)};
  ans.insert(ans.end(), ans1.begin(), ans1.end());
  return ans;
}

// @brief エラーメッセージを出力する．
// @param[in] error エラー情報
void
ElbProxy::put_error(const ElbError& error)
{
  MsgMgr::put_msg(error.file(), error.line(),
		  error.file_region(),
		  MsgType::Error,
		  error.label().c_str(),
		  error.message());
}

// @brief 警告メッセージを出力する．
// @param[in] file ソースファイル名
// @param[in] line ソースファイル上の行番号
// @param[in] loc 警告箇所
// @param[in] label ラベル
// @param[in] msg メッセージ
void
ElbProxy::put_warning(const char* file,
		      int line,
		      const FileRegion& loc,
		      const char* label,
		      const string& msg)

{
  MsgMgr::put_msg(file, line,
		  loc,
		  MsgType::Warning,
		  label,
		  msg);
}

// @brief 情報メッセージを出力する．
// @param[in] file ソースファイル名
// @param[in] line ソースファイル上の行番号
// @param[in] loc 対象の箇所
// @param[in] label ラベル
// @param[in] msg メッセージ
void
ElbProxy::put_info(const char* file,
		   int line,
		   const FileRegion& loc,
		   const char* label,
		   const string& msg)
{
  MsgMgr::put_msg(file, line,
		  loc,
		  MsgType::Info,
		  label,
		  msg);
}

END_NAMESPACE_YM_VERILOG
