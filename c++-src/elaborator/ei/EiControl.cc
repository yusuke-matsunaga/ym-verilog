
/// @file EiControl.cc
/// @brief EiControl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.

#include "ei/EiFactory.h"
#include "ei/EiControl.h"

#include "elaborator/ElbExpr.h"

#include "ym/pt/PtMisc.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief 遅延コントロールを生成する．
const VlControl*
EiFactory::new_DelayControl(
  const PtControl* pt_control,
  ElbExpr* delay
)
{
  auto control = new EiDelayControl(pt_control, delay);
  return control;
}

// @brief イベントコントロールを生成する．
const VlControl*
EiFactory::new_EventControl(
  const PtControl* pt_control,
  const vector<ElbExpr*>& event_list
)
{
  auto control = new EiEventControl(pt_control, event_list);
  return control;
}

// @brief リピートコントロールを生成する．
const VlControl*
EiFactory::new_RepeatControl(
  const PtControl* pt_control,
  ElbExpr* rep,
  const vector<ElbExpr*>& event_list
)
{
  auto control = new EiRepeatControl(pt_control, rep, event_list);
  return control;
}


//////////////////////////////////////////////////////////////////////
// クラス EiControl
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiControl::EiControl(
  const PtControl* pt_control
) : mPtControl{pt_control}
{
}

// デストラクタ
EiControl::~EiControl()
{
}

// @brief ファイル位置を返す．
FileRegion
EiControl::file_region() const
{
  return mPtControl->file_region();
}

// @brief 遅延式を返す．
const VlExpr*
EiControl::delay() const
{
  return nullptr;
}

// @brief 繰り返し式を返す．
const VlExpr*
EiControl::expr() const
{
  return nullptr;
}

// @brief イベント条件式の数を返す．
SizeType
EiControl::event_num() const
{
  return 0;
}

// @brief イベント条件式を返す．
const VlExpr*
EiControl::event(
  SizeType pos
) const
{
  return nullptr;
}

// @brief イベント条件式のリストを返す．
vector<const VlExpr*>
EiControl::event_list() const
{
  return {};
}


//////////////////////////////////////////////////////////////////////
/// クラス EiDelayControl
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiDelayControl::EiDelayControl(
  const PtControl* pt_control,
  ElbExpr* delay
) : EiControl{pt_control},
    mDelay{delay}
{
}

// デストラクタ
EiDelayControl::~EiDelayControl()
{
}

// @brief 型の取得
VpiObjType
EiDelayControl::type() const
{
  return VpiObjType::DelayControl;
}

// 遅延式を返す．
const VlExpr*
EiDelayControl::delay() const
{
  return mDelay;
}


//////////////////////////////////////////////////////////////////////
/// クラス EiEventControl EiControl.h "EiControl.h"
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiEventControl::EiEventControl(
  const PtControl* pt_control,
  const vector<ElbExpr*>& event_list
) : EiControl{pt_control},
    mEventList{event_list}
{
}

// @brief デストラクタ
EiEventControl::~EiEventControl()
{
}

// @brief 型の取得
VpiObjType
EiEventControl::type() const
{
  return VpiObjType::EventControl;
}

// @brief イベント条件式の数を返す．
SizeType
EiEventControl::event_num() const
{
  return mEventList.size();
}

// @brief イベント条件式を返す．
const VlExpr*
EiEventControl::event(
  SizeType pos
) const
{
  ASSERT_COND( 0 <= pos && pos < event_num() );
  return mEventList[pos];
}

// @brief イベント条件式のリストを返す．
vector<const VlExpr*>
EiEventControl::event_list() const
{
  return vector<const VlExpr*>{mEventList.begin(), mEventList.end()};
}


//////////////////////////////////////////////////////////////////////
/// クラス EiRepeatControl EiControl.h "EiControl.h"
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiRepeatControl::EiRepeatControl(
  const PtControl* pt_control,
  ElbExpr* rep,
  const vector<ElbExpr*>& event_list
) : EiEventControl{pt_control, event_list},
    mExpr{rep}
{
}

// デストラクタ
EiRepeatControl::~EiRepeatControl()
{
}

// @brief 型の取得
VpiObjType
EiRepeatControl::type() const
{
  return VpiObjType::RepeatControl;
}

// 繰り返し式を返す．
const VlExpr*
EiRepeatControl::expr() const
{
  return mExpr;
}

END_NAMESPACE_YM_VERILOG
