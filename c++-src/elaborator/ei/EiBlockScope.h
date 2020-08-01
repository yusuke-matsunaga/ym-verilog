#ifndef EIBLOCKSCOPE_H
#define EIBLOCKSCOPE_H

/// @file libym/elaborator/ei/EiBlockScope.h
/// @brief EiBlockScope のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym/vl/VlNamedObj.h"
#include "ym/pt/PtP.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiBlockScope EiBlockScope.h "EiBlockScope.h"
/// @brief elaboration 中の statement block を表すクラス
//////////////////////////////////////////////////////////////////////
class EiBlockScope :
  public VlNamedObj
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ環境
  /// @param[in] pt_stmt 対応するパース木の要素
  EiBlockScope(const VlNamedObj* parent,
	       const PtStmt* pt_stmt);

  /// @brief デストラクタ
  ~EiBlockScope();


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
  // VlNamedObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief このオブジェクトの属しているスコープを返す．
  const VlNamedObj*
  parent() const override;

  /// @brief 名前の取得
  const char*
  name() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親のスコープ
  const VlNamedObj* mParent;

  // 対応するパース木の要素
  const PtStmt* mPtStmt;

};

END_NAMESPACE_YM_VERILOG

#endif // EIBLOCKSCOPE_H
