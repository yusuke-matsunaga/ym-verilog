
/// @file AttrHash.cc
/// @brief AttrHash の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "elaborator/AttrHash.h"
#include "ym/vl/VlObj.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス AttrHash
//////////////////////////////////////////////////////////////////////

// @brief 内容をクリアする．
void
AttrHash::clear()
{
  mHash.clear();
}

// @brief 属性を追加する．
// @param[in] obj 対象のオブジェクト
// @param[in] def 定義側の属性の時 true とするフラグ
// @param[in] attr_list 付加する属性リスト
void
AttrHash::add(const VlObj* obj,
	      bool def,
	      const vector<const VlAttribute*>& attr_list)
{
  if ( mHash.count(obj) == 0 ) {
    mHash.emplace(obj, Cell{{{}, {}}});
  }
  SizeType pos = (def) ? 1 : 0;
  auto& cell = mHash.at(obj);
  ASSERT_COND(cell.mAttrList[pos].empty() );
  cell.mAttrList[pos] = attr_list;
}

// @brief 属性を取り出す．
// @param[in] obj 対象のオブジェクト
// @param[in] def 定義側の属性の時 true とするフラグ
vector<const VlAttribute*>
AttrHash::find(const VlObj* obj,
	       bool def) const
{
  if ( mHash.count(obj) > 0 ) {
    SizeType pos = (def) ? 1 : 0;
    return mHash.at(obj).mAttrList[pos];
  }
  else {
    return {};
  }
}

END_NAMESPACE_YM_VERILOG
