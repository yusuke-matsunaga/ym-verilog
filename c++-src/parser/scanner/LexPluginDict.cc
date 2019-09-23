﻿
/// @file LexPluginDict.cc
/// @brief LexPlugin の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "LexPluginDict.h"
#include "LexPlugin.h"


BEGIN_NAMESPACE_YM_VERILOG

BEGIN_NONAMESPACE

// ハッシュ関数
inline
ymuint
hash_func(const char* str)
{
  ymuint h = 0;
  ymuint c;
  for ( ; (c = static_cast<ymuint>(*str)); ++ str) {
    h = h * 37 + c;
  }
  return h;
}

END_NONAMESPACE


// @brief コンストラクタ
LexPluginDict::LexPluginDict() :
  mTableSize(0),
  mHashTable(nullptr),
  mNum(0)
{
  new_table(32);
}

// @brief デストラクタ
LexPluginDict::~LexPluginDict()
{
  for (ymuint i = 0; i < mTableSize; ++ i) {
    for (LexPlugin* p = mHashTable[i]; p; ) {
      LexPlugin* next = p->mLink;
      delete p;
      p = next;
    }
  }
  delete [] mHashTable;
}

// @brief プラグインを登録できるか調べる．
// @param[in] name プラグイン名
// @return true 登録可能
// @return false 同名の定義済みプラグインがあり，登録不可能
bool
LexPluginDict::check_name(const char* name)
{
  LexPlugin* p = find_plugin(name);
  if ( p == nullptr || p->is_macro() ) {
    return true;
  }
  return false;
}

// @brief プラグインを登録する．
// @param[in] plugin プラグイン
void
LexPluginDict::reg_plugin(LexPlugin* plugin)
{
  const char* name = plugin->name();
  ymuint hash_value = hash_func(name);
  ymuint pos = hash_value % mTableSize;
  LexPlugin* p;
  for (LexPlugin** prev = &mHashTable[pos]; (p = *prev); ) {
    if ( strcmp(p->name(), name) == 0 ) {
      if ( p->is_macro() ) {
	// 古いプラグイン(マクロ)を削除する．
	*prev = plugin;
	plugin->mLink = p->mLink;
	delete p;
	return;
      }
      else {
	// コンパイラディレクティブは上書きできない．
	ASSERT_NOT_REACHED;
      }
    }
    prev = &p->mLink;
  }

  if ( mNum >= mNextLimit ) {
    LexPlugin** old_table = mHashTable;
    ymuint old_size = mTableSize;
    new_table(mTableSize << 1);
    for (ymuint i = 0; i < old_size; ++ i) {
      for (LexPlugin* p = old_table[i]; p; ) {
	LexPlugin* next = p->mLink;
	ymuint pos1 = hash_func(p->name()) % mTableSize;
	p->mLink = mHashTable[pos1];
	mHashTable[pos1] = p;
	p = next;
      }
    }
    delete [] old_table;
    pos = hash_value % mTableSize;
  }

  plugin->mLink = mHashTable[pos];
  mHashTable[pos] = plugin;
  ++ mNum;
  return;
}

// @brief プラグインの登録を解除する．
// @param[in] name 名前
// @retval true 指定された名前のプラグインの登録解除が成功した．
// @retval false 指定された名前のプラグインは登録されていなかった．
bool
LexPluginDict::unreg_plugin(const char* name)
{
  ymuint hash_value = hash_func(name);
  ymuint pos = hash_value % mTableSize;
  LexPlugin** prev = &mHashTable[pos];
  for (LexPlugin* p = *prev; p; ) {
    if ( strcmp(p->name(), name) == 0 ) {
      *prev = p->mLink;
      delete p;
      -- mNum;
      return true;
    }
    prev = &p->mLink;
  }
  return false;
}

// @brief プラグインを検索する．
// @param[in] name 名前
// @return name という名前で登録されているプラグインを返す．
LexPlugin*
LexPluginDict::find_plugin(const char* name) const
{
  ymuint hash_value = hash_func(name);
  ymuint pos = hash_value & mHashMask;
  for (LexPlugin* p = mHashTable[pos]; p; p = p->mLink) {
    if ( strcmp(p->name(), name) == 0 ) {
      return p;
    }
  }
  return nullptr;
}

// @brief ハッシュ表を確保する．
void
LexPluginDict::new_table(ymuint size)
{
  mTableSize = size;
  mHashMask = mTableSize - 1;
  mNextLimit = static_cast<ymuint>(mTableSize * 1.8);
  mHashTable = new LexPlugin*[mTableSize];
  for (ymuint i = 0; i < mTableSize; ++ i) {
    mHashTable[i] = nullptr;
  }
}

END_NAMESPACE_YM_VERILOG
