#ifndef ELBPROCESS_H
#define ELBPROCESS_H

/// @file ElbProcess.h
/// @brief ElbProcess のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym/vl/VlProcess.h"
#include "ElbFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ElbProcess ElbProcess.h "ElbProcess.h"
/// @brief initial/always 文を表すクラス
/// IEEE Std 1364-2001 26.6.27 Process, block, statement, event statement
//////////////////////////////////////////////////////////////////////
class ElbProcess :
  public VlProcess
{
protected:

  /// @brief コンストラクタ
  ElbProcess() = default;

  /// @brief デストラクタ
  ~ElbProcess() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbProcess の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 本体のステートメントをセットする．
  /// @param[in] stmt 本体のステートメント
  virtual
  void
  set_stmt(const VlStmt* stmt) = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // ELBPROCESS_H
