﻿
/// @file EiTaskFunc.cc
/// @brief EiTaskFunc の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "EiFactory.h"
#include "EiTaskFunc.h"
#include "EiIODecl.h"
#include "ElbStmt.h"
#include "ElbDecl.h"

#include "ym/VlTime.h"
#include "ym/BitVector.h"
#include "ym/pt/PtDecl.h"
#include "ym/pt/PtItem.h"



BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief function を生成する．
// @param[in] parent 親のスコープ
// @param[in] pt_item パース木の定義
// @param[in] left 範囲の MSB の式
// @param[in] right 範囲の LSB の式
// @param[in] left_val 範囲の MSB の値
// @param[in] right_val 範囲の LSB の値
ElbTaskFunc*
EiFactory::new_Function(const VlNamedObj* parent,
			const PtItem* pt_item,
			const PtExpr* left,
			const PtExpr* right,
			int left_val,
			int right_val)
{
  ASSERT_COND( left != nullptr && right != nullptr );

  // IO数を数え配列を初期化する．
  int io_num = pt_item->ioitem_num();
  void* q = mAlloc.get_memory(sizeof(EiIODecl) * io_num);
  EiIODecl* io_array = new (q) EiIODecl[io_num];

  void* p = mAlloc.get_memory(sizeof(EiFunctionV));
  EiFunction* func = new (p) EiFunctionV(parent, pt_item, io_num, io_array,
					 left, right, left_val, right_val);

  return func;
}

// @brief function を生成する．
// @param[in] parent 親のスコープ
// @param[in] pt_item パース木の定義
ElbTaskFunc*
EiFactory::new_Function(const VlNamedObj* parent,
			const PtItem* pt_item)
{
  // IO数を数え配列を初期化する．
  int io_num = pt_item->ioitem_num();
  void* q = mAlloc.get_memory(sizeof(EiIODecl) * io_num);
  EiIODecl* io_array = new (q) EiIODecl[io_num];

  void* p = mAlloc.get_memory(sizeof(EiFunction));
  EiFunction* func = new (p) EiFunction(parent, pt_item, io_num, io_array);

  return func;
}

// @brief task を生成する．
// @param[in] parent 親のスコープ
// @param[in] pt_item パース木の定義
ElbTaskFunc*
EiFactory::new_Task(const VlNamedObj* parent,
		    const PtItem* pt_item)
{
  // IO数を数え配列を初期化する．
  int io_num = pt_item->ioitem_num();
  void* q = mAlloc.get_memory(sizeof(EiIODecl) * io_num);
  EiIODecl* io_array = new (q) EiIODecl[io_num];

  void* p = mAlloc.get_memory(sizeof(EiTask));
  EiTask* task = new (p) EiTask(parent, pt_item, io_num, io_array);
  return task;
}


