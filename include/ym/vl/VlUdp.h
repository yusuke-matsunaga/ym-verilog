﻿#ifndef YM_VL_VLUDP_H
#define YM_VL_VLUDP_H

/// @file ym/vl/VlUdp.h
/// @brief VlUdp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym/vl/VlObj.h"
#include "ym/vl/VlFwd.h"
#include "ym/VlUdpVal.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class VlUdpDefn VlUdp.h "ym/vl/VlUdp.h"
/// @brief UDP定義を表すクラス
/// IEEE Std 1364-2001 26.6.14 UDP
//////////////////////////////////////////////////////////////////////
class VlUdpDefn :
  public VlObj
{
public:

  /// @brief デストラクタ
  virtual
  ~VlUdpDefn() {}


public:
  //////////////////////////////////////////////////////////////////////
  // VlUdpDefn に固有の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 定義された名前を返す．
  virtual
  const char*
  def_name() const = 0;

  /// @brief primitive type を返す．
  virtual
  tVpiPrimType
  prim_type() const = 0;

  /// @brief ポート数を返す．
  virtual
  ymuint
  port_num() const = 0;

  /// @brief 入力の宣言要素を返す．
  /// @param[in] pos 入力番号 ( 0 <= pos < port_num() - 1 )
  virtual
  const VlIODecl*
  input(ymuint pos) const = 0;

  /// @brief 出力の宣言要素を返す．
  virtual
  const VlIODecl*
  output() const = 0;

  /// @brief protected かどうかを返す．
  virtual
  bool
  is_protected() const = 0;

  /// @brief 初期値を返す．
  /// @return 0/1/X を返す．
  virtual
  VlScalarVal
  init_val() const = 0;

  /// @brief 初期値を表す文字列を返す．
  virtual
  string
  init_val_string() const = 0;

  /// @brief table entry の行数を返す．
  virtual
  ymuint
  table_size() const = 0;

  /// @brief table entry を返す．
  /// @param[in] pos 行番号
  virtual
  const VlTableEntry*
  table_entry(ymuint pos) const = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class VlTableEntry VlUdp.h "ym/vl/VlUdp.h"
/// @brief UDP の table entry を表すクラス
//////////////////////////////////////////////////////////////////////
class VlTableEntry :
  public VlObj
{
public:

  /// @brief デストラクタ
  ~VlTableEntry() { }


public:
  //////////////////////////////////////////////////////////////////////
  // VlTableEntry に固有の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 一行の要素数を返す．
  virtual
  ymuint
  size() const = 0;

  /// @brief pos 番目の位置の値を返す．
  virtual
  VlUdpVal
  val(ymuint pos) const = 0;

  /// @brief 一行文の内容を表す文字列をつくる．
  virtual
  string
  str() const = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // YM_VL_VLUDP_H
