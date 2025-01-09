#ifndef EIPROCESS_H
#define EIPROCESS_H

/// @file EiProcess.h
/// @brief EiProcess のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.

#include "elaborator/ElbProcess.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiProcess EiProcess.h "EiProcess.h"
/// @brief initial/always 文を表すクラス
//////////////////////////////////////////////////////////////////////
class EiProcess :
  public ElbProcess
{
public:

  /// @brief コンストラクタ
  EiProcess(
    const VlScope* parent, ///< [in] 親のスコープ
    const PtItem* pt_item  ///< [in] パース木の要素定義
  );

  /// @brief デストラクタ
  ~EiProcess();


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
  // VlProcess の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 親のスコープを返す．
  const VlScope*
  parent_scope() const override;

  /// @brief 本体のステートメントの取得
  const VlStmt*
  stmt() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbProcess の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 本体のステートメントをセットする．
  void
  set_stmt(
    const VlStmt* stmt ///< [in] 本体のステートメント
  ) override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親のスコープ
  const VlScope* mParent;

  // パース木の要素定義
  const PtItem* mPtItem;

  // 本体のステートメント
  const VlStmt* mStmt;

};

END_NAMESPACE_YM_VERILOG

#endif // EIPROCESS_H
