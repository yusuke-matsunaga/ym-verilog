
/// @file ErrorGen.cc
/// @brief エラーメッセージ出力用のクラス
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.


#include "ErrorGen.h"
#include "ElbError.h"
#include "ElbParamCon.h"
#include "ym/pt/PtModule.h"
#include "ym/pt/PtDecl.h"
#include "ym/pt/PtItem.h"
#include "ym/pt/PtExpr.h"
#include "ym/vl/VlDeclArray.h"
#include "ym/vl/VlExpr.h"


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

// @brief パラメータポートの割り当て数が多すぎる．
// @param[in] file ファイル名
// @param[in] line 行番号
// @param[in] param_con_list パラメータポートの割り当てリスト
void
ErrorGen::too_many_param(const char* file,
			 int line,
			 const vector<ElbParamCon>& param_con_list)
{
  auto last{param_con_list.back()};
  error_common(file, line, last.mPtCon->file_region(),
	       "ELABXXX",
	       "Too many parameters.");
}

// @brief パラメータポートに現れるパラメータが存在しない．
// @param[in] file ファイル名
// @param[in] line 行番号
// @param[in] pt_con パラメータポート割り当てのパース木
// @param[in] name パラメータ名
void
ErrorGen::no_param(const char* file,
		   int line,
		   const PtConnection* pt_con,
		   const char* name)
{
  ostringstream buf;
  buf << name << " : No such parameter.";
  error_common(file, line, pt_con->file_region(),
	       "ELABXXX",
	       buf.str());
}

// @brief 対象の要素が見つからない．
// @param[in] file ファイル名
// @param[in] line 行番号
// @param[in] loc エラー箇所
// @param[in] name 名前
void
ErrorGen::not_found(const char* file,
		    int line,
		    const FileRegion& loc,
		    const char* name)
{
  ostringstream buf;
  buf << name << ": Not found.";
  error_common(file, line, loc,
	       "ELABXXX",
	       buf.str());
}

// @brief ポートに配列が使われている．
// @param[in] file ファイル名
// @param[in] line 行番号
// @param[in] loc エラー箇所
// @param[in] array 対象の配列
void
ErrorGen::port_array(const char* file,
		     int line,
		     const FileRegion& loc,
		     const VlDeclArray* array)
{
  ostringstream buf;
  buf << array->full_name()
      << ": Array shall not be connected to a module port.";
  error_common(file, line, loc,
	       "ELABXXX",
	       buf.str());
}

// @brief ポートに使われている要素が宣言要素でなかった．
// @param[in] file ファイル名
// @param[in] line 行番号
// @param[in] loc エラー箇所
// @param[in] name 名前
void
ErrorGen::illegal_port(const char* file,
		       int line,
		       const FileRegion& loc,
		       const char* name)
{
  ostringstream buf;
  buf << name << ": Illegal type for port connection.";
  error_common(file, line, loc,
	       "ELABXXX",
	       buf.str());
}

// @brief 暗黙の宣言が禁止されている．
// @param[in] file ファイル名
// @param[in] line 行番号
// @param[in] pt_item 対象の構文木要素
void
ErrorGen::no_impnet(const char* file,
		    int line,
		    const PtIOItem* pt_item)
{
  ostringstream buf;
  buf << pt_item->name() << " : Implicit declaration is inhibited "
      << " because default_nettype = \"none\".";
  error_common(file, line, pt_item->file_region(),
	       "ELABXXX",
	       buf.str());
}

// @brief 暗黙ネットが初期値を持っている．
// @param[in] file ファイル名
// @param[in] line 行番号
// @param[in] pt_item 対象の構文木要素
void
ErrorGen::impnet_with_init(const char* file,
			   int line,
			   const PtIOItem* pt_item)
{
  ostringstream buf;
  buf << pt_item->name()
      << " : Implicit net declaration cannot have initial value.";
  error_common(file, line, pt_item->file_region(),
	       "ELABXXX",
	       buf.str());
}

