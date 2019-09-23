
/// @file UdpGen.cc
/// @brief UdpGen の実装ファイル (UDPのインスタンス化関係)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "UdpGen.h"
#include "ElbEnv.h"

#include "ym/pt/PtUdp.h"
#include "ym/pt/PtPort.h"
#include "ym/pt/PtDecl.h"
#include "ym/pt/PtExpr.h"
#include "ym/pt/PtArray.h"
#include "ym/vl/VlExpr.h"

#include "elb/ElbUdp.h"
#include "elb/ElbExpr.h"

#include "ym/MsgMgr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス UdpGen
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] elab 生成器
// @param[in] elb_mgr Elbオブジェクトを管理するクラス
// @param[in] elb_factory Elbオブジェクトを生成するファクトリクラス
UdpGen::UdpGen(Elaborator& elab,
	       ElbMgr& elb_mgr,
	       ElbFactory& elb_factory) :
  ElbProxy(elab, elb_mgr, elb_factory)
{
}

// @brief デストラクタ
UdpGen::~UdpGen()
{
}

// @brief UDP定義を生成する．
// @param[in] pt_udp パース木の UDP 定義
void
UdpGen::instantiate_udp(const PtUdp* pt_udp)
{
  typedef unordered_map<string, pair<const PtIOHead*, const PtIOItem*> > IODict;

  const FileRegion& file_region = pt_udp->file_region();
  const char* def_name = pt_udp->name();

  ostringstream buf;
  buf << "instantiating UDP \"" << def_name << "\".";
  MsgMgr::put_msg(__FILE__, __LINE__,
		  file_region,
		  MsgType::Info,
		  "ELAB",
		  buf.str());

  SizeType io_size = pt_udp->port_list().size();

  SizeType isize = io_size - 1;
  VpiPrimType ptype = pt_udp->prim_type();
  bool is_protected = true; // 何これ?

  ElbUdpDefn* udp = factory().new_UdpDefn(pt_udp, is_protected);

  // 最初のポート名 = 出力のポート名
  const char* outname = pt_udp->port_list()[0]->ext_name();

  // ポート名をキーにしたIOテンプレートの辞書を作る．
  IODict iodict;
  const PtIOHead* outhead = nullptr;
  for ( auto iohead: pt_udp->iohead_array() ) {
    for ( auto elem: iohead->item_list() ) {
      const char* name = elem->name();
      iodict[name] = make_pair(iohead, elem);
      if ( strcmp(name, outname) == 0 ) {
	outhead = iohead;
      }
    }
  }

  // IOポートを実体化する．
  // ただし port_list に現れる名前の順番にしたがって実体化しなければならない．
  int index = 0;
  for ( auto port: pt_udp->port_list() ) {
    const char* name = port->ext_name();
    ASSERT_COND( iodict.count(name) > 0 );
    auto tmp = iodict.at(name);
    const PtIOHead* pt_header = tmp.first;
    const PtIOItem* pt_item = tmp.second;
    udp->set_io(index, pt_header, pt_item);
    ++ index;
  }

  // 初期化文を実体化させる．
  // initial 文がある場合と変数宣言の中に初期化式が含まれている場合がある．
  const PtExpr* pt_init_value = pt_udp->init_value();
  if ( pt_init_value == nullptr ) {
    pt_init_value = outhead->item_list()[0]->init_value();
  }
  if ( pt_init_value ) {
    // このチェックはパース時に済んでいるはずなので念のため．
    ASSERT_COND(ptype == VpiPrimType::Seq );

    const FileRegion& ifr = pt_init_value->file_region();

    VlScalarVal val;
    if ( !evaluate_scalar(nullptr, pt_init_value, val, true) ) {
      MsgMgr::put_msg(__FILE__, __LINE__,
		      ifr,
		      MsgType::Error,
		      "ELAB",
		      "Only 1-bit constants are allowed here.");
      return;
    }
    // 初期値を設定する．
    udp->set_initial(pt_init_value, val);
  }

  // テーブルの中身を作る．
  // 似た様なことをしているが単純にするために組合わせ回路と順序回路を
  // 分ける．
  if ( ptype == VpiPrimType::Comb ) {
    // 組合わせ回路

    // 一行のサイズは入出力数と一致する．
    int row_size = io_size;

    // 出力値の位置
    int opos = row_size - 1;

    // 一行文のデータを保持しておくためのバッファ
    vector<VlUdpVal> row_data(row_size);

    const PtUdpEntryArray& table = pt_udp->table_array();
    int pos = 0;
    for ( auto pt_udp_entry: table ) {
      const FileRegion& tfr = pt_udp_entry->file_region();
      PtUdpValueArray input_array = pt_udp_entry->input_array();
      if ( input_array.size() != isize ) {
	// サイズが合わない．
	MsgMgr::put_msg(__FILE__, __LINE__,
			tfr,
			MsgType::Error,
			"ELAB",
			"Number of input symbols mimatch.");
	return;
      }

      // 入力
      for ( int j = 0; j < isize; ++ j ) {
	const PtUdpValue* pt_v = input_array[j];
	VlUdpVal symbol = pt_v->symbol();

	if ( symbol.is_edge_symbol() ) {
	  // 組合せ回路の場合にはエッジタイプの値は使えない．
	  ostringstream buf;
	  buf << symbol.to_string()
	      << " : transition symbol for combinational UDP";
	  MsgMgr::put_msg(__FILE__, __LINE__,
			  pt_v->file_region(),
			  MsgType::Error,
			  "ELAB",
			  buf.str());
	  return;
	}
	if ( symbol.is_nc_symbol() ) {
	  // NC は状態出力にしか使えない
	  ostringstream buf;
	  buf << symbol.to_string() << " : illegal symbol for input field.";
	  MsgMgr::put_msg(__FILE__, __LINE__,
			  pt_v->file_region(),
			  MsgType::Error,
			  "ELAB",
			  buf.str());
	  return;
	}

	row_data[j] = symbol;
      }

      { // 現状態
	if ( pt_udp_entry->current() ) {
	  MsgMgr::put_msg(__FILE__, __LINE__,
			  pt_udp_entry->file_region(),
			  MsgType::Error,
			  "ELAB",
			  "Combinational UDP should not have "
			  "\'current state\' value.");
	  return;
	}
      }

      { // 出力
	const PtUdpValue* pt_v = pt_udp_entry->output();
	VlUdpVal symbol = pt_v->symbol();

	if ( symbol.is_composite_symbol() ) {
	  // 出力には複合値は使えない
	  ostringstream buf;
	  buf << symbol.to_string() << " : illegal symbol for output field.";
	  MsgMgr::put_msg(__FILE__, __LINE__,
			  pt_v->file_region(),
			  MsgType::Error,
			  "ELAB",
			  buf.str());
	  return;
	}

	row_data[opos] = symbol;
      }

      // 一行文のデータを設定する．
      udp->set_tableentry(pos, pt_udp_entry, row_data);
      ++ pos;
    }
  }
  else {
    // 順序回路

    // 一行のサイズは入出力 + 現状態
    SizeType row_size = io_size + 1;

    // 現状態値の位置
    int cpos = isize;

    // 出力値の位置
    int opos = io_size;

    // 一行文のデータを保持しておくためのバッファ
    vector<VlUdpVal> row_data(row_size);

    const PtUdpEntryArray& table = pt_udp->table_array();
    int pos = 0;
    for ( auto pt_udp_entry: table ) {
      const FileRegion& tfr = pt_udp_entry->file_region();
      PtUdpValueArray input_array = pt_udp_entry->input_array();

      if ( input_array.size() != isize ) {
	// サイズが合わない．
	MsgMgr::put_msg(__FILE__, __LINE__,
			tfr,
			MsgType::Error,
			"ELAB",
			"Number of input symbols mimatch.");
	return;
      }

      // 一行中に含まれるエッジタイプのシンボルの数
      int nt = 0;

      // 入力
      for ( int j = 0; j < isize; ++ j ) {
	const PtUdpValue* pt_v = input_array[j];
	VlUdpVal symbol = pt_v->symbol();

	if ( symbol.is_edge_symbol() ) {
	  ++ nt;
	  if ( nt > 1 ) {
	    MsgMgr::put_msg(__FILE__, __LINE__,
			    pt_v->file_region(),
			    MsgType::Error,
			    "ELAB",
			    "More than one transition symbols "
			    "in the same row.");
	    return;
	  }
	}

	// 順序回路の入力は全ての値/シンボルが使用可
	row_data[j] = symbol;
      }

      { // 現状態
	const PtUdpValue* pt_v = pt_udp_entry->current();
	if ( !pt_v ) {
	  MsgMgr::put_msg(__FILE__, __LINE__,
			  tfr,
			  MsgType::Error,
			  "ELAB",
			  "Sequential UDP requires \'current state\' value.");
	  return;
	}

	VlUdpVal symbol = pt_v->symbol();

	if ( symbol.is_edge_symbol() ) {
	  // エッジタイプの値は使えない．
	  ostringstream buf;
	  buf << symbol.to_string()
	      << " : transition symbol for current state field.";
	  MsgMgr::put_msg(__FILE__, __LINE__,
			  pt_v->file_region(),
			  MsgType::Error,
			  "ELAB",
			  buf.str());
	  return;
	}
	if ( symbol.is_nc_symbol() ) {
	  // NC は状態出力にしか使えない
	  ostringstream buf;
	  buf << symbol.to_string()
	      << " : illegal symbol for current state field.";
	  MsgMgr::put_msg(__FILE__, __LINE__,
			  pt_v->file_region(),
			  MsgType::Error,
			  "ELAB",
			  buf.str());
	  return;
	}

	row_data[cpos] = symbol;
      }

      { // 出力
	const PtUdpValue* pt_v = pt_udp_entry->output();
	VlUdpVal symbol = pt_v->symbol();

	if ( !symbol.is_nc_symbol() && symbol.is_edge_symbol() ) {
	  // エッジタイプの値は使えない．
	  ostringstream buf;
	  buf << symbol.to_string()
	      << " : transition symbol for output field.";
	  MsgMgr::put_msg(__FILE__, __LINE__,
			  pt_v->file_region(),
			  MsgType::Error,
			  "ELAB",
			  buf.str());
	  return;
	}
	if ( symbol.is_composite_symbol() ) {
	  // 出力には複合値は使えない
	  ostringstream buf;
	  buf << symbol.to_string()
	      << " : illegal symbol for output field.";
	  MsgMgr::put_msg(__FILE__, __LINE__,
			  pt_v->file_region(),
			  MsgType::Error,
			  "ELAB",
			  buf.str());
	  return;
	}

	row_data[opos] = symbol;
      }

      // 一行文のデータを設定する．
      udp->set_tableentry(pos, pt_udp_entry, row_data);
      ++ pos;
    }
  }

  // UDP を登録する．
  reg_udp(def_name, udp);
}

END_NAMESPACE_YM_VERILOG
