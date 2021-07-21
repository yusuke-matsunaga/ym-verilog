#ifndef ATTRHASH_H
#define ATTRHASH_H

/// @file AttrHash.h
/// @brief AttrHash のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2020, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym/verilog.h"
#include "ElbFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class AttrHash AttrHash.h "AttrHash.h"
/// @brief VlObj に付帯する VlAttribute を管理するクラス
//////////////////////////////////////////////////////////////////////
class AttrHash
{
public:

  /// @brief コンストラクタ
  AttrHash() = default;

  /// @brief デストラクタ
  ~AttrHash() = default;


public:

  /// @brief 内容をクリアする．
  void
  clear();

  /// @brief 属性を追加する．
  void
  add(
    const VlObj* obj,                           ///< [in] 対象のオブジェクト
    bool def,                                   ///< [in] 定義側の属性の時 true とするフラグ
    const vector<const VlAttribute*>& attr_list ///< [in] 付加する属性リスト
  );

  /// @brief 属性を取り出す．
  vector<const VlAttribute*>
  find(
    const VlObj* obj, ///< [in] 対象のオブジェクト
    bool def          ///< [in] 定義側の属性の時 true とするフラグ
  ) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で使われるデータ構造
  //////////////////////////////////////////////////////////////////////

  struct Cell
  {
    // 対応する属性リスト
    vector<const VlAttribute*> mAttrList[2];
  };


private:

  /// @brief 新しい Cell を生成する．
  Cell*
  new_cell(
    const VlObj* obj ///< [in] 対象のオブジェクト
  );

  /// @brief オブジェクトから対応する Cell を取り出す．
  Cell*
  find_cell(
    const VlObj* obj ///< [in] 対象のオブジェクト
  ) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // const VlObj* をキーにして Cell を納めるハッシュ表
  unordered_map<const VlObj*, Cell> mHash;

};

END_NAMESPACE_YM_VERILOG

#endif // ATTRHASH_H
