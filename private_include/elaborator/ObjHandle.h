#ifndef OBJHANDLE_H
#define OBJHANDLE_H

/// @file ObjHandle.h
/// @brief ObjHandle のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.


#include "ym/verilog.h"
#include "ym/FileRegion.h"
#include "ym/vl/VlFwd.h"
#include "ElbFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ObjHandle ObjHandle.h "ObjHandle.h"
//////////////////////////////////////////////////////////////////////
class ObjHandle
{
public:

  /// @brief コンストラクタ
  ObjHandle() = default;

  /// @brief デストラクタ
  virtual
  ~ObjHandle() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief VlNamedObj を返す．
  virtual
  const VlNamedObj*
  obj() const = 0;

  /// @brief 親のスコープを返す．
  virtual
  const VlNamedObj*
  parent() const;

  /// @brief オブジェクトの名前を返す．
  virtual
  const char*
  name() const;

  /// @brief オブジェクトの型を返す．
  virtual
  VpiObjType
  type() const;

  /// @brief ファイル位置の取得
  virtual
  FileRegion
  file_region() const;

  /// @brief オブジェクトの階層付き名前を返す．
  virtual
  string
  full_name() const;

  /// @brief 配列要素を返す．
  /// @note このクラスでは nullptr を返す．
  virtual
  const VlNamedObj*
  array_elem(int index) const;

  /// @brief VlDecl を返す．
  /// @note このクラスでは nullptr を返す．
  virtual
  ElbDecl*
  decl() const;

  /// @brief VlDeclArray を返す．
  /// @note このクラスでは nullptr を返す．
  virtual
  const VlDeclArray*
  declarray() const;

  /// @brief ElbParameter を返す．
  /// @note このクラスでは nullptr を返す．
  virtual
  ElbParameter*
  parameter() const;

  /// @brief ElbTaskFunc を返す．
  /// @note このクラスでは nullptr を返す．
  virtual
  const VlTaskFunc*
  taskfunc() const;

  /// @brief ElbModuleArray を返す．
  /// @note このクラスでは nullptr を返す．
  virtual
  const VlModuleArray*
  module_array() const;

  /// @brief ElbPrimArray を返す．
  /// @note このクラスでは nullptr を返す．
  virtual
  const VlPrimArray*
  prim_array() const;

  /// @brief ElbPrimitive を返す．
  /// @note このクラスでは nullptr を返す．
  virtual
  const VlPrimitive*
  primitive() const;

  /// @brief ElbGfRoot を返す．
  /// @note このクラスでは nullptr を返す．
  virtual
  ElbGfRoot*
  gfroot() const;

  /// @brief ElbGenvar を返す．
  /// @note このクラスでは nullptr を返す．
  virtual
  ElbGenvar*
  genvar() const;

  /// @brief ハッシュ値を返す．
  SizeType
  hash() const;

};

/// @brief ObjHandle のハッシュ関数クラス
struct ObjHandleHash
{
  SizeType
  operator()(const ObjHandle* h) const
  {
    return h->hash();
  }
};

/// @brief ObjHandle の等価比較関数クラス
struct ObjHandleEq
{
  bool
  operator()(const ObjHandle* h1,
	     const ObjHandle* h2) const
  {
    return h1->parent() == h2->parent() && strcmp(h1->name(), h2->name()) == 0;
  }
};

END_NAMESPACE_YM_VERILOG

#endif // OBJHANDLE_H
