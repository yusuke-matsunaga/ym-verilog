﻿/// @file Parser_expr.cc
/// @brief Parser の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.


#include "parser/Parser.h"
#include "parser/PtiFactory.h"
#include "ym/pt/PtExpr.h"


BEGIN_NAMESPACE_YM_VERILOG

// @brief 単項演算子の生成
// @param[in] fr ファイル位置の情報
// @param[in] type 演算の種類
// @param[in] opr オペランド
// @return 生成された演算子
const PtExpr*
Parser::new_Opr(const FileRegion& fr,
		VpiOpType type,
		const PtExpr* opr,
		PtrList<const PtAttrInst>* ai_list)
{
  auto expr{mFactory->new_Opr(fr, type, opr)};
  reg_attrinst(expr, ai_list);
  return expr;
}

// @brief 二項演算子の生成
// @param[in] fr ファイル位置の情報
// @param[in] type 演算の種類
// @param[in] opr1 オペランド1
// @param[in] opr2 オペランド2
// @return 生成された演算子
const PtExpr*
Parser::new_Opr(const FileRegion& fr,
		VpiOpType type,
		const PtExpr* opr1,
		const PtExpr* opr2,
		PtrList<const PtAttrInst>* ai_list)
{
  auto expr{mFactory->new_Opr(fr, type, opr1, opr2)};
  reg_attrinst(expr, ai_list);
  return expr;
}

// @brief 三項演算子の生成
// @param[in] fr ファイル位置の情報
// @param[in] type 演算の種類
// @param[in] opr1 オペランド1
// @param[in] opr2 オペランド2
// @param[in] opr3 オペランド3
// @return 生成された演算子
const PtExpr*
Parser::new_Opr(const FileRegion& fr,
		VpiOpType type,
		const PtExpr* opr1,
		const PtExpr* opr2,
		const PtExpr* opr3,
		PtrList<const PtAttrInst>* ai_list)
{
  auto expr{mFactory->new_Opr(fr, type, opr1, opr2, opr3)};
  reg_attrinst(expr, ai_list);
  return expr;
}

// @brief concatination 演算子の生成
// @param[in] fr ファイル位置の情報
// @param[in] expr_list オペランドのリスト
// @return 生成された concatination 演算子
const PtExpr*
Parser::new_Concat(const FileRegion& fr,
		   PtrList<const PtExpr>* expr_list)
{
  auto expr{mFactory->new_Concat(fr, expr_list->to_vector())};
  return expr;
}

// @brief multi-concatination 演算子の生成
// @param[in] fr ファイル位置の情報
// @param[in] rep 繰り返し数
// @param[in] expr_list 結合するオペランドのリスト
// @return 生成された multi-concatination 演算子
const PtExpr*
Parser::new_MultiConcat(const FileRegion& fr,
			const PtExpr* rep,
			PtrList<const PtExpr>* expr_list)
{
  expr_list->push_front(rep);
  auto expr{mFactory->new_MultiConcat(fr, expr_list->to_vector())};
  return expr;
}

// @brief min/typ/max delay 演算子の生成
// @param[in] fr ファイル位置の情報
// @param[in] val0 minimum 値
// @param[in] val1 typical 値
// @param[in] val2 maximum 値
// @return 生成された min/typ/max 演算子
const PtExpr*
Parser::new_MinTypMax(const FileRegion& fr,
		      const PtExpr* val0,
		      const PtExpr* val1,
		      const PtExpr* val2)
{
  auto expr{mFactory->new_MinTypMax(fr, val0, val1, val2)};
  return expr;
}

// @brief primary の生成
// @param[in] fr ファイル位置の情報
// @param[in] name 識別子名
// @return 生成された primary
const PtExpr*
Parser::new_Primary(const FileRegion& fr,
		    const char* name)
{
  auto expr{mFactory->new_Primary(fr, name)};
  return expr;
}

// @brief インデックス付き primary の生成
// @param[in] fr ファイル位置の情報
// @param[in] name 識別子名
// @param[in] index_list インデックスのリスト
// @return 生成された primary
const PtExpr*
Parser::new_Primary(const FileRegion& fr,
		    const char* name,
		    PtrList<const PtExpr>* index_array)
{
  auto expr{mFactory->new_Primary(fr, name, index_array->to_vector())};
  return expr;
}

