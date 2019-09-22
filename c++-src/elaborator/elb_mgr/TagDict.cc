﻿
/// @file TagDict.cc
/// @brief TagDict の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "TagDict.h"
#include "TagDictCell.h"
#include "ym/vl/VlObj.h"
#include "ElbScope.h"
#include "ElbModule.h"
#include "ElbParamAssign.h"
#include "ElbDecl.h"
#include "ElbParameter.h"
#include "ElbPrimitive.h"
#include "ElbTaskFunc.h"
#include "ElbContAssign.h"
#include "ElbProcess.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス TagDict
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] alloc メモリ確保用のオブジェクト
TagDict::TagDict(Alloc& alloc) :
  mAlloc(alloc),
  mNum(0)
{
  alloc_table(1024);
}

// @brief デストラクタ
TagDict::~TagDict()
{
  delete [] mTable;
}

// @brief 内容を空にする
void
TagDict::clear()
{
  for (ymuint i = 0; i < mSize; ++ i) {
    mTable[i] = nullptr;
  }
  mNum = 0;
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
  if ( mNum >= mLimit ) {
    // テーブルを拡張する．
    ymuint old_size = mSize;
    TagDictCell** old_table = mTable;
    alloc_table(old_size << 1);
    for (ymuint i = 0; i < old_size; ++ i) {
      for (TagDictCell* cell = old_table[i]; cell; ) {
	TagDictCell* next = cell->mLink;
	ymuint pos = hash_func(cell->mParent, cell->mTag);
	cell->mLink = mTable[pos];
	mTable[pos] = cell;
	cell = next;
      }
    }
    delete [] old_table;
  }
  ymuint pos = hash_func(parent, tag);
  cell->mParent = parent;
  cell->mTag = tag;
  cell->mLink = mTable[pos];
  mTable[pos] = cell;
  ++ mNum;
}

// @brief タグから該当する Cell を探す．
// @param[in] parent 親のスコープ
// @param[in] tag 要素の型を表すタグ (vpi_user.h 参照)
TagDictCell*
TagDict::find_cell(const VlNamedObj* parent,
		   int tag) const
{
  ymuint pos = hash_func(parent, tag);
  for (TagDictCell* cell = mTable[pos]; cell; cell = cell->mLink) {
    if ( cell->mParent == parent && cell->mTag == tag ) {
      return cell;
    }
  }
  return nullptr;
}

// @brief このオブジェクトが使用しているメモリ量を返す．
ymuint
TagDict::allocated_size() const
{
  return sizeof(TagDictCell*) * mSize;
}

// @brief テーブルの領域を確保する．
void
TagDict::alloc_table(ymuint size)
{
  mSize = size;
  mLimit = static_cast<ymuint>(mSize * 1.8);
  mTable = new TagDictCell*[mSize];
  for (ymuint i = 0; i < mSize; ++ i) {
    mTable[i] = nullptr;
  }
}

// @brief ハッシュ値を計算する．
ymuint
TagDict::hash_func(const VlNamedObj* parent,
		   int tag) const
{
  return ((reinterpret_cast<ympuint>(parent) * tag) >> 8) % mSize;
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
  virtual
  void
  add_internalscope(ElbScope* obj);

  /// @brief 要素の先頭を得る．
  virtual
  const ElbScope*
  internalscope();

  /// @brief 要素数の取得
  virtual
  ymuint
  num();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 先頭の要素
  ElbScope* mTop;

  // 末尾の要素
  ElbScope* mTail;

  // 要素数
  ymuint32 mNum;

};

// @brief コンストラクタ
CellScope::CellScope(ElbScope* obj) :
  mTop(obj),
  mTail(obj),
  mNum(1)
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
ymuint
CellScope::num()
{
  return mNum;
}

