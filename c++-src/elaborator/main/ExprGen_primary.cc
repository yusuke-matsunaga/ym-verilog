﻿
/// @file ExprGen_primary.cc
/// @brief ElbMgr の実装ファイル(式の実体化)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.


#include "ExprGen.h"
#include "ElbEnv.h"

#include "ym/pt/PtModule.h"
#include "ym/pt/PtExpr.h"
#include "ym/pt/PtMisc.h"

#include "ym/vl/VlModule.h"
#include "ym/vl/VlDeclArray.h"
#include "ym/vl/VlRange.h"

#include "elaborator/ElbDecl.h"
#include "elaborator/ElbParameter.h"
#include "elaborator/ElbPrimitive.h"
#include "elaborator/ElbExpr.h"
#include "elaborator/ElbGenvar.h"

#include "ym/MsgMgr.h"


BEGIN_NAMESPACE_YM_VERILOG

// @brief PtPrimary から ElbExpr を生成する．
// @param[in] parent 親のスコープ
// @param[in] env 生成時の環境
// @param[in] pt_expr 式を表すパース木
ElbExpr*
ExprGen::instantiate_primary(const VlScope* parent,
			     const ElbEnv& env,
			     const PtExpr* pt_expr)
{
  // 識別子の階層
  bool has_hname{(pt_expr->namebranch_num() > 0)};
  if ( has_hname ) {
    if ( env.is_constant() ) {
      // 階層つき識別子はだめ
      error_hname_in_ce(pt_expr);
      return nullptr;
    }
    if ( env.inside_constant_function() ) {
      // 階層つき識別子はだめ
      error_hname_in_cf(pt_expr);
      return nullptr;
    }
  }

  // 識別子の名前
  auto name{pt_expr->name()};

  // 識別子の添字の次元
  SizeType isize{pt_expr->index_num()};

  // 名前に対応したオブジェクトのハンドル
  ObjHandle* handle = nullptr;

  if ( env.is_constant() ) {
    handle = find_const_handle(parent, pt_expr);
    if ( handle == nullptr ) {
      // エラーが起こった．
      // メッセージは find_const_handle() 内で出力されている．
      return nullptr;
    }
  }
  else if ( env.inside_constant_function() ) {
    // まず関数内の識別子を探索する．
    handle = find_obj_up(parent, pt_expr, env.constant_function());
    if ( handle == nullptr && !env.is_lhs() ) {
      // 右辺ならモジュール内の定数識別子を探索する．
      handle = find_const_handle(parent, pt_expr);
    }
    if ( handle == nullptr ) {
      // エラーが起こった．
      // メッセージは find_const_handle() 内で出力されている．
      return nullptr;
    }
  }
  else {
    // 通常のスコープで探索する．
    // たぶんモジュール内でいいはず．
    handle = find_obj_up(parent, pt_expr,  parent->parent_module());
    if ( handle == nullptr ) {
      // 見つからなくてもデフォルトネットタイプが kVpiNone でないかぎり
      // 暗黙の1ビットネット宣言を行う．
      // ただし識別子に添字がついていたらだめ
      auto parent_module{parent->parent_module()};
      auto def_nettype{parent_module->def_net_type()};
      if ( pt_expr->is_simple() &&
	   !has_hname &&
	   isize == 0 &&
	   def_nettype != VpiNetType::None ) {
	auto decl{mgr().new_ImpNet(parent, pt_expr, def_nettype)};
	reg_decl(vpiNet, decl);

	handle = find_obj(parent, name);
	// 今作ったはずなので絶対見つかるはず．
	ASSERT_COND( handle );
      }
    }
    if ( handle == nullptr ) {
      error_not_found(pt_expr);
      return nullptr;
    }
  }


  if ( env.is_system_tf_arg() ) {
    // システム関数/タスクの引数の場合
    if ( isize == 0 ) {
      auto scope{handle->scope()};
      if ( scope ) {
	return mgr().new_ArgHandle(pt_expr, scope);
      }

      auto primitive{handle->primitive()};
      if ( primitive ) {
	return mgr().new_ArgHandle(pt_expr, primitive);
      }

      auto decl{handle->decl()};
      if ( decl ) {
	return mgr().new_Primary(pt_expr, decl);
      }

      auto declarray{handle->declarray()};
      if ( declarray ) {
	return mgr().new_ArgHandle(pt_expr, declarray);
      }
    }
    else if ( isize == 1 ) {
      auto pt_expr1{pt_expr->index(0)};
      int index;
      bool stat{evaluate_int(parent, pt_expr1, index, true)};
      if ( !stat ) {
	return nullptr;
      }

      auto scope{handle->array_elem(index)};
      if ( scope ) {
	return mgr().new_ArgHandle(pt_expr, scope);
      }

      auto prim_array{handle->prim_array()};
      if ( prim_array ) {
	//auto primitive = prim_array->_primitive_by_index(index);
	auto primitive{prim_array->elem_by_index(index)};
	if ( primitive ) {
	  return mgr().new_ArgHandle(pt_expr, primitive);
	}
      }
    }
    error_illegal_object(pt_expr);
    return nullptr;
  }

  if ( !env.is_lhs() ) {
    // 対象のオブジェクトが genvar の場合
    auto genvar{handle->genvar()};
    if ( genvar ) {
      return instantiate_genvar(parent, pt_expr, genvar->value());
    }
  }

  // 添字には constant/constant function 以外の情報は引き継がない
  ElbEnv index_env;
  if ( pt_expr->is_const_index() ) {
    index_env = ElbConstantEnv();
  }
  else if ( env.inside_constant_function() ) {
    index_env = ElbConstantFunctionEnv(env.constant_function());
  }

  // 対象のオブジェクトが宣言要素だった場合
  bool is_array;
  bool has_range_select;
  bool has_bit_select;
  auto primary = instantiate_primary_sub(handle, parent,
					 index_env, pt_expr,
					 is_array,
					 has_range_select,
					 has_bit_select);
  if ( primary == nullptr ) {
    // エラー
    // メッセージは instantiate_decl() 内で出力されている．
    return nullptr;
  }

  auto decl_base{primary->decl_base()};
  ASSERT_COND( decl_base != nullptr );
  auto decl_type{decl_base->type()};

  if ( !check_decl(env, pt_expr, decl_type, is_array,
		   has_range_select | has_bit_select) ) {
    // エラー
    // メッセージは check_decl() 内で出力されている．
    return nullptr;
  }

  if ( has_bit_select ) {
    // ビット指定付きの場合
    auto pt_expr1{pt_expr->index(isize - 1)};
    int index_val;
    bool stat1{evaluate_int(parent, pt_expr1, index_val, false)};
    if ( stat1 ) {
      // 固定インデックスだった．
      SizeType offset;
      bool stat2{decl_base->calc_bit_offset(index_val, offset)};
      if ( !stat2 ) {
	// インデックスが範囲外
	MsgMgr::put_msg(__FILE__, __LINE__,
			pt_expr1->file_region(),
			MsgType::Warning,
			"ELAB",
			"Bit-Select index is out of range.");
	// ただ値が X になるだけでエラーにはならないそうだ．
      }
      return mgr().new_BitSelect(pt_expr, primary, pt_expr1, index_val);
    }
    auto index = instantiate_expr(parent, index_env, pt_expr1);
    if ( !index ) {
      return nullptr;
    }
    return mgr().new_BitSelect(pt_expr, primary, index);
  }
  if ( has_range_select ) {
    // 範囲指定付きの場合
    switch ( pt_expr->range_mode() ) {
    case VpiRangeMode::Const:
      {
	auto pt_left{pt_expr->left_range()};
	int index1_val;
	bool stat1{evaluate_int(parent, pt_left, index1_val, true)};
	if ( !stat1 ) {
	  return nullptr;
	}

	auto pt_right{pt_expr->right_range()};
	int index2_val;
	bool stat2{evaluate_int(parent, pt_right, index2_val, true)};
	if ( !stat2 ) {
	  return nullptr;
	}

	bool big{(index1_val >= index2_val)};
	if ( big ^ decl_base->is_big_endian() ) {
	  // 範囲の順番が逆
	  error_range_order(pt_expr);
	  return nullptr;
	}

	SizeType offset;
	bool stat3{decl_base->calc_bit_offset(index1_val, offset)};
	if ( !stat3 ) {
	  // 左のインデックスが範囲外
	  MsgMgr::put_msg(__FILE__, __LINE__,
			  pt_left->file_region(),
			  MsgType::Warning,
			  "ELAB",
			  "Left index is out of range.");
	  // ただ値が X になるだけでエラーにはならないそうだ．
	}

	bool stat4{decl_base->calc_bit_offset(index2_val, offset)};
	if ( !stat4 ) {
	  // 右のインデックスが範囲外
	  MsgMgr::put_msg(__FILE__, __LINE__,
			  pt_right->file_region(),
			  MsgType::Warning,
			  "ELAB",
			  "Right index is out of range.");
	  // ただ値が X になるだけでエラーにはならないそうだ．
	}

	return mgr().new_PartSelect(pt_expr, primary,
				    pt_left, pt_right,
				    index1_val, index2_val);
      }

    case VpiRangeMode::Plus:
      {
	auto pt_range{pt_expr->right_range()};
	int range_val;
	bool stat1{evaluate_int(parent, pt_range, range_val, true)};
	if ( !stat1 ) {
	  // range は常に固定でなければならない．
	  return nullptr;
	}

	auto pt_base{pt_expr->left_range()};
	int base_val;
	bool stat2{evaluate_int(parent, pt_base, base_val, false)};
	if ( stat2 ) {
	  // 固定インデックスだった．
	  int index1_val;
	  int index2_val;
	  if ( decl_base->is_big_endian() ) {
	    index1_val = base_val + range_val - 1;
	    index2_val = base_val;
	  }
	  else {
	    index1_val = base_val;
	    index2_val = base_val + range_val - 1;
	  }

	  SizeType offset;
	  bool stat3{decl_base->calc_bit_offset(index1_val, offset)};
	  bool stat4{decl_base->calc_bit_offset(index2_val, offset)};
	  if ( !stat3 || !stat4 ) {
	    // 左か右のインデックスが範囲外
	    MsgMgr::put_msg(__FILE__, __LINE__,
			    pt_expr->file_region(),
			    MsgType::Warning,
			    "ELAB",
			    "Index is out of range.");
	    // ただ値が X になるだけでエラーにはならないそうだ．
	  }
	  return mgr().new_PartSelect(pt_expr, primary,
					  pt_base, pt_range,
					  index1_val, index2_val);
	}
	else {
	  auto base{instantiate_expr(parent, index_env, pt_base)};
	  if ( !base ) {
	    return nullptr;
	  }
	  return mgr().new_PlusPartSelect(pt_expr, primary,
					  base, pt_range, range_val);
	}
      }

    case VpiRangeMode::Minus:
      {
	auto pt_range{pt_expr->right_range()};
	int range_val;
	bool stat1{evaluate_int(parent, pt_range, range_val, true)};
	if ( !stat1 ) {
	  // range は常に固定でなければならない．
	  return nullptr;
	}

	auto pt_base{pt_expr->left_range()};
	int base_val;
	bool stat2{evaluate_int(parent, pt_base, base_val, false)};
	if ( stat2 ) {
	  // 固定インデックスだった．
	  int index1_val;
	  int index2_val;
	  if ( decl_base->is_big_endian() ) {
	    index1_val = base_val;
	    index2_val = base_val - range_val + 1;
	  }
	  else {
	    index1_val = base_val - range_val + 1;
	    index2_val = base_val;
	  }

	  SizeType offset;
	  bool stat3{decl_base->calc_bit_offset(index1_val, offset)};
	  bool stat4{decl_base->calc_bit_offset(index2_val, offset)};
	  if ( !stat3 || !stat4 ) {
	    // 左か右のインデックスが範囲外
	    MsgMgr::put_msg(__FILE__, __LINE__,
			    pt_expr->file_region(),
			    MsgType::Warning,
			    "ELAB",
			    "Index is out of range.");
	    // ただ値が X になるだけでエラーにはならないそうだ．
	  }
	  return mgr().new_PartSelect(pt_expr, primary,
					  pt_base, pt_range,
					  index1_val, index2_val);
	}
	else {
	  auto base{instantiate_expr(parent, index_env, pt_base)};
	  if ( !base ) {
	    return nullptr;
	  }
	  return mgr().new_MinusPartSelect(pt_expr, primary,
					       base, pt_range, range_val);
	}
      }

    case VpiRangeMode::No:
      ASSERT_NOT_REACHED;
      break;
    }
  }
  return primary;
}

