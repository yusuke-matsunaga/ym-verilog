﻿
/// @file EiFuncCall.cc
/// @brief EiFuncCall の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.

#include "ei/EiFactory.h"
#include "ei/EiFuncCall.h"

#include "ym/vl/VlTaskFunc.h"
#include "ym/vl/VlUserSystf.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief 関数呼び出し式を生成する．
ElbExpr*
EiFactory::new_FuncCall(
  const PtExpr* pt_expr,
  const VlTaskFunc* func,
  const vector<ElbExpr*>& arg_list
)
{
  auto expr = new EiFuncCall{pt_expr, func, arg_list};
  return expr;
}

// @brief システム関数呼び出し式を生成する．
ElbExpr*
EiFactory::new_SysFuncCall(
  const PtExpr* pt_expr,
  const VlUserSystf* user_systf,
  const vector<ElbExpr*>& arg_list
)
{
  auto expr = new EiSysFuncCall{pt_expr, user_systf, arg_list};
  return expr;
}


//////////////////////////////////////////////////////////////////////
// クラス EiFcBase
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiFcBase::EiFcBase(
  const PtExpr* pt_expr,
  const vector<ElbExpr*>& arg_list
) : EiExprBase{pt_expr},
    mArgList{arg_list}
{
}

// @brief デストラクタ
EiFcBase::~EiFcBase()
{
}

// @brief 引数の数を返す．
SizeType
EiFcBase::argument_num() const
{
  return mArgList.size();
}

// @brief 引数の取得
const VlExpr*
EiFcBase::argument(
  SizeType pos
) const
{
  ASSERT_COND( 0 <= pos && pos < argument_num() );

  return mArgList[pos];
}

// @brief 引数のリストの取得
vector<const VlExpr*>
EiFcBase::argument_list() const
{
  return vector<const VlExpr*>{mArgList.begin(), mArgList.end()};
}

// @brief 要求される式の型を計算してセットする．
void
EiFcBase::_set_reqsize(
  const VlValueType& type
)
{
  // なにもしない．
}


//////////////////////////////////////////////////////////////////////
// クラス EiFuncCall
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiFuncCall::EiFuncCall(
  const PtExpr* pt_expr,
  const VlTaskFunc* func,
  const vector<ElbExpr*>& arg_list
) : EiFcBase{pt_expr, arg_list},
    mFunc{func}
{
}

// @brief デストラクタ
EiFuncCall::~EiFuncCall()
{
}

// @brief 型の取得
VpiObjType
EiFuncCall::type() const
{
  return VpiObjType::FuncCall;
}

// @brief 式のタイプを返す．
VlValueType
EiFuncCall::value_type() const
{
  switch ( mFunc->func_type() ) {
  case VpiFuncType::Int:
    return VlValueType::int_type();

  case VpiFuncType::Real:
    return VlValueType::real_type();

  case VpiFuncType::Time:
    return VlValueType::time_type();

  case VpiFuncType::Sized:
    return VlValueType(false, true, mFunc->bit_size());

  case VpiFuncType::SizedSigned:
    return VlValueType(true, true, mFunc->bit_size());

  default:
    ASSERT_NOT_REACHED;
  }
  return VlValueType();
}

// @brief 定数の時 true を返す．
bool
EiFuncCall::is_const() const
{
  if ( mFunc->is_constant_function() ) {
    for ( auto arg: mArgList ) {
      if ( !arg->is_const() ) {
	return false;
      }
    }
    return true;
  }
  return false;
}

// @brief 関数呼び出しの時に true を返す．
bool
EiFuncCall::is_funccall() const
{
  return true;
}

// @brief 対象の関数を返す．
const VlTaskFunc*
EiFuncCall::function() const
{
  return mFunc;
}


//////////////////////////////////////////////////////////////////////
// クラス EiSysFuncCall
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiSysFuncCall::EiSysFuncCall(
  const PtExpr* pt_expr,
  const VlUserSystf* user_systf,
  const vector<ElbExpr*>& arg_list
) : EiFcBase{pt_expr, arg_list},
    mUserSystf{user_systf}
{
}

// @brief デストラクタ
EiSysFuncCall::~EiSysFuncCall()
{
}

// @brief 型の取得
VpiObjType
EiSysFuncCall::type() const
{
  return VpiObjType::SysFuncCall;
}

// @brief 式のタイプを返す．
VlValueType
EiSysFuncCall::value_type() const
{
  ASSERT_COND( mUserSystf->system_function() );

  switch ( mUserSystf->function_type() ) {
  case VpiFuncType::Int:
    return VlValueType::int_type();

  case VpiFuncType::Real:
    return VlValueType::real_type();

  case VpiFuncType::Time:
    return VlValueType::time_type();

  case VpiFuncType::Sized:
    return VlValueType(false, true, mUserSystf->size());

  case VpiFuncType::SizedSigned:
    return VlValueType(true, true, mUserSystf->size());

  default:
    ASSERT_NOT_REACHED;
  }
  return VlValueType();
}

// @brief 定数の時 true を返す．
bool
EiSysFuncCall::is_const() const
{
  return false;
}

// @brief システム関数よびあどい時に true を返す．
bool
EiSysFuncCall::is_sysfunccall() const
{
  return true;
}

// @brief 対象のシステム関数を返す．
const VlUserSystf*
EiSysFuncCall::user_systf() const
{
  return mUserSystf;
}

END_NAMESPACE_YM_VERILOG
