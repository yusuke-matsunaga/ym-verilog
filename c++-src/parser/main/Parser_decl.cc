/// @file Parser_decl.cc
/// @brief Parser の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.

#include "parser/Parser.h"
#include "parser/PtiFactory.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// PtiIOHead の生成
//////////////////////////////////////////////////////////////////////

// @brief IO 宣言のヘッダの生成
PtiIOHead*
Parser::new_IOHead(
  const FileRegion& fr,
  VpiDir dir,
  bool sign
)
{
  auto decl = mFactory->new_IOHead(fr, dir, sign, nullptr, nullptr);
  return decl;
}

// @brief IO 宣言のヘッダの生成 (reg 型)
PtiIOHead*
Parser::new_RegIOHead(
  const FileRegion& fr,
  VpiDir dir,
  bool sign
)
{
  auto decl = mFactory->new_RegIOHead(fr, dir, sign, nullptr, nullptr);
  return decl;
}

// @brief IO 宣言のヘッダの生成 (ネット型)
PtiIOHead*
Parser::new_NetIOHead(
  const FileRegion& fr,
  VpiDir dir,
  VpiNetType net_type,
  bool sign
)
{
  auto decl = mFactory->new_NetIOHead(fr, dir, net_type, sign, nullptr, nullptr);
  return decl;
}

// @brief IO 宣言のヘッダの生成 (変数型)
PtiIOHead*
Parser::new_VarIOHead(
  const FileRegion& fr,
  VpiDir dir,
  VpiVarType var_type
)
{
  auto decl = mFactory->new_VarIOHead(fr, dir, var_type);
  return decl;
}

// @brief 範囲付きの IO 宣言のヘッダの生成
PtiIOHead*
Parser::new_IOHead(
  const FileRegion& fr,
  VpiDir dir,
  bool sign,
  const PtExpr* left,
  const PtExpr* right
)
{
  auto decl = mFactory->new_IOHead(fr, dir, sign, left, right);
  return decl;
}

// @brief 範囲付きの IO 宣言のヘッダの生成 (reg 型)
PtiIOHead*
Parser::new_RegIOHead(
  const FileRegion& fr,
  VpiDir dir,
  bool sign,
  const PtExpr* left,
  const PtExpr* right
)
{
  auto decl = mFactory->new_RegIOHead(fr, dir, sign, left, right);
  return decl;
}

// @brief 範囲付きの IO 宣言のヘッダの生成 (ネット型)
PtiIOHead*
Parser::new_NetIOHead(
  const FileRegion& fr,
  VpiDir dir,
  VpiNetType net_type,
  bool sign,
  const PtExpr* left,
  const PtExpr* right
)
{
  auto decl = mFactory->new_NetIOHead(fr, dir, net_type, sign, left, right);
  return decl;
}


//////////////////////////////////////////////////////////////////////
// PtIOItem の生成
//////////////////////////////////////////////////////////////////////

// @brief IO 宣言の要素の生成
void
Parser::new_IOItem(
  const FileRegion& fr,
  const char* name
)
{
  auto item = mFactory->new_IOItem(fr, name, nullptr);
  add_io_item(item);
}

// @brief 初期値付き IO 宣言の要素の生成
void
Parser::new_IOItem(
  const FileRegion& fr,
  const char* name,
  const PtExpr* init_value
)
{
  auto item = mFactory->new_IOItem(fr, name, init_value);
  add_io_item(item);
}


//////////////////////////////////////////////////////////////////////
// PtDeclHead ( parameter ) の生成
//////////////////////////////////////////////////////////////////////

// @brief パラメータ宣言のヘッダの生成 (型指定なし)
PtiDeclHead*
Parser::new_ParamH(
  const FileRegion& fr
)
{
  auto decl = mFactory->new_ParamH(fr, false, nullptr, nullptr);
  return decl;
}

// @brief 範囲指定型パラメータ宣言のヘッダの生成
PtiDeclHead*
Parser::new_ParamH(
  const FileRegion& fr,
  bool sign,
  const PtExpr* left,
  const PtExpr* right
)
{
  auto decl = mFactory->new_ParamH(fr, sign, left, right);
  return decl;
}

