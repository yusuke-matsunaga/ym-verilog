#ifndef OBJDICT_H
#define OBJDICT_H

/// @file ObjDict.h
/// @brief ObjDict のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym/verilog.h"
#include "ym/FileRegion.h"
#include "ym/vl/VlFwd.h"
#include "ElbFwd.h"
#include "HierName.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ElbObjHandle ObjDict.h "ObjDict.h"
//////////////////////////////////////////////////////////////////////
class ElbObjHandle
{
public:

  /// @brief コンストラクタ
  ElbObjHandle();

  /// @brief デストラクタ
  virtual
  ~ElbObjHandle();


public:

  /// @brief VlNamedObj を返す．
  virtual
  const VlNamedObj*
  obj() = 0;

  /// @brief オブジェクトの型を返す．
  VpiObjType
  type();

  /// @brief ファイル位置の取得
  virtual
  FileRegion
  file_region();

  /// @brief オブジェクトの階層付き名前を返す．
  string
  full_name();

  /// @brief 配列要素を返す．
  /// @note このクラスでは nullptr を返す．
  virtual
  const VlNamedObj*
  array_elem(int index);

  /// @brief ElbDecl を返す．
  /// @note このクラスでは nullptr を返す．
  virtual
  ElbDecl*
  decl();

  /// @brief ElbDeclArray を返す．
  /// @note このクラスでは nullptr を返す．
  virtual
  ElbDeclArray*
  declarray();

  /// @brief ElbParameter を返す．
  /// @note このクラスでは nullptr を返す．
  virtual
  ElbParameter*
  parameter();

  /// @brief ElbTaskFunc を返す．
  /// @note このクラスでは nullptr を返す．
  virtual
  ElbTaskFunc*
  taskfunc();

  /// @brief ElbModuleArray を返す．
  /// @note このクラスでは nullptr を返す．
  virtual
  ElbModuleArray*
  module_array();

  /// @brief ElbPrimArray を返す．
  /// @note このクラスでは nullptr を返す．
  virtual
  ElbPrimArray*
  prim_array();

  /// @brief ElbPrimitive を返す．
  /// @note このクラスでは nullptr を返す．
  virtual
  ElbPrimitive*
  primitive();

  /// @brief ElbGfRoot を返す．
  /// @note このクラスでは nullptr を返す．
  virtual
  ElbGfRoot*
  gfroot();

  /// @brief ElbGenvar を返す．
  /// @note このクラスでは nullptr を返す．
  virtual
  ElbGenvar*
  genvar();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

};


//////////////////////////////////////////////////////////////////////
/// @class ElbTaskFuncHandle
//////////////////////////////////////////////////////////////////////
class ElbTaskFuncHandle :
  public ElbObjHandle
{
public:

  /// @brief コンストラクタ
  ElbTaskFuncHandle(ElbTaskFunc* obj);

  /// @brief デストラクタ
  virtual
  ~ElbTaskFuncHandle();


public:

  /// @brief VlNamedObj を返す．
  const VlNamedObj*
  obj() override;

  /// @brief ElbTaskFunc を返す．
  ElbTaskFunc*
  taskfunc() override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象のオブジェクト
  ElbTaskFunc* mObj;

};


//////////////////////////////////////////////////////////////////////
/// @class ElbDeclHandle
//////////////////////////////////////////////////////////////////////
class ElbDeclHandle :
  public ElbObjHandle
{
public:

  /// @brief コンストラクタ
  ElbDeclHandle(ElbDecl* obj);

  /// @brief デストラクタ
  virtual
  ~ElbDeclHandle();


public:

  /// @brief VlNamedObj を返す．
  const VlNamedObj*
  obj() override;

  /// @brief ElbDecl を返す．
  ElbDecl*
  decl() override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象のオブジェクト
  ElbDecl* mObj;

};


//////////////////////////////////////////////////////////////////////
/// @class ElbDeclArrayHandle
//////////////////////////////////////////////////////////////////////
class ElbDeclArrayHandle :
  public ElbObjHandle
{
public:

  /// @brief コンストラクタ
  ElbDeclArrayHandle(ElbDeclArray* obj);

  /// @brief デストラクタ
  virtual
  ~ElbDeclArrayHandle();


public:

  /// @brief VlNamedObj を返す．
  const VlNamedObj*
  obj() override;

  /// @brief ElbDeclArray を返す．
  ElbDeclArray*
  declarray() override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象のオブジェクト
  ElbDeclArray* mObj;

};


//////////////////////////////////////////////////////////////////////
/// @class ElbParamHandle
//////////////////////////////////////////////////////////////////////
class ElbParamHandle :
  public ElbObjHandle
{
public:

  /// @brief コンストラクタ
  ElbParamHandle(ElbParameter* obj);

  /// @brief デストラクタ
  virtual
  ~ElbParamHandle();


public:

  /// @brief VlNamedObj を返す．
  const VlNamedObj*
  obj() override;

  /// @brief ElbParameterを返す．
  ElbParameter*
  parameter() override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象のオブジェクト
  ElbParameter* mObj;

};


//////////////////////////////////////////////////////////////////////
/// @class ElbModuleArrayHandle
//////////////////////////////////////////////////////////////////////
class ElbModuleArrayHandle :
  public ElbObjHandle
{
public:

  /// @brief コンストラクタ
  ElbModuleArrayHandle(ElbModuleArray* obj);

  /// @brief デストラクタ
  virtual
  ~ElbModuleArrayHandle();


public:

  /// @brief VlNamedObj を返す．
  const VlNamedObj*
  obj() override;

  /// @brief 配列要素を返す．
  const VlNamedObj*
  array_elem(int index) override;

  /// @brief ElbModuleArray を返す．
  ElbModuleArray*
  module_array() override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象のオブジェクト
  ElbModuleArray* mObj;

};


