#ifndef HIERNAME_H
#define HIERNAME_H

/// @file HierName.h
/// @brief HierName のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2019 Yusuke Matsunaga
/// All rights reserved.

#include "ym/verilog.h"


BEGIN_NAMESPACE_YM_VERILOG

class VlNamedObj;

//////////////////////////////////////////////////////////////////////
/// @class HierName HierName.h "HierName.h"
/// @brief ハッシュ表のキーとなる階層名を表すクラス
//////////////////////////////////////////////////////////////////////
class HierName
{
public:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] name 名前
  HierName(const VlNamedObj* parent,
	   const char* name);

  /// @brief デストラクタ
  ~HierName() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 親のスコープを返す．
  const VlNamedObj*
  parent() const;

  /// @brief 名前を返す．
  const char*
  name() const;

  /// @brief 等価比較演算子
  bool
  operator==(const HierName& right) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親のスコープ
  const VlNamedObj* mParent;

  // 名前
  const char* mName;

};


//////////////////////////////////////////////////////////////////////
/// @class HierNameHash HierNameHash.h "HierNameHash.h"
/// @brief HierName 用のハッシュ関数クラス
//////////////////////////////////////////////////////////////////////
class HierNameHash
{
public:

  /// @brief コンストラクタ
  HierNameHash() = default;

  /// @brief デストラクタ
  ~HierNameHash() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief HierName のハッシュ関数
  SizeType
  operator()(const HierName& hname) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////


};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のスコープ
// @param[in] name 名前
inline
HierName::HierName(const VlNamedObj* parent,
		   const char* name) :
  mParent{parent},
  mName{name}
{
}

// @brief 親のスコープを返す．
inline
const VlNamedObj*
HierName::parent() const
{
  return mParent;
}

// @brief 名前を返す．
inline
const char*
HierName::name() const
{
  return mName;
}

// @brief 等価比較演算子
inline
bool
HierName::operator==(const HierName& right) const
{
  return mParent == right.mParent && strcmp(mName, right.mName) == 0;
}

// @brief HierName のハッシュ関数
inline
SizeType
HierNameHash::operator()(const HierName& hname) const
{
  SizeType h = 0;
  SizeType c;
  for ( auto name = hname.name(); (c = static_cast<SizeType>(*name)); ++ name ) {
    h = h * 37 + c;
  }
  return ((reinterpret_cast<ympuint>(hname.parent()) * h) >> 8);
}

END_NAMESPACE_YM_VERILOG

#endif // HIERNAME_H
