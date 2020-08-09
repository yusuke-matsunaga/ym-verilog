﻿#ifndef EIGENVAR_H
#define EIGENVAR_H

/// @file EiGenvar.h
/// @brief EiGenvar のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "elaborator/ElbGenvar.h"
#include "ym/pt/PtP.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiGenvar EiGenvar.h "EiGenvar.h"
/// @brief genvar を表すクラス
//////////////////////////////////////////////////////////////////////
class EiGenvar :
  public ElbGenvar
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_item パース木の定義要素
  /// @param[in] val 初期値
  EiGenvar(const VlNamedObj* parent,
	   const PtDeclItem* pt_item,
	   int val);

  /// @brief デストラクタ
  ~EiGenvar();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  VpiObjType
  type() const override;

  /// @brief ファイル位置を返す．
  FileRegion
  file_region() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // VlNamedObj の派生クラスに共通な仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief このオブジェクトの属しているスコープを返す．
  const VlNamedObj*
  parent() const override;

  /// @brief 名前の取得
  const char*
  name() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbGenvar に固有の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 使用中の時 true を返す．
  bool
  is_inuse() const override;

  /// @brief 使用中にする．
  void
  set_inuse() override;

  /// @brief 使用を終える．
  void
  reset_inuse() override;

  /// @brief 現在の値を返す．
  int
  value() const override;

  /// @brief 値を設定する．
  void
  set_value(int value) override;

  /// @brief 元となったパース木の定義要素を返す．
  const PtDeclItem*
  pt_item() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親のスコープ
  const VlNamedObj* mParent;

  // パース木の定義要素
  const PtDeclItem* mPtItem;

  // 使用中の時に true にする flag
  bool mInUse;

  // 値
  int mValue;

};

END_NAMESPACE_YM_VERILOG

#endif // EIGENVAR_H
