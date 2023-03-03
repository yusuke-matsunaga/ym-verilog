
/// @file TagDict.cc
/// @brief TagDict の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.

#include "elaborator/TagDict.h"
#include "TagDictCell.h"
#include "ym/vl/VlNamedObj.h"
#include "ym/vl/VlContAssign.h"
#include "ym/vl/VlDecl.h"
#include "ym/vl/VlDeclArray.h"
#include "ym/vl/VlModule.h"
#include "ym/vl/VlScope.h"
#include "ym/vl/VlParamAssign.h"
#include "ym/vl/VlPrimitive.h"
#include "ym/vl/VlProcess.h"
#include "ym/vl/VlTaskFunc.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス TagDict
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
TagDict::TagDict()
{
}

// @brief デストラクタ
TagDict::~TagDict()
{
}

// @brief 内容を空にする
void
TagDict::clear()
{
  for ( auto p: mHash ) {
    delete p.second;
  }
  mHash.clear();
}

// @brief Cell を登録する．
void
TagDict::put_cell(
  const VlScope* parent,
  int tag,
  TagDictCell* cell
)
{
  Key key{parent, tag};
  mHash.emplace(key, cell);
}

// @brief タグから該当する Cell を探す．
TagDictCell*
TagDict::find_cell(
  const VlScope* parent,
  int tag
) const
{
  Key key{parent, tag};
  if ( mHash.count(key) > 0 ) {
    return mHash.at(key);
  }
  return nullptr;
}


//////////////////////////////////////////////////////////////////////
// TagDictCell
//////////////////////////////////////////////////////////////////////

// 宣言要素を追加する．
void
TagDictCell::add_decl(
  const VlDecl* obj
)
{
  ASSERT_NOT_REACHED;
}

// 宣言要素のリストを得る．
vector<const VlDecl*>
TagDictCell::decl_list()
{
  ASSERT_NOT_REACHED;
  return vector<const VlDecl*>{};
}

// 宣言要素配列を追加する．
void
TagDictCell::add_declarray(
  const VlDeclArray* obj
)
{
  ASSERT_NOT_REACHED;
}

// 宣言要素配列のリストを得る．
vector<const VlDeclArray*>
TagDictCell::declarray_list()
{
  ASSERT_NOT_REACHED;
  return vector<const VlDeclArray*>{};
}

// defparam を追加する．
void
TagDictCell::add_defparam(
  const VlDefParam* obj
)
{
  ASSERT_NOT_REACHED;
}

// defparam のリストを得る．
vector<const VlDefParam*>
TagDictCell::defparam_list()
{
  ASSERT_NOT_REACHED;
  return vector<const VlDefParam*>{};
}

// param assign を追加する．
void
TagDictCell::add_paramassign(
  const VlParamAssign* obj
)
{
  ASSERT_NOT_REACHED;
}

// param assign のリストを得る．
vector<const VlParamAssign*>
TagDictCell::paramassign_list()
{
  ASSERT_NOT_REACHED;
  return vector<const VlParamAssign*>{};
}

// module array を追加する．
void
TagDictCell::add_modulearray(
  const VlModuleArray* obj
)
{
  ASSERT_NOT_REACHED;
}

// module array のリストを得る．
vector<const VlModuleArray*>
TagDictCell::modulearray_list()
{
  ASSERT_NOT_REACHED;
  return vector<const VlModuleArray*>{};
}

// module を追加する．
void
TagDictCell::add_module(
  const VlModule* obj
)
{
  ASSERT_NOT_REACHED;
}

// module のリストを得る．
vector<const VlModule*>
TagDictCell::module_list()
{
  ASSERT_NOT_REACHED;
  return vector<const VlModule*>{};
}

// primitive array を追加する．
void
TagDictCell::add_primarray(
  const VlPrimArray* obj
)
{
  ASSERT_NOT_REACHED;
}

// primitive array のリストを得る．
vector<const VlPrimArray*>
TagDictCell::primarray_list()
{
  ASSERT_NOT_REACHED;
  return vector<const VlPrimArray*>{};
}

