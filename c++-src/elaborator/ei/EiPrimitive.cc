﻿
/// @file EiPrimitive.cc
/// @brief EiPrimitive の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.

#include "ei/EiFactory.h"
#include "ei/EiPrimitive.h"

#include "ym/vl/VlUdp.h"
#include "ym/vl/VlDelay.h"
#include "ym/vl/VlExpr.h"

#include "ym/pt/PtItem.h"
#include "ym/pt/PtMisc.h"

#include "ym/ClibCell.h"
#include "ym/ClibPin.h"

#include "ym/Range.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief ゲートプリミティブのヘッダを生成する．
ElbPrimHead*
EiFactory::new_PrimHead(
  const VlScope* parent,
  const PtItem* pt_header,
  bool has_delay
)
{
  EiPrimHead* head = nullptr;
  if ( has_delay ) {
    head = new EiPrimHeadD{parent, pt_header};
  }
  else {
    head = new EiPrimHead{parent, pt_header};
  }
  return head;
}

// @brief UDPプリミティブのヘッダを生成する．
ElbPrimHead*
EiFactory::new_UdpHead(
  const VlScope* parent,
  const PtItem* pt_header,
  const VlUdpDefn* udp,
  bool has_delay
)
{
  EiPrimHead* head = nullptr;
  if ( has_delay ) {
    head = new EiPrimHeadUD{parent, pt_header, udp};
  }
  else {
    head = new EiPrimHeadU{parent, pt_header, udp};
  }
  return head;
}

// @brief セルプリミティブのヘッダを生成する．
ElbPrimHead*
EiFactory::new_CellHead(
  const VlScope* parent,
  const PtItem* pt_header,
  const ClibCell& cell
)
{
  auto head = new EiPrimHeadC{parent, pt_header, cell};
  return head;
}

// @brief プリミティブインスタンスを生成する．
ElbPrimitive*
EiFactory::new_Primitive(
  ElbPrimHead* head,
  const PtInst* pt_inst
)
{
  auto prim = new EiPrimitive2{head, pt_inst};
  return prim;
}

// @brief プリミティブ配列インスタンスを生成する．
ElbPrimArray*
EiFactory::new_PrimitiveArray(
  ElbPrimHead* head,
  const PtInst* pt_inst,
  const PtExpr* left,
  const PtExpr* right,
  int left_val,
  int right_val
)
{
  EiRangeImpl range;
  range.set(left, right, left_val, right_val);

  auto prim_array = new EiPrimArray{head, pt_inst, range};
  return prim_array;
}

// @brief セルプリミティブインスタンスを生成する．
ElbPrimitive*
EiFactory::new_CellPrimitive(
  ElbPrimHead* head,
  const ClibCell& cell,
  const PtInst* pt_inst
)
{
  auto prim = new EiPrimitive2{head, cell, pt_inst};
  return prim;
}

// @brief セルプリミティブ配列インスタンスを生成する．
ElbPrimArray*
EiFactory::new_CellPrimitiveArray(
  ElbPrimHead* head,
  const ClibCell& cell,
  const PtInst* pt_inst,
  const PtExpr* left,
  const PtExpr* right,
  int left_val,
  int right_val
)
{
  EiRangeImpl range;
  range.set(left, right, left_val, right_val);

  auto prim_array = new EiPrimArray{head, cell, pt_inst, range};
  return prim_array;
}


//////////////////////////////////////////////////////////////////////
// クラス EiPrimHead
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiPrimHead::EiPrimHead(
  const VlScope* parent,
  const PtItem* pt_header
) : mParent{parent},
    mPtHead{pt_header}
{
}

// @brief デストラクタ
EiPrimHead::~EiPrimHead()
{
}

// @brief このオブジェクトの属しているスコープを返す．
const VlScope*
EiPrimHead::parent_scope() const
{
  return mParent;
}

// @brief primitive type を返す．
VpiPrimType
EiPrimHead::prim_type() const
{
  return mPtHead->prim_type();
}

