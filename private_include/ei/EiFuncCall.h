#ifndef EIFUNCCALL_H
#define EIFUNCCALL_H

/// @file EiFuncCall.h
/// @brief EiFuncCall のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.

#include "ei/EiExpr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiFcBase EiFuncCall.h "EiFuncCall.h"
/// @brief function call/system-function call に共通な基底クラス
//////////////////////////////////////////////////////////////////////
class EiFcBase :
  public EiExprBase
{
public:

  /// @brief コンストラクタ
  EiFcBase(
    const PtExpr* pt_expr,           ///< [in] パース木の定義要素
    const vector<ElbExpr*>& arg_list ///< [in] 引数のリスト
  );

  /// @brief デストラクタ
  ~EiFcBase();


public:
  //////////////////////////////////////////////////////////////////////
  // ElbExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 引数の数を返す．
  SizeType
  argument_num() const override;

  /// @brief 引数の取得
  const VlExpr*
  argument(
    SizeType pos ///< [in] 位置番号 ( 0 <= pos < argument_num() )
  ) const override;

  /// @brief 引数のリストの取得
  vector<const VlExpr*>
  argument_list() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要求される式の型を計算してセットする．
  void
  _set_reqsize(
    const VlValueType& type ///< [in] 要求される式の型
  ) override;


protected:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 引数の配列
  vector<ElbExpr*> mArgList;

};


//////////////////////////////////////////////////////////////////////
/// @class EiFuncCall EiFuncCall.h "EiFuncCall.h"
/// @brief 関数呼び出しを表すクラス
//////////////////////////////////////////////////////////////////////
class EiFuncCall :
  public EiFcBase
{
public:

  /// @brief コンストラクタ
  EiFuncCall(
    const PtExpr* pt_expr,           ///< [in] パース木の定義要素
    const VlTaskFunc* func,          ///< [in] 関数
    const vector<ElbExpr*>& arg_list ///< [in] 引数のリスト
  );

  /// @brief デストラクタ
  ~EiFuncCall();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  VpiObjType
  type() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 式のタイプを返す．
  VlValueType
  value_type() const override;

  /// @brief 定数の時 true を返す．
  bool
  is_const() const override;

  /// @brief 関数呼び出しの時に true を返す．
  bool
  is_funccall() const override;

  /// @brief 対象の関数を返す．
  const VlTaskFunc*
  function() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 関数
  const VlTaskFunc* mFunc;

};


//////////////////////////////////////////////////////////////////////
/// @class EiSysFuncCall EiFuncCall.h "EiFuncCall.h"
/// @brief システム関数呼び出しを表すクラス
//////////////////////////////////////////////////////////////////////
class EiSysFuncCall :
  public EiFcBase
{
public:

  /// @brief コンストラクタ
  EiSysFuncCall(
    const PtExpr* pt_expr,           ///< [in] パース木の定義要素
    const VlUserSystf* user_systf,   ///< [in] システム関数
    const vector<ElbExpr*>& arg_list ///< [in] 引数のリスト
  );

  /// @brief デストラクタ
  ~EiSysFuncCall();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  VpiObjType
  type() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 式のタイプを返す．
  VlValueType
  value_type() const override;

  /// @brief 定数の時 true を返す．
  bool
  is_const() const override;

  /// @brief システム関数よびあどい時に true を返す．
  bool
  is_sysfunccall() const override;

  /// @brief 対象のシステム関数を返す．
  const VlUserSystf*
  user_systf() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // システム関数
  const VlUserSystf* mUserSystf;

};

END_NAMESPACE_YM_VERILOG

#endif // EIFUNCCALL_H
