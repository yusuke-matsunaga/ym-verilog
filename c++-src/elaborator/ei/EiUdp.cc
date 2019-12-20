﻿
/// @file EiUdp.cc
/// @brief EiUdp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "EiFactory.h"
#include "EiUdp.h"

#include "elb/ElbExpr.h"

#include "ym/pt/PtUdp.h"
#include "ym/pt/PtDecl.h"
#include "ym/pt/PtArray.h"
#include "ym/pt/PtExpr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief UDP定義を生成する．
// @param[in] pt_udp パース木の UDP 定義
// @param[in] is_protected プロテクト属性
ElbUdpDefn*
EiFactory::new_UdpDefn(const PtUdp* pt_udp,
		       bool is_protected)
{
  SizeType port_num = pt_udp->port_list().size();
  EiUdpIO* iodecl = new EiUdpIO[port_num];
  SizeType table_size = pt_udp->table_array().size();
  EiTableEntry* table = new EiTableEntry[table_size];

  SizeType row_size = port_num;
  if ( pt_udp->prim_type() == VpiPrimType::Seq ) {
    ++ row_size;
  }
  SizeType vsize = row_size * table_size;
  VlUdpVal* val_array = new VlUdpVal[vsize];

  EiUdpDefn* udp = new EiUdpDefn(pt_udp, is_protected,
				 port_num, iodecl,
				 table_size, table,
				 val_array);
  for ( int i = 0; i < port_num; ++ i ) {
    iodecl[i].mUdp = udp;
  }
  for ( int i = 0; i < table_size; ++ i ) {
    table[i].mUdp = udp;
    table[i].mValArray = val_array;
    val_array += row_size;
  }

  return udp;
}


//////////////////////////////////////////////////////////////////////
// クラス EiUdpDefn
//////////////////////////////////////////////////////////////////////

// @param[in] pt_udp パース木の UDP 定義
// @param[in] is_protected プロテクト属性
// @param[in] io_num ポート数
// @param[in] io_array IOの配列
// @param[in] table_num テーブルの行数
// @param[in] table テーブル
// @param[in] val_array テーブル中の値を納める配列
EiUdpDefn::EiUdpDefn(const PtUdp* pt_udp,
		     bool is_protected,
		     SizeType io_num,
		     EiUdpIO* io_array,
		     SizeType table_num,
		     EiTableEntry* table,
		     VlUdpVal* val_array) :
  mPtUdp(pt_udp),
  mPortNum(io_num),
  mProtected(is_protected),
  mIODeclList(io_array),
  mInitExpr(nullptr),
  mInitVal(VlScalarVal::x()),
  mTableEntrySize(table_num),
  mTableEntryList(table),
  mValArray(val_array)
{
}

// @brief デストラクタ
EiUdpDefn::~EiUdpDefn()
{
}

// @brief 型の取得
VpiObjType
EiUdpDefn::type() const
{
  return VpiObjType::UdpDefn;
}

// @brief ファイル位置を返す．
FileRegion
EiUdpDefn::file_region() const
{
  return mPtUdp->file_region();
}

// @brief 定義された名前を返す．
const char*
EiUdpDefn::def_name() const
{
  return mPtUdp->name();
}

// @brief primitive type を返す．
VpiPrimType
EiUdpDefn::prim_type() const
{
  return mPtUdp->prim_type();
}

// @brief ポート数を返す．
SizeType
EiUdpDefn::port_num() const
{
  return mPortNum;
}

// @brief 入力の宣言要素を返す．
// @param[in] pos 入力番号
const VlIODecl*
EiUdpDefn::input(SizeType pos) const
{
  return &mIODeclList[pos];
}

// @brief 出力の宣言要素を返す．
const VlIODecl*
EiUdpDefn::output() const
{
  return &mIODeclList[mPortNum - 1];
}

// @brief protected かどうかを返す．
bool
EiUdpDefn::is_protected() const
{
  return mProtected;
}

// @brief 初期値を返す．
// @return 0/1/X を返す．
VlScalarVal
EiUdpDefn::init_val() const
{
  return mInitVal;
}

// @brief 初期値を表す文字列を返す．
string
EiUdpDefn::init_val_string() const
{
  if ( mInitExpr ) {
    return mInitExpr->decompile();
  }
  else {
    return string();
  }
}

// @brief table entry の行数を返す．
SizeType
EiUdpDefn::table_size() const
{
  return mTableEntrySize;
}

// @brief table entry を返す．
// @param[in] pos 行番号
const VlTableEntry*
EiUdpDefn::table_entry(SizeType pos) const
{
  return &mTableEntryList[pos];
}

// @brief 入出力オブジェクトの内容を設定する．
// @param[in] pos ポート中の位置
// @param[in] file_region ソースファイル上の位置
// @param[in] name 名前
// @param[in] dir 向き
void
EiUdpDefn::set_io(SizeType pos,
		  const PtIOHead* pt_header,
		  const PtIOItem* pt_item)
{
  mIODeclList[pos].set(pt_header, pt_item);
}

// @brief 初期値を設定する．
// @param[in] init_expr 初期値を表す式
// @param[in] init_val 初期値
void
EiUdpDefn::set_initial(const PtExpr* init_expr,
		       const VlScalarVal& init_val)
{
  mInitExpr = init_expr;
  mInitVal = init_val;
}

