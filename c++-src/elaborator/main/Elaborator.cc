
/// @file Elaborator.cc
/// @brief Elaborator の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.


#include "elaborator/Elaborator.h"

#include "UdpGen.h"
#include "ModuleGen.h"
#include "DeclGen.h"
#include "ItemGen.h"
#include "StmtGen.h"
#include "ExprGen.h"
#include "AttrGen.h"

#include "ym/pt/PtModule.h"
#include "ym/pt/PtItem.h"
#include "ym/pt/PtMisc.h"

#include "ym/ClibCellLibrary.h"

#include "parser/PtMgr.h"

#include "elaborator/ElbModule.h"
#include "elaborator/ElbUdp.h"
#include "elaborator/ElbModule.h"
#include "elaborator/ElbTaskFunc.h"
#include "elaborator/ElbGfRoot.h"
#include "elaborator/ElbDecl.h"
#include "elaborator/ElbParameter.h"
#include "elaborator/ElbPrimitive.h"
#include "elaborator/ElbContAssign.h"
#include "elaborator/ElbProcess.h"
#include "elaborator/ElbGenvar.h"

#include "ym/MsgMgr.h"


BEGIN_NAMESPACE_YM_VERILOG

#define dout cerr

const ymuint debug_none       = 0x00000000;
const ymuint debug_objdict    = 0x00000001;
const ymuint debug_find_scope = 0x00000010;
const ymuint debug_all        = 0xFFFFFFFF;

#if 1
const ymuint debug = debug_none;
#else
const ymuint debug = debug_all;
#endif

BEGIN_NONAMESPACE

// mFuncDict 用のキー生成関数
inline
string
gen_funckey(const PtModule* module,
	    const char* name)
{
  return string(module->name()) + " " + string(name);
}

END_NONAMESPACE

// @brief コンストラクタ
// @param[in] elb_mgr Elbオブジェクトを管理するクラス
// @param[in] elb_factory Elbオブジェクトを生成するファクトリクラス
// @param[in] cell_library セルライブラリ
Elaborator::Elaborator(ElbMgr& elb_mgr,
		       ElbFactory& elb_factory,
		       const ClibCellLibrary& cell_library) :
  mMgr{elb_mgr},
  mFactory{elb_factory},
  mCellLibrary{cell_library},
  mUdpGen{new UdpGen(*this, elb_mgr, elb_factory)},
  mModuleGen{new ModuleGen(*this, elb_mgr, elb_factory)},
  mDeclGen{new DeclGen(*this, elb_mgr, elb_factory)},
  mItemGen{new ItemGen(*this, elb_mgr, elb_factory)},
  mStmtGen{new StmtGen(*this, elb_mgr, elb_factory)},
  mExprGen{new ExprGen(*this, elb_mgr, elb_factory)},
  mAttrGen{new AttrGen(*this, elb_mgr, elb_factory)}
{
  mAllowEmptyIORange = true;

  mUdpGen->init(mModuleGen.get(), mDeclGen.get(), mItemGen.get(), mStmtGen.get(),
		mExprGen.get(), mAttrGen.get());
  mModuleGen->init(mModuleGen.get(), mDeclGen.get(), mItemGen.get(), mStmtGen.get(),
		   mExprGen.get(), mAttrGen.get());
  mDeclGen->init(mModuleGen.get(), mDeclGen.get(), mItemGen.get(), mStmtGen.get(),
		 mExprGen.get(), mAttrGen.get());
  mItemGen->init(mModuleGen.get(), mDeclGen.get(), mItemGen.get(), mStmtGen.get(),
		 mExprGen.get(), mAttrGen.get());
  mStmtGen->init(mModuleGen.get(), mDeclGen.get(), mItemGen.get(), mStmtGen.get(),
		 mExprGen.get(), mAttrGen.get());
  mExprGen->init(mModuleGen.get(), mDeclGen.get(), mItemGen.get(), mStmtGen.get(),
		 mExprGen.get(), mAttrGen.get());
  mAttrGen->init(mModuleGen.get(), mDeclGen.get(), mItemGen.get(), mStmtGen.get(),
		 mExprGen.get(), mAttrGen.get());
}