// primitive を追加する．
void
TagDictCell::add_primitive(
  const VlPrimitive* obj
)
{
  ASSERT_NOT_REACHED;
}

// primitive のリストを得る．
vector<const VlPrimitive*>
TagDictCell::primitive_list()
{
  ASSERT_NOT_REACHED;
  return vector<const VlPrimitive*>{};
}

// @brief タスク/関数を追加する．
void
TagDictCell::add_taskfunc(
  const VlTaskFunc* obj
)
{
  ASSERT_NOT_REACHED;
}

// @brief タスク/関数のリストを得る．
vector<const VlTaskFunc*>
TagDictCell::taskfunc_list()
{
  ASSERT_NOT_REACHED;
  return vector<const VlTaskFunc*>{};
}

// continuous assignment を追加する．
void
TagDictCell::add_contassign(
  const VlContAssign* obj
)
{
  ASSERT_NOT_REACHED;
}

// continuous assignment のリストを得る．
vector<const VlContAssign*>
TagDictCell::contassign_list()
{
  ASSERT_NOT_REACHED;
  return vector<const VlContAssign*>{};
}

// process を追加する．
void
TagDictCell::add_process(
  const VlProcess* process
)
{
  ASSERT_NOT_REACHED;
}

// process のリストを得る．
vector<const VlProcess*>
TagDictCell::process_list()
{
  ASSERT_NOT_REACHED;
  return vector<const VlProcess*>{};
}

// internal scope を追加する．
void
TagDictCell::add_internalscope(
  const VlScope* obj
)
{
  ASSERT_NOT_REACHED;
}

// internal scope のリストを得る．
vector<const VlScope*>
TagDictCell::internalscope_list()
{
  ASSERT_NOT_REACHED;
  return vector<const VlScope*>{};
}


//////////////////////////////////////////////////////////////////////
// internal scope 用のセル
//////////////////////////////////////////////////////////////////////
class CellScope :
  public TagDictCell
{
public:

  /// @brief コンストラクタ
  CellScope(
    const VlScope* obj
  );

  /// @brief 要素の追加
  void
  add_internalscope(
    const VlScope* obj
  ) override;

  /// @brief 要素のリストを返す．
  vector<const VlScope*>
  internalscope_list() override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 要素のリスト
  vector<const VlScope*> mList;

};

// @brief コンストラクタ
CellScope::CellScope(
  const VlScope* obj
) : mList{obj}
{
}

// @brief 要素の追加
void
CellScope::add_internalscope(
  const VlScope* obj
)
{
  mList.push_back(obj);
}

// @brief 要素のリストを得る．
vector<const VlScope*>
CellScope::internalscope_list()
{
  return mList;
}

// @brief internal scope を追加する．
// @param[in] obj 登録する要素
void
TagDict::add_internalscope(
  const VlScope* obj
)
{
  auto parent = obj->parent_scope();

  // 該当の Cell が存在するか調べる．
  auto cell = find_cell(parent, vpiInternalScope);
  if ( cell ) {
    cell->add_internalscope(obj);
  }
  else {
    auto cell = new CellScope(obj);
    put_cell(parent, vpiInternalScope, cell);
  }
}

// @brief internal scope のリストを取り出す．
vector<const VlScope*>
TagDict::find_internalscope_list(
  const VlScope* parent
) const
{
  // 該当の Cell が存在するか調べる．
  auto cell = find_cell(parent, vpiInternalScope);
  if ( cell ) {
    return cell->internalscope_list();
  }
  else {
    return vector<const VlScope*>();
  }
}


//////////////////////////////////////////////////////////////////////
// 宣言要素用のセル
//////////////////////////////////////////////////////////////////////
class CellDecl :
  public TagDictCell
{
public:

  /// @brief コンストラクタ
  CellDecl(
    const VlDecl* obj
  );

  /// @brief 要素の追加
  void
  add_decl(
    const VlDecl* obj
  ) override;

  /// @brief 宣言要素のリストを得る．
  vector<const VlDecl*>
  decl_list() override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 要素のリスト
  vector<const VlDecl*> mList;

};

