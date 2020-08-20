#ifndef YM_VL_VLSCOPE_H
#define YM_VL_VLSCOPE_H

/// @file VlScope.h
/// @brief VlScope のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2020 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/VlNamedObj.h"
#include "ym/vl/VlFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class VlScope VlScope.h "ym/VlScope.h"
/// @brief スコープを表すクラス
///
/// 実は VlNamedObj と同一のクラス
//////////////////////////////////////////////////////////////////////
class VlScope :
  public VlNamedObj
{
};

END_NAMESPACE_YM_VERILOG

#endif // YM_VL_VLSCOPE_H