//////////////////////////////////////////////////////////////////////
/// @class ElbPrimArrayHandle
//////////////////////////////////////////////////////////////////////
class ElbPrimArrayHandle :
  public ElbObjHandle
{
public:

  /// @brief コンストラクタ
  ElbPrimArrayHandle(ElbPrimArray* obj);

  /// @brief デストラクタ
  virtual
  ~ElbPrimArrayHandle();


public:

  /// @brief VlNamedObj を返す．
  const VlNamedObj*
  obj() override;

  /// @brief ElbPrimArray を返す．
  ElbPrimArray*
  prim_array() override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象のオブジェクト
  ElbPrimArray* mObj;

};


//////////////////////////////////////////////////////////////////////
/// @class ElbPrimitiveHandle
//////////////////////////////////////////////////////////////////////
class ElbPrimitiveHandle :
  public ElbObjHandle
{
public:

  /// @brief コンストラクタ
  ElbPrimitiveHandle(ElbPrimitive* obj);

  /// @brief デストラクタ
  virtual
  ~ElbPrimitiveHandle();


public:

  /// @brief VlNamedObj を返す．
  const VlNamedObj*
  obj() override;

  /// @brief ElbPrimitive を返す．
  ElbPrimitive*
  primitive() override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象のオブジェクト
  ElbPrimitive* mObj;

};


//////////////////////////////////////////////////////////////////////
/// @class ElbScopeHandle
//////////////////////////////////////////////////////////////////////
class ElbScopeHandle :
  public ElbObjHandle
{
public:

  /// @brief コンストラクタ
  ElbScopeHandle(const VlNamedObj* obj);

  /// @brief デストラクタ
  virtual
  ~ElbScopeHandle();


public:

  /// @brief VlNamedObj を返す．
  const VlNamedObj*
  obj() override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象のオブジェクト
  const VlNamedObj* mObj;

};


//////////////////////////////////////////////////////////////////////
/// @class ElbGfRootHandle
//////////////////////////////////////////////////////////////////////
class ElbGfRootHandle :
  public ElbObjHandle
{
public:

  /// @brief コンストラクタ
  ElbGfRootHandle(ElbGfRoot* obj);

  /// @brief デストラクタ
  virtual
  ~ElbGfRootHandle();


public:

  /// @brief VlNamedObj を返す．
  const VlNamedObj*
  obj() override;

  /// @brief 配列要素を返す．
  const VlNamedObj*
  array_elem(int index) override;

  /// @brief ElbGfRoot を返す．
  ElbGfRoot*
  gfroot() override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象のオブジェクト
  ElbGfRoot* mObj;

};


//////////////////////////////////////////////////////////////////////
/// @class ElbGenvarHandle
//////////////////////////////////////////////////////////////////////
class ElbGenvarHandle :
  public ElbObjHandle
{
public:

  /// @brief コンストラクタ
  ElbGenvarHandle(ElbGenvar* obj);

  /// @brief デストラクタ
  virtual
  ~ElbGenvarHandle();


public:

  /// @brief VlNamedObj を返す．
  const VlNamedObj*
  obj() override;

  /// @brief ElbGenvar を返す．
  ElbGenvar*
  genvar() override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象のオブジェクト
  ElbGenvar* mObj;

};


//////////////////////////////////////////////////////////////////////
/// @class ObjDict ObjDict.h "ObjDict.h"
/// @brief VlNamedObj を格納するハッシュ表
//////////////////////////////////////////////////////////////////////
class ObjDict
{
public:

  /// @brief コンストラクタ
  ObjDict() = default;

  /// @brief デストラクタ
  ~ObjDict() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // ObjDict の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を空にする．
  void
  clear();

  /// @brief 要素を追加する．
  void
  add(const VlNamedObj* obj);

  /// @brief 要素を追加する．
  void
  add(ElbTaskFunc* obj);

  /// @brief 要素を追加する．
  void
  add(ElbDecl* obj);

  /// @brief 要素を追加する．
  void
  add(ElbDeclArray* obj);

  /// @brief 要素を追加する．
  void
  add(ElbParameter* obj);

  /// @brief 要素を追加する．
  void
  add(ElbModuleArray* obj);

  /// @brief 要素を追加する．
  void
  add(ElbPrimArray* obj);

  /// @brief 要素を追加する．
  void
  add(ElbPrimitive* obj);

  /// @brief 要素を追加する．
  void
  add(ElbGfRoot* obj);

  /// @brief 要素を追加する．
  void
  add(ElbGenvar* obj);

  /// @brief 名前から該当する要素を検索する．
  /// @note なければ nullptr を返す．
  ElbObjHandle*
  find(const VlNamedObj* parent,
       const char* name) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ハンドルを追加する．
  void
  add_handle(const VlNamedObj* parent,
	     const char* name,
	     ElbObjHandle* handle);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で使用されるデータ構造
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // HierName をキーにして ElbObjHandle を納めるハッシュ表
  unordered_map<HierName, ElbObjHandle*, HierNameHash> mHash;

  // ElbObjHandle のリスト
  vector<unique_ptr<ElbObjHandle>> mHandleList;

};


END_NAMESPACE_YM_VERILOG

#endif // OBJDICT_H
