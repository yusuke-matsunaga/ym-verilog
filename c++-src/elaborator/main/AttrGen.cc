
/// @file AttrGen.cc
/// @brief AttrGen の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.


#include "AttrGen.h"
#include "elaborator/ElbExpr.h"
#include "ym/pt/PtMisc.h"
#include "ym/vl/VlAttribute.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス AttrGen
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] elab 生成器
// @param[in] elb_mgr Elbオブジェクトを管理するクラス
AttrGen::AttrGen(Elaborator& elab,
		 ElbMgr& elb_mgr) :
  ElbProxy(elab, elb_mgr)
{
}

// @brief デストラクタ
AttrGen::~AttrGen()
{
}

// @brief PtAttr から ElbAttr を生成する．
// @param[in] attr_info 属性リストの情報
void
AttrGen::instantiate_attribute(const PtiAttrInfo& attr_info)
{
  auto pt_obj = attr_info.obj();
  if ( mHash.count(pt_obj) == 0 ) {
    // また未生成なので作る．
    auto pt_attr_list = attr_info.attr_list();
    bool def = attr_info.def();
    vector<const VlAttribute*> attr_list;
    for ( auto pt_ai: pt_attr_list ) {
      auto pt_as_list = pt_ai->attrspec_list();
      for ( auto pt_as: pt_as_list ) {
	auto expr = instantiate_constant_expr(nullptr, pt_as->expr());
	if ( !expr ) {
	  // エラー．たぶん expr() が constant_expression ではなかった．
	  // でも無視する．
	}
	// attr_list に pt_as, expr, def を追加
	auto attr{mgr().new_Attribute(pt_as, expr, def)};
	attr_list.push_back(attr);
      }
    }
    // attr_list が空でも処理済みの意味で追加する．
    mHash.emplace(pt_obj, attr_list);
  }
  mHash.at(pt_obj);
}

// @brief 構文木要素に対応する属性リストを返す．
// @param[in] pt_obj 元となる構文木要素
const vector<const VlAttribute*>&
AttrGen::attribute_list(const PtBase* pt_obj)
{
  if ( mHash.count(pt_obj) > 0 ) {
    return mHash.at(pt_obj);
  }
  else {
    return mEmptyList;
  }
}

END_NAMESPACE_YM_VERILOG