// @brief IO 宣言に aux_type と宣言が重複している．
// @param[in] file ファイル名
// @param[in] line 行番号
// @param[in] pt_item IO宣言のパース木の要素
// @param[in] prev_loc 元の要素が宣言されていた箇所
void
ErrorGen::duplicate_type(const char* file,
			 int line,
			 const PtIOItem* pt_item,
			 const FileRegion& prev_loc)
{
  ostringstream buf;
  buf << pt_item->name() << " : has an aux-type declaration"
      << ", while it also has another declaration in "
      << prev_loc << ".";
  error_common(file, line, pt_item->file_region(),
	       "ELABXXX",
	       buf.str());
}

// @brief 配列要素が IO 宣言として現れていた．
// @param[in] file ファイル名
// @param[in] line 行番号
// @param[in] pt_item IO宣言のパース木の要素
void
ErrorGen::array_io(const char* file,
		   int line,
		   const PtIOItem* pt_item)
{
  ostringstream buf;
  buf << pt_item->name()
      << ": Array object shall not be connected to IO port.";
  error_common(file, line, pt_item->file_region(),
	       "ELABXXX",
	       buf.str());
}

// @brief IO 宣言に不適切な宣言要素が使われていた．
// @param[in] file ファイル名
// @param[in] line 行番号
// @param[in] pt_item IO宣言のパース木の要素
// @param[in] name 名前
// @param[in] is_module モジュールのIOの時 true とする．
void
ErrorGen::illegal_io(const char* file,
		     int line,
		     const PtIOItem* pt_item,
		     const string& name,
		     bool is_module)
{
  ostringstream buf;
  buf << name << ": Should be a ";
  if ( is_module ) {
    buf << "net, ";
  }
  buf << "reg or integer/time variable.";
  error_common(file, line, pt_item->file_region(),
	       "ELABXXX",
	       buf.str());
}

// @brief IO 宣言と宣言要素の範囲指定が異なる．
// @param[in] file ファイル名
// @param[in] line 行番号
// @param[in] pt_item 対象の構文木要素
void
ErrorGen::conflict_io_range(const char* file,
			    int line,
			    const PtIOItem* pt_item)
{
  ostringstream buf;
  buf << "Conflictive range declaration of \""
      << pt_item->name() << "\".";
  error_common(file, line, pt_item->file_region(),
	       "ELABXXX",
	       buf.str());
}

// @brief 対象が parameter ではなかった．
// @param[in] file ファイル名
// @param[in] line 行番号
// @param[in] pt_item 対象の構文木要素
void
ErrorGen::not_a_parameter(const char* file,
			  int line,
			  const PtDefParam* pt_item)
{
  ostringstream buf;
  buf << "\"" << pt_item->fullname() << "\" is not a parameter.";
  error_common(file, line, pt_item->file_region(),
	       "ELABXXX",
	       buf.str());
}

// @brief 対象が localparam だった．
// @param[in] file ファイル名
// @param[in] line 行番号
// @param[in] pt_item 対象の構文木要素
void
ErrorGen::is_a_localparam(const char* file,
			  int line,
			  const PtDefParam* pt_item)
{
  ostringstream buf;
  buf << "\"" << pt_item->fullname()
      << "\" is a localparam, which shall not be override.";
  error_common(file, line, pt_item->file_region(),
	       "ELABXXX",
	       buf.str());
}

// @brief 重複した generate case ラベル
// @param[in] file ファイル名
// @param[in] line 行番号
// @param[in] pt_item 対象の構文木要素
void
ErrorGen::duplicate_gencase_labels(const char* file,
				   int line,
				   const PtItem* pt_item)
{
  error_common(file, line, pt_item->file_region(),
	       "ELABXXX",
	       "Matches more than one labels.");
}

// @brief generate for 文の変数が見つからない．
// @param[in] file ファイル名
// @param[in] line 行番号
// @param[in] pt_item 対象の構文木要素
void
ErrorGen::genvar_not_found(const char* file,
			   int line,
			   const PtItem* pt_item)
{
  ostringstream buf;
  buf << pt_item->loop_var() << " : Not found.";
  error_common(file, line, pt_item->file_region(),
	       "ELABXXX",
	       buf.str());
}

