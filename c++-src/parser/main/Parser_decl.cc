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
// @param[in] fr ファイル位置の情報
// @param[in] dir IO の種類
// @param[in] sign 符号付きのとき true となるフラグ
// @return 生成された IO宣言ヘッダ
PtiIOHead*
Parser::new_IOHead(const FileRegion& fr,
		   VpiDir dir,
		   bool sign)
{
  auto decl{mFactory.new_IOHead(fr, dir, sign)};
  return decl;
}

// @brief IO 宣言のヘッダの生成 (reg 型)
// @param[in] fr ファイル位置の情報
// @param[in] dir IO の種類
// @param[in] sign 符号付きのとき true となるフラグ
// @return 生成された IO宣言ヘッダ
PtiIOHead*
Parser::new_RegIOHead(const FileRegion& fr,
		      VpiDir dir,
		      bool sign)
{
  auto decl{mFactory.new_RegIOHead(fr, dir, sign)};
  return decl;
}

// @brief IO 宣言のヘッダの生成 (ネット型)
// @param[in] fr ファイル位置の情報
// @param[in] dir IO の種類
// @param[in] net_type 補助的なネット型
// @param[in] sign 符号付きのとき true となるフラグ
// @return 生成された IO宣言ヘッダ
PtiIOHead*
Parser::new_NetIOHead(const FileRegion& fr,
		      VpiDir dir,
		      VpiNetType net_type,
		      bool sign)
{
  auto decl{mFactory.new_NetIOHead(fr, dir, net_type, sign)};
  return decl;
}

// @brief IO 宣言のヘッダの生成 (変数型)
// @param[in] fr ファイル位置の情報
// @param[in] dir IO の種類
// @param[in] var_type 補助的な変数型
// @return 生成された IO宣言ヘッダ
PtiIOHead*
Parser::new_VarIOHead(const FileRegion& fr,
		      VpiDir dir,
		      VpiVarType var_type)
{
  auto decl{mFactory.new_VarIOHead(fr, dir, var_type)};
  return decl;
}

// @brief 範囲付きの IO 宣言のヘッダの生成
// @param[in] fr ファイル位置の情報
// @param[in] dir IO の種類
// @param[in] sign 符号付きのとき true となるフラグ
// @param[in] left 範囲の左側の式
// @param[in] right 範囲の右側の式
// @return 生成された IO宣言ヘッダ
PtiIOHead*
Parser::new_IOHead(const FileRegion& fr,
		   VpiDir dir,
		   bool sign,
		   const PtExpr* left,
		   const PtExpr* right)
{
  auto decl{mFactory.new_IOHead(fr, dir, sign, left, right)};
  return decl;
}

// @brief 範囲付きの IO 宣言のヘッダの生成 (reg 型)
// @param[in] fr ファイル位置の情報
// @param[in] dir IO の種類
// @param[in] sign 符号付きのとき true となるフラグ
// @param[in] left 範囲の左側の式
// @param[in] right 範囲の右側の式
// @return 生成された IO宣言ヘッダ
PtiIOHead*
Parser::new_RegIOHead(const FileRegion& fr,
		      VpiDir dir,
		      bool sign,
		      const PtExpr* left,
		      const PtExpr* right)
{
  auto decl{mFactory.new_RegIOHead(fr, dir, sign, left, right)};
  return decl;
}

// @brief 範囲付きの IO 宣言のヘッダの生成 (ネット型)
// @param[in] fr ファイル位置の情報
// @param[in] dir IO の種類
// @param[in] net_type 補助的なネット型
// @param[in] sign 符号付きのとき true となるフラグ
// @param[in] left 範囲の左側の式
// @param[in] right 範囲の右側の式
// @return 生成された IO宣言ヘッダ
PtiIOHead*
Parser::new_NetIOHead(const FileRegion& fr,
		      VpiDir dir,
		      VpiNetType net_type,
		      bool sign,
		      const PtExpr* left,
		      const PtExpr* right)
{
  auto decl{mFactory.new_NetIOHead(fr, dir, net_type, sign, left, right)};
  return decl;
}


//////////////////////////////////////////////////////////////////////
// PtIOItem の生成
//////////////////////////////////////////////////////////////////////

