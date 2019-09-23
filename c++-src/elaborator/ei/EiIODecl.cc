﻿
/// @file EiIODecl.cc
/// @brief EiIODecl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "EiFactory.h"
#include "EiIODecl.h"

#include "elb/ElbModule.h"
#include "elb/ElbTaskFunc.h"
#include "elb/ElbDecl.h"

#include "ym/pt/PtDecl.h"



BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief module IO ヘッダを生成する．
// @param[in] module 親のモジュール
// @param[in] pt_header パース木のIO宣言ヘッダ
ElbIOHead*
EiFactory::new_ModIOHead(ElbModule* module,
			 const PtIOHead* pt_header)
{
  void* p = mAlloc.get_memory(sizeof(EiModIOHead));
  EiIOHead* head = new (p) EiModIOHead(module, pt_header);
  return head;
}

// @brief タスク用の IO ヘッダを生成する．
// @param[in] task 親のタスク
// @param[in] pt_header パース木のIO宣言ヘッダ
ElbIOHead*
EiFactory::new_TaskIOHead(ElbTaskFunc* task,
			  const PtIOHead* pt_header)
{
  void* p = mAlloc.get_memory(sizeof(EiTaskIOHead));
  EiIOHead* head = new (p) EiTaskIOHead(task, pt_header);
  return head;
}

// @brief 関数用の IO ヘッダを生成する．
// @param[in] func 親の関数
// @param[in] pt_header パース木のIO宣言ヘッダ
ElbIOHead*
EiFactory::new_FunctionIOHead(ElbTaskFunc* func,
			      const PtIOHead* pt_header)
{
  void* p = mAlloc.get_memory(sizeof(EiFunctionIOHead));
  EiIOHead* head = new (p) EiFunctionIOHead(func, pt_header);
  return head;
}


//////////////////////////////////////////////////////////////////////
// クラス EiIOHead
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_header パース木のIO宣言ヘッダ
EiIOHead::EiIOHead(const PtIOHead* pt_header) :
  mPtHead(pt_header)
{
}

// @brief デストラクタ
EiIOHead::~EiIOHead()
{
}

// @brief 方向を返す．
VpiDir
EiIOHead::direction() const
{
  switch ( mPtHead->type() ) {
  case kPtIO_Input:  return VpiDir::Input;
  case kPtIO_Output: return VpiDir::Output;
  case kPtIO_Inout:  return VpiDir::Inout;
  default: ASSERT_NOT_REACHED;
  }
  // ダミー
  return VpiDir::NoDirection;
}

// @brief 親のモジュールの取得
ElbModule*
EiIOHead::module() const
{
  return nullptr;
}

// @brief 親のタスクの取得
ElbTaskFunc*
EiIOHead::task() const
{
  return nullptr;
}

// @brief 親の関数の取得
ElbTaskFunc*
EiIOHead::function() const
{
  return nullptr;
}


//////////////////////////////////////////////////////////////////////
// クラス EiModIOHead
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] pt_header パース木のIO宣言ヘッダ
EiModIOHead::EiModIOHead(ElbModule* module,
			 const PtIOHead* pt_header) :
  EiIOHead(pt_header),
  mModule(module)
{
}

// @brief デストラクタ
EiModIOHead::~EiModIOHead()
{
}

// @brief 親のモジュールの取得
ElbModule*
EiModIOHead::module() const
{
  return mModule;
}


//////////////////////////////////////////////////////////////////////
// クラス EiTaskIOHead
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] task 親のタスク/関数
// @param[in] pt_header パース木のIO宣言ヘッダ
EiTaskIOHead::EiTaskIOHead(ElbTaskFunc* task,
			   const PtIOHead* pt_header) :
  EiIOHead(pt_header),
  mTask(task)
{
}

// @brief デストラクタ
EiTaskIOHead::~EiTaskIOHead()
{
}

// @brief 親のタスクの取得
ElbTaskFunc*
EiTaskIOHead::task() const
{
  return mTask;
}


//////////////////////////////////////////////////////////////////////
// クラス EiFunctionIOHead
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] func 親の関数
// @param[in] pt_header パース木のIO宣言ヘッダ
EiFunctionIOHead::EiFunctionIOHead(ElbTaskFunc* func,
				   const PtIOHead* pt_header) :
  EiIOHead(pt_header),
  mFunction(func)
{
}

// @brief デストラクタ
EiFunctionIOHead::~EiFunctionIOHead()
{
}

// @brief 親の関数の取得
ElbTaskFunc*
EiFunctionIOHead::function() const
{
  return mFunction;
}


//////////////////////////////////////////////////////////////////////
// クラス EiIODecl
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiIODecl::EiIODecl()
{
}

// @brief デストラクタ
EiIODecl::~EiIODecl()
{
}

// @brief 初期化を行う．
// @param[in] head ヘッダ
// @param[in] pt_item パース木のIO宣言要素
// @param[in] decl 対応する宣言要素
void
EiIODecl::init(ElbIOHead* head,
	       const PtIOItem* pt_item,
	       ElbDecl* decl)
{
  mHead = head;
  mPtItem = pt_item;
  mDecl = decl;
}

// @brief 型の取得
VpiObjType
EiIODecl::type() const
{
  return VpiObjType::IODecl;
}

// @brief ファイル位置の取得
FileRegion
EiIODecl::file_region() const
{
  return mPtItem->file_region();
}

// @brief 名前を返す．
const char*
EiIODecl::name() const
{
  return mPtItem->name();
}

// @brief 方向を返す．
VpiDir
EiIODecl::direction() const
{
  return mHead->direction();
}

// @brief 符号の取得
// @retval true 符号つき
// @retval false 符号なし
bool
EiIODecl::is_signed() const
{
  return mDecl->is_signed();
}

// @brief 範囲指定を持つとき true を返す．
bool
EiIODecl::has_range() const
{
  return mDecl->has_range();
}

// @brief 範囲の MSB の値を返す．
// @note 範囲を持たないときの値は不定
int
EiIODecl::left_range_val() const
{
  return mDecl->left_range_val();
}

// @brief 範囲の LSB の値を返す．
// @note 範囲を持たないときの値は不定
int
EiIODecl::right_range_val() const
{
  return mDecl->right_range_val();
}

// @brief 範囲のMSBを表す文字列の取得
// @note 範囲を持たない時の値は不定
string
EiIODecl::left_range_string() const
{
  return mDecl->left_range_string();
}

// @brief 範囲のLSBを表す文字列の取得
// @note 範囲を持たない時の値は不定
string
EiIODecl::right_range_string() const
{
  return mDecl->right_range_string();
}

// @brief ビット幅を返す．
SizeType
EiIODecl::bit_size() const
{
  return mDecl->bit_size();
}

// @brief 対応する宣言要素を返す．
const VlDecl*
EiIODecl::decl() const
{
  return mDecl;
}

// @brief 親のモジュールの取得
const VlModule*
EiIODecl::module() const
{
  return mHead->module();
}

// @brief 親の UDP の取得
// @note このクラスでは nullptr を返す．
const VlUdpDefn*
EiIODecl::udp_defn() const
{
  return nullptr;
}

// @brief 親のタスクの取得
const VlTaskFunc*
EiIODecl::task() const
{
  return mHead->task();
}

// @brief 親の関数の取得
const VlTaskFunc*
EiIODecl::function() const
{
  return mHead->function();
}

// @brief 対応する ElbDecl を返す．
ElbDecl*
EiIODecl::_decl() const
{
  return mDecl;
}

END_NAMESPACE_YM_VERILOG