// @brief genvar でなかった．
// @param[in] file ファイル名
// @param[in] line 行番号
// @param[in] pt_item 対象の構文木要素
void
ErrorGen::not_a_genvar(const char* file,
		       int line,
		       const PtItem* pt_item)
{
  ostringstream buf;
  buf << pt_item->loop_var() << " : Not a genvar.";
  error_common(file, line, pt_item->file_region(),
	       "ELABXXX",
	       buf.str());
}

// @brief 他の generate for loop ですでに用いられている．
// @param[in] file ファイル名
// @param[in] line 行番号
// @param[in] pt_item 対象の構文木要素
void
ErrorGen::genvar_in_use(const char* file,
			int line,
			const PtItem* pt_item)
{
  ostringstream buf;
  buf << pt_item->loop_var() << " : Already in use.";
  error_common(file, line, pt_item->file_region(),
	       "ELABXXX",
	       buf.str());
}

// @brief genvar の初期値が負の値だった．
// @param[in] file ファイル名
// @param[in] line 行番号
// @param[in] pt_item 対象の構文木要素
void
ErrorGen::genvar_negative(const char* file,
			  int line,
			  const PtItem* pt_item)
{
  error_common(file, line, pt_item->file_region(),
	       "ELABXXX",
	       "genvar should not be negative.");
}

// @brief モジュールの依存関係が循環している．
// @param[in] file ファイル名
// @param[in] line 行番号
// @param[in] pt_module 対象の構文木要素
void
ErrorGen::cyclic_dependency(const char* file,
			    int line,
			    const PtModule* pt_module)
{
  ostringstream buf;
  buf << pt_module->name() << " : instantiated within itself.";
  error_common(file, line, pt_module->file_region(),
	       "ELABXXX",
	       buf.str());
}

// @brief 名無しのモジュール定義
// @param[in] file ファイル名
// @param[in] line 行番号
// @param[in] pt_inst 対象の構文木要素
void
ErrorGen::noname_module(const char* file,
			int line,
			const PtInst* pt_inst)
{
  error_common(file, line, pt_inst->file_region(),
	       "ELABXXX",
	       "No module instance name given.");
}

// @brief UDP インスタンスに名前付きのparameter割り当てがあった．
// @param[in] file ファイル名
// @param[in] line 行番号
// @param[in] pt_item 対象の構文木要素
void
ErrorGen::udp_with_named_paramassign(const char* file,
				     int line,
				     const PtItem* pt_item)
{
  error_common(file, line, pt_item->file_region(),
	       "ELABXXX",
	       "UDP instance cannot have named parameter list.");
}

// @brief UDP インスタンスにparameter割り当てがあった．
// @param[in] file ファイル名
// @param[in] line 行番号
// @param[in] pt_item 対象の構文木要素
void
ErrorGen::udp_with_ordered_paramassign(const char* file,
				       int line,
				       const PtItem* pt_item)
{
  error_common(file, line, pt_item->file_region(),
	       "ELABXXX",
	       "UDP instance cannot have ordered parameter list.");
}

// @brief セルインスタンスにparameter割り当てがあった．
// @param[in] file ファイル名
// @param[in] line 行番号
// @param[in] pt_inst 対象の構文木要素
void
ErrorGen::cell_with_paramassign(const char* file,
				int line,
				const PtItem* pt_item)
{
  error_common(file, line, pt_item->file_region(),
	       "ELABXXX",
	       "Cell instance cannot have parameter list.");
}

// @brief インスタンスが見つからなかった．
// @param[in] file ファイル名
// @param[in] line 行番号
// @param[in] pt_item 対象の構文木要素
void
ErrorGen::instance_not_found(const char* file,
			     int line,
			     const PtItem* pt_item)
{
  ostringstream buf;
  buf << pt_item->name() << " : No such module or UDP.";
  error_common(file, line, pt_item->file_region(),
	       "ELABXXX",
	       buf.str());
}

