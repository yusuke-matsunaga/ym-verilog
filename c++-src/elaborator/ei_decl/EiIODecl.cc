
/// @file EiIODecl.cc
/// @brief EiIODecl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.

#include "ei/EiIODecl.h"
#include "ei/EiFactory.h"

#include "elaborator/ElbDecl.h"

#include "ym/vl/VlScope.h"
#include "ym/vl/VlTaskFunc.h"
#include "ym/pt/PtDecl.h"



BEGIN_NAMESPACE_YM_VERILOG

// @brief module IO ヘッダを生成する．
ElbIOHead*
EiFactory::new_IOHead(
  const VlModule* module,
  const PtIOHead* pt_header
)
{
  auto head = new EiModIOHead{module, pt_header};
  return head;
}

// @brief タスク/関数 IO ヘッダを生成する．
ElbIOHead*
EiFactory::new_IOHead(
  const VlTaskFunc* taskfunc,
  const PtIOHead* pt_header
)
{
  if ( taskfunc->type() == VpiObjType::Task ) {
    auto head = new EiTaskIOHead{taskfunc, pt_header};
    return head;
  }
  else { // VpiObjType::Function
    auto head = new EiFunctionIOHead{taskfunc, pt_header};
    return head;
  }
}


//////////////////////////////////////////////////////////////////////
// クラス EiIOHead
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiIOHead::EiIOHead(
  const PtIOHead* pt_header
) : mPtHead{pt_header}
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
  return mPtHead->direction();
}

// @brief 親のモジュールの取得
const VlModule*
EiIOHead::module() const
{
  return nullptr;
}

// @brief 親のタスクの取得
const VlTaskFunc*
EiIOHead::task() const
{
  return nullptr;
}

// @brief 親の関数の取得
const VlTaskFunc*
EiIOHead::function() const
{
  return nullptr;
}


//////////////////////////////////////////////////////////////////////
// クラス EiModIOHead
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiModIOHead::EiModIOHead(
  const VlModule* module,
  const PtIOHead* pt_header
) : EiIOHead{pt_header},
    mModule{module}
{
}

// @brief デストラクタ
EiModIOHead::~EiModIOHead()
{
}

// @brief 親のモジュールの取得
const VlModule*
EiModIOHead::module() const
{
  return mModule;
}


//////////////////////////////////////////////////////////////////////
// クラス EiTaskIOHead
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiTaskIOHead::EiTaskIOHead(
  const VlTaskFunc* task,
  const PtIOHead* pt_header
) : EiIOHead{pt_header},
    mTask{task}
{
}

// @brief デストラクタ
EiTaskIOHead::~EiTaskIOHead()
{
}

// @brief 親のタスクの取得
const VlTaskFunc*
EiTaskIOHead::task() const
{
  return mTask;
}


//////////////////////////////////////////////////////////////////////
// クラス EiFunctionIOHead
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiFunctionIOHead::EiFunctionIOHead(
  const VlTaskFunc* func,
  const PtIOHead* pt_header
) : EiIOHead{pt_header},
    mFunction{func}
{
}

// @brief デストラクタ
EiFunctionIOHead::~EiFunctionIOHead()
{
}

// @brief 親の関数の取得
const VlTaskFunc*
EiFunctionIOHead::function() const
{
  return mFunction;
}


//////////////////////////////////////////////////////////////////////
// クラス EiIODecl
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiIODecl::EiIODecl(
  ElbIOHead* head,
  const PtIOItem* pt_item,
  const VlDecl* decl
) : mHead{head},
    mPtItem{pt_item},
    mDecl{decl}
{
}

// @brief デストラクタ
EiIODecl::~EiIODecl()
{
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
string
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
int
EiIODecl::left_range_val() const
{
  return mDecl->left_range_val();
}

// @brief 範囲の LSB の値を返す．
int
EiIODecl::right_range_val() const
{
  return mDecl->right_range_val();
}

// @brief 範囲のMSBを表す文字列の取得
string
EiIODecl::left_range_string() const
{
  return mDecl->left_range_string();
}

// @brief 範囲のLSBを表す文字列の取得
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

END_NAMESPACE_YM_VERILOG
