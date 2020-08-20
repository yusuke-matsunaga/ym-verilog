#ifndef ELBMODULEARRAY_H
#define ELBMODULEARRAY_H

/// @file ElbModuleArray.h
/// @brief ElbModuleArray のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.


#include "ym/vl/VlModule.h"
#include "ym/pt/PtP.h"
#include "ElbFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ElbModuleArray ElbModuleArray.h "ElbModuleArray.h"
/// @brief elaboration 中の module 配列を表すクラス
//////////////////////////////////////////////////////////////////////
class ElbModuleArray :
  public VlModuleArray
{
public:
  //////////////////////////////////////////////////////////////////////
  // ElbModuleArray の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素を取り出す．
  /// @param[in] index モジュール番号
  virtual
  ElbModule*
  elem(SizeType index) = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // ELBMODULEARRAY_H
