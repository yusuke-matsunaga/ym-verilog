
/// @file ObjDict.cc
/// @brief ObjDict の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "elb/ObjDict.h"

#include "ym/vl/VlNamedObj.h"

#include "elb/ElbDecl.h"
#include "elb/ElbParameter.h"
#include "elb/ElbModule.h"
#include "elb/ElbTaskFunc.h"
#include "elb/ElbPrimitive.h"
#include "elb/ElbGfRoot.h"
#include "elb/ElbGenvar.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス ElbObjHandle
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ElbObjHandle::ElbObjHandle()
{
}

// @brief デストラクタ
ElbObjHandle::~ElbObjHandle()
{
}

// @brief オブジェクトの型を返す．
VpiObjType
ElbObjHandle::type()
{
  return obj()->type();
}

// @brief ファイル位置の取得
FileRegion
ElbObjHandle::file_region()
{
  return obj()->file_region();
}

// @brief オブジェクトの階層付き名前を返す．
string
ElbObjHandle::full_name()
{
  return obj()->full_name();
}

// @brief 配列要素の VlNamedObj を返す．
// @note このクラスでは nullptr を返す．
const VlNamedObj*
ElbObjHandle::array_elem(int index)
{
  return nullptr;
}

// @brief ElbDecl を返す．
// @note このクラスでは nullptr を返す．
ElbDecl*
ElbObjHandle::decl()
{
  return nullptr;
}

// @brief ElbDeclArray を返す．
// @note このクラスでは nullptr を返す．
ElbDeclArray*
ElbObjHandle::declarray()
{
  return nullptr;
}

// @brief ElbParameter を返す．
// @note このクラスでは nullptr を返す．
ElbParameter*
ElbObjHandle::parameter()
{
  return nullptr;
}

// @brief ElbTaskFunc を返す．
// @note このクラスでは nullptr を返す．
ElbTaskFunc*
ElbObjHandle::taskfunc()
{
  return nullptr;
}

// @brief ElbModuleArray を返す．
// @note このクラスでは nullptr を返す．
ElbModuleArray*
ElbObjHandle::module_array()
{
  return nullptr;
}

// @brief ElbPrimArray を返す．
// @note このクラスでは nullptr を返す．
ElbPrimArray*
ElbObjHandle::prim_array()
{
  return nullptr;
}

// @brief ElbPrimitive を返す．
// @note このクラスでは nullptr を返す．
ElbPrimitive*
ElbObjHandle::primitive()
{
  return nullptr;
}

// @brief ElbGfRoot を返す．
// @note このクラスでは nullptr を返す．
ElbGfRoot*
ElbObjHandle::gfroot()
{
  return nullptr;
}

// @brief ElbGenvar を返す．
// @note このクラスでは nullptr を返す．
ElbGenvar*
ElbObjHandle::genvar()
{
  return nullptr;
}


//////////////////////////////////////////////////////////////////////
// クラス ElbTaskFuncHandle
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ElbTaskFuncHandle::ElbTaskFuncHandle(ElbTaskFunc* obj) :
  mObj(obj)
{
}

// @brief デストラクタ
ElbTaskFuncHandle::~ElbTaskFuncHandle()
{
}

// @brief VlNamedObj を返す．
const VlNamedObj*
ElbTaskFuncHandle::obj()
{
  return mObj;
}

// @brief ElbTaskFunc を返す．
ElbTaskFunc*
ElbTaskFuncHandle::taskfunc()
{
  return mObj;
}


//////////////////////////////////////////////////////////////////////
// クラス ElbDeclHandle
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ElbDeclHandle::ElbDeclHandle(ElbDecl* obj) :
  mObj(obj)
{
}

// @brief デストラクタ
ElbDeclHandle::~ElbDeclHandle()
{
}

// @brief VlNamedObj を返す．
const VlNamedObj*
ElbDeclHandle::obj()
{
  return mObj;
}

// @brief ElbDecl を返す．
ElbDecl*
ElbDeclHandle::decl()
{
  return mObj;
}


