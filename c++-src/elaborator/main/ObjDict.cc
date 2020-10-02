
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

#define DOUT cerr

const ymuint debug_none       = 0x00000000;
const ymuint debug_objdict    = 0x00000001;
const ymuint debug_find_scope = 0x00000010;
const ymuint debug_all        = 0xFFFFFFFF;

#if 1
const ymuint debug = debug_none;
#else
const ymuint debug = debug_all;
#endif

//////////////////////////////////////////////////////////////////////
// クラス ObjHandle
//////////////////////////////////////////////////////////////////////

// @brief VlScope を返す．
const VlScope*
ObjHandle::scope() const
{
  return nullptr;
}

// @brief VlModule を返す．
const VlModule*
ObjHandle::module() const
{
  return nullptr;
}

// @brief VlModuleArray を返す．
const VlModuleArray*
ObjHandle::module_array() const
{
  return nullptr;
}

// @brief ElbTaskFunc を返す．
const VlTaskFunc*
ObjHandle::taskfunc() const
{
  return nullptr;
}

// @brief 配列要素の VlScope を返す．
const VlScope*
ObjHandle::array_elem(int index) const
{
  return nullptr;
}

// @brief ElbDecl を返す．
ElbDecl*
ObjHandle::decl() const
{
  return nullptr;
}

// @brief ElbDeclArray を返す．
const VlDeclArray*
ObjHandle::declarray() const
{
  return nullptr;
}

// @brief ElbParameter を返す．
ElbParameter*
ObjHandle::parameter() const
{
  return nullptr;
}

// @brief ElbPrimArray を返す．
const VlPrimArray*
ObjHandle::prim_array() const
{
  return nullptr;
}

// @brief ElbPrimitive を返す．
const VlPrimitive*
ObjHandle::primitive() const
{
  return nullptr;
}

// @brief ElbGfRoot を返す．
ElbGfRoot*
ObjHandle::gfroot() const
{
  return nullptr;
}

// @brief ElbGenvar を返す．
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
  for ( auto c: name() ) {
    h = h * 37 + static_cast<SizeType>(c);
  }
  return ((reinterpret_cast<ympuint>(parent_scope()) * h) >> 8);
}


//////////////////////////////////////////////////////////////////////
// クラス ElbScopeHandle
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ElbScopeHandle::ElbScopeHandle(const VlScope* obj) :
  mObj{obj}
{
}

// @brief 親のスコープを返す．
const VlScope*
ElbScopeHandle::parent_scope() const
{
  return mObj->parent_scope();
}

// @brief オブジェクトの名前を返す．
string
ElbScopeHandle::name() const
{
  return mObj->name();
}

// @brief オブジェクトの型を返す．
VpiObjType
ElbScopeHandle::type() const
{
  return VpiObjType::Scope;
}

// @brief ファイル位置の取得
FileRegion
ElbScopeHandle::file_region() const
{
  return FileRegion();
}

// @brief オブジェクトの階層付き名前を返す．
string
ElbScopeHandle::full_name() const
{
  return mObj->full_name();
}

// @brief VlScope を返す．
const VlScope*
ElbScopeHandle::scope() const
{
  return mObj;
}


//////////////////////////////////////////////////////////////////////
// クラス KeyObjHandle
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のスコープ
// @param[in] name 名前
KeyObjHandle::KeyObjHandle(const VlScope* parent,
			   const string& name) :
  mParent{parent},
  mName{name}
{
}

// @brief 親のスコープを返す．
const VlScope*
KeyObjHandle::parent_scope() const
{
  return mParent;
}

// @brief オブジェクトの名前を返す．
string
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
// クラス NamedObjHandle
//////////////////////////////////////////////////////////////////////

// @brief 親のスコープを返す．
const VlScope*
NamedObjHandle::parent_scope() const
{
  return _namedobj()->parent_scope();
}

// @brief オブジェクトの名前を返す．
string
NamedObjHandle::name() const
{
  return _namedobj()->name();
}

// @brief オブジェクトの型を返す．
VpiObjType
NamedObjHandle::type() const
{
  return _namedobj()->type();
}

// @brief ファイル位置の取得
FileRegion
NamedObjHandle::file_region() const
{
  return _namedobj()->file_region();
}

// @brief オブジェクトの階層付き名前を返す．
string
NamedObjHandle::full_name() const
{
  return _namedobj()->full_name();
}


//////////////////////////////////////////////////////////////////////
// クラス ElbTaskFuncHandle
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ElbTaskFuncHandle::ElbTaskFuncHandle(const VlTaskFunc* obj) :
  mObj{obj}
{
}

