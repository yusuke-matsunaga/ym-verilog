
/// @file EiTaskFunc.cc
/// @brief EiTaskFunc の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.

#include "ei/EiFactory.h"
#include "ei/EiTaskFunc.h"
#include "ei/EiIODecl.h"
#include "ei/EiDeclHead.h"

#include "elaborator/ElbDecl.h"

#include "ym/VlTime.h"
#include "ym/vl/VlStmt.h"

#include "ym/BitVector.h"
#include "ym/pt/PtDecl.h"
#include "ym/pt/PtItem.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief function を生成する．
ElbTaskFunc*
EiFactory::new_Function(
  const VlScope* parent,
  const PtItem* pt_item,
  const PtExpr* left,
  const PtExpr* right,
  int left_val,
  int right_val,
  bool const_func
)
{
  ASSERT_COND( left != nullptr && right != nullptr );

  // IO数を数え配列を初期化する．
  SizeType io_num = pt_item->ioitem_num();
  auto func = new EiFunctionV{parent, pt_item, io_num,
			      left, right, left_val, right_val,
			      const_func};
  return func;
}

// @brief function を生成する．
ElbTaskFunc*
EiFactory::new_Function(
  const VlScope* parent,
  const PtItem* pt_item,
  bool const_func
)
{
  // IO数を数え配列を初期化する．
  SizeType io_num = pt_item->ioitem_num();
  auto func = new EiFunction{parent, pt_item, io_num, const_func};
  return func;
}

// @brief task を生成する．
ElbTaskFunc*
EiFactory::new_Task(
  const VlScope* parent,
  const PtItem* pt_item
)
{
  // IO数を数え配列を初期化する．
  SizeType io_num = pt_item->ioitem_num();
  auto task = new EiTask{parent, pt_item, io_num};
  return task;
}


//////////////////////////////////////////////////////////////////////
// クラス EiTaskFunc
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiTaskFunc::EiTaskFunc(
  const VlScope* parent,
  const PtItem* pt_item,
  SizeType io_num
) : mParent{parent},
    mPtItem{pt_item}
{
  mIODeclList.reserve(io_num);
}

// @brief デストラクタ
EiTaskFunc::~EiTaskFunc()
{
}

// @brief ファイル位置の取得
FileRegion
EiTaskFunc::file_region() const
{
  return mPtItem->file_region();
}

// @brief このオブジェクトの属しているスコープを返す．
const VlScope*
EiTaskFunc::parent_scope() const
{
  return mParent;
}

// @brief 名前の取得
string
EiTaskFunc::name() const
{
  return mPtItem->name();
}

// @brief automatic 宣言されていたら true を返す．
bool
EiTaskFunc::automatic() const
{
  return mPtItem->automatic();
}

// @brief 入出力数を得る．
SizeType
EiTaskFunc::io_num() const
{
  return mIODeclList.size();
}

// @brief 入出力を得る．
const VlIODecl*
EiTaskFunc::io(
  SizeType pos
) const
{
  return &mIODeclList[pos];
}

// @brief 本体のステートメントを得る．
const VlStmt*
EiTaskFunc::stmt() const
{
  return mStmt;
}

// @brief 入出力の初期設定を行う．
void
EiTaskFunc::add_iodecl(
  ElbIOHead* head,
  const PtIOItem* pt_item,
  const VlDecl* decl
)
{
  mIODeclList.push_back({head, pt_item, decl});
}

// @brief 本体のステートメントをセットする．
void
EiTaskFunc::set_stmt(
  const VlStmt* stmt
)
{
  mStmt = stmt;
}


//////////////////////////////////////////////////////////////////////
// クラス EiTask
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiTask::EiTask(
  const VlScope* parent,
  const PtItem* pt_item,
  SizeType io_num
) : EiTaskFunc{parent, pt_item, io_num}
{
}

// @brief デストラクタ
EiTask::~EiTask()
{
}

// @brief 型の取得
VpiObjType
EiTask::type() const
{
  return VpiObjType::Task;
}

// @brief function type を返す．
VpiFuncType
EiTask::func_type() const
{
  return VpiFuncType::Int;
}

// @brief 符号付きの時 true を返す．
bool
EiTask::is_signed() const
{
  return false;
}

// @brief 範囲指定を持つとき true を返す．
bool
EiTask::has_range() const
{
  return false;
}

// @brief 範囲の MSB の値を返す．
int
EiTask::left_range_val() const
{
  return 0;
}

// @brief 範囲の LSB の値を返す．
int
EiTask::right_range_val() const
{
  return 0;
}

// @brief 範囲のMSBを表す文字列の取得
string
EiTask::left_range_string() const
{
  return string();
}

// @brief 範囲のLSBを表す文字列の取得
string
EiTask::right_range_string() const
{
  return string();
}

