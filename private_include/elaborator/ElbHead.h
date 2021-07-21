#ifndef ELBHEAD_H
#define ELBHEAD_H

/// @file ElbHead.h
/// @brief ElbHead のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2020, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym/verilog.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ElbHead ElbHead.h "ElbHead.h"
/// @brief ヘッダの共通な親クラス
///
/// 仮想デストラクタを提供するためだけに存在している．
//////////////////////////////////////////////////////////////////////
class ElbHead
{
public:

  /// @brief コンストラクタ
  ElbHead() = default;

  /// @brief デストラクタ
  virtual
  ~ElbHead() = default;

};

END_NAMESPACE_YM_VERILOG

#endif // ELBHEAD_H
