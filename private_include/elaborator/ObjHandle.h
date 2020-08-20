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

  /// @brief 親のスコープを返す．
  virtual
  const VlScope*
  parent_scope() const = 0;

  /// @brief オブジェクトの名前を返す．
  virtual
  string
  name() const = 0;

  /// @brief オブジェクトの型を返す．
  virtual
  VpiObjType
  type() const = 0;

  /// @brief ファイル位置の取得
  virtual
  FileRegion
  file_region() const = 0;

  /// @brief オブジェクトの階層付き名前を返す．
  virtual
  string
  full_name() const = 0;

  /// @brief VlScope を返す．
  ///
  /// このクラスでは nullptr を返す．
  virtual
  const VlScope*
  scope() const;

  /// @brief VlModule を返す．
  ///
  /// このクラスでは nullptr を返す．
  virtual
  const VlModule*
  module() const;

  /// @brief VlModuleArray を返す．
  ///
  /// このクラスでは nullptr を返す．
  virtual
  const VlModuleArray*
  module_array() const;

  /// @brief VlTaskFunc を返す．
  ///
  /// このクラスでは nullptr を返す．
  virtual
  const VlTaskFunc*
  taskfunc() const;

  /// @brief 配列要素を返す．
  ///
  /// このクラスでは nullptr を返す．
  virtual
  const VlScope*
  array_elem(int index) const;

  /// @brief VlDecl を返す．
  ///
  /// このクラスでは nullptr を返す．
  virtual
  ElbDecl*
  decl() const;

  /// @brief VlDeclArray を返す．
  ///
  /// このクラスでは nullptr を返す．
  virtual
  const VlDeclArray*
  declarray() const;

  /// @brief ElbParameter を返す．
  ///
  /// このクラスでは nullptr を返す．
  virtual
  ElbParameter*
  parameter() const;

  /// @brief ElbPrimArray を返す．
  ///
  /// このクラスでは nullptr を返す．
  virtual
  const VlPrimArray*
  prim_array() const;

  /// @brief ElbPrimitive を返す．
  ///
  /// このクラスでは nullptr を返す．
  virtual
  const VlPrimitive*
  primitive() const;

  /// @brief ElbGfRoot を返す．
  ///
  /// このクラスでは nullptr を返す．
  virtual
  ElbGfRoot*
  gfroot() const;

  /// @brief ElbGenvar を返す．
  ///
  /// このクラスでは nullptr を返す．
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
    return h1->parent_scope() == h2->parent_scope() && h1->name() == h2->name();
  }
};

END_NAMESPACE_YM_VERILOG

#endif // OBJHANDLE_H
