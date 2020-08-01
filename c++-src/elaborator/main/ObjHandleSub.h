#ifndef OBJHANDLESUB_H
#define OBJHANDLESUB_H

/// @file ObjHandleSub.h
/// @brief ObjHandleSub のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "elb/ObjHandle.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class KeyObjHandle KeyObjHandle.h "KeyObjHandle.h"
/// @brief 検索用のキーを表すクラス
//////////////////////////////////////////////////////////////////////
class KeyObjHandle :
  public ObjHandle
{
public:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] name 名前
  KeyObjHandle(const VlNamedObj* parent,
	       const char* name);

  /// @brief デストラクタ
  ~KeyObjHandle() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief VlNamedObj を返す．
  const VlNamedObj*
  obj() const override;

  /// @brief 親のスコープを返す．
  const VlNamedObj*
  parent() const override;

  /// @brief オブジェクトの名前を返す．
  const char*
  name() const override;

  /// @brief オブジェクトの型を返す．
  VpiObjType
  type() const override;

  /// @brief ファイル位置の取得
  FileRegion
  file_region() const override;

  /// @brief オブジェクトの階層付き名前を返す．
  string
  full_name() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親のスコープ
  const VlNamedObj* mParent;

  // 名前
  const char* mName;

};


//////////////////////////////////////////////////////////////////////
/// @class ElbTaskFuncHandle
//////////////////////////////////////////////////////////////////////
class ElbTaskFuncHandle :
  public ObjHandle
{
public:

  /// @brief コンストラクタ
  ElbTaskFuncHandle(ElbTaskFunc* obj);

  /// @brief デストラクタ
  ~ElbTaskFuncHandle() = default;


public:

  /// @brief VlNamedObj を返す．
  const VlNamedObj*
  obj() const override;

  /// @brief ElbTaskFunc を返す．
  ElbTaskFunc*
  taskfunc() const override;


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
  public ObjHandle
{
public:

  /// @brief コンストラクタ
  ElbDeclHandle(ElbDecl* obj);

  /// @brief デストラクタ
  ~ElbDeclHandle() = default;


public:

  /// @brief VlNamedObj を返す．
  const VlNamedObj*
  obj() const override;

  /// @brief ElbDecl を返す．
  ElbDecl*
  decl() const override;


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
  public ObjHandle
{
public:

  /// @brief コンストラクタ
  ElbDeclArrayHandle(ElbDeclArray* obj);

  /// @brief デストラクタ
  ~ElbDeclArrayHandle() = default;


public:

  /// @brief VlNamedObj を返す．
  const VlNamedObj*
  obj() const override;

  /// @brief ElbDeclArray を返す．
  ElbDeclArray*
  declarray() const override;


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
  public ObjHandle
{
public:

  /// @brief コンストラクタ
  ElbParamHandle(ElbParameter* obj);

  /// @brief デストラクタ
  ~ElbParamHandle() = default;


public:

  /// @brief VlNamedObj を返す．
  const VlNamedObj*
  obj() const override;

  /// @brief ElbParameterを返す．
  ElbParameter*
  parameter() const override;


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
  public ObjHandle
{
public:

  /// @brief コンストラクタ
  ElbModuleArrayHandle(ElbModuleArray* obj);

  /// @brief デストラクタ
  ~ElbModuleArrayHandle() = default;


public:

  /// @brief VlNamedObj を返す．
  const VlNamedObj*
  obj() const override;

  /// @brief 配列要素を返す．
  const VlNamedObj*
  array_elem(int index) const override;

  /// @brief ElbModuleArray を返す．
  ElbModuleArray*
  module_array() const override;


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
  public ObjHandle
{
public:

  /// @brief コンストラクタ
  ElbPrimArrayHandle(ElbPrimArray* obj);

  /// @brief デストラクタ
  ~ElbPrimArrayHandle() = default;


public:

  /// @brief VlNamedObj を返す．
  const VlNamedObj*
  obj() const override;

  /// @brief ElbPrimArray を返す．
  ElbPrimArray*
  prim_array() const override;


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
  public ObjHandle
{
public:

  /// @brief コンストラクタ
  ElbPrimitiveHandle(ElbPrimitive* obj);

  /// @brief デストラクタ
  ~ElbPrimitiveHandle() = default;


public:

  /// @brief VlNamedObj を返す．
  const VlNamedObj*
  obj() const override;

  /// @brief ElbPrimitive を返す．
  ElbPrimitive*
  primitive() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象のオブジェクト
  ElbPrimitive* mObj;

};


//////////////////////////////////////////////////////////////////////
/// @class ElbNamedObjHandle
//////////////////////////////////////////////////////////////////////
class ElbNamedObjHandle :
  public ObjHandle
{
public:

  /// @brief コンストラクタ
  ElbNamedObjHandle(const VlNamedObj* obj);

  /// @brief デストラクタ
  ~ElbNamedObjHandle() = default;


public:

  /// @brief VlNamedObj を返す．
  const VlNamedObj*
  obj() const override;


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
  public ObjHandle
{
public:

  /// @brief コンストラクタ
  ElbGfRootHandle(ElbGfRoot* obj);

  /// @brief デストラクタ
  ~ElbGfRootHandle() = default;


public:

  /// @brief VlNamedObj を返す．
  const VlNamedObj*
  obj() const override;

  /// @brief 配列要素を返す．
  const VlNamedObj*
  array_elem(int index) const override;

  /// @brief ElbGfRoot を返す．
  ElbGfRoot*
  gfroot() const override;


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
  public ObjHandle
{
public:

  /// @brief コンストラクタ
  ElbGenvarHandle(ElbGenvar* obj);

  /// @brief デストラクタ
  ~ElbGenvarHandle() = default;


public:

  /// @brief VlNamedObj を返す．
  const VlNamedObj*
  obj() const override;

  /// @brief ElbGenvar を返す．
  ElbGenvar*
  genvar() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象のオブジェクト
  ElbGenvar* mObj;

};

END_NAMESPACE_YM_VERILOG

#endif // OBJHANDLESUB_H