// @brief コンストラクタ
CellDecl::CellDecl(
  const VlDecl* obj
) : mList{obj}
{
}

// @brief 要素の追加
void
CellDecl::add_decl(
  const VlDecl* obj
)
{
  mList.push_back(obj);
}

// @brief 宣言のリストを得る．
vector<const VlDecl*>
CellDecl::decl_list()
{
  return mList;
}

// @brief 宣言要素を追加する．
void
TagDict::add_decl(
  int tag,
  const VlDecl* obj
)
{
  auto parent = obj->parent_scope();

  // 該当の Cell が存在するか調べる．
  auto cell = find_cell(parent, tag);
  if ( cell ) {
    cell->add_decl(obj);
  }
  else {
    auto cell = new CellDecl(obj);
    put_cell(parent, tag, cell);
  }
}

// @brief タグから該当する宣言要素のリストを返す．
vector<const VlDecl*>
TagDict::find_decl_list(
  const VlScope* parent,
  int tag
) const
{
  // 該当の Cell が存在するか調べる．
  auto cell = find_cell(parent, tag);
  if ( cell ) {
    return cell->decl_list();
  }
  else {
    return vector<const VlDecl*>{};
  }
}


//////////////////////////////////////////////////////////////////////
// 宣言要素用のセル
//////////////////////////////////////////////////////////////////////
class CellDeclArray :
  public TagDictCell
{
public:

  /// @brief コンストラクタ
  CellDeclArray(
    const VlDeclArray* obj
  );

  /// @brief 要素の追加
  void
  add_declarray(
    const VlDeclArray* obj
  ) override;

  /// @brief 宣言のリストを得る．
  vector<const VlDeclArray*>
  declarray_list() override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 要素のリスト
  vector<const VlDeclArray*> mList;

};

// @brief コンストラクタ
CellDeclArray::CellDeclArray(
  const VlDeclArray* obj
) : mList{obj}
{
}

// @brief 要素の追加
void
CellDeclArray::add_declarray(
  const VlDeclArray* obj
)
{
  mList.push_back(obj);
}

// @brief 宣言素のリストを得る．
vector<const VlDeclArray*>
CellDeclArray::declarray_list()
{
  return mList;
}

// @brief 宣言要素を追加する．
void
TagDict::add_declarray(
  int tag,
  const VlDeclArray* obj
)
{
  auto parent = obj->parent_scope();

  // 該当の Cell が存在するか調べる．
  auto cell = find_cell(parent, tag);
  if ( cell ) {
    cell->add_declarray(obj);
  }
  else {
    auto cell = new CellDeclArray(obj);
    put_cell(parent, tag, cell);
  }
}

// @brief タグから該当する宣言要素のリストを返す．
vector<const VlDeclArray*>
TagDict::find_declarray_list(
  const VlScope* parent,
  int tag
) const
{
  // 該当の Cell が存在するか調べる．
  auto cell = find_cell(parent, tag);
  if ( cell ) {
    return cell->declarray_list();
  }
  else {
    return vector<const VlDeclArray*>{};
  }
}


//////////////////////////////////////////////////////////////////////
// defparam 用のセル
//////////////////////////////////////////////////////////////////////
class CellDefParam :
  public TagDictCell
{
public:

  /// @brief コンストラクタ
  CellDefParam(
    const VlDefParam* obj
  );

  /// @brief 要素の追加
  void
  add_defparam(
    const VlDefParam* obj
  ) override;

  /// @brief defparam のを得る．
  vector<const VlDefParam*>
  defparam_list() override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 要素のリスト
  vector<const VlDefParam*> mList;

};

// @brief コンストラクタ
CellDefParam::CellDefParam(
  const VlDefParam* obj
) : mList{obj}
{
}

// @brief 要素の追加
void
CellDefParam::add_defparam(
  const VlDefParam* obj
)
{
  mList.push_back(obj);
}

