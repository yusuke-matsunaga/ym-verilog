﻿
/// @file EiPrimitive.cc
/// @brief EiPrimitive の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "EiFactory.h"
#include "EiPrimitive.h"
#include "ElbUdp.h"
#include "ElbExpr.h"
#include "ElbDelay.h"

#include "ym/pt/PtItem.h"
#include "ym/pt/PtMisc.h"
#include "ym/Cell.h"
#include "ym/CellPin.h"


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
    void* p = mAlloc.get_memory(sizeof(EiPrimHeadD));
    head = new (p) EiPrimHeadD(parent, pt_header);
  }
  else {
    void* p = mAlloc.get_memory(sizeof(EiPrimHead));
    head = new (p) EiPrimHead(parent, pt_header);
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
    void* p = mAlloc.get_memory(sizeof(EiPrimHeadUD));
    head = new (p) EiPrimHeadUD(parent, pt_header, udp);
  }
  else {
    void* p = mAlloc.get_memory(sizeof(EiPrimHeadU));
    head = new (p) EiPrimHeadU(parent, pt_header, udp);
  }
  return head;
}

// @brief セルプリミティブのヘッダを生成する．
// @param[in] parent 親のスコープ
// @param[in] pt_header パース木の定義
// @param[in] cell セル
ElbPrimHead*
EiFactory::new_CellHead(const VlNamedObj* parent,
			const PtItem* pt_header,
			const Cell* cell)
{
  EiPrimHead* head = nullptr;
  void* p = mAlloc.get_memory(sizeof(EiPrimHeadC));
  head = new (p) EiPrimHeadC(parent, pt_header, cell);
  return head;
}

