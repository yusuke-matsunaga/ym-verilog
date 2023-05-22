
/// @file CptModule.cc
/// @brief CptModule の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.

#include "CptModule.h"
#include "alloc/Alloc.h"
#include "parser/CptFactory.h"
#include "parser/PtiDecl.h"
#include "ym/pt/PtItem.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// module を表すノード
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptModule::CptModule(
  const FileRegion& file_region,
  const char* name,
  bool macro,
  bool is_cell,
  bool is_protected,
  int time_unit,
  int time_precision,
  VpiNetType net_type,
  VpiUnconnDrive unconn,
  VpiDefDelayMode delay,
  int decay,
  bool explicit_name,
  bool portfaults,
  bool suppress_faults,
  const string& config,
  const string& library,
  const string& cell,
  PtiDeclHeadArray&& paramport_array,
  PtiPortArray&& port_array,
  PtiIOHeadArray&& iohead_array,
  PtiDeclHeadArray&& declhead_array,
  PtiItemArray&& item_array
) : mFileRegion{file_region},
    mName{name},
    mDefDecayTime{decay},
    mConfig{config},
    mLibrary{library},
    mCell{cell},
    mParamPortArray{move(paramport_array)},
    mPortArray{move(port_array)},
    mIOHeadArray{move(iohead_array)},
    mDeclHeadArray{move(declhead_array)},
    mItemArray{move(item_array)}
{
  mFlags =
    static_cast<std::uint32_t>(is_cell)
    | (static_cast<std::uint32_t>(is_protected) << 1)
    | (static_cast<std::uint32_t>(time_precision + 16) << 2)
    | (static_cast<std::uint32_t>(time_unit + 16) << 7)
    | (static_cast<std::uint32_t>(net_type) << 12)
    | (static_cast<std::uint32_t>(unconn) << 16)
    | (static_cast<std::uint32_t>(delay) << 18)
    | (static_cast<std::uint32_t>(macro) << 21)
    | (static_cast<std::uint32_t>(explicit_name) << 22)
    | (1U << 23) // top_module
    | (static_cast<std::uint32_t>(portfaults) << 25)
    | (static_cast<std::uint32_t>(suppress_faults) << 26)
    ;

  mIODeclNum = 0;
  for ( auto head: mIOHeadArray ) {
    mIODeclNum += head->item_num();
  }
}

// デストラクタ
CptModule::~CptModule()
{
}

// ファイル位置を返す．
FileRegion
CptModule::file_region() const
{
  return mFileRegion;
}

// 名前を取り出す．
const char*
CptModule::name() const
{
  return mName;
}

// @brief パラメータポート宣言配列の要素数の取得
SizeType
CptModule::paramport_num() const
{
  return mParamPortArray.size();
}

// @brief パラメータポート宣言の取得
const PtDeclHead*
CptModule::paramport(
  SizeType pos
) const
{
  return mParamPortArray[pos];
}

// @brief ポート数を取り出す．
SizeType
CptModule::port_num() const
{
  return mPortArray.size();
}

// @brief ポートを取り出す．
const PtPort*
CptModule::port(
  SizeType pos
) const
{
  return mPortArray[pos];
}

// @brief 入出力宣言ヘッダ配列の要素数の取得
SizeType
CptModule::iohead_num() const
{
  return mIOHeadArray.size();
}

// @brief 入出力宣言の取得
const PtIOHead*
CptModule::iohead(
  SizeType pos
) const
{
  return mIOHeadArray[pos];
}

// @brief 入出力宣言の要素数の取得
SizeType
CptModule::iodecl_num() const
{
  return mIODeclNum;
}

// @brief 宣言ヘッダ配列の要素数の取得
SizeType
CptModule::declhead_num() const
{
  return mDeclHeadArray.size();
}

// @brief 宣言ヘッダの取得
const PtDeclHead*
CptModule::declhead(
  SizeType pos
) const
{
  return mDeclHeadArray[pos];
}

// @brief item 配列の要素数の取得
SizeType
CptModule::item_num() const
{
  return mItemArray.size();
}

// @brief item の取得
const PtItem*
CptModule::item(
  SizeType pos
) const
{
  return mItemArray[pos];
}

// macromodule の時 true を返す．
bool
CptModule::is_macromodule() const
{
  return static_cast<bool>((mFlags >> 21) & 1);
}

// cell の時 true を返す．
bool
CptModule::is_cell() const
{
  return static_cast<bool>(mFlags & 1);
}

// protect されていたら true を返す．
bool
CptModule::is_protected() const
{
  return static_cast<bool>((mFlags >> 1) & 1);
}

