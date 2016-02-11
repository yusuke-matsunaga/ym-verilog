﻿#ifndef CFDICT_H
#define CFDICT_H

/// @file CfDict.h
/// @brief CfDict のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym/ym_verilog.h"
#include "ym/UnitAlloc.h"

#include "ElbFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class CfDict CfDict.h "CfDict.h"
/// @brief constant function を格納するハッシュ表
//////////////////////////////////////////////////////////////////////
class CfDict
{
public:

  /// @brief コンストラクタ
  CfDict();

  /// @brief デストラクタ
  ~CfDict();


public:
  //////////////////////////////////////////////////////////////////////
  // CfDict の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 関数を追加する．
  /// @param[in] scope 親のスコープ
  /// @param[in] name 名前
  /// @param[in] func 登録する関数
  void
  add(const VlNamedObj* scope,
      const char* name,
      ElbTaskFunc* func);

  /// @brief 名前から該当する要素を検索する．
  /// @param[in] scope 親のスコープ
  /// @param[in] name 名前
  ElbTaskFunc*
  find(const VlNamedObj* scope,
       const char* name) const;

  /// @brief 内容をクリアする．
  void
  clear();

  /// @brief このオブジェクトが使用しているメモリ量を返す．
  ymuint
  allocated_size() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief テーブルの領域を確保する．
  void
  alloc_table(ymuint size);

  /// @brief ハッシュ値を計算する．
  ymuint
  hash_func(const VlNamedObj* scope,
	    const char* name) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で使用するデータ構造
  //////////////////////////////////////////////////////////////////////

  struct Cell
  {
    // 親のスコープ
    const VlNamedObj* mScope;

    // 名前
    const char* mName;

    // 対象の関数
    ElbTaskFunc* mFunc;

    // 次の要素を指すリンク
    Cell* mLink;

  };


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // Cell 用のアロケータ
  UnitAlloc mAlloc;

  // ハッシュ表のサイズ
  ymuint32 mSize;

  // ハッシュ表
  Cell** mTable;

  // ハッシュ表を拡大するしきい値
  ymuint32 mLimit;

  // 要素数
  ymuint32 mNum;

};


END_NAMESPACE_YM_VERILOG

#endif // CFDICT_H
