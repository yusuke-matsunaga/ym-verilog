#ifndef ELBPROXY_H
#define ELBPROXY_H

/// @file ElbProxy.h
/// @brief ElbProxy のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym/verilog.h"
#include "ym/VlValue.h"
#include "ym/pt/PtP.h"
#include "ym/ClibCellLibrary.h"
#include "ym/ClibCell.h"
#include "elaborator/Elaborator.h"
#include "elaborator/ElbMgr.h"
#include "elaborator/ElbFactory.h"
#include "ElbStub.h"


BEGIN_NAMESPACE_YM_VERILOG

class BitVector;
class ElbEnv;
class ElbError;
class ElbParamCon;
class ElbScope;
class PtiAttrInfo;

//////////////////////////////////////////////////////////////////////
/// @class ElbProxy ElbProxy.h "ElbProxy.h"
/// @brief Elaborator の代理クラス
/// @note このクラスを継承して用いる．
//////////////////////////////////////////////////////////////////////
class ElbProxy
{
protected:

  /// @brief コンストラクタ
  /// @param[in] elab 生成器
  /// @param[in] elb_mgr Elbオブジェクトを管理するクラス
  ElbProxy(Elaborator& elab,
	   ElbMgr& elb_mgr);

  /// @brief デストラクタ
  ~ElbProxy();


public:
  //////////////////////////////////////////////////////////////////////
  // 初期化
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期化を行う．
  /// @param[in] module_gen モジュール生成用のオブジェクト
  /// @param[in] decl_gen 宣言要素生成用のオブジェクト
  /// @param[in] item_gen 構成要素生成用のオブジェクト
  /// @param[in] stmt_gen ステートメント生成用のオブジェクト
  /// @param[in] expr_gen 式生成用のオブジェクト
  /// @param[in] expr_eval 定数式評価用のオブジェクト
  /// @param[in] attr_gen 属性生成用のオブジェクト
  void
  init(ModuleGen* module_gen,
       DeclGen* decl_gen,
       ItemGen* item_gen,
       StmtGen* stmt_gen,
       ExprGen* expr_gen,
       ExprEval* expr_eval,
       AttrGen* attr_gen);


protected:
  //////////////////////////////////////////////////////////////////////
  // コンパイルオプション
  //////////////////////////////////////////////////////////////////////

  /// @brief 対応する宣言に範囲があり，IO宣言に範囲がない記述を認めるか
  bool
  allow_empty_io_range();


protected:
  //////////////////////////////////////////////////////////////////////
  // 検索用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前からモジュール定義を取り出す．
  /// @param[in] name 名前
  /// @return name という名のモジュール定義
  /// @return なければ nullptr を返す．
  const PtModule*
  find_moduledef(const string& name) const;

  /// @brief 関数定義を探す．
  /// @param[in] module 親のモジュール
  /// @param[in] name 関数名
  const PtItem*
  find_funcdef(const VlModule* module,
	       const string& name) const;

  /// @brief constant function を取り出す．
  /// @param[in] parent 検索対象のスコープ
  /// @param[in] name 名前
  /// @return parent というスコープ内の name という関数を返す．
  /// @return なければ nullptr を返す．
  const VlTaskFunc*
  find_constant_function(const VlScope* parent,
			 const string& name) const;

  /// @brief セルの探索
  /// @param[in] name セル名
  /// @return name という名のセルを返す．
  ///
  /// なければ不正値を返す．
  ClibCell
  find_cell(const string& name) const;


protected:
  //////////////////////////////////////////////////////////////////////
  // 要素を登録する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief constant function を登録する．
  /// @param[in] func 関数
  void
  reg_constant_function(const VlTaskFunc* func);


protected:
  //////////////////////////////////////////////////////////////////////
  // elaboration で用いられる下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 後で処理する defparam 文を登録する．
  /// @param[in] header アイテムテンプレートのヘッダ (defparam を持つ)
  void
  add_defparamstub(const VlModule* module,
		   const PtItem* header);

  /// @brief phase1 で行う処理を登録する．
  /// @param[in] stub phase1 で行う処理を表すスタブ
  void
  add_phase1stub(ElbStub* stub);

  /// @brief phase2 で行う処理を登録する．
  /// @param[in] stub phase2 で行う処理を表すスタブ
  void
  add_phase2stub(ElbStub* stub);

  /// @brief phase3 で行う処理を登録する．
  /// @param[in] stub phase3 で行う処理を表すスタブ
  void
  add_phase3stub(ElbStub* stub);

  /// @brief 1引数版の ElbStub を作る．
  template<typename T,
	   typename A>
  ElbStub*
  make_stub(T* obj,
	    void (T::*memfunc)(A),
	    A a);

