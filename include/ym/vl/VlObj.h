#ifndef YM_VL_VLDOBJ_H
#define YM_VL_VLDOBJ_H

/// @file ym/vl/VlObj.h
/// @brief VlObj のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym/verilog.h"
#include "ym/FileRegion.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class VlObj VlObj.h "ym/vl/VlObj.h"
/// @brief エラボレーション中のオブジェクトを表す基底クラス
//////////////////////////////////////////////////////////////////////
class VlObj
{
public:

  /// @brief デストラクタ
  virtual
  ~VlObj() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の派生クラスに共通な仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  virtual
  VpiObjType
  type() const = 0;

  /// @brief ファイル位置の取得
  virtual
  FileRegion
  file_region() const = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // YM_VL_VLOBJ_H