// @brief モジュールインスタンスのポート数が合わない．
// @param[in] file ファイル名
// @param[in] line 行番号
// @param[in] pt_inst 対象の構文木要素
void
ErrorGen::too_many_items_in_port_list(const char* file,
				      int line,
				      const PtInst* pt_inst)
{
  error_common(file, line, pt_inst->file_region(),
	       "ELABXXX",
	       "Too many items in the port list.");
}

// @brief ポート名が存在しなかった．
// @param[in] file ファイル名
// @param[in] line 行番号
// @param[in] pt_con 対象の構文木要素
void
ErrorGen::illegal_port_name(const char* file,
			    int line,
			    const PtConnection* pt_con)
{
  auto port_name{pt_con->name()};
  ostringstream buf;
  buf << port_name << " : does not exist in the port list.";
  error_common(file, line, pt_con->file_region(),
	       "ELABXXX",
	       buf.str());
}

// @brief ポートに real 型の式を接続していた．
// @param[in] file ファイル名
// @param[in] line 行番号
// @param[in] expr 対象の式
void
ErrorGen::real_type_in_port_list(const char* file,
				 int line,
				 const VlExpr* expr)
{
  error_common(file, line, expr->file_region(),
	       "ELABXXX",
	       "Real expression cannot connect to module port.");

}

// @brief ポートサイズが合わない．
// @param[in] file ファイル名
// @param[in] line 行番号
// @param[in] pt_expr 対象の式
// @param[in] name モジュール名
// @param[in] index ポート番号(0から始まる)
void
ErrorGen::port_size_mismatch(const char* file,
			     int line,
			     const PtExpr* pt_expr,
			     const string& name,
			     int index)
{
  ostringstream buf;
  buf << name << " : "
      << (index + 1) << num_suffix(index + 1)
      << " port : port size does not match with the expression.";
  error_common(file, line, pt_expr->file_region(),
	       "ELABXXX",
	       buf.str());
}

// @brief UDP instance に名前付きポート割り当てがあった．
// @param[in] file ファイル名
// @param[in] line 行番号
// @param[in] pt_inst 対象の構文木要素
void
ErrorGen::named_port_in_udp_instance(const char* file,
				     int line,
				     const PtInst* pt_inst)
{
  error_common(file, line, pt_inst->file_region(),
	       "ELABXXX",
	       "UDP instance cannot have named port list.");
}

// @brief UDP instance のポート数が合わない．
// @param[in] file ファイル名
// @param[in] line 行番号
// @param[in] pt_inst 対象の構文木要素
void
ErrorGen::port_num_mismatch(const char* file,
			    int line,
			    const PtInst* pt_inst)
{
  error_common(file, line, pt_inst->file_region(),
	       "ELABXXX",
	       "Number of ports mismatch.");
}

// @brief cell instance のピン名が合わない．
// @param[in] file ファイル名
// @param[in] line 行番号
// @param[in] pt_con 対象の構文木要素
void
ErrorGen::illegal_pin_name(const char* file,
			   int line,
			   const PtConnection* pt_con)
{
  auto pin_name{pt_con->name()};
  ostringstream buf;
  buf << pin_name << ": No such pin.";
  error_common(file, line, pt_con->file_region(),
	       "ELABXXX",
	       buf.str());
}

// @brief 空のポート式
// @param[in] file ファイル名
// @param[in] line 行番号
// @param[in] pt_con 対象の構文木要素
void
ErrorGen::empty_port_expression(const char* file,
				int line,
				const PtConnection* pt_con)
{
  error_common(__FILE__, __LINE__, pt_con->file_region(),
	       "ELABXXX",
	       "Empty expression in UDP/primitive instance port is not allowed.");
}

// @brief constant function 中にシステム関数呼び出し
// @param[in] file ファイル名
// @param[in] line 行番号
// @param[in] pt_expr 式を表すパース木
void
ErrorGen::illegal_sysfunccall_in_cf(const char* file,
				    int line,
				    const PtExpr* pt_expr)
{
  const_common(file, line, pt_expr,
	       "ELABXXX",
	       "System-function call shall not be used in constant function.");
}