  /// @brief 2引数版の ElbStub を作る．
  template<typename T,
	   typename A,
	   typename B>
  ElbStub*
  make_stub(T* obj,
	    void (T::*memfunc)(A, B),
	    A a,
	    B b);

  /// @brief 3引数版の ElbStub を作る．
  template<typename T,
	   typename A,
	   typename B,
	   typename C>
  ElbStub*
  make_stub(T* obj,
	    void (T::*memfunc)(A, B, C),
	    A a,
	    B b,
	    C c);

  /// @brief 4引数版の ElbStub を作る．
  template<typename T,
	   typename A,
	   typename B,
	   typename C,
	   typename D>
  ElbStub*
  make_stub(T* obj,
	    void (T::*memfunc)(A, B, C, D),
	    A a,
	    B b,
	    C c,
	    D d);


protected:
  //////////////////////////////////////////////////////////////////////
  // ElbModule の生成関係の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief module の中身のうちスコープに関係するインスタンス化を行う．
  /// @param[in] modle モジュール
  /// @param[in] pt_module モジュール定義
  /// @param[in] param_con パラメータ割り当ての情報
  void
  phase1_module_item(ElbModule* module,
		     const PtModule* pt_module,
		     const vector<ElbParamCon>& param_con_list);


public:
  //////////////////////////////////////////////////////////////////////
  // 宣言要素のインスタンス化関係の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief parameter と genvar を実体化する．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_head_array 宣言ヘッダの配列
  /// @param[in] force_to_local true なら parameter を localparam にする．
  void
  phase1_decl(const VlScope* parent,
	      const vector<const PtDeclHead*>& pt_head_array,
	      bool force_to_local);

  /// @brief IO宣言要素を実体化する．
  /// @param[in] module 親のモジュール
  /// @param[in] pt_head_array IO宣言ヘッダの配列
  void
  instantiate_iodecl(ElbModule* module,
		     const vector<const PtIOHead*>& pt_head_array);

  /// @brief IO宣言要素を実体化する．
  /// @param[in] taskfunc 親のタスク/関数
  /// @param[in] pt_head_array IO宣言ヘッダの配列
  void
  instantiate_iodecl(ElbTaskFunc* taskfunc,
		     const vector<const PtIOHead*>& pt_head_array);

  /// @brief 宣言要素のリストをインスタンス化する．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_head_array 宣言ヘッダの配列
  void
  instantiate_decl(const VlScope* parent,
		   const vector<const PtDeclHead*>& pt_head_array);


protected:
  //////////////////////////////////////////////////////////////////////
  // 構成要素 (module item) のインスタンス化関係の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief スコープに関係する要素を実体化する．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_item_array 要素定義の配列
  void
  phase1_items(const VlScope* parent,
	       const vector<const PtItem*>& pt_item_array);

  /// @brief constant function の生成を行う．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_function 関数定義
  const VlTaskFunc*
  instantiate_constant_function(const VlScope* parent,
				const PtItem* pt_function);


protected:
  //////////////////////////////////////////////////////////////////////
  // ステートメントのインスタンス化関係の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief スコープに関係するステートメントの実体化を行う．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_stmt 対象のステートメント
  /// @param[in] cf constant function 中のステートメントの時 true
  void
  phase1_stmt(const VlScope* parent,
	      const PtStmt* pt_stmt,
	      bool cf = false);

  /// @brief ステートメントの実体化を行う．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス
  /// @param[in] env 生成時の環境
  /// @param[in] pt_stmt 対象のステートメント
  const VlStmt*
  instantiate_stmt(const VlScope* parent,
		   const VlProcess* process,
		   const ElbEnv& env,
		   const PtStmt* pt_stmt);


protected:
  //////////////////////////////////////////////////////////////////////
  // 式のインスタンス化関係の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief PtExpr から ElbExpr を生成する
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

  /// @brief PtExpr からイベント式の ElbExpr を生成する
  /// @param[in] parent 親のスコープ
  /// @param[in] env 生成時の環境
  /// @param[in] pt_expr 式を表すパース木
  /// @return 生成された ElbExpr のポインタを返す．
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

  /// @brief PtExpr から右辺式を生成する
  /// @param[in] parent 親のスコープ
  /// @param[in] env 生成時の環境
  /// @param[in] pt_expr 式を表すパース木
  /// @param[in] lhs 左辺式
  /// @return 生成された ElbExpr のポインタを返す．
  /// @note 不適切な式ならばエラーメッセージを出力し nullptr を返す．
  ElbExpr*
  instantiate_rhs(const VlScope* parent,
		  const ElbEnv& env,
		  const PtExpr* pt_expr,
		  ElbExpr* lhs);

  /// @brief PtExpr(primary) から named_event を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_expr 式を表すパース木
  ElbExpr*
  instantiate_namedevent(const VlScope* parent,
			 const PtExpr* pt_expr);

