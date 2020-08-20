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
#if 0
  /// @brief 入出力用のヘッダを生成する．
  // @param[in] pt_header パース木のIO宣言ヘッダ
  virtual
  ElbIOHead*
  new_IOHead(const PtIOHead* pt_header) = 0;
#endif

  /// @brief 入出力を追加する．
  /// @param[in] head ヘッダ
  /// @param[in] pt_item パース木のIO宣言要素
  /// @param[in] decl 対応する宣言要素
  virtual
  void
  add_iodecl(ElbIOHead* head,
	     const PtIOItem* pt_item,
	     const VlDecl* decl) = 0;

#if 0
  /// @brief 宣言要素用のヘッダを生成する．
  /// @param[in] pt_header パース木の宣言ヘッダ
  virtual
  ElbDeclHead*
  new_DeclHead(const PtDeclHead* pt_header) = 0;

  /// @brief 宣言要素のヘッダを生成する．(IODecl 中の宣言用)
  /// @param[in] pt_head パース木のIO宣言ヘッダ
  /// @param[in] aux_type 補助的なデータ型
  virtual
  ElbDeclHead*
  new_DeclHead(const PtIOHead* pt_head,
	       VpiAuxType aux_type) = 0;

  /// @brief 宣言要素のヘッダを生成する．(IODecl 中の宣言用)
  /// @param[in] pt_head パース木のIO宣言ヘッダ
  /// @param[in] aux_type 補助的なデータ型
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @param[in] left_val 範囲の MSB の値
  /// @param[in] right_val 範囲の LSB の値
  virtual
  ElbDeclHead*
  new_DeclHead(const PtIOHead* pt_head,
	       VpiAuxType aux_type,
	       const PtExpr* left,
	       const PtExpr* right,
	       int left_val,
	       int right_val) = 0;

  /// @brief 宣言要素のヘッダを生成する．(function の暗黙宣言用)
  /// @param[in] pt_item パース木の関数定義
  virtual
  ElbDeclHead*
  new_DeclHead(const PtItem* pt_item) = 0;

  /// @brief 宣言要素のヘッダを生成する．(function の暗黙宣言用)
  /// @param[in] pt_item パース木の関数定義
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @param[in] left_val 範囲の MSB の値
  /// @param[in] right_val 範囲の LSB の値
  virtual
  ElbDeclHead*
  new_DeclHead(const PtItem* pt_item,
	       const PtExpr* left,
	       const PtExpr* right,
	       int left_val,
	       int right_val) = 0;
#endif

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
