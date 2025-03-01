﻿#ifndef EITASKFUNC_H
#define EITASKFUNC_H

/// @file EiTaskFunc.h
/// @brief EiTaskFunc のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.

#include "elaborator/ElbTaskFunc.h"
#include "ei/EiRange.h"


BEGIN_NAMESPACE_YM_VERILOG

class EiIODecl;

//////////////////////////////////////////////////////////////////////
/// @class EiTask EiTaskFunc.h "EiTaskFunc.h"
/// @brief ElbTaskFunc の実装クラス
//////////////////////////////////////////////////////////////////////
class EiTaskFunc :
  public ElbTaskFunc
{
public:

  /// @brief コンストラクタ
  EiTaskFunc(
    const VlScope* parent, ///< [in] 親のスコープ
    const PtItem* pt_item, ///< [in] パース木の定義
    SizeType io_num        ///< [in] IOの数
  );

  /// @brief デストラクタ
  ~EiTaskFunc();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  FileRegion
  file_region() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // VlNamedObj の派生クラスに共通な仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief このオブジェクトの属しているスコープを返す．
  const VlScope*
  parent_scope() const override;

  /// @brief 名前の取得
  string
  name() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // VlTaskFunc の仮想関数(タスク/関数共通)
  //////////////////////////////////////////////////////////////////////

  /// @brief automatic 宣言されていたら true を返す．
  bool
  automatic() const override;

  /// @brief 入出力数を得る．
  SizeType
  io_num() const override;

  /// @brief 入出力の取得
  const VlIODecl*
  io(
    SizeType pos ///< [in] 位置番号 ( 0 <= pos < io_num() )
  ) const override;

  /// @brief 入出力のリストの取得
  vector<const VlIODecl*>
  io_list() const override;

  /// @brief 本体のステートメントを得る．
  const VlStmt*
  stmt() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbTaskFunc の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入出力の初期設定を行う．
  void
  add_iodecl(
    ElbIOHead* head,         ///< [in] ヘッダ
    const PtIOItem* pt_item, ///< [in] パース木のIO宣言要素
    const VlDecl* decl       ///< [in] 対応する宣言要素
  ) override;

  /// @brief 本体のステートメントをセットする．
  void
  set_stmt(
    const VlStmt* stmt
  ) override;


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief PtItem を取り出す．
  const PtItem*
  pt_item() const
  {
    return mPtItem;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親のスコープ
  const VlScope* mParent;

  // パース木のタスク/関数定義
  const PtItem* mPtItem;

  // 入出力用ヘッダのリスト
  vector<ElbIOHead*> mIOHeadList;

  // 入出力のリスト
  vector<EiIODecl> mIODeclList;

  // 本体のステートメント
  const VlStmt* mStmt;

};


//////////////////////////////////////////////////////////////////////
/// @class EiTask EiTaskFunc.h "EiTaskFunc.h"
/// @brief タスクを表す EiTaskFunc の実装クラス
//////////////////////////////////////////////////////////////////////
class EiTask :
  public EiTaskFunc
{
public:

  /// @brief コンストラクタ
  EiTask(
    const VlScope* parent, ///< [in] 親のスコープ
    const PtItem* pt_item, ///< [in] パース木の定義
    SizeType io_num        ///< [in] IOの数
  );

  /// @brief デストラクタ
  ~EiTask();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  VpiObjType
  type() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // VlTaskFunc の仮想関数(関数の時のみ意味を持つもの)
  //////////////////////////////////////////////////////////////////////

  /// @brief function type を返す．
  VpiFuncType
  func_type() const override;

  /// @brief 符号付きの時 true を返す．
  bool
  is_signed() const override;

  /// @brief 範囲指定を持つとき true を返す．
  bool
  has_range() const override;

  /// @brief 範囲の MSB の値を返す．
  int
  left_range_val() const override;

  /// @brief 範囲の LSB の値を返す．
  int
  right_range_val() const override;

  /// @brief 範囲のMSBを表す文字列の取得
  string
  left_range_string() const override;

  /// @brief 範囲のLSBを表す文字列の取得
  string
  right_range_string() const override;

  /// @brief 出力のビット幅を返す．
  SizeType
  bit_size() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbTaskFunc の仮想関数
  // ただし Function の時しか意味を持たない．
  //////////////////////////////////////////////////////////////////////

  /// @brief 出力変数をセットする．
  void
  set_ovar(
    ElbDecl* ovar ///< [in] 出力変数
  ) override;

  /// @brief constant function の時に true を返す．
  bool
  is_constant_function() const override;

  /// @brief 出力変数を返す．
  ///
  /// 出力変数とは関数名と同名の変数
  VlDecl*
  ovar() const override;

};


//////////////////////////////////////////////////////////////////////
/// @class EiFunction EiTaskFunc.h "EiTaskFunc.h"
/// @brief 関数を表す EiTaskFunc の実装クラス
//////////////////////////////////////////////////////////////////////
class EiFunction :
  public EiTaskFunc
{
public:

  /// @brief コンストラクタ
  EiFunction(
    const VlScope* parent, ///< [in] 親のスコープ
    const PtItem* pt_item, ///< [in] パース木の定義
    SizeType io_num,       ///< [in] IOの数
    bool const_func        ///< [in] constant function の時 true にする．
  );

  /// @brief デストラクタ
  ~EiFunction();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  VpiObjType
  type() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // VlTaskFunc の仮想関数(関数の時のみ意味を持つもの)
  //////////////////////////////////////////////////////////////////////

  /// @brief function type を返す．
  VpiFuncType
  func_type() const override;

  /// @brief 符号付きの時 true を返す．
  bool
  is_signed() const override;

  /// @brief 範囲指定を持つとき true を返す．
  bool
  has_range() const override;

  /// @brief 範囲の MSB の値を返す．
  int
  left_range_val() const override;

  /// @brief 範囲の LSB の値を返す．
  int
  right_range_val() const override;

  /// @brief 範囲のMSBを表す文字列の取得
  string
  left_range_string() const override;

  /// @brief 範囲のLSBを表す文字列の取得
  string
  right_range_string() const override;

  /// @brief 出力のビット幅を返す．
  SizeType
  bit_size() const override;

  /// @brief constant function の時に true を返す．
  bool
  is_constant_function() const override;

  /// @brief 出力変数を返す．
  ///
  /// 出力変数とは関数名と同名の変数
  VlDecl*
  ovar() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbTaskFunc の仮想関数
  // ただし Function の時しか意味を持たない．
  //////////////////////////////////////////////////////////////////////

  /// @brief 出力変数をセットする．
  void
  set_ovar(
    ElbDecl* ovar ///< [in] 出力変数
  ) override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 出力変数
  ElbDecl* mOvar;

  // 定数関数フラグ
  bool mConstFunc;

};


//////////////////////////////////////////////////////////////////////
/// @class EiFunctionV EiaskFunc.h "EiTaskFunc.h"
/// @brief ベクタ型の関数を表すクラス
//////////////////////////////////////////////////////////////////////
class EiFunctionV :
  public EiFunction
{
public:

  /// @brief コンストラクタ
  EiFunctionV(
    const VlScope* parent, ///< [in] 親のスコープ
    const PtItem* pt_item, ///< [in] パース木の定義
    SizeType io_num,       ///< [in] IOの数
    const PtExpr* left,    ///< [in] 範囲の MSB の式
    const PtExpr* right,   ///< [in] 範囲の LSB の式
    int left_val,          ///< [in] 範囲の MSB の値
    int right_val,         ///< [in] 範囲の LSB の値
    bool const_func        ///< [in] 定数関数フラグ
  );

  /// @brief デストラクタ
  ~EiFunctionV();


public:
  //////////////////////////////////////////////////////////////////////
  // VlFunction の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 範囲指定を持つとき true を返す．
  bool
  has_range() const override;

  /// @brief 範囲の MSB の値を返す．
  int
  left_range_val() const override;

  /// @brief 範囲の LSB の値を返す．
  int
  right_range_val() const override;

  /// @brief 範囲のMSBを表す文字列の取得
  string
  left_range_string() const override;

  /// @brief 範囲のLSBを表す文字列の取得
  string
  right_range_string() const override;

  /// @brief 出力のビット幅を返す．
  SizeType
  bit_size() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 範囲
  EiRangeImpl mRange;

};

END_NAMESPACE_YM_VERILOG

#endif // EITASKFUNC_H
