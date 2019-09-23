#ifndef EICONTROL_H
#define EICONTROL_H

/// @file EiControl.h
/// @brief ElbControl の実装クラスのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.

// IEEE Std 1364-2001 26.6.29 Delay control
// IEEE Std 1364-2001 26.6.30 Event control
// IEEE Std 1364-2001 26.6.31 Repeat control


#include "elb/ElbControl.h"
#include "ym/pt/PtP.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiControl EiControl.h "EiControl.h"
/// @brief ElbControl の実装クラス
//////////////////////////////////////////////////////////////////////
class EiControl :
  public ElbControl
{
protected:

  /// @brief コンストラクタ
  /// @param[in] pt_control パース木の定義要素
  EiControl(const PtControl* pt_control);

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
  /// @note このクラスでは nullptr を返す．
  const VlExpr*
  delay() const override;

  /// @brief 繰り返し式を返す．
  /// @note このクラスでは nullptr を返す．
  const VlExpr*
  expr() const override;

  /// @brief イベント条件式の数を返す．
  /// @note このクラスでは 0 を返す．
  SizeType
  event_num() const override;

  /// @brief イベント条件式を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < event_num() )
  /// @note このクラスでは nullptr を返す．
  const VlExpr*
  event(SizeType pos) const override;


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
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] pt_control パース木の定義要素
  /// @param[in] delay 遅延式
  EiDelayControl(const PtControl* pt_control,
		 ElbExpr* delay);

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
  // ElbControl に固有の仮想関数
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
  friend class EiFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] pt_control パース木の定義要素
  /// @param[in] event_num イベントリストの要素数
  /// @param[in] event_list イベントリストを表す配列
  EiEventControl(const PtControl* pt_control,
		 int event_num,
		 ElbExpr** event_list);

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
  /// @param[in] pos 位置番号 ( 0 <= pos < event_num() )
  const VlExpr*
  event(SizeType pos) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // イベントリストの要素数
  SizeType mEventNum;

  // イベントリスト
  ElbExpr** mEventList;

};


//////////////////////////////////////////////////////////////////////
/// @class EiRepeatControl ElbControl.h "ElbControl.h"
/// @brief repeat control を表すクラス
/// IEEE Std 1364-2001 26.6.31 Repeat control
//////////////////////////////////////////////////////////////////////
class EiRepeatControl :
  public EiEventControl
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] pt_control パース木の定義要素
  /// @param[in] rep 繰り返し式
  /// @param[in] event_num イベントリストの要素数
  /// @param[in] event_list イベントリストを表す配列
  EiRepeatControl(const PtControl* pt_control,
		  ElbExpr* rep,
		  int event_num,
		  ElbExpr** event_list);

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
