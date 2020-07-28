﻿
/// @file Elaborator.cc
/// @brief Elaborator の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "elb/Elaborator.h"

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
#include "elb/ElbModule.h"

#include "ym/MsgMgr.h"


BEGIN_NAMESPACE_YM_VERILOG

// @brief コンストラクタ
// @param[in] elb_mgr Elbオブジェクトを管理するクラス
// @param[in] elb_factory Elbオブジェクトを生成するファクトリクラス
// @param[in] cell_library セルライブラリ
Elaborator::Elaborator(ElbMgr& elb_mgr,
		       ElbFactory& elb_factory,
		       const ClibCellLibrary& cell_library) :
  mMgr(elb_mgr),
  mFactory(elb_factory),
  mCellLibrary(cell_library)

{
  mAllowEmptyIORange = true;

  mUdpGen = new UdpGen(*this, elb_mgr, elb_factory);
  mModuleGen = new ModuleGen(*this, elb_mgr, elb_factory);
  mDeclGen = new DeclGen(*this, elb_mgr, elb_factory);
  mItemGen = new ItemGen(*this, elb_mgr, elb_factory);
  mStmtGen = new StmtGen(*this, elb_mgr, elb_factory);
  mExprGen = new ExprGen(*this, elb_mgr, elb_factory);
  mAttrGen = new AttrGen(*this, elb_mgr, elb_factory);

  mUdpGen->init(mModuleGen, mDeclGen, mItemGen, mStmtGen, mExprGen, mAttrGen);
  mModuleGen->init(mModuleGen, mDeclGen, mItemGen, mStmtGen, mExprGen, mAttrGen);
  mDeclGen->init(mModuleGen, mDeclGen, mItemGen, mStmtGen, mExprGen, mAttrGen);
  mItemGen->init(mModuleGen, mDeclGen, mItemGen, mStmtGen, mExprGen, mAttrGen);
  mStmtGen->init(mModuleGen, mDeclGen, mItemGen, mStmtGen, mExprGen, mAttrGen);
  mExprGen->init(mModuleGen, mDeclGen, mItemGen, mStmtGen, mExprGen, mAttrGen);
  mAttrGen->init(mModuleGen, mDeclGen, mItemGen, mStmtGen, mExprGen, mAttrGen);
}

// @brief デストラクタ
Elaborator::~Elaborator()
{
  delete mUdpGen;
  delete mModuleGen;
  delete mDeclGen;
  delete mItemGen;
  delete mStmtGen;
  delete mExprGen;
  delete mAttrGen;
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
  auto& udp_list = pt_mgr.pt_udp_list();
  auto& module_list = pt_mgr.pt_module_list();

  // UDP の生成
  for ( auto pt_udp: udp_list ) {
    mUdpGen->instantiate_udp(pt_udp);
  }

  // モジュールテンプレートの辞書を作る．
  // と同時に UDP 名とモジュール名の重複チェックを行う．
  int nerr = 0;
  for ( auto module: module_list ) {
    const char* name = module->name();
    if ( mMgr.find_udp(name) != nullptr ) {
      ostringstream buf;
      buf << "\"" << name
	  << "\" is duplicately defined as module and as UDP.";
      MsgMgr::put_msg(__FILE__, __LINE__,
		      module->file_region(),
		      MsgType::Error,
		      "ELAB",
		      buf.str());
      ++ nerr;
    }
    else if ( mModuleDict.count(name) > 0 ) {
      ostringstream buf;
      buf << "module \"" << name<< "\" is redefined.";
      MsgMgr::put_msg(__FILE__, __LINE__,
		      module->file_region(),
		      MsgType::Error,
		      "ELAB",
		      buf.str());
      ++ nerr;
    }
    else {
      // モジュール名をキーにして登録する．
      mModuleDict[name] = module;
    }
  }
  if ( nerr == 0 ) {

    // トップレベル階層の生成
    const VlNamedObj* toplevel = mFactory.new_Toplevel();
    mMgr.reg_toplevel(toplevel);

    // トップモジュールの生成
    for ( auto module: module_list ) {
      if ( !pt_mgr.check_def_name(module->name()) ) {
	// 他のモジュールから参照されていないモジュールをトップモジュールとみなす．
	mModuleGen->phase1_topmodule(toplevel, module);
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
      for (list<DefParamStub>::iterator p = mDefParamStubList.begin();
	   p != mDefParamStubList.end(); ) {
	// リストの中で要素の削除を行うので反復子の扱いがちょっと複雑
	list<DefParamStub>::iterator q = p;
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
  for ( SizeType i = 0; i < pt_header->defparam_num(); ++ i ) {
    auto pt_defparam = pt_header->defparam(i);
    mDefParamStubList.push_back(DefParamStub(module, pt_header, pt_defparam));
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

// @brief constant function を取り出す．
// @param[in] parent 親のスコープ
// @param[in] name 名前
const ElbTaskFunc*
Elaborator::find_constant_function(const VlNamedObj* parent,
				   const char* name) const
{
  return mCfDict.find(parent, name);
}

// @brief constant function を登録する．
// @param[in] parent 親のスコープ
// @param[in] name 名前
// @param[in] func 関数
void
Elaborator::reg_constant_function(const VlNamedObj* parent,
				  const char* name,
				  ElbTaskFunc* func)
{
  mCfDict.add(parent, name, func);
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

END_NAMESPACE_YM_VERILOG
