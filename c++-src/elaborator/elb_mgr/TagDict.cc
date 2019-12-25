
/// @file TagDict.cc
/// @brief TagDict の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "elb/TagDict.h"
#include "TagDictCell.h"

#include "ym/vl/VlObj.h"

#include "elb/ElbScope.h"
#include "elb/ElbModule.h"
#include "elb/ElbParamAssign.h"
#include "elb/ElbDecl.h"
#include "elb/ElbParameter.h"
#include "elb/ElbPrimitive.h"
#include "elb/ElbTaskFunc.h"
#include "elb/ElbContAssign.h"
#include "elb/ElbProcess.h"


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
  mHash.clear();
  mCellList.clear();
}

// @brief Cell を登録する．
// @param[in] parent 親のスコープ
// @param[in] tag 要素の型を表すタグ (vpi_user.h 参照)
// @param[in] cell 対象の Cell
void
TagDict::put_cell(const VlNamedObj* parent,
		  int tag,
		  TagDictCell* cell)
{
  Key key{parent, tag};
  mHash.emplace(key, cell);
  mCellList.push_back(unique_ptr<TagDictCell>{cell});
}

// @brief タグから該当する Cell を探す．
// @param[in] parent 親のスコープ
// @param[in] tag 要素の型を表すタグ (vpi_user.h 参照)
TagDictCell*
TagDict::find_cell(const VlNamedObj* parent,
		   int tag) const
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
TagDictCell::add_decl(ElbDecl* obj)
{
  ASSERT_NOT_REACHED;
}

// 宣言要素の先頭を得る．
const ElbDecl*
TagDictCell::decl()
{
  ASSERT_NOT_REACHED;
  return nullptr;
}

// 宣言要素配列を追加する．
void
TagDictCell::add_declarray(ElbDeclArray* obj)
{
  ASSERT_NOT_REACHED;
}

// 宣言要素配列の先頭を得る．
const ElbDeclArray*
TagDictCell::declarray()
{
  ASSERT_NOT_REACHED;
  return nullptr;
}

// @brief  パラメータを追加する．
void
TagDictCell::add_parameter(ElbParameter* obj)
{
  ASSERT_NOT_REACHED;
}

// @brief  パラメータの先頭を得る．
const ElbParameter*
TagDictCell::parameter()
{
  ASSERT_NOT_REACHED;
  return nullptr;
}

// defparam を追加する．
void
TagDictCell::add_defparam(ElbDefParam* obj)
{
  ASSERT_NOT_REACHED;
}

// defparam の先頭を得る．
const ElbDefParam*
TagDictCell::defparam()
{
  ASSERT_NOT_REACHED;
  return nullptr;
}

// param assign を追加する．
void
TagDictCell::add_paramassign(ElbParamAssign* obj)
{
  ASSERT_NOT_REACHED;
}

// param assign の先頭を得る．
const ElbParamAssign*
TagDictCell::paramassign()
{
  ASSERT_NOT_REACHED;
  return nullptr;
}

// module array を追加する．
void
TagDictCell::add_modulearray(ElbModuleArray* obj)
{
  ASSERT_NOT_REACHED;
}

// module array の先頭を得る．
const ElbModuleArray*
TagDictCell::modulearray()
{
  ASSERT_NOT_REACHED;
  return nullptr;
}

// module を追加する．
void
TagDictCell::add_module(ElbModule* obj)
{
  ASSERT_NOT_REACHED;
}

// module の先頭を得る．
const ElbModule*
TagDictCell::module()
{
  ASSERT_NOT_REACHED;
  return nullptr;
}

// primitive array を追加する．
void
TagDictCell::add_primarray(ElbPrimArray* obj)
{
  ASSERT_NOT_REACHED;
}

// primitive array の先頭を得る．
const ElbPrimArray*
TagDictCell::primarray()
{
  ASSERT_NOT_REACHED;
  return nullptr;
}

// primitive を追加する．
void
TagDictCell::add_primitive(ElbPrimitive* obj)
{
  ASSERT_NOT_REACHED;
}

// primitive の先頭を得る．
const ElbPrimitive*
TagDictCell::primitive()
{
  ASSERT_NOT_REACHED;
  return nullptr;
}

// @brief タスクを追加する．
void
TagDictCell::add_task(ElbTaskFunc* obj)
{
  ASSERT_NOT_REACHED;
}

// @brief タスクの先頭を得る．
const ElbTaskFunc*
TagDictCell::task()
{
  ASSERT_NOT_REACHED;
  return nullptr;
}

