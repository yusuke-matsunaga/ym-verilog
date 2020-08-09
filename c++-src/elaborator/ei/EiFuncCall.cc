
/// @file EiFuncCall.cc
/// @brief EiFuncCall の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "EiFactory.h"
#include "EiFuncCall.h"
#include "ym/vl/VlTaskFunc.h"
#include "ym/vl/VlUserSystf.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief 関数呼び出し式を生成する．
// @param[in] pt_expr パース木の定義要素
// @param[in] func 関数
// @param[in] arg_size 引数の数
// @param[in] arg_list 引数のリスト
ElbExpr*
EiFactory::new_FuncCall(const PtExpr* pt_expr,
			const VlTaskFunc* func,
			SizeType arg_size,
			ElbExpr** arg_list)
{
  EiFuncCall* expr = new EiFuncCall(pt_expr, func,
				    arg_size, arg_list);

  return expr;
}

// @brief システム関数呼び出し式を生成する．
// @param[in] pt_expr パース木の定義要素
// @param[in] user_systf システム関数
// @param[in] arg_size 引数の数
// @param[in] arg_list 引数のリスト
ElbExpr*
EiFactory::new_SysFuncCall(const PtExpr* pt_expr,
			   const VlUserSystf* user_systf,
			   SizeType arg_size,
			   ElbExpr** arg_list)
{
  EiSysFuncCall* expr = new EiSysFuncCall(pt_expr, user_systf,
					  arg_size, arg_list);

  return expr;
}


//////////////////////////////////////////////////////////////////////
// クラス EiFcBase
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_expr パース木の定義要素
// @param[in] arg_size 引数の数
// @param[in] arg_list 引数のリスト
EiFcBase::EiFcBase(const PtExpr* pt_expr,
		   SizeType arg_size,
		   ElbExpr** arg_list) :
  EiExprBase(pt_expr),
  mArgNum{arg_size},
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
  return mArgNum;
}

// @brief 引数の取得
// @param[in] pos 位置番号 ( 0 <= pos < argument_num() )
const VlExpr*
EiFcBase::argument(SizeType pos) const
{
  return mArgList[pos];
}

// @brief 要求される式の型を計算してセットする．
// @param[in] type 要求される式の型
// @note 必要であればオペランドに対して再帰的に処理を行なう．
void
EiFcBase::_set_reqsize(const VlValueType& type)
{
  // なにもしない．
}


//////////////////////////////////////////////////////////////////////
// クラス EiFuncCall
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_expr パース木の定義要素
// @param[in] func 関数
// @param[in] arg_size 引数の数
// @param[in] arg_list 引数のリスト
EiFuncCall::EiFuncCall(const PtExpr* pt_expr,
		       const VlTaskFunc* func,
		       SizeType arg_size,
		       ElbExpr** arg_list) :
  EiFcBase(pt_expr, arg_size, arg_list),
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
    SizeType n = argument_num();
    for ( SizeType i = 0; i < n; ++ i ) {
      if ( !argument(i)->is_const() ) {
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
// @note VpiFuncType::Call の時，意味を持つ．
const VlTaskFunc*
EiFuncCall::function() const
{
  return mFunc;
}


//////////////////////////////////////////////////////////////////////
// クラス EiSysFuncCall
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_expr パース木の定義要素
// @param[in] user_systf システム関数
// @param[in] arg_size 引数の数
// @param[in] arg_list 引数のリスト
EiSysFuncCall::EiSysFuncCall(const PtExpr* pt_expr,
			     const VlUserSystf* user_systf,
			     SizeType arg_size,
			     ElbExpr** arg_list) :
  EiFcBase(pt_expr, arg_size, arg_list),
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
// @note このクラスは false を返す．
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
// @note VpiFuncType::SysCall の時，意味を持つ．
const VlUserSystf*
EiSysFuncCall::user_systf() const
{
  return mUserSystf;
}

END_NAMESPACE_YM_VERILOG