// @brief デストラクタ
Elaborator::~Elaborator()
{
}

// @brief エラボレーションを行う．
// @param[in] pt_mgr パース木を管理するクラス
// @param[in] elb_mgr Elbオブジェクトを管理するクラス
// @param[in] elb_factory Elbオブジェクトを生成するファクトリクラス
// @param[in] msg_mgr メッセージマネージャ
// @return エラー数を返す．
int
Elaborator::operator()(const PtMgr& pt_mgr)
{
  auto& pt_udp_list = pt_mgr.pt_udp_list();
  auto& pt_module_list = pt_mgr.pt_module_list();

  // UDP の生成
  for ( auto pt_udp: pt_udp_list ) {
    mUdpGen->instantiate_udp(pt_udp);
  }

  // モジュールテンプレートの辞書を作る．
  // と同時に UDP 名とモジュール名の重複チェックを行う．
  // と同時に関数定義の辞書を作る．
  int nerr = 0;
  for ( auto pt_module: pt_module_list ) {
    const char* name = pt_module->name();
    if ( mMgr.find_udp(name) != nullptr ) {
      ostringstream buf;
      buf << "\"" << name
	  << "\" is duplicately defined as module and as UDP.";
      MsgMgr::put_msg(__FILE__, __LINE__,
		      pt_module->file_region(),
		      MsgType::Error,
		      "ELAB",
		      buf.str());
      ++ nerr;
    }
    else if ( mModuleDict.count(name) > 0 ) {
      ostringstream buf;
      buf << "module \"" << name<< "\" is redefined.";
      MsgMgr::put_msg(__FILE__, __LINE__,
		      pt_module->file_region(),
		      MsgType::Error,
		      "ELAB",
		      buf.str());
      ++ nerr;
    }
    else {
      // モジュール名をキーにして登録する．
      mModuleDict.emplace(name, pt_module);
    }

    // 関数の辞書を作る．
    for ( auto item: pt_module->item_list() ) {
      if ( item->type() == PtItemType::Func ) {
	auto key = gen_funckey(pt_module, item->name());
	mFuncDict.emplace(key, item);
      }
    }
  }

  if ( nerr == 0 ) {

    // トップレベル階層の生成
    const VlNamedObj* toplevel = mFactory.new_Toplevel();
    mMgr.reg_toplevel(toplevel);

    // トップモジュールの生成
    for ( auto pt_module: pt_module_list ) {
      if ( !pt_mgr.check_def_name(pt_module->name()) ) {
	// 他のモジュールから参照されていないモジュールをトップモジュールとみなす．
	mModuleGen->phase1_topmodule(toplevel, pt_module);
      }
    }

    // 残りの要素の生成

    // Phase 1
    // トップモジュールから名前空間を表す骨組みを作る．
    // 最下位レベルのモジュールに行き着くか配列型のモジュールインスタンス
    // で止る．

    // 配列型 module instance および generate 文から骨組みの生成を行う．
    for ( ; ; ) {
      // defparam 文で適用できるものがあれば適用する．
      MsgMgr::put_msg(__FILE__, __LINE__,
		      FileRegion(),
		      MsgType::Debug,
		      "ELAB",
		      "\"instantiate_defparam\" starts.");

      // 未処理の defparam 文を処理する．
      // 処理された要素は mDefParamList から削除される．
      // 中にはまだ名前空間が構築されていないものもあるので mDefParamList
      // が空にならない場合もある．
      for ( auto p = mDefParamStubList.begin(); p != mDefParamStubList.end(); ) {
	// リストの中で要素の削除を行うので反復子の扱いがちょっと複雑
	auto q = p;
	++ p;

	const VlModule* module = q->mModule;
	const PtItem* pt_header = q->mPtHeader;
	const PtDefParam* pt_defparam = q->mPtDefparam;
	if ( mItemGen->defparam_override(module, pt_header, pt_defparam, nullptr) ) {
	  // オーバーライドがうまく行ったらこの要素は捨てる．
	  mDefParamStubList.erase(q);
	}
      }

      // その結果にもとづいてモジュール配列インスタンスや
      // generate block の生成を行う．
      MsgMgr::put_msg(__FILE__, __LINE__,
		      FileRegion(),
		      MsgType::Debug,
		      "ELAB",
		      "Phase 1 starts.");

      if ( mPhase1StubList1.empty() ) {
	// 処理する要素が残っていない．
	break;
      }
      mPhase1StubList2.move(mPhase1StubList1);
      mPhase1StubList2.eval();
    }

    // 適用できなかった defparam 文のチェック
    for ( auto stub: mDefParamStubList ) {
      auto pt_defparam = stub.mPtDefparam;
      ostringstream buf;
      buf << pt_defparam->fullname() << " : not found.";
      MsgMgr::put_msg(__FILE__, __LINE__,
		      pt_defparam->file_region(),
		      MsgType::Error,
		      "ELAB",
		      buf.str());
    }

    // Phase 2
    // 配列要素やビット要素の生成を行う．
    MsgMgr::put_msg(__FILE__, __LINE__,
		    FileRegion(),
		    MsgType::Debug,
		    "ELAB",
		    "Phase 2 starts.");

    mPhase2StubList.eval();

    // Phase 3
    // 名前の解決(リンク)を行う．
    MsgMgr::put_msg(__FILE__, __LINE__,
		    FileRegion(),
		    MsgType::Debug,
		    "ELAB",
		    "Phase 3 starts.");

    mPhase3StubList.eval();

  }

  mObjDict.clear();
  mCfDict.clear();
  mModuleDict.clear();
  mDefParamStubList.clear();
  mPhase1StubList1.clear();
  mPhase1StubList2.clear();
  mPhase2StubList.clear();
  mPhase3StubList.clear();

  return nerr;
}

