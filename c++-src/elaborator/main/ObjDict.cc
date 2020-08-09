
/// @file ObjDict.cc
/// @brief ObjDict の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.


#include "elaborator/ObjDict.h"
#include "ObjHandleSub.h"

#include "ym/vl/VlNamedObj.h"
#include "ym/vl/VlDecl.h"
#include "ym/vl/VlTaskFunc.h"

#include "elaborator/ElbParameter.h"
#include "elaborator/ElbModule.h"
#include "elaborator/ElbPrimitive.h"
#include "elaborator/ElbGfRoot.h"
#include "elaborator/ElbGenvar.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス ObjHandle
//////////////////////////////////////////////////////////////////////

// @brief 親のスコープを返す．
const VlNamedObj*
ObjHandle::parent() const
{
  return obj()->parent();
}

// @brief オブジェクトの名前を返す．
const char*
ObjHandle::name() const
{
  return obj()->name();
}

// @brief オブジェクトの型を返す．
VpiObjType
ObjHandle::type() const
{
  return obj()->type();
}

// @brief ファイル位置の取得
FileRegion
ObjHandle::file_region() const
{
  return obj()->file_region();
}

// @brief オブジェクトの階層付き名前を返す．
string
ObjHandle::full_name() const
{
  return obj()->full_name();
}

// @brief 配列要素の VlNamedObj を返す．
// @note このクラスでは nullptr を返す．
const VlNamedObj*
ObjHandle::array_elem(int index) const
{
  return nullptr;
}

// @brief ElbDecl を返す．
// @note このクラスでは nullptr を返す．
ElbDecl*
ObjHandle::decl() const
{
  return nullptr;
}

// @brief ElbDeclArray を返す．
// @note このクラスでは nullptr を返す．
const VlDeclArray*
ObjHandle::declarray() const
{
  return nullptr;
}

// @brief ElbParameter を返す．
// @note このクラスでは nullptr を返す．
ElbParameter*
ObjHandle::parameter() const
{
  return nullptr;
}

// @brief ElbTaskFunc を返す．
// @note このクラスでは nullptr を返す．
const VlTaskFunc*
ObjHandle::taskfunc() const
{
  return nullptr;
}

// @brief ElbModuleArray を返す．
// @note このクラスでは nullptr を返す．
const VlModuleArray*
ObjHandle::module_array() const
{
  return nullptr;
}

// @brief ElbPrimArray を返す．
// @note このクラスでは nullptr を返す．
const VlPrimArray*
ObjHandle::prim_array() const
{
  return nullptr;
}

// @brief ElbPrimitive を返す．
// @note このクラスでは nullptr を返す．
const VlPrimitive*
ObjHandle::primitive() const
{
  return nullptr;
}

// @brief ElbGfRoot を返す．
// @note このクラスでは nullptr を返す．
ElbGfRoot*
ObjHandle::gfroot() const
{
  return nullptr;
}

// @brief ElbGenvar を返す．
// @note このクラスでは nullptr を返す．
ElbGenvar*
ObjHandle::genvar() const
{
  return nullptr;
}

// @brief ハッシュ値を返す．
SizeType
ObjHandle::hash() const
{
  SizeType h = 0;
  SizeType c;
  for ( auto p = name(); (c = static_cast<SizeType>(*p)); ++ p ) {
    h = h * 37 + c;
  }
  return ((reinterpret_cast<ympuint>(parent()) * h) >> 8);
}


//////////////////////////////////////////////////////////////////////
// クラス KeyObjHandle
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のスコープ
// @param[in] name 名前
KeyObjHandle::KeyObjHandle(const VlNamedObj* parent,
			   const char* name) :
  mParent{parent},
  mName{name}
{
}

// @brief VlNamedObj を返す．
const VlNamedObj*
KeyObjHandle::obj() const
{
  return nullptr;
}

// @brief 親のスコープを返す．
const VlNamedObj*
KeyObjHandle::parent() const
{
  return mParent;
}

