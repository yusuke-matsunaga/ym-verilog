#ifndef MODDEFDICT_H
#define MODDEFDICT_H

/// @file ModDefDict.h
/// @brief ModDefDict のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2020, 2021 Yusuke Matsunaga
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
  clear()
  {
    mHash.clear();
  }

  /// @brief 要素を追加する．
  ///
  /// 同名のモジュールが登録されていたらそのエントリを無効化する．
  void
  add(
    const VlModule* obj ///< [in] モジュール
  )
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

  /// @brief 名前から該当する要素を検索する．
  const VlModule*
  find(
    const VlScope* parent, ///< [in] 親のスコープ
    const char* name       ///< [in] 名前
  ) const
  {
    Key key{parent, name};
    if ( mHash.count(key) > 0 ) {
      return mHash.at(key);
    }
    else {
      return nullptr;
    }
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  /// @brief 親のスコープと名前を持つ構造体
  struct Key
  {
    const VlScope* mParent;
    string mName;
  };

  struct KeyHash
  {
    SizeType
    operator()(
      const Key& key
    ) const
    {
      SizeType h = 0;
      for ( auto c: key.mName ) {
	h = h * 37 + static_cast<SizeType>(c);
      }
      return ((reinterpret_cast<PtrIntType>(key.mParent) * h) >> 8);
    }
  };

  struct KeyEq
  {
    bool
    operator()(
      const Key& key1,
      const Key& key2
    ) const
    {
      return key1.mParent == key2.mParent && key1.mName == key2.mName;
    }
  };

  // const VlModule* を納めるハッシュ表
  unordered_map<Key, const VlModule*, KeyHash, KeyEq> mHash;

};

END_NAMESPACE_YM_VERILOG

#endif // MODULEHASH_H