// @brief 組み込み型パラメータ宣言のヘッダの生成
PtiDeclHead*
Parser::new_ParamH(
  const FileRegion& fr,
  VpiVarType var_type
)
{
  auto decl = mFactory->new_ParamH(fr, var_type);
  return decl;
}


//////////////////////////////////////////////////////////////////////
// PtDeclHead ( localparam ) の生成
//////////////////////////////////////////////////////////////////////

// @brief local param 宣言のヘッダの生成 (型指定なし)
PtiDeclHead*
Parser::new_LocalParamH(
  const FileRegion& fr
)
{
  auto decl = mFactory->new_ParamH(fr, false, nullptr, nullptr, true);
  return decl;
}

// @brief 範囲指定型 local param 宣言のヘッダの生成
PtiDeclHead*
Parser::new_LocalParamH(
  const FileRegion& fr,
  bool sign,
  const PtExpr* left,
  const PtExpr* right
)
{
  auto decl = mFactory->new_ParamH(fr, sign, left, right, true);
  return decl;
}

// @brief 組み込み型 local param 宣言のヘッダの生成
PtiDeclHead*
Parser::new_LocalParamH(
  const FileRegion& fr,
  VpiVarType var_type
)
{
  auto decl = mFactory->new_ParamH(fr, var_type, true);
  return decl;
}


//////////////////////////////////////////////////////////////////////
// PtDeclHead の生成
//////////////////////////////////////////////////////////////////////

// @brief specparam 宣言のヘッダの生成
PtiDeclHead*
Parser::new_SpecParamH(
  const FileRegion& fr
)
{
  auto decl = mFactory->new_SpecParamH(fr, nullptr, nullptr);
  return decl;
}

// @brief 範囲指定型 specparam 宣言のヘッダの生成
PtiDeclHead*
Parser::new_SpecParamH(
  const FileRegion& fr,
  const PtExpr* left,
  const PtExpr* right
)
{
  auto decl = mFactory->new_SpecParamH(fr, left, right);
  return decl;
}

// @brief イベント宣言のヘッダの生成
PtiDeclHead*
Parser::new_EventH(
  const FileRegion& fr
)
{
  auto decl = mFactory->new_EventH(fr);
  return decl;
}

// @brief genvar 宣言のヘッダの生成
PtiDeclHead*
Parser::new_GenvarH(
  const FileRegion& fr
)
{
  auto decl = mFactory->new_GenvarH(fr);
  return decl;
}

// @brief 変数宣言のヘッダの生成
PtiDeclHead*
Parser::new_VarH(
  const FileRegion& fr,
  VpiVarType var_type
)
{
  auto decl = mFactory->new_VarH(fr, var_type);
  return decl;
}

// @brief 1ビット型 reg 宣言のヘッダの生成
PtiDeclHead*
Parser::new_RegH(
  const FileRegion& fr,
  bool sign
)
{
  auto decl = mFactory->new_RegH(fr, sign, nullptr, nullptr);
  return decl;
}

// @brief 範囲指定型 reg 宣言のヘッダの生成
PtiDeclHead*
Parser::new_RegH(
  const FileRegion& fr,
  bool sign,
  const PtExpr* left,
  const PtExpr* right
)
{
  auto decl = mFactory->new_RegH(fr, sign, left, right);
  return decl;
}

// @brief 1ビット型 net 宣言のヘッダの生成
PtiDeclHead*
Parser::new_NetH(
  const FileRegion& fr,
  VpiNetType type,
  bool sign
)
{
  auto decl = mFactory->new_NetH(fr, type, VpiVsType::None,
				 sign, nullptr, nullptr,
				 nullptr, nullptr);
  return decl;
}

// @brief 1ビット型 net 宣言のヘッダの生成 (strength あり)
PtiDeclHead*
Parser::new_NetH(
  const FileRegion& fr,
  VpiNetType type,
  bool sign,
  const PtStrength* strength
)
{
  auto decl = mFactory->new_NetH(fr, type, VpiVsType::None,
				 sign, nullptr, nullptr,
				 strength, nullptr);
  return decl;
}