// time unit を返す．
int
CptModule::time_unit() const
{
  return ((mFlags >> 7) & 0x1f) - 16;
}

// time precision を返す．
int
CptModule::time_precision() const
{
  return ((mFlags >> 2) & 0x1f) - 16;
}

// default net type を返す．
VpiNetType
CptModule::nettype() const
{
  return static_cast<VpiNetType>((mFlags >> 12) & 0xf);
}

// unconnected drive を返す．
VpiUnconnDrive
CptModule::unconn_drive() const
{
  return static_cast<VpiUnconnDrive>((mFlags >> 16) & 0x3);
}

// default delay mode を返す．
VpiDefDelayMode
CptModule::delay_mode() const
{
  return static_cast<VpiDefDelayMode>((mFlags >> 18) & 0x7);
}

// default decay time を返す．
int
CptModule::decay_time() const
{
  return mDefDecayTime;
}

// 名無しのポートを持つことを記録する．
void
CptModule::set_named_port()
{
  mFlags |= (1 << 22);
}

// すべてのポートが外部名を持っているときに true を返す．
bool
CptModule::explicit_name() const
{
  return (mFlags >> 22) & 1;
}

// 親がいないときに true を返す．
bool
CptModule::is_topmodule() const
{
  return static_cast<bool>((mFlags >> 23) & 1);
}

// top_module フラグを下ろす．
void
CptModule::clear_topmodule() const
{
  mFlags &= ~(1 << 23);
}

// @brief in_use フラグの設定
void
CptModule::set_in_use() const
{
  mFlags |= (1 << 24);
}

// @brief in_use フラグの解除
void
CptModule::reset_in_use() const
{
  mFlags &= ~(1 << 24);
}

// @brief in_use フラグの取得
bool
CptModule::is_in_use() const
{
  return static_cast<bool>((mFlags >> 24) & 1);
}

// portfaults の状態を返す．
bool
CptModule::portfaults() const
{
  return static_cast<bool>((mFlags >> 25) & 1);
}

// suppress_faults の状態を返す．
bool
CptModule::suppress_faults() const
{
  return static_cast<bool>((mFlags >> 26) & 1);
}

// config 情報を返す．
const string&
CptModule::config() const
{
  return mConfig;
}

// library 情報を返す．
const string&
CptModule::library() const
{
  return mLibrary;
}

// cell 情報を返す．
const string&
CptModule::cell() const
{
  return mCell;
}


//////////////////////////////////////////////////////////////////////
// port を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptPort::CptPort(
  const FileRegion& file_region,
  const char* ext_name
) : mFileRegion{file_region},
    mExtName{ext_name}
{
}

// デストラクタ
CptPort::~CptPort()
{
}

// ファイル位置を返す．
FileRegion
CptPort::file_region() const
{
  return mFileRegion;
}

// 外向の名前(本当のポート名)を取出す
const char*
CptPort::ext_name() const
{
  return mExtName;
}

// @brief 内側のポート結線を表す式の取得
const PtExpr*
CptPort::portref() const
{
  return nullptr;
}

// @brief 内部のポート結線リストのサイズの取得
SizeType
CptPort::portref_size() const
{
  return 0;
}

// @brief 内部のポート結線リストの取得
const PtExpr*
CptPort::portref_elem(
  int pos
) const
{
  ASSERT_NOT_REACHED;
  return nullptr;
}

//@brief 内部ポート結線の方向の取得
VpiDir
CptPort::portref_dir(
  int pos
) const
{
  ASSERT_NOT_REACHED;
  return VpiDir::NoDirection;
}

// @brief portref の方向を設定する．
void
CptPort::_set_portref_dir(
  int pos,
  VpiDir dir
)
{
  ASSERT_NOT_REACHED;
}


//////////////////////////////////////////////////////////////////////
// port を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptPort1::CptPort1(
  const FileRegion& file_region,
  const PtExpr* portref,
  const char* ext_name
) : CptPort{file_region, ext_name},
    mPortRef{portref}
{
}

// デストラクタ
CptPort1::~CptPort1()
{
}

// @brief 内側のポート結線を表す式の取得
const PtExpr*
CptPort1::portref() const
{
  return mPortRef;
}

// @brief 内部のポート結線リストのサイズの取得
SizeType
CptPort1::portref_size() const
{
  return 1;
}

// @brief 内部のポート結線リストの取得
const PtExpr*
CptPort1::portref_elem(
  int pos
) const
{
  ASSERT_COND( pos == 0 );
  return mPortRef;
}