// @brief defparam のリストを得る．
vector<const VlDefParam*>
CellDefParam::defparam_list()
{
  return mList;
}

// @brief defparam を追加する．
void
TagDict::add_defparam(
  const VlDefParam* obj
)
{
  auto parent = obj->parent_module();

  // 該当の Cell が存在するか調べる．
  auto cell = find_cell(parent, vpiDefParam);
  if ( cell ) {
    cell->add_defparam(obj);
  }
  else {
    auto cell = new CellDefParam(obj);
    put_cell(parent, vpiDefParam, cell);
  }
}

// @brief defparam のリストを取り出す．
vector<const VlDefParam*>
TagDict::find_defparam_list(
  const VlScope* parent
) const
{
  // 該当の Cell が存在するか調べる．
  auto cell = find_cell(parent, vpiDefParam);
  if ( cell ) {
    return cell->defparam_list();
  }
  else {
    return vector<const VlDefParam*>{};
  }
}


//////////////////////////////////////////////////////////////////////
// param assign 用のセル
//////////////////////////////////////////////////////////////////////
class CellParamAssign :
  public TagDictCell
{
public:

  /// @brief コンストラクタ
  CellParamAssign(
    const VlParamAssign* obj
  );

  /// @brief 要素の追加
  void
  add_paramassign(
    const VlParamAssign* obj
  ) override;

  /// @brief param assign のリストを得る．
  vector<const VlParamAssign*>
  paramassign_list() override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 要素のリスト
  vector<const VlParamAssign*> mList;

};

// @brief コンストラクタ
CellParamAssign::CellParamAssign(
  const VlParamAssign* obj
) : mList{obj}
{
}

// @brief 要素の追加
void
CellParamAssign::add_paramassign(
  const VlParamAssign* obj
)
{
  mList.push_back(obj);
}

// @brief param assign のリストを得る．
vector<const VlParamAssign*>
CellParamAssign::paramassign_list()
{
  return mList;
}

// @brief param assign を追加する．
void
TagDict::add_paramassign(
  const VlParamAssign* obj
)
{
  auto parent = obj->parent_module();

  // 該当の Cell が存在するか調べる．
  auto cell = find_cell(parent, vpiParamAssign);
  if ( cell ) {
    cell->add_paramassign(obj);
  }
  else {
    auto cell = new CellParamAssign(obj);
    put_cell(parent, vpiParamAssign, cell);
  }
}

// @brief param assign のリストを取り出す．
vector<const VlParamAssign*>
TagDict::find_paramassign_list(
  const VlScope* parent
) const
{
  // 該当の Cell が存在するか調べる．
  auto cell = find_cell(parent, vpiParamAssign);
  if ( cell ) {
    return cell->paramassign_list();
  }
  else {
    return vector<const VlParamAssign*>{};
  }
}


//////////////////////////////////////////////////////////////////////
// module array 用のセル
//////////////////////////////////////////////////////////////////////
class CellModuleArray :
  public TagDictCell
{
public:

  /// @brief コンストラクタ
  CellModuleArray(
    const VlModuleArray* obj
  );

  /// @brief 要素の追加
  void
  add_modulearray(
    const VlModuleArray* obj
  ) override;

  /// @brief module array のリストを得る．
  vector<const VlModuleArray*>
  modulearray_list() override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 要素のリスト
  vector<const VlModuleArray*> mList;

};

// @brief コンストラクタ
CellModuleArray::CellModuleArray(
  const VlModuleArray* obj
) : mList{obj}
{
}

// @brief 要素の追加
void
CellModuleArray::add_modulearray(
  const VlModuleArray* obj
)
{
  mList.push_back(obj);
}

// @brief module array のリストを得る．
vector<const VlModuleArray*>
CellModuleArray::modulearray_list()
{
  return mList;
}

// @brief module array を追加する．
void
TagDict::add_modulearray(
  const VlModuleArray* obj
)
{
  auto parent = obj->parent_scope();

  // 該当の Cell が存在するか調べる．
  auto cell = find_cell(parent, vpiModuleArray);
  if ( cell ) {
    cell->add_modulearray(obj);
  }
  else {
    auto cell = new CellModuleArray(obj);
    put_cell(parent, vpiModuleArray, cell);
  }
}

