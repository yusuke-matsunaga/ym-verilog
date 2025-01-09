#ifndef EILHS_H
#define EILHS_H

/// @file EiLhs.h
/// @brief EiLhs のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.

#include "EiConcatOp.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiLhs EiLhs.h "EiLhs.h"
/// @brief 連結演算子の左辺式を表すクラス
//////////////////////////////////////////////////////////////////////
class EiLhs :
  public EiConcatOp
{
public:

  /// @brief コンストラクタ
  EiLhs(
    const PtExpr* pt_expr,                 ///< [in] パース木の定義要素
    const vector<ElbExpr*>& opr_array,     ///< [in] オペランドを格納する配列
    const vector<ElbExpr*>& lhs_elem_array ///< [in] 左辺の要素の配列
  );

  /// @brief デストラクタ
  ~EiLhs();


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 左辺式の要素数の取得
  SizeType
  lhs_elem_num() const override;

  /// @brief 左辺式の要素の取得
  const VlExpr*
  lhs_elem(
    SizeType pos ///< [in] 位置 ( 0 <= pos < lhs_elem_num() )
  ) const override;

  /// @brief 左辺式の要素のリストの取得
  vector<const VlExpr*>
  lhs_elem_list() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 要素の配列
  vector<ElbExpr*> mArray;

};

END_NAMESPACE_YM_VERILOG

#endif // EILHS_H