// @brief internal scope を追加する．
// @param[in] obj 登録する要素
void
TagDict::add_internalscope(ElbScope* obj)
{
  const VlNamedObj* parent = obj->parent();

  // 該当の Cell が存在するか調べる．
  TagDictCell* cell = find_cell(parent, vpiInternalScope);
  if ( cell ) {
    cell->add_internalscope(obj);
  }
  else {
    void* p = mAlloc.get_memory(sizeof(CellScope));
    TagDictCell* cell = new (p) CellScope(obj);
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
  TagDictCell* cell = find_cell(parent, vpiInternalScope);
  if ( cell ) {
    obj_list.clear();
    obj_list.reserve(cell->num());
    for (const ElbScope* obj = cell->internalscope();
	 obj; obj = obj->next()) {
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
  virtual
  void
  add_decl(ElbDecl* obj);

  /// @brief 宣言要素の先頭を得る．
  virtual
  ElbDecl*
  decl();

  /// @brief 要素数の取得
  virtual
  ymuint
  num();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 先頭の要素
  ElbDecl* mTop;

  // 末尾の要素
  ElbDecl* mTail;

  // 要素数
  ymuint32 mNum;

};

// @brief コンストラクタ
CellDecl::CellDecl(ElbDecl* obj) :
  mTop(obj),
  mTail(obj),
  mNum(1)
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
ymuint
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
  const VlNamedObj* parent = obj->parent();

  // 該当の Cell が存在するか調べる．
  TagDictCell* cell = find_cell(parent, tag);
  if ( cell ) {
    cell->add_decl(obj);
  }
  else {
    void* p = mAlloc.get_memory(sizeof(CellDecl));
    TagDictCell* cell = new (p) CellDecl(obj);
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
  TagDictCell* cell = find_cell(parent, tag);
  if ( cell ) {
    obj_list.clear();
    obj_list.reserve(cell->num());
    if ( tag == kVpiParameter || tag == kVpiSpecParam ) {
      for (const ElbParameter* obj = cell->parameter();
	   obj; obj = obj->next()) {
	obj_list.push_back(obj);
      }
    }
    else {
      for (const ElbDecl* obj = cell->decl();
	   obj; obj = obj->next()) {
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
  virtual
  void
  add_declarray(ElbDeclArray* obj);

  /// @brief 宣言要素の先頭を得る．
  virtual
  ElbDeclArray*
  declarray();

  /// @brief 要素数の取得
  virtual
  ymuint
  num();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 先頭の要素
  ElbDeclArray* mTop;

  // 末尾の要素
  ElbDeclArray* mTail;

  // 要素数
  ymuint32 mNum;

};

// @brief コンストラクタ
CellDeclArray::CellDeclArray(ElbDeclArray* obj) :
  mTop(obj),
  mTail(obj),
  mNum(1)
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
ymuint
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
  const VlNamedObj* parent = obj->parent();

  // 該当の Cell が存在するか調べる．
  TagDictCell* cell = find_cell(parent, tag);
  if ( cell ) {
    cell->add_declarray(obj);
  }
  else {
    void* p = mAlloc.get_memory(sizeof(CellDeclArray));
    TagDictCell* cell = new (p) CellDeclArray(obj);
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
  TagDictCell* cell = find_cell(parent, tag);
  if ( cell ) {
    obj_list.clear();
    obj_list.reserve(cell->num());
    for (const ElbDeclArray* obj = cell->declarray();
	 obj; obj = obj->next()) {
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
  virtual
  void
  add_parameter(ElbParameter* obj);

  /// @brief 宣言要素の先頭を得る．
  virtual
  ElbParameter*
  parameter();

  /// @brief 要素数の取得
  virtual
  ymuint
  num();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 先頭の要素
  ElbParameter* mTop;

  // 末尾の要素
  ElbParameter* mTail;

  // 要素数
  ymuint32 mNum;

};

// @brief コンストラクタ
CellParam::CellParam(ElbParameter* obj) :
  mTop(obj),
  mTail(obj),
  mNum(1)
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
ymuint
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
  const VlNamedObj* parent = obj->parent();

  // 該当の Cell が存在するか調べる．
  TagDictCell* cell = find_cell(parent, tag);
  if ( cell ) {
    cell->add_parameter(obj);
  }
  else {
    void* p = mAlloc.get_memory(sizeof(CellParam));
    TagDictCell* cell = new (p) CellParam(obj);
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
  virtual
  void
  add_defparam(ElbDefParam* obj);

  /// @brief defparam の先頭を得る．
  virtual
  ElbDefParam*
  defparam();

  /// @brief 要素数の取得
  virtual
  ymuint
  num();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 先頭の要素
  ElbDefParam* mTop;

  // 末尾の要素
  ElbDefParam* mTail;

  // 要素数
  ymuint32 mNum;

};

// @brief コンストラクタ
CellDefParam::CellDefParam(ElbDefParam* obj) :
  mTop(obj),
  mTail(obj),
  mNum(1)
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
ymuint
CellDefParam::num()
{
  return mNum;
}

// @brief defparam を追加する．
// @param[in] dobj 登録する要素
void
TagDict::add_defparam(ElbDefParam* obj)
{
  const VlNamedObj* parent = obj->parent();

  // 該当の Cell が存在するか調べる．
  TagDictCell* cell = find_cell(parent, vpiDefParam);
  if ( cell ) {
    cell->add_defparam(obj);
  }
  else {
    void* p = mAlloc.get_memory(sizeof(CellDefParam));
    TagDictCell* cell = new (p) CellDefParam(obj);
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
  TagDictCell* cell = find_cell(parent, vpiDefParam);
  if ( cell ) {
    obj_list.clear();
    obj_list.reserve(cell->num());
    for (const ElbDefParam* obj = cell->defparam();
	 obj; obj = obj->next()) {
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
  virtual
  void
  add_paramassign(ElbParamAssign* obj);

  /// @brief param assign の先頭を得る．
  virtual
  ElbParamAssign*
  paramassign();

  /// @brief 要素数の取得
  virtual
  ymuint
  num();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 先頭の要素
  ElbParamAssign* mTop;

  // 末尾の要素
  ElbParamAssign* mTail;

  // 要素数
  ymuint32 mNum;

};

// @brief コンストラクタ
CellParamAssign::CellParamAssign(ElbParamAssign* obj) :
  mTop(obj),
  mTail(obj),
  mNum(1)
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
ymuint
CellParamAssign::num()
{
  return mNum;
}

// @brief param assign を追加する．
// @param[in] obj 登録する要素
void
TagDict::add_paramassign(ElbParamAssign* obj)
{
  const VlNamedObj* parent = obj->parent();

  // 該当の Cell が存在するか調べる．
  TagDictCell* cell = find_cell(parent, vpiParamAssign);
  if ( cell ) {
    cell->add_paramassign(obj);
  }
  else {
    void* p = mAlloc.get_memory(sizeof(CellParamAssign));
    TagDictCell* cell = new (p) CellParamAssign(obj);
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
  TagDictCell* cell = find_cell(parent, vpiParamAssign);
  if ( cell ) {
    obj_list.clear();
    obj_list.reserve(cell->num());
    for (const ElbParamAssign* obj = cell->paramassign();
	 obj; obj = obj->next()) {
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
  virtual
  void
  add_modulearray(ElbModuleArray* obj);

  /// @brief module array の先頭を得る．
  virtual
  ElbModuleArray*
  modulearray();

  /// @brief 要素数の取得
  virtual
  ymuint
  num();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 先頭の要素
  ElbModuleArray* mTop;

  // 末尾の要素
  ElbModuleArray* mTail;

  // 要素数
  ymuint32 mNum;

};

// @brief コンストラクタ
CellModuleArray::CellModuleArray(ElbModuleArray* obj) :
  mTop(obj),
  mTail(obj),
  mNum(1)
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
ymuint
CellModuleArray::num()
{
  return mNum;
}

// @brief module array を追加する．
// @param[in] obj 登録する要素
void
TagDict::add_modulearray(ElbModuleArray* obj)
{
  const VlNamedObj* parent = obj->parent();

  // 該当の Cell が存在するか調べる．
  TagDictCell* cell = find_cell(parent, vpiModuleArray);
  if ( cell ) {
    cell->add_modulearray(obj);
  }
  else {
    void* p = mAlloc.get_memory(sizeof(CellModuleArray));
    TagDictCell* cell = new (p) CellModuleArray(obj);
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
  TagDictCell* cell = find_cell(parent, vpiModuleArray);
  if ( cell ) {
    obj_list.clear();
    obj_list.reserve(cell->num());
    for (const ElbModuleArray* obj = cell->modulearray();
	 obj; obj = obj->next()) {
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
  virtual
  void
  add_module(ElbModule* obj);

  /// @brief module の先頭を得る．
  virtual
  ElbModule*
  module();

  /// @brief 要素数の取得
  virtual
  ymuint
  num();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 先頭の要素
  ElbModule* mTop;

  // 末尾の要素
  ElbModule* mTail;

  // 要素数
  ymuint32 mNum;

};

// @brief コンストラクタ
CellModule::CellModule(ElbModule* obj) :
  mTop(obj),
  mTail(obj),
  mNum(1)
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
ymuint
CellModule::num()
{
  return mNum;
}

// @brief module を追加する．
// @param[in] obj 登録する要素
void
TagDict::add_module(ElbModule* obj)
{
  const VlNamedObj* parent = obj->parent();

  // 該当の Cell が存在するか調べる．
  TagDictCell* cell = find_cell(parent, vpiModule);
  if ( cell ) {
    cell->add_module(obj);
  }
  else {
    void* p = mAlloc.get_memory(sizeof(CellModule));
    TagDictCell* cell = new (p) CellModule(obj);
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
  TagDictCell* cell = find_cell(parent, vpiModule);
  if ( cell ) {
    obj_list.clear();
    obj_list.reserve(cell->num());
    for (const ElbModule* obj = cell->module();
	 obj; obj = obj->next()) {
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
  virtual
  void
  add_primarray(ElbPrimArray* obj);

  /// @brief primitive array の先頭を得る．
  virtual
  ElbPrimArray*
  primarray();

  /// @brief 要素数の取得
  virtual
  ymuint
  num();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 先頭の要素
  ElbPrimArray* mTop;

  // 末尾の要素
  ElbPrimArray* mTail;

  // 要素数
  ymuint32 mNum;

};

// @brief コンストラクタ
CellPrimArray::CellPrimArray(ElbPrimArray* obj) :
  mTop(obj),
  mTail(obj),
  mNum(1)
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
ymuint
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
  const VlNamedObj* parent = obj->parent();

  // 該当の Cell が存在するか調べる．
  TagDictCell* cell = find_cell(parent, vpiPrimitiveArray);
  if ( cell ) {
    cell->add_primarray(obj);
  }
  else {
    void* p = mAlloc.get_memory(sizeof(CellPrimArray));
    TagDictCell* cell = new (p) CellPrimArray(obj);
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
  TagDictCell* cell = find_cell(parent, vpiPrimitiveArray);
  if ( cell ) {
    obj_list.clear();
    obj_list.reserve(cell->num());
    for (const ElbPrimArray* obj = cell->primarray();
	 obj; obj = obj->next()) {
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
  virtual
  void
  add_primitive(ElbPrimitive* obj);

  /// @brief primitive の先頭を得る．
  virtual
  ElbPrimitive*
  primitive();

  /// @brief 要素数の取得
  virtual
  ymuint
  num();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 先頭の要素
  ElbPrimitive* mTop;

  // 末尾の要素
  ElbPrimitive* mTail;

  // 要素数
  ymuint32 mNum;

};

// @brief コンストラクタ
CellPrimitive::CellPrimitive(ElbPrimitive* obj) :
  mTop(obj),
  mTail(obj),
  mNum(1)
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
ymuint
CellPrimitive::num()
{
  return mNum;
}

// @brief primitive を追加する．
// @param[in] obj 登録する要素
void
TagDict::add_primitive(ElbPrimitive* obj)
{
  const VlNamedObj* parent = obj->parent();

  // 該当の Cell が存在するか調べる．
  TagDictCell* cell = find_cell(parent, vpiPrimitive);
  if ( cell ) {
    cell->add_primitive(obj);
  }
  else {
    void* p = mAlloc.get_memory(sizeof(CellPrimitive));
    TagDictCell* cell = new (p) CellPrimitive(obj);
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
  TagDictCell* cell = find_cell(parent, vpiPrimitive);
  if ( cell ) {
    obj_list.clear();
    obj_list.reserve(cell->num());
    for (const ElbPrimitive* obj = cell->primitive();
	 obj; obj = obj->next()) {
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
  virtual
  void
  add_task(ElbTaskFunc* obj);

  /// @brief 要素の先頭を得る．
  virtual
  ElbTaskFunc*
  task();

  /// @brief 要素数の取得
  virtual
  ymuint
  num();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 先頭の要素
  ElbTaskFunc* mTop;

  // 末尾の要素
  ElbTaskFunc* mTail;

  // 要素数
  ymuint32 mNum;

};

// @brief コンストラクタ
CellTask::CellTask(ElbTaskFunc* obj) :
  mTop(obj),
  mTail(obj),
  mNum(1)
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
ymuint
CellTask::num()
{
  return mNum;
}

// @brief タスクを追加する．
// @param[in] obj 登録する要素
void
TagDict::add_task(ElbTaskFunc* obj)
{
  const VlNamedObj* parent = obj->parent();

  // 該当の Cell が存在するか調べる．
  TagDictCell* cell = find_cell(parent, vpiTask);
  if ( cell ) {
    cell->add_task(obj);
  }
  else {
    void* p = mAlloc.get_memory(sizeof(CellTask));
    TagDictCell* cell = new (p) CellTask(obj);
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
  TagDictCell* cell = find_cell(parent, vpiTask);
  if ( cell ) {
    obj_list.clear();
    obj_list.reserve(cell->num());
    for (const ElbTaskFunc* obj = cell->task();
	 obj; obj = obj->next()) {
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
  virtual
  void
  add_function(ElbTaskFunc* obj);

  /// @brief 要素の先頭を得る．
  virtual
  ElbTaskFunc*
  function();

  /// @brief 要素数の取得
  virtual
  ymuint
  num();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 先頭の要素
  ElbTaskFunc* mTop;

  // 末尾の要素
  ElbTaskFunc* mTail;

  // 要素数
  ymuint32 mNum;

};

// @brief コンストラクタ
CellFunction::CellFunction(ElbTaskFunc* obj) :
  mTop(obj),
  mTail(obj),
  mNum(1)
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
ymuint
CellFunction::num()
{
  return mNum;
}

// @brief function を追加する．
// @param[in] obj 登録する要素
void
TagDict::add_function(ElbTaskFunc* obj)
{
  const VlNamedObj* parent = obj->parent();

  // 該当の Cell が存在するか調べる．
  TagDictCell* cell = find_cell(parent, vpiFunction);
  if ( cell ) {
    cell->add_function(obj);
  }
  else {
    void* p = mAlloc.get_memory(sizeof(CellFunction));
    TagDictCell* cell = new (p) CellFunction(obj);
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
  TagDictCell* cell = find_cell(parent, vpiFunction);
  if ( cell ) {
    obj_list.clear();
    obj_list.reserve(cell->num());
    for (const ElbTaskFunc* obj = cell->function();
	 obj; obj = obj->next()) {
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
  virtual
  void
  add_contassign(ElbContAssign* obj);

  /// @brief 要素の先頭を得る．
  virtual
  ElbContAssign*
  contassign();

  /// @brief 要素数の取得
  virtual
  ymuint
  num();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 先頭の要素
  ElbContAssign* mTop;

  // 末尾の要素
  ElbContAssign* mTail;

  // 要素数
  ymuint32 mNum;

};

// @brief コンストラクタ
CellContAssign::CellContAssign(ElbContAssign* obj) :
  mTop(obj),
  mTail(obj),
  mNum(1)
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
ymuint
CellContAssign::num()
{
  return mNum;
}

// @brief continuous assignment を追加する．
// @param[in] obj 登録する要素
void
TagDict::add_contassign(ElbContAssign* obj)
{
  const VlNamedObj* parent = obj->module();

  // 該当の Cell が存在するか調べる．
  TagDictCell* cell = find_cell(parent, vpiContAssign);
  if ( cell ) {
    cell->add_contassign(obj);
  }
  else {
    void* p = mAlloc.get_memory(sizeof(CellContAssign));
    TagDictCell* cell = new (p) CellContAssign(obj);
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
  TagDictCell* cell = find_cell(parent, vpiContAssign);
  if ( cell ) {
    obj_list.clear();
    obj_list.reserve(cell->num());
    for (const ElbContAssign* obj = cell->contassign();
	 obj; obj = obj->next()) {
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
  virtual
  void
  add_process(ElbProcess* obj);

  /// @brief 要素の先頭を得る．
  virtual
  ElbProcess*
  process();

  /// @brief 要素数の取得
  virtual
  ymuint
  num();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 先頭の要素
  ElbProcess* mTop;

  // 末尾の要素
  ElbProcess* mTail;

  // 要素数
  ymuint32 mNum;

};

// @brief コンストラクタ
CellProcess::CellProcess(ElbProcess* obj) :
  mTop(obj),
  mTail(obj),
  mNum(1)
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
ymuint
CellProcess::num()
{
  return mNum;
}

// @brief process を追加する．
// @param[in] obj 登録する要素
void
TagDict::add_process(ElbProcess* obj)
{
  const VlNamedObj* parent = obj->parent();

  // 該当の Cell が存在するか調べる．
  TagDictCell* cell = find_cell(parent, vpiProcess);
  if ( cell ) {
    cell->add_process(obj);
  }
  else {
    void* p = mAlloc.get_memory(sizeof(CellProcess));
    TagDictCell* cell = new (p) CellProcess(obj);
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
  TagDictCell* cell = find_cell(parent, vpiProcess);
  if ( cell ) {
    obj_list.clear();
    obj_list.reserve(cell->num());
    for (const ElbProcess* obj = cell->process();
	 obj; obj = obj->next()) {
      obj_list.push_back(obj);
    }
    return true;
  }
  return false;
}

END_NAMESPACE_YM_VERILOG