// @brief table entry の内容を設定する．
// @param[in] pos 行番号
// @param[in] pt_udp_entry パース木の一行分の定義
// @param[in] vals シンボル値の配列
void
EiUdpDefn::set_tableentry(SizeType pos,
			  const PtUdpEntry* pt_udp_entry,
			  const vector<VlUdpVal>& vals)
{
  mTableEntryList[pos].set(pt_udp_entry, vals);
}


//////////////////////////////////////////////////////////////////////
/// クラス EiUdpIO
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiUdpIO::EiUdpIO()
{
}

// @brief デストラクタ
EiUdpIO::~EiUdpIO()
{
}

// @brief 型の取得
VpiObjType
EiUdpIO::type() const
{
  return VpiObjType::IODecl;
}

// ファイル位置を返す．
FileRegion
EiUdpIO::file_region() const
{
  return mPtItem->file_region();
}

// @brief 名前を返す．
const char*
EiUdpIO::name() const
{
  return mPtItem->name();
}

// @brief 方向を返す．
VpiDir
EiUdpIO::direction() const
{
  switch ( mPtHeader->type() ) {
  case kPtIO_Input:  return VpiDir::Input;
  case kPtIO_Output: return VpiDir::Output;
  default: ASSERT_NOT_REACHED;
  }
  // ダミー
  return VpiDir::NoDirection;
}

// @brief 符号の属性の取得
// @return 符号付きのとき true を返す．
// @note このクラスは false を返す．
bool
EiUdpIO::is_signed() const
{
  return false;
}

// @brief 範囲指定を持つとき true を返す．
bool
EiUdpIO::has_range() const
{
  return false;
}

// @brief MSB の値を返す．
// @note 範囲を持たないときの値は不定
int
EiUdpIO::left_range_val() const
{
  return 0;
}

// @brief LSB の値を返す．
// @note 範囲を持たないときの値は不定
int
EiUdpIO::right_range_val() const
{
  return 0;
}

// @brief 範囲のMSBを表す文字列の取得
// @note 範囲を持たない時の値は不定
string
EiUdpIO::left_range_string() const
{
  return string();
}

// @brief 範囲のLSBを表す文字列の取得
// @note 範囲を持たない時の値は不定
string
EiUdpIO::right_range_string() const
{
  return string();
}

// @brief サイズを返す．
// このクラスは 1 を返す．
SizeType
EiUdpIO::bit_size() const
{
  return 1;
}

// @brief 対応する宣言要素を返す．
// @note このクラスでは nullptr を返す．
const VlDecl*
EiUdpIO::decl() const
{
  return nullptr;
}

// @brief 親のモジュールの取得
// @return このクラスは nullptr を返す．
const VlModule*
EiUdpIO::module() const
{
  return nullptr;
}

// @brief 親のUDP定義を返す．
const VlUdpDefn*
EiUdpIO::udp_defn() const
{
  return mUdp;
}

// @brief 親のタスク/の取得
// @return このクラスは nullptr を返す．
const VlTaskFunc*
EiUdpIO::task() const
{
  return nullptr;
}

// @brief 親の関数の取得
// @return このクラスは nullptr を返す．
const VlTaskFunc*
EiUdpIO::function() const
{
  return nullptr;
}

// @brief 内容を設定する．
// @param[in] pt_header パース木のIO宣言ヘッダ
// @param[in] pt_item パース木のIO宣言定義
void
EiUdpIO::set(const PtIOHead* pt_header,
	     const PtIOItem* pt_item)
{
  mPtHeader = pt_header;
  mPtItem = pt_item;
}


//////////////////////////////////////////////////////////////////////
/// クラス EiTableEntry
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiTableEntry::EiTableEntry()
{
}

// @brief デストラクタ
EiTableEntry::~EiTableEntry()
{
}

// @brief 型の取得
VpiObjType
EiTableEntry::type() const
{
  return VpiObjType::TableEntry;
}

// @brief ファイル位置を返す．
FileRegion
EiTableEntry::file_region() const
{
  return mPtUdpEntry->file_region();
}

// @brief 一行の要素数を返す．
SizeType
EiTableEntry::size() const
{
  SizeType row_size = mUdp->port_num();
  if ( mUdp->prim_type() == VpiPrimType::Seq ) {
    ++ row_size;
  }
  return row_size;
}

// @brief pos 番目の位置の値を返す．
VlUdpVal
EiTableEntry::val(SizeType pos) const
{
  return mValArray[pos];
}

// @brief 一行文の内容を表す文字列をつくる．
string
EiTableEntry::str() const
{
  int n = size();
  int in = n - 1; // 出力変数の分を減らす
  if ( mUdp->prim_type() == VpiPrimType::Seq ) {
    -- in; // さらに状態変数の分を減らす．
  }
  int in1 = in - 1;
  int n1 = n - 1;
  string s;
  for ( int pos = 0; pos < n; ++ pos ) {
    s += val(pos).to_string();
    if ( pos < in1 ) {
      s += " ";
    }
    else if ( pos < n1 ) {
      s += " : ";
    }
  }
  return s;
}

// @brief 設定する．
void
EiTableEntry::set(const PtUdpEntry* pt_entry,
		  const vector<VlUdpVal>& vals)
{
  mPtUdpEntry = pt_entry;
  int n = size();
  for ( int i = 0; i < n; ++ i ) {
    mValArray[i] = vals[i];
  }
}

END_NAMESPACE_YM_VERILOG