// @brief プリミティブの定義名を返す．
string
EiPrimHead::def_name() const
{
  string nm;
  switch ( prim_type() ) {
  case VpiPrimType::And:      nm = "and"; break;
  case VpiPrimType::Nand:     nm = "nand"; break;
  case VpiPrimType::Nor:      nm = "nor"; break;
  case VpiPrimType::Or:       nm = "or"; break;
  case VpiPrimType::Xor:      nm = "xor"; break;
  case VpiPrimType::Xnor:     nm = "xnor"; break;
  case VpiPrimType::Buf:      nm = "buf"; break;
  case VpiPrimType::Not:      nm = "not"; break;
  case VpiPrimType::Bufif0:   nm = "bufif0"; break;
  case VpiPrimType::Bufif1:   nm = "bufif1"; break;
  case VpiPrimType::Notif0:   nm = "notif0"; break;
  case VpiPrimType::Notif1:   nm = "notif1"; break;
  case VpiPrimType::Nmos:     nm = "nmos"; break;
  case VpiPrimType::Pmos:     nm = "pmos"; break;
  case VpiPrimType::Cmos:     nm = "cmos"; break;
  case VpiPrimType::Rnmos:    nm = "rnmos"; break;
  case VpiPrimType::Rpmos:    nm = "rpmos"; break;
  case VpiPrimType::Rcmos:    nm = "rcmos"; break;
  case VpiPrimType::Rtran:    nm = "rtran"; break;
  case VpiPrimType::Rtranif0: nm = "rtranif0"; break;
  case VpiPrimType::Rtranif1: nm = "rtranif1"; break;
  case VpiPrimType::Tran:     nm = "tran"; break;
  case VpiPrimType::Tranif0:  nm = "tranif0"; break;
  case VpiPrimType::Tranif1:  nm = "tranif1"; break;
  case VpiPrimType::Pullup:   nm = "pullup"; break;
  case VpiPrimType::Pulldown: nm = "pulldown"; break;
  case VpiPrimType::Cell:     nm = "cell"; break;
  case VpiPrimType::Seq:
  case VpiPrimType::Comb:
    ASSERT_NOT_REACHED;
    break;
  }
  return nm;
}

// @brief UDP 定義を返す．
const VlUdpDefn*
EiPrimHead::udp_defn() const
{
  return nullptr;
}

// @brief セルを返す．
ClibCell
EiPrimHead::cell() const
{
  // 不正値
  return {};
}

// @brief 0 の強さを得る．
VpiStrength
EiPrimHead::drive0() const
{
  if ( mPtHead->strength() ) {
    return mPtHead->strength()->drive0();
  }
  return VpiStrength::NoStrength;
}

// @brief 1 の強さを得る．
VpiStrength
EiPrimHead::drive1() const
{
  if ( mPtHead->strength() ) {
    return mPtHead->strength()->drive1();
  }
  return VpiStrength::NoStrength;
}

// @brief 遅延式を得る．
const VlDelay*
EiPrimHead::delay() const
{
  return nullptr;
}

// @brief 遅延式を設定する．
void
EiPrimHead::set_delay(
  const VlDelay* expr
)
{
}


//////////////////////////////////////////////////////////////////////
// クラス EiPrimHeadD
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiPrimHeadD::EiPrimHeadD(
  const VlScope* parent,
  const PtItem* pt_header
) : EiPrimHead{parent, pt_header}
{
}

// @brief デストラクタ
EiPrimHeadD::~EiPrimHeadD()
{
}

// @brief 遅延式を得る．
const VlDelay*
EiPrimHeadD::delay() const
{
  return mDelay;
}

// @brief 遅延式を設定する．
void
EiPrimHeadD::set_delay(
  const VlDelay* expr
)
{
  mDelay = expr;
}


//////////////////////////////////////////////////////////////////////
// クラス EiPrimHeadU
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiPrimHeadU::EiPrimHeadU(
  const VlScope* parent,
  const PtItem* pt_header,
  const VlUdpDefn* udp
) : EiPrimHead{parent, pt_header},
    mUdp{udp}
{
}

// @brief デストラクタ
EiPrimHeadU::~EiPrimHeadU()
{
}

// @brief primitive type を返す．
VpiPrimType
EiPrimHeadU::prim_type() const
{
  return mUdp->prim_type();
}

// @brief プリミティブの定義名を返す．
string
EiPrimHeadU::def_name() const
{
  return mUdp->def_name();
}

// @brief UDP 定義を返す．
const VlUdpDefn*
EiPrimHeadU::udp_defn() const
{
  return mUdp;
}


//////////////////////////////////////////////////////////////////////
// クラス EiPrimHeadUD
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiPrimHeadUD::EiPrimHeadUD(
  const VlScope* parent,
  const PtItem* pt_header,
  const VlUdpDefn* udp
) : EiPrimHeadU{parent, pt_header, udp}
{
}

// @brief デストラクタ
EiPrimHeadUD::~EiPrimHeadUD()
{
}

