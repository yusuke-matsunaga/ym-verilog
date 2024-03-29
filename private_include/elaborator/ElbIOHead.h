﻿#ifndef ELBIODECL_H
#define ELBIODECL_H

/// @file ElbIODecl.h
/// @brief ElbIODecl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "elaborator/ElbHead.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ElbIOHead  ElbIODecl.h "ElbIODecl.h"
/// @brief IO のヘッダの基底クラス
//////////////////////////////////////////////////////////////////////
class ElbIOHead :
  public ElbHead
{
public:
  //////////////////////////////////////////////////////////////////////
  // ElbIOHead の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 方向を返す．
  virtual
  VpiDir
  direction() const = 0;

  /// @brief 親のモジュールの取得
  virtual
  const VlModule*
  module() const = 0;

  /// @brief 親のタスクの取得
  virtual
  const VlTaskFunc*
  task() const = 0;

  /// @brief 親の関数の取得
  virtual
  const VlTaskFunc*
  function() const = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // ELBIODECL_H