// @brief オブジェクトの名前を返す．
const char*
KeyObjHandle::name() const
{
  return mName;
}

// @brief オブジェクトの型を返す．
VpiObjType
KeyObjHandle::type() const
{
  return VpiObjType::Scope; // 嘘
}

// @brief ファイル位置の取得
FileRegion
KeyObjHandle::file_region() const
{
  return FileRegion();
}

// @brief オブジェクトの階層付き名前を返す．
string
KeyObjHandle::full_name() const
{
  return string();
}


//////////////////////////////////////////////////////////////////////
// クラス ElbTaskFuncHandle
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ElbTaskFuncHandle::ElbTaskFuncHandle(const VlTaskFunc* obj) :
  mObj{obj}
{
}

// @brief VlNamedObj を返す．
const VlNamedObj*
ElbTaskFuncHandle::obj() const
{
  return mObj;
}

// @brief ElbTaskFunc を返す．
const VlTaskFunc*
ElbTaskFuncHandle::taskfunc() const
{
  return mObj;
}


//////////////////////////////////////////////////////////////////////
// クラス ElbDeclHandle
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ElbDeclHandle::ElbDeclHandle(ElbDecl* obj) :
  mObj{obj}
{
}

// @brief VlNamedObj を返す．
const VlNamedObj*
ElbDeclHandle::obj() const
{
  return mObj;
}

// @brief ElbDecl を返す．
ElbDecl*
ElbDeclHandle::decl() const
{
  return mObj;
}


//////////////////////////////////////////////////////////////////////
// クラス ElbDeclArrayHandle
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ElbDeclArrayHandle::ElbDeclArrayHandle(const VlDeclArray* obj) :
  mObj{obj}
{
}

// @brief VlNamedObj を返す．
const VlNamedObj*
ElbDeclArrayHandle::obj() const
{
  return mObj;
}

// @brief ElbDeclArray を返す．
const VlDeclArray*
ElbDeclArrayHandle::declarray() const
{
  return mObj;
}


//////////////////////////////////////////////////////////////////////
// クラス ElbParamHandle
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ElbParamHandle::ElbParamHandle(ElbParameter* obj) :
  mObj{obj}
{
}

// @brief VlNamedObj を返す．
const VlNamedObj*
ElbParamHandle::obj() const
{
  return mObj;
}

// @brief ElbParameter を返す．
ElbParameter*
ElbParamHandle::parameter() const
{
  return mObj;
}


//////////////////////////////////////////////////////////////////////
// クラス ElbModuleArrayHandle
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ElbModuleArrayHandle::ElbModuleArrayHandle(const VlModuleArray* obj) :
  mObj{obj}
{
}

// @brief VlNamedObj を返す．
const VlNamedObj*
ElbModuleArrayHandle::obj() const
{
  return mObj;
}

// @brief 配列要素の VlNamedObj を返す．
const VlNamedObj*
ElbModuleArrayHandle::array_elem(int index) const
{
  return mObj->elem_by_index(index);
}

// @brief ElbModuleArray を返す．
const VlModuleArray*
ElbModuleArrayHandle::module_array() const
{
  return mObj;
}


//////////////////////////////////////////////////////////////////////
// クラス ElbPrimArrayHandle
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ElbPrimArrayHandle::ElbPrimArrayHandle(const VlPrimArray* obj) :
  mObj{obj}
{
}

// @brief VlNamedObj を返す．
const VlNamedObj*
ElbPrimArrayHandle::obj() const
{
  return mObj;
}

// @brief ElbPrimArray を返す．
const VlPrimArray*
ElbPrimArrayHandle::prim_array() const
{
  return mObj;
}


//////////////////////////////////////////////////////////////////////
// クラス VlPrimitive
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ElbPrimitiveHandle::ElbPrimitiveHandle(const VlPrimitive* obj) :
  mObj{obj}
{
}

// @brief VlNamedObj を返す．
const VlNamedObj*
ElbPrimitiveHandle::obj() const
{
  return mObj;
}