  /// @brief PtDelay から ElbExpr を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_delay 遅延を表すパース木
  const VlDelay*
  instantiate_delay(const VlScope* parent,
		    const PtDelay* pt_delay);

  /// @brief PtOrderedCon から ElbExpr を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_head 順序付きわりあて式
  /// これは PtInst の前にある # つきの式がパラメータ割り当てなのか
  /// 遅延なのかわからないので PtOrderedCon で表していることによる．
  const VlDelay*
  instantiate_delay(const VlScope* parent,
		    const PtItem* pt_head);

  /// @brief 定数式の値を評価する．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_expr 式を表すパース木
  /// @return 評価した値を返す．
  ///
  /// * 定数式でなければ EvalError 例外を送出する．
  VlValue
  evaluate_expr(const VlScope* parent,
		const PtExpr* pt_expr);

  /// @brief 定数式を評価し int 値を返す．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_expr 式を表すパース木
  /// @return 評価した値を返す．
  ///
  /// * 定数式でなければ EvalError 例外を送出する．
  /// * 評価結果が int でなければ EvalError 例外を送出する．
  int
  evaluate_int(const VlScope* parent,
	       const PtExpr* pt_expr);

  /// @brief 定数式ならばを評価し int 値を返す．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_expr 式を表すパース木
  /// @param[out] is_const 定数式の時に true を返す．
  /// @return 評価した値を返す．
  ///
  /// * 評価結果が int でなければ EvalError 例外を送出する．
  int
  evaluate_int_if_const(const VlScope* parent,
			const PtExpr* pt_expr,
			bool& is_const);

  /// @brief 定数式を評価しスカラー値を返す．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_expr 式を表すパース木
  /// @return 評価した値を返す．
  ///
  /// * 定数式でなければ EvalError 例外を送出する．
  /// * いかなる型でもスカラー値に変換可能
  VlScalarVal
  evaluate_scalar(const VlScope* parent,
		  const PtExpr* pt_expr);

  /// @brief 定数式を評価し bool 値を返す．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_expr 式を表すパース木
  /// @return 評価した値を返す．
  ///
  /// * 定数式でなければ EvalError 例外を送出する．
  /// * いかなる型でも bool 値に変換可能
  bool
  evaluate_bool(const VlScope* parent,
		const PtExpr* pt_expr);

  /// @brief 定数式を評価しビットベクタ値を返す．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_expr 式を表すパース木
  /// @return 評価した値を返す．
  ///
  /// * 定数式でなければ EvalError 例外を送出する．
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
  /// * 定数式でなければ EvalError 例外を送出する．
  /// * 評価結果が int でなければ EvalError 例外を送出する．
  pair<int, int>
  evaluate_range(const VlScope* parent,
		 const PtExpr* pt_left,
		 const PtExpr* pt_right);


protected:
  //////////////////////////////////////////////////////////////////////
  // attribute instance の生成関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 構文木要素に対応する属性リストを返す．
  /// @param[in] pt_obj 元となる構文木要素
  const vector<const VlAttribute*>&
  attribute_list(const PtBase* pt_obj);

  /// @brief 構文木要素に対応する属性リストを返す．
  /// @param[in] pt_obj 元となる構文木要素
  vector<const VlAttribute*>
  attribute_list(const PtBase* pt_obj1,
		 const PtBase* pt_obj2);


protected:
  //////////////////////////////////////////////////////////////////////
  // メッセージ出力
  //////////////////////////////////////////////////////////////////////

  /// @brief エラーメッセージを出力する．
  /// @param[in] error エラー情報
  void
  put_error(const ElbError& error);

  /// @brief 警告メッセージを出力する．
  /// @param[in] file ソースファイル名
  /// @param[in] line ソースファイル上の行番号
  /// @param[in] loc 警告箇所
  /// @param[in] label ラベル
  /// @param[in] msg メッセージ
  void
  put_warning(const char* file,
	      int line,
	      const FileRegion& loc,
	      const char* label,
	      const string& msg);

  /// @brief 情報メッセージを出力する．
  /// @param[in] file ソースファイル名
  /// @param[in] line ソースファイル上の行番号
  /// @param[in] loc 対象の箇所
  /// @param[in] label ラベル
  /// @param[in] msg メッセージ
  void
  put_info(const char* file,
	   int line,
	   const FileRegion& loc,
	   const char* label,
	   const string& msg);


protected:
  //////////////////////////////////////////////////////////////////////
  // メンバに対するアクセス
  //////////////////////////////////////////////////////////////////////

  /// @brief ElbMgr を返す．
  ElbMgr&
  mgr();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 本当の生成器
  Elaborator& mElaborator;

  // 生成したオブジェクトを管理するマネージャクラス
  ElbMgr& mMgr;

