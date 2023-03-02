/// @file Parser_misc.cc
/// @brief Parser の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.

#include "parser/Parser.h"
#include "parser/PtiFactory.h"
#include "ym/pt/PtMisc.h"


BEGIN_NAMESPACE_YM_VERILOG

// @brief ディレイコントロールの生成
const PtControl*
Parser::new_DelayControl(
  const FileRegion& fr,
  const PtExpr* value
)
{
  auto control = mFactory->new_DelayControl(fr, value);
  return control;
}

// @brief イベントコントロールの生成
const PtControl*
Parser::new_EventControl(
  const FileRegion& fr
)
{
  auto control = mFactory->new_EventControl(fr, {});
  return control;
}

// @brief イベントコントロールの生成
const PtControl*
Parser::new_EventControl(
  const FileRegion& fr,
  const char* event_name,
  const FileRegion& name_loc
)
{
  auto expr = new_Primary(name_loc, event_name);
  auto control = mFactory->new_EventControl(fr, {expr});
  return control;
}

// @brief イベントコントロールの生成
const PtControl*
Parser::new_EventControl(
  const FileRegion& fr,
  PuHierName* event_name,
  const FileRegion& name_loc
)
{
  auto expr = new_Primary(name_loc, event_name);
  auto control = mFactory->new_EventControl(fr, {expr});
  return control;
}

// @brief イベントコントロールの生成
const PtControl*
Parser::new_EventControl(
  const FileRegion& fr,
  PtrList<const PtExpr>* event_list
)
{
  auto control = mFactory->new_EventControl(fr, event_list->to_vector());
  return control;
}

// @brief リピートコントロールの生成
const PtControl*
Parser::new_RepeatControl(
  const FileRegion& fr,
  const PtExpr* expr
)
{
  auto control = mFactory->new_RepeatControl(fr, expr, {});
  return control;
}

// @brief リピートコントロールの生成
const PtControl*
Parser::new_RepeatControl(
  const FileRegion& fr,
  const PtExpr* rep,
  const char* event_name,
  const FileRegion& name_loc
)
{
  auto expr = new_Primary(name_loc, event_name);
  auto control = mFactory->new_RepeatControl(fr, rep, {expr});
  return control;
}

// @brief リピートコントロールの生成
const PtControl*
Parser::new_RepeatControl(
  const FileRegion& fr,
  const PtExpr* rep,
  PuHierName* event_name,
  const FileRegion& name_loc
)
{
  auto expr = new_Primary(name_loc, event_name);
  auto control = mFactory->new_RepeatControl(fr, rep, {expr});
  return control;
}

// @brief リピートコントロールの生成
const PtControl*
Parser::new_RepeatControl(
  const FileRegion& fr,
  const PtExpr* rep,
  PtrList<const PtExpr>* event_list
)
{
  auto control = mFactory->new_RepeatControl(fr, rep, event_list->to_vector());
  return control;
}

// @brief 順序つき結合子の生成
const PtConnection*
Parser::new_OrderedCon(
  const PtExpr* expr
)
{
  auto con = mFactory->new_OrderedCon(expr);
  return con;
}

// @brief 順序つき結合子の生成
const PtConnection*
Parser::new_OrderedCon(
  const FileRegion& fr,
  const PtExpr* expr,
  PtrList<const PtAttrInst>* ai_list
)
{
  auto con = mFactory->new_OrderedCon(fr, expr);
  reg_attrinst(con, ai_list);
  return con;
}

// @brief 名前付き結合子の生成
const PtConnection*
Parser::new_NamedCon(
  const FileRegion& fr,
  const char* name,
  const PtExpr* expr,
  PtrList<const PtAttrInst>* ai_list
)
{
  auto con = mFactory->new_NamedCon(fr, name, expr);
  reg_attrinst(con, ai_list);
  return con;
}

// @brief strength の生成
const PtStrength*
Parser::new_Strength(
  const FileRegion& fr,
  VpiStrength value0,
  VpiStrength value1
)
{
  auto str = mFactory->new_Strength(fr, value0, value1);
  return str;
}

// @brief charge strength の生成
const PtStrength*
Parser::new_Strength(
  const FileRegion& fr,
  VpiStrength value
)
{
  auto str = mFactory->new_Strength(fr, value);
  return str;
}

// @brief 遅延値の生成 (1つの値)
const PtDelay*
Parser::new_Delay(
  const FileRegion& fr,
  const PtExpr* value1
)
{
  auto delay = mFactory->new_Delay(fr, value1);
  return delay;
}

// @brief 遅延値の生成 (2つの値)
const PtDelay*
Parser::new_Delay(
  const FileRegion& fr,
  const PtExpr* value1,
  const PtExpr* value2
)
{
  auto delay = mFactory->new_Delay(fr, value1, value2);
  return delay;
}

// @brief 遅延値の生成 (3つの値)
const PtDelay*
Parser::new_Delay(
  const FileRegion& fr,
  const PtExpr* value1,
  const PtExpr* value2,
  const PtExpr* value3
)
{
  auto delay = mFactory->new_Delay(fr, value1, value2, value3);
  return delay;
}

// @brief attribute instance の生成
const PtAttrInst*
Parser::new_AttrInst(
  const FileRegion& fr,
  PtrList<const PtAttrSpec>* as_list
)
{
  auto ai = mFactory->new_AttrInst(fr, as_list->to_vector());
  return ai;
}

// @brief attribute spec の生成
const PtAttrSpec*
Parser::new_AttrSpec(
  const FileRegion& fr,
  const char* name,
  const PtExpr* expr
)
{
  auto as = mFactory->new_AttrSpec(fr, name, expr);
  return as;
}

END_NAMESPACE_YM_VERILOG
