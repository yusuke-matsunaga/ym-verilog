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
#include "elaborator/Elaborator.h"
#include "elaborator/ElbMgr.h"
#include "elaborator/ElbFactory.h"
#include "ElbStub.h"


BEGIN_NAMESPACE_YM_VERILOG

class BitVector;
class ElbEnv;
class ElbParamCon;
class ElbScope;

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
  /// @param[in] attr_gen 属性生成用のオブジェクト
  void
  init(ModuleGen* module_gen,
       DeclGen* decl_gen,
       ItemGen* item_gen,
       StmtGen* stmt_gen,
       ExprGen* expr_gen,
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

  /// @brief 名前から UDP 定義を取出す．
  /// @param[in] name 名前
  /// @return name という名の UDP を返す．
  /// @return なければ nullptr を返す．
  const VlUdpDefn*
  find_udp(const char* name) const;

  /// @brief 名前から UserSystf を取出す．
  /// @param[in] name 名前
  /// @return name という名のユーザー定義関数を返す．
  /// @return なければ nullptr を返す．
  const VlUserSystf*
  find_user_systf(const char* name) const;

  /// @brief スコープと名前から名前付き要素を取り出す．
  /// @param[in] parent 検索対象のスコープ
  /// @param[in] name 名前
  /// @return parent というスコープ内の name という要素を返す．
  /// @return なければ nullptr を返す．
  ObjHandle*
  find_obj(const VlScope* parent,
	   const char* name) const;

  /// @brief スコープと名前からスコープを取り出す．
  /// @param[in] parent 検索対象のスコープ
  /// @param[in] name 名前
  /// @return parent というスコープ内の name というスコープを返す．
  /// @return なければ nullptr を返す．
  const VlScope*
  find_namedobj(const VlScope* parent,
		const char* name) const;

  /// @brief 名前によるオブジェクトの探索
  /// @param[in] base_scope 起点となるスコープ
  /// @param[in] pt_obj 階層名付きのオブジェクト
  /// @param[in] ulimit 探索する名前空間の上限
  /// @return 見付かったオブジェクトを返す．
  /// 見付からなかったら nullptr を返す．
  ObjHandle*
  find_obj_up(const VlScope* base_scope,
	      const PtHierNamedBase* pt_obj,
	      const VlScope* ulimit);

  /// @brief 名前からモジュール定義を取り出す．
  /// @param[in] name 名前
  /// @return name という名のモジュール定義
  /// @return なければ nullptr を返す．
  const PtModule*
  find_moduledef(const char* name) const;

  /// @brief 関数定義を探す．
  /// @param[in] parent 親のモジュール
  /// @param[in] name 関数名
  const PtItem*
  find_funcdef(const VlScope* parent,
	       const char* name) const;

  /// @brief constant function を取り出す．
  /// @param[in] parent 検索対象のスコープ
  /// @param[in] name 名前
  /// @return parent というスコープ内の name という関数を返す．
  /// @return なければ nullptr を返す．
  const VlTaskFunc*
  find_constant_function(const VlScope* parent,
			 const char* name) const;

  /// @brief パース木の属性定義から属性リストを取り出す．
  /// @param[in] pt_attr パース木の属性定義
  vector<const VlAttribute*>
  find_attr_list(const PtAttrInst* pt_attr) const;

  /// @brief セルの探索
  /// @param[in] name セル名
  /// @return name という名のセル番号を返す．
  ///
  /// なければ -1 を返す．
  int
  find_cell_id(const char* name) const;

  /// @brief セルの取得
  /// @param[in] cell_id セル番号
  const ClibCell&
  get_cell(int cell_id) const;


protected:
  //////////////////////////////////////////////////////////////////////
  // 要素を登録する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief UDP を登録する．
  /// @param[in] def_name 定義名
  /// @param[in] udp 登録する UDP
  void
  reg_udp(const char* def_name,
	  const VlUdpDefn* udp);

  /// @brief internal scope を登録する．
  /// @param[in] obj 登録するオブジェクト
  void
  reg_internalscope(const VlScope* obj);

  /// @brief タスクを登録する．
  /// @param[in] obj 登録するオブジェクト
  void
  reg_task(const VlTaskFunc* obj);

  /// @brief 関数を登録する．
  /// @param[in] obj 登録するオブジェクト
  void
  reg_function(const VlTaskFunc* obj);

  /// @brief 宣言要素を登録する．
  /// @param[in] tag タグ
  /// @param[in] obj 登録するオブジェクト
  void
  reg_decl(int tag,
	   ElbDecl* obj);

  /// @brief 配列型の宣言要素を登録する．
  /// @param[in] tag タグ
  /// @param[in] obj 登録するオブジェクト
  void
  reg_declarray(int tag,
		const VlDeclArray* obj);

  /// @brief パラメータを登録する．
  /// @param[in] tag タグ
  /// @param[in] obj 登録するオブジェクト
  void
  reg_parameter(int tag,
		ElbParameter* obj);

  /// @brief モジュール配列を登録する．
  /// @param[in] obj 登録するオブジェクト
  void
  reg_modulearray(const VlModuleArray* obj);

  /// @brief ElbModule を登録する．
  /// @param[in] module 登録するモジュール
  void
  reg_module(ElbModule* module);

  /// @brief プリミティブ配列を登録する．
  /// @param[in] obj 登録するオブジェクト
  void
  reg_primarray(ElbPrimArray* obj);

  /// @brief プリミティブを登録する．
  /// @param[in] obj 登録するオブジェクト
  void
  reg_primitive(ElbPrimitive* obj);

  /// @brief defparam を登録する．
  /// @param[in] obj 登録するオブジェクト
  void
  reg_defparam(const VlDefParam* obj);

  /// @brief paramassign を登録する．
  /// @param[in] obj 登録するオブジェクト
  void
  reg_paramassign(const VlParamAssign* obj);

  /// @brief continuous assignment を登録する．
  /// @param[in] obj 登録するオブジェクト
  void
  reg_contassign(const VlContAssign* obj);

  /// @brief process を登録する．
  /// @param[in] obj 登録するオブジェクト
  void
  reg_process(ElbProcess* obj);

  /// @brief genvar を登録する．
  /// @param[in] obj 登録するオブジェクト
  void
  reg_genvar(ElbGenvar* obj);

  /// @brief gfroot を登録する．
  /// @param[in] obj 登録するオブジェクト
  void
  reg_gfroot(ElbGfRoot* obj);

  /// @brief constant function を登録する．
  /// @param[in] func 関数
  void
  reg_constant_function(const VlTaskFunc* func);

  /// @brief 属性リストを登録する．
  /// @param[in] パース木の属性定義
  /// @param[in] attr_list 属性リスト
  void
  reg_attr_list(const PtAttrInst* pt_attr,
		const vector<const VlAttribute*>& attr_list);

  /// @brief 属性リストを登録する．
  /// @param[in] obj 対象のオブジェクト
  /// @param[in] def 定義側の属性の時 true とするフラグ
  /// @param[in] attr_list 属性リスト
  void
  reg_attr(const VlObj* obj,
	   bool def,
	   const vector<const VlAttribute*>& attr_list);


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
  phase1_item(const VlScope* parent,
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

  /// @brief 範囲を表す式を評価する．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_left 範囲のMSBを表すパース木
  /// @param[in] pt_right 範囲のLSBを表すパース木
  /// @param[out] left_val 範囲の MSB の値
  /// @param[out] right_val 範囲の LSB の値
  bool
  evaluate_range(const VlScope* parent,
		 const PtExpr* pt_left,
		 const PtExpr* pt_right,
		 int& left_val,
		 int& right_val);


protected:
  //////////////////////////////////////////////////////////////////////
  // attribute instance の生成関数
  //////////////////////////////////////////////////////////////////////

  /// @brief PtAttrInst から属性リストを生成し，オブジェクトに付加する．
  /// @param[in] pt_attr 属性を表すパース木
  /// @param[in] def 定義側の属性の時 true とするフラグ
  /// @param[in] obj 付加する対象のオブジェクト
  void
  instantiate_attribute(const PtAttrInst* pt_attr,
			bool def,
			const VlObj* obj);

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

  // attribute instance 生成用のオブジェクト
  AttrGen* mAttrGen;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 名前から UDP 定義を取出す．
// @param[in] name 名前
// @return name という名の UDP を返す．
// @return なければ nullptr を返す．
inline
const VlUdpDefn*
ElbProxy::find_udp(const char* name) const
{
  return mMgr.find_udp(name);
}

// @brief 名前から UserSystf を取出す．
// @param[in] name 名前
// @return name という名のユーザー定義関数を返す．
// @return なければ nullptr を返す．
inline
const VlUserSystf*
ElbProxy::find_user_systf(const char* name) const
{
  return mMgr.find_user_systf(name);
}

// @brief スコープと名前から名前付き要素を取り出す．
// @param[in] parent 検索対象のスコープ
// @param[in] name 名前
// @return parent というスコープ内の name という要素を返す．
// @return なければ nullptr を返す．
inline
ObjHandle*
ElbProxy::find_obj(const VlScope* parent,
		   const char* name) const
{
  return mElaborator.find_obj(parent, name);
}

// @brief スコープと名前からスコープを取り出す．
// @param[in] parent 検索対象のスコープ
// @param[in] name 名前
// @return parent というスコープ内の name というスコープを返す．
// @return なければ nullptr を返す．
inline
const VlScope*
ElbProxy::find_namedobj(const VlScope* parent,
			const char* name) const
{
  return mElaborator.find_namedobj(parent, name);
}

// @brief 名前によるオブジェクトの探索
// @param[in] base_scope 起点となるスコープ
// @param[in] nb_array 階層名の上部 (nullptr の場合も有りうる)
// @param[in] name 末尾の名前
// @param[in] ulimit 探索する名前空間の上限
// @return 見付かったオブジェクトを返す．
// 見付からなかったら nullptr を返す．
inline
ObjHandle*
ElbProxy::find_obj_up(const VlScope* base_scope,
		      const PtHierNamedBase* pt_obj,
		      const VlScope* ulimit)
{
  return mElaborator.find_obj_up(base_scope, pt_obj, ulimit);
}

// @brief セルの探索
// @param[in] name セル名
// @return name という名のセル番号を返す．
//
// なければ -1 を返す．
inline
int
ElbProxy::find_cell_id(const char* name) const
{
  return mElaborator.find_cell_id(name);
}

// @brief セルの取得
// @param[in] cell_id セル番号
inline
const ClibCell&
ElbProxy::get_cell(int cell_id) const
{
  return mElaborator.mCellLibrary.cell(cell_id);
}

// @brief UDP を登録する．
// @param[in] def_name 定義名
// @param[in] udp 登録する UDP
inline
void
ElbProxy::reg_udp(const char* def_name,
		  const VlUdpDefn* udp)
{
  mMgr.reg_udp(def_name, udp);
}

// @brief internal scope を登録する．
// @param[in] obj 登録するオブジェクト
inline
void
ElbProxy::reg_internalscope(const VlScope* obj)
{
  mElaborator.reg_internalscope(obj);
}

// @brief タスクを登録する．
// @param[in] obj 登録するオブジェクト
inline
void
ElbProxy::reg_task(const VlTaskFunc* obj)
{
  mElaborator.reg_task(obj);
}

// @brief 関数を登録する．
// @param[in] obj 登録するオブジェクト
inline
void
ElbProxy::reg_function(const VlTaskFunc* obj)
{
  mElaborator.reg_function(obj);
}

// @brief 宣言要素を登録する．
/// @param[in] tag タグ
// @param[in] obj 登録するオブジェクト
inline
void
ElbProxy::reg_decl(int tag,
		   ElbDecl* obj)
{
  mElaborator.reg_decl(tag, obj);
}

// @brief 配列型の宣言要素を登録する．
/// @param[in] tag タグ
// @param[in] obj 登録するオブジェクト
inline
void
ElbProxy::reg_declarray(int tag,
			const VlDeclArray* obj)
{
  mElaborator.reg_declarray(tag, obj);
}

// @brief パラメータを登録する．
// @param[in] tag タグ
// @param[in] obj 登録するオブジェクト
inline
void
ElbProxy::reg_parameter(int tag,
			ElbParameter* obj)
{
  mElaborator.reg_parameter(tag, obj);
}

// @brief モジュール配列を登録する．
// @param[in] obj 登録するオブジェクト
inline
void
ElbProxy::reg_modulearray(const VlModuleArray* obj)
{
  mElaborator.reg_modulearray(obj);
}

// @brief モジュールを登録する．
// @param[in] module 登録するモジュール
inline
void
ElbProxy::reg_module(ElbModule* module)
{
  mElaborator.reg_module(module);
}

// @brief プリミティブ配列を登録する．
// @param[in] obj 登録するオブジェクト
inline
void
ElbProxy::reg_primarray(ElbPrimArray* obj)
{
  mElaborator.reg_primarray(obj);
}

// @brief プリミティブを登録する．
// @param[in] obj 登録するオブジェクト
inline
void
ElbProxy::reg_primitive(ElbPrimitive* obj)
{
  mElaborator.reg_primitive(obj);
}

// @brief defparam を登録する．
// @param[in] obj 登録するオブジェクト
inline
void
ElbProxy::reg_defparam(const VlDefParam* obj)
{
  mElaborator.reg_defparam(obj);
}

// @brief paramassign を登録する．
// @param[in] obj 登録するオブジェクト
inline
void
ElbProxy::reg_paramassign(const VlParamAssign* obj)
{
  mElaborator.reg_paramassign(obj);
}

// @brief continuous assignment を登録する．
// @param[in] obj 登録するオブジェクト
inline
void
ElbProxy::reg_contassign(const VlContAssign* obj)
{
  mElaborator.reg_contassign(obj);
}

// @brief process を登録する．
// @param[in] obj 登録するオブジェクト
inline
void
ElbProxy::reg_process(ElbProcess* obj)
{
  mElaborator.reg_process(obj);
}

// @brief genvar を登録する．
// @param[in] obj 登録するオブジェクト
inline
void
ElbProxy::reg_genvar(ElbGenvar* obj)
{
  mElaborator.reg_genvar(obj);
}

// @brief gfroot を登録する．
// @param[in] obj 登録するオブジェクト
inline
void
ElbProxy::reg_gfroot(ElbGfRoot* obj)
{
  mElaborator.reg_gfroot(obj);
}

// @brief 名前からモジュール定義を取り出す．
// @param[in] name 名前
// @return name という名のモジュール定義
// @return なければ nullptr を返す．
inline
const PtModule*
ElbProxy::find_moduledef(const char* name) const
{
  return mElaborator.find_moduledef(name);
}

// @brief 関数定義を探す．
// @param[in] parent 親のモジュール
// @param[in] name 関数名
inline
const PtItem*
ElbProxy::find_funcdef(const VlScope* parent,
		       const char* name) const
{
  return mElaborator.find_funcdef(parent, name);
}

// @brief constant function を取り出す．
// @param[in] parent 検索対象のスコープ
// @param[in] name 名前
// @return parent というスコープ内の name という関数を返す．
// @return なければ nullptr を返す．
inline
const VlTaskFunc*
ElbProxy::find_constant_function(const VlScope* parent,
				 const char* name) const
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

// @brief パース木の属性定義から属性リストを取り出す．
// @param[in] pt_attr パース木の属性定義
inline
vector<const VlAttribute*>
ElbProxy::find_attr_list(const PtAttrInst* pt_attr) const
{
  return mElaborator.mAttrDict.find(pt_attr);
}

// @brief 属性リストを登録する．
// @param[in] パース木の属性定義
// @param[in] attr_list 属性リスト
inline
void
ElbProxy::reg_attr_list(const PtAttrInst* pt_attr,
			const vector<const VlAttribute*>& attr_list)
{
  return mElaborator.mAttrDict.add(pt_attr, attr_list);
}

// @brief 属性リストを登録する．
// @param[in] obj 対象のオブジェクト
// @param[in] def 定義側の属性の時 true とするフラグ
// @param[in] attr_list 属性リスト
inline
void
ElbProxy::reg_attr(const VlObj* obj,
		   bool def,
		   const vector<const VlAttribute*>& attr_list)
{
  mMgr.reg_attr(obj, def, attr_list);
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
