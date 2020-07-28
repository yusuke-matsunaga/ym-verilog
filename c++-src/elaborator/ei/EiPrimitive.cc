
/// @file EiPrimitive.cc
/// @brief EiPrimitive の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "EiFactory.h"
#include "EiPrimitive.h"

#include "elb/ElbUdp.h"
#include "elb/ElbExpr.h"
#include "elb/ElbDelay.h"

#include "ym/pt/PtItem.h"
#include "ym/pt/PtMisc.h"

#include "ym/ClibCell.h"
#include "ym/ClibCellPin.h"
#include "ym/ClibObjList.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief ゲートプリミティブのヘッダを生成する．
// @param[in] parent 親のスコープ
// @param[in] pt_header パース木の定義
// @param[in] has_delay 遅延値を持つとき true
ElbPrimHead*
EiFactory::new_PrimHead(const VlNamedObj* parent,
			const PtItem* pt_header,
			bool has_delay)
{
  EiPrimHead* head = nullptr;
  if ( has_delay ) {
    head = new EiPrimHeadD(parent, pt_header);
  }
  else {
    head = new EiPrimHead(parent, pt_header);
  }
  return head;
}

// @brief UDPプリミティブのヘッダを生成する．
// @param[in] parent 親のスコープ
// @param[in] pt_header パース木の定義
// @param[in] udp UDP 定義
// @param[in] has_delay 遅延値を持つとき true
ElbPrimHead*
EiFactory::new_UdpHead(const VlNamedObj* parent,
		       const PtItem* pt_header,
		       const ElbUdpDefn* udp,
		       bool has_delay)
{
  EiPrimHead* head = nullptr;
  if ( has_delay ) {
    head = new EiPrimHeadUD(parent, pt_header, udp);
  }
  else {
    head = new EiPrimHeadU(parent, pt_header, udp);
  }
  return head;
}

// @brief セルプリミティブのヘッダを生成する．
// @param[in] parent 親のスコープ
// @param[in] pt_header パース木の定義
// @param[in] cell_id セル番号
ElbPrimHead*
EiFactory::new_CellHead(const VlNamedObj* parent,
			const PtItem* pt_header,
			int cell_id)
{
  auto head = new EiPrimHeadC(parent, pt_header, cell_id);
  return head;
}

// @brief プリミティブインスタンスを生成する．
// @param[in] head ヘッダ
// @param[in] pt_inst インスタンス定義
ElbPrimitive*
EiFactory::new_Primitive(ElbPrimHead* head,
			 const PtInst* pt_inst)
{
  SizeType port_num = pt_inst->port_num();
  EiPrimTerm* term_array = new EiPrimTerm[port_num];
  EiPrimitive* prim = new EiPrimitive2(head, pt_inst, term_array);

  return prim;
}

// @brief プリミティブ配列インスタンスを生成する．
// @param[in] head ヘッダ
// @param[in] pt_inst インスタンス定義
// @param[in] left 範囲の MSB の式
// @param[in] right 範囲の LSB の式
// @param[in] left_val 範囲の MSB の値
// @param[in] right_val 範囲の LSB の値
ElbPrimArray*
EiFactory::new_PrimitiveArray(ElbPrimHead* head,
			      const PtInst* pt_inst,
			      const PtExpr* left,
			      const PtExpr* right,
			      int left_val,
			      int right_val)
{
  EiRangeImpl range;
  range.set(left, right, left_val, right_val);
  SizeType n = range.size();
  EiPrimitive1* array = new EiPrimitive1[n];

  SizeType nt = n * pt_inst->port_num();
  EiPrimTerm* term_array = new EiPrimTerm[nt];
  EiPrimArray* prim_array = new EiPrimArray(head, pt_inst, range,
					    array, term_array);

  return prim_array;
}

// @brief セルプリミティブインスタンスを生成する．
// @param[in] head ヘッダ
// @param[in] cell セル
// @param[in] pt_inst インスタンス定義
ElbPrimitive*
EiFactory::new_CellPrimitive(ElbPrimHead* head,
			     const ClibCell& cell,
			     const PtInst* pt_inst)
{
  SizeType port_num = pt_inst->port_num();
  EiPrimTerm* term_array = new EiPrimTerm[port_num];
  EiPrimitive* prim = new EiPrimitive2(head, cell, pt_inst, term_array);

  return prim;
}