// @brief IO 宣言の要素の生成
// @param[in] fr ファイル位置の情報
// @param[in] name 要素名
void
Parser::new_IOItem(const FileRegion& fr,
		   const char* name)
{
  auto item{mFactory.new_IOItem(fr, name)};
  add_io_item(item);
}

// @brief 初期値付き IO 宣言の要素の生成
// @param[in] fr ファイル位置の情報
// @param[in] name 要素名
// @param[in] init_value 初期値を表す式
void
Parser::new_IOItem(const FileRegion& fr,
		   const char* name,
		   const PtExpr* init_value)
{
  auto item{mFactory.new_IOItem(fr, name, init_value)};
  add_io_item(item);
}


//////////////////////////////////////////////////////////////////////
// PtDeclHead ( parameter ) の生成
//////////////////////////////////////////////////////////////////////

// @brief パラメータ宣言のヘッダの生成 (型指定なし)
// @param[in] fr ファイル位置の情報
PtiDeclHead*
Parser::new_ParamH(const FileRegion& fr)
{
  auto decl{mFactory.new_ParamH(fr)};
  return decl;
}

// @brief 範囲指定型パラメータ宣言のヘッダの生成
// @param[in] fr ファイル位置の情報
// @param[in] sign 符号付きのとき true となるフラグ
// @param[in] left 範囲の左側の式
// @param[in] right 範囲の右側の式
PtiDeclHead*
Parser::new_ParamH(const FileRegion& fr,
		   bool sign,
		   const PtExpr* left,
		   const PtExpr* right)
{
  auto decl{mFactory.new_ParamH(fr, sign, left, right)};
  return decl;
}

// @brief 組み込み型パラメータ宣言のヘッダの生成
// @param[in] fr ファイル位置の情報
// @param[in] var_type データ型
PtiDeclHead*
Parser::new_ParamH(const FileRegion& fr,
		   VpiVarType var_type)
{
  auto decl{mFactory.new_ParamH(fr, var_type)};
  return decl;
}


//////////////////////////////////////////////////////////////////////
// PtDeclHead ( localparam ) の生成
//////////////////////////////////////////////////////////////////////

// @brief local param 宣言のヘッダの生成 (型指定なし)
// @param[in] fr ファイル位置の情報
PtiDeclHead*
Parser::new_LocalParamH(const FileRegion& fr)
{
  auto decl{mFactory.new_LocalParamH(fr)};
  return decl;
}

// @brief 範囲指定型 local param 宣言のヘッダの生成
// @param[in] fr ファイル位置の情報
// @param[in] sign 符号付きのとき true となるフラグ
// @param[in] left 範囲の左側の式
// @param[in] right 範囲の右側の式
PtiDeclHead*
Parser::new_LocalParamH(const FileRegion& fr,
			bool sign,
			const PtExpr* left,
			const PtExpr* right)
{
  auto decl{mFactory.new_LocalParamH(fr, sign, left, right)};
  return decl;
}

// @brief 組み込み型パラメータ宣言のヘッダの生成
// @param[in] fr ファイル位置の情報
// @param[in] var_type データ型
PtiDeclHead*
Parser::new_LocalParamH(const FileRegion& fr,
			VpiVarType var_type)
{
  auto decl{mFactory.new_LocalParamH(fr, var_type)};
  return decl;
}


//////////////////////////////////////////////////////////////////////
// PtDeclHead の生成
//////////////////////////////////////////////////////////////////////

// @brief specparam 宣言のヘッダの生成
// @param[in] fr ファイル位置の情報
// @return 生成された specparam
PtiDeclHead*
Parser::new_SpecParamH(const FileRegion& fr)
{
  auto decl{mFactory.new_SpecParamH(fr)};
  return decl;
}

// @brief 範囲指定型 specparam 宣言のヘッダの生成
// @param[in] fr ファイル位置の情報
// @param[in] left 範囲の左側の式
// @param[in] right 範囲の右側の式
PtiDeclHead*
Parser::new_SpecParamH(const FileRegion& fr,
		       const PtExpr* left,
		       const PtExpr* right)
{
  auto decl{mFactory.new_SpecParamH(fr, left, right)};
  return decl;
}

