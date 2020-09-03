
/// @file PtMgr.cc
/// @brief PtMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.

#include "alloc/SimpleAlloc.h"
#include "parser/PtMgr.h"
#include "ym/pt/PtModule.h"
#include "ym/pt/PtUdp.h"
#include "ym/pt/PtMisc.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス PtMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PtMgr::PtMgr() :
  mAlloc{new SimpleAlloc}
{
}

// @brief デストラクタ
PtMgr::~PtMgr()
{
  clear();
}

// @brief 登録されているモジュールのリストを返す．
// @return 登録されているモジュールのリスト
const vector<const PtModule*>&
PtMgr::pt_module_list() const
{
  return mModuleList;
}

// @brief 登録されている UDP のリストを返す．
// @return 登録されている UDP のリスト
const vector<const PtUdp*>&
PtMgr::pt_udp_list() const
{
  return mUdpList;
}

// @brief インスタンス記述で用いられている名前かどうか調べる．
// @param[in] name 調べる名前
// @return 用いられていたら true を返す．
bool
PtMgr::check_def_name(const string& name) const
{
  return mDefNames.count(name) > 0;
}

// @brief attribute instance を取り出す．
// @param[in] pt_obj 対象の構文木の要素
// @return PtAttrInst のリスト
//
// 空の場合もある．
vector<const PtAttrInst*>
PtMgr::find_attr_list(const PtBase* pt_obj) const
{
  PtiAttrInfo key{pt_obj, {}};
  if ( mAttrDict.count(key) > 0 ) {
    const auto& attr_info{mAttrDict.find(key)};
    return attr_info->attr_list();
  }
  else {
    return {};
  }
}

// @brief 全ての属性リストのリストを返す．
vector<const PtiAttrInfo>
PtMgr::all_attr_list() const
{
  vector<const PtiAttrInfo> ans;
  ans.reserve(mAttrDict.size());
  for ( const auto& ai: mAttrDict ) {
    ans.push_back(ai);
  }
  return ans;
}

// @brief 今までに生成したインスタンスをすべて破壊する．
void
PtMgr::clear()
{
  mUdpList.clear();
  mModuleList.clear();
  mDefNames.clear();
  mStringPool.clear();

  FileInfo::clear();
  mAlloc->destroy();
}

// UDP の登録
// @param udp 登録する UDP
void
PtMgr::reg_udp(const PtUdp* udp)
{
  mUdpList.push_back(udp);
}

// モジュールの登録
// @param module 登録するモジュール
void
PtMgr::reg_module(const PtModule* module)
{
  mModuleList.push_back(module);
}

// @brief インスタンス定義名を追加する．
void
PtMgr::reg_defname(const string& name)
{
  mDefNames.insert(name);
}

// @brief attribute instance を登録する．
void
PtMgr::reg_attrinst(const PtBase* pt_obj,
		    PtrList<const PtAttrInst>* ai_list,
		    bool def)
{
  if ( ai_list ) {
    mAttrDict.emplace(PtiAttrInfo{pt_obj, ai_list->to_vector(), def});
  }
}

// @brief 文字列領域を確保する．
// @param[in] str 文字列
// @return 文字列を返す．
//
// 同一の文字列は共有する．
const char*
PtMgr::save_string(const char* str)
{
  auto p = mStringPool.find(str);
  if ( p == mStringPool.end() ) {
    // str と同じ内容は登録されていなかった．
    // 新しい領域を確保して登録する．
    mStringPool.insert(str);
  }
  p = mStringPool.find(str);
  ASSERT_COND( p != mStringPool.end() );
  return (*p).c_str();
}

// @brief メモリアロケーターを返す．
Alloc&
PtMgr::alloc()
{
  return *mAlloc;
}

END_NAMESPACE_YM_VERILOG
