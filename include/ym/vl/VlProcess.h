#ifndef YM_VL_VLPROCESS_H
#define YM_VL_VLPROCESS_H

/// @file ym/vl/VlProcess.h
/// @brief VlProcess のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym/vl/VlObj.h"
#include "ym/vl/VlFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class VlProcess VlProcess.h "ym/vl/VlProcess.h"
/// @brief initial/always 文を表すクラス
/// IEEE Std 1364-2001 26.6.27 Process, block, statement, event statement
//////////////////////////////////////////////////////////////////////
class VlProcess :
  public VlObj
{
public:
  //////////////////////////////////////////////////////////////////////
  // VlProcess の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 親のスコープを返す．
  virtual
  const VlScope*
  parent_scope() const = 0;

  /// @brief 本体のステートメントの取得
  virtual
  const VlStmt*
  stmt() const = 0;

};


END_NAMESPACE_YM_VERILOG

#endif // YM_VL_VLPROCESS_H
