﻿#ifndef EICONTROL_H
#define EICONTROL_H

/// @file EiControl.h
/// @brief VlControl の実装クラスのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.

// IEEE Std 1364-2001 26.6.29 Delay control
// IEEE Std 1364-2001 26.6.30 Event control
// IEEE Std 1364-2001 26.6.31 Repeat control

#include "ym/vl/VlControl.h"
#include "ym/pt/PtP.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiControl EiControl.h "EiControl.h"
/// @brief VlControl の実装クラス
//////////////////////////////////////////////////////////////////////
class EiControl :
  public VlControl
{
protected:

  /// @brief コンストラクタ
  EiControl(
    const PtControl* pt_control ///< [in] パース木の定義要素
  );

  /// デストラクタ
  ~EiControl();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  FileRegion
  file_region() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // VlControl に固有の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 遅延式を返す．
  const VlExpr*
  delay() const override;

  /// @brief 繰り返し式を返す．
  const VlExpr*
  expr() const override;

  /// @brief イベント条件式の数を返す．
  SizeType
  event_num() const override;

  /// @brief イベント条件式を返す．
  const VlExpr*
  event(
    SizeType pos
  ) const override;

  /// @brief イベント条件式のリストを返す．
  vector<const VlExpr*>
  event_list() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // パース木の定義要素
  const PtControl* mPtControl;

};


//////////////////////////////////////////////////////////////////////
/// @class EiDelayControl EiControl.h "EiControl.h"
/// @brief delay control statement を表すクラス
/// IEEE1364-2001 では procedural timing control
/// とこれを混同している．
/// 正確には delay control はステートメントではない．
//////////////////////////////////////////////////////////////////////
class EiDelayControl :
  public EiControl
{
public:

  /// @brief コンストラクタ
  EiDelayControl(
    const PtControl* pt_control, ///< [in] パース木の定義要素
    ElbExpr* delay               ///< [in] 遅延式
  );

  /// デストラクタ
  ~EiDelayControl();


public:
  //////////////////////////////////////////////////////////////////////
  // ElbObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  VpiObjType
  type() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // VlControl に固有の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// 遅延式を返す．
  const VlExpr*
  delay() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 遅延式
  ElbExpr* mDelay;

};


//////////////////////////////////////////////////////////////////////
/// @class EiEventControl EiControl.h "EiControl.h"
/// @brief event control statement を表すクラス
/// IEEE1364-2001 では procedural timing control
/// とこれを混同している．
/// 正確には event control はステートメントではない．
//////////////////////////////////////////////////////////////////////
class EiEventControl :
  public EiControl
{
public:

  /// @brief コンストラクタ
  EiEventControl(
    const PtControl* pt_control,       ///< [in] パース木の定義要素
    const vector<ElbExpr*>& event_list ///< [in] イベントリスト
  );

  /// @brief デストラクタ
  ~EiEventControl();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  VpiObjType
  type() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // VlControl に固有の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief イベント条件式の数を返す．
  SizeType
  event_num() const override;

  /// @brief イベント条件式を返す．
  const VlExpr*
  event(
    SizeType pos ///< [in] 位置番号 ( 0 <= pos < event_num() )
  ) const override;

  /// @brief イベント条件式のリストを返す．
  vector<const VlExpr*>
  event_list() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // イベントリスト
  vector<ElbExpr*> mEventList;

};


//////////////////////////////////////////////////////////////////////
/// @class EiRepeatControl EiControl.h "EiControl.h"
/// @brief repeat control を表すクラス
/// IEEE Std 1364-2001 26.6.31 Repeat control
//////////////////////////////////////////////////////////////////////
class EiRepeatControl :
  public EiEventControl
{
public:

  /// @brief コンストラクタ
  EiRepeatControl(
    const PtControl* pt_control,       ///< [in] パース木の定義要素
    ElbExpr* rep,                      ///< [in] 繰り返し式
    const vector<ElbExpr*>& event_list ///< [in] イベントリスト
  );

  /// デストラクタ
  ~EiRepeatControl();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  VpiObjType
  type() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // VlControl に固有の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// 繰り返し式を返す．
  const VlExpr*
  expr() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 繰り返し式
  ElbExpr* mExpr;

};

END_NAMESPACE_YM_VERILOG

#endif // EICONTROL_H
