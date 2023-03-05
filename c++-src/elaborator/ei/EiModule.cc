
/// @file EiModule.cc
/// @brief EiModule の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.

#include "ei/EiFactory.h"
#include "ei/EiModule.h"
#include "ei/EiPort.h"
#include "ei/EiIODecl.h"
#include "ei/EiDeclHead.h"
#include "ei/EiExpr.h"

#include "ym/pt/PtModule.h"
#include "ym/pt/PtDecl.h"
#include "ym/pt/PtItem.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief module を生成する．
ElbModule*
EiFactory::new_Module(
  const VlScope* parent,
  const PtModule* pt_module,
  const PtItem* pt_head,
  const PtInst* pt_inst
)
{
  auto module = new EiModule2{parent,
			      pt_module,
			      pt_head,
			      pt_inst};
  return module;
}

// @brief module array を生成する．
ElbModuleArray*
EiFactory::new_ModuleArray(
  const VlScope* parent,
  const PtModule* pt_module,
  const PtItem* pt_head,
  const PtInst* pt_inst,
  const PtExpr* left,
  const PtExpr* right,
  int left_val,
  int right_val
)
{
  EiRangeImpl range;
  range.set(left, right, left_val, right_val);

  auto module_array = new EiModuleArray{parent,
					pt_module,
					pt_head,
					pt_inst,
					range};
  return module_array;
}


//////////////////////////////////////////////////////////////////////
// クラス EiModuleHead
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiModuleHead::EiModuleHead(
  const VlScope* parent,
  const PtModule* pt_module,
  const PtItem* pt_head,
  const PtInst* pt_inst
) : mParent{parent},
    mPtModule{pt_module},
    mPtHead{pt_head},
    mPtInst{pt_inst}
{
}

// @brief デストラクタ
EiModuleHead::~EiModuleHead()
{
}

// @brief このオブジェクトの属しているスコープを返す．
const VlScope*
EiModuleHead::parent() const
{
  return mParent;
}

// @brief ファイル位置の取得
FileRegion
EiModuleHead::file_region() const
{
  if ( mPtInst ) {
    return mPtInst->file_region();
  }
  else {
    return mPtModule->file_region();
  }
}

// @brief インスタンス名を返す．
string
EiModuleHead::name() const
{
  if ( mPtInst ) {
    return mPtInst->name();
  }
  else {
    return mPtModule->name();
  }
}

// @brief definition location を返す．
FileRegion
EiModuleHead::def_file_region() const
{
  return mPtModule->file_region();
}

// @brief definition name を返す．
string
EiModuleHead::def_name() const
{
  return mPtModule->name();
}

// @brief ポート数を返す．
SizeType
EiModuleHead::port_num() const
{
  return mPtModule->port_num();
}

// @brief 入出力宣言数を返す．
SizeType
EiModuleHead::io_num() const
{
  return mPtModule->iodecl_num();
}

// @brief cell instance のチェック
bool
EiModuleHead::is_cell_instance() const
{
  return mPtModule->is_cell();
}

// @brief protect のチェック
bool
EiModuleHead::is_protected() const
{
  return mPtModule->is_protected();
}

// @brief top module の時 true を返す．
bool
EiModuleHead::is_top_module() const
{
  return mPtInst == nullptr;
}

// @brief time unit を返す．
int
EiModuleHead::time_unit() const
{
  return mPtModule->time_unit();
}

// @brief time precision を返す．
int
EiModuleHead::time_precision() const
{
  return mPtModule->time_precision();
}

// @brief default net type を返す．
VpiNetType
EiModuleHead::def_net_type() const
{
  return mPtModule->nettype();
}

// @brief unconnected drive を返す．
VpiUnconnDrive
EiModuleHead::unconn_drive() const
{
  return mPtModule->unconn_drive();
}

// @brief default delay mode を返す．
VpiDefDelayMode
EiModuleHead::def_delay_mode() const
{
  return mPtModule->delay_mode();
}

// @brief default decay time を返す．
int
EiModuleHead::def_decay_time() const
{
  return mPtModule->decay_time();
}

// @brief config 情報を返す．
string
EiModuleHead::config() const
{
  return mPtModule->config();
}

// @brief library 情報を返す．
string
EiModuleHead::library() const
{
  return mPtModule->library();
}

// @brief cell 情報を返す．
string
EiModuleHead::cell() const
{
  return mPtModule->cell();
}


//////////////////////////////////////////////////////////////////////
// クラス EiModule
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiModule::EiModule()
{
}

// @brief デストラクタ
EiModule::~EiModule()
{
}

