#ifndef PTIATTRINFO_H
#define PTIATTRINFO_H

/// @file PtiAttrInfo.h
/// @brief PtiAttrInfo のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2020, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym/verilog.h"
#include "ym/pt/PtP.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class PtiAttrInfo PtiAttrInfo.h "PtiAttrInfo.h"
/// @brief PtAttrInst に関する情報を表すクラス
//////////////////////////////////////////////////////////////////////
class PtiAttrInfo
{
public:

  /// @brief コンストラクタ
  PtiAttrInfo(
    const PtBase* pt_obj,                          ///< [in] 元の構文木要素
    const vector<const PtAttrInst*>& pt_attr_list, ///< [in] attribute instance の構文木要素のリスト
    bool def = false                               ///< [in] 定義側の時 true とするフラグ
  ) : mObj{pt_obj},
      mAttrList{pt_attr_list},
      mDef{def}
  {
  }

  /// @brief デストラクタ
  ~PtiAttrInfo() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// 元の構文木要素を返す．
  const PtBase*
  obj() const
  {
    return mObj;
  }

  /// @brief 属性リストを返す．
  const vector<const PtAttrInst*>&
  attr_list() const
  {
    return mAttrList;
  }

  /// @brief 定義側の属性の時 true を返す．
  bool
  def() const
  {
    return mDef;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 元の構文木要素
  const PtBase* mObj;

  // 属性リスト
  vector<const PtAttrInst*> mAttrList;

  // 定義側のフラグ
  bool mDef;

};

END_NAMESPACE_YM_VERILOG

#endif // PTIATTRINFO_H
