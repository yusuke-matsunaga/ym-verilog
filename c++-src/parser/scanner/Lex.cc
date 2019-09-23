
/// @file Lex.cc
/// @brief Lex の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "scanner/Lex.h"
#include "LexModuleState.h"
#include "LexModulePlugin.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// Verilog 用の LEX クラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
Lex::Lex()
{
  mModuleState = new LexModuleState(*this);

  // 生成されたプラグインは Lex の管理下におかれるのでここでポインタを
  // 保持しておく必要はない．
  new LpNetType(*this, "default_nettype", mModuleState);
  new LpTimeScale(*this, "timescale", mModuleState);
  new LpCellDefine(*this, "celldefine", mModuleState);
  new LpEndCellDefine(*this, "endcelldefine", mModuleState);
  new LpUnconnDrive(*this, "unconnected_drive", mModuleState);
  new LpNounconnDrive(*this, "nounconnected_drive", mModuleState);
  new LpDecayTime(*this, "default_decay_time", mModuleState);
  new LpTriregStrength(*this, "default_trireg_strength", mModuleState);
  new LpDelayMode(*this, "delay_mode_distribute", mModuleState, VpiDefDelayMode::Distrib);
  new LpDelayMode(*this, "delay_mode_path", mModuleState, VpiDefDelayMode::Path);
  new LpDelayMode(*this, "delay_mode_unit", mModuleState, VpiDefDelayMode::Unit);
  new LpDelayMode(*this, "delay_mode_zero", mModuleState, VpiDefDelayMode::Zero);
}

// @brief デストラクタ
Lex::~Lex()
{
}

// @brief cell_define の状態を取得する
// @retval true cell_define されている
// @retval false cell_define されていない
bool
Lex::cell_define() const
{
  return mModuleState->cell_define();
}

// @brief net_type を取得する．
VpiNetType
Lex::default_nettype() const
{
  return mModuleState->default_nettype();
}

// @brief time_unit の取得
int
Lex::time_unit() const
{
  return mModuleState->time_unit();
}

// @brief time_precision の取得
int
Lex::time_precision() const
{
  return mModuleState->time_precision();
}

// @brief unconnected_drive の値を取得する．
VpiUnconnDrive
Lex::unconnected_drive() const
{
  return mModuleState->unconnected_drive();
}

// @brief decay_time の値を取得する．
int
Lex::default_decay_time() const
{
  return mModuleState->default_decay_time();
}

// @brief trireg_strength の値を取得する．
int
Lex::default_trireg_strength() const
{
  return mModuleState->default_trireg_strength();
}

// @brief delay_mode の値を取得する．
VpiDefDelayMode
Lex::delay_mode() const
{
  return mModuleState->delay_mode();
}

END_NAMESPACE_YM_VERILOG
