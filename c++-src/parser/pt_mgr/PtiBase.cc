
/// @file PtiBase.cc
/// @brief PtiXXX の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "parser/PtiDecl.h"
#include "parser/PtiExpr.h"
#include "ym/pt/PtMisc.h"
#include "ym/pt/PtArray.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス PtiExpr
//////////////////////////////////////////////////////////////////////


BEGIN_NONAMESPACE

string
decompile_impl(const PtExpr* expr,
	       int ppri);

// @brief 演算子の場合の decompile() 処理
string
decompile_opr(const PtExpr* expr,
	      int ppri)
{
  // 各演算子を表す文字列のテーブル
  static const char* sym_table[] = {
    "",               // dummy
    "-",              // vpiMinusOp
    "+",              // vpiPlusOp
    "!",              // vpiNotOp
    "~",              // vpiBitNeg
    "&",              // vpiUnaryAndOp
    "~&",             // vpiUnaryNandOp
    "|",              // vpiUnaryOrOp
    "~|",             // vpiUnaryNorOp
    "^",              // vpiUnaryXorOp
    "~^",             // vpiUnaryXNorOp
    "-",              // vpiSubOp
    "/",              // vpiDivOp
    "%",              // vpiModOp
    "==",             // vpiEqOp
    "!=",             // vpiNeqOp
    "===",            // vpiCaseEqOp
    "!==",            // vpiCaseNeqOp
    ">",              // vpiGtOp
    ">=",             // vpiGeOp
    "<",              // vpiLtOp
    "<=",             // vpiLeOp
    "<<",             // vpiLShiftOp
    ">>",             // vpiRShiftOp
    "+",              // vpiAddOp
    "*",              // vpiMultOp
    "&&",             // vpiLogAndOp
    "||",             // vpiLogOrOp
    "&",              // vpiBitAndOp
    "|",              // vpiBitOrOp
    "^",              // vpiBitXorOp
    "~^",             // vpiBitXNorOp
    "?:",             // vpiConditionOp(dummy)
    "{}",             // vpiConcatOp(dummy)
    "{{}}",           // vpiMultiConcatOp(dummy)
    "or",             // vpiEventOrOp(dummy)
    ";",              // vpiNullOp(dummy)
    ",",              // vpiListOp(dummy)
    ":",              // vpiMinTypMaxOp(dummy)
    "posedge ",       // vpiPosedgeOp
    "negedge ",       // vpiNegedgeOp
    "<<<",            // vpiArithLShiftOp
    ">>>",            // vpiArithRShiftOp
    "**"              // vpiPowerOp
  };

  // 優先順位のテーブル
  static int pri_table[] = {
    0,                // dummy
    13,               // vpiMinusOp
    13,               // vpiPlusOp
    13,               // vpiNotOp
    13,               // vpiBitNegOp
    13,               // vpiUnaryAndOp
    13,               // vpiUnaryNandOp
    13,               // vpiUnaryOrOp
    13,               // vpiUnaryNorOp
    13,               // vpiUnaryXorOp
    13,               // vpiUnaryXNorOp
    10,               // vpiSubOp
    11,               // vpiDivOp
    11,               // vpiModOp
    7,                // vpiEqOp
    7,                // vpiNeqOp
    7,                // vpiCaseEqOp
    7,                // vpiCaseNeqOp
    8,                // vpiGtOp
    8,                // vpiGeOp
    8,                // vpiLtOp
    8,                // vpiLeOp
    9,                // vpiLShiftOp
    9,                // vpiRShiftOp
    10,               // vpiAddOp
    11,               // vpiMultOp
    3,                // vpiLogAndOp
    2,                // vpiLogOrOp
    6,                // vpiBitAndOp
    4,                // vpiBitOrOp
    5,                // vpiBitXorOp
    5,                // vpiBitXNorOp
    1,                // vpiConditionOp
    0,                // vpiConcatOp
    0,                // vpiMultiConcatOp
    0,                // vpiEventOrOp
    0,                // vpiNullOp
    0,                // vpiListOp
    0,                // vpiMinTypMaxOp
    0,                // vpiPosedgeOp
    0,                // vpiNegedgeOp
    9,                // vpiArithLShiftOp
    9,                // vpiArithRShiftOp
    12                // vpiPowerOp
  };

  string ans;

  VpiOpType optype = expr->op_type();
  // parent_optype の優先順位が自分の優先順位よりも高ければ括弧が必要
  bool need_par = false;
  int pri = pri_table[static_cast<int>(optype)];
  if ( ppri > pri ) {
    need_par = true;
    ans += "(";
  }

  switch ( optype ) {
    // 空
  case VpiOpType::Null:
    ans += decompile_impl(expr->operand(0), 0);
    break;

    // 単項演算子
  case VpiOpType::Minus:
  case VpiOpType::Not:
  case VpiOpType::BitNeg:
  case VpiOpType::Plus:
  case VpiOpType::UnaryAnd:
  case VpiOpType::UnaryNand:
  case VpiOpType::UnaryNor:
  case VpiOpType::UnaryOr:
  case VpiOpType::UnaryXNor:
  case VpiOpType::UnaryXor:
  case VpiOpType::Posedge:
  case VpiOpType::Negedge:
    ans += sym_table[static_cast<int>(optype)] + decompile_impl(expr->operand(0), pri);
    break;

    // 二項演算子
  case VpiOpType::Add:
  case VpiOpType::ArithLShift:
  case VpiOpType::ArithRShift:
  case VpiOpType::BitAnd:
  case VpiOpType::BitOr:
  case VpiOpType::BitXNor:
  case VpiOpType::BitXor:
  case VpiOpType::CaseEq:
  case VpiOpType::CaseNeq:
  case VpiOpType::Div:
  case VpiOpType::Eq:
  case VpiOpType::Ge:
  case VpiOpType::Gt:
  case VpiOpType::LShift:
  case VpiOpType::Le:
  case VpiOpType::LogAnd:
  case VpiOpType::LogOr:
  case VpiOpType::Lt:
  case VpiOpType::Mod:
  case VpiOpType::Mult:
  case VpiOpType::Neq:
  case VpiOpType::Power:
  case VpiOpType::RShift:
  case VpiOpType::Sub:
    ans += decompile_impl(expr->operand(0), pri) +
      sym_table[static_cast<int>(optype)] +
      decompile_impl(expr->operand(1), pri);
    break;

    // 三項演算子
  case VpiOpType::Condition:
    ans += decompile_impl(expr->operand(0), pri) + "?" +
      decompile_impl(expr->operand(1), pri) + ":" +
      decompile_impl(expr->operand(2), pri);
    break;

  case VpiOpType::MinTypMax:
    ans += decompile_impl(expr->operand(0), pri) + ":" +
      decompile_impl(expr->operand(1), pri) + ":" +
      decompile_impl(expr->operand(2), pri);
    break;

  case VpiOpType::Concat:
    {
      ans += "{";
      const char* delim = "";
      ymuint n = expr->operand_num();
      for (ymuint i = 0; i < n; ++ i) {
	ans += delim + expr->operand(i)->decompile();
	delim = ",";
      }
      ans += "}";
    }
    break;

  case VpiOpType::MultiConcat:
    {
      ans = "{";
      ans += expr->operand(0)->decompile() + "{";
      const char* delim = "";
      ymuint n = expr->operand_num();
      for (ymuint i = 1; i < n; ++ i) {
	ans += delim + expr->operand(i)->decompile();
	delim = ",";
      }
      ans += "}}";
    }
    break;

  default:
    ASSERT_NOT_REACHED;
  }
  if ( need_par ) {
    ans += ")";
  }
  return ans;
}