// @brief 関数を追加する．
void
TagDictCell::add_function(ElbTaskFunc* obj)
{
  ASSERT_NOT_REACHED;
}

// @brief 関数の先頭を得る．
const ElbTaskFunc*
TagDictCell::function()
{
  ASSERT_NOT_REACHED;
  return nullptr;
}

// continuous assignment を追加する．
void
TagDictCell::add_contassign(ElbContAssign* obj)
{
  ASSERT_NOT_REACHED;
}

// continuous assignment の先頭を得る．
const ElbContAssign*
TagDictCell::contassign()
{
  ASSERT_NOT_REACHED;
  return nullptr;
}

// process を追加する．
void
TagDictCell::add_process(ElbProcess* process)
{
  ASSERT_NOT_REACHED;
}

// process の先頭を得る．
const ElbProcess*
TagDictCell::process()
{
  ASSERT_NOT_REACHED;
  return nullptr;
}

// internal scope を追加する．
void
TagDictCell::add_internalscope(ElbScope* obj)
{
  ASSERT_NOT_REACHED;
}

// internal scope の先頭を得る．
const ElbScope*
TagDictCell::internalscope()
{
  ASSERT_NOT_REACHED;
  return nullptr;
}


//////////////////////////////////////////////////////////////////////
// internal scope 用のセル
//////////////////////////////////////////////////////////////////////
class CellScope :
  public TagDictCell
{
public:

  /// @brief コンストラクタ
  CellScope(ElbScope* obj);

  /// @brief 要素の追加
  void
  add_internalscope(ElbScope* obj) override;

  /// @brief 要素の先頭を得る．
  const ElbScope*
  internalscope() override;

  /// @brief 要素数の取得
  int
  num() override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 先頭の要素
  ElbScope* mTop;

  // 末尾の要素
  ElbScope* mTail;

  // 要素数
  int mNum;

};

// @brief コンストラクタ
CellScope::CellScope(ElbScope* obj) :
  mTop{obj},
  mTail{obj},
  mNum{1}
{
}

// @brief 要素の追加
void
CellScope::add_internalscope(ElbScope* obj)
{
  mTail->mNext = obj;
  mTail = obj;
  ++ mNum;
}

// @brief 要素の先頭を得る．
const ElbScope*
CellScope::internalscope()
{
  return mTop;
}

// @brief 要素数の取得
int
CellScope::num()
{
  return mNum;
}

