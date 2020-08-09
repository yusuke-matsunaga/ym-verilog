#ifndef ELBPARAMCON_H
#define ELBPARAMCON_H

/// @file ElbParamCon.h
/// @brief ElbParamCon のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym/verilog.h"
#include "ym/pt/PtMisc.h"
#include "ym/VlValue.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ElbParamCon ElbParamCon.h "ElbParamCon.h"
/// @brief インスタンス化で用いる stub クラス
//////////////////////////////////////////////////////////////////////
struct ElbParamCon
{
  // パース木の要素
  const PtConnection* mPtCon;

  // 式
  const PtExpr* mExpr;

  // 値
  VlValue mValue;

};

END_NAMESPACE_YM_VERILOG

#endif // ELBPARAMCON_H
