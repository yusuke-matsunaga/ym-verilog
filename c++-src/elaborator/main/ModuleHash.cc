
/// @file ModuleHash.cc
/// @brief ModuleHash の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "elb/ModuleHash.h"
#include "ym/vl/VlModule.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// ElbNamedObj を格納するハッシュ表
//////////////////////////////////////////////////////////////////////

// @brief 内容を空にする．
void
ModuleHash::clear()
{
  mHash.clear();
}

// @brief 要素を追加する．
void
ModuleHash::add(const VlModule* obj)
{
  const VlNamedObj* parent = obj->parent();
  const char* name = obj->def_name();
  HierName key{parent, name};
  if ( mHash.count(key) > 0 ) {
    // 同じモジュールが既に登録されていたらモジュール名での検索はできない．
    mHash.emplace(key, nullptr);
  }
  else {
    mHash.emplace(key, obj);
  }
}

// @brief 名前から該当する要素を検索する．
const VlModule*
ModuleHash::find(const VlNamedObj* parent,
		 const char* name) const
{
  HierName key{parent, name};
  if ( mHash.count(key) > 0 ) {
    return mHash.at(key);
  }
  else {
    return nullptr;
  }
}

END_NAMESPACE_YM_VERILOG
