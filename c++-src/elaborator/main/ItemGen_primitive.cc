
/// @file ItemGen_primitive.cc
/// @brief ElbMgr の実装ファイル(UDP インスタンスの実体化)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ItemGen.h"
#include "ElbEnv.h"

#include "ym/BitVector.h"

#include "ym/pt/PtItem.h"
#include "ym/pt/PtExpr.h"
#include "ym/pt/PtMisc.h"

#include "ym/ClibCell.h"
#include "ym/ClibCellPin.h"

#include "elb/ElbUdp.h"
#include "elb/ElbPrimitive.h"
#include "elb/ElbExpr.h"

#include "ym/MsgMgr.h"


BEGIN_NAMESPACE_YM_VERILOG

BEGIN_NONAMESPACE

// 英語の序数の接尾語を作る関数
const char*
num_suffix(int num)
{
  switch ( num ) {
  case 1: return "st";
  case 2: return "nd";
  case 3: return "rd";
  default: return "th";
  }
}

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// プリミティブインスタンス関係の instantiate 関数 (変な日本語)
//////////////////////////////////////////////////////////////////////

// @brief gate instance の生成を行う
// @param[in] parent 親のスコープ
// @param[in] pt_head ヘッダ
void
ItemGen::instantiate_gateheader(const VlNamedObj* parent,
				const PtItem* pt_head)
{
  const PtDelay* pt_delay = pt_head->delay();
  bool has_delay = (pt_delay != nullptr);

  ElbPrimHead* prim_head = factory().new_PrimHead(parent, pt_head, has_delay);
  if ( has_delay ) {
    add_phase3stub(make_stub(this, &ItemGen::link_gate_delay,
			     prim_head, pt_delay));
  }

  for ( auto pt_inst: pt_head->inst_list() ) {
    const FileRegion& fr = pt_inst->file_region();
    SizeType port_num = pt_inst->port_num();
    SizeType output_num;
    SizeType inout_num;
    SizeType input_num;
    switch ( ElbPrimitive::get_port_size(pt_head->prim_type(), port_num,
					 output_num, inout_num, input_num) ) {
    case -1:
      MsgMgr::put_msg(__FILE__, __LINE__,
		      pt_inst->file_region(),
		      MsgType::Error,
		      "ELAB",
		      "Too few port connections.");
      continue;

    case 1:
      MsgMgr::put_msg(__FILE__, __LINE__,
		      pt_inst->file_region(),
		      MsgType::Error,
		      "ELAB",
		      "Too many port connections.");
      continue;
    }

    const PtExpr* pt_left = pt_inst->left_range();
    const PtExpr* pt_right = pt_inst->right_range();
    if ( pt_left && pt_right ) {
      // 配列の場合
      int left_val = 0;
      int right_val = 0;
      if ( !evaluate_range(parent, pt_left, pt_right,
			   left_val, right_val) ) {
	return;
      }

      ElbPrimArray* prim_array
	= factory().new_PrimitiveArray(prim_head,
				       pt_inst,
				       pt_left, pt_right,
				       left_val, right_val);
      reg_primarray(prim_array);

#if 0
      // attribute instance の生成
      instantiate_attribute(pt_head->attr_top(), false, prim_array);
#else
#warning "TODO:2011-02-09-01"
#endif

      ostringstream buf;
      buf << "instantiating primitive array: " << prim_array->full_name();
      MsgMgr::put_msg(__FILE__, __LINE__,
		      fr,
		      MsgType::Info,
		      "ELAB",
		      buf.str());

      add_phase3stub(make_stub(this, &ItemGen::link_prim_array,
			       prim_array, pt_inst));
    }
    else {
      // 単一の要素の場合
      ElbPrimitive* prim = factory().new_Primitive(prim_head,
						   pt_inst);
      reg_primitive(prim);

#if 0
      // attribute instance の生成
      instantiate_attribute(pt_head->attr_top(), false, prim);
#else
#warning "TODO:2011-02-09-01"
#endif

      ostringstream buf;
      buf << "instantiating primitive: " << prim->full_name();
      MsgMgr::put_msg(__FILE__, __LINE__,
		      fr,
		      MsgType::Info,
		      "ELAB",
		      buf.str());

      add_phase3stub(make_stub(this, &ItemGen::link_primitive,
			       prim, pt_inst));
    }
  }
}

