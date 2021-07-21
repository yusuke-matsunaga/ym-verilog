#ifndef ELBEXPR_H
#define ELBEXPR_H

/// @file ElbExpr.h
/// @brief ElbExpr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/VlExpr.h"
#include "ym/VlValueType.h"
#include "ElbFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ElbExpr ElbExpr.h "ElbExpr.h"
/// @brief エラボレーション中の expression を表す基底クラス
//////////////////////////////////////////////////////////////////////
class ElbExpr :
  public VlExpr
{
public:

  /// @brief コンストラクタ
  ElbExpr() = default;

  /// @brief デストラクタ
  ~ElbExpr() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要求された値のタイプを返す．
  VlValueType
  req_type() const override;

  /// @brief 式のビット幅を返す．
  SizeType
  bit_size() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // ElxExpr の外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 要求される式の型を計算してセットする．
  ///
  /// 必要であればオペランドに対して再帰的に処理を行なう．
  void
  set_reqsize(
    const VlValueType& type ///< [in] 要求される式の型
  )
  {
    mReqType = type;
    _set_reqsize(type);
  }

  /// @brief 要求される式のサイズを自分で決めてセットする．
  ///
  /// 必要であればオペランドに対して再帰的に処理を行なう．
  void
  set_selfsize()
  {
    set_reqsize(value_type());
  }


protected:
  //////////////////////////////////////////////////////////////////////
  // ElbExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief set_reqsize() の下請け関数
  ///
  /// 必要であればオペランドに対して再帰的に処理を行なう．
  virtual
  void
  _set_reqsize(
    const VlValueType& type ///< [in] 要求される式の型
  ) = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // 便利関数
  //////////////////////////////////////////////////////////////////////

  // 二項演算のタイプとサイズを決める．
  static
  VlValueType
  calc_type(
    const VlValueType& type0,
    const VlValueType& type1
  );


  // 巾乗演算のタイプとサイズを決める．
  static
  VlValueType
  calc_type2(
    const VlValueType& type0,
    const VlValueType& type1
  );

  // 出力に要求されているサイズから自分のサイズを決める．
  static
  VlValueType
  update_size(
    const VlValueType& type,
    const VlValueType& req_type
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 要求された値のタイプ
  VlValueType mReqType;

};

END_NAMESPACE_YM_VERILOG

#endif // ELBEXPR_H