// @brief 範囲指定付き primary の生成
// @param[in] fr ファイル位置の情報
// @param[in] name 識別子名
// @param[in] mode 範囲指定のモード
// @param[in] left 範囲の左側の式
// @param[in] right 範囲の右側の式
// @return 生成された primary
const PtExpr*
Parser::new_Primary(const FileRegion& fr,
		    const char* name,
		    VpiRangeMode mode,
		    const PtExpr* left,
		    const PtExpr* right)
{
  auto expr{mFactory->new_Primary(fr, name, mode, left, right)};
  return expr;
}

// @brief インデックスと範囲指定付き primary の生成
// @param[in] fr ファイル位置の情報
// @param[in] name 識別子名
// @param[in] index_list インデックスのリスト
// @param[in] mode 範囲指定のモード
// @param[in] left 範囲の左側の式
// @param[in] right 範囲の右側の式
// @return 生成された primary
const PtExpr*
Parser::new_Primary(const FileRegion& fr,
		    const char* name,
		    PtrList<const PtExpr>* index_list,
		    VpiRangeMode mode,
		    const PtExpr* left,
		    const PtExpr* right)
{
  auto expr{mFactory->new_Primary(fr, name, index_list->to_vector(),
				 mode, left, right)};
  return expr;
}

// @brief primary の生成 (階層付き)
// @param[in] fr ファイル位置の情報
// @param[in] hname 階層名
// @return 生成された primary
const PtExpr*
Parser::new_Primary(const FileRegion& fr,
		    PuHierName* hname)
{
  auto expr{mFactory->new_Primary(fr, hname)};
  return expr;
}

// @brief インデックス付き primary の生成 (階層付き)
// @param[in] fr ファイル位置の情報
// @param[in] hname 階層名
// @param[in] index_list インデックスのリスト
// @return 生成された primary
const PtExpr*
Parser::new_Primary(const FileRegion& fr,
		    PuHierName* hname,
		    PtrList<const PtExpr>* index_list)
{
  auto expr{mFactory->new_Primary(fr, hname, index_list->to_vector())};
  return expr;
}

// @brief 範囲指定付き primary の生成 (階層付き)
// @param[in] fr ファイル位置の情報
// @param[in] hname 階層名
// @param[in] mode 範囲指定のモード
// @param[in] left 範囲の左側の式
// @param[in] right 範囲の右側の式
// @return 生成された primary
const PtExpr*
Parser::new_Primary(const FileRegion& fr,
		    PuHierName* hname,
		    VpiRangeMode mode,
		    const PtExpr* left,
		    const PtExpr* right)
{
  auto expr{mFactory->new_Primary(fr, hname, mode, left, right)};
  return expr;
}

// @brief インデックスと範囲指定付き primary の生成 (階層付き)
// @param[in] fr ファイル位置の情報
// @param[in] hname 階層名
// @param[in] index_list インデックスのリスト
// @param[in] mode 範囲指定のモード
// @param[in] left 範囲の左側の式
// @param[in] right 範囲の右側の式
// @return 生成された primary
const PtExpr*
Parser::new_Primary(const FileRegion& fr,
		    PuHierName* hname,
		    PtrList<const PtExpr>* index_list,
		    VpiRangeMode mode,
		    const PtExpr* left,
		    const PtExpr* right)
{
  auto expr{mFactory->new_Primary(fr, hname, index_list->to_vector(),
				 mode, left, right)};
  return expr;
}

// @brief constant primary の生成
// @param[in] fr ファイル位置の情報
// @param[in] name 識別子名
// @param[in] index インデックス
// @return 生成された const primary
const PtExpr*
Parser::new_CPrimary(const FileRegion& fr,
		     const char* name,
		     const PtExpr* index)
{
  auto expr{mFactory->new_CPrimary(fr, name, index)};
  return expr;
}

// @brief constant primary の生成
// @param[in] fr ファイル位置の情報
// @param[in] name 識別子名
// @param[in] index_list インデックスのリスト
// @return 生成された const primary
const PtExpr*
Parser::new_CPrimary(const FileRegion& fr,
		     const char* name,
		     PtrList<const PtExpr>* index_list)
{
  auto expr{mFactory->new_CPrimary(fr, name, index_list->to_vector())};
  return expr;
}

// @brief 範囲指定付き constant primary の生成
// @param[in] fr ファイル位置の情報
// @param[in] name 識別子名
// @param[in] mode 範囲指定のモード
// @param[in] left 範囲の左側の式
// @param[in] right 範囲の右側の式
// @return 生成された constant primary
const PtExpr*
Parser::new_CPrimary(const FileRegion& fr,
		     const char* name,
		     VpiRangeMode mode,
		     const PtExpr* left,
		     const PtExpr* right)
{
  auto expr{mFactory->new_CPrimary(fr, name, mode, left, right)};
  return expr;
}

