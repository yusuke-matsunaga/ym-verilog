#ifndef EIDELAY_H
#define EIDELAY_H

/// @file EiDelay.h
/// @brief EiExpr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/VlDelay.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiDelay EiDelay.h "EiDelay.h"
/// @brief ElbDelay の実装クラス
//////////////////////////////////////////////////////////////////////
class EiDelay :
  public VlDelay
{
public:

  /// @brief コンストラクタ
  EiDelay(
    const PtBase* pt_obj,             ///< [in] パース木の定義要素
    const vector<ElbExpr*>& expr_list ///< [in] 式の配列
  );

  /// @brief デストラクタ
  ~EiDelay();


public:
  //////////////////////////////////////////////////////////////////////
  // ElbObj の派生クラスに共通な仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  VpiObjType
  type() const override;

  /// @brief ファイル位置の取得
  FileRegion
  file_region() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbDelay の派生クラスに共通な仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素数を返す．
  SizeType
  elem_num() const override;

  /// @brief 値を返す．
  const VlExpr*
  expr(
    SizeType pos ///< [in] 位置番号 ( 0 <= pos < elem_num() )
  ) const override;

  /// @brief 値のリストを返す．
  vector<const VlExpr*>
  expr_list() const override;

  /// @brief Verilog-HDL の文字列を得る．
  string
  decompile() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // パース木の定義要素
  const PtBase* mPtObj;

  // 要素の配列
  vector<ElbExpr*> mElemList;

};

END_NAMESPACE_YM_VERILOG

#endif // EIDELAY_H