// @brief PtExpr(primary) から named_event を生成する．
// @param[in] parent 親のスコープ
// @param[in] pt_expr 式を表すパース木
ElbExpr*
ExprGen::instantiate_namedevent(const VlScope* parent,
				const PtExpr* pt_expr)
{
  ASSERT_COND( pt_expr->type()        == PtExprType::Primary );
  ASSERT_COND( pt_expr->left_range()  == nullptr );
  ASSERT_COND( pt_expr->right_range() == nullptr );

  // 名前に対応したオブジェクトのハンドルを求める．
  auto handle{find_obj_up(parent, pt_expr, nullptr)};
  if ( handle == nullptr ) {
    // 見つからなかった．
    error_not_found(pt_expr);
    return nullptr;
  }

  // 配列要素などの処理を行う．
  ElbEnv env0;
  if ( pt_expr->is_const_index() ) {
    env0 = ElbConstantEnv();
  }

  bool is_array;
  bool has_range_select;
  bool has_bit_select;
  auto primary = instantiate_primary_sub(handle, parent, env0, pt_expr,
					 is_array,
					 has_range_select,
					 has_bit_select);
  if ( primary == nullptr ) {
    // エラー
    // メッセージは instantiate_decl() 内で出力されている．
    return nullptr;
  }

  auto decl_base{primary->decl_base()};
  ASSERT_COND( decl_base != nullptr );
  auto decl_type{decl_base->type()};
  if ( decl_type != VpiObjType::NamedEvent ) {
    // 型が違う
    error_not_a_namedevent(pt_expr);
    return nullptr;
  }
  if ( has_range_select || has_bit_select ) {
    // 部分選択，ビット選択は使えない．
    error_select_for_namedevent(pt_expr);
    return nullptr;
  }

  return primary;
}

