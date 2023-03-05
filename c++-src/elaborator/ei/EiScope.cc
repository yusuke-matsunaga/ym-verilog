
/// @file EiScope.cc
/// @brief EiScope の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2020 Yusuke Matsunaga
/// All rights reserved.

#include "ei/EiScope.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス EiScope
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiScope::EiScope(
  const VlScope* parent
) : mParent{parent}
{
}

// @brief デストラクタ
EiScope::~EiScope()
{
}

// @brief 型の取得
VpiObjType
EiScope::type() const
{
  return VpiObjType::Scope;
}

// @brief このオブジェクトの属している scope を返す．
const VlScope*
EiScope::parent_scope() const
{
  return mParent;
}

END_NAMESPACE_YM_VERILOG