// @brief constant expression 中にシステム関数呼び出し
// @param[in] file ファイル名
// @param[in] line 行番号
// @param[in] pt_expr 式を表すパース木
void
ErrorGen::illegal_sysfunccall_in_ce(const char* file,
				    int line,
				    const PtExpr* pt_expr)
{
  const_common(file, line, pt_expr,
	       "ELABXXX",
	       "System-function call shall not be used in constant expression.");
}

// @brief 定数関数は自己再帰できない．
// @param[in] file ファイル名
// @param[in] line 行番号
// @param[in] pt_expr 式を表すパース木
void
ErrorGen::uses_itself(const char* file,
		      int line,
		      const PtExpr* pt_expr)
{
  const_with_hname(file, line, pt_expr,
		   "ELABXXX",
		   "Uses itself.");
}

// @brief 定数関数ではない．
// @param[in] file ファイル名
// @param[in] line 行番号
// @param[in] pt_expr 式を表すパース木
void
ErrorGen::not_a_constant_function(const char* file,
				  int line,
				  const PtExpr* pt_expr)
{
  const_with_hname(file, line, pt_expr,
		   "ELABXXX",
		   "Not a constant function.");
}

// @brief オブジェクトの型が constant function 用として不適切
// @param[in] file ファイル名
// @param[in] line 行番号
// @param[in] pt_expr 式を表すパース木
void
ErrorGen::illegal_object_cf(const char* file,
			    int line,
			    const PtExpr* pt_expr)
{
  const_with_hname(file, line, pt_expr,
		   "ELABXXX",
		   "Illegal object type inside constant function.");
}

// @brief 階層名が constant expression 中にあった
// @param[in] file ファイル名
// @param[in] line 行番号
// @param[in] pt_expr 式を表すパース木
void
ErrorGen::hname_in_ce(const char* file,
		      int line,
		      const PtExpr* pt_expr)
{
  const_with_hname(file, line, pt_expr,
		   "ELABXXX",
		   "Hierarchical name shall not be used"
		   " in constant expression.");
}

// @brief 階層名が constant function 中にあった
// @param[in] file ファイル名
// @param[in] line 行番号
// @param[in] pt_expr 式を表すパース木
void
ErrorGen::hname_in_cf(const char* file,
		      int line,
		      const PtExpr* pt_expr)
{
  const_with_hname(file, line, pt_expr,
		   "ELABXXX",
		   "Hierarchical name shall not be used"
		   " inside constant function.");
}

// @brief オブジェクトが parameter でなかった
// @param[in] file ファイル名
// @param[in] line 行番号
// @param[in] pt_expr 式を表すパース木
void
ErrorGen::not_a_parameter(const char* file,
			  int line,
			  const PtExpr* pt_expr)
{
  const_with_hname(file, line, pt_expr,
		   "ELABXXX",
		   "Not a parameter.");
}

// @brief イベント式の根元に定数
// @param[in] file ファイル名
// @param[in] line 行番号
// @param[in] pt_expr 式を表すパース木
void
ErrorGen::illegal_constant_in_event_expression(const char* file,
					       int line,
					       const PtExpr* pt_expr)
{
  expr_common(file, line, pt_expr,
	       "ELABXXX",
	       "Constant shall not be used in event description.");
}

// @brief イベント式の根元に関数呼び出し
// @param[in] file ファイル名
// @param[in] line 行番号
// @param[in] pt_expr 式を表すパース木
void
ErrorGen::illegal_funccall_in_event_expression(const char* file,
					       int line,
					       const PtExpr* pt_expr)
{
  expr_common(file, line, pt_expr,
	       "ELABXXX",
	       "Function call shall not be used in event description.");
}

// @brief イベント式の根元にシステム関数呼び出し
// @param[in] file ファイル名
// @param[in] line 行番号
// @param[in] pt_expr 式を表すパース木
void
ErrorGen::illegal_sysfunccall_in_event_expression(const char* file,
						  int line,
						  const PtExpr* pt_expr)
{
  expr_common(file, line, pt_expr,
	      "ELABXXX",
	      "System-function call shall not be used in event description.");
}