// @brief UDP instance の生成を行う
// @param[in] parent 親のスコープ
// @param[in] pt_head ヘッダ
// @param[in] udpdefn UDP
void
ItemGen::instantiate_udpheader(const VlNamedObj* parent,
			       const PtItem* pt_head,
			       const VlUdpDefn* udpdefn)
{
  SizeType param_size = pt_head->paramassign_num();
  const PtDelay* pt_delay = pt_head->delay();
  bool has_delay = ( pt_delay || param_size == 1 );

  ElbPrimHead* prim_head = factory().new_UdpHead(parent,
						 pt_head,
						 udpdefn,
						 has_delay);
  if ( has_delay ) {
    add_phase3stub(make_stub(this, &ItemGen::link_udp_delay,
			     prim_head, pt_head));
  }

  for ( auto pt_inst: pt_head->inst_list() ) {
    SizeType port_num = pt_inst->port_num();
    if ( port_num > 0 && pt_inst->port(0)->name() != nullptr ) {
      MsgMgr::put_msg(__FILE__, __LINE__,
		      pt_inst->file_region(),
		      MsgType::Error,
		      "ELAB",
		      "UDP instance cannot have named port list.");
      continue;
    }

    if ( udpdefn->port_num() != port_num ) {
      MsgMgr::put_msg(__FILE__, __LINE__,
		      pt_inst->file_region(),
		      MsgType::Error,
		      "ELAB",
		      "Number of ports mismatch.");
      continue;
    }

    const PtExpr* pt_left = pt_inst->left_range();
    const PtExpr* pt_right = pt_inst->right_range();
    if ( pt_left && pt_right ) {
      // 配列
      int left_val = 0;
      int right_val = 0;
      if ( !evaluate_range(parent, pt_left, pt_right,
			   left_val, right_val) ) {
	return;
      }

      ElbPrimArray* prim_array
	= factory().new_PrimitiveArray(prim_head,
				       pt_inst,
				       pt_left, pt_right,
				       left_val, right_val);
      reg_primarray(prim_array);

#if 0
      // attribute instance の生成
      instantiate_attribute(pt_head->attr_top(), false, prim_array);
#else
#warning "TODO:2011-02-09-01"
#endif

      add_phase3stub(make_stub(this, &ItemGen::link_prim_array,
			       prim_array, pt_inst));
    }
    else {
      // 単一の要素
      ElbPrimitive* primitive
	= factory().new_Primitive(prim_head,
				  pt_inst);
      reg_primitive(primitive);

#if 0
      // attribute instance の生成
      instantiate_attribute(pt_head->attr_top(), false, primitive);
#else
#warning "TODO:2011-02-09-01"
#endif

      add_phase3stub(make_stub(this, &ItemGen::link_primitive,
			       primitive, pt_inst));
    }
  }
}

// @brief セル instance の生成を行う
// @param[in] parent 親のスコープ
// @param[in] pt_head ヘッダ
// @param[in] cell_id セル番号
void
ItemGen::instantiate_cell(const VlNamedObj* parent,
			  const PtItem* pt_head,
			  int cell_id)
{
  ElbPrimHead* prim_head = factory().new_CellHead(parent,
						  pt_head,
						  cell_id);
  const ClibCell& cell = get_cell(cell_id);
  for ( auto pt_inst: pt_head->inst_list() ) {
    // ポート数のチェックを行う．
    SizeType port_num = pt_inst->port_num();
    if ( port_num > 0 && pt_inst->port(0)->name() != nullptr ) {
      // 名前による結合
      for ( auto pt_con: pt_inst->port_list() ) {
	const char* pin_name = pt_con->name();
	int pin_id = cell.pin_id(pin_name);
	if ( pin_id == -1 ) {
	  ostringstream buf;
	  buf << pin_name << ": No such pin.";
	  MsgMgr::put_msg(__FILE__, __LINE__,
			  pt_con->file_region(),
			  MsgType::Error,
			  "ELAB",
			  buf.str());
	  continue;
	}
      }
    }
    else {
      if ( cell.pin_num() != port_num ) {
	MsgMgr::put_msg(__FILE__, __LINE__,
			pt_inst->file_region(),
			MsgType::Error,
			"ELAB",
			"Number of ports mismatch.");
	continue;
      }
    }

    // インスタンスの生成を行う．
    const PtExpr* pt_left = pt_inst->left_range();
    const PtExpr* pt_right = pt_inst->right_range();
    if ( pt_left && pt_right ) {
      // 配列
      int left_val = 0;
      int right_val = 0;
      if ( !evaluate_range(parent, pt_left, pt_right,
			   left_val, right_val) ) {
	return;
      }

      ElbPrimArray* prim_array
	= factory().new_PrimitiveArray(prim_head,
				       pt_inst,
				       pt_left, pt_right,
				       left_val, right_val);
      reg_primarray(prim_array);

#if 0
      // attribute instance の生成
      instantiate_attribute(pt_head->attr_top(), false, prim_array);
#else
#warning "TODO:2011-02-09-01"
#endif

      add_phase3stub(make_stub(this, &ItemGen::link_cell_array,
			       prim_array, pt_inst));
    }
    else {
      // 単一の要素
      ElbPrimitive* primitive
	= factory().new_Primitive(prim_head,
				  pt_inst);
      reg_primitive(primitive);

#if 0
      // attribute instance の生成
      instantiate_attribute(pt_head->attr_top(), false, primitive);
#else
#warning "TODO:2011-02-09-01"
#endif

      add_phase3stub(make_stub(this, &ItemGen::link_cell,
			       primitive, pt_inst));
    }
  }
}