// 後で処理する defparam 文を登録する．
void
Elaborator::add_defparamstub(const VlModule* module,
			     const PtItem* pt_header)
{
  for ( auto pt_defparam: pt_header->defparam_list() ) {
    mDefParamStubList.push_back(DefParamStub{module, pt_header, pt_defparam});
  }
}

// @brief phase1 で行う処理を登録する．
// @param[in] stub phase1 で行う処理を表すスタブ
void
Elaborator::add_phase1stub(ElbStub* stub)
{
  mPhase1StubList1.push_back(stub);
}

// @brief phase2 で行う処理を登録する．
// @param[in] stub phase2 で行う処理を表すスタブ
void
Elaborator::add_phase2stub(ElbStub* stub)
{
  mPhase2StubList.push_back(stub);
}

// phase3 で行う処理を登録する．
void
Elaborator::add_phase3stub(ElbStub* stub)
{
  mPhase3StubList.push_back(stub);
}

// @brief 名前からモジュール定義を取り出す．
// @param[in] name 名前
// @return name という名のモジュール定義
// @return なければ nullptr を返す．
const PtModule*
Elaborator::find_moduledef(const char* name) const
{
  if ( mModuleDict.count(name) > 0 ) {
    return mModuleDict.at(name);
  }
  else {
    return nullptr;
  }
}

// @brief 関数定義を探す．
// @param[in] parent 親のモジュール
// @param[in] name 関数名
const PtItem*
Elaborator::find_funcdef(const VlNamedObj* parent,
			 const char* name) const
{
  auto pt_module = find_moduledef(parent->name());
  if ( pt_module == nullptr ) {
    return nullptr;
  }
  auto key = gen_funckey(pt_module, name);
  if ( mFuncDict.count(key) > 0 ) {
    return mFuncDict.at(key);
  }
  else {
    return nullptr;
  }
}

// @brief constant function を取り出す．
// @param[in] parent 親のスコープ
// @param[in] name 名前
const ElbTaskFunc*
Elaborator::find_constant_function(const VlNamedObj* parent,
				   const char* name) const
{
  auto h = mCfDict.find(parent, name);
  if ( h != nullptr ) {
    return h->taskfunc();
  }
  else {
    return nullptr;
  }
}