// @brief decompile() の下請け関数
// @param[in] ppri 直前の演算子の優先順位
string
decompile_impl(const PtExpr* expr,
	       int ppri)
{
  switch ( expr->type() ) {
  case kPtOprExpr:
    return decompile_opr(expr, ppri);
    break;

  case kPtConstExpr:
    {
      ostringstream buf;
      if ( expr->const_size() > 0 ) {
	buf << expr->const_size() << "'";
      }

      switch ( expr->const_type() ) {
      case VpiConstType::Real:
	buf << expr->const_real();
	return buf.str();

      case VpiConstType::String:
	return expr->const_str();

      case VpiConstType::Int:
	if ( expr->const_str() == nullptr ) {
	  buf << expr->const_uint();
	  return buf.str();
	}
	break;

      case VpiConstType::SignedBinary:
	buf << "s";
      case VpiConstType::Binary:
	buf << "b";
	break;

      case VpiConstType::SignedOct:
	buf << "s";
      case VpiConstType::Oct:
	buf << "b";
	break;

      case VpiConstType::SignedDec:
	buf << "s";
      case VpiConstType::Dec:
	buf << "d";
	break;

      case VpiConstType::SignedHex:
	buf << "s";
      case VpiConstType::Hex:
	buf << "h";
	break;
      }
      buf << expr->const_str();
      return buf.str();
    }

  case kPtFuncCallExpr:
  case kPtSysFuncCallExpr:
    {
      string ans(expr->name());
      ans += "(";
      ymuint n = expr->operand_num();
      const char* comma = "";
      for (ymuint i = 0; i < n; ++ i) {
	ans += comma + expr->operand(i)->decompile();
	comma = ", ";
      }
      ans += ")";
      return ans;
    }

  case kPtPrimaryExpr:
    {
      string ans(expand_full_name(expr->namebranch_array(), expr->name()));
      ymuint n = expr->index_num();
      for (ymuint i = 0; i < n; ++ i) {
	ans += "[" + expr->index(i)->decompile() + "]";
      }
      if ( expr->range_mode() != VpiRangeMode::No ) {
	const char* delim = nullptr;
	switch ( expr->range_mode() ) {
	case VpiRangeMode::Const: delim = ":"; break;
	case VpiRangeMode::Plus:  delim = "+:"; break;
	case VpiRangeMode::Minus: delim = "-:"; break;
	case VpiRangeMode::No:    ASSERT_NOT_REACHED;
	}
	ans += "[" + expr->left_range()->decompile() + delim +
	  expr->right_range()->decompile() + "]";
      }
      return ans;
    }

  default:
    ASSERT_NOT_REACHED;
  }

  return string();
}

END_NONAMESPACE


// @brief 式を表す文字列表現の取得
string
PtiExpr::decompile() const
{
  return decompile_impl(this, 0);
}



//////////////////////////////////////////////////////////////////////
// PtNameBranch 用の関数
// 適当な場所がないのでここに置いている．
//////////////////////////////////////////////////////////////////////

// @brief 階層名を作り出す関数
// @param[in] nb 階層ブランチリストの先頭
// @param[in] name 末尾の名前
// @return 階層名を展開したものを返す．
string
expand_full_name(const PtNameBranchArray& nb_array,
		 const char* name)
{
  ostringstream buf;
  const char* period = "";
  for (ymuint i = 0; i < nb_array.size(); ++ i) {
    const PtNameBranch* nb = nb_array[i];
    buf << period << nb->name();
    if ( nb->has_index() ) {
      buf << "[" << nb->index() << "]";
    }
    period = ".";
  }
  if ( name != nullptr ) {
    buf << period << name;
  }
  buf.flush();
  return buf.str();
}

END_NAMESPACE_YM_VERILOG