// @brief イベント宣言のヘッダの生成
// @param[in] fr ファイル位置の情報
// @return 生成されたイベント
PtiDeclHead*
Parser::new_EventH(const FileRegion& fr)
{
  auto decl{mFactory.new_EventH(fr)};
  return decl;
}

// @brief genvar 宣言のヘッダの生成
// @param[in] fr ファイル位置の情報
PtiDeclHead*
Parser::new_GenvarH(const FileRegion& fr)
{
  auto decl{mFactory.new_GenvarH(fr)};
  return decl;
}

// @brief 変数宣言のヘッダの生成
// @param[in] fr ファイル位置の情報
// @param[in] var_type データ型
PtiDeclHead*
Parser::new_VarH(const FileRegion& fr,
		 VpiVarType var_type)
{
  auto decl{mFactory.new_VarH(fr, var_type)};
  return decl;
}

// @brief 1ビット型 reg 宣言のヘッダの生成
// @param[in] fr ファイル位置の情報
// @param[in] sign 符号付きの時 true となるフラグ
PtiDeclHead*
Parser::new_RegH(const FileRegion& fr,
		 bool sign)
{
  auto decl{mFactory.new_RegH(fr, sign)};
  return decl;
}

// @brief 範囲指定型 reg 宣言のヘッダの生成
// @param[in] fr ファイル位置の情報
// @param[in] sign 符号付きの時 true となるフラグ
// @param[in] left 範囲の左側の式
// @param[in] right 範囲の右側の式
PtiDeclHead*
Parser::new_RegH(const FileRegion& fr,
		 bool sign,
		 const PtExpr* left,
		 const PtExpr* right)
{
  auto decl{mFactory.new_RegH(fr, sign, left, right)};
  return decl;
}

// @brief 1ビット型 net 宣言のヘッダの生成
// @param[in] fr ファイル位置の情報
// @param[in] type net の型
// @param[in] sign 符号の有無を表すフラグ
PtiDeclHead*
Parser::new_NetH(const FileRegion& fr,
		 VpiNetType type,
		 bool sign)
{
  auto decl{mFactory.new_NetH(fr, type, sign)};
  return decl;
}

// @brief 1ビット型 net 宣言のヘッダの生成 (strength あり)
// @param[in] fr ファイル位置の情報
// @param[in] type net の型
// @param[in] sign 符号の有無を表すフラグ
// @param[in] strength 信号強度
PtiDeclHead*
Parser::new_NetH(const FileRegion& fr,
		 VpiNetType type,
		 bool sign,
		 const PtStrength* strength)
{
  auto decl{mFactory.new_NetH(fr, type, sign, strength)};
  return decl;
}

// @brief 1ビット型 net 宣言のヘッダの生成 (遅延あり)
// @param[in] fr ファイル位置の情報
// @param[in] type net の型
// @param[in] sign 符号の有無を表すフラグ
// @param[in] delay 遅延
PtiDeclHead*
Parser::new_NetH(const FileRegion& fr,
		 VpiNetType type,
		 bool sign,
		 const PtDelay* delay)
{
  auto decl{mFactory.new_NetH(fr, type, sign, delay)};
  return decl;
}

// @brief 1ビット型 net 宣言のヘッダの生成 (strength, 遅延あり)
// @param[in] fr ファイル位置の情報
// @param[in] type net の型
// @param[in] sign 符号の有無を表すフラグ
// @param[in] strength 信号強度
// @param[in] delay 遅延
PtiDeclHead*
Parser::new_NetH(const FileRegion& fr,
		 VpiNetType type,
		 bool sign,
		 const PtStrength* strength,
		 const PtDelay* delay)
{
  auto decl{mFactory.new_NetH(fr, type, sign, strength, delay)};
  return decl;
}

// @brief 範囲指定型 net 宣言のヘッダの生成
// @param[in] fr ファイル位置の情報
// @param[in] type net の型
// @param[in] vstype vector/scalar 指定
// @param[in] sign 符号の有無を表すフラグ
// @param[in] left 範囲の左側の式
// @param[in] right 範囲の右側の式
PtiDeclHead*
Parser::new_NetH(const FileRegion& fr,
		 VpiNetType type,
		 VpiVsType vstype,
		 bool sign,
		 const PtExpr* left,
		 const PtExpr* right)
{
  auto decl{mFactory.new_NetH(fr, type, vstype, sign, left, right)};
  return decl;
}

