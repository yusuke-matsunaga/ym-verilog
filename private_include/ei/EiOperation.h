#ifndef EIOPERATION_H
#define EIOPERATION_H

/// @file EiOperation.h
/// @brief EiOperation のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.

#include "EiExpr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiOperation EiOperation.h "EiOperation.h"
/// @brief 演算子を表すクラス
//////////////////////////////////////////////////////////////////////
class EiOperation :
  public EiExprBase
{
protected:

  /// @brief コンストラクタ
  EiOperation(
    const PtExpr* pt_expr ///< [in] パース木の定義要素
  );

  /// @brief デストラクタ
  ~EiOperation();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  VpiObjType
  type() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の派生クラスに共通な仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 演算子の時に true を返す．
  bool
  is_operation() const override;

  /// @brief 演算子のタイプを返す．
  VpiOpType
  op_type() const override;

};

END_NAMESPACE_YM_VERILOG

#endif // EIOPERATION_H