// @brief 遅延式を得る．
const VlDelay*
EiPrimHeadUD::delay() const
{
  return mDelay;
}

// @brief 遅延式を設定する．
void
EiPrimHeadUD::set_delay(
  const VlDelay* expr
)
{
  mDelay = expr;
}


//////////////////////////////////////////////////////////////////////
// クラス EiPrimHeadC
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiPrimHeadC::EiPrimHeadC(
  const VlScope* parent,
  const PtItem* pt_header,
  const ClibCell& cell
) : EiPrimHead{parent, pt_header},
    mCell{cell}
{
}

// @brief デストラクタ
EiPrimHeadC::~EiPrimHeadC()
{
}

// @brief primitive type を返す．
VpiPrimType
EiPrimHeadC::prim_type() const
{
  return VpiPrimType::Cell;
}

// @brief プリミティブの定義名を返す．
string
EiPrimHeadC::def_name() const
{
  return mCell.name();
}

// @brief セル番号を返す．
ClibCell
EiPrimHeadC::cell() const
{
  return mCell;
}


//////////////////////////////////////////////////////////////////////
// クラス EiPrimArray
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiPrimArray::EiPrimArray(
  ElbPrimHead* head,
  const PtInst* pt_inst,
  const EiRangeImpl& range
) : mHead{head},
    mPtInst{pt_inst},
    mRange{range},
    mArray(mRange.size())
{
  SizeType n = mRange.size();
  SizeType port_num = pt_inst->port_num();
  for ( SizeType i = 0; i < n; ++ i ) {
    SizeType index = mRange.index(i);
    mArray[i].init(this, index, port_num);
  }
}

// @brief コンストラクタ
EiPrimArray::EiPrimArray(
  ElbPrimHead* head,
  const ClibCell& cell,
  const PtInst* pt_inst,
  const EiRangeImpl& range
) : mHead{head},
    mPtInst{pt_inst},
    mRange{range},
    mArray(mRange.size())
{
  SizeType n = mRange.size();
  SizeType port_num = pt_inst->port_num();
  for ( SizeType i = 0; i < n; ++ i ) {
    SizeType index = mRange.index(i);
    mArray[i].init(this, index, port_num);
  }
}

// @brief デストラクタ
EiPrimArray::~EiPrimArray()
{
}

// @brief 型の取得
VpiObjType
EiPrimArray::type() const
{
  if ( udp_defn() ) {
    return VpiObjType::UdpArray;
  }
  else {
    return VpiObjType::PrimitiveArray;
  }
}

// @brief ファイル位置の取得
FileRegion
EiPrimArray::file_region() const
{
  return mPtInst->file_region();
}

// @brief このオブジェクトの属しているスコープを返す．
const VlScope*
EiPrimArray::parent_scope() const
{
  return mHead->parent_scope();
}

// @brief 名前の取得
string
EiPrimArray::name() const
{
  return mPtInst->name();
}

// @brief primitive type を返す．
VpiPrimType
EiPrimArray::prim_type() const
{
  return head()->prim_type();
}

// @brief プリミティブの定義名を返す．
string
EiPrimArray::def_name() const
{
  return head()->def_name();
}

// @brief UDP 定義を返す．
const VlUdpDefn*
EiPrimArray::udp_defn() const
{
  return head()->udp_defn();
}

// @brief セル番号を返す．
ClibCell
EiPrimArray::cell() const
{
  return head()->cell();
}

// @brief 0 の強さを得る．
VpiStrength
EiPrimArray::drive0() const
{
  return head()->drive0();
}

// @brief 1 の強さを得る．
VpiStrength
EiPrimArray::drive1() const
{
  return head()->drive1();
}

// @brief 遅延式を得る．
const VlDelay*
EiPrimArray::delay() const
{
  return head()->delay();
}

// @brief 範囲の MSB の値を返す．
int
EiPrimArray::left_range_val() const
{
  return mRange.left_range_val();
}

// @brief 範囲の LSB の値を返す．
int
EiPrimArray::right_range_val() const
{
  return mRange.right_range_val();
}

// @brief 範囲のMSBを表す文字列の取得
string
EiPrimArray::left_range_string() const
{
  return mRange.left_range_string();
}

// @brief 範囲のLSBを表す文字列の取得
string
EiPrimArray::right_range_string() const
{
  return mRange.right_range_string();
}

// @brief 要素数を返す．
SizeType
EiPrimArray::elem_num() const
{
  return mRange.size();
}

// @brief 要素のプリミティブを返す．
const VlPrimitive*
EiPrimArray::elem_by_offset(
  SizeType offset
) const
{
  return &mArray[offset];
}

