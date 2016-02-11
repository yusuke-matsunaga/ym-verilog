﻿
/// @file AttrHash.cc
/// @brief AttrHash の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AttrHash.h"
#include "ym/vl/VlObj.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス AttrHash
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] alloc メモリ確保用のアロケータ
AttrHash::AttrHash(Alloc& alloc) :
  mAlloc(alloc),
  mNum(0)
{
  alloc_table(1024);
}

// @brief デストラクタ
AttrHash::~AttrHash()
{
  delete [] mTable;
}

// @brief 内容をクリアする．
void
AttrHash::clear()
{
  for (ymuint i = 0; i < mSize; ++ i ) {
    mTable[i] = nullptr;
  }
  mNum = 0;
}

// @brief 属性を追加する．
// @param[in] obj 対象のオブジェクト
// @param[in] def 定義側の属性の時 true とするフラグ
// @param[in] attr_list 付加する属性リスト
void
AttrHash::add(const VlObj* obj,
	      bool def,
	      ElbAttrList* attr_list)
{
  // 該当の Cell が存在するか調べる．
  Cell* cell = find_cell(obj);
  if ( !cell ) {
    cell = new_cell(obj);
  }
  ymuint pos = (def) ? 1 : 0;
  ASSERT_COND(cell->mAttrList[pos] == nullptr );
  cell->mAttrList[pos] = attr_list;
}

// @brief 属性を取り出す．
// @param[in] obj 対象のオブジェクト
// @param[in] def 定義側の属性の時 true とするフラグ
ElbAttrList*
AttrHash::find(const VlObj* obj,
	       bool def) const
{
  // 該当の Cell が存在するか調べる．
  Cell* cell = find_cell(obj);
  if ( cell ) {
    ymuint pos = (def) ? 1 : 0;
    return cell->mAttrList[pos];
  }
  return nullptr;
}

// @brief 新しい Cell を生成する．
// @param[in] obj 対象のオブジェクト
AttrHash::Cell*
AttrHash::new_cell(const VlObj* obj)
{
  if ( mNum >= mLimit ) {
    // テーブルを拡張する．
    ymuint old_size = mSize;
    Cell** old_table = mTable;
    alloc_table(old_size << 1);
    for (ymuint i = 0; i < old_size; ++ i) {
      for (Cell* cell = old_table[i]; cell; ) {
	Cell* next = cell->mLink;
	ymuint pos = hash_func(cell->mObj);
	cell->mLink = mTable[pos];
	mTable[pos] = cell;
	cell = next;
      }
    }
    delete [] old_table;
  }
  ymuint pos = hash_func(obj);
  void* p = mAlloc.get_memory(sizeof(Cell));
  Cell* cell = new (p) Cell;
  cell->mObj = obj;
  cell->mAttrList[0] = nullptr;
  cell->mAttrList[1] = nullptr;
  cell->mLink = mTable[pos];
  mTable[pos] = cell;
  ++ mNum;
  return cell;
}

// @brief オブジェクトから対応する Cell を取り出す．
// @param[in] obj 対象のオブジェクト
AttrHash::Cell*
AttrHash::find_cell(const VlObj* obj) const
{
  ymuint pos = hash_func(obj);
  for (Cell* cell = mTable[pos]; cell; cell = cell->mLink) {
    if ( cell->mObj == obj ) {
      return cell;
    }
  }
  return nullptr;
}

// @brief このオブジェクトが使用しているメモリ量を返す．
ymuint
AttrHash::allocated_size() const
{
  return sizeof(Cell*) * mSize;
}

// @brief テーブルの領域を確保する．
void
AttrHash::alloc_table(ymuint size)
{
  mSize = size;
  mLimit = static_cast<ymuint>(mSize * 1.8);
  mTable = new Cell*[mSize];
  for (ymuint i = 0; i < mSize; ++ i) {
    mTable[i] = nullptr;
  }
}

// @brief ハッシュ値を計算する．
ymuint
AttrHash::hash_func(const VlObj* obj) const
{
  ympuint tmp = reinterpret_cast<ympuint>(obj);
  return ((tmp * tmp) >> 12) % mSize;
}


END_NAMESPACE_YM_VERILOG
