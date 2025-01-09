#ifndef EIPARAMASSIGN_H
#define EIPARAMASSIGN_H

/// @file EiParamAssign.h
/// @brief EiParamAssign のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/VlParamAssign.h"
#include "ym/VlValue.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiParamAssign EiParamAssign.h "EiParamAssign.h"
/// @brief VlParamAssign の実装クラス
//////////////////////////////////////////////////////////////////////
class EiParamAssign :
  public VlParamAssign
{
public:

  /// @brief コンストラクタ
  EiParamAssign(
    const VlModule* module,  ///< [in] 親のモジュール
    const PtBase* pt_obj,    ///< [in] パース木の宣言要素
    ElbParameter* param,     ///< [in] 対象の parameter
    const PtExpr* rhs_expr,  ///< [in] 割り当て式の右辺
    const VlValue& rhs_value ///< [in] 右辺の値
  );

  /// @brief デストラクタ
  ~EiParamAssign();


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
  // VlParamAssign の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 親のモジュールを返す．
  const VlModule*
  parent_module() const override;

  /// @brief 左辺のパラメータを返す．
  const VlDecl*
  lhs() const override;

  /// @brief 右辺値を返す．
  VlValue
  rhs_value() const override;

  /// @brief 右辺の式を表す文字列を返す．
  string
  rhs_string() const override;

  /// @brief 名前による接続の場合に true を返す．
  bool
  is_conn_by_name() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親のモジュール
  const VlModule* mModule;

  // パース木中の対応するオブジェクト
  const PtBase* mPtObj;

  // パラメータ
  ElbParameter* mLhs;

  // 右辺式
  const PtExpr* mRhsExpr;

  // 右辺の値
  VlValue mRhsValue;

};


//////////////////////////////////////////////////////////////////////
/// @class EiParamAssign2 EiParamAssign.h "EiParamAssign.h"
/// @brief ElbParamAssign の実装クラス (名前による割り当て)
//////////////////////////////////////////////////////////////////////
class EiParamAssign2 :
  public EiParamAssign
{
public:

  /// @brief コンストラクタ
  EiParamAssign2(
    const VlModule* module,  ///< [in] 親のモジュール
    const PtBase* pt_obj,    ///< [in] パース木の接続を表す要素
    ElbParameter* param,     ///< [in] 対象の parameter
    const PtExpr* rhs_expr,  ///< [in] 割り当て式の右辺
    const VlValue& rhs_value ///< [in] 右辺の値
  );

  /// @brief デストラクタ
  ~EiParamAssign2();


public:
  //////////////////////////////////////////////////////////////////////
  // ElbParamAssign の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前による接続の場合に true を返す．
  /// @note このクラスでは true を返す．
  bool
  is_conn_by_name() const override;

};


//////////////////////////////////////////////////////////////////////
/// @class ElbDefParam EiParamAssign.h "EiParamAssign.h"
/// @brief VlDefParam の実装クラス
//////////////////////////////////////////////////////////////////////
class EiDefParam :
  public VlDefParam
{
public:

  /// @brief コンストラクタ
  EiDefParam(
    const VlModule* module,        ///< [in] 親のモジュール
    const PtItem* pt_header,       ///< [in] パース木の defparam ヘッダ
    const PtDefParam* pt_defparam, ///< [in] パース木の defparam 定義
    ElbParameter* param,           ///< [in] 対象の parameter
    const PtExpr* rhs_expr,        ///< [in] 割り当て式の右辺
    const VlValue& rhs_value       ///< [in] 右辺の値
  );

  /// @brief デストラクタ
  ~EiDefParam();


public:
  //////////////////////////////////////////////////////////////////////
  // ElbObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  VpiObjType
  type() const override;

  /// @brief ファイル位置を返す．
  FileRegion
  file_region() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // VlDefParam の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 親のモジュールを返す．
  const VlModule*
  parent_module() const override;

  /// @brief 左辺値を返す．
  const VlDecl*
  lhs() const override;

  /// @brief 右辺値を返す．
  VlValue
  rhs_value() const override;

  /// @brief 右辺の式を表す文字列を返す．
  string
  rhs_string() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親のモジュール
  const VlModule* mModule;

  // パース木の defparam ヘッダ
  const PtItem* mPtHead;

  // パース木の defparam 定義
  const PtDefParam* mPtDefParam;

  // パラメータ
  ElbParameter* mLhs;

  // 右辺式
  const PtExpr* mRhsExpr;

  // 右辺の値
  VlValue mRhsValue;

};

END_NAMESPACE_YM_VERILOG

#endif // EIPARAMASSIGN_H
