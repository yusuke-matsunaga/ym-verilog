#ifndef ELABORATOR_H
#define ELABORATOR_H

/// @file Elaborator.h
/// @brief Elaborator のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym/verilog.h"
#include "ym/pt/PtP.h"
#include "ym/vl/VlFwd.h"
#include "ym/ClibCellLibrary.h"

#include "ObjDict.h"
#include "ModuleHash.h"
#include "CfDict.h"
#include "AttrDict.h"
#include "ElbStub.h"

#include "ElbFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

class PtMgr;

class ElbMgr;
class ElbFactory;

class UdpGen;
class ModuleGen;
class DeclGen;
class ItemGen;
class StmtGen;
class ExprGen;
class AttrGen;

//////////////////////////////////////////////////////////////////////
/// @class Elaborator Elaborator.h "Elaborator.h"
/// @brief elaboration を行うクラス
/// 基本的には 内部に状態を持たないファンクタークラス
/// 実際には elaboration 途中でオブジェクトを保持しておくハッシュ表
/// などを持つ．最終結果は引数の ElbMgr に格納される．
//////////////////////////////////////////////////////////////////////
class Elaborator
{
  friend class ElbProxy;

public:

  /// @brief コンストラクタ
  /// @param[in] elb_mgr Elbオブジェクトを管理するクラス
  /// @param[in] elb_factory Elbオブジェクトを生成するファクトリクラス
  /// @param[in] cell_library セルライブラリ
  Elaborator(ElbMgr& elb_mgr,
	     ElbFactory& elb_factory,
	     const ClibCellLibrary& cell_library);

  /// @brief デストラクタ
  ~Elaborator();


public:

  /// @brief エラボレーションを行う．
  /// @param[in] pt_mgr パース木を管理するクラス
  /// @return エラー数を返す．
  int
  operator()(const PtMgr& pt_mgr);


private:
  //////////////////////////////////////////////////////////////////////
  // elaboration で用いられる下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 後で処理する defparam 文を登録する．
  /// @param[in] header アイテムテンプレートのヘッダ (defparam を持つ)
  /// @param[in] defparam defparam 文のテンプレート
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


private:
  //////////////////////////////////////////////////////////////////////
  // 要素を検索する関数
  //////////////////////////////////////////////////////////////////////

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
  find_funcdef(const VlNamedObj* parent,
	       const char* name) const;

  /// @brief constant function を取り出す．
  /// @param[in] parent 検索対象のスコープ
  /// @param[in] name 名前
  /// @return parent というスコープ内の name という関数を返す．
  /// @return なければ nullptr を返す．
  const ElbTaskFunc*
  find_constant_function(const VlNamedObj* parent,
			 const char* name) const;

  /// @brief セルの探索
  /// @param[in] name セル名
  /// @return name という名のセル番号を返す．
  ///
  /// なければ -1 を返す．
  int
  find_cell_id(const char* name) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 要素を登録する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief internal scope を登録する．
  /// @param[in] obj 登録するオブジェクト
  void
  reg_internalscope(const VlNamedObj* obj);

  /// @brief タスクを登録する．
  /// @param[in] obj 登録するオブジェクト
  void
  reg_task(ElbTaskFunc* obj);

  /// @brief 関数を登録する．
  /// @param[in] obj 登録するオブジェクト
  void
  reg_function(ElbTaskFunc* obj);

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
		ElbDeclArray* obj);

  /// @brief パラメータを登録する．
  /// @param[in] tag タグ
  /// @param[in] obj 登録するオブジェクト
  void
  reg_parameter(int tag,
		ElbParameter* obj);

  /// @brief モジュール配列を登録する．
  /// @param[in] obj 登録するオブジェクト
  void
  reg_modulearray(ElbModuleArray* obj);

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
  /// @param[in] parent 親のスコープ
  /// @param[in] name 名前
  /// @param[in] func 関数
  void
  reg_constant_function(const VlNamedObj* parent,
			const char* name,
			ElbTaskFunc* func);