// @brief 左辺式で用いることのできない演算子
// @param[in] file ファイル名
// @param[in] line 行番号
// @param[in] pt_expr 式を表すパース木
void
ErrorGen::illegal_operator_in_lhs(const char* file,
				  int line,
				  const PtExpr* pt_expr)
{
  expr_common(file, line, pt_expr,
	      "ELABXXX",
	      "Illegal operator in LHS.");
}

// @brief 左辺式に定数
// @param[in] file ファイル名
// @param[in] line 行番号
// @param[in] pt_expr 式を表すパース木
void
ErrorGen::illegal_constant_in_lhs(const char* file,
				  int line,
				  const PtExpr* pt_expr)
{
  expr_common(file, line, pt_expr,
	      "ELABXXX",
	      "Constant shall not be used in LHS.");
}

// @brief 左辺式に関数呼び出し
// @param[in] file ファイル名
// @param[in] line 行番号
// @param[in] pt_expr 式を表すパース木
void
ErrorGen::illegal_funccall_in_lhs(const char* file,
				  int line,
				  const PtExpr* pt_expr)
{
  expr_common(file, line, pt_expr,
	      "ELABXXX",
	      "Function call shall not be used in LHS.");
}

// @brief 左辺式にシステム関数呼び出し
// @param[in] file ファイル名
// @param[in] line 行番号
// @param[in] pt_expr 式を表すパース木
void
ErrorGen::illegal_sysfunccall_in_lhs(const char* file,
				     int line,
				     const PtExpr* pt_expr)
{
  expr_common(file, line, pt_expr,
	      "ELABXXX",
	      "System-function call shall not be used in LHS.");
}

// @brief int 型が要求されている所で互換性のない型があった．
// @param[in] file ファイル名
// @param[in] line 行番号
// @param[in] loc エラー箇所
void
ErrorGen::int_required(const char* file,
		       int line,
		       const FileRegion& loc)
{
  throw ElbIntError(file, line, loc);
}

// @brief ビットベクタ型が要求されている所で互換性のない型があった．
// @param[in] file ファイル名
// @param[in] line 行番号
// @param[in] loc エラー箇所
void
ErrorGen::bv_required(const char* file,
		      int line,
		      const FileRegion& loc)
{
  throw ElbBvError(file, line, loc);
}

// @brief 通常の式中に edge descriptor
// @param[in] file ファイル名
// @param[in] line 行番号
// @param[in] pt_expr 式を表すパース木
void
ErrorGen::illegal_edge_descriptor(const char* file,
				  int line,
				  const PtExpr* pt_expr)
{
  expr_common(file, line, pt_expr,
	      "ELABXXX",
	      "Edge descriptor in an expression");
}

// @brief real 型のオペランドをとれない
// @param[in] file ファイル名
// @param[in] line 行番号
// @param[in] pt_expr 式を表すパース木
void
ErrorGen::illegal_real_type(const char* file,
			    int line,
			    const PtExpr* pt_expr)
{
  expr_common(file, line, pt_expr,
	      "ELABXXX",
	      "Shall not have a real-type operand.");
}

// @brief 該当する関数が存在しない．
// @param[in] file ファイル名
// @param[in] line 行番号
// @param[in] pt_expr 式を表すパース木
void
ErrorGen::no_such_function(const char* file,
			   int line,
			   const PtExpr* pt_expr)
{
  expr_with_hname(file, line, pt_expr,
		  "ELABXXX",
		  "No such function.");
}

// @brief 該当するシステム関数が存在しない．
// @param[in] file ファイル名
// @param[in] line 行番号
// @param[in] pt_expr 式を表すパース木
void
ErrorGen::no_such_sysfunction(const char* file,
			      int line,
			      const PtExpr* pt_expr)
{
  expr_with_hname(file, line, pt_expr,
		  "ELABXXX",
		  "No such system function.");
}

// @brief 関数ではない．
// @param[in] file ファイル名
// @param[in] line 行番号
// @param[in] pt_expr 式を表すパース木
void
ErrorGen::not_a_function(const char* file,
			 int line,
			 const PtExpr* pt_expr)
{
  expr_with_hname(file, line, pt_expr,
		  "ELABXXX",
		  "Not a function.");
}