// @brief 範囲指定型 net 宣言のヘッダの生成 (strengthあり)
// @param[in] fr ファイル位置の情報
// @param[in] type net の型
// @param[in] vstype vector/scalar 指定
// @param[in] sign 符号の有無を表すフラグ
// @param[in] left 範囲の左側の式
// @param[in] right 範囲の右側の式
// @param[in] strength 信号強度
PtiDeclHead*
Parser::new_NetH(const FileRegion& fr,
		 VpiNetType type,
		 VpiVsType vstype,
		 bool sign,
		 const PtExpr* left,
		 const PtExpr* right,
		 const PtStrength* strength)
{
  auto decl{mFactory.new_NetH(fr, type, vstype, sign, left, right, strength)};
  return decl;
}

// @brief 範囲指定型 net 宣言のヘッダの生成 (遅延あり)
// @param[in] fr ファイル位置の情報
// @param[in] type net の型
// @param[in] vstype vector/scalar 指定
// @param[in] sign 符号の有無を表すフラグ
// @param[in] left 範囲の左側の式
// @param[in] right 範囲の右側の式
// @param[in] delay 遅延
PtiDeclHead*
Parser::new_NetH(const FileRegion& fr,
		 VpiNetType type,
		 VpiVsType vstype,
		 bool sign,
		 const PtExpr* left,
		 const PtExpr* right,
		 const PtDelay* delay)
{
  auto decl{mFactory.new_NetH(fr, type, vstype, sign, left, right, delay)};
  return decl;
}

// @brief 範囲指定型 net 宣言のヘッダの生成 (strength, 遅延あり)
// @param[in] fr ファイル位置の情報
// @param[in] type net の型
// @param[in] vstype vector/scalar 指定
// @param[in] sign 符号の有無を表すフラグ
// @param[in] left 範囲の左側の式
// @param[in] right 範囲の右側の式
// @param[in] strength 信号強度
// @param[in] delay 遅延
PtiDeclHead*
Parser::new_NetH(const FileRegion& fr,
		 VpiNetType type,
		 VpiVsType vstype,
		 bool sign,
		 const PtExpr* left,
		 const PtExpr* right,
		 const PtStrength* strength,
		 const PtDelay* delay)
{
  auto decl{mFactory.new_NetH(fr, type, vstype, sign, left, right, strength, delay)};
  return decl;
}


//////////////////////////////////////////////////////////////////////
// PtDeclItem の生成
//////////////////////////////////////////////////////////////////////

// @brief 宣言要素の生成
// @param[in] fr ファイル位置の情報
// @param[in] name 名前
void
Parser::new_DeclItem(const FileRegion& fr,
		     const char* name)
{
  auto item{mFactory.new_DeclItem(fr, name)};
  add_decl_item(item);
}

// @brief 初期値付き宣言要素の生成
// @param[in] fr ファイル位置の情報
// @param[in] name 名前
// @param[in] init_value 初期値を表す式
void
Parser::new_DeclItem(const FileRegion& fr,
		     const char* name,
		     const PtExpr* init_value)
{
  auto item{mFactory.new_DeclItem(fr, name, init_value)};
  add_decl_item(item);
}

// @brief 配列型宣言要素の生成
// @param[in] fr ファイル位置の情報
// @param[in] name 名前
// @param[in] range_array 配列の各次元の範囲のリスト
void
Parser::new_DeclItem(const FileRegion& fr,
		     const char* name,
		     PtrList<const PtRange>* range_list)
{
  auto item{mFactory.new_DeclItem(fr, name, range_list->to_vector())};
  add_decl_item(item);
}


//////////////////////////////////////////////////////////////////////
// PtRange の生成
//////////////////////////////////////////////////////////////////////

// @brief 範囲の生成
// @param[in] msb MSB を表す式
// @param[in] lsb LSB を表す式
const PtRange*
Parser::new_Range(const FileRegion& fr,
		  const PtExpr* msb,
		  const PtExpr* lsb)
{
  auto range{mFactory.new_Range(fr, msb, lsb)};
  return range;
}

END_NAMESPACE_YM_VERILOG
