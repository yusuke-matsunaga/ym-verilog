
/// @file PtMgr.cc
/// @brief PtMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "parser/PtMgr.h"
#include "ym/pt/PtModule.h"
#include "ym/pt/PtUdp.h"
#include "ym/pt/PtMisc.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス PtMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] factory Pti オブジェクトのファクトリクラス
PtMgr::PtMgr()
{
}

// @brief デストラクタ
PtMgr::~PtMgr()
{
  clear();
}

// @brief アロケータを返す．
Alloc&
PtMgr::alloc()
{
  return mAlloc;
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
PtMgr::check_def_name(const char* name) const
{
  return mDefNames.count(name) > 0;
}

// @brief 今までに生成したインスタンスをすべて破壊する．
void
PtMgr::clear()
{
  mUdpList.clear();
  mModuleList.clear();
  mDefNames.clear();
  mStringPool.clear();

  mAlloc.destroy();
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
PtMgr::reg_defname(const char* name)
{
  mDefNames.insert(name);
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
    SizeType n = strlen(str) + 1;
    void* p{mAlloc.get_memory(sizeof(char) * n)};
    auto new_str{new (p) char[n]};
    strcpy(new_str, str);
    mStringPool.insert(new_str);
    return new_str;
  }
  else {
    // 登録されていたらその文字列を返す．
    return *p;
  }
}

END_NAMESPACE_YM_VERILOG