// @brief internal scope を登録する．
// @param[in] obj 登録するオブジェクト
void
Elaborator::reg_internalscope(const VlNamedObj* obj)
{
  if ( debug & debug_objdict ) {
    dout << "reg_internalscope( " << obj->name() << " @ "
	 << obj->parent()->full_name()
	 << " ["
	 << hex << reinterpret_cast<ympuint>(obj->parent()) << dec
	 << "] )" << endl
	 << endl;
  }
  mObjDict.add(obj);
  mMgr.reg_internalscope(obj);
}

// @brief タスクを登録する．
// @param[in] obj 登録するオブジェクト
void
Elaborator::reg_task(ElbTaskFunc* obj)
{
  if ( debug & debug_objdict ) {
    dout << "reg_task( " << obj->name() << " @ "
	 << obj->parent()->full_name()
	 << " ["
	 << hex << reinterpret_cast<ympuint>(obj->parent()) << dec
	 << "] )" << endl << endl;
  }
  mObjDict.add(obj);
  mMgr.reg_task(obj);
}

// @brief 関数を登録する．
// @param[in] obj 登録するオブジェクト
void
Elaborator::reg_function(ElbTaskFunc* obj)
{
  if ( debug & debug_objdict ) {
    dout << "reg_function( " << obj->name() << " @ "
	 << obj->parent()->full_name()
	 << " ["
	 << hex << reinterpret_cast<ympuint>(obj->parent()) << dec
	 << "] )" << endl << endl;
  }
  mObjDict.add(obj);
  mMgr.reg_function(obj);
}

// @brief 宣言要素を登録する．
// @param[in] tag タグ
// @param[in] obj 登録するオブジェクト
void
Elaborator::reg_decl(int tag,
		     ElbDecl* obj)
{
  if ( debug & debug_objdict ) {
    dout << "reg_decl( " << obj->name() << " @ "
	 << obj->parent()->full_name()
	 << " ["
	 << hex << reinterpret_cast<ympuint>(obj->parent()) << dec
	 << "] )" << endl << endl;
  }
  mObjDict.add(obj);
  mMgr.reg_decl(tag, obj);
}

// @brief 配列型の宣言要素を登録する．
// @param[in] tag タグ
// @param[in] obj 登録するオブジェクト
void
Elaborator::reg_declarray(int tag,
			  ElbDeclArray* obj)
{
  if ( debug & debug_objdict ) {
    dout << "reg_declarray( " << obj->name() << " @ "
	 << obj->parent()->full_name()
	 << " ["
	 << hex << reinterpret_cast<ympuint>(obj->parent()) << dec
	 << "] )" << endl << endl;
  }
  mObjDict.add(obj);
  mMgr.reg_declarray(tag, obj);
}

// @brief パラメータを登録する．
// @param[in] tag タグ
// @param[in] obj 登録するオブジェクト
void
Elaborator::reg_parameter(int tag,
			  ElbParameter* obj)
{
  if ( debug & debug_objdict ) {
    dout << "reg_decl( " << obj->name() << " @ "
	 << obj->parent()->full_name()
	 << " ["
	 << hex << reinterpret_cast<ympuint>(obj->parent()) << dec
	 << "] )" << endl << endl;
  }
  mObjDict.add(obj);
  mMgr.reg_decl(tag, obj);
}

// @brief モジュール配列を登録する．
// @param[in] obj 登録するオブジェクト
void
Elaborator::reg_modulearray(ElbModuleArray* obj)
{
  if ( debug & debug_objdict ) {
    dout << "reg_modulearray( " << obj->name() << " @ "
	 << obj->parent()->full_name()
	 << " ["
	 << hex << reinterpret_cast<ympuint>(obj->parent()) << dec
	 << "] )" << endl << endl;
  }
  mObjDict.add(obj);
  mMgr.reg_modulearray(obj);
}

// @brief ElbModule を登録する．
// @param[in] module 登録するモジュール
void
Elaborator::reg_module(ElbModule* obj)
{
  if ( debug & debug_objdict ) {
    dout << "reg_module( " << obj->name() << " @ "
	 << obj->parent()->full_name()
	 << " ["
	 << hex << reinterpret_cast<ympuint>(obj->parent()) << dec
	 << "] )" << endl << endl;
  }
  mObjDict.add(obj);
  mMgr.reg_module(obj);
}

