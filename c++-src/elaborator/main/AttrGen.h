#ifndef ATTRGEN_H
#define ATTRGEN_H

/// @file AttrGen.h
/// @brief AttrGen のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym/verilog.h"
#include "ym/pt/PtP.h"
#include "ElbProxy.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class AttrGen AttrGen.h "AttrGen.h"
/// @brief ElbAttr を生成するクラス
//////////////////////////////////////////////////////////////////////
class AttrGen :
  public ElbProxy
{
public:

  /// @brief コンストラクタ
  /// @param[in] elab 生成器
  /// @param[in] elb_mgr Elbオブジェクトを管理するクラス
  AttrGen(Elaborator& elab,
	  ElbMgr& elb_mgr);

  /// @brief デストラクタ
  ~AttrGen();


public:
  //////////////////////////////////////////////////////////////////////
  // AttrGen の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief PtAttr から ElbAttr を生成する．
  /// @param[in] pt_attr_array 属性を表すパース木
  /// @param[in] def 定義側の属性の時 true とするフラグ
  /// @param[in] obj 属性を設定する対象のオブジェクト
  void
  instantiate_attribute(const PtAttrInst* pt_attr,
			bool def,
			const VlObj* obj);

};

END_NAMESPACE_YM_VERILOG

#endif // ATTRGEN_H
