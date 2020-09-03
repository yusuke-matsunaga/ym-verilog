#ifndef ELBTASKFUNC_H
#define ELBTASKFUNC_H

/// @file ElbTaskFunc.h
/// @brief ElbTaskFunc のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
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
  /// @param[in] head ヘッダ
  /// @param[in] pt_item パース木のIO宣言要素
  /// @param[in] decl 対応する宣言要素
  virtual
  void
  add_iodecl(ElbIOHead* head,
	     const PtIOItem* pt_item,
	     const VlDecl* decl) = 0;

  /// @brief 本体のステートメントをセットする．
  virtual
  void
  set_stmt(const VlStmt* stmt) = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbTaskFunc の仮想関数
  // ただし Function の時しか意味を持たない．
  //////////////////////////////////////////////////////////////////////

  /// @brief 出力変数をセットする．
  /// @param[in] ovar 出力変数
  virtual
  void
  set_ovar(ElbDecl* ovar) = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // ELBTASKFUNC_H
