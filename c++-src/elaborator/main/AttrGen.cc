﻿
/// @file AttrGen.cc
/// @brief AttrGen の実装ファイル(メイン)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AttrGen.h"

#include "ym/pt/PtMisc.h"
#include "ym/vl/VlAttribute.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス AttrGen
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] elab 生成器
// @param[in] elb_mgr Elbオブジェクトを管理するクラス
// @param[in] elb_factory Elbオブジェクトを生成するファクトリクラス
AttrGen::AttrGen(Elaborator& elab,
		 ElbMgr& elb_mgr,
		 ElbFactory& elb_factory) :
  ElbProxy(elab, elb_mgr, elb_factory)
{
}

// @brief デストラクタ
AttrGen::~AttrGen()
{
}

// @brief PtAttr から ElbAttr を生成する．
// @param[in] pt_attr 式を表すパース木
// @param[in] def 定義側の属性の時 true とするフラグ
// @param[in] obj 属性を設定する対象のオブジェクト
void
AttrGen::instantiate_attribute(const PtAttrInst* pt_attr,
			       bool def,
			       const VlObj* obj)
{
#if 0
  auto attr_list = find_attr_list(pt_attr);
  if ( attr_list.empty() ) {
    SizeType n = 0;
    for (const PtAttrInst* pt_ai = pt_attr;
	 pt_ai; pt_ai = pt_ai->next()) {
      for (const PtAttrSpec* pt_as = pt_ai->attr_spec_top();
	   pt_as; pt_as = pt_as->next()) {
	++ n;
      }
    }
    attr_list = factory().new_AttrList(n);
    ymuint i = 0;
    for ( auto pt_ai = pt_attr; pt_ai; pt_ai = pt_ai->next() ) {
      for ( auto pt_as = pt_ai->attr_spec_top();
	    pt_as; pt_as = pt_as->next()) {
	auto expr = instantiate_constant_expr(nullptr, pt_as->expr());
	if ( !expr ) {
	  // エラー．たぶん expr() が constant_expression ではなかった．
	  // でも無視する．
	}
	attr_list->set(i, pt_as, expr, def);
	++ i;
      }
    }
    reg_attr_list(pt_attr, attr_list);
  }
  reg_attr(obj, def, attr_list);
#endif
}

END_NAMESPACE_YM_VERILOG