  // モジュール生成用のオブジェクト
  ModuleGen* mModuleGen;

  // 宣言要素生成用のオブジェクト
  DeclGen* mDeclGen;

  // 構成要素生成用のオブジェクト
  ItemGen* mItemGen;

  // ステートメント生成用のオブジェクト
  StmtGen* mStmtGen;

  // 式生成用のオブジェクト
  ExprGen* mExprGen;

  // 定数式評価用のオブジェクト
  ExprEval* mExprEval;

  // attribute instance 生成用のオブジェクト
  AttrGen* mAttrGen;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief セルの探索
// @param[in] name セル名
// @return name という名のセル番号を返す．
//
// なければ -1 を返す．
inline
ClibCell
ElbProxy::find_cell(const string& name) const
{
  return mElaborator.find_cell(name);
}

// @brief 名前からモジュール定義を取り出す．
// @param[in] name 名前
// @return name という名のモジュール定義
// @return なければ nullptr を返す．
inline
const PtModule*
ElbProxy::find_moduledef(const string& name) const
{
  return mElaborator.find_moduledef(name);
}

// @brief 関数定義を探す．
// @param[in] module 親のモジュール
// @param[in] name 関数名
inline
const PtItem*
ElbProxy::find_funcdef(const VlModule* module,
		       const string& name) const
{
  return mElaborator.find_funcdef(module, name);
}

// @brief constant function を取り出す．
// @param[in] parent 検索対象のスコープ
// @param[in] name 名前
// @return parent というスコープ内の name という関数を返す．
// @return なければ nullptr を返す．
inline
const VlTaskFunc*
ElbProxy::find_constant_function(const VlScope* parent,
				 const string& name) const
{
  return mElaborator.find_constant_function(parent, name);
}

// @brief constant function を登録する．
// @param[in] func 関数
inline
void
ElbProxy::reg_constant_function(const VlTaskFunc* func)
{
  mElaborator.reg_constant_function(func);
}

// @brief 後で処理する defparam 文を登録する．
// @param[in] module 親のモジュール
// @param[in] pt_head アイテムテンプレートのヘッダ (defparam を持つ)
inline
void
ElbProxy::add_defparamstub(const VlModule* module,
			   const PtItem* pt_head)
{
  mElaborator.add_defparamstub(module, pt_head);
}

// @brief phase1 で行う処理を登録する．
// @param[in] stub phase1 で行う処理を表すスタブ
inline
void
ElbProxy::add_phase1stub(ElbStub* stub)
{
  mElaborator.add_phase1stub(stub);
}

// @brief phase2 で行う処理を登録する．
// @param[in] stub phase2 で行う処理を表すスタブ
inline
void
ElbProxy::add_phase2stub(ElbStub* stub)
{
  mElaborator.add_phase2stub(stub);
}

// @brief phase3 で行う処理を登録する．
// @param[in] stub phase3 で行う処理を表すスタブ
inline
void
ElbProxy::add_phase3stub(ElbStub* stub)
{
  mElaborator.add_phase3stub(stub);
}


//////////////////////////////////////////////////////////////////////
// テンプレート関数の定義
//////////////////////////////////////////////////////////////////////

/// @brief 1引数版の ElbStub を作る．
template<typename T,
	 typename A>
inline
ElbStub*
ElbProxy::make_stub(T* obj,
		    void (T::*memfunc)(A),
		    A a)
{
  return new ElbStubT1<T, A>(obj, memfunc, a);
}

// @brief 2引数版の ElbStub を作る．
template<typename T,
	 typename A,
	 typename B>
inline
ElbStub*
ElbProxy::make_stub(T* obj,
		    void (T::*memfunc)(A, B),
		    A a,
		    B b)
{
  return new ElbStubT2<T, A, B>(obj, memfunc, a, b);
}

// @brief 3引数版の ElbStub を作る．
template<typename T,
	 typename A,
	 typename B,
	 typename C>
inline
ElbStub*
ElbProxy::make_stub(T* obj,
		    void (T::*memfunc)(A, B, C),
		    A a,
		    B b,
		    C c)
{
  return new ElbStubT3<T, A, B, C>(obj, memfunc, a, b, c);
}

// @brief 4引数版の ElbStub を作る．
template<typename T,
	 typename A,
	 typename B,
	 typename C,
	 typename D>
inline
ElbStub*
ElbProxy::make_stub(T* obj,
		    void (T::*memfunc)(A, B, C, D),
		    A a,
		    B b,
		    C c,
		    D d)
{
  return new ElbStubT4<T, A, B, C, D>(obj, memfunc, a, b, c, d);
}

// @brief ElbMgr を返す．
inline
ElbMgr&
ElbProxy::mgr()
{
  return mMgr;
}

END_NAMESPACE_YM_VERILOG

#endif // ELBPROXY_H