// @brief gate delay の生成を行う
// @param[in] prim_head ゲートプリミティブのヘッダ
// @param[in] pt_delay パース木の遅延式
void
ItemGen::link_gate_delay(ElbPrimHead* prim_head,
			 const PtDelay* pt_delay)
{
  const VlNamedObj* parent = prim_head->parent();
  auto delay = instantiate_delay(parent, pt_delay);
  prim_head->set_delay(delay);
}

// @brief UDP instance に付随する遅延式の生成を行う．
// @param[in] parent 親のスコープ
// @param[in] prim_head ヘッダ
// @param[in] pt_head パース木のインスタンス定義ヘッダ
void
ItemGen::link_udp_delay(ElbPrimHead* prim_head,
			const PtItem* pt_head)
{
  const VlNamedObj* parent = prim_head->parent();
  SizeType param_size = pt_head->paramassign_num();
  const PtDelay* pt_delay = pt_head->delay();
  const VlDelay* delay = nullptr;
  if ( pt_delay ) {
    delay = instantiate_delay(parent, pt_delay);
  }
  else if ( param_size == 1 ) {
    // ordered_param_list が実は遅延式だった．
    delay = instantiate_delay(parent, pt_head);
  }
  if ( delay ) {
    prim_head->set_delay(delay);
  }
}

// @brief UDP/primitive array instance で使われている式の名前解決を行う．
// @param[in] pt_inst インスタンス定義
// @param[in] prim_array primitive 配列
void
ItemGen::link_prim_array(ElbPrimArray* prim_array,
			 const PtInst* pt_inst)
{
  const VlNamedObj* parent = prim_array->parent();
  SizeType arraysize = prim_array->elem_num();

  // ポートの情報を得るために先頭の要素を取り出す．
  const VlPrimitive* prim = prim_array->elem_by_offset(0);

  ElbEnv env1;
  ElbNetLhsEnv env2(env1);
  int index = 0;
  for ( auto pt_con: pt_inst->port_list() ) {
    const PtExpr* pt_expr = pt_con->expr();
    if ( !pt_expr ) {
      // 空の接続式は許されない．
      MsgMgr::put_msg(__FILE__, __LINE__,
		      pt_con->file_region(),
		      MsgType::Error,
		      "ELAB",
		      "Empty expression in UDP/primitive instance port.");
      continue;
    }

    const VlPrimTerm* term = prim->prim_term(index);
    ++ index;
    ElbExpr* tmp = nullptr;
    if ( term->direction() == VpiDir::Input ) {
      // 入力に接続するのは通常の右辺式
      tmp = instantiate_expr(parent, env1, pt_expr);
    }
    else {
      // それ以外は左辺式
      tmp = instantiate_lhs(parent, env2, pt_expr);
    }
    if ( !tmp ) {
      continue;
    }

    VlValueType type = tmp->value_type();
    if ( type.is_real_type() ) {
      MsgMgr::put_msg(__FILE__, __LINE__,
		      tmp->file_region(),
		      MsgType::Error,
		      "ELAB",
		      "Real expression cannot connect to UDP port.");
      continue;
    }
    SizeType expr_size = type.size();
    if ( expr_size == 1 ) {
      // サイズが等しければそのまま接続する．
      for ( int i = 0; i < arraysize; ++ i ) {
	ElbPrimitive* prim = prim_array->_primitive_by_offset(i);
	prim->connect(index, tmp);
      }
    }
    else if ( expr_size == 0 ) {
      // サイズがなければ1ビットに直してから接続する．
      tmp->set_reqsize(VlValueType(false, true, 1));
      for ( int i = 0; i < arraysize; ++ i ) {
	ElbPrimitive* prim = prim_array->_primitive_by_offset(i);
	prim->connect(index, tmp);
      }
    }
    else if ( expr_size == arraysize ) {
      // tmp を 1 ビットずつに分割して接続する．
      for ( int i = 0; i < arraysize; ++ i ) {
	ElbPrimitive* prim = prim_array->_primitive_by_offset(i);
	ElbExpr* bit = factory().new_BitSelect(pt_expr, tmp, i);
	prim->connect(index, bit);
      }
    }
    else {
      ostringstream buf;
      buf << (index + 1) << num_suffix(index + 1)
	  << " port expression has illegal size.";
      MsgMgr::put_msg(__FILE__, __LINE__,
		      pt_con->file_region(),
		      MsgType::Error,
		      "ELAB",
		      buf.str());
    }
  }
}