// @brief プリミティブインスタンスを生成する．
// @param[in] head ヘッダ
// @param[in] pt_inst インスタンス定義
ElbPrimitive*
EiFactory::new_Primitive(ElbPrimHead* head,
			 const PtInst* pt_inst)
{
  ymuint port_num = pt_inst->port_num();
  void* q = mAlloc.get_memory(sizeof(EiPrimTerm) * port_num);
  EiPrimTerm* term_array = new (q) EiPrimTerm[port_num];

  void* p = mAlloc.get_memory(sizeof(EiPrimitive2));
  EiPrimitive* prim = new (p) EiPrimitive2(head, pt_inst, term_array);

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
  ymuint n = range.size();
  void* q = mAlloc.get_memory(sizeof(EiPrimitive1) * n);
  EiPrimitive1* array = new (q) EiPrimitive1[n];

  ymuint nt = n * pt_inst->port_num();
  void* r = mAlloc.get_memory(sizeof(EiPrimTerm) * nt);
  EiPrimTerm* term_array = new (r) EiPrimTerm[nt];

  void* p = mAlloc.get_memory(sizeof(EiPrimArray));
  EiPrimArray* prim_array = new (p) EiPrimArray(head, pt_inst, range,
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
tVpiPrimType
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
  case kVpiAndPrim:      nm = "and"; break;
  case kVpiNandPrim:     nm = "nand"; break;
  case kVpiNorPrim:      nm = "nor"; break;
  case kVpiOrPrim:       nm = "or"; break;
  case kVpiXorPrim:      nm = "xor"; break;
  case kVpiXnorPrim:     nm = "xnor"; break;
  case kVpiBufPrim:      nm = "buf"; break;
  case kVpiNotPrim:      nm = "not"; break;
  case kVpiBufif0Prim:   nm = "bufif0"; break;
  case kVpiBufif1Prim:   nm = "bufif1"; break;
  case kVpiNotif0Prim:   nm = "notif0"; break;
  case kVpiNotif1Prim:   nm = "notif1"; break;
  case kVpiNmosPrim:     nm = "nmos"; break;
  case kVpiPmosPrim:     nm = "pmos"; break;
  case kVpiCmosPrim:     nm = "cmos"; break;
  case kVpiRnmosPrim:    nm = "rnmos"; break;
  case kVpiRpmosPrim:    nm = "rpmos"; break;
  case kVpiRcmosPrim:    nm = "rcmos"; break;
  case kVpiRtranPrim:    nm = "rtran"; break;
  case kVpiRtranif0Prim: nm = "rtranif0"; break;
  case kVpiRtranif1Prim: nm = "rtranif1"; break;
  case kVpiTranPrim:     nm = "tran"; break;
  case kVpiTranif0Prim:  nm = "tranif0"; break;
  case kVpiTranif1Prim:  nm = "tranif1"; break;
  case kVpiPullupPrim:   nm = "pullup"; break;
  case kVpiPulldownPrim: nm = "pulldown"; break;
  case kVpiCellPrim:     nm = "cell"; break;
  case kVpiSeqPrim:
  case kVpiCombPrim:
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

// @brief セルを返す．
const Cell*
EiPrimHead::cell() const
{
  return nullptr;
}

// @brief 0 の強さを得る．
tVpiStrength
EiPrimHead::drive0() const
{
  if ( mPtHead->strength() ) {
    return mPtHead->strength()->drive0();
  }
  return kVpiNoStrength;
}

// @brief 1 の強さを得る．
tVpiStrength
EiPrimHead::drive1() const
{
  if ( mPtHead->strength() ) {
    return mPtHead->strength()->drive1();
  }
  return kVpiNoStrength;
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
tVpiPrimType
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
// @param[in] cell セル
EiPrimHeadC::EiPrimHeadC(const VlNamedObj* parent,
			 const PtItem* pt_header,
			 const Cell* cell) :
  EiPrimHead(parent, pt_header),
  mCell(cell)
{
}

// @brief デストラクタ
EiPrimHeadC::~EiPrimHeadC()
{
}

// @brief primitive type を返す．
tVpiPrimType
EiPrimHeadC::prim_type() const
{
  return kVpiCellPrim;
}

// @brief プリミティブの定義名を返す．
const char*
EiPrimHeadC::def_name() const
{
  return mCell->name().c_str();
}

// @brief セルを返す．
const Cell*
EiPrimHeadC::cell() const
{
  return mCell;
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
  ymuint n = mRange.size();
  ymuint port_num = pt_inst->port_num();
  for (ymuint i = 0; i < n; ++ i) {
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
tVpiObjType
EiPrimArray::type() const
{
  if ( udp_defn() ) {
    return kVpiUdpArray;
  }
  else {
    return kVpiPrimitiveArray;
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
tVpiPrimType
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

// @brief セルを返す．
const Cell*
EiPrimArray::cell() const
{
  return head()->cell();
}

// @brief 0 の強さを得る．
tVpiStrength
EiPrimArray::drive0() const
{
  return head()->drive0();
}

// @brief 1 の強さを得る．
tVpiStrength
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
ymuint
EiPrimArray::elem_num() const
{
  return mRange.size();
}

// @brief 要素のプリミティブを返す．
// @param[in] offset 位置番号 ( 0 <= offset < elem_num() )
const VlPrimitive*
EiPrimArray::elem_by_offset(ymuint offset) const
{
  return &mArray[offset];
}

// @brief 要素を返す．
// @param[in] index インデックス
const VlPrimitive*
EiPrimArray::elem_by_index(int index) const
{
  ymuint offset;
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
EiPrimArray::_primitive_by_offset(ymuint offset) const
{
  return &mArray[offset];
}

// @brief 要素のプリミティブを取り出す．
ElbPrimitive*
EiPrimArray::_primitive_by_index(int index) const
{
  ymuint offset;
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
tVpiObjType
EiPrimitive::type() const
{
  if ( udp_defn() ) {
    return kVpiUdp;
  }
  else {
    return kVpiGate;
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
tVpiPrimType
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

// @brief セルを返す．
const Cell*
EiPrimitive::cell() const
{
  return head()->cell();
}

// @brief 0 の強さを得る．
tVpiStrength
EiPrimitive::drive0() const
{
  return head()->drive0();
}

// @brief 1 の強さを得る．
tVpiStrength
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
ymuint
EiPrimitive::port_num() const
{
  return pt_inst()->port_num();
}

// @brief ポート端子を得る．
// @param[in] pos 位置番号 (0 <= pos < port_num())
const VlPrimTerm*
EiPrimitive::prim_term(ymuint pos) const
{
  return &mPortArray[pos];
}

// @brief ポート配列を初期化する．
// @param[in] term_array 端子の配列
void
EiPrimitive::init_port(EiPrimTerm* term_array)
{
  mPortArray = term_array;

  const Cell* cell = this->cell();
  if ( cell == nullptr ) {
    ymuint output_num;
    ymuint inout_num;
    ymuint input_num;
    int stat = get_port_size(prim_type(), port_num(),
			     output_num, inout_num, input_num);
    ASSERT_COND(stat == 0 );

    ymuint index = 0;
    for (ymuint i = 0; i < output_num; ++ i, ++ index) {
      mPortArray[index].set(this, index, kVlOutput);
    }
    for (ymuint i = 0; i < inout_num; ++ i, ++ index) {
      mPortArray[index].set(this, index, kVlInout);
    }
    for (ymuint i = 0; i < input_num; ++ i, ++ index) {
      mPortArray[index].set(this, index, kVlInput);
    }
  }
  else {
    ymuint n = cell->pin_num();
    for (ymuint i = 0; i < n; ++ i) {
      const CellPin* pin = cell->pin(i);
      tVlDirection dir;
      if ( pin->is_input() ) {
	dir = kVlInput;
      }
      else if ( pin->is_output() ) {
	dir = kVlOutput;
      }
      else if ( pin->is_inout() ) {
	dir = kVlInout;
      }
      else {
	ASSERT_NOT_REACHED;
      }
      mPortArray[i].set(this, i, dir);
    }
  }
}

// @brief 接続する．
// @param[in] pos ポート番号 (0 から始まる)
// @param[in] expr 接続する式
void
EiPrimitive::connect(ymuint pos,
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
tVpiObjType
EiPrimTerm::type() const
{
  return kVpiPrimTerm;
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
tVlDirection
EiPrimTerm::direction() const
{
  return static_cast<tVlDirection>( (mIndexDir & 7U) );
}

// @brief 端子番号を返す．
ymuint
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
		ymuint index,
		tVlDirection dir)
{
  mPrimitive = primitive;
  mIndexDir = (index << 3) | static_cast<ymuint32>(dir);
}

END_NAMESPACE_YM_VERILOG