//////////////////////////////////////////////////////////////////////
// クラス ElbDeclArrayHandle
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ElbDeclArrayHandle::ElbDeclArrayHandle(ElbDeclArray* obj) :
  mObj(obj)
{
}

// @brief デストラクタ
ElbDeclArrayHandle::~ElbDeclArrayHandle()
{
}

// @brief VlNamedObj を返す．
const VlNamedObj*
ElbDeclArrayHandle::obj()
{
  return mObj;
}

// @brief ElbDeclArray を返す．
ElbDeclArray*
ElbDeclArrayHandle::declarray()
{
  return mObj;
}


//////////////////////////////////////////////////////////////////////
// クラス ElbParamHandle
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ElbParamHandle::ElbParamHandle(ElbParameter* obj) :
  mObj(obj)
{
}

// @brief デストラクタ
ElbParamHandle::~ElbParamHandle()
{
}

// @brief VlNamedObj を返す．
const VlNamedObj*
ElbParamHandle::obj()
{
  return mObj;
}

// @brief ElbParameter を返す．
ElbParameter*
ElbParamHandle::parameter()
{
  return mObj;
}


//////////////////////////////////////////////////////////////////////
// クラス ElbModuleArrayHandle
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ElbModuleArrayHandle::ElbModuleArrayHandle(ElbModuleArray* obj) :
  mObj(obj)
{
}

// @brief デストラクタ
ElbModuleArrayHandle::~ElbModuleArrayHandle()
{
}

// @brief VlNamedObj を返す．
const VlNamedObj*
ElbModuleArrayHandle::obj()
{
  return mObj;
}

// @brief 配列要素の VlNamedObj を返す．
const VlNamedObj*
ElbModuleArrayHandle::array_elem(int index)
{
  return mObj->elem_by_index(index);
}

// @brief ElbModuleArray を返す．
ElbModuleArray*
ElbModuleArrayHandle::module_array()
{
  return mObj;
}


//////////////////////////////////////////////////////////////////////
// クラス ElbPrimArrayHandle
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ElbPrimArrayHandle::ElbPrimArrayHandle(ElbPrimArray* obj) :
  mObj(obj)
{
}

// @brief デストラクタ
ElbPrimArrayHandle::~ElbPrimArrayHandle()
{
}

// @brief VlNamedObj を返す．
const VlNamedObj*
ElbPrimArrayHandle::obj()
{
  return mObj;
}

// @brief ElbPrimArray を返す．
ElbPrimArray*
ElbPrimArrayHandle::prim_array()
{
  return mObj;
}


//////////////////////////////////////////////////////////////////////
// クラス VlPrimitive
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ElbPrimitiveHandle::ElbPrimitiveHandle(ElbPrimitive* obj) :
  mObj(obj)
{
}

// @brief デストラクタ
ElbPrimitiveHandle::~ElbPrimitiveHandle()
{
}

// @brief VlNamedObj を返す．
const VlNamedObj*
ElbPrimitiveHandle::obj()
{
  return mObj;
}

// @brief ElbPrimitive を返す．
ElbPrimitive*
ElbPrimitiveHandle::primitive()
{
  return mObj;
}


//////////////////////////////////////////////////////////////////////
// クラス ElbScopeHandle
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ElbScopeHandle::ElbScopeHandle(const VlNamedObj* obj) :
  mObj(obj)
{
}

// @brief デストラクタ
ElbScopeHandle::~ElbScopeHandle()
{
}

// @brief VlNamedObj を返す．
const VlNamedObj*
ElbScopeHandle::obj()
{
  return mObj;
}


//////////////////////////////////////////////////////////////////////
// クラス ElbGfRootHandle
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ElbGfRootHandle::ElbGfRootHandle(ElbGfRoot* obj) :
  mObj(obj)
{
}

// @brief デストラクタ
ElbGfRootHandle::~ElbGfRootHandle()
{
}

// @brief VlNamedObj を返す．
const VlNamedObj*
ElbGfRootHandle::obj()
{
  return mObj;
}

