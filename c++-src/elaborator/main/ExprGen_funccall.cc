
/// @file ExprGen_funccall.cc
/// @brief ElbMgr の実装ファイル(式の実体化)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.


#include "ExprGen.h"
#include "ElbEnv.h"
#include "ErrorGen.h"

#include "ym/pt/PtModule.h"
#include "ym/pt/PtItem.h"
#include "ym/pt/PtExpr.h"
#include "ym/pt/PtMisc.h"

#include "ym/vl/VlModule.h"
#include "ym/vl/VlIODecl.h"
#include "ym/vl/VlDecl.h"
#include "ym/vl/VlTaskFunc.h"
#include "ym/vl/VlStmt.h"

#include "elaborator/ElbExpr.h"


BEGIN_NONAMESPACE

const int debug = 0;
#define dout cout

END_NONAMESPACE


BEGIN_NAMESPACE_YM_VERILOG

BEGIN_NONAMESPACE

void
put_value_type(ostream& s,
	       const VlValueType& type)
{
  if ( type.is_int_type() ) {
    s << "integer type";
  }
  else if ( type.is_real_type() ) {
    s << "real type";
  }
  else if ( type.is_time_type() ) {
    s << "time type";
  }
  else {
    s << "bitvector type: ";
    if ( type.is_signed() ) {
      s << "signed";
    }
    else {
      s << "unsigned";
    }
    s << " , ";
    if ( type.is_sized() ) {
      s << type.size() << " bits";
    }
    else {
      s << "unsized";
    }
  }
}

END_NONAMESPACE

// @brief PtFuncCall から ElbExpr を生成する．
// @param[in] parent 親のスコープ
// @param[in] env 生成時の環境
// @param[in] pt_expr 式を表すパース木
ElbExpr*
ExprGen::instantiate_funccall(const VlScope* parent,
			      const ElbEnv& env,
			      const PtExpr* pt_expr)
{
  const VlTaskFunc* child_func{nullptr};
  if ( env.is_constant() ) {
    // 定数関数を探し出す．
    if ( pt_expr->namebranch_num() > 0 ) {
      // 階層名は受け付けない．
      ErrorGen::hname_in_ce(__FILE__, __LINE__, pt_expr);
    }

    // 関数名
    auto name{pt_expr->name()};

    // 関数本体を探し出す．
    // constant function はモジュール直下にしかあり得ない
    // <- generated scope 内の関数は constant function ではない．
    auto module{parent->parent_module()};
    auto pt_func{find_funcdef(module, name)};
    if ( !pt_func ) {
      // 関数が見つからなかった．
      ErrorGen::no_such_function(__FILE__, __LINE__, pt_expr);
    }

    if ( pt_func->is_in_use() ) {
      // 自分自身を呼び出している．
      ErrorGen::uses_itself(__FILE__, __LINE__, pt_expr);
    }

    child_func = find_constant_function(module, name);
    if ( child_func == nullptr ) {
      pt_func->set_in_use();
      // なかったので作る．
      child_func = instantiate_constant_function(parent, pt_func);
      pt_func->clear_in_use();
    }
    if ( !child_func ) {
      // 定数関数ではなかった．
      ErrorGen::not_a_constant_function(__FILE__, __LINE__, pt_expr);
    }
  }
  else {
    // 関数本体を探し出す．
    auto handle{mgr().find_obj_up(parent, pt_expr, nullptr)};
    if ( handle == nullptr ) {
      // 関数が見つからなかった．
      ErrorGen::no_such_function(__FILE__, __LINE__, pt_expr);
    }
    if ( handle->type() != VpiObjType::Function ) {
      // 関数ではなかった．
      ErrorGen::not_a_function(__FILE__, __LINE__, pt_expr);
    }
    child_func = handle->taskfunc();
    ASSERT_COND( child_func );
  }

  // 引数の生成
  SizeType n{pt_expr->operand_num()};
  if ( n != child_func->io_num() ) {
    // 引数の数が合わなかった．
    ErrorGen::n_of_arguments_mismatch(__FILE__, __LINE__, pt_expr);
  }

  vector<ElbExpr*> arg_list(n);
  for ( SizeType i = 0; i < n; ++ i ) {
    auto pt_expr1{pt_expr->operand(i)};
    auto expr1{instantiate_expr(parent, env, pt_expr1)};
    auto io_decl{child_func->io(i)};
    auto decl{io_decl->decl()};
    if ( decl->value_type() != expr1->value_type() ) {
      if ( debug ) {
	dout << "decl->value_type() = ";
	put_value_type(dout, decl->value_type());
	dout << endl
	     << "expr1->value_type() = ";
	put_value_type(dout, expr1->value_type());
	dout << endl;
      }
      ErrorGen::illegal_argument_type(__FILE__, __LINE__, pt_expr);
    }
    arg_list[i] = expr1;
  }

  // function call の生成
  auto expr{mgr().new_FuncCall(pt_expr, child_func, arg_list)};

  // attribute instance の生成
  auto attr_list{attribute_list(pt_expr)};
  mgr().reg_attr(expr, attr_list);

  return expr;
}

// PtSysFuncCall から引数を生成する．
// @param[in] parent 親のスコープ
// @param[in] env 生成時の環境
// @param[in] pt_expr 式を表すパース木
ElbExpr*
ExprGen::instantiate_sysfunccall(const VlScope* parent,
				 const ElbEnv& env,
				 const PtExpr* pt_expr)
{
  auto name{pt_expr->name()};

  // system function を探し出す．
  auto user_systf{mgr().find_user_systf(name)};
  if ( user_systf == nullptr ) {
    ErrorGen::no_such_sysfunction(__FILE__, __LINE__, pt_expr);
  }

#warning "TODO: 2011-02-09-04 引数の個数と型のチェック"
  // これは user_systf の仮想関数として実装すべき

  // 引数の生成
  SizeType n{pt_expr->operand_num()};
  vector<ElbExpr*> arg_list(n);
  for ( SizeType i = 0; i < n; ++ i ) {
    auto pt_expr1{pt_expr->operand(i)};
    ElbExpr* arg{nullptr};
    if ( pt_expr ) {
      arg = instantiate_arg(parent, env, pt_expr1);
    }
    else {
      // 関数呼び出しと異なり，空の引数がありうる．
      ;
    }
    arg_list[i] = arg;
  }

  // system function call の生成
  auto expr{mgr().new_SysFuncCall(pt_expr, user_systf, arg_list)};

  return expr;
}

END_NAMESPACE_YM_VERILOG
