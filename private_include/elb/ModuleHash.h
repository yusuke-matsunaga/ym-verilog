#ifndef MODULEHASH_H
#define MODULEHASH_H

/// @file ModuleHash.h
/// @brief ModuleHash のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym/verilog.h"
#include "HierName.h"


BEGIN_NAMESPACE_YM_VERILOG

class VlModule;
class VlNamedObj;

//////////////////////////////////////////////////////////////////////
/// @class ModuleHash ModuleHash.h "ModuleHash.h"
/// @brief モジュール定義名をキーにして VlModule を格納するハッシュ表
//////////////////////////////////////////////////////////////////////
class ModuleHash
{
public:

  /// @brief コンストラクタ
  ModuleHash() = default;

  /// @brief デストラクタ
  ~ModuleHash() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // ModuleHash の関数
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
  find(const VlNamedObj* parent,
       const char* name) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // HierName をキーにして const VlModule* を納めるハッシュ表
  unordered_map<HierName, const VlModule*, HierNameHash> mHash;

};


END_NAMESPACE_YM_VERILOG

#endif // MODULEHASH_H