// @brief module array のリストを取り出す．
vector<const VlModuleArray*>
TagDict::find_modulearray_list(
  const VlScope* parent
) const
{
  // 該当の Cell が存在するか調べる．
  auto cell = find_cell(parent, vpiModuleArray);
  if ( cell ) {
    return cell->modulearray_list();
  }
  else {
    return vector<const VlModuleArray*>{};
  }
}


//////////////////////////////////////////////////////////////////////
// module 用のセル
//////////////////////////////////////////////////////////////////////
class CellModule :
  public TagDictCell
{
public:

  /// @brief コンストラクタ
  CellModule(
    const VlModule* obj
  );

  /// @brief 要素の追加
  void
  add_module(
    const VlModule* obj
  ) override;

  /// @brief module のリストを得る．
  vector<const VlModule*>
  module_list() override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 要素のリスト
  vector<const VlModule*> mList;

};

// @brief コンストラクタ
CellModule::CellModule(
  const VlModule* obj
) : mList{obj}
{
}

// @brief 要素の追加
void
CellModule::add_module(
  const VlModule* obj
)
{
  mList.push_back(obj);
}

// @brief module のリストを得る．
vector<const VlModule*>
CellModule::module_list()
{
  return mList;
}

// @brief module を追加する．
void
TagDict::add_module(
  const VlModule* obj
)
{
  auto parent = obj->parent_scope();

  // 該当の Cell が存在するか調べる．
  auto cell = find_cell(parent, vpiModule);
  if ( cell ) {
    cell->add_module(obj);
  }
  else {
    auto cell = new CellModule(obj);
    put_cell(parent, vpiModule, cell);
  }
}

// @brief module のリストを取り出す．
vector<const VlModule*>
TagDict::find_module_list(
  const VlScope* parent
) const
{
  // 該当の Cell が存在するか調べる．
  auto cell = find_cell(parent, vpiModule);
  if ( cell ) {
    return cell->module_list();
  }
  else {
    return vector<const VlModule*>{};
  }
}


//////////////////////////////////////////////////////////////////////
// primitive array 用のセル
//////////////////////////////////////////////////////////////////////
class CellPrimArray :
  public TagDictCell
{
public:

  /// @brief コンストラクタ
  CellPrimArray(
    const VlPrimArray* obj
  );

  /// @brief 要素の追加
  void
  add_primarray(
    const VlPrimArray* obj
  ) override;

  /// @brief primitive array のリストを得る．
  vector<const VlPrimArray*>
  primarray_list() override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 要素のリスト
  vector<const VlPrimArray*> mList;

};

// @brief コンストラクタ
CellPrimArray::CellPrimArray(
  const VlPrimArray* obj
) : mList{obj}
{
}

// @brief 要素の追加
void
CellPrimArray::add_primarray(
  const VlPrimArray* obj
)
{
  mList.push_back(obj);
}

// @brief primitive array のリストを得る．
vector<const VlPrimArray*>
CellPrimArray::primarray_list()
{
  return mList;
}

// @brief primitive array を追加する．
void
TagDict::add_primarray(
  const VlPrimArray* obj
)
{
  auto parent = obj->parent_scope();

  // 該当の Cell が存在するか調べる．
  auto cell = find_cell(parent, vpiPrimitiveArray);
  if ( cell ) {
    cell->add_primarray(obj);
  }
  else {
    auto cell = new CellPrimArray(obj);
    put_cell(parent, vpiPrimitiveArray, cell);
  }
}

// @brief primitive array のリストを取り出す．
vector<const VlPrimArray*>
TagDict::find_primarray_list(
  const VlScope* parent
) const
{
  // 該当の Cell が存在するか調べる．
  auto cell = find_cell(parent, vpiPrimitiveArray);
  if ( cell ) {
    return cell->primarray_list();
  }
  else {
    return vector<const VlPrimArray*>{};
  }
}