// @brief internal scope を追加する．
// @param[in] obj 登録する要素
void
TagDict::add_internalscope(ElbScope* obj)
{
  auto parent = obj->parent();

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
// @param[in] parent 親のスコープ
// @param[out] obj_list 結果を格納するリスト
// @retval true 該当する要素が1つ以上あった．
// @retval false 該当する要素がなかった．
bool
TagDict::find_internalscope_list(const VlNamedObj* parent,
				 vector<const VlNamedObj*>& obj_list) const
{
  // 該当の Cell が存在するか調べる．
  auto cell = find_cell(parent, vpiInternalScope);
  if ( cell ) {
    obj_list.clear();
    obj_list.reserve(cell->num());
    for ( auto obj = cell->internalscope(); obj; obj = obj->next()) {
      obj_list.push_back(obj);
    }
    return true;
  }
  return false;
}


//////////////////////////////////////////////////////////////////////
// 宣言要素用のセル
//////////////////////////////////////////////////////////////////////
class CellDecl :
  public TagDictCell
{
public:

  /// @brief コンストラクタ
  CellDecl(ElbDecl* obj);

  /// @brief 要素の追加
  void
  add_decl(ElbDecl* obj) override;

  /// @brief 宣言要素の先頭を得る．
  ElbDecl*
  decl() override;

  /// @brief 要素数の取得
  int
  num() override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 先頭の要素
  ElbDecl* mTop;

  // 末尾の要素
  ElbDecl* mTail;

  // 要素数
  int mNum;

};

// @brief コンストラクタ
CellDecl::CellDecl(ElbDecl* obj) :
  mTop{obj},
  mTail{obj},
  mNum{1}
{
}

// @brief 要素の追加
void
CellDecl::add_decl(ElbDecl* obj)
{
  mTail->mNext = obj;
  mTail = obj;
  ++ mNum;
}

// @brief 宣言要素の先頭を得る．
ElbDecl*
CellDecl::decl()
{
  return mTop;
}

// @brief 要素数の取得
int
CellDecl::num()
{
  return mNum;
}

// @brief 宣言要素を追加する．
// @param[in] tag 要素の型を表すタグ (vpi_user.h 参照)
// @param[in] obj 登録する要素
void
TagDict::add_decl(int tag,
		  ElbDecl* obj)
{
  auto parent = obj->parent();

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
// @param[in] parent 親のスコープ
// @param[in] tag 要素の型を表すタグ (vpi_user.h 参照)
// @param[out] obj_list 結果を格納するリスト
// @retval true 該当する要素が1つ以上あった．
// @retval false 該当する要素がなかった．
// @note scope というスコープ内の tag というタグを持つ要素を
// decl_list に入れる．
bool
TagDict::find_decl_list(const VlNamedObj* parent,
			int tag,
			vector<const VlDecl*>& obj_list) const
{
  // 該当の Cell が存在するか調べる．
  auto cell = find_cell(parent, tag);
  if ( cell ) {
    obj_list.clear();
    obj_list.reserve(cell->num());
    if ( tag == vpiParameter || tag == vpiSpecParam ) {
      for ( auto obj = cell->parameter(); obj; obj = obj->next() ) {
	obj_list.push_back(obj);
      }
    }
    else {
      for ( auto obj = cell->decl(); obj; obj = obj->next()) {
	obj_list.push_back(obj);
      }
    }
    return true;
  }
  return false;
}


//////////////////////////////////////////////////////////////////////
// 宣言要素用のセル
//////////////////////////////////////////////////////////////////////
class CellDeclArray :
  public TagDictCell
{
public:

  /// @brief コンストラクタ
  CellDeclArray(ElbDeclArray* obj);

  /// @brief 要素の追加
  void
  add_declarray(ElbDeclArray* obj) override;

  /// @brief 宣言要素の先頭を得る．
  ElbDeclArray*
  declarray() override;

  /// @brief 要素数の取得
  int
  num() override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 先頭の要素
  ElbDeclArray* mTop;

  // 末尾の要素
  ElbDeclArray* mTail;

  // 要素数
  int mNum;

};

// @brief コンストラクタ
CellDeclArray::CellDeclArray(ElbDeclArray* obj) :
  mTop{obj},
  mTail{obj},
  mNum{1}
{
}

// @brief 要素の追加
void
CellDeclArray::add_declarray(ElbDeclArray* obj)
{
  mTail->mNext = obj;
  mTail = obj;
  ++ mNum;
}

// @brief 宣言要素の先頭を得る．
ElbDeclArray*
CellDeclArray::declarray()
{
  return mTop;
}

// @brief 要素数の取得
int
CellDeclArray::num()
{
  return mNum;
}

// @brief 宣言要素を追加する．
// @param[in] tag 要素の型を表すタグ (vpi_user.h 参照)
// @param[in] obj 登録する要素
void
TagDict::add_declarray(int tag,
		       ElbDeclArray* obj)
{
  auto parent = obj->parent();

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
// @param[in] parent 親のスコープ
// @param[in] tag 要素の型を表すタグ (vpi_user.h 参照)
// @param[out] obj_list 結果を格納するリスト
// @retval true 該当する要素が1つ以上あった．
// @retval false 該当する要素がなかった．
// @note scope というスコープ内の tag というタグを持つ要素を
// obj_list に入れる．
bool
TagDict::find_declarray_list(const VlNamedObj* parent,
			     int tag,
			     vector<const VlDeclArray*>& obj_list) const
{
  // 該当の Cell が存在するか調べる．
  auto cell = find_cell(parent, tag);
  if ( cell ) {
    obj_list.clear();
    obj_list.reserve(cell->num());
    for ( auto obj = cell->declarray(); obj; obj = obj->next()) {
      obj_list.push_back(obj);
    }
    return true;
  }
  return false;
}


//////////////////////////////////////////////////////////////////////
// パラメータ用ののセル
//////////////////////////////////////////////////////////////////////
class CellParam :
  public TagDictCell
{
public:

  /// @brief コンストラクタ
  CellParam(ElbParameter* obj);

  /// @brief 要素の追加
  void
  add_parameter(ElbParameter* obj) override;

  /// @brief 宣言要素の先頭を得る．
  ElbParameter*
  parameter() override;

  /// @brief 要素数の取得
  int
  num() override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 先頭の要素
  ElbParameter* mTop;

  // 末尾の要素
  ElbParameter* mTail;

  // 要素数
  int mNum;

};

// @brief コンストラクタ
CellParam::CellParam(ElbParameter* obj) :
  mTop{obj},
  mTail{obj},
  mNum{1}
{
}

// @brief 要素の追加
void
CellParam::add_parameter(ElbParameter* obj)
{
  mTail->mNext = obj;
  mTail = obj;
  ++ mNum;
}

// @brief 宣言要素の先頭を得る．
ElbParameter*
CellParam::parameter()
{
  return mTop;
}

// @brief 要素数の取得
int
CellParam::num()
{
  return mNum;
}

// @brief 宣言要素を追加する．
// @param[in] tag 要素の型を表すタグ (vpi_user.h 参照)
// @param[in] obj 登録する要素
void
TagDict::add_parameter(int tag,
		       ElbParameter* obj)
{
  auto parent = obj->parent();

  // 該当の Cell が存在するか調べる．
  auto cell = find_cell(parent, tag);
  if ( cell ) {
    cell->add_parameter(obj);
  }
  else {
    auto cell = new CellParam(obj);
    put_cell(parent, tag, cell);
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
  CellDefParam(ElbDefParam* obj);

  /// @brief 要素の追加
  void
  add_defparam(ElbDefParam* obj) override;

  /// @brief defparam の先頭を得る．
  ElbDefParam*
  defparam() override;

  /// @brief 要素数の取得
  int
  num() override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 先頭の要素
  ElbDefParam* mTop;

  // 末尾の要素
  ElbDefParam* mTail;

  // 要素数
  int mNum;

};

// @brief コンストラクタ
CellDefParam::CellDefParam(ElbDefParam* obj) :
  mTop{obj},
  mTail{obj},
  mNum{1}
{
}

// @brief 要素の追加
void
CellDefParam::add_defparam(ElbDefParam* obj)
{
  mTail->mNext = obj;
  mTail = obj;
  ++ mNum;
}

// @brief defparam の先頭を得る．
ElbDefParam*
CellDefParam::defparam()
{
  return mTop;
}

// @brief 要素数の取得
int
CellDefParam::num()
{
  return mNum;
}

// @brief defparam を追加する．
// @param[in] dobj 登録する要素
void
TagDict::add_defparam(ElbDefParam* obj)
{
  auto parent = obj->parent();

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
// @param[in] parent 親のスコープ
// @param[out] obj_list 結果を格納するリスト
// @retval true 該当する要素が1つ以上あった．
// @retval false 該当する要素がなかった．
bool
TagDict::find_defparam_list(const VlNamedObj* parent,
			    vector<const VlDefParam*>& obj_list) const
{
  // 該当の Cell が存在するか調べる．
  auto cell = find_cell(parent, vpiDefParam);
  if ( cell ) {
    obj_list.clear();
    obj_list.reserve(cell->num());
    for ( auto obj = cell->defparam(); obj; obj = obj->next()) {
      obj_list.push_back(obj);
    }
    return true;
  }
  return false;
}


//////////////////////////////////////////////////////////////////////
// param assign 用のセル
//////////////////////////////////////////////////////////////////////
class CellParamAssign :
  public TagDictCell
{
public:

  /// @brief コンストラクタ
  CellParamAssign(ElbParamAssign* obj);

  /// @brief 要素の追加
  void
  add_paramassign(ElbParamAssign* obj) override;

  /// @brief param assign の先頭を得る．
  ElbParamAssign*
  paramassign() override;

  /// @brief 要素数の取得
  int
  num() override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 先頭の要素
  ElbParamAssign* mTop;

  // 末尾の要素
  ElbParamAssign* mTail;

  // 要素数
  int mNum;

};

// @brief コンストラクタ
CellParamAssign::CellParamAssign(ElbParamAssign* obj) :
  mTop{obj},
  mTail{obj},
  mNum{1}
{
}

// @brief 要素の追加
void
CellParamAssign::add_paramassign(ElbParamAssign* obj)
{
  mTail->mNext = obj;
  mTail = obj;
  ++ mNum;
}

// @brief param assign の先頭を得る．
ElbParamAssign*
CellParamAssign::paramassign()
{
  return mTop;
}

// @brief 要素数の取得
int
CellParamAssign::num()
{
  return mNum;
}

// @brief param assign を追加する．
// @param[in] obj 登録する要素
void
TagDict::add_paramassign(ElbParamAssign* obj)
{
  auto parent = obj->parent();

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
// @param[in] parent 親のスコープ
// @param[out] obj_list 結果を格納するリスト
// @retval true 該当する要素が1つ以上あった．
// @retval false 該当する要素がなかった．
bool
TagDict::find_paramassign_list(const VlNamedObj* parent,
			       vector<const VlParamAssign*>& obj_list) const
{
  // 該当の Cell が存在するか調べる．
  auto cell = find_cell(parent, vpiParamAssign);
  if ( cell ) {
    obj_list.clear();
    obj_list.reserve(cell->num());
    for ( auto obj = cell->paramassign(); obj; obj = obj->next() ) {
      obj_list.push_back(obj);
    }
    return true;
  }
  return false;
}


//////////////////////////////////////////////////////////////////////
// module array 用のセル
//////////////////////////////////////////////////////////////////////
class CellModuleArray :
  public TagDictCell
{
public:

  /// @brief コンストラクタ
  CellModuleArray(ElbModuleArray* obj);

  /// @brief 要素の追加
  void
  add_modulearray(ElbModuleArray* obj) override;

  /// @brief module array の先頭を得る．
  ElbModuleArray*
  modulearray() override;

  /// @brief 要素数の取得
  int
  num() override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 先頭の要素
  ElbModuleArray* mTop;

  // 末尾の要素
  ElbModuleArray* mTail;

  // 要素数
  int mNum;

};

// @brief コンストラクタ
CellModuleArray::CellModuleArray(ElbModuleArray* obj) :
  mTop{obj},
  mTail{obj},
  mNum{1}
{
}

// @brief 要素の追加
void
CellModuleArray::add_modulearray(ElbModuleArray* obj)
{
  mTail->mNext = obj;
  mTail = obj;
  ++ mNum;
}

// @brief module array の先頭を得る．
ElbModuleArray*
CellModuleArray::modulearray()
{
  return mTop;
}

// @brief 要素数の取得
int
CellModuleArray::num()
{
  return mNum;
}

// @brief module array を追加する．
// @param[in] obj 登録する要素
void
TagDict::add_modulearray(ElbModuleArray* obj)
{
  auto parent = obj->parent();

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
// @param[in] parent 親のスコープ
// @param[out] obj_list 結果を格納するリスト
// @retval true 該当する要素が1つ以上あった．
// @retval false 該当する要素がなかった．
bool
TagDict::find_modulearray_list(const VlNamedObj* parent,
			       vector<const VlModuleArray*>& obj_list) const
{
  // 該当の Cell が存在するか調べる．
  auto cell = find_cell(parent, vpiModuleArray);
  if ( cell ) {
    obj_list.clear();
    obj_list.reserve(cell->num());
    for ( auto obj = cell->modulearray(); obj; obj = obj->next()) {
      obj_list.push_back(obj);
    }
    return true;
  }
  return false;
}


//////////////////////////////////////////////////////////////////////
// module 用のセル
//////////////////////////////////////////////////////////////////////
class CellModule :
  public TagDictCell
{
public:

  /// @brief コンストラクタ
  CellModule(ElbModule* obj);

  /// @brief 要素の追加
  void
  add_module(ElbModule* obj) override;

  /// @brief module の先頭を得る．
  ElbModule*
  module() override;

  /// @brief 要素数の取得
  int
  num() override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 先頭の要素
  ElbModule* mTop;

  // 末尾の要素
  ElbModule* mTail;

  // 要素数
  int mNum;

};

// @brief コンストラクタ
CellModule::CellModule(ElbModule* obj) :
  mTop{obj},
  mTail{obj},
  mNum{1}
{
}

// @brief 要素の追加
void
CellModule::add_module(ElbModule* obj)
{
  mTail->mNext = obj;
  mTail = obj;
  ++ mNum;
}

// @brief module の先頭を得る．
ElbModule*
CellModule::module()
{
  return mTop;
}

// @brief 要素数の取得
int
CellModule::num()
{
  return mNum;
}

// @brief module を追加する．
// @param[in] obj 登録する要素
void
TagDict::add_module(ElbModule* obj)
{
  auto parent = obj->parent();

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
// @param[in] parent 親のスコープ
// @param[out] obj_list 結果を格納するリスト
// @retval true 該当する要素が1つ以上あった．
// @retval false 該当する要素がなかった．
bool
TagDict::find_module_list(const VlNamedObj* parent,
			  vector<const VlModule*>& obj_list) const
{
  // 該当の Cell が存在するか調べる．
  auto cell = find_cell(parent, vpiModule);
  if ( cell ) {
    obj_list.clear();
    obj_list.reserve(cell->num());
    for ( auto obj = cell->module(); obj; obj = obj->next()) {
      obj_list.push_back(obj);
    }
    return true;
  }
  return false;
}


//////////////////////////////////////////////////////////////////////
// primitive array 用のセル
//////////////////////////////////////////////////////////////////////
class CellPrimArray :
  public TagDictCell
{
public:

  /// @brief コンストラクタ
  CellPrimArray(ElbPrimArray* obj);

  /// @brief 要素の追加
  void
  add_primarray(ElbPrimArray* obj) override;

  /// @brief primitive array の先頭を得る．
  ElbPrimArray*
  primarray() override;

  /// @brief 要素数の取得
  int
  num() override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 先頭の要素
  ElbPrimArray* mTop;

  // 末尾の要素
  ElbPrimArray* mTail;

  // 要素数
  int mNum;

};

// @brief コンストラクタ
CellPrimArray::CellPrimArray(ElbPrimArray* obj) :
  mTop{obj},
  mTail{obj},
  mNum{1}
{
}

// @brief 要素の追加
void
CellPrimArray::add_primarray(ElbPrimArray* obj)
{
  mTail->mNext = obj;
  mTail = obj;
  ++ mNum;
}

// @brief primitive array の先頭を得る．
ElbPrimArray*
CellPrimArray::primarray()
{
  return mTop;
}

// @brief 要素数の取得
int
CellPrimArray::num()
{
  return mNum;
}

// @brief primitive array を追加する．
// @param[in] parent 親のスコープ
// @param[in] obj 登録する要素
void
TagDict::add_primarray(ElbPrimArray* obj)
{
  auto parent = obj->parent();

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
// @param[in] parent 親のスコープ
// @param[out] obj_list 結果を格納するリスト
// @retval true 該当する要素が1つ以上あった．
// @retval false 該当する要素がなかった．
bool
TagDict::find_primarray_list(const VlNamedObj* parent,
			     vector<const VlPrimArray*>& obj_list) const
{
  // 該当の Cell が存在するか調べる．
  auto cell = find_cell(parent, vpiPrimitiveArray);
  if ( cell ) {
    obj_list.clear();
    obj_list.reserve(cell->num());
    for ( auto obj = cell->primarray(); obj; obj = obj->next()) {
      obj_list.push_back(obj);
    }
    return true;
  }
  return false;
}


//////////////////////////////////////////////////////////////////////
// primitive 用のセル
//////////////////////////////////////////////////////////////////////
class CellPrimitive :
  public TagDictCell
{
public:

  /// @brief コンストラクタ
  CellPrimitive(ElbPrimitive* obj);

  /// @brief 要素の追加
  void
  add_primitive(ElbPrimitive* obj) override;

  /// @brief primitive の先頭を得る．
  ElbPrimitive*
  primitive() override;

  /// @brief 要素数の取得
  int
  num() override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 先頭の要素
  ElbPrimitive* mTop;

  // 末尾の要素
  ElbPrimitive* mTail;

  // 要素数
  int mNum;

};

// @brief コンストラクタ
CellPrimitive::CellPrimitive(ElbPrimitive* obj) :
  mTop{obj},
  mTail{obj},
  mNum{1}
{
}

// @brief 要素の追加
void
CellPrimitive::add_primitive(ElbPrimitive* obj)
{
  mTail->mNext = obj;
  mTail = obj;
  ++ mNum;
}

// @brief primitive の先頭を得る．
ElbPrimitive*
CellPrimitive::primitive()
{
  return mTop;
}

// @brief 要素数の取得
int
CellPrimitive::num()
{
  return mNum;
}

// @brief primitive を追加する．
// @param[in] obj 登録する要素
void
TagDict::add_primitive(ElbPrimitive* obj)
{
  auto parent = obj->parent();

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
// @param[in] parent 親のスコープ
// @param[out] obj_list 結果を格納するリスト
// @retval true 該当する要素が1つ以上あった．
// @retval false 該当する要素がなかった．
bool
TagDict::find_primitive_list(const VlNamedObj* parent,
			     vector<const VlPrimitive*>& obj_list) const
{
  // 該当の Cell が存在するか調べる．
  auto cell = find_cell(parent, vpiPrimitive);
  if ( cell ) {
    obj_list.clear();
    obj_list.reserve(cell->num());
    for ( auto obj = cell->primitive(); obj; obj = obj->next()) {
      obj_list.push_back(obj);
    }
    return true;
  }
  return false;
}


//////////////////////////////////////////////////////////////////////
// タスク用のセル
//////////////////////////////////////////////////////////////////////
class CellTask :
  public TagDictCell
{
public:

  /// @brief コンストラクタ
  CellTask(ElbTaskFunc* obj);

  /// @brief 要素の追加
  void
  add_task(ElbTaskFunc* obj) override;

  /// @brief 要素の先頭を得る．
  ElbTaskFunc*
  task() override;

  /// @brief 要素数の取得
  int
  num() override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 先頭の要素
  ElbTaskFunc* mTop;

  // 末尾の要素
  ElbTaskFunc* mTail;

  // 要素数
  int mNum;

};

// @brief コンストラクタ
CellTask::CellTask(ElbTaskFunc* obj) :
  mTop{obj},
  mTail{obj},
  mNum{1}
{
}

// @brief 要素の追加
void
CellTask::add_task(ElbTaskFunc* obj)
{
  mTail->mNext = obj;
  mTail = obj;
  ++ mNum;
}

// @brief 要素の先頭を得る．
ElbTaskFunc*
CellTask::task()
{
  return mTop;
}

// @brief 要素数の取得
int
CellTask::num()
{
  return mNum;
}

// @brief タスクを追加する．
// @param[in] obj 登録する要素
void
TagDict::add_task(ElbTaskFunc* obj)
{
  auto parent = obj->parent();

  // 該当の Cell が存在するか調べる．
  auto cell = find_cell(parent, vpiTask);
  if ( cell ) {
    cell->add_task(obj);
  }
  else {
    auto cell = new CellTask(obj);
    put_cell(parent, vpiTask, cell);
  }
}

// @brief タスクのリストを取り出す．
// @param[in] parent 親のスコープ
// @param[out] obj_list 結果を格納するリスト
// @retval true 該当する要素が1つ以上あった．
// @retval false 該当する要素がなかった．
bool
TagDict::find_task_list(const VlNamedObj* parent,
			vector<const VlTaskFunc*>& obj_list) const
{
  // 該当の Cell が存在するか調べる．
  auto cell = find_cell(parent, vpiTask);
  if ( cell ) {
    obj_list.clear();
    obj_list.reserve(cell->num());
    for ( auto obj = cell->task();obj; obj = obj->next() ) {
      obj_list.push_back(obj);
    }
    return true;
  }
  return false;
}


//////////////////////////////////////////////////////////////////////
// 関数用のセル
//////////////////////////////////////////////////////////////////////
class CellFunction :
  public TagDictCell
{
public:

  /// @brief コンストラクタ
  CellFunction(ElbTaskFunc* obj);

  /// @brief 要素の追加
  void
  add_function(ElbTaskFunc* obj) override;

  /// @brief 要素の先頭を得る．
  ElbTaskFunc*
  function() override;

  /// @brief 要素数の取得
  int
  num() override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 先頭の要素
  ElbTaskFunc* mTop;

  // 末尾の要素
  ElbTaskFunc* mTail;

  // 要素数
  int mNum;

};

// @brief コンストラクタ
CellFunction::CellFunction(ElbTaskFunc* obj) :
  mTop{obj},
  mTail{obj},
  mNum{1}
{
}

// @brief 要素の追加
void
CellFunction::add_function(ElbTaskFunc* obj)
{
  mTail->mNext = obj;
  mTail = obj;
  ++ mNum;
}

// @brief 要素の先頭を得る．
ElbTaskFunc*
CellFunction::function()
{
  return mTop;
}

// @brief 要素数の取得
int
CellFunction::num()
{
  return mNum;
}

// @brief function を追加する．
// @param[in] obj 登録する要素
void
TagDict::add_function(ElbTaskFunc* obj)
{
  auto parent = obj->parent();

  // 該当の Cell が存在するか調べる．
  auto cell = find_cell(parent, vpiFunction);
  if ( cell ) {
    cell->add_function(obj);
  }
  else {
    auto cell = new CellFunction(obj);
    put_cell(parent, vpiFunction, cell);
  }
}

// @brief function のリストを取り出す．
// @param[in] parent 親のスコープ
// @param[out] obj_list 結果を格納するリスト
// @retval true 該当する要素が1つ以上あった．
// @retval false 該当する要素がなかった．
bool
TagDict::find_function_list(const VlNamedObj* parent,
			    vector<const VlTaskFunc*>& obj_list) const
{
  // 該当の Cell が存在するか調べる．
  auto cell = find_cell(parent, vpiFunction);
  if ( cell ) {
    obj_list.clear();
    obj_list.reserve(cell->num());
    for ( auto obj = cell->function(); obj; obj = obj->next()) {
      obj_list.push_back(obj);
    }
    return true;
  }
  return false;
}


//////////////////////////////////////////////////////////////////////
// continuous assignment 用のセル
//////////////////////////////////////////////////////////////////////
class CellContAssign :
  public TagDictCell
{
public:

  /// @brief コンストラクタ
  CellContAssign(ElbContAssign* obj);

  /// @brief 要素の追加
  void
  add_contassign(ElbContAssign* obj) override;

  /// @brief 要素の先頭を得る．
  ElbContAssign*
  contassign() override;

  /// @brief 要素数の取得
  int
  num() override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 先頭の要素
  ElbContAssign* mTop;

  // 末尾の要素
  ElbContAssign* mTail;

  // 要素数
  int mNum;

};

// @brief コンストラクタ
CellContAssign::CellContAssign(ElbContAssign* obj) :
  mTop{obj},
  mTail{obj},
  mNum{1}
{
}

// @brief 要素の追加
void
CellContAssign::add_contassign(ElbContAssign* obj)
{
  mTail->mNext = obj;
  mTail = obj;
  ++ mNum;
}

// @brief 要素の先頭を得る．
ElbContAssign*
CellContAssign::contassign()
{
  return mTop;
}

// @brief 要素数の取得
int
CellContAssign::num()
{
  return mNum;
}

// @brief continuous assignment を追加する．
// @param[in] obj 登録する要素
void
TagDict::add_contassign(ElbContAssign* obj)
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
// @param[in] parent 親のスコープ
// @param[out] obj_list 結果を格納するリスト
// @retval true 該当する要素が1つ以上あった．
// @retval false 該当する要素がなかった．
bool
TagDict::find_contassign_list(const VlNamedObj* parent,
			      vector<const VlContAssign*>& obj_list) const
{
  // 該当の Cell が存在するか調べる．
  auto cell = find_cell(parent, vpiContAssign);
  if ( cell ) {
    obj_list.clear();
    obj_list.reserve(cell->num());
    for ( auto obj = cell->contassign(); obj; obj = obj->next() ) {
      obj_list.push_back(obj);
    }
    return true;
  }
  return false;
}


//////////////////////////////////////////////////////////////////////
// process 用のセル
//////////////////////////////////////////////////////////////////////
class CellProcess :
  public TagDictCell
{
public:

  /// @brief コンストラクタ
  CellProcess(ElbProcess* obj);

  /// @brief 要素の追加
  void
  add_process(ElbProcess* obj) override;

  /// @brief 要素の先頭を得る．
  ElbProcess*
  process() override;

  /// @brief 要素数の取得
  int
  num() override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 先頭の要素
  ElbProcess* mTop;

  // 末尾の要素
  ElbProcess* mTail;

  // 要素数
  int mNum;

};

// @brief コンストラクタ
CellProcess::CellProcess(ElbProcess* obj) :
  mTop{obj},
  mTail{obj},
  mNum{1}
{
}

// @brief 要素の追加
void
CellProcess::add_process(ElbProcess* obj)
{
  mTail->mNext = obj;
  mTail = obj;
  ++ mNum;
}

// @brief 要素の先頭を得る．
ElbProcess*
CellProcess::process()
{
  return mTop;
}

// @brief 要素数の取得
int
CellProcess::num()
{
  return mNum;
}

// @brief process を追加する．
// @param[in] obj 登録する要素
void
TagDict::add_process(ElbProcess* obj)
{
  auto parent = obj->parent();

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
// @param[in] parent 親のスコープ
// @param[out] obj_list 結果を格納するリスト
// @retval true 該当する要素が1つ以上あった．
// @retval false 該当する要素がなかった．
bool
TagDict::find_process_list(const VlNamedObj* parent,
			   vector<const VlProcess*>& obj_list) const
{
  // 該当の Cell が存在するか調べる．
  auto cell = find_cell(parent, vpiProcess);
  if ( cell ) {
    obj_list.clear();
    obj_list.reserve(cell->num());
    for ( auto obj = cell->process(); obj; obj = obj->next()) {
      obj_list.push_back(obj);
    }
    return true;
  }
  return false;
}

END_NAMESPACE_YM_VERILOG