// @brief 要素を返す．
const VlPrimitive*
EiPrimArray::elem_by_index(
  int index
) const
{
  SizeType offset;
  if ( mRange.calc_offset(index, offset) ) {
    return &mArray[offset];
  }
  else {
    // 範囲外
    ASSERT_NOT_REACHED;
    return nullptr;
  }
}

// @brief 要素のプリミティブを取り出す．
ElbPrimitive*
EiPrimArray::_primitive_by_offset(
  SizeType offset
)
{
  return &mArray[offset];
}

// @brief 要素のプリミティブを取り出す．
ElbPrimitive*
EiPrimArray::_primitive_by_index(
  int index
)
{
  SizeType offset;
  if ( mRange.calc_offset(index, offset) ) {
    return &mArray[offset];
  }
  else {
    // 範囲外
    ASSERT_NOT_REACHED;
    return nullptr;
  }
}

// @brief ヘッダを得る．
ElbPrimHead*
EiPrimArray::head() const
{
  return mHead;
}

// @brief パース木のインスタンス定義を得る．
const PtInst*
EiPrimArray::pt_inst() const
{
  return mPtInst;
}


//////////////////////////////////////////////////////////////////////
// クラス EiPrimitive
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiPrimitive::EiPrimitive()
{
}

// @brief デストラクタ
EiPrimitive::~EiPrimitive()
{
}

// @brief 型の取得
VpiObjType
EiPrimitive::type() const
{
  if ( udp_defn() ) {
    return VpiObjType::Udp;
  }
  else {
    return VpiObjType::Gate;
  }
}

// @brief ファイル位置の取得
FileRegion
EiPrimitive::file_region() const
{
  return pt_inst()->file_region();
}

// @brief このオブジェクトの属しているスコープを返す．
const VlScope*
EiPrimitive::parent_scope() const
{
  return head()->parent_scope();
}

// @brief primitive type を返す．
VpiPrimType
EiPrimitive::prim_type() const
{
  return head()->prim_type();
}

// @brief プリミティブの定義名を返す．
string
EiPrimitive::def_name() const
{
  return head()->def_name();
}

// @brief UPD 定義を返す．
const VlUdpDefn*
EiPrimitive::udp_defn() const
{
  return head()->udp_defn();
}

// @brief セル番号を返す．
ClibCell
EiPrimitive::cell() const
{
  return head()->cell();
}

// @brief 0 の強さを得る．
VpiStrength
EiPrimitive::drive0() const
{
  return head()->drive0();
}

// @brief 1 の強さを得る．
VpiStrength
EiPrimitive::drive1() const
{
  return head()->drive1();
}

// @brief 遅延式を得る．
const VlDelay*
EiPrimitive::delay() const
{
  return head()->delay();
}

// @brief ポート数を得る．
SizeType
EiPrimitive::port_num() const
{
  return pt_inst()->port_num();
}

// @brief ポート端子を得る．
const VlPrimTerm*
EiPrimitive::prim_term(
  SizeType pos
) const
{
  return &mPortArray[pos];
}

// @brief ポート端子のリストを得る．
vector<const VlPrimTerm*>
EiPrimitive::prim_term_list() const
{
  vector<const VlPrimTerm*> term_list;
  term_list.reserve(port_num());
  for ( auto& term: mPortArray ) {
    term_list.push_back(&term);
  }
  return term_list;
}

// @brief ポート配列を初期化する．
void
EiPrimitive::init_port(
  SizeType port_num
)
{
  mPortArray = vector<EiPrimTerm>(port_num);

  SizeType output_num;
  SizeType inout_num;
  SizeType input_num;
  int stat = get_port_size(prim_type(), port_num,
			   output_num, inout_num, input_num);
  ASSERT_COND( stat == 0 );

  SizeType index = 0;
  for ( SizeType i = 0; i < output_num; ++ i, ++ index ) {
    mPortArray[index].set(this, index, VpiDir::Output);
  }
  for ( SizeType i = 0; i < inout_num; ++ i, ++ index ) {
    mPortArray[index].set(this, index, VpiDir::Inout);
  }
  for ( SizeType i = 0; i < input_num; ++ i, ++ index ) {
    mPortArray[index].set(this, index, VpiDir::Input);
  }
}