//////////////////////////////////////////////////////////////////////
// primitive 用のセル
//////////////////////////////////////////////////////////////////////
class CellPrimitive :
  public TagDictCell
{
public:

  /// @brief コンストラクタ
  CellPrimitive(
    const VlPrimitive* obj
  );

  /// @brief 要素の追加
  void
  add_primitive(
    const VlPrimitive* obj
  ) override;

  /// @brief primitive のリストを得る．
  vector<const VlPrimitive*>
  primitive_list() override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 要素のリスト
  vector<const VlPrimitive*> mList;

};

// @brief コンストラクタ
CellPrimitive::CellPrimitive(
  const VlPrimitive* obj
) : mList{obj}
{
}

// @brief 要素の追加
void
CellPrimitive::add_primitive(
  const VlPrimitive* obj
)
{
  mList.push_back(obj);
}

// @brief primitive のリストを得る．
vector<const VlPrimitive*>
CellPrimitive::primitive_list()
{
  return mList;
}

// @brief primitive を追加する．
void
TagDict::add_primitive(
  const VlPrimitive* obj
)
{
  auto parent = obj->parent_scope();

  // 該当の Cell が存在するか調べる．
  auto cell = find_cell(parent, vpiPrimitive);
  if ( cell ) {
    cell->add_primitive(obj);
  }
  else {
    auto cell = new CellPrimitive(obj);
    put_cell(parent, vpiPrimitive, cell);
  }
}

// @brief primitive のリストを取り出す．
vector<const VlPrimitive*>
TagDict::find_primitive_list(
  const VlScope* parent
) const
{
  // 該当の Cell が存在するか調べる．
  auto cell = find_cell(parent, vpiPrimitive);
  if ( cell ) {
    return cell->primitive_list();
  }
  else {
    return vector<const VlPrimitive*>{};
  }
}


//////////////////////////////////////////////////////////////////////
// タスク用のセル
//////////////////////////////////////////////////////////////////////
class CellTaskFunc :
  public TagDictCell
{
public:

  /// @brief コンストラクタ
  CellTaskFunc(
    const VlTaskFunc* obj
  );

  /// @brief 要素の追加
  void
  add_taskfunc(
    const VlTaskFunc* obj
  ) override;

  /// @brief 要素のリストを得る．
  vector<const VlTaskFunc*>
  taskfunc_list() override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 要素のリスト
  vector<const VlTaskFunc*> mList;

};

// @brief コンストラクタ
CellTaskFunc::CellTaskFunc(
  const VlTaskFunc* obj
) : mList{obj}
{
}

// @brief 要素の追加
void
CellTaskFunc::add_taskfunc(
  const VlTaskFunc* obj
)
{
  mList.push_back(obj);
}

// @brief 要素のリストを得る．
vector<const VlTaskFunc*>
CellTaskFunc::taskfunc_list()
{
  return mList;
}

// @brief タスクを追加する．
void
TagDict::add_task(
  const VlTaskFunc* obj
)
{
  auto parent = obj->parent_scope();

  // 該当の Cell が存在するか調べる．
  auto cell = find_cell(parent, vpiTask);
  if ( cell ) {
    cell->add_taskfunc(obj);
  }
  else {
    auto cell = new CellTaskFunc(obj);
    put_cell(parent, vpiTask, cell);
  }
}

// @brief タスクのリストを取り出す．
vector<const VlTaskFunc*>
TagDict::find_task_list(
  const VlScope* parent
) const
{
  // 該当の Cell が存在するか調べる．
  auto cell = find_cell(parent, vpiTask);
  if ( cell ) {
    return cell->taskfunc_list();
  }
  else {
    return vector<const VlTaskFunc*>{};
  }
}

// @brief function を追加する．
void
TagDict::add_function(
  const VlTaskFunc* obj
)
{
  auto parent = obj->parent_scope();

  // 該当の Cell が存在するか調べる．
  auto cell = find_cell(parent, vpiFunction);
  if ( cell ) {
    cell->add_taskfunc(obj);
  }
  else {
    auto cell = new CellTaskFunc(obj);
    put_cell(parent, vpiFunction, cell);
  }
}

