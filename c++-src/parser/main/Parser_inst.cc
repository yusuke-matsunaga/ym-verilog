﻿/// @file Parser_inst.cc
/// @brief Parser の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.

#include "parser/Parser.h"
#include "parser/PtiFactory.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// gate/module/UDP instance の生成
//////////////////////////////////////////////////////////////////////

// @brief gate instance 文のヘッダの生成
const PtItem*
Parser::new_GateH(
  const FileRegion& fr,
  VpiPrimType type
)
{
  auto item = mFactory->new_GateH(fr, type, nullptr, nullptr, mInstList);
  return item;
}

// @brief gate instance 文のヘッダの生成 (strength付き)
const PtItem*
Parser::new_GateH(
  const FileRegion& fr,
  VpiPrimType type,
  const PtStrength* strength
)
{
  auto item = mFactory->new_GateH(fr, type, strength, nullptr, mInstList);
  return item;
}

// @brief gate instance 文のヘッダの生成 (遅延付き)
const PtItem*
Parser::new_GateH(
  const FileRegion& fr,
  VpiPrimType type,
  const PtDelay* delay
)
{
  auto item = mFactory->new_GateH(fr, type, nullptr, delay, mInstList);
  return item;
}

// @brief gate instance 文のヘッダの生成 (strength, 遅延付き)
const PtItem*
Parser::new_GateH(
  const FileRegion& fr,
  VpiPrimType type,
  const PtStrength* strength,
  const PtDelay* delay
)
{
  auto item = mFactory->new_GateH(fr, type, strength, delay, mInstList);
  return item;
}

// @brief module instance/UDP instance 文のヘッダの生成
const PtItem*
Parser::new_MuH(
  const FileRegion& fr,
  const char* def_name
)
{
  auto item = mFactory->new_MuH(fr, def_name, nullptr, nullptr, mInstList);
  reg_defname(def_name);
  return item;
}

// @brief module instance/UDP instance 文のヘッダの生成 (strength付き)
const PtItem*
Parser::new_MuH(
  const FileRegion& fr,
  const char* def_name,
  const PtStrength* strength
)
{
  auto item = mFactory->new_MuH(fr, def_name, strength, nullptr, mInstList);
  reg_defname(def_name);
  return item;
}

// @brief module instance/UDP instance 文のヘッダの生成 (遅延付き)
const PtItem*
Parser::new_MuH(
  const FileRegion& fr,
  const char* def_name,
  const PtDelay* delay
)
{
  auto item = mFactory->new_MuH(fr, def_name, nullptr, delay, mInstList);
  reg_defname(def_name);
  return item;
}

// @brief module instance/UDP instance 文のヘッダの生成 (strength, 遅延付き)
const PtItem*
Parser::new_MuH(
  const FileRegion& fr,
  const char* def_name,
  const PtStrength* strength,
  const PtDelay* delay
)
{
  auto item = mFactory->new_MuH(fr, def_name, strength, delay, mInstList);
  reg_defname(def_name);
  return item;
}

// @brief module instance/UDP instance 文のヘッダの生成
const PtItem*
Parser::new_MuH(
  const FileRegion& fr,
  const char* def_name,
  PtrList<const PtConnection>* con_list
)
{
  auto item = mFactory->new_MuH(fr, def_name, con_list->to_vector(), mInstList);
  reg_defname(def_name);
  return item;
}

// @brief instance リストを初期化する．
void
Parser::init_inst()
{
  mInstList.clear();
}

// @brief module instance/UDP/gate instance の要素の生成
void
Parser::new_Inst(
  const FileRegion& fr,
  PtrList<const PtConnection>* con_list
)
{
  auto convec = con_list->to_vector();
  auto item = mFactory->new_Inst(fr, nullptr, nullptr, nullptr, convec);
  add_inst(item);
}

// @brief module instance/UDP/gate instance の要素の生成
void
Parser::new_Inst(
  const FileRegion& fr,
  const PtExpr* expr1
)
{
  auto convec = mFactory->new_OrderedConVector(expr1);
  auto item = mFactory->new_Inst(fr, nullptr, nullptr, nullptr, convec);
  add_inst(item);
}

// @brief module instance/UDP/gate instance の要素の生成
void
Parser::new_Inst(
  const FileRegion& fr,
  const PtExpr* expr1,
  const PtExpr* expr2
)
{
  auto convec = mFactory->new_OrderedConVector(expr1, expr2);
  auto item = mFactory->new_Inst(fr, nullptr, nullptr, nullptr, convec);
  add_inst(item);
}

// @brief module instance/UDP/gate instance の要素の生成
void
Parser::new_Inst(
  const FileRegion& fr,
  const PtExpr* expr1,
  const PtExpr* expr2,
  const PtExpr* expr3
)
{
  auto convec = mFactory->new_OrderedConVector(expr1, expr2, expr3);
  auto item = mFactory->new_Inst(fr, nullptr, nullptr, nullptr, convec);
  add_inst(item);
}