// @brief ポート配列を初期化する．
void
EiPrimitive::init_port(
  SizeType port_num,
  const ClibCell& cell
)
{
  mPortArray = vector<EiPrimTerm>(port_num);

  for ( auto id: Range(cell.pin_num()) ) {
    auto pin = cell.pin(id);
    VpiDir dir;
    if ( pin.is_input() ) {
      dir = VpiDir::Input;
    }
    else if ( pin.is_output() ) {
      dir = VpiDir::Output;
    }
    else if ( pin.is_inout() ) {
      dir = VpiDir::Inout;
    }
    else {
      ASSERT_NOT_REACHED;
    }
    mPortArray[id].set(this, id, dir);
  }
}

// @brief 接続する．
void
EiPrimitive::connect(
  SizeType pos,
  const VlExpr* expr
)
{
  mPortArray[pos].set_expr(expr);
}


//////////////////////////////////////////////////////////////////////
// クラス EiPrimitive1
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiPrimitive1::EiPrimitive1()
{
}

// @brief デストラクタ
EiPrimitive1::~EiPrimitive1()
{
}

// @brief 初期設定を行う．
void
EiPrimitive1::init(
  EiPrimArray* prim_array,
  SizeType index,
  SizeType port_num
)
{
  mPrimArray = prim_array;
  mIndex = index;

  init_port(port_num);
}

// @brief 初期設定を行う．
void
EiPrimitive1::init(
  EiPrimArray* prim_array,
  SizeType index,
  SizeType port_num,
  const ClibCell& cell
)
{
  mPrimArray = prim_array;
  mIndex = index;

  init_port(port_num, cell);
}

// @brief 名前の取得
string
EiPrimitive1::name() const
{
  ostringstream buf;
  buf << mPrimArray->name() << "[" << mIndex << "]";
  return buf.str();
}

// @brief ヘッダを得る．
ElbPrimHead*
EiPrimitive1::head() const
{
  return mPrimArray->head();
}

// @brief パース木のインスタンス定義を得る．
const PtInst*
EiPrimitive1::pt_inst() const
{
  return mPrimArray->pt_inst();
}


//////////////////////////////////////////////////////////////////////
// クラス EiPrimitive2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiPrimitive2::EiPrimitive2(
  ElbPrimHead* head,
  const PtInst* pt_inst
) : mHead{head},
    mPtInst{pt_inst}
{
  init_port(pt_inst->port_num());
}

// @brief コンストラクタ
EiPrimitive2::EiPrimitive2(
  ElbPrimHead* head,
  const ClibCell& cell,
  const PtInst* pt_inst
) : mHead{head},
    mPtInst{pt_inst}
{
  init_port(pt_inst->port_num(), cell);
}

// @brief デストラクタ
EiPrimitive2::~EiPrimitive2()
{
}

// @brief 名前の取得
string
EiPrimitive2::name() const
{
  return mPtInst->name();
}

// @brief ヘッダを得る．
ElbPrimHead*
EiPrimitive2::head() const
{
  return mHead;
}

// @brief パース木のインスタンス定義を得る．
const PtInst*
EiPrimitive2::pt_inst() const
{
  return mPtInst;
}


//////////////////////////////////////////////////////////////////////
// クラス EiPrimTerm
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiPrimTerm::EiPrimTerm()
{
}

// @brief デストラクタ
EiPrimTerm::~EiPrimTerm()
{
}

// @brief 型の取得
VpiObjType
EiPrimTerm::type() const
{
  return VpiObjType::PrimTerm;
}

// @brief ファイル位置の取得
FileRegion
EiPrimTerm::file_region() const
{
  return mPrimitive->file_region();
}

// @brief 親のプリミティブを返す．
const VlPrimitive*
EiPrimTerm::primitive() const
{
  return mPrimitive;
}

// @brief 入出力の種類を返す．
VpiDir
EiPrimTerm::direction() const
{
  return static_cast<VpiDir>( (mIndexDir & 7U) );
}

// @brief 端子番号を返す．
SizeType
EiPrimTerm::term_index() const
{
  return (mIndexDir >> 3);
}

// @brief 接続しているネットを表す式を返す．
const VlExpr*
EiPrimTerm::expr() const
{
  return mExpr;
}

// @brief 内容を設定する．
void
EiPrimTerm::set(
  const VlPrimitive* primitive,
  SizeType index,
  VpiDir dir
)
{
  mPrimitive = primitive;
  mIndexDir = (index << 3) | static_cast<std::uint32_t>(dir);
}

// @brief 接続している式を設定する．
void
EiPrimTerm::set_expr(
  const VlExpr* expr
)
{
  mExpr = expr;
}

END_NAMESPACE_YM_VERILOG