// @brief 内部ポート結線の方向の取得
VpiDir
CptPort1::portref_dir(
  int pos
) const
{
  return mDir;
}

// @brief portref の方向を設定する．
void
CptPort1::_set_portref_dir(
  int pos,
  VpiDir dir
)
{
  ASSERT_COND( pos == 0 );
  mDir = dir;
}


//////////////////////////////////////////////////////////////////////
// port を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptPort2::CptPort2(
  const FileRegion& file_region,
  const PtExpr* portref,
  PtiExprArray&& portref_array,
  const char* ext_name,
  void* q
) : CptPort1{file_region, portref, ext_name},
    mPortRefArray{move(portref_array)},
    mDirArray{new (q) VpiDir[mPortRefArray.size()]}
{
}

// デストラクタ
CptPort2::~CptPort2()
{
}

// @brief 内部のポート結線リストのサイズの取得
SizeType
CptPort2::portref_size() const
{
  return mPortRefArray.size();
}

// @brief 内部のポート結線リストの取得
const PtExpr*
CptPort2::portref_elem(
  int pos
) const
{
  return mPortRefArray[pos];
}

// @brief 内部ポート結線の方向の取得
VpiDir
CptPort2::portref_dir(
  int pos
) const
{
  ASSERT_COND( 0 <= pos && pos < portref_size() );
  return mDirArray[pos];
}

// @brief portref の方向を設定する．
void
CptPort2::_set_portref_dir(
  int pos,
  VpiDir dir
)
{
  ASSERT_COND( 0 <= pos && pos < portref_size() );
  mDirArray[pos] = dir;
}


//////////////////////////////////////////////////////////////////////
// モジュール関係
//////////////////////////////////////////////////////////////////////

// モジュールの生成
const PtModule*
CptFactory::new_Module(
  const FileRegion& file_region,
  const char* name,
  bool macro,
  bool is_cell,
  bool is_protected,
  int time_unit,
  int time_precision,
  VpiNetType net_type,
  VpiUnconnDrive unconn,
  VpiDefDelayMode delay,
  int decay,
  bool explicit_name,
  bool portfaults,
  bool suppress_faults,
  const string& config,
  const string& library,
  const string& cell,
  const vector<const PtDeclHead*>& paramport_array,
  const vector<const PtPort*>& port_array,
  const vector<const PtIOHead*>& iohead_array,
  const vector<const PtDeclHead*>& declhead_array,
  const vector<const PtItem*>& item_array
)
{
  ++ mNumModule;
  void* p = mAlloc.get_memory(sizeof(CptModule));
  auto obj = new (p) CptModule{file_region, name,
			       macro, is_cell, is_protected,
			       time_unit, time_precision,
			       net_type, unconn,
			       delay, decay,
			       explicit_name,
			       portfaults, suppress_faults,
			       config, library, cell,
			       PtiArray<const PtDeclHead>{mAlloc, paramport_array},
			       PtiArray<const PtPort>{mAlloc, port_array},
			       PtiArray<const PtIOHead>{mAlloc, iohead_array},
			       PtiArray<const PtDeclHead>{mAlloc, declhead_array},
			       PtiArray<const PtItem>{mAlloc, item_array}};
  return obj;
}


//////////////////////////////////////////////////////////////////////
// ポート関係
//////////////////////////////////////////////////////////////////////

// @brief ポートの生成
PtiPort*
CptFactory::new_Port(
  const FileRegion& file_region,
  const char* ext_name
)
{
  ++ mNumPort;
  void* p = mAlloc.get_memory(sizeof(CptPort));
  auto obj = new (p) CptPort{file_region, ext_name};
  return obj;
}

// ポートの生成
PtiPort*
CptFactory::new_Port(
  const FileRegion& file_region,
  const PtExpr* portref,
  const char* ext_name
)
{
  ++ mNumPort;
  void* p = mAlloc.get_memory(sizeof(CptPort1));
  auto obj = new (p) CptPort1{file_region, portref, ext_name};
  return obj;
}

// ポートの生成
PtiPort*
CptFactory::new_Port(
  const FileRegion& file_region,
  const PtExpr* portref,
  const vector<const PtExpr*>& portref_array,
  const char* ext_name
)
{
  ++ mNumPort;
  void* p = mAlloc.get_memory(sizeof(CptPort2));
  void* q = mAlloc.get_memory(sizeof(VpiDir) * portref_array.size());
  auto obj = new (p) CptPort2{file_region, portref,
			      PtiArray<const PtExpr>{mAlloc, portref_array},
			      ext_name, q};
  return obj;
}

END_NAMESPACE_YM_VERILOG