// @brief ポート配列とIO配列を初期化する．
void
EiModule::init(
  SizeType port_num,
  SizeType io_num
)
{
  mPortList = vector<EiPort>(port_num);
  mIODeclList.reserve(io_num);
}

// @brief 型の取得
VpiObjType
EiModule::type() const
{
  return VpiObjType::Module;
}

// @brief ファイル位置の取得
FileRegion
EiModule::file_region() const
{
  return head().file_region();
}

// @brief このオブジェクトの属しているスコープを返す．
const VlScope*
EiModule::parent_scope() const
{
  return head().parent();
}

// @brief definition location を返す．
FileRegion
EiModule::def_file_region() const
{
  return head().def_file_region();
}

// @brief definition name を返す．
string
EiModule::def_name() const
{
  return head().def_name();
}

// @brief cell instance のチェック
bool
EiModule::is_cell_instance() const
{
  return head().is_cell_instance();
}

// @brief protect のチェック
bool
EiModule::is_protected() const
{
  return head().is_protected();
}

// @brief top module の時 true を返す．
bool
EiModule::is_top_module() const
{
  return head().is_top_module();
}

// @brief time unit を返す．
int
EiModule::time_unit() const
{
  return head().time_unit();
}

// @brief time precision を返す．
int
EiModule::time_precision() const
{
  return head().time_precision();
}

// @brief default net type を返す．
VpiNetType
EiModule::def_net_type() const
{
  return head().def_net_type();
}

// @brief unconnected drive を返す．
VpiUnconnDrive
EiModule::unconn_drive() const
{
  return head().unconn_drive();
}

// @brief default delay mode を返す．
VpiDefDelayMode
EiModule::def_delay_mode() const
{
  return head().def_delay_mode();
}

// @brief default decay time を返す．
int
EiModule::def_decay_time() const
{
  return head().def_decay_time();
}

// @brief config 情報を返す．
string
EiModule::config() const
{
  return head().config();
}

// @brief library 情報を返す．
string
EiModule::library() const
{
  return head().library();
}

// @brief cell 情報を返す．
string
EiModule::cell() const
{
  return head().cell();
}

// @brief ポート数を返す．
SizeType
EiModule::port_num() const
{
  return head().port_num();
}

// @brief ポートの取得
const VlPort*
EiModule::port(
  SizeType pos
) const
{
  ASSERT_COND( 0 <= pos && pos < port_num() );
  return &mPortList[pos];
}

// @brief 入出力数を得る．
SizeType
EiModule::io_num() const
{
  return head().io_num();
}

// @brief 入出力の取得
const VlIODecl*
EiModule::io(
  SizeType pos
) const
{
  ASSERT_COND( 0 <= pos && pos < io_num() );
  return &mIODeclList[pos];
}

// @brief 入出力を追加する．
void
EiModule::add_iodecl(
  ElbIOHead* head,
  const PtIOItem* pt_item,
  const VlDecl* decl
)
{
  mIODeclList.push_back({head, pt_item, decl});
}

// @brief ポートの初期設定を行う．
void
EiModule::init_port(
  SizeType index,
  const PtPort* pt_port,
  ElbExpr* low_conn,
  VpiDir dir
)
{
  ASSERT_COND( 0 <= index && index < port_num() );
  mPortList[index].init(this, pt_port, index, low_conn, dir);
}

// @brief ポートの high_conn を接続する．
void
EiModule::set_port_high_conn(
  SizeType index,
  ElbExpr* high_conn,
  bool conn_by_name
)
{
  ASSERT_COND( 0 <= index && index < port_num() );
  mPortList[index].set_high_conn(high_conn, conn_by_name);
}


//////////////////////////////////////////////////////////////////////
// クラス EiModule1
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiModule1::EiModule1()
{
}

// @brief デストラクタ
EiModule1::~EiModule1()
{
}

// @brief 初期設定を行う．
void
EiModule1::init(
  SizeType port_num,
  SizeType io_num,
  EiModuleArray* module_array,
  int index
)
{
  EiModule::init(port_num, io_num);

  mModuleArray = module_array;
  mIndex = index;

  // 名前の生成
}

// @brief 名前の取得
string
EiModule1::name() const
{
  ostringstream buf;
  buf << mModuleArray->name() << "[" << mIndex << "]";
  return buf.str();
}

// @brief 配列要素の時 true を返す．
bool
EiModule1::is_array() const
{
  return true;
}

// @brief インデックスの値を返す．
int
EiModule1::index() const
{
  return mIndex;
}

// @brief 親の配列を返す．
const VlModuleArray*
EiModule1::module_array() const
{
  return mModuleArray;
}

