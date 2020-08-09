#ifndef ELBCAHEAD_H
#define ELBCAHEAD_H

/// @file ElbCaHead.h
/// @brief ElbCaHead のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.


#include "ym/verilog.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ElbCaHead ElbCaHead.h "ElbCaHead.h"
/// @brief 継続的代入文のヘッダ
///
/// とは言ってもただの place holder クラス
//////////////////////////////////////////////////////////////////////
class ElbCaHead
{
protected:

  /// @brief コンストラクタ
  ElbCaHead() = default;

  /// @brief デストラクタ
  virtual
  ~ElbCaHead() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbCaHead の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 親のモジュールを返す．
  virtual
  const VlModule*
  module() const = 0;

  /// @brief 0の強さを返す．
  virtual
  VpiStrength
  drive0() const = 0;

  /// @brief 1の強さを返す．
  virtual
  VpiStrength
  drive1() const = 0;

  /// @brief 遅延を表す式を返す．
  virtual
  const VlDelay*
  delay() const = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // ELBCAHEAD_H