// @brief 引数の数が合わない．
// @param[in] file ファイル名
// @param[in] line 行番号
// @param[in] pt_expr 式を表すパース木
void
ErrorGen::n_of_arguments_mismatch(const char* file,
				  int line,
				  const PtExpr* pt_expr)
{
  expr_common(file, line, pt_expr,
	      "ELABXXX",
	      "# of arguments does not match.");
}

// @brief 引数の型が合わない．
// @param[in] file ファイル名
// @param[in] line 行番号
// @param[in] pt_expr 式を表すパース木
void
ErrorGen::illegal_argument_type(const char* file,
				int line,
				const PtExpr* pt_expr)
{
  expr_common(file, line, pt_expr,
	      "ELABXXX",
	      "Actual argument type does not match"
	      " with formal argument.");
}

// @brief オブジェクトが存在しない
// @param[in] file ファイル名
// @param[in] line 行番号
// @param[in] pt_expr 式を表すパース木
void
ErrorGen::not_found(const char* file,
		    int line,
		    const PtExpr* pt_expr)
{
  expr_with_hname(file, line, pt_expr,
		  "ELABXXX",
		  "Not found.");
}

// @brief オブジェクトの型が不適切だった
// @param[in] file ファイル名
// @param[in] line 行番号
// @param[in] pt_expr 式を表すパース木
void
ErrorGen::illegal_object(const char* file,
			 int line,
			 const PtExpr* pt_expr)
{
  expr_with_hname(file, line, pt_expr,
		  "ELABXXX",
		  "Illegal object type.");
}

// @brief オブジェクトが named-event でなかった
// @param[in] file ファイル名
// @param[in] line 行番号
// @param[in] pt_expr 式を表すパース木
void
ErrorGen::not_a_namedevent(const char* file,
			   int line,
			   const PtExpr* pt_expr)
{
  expr_with_hname(file, line, pt_expr,
		  "ELABXXX",
		  "Not a named-event.");
}

// @brief 要素の範囲の順番と範囲指定の順番が異なる．
// @param[in] file ファイル名
// @param[in] line 行番号
// @param[in] pt_expr 式を表すパース木
void
ErrorGen::range_order(const char* file,
		      int line,
		      const PtExpr* pt_expr)
{
  expr_common(file, line, pt_expr,
	      "ELABXXX",
	      "Range order conflict.");
}

// @brief named-event に対する範囲指定
// @param[in] file ファイル名
// @param[in] line 行番号
// @param[in] pt_expr 式を表すパース木
void
ErrorGen::select_for_namedevent(const char* file,
				int line,
				const PtExpr* pt_expr)
{
  expr_with_hname(file, line, pt_expr,
		  "ELABXXX",
		  "Illegal part-select for a named-event.");
}

// @brief assign/deassign に不適切なビット/範囲指定
// @param[in] file ファイル名
// @param[in] line 行番号
// @param[in] pt_expr 式を表すパース木
void
ErrorGen::select_in_pca(const char* file,
			int line,
			const PtExpr* pt_expr)
{
  expr_with_hname(file, line, pt_expr,
		  "ELABXXX",
		  "Bit/part-select shall not be used"
		  " in LHS of assign/deassign statement.");
}

// @brief force/release に不適切なビット/範囲指定
// @param[in] file ファイル名
// @param[in] line 行番号
// @param[in] pt_expr 式を表すパース木
void
ErrorGen::select_in_force(const char* file,
			  int line,
			  const PtExpr* pt_expr)
{
  expr_with_hname(file, line, pt_expr,
		  "ELABXXX",
		  "Bit/part-select shall not be used"
		  " in LHS of force/release statement.");
}

// @brief assign/deassign に不適切な配列要素
// @param[in] file ファイル名
// @param[in] line 行番号
// @param[in] pt_expr 式を表すパース木
void
ErrorGen::array_in_pca(const char* file,
		       int line,
		       const PtExpr* pt_expr)
{
  expr_with_hname(file, line, pt_expr,
		  "ELABXXX",
		  "Array element shall not be used"
		  "in LHS of assign/deassign statement.");
}

