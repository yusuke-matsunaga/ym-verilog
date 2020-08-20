#ifndef DEFPARAMSTUB_H
#define DEFPARAMSTUB_H

/// @file DefParamStub.h
/// @brief DefParamStub のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2020 Yusuke Matsunaga
/// All rights reserved.

#include "ym/verilog.h"


BEGIN_NAMESPACE_YM_VERILOG

/// @brief defparam 文の情報を表すクラス
struct DefParamStub
{

  /// @brief 対象のモジュール
  const VlModule* mModule;

  /// @brief パース木の DefParam ヘッダ
  const PtItem* mPtHeader;

  /// @brief パース木の DefParam 文
  const PtDefParam* mPtDefparam;
};

END_NAMESPACE_YM_VERILOG

#endif // DEFPARAMSTUB_H