// @brief UDP/primitive instance で使われている式の名前解決を行う．
// @param[in] pt_inst インスタンス定義
// @param[in] primitive primitive
void
ItemGen::link_primitive(ElbPrimitive* primitive,
			const PtInst* pt_inst)
{
  const VlNamedObj* parent = primitive->parent();

  ElbEnv env1;
  ElbNetLhsEnv env2(env1);
  int index = 0;
  for ( auto pt_con: pt_inst->port_list() ) {
    // UDP instance の場合には ai_list は無視する．
    const PtExpr* pt_expr = pt_con->expr();
    if ( !pt_expr ) {
      continue;
    }

    const VlPrimTerm* term = primitive->prim_term(index);
    ++ index;
    ElbExpr* tmp = nullptr;
    if ( term->direction() == VpiDir::Input ) {
      // 入力に接続するのは通常の右辺式
      tmp = instantiate_expr(parent, env1, pt_expr);
    }
    else {
      // それ以外は左辺式
      tmp = instantiate_lhs(parent, env2, pt_expr);
    }
    if ( !tmp ) {
      continue;
    }

    VlValueType type = tmp->value_type();
    if ( type.is_real_type() ) {
      MsgMgr::put_msg(__FILE__, __LINE__,
		      tmp->file_region(),
		      MsgType::Error,
		      "ELAB",
		      "Real expression cannot connect to UDP port.");
      continue;
    }

    SizeType expr_size = type.size();
    if ( expr_size == 1 ) {
      // サイズが等しければそのまま接続する．
      primitive->connect(index, tmp);
    }
    else if ( expr_size == 0 ) {
      // サイズがなければ1ビットに直してから接続する．
      tmp->set_reqsize(VlValueType(false, true, 1));
      primitive->connect(index, tmp);
    }
    else {
      ostringstream buf;
      buf << (index + 1) << num_suffix(index + 1)
	  << " port expression has illegal size.";
      MsgMgr::put_msg(__FILE__, __LINE__,
		      pt_con->file_region(),
		      MsgType::Error,
		      "ELAB",
		      buf.str());
    }
  }
}

// @brief cell array instance で使われている式の名前解決を行う．
// @param[in] pt_inst インスタンス定義
// @param[in] prim_array primitive 配列
void
ItemGen::link_cell_array(ElbPrimArray* prim_array,
			 const PtInst* pt_inst)
{
  const VlNamedObj* parent = prim_array->parent();
  SizeType arraysize = prim_array->elem_num();

  // ポートの情報を得るために先頭の要素を取り出す．
  const VlPrimitive* prim = prim_array->elem_by_offset(0);

  // YACC の文法から一つでも named_con なら全部そう
  bool conn_by_name = (pt_inst->port(0)->name() != nullptr);

  const ClibCell& cell = get_cell(prim->cell_id());

  ElbEnv env1;
  ElbNetLhsEnv env2(env1);
  int pos = 0;
  for ( auto pt_con: pt_inst->port_list() ) {
    int index;
    if ( conn_by_name ) {
      int pin_id = cell.pin_id(pt_con->name());
      if ( pin_id == -1 ) {
	ostringstream buf;
	buf << pt_con->name() << ": No such pin.";
	MsgMgr::put_msg(__FILE__, __LINE__,
			pt_con->file_region(),
			MsgType::Error,
			"ELAB",
			buf.str());
	continue;
      }
      index = pin_id;
    }
    else {
      index = pos;
      ++ pos;
    }
    const PtExpr* pt_expr = pt_con->expr();
    if ( !pt_expr ) {
      // 空の接続式は許されない．
      MsgMgr::put_msg(__FILE__, __LINE__,
		      pt_con->file_region(),
		      MsgType::Error,
		      "ELAB",
		      "Empty expression in UDP/primitive instance port.");
      continue;
    }

    const VlPrimTerm* term = prim->prim_term(index);
    ElbExpr* tmp = nullptr;
    if ( term->direction() == VpiDir::Input ) {
      // 入力に接続するのは通常の右辺式
      tmp = instantiate_expr(parent, env1, pt_expr);
    }
    else {
      // それ以外は左辺式
      tmp = instantiate_lhs(parent, env2, pt_expr);
    }
    if ( !tmp ) {
      continue;
    }

    VlValueType type = tmp->value_type();
    if ( type.is_real_type() ) {
      MsgMgr::put_msg(__FILE__, __LINE__,
		      tmp->file_region(),
		      MsgType::Error,
		      "ELAB",
		      "Real expression cannot connect to UDP port.");
      continue;
    }

    SizeType expr_size = type.size();
    if ( expr_size == 1 ) {
      // サイズが等しければそのまま接続する．
      for ( int i = 0; i < arraysize; ++ i ) {
	ElbPrimitive* prim = prim_array->_primitive_by_offset(i);
	prim->connect(index, tmp);
      }
    }
    else if ( expr_size == 0 ) {
      // サイズがなければ1ビットに直してから接続する．
      tmp->set_reqsize(VlValueType(false, true, 1));
      for ( int i = 0; i < arraysize; ++ i ) {
	ElbPrimitive* prim = prim_array->_primitive_by_offset(i);
	prim->connect(index, tmp);
      }
    }
    else if ( expr_size == arraysize ) {
      // tmp を 1 ビットずつに分割して接続する．
      for ( int i = 0; i < arraysize; ++ i ) {
	ElbPrimitive* prim = prim_array->_primitive_by_offset(i);
	ElbExpr* bit = factory().new_BitSelect(pt_expr, tmp, i);
	prim->connect(index, bit);
      }
    }
    else {
      ostringstream buf;
      buf << (index + 1) << num_suffix(index + 1)
	  << " port expression has illegal size.";
      MsgMgr::put_msg(__FILE__, __LINE__,
		      pt_con->file_region(),
		      MsgType::Error,
		      "ELAB",
		      buf.str());
    }
  }
}

