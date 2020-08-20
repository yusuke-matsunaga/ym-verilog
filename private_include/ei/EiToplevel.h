#ifndef EITOPLEVEL_H
#define EITOPLEVEL_H

/// @file EiToplevel.h
/// @brief ElbFrameToplevel のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.


#include "ei/EiScope.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiToplevel EiToplevel.h "EiToplevel.h"
/// @brief elaboration 中の toplevel scope を表すクラス
//////////////////////////////////////////////////////////////////////
class EiToplevel :
  public EiScope
{
public:

  /// @brief コンストラクタ
  EiToplevel();

  /// @brief デストラクタ
  ~EiToplevel();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置の取得
  FileRegion
  file_region() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // VlNamedObj の派生クラスに共通な仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief このオブジェクトの属している module を返す．
  ///
  /// このクラスでは nullptr を返す．
  const VlModule*
  parent_module() const override;

  /// @brief 名前の取得
  string
  name() const override;

  /// @brief fullname の取得
  string
  full_name() const override;

};

END_NAMESPACE_YM_VERILOG

#endif // EITOPLEVEL_H