// @brief ElbTaskFunc を返す．
const VlTaskFunc*
ElbTaskFuncHandle::taskfunc() const
{
  return mObj;
}

// @brief 対象のオブジェクトを返す．
const VlNamedObj*
ElbTaskFuncHandle::_namedobj() const
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

// @brief ElbDecl を返す．
ElbDecl*
ElbDeclHandle::decl() const
{
  return mObj;
}

// @brief 対象のオブジェクトを返す．
const VlNamedObj*
ElbDeclHandle::_namedobj() const
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

// @brief ElbDeclArray を返す．
const VlDeclArray*
ElbDeclArrayHandle::declarray() const
{
  return mObj;
}

// @brief 対象のオブジェクトを返す．
const VlNamedObj*
ElbDeclArrayHandle::_namedobj() const
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

// @brief ElbParameter を返す．
ElbParameter*
ElbParamHandle::parameter() const
{
  return mObj;
}

// @brief 対象のオブジェクトを返す．
const VlNamedObj*
ElbParamHandle::_namedobj() const
{
  return mObj;
}


//////////////////////////////////////////////////////////////////////
// クラス ElbModuleHandle
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ElbModuleHandle::ElbModuleHandle(const VlModule* obj) :
  mObj{obj}
{
}

// @brief VlModule を返す．
const VlModule*
ElbModuleHandle::module() const
{
  return mObj;
}

// @brief 対象のオブジェクトを返す．
const VlNamedObj*
ElbModuleHandle::_namedobj() const
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

// @brief 配列要素の VlScope を返す．
const VlScope*
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

// @brief 対象のオブジェクトを返す．
const VlNamedObj*
ElbModuleArrayHandle::_namedobj() const
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

// @brief ElbPrimArray を返す．
const VlPrimArray*
ElbPrimArrayHandle::prim_array() const
{
  return mObj;
}

// @brief 対象のオブジェクトを返す．
const VlNamedObj*
ElbPrimArrayHandle::_namedobj() const
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

// @brief ElbPrimitive を返す．
const VlPrimitive*
ElbPrimitiveHandle::primitive() const
{
  return mObj;
}

// @brief 対象のオブジェクトを返す．
const VlNamedObj*
ElbPrimitiveHandle::_namedobj() const
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

// @brief 配列要素の VlNamedObj を返す．
const VlScope*
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

// @brief 対象のオブジェクトを返す．
const VlNamedObj*
ElbGfRootHandle::_namedobj() const
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

// @brief ElbGenvar を返す．
ElbGenvar*
ElbGenvarHandle::genvar() const
{
  return mObj;
}

// @brief 対象のオブジェクトを返す．
const VlNamedObj*
ElbGenvarHandle::_namedobj() const
{
  return mObj;
}


//////////////////////////////////////////////////////////////////////
// VlNamedObj を格納するハッシュ表
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ObjDict::ObjDict()
{
}

// @brief デストラクタ
ObjDict::~ObjDict()
{
  clear();
}

// @brief 内容を空にする．
void
ObjDict::clear()
{
  for ( auto handle: mHash ) {
    delete handle;
  }
  mHash.clear();
}

// @brief 要素を追加する．
void
ObjDict::add(const VlScope* obj)
{
  auto handle{new ElbScopeHandle(obj)};
  add_handle(handle);
}

// @brief 要素を追加する．
void
ObjDict::add(const VlTaskFunc* obj)
{
  if ( debug & debug_objdict ) {
    auto parent{obj->parent_scope()};
    DOUT << "reg_taskfunc( " << obj->name() << " @ "
	 << parent->full_name()
	 << " ["
	 << hex << reinterpret_cast<ympuint>(parent) << dec
	 << "] )" << endl << endl;
  }

  auto handle{new ElbTaskFuncHandle(obj)};
  add_handle(handle);
}

// @brief 要素を追加する．
void
ObjDict::add(ElbDecl* obj)
{
  if ( debug & debug_objdict ) {
    auto parent{obj->parent_scope()};
    DOUT << "reg_decl( " << obj->name() << " @ "
	 << parent->full_name()
	 << " ["
	 << hex << reinterpret_cast<ympuint>(parent) << dec
	 << "] )" << endl << endl;
  }

  auto handle{new ElbDeclHandle(obj)};
  add_handle(handle);
}

