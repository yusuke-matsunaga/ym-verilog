﻿
/// @file Parser_module.cc
/// @brief Parser の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "parser/Parser.h"

#include "scanner/Lex.h"

#include "parser/PtMgr.h"
#include "parser/PtiFactory.h"
#include "parser/PtiDecl.h"
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
  mPortList.clear();
  mParamPortHeadList.clear();

  mCurIOHeadList = &mModuleIOHeadList;
  mCurDeclHeadList = &mModuleDeclHeadList;
  mCurItemList = &mModuleItemList;

  mCurIOHeadList->clear();
  mIOItemList.clear();

  mCurDeclHeadList->clear();
  mDeclItemList.clear();

  mCurItemList->clear();
}

// @brief モジュール定義の終了
void
Parser::end_module()
{
}

// Verilog1995 タイプのモジュールを生成する．
void
Parser::new_Module1995(const FileRegion& file_region,
		       bool is_macro,
		       const char* module_name,
		       PtrList<const PtAttrInst>* ai_list)
{
  vector<PtiPort*> port_vector = get_port_vector();
  PtDeclHeadArray paramport_array = get_paramport_array();
  PtIOHeadArray iohead_array = get_module_io_array();
  PtDeclHeadArray declhead_array = get_module_decl_array();
  PtItemArray item_array = get_module_item_array();

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

  // port_vector をスキャンして中で用いられている名前を portref_dic
  // に登録する．
  unordered_set<string> portref_dic;
  for ( auto port: port_vector ) {
    SizeType n = port->portref_size();
    for ( int j = 0; j < n; ++ j ) {
      const PtExpr* portref = port->portref_elem(j);
      const char* name = portref->name();
      portref_dic.insert(name);
    }
  }

  // 入出力ポート宣言に現れる名前を iodecl_names に入れる．
  // ポート宣言が型を持つ場合にはモジュール内部の宣言要素を生成する．
  // 持たない場合にはデフォルトタイプのネットを生成する．
  unordered_map<string, VpiDir> iodecl_dirs;
  for ( auto io_head: iohead_array ) {
    // 名前をキーにして方向を記録しておく
    VpiDir dir = VpiDir::NoDirection;
    switch ( io_head->type() ) {
    case kPtIO_Input:  dir = VpiDir::Input; break;
    case kPtIO_Output: dir = VpiDir::Output; break;
    case kPtIO_Inout:  dir = VpiDir::Inout; break;
    default:
      ASSERT_NOT_REACHED;
    }
    for ( auto elem: io_head->item_list() ) {
      const char* elem_name = elem->name();

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

  // 今度はポートリストに現れている信号線が入出力ポート宣言されているか
  // 調べる．
  // 同時に名無しのポートがあるかどうかしらべる．
  bool named_port = true;
  for ( auto port: port_vector ) {
    if ( port->ext_name() == nullptr ) {
      // 1つでも名前を持たないポートがあったら名前での結合はできない．
      named_port = false;
    }
    SizeType n = port->portref_size();
    for ( int j = 0; j < n; ++ j ) {
      const PtExpr* portref = port->portref_elem(j);
      const char* name = portref->name();
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
	port->_set_portref_dir(j, dir);
      }
    }
  }

  PtPortArray port_array = new_PortArray(port_vector);

  const PtModule* module = mFactory.new_Module(file_region,
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
					       port_array,
					       iohead_array,
					       declhead_array,
					       item_array);
  mPtMgr.reg_module(module);
  reg_attrinst(module, ai_list);
}

// Verilog2001 タイプのモジュールを生成する．
void
Parser::new_Module2001(const FileRegion& file_region,
		       bool is_macro,
		       const char* module_name,
		       PtrList<const PtAttrInst>* ai_list)
{
  PtDeclHeadArray paramport_array = get_paramport_array();
  PtIOHeadArray iohead_array = get_module_io_array();
  PtDeclHeadArray declhead_array = get_module_decl_array();
  PtItemArray item_array = get_module_item_array();

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
  PtPortArray port_array = new_PortArray(iohead_array);

  const PtModule* module = mFactory.new_Module(file_region,
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
					       declhead_array,
					       item_array);
  mPtMgr.reg_module(module);
  reg_attrinst(module, ai_list);
}

// @brief ポートリストをvectorに変換する．
inline
vector<PtiPort*>
Parser::get_port_vector()
{
  return mPortList.to_vector();
}

// @brief IO宣言リストを配列に変換する．
inline
PtIOHeadArray
Parser::get_module_io_array()
{
  return mModuleIOHeadList.to_array(mAlloc);
}

// @brief parameter port リストを配列に変換する．
inline
PtDeclHeadArray
Parser::get_paramport_array()
{
  return mParamPortHeadList.to_array(mAlloc);
}

// @brief module 用の宣言リストを配列に変換する．
inline
PtDeclHeadArray
Parser::get_module_decl_array()
{
  return mModuleDeclHeadList.to_array(mAlloc);
}

// @brief module 用の item リストを配列に変換する．
inline
PtItemArray
Parser::get_module_item_array()
{
  return mModuleItemList.to_array(mAlloc);
}

END_NAMESPACE_YM_VERILOG