// @brief 定数識別子を探す．
// @param[in] parent 親のスコープ
// @param[in] pt_expr 式を表すパース木
ObjHandle*
ExprGen::find_const_handle(const VlScope* parent,
			   const PtExpr* pt_expr)
{
  // モジュール内の識別子を探索する．
  auto handle{find_obj_up(parent, pt_expr, parent->parent_module())};
  if ( handle == nullptr ) {
    // 見つからなかった．
    error_not_found(pt_expr);
    return nullptr;
  }

  // handle が持つオブジェクトは genvar か parameter でなければならない．
  if ( handle->genvar() == nullptr && handle->parameter() == nullptr ) {
    error_not_a_parameter(pt_expr);
    return nullptr;
  }

  return handle;
}

// @brief genvar に対応した定数を生成する．
// @param[in] pt_expr 式を表すパース木
// @param[in] val 値
// @note pt_expr に添字が付いていたらエラーとなる．
ElbExpr*
ExprGen::instantiate_genvar(const VlScope* parent,
			    const PtExpr* pt_expr,
			    int val)
{
  bool has_range_select{(pt_expr->left_range() && pt_expr->right_range())};
  SizeType isize{pt_expr->index_num()};
  if (  isize > 1 || (isize == 1 && has_range_select) ) {
    // 配列型ではない．
    error_dimension_mismatch(pt_expr);
    return nullptr;
  }

  bool has_bit_select{(isize == 1)};
  if ( has_bit_select ) {
    int index1{0};
    if ( !evaluate_int(parent, pt_expr->index(0), index1, true) ) {
      return nullptr;
    }
    val >>= index1;
    val &= 1;
  }
  else if ( has_range_select ) {
    int index1{0};
    int index2{0};
    bool stat1{evaluate_int(parent, pt_expr->left_range(), index1, true)};
    bool stat2{evaluate_int(parent, pt_expr->right_range(), index2, true)};
    if ( !stat1 || !stat2 ) {
      return nullptr;
    }
    val >>= index2;
    val &= ((1 << (index1 - index2 + 1)) - 1);
  }

  // genvar の値に対応した定数式を生成
  return mgr().new_GenvarConstant(pt_expr, val);
}

