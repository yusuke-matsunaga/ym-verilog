#ifndef ELBTASKFUNC_H
#define ELBTASKFUNC_H

/// @file ElbTaskFunc.h
/// @brief ElbTaskFunc のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/VlTaskFunc.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ElbTaskFunc ElbTaskFunc.h "ElbTaskFunc.h"
/// @brief elaboration 中の task/function を表すクラス
/// IEEE Std 1364-2001 26.6.18 Task, function declaration
//////////////////////////////////////////////////////////////////////
class ElbTaskFunc :
  public VlTaskFunc
{
public:
  //////////////////////////////////////////////////////////////////////
  // ElbTaskFunc の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入出力を追加する．
  virtual
  void
  add_iodecl(
    ElbIOHead* head,         ///< [in] ヘッダ
    const PtIOItem* pt_item, ///< [in] パース木のIO宣言要素
    const VlDecl* decl       ///< [in] 対応する宣言要素
  ) = 0;

  /// @brief 本体のステートメントをセットする．
  virtual
  void
  set_stmt(
    const VlStmt* stmt ///< [in] 本体のステートメント
  ) = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbTaskFunc の仮想関数
  // ただし Function の時しか意味を持たない．
  //////////////////////////////////////////////////////////////////////

  /// @brief 出力変数をセットする．
  virtual
  void
  set_ovar(
    ElbDecl* ovar ///< [in] 出力変数
  ) = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // ELBTASKFUNC_H
