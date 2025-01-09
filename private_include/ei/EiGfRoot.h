#ifndef EIGFROOT_H
#define EIGFROOT_H

/// @file EiGenBlock.h
/// @brief EiGenBlock のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/VlNamedObj.h"
#include "elaborator/ElbGfRoot.h"

#include "ym/pt/PtP.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiGfRoot EiGfRoot.h "EiGfRoot.h"
/// @brief GfBlock の親となるクラス
///
/// スコープとしての親ではなく，名前による検索のためのオブジェクト
//////////////////////////////////////////////////////////////////////
class EiGfRoot :
  public ElbGfRoot
{
public:

  /// @brief コンストラクタ
  EiGfRoot(
    const VlScope* parent, ///< [in] 親のスコープ
    const PtItem* pt_item  ///< [in] 対応するパース木の要素
  );

  /// @brief デストラクタ
  ~EiGfRoot();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の派生クラスに共通な仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  VpiObjType
  type() const override;

  /// @brief ファイル位置の取得
  FileRegion
  file_region() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // VlNamedObj の派生クラスに共通な仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief このオブジェクトの属しているスコープを返す．
  const VlScope*
  parent_scope() const override;

  /// @brief 名前の取得
  string
  name() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbGfRoot の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 子供のスコープを追加する．
  void
  add(
    SizeType index,
    const VlScope* block
  ) override;

  /// @brief 子供のスコープを取り出す．
  const VlScope*
  elem_by_index(
    SizeType index
  ) override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親のスコープ
  const VlScope* mParent;

  // 対応するパース木の要素
  const PtItem* mPtItem;

  // 子供のスコープのハッシュ表
  unordered_map<SizeType, const VlScope*> mTable;

};

END_NAMESPACE_YM_VERILOG

#endif // EIGFROOT_H
