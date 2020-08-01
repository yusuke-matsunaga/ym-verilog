#ifndef TAGDICTCELL_H
#define TAGDICTCELL_H

/// @file TagDictCell.h
/// @brief TagDictCell のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym/verilog.h"
#include "ym/vl/VlFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class TagDictCell TagDictCell.h "TagDictCell.h"
/// @brief TagDict 用のセル
//////////////////////////////////////////////////////////////////////
class TagDictCell
{
public:

  virtual
  ~TagDictCell() = default;

  /// @brief  宣言要素を追加する．
  virtual
  void
  add_decl(const VlDecl* obj);

  /// @brief  宣言要素のリストを得る．
  virtual
  vector<const VlDecl*>
  decl_list();

  /// @brief  配列型宣言要素を追加する．
  virtual
  void
  add_declarray(const VlDeclArray* obj);

  /// @brief  配列型宣言要素のリストを得る．
  virtual
  vector<const VlDeclArray*>
  declarray_list();

  /// @brief  defparam を追加する．
  virtual
  void
  add_defparam(const VlDefParam* obj);

  /// @brief  defparam のリストを得る．
  virtual
  vector<const VlDefParam*>
  defparam_list();

  /// @brief  param assign を追加する．
  virtual
  void
  add_paramassign(const VlParamAssign* obj);

  /// @brief  param assign のリストを得る．
  virtual
  vector<const VlParamAssign*>
  paramassign_list();

  /// @brief module array を追加する．
  virtual
  void
  add_modulearray(const VlModuleArray* obj);

  /// @brief module array のリストを得る．
  virtual
  vector<const VlModuleArray*>
  modulearray_list();

  /// @brief  module を追加する．
  virtual
  void
  add_module(const VlModule* obj);

  /// @brief  module のリストを得る．
  virtual
  vector<const VlModule*>
  module_list();

  /// @brief  primitive array を追加する．
  virtual
  void
  add_primarray(const VlPrimArray* obj);

  /// @brief  primitive array のリストを得る．
  virtual
  vector<const VlPrimArray*>
  primarray_list();

  /// @brief  primitive を追加する．
  virtual
  void
  add_primitive(const VlPrimitive* obj);

  /// @brief  primitive のリストを得る．
  virtual
  vector<const VlPrimitive*>
  primitive_list();

  /// @brief タスク/関数を追加する．
  virtual
  void
  add_taskfunc(const VlTaskFunc* obj);

  /// @brief タスク/関数のリストを得る．
  virtual
  vector<const VlTaskFunc*>
  taskfunc_list();

  /// @brief continuous assignment を追加する．
  virtual
  void
  add_contassign(const VlContAssign* obj);

  /// @brief  continuous assignment のリストを得る．
  virtual
  vector<const VlContAssign*>
  contassign_list();

  /// @brief  process を追加する．
  virtual
  void
  add_process(const VlProcess* process);

  /// @brief  process のリストを得る．
  virtual
  vector<const VlProcess*>
  process_list();

  /// @brief internal scope を追加する．
  virtual
  void
  add_internalscope(const VlNamedObj* obj);

  /// @brief internal scope のリストを得る．
  virtual
  vector<const VlNamedObj*>
  internalscope_list();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
#if 0
  // 親のスコープ
  const VlNamedObj* mParent;

  // タグの値
  int mTag;
#endif

};

END_NAMESPACE_YM_VERILOG

#endif // TAGDICTCELL_H
