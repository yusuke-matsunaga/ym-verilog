#ifndef MODDEFDICT_H
#define MODDEFDICT_H

/// @file ModDefDict.h
/// @brief ModDefDict のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.


#include "ym/verilog.h"
#include "ym/vl/VlModule.h"
#include "ym/vl/VlScope.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ModDefDict ModDefDict.h "ModDefDict.h"
/// @brief モジュール定義名をキーにして VlModule を格納するハッシュ表
///
/// ObjDict と似たような辞書だがオブジェクト名ではなくモジュールの定義名
/// を用いるところが異なる．さらに同じモジュール定義名を持つモジュール
/// が複数ある場合にはそのエントリを削除する．
//////////////////////////////////////////////////////////////////////
class ModDefDict
{
public:

  /// @brief コンストラクタ
  ModDefDict() = default;

  /// @brief デストラクタ
  ~ModDefDict() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // ModDefDict の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を空にする．
  void
  clear();

  /// @brief 要素を追加する．
  /// @note 同名のモジュールが登録されていたらそのエントリを無効化する．
  void
  add(const VlModule* obj);

  /// @brief 名前から該当する要素を検索する．
  /// @param[in] parent 親のスコープ
  /// @param[in] name 名前
  const VlModule*
  find(const VlScope* parent,
       const char* name) const;

  /// @brief 親のスコープと名前を持つ構造体
  struct Key
  {
    const VlScope* mParent;
    string mName;
  };

  struct KeyHash
  {
    SizeType
    operator()(const Key& key) const;
  };

  struct KeyEq
  {
    bool
    operator()(const Key& key1,
	       const Key& key2) const;
  };


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // const VlModule* を納めるハッシュ表
  unordered_map<Key, const VlModule*, KeyHash, KeyEq> mHash;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 内容を空にする．
inline
void
ModDefDict::clear()
{
  mHash.clear();
}

// @brief 要素を追加する．
// @note 同名のモジュールが登録されていたらそのエントリを無効化する．
inline
void
ModDefDict::add(const VlModule* obj)
{
  Key key{obj->parent_scope(), obj->def_name()};
  if ( mHash.count(key) > 0 ) {
    // 同じモジュールがすでに登録されていた．
    // そのエントリを無効化する．
    mHash.emplace(key, nullptr);
  }
  else {
    // 登録する．
    mHash.emplace(key, obj);
  }
}

// @brief 名前から該当する要素を検索する．
// @param[in] parent 親のスコープ
// @param[in] name 名前
inline
const VlModule*
ModDefDict::find(const VlScope* parent,
		 const char* name) const
{
  Key key{parent, name};
  if ( mHash.count(key) > 0 ) {
    return mHash.at(key);
  }
  else {
    return nullptr;
  }
}

inline
SizeType
ModDefDict::KeyHash::operator()(const Key& key) const
{
  SizeType h = 0;
  for ( auto c: key.mName ) {
    h = h * 37 + static_cast<SizeType>(c);
  }
  return ((reinterpret_cast<ympuint>(key.mParent) * h) >> 8);
}

inline
bool
ModDefDict::KeyEq::operator()(const Key& key1,
			      const Key& key2) const
{
  return key1.mParent == key2.mParent && key1.mName == key2.mName;
}

END_NAMESPACE_YM_VERILOG

#endif // MODULEHASH_H