// @brief ヘッダ情報を返す．
const EiModuleHead&
EiModule1::head() const
{
  return mModuleArray->head();
}

// @brief ヘッダ情報を返す．
EiModuleHead&
EiModule1::head()
{
  return mModuleArray->head();
}


//////////////////////////////////////////////////////////////////////
// クラス EiModule2
//////////////////////////////////////////////////////////////////////

// コンストラクタ
EiModule2::EiModule2(
  const VlScope* parent,
  const PtModule* pt_module,
  const PtItem* pt_head,
  const PtInst* pt_inst
) : mHead{parent, pt_module, pt_head, pt_inst}
{
  SizeType port_num = pt_module->port_num();
  SizeType io_num = pt_module->iodecl_num();
  init(port_num, io_num);
}

// デストラクタ
EiModule2::~EiModule2()
{
}

// @brief 名前の取得
string
EiModule2::name() const
{
  return mHead.name();
}

// @brief 配列要素の時 true を返す．
bool
EiModule2::is_array() const
{
  return false;
}

// @brief インデックスの値を返す．
int
EiModule2::index() const
{
  return 0;
}

// @brief 親の配列を返す．
const VlModuleArray*
EiModule2::module_array() const
{
  return nullptr;
}

// @brief ヘッダ情報を返す．
const EiModuleHead&
EiModule2::head() const
{
  return mHead;
}

// @brief ヘッダ情報を返す．
EiModuleHead&
EiModule2::head()
{
  return mHead;
}


//////////////////////////////////////////////////////////////////////
/// クラス EiModuleArray
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiModuleArray::EiModuleArray(
  const VlScope* parent,
  const PtModule* pt_module,
  const PtItem* pt_head,
  const PtInst* pt_inst,
  const EiRangeImpl& range
) : mHead{parent, pt_module, pt_head, pt_inst},
    mRange{range},
    mArray(range.size())
{
  SizeType port_num = pt_module->port_num();
  SizeType io_num = pt_module->iodecl_num();
  SizeType n = mArray.size();
  for ( SizeType i = 0; i < n; ++ i ) {
    int index = range.index(i);
    mArray[i].init(port_num, io_num, this, index);
  }
}

// @brief デストラクタ
EiModuleArray::~EiModuleArray()
{
}

// @brief 型の取得
VpiObjType
EiModuleArray::type() const
{
  return VpiObjType::Module;
}

// @brief ファイル位置の取得
FileRegion
EiModuleArray::file_region() const
{
  return mHead.file_region();
}

// @brief このオブジェクトの属しているスコープを返す．
const VlScope*
EiModuleArray::parent_scope() const
{
  return mHead.parent();
}

// @brief 名前の取得
string
EiModuleArray::name() const
{
  return mHead.name();
}

// @brief 範囲の MSB の値を返す．
int
EiModuleArray::left_range_val() const
{
  return mRange.left_range_val();
}

// @brief 範囲の LSB の値を返す．
int
EiModuleArray::right_range_val() const
{
  return mRange.right_range_val();
}

// @brief 範囲のMSBを表す文字列の取得
string
EiModuleArray::left_range_string() const
{
  return mRange.left_range_string();
}

// @brief 範囲のLSBを表す文字列の取得
string
EiModuleArray::right_range_string() const
{
  return mRange.right_range_string();
}

// @brief 要素数を返す．
SizeType
EiModuleArray::elem_num() const
{
  return mRange.size();
}

// @brief 要素を返す．
const VlModule*
EiModuleArray::elem_by_offset(
  SizeType offset
) const
{
  ASSERT_COND( 0 <= offset && offset < elem_num() );
  return &mArray[offset];
}

// @brief 要素を返す．
const VlModule*
EiModuleArray::elem_by_index(
  int index
) const
{
  SizeType offset;
  if ( mRange.calc_offset(index, offset) ) {
    return elem_by_offset(offset);
  }
  else {
    // index が範囲外だった．
    ASSERT_NOT_REACHED;
    return nullptr;
  }
}

// @brief 要素を取り出す．
ElbModule*
EiModuleArray::elem(
  SizeType index
)
{
  return &mArray[index];
}

// @brief ヘッダ情報を返す．
const EiModuleHead&
EiModuleArray::head() const
{
  return mHead;
}

// @brief ヘッダ情報を返す．
EiModuleHead&
EiModuleArray::head()
{
  return mHead;
}

// @brief 範囲を返す．
const EiRangeImpl&
EiModuleArray::range() const
{
  return mRange;
}

END_NAMESPACE_YM_VERILOG
