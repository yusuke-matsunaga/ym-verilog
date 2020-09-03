#ifndef PTIATTRINFO_H
#define PTIATTRINFO_H

/// @file PtiAttrInfo.h
/// @brief PtiAttrInfo のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2020 Yusuke Matsunaga
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
  /// @param[in] pt_obj 元の構文木要素
  /// @param[in] pt_attr_list attribute instance の構文木要素のリスト
  /// @param[in] def 定義側の時 true とするフラグ
  PtiAttrInfo(const PtBase* pt_obj,
	      const vector<const PtAttrInst*>& pt_att_list,
	      bool def = false);

  /// @brief デストラクタ
  ~PtiAttrInfo() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// 元の構文木要素を返す．
  const PtBase*
  obj() const;

  /// @brief 属性リストを返す．
  const vector<const PtAttrInst*>&
  attr_list() const;

  /// @brief 定義側の属性の時 true を返す．
  bool
  def() const;


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


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_obj 元の構文木要素
// @param[in] pt_attr_list attribute instance の構文木要素のリスト
// @param[in] def 定義側の時 true とするフラグ
inline
PtiAttrInfo::PtiAttrInfo(const PtBase* pt_obj,
			 const vector<const PtAttrInst*>& pt_attr_list,
			 bool def) :
  mObj{pt_obj},
  mAttrList{pt_attr_list},
  mDef{def}
{
}

// 元の構文木要素を返す．
inline
const PtBase*
PtiAttrInfo::obj() const
{
  return mObj;
}

// @brief 属性リストを返す．
inline
const vector<const PtAttrInst*>&
PtiAttrInfo::attr_list() const
{
  return mAttrList;
}

// @brief 定義側の属性の時 true を返す．
inline
bool
PtiAttrInfo::def() const
{
  return mDef;
}

END_NAMESPACE_YM_VERILOG

#endif // PTIATTRINFO_H