// @brief 配列要素の VlNamedObj を返す．
const VlNamedObj*
ElbGfRootHandle::array_elem(int index)
{
  return mObj->elem_by_index(index);
}

// @brief ElbGfRoot を返す．
ElbGfRoot*
ElbGfRootHandle::gfroot()
{
  return mObj;
}


//////////////////////////////////////////////////////////////////////
// クラス ElbGenvarHandle
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ElbGenvarHandle::ElbGenvarHandle(ElbGenvar* obj) :
  mObj(obj)
{
}

// @brief デストラクタ
ElbGenvarHandle::~ElbGenvarHandle()
{
}

// @brief VlNamedObj を返す．
const VlNamedObj*
ElbGenvarHandle::obj()
{
  return mObj;
}

// @brief ElbGenvar を返す．
ElbGenvar*
ElbGenvarHandle::genvar()
{
  return mObj;
}


//////////////////////////////////////////////////////////////////////
// VlNamedObj を格納するハッシュ表
//////////////////////////////////////////////////////////////////////

// @brief 内容を空にする．
void
ObjDict::clear()
{
  mHash.clear();
  mHandleList.clear();
}

// @brief 要素を追加する．
void
ObjDict::add(const VlNamedObj* obj)
{
  auto handle = new ElbScopeHandle(obj);
  add_handle(obj->parent(), obj->name(), handle);
}

// @brief 要素を追加する．
void
ObjDict::add(ElbTaskFunc* obj)
{
  auto handle = new ElbTaskFuncHandle(obj);
  add_handle(obj->parent(), obj->name(), handle);
}

// @brief 要素を追加する．
void
ObjDict::add(ElbDecl* obj)
{
  auto handle = new ElbDeclHandle(obj);
  add_handle(obj->parent(), obj->name(), handle);
}

// @brief 要素を追加する．
void
ObjDict::add(ElbDeclArray* obj)
{
  auto handle = new ElbDeclArrayHandle(obj);
  add_handle(obj->parent(), obj->name(), handle);
}

// @brief 要素を追加する．
void
ObjDict::add(ElbParameter* obj)
{
  auto handle = new ElbParamHandle(obj);
  add_handle(obj->parent(), obj->name(), handle);
}

// @brief 要素を追加する．
void
ObjDict::add(ElbModuleArray* obj)
{
  auto handle = new ElbModuleArrayHandle(obj);
  add_handle(obj->parent(), obj->name(), handle);
}

// @brief 要素を追加する．
void
ObjDict::add(ElbPrimArray* obj)
{
  auto handle = new ElbPrimArrayHandle(obj);
  add_handle(obj->parent(), obj->name(), handle);
}

// @brief 要素を追加する．
void
ObjDict::add(ElbPrimitive* obj)
{
  auto handle = new ElbPrimitiveHandle(obj);
  add_handle(obj->parent(), obj->name(), handle);
}

// @brief 要素を追加する．
void
ObjDict::add(ElbGfRoot* obj)
{
  auto handle = new ElbGfRootHandle(obj);
  add_handle(obj->parent(), obj->name(), handle);
}

// @brief 要素を追加する．
void
ObjDict::add(ElbGenvar* obj)
{
  auto handle = new ElbGenvarHandle(obj);
  add_handle(obj->parent(), obj->name(), handle);
}

// @brief セルを追加する．
void
ObjDict::add_handle(const VlNamedObj* parent,
		    const char* name,
		    ElbObjHandle* handle)
{
  HierName key{parent, name};
  mHash.emplace(key, handle);
  mHandleList.push_back(unique_ptr<ElbObjHandle>{handle});
}

// @brief 名前から該当する要素を検索する．
ElbObjHandle*
ObjDict::find(const VlNamedObj* parent,
	      const char* name) const
{
  HierName key{parent, name};
  if ( mHash.count(key) > 0 ) {
    return mHash.at(key);
  }
  return nullptr;
}

END_NAMESPACE_YM_VERILOG