// @brief cell instance で使われている式の名前解決を行う．
// @param[in] pt_inst インスタンス定義
// @param[in] primitive primitive
void
ItemGen::link_cell(ElbPrimitive* primitive,
		   const PtInst* pt_inst)
{
  const VlNamedObj* parent = primitive->parent();

  // YACC の文法から一つでも named_con なら全部そう
  bool conn_by_name = (pt_inst->port(0)->name() != nullptr);

  const ClibCell& cell = get_cell(primitive->cell_id());

  ElbEnv env1;
  ElbNetLhsEnv env2(env1);
  int pos = 0;
  for ( auto pt_con: pt_inst->port_list() ) {
    int index;
    if ( conn_by_name ) {
      int pin_id = cell.pin_id(pt_con->name());
      if ( pin_id == -1 ) {
	ostringstream buf;
	buf << pt_con->name() << ": No such pin.";
	MsgMgr::put_msg(__FILE__, __LINE__,
			pt_con->file_region(),
			MsgType::Error,
			"ELAB",
			buf.str());
	continue;
      }
      index = pin_id;
    }
    else {
      index = pos;
      ++ pos;
    }
    // ai_list は無視する．
    const PtExpr* pt_expr = pt_con->expr();
    if ( !pt_expr ) {
      continue;
    }

    const VlPrimTerm* term = primitive->prim_term(index);
    ElbExpr* tmp = nullptr;
    if ( term->direction() == VpiDir::Input ) {
      // 入力に接続するのは通常の右辺式
      tmp = instantiate_expr(parent, env1, pt_expr);
    }
    else {
      // それ以外は左辺式
      tmp = instantiate_lhs(parent, env2, pt_expr);
    }
    if ( !tmp ) {
      continue;
    }

    VlValueType type = tmp->value_type();
    if ( type.is_real_type() ) {
      MsgMgr::put_msg(__FILE__, __LINE__,
		      tmp->file_region(),
		      MsgType::Error,
		      "ELAB",
		      "Real expression cannot connect to UDP port.");
      continue;
    }
    int expr_size = type.size();
    if ( expr_size == 1 ) {
      // サイズが等しければそのまま接続する．
      primitive->connect(index, tmp);
    }
    else if ( expr_size == 0 ) {
      // サイズがなければ1ビットに直してから接続する．
      tmp->set_reqsize(VlValueType(false, true, 1));
      primitive->connect(index, tmp);
    }
    else {
      ostringstream buf;
      buf << (index + 1) << num_suffix(index + 1)
	  << " port expression has illegal size.";
      MsgMgr::put_msg(__FILE__, __LINE__,
		      pt_con->file_region(),
		      MsgType::Error,
		      "ELAB",
		      buf.str());
    }
  }
}

END_NAMESPACE_YM_VERILOG