// @brief force/release に不適切な配列要素
// @param[in] file ファイル名
// @param[in] line 行番号
// @param[in] pt_expr 式を表すパース木
void
ErrorGen::array_in_force(const char* file,
			 int line,
			 const PtExpr* pt_expr)
{
  expr_with_hname(file, line, pt_expr,
		  "ELABXXX",
		  "Array element shall not be used"
		  "in LHS of force/release statement.");
}

// @brief 配列の次元が合わない
// @param[in] file ファイル名
// @param[in] line 行番号
// @param[in] pt_expr 式を表すパース木
void
ErrorGen::dimension_mismatch(const char* file,
			     int line,
			     const PtExpr* pt_expr)
{
  expr_with_hname(file, line, pt_expr,
		  "ELABXXX",
		  "Dimension mismatch.");
}

// @brief real 型に対するビット選択あるいは部分選択があった
// @param[in] file ファイル名
// @param[in] line 行番号
// @param[in] pt_expr 式を表すパース木
void
ErrorGen::select_for_real(const char* file,
			  int line,
			  const PtExpr* pt_expr)
{
  expr_with_hname(file, line, pt_expr,
		  "ELABXXX",
		  "Bit-select/Part-select for real.");
}

// @brief 階層名付きのエラーメッセージを生成する共通部分
// @param[in] file ファイル名
// @param[in] line 行番号
// @param[in] pt_expr 式を表すパース木
// @param[in] label エラーラベル
// @param[in] msg エラーメッセージ
void
ErrorGen::const_with_hname(const char* file,
			   int line,
			   const PtExpr* pt_expr,
			   const char* label,
			   const string& msg)
{
  auto named_msg{make_message(pt_expr, msg)};
  const_common(file, line, pt_expr, label, msg);
}

// @brief ElbConstError を生成する共通部分
// @param[in] file ファイル名
// @param[in] line 行番号
// @param[in] pt_expr 式を表すパース木
// @param[in] label エラーラベル
// @param[in] msg エラーメッセージ
void
ErrorGen::const_common(const char* file,
		       int line,
		       const PtExpr* pt_expr,
		       const char* label,
		       const string& msg)
{
  throw ElbConstError(file, line, pt_expr->file_region(), label, msg);
}

// @brief 階層名付きのエラーメッセージを生成する共通部分
// @param[in] file ファイル名
// @param[in] line 行番号
// @param[in] pt_expr 式を表すパース木
// @param[in] label エラーラベル
// @param[in] msg エラーメッセージ
void
ErrorGen::expr_with_hname(const char* file,
			  int line,
			  const PtExpr* pt_expr,
			  const char* label,
			  const string& msg)
{
  auto named_msg{make_message(pt_expr, msg)};
  expr_common(file, line, pt_expr, label, msg);
}

// @brief PtExpr に関するエラーメッセージを生成する共通部分
// @param[in] file ファイル名
// @param[in] line 行番号
// @param[in] pt_expr 式を表すパース木
// @param[in] label エラーラベル
// @param[in] msg エラーメッセージ
void
ErrorGen::expr_common(const char* file,
		      int line,
		      const PtExpr* pt_expr,
		      const char* label,
		      const string& msg)
{
  error_common(file, line, pt_expr->file_region(), label, msg);
}

// @brief エラーメッセージを生成する共通部分
// @param[in] file ファイル名
// @param[in] line 行番号
// @param[in] loc ファイル位置
// @param[in] label エラーラベル
// @param[in] msg エラーメッセージ
void
ErrorGen::error_common(const char* file,
		       int line,
		       const FileRegion& loc,
		       const char* label,
		       const string& msg)
{
  throw ElbError(file, line, loc, label, msg);
}

string
ErrorGen::make_message(const PtExpr* pt_expr,
		       const string& msg)
{
  ostringstream buf;
  buf << pt_expr->fullname() << " : " << msg;
  return buf.str();
}

END_NAMESPACE_YM_VERILOG