// @brief ElbPrimitive を返す．
const VlPrimitive*
ElbPrimitiveHandle::primitive() const
{
  return mObj;
}


//////////////////////////////////////////////////////////////////////
// クラス ElbNamedObjHandle
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ElbNamedObjHandle::ElbNamedObjHandle(const VlNamedObj* obj) :
  mObj{obj}
{
}

// @brief VlNamedObj を返す．
const VlNamedObj*
ElbNamedObjHandle::obj() const
{
  return mObj;
}


//////////////////////////////////////////////////////////////////////
// クラス ElbGfRootHandle
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ElbGfRootHandle::ElbGfRootHandle(ElbGfRoot* obj) :
  mObj{obj}
{
}

// @brief VlNamedObj を返す．
const VlNamedObj*
ElbGfRootHandle::obj() const
{
  return mObj;
}

// @brief 配列要素の VlNamedObj を返す．
const VlNamedObj*
ElbGfRootHandle::array_elem(int index) const
{
  return mObj->elem_by_index(index);
}

// @brief ElbGfRoot を返す．
ElbGfRoot*
ElbGfRootHandle::gfroot() const
{
  return mObj;
}


//////////////////////////////////////////////////////////////////////
// クラス ElbGenvarHandle
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ElbGenvarHandle::ElbGenvarHandle(ElbGenvar* obj) :
  mObj{obj}
{
}

// @brief VlNamedObj を返す．
const VlNamedObj*
ElbGenvarHandle::obj() const
{
  return mObj;
}

// @brief ElbGenvar を返す．
ElbGenvar*
ElbGenvarHandle::genvar() const
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
  auto handle = new ElbNamedObjHandle(obj);
  add_handle(handle);
}

// @brief 要素を追加する．
void
ObjDict::add(const VlTaskFunc* obj)
{
  auto handle = new ElbTaskFuncHandle(obj);
  add_handle(handle);
}

// @brief 要素を追加する．
void
ObjDict::add(ElbDecl* obj)
{
  auto handle = new ElbDeclHandle(obj);
  add_handle(handle);
}

// @brief 要素を追加する．
void
ObjDict::add(const VlDeclArray* obj)
{
  auto handle = new ElbDeclArrayHandle(obj);
  add_handle(handle);
}

// @brief 要素を追加する．
void
ObjDict::add(ElbParameter* obj)
{
  auto handle = new ElbParamHandle(obj);
  add_handle(handle);
}

// @brief 要素を追加する．
void
ObjDict::add(const VlModuleArray* obj)
{
  auto handle = new ElbModuleArrayHandle(obj);
  add_handle(handle);
}

// @brief 要素を追加する．
void
ObjDict::add(const VlPrimArray* obj)
{
  auto handle = new ElbPrimArrayHandle(obj);
  add_handle(handle);
}

// @brief 要素を追加する．
void
ObjDict::add(const VlPrimitive* obj)
{
  auto handle = new ElbPrimitiveHandle(obj);
  add_handle(handle);
}

// @brief 要素を追加する．
void
ObjDict::add(ElbGfRoot* obj)
{
  auto handle = new ElbGfRootHandle(obj);
  add_handle(handle);
}

// @brief 要素を追加する．
void
ObjDict::add(ElbGenvar* obj)
{
  auto handle = new ElbGenvarHandle(obj);
  add_handle(handle);
}

// @brief セルを追加する．
void
ObjDict::add_handle(ObjHandle* handle)
{
  mHash.emplace(handle);
  mHandleList.push_back(unique_ptr<ObjHandle>{handle});
}

// @brief 名前から該当する要素を検索する．
ObjHandle*
ObjDict::find(const VlNamedObj* parent,
	      const char* name) const
{
  KeyObjHandle key{parent, name};
  auto p = mHash.find(&key);
  if ( p != mHash.end() ) {
    return *p;
  }
  else {
    return nullptr;
  }
}

END_NAMESPACE_YM_VERILOG
