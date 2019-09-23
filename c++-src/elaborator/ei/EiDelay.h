#ifndef EIDELAY_H
#define EIDELAY_H

/// @file EiDelay.h
/// @brief EiExpr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ElbDelay.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiDelay EiDelay.h "EiDelay.h"
/// @brief ElbDelay の実装クラス
//////////////////////////////////////////////////////////////////////
class EiDelay :
  public ElbDelay
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] pt_obj パース木の定義要素
  /// @param[in] elem_num 要素数
  /// @param[in] expr_list 式の配列
  EiDelay(const PtBase* pt_obj,
	  SizeType elem_num,
	  ElbExpr** expr_list);

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
  /// @param[in] pos 位置番号 ( 0 <= pos < elem_num() )
  const VlExpr*
  expr(SizeType pos) const override;

  /// @brief Verilog-HDL の文字列を得る．
  string
  decompile() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // パース木の定義要素
  const PtBase* mPtObj;

  // 要素数
  SizeType mElemNum;

  // 要素の配列
  ElbExpr** mElemArray;

};

END_NAMESPACE_YM_VERILOG

#endif // EIDELAY_H