// @brief 宣言要素のインスタンス化を行う．
// @param[in] handle オブジェクトハンドル
// @param[in] parent 親のスコープ
// @param[in] pt_expr 式を表すパース木
// @param[out] is_array 対象が配列の時 true を返す．
// @param[out] has_range_select 範囲指定を持っていたら true を返す．
// @param[out] has_bit_select ビット指定を持っていたら true を返す．
ElbExpr*
ExprGen::instantiate_primary_sub(ObjHandle* handle,
				 const VlScope* parent,
				 const ElbEnv& env,
				 const PtExpr* pt_expr,
				 bool& is_array,
				 bool& has_range_select,
				 bool& has_bit_select)
{
  // 配列の次元
  SizeType dsize{0};
  // プライマリ式の次元 (ビット指定を含んでいる可能性あり)
  SizeType isize {pt_expr->index_num()};

  // 範囲指定があるとき true となるフラグ
  has_range_select = (pt_expr->left_range() && pt_expr->right_range());

  // 答え
  ElbExpr* primary{nullptr};
  VlValueType value_type;
  if ( handle->type() == VpiObjType::Parameter ) {
    // パラメータの場合
    auto param{handle->parameter()};
    primary = mgr().new_Primary(pt_expr, param);
    is_array = false;
    value_type = param->value_type();
  }
  else {
    // それ以外の宣言要素の場合
    auto decl{handle->decl()};
    auto declarray{handle->declarray()};
    if ( decl != nullptr ) {
      primary = mgr().new_Primary(pt_expr, decl);
      is_array = false;
      value_type = decl->value_type();
    }
    else if ( declarray != nullptr ) {
      // 配列の次元
      dsize = declarray->dimension();
      if ( isize != dsize && (isize != dsize + 1 || has_range_select) ) {
	// 次元が合わない．
	error_dimension_mismatch(pt_expr);
	return nullptr;
      }

      is_array = true;
      value_type = declarray->value_type();

      // 添字が定数ならオフセットを計算する．
      SizeType offset{0};
      SizeType mlt{1};
      bool const_index{true};
      for ( SizeType i = 0; i < dsize; ++ i ) {
	SizeType j{dsize - i - 1};
	auto pt_expr1{pt_expr->index(j)};
	int index_val;
	bool stat{evaluate_int(parent, pt_expr1, index_val, false)};
	if ( !stat ) {
	  const_index = false;
	  break;
	}
	offset += index_val * mlt;
	mlt *= declarray->range(j)->size();
      }
      if ( const_index ) {
	primary = mgr().new_Primary(pt_expr, declarray, offset);
      }
      else {
	// 添字の式を生成する．
	vector<ElbExpr*> index_list;
	index_list.reserve(dsize);
	for ( SizeType i = 0; i < dsize; ++ i ) {
	  auto pt_expr1{pt_expr->index(i)};
	  auto expr1{instantiate_expr(parent, env, pt_expr1)};
	  if ( !expr1 ) {
	    return nullptr;
	  }
	  index_list.push_back(expr1);
	}

	primary = mgr().new_Primary(pt_expr, declarray, index_list);
      }
    }
  }
  if ( primary == nullptr ) {
    // 適切な型ではなかった．
    error_illegal_object(pt_expr);
    return nullptr;
  }

  // ビット指定があるとき true となるフラグ
  has_bit_select = false;

  if ( isize == dsize + 1 && !has_range_select ) {
    // 識別子の添字の次元と配列の次元が 1 違いで
    // 範囲がなければ識別子の最後の添字はビット指定と見なす．
    has_bit_select = true;
    -- isize;
  }

  if ( isize != dsize ) {
    // 次元が会わない．
    error_dimension_mismatch(pt_expr);
    return nullptr;
  }

  if ( has_range_select || has_bit_select ) {
    if ( value_type.is_real_type() ) {
      error_select_for_real(pt_expr);
      return nullptr;
    }
  }

  return primary;
}