// @brief 出力のビット幅を返す．
SizeType
EiTask::bit_size() const
{
  return 0;
}

// @brief 出力変数をセットする．
void
EiTask::set_ovar(
  ElbDecl* ovar
)
{
  ASSERT_NOT_REACHED;
}

// @brief constant function の時に true を返す．
bool
EiTask::is_constant_function() const
{
  ASSERT_NOT_REACHED;
  return false;
}

// @brief 出力変数を返す．
//
// 出力変数とは関数名と同名の変数
VlDecl*
EiTask::ovar() const
{
  return nullptr;
}


//////////////////////////////////////////////////////////////////////
// クラス EiFunction
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiFunction::EiFunction(
  const VlScope* parent,
  const PtItem* pt_item,
  SizeType io_num,
  bool const_func
) : EiTaskFunc{parent, pt_item, io_num},
    mConstFunc{const_func}
{
}

// @brief デストラクタ
EiFunction::~EiFunction()
{
}

// @brief 型の取得
VpiObjType
EiFunction::type() const
{
  return VpiObjType::Function;
}

// @brief function type を返す．
VpiFuncType
EiFunction::func_type() const
{
  switch ( pt_item()->data_type() ) {
  case VpiVarType::None:
    return VpiFuncType::Sized;

  case VpiVarType::Integer:
    return VpiFuncType::Int;

  case VpiVarType::Real:
    return VpiFuncType::Real;

  case VpiVarType::Time:
    return VpiFuncType::Time;

  case VpiVarType::Realtime:
    return VpiFuncType::Realtime;

  default:
    break;
  }
  ASSERT_NOT_REACHED;
  return VpiFuncType::Int;
}

// @brief 符号付きの時 true を返す．
bool
EiFunction::is_signed() const
{
  return pt_item()->is_signed();
}

// @brief 範囲指定を持つとき true を返す．
bool
EiFunction::has_range() const
{
  return false;
}

// @brief 範囲の MSB の値を返す．
int
EiFunction::left_range_val() const
{
  return 0;
}

// @brief 範囲の LSB の値を返す．
int
EiFunction::right_range_val() const
{
  return 0;
}

// @brief 範囲のMSBを表す文字列の取得
string
EiFunction::left_range_string() const
{
  return string();
}

// @brief 範囲のLSBを表す文字列の取得
string
EiFunction::right_range_string() const
{
  return string();
}

// @brief 出力のビット幅を返す．
SizeType
EiFunction::bit_size() const
{
  switch ( pt_item()->data_type() ) {
  case VpiVarType::None:
    return 1;

  case VpiVarType::Integer:
    return kVpiSizeInteger;

  case VpiVarType::Real:
  case VpiVarType::Realtime:
    return kVpiSizeReal;

  case VpiVarType::Time:
    return kVpiSizeTime;

  default:
    break;
  }
  ASSERT_NOT_REACHED;
  return 0;
}

// @brief 出力変数をセットする．
void
EiFunction::set_ovar(
  ElbDecl* ovar
)
{
  mOvar = ovar;
}

// @brief constant function の時に true を返す．
bool
EiFunction::is_constant_function() const
{
  return mConstFunc;
}

// @brief 出力変数を返す．
//
// 出力変数とは関数名と同名の変数
VlDecl*
EiFunction::ovar() const
{
  return mOvar;
}


//////////////////////////////////////////////////////////////////////
// クラス EiFunctionV
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiFunctionV::EiFunctionV(
  const VlScope* parent,
  const PtItem* pt_item,
  SizeType io_num,
  const PtExpr* left,
  const PtExpr* right,
  int left_val,
  int right_val,
  bool const_func
) : EiFunction{parent, pt_item, io_num, const_func}
{
  mRange.set(left, right, left_val, right_val);
}

// @brief デストラクタ
EiFunctionV::~EiFunctionV()
{
}

// @brief 範囲指定を持つとき true を返す．
bool
EiFunctionV::has_range() const
{
  return true;
}

// @brief 範囲の MSB の値を返す．
int
EiFunctionV::left_range_val() const
{
  return mRange.left_range_val();
}

// @brief 範囲の LSB の値を返す．
int
EiFunctionV::right_range_val() const
{
  return mRange.right_range_val();
}

// @brief 範囲のMSBを表す文字列の取得
string
EiFunctionV::left_range_string() const
{
  return mRange.left_range_string();
}

// @brief 範囲のLSBを表す文字列の取得
string
EiFunctionV::right_range_string() const
{
  return mRange.right_range_string();
}

// @brief 出力のビット幅を返す．
SizeType
EiFunctionV::bit_size() const
{
  return mRange.size();
}

END_NAMESPACE_YM_VERILOG