// @brief module instance/UDP/gate instance の要素の生成
void
Parser::new_Inst(
  const FileRegion& fr,
  const PtExpr* expr1,
  const PtExpr* expr2,
  const PtExpr* expr3,
  const PtExpr* expr4
)
{
  auto convec = mFactory->new_OrderedConVector(expr1, expr2, expr3, expr4);
  auto item = mFactory->new_Inst(fr, nullptr, nullptr, nullptr, convec);
  add_inst(item);
}

// @brief 名前付き module instance/UDP/gate instance の要素の生成
void
Parser::new_InstN(
  const FileRegion& fr,
  const char* name,
  PtrList<const PtConnection>* con_list
)
{
  auto convec = con_list->to_vector();
  auto item = mFactory->new_Inst(fr, name, nullptr, nullptr, convec);
  add_inst(item);
}

// @brief 名前付き module instance/UDP/gate instance の要素の生成
void
Parser::new_InstN(
  const FileRegion& fr,
  const char* name,
  const PtExpr* expr1
)
{
  auto convec = mFactory->new_OrderedConVector(expr1);
  auto item = mFactory->new_Inst(fr, name, nullptr, nullptr, convec);
  add_inst(item);
}

// @brief 名前付き module instance/UDP/gate instance の要素の生成
void
Parser::new_InstN(
  const FileRegion& fr,
  const char* name,
  const PtExpr* expr1,
  const PtExpr* expr2
)
{
  auto convec = mFactory->new_OrderedConVector(expr1, expr2);
  auto item = mFactory->new_Inst(fr, name, nullptr, nullptr, convec);
  add_inst(item);
}

// @brief 名前付き module instance/UDP/gate instance の要素の生成
void
Parser::new_InstN(
  const FileRegion& fr,
  const char* name,
  const PtExpr* expr1,
  const PtExpr* expr2,
  const PtExpr* expr3
)
{
  auto convec = mFactory->new_OrderedConVector(expr1, expr2, expr3);
  auto item = mFactory->new_Inst(fr, name, nullptr, nullptr, convec);
  add_inst(item);
}

// @brief 名前付き module instance/UDP/gate instance の要素の生成
void
Parser::new_InstN(
  const FileRegion& fr,
  const char* name,
  const PtExpr* expr1,
  const PtExpr* expr2,
  const PtExpr* expr3,
  const PtExpr* expr4
)
{
  auto convec = mFactory->new_OrderedConVector(expr1, expr2, expr3, expr4);
  auto item = mFactory->new_Inst(fr, name, nullptr, nullptr, convec);
  add_inst(item);
}

// @brief 名前と範囲付き module instance/UDP/gate instance の要素の生成
void
Parser::new_InstV(
  const FileRegion& fr,
  const char* name,
  const PtExpr* left,
  const PtExpr* right,
  PtrList<const PtConnection>* con_list
)
{
  auto convec = con_list->to_vector();
  auto item = mFactory->new_Inst(fr, name, left, right, convec);
  add_inst(item);
}

// @brief 名前と範囲付き module instance/UDP/gate instance の要素の生成
void
Parser::new_InstV(
  const FileRegion& fr,
  const char* name,
  const PtExpr* left,
  const PtExpr* right,
  const PtExpr* expr1
)
{
  auto convec = mFactory->new_OrderedConVector(expr1);
  auto item = mFactory->new_Inst(fr, name, left, right, convec);
  add_inst(item);
}

// @brief 名前と範囲付き module instance/UDP/gate instance の要素の生成
void
Parser::new_InstV(
  const FileRegion& fr,
  const char* name,
  const PtExpr* left,
  const PtExpr* right,
  const PtExpr* expr1,
  const PtExpr* expr2
)
{
  auto convec = mFactory->new_OrderedConVector(expr1, expr2);
  auto item = mFactory->new_Inst(fr, name, left, right, convec);
  add_inst(item);
}

// @brief 名前と範囲付き module instance/UDP/gate instance の要素の生成
void
Parser::new_InstV(
  const FileRegion& fr,
  const char* name,
  const PtExpr* left,
  const PtExpr* right,
  const PtExpr* expr1,
  const PtExpr* expr2,
  const PtExpr* expr3
)
{
  auto convec = mFactory->new_OrderedConVector(expr1, expr2, expr3);
  auto item = mFactory->new_Inst(fr, name, left, right, convec);
  add_inst(item);
}

// @brief 名前と範囲付き module instance/UDP/gate instance の要素の生成
void
Parser::new_InstV(
  const FileRegion& fr,
  const char* name,
  const PtExpr* left,
  const PtExpr* right,
  const PtExpr* expr1,
  const PtExpr* expr2,
  const PtExpr* expr3,
  const PtExpr* expr4
)
{
  auto convec = mFactory->new_OrderedConVector(expr1, expr2, expr3, expr4);
  auto item = mFactory->new_Inst(fr, name, left, right, convec);
  add_inst(item);
}

// @brief instance リストに要素を追加する．
inline
void
Parser::add_inst(
  const PtInst* inst
)
{
  mInstList.push_back(inst);
}

END_NAMESPACE_YM_VERILOG
