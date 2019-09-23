#ifndef EIPARAMASSIGN_H
#define EIPARAMASSIGN_H

/// @file EiParamAssign.h
/// @brief EiParamAssign のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "elb/ElbParamAssign.h"

#include "ym/VlValue.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiParamAssign EiParamAssign.h "EiParamAssign.h"
/// @brief ElbParamAssign の実装クラス
//////////////////////////////////////////////////////////////////////
class EiParamAssign :
  public ElbParamAssign
{
  friend class EiFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] module 親のモジュール
  /// @param[in] param 対象の parameter
  /// @param[in] rhs_expr 割り当て式の右辺
  /// @param[in] rhs_value 右辺の値
  EiParamAssign(const VlModule* module,
		const PtBase* pt_obj,
		ElbParameter* param,
		const PtExpr* rhs_expr,
		const VlValue& rhs_value);

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
  parent() const override;

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
  /// @note このクラスでは false を返す．
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
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] module 親のモジュール
  /// @param[in] pt_obj パース木の接続を表す要素
  /// @param[in] param 対象の parameter
  /// @param[in] rhs_expr 割り当て式の右辺
  /// @param[in] rhs_value 右辺の値
  EiParamAssign2(const VlModule* module,
		 const PtBase* pt_obj,
		 ElbParameter* param,
		 const PtExpr* rhs_expr,
		 const VlValue& rhs_value);

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
/// @brief ElbDefParam の実装クラス
//////////////////////////////////////////////////////////////////////
class EiDefParam :
  public ElbDefParam
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] module 親のモジュール
  /// @param[in] pt_header パース木の defparam ヘッダ
  /// @param[in] pt_item パース木の defparam 定義
  /// @param[in] param 対象の parameter
  /// @param[in] rhs_expr 割り当て式の右辺
  /// @param[in] rhs_value 右辺の値
  EiDefParam(const VlModule* module,
	     const PtItem* pt_header,
	     const PtDefParam* pt_defparam,
	     ElbParameter* param,
	     const PtExpr* rhs_expr,
	     const VlValue& rhs_value);

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
  // ElbDefParam の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 親のモジュールを返す．
  const VlModule*
  parent() const override;

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