// @brief function のリストを取り出す．
vector<const VlTaskFunc*>
TagDict::find_function_list(
  const VlScope* parent
) const
{
  // 該当の Cell が存在するか調べる．
  auto cell = find_cell(parent, vpiFunction);
  if ( cell ) {
    return cell->taskfunc_list();
  }
  else {
    return vector<const VlTaskFunc*>{};
  }
}


//////////////////////////////////////////////////////////////////////
// continuous assignment 用のセル
//////////////////////////////////////////////////////////////////////
class CellContAssign :
  public TagDictCell
{
public:

  /// @brief コンストラクタ
  CellContAssign(
    const VlContAssign* obj
  );

  /// @brief 要素の追加
  void
  add_contassign(
    const VlContAssign* obj
  ) override;

  /// @brief 要素のリストを得る．
  vector<const VlContAssign*>
  contassign_list() override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 要素のリスト
  vector<const VlContAssign*> mList;

};

// @brief コンストラクタ
CellContAssign::CellContAssign(
  const VlContAssign* obj
) : mList{obj}
{
}

// @brief 要素の追加
void
CellContAssign::add_contassign(
  const VlContAssign* obj
)
{
  mList.push_back(obj);
}

// @brief 要素のリストを得る．
vector<const VlContAssign*>
CellContAssign::contassign_list()
{
  return mList;
}

// @brief continuous assignment を追加する．
void
TagDict::add_contassign(
  const VlContAssign* obj
)
{
  auto parent = obj->module();

  // 該当の Cell が存在するか調べる．
  auto cell = find_cell(parent, vpiContAssign);
  if ( cell ) {
    cell->add_contassign(obj);
  }
  else {
    auto cell = new CellContAssign(obj);
    put_cell(parent, vpiContAssign, cell);
  }
}

// @brief continuous assignment のリストを取り出す．
vector<const VlContAssign*>
TagDict::find_contassign_list(
  const VlScope* parent
) const
{
  // 該当の Cell が存在するか調べる．
  auto cell = find_cell(parent, vpiContAssign);
  if ( cell ) {
    return cell->contassign_list();
  }
  else {
    return vector<const VlContAssign*>{};
  }
}


//////////////////////////////////////////////////////////////////////
// process 用のセル
//////////////////////////////////////////////////////////////////////
class CellProcess :
  public TagDictCell
{
public:

  /// @brief コンストラクタ
  CellProcess(
    const VlProcess* obj
  );

  /// @brief 要素の追加
  void
  add_process(
    const VlProcess* obj
  ) override;

  /// @brief 要素のリストを得る．
  vector<const VlProcess*>
  process_list() override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 要素のリスト
  vector<const VlProcess*> mList;

};

// @brief コンストラクタ
CellProcess::CellProcess(
  const VlProcess* obj
) : mList{obj}
{
}

// @brief 要素の追加
void
CellProcess::add_process(
  const VlProcess* obj
)
{
  mList.push_back(obj);
}

// @brief 要素のリストを得る．
vector<const VlProcess*>
CellProcess::process_list()
{
  return mList;
}

// @brief process を追加する．
void
TagDict::add_process(
  const VlProcess* obj
)
{
  auto parent = obj->parent_scope();

  // 該当の Cell が存在するか調べる．
  auto cell = find_cell(parent, vpiProcess);
  if ( cell ) {
    cell->add_process(obj);
  }
  else {
    auto cell = new CellProcess(obj);
    put_cell(parent, vpiProcess, cell);
  }
}

// @brief process のリストを取り出す．
vector<const VlProcess*>
TagDict::find_process_list(
  const VlScope* parent
) const
{
  // 該当の Cell が存在するか調べる．
  auto cell = find_cell(parent, vpiProcess);
  if ( cell ) {
    return cell->process_list();
  }
  else {
    return vector<const VlProcess*>{};
  }
}

END_NAMESPACE_YM_VERILOG