// @brief プリミティブ配列を登録する．
// @param[in] obj 登録するオブジェクト
void
Elaborator::reg_primarray(ElbPrimArray* obj)
{
  if ( obj->name() != nullptr ) {
    if ( debug & debug_objdict ) {
      dout << "reg_primarray( " << obj->name() << " @ "
	   << obj->parent()->full_name()
	 << " ["
	 << hex << reinterpret_cast<ympuint>(obj->parent()) << dec
	 << "] )" << endl << endl;
    }
    mObjDict.add(obj);
  }
  mMgr.reg_primarray(obj);
}

// @brief プリミティブを登録する．
// @param[in] obj 登録するオブジェクト
void
Elaborator::reg_primitive(ElbPrimitive* obj)
{
  if ( obj->name() != nullptr ) {
    if ( debug & debug_objdict ) {
      dout << "reg_primitive( " << obj->name() << " @ "
	   << obj->parent()->full_name()
	   << " ["
	   << hex << reinterpret_cast<ympuint>(obj->parent()) << dec
	   << "] )" << endl << endl;
    }
    mObjDict.add(obj);
  }
  mMgr.reg_primitive(obj);
}

// @brief defparam を登録する．
// @param[in] obj 登録するオブジェクト
void
Elaborator::reg_defparam(const VlDefParam* obj)
{
  mMgr.reg_defparam(obj);
}

// @brief paramassign を登録する．
// @param[in] obj 登録するオブジェクト
void
Elaborator::reg_paramassign(const VlParamAssign* obj)
{
  mMgr.reg_paramassign(obj);
}

// @brief continuous assignment を登録する．
// @param[in] obj 登録するオブジェクト
void
Elaborator::reg_contassign(const VlContAssign* obj)
{
  mMgr.reg_contassign(obj);
}

// @brief process を登録する．
// @param[in] obj 登録するオブジェクト
void
Elaborator::reg_process(ElbProcess* obj)
{
  mMgr.reg_process(obj);
}

// @brief genvar を登録する．
// @param[in] obj 登録するオブジェクト
void
Elaborator::reg_genvar(ElbGenvar* obj)
{
  if ( debug & debug_objdict ) {
    dout << "reg_genvar( " << obj->name() << " @ "
	 << obj->parent()->full_name()
	 << " ["
	 << hex << reinterpret_cast<ympuint>(obj->parent()) << dec
	 << "] )" << endl << endl;
  }
  mObjDict.add(obj);
}

// @brief gfroot を登録する．
// @param[in] obj 登録するオブジェクト
void
Elaborator::reg_gfroot(ElbGfRoot* obj)
{
  if ( debug & debug_objdict ) {
    dout << "reg_gfroot( " << obj->name() << " @ "
	 << obj->parent()->full_name()
	 << " ["
	 << hex << reinterpret_cast<ympuint>(obj->parent()) << dec
	 << "] )" << endl << endl;
  }
  mObjDict.add(obj);
}

// @brief constant function を登録する．
// @param[in] func 関数
void
Elaborator::reg_constant_function(ElbTaskFunc* func)
{
  mCfDict.add(func);
}

// @brief セルの探索
// @param[in] name セル名
// @return name という名のセル番号を返す．
// @note なければ -1 を返す．
int
Elaborator::find_cell_id(const char* name) const
{
  return mCellLibrary.cell_id(name);
}

// @brief スコープと名前から名前付き要素を取り出す．
// @param[in] parent 検索対象のスコープ
// @param[in] name 名前
// @return parent というスコープ内の name という要素を返す．
// @return なければ nullptr を返す．
ObjHandle*
Elaborator::find_obj(const VlNamedObj* scope,
		     const char* name) const
{
  if ( debug & debug_find_scope ) {
    dout << "find_obj( " << name << ", @ "
	 << scope->full_name()
	 << " ["
	 << hex << reinterpret_cast<ympuint>(scope) << dec
	 << "] )" << endl << endl;
  }

  ObjHandle* handle = mObjDict.find(scope, name);

  if ( handle == nullptr ) {
    if ( debug & debug_find_scope ) {
      dout << "--> Not Found"
	   << endl << endl;
    }
    // なければ nullptr を返す。
    return nullptr;
  }

  if ( debug & debug_find_scope ) {
    dout << "--> Found"
	 << endl << endl;
  }

  return handle;
}

