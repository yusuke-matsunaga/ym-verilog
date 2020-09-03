
/// @file Parser_module.cc
/// @brief Parser の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.


#include "parser/Parser.h"

#include "scanner/Lex.h"

#include "parser/PtMgr.h"
#include "parser/PtiFactory.h"
#include "ym/pt/PtModule.h"
#include "ym/pt/PtExpr.h"

#include "ym/MsgMgr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// Module 関係
//////////////////////////////////////////////////////////////////////

// @brief モジュール定義の開始
// - port list の初期化
// - paramport list の初期化
// - iohead list の初期化
// - paramhead list の初期化
// - localparamhead list の初期化
// - declhead list の初期化
// - item list の初期化
// を行う．
void
Parser::init_module()
{

  mCurIOHeadList = &mModuleIOHeadList;
  push_declhead_list();
  push_item_list();

  mPortList.clear();
  mParamPortHeadList.clear();
  mCurIOHeadList->clear();
  mIOItemList.clear();
  cur_declhead_list().clear();
  mDeclItemList.clear();
  cur_item_list().clear();
}

// @brief モジュール定義の終了
void
Parser::end_module()
{
  mCurDeclArray = pop_declhead_list();
  mCurItemArray = pop_item_list();
}

// Verilog1995 タイプのモジュールを生成する．
void
Parser::new_Module1995(const FileRegion& file_region,
		       bool is_macro,
		       const char* module_name,
		       PtrList<const PtAttrInst>* ai_list)
{
  auto port_vector = get_port_vector();
  auto paramport_array = get_paramport_array();
  auto iohead_array = get_module_io_array();

  bool is_cell = lex().cell_define();
  bool is_protected = false; // これどうやって決めるの？
  int time_u = lex().time_unit();
  int time_p = lex().time_precision();
  VpiNetType nettype = lex().default_nettype();
  VpiUnconnDrive unconn = lex().unconnected_drive();
  VpiDefDelayMode delay = lex().delay_mode();
  int decay = lex().default_decay_time();

#if 0 // VERIFAULT
  bool portfaults = lex().portfaults();
  bool suppress_faults = lex().suppress_faults();
#else
  bool portfaults = false;
  bool suppress_faults = false;
#endif

  string config;  // ?
  string library; // ?
  string cell;    // ?

  // ポート宣言とIO宣言のチェックを行う．
  unordered_map<string, VpiDir> iodecl_dirs;
  check_IO(port_vector, iohead_array, iodecl_dirs);

  // 今度はポートリストに現れている信号線が入出力ポート宣言されているか
  // 調べる．
  // 同時に名無しのポートがあるかどうかしらべる．
  bool named_port = true;
  for ( auto port: mPortList ) {
    if ( port->ext_name() == nullptr ) {
      // 1つでも名前を持たないポートがあったら名前での結合はできない．
      named_port = false;
    }
    SizeType n = port->portref_size();
    for ( int i = 0; i < n; ++ i ) {
      auto portref = port->portref_elem(i);
      auto name = portref->name();
      if ( iodecl_dirs.count(name) == 0 ) {
	// name は IOH リストに存在しない．
	ostringstream buf;
	buf << "\"" << name << "\" is in the port list but not declared.";
	MsgMgr::put_msg(__FILE__, __LINE__,
			port->file_region(),
			MsgType::Error,
			"ELAB",
			buf.str());
      }
      else {
	VpiDir dir = iodecl_dirs.at(name);
	port->_set_portref_dir(i, dir);
      }
    }
  }

  auto module{mFactory->new_Module(file_region,
				  module_name,
				  is_macro,
				  is_cell,
				  is_protected,
				  time_u, time_p,
				  nettype, unconn,
				  delay, decay,
				  named_port,
				  portfaults, suppress_faults,
				  config, library, cell,
				  paramport_array,
				  port_vector,
				  iohead_array,
				  mCurDeclArray,
				  mCurItemArray)};
  mPtMgr.reg_module(module);
  reg_attrinst(module, ai_list, true);
}