//////////////////////////////////////////////////////////////////////
// クラス EiTaskFunc
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のスコープ環境
// @param[in] pt_item パース木の定義
// @param[in] io_num IOの数
// @param[in] io_array IO の配列
EiTaskFunc::EiTaskFunc(const VlNamedObj* parent,
		       const PtItem* pt_item,
		       int io_num,
		       EiIODecl* io_array):
  mParent(parent),
  mPtItem(pt_item),
  mIODeclNum(io_num),
  mIODeclList(io_array),
  mStmt(nullptr)
{
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
const VlNamedObj*
EiTaskFunc::parent() const
{
  return mParent;
}

// @brief 名前の取得
const char*
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
int
EiTaskFunc::io_num() const
{
  return mIODeclNum;
}

// @brief 入出力を得る．
// @param[in] pos 位置番号 ( 0 <= pos < io_num() )
const VlIODecl*
EiTaskFunc::io(int pos) const
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
// @param[in] pos 位置番号
// @param[in] pos 位置番号
// @param[in] head ヘッダ
// @param[in] pt_item パース木のIO宣言要素
// @param[in] decl 対応する宣言要素
void
EiTaskFunc::init_iodecl(int pos,
			ElbIOHead* head,
			const PtIOItem* pt_item,
			ElbDecl* decl)
{
  mIODeclList[pos].init(head, pt_item, decl);
}

// @brief 本体のステートメントをセットする．
void
EiTaskFunc::set_stmt(ElbStmt* stmt)
{
  mStmt = stmt;
}

// @brief 入出力を得る．
// @param[in] pos 位置番号 ( 0 <= pos < io_num() )
ElbIODecl*
EiTaskFunc::_io(int pos) const
{
  return &mIODeclList[pos];
}

// @brief 本体の ElbStmt を得る．
ElbStmt*
EiTaskFunc::_stmt() const
{
  return mStmt;
}


//////////////////////////////////////////////////////////////////////
// クラス EiTask
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のスコープ環境
// @param[in] pt_item パース木の定義
// @param[in] io_num IOの数
// @param[in] io_array IO の配列
EiTask::EiTask(const VlNamedObj* parent,
	       const PtItem* pt_item,
	       int io_num,
	       EiIODecl* io_array) :
  EiTaskFunc(parent, pt_item, io_num, io_array)
{
}

// @brief デストラクタ
EiTask::~EiTask()
{
}

// @brief 型の取得
tVpiObjType
EiTask::type() const
{
  return kVpiTask;
}

// @brief function type を返す．
tVpiFuncType
EiTask::func_type() const
{
  return kVpiIntFunc;
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
// @note 範囲を持たないときの値は不定
int
EiTask::left_range_val() const
{
  return 0;
}

// @brief 範囲の LSB の値を返す．
// @note 範囲を持たないときの値は不定
int
EiTask::right_range_val() const
{
  return 0;
}

// @brief 範囲のMSBを表す文字列の取得
// @note 範囲を持たない時の値は不定
string
EiTask::left_range_string() const
{
  return string();
}

// @brief 範囲のLSBを表す文字列の取得
// @note 範囲を持たない時の値は不定
string
EiTask::right_range_string() const
{
  return string();
}

// @brief 出力のビット幅を返す．
int
EiTask::bit_size() const
{
  return 0;
}

// @brief 出力変数をセットする．
// @param[in] ovar 出力変数
void
EiTask::set_ovar(ElbDecl* ovar)
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


//////////////////////////////////////////////////////////////////////
// クラス EiFunction
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のスコープ環境
// @param[in] pt_item パース木の定義
// @param[in] io_num IOの数
// @param[in] io_array IO の配列
EiFunction::EiFunction(const VlNamedObj* parent,
		       const PtItem* pt_item,
		       int io_num,
		       EiIODecl* io_array) :
  EiTaskFunc(parent, pt_item, io_num, io_array)
{
}

// @brief デストラクタ
EiFunction::~EiFunction()
{
}

// @brief 型の取得
tVpiObjType
EiFunction::type() const
{
  return kVpiFunction;
}

// @brief function type を返す．
tVpiFuncType
EiFunction::func_type() const
{
  switch ( pt_item()->data_type() ) {
  case kVpiVarNone:
    return kVpiSizedFunc;

  case kVpiVarInteger:
    return kVpiIntFunc;

  case kVpiVarReal:
    return kVpiRealFunc;

  case kVpiVarTime:
    return kVpiTimeFunc;

  case kVpiVarRealtime:
    return kVpiRealtimeFunc;

  default:
    break;
  }
  ASSERT_NOT_REACHED;
  return kVpiIntFunc;
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
// @note 範囲を持たないときの値は不定
int
EiFunction::left_range_val() const
{
  return 0;
}

// @brief 範囲の LSB の値を返す．
// @note 範囲を持たないときの値は不定
int
EiFunction::right_range_val() const
{
  return 0;
}

// @brief 範囲のMSBを表す文字列の取得
// @note 範囲を持たない時の値は不定
string
EiFunction::left_range_string() const
{
  return string();
}

// @brief 範囲のLSBを表す文字列の取得
// @note 範囲を持たない時の値は不定
string
EiFunction::right_range_string() const
{
  return string();
}

// @brief 出力のビット幅を返す．
int
EiFunction::bit_size() const
{
  switch ( pt_item()->data_type() ) {
  case kVpiVarNone:
    return 1;

  case kVpiVarInteger:
    return kVpiSizeInteger;

  case kVpiVarReal:
  case kVpiVarRealtime:
    return kVpiSizeReal;

  case kVpiVarTime:
    return kVpiSizeTime;

  default:
    break;
  }
  ASSERT_NOT_REACHED;
  return 0;
}

// @brief 出力変数をセットする．
// @param[in] ovar 出力変数
// @note 関数の場合のみ意味を持つ．
void
EiFunction::set_ovar(ElbDecl* ovar)
{
  mOvar = ovar;
}

// @brief constant function の時に true を返す．
bool
EiFunction::is_constant_function() const
{
  return false;
}


//////////////////////////////////////////////////////////////////////
// クラス EiFunctionV
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のスコープ環境
// @param[in] pt_item パース木の定義
// @param[in] io_num IOの数
// @param[in] io_array IO の配列
// @param[in] left 範囲の MSB の式
// @param[in] right 範囲の LSB の式
// @param[in] left_val 範囲の MSB の値
// @param[in] right_val 範囲の LSB の値
EiFunctionV::EiFunctionV(const VlNamedObj* parent,
			 const PtItem* pt_item,
			 int io_num,
			 EiIODecl* io_array,
			 const PtExpr* left,
			 const PtExpr* right,
			 int left_val,
			 int right_val) :
  EiFunction(parent, pt_item, io_num, io_array)
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
// @note 範囲を持たないときの値は不定
int
EiFunctionV::left_range_val() const
{
  return mRange.left_range_val();
}

// @brief 範囲の LSB の値を返す．
// @note 範囲を持たないときの値は不定
int
EiFunctionV::right_range_val() const
{
  return mRange.right_range_val();
}

// @brief 範囲のMSBを表す文字列の取得
// @note 範囲を持たない時の値は不定
string
EiFunctionV::left_range_string() const
{
  return mRange.left_range_string();
}

// @brief 範囲のLSBを表す文字列の取得
// @note 範囲を持たない時の値は不定
string
EiFunctionV::right_range_string() const
{
  return mRange.right_range_string();
}

// @brief 出力のビット幅を返す．
int
EiFunctionV::bit_size() const
{
  return mRange.size();
}

END_NAMESPACE_YM_VERILOG