// @brief セルプリミティブ配列インスタンスを生成する．
// @param[in] head ヘッダ
// @param[in] cell セル
// @param[in] pt_inst インスタンス定義
// @param[in] left 範囲の MSB の式
// @param[in] right 範囲の LSB の式
// @param[in] left_val 範囲の MSB の値
// @param[in] right_val 範囲の LSB の値
ElbPrimArray*
EiFactory::new_CellPrimitiveArray(ElbPrimHead* head,
				  const ClibCell& cell,
				  const PtInst* pt_inst,
				  const PtExpr* left,
				  const PtExpr* right,
				  int left_val,
				  int right_val)
{
  EiRangeImpl range;
  range.set(left, right, left_val, right_val);
  SizeType n = range.size();
  EiPrimitive1* array = new EiPrimitive1[n];

  SizeType nt = n * pt_inst->port_num();
  EiPrimTerm* term_array = new EiPrimTerm[nt];
  EiPrimArray* prim_array = new EiPrimArray(head, cell, pt_inst, range,
					    array, term_array);

  return prim_array;
}


//////////////////////////////////////////////////////////////////////
// クラス EiPrimHead
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のスコープ
// @param[in] pt_header パース木の定義
EiPrimHead::EiPrimHead(const VlNamedObj* parent,
		       const PtItem* pt_header) :
  mParent(parent),
  mPtHead(pt_header)
{
}

// @brief デストラクタ
EiPrimHead::~EiPrimHead()
{
}

