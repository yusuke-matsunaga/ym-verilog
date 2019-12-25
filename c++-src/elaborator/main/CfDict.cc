
/// @file CfDict.cc
/// @brief CfDict の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "elb/CfDict.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// constant function を格納するハッシュ表
//////////////////////////////////////////////////////////////////////

// @brief 要素を追加する．
// @param[in] scope 親のスコープ
// @param[in] name 名前
// @param[in] func 登録する関数
void
CfDict::add(const VlNamedObj* scope,
	    const char* name,
	    ElbTaskFunc* func)
{
  HierName key{scope, name};
  mHash.emplace(key, func);
}

// @brief 名前から該当する要素を検索する．
// @param[in] scope 親のスコープ
// @param[in] name 名前
ElbTaskFunc*
CfDict::find(const VlNamedObj* scope,
	     const char* name) const
{
  HierName key{scope, name};
  if ( mHash.count(key) > 0 ) {
    return mHash.at(key);
  }
  return nullptr;
}

// @brief 内容をクリアする．
void
CfDict::clear()
{
  mHash.clear();
}

END_NAMESPACE_YM_VERILOG