// @brief 1ビット型 net 宣言のヘッダの生成 (遅延あり)
PtiDeclHead*
Parser::new_NetH(
  const FileRegion& fr,
  VpiNetType type,
  bool sign,
  const PtDelay* delay
)
{
  auto decl = mFactory->new_NetH(fr, type, VpiVsType::None,
				 sign, nullptr, nullptr,
				 nullptr, delay);
  return decl;
}

// @brief 1ビット型 net 宣言のヘッダの生成 (strength, 遅延あり)
PtiDeclHead*
Parser::new_NetH(
  const FileRegion& fr,
  VpiNetType type,
  bool sign,
  const PtStrength* strength,
  const PtDelay* delay
)
{
  auto decl = mFactory->new_NetH(fr, type, VpiVsType::None,
				 sign, nullptr, nullptr,
				 strength, delay);
  return decl;
}

// @brief 範囲指定型 net 宣言のヘッダの生成
PtiDeclHead*
Parser::new_NetH(
  const FileRegion& fr,
  VpiNetType type,
  VpiVsType vstype,
  bool sign,
  const PtExpr* left,
  const PtExpr* right
)
{
  auto decl = mFactory->new_NetH(fr, type, vstype,
				 sign, left, right,
				 nullptr, nullptr);
  return decl;
}

// @brief 範囲指定型 net 宣言のヘッダの生成 (strengthあり)
PtiDeclHead*
Parser::new_NetH(
  const FileRegion& fr,
  VpiNetType type,
  VpiVsType vstype,
  bool sign,
  const PtExpr* left,
  const PtExpr* right,
  const PtStrength* strength
)
{
  auto decl = mFactory->new_NetH(fr, type, vstype,
				 sign, left, right,
				 strength, nullptr);
  return decl;
}

// @brief 範囲指定型 net 宣言のヘッダの生成 (遅延あり)
PtiDeclHead*
Parser::new_NetH(
  const FileRegion& fr,
  VpiNetType type,
  VpiVsType vstype,
  bool sign,
  const PtExpr* left,
  const PtExpr* right,
  const PtDelay* delay
)
{
  auto decl = mFactory->new_NetH(fr, type, vstype,
				 sign, left, right,
				 nullptr, delay);
  return decl;
}

// @brief 範囲指定型 net 宣言のヘッダの生成 (strength, 遅延あり)
PtiDeclHead*
Parser::new_NetH(
  const FileRegion& fr,
  VpiNetType type,
  VpiVsType vstype,
  bool sign,
  const PtExpr* left,
  const PtExpr* right,
  const PtStrength* strength,
  const PtDelay* delay
)
{
  auto decl = mFactory->new_NetH(fr, type, vstype,
				 sign, left, right,
				 strength, delay);
  return decl;
}


//////////////////////////////////////////////////////////////////////
// PtDeclItem の生成
//////////////////////////////////////////////////////////////////////

// @brief 宣言要素の生成
void
Parser::new_DeclItem(
  const FileRegion& fr,
  const char* name
)
{
  auto item = mFactory->new_DeclItem(fr, name);
  add_decl_item(item);
}

// @brief 初期値付き宣言要素の生成
void
Parser::new_DeclItem(
  const FileRegion& fr,
  const char* name,
  const PtExpr* init_value
)
{
  auto item = mFactory->new_DeclItem(fr, name, init_value);
  add_decl_item(item);
}

// @brief 配列型宣言要素の生成
void
Parser::new_DeclItem(
  const FileRegion& fr,
  const char* name,
  PtrList<const PtRange>* range_list
)
{
  auto item = mFactory->new_DeclItem(fr, name, range_list->to_vector());
  add_decl_item(item);
}


//////////////////////////////////////////////////////////////////////
// PtRange の生成
//////////////////////////////////////////////////////////////////////

// @brief 範囲の生成
const PtRange*
Parser::new_Range(
  const FileRegion& fr,
  const PtExpr* msb,
  const PtExpr* lsb
)
{
  auto range = mFactory->new_Range(fr, msb, lsb);
  return range;
}

END_NAMESPACE_YM_VERILOG
