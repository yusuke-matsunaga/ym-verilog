﻿// $Id: SimDelayNode.cc 700 2007-05-31 00:41:30Z matsunaga $

// 遅延ノード

#if HAVE_CONFIG_H
#include <ymconfig.h>
#endif

#include "SimDelayNode.h"

#include <ym/vpi/VpiExpr.h>
#include "SimEngine.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// delay を持ったノード
//////////////////////////////////////////////////////////////////////

// コンストラクタ
SimDelayNode::SimDelayNode(SimEngine* engine,
			   const VpiScope* scope,
			   VpiExpr* delay) :
  SimNode(engine, scope),
  mDelay(delay)
{
}

// デストラクタ
SimDelayNode::~SimDelayNode()
{
}

// 遅延値を得る．
VlTime
SimDelayNode::delay() const
{
  return mDelay->eval_time();
}

// 実行する．
void
SimDelayNode::exec()
{
  //schedule_next(delay());
}


//////////////////////////////////////////////////////////////////////
// delay を持った nonblocking assign ノード
//////////////////////////////////////////////////////////////////////

// コンストラクタ
SimNbDelayNode::SimNbDelayNode(SimEngine* engine,
			       const VpiScope* scope,
			       VpiExpr* delay,
			       SimUpdateNode* update) :
  SimDelayNode(engine, scope, delay),
  mUpdate(update)
{
}

// デストラクタ
SimNbDelayNode::~SimNbDelayNode()
{
}

// 実行する．
void
SimNbDelayNode::exec()
{
  ASSERT_COND(mUpdate );
  engine()->reg_nonblocking_event(mUpdate, delay());

  //schedule_next();
}

END_NAMESPACE_YM_VERILOG