// @brief 名前によるオブジェクトの探索
// @param[in] base_scope 起点となるスコープ
// @param[in] pt_objy 階層名付きのオブジェクト
// @param[in] ulimit 探索する名前空間の上限
// @return 見付かったオブジェクトを返す．
// 見付からなかったら nullptr を返す．
ObjHandle*
Elaborator::find_obj_up(const VlNamedObj* base_scope,
			const PtHierNamedBase* pt_obj,
			const VlNamedObj* ulimit)
{
  // まず nb の部分の解決を行う．
  base_scope = find_scope_up(base_scope, pt_obj, ulimit);
  if ( base_scope == nullptr ) {
    return nullptr;
  }

  if ( debug & debug_find_scope ) {
    dout << "find_obj_up( " << pt_obj->name() << " )@"
	 << base_scope->full_name() << endl;
  }

  // base_scope を起点として name というオブジェクトを探す．
  for ( ; base_scope; base_scope = base_scope->parent()) {
    ObjHandle* handle = find_obj(base_scope, pt_obj->name());
    if ( handle ) {
      // 見つけた
      if ( debug & debug_find_scope ) {
	dout << "--> Found: " << handle->obj()->name() << " @ "
	     << base_scope->name() << endl << endl;
      }
      return handle;
    }
    // base_scope が上限だったのでこれ以上 upward search できない．
    if ( base_scope == ulimit ) {
      if ( debug & debug_find_scope ) {
	dout << "--> Not found: reaches to ulimit" << endl << endl;
      }
      return nullptr;
    }
  }
  // ダミー
  return nullptr;
}

// base_scope を起点として (name_branch, "" ) という名前のスコープを探す．
// なければ親のスコープに対して同様の探索を繰り返す．
const VlNamedObj*
Elaborator::find_scope_up(const VlNamedObj* base_scope,
			  const PtHierNamedBase* pt_obj,
			  const VlNamedObj* ulimit)
{
  if ( debug & debug_find_scope ) {
    dout << "find_scope_up( "
	 << pt_obj->fullname()
	 << " ) @"
	 << base_scope->full_name() << endl;
  }

  SizeType n = pt_obj->namebranch_num();
  const VlNamedObj* cur_scope = base_scope;
  for ( auto name_branch: pt_obj->namebranch_list() ) {
    const char* top_name = name_branch->name();
    const VlNamedObj* top_scope = nullptr;
    // まず普通に探す．
    ObjHandle* handle = find_obj(cur_scope, top_name);
    if ( handle ) {
      if ( name_branch->has_index() ) {
	top_scope = handle->array_elem(name_branch->index());
      }
      else {
	top_scope = handle->obj();
      }
    }
    else if ( !name_branch->has_index() ) {
      // モジュール定義名として探す．
      top_scope = mModuleDefDict.find(cur_scope, top_name);
    }
    if ( top_scope == nullptr) {
      // cur_scope が上限もしくは cur_scope の親がいなければ
      // upward search できない．
      if ( cur_scope == ulimit || cur_scope->parent() == nullptr ) {

	if ( debug & debug_find_scope ) {
	  dout << "--> Not Found" << endl << endl;
	}

	return nullptr;
      }

      if ( debug & debug_find_scope ) {
	dout << " upward search" << endl;
      }

      cur_scope = cur_scope->parent();
    }
    else {

      if ( debug & debug_find_scope ) {
	dout << " downward search" << endl;
      }

      cur_scope = top_scope;
    }
  }

  if ( debug & debug_find_scope ) {
    dout << "--> Found: " << cur_scope->full_name() << endl << endl;
  }

  return cur_scope;
}

END_NAMESPACE_YM_VERILOG
