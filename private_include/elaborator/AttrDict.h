#ifndef ATTRDICT_H
#define ATTRDICT_H

/// @file AttrDict.h
/// @brief AttrDict のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym/verilog.h"

#include "ElbFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

class PtAttrInst;

//////////////////////////////////////////////////////////////////////
/// @class AttrDict AttrDict.h "AttrDict.h"
/// @brief PtAttrInst をキーとして ElbAttrList を格納するハッシュ表
//////////////////////////////////////////////////////////////////////
class AttrDict
{
public:

  /// @brief コンストラクタ
  AttrDict() = default;

  /// @brief デストラクタ
  ~AttrDict() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // AttrDict の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 登録する．
  /// @param[in] pt_attr パース木の属性定義
  /// @param[in] attr_list 登録する属性のリスト
  void
  add(const PtAttrInst* pt_attr,
      ElbAttrList* attr_list);

  /// @brief 属性リストを取り出す．
  /// @param[in] pt_attr パース木の属性定義
  ElbAttrList*
  find(const PtAttrInst* pt_attr) const;

  /// @brief 内容をクリアする．
  void
  clear();


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ハッシュ表
  unordered_map<const PtAttrInst*, ElbAttrList*> mHash;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 要素を追加する．
// @param[in] pt_attr パース木の属性定義
// @param[in] attr_list 登録する属性のリスト
inline
void
AttrDict::add(const PtAttrInst* pt_attr,
	      ElbAttrList* attr_list)
{
  mHash.emplace(pt_attr, attr_list);
}

// @brief 属性リストを取り出す．
// @param[in] pt_attr パース木の属性定義
inline
ElbAttrList*
AttrDict::find(const PtAttrInst* pt_attr) const
{
  if ( mHash.count(pt_attr) > 0 ) {
    return mHash.at(pt_attr);
  }
  return nullptr;
}

// @brief 内容をクリアする．
inline
void
AttrDict::clear()
{
  mHash.clear();
}


END_NAMESPACE_YM_VERILOG

#endif // ATTRDICT_H