public:
  //////////////////////////////////////////////////////////////////////
  // 検索関数
  //////////////////////////////////////////////////////////////////////

  /// @brief スコープと名前から名前付き要素を取り出す．
  /// @param[in] parent 検索対象のスコープ
  /// @param[in] name 名前
  /// @return parent というスコープ内の name という要素を返す．
  /// @return なければ nullptr を返す．
  ObjHandle*
  find_obj(const VlNamedObj* parent,
	   const char* name) const;

  /// @brief スコープと階層名から要素を取り出す．
  /// @param[in] base_scope 起点となるスコープ
  /// @param[in] pt_objy 階層名付きのオブジェクト
  /// @param[in] ulimit 探索する名前空間の上限
  /// @return 見付かったオブジェクトを返す．
  /// 見付からなかったら nullptr を返す．
  ObjHandle*
  find_obj_up(const VlNamedObj* base_scope,
	      const PtHierNamedBase* pt_obj,
	      const VlNamedObj* ulimit);


private:
  //////////////////////////////////////////////////////////////////////
  // 検索の下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief base_scope を起点として (nb, "") という名前のスコープを探す．
  /// なければ親のスコープに対して同様の探索を繰り返す．
  const VlNamedObj*
  find_scope_up(const VlNamedObj* base_scope,
		const PtHierNamedBase* pt_obj,
		const VlNamedObj* ulimit);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる型定義
  //////////////////////////////////////////////////////////////////////

  struct DefParamStub
  {

    /// @brief 対象のモジュール
    const VlModule* mModule;

    /// @brief パース木の DefParam ヘッダ
    const PtItem* mPtHeader;

    /// @brief パース木の DefParam 文
    const PtDefParam* mPtDefparam;
  };


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 生成したオブジェクトを管理するクラス
  ElbMgr& mMgr;

  // オブジェクト生成用のファクトリクラス
  ElbFactory& mFactory;

  // セルライブラリ
  ClibCellLibrary mCellLibrary;

  // UDP 生成用のオブジェクト
  unique_ptr<UdpGen> mUdpGen;

  // モジュール生成用のオブジェクト
  unique_ptr<ModuleGen> mModuleGen;

  // 宣言要素生成用のオブジェクト
  unique_ptr<DeclGen> mDeclGen;

  // 構成要素生成用のオブジェクト
  unique_ptr<ItemGen> mItemGen;

  // ステートメント生成用のオブジェクト
  unique_ptr<StmtGen> mStmtGen;

  // 式生成用のオブジェクト
  unique_ptr<ExprGen> mExprGen;

  // attribute instance 生成用のオブジェクト
  unique_ptr<AttrGen> mAttrGen;

  // 関数定義の辞書
  unordered_map<string, const PtItem*> mFuncDict;

  // 名前をキーにしたオブジェクトの辞書
  ObjDict mObjDict;

  // モジュール名をキーにしたモジュールインスタンスの辞書
  ModuleHash mModInstDict;

  // constant function の辞書
  CfDict mCfDict;

  // モジュールテンプレートの辞書
  unordered_map<string, const PtModule*> mModuleDict;

  // attribute instance の辞書
  AttrDict mAttrDict;

  // defparam 文の元のリスト
  list<DefParamStub> mDefParamStubList;

  // phase1 で生成するオブジェクトを追加するリスト
  ElbStubList mPhase1StubList1;

  // phase1 で生成するオブジェクトを読み出すリスト
  ElbStubList mPhase1StubList2;

  // phase2 生成するオブジェクトを入れたリスト
  ElbStubList mPhase2StubList;

  // phase3 で link するオブジェクトを入れたリスト
  ElbStubList mPhase3StubList;


private:
  //////////////////////////////////////////////////////////////////////
  // コンパイルオプション
  //////////////////////////////////////////////////////////////////////

  // IOに範囲がなく宣言のみに範囲を持つ場合を許すとき true
  bool mAllowEmptyIORange;

};

END_NAMESPACE_YM_VERILOG

#endif // ELABORATOR_H
