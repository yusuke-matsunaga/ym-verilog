
/// @file ElbMgr.cc
/// @brief ElbMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "elaborator/ElbMgr.h"

#include "ym/pt/PtMisc.h"
#include "ym/vl/VlUserSystf.h"
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

#include "parser/PtiArray.h"


BEGIN_NAMESPACE_YM_VERILOG

// @brief コンストラクタ
ElbMgr::ElbMgr()
{
}

// @brief デストラクタ
ElbMgr::~ElbMgr()
{
}

// @brief 内容をクリアする．
void
ElbMgr::clear()
{
  mUdpList.clear();
  mUdpHash.clear();
  mTopmoduleList.clear();
  mSystfHash.clear();
  mTagDict.clear();
  mAttrHash.clear();
  mTopLevel = nullptr;
}

// @brief UDP 定義のリストを返す．
const vector<const VlUdpDefn*>&
ElbMgr::udp_list() const
{
  return mUdpList;
}

// @brief 名前から UDP 定義を取出す．
// @param[in] name 名前
// @return name という名の UDP を返す．
// @return なければ nullptr を返す．
const VlUdpDefn*
ElbMgr::find_udp(const char* name) const
{
  if ( mUdpHash.count(name) > 0 ) {
    return mUdpHash.at(name);
  }
  else {
    return nullptr;
  }
}

// @brief UDP を登録する．
// @param[in] def_name 定義名
// @param[in] udp 登録する UDP
void
ElbMgr::reg_udp(const char* def_name,
		const VlUdpDefn* udp)
{
  mUdpList.push_back(udp);
  mUdpHash[def_name] = udp;
}

// @brief グローバルスコープを登録する．
void
ElbMgr::reg_toplevel(const VlNamedObj* toplevel)
{
  mTopLevel = toplevel;
}

// @brief topmodule のリストを返す．
const vector<const VlModule*>&
ElbMgr::topmodule_list() const
{
  return mTopmoduleList;
}

// @brief 名前から UserSystf を取出す．
// @param[in] name 名前
// @return name という名のユーザー定義関数を返す．
// @return なければ nullptr を返す．
const VlUserSystf*
ElbMgr::find_user_systf(const char* name) const
{
  if ( mSystfHash.count(name) > 0 ) {
    return mSystfHash.at(name);
  }
  else {
    return nullptr;
  }
}

// @brief システムタスク/システム関数を登録する．
void
ElbMgr::reg_user_systf(const VlUserSystf* systf)
{
  mSystfHash[systf->name()] = systf;
}

// @brief internal scope を登録する．
// @param[in] obj 登録するオブジェクト
void
ElbMgr::reg_internalscope(const VlNamedObj* obj)
{
  mTagDict.add_internalscope(obj);
}

// @brief 宣言要素を登録する．
// @param[in] tag タグ
// @param[in] obj 登録するオブジェクト
void
ElbMgr::reg_decl(int tag,
		 const VlDecl* obj)
{
  if ( tag ) {
    mTagDict.add_decl(tag, obj);
  }
}

// @brief 配列型の宣言要素を登録する．
// @param[in] tag タグ
// @param[in] obj 登録するオブジェクト
void
ElbMgr::reg_declarray(int tag,
		      const VlDeclArray* obj)
{
  if ( tag ) {
    if ( tag == vpiVariables ) {
      // ちょっと汚い補正
      tag += 100;
    }
    mTagDict.add_declarray(tag, obj);
  }
}

// @brief defparam を登録する．
// @param[in] obj 登録するオブジェクト
void
ElbMgr::reg_defparam(const VlDefParam* obj)
{
  mTagDict.add_defparam(obj);
}

// @brief paramassign を登録する．
// @param[in] obj 登録するオブジェクト
void
ElbMgr::reg_paramassign(const VlParamAssign* obj)
{
  mTagDict.add_paramassign(obj);
}

// @brief モジュール配列を登録する．
// @param[in] obj 登録するオブジェクト
void
ElbMgr::reg_modulearray(const VlModuleArray* obj)
{
  mTagDict.add_modulearray(obj);
}

// @brief モジュールを登録する．
void
ElbMgr::reg_module(const VlModule* obj)
{
  mTagDict.add_module(obj);
  if ( obj->parent() == mTopLevel ) {
    mTopmoduleList.push_back(obj);
  }
}

// @brief プリミティブ配列を登録する．
// @param[in] obj 登録するオブジェクト
void
ElbMgr::reg_primarray(const VlPrimArray* obj)
{
  mTagDict.add_primarray(obj);
}

// @brief プリミティブを登録する．
// @param[in] obj 登録するオブジェクト
void
ElbMgr::reg_primitive(const VlPrimitive* obj)
{
  mTagDict.add_primitive(obj);
}

// @brief タスクを登録する．
// @param[in] obj 登録するオブジェクト
void
ElbMgr::reg_task(const VlTaskFunc* obj)
{
  mTagDict.add_task(obj);
}

// @brief 関数を登録する．
// @param[in] obj 登録するオブジェクト
void
ElbMgr::reg_function(const VlTaskFunc* obj)
{
  mTagDict.add_function(obj);
}

// @brief continuous assignment を登録する．
// @param[in] obj 登録するオブジェクト
void
ElbMgr::reg_contassign(const VlContAssign* obj)
{
  mTagDict.add_contassign(obj);
}

// @brief process を登録する．
// @param[in] obj 登録するオブジェクト
void
ElbMgr::reg_process(const VlProcess* obj)
{
  mTagDict.add_process(obj);
}

// @brief 属性リストを登録する．
// @param[in] obj 対象のオブジェクト
// @param[in] def 定義側の属性の時 true とするフラグ
// @param[in] attr_list 属性リスト
void
ElbMgr::reg_attr(const VlObj* obj,
		 bool def,
		 ElbAttrList* attr_list)
{
  mAttrHash.add(obj, def, attr_list);
}

END_NAMESPACE_YM_VERILOG
