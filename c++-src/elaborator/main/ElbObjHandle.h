#ifndef ELBOBJHANDLE_H
#define ELBOBJHANDLE_H

/// @file ElbObjHandle.h
/// @brief ElbObjHandle のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym/verilog.h"
#include "ym/FileRegion.h"
#include "ym/vl/VlFwd.h"
#include "elb/HierName.h"


BEGIN_NAMESPACE_YM_VERILOG

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
/// @class ElbNamedObjHandle
//////////////////////////////////////////////////////////////////////
class ElbNamedObjHandle :
  public ElbObjHandle
{
public:

  /// @brief コンストラクタ
  ElbNamedObjHandle(const VlNamedObj* obj);

  /// @brief デストラクタ
  virtual
  ~ElbNamedObjHandle();


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


END_NAMESPACE_YM_VERILOG

#endif // ELBOBJHANDLE_H