// @brief インデックス付き constant primary の生成 (階層付き)
// @param[in] fr ファイル位置の情報
// @param[in] hname 階層名
// @param[in] index_list インデックスのリスト
// @return 生成された constant primary
const PtExpr*
Parser::new_CPrimary(const FileRegion& fr,
		     PuHierName* hname,
		     PtrList<const PtExpr>* index_list)
{
  auto expr{mFactory->new_CPrimary(fr, hname, index_list->to_vector())};
  return expr;
}

// @brief function call の生成
// @param[in] fr ファイル位置の情報
// @param[in] name 関数名
// @param[in] arg_list 引数のリスト
// @return 生成された function call
const PtExpr*
Parser::new_FuncCall(const FileRegion& fr,
		     const char* name,
		     PtrList<const PtExpr>* arg_list,
		     PtrList<const PtAttrInst>* ai_list)
{
  auto expr{mFactory->new_FuncCall(fr, name, arg_list->to_vector())};
  reg_attrinst(expr, ai_list);
  return expr;
}

// @brief function call の生成 (階層付き)
// @param[in] fr ファイル位置の情報
// @param[in] hname 階層名
// @param[in] arg_list 引数のリスト
// @return 生成された function call
const PtExpr*
Parser::new_FuncCall(const FileRegion& fr,
		     PuHierName* hname,
		     PtrList<const PtExpr>* arg_list,
		     PtrList<const PtAttrInst>* ai_list)
{
  auto expr{mFactory->new_FuncCall(fr, hname, arg_list->to_vector())};
  reg_attrinst(expr, ai_list);
  return expr;
}

// @brief system function call の生成
// @param[in] fr ファイル位置の情報
// @param[in] name 関数名
// @param[in] arg_array 引数のリスト
// @return 生成された function call
const PtExpr*
Parser::new_SysFuncCall(const FileRegion& fr,
			const char* name,
			PtrList<const PtExpr>* arg_list)
{
  auto expr{mFactory->new_SysFuncCall(fr, name, arg_list->to_vector())};
  return expr;
}

// @brief 整数型の定数の生成
// @param[in] fr ファイル位置の情報
// @param[in] value 定数を表す整数値
// @return 生成された定数
const PtExpr*
Parser::new_IntConst(const FileRegion& fr,
		     unsigned int value)
{
  auto expr{mFactory->new_IntConst(fr, value)};
  return expr;
}

// @brief 整数型の定数の生成
// @param[in] fr ファイル位置の情報
// @param[in] value 定数の文字列表現
// @return 生成された定数
const PtExpr*
Parser::new_IntConst(const FileRegion& fr,
		     const char* value)
{
  auto expr{mFactory->new_IntConst(fr, value)};
  return expr;
}

// @brief 基底付き整数型の定数の生成
// @param[in] fr ファイル位置の情報
// @param[in] const_type 定数の種類
// @param[in] value 定数の文字列表現
// @return 生成された定数
const PtExpr*
Parser::new_IntConst(const FileRegion& fr,
		     VpiConstType const_type,
		     const char* value)
{
  auto expr{mFactory->new_IntConst(fr, const_type, value)};
  return expr;
}

// @brief サイズと基底付き定数の生成
// @param[in] fr ファイル位置の情報
// @param[in] size サイズ
// @param[in] const_type 定数の種類
// @param[in] value 定数の文字列表現
// @return 生成された定数
const PtExpr*
Parser::new_IntConst(const FileRegion& fr,
		     SizeType size,
		     VpiConstType const_type,
		     const char* value)
{
  auto expr{mFactory->new_IntConst(fr, size, const_type, value)};
  return expr;
}

// @brief 実数型の定数の生成
// @param[in] fr ファイル位置の情報
// @param[in] value 定数の文字列表現
// @return 生成された定数
const PtExpr*
Parser::new_RealConst(const FileRegion& fr,
		      double value)
{
  auto expr{mFactory->new_RealConst(fr, value)};
  return expr;
}

// @brief 文字列型の定数の生成
// @param[in] fr ファイル位置の情報
// @param[in] value 文字列
// @return 生成された定数
const PtExpr*
Parser::new_StringConst(const FileRegion& fr,
			const char* value)
{
  auto expr{mFactory->new_StringConst(fr, value)};
  return expr;
}

END_NAMESPACE_YM_VERILOG