// Verilog2001 タイプのモジュールを生成する．
void
Parser::new_Module2001(const FileRegion& file_region,
		       bool is_macro,
		       const char* module_name,
		       PtrList<const PtAttrInst>* ai_list)
{
  auto paramport_array = get_paramport_array();
  auto iohead_array = get_module_io_array();

  bool is_cell = lex().cell_define();
  bool is_protected = false; // これどうやって決めるの？
  int time_u = lex().time_unit();
  int time_p = lex().time_precision();
  VpiNetType nettype = lex().default_nettype();
  VpiUnconnDrive unconn = lex().unconnected_drive();
  VpiDefDelayMode delay = lex().delay_mode();
  int decay = lex().default_decay_time();

#if 0 // VERIFAULT
  bool portfaults = lex().portfaults();
  bool suppress_faults = lex().suppress_faults();
#else
  bool portfaults = false;
  bool suppress_faults = false;
#endif

  string config;  // ?
  string library; // ?
  string cell;    // ?

  if ( !check_PortArray(iohead_array) ) {
    return;
  }

  // iohead_array からポートの配列を作る．
  auto port_array = new_PortArray(iohead_array);

  auto module{mFactory->new_Module(file_region,
				  module_name,
				  is_macro, is_cell, is_protected,
				  time_u, time_p, nettype,
				  unconn, delay, decay,
				  true,
				  portfaults, suppress_faults,
				  config, library, cell,
				  paramport_array,
				  port_array,
				  iohead_array,
				  mCurDeclArray,
				  mCurItemArray)};
  mPtMgr.reg_module(module);
  reg_attrinst(module, ai_list, true);
}

// @brief ポート宣言とIO宣言の齟齬をチェックする．
// @param[in] port_vector ポート宣言のリスト
// @param[in] iohead_array IO宣言のリスト
// @param[out] iodecl_dirs IO宣言名をキーとして向きを保持する辞書
void
Parser::check_IO(const vector<const PtPort*>& port_array,
		 const vector<const PtIOHead*>& iohead_array,
		 unordered_map<string, VpiDir>& iodecl_dirs)
{
  // port_array をスキャンして中で用いられている名前を portref_dic
  // に登録する．
  unordered_set<string> portref_dic;
  for ( auto port: port_array ) {
    SizeType n = port->portref_size();
    for ( int i = 0; i < n; ++ i ) {
      auto portref = port->portref_elem(i);
      auto name = portref->name();
      portref_dic.insert(name);
    }
  }

  // 入出力ポート宣言に現れる名前を iodecl_names に入れる．
  // ポート宣言が型を持つ場合にはモジュール内部の宣言要素を生成する．
  // 持たない場合にはデフォルトタイプのネットを生成する．
  for ( auto io_head: iohead_array ) {
    // 名前をキーにして方向を記録しておく
    VpiDir dir = io_head->direction();
    for ( auto elem: io_head->item_list() ) {
      auto elem_name = elem->name();

      // まず未定義/多重定義のエラーをチェックする．
      if ( portref_dic.count(elem_name) == 0 ) {
	// port expression に現れない信号線名
	// 未定義エラー
	ostringstream buf;
	buf << "\"" << elem_name << "\" is not defined in the port list.";
	MsgMgr::put_msg(__FILE__, __LINE__,
			elem->file_region(),
			MsgType::Error,
			"ELAB",
			buf.str());
      }
      if ( iodecl_dirs.count(elem_name) > 0 ) {
	// 二重登録エラー
	ostringstream buf;
	buf << "\"" << elem_name << "\" is redefined.";
	MsgMgr::put_msg(__FILE__, __LINE__,
			elem->file_region(),
			MsgType::Error,
			"ELAB",
			buf.str());
      }
      else {
	iodecl_dirs[elem_name] = dir;
      }
    }
  }
}

END_NAMESPACE_YM_VERILOG