// @brief このオブジェクトの属しているスコープを返す．
const VlNamedObj*
EiPrimHead::parent() const
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
const char*
EiPrimHead::def_name() const
{
  const char* nm = nullptr;
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
// @note このクラスでは nullptr を返す．
const ElbUdpDefn*
EiPrimHead::udp_defn() const
{
  return nullptr;
}

// @brief セル番号を返す．
int
EiPrimHead::cell_id() const
{
  return -1;
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
// @note このクラスでは nullptr を返す．
ElbDelay*
EiPrimHead::delay() const
{
  return nullptr;
}

// @brief 遅延式を設定する．
void
EiPrimHead::set_delay(ElbDelay* expr)
{
}


//////////////////////////////////////////////////////////////////////
// クラス EiPrimHeadD
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のスコープ
// @param[in] pt_header パース木の定義
EiPrimHeadD::EiPrimHeadD(const VlNamedObj* parent,
			 const PtItem* pt_header) :
  EiPrimHead(parent, pt_header)
{
}

// @brief デストラクタ
EiPrimHeadD::~EiPrimHeadD()
{
}

// @brief 遅延式を得る．
ElbDelay*
EiPrimHeadD::delay() const
{
  return mDelay;
}

// @brief 遅延式を設定する．
void
EiPrimHeadD::set_delay(ElbDelay* expr)
{
  mDelay = expr;
}


//////////////////////////////////////////////////////////////////////
// クラス EiPrimHeadU
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のスコープ
// @param[in] pt_header パース木の定義
// @param[in] udp UDP 定義
EiPrimHeadU::EiPrimHeadU(const VlNamedObj* parent,
			 const PtItem* pt_header,
			 const ElbUdpDefn* udp) :
  EiPrimHead(parent, pt_header),
  mUdp(udp)
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
const char*
EiPrimHeadU::def_name() const
{
  return mUdp->def_name();
}

// @brief UDP 定義を返す．
const ElbUdpDefn*
EiPrimHeadU::udp_defn() const
{
  return mUdp;
}


//////////////////////////////////////////////////////////////////////
// クラス EiPrimHeadUD
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のスコープ
// @param[in] pt_header パース木の定義
// @param[in] udp UDP 定義
EiPrimHeadUD::EiPrimHeadUD(const VlNamedObj* parent,
			   const PtItem* pt_header,
			   const ElbUdpDefn* udp) :
  EiPrimHeadU(parent, pt_header, udp)
{
}

// @brief デストラクタ
EiPrimHeadUD::~EiPrimHeadUD()
{
}

// @brief 遅延式を得る．
ElbDelay*
EiPrimHeadUD::delay() const
{
  return mDelay;
}

// @brief 遅延式を設定する．
void
EiPrimHeadUD::set_delay(ElbDelay* expr)
{
  mDelay = expr;
}


//////////////////////////////////////////////////////////////////////
// クラス EiPrimHeadC
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のスコープ
// @param[in] pt_header パース木の定義
// @param[in] cell_id セル番号
EiPrimHeadC::EiPrimHeadC(const VlNamedObj* parent,
			 const PtItem* pt_header,
			 int cell_id) :
  EiPrimHead(parent, pt_header),
  mCellId(cell_id)
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
const char*
EiPrimHeadC::def_name() const
{
#warning "TODO: 未完"
  //return mCell->name().c_str();
  return "";
}

// @brief セル番号を返す．
int
EiPrimHeadC::cell_id() const
{
  return mCellId;
}


//////////////////////////////////////////////////////////////////////
// クラス EiPrimArray
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] head ヘッダ
// @param[in] pt_inst インスタンス定義
// @param[in] range 範囲
// @param[in] elem_array 要素の配列
// @param[in] term_array 端子の配列
EiPrimArray::EiPrimArray(ElbPrimHead* head,
			 const PtInst* pt_inst,
			 const EiRangeImpl& range,
			 EiPrimitive1* elem_array,
			 EiPrimTerm* term_array) :
  mHead(head),
  mPtInst(pt_inst),
  mRange(range),
  mArray(elem_array)
{
  SizeType n = mRange.size();
  SizeType port_num = pt_inst->port_num();
  for ( int i = 0; i < n; ++ i ) {
    int index = mRange.index(i);
    mArray[i].init(this, index, term_array);
    term_array += port_num;
  }
}

// @brief コンストラクタ
// @param[in] head ヘッダ
// @param[in] cell セル
// @param[in] pt_inst インスタンス定義
// @param[in] range 範囲
// @param[in] elem_array 要素の配列
// @param[in] term_array 端子の配列
EiPrimArray::EiPrimArray(ElbPrimHead* head,
			 const ClibCell& cell,
			 const PtInst* pt_inst,
			 const EiRangeImpl& range,
			 EiPrimitive1* elem_array,
			 EiPrimTerm* term_array) :
  mHead(head),
  mPtInst(pt_inst),
  mRange(range),
  mArray(elem_array)
{
  SizeType n = mRange.size();
  SizeType port_num = pt_inst->port_num();
  for ( int i = 0; i < n; ++ i ) {
    int index = mRange.index(i);
    mArray[i].init(this, index, term_array);
    term_array += port_num;
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
const VlNamedObj*
EiPrimArray::parent() const
{
  return mHead->parent();
}

// @brief 名前の取得
const char*
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
const char*
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
int
EiPrimArray::cell_id() const
{
  return head()->cell_id();
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
// @param[in] offset 位置番号 ( 0 <= offset < elem_num() )
const VlPrimitive*
EiPrimArray::elem_by_offset(SizeType offset) const
{
  return &mArray[offset];
}

// @brief 要素を返す．
// @param[in] index インデックス
const VlPrimitive*
EiPrimArray::elem_by_index(int index) const
{
  int offset;
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
EiPrimArray::_primitive_by_offset(int offset) const
{
  return &mArray[offset];
}

// @brief 要素のプリミティブを取り出す．
ElbPrimitive*
EiPrimArray::_primitive_by_index(int index) const
{
  int offset;
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
EiPrimitive::EiPrimitive() :
  mPortNum(0),
  mPortArray(nullptr)
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
const VlNamedObj*
EiPrimitive::parent() const
{
  return head()->parent();
}

// @brief primitive type を返す．
VpiPrimType
EiPrimitive::prim_type() const
{
  return head()->prim_type();
}

// @brief プリミティブの定義名を返す．
const char*
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
int
EiPrimitive::cell_id() const
{
  return head()->cell_id();
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
// @param[in] pos 位置番号 (0 <= pos < port_num())
const VlPrimTerm*
EiPrimitive::prim_term(SizeType pos) const
{
  return &mPortArray[pos];
}

// @brief ポート配列を初期化する．
// @param[in] term_array 端子の配列
void
EiPrimitive::init_port(EiPrimTerm* term_array)
{
  mPortArray = term_array;

  SizeType output_num;
  SizeType inout_num;
  SizeType input_num;
  int stat = get_port_size(prim_type(), port_num(),
			   output_num, inout_num, input_num);
  ASSERT_COND( stat == 0 );

  int index = 0;
  for ( int i = 0; i < output_num; ++ i, ++ index ) {
    mPortArray[index].set(this, index, VpiDir::Output);
  }
  for ( int i = 0; i < inout_num; ++ i, ++ index ) {
    mPortArray[index].set(this, index, VpiDir::Inout);
  }
  for ( int i = 0; i < input_num; ++ i, ++ index ) {
    mPortArray[index].set(this, index, VpiDir::Input);
  }
}

// @brief ポート配列を初期化する．
// @param[in] term_array 端子の配列
// @param[in] cell セル
void
EiPrimitive::init_port(EiPrimTerm* term_array,
		       const ClibCell& cell)
{
  mPortArray = term_array;
  for ( auto& pin: cell.pin_list() ) {
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
    int pid = pin.pin_id();
    mPortArray[pid].set(this, pid, dir);
  }
}

// @brief 接続する．
// @param[in] pos ポート番号 (0 から始まる)
// @param[in] expr 接続する式
void
EiPrimitive::connect(int pos,
		     ElbExpr* expr)
{
  mPortArray[pos].mExpr = expr;
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
// @param[in] prim_array 親の配列
// @param[in] index インデックス番号
// @param[in] term_array 端子の配列
void
EiPrimitive1::init(EiPrimArray* prim_array,
		   int index,
		   EiPrimTerm* term_array)
{
  mPrimArray = prim_array;
  mIndex = index;

  init_port(term_array);

  // 名前の生成
  ostringstream buf;
  buf << prim_array->name() << "[" << index << "]";
  mName = buf.str().c_str();
}

// @brief 初期設定を行う．
// @param[in] prim_array 親の配列
// @param[in] index インデックス番号
// @param[in] cell セル
// @param[in] term_array 端子の配列
void
EiPrimitive1::init(EiPrimArray* prim_array,
		   int index,
		   const ClibCell& cell,
		   EiPrimTerm* term_array)
{
  mPrimArray = prim_array;
  mIndex = index;

  init_port(term_array, cell);

  // 名前の生成
  ostringstream buf;
  buf << prim_array->name() << "[" << index << "]";
  mName = buf.str().c_str();
}

// @brief 名前の取得
const char*
EiPrimitive1::name() const
{
  return mName;
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
// @param[in] head ヘッダ
// @param[in] pt_inst インスタンス定義
// @param[in] term_array 端子の配列
EiPrimitive2::EiPrimitive2(ElbPrimHead* head,
			   const PtInst* pt_inst,
			   EiPrimTerm* term_array) :
  mHead(head),
  mPtInst(pt_inst)
{
  init_port(term_array);
}

// @brief コンストラクタ
// @param[in] head ヘッダ
// @param[in] cell セル
// @param[in] pt_inst インスタンス定義
// @param[in] term_array 端子の配列
EiPrimitive2::EiPrimitive2(ElbPrimHead* head,
			   const ClibCell& cell,
			   const PtInst* pt_inst,
			   EiPrimTerm* term_array) :
  mHead(head),
  mPtInst(pt_inst)
{
  init_port(term_array, cell);
}

// @brief デストラクタ
EiPrimitive2::~EiPrimitive2()
{
}

// @brief 名前の取得
const char*
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
EiPrimTerm::set(ElbPrimitive* primitive,
		int index,
		VpiDir dir)
{
  mPrimitive = primitive;
  mIndexDir = (index << 3) | static_cast<ymuint32>(dir);
}

END_NAMESPACE_YM_VERILOG