// @brief decl の型が適切がチェックする．
bool
ExprGen::check_decl(const ElbEnv& env,
		    const PtExpr* pt_expr,
		    VpiObjType decl_type,
		    bool is_array,
		    bool has_select)
{
  if ( env.is_pca_lhs() ) {
    // procedural continuous assignment 文の左辺式
    if ( is_array ) {
      // 配列要素はダメ
      error_array_in_pca(pt_expr);
      return false;
    }
    if ( has_select ) {
      // 部分指定はダメ
      error_select_in_pca(pt_expr);
      return false;
    }
    if ( decl_type != VpiObjType::Reg &&
	 decl_type != VpiObjType::IntegerVar &&
	 decl_type != VpiObjType::RealVar &&
	 decl_type != VpiObjType::TimeVar) {
      // reg/変数以外はダメ
      error_illegal_object(pt_expr);
      return false;
    }
  }
  else if ( env.is_force_lhs() ) {
    // force 文の左辺式
    if ( is_array ) {
      // 配列要素はダメ
      error_array_in_force(pt_expr);
      return false;
    }
    if ( has_select ) {
      // 部分指定はダメ
      error_select_in_force(pt_expr);
      return false;
    }
    if ( decl_type != VpiObjType::Net &&
	 decl_type != VpiObjType::Reg &&
	 decl_type != VpiObjType::IntegerVar &&
	 decl_type != VpiObjType::RealVar &&
	 decl_type != VpiObjType::TimeVar) {
      // net/reg/変数以外はダメ
      error_illegal_object(pt_expr);
      return false;
    }
  }
  else if ( env.is_net_lhs() ) {
    if ( decl_type != VpiObjType::Net &&
	 (decl_type != VpiObjType::NetArray || !is_array) ) {
      // net 以外はダメ
      error_illegal_object(pt_expr);
      return false;
    }
  }
  else if ( env.is_var_lhs() ) {
    if ( decl_type != VpiObjType::Reg &&
	 (decl_type != VpiObjType::RegArray || !is_array) &&
	 decl_type != VpiObjType::IntegerVar &&
	 decl_type != VpiObjType::RealVar &&
	 decl_type != VpiObjType::TimeVar &&
	 decl_type != VpiObjType::VarSelect ) {
      // reg/変数以外はダメ
      error_illegal_object(pt_expr);
      return false;
    }
  }
  else {
    // 右辺系の環境
    if ( env.is_constant() ) {
      // 定数式
      if ( decl_type != VpiObjType::Parameter &&
	   decl_type != VpiObjType::SpecParam ) {
	// 定数(parameter)でないのでダメ
	error_illegal_object(pt_expr);
	return false;
      }
    }

    // あとは個別の型ごとにチェックする．
    if ( decl_type == VpiObjType::RealVar && has_select ) {
      // real の部分選択は無効
      error_select_for_real(pt_expr);
      return false;
    }
    if ( decl_type == VpiObjType::NamedEvent && !env.is_event_expr() ) {
      // イベント式以外では名前つきイベントは使えない．
      error_illegal_object(pt_expr);
      return false;
    }
  }
  return true;
}

END_NAMESPACE_YM_VERILOG