// @brief 要素を追加する．
void
ObjDict::add(const VlDeclArray* obj)
{
  if ( debug & debug_objdict ) {
    auto parent{obj->parent_scope()};
    DOUT << "reg_declarray( " << obj->name() << " @ "
	 << parent->full_name()
	 << " ["
	 << hex << reinterpret_cast<ympuint>(parent) << dec
	 << "] )" << endl << endl;
  }

  auto handle{new ElbDeclArrayHandle(obj)};
  add_handle(handle);
}

// @brief 要素を追加する．
void
ObjDict::add(ElbParameter* obj)
{
  auto handle{new ElbParamHandle(obj)};
  add_handle(handle);
}

// @brief 要素を追加する．
void
ObjDict::add(const VlModule* obj)
{
  if ( debug & debug_objdict ) {
    auto parent{obj->parent_scope()};
    DOUT << "reg_module( " << obj->name() << " @ "
	 << parent->full_name()
	 << " ["
	 << hex << reinterpret_cast<ympuint>(parent) << dec
	 << "] )" << endl << endl;
  }

  auto handle{new ElbModuleHandle(obj)};
  add_handle(handle);
}

// @brief 要素を追加する．
void
ObjDict::add(const VlModuleArray* obj)
{
  if ( debug & debug_objdict ) {
    auto parent{obj->parent_scope()};
    DOUT << "reg_modulearray( " << obj->name() << " @ "
	 << parent->full_name()
	 << " ["
	 << hex << reinterpret_cast<ympuint>(parent) << dec
	 << "] )" << endl << endl;
  }

  auto handle{new ElbModuleArrayHandle(obj)};
  add_handle(handle);
}

// @brief 要素を追加する．
void
ObjDict::add(const VlPrimArray* obj)
{
  if ( obj->name() != string() ) {
    if ( debug & debug_objdict ) {
      auto parent{obj->parent_scope()};
      DOUT << "reg_primarray( " << obj->name() << " @ "
	   << parent->full_name()
	 << " ["
	 << hex << reinterpret_cast<ympuint>(parent) << dec
	 << "] )" << endl << endl;
    }
    auto handle{new ElbPrimArrayHandle(obj)};
    add_handle(handle);
  }
}

// @brief 要素を追加する．
void
ObjDict::add(const VlPrimitive* obj)
{
  if ( obj->name() != string() ) {
    if ( debug & debug_objdict ) {
      auto parent{obj->parent_scope()};
      DOUT << "reg_primitive( " << obj->name() << " @ "
	   << parent->full_name()
	   << " ["
	   << hex << reinterpret_cast<ympuint>(parent) << dec
	   << "] )" << endl << endl;
    }
    auto handle{new ElbPrimitiveHandle(obj)};
    add_handle(handle);
  }
}

// @brief 要素を追加する．
void
ObjDict::add(ElbGfRoot* obj)
{
  if ( debug & debug_objdict ) {
    auto parent{obj->parent_scope()};
    DOUT << "reg_gfroot( " << obj->name() << " @ "
	 << parent->full_name()
	 << " ["
	 << hex << reinterpret_cast<ympuint>(parent) << dec
	 << "] )" << endl << endl;
  }

  auto handle{new ElbGfRootHandle(obj)};
  add_handle(handle);
}

// @brief 要素を追加する．
void
ObjDict::add(ElbGenvar* obj)
{
  if ( debug & debug_objdict ) {
    auto parent{obj->parent_scope()};
    DOUT << "reg_genvar( " << obj->name() << " @ "
	 << parent->full_name()
	 << " ["
	 << hex << reinterpret_cast<ympuint>(parent) << dec
	 << "] )" << endl << endl;
  }

  auto handle{new ElbGenvarHandle(obj)};
  add_handle(handle);
}

// @brief セルを追加する．
void
ObjDict::add_handle(ObjHandle* handle)
{
  mHash.emplace(handle);
}

// @brief 名前から該当する要素を検索する．
ObjHandle*
ObjDict::find(const VlScope* parent,
	      const string& name) const
{
  if ( debug & debug_find_scope ) {
    DOUT << "find_obj( " << name << ", @ "
	 << parent->full_name()
	 << " ["
	 << hex << reinterpret_cast<ympuint>(parent) << dec
	 << "] )" << endl << endl;
  }

  KeyObjHandle key{parent, name};
  auto p{mHash.find(&key)};
  if ( p != mHash.end() ) {
    if ( debug & debug_find_scope ) {
      DOUT << "--> Found"
	   << endl << endl;
    }

    return *p;
  }
  else {
    if ( debug & debug_find_scope ) {
      DOUT << "--> Not Found"
	   << endl << endl;
    }

    return nullptr;
  }
}

END_NAMESPACE_YM_VERILOG
