
/// @file Evaluator.cc
/// @brief Evaluator の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2020 Yusuke Matsunaga
/// All rights reserved.


#include "elaborator/Evaluator.h"
#include "ym/vl/VlTaskFunc.h"
#include "ym/vl/VlIODecl.h"
#include "ym/vl/VlDecl.h"
#include "ym/vl/VlDeclArray.h"
#include "ym/vl/VlStmt.h"
#include "ym/vl/VlExpr.h"
#include "ym/MsgMgr.h"


BEGIN_NAMESPACE_YM_VERILOG

// @brief コンストラクタ
// @param[in] function 関数
// @param[in] put_error エラーを出力する時 true にするフラグ
Evaluator::Evaluator(const VlTaskFunc* function,
		     bool put_error) :
  mFunction(function),
  mPutError{put_error}
{
}

// @brief デストラクタ
Evaluator::~Evaluator()
{
}

// @brief 関数を評価する．
// @param[in] arg_list 引数のリスト
VlValue
Evaluator::operator()(const vector<VlValue>& arg_list)
{
  // 入力変数の値をセットする．
  SizeType io_num{mFunction->io_num()};
  ASSERT_COND( arg_list.size() == io_num );

  for ( SizeType index = 0; index < io_num; ++ index ) {
    auto io{mFunction->io(index)};
    ASSERT_COND( io->direction() == VpiDir::Input );
    auto decl{io->decl()};
    reg_val(decl, arg_list[index]);
  }

  // 本体のステートメントを実行する．
  auto body{mFunction->stmt()};
  evaluate_stmt(body);

  // 出力結果を得る．
  auto ovar{mFunction->ovar()};
  auto val = get_val(ovar);

  return val;
}

// ステートメントを実行する．
const VlScope*
Evaluator::evaluate_stmt(const VlStmt* stmt)
{
  // ここでは個々のステートメント用の関数にディスパッチするだけ．
  const VlScope* break_scope = nullptr;
  switch ( stmt->type() ) {
  case VpiObjType::Begin:
    break_scope = evaluate_seqblock(stmt);
    break;

  case VpiObjType::NamedBegin:
    break_scope = evaluate_namedseqblock(stmt);
    break;

  case VpiObjType::NullStmt:
    // なにもしない．
    break;

  case VpiObjType::Assignment:
    break_scope = evaluate_assign(stmt);
    break;

  case VpiObjType::While:
    break_scope = evaluate_while(stmt);
    break;

  case VpiObjType::Repeat:
    break_scope = evaluate_repeat(stmt);
    break;

  case VpiObjType::For:
    break_scope = evaluate_for(stmt);
    break;

  case VpiObjType::Forever:
    break_scope = evaluate_forever(stmt);
    break;

  case VpiObjType::If:
    break_scope = evaluate_if(stmt);
    break;

  case VpiObjType::IfElse:
    break_scope = evaluate_ifelse(stmt);
    break;

  case VpiObjType::Case:
    break_scope = evaluate_case(stmt);
    break;

  case VpiObjType::Disable:
    break_scope = evaluate_disable(stmt);
    break;

  case VpiObjType::SysTaskCall:
    // constant expression の評価時には無視される．
    break;

  default:
    // 上記以外はエラー
    ASSERT_NOT_REACHED;
    break;
  }

  return break_scope;
}

// @brief begin-end ブロックの実行を行う．
// @param[in] stmt 対象のステートメント
// @return break 対象のスコープを返す．
const VlScope*
Evaluator::evaluate_seqblock(const VlStmt* stmt)
{
  SizeType n{stmt->child_stmt_num()};
  for ( SizeType i = 0; i < n; ++ i ) {
    auto break_scope{evaluate_stmt(stmt->child_stmt(i))};
    if ( break_scope ) {
      return break_scope;
    }
  }
  return nullptr;
}

// @brief 名前付き begin-end ブロックの実行を行う．
// @param[in] stmt 対象のステートメント
// @return break 対象のスコープを返す．
const VlScope*
Evaluator::evaluate_namedseqblock(const VlStmt* stmt)
{
  SizeType n{stmt->child_stmt_num()};
  for ( SizeType i = 0; i < n; ++ i ) {
    auto break_scope{evaluate_stmt(stmt->child_stmt(i))};
    if ( break_scope ) {
      if ( break_scope == stmt->scope() ) {
	// 自分自身のスコープの実行を終了する．
	// 上のスコープには通常の終了として通知する．
	break_scope = nullptr;
      }
      return break_scope;
    }
  }
  return nullptr;
}

// @brief 代入文の実行を行う．
// @param[in] stmt 対象のステートメント
// @return break 対象のスコープを返す．
const VlScope*
Evaluator::evaluate_assign(const VlStmt* stmt)
{
  ASSERT_COND( stmt->control() == nullptr );
  ASSERT_COND( stmt->is_blocking() );

  auto rhs{stmt->rhs()};
  auto val{evaluate_expr(rhs)};

  // lhs に val を代入する．
  auto lhs{stmt->lhs()};
  SizeType nl = lhs->lhs_elem_num();
  if ( nl == 1 ) {
    assign_value(lhs, val);
  }
  else {
    // 左辺が連結式の場合
    auto bv{val.bitvector_value()};
    SizeType base = 0;
    for ( SizeType i = 0; i < nl; ++ i ) {
      auto expr{lhs->lhs_elem(i)};
      SizeType w{expr->bit_size()};
      auto part_val{bv.part_select_op(base + w - 1, base)};
      assign_value(expr, VlValue(part_val));
      base += w;
    }
  }

  return nullptr;
}

// @brief 左辺の要素を値を代入する．
// @param[in] expr 左辺式
// @param[in] val 値
//
// expr は primary か bit-select, part-select
void
Evaluator::assign_value(const VlExpr* expr,
			const VlValue& val)
{
  // 対象が
  // - 単独の要素
  // - 配列要素
  // の２通り．
  // 代入範囲が
  // - 要素全体
  // - ビット選択
  // - 範囲選択
  // の3通りがある．

  auto decl{expr->decl_obj()};
  auto declarray{expr->declarray_obj()};
  if ( decl ) {
    // 単独の要素
    if ( expr->is_primary() ) {
      // プライマリ
      reg_val(decl, val);
    }
    else if ( expr->is_bitselect() ) {
      // ビット選択
      auto index_expr{expr->index()};
      int index{evaluate_int(index_expr)};
      reg_val(decl, val, index);
    }
    else if ( expr->is_partselect() ) {
      // 範囲選択
      auto left_expr{expr->left_range()};
      int left_index{evaluate_int(left_expr)};
      auto right_expr{expr->right_range()};
      int right_index{evaluate_int(right_expr)};
      reg_val(decl, val, left_index, right_index);
    }
  }
  else if ( declarray ) {
    // インデックスの値を求める．
    SizeType n{expr->declarray_dimension()};
    vector<int> index_array(n);
    for ( SizeType i = 0; i < n; ++ i ) {
      auto index_expr{expr->declarray_index(i)};
      index_array[i] = evaluate_int(index_expr);
    }
    SizeType offset;
    bool stat = declarray->calc_array_offset(index_array, offset);
    ASSERT_COND( stat );
    if ( expr->is_primary() ) {
      // プライマリ
      reg_val(declarray, offset, val);
    }
    else if ( expr->is_bitselect() ) {
      // ビット選択
      auto index_expr{expr->index()};
      int index{evaluate_int(index_expr)};
      reg_val(declarray, offset, val, index);
    }
    else if ( expr->is_partselect() ) {
      // 範囲選択
      auto left_expr{expr->left_range()};
      int left_index{evaluate_int(left_expr)};
      auto right_expr{expr->right_range()};
      int right_index{evaluate_int(right_expr)};
      reg_val(declarray, offset, val, left_index, right_index);
    }
  }
}

// @brief while 文の実行を行う．
// @param[in] stmt 対象のステートメント
// @return break 対象のスコープを返す．
const VlScope*
Evaluator::evaluate_while(const VlStmt* stmt)
{
  auto cond_expr{stmt->expr()};
  auto body_stmt{stmt->body_stmt()};
  while ( evaluate_bool(cond_expr) ) {
    auto break_scope{evaluate_stmt(body_stmt)};
    if ( break_scope ) {
      return break_scope;
    }
  }
  return nullptr;
}

// @brief repeat 文の実行を行う．
// @param[in] stmt 対象のステートメント
// @return break 対象のスコープを返す．
const VlScope*
Evaluator::evaluate_repeat(const VlStmt* stmt)
{
  auto rep_expr{stmt->expr()};
  int rep_num{evaluate_int(rep_expr)};
  auto body_stmt{stmt->body_stmt()};
  for ( int i = 0; i < rep_num; ++ i ) {
    auto break_scope{evaluate_stmt(body_stmt)};
    if ( break_scope ) {
      return break_scope;
    }
  }
  return nullptr;
}

// @brief for 文の実行を行う．
// @param[in] stmt 対象のステートメント
// @return break 対象のスコープを返す．
const VlScope*
Evaluator::evaluate_for(const VlStmt* stmt)
{
  auto cond_expr{stmt->expr()};
  auto init_stmt{stmt->init_stmt()};
  auto body_stmt{stmt->body_stmt()};
  auto next_stmt{stmt->inc_stmt()};
  auto break_scope{evaluate_stmt(init_stmt)};
  if ( break_scope ) {
    return break_scope;
  }
  while ( evaluate_bool(cond_expr) ) {
    break_scope = evaluate_stmt(body_stmt);
    if ( break_scope ) {
      return break_scope;
    }
    break_scope = evaluate_stmt(next_stmt);
    if ( break_scope ) {
      return break_scope;
    }
  }
  return nullptr;
}

// @brief forever 文の実行を行う．
// @param[in] stmt 対象のステートメント
// @return break 対象のスコープを返す．
const VlScope*
Evaluator::evaluate_forever(const VlStmt* stmt)
{
  auto body_stmt{stmt->body_stmt()};
  for ( ; ; ) {
    auto break_scope{evaluate_stmt(body_stmt)};
    if ( break_scope ) {
      return break_scope;
    }
  }
  return nullptr;
}

// @brief if 文の実行を行う．
// @param[in] stmt 対象のステートメント
// @return break 対象のスコープを返す．
const VlScope*
Evaluator::evaluate_if(const VlStmt* stmt)
{
  auto cond_expr{stmt->expr()};
  if ( evaluate_bool(cond_expr) ) {
    auto then_stmt{stmt->body_stmt()};
    auto break_scope{evaluate_stmt(then_stmt)};
    return break_scope;
  }
  return nullptr;
}

// @brief if-else 文の実行を行う．
// @param[in] stmt 対象のステートメント
// @return break 対象のスコープを返す．
const VlScope*
Evaluator::evaluate_ifelse(const VlStmt* stmt)
{
  auto cond_expr{stmt->expr()};
  if ( evaluate_bool(cond_expr) ) {
    auto then_stmt{stmt->body_stmt()};
    auto break_scope{evaluate_stmt(then_stmt)};
    return break_scope;
  }
  else {
    auto else_stmt{stmt->else_stmt()};
    auto break_scope{evaluate_stmt(else_stmt)};
    return break_scope;
  }
}

// @brief case 文の実行を行う．
// @param[in] stmt 対象のステートメント
// @return break 対象のスコープを返す．
const VlScope*
Evaluator::evaluate_case(const VlStmt* stmt)
{
  auto case_type{stmt->case_type()};
  auto expr{stmt->expr()};
  auto switch_val{evaluate_expr(expr)};
  SizeType n{stmt->caseitem_num()};
  for ( SizeType i = 0; i < n; ++ i ) {
    auto caseitem{stmt->caseitem(i)};
    if ( match(case_type, switch_val, caseitem) ) {
      auto body{caseitem->body_stmt()};
      auto break_scope{evaluate_stmt(body)};
      return break_scope;
    }
  }
  return nullptr;
}

// @brief caseitem のラベルと一致するか調べる．
// @param[in] case_type case 文の種類
// @param[in] val case 文の値
// @param[in] caseitem 対象の caseitem
bool
Evaluator::match(VpiCaseType case_type,
		 const VlValue& val,
		 const VlCaseItem* caseitem)
{
  SizeType nexp{caseitem->expr_num()};
  if ( nexp == 0 ) {
    // default なのでマッチする．
    return true;
  }

  for ( SizeType i = 0; i < nexp; ++ i ) {
    auto label_expr{caseitem->expr(i)};
    auto label_val{evaluate_expr(label_expr)};
    VlValue eq_val;
    if ( case_type == VpiCaseType::Exact ) {
      eq_val = eq(val, label_val);
    }
    else if ( case_type == VpiCaseType::X ) {
      eq_val = eq_with_x(val, label_val);
    }
    else if ( case_type == VpiCaseType::Z ) {
      eq_val = eq_with_xz(val, label_val);
    }
    else {
      ASSERT_NOT_REACHED;
    }
    return eq_val.logic_value().to_bool();
  }
  return false;
}

// @brief disable 文の実行を行う．
// @param[in] stmt 対象のステートメント
// @return break 対象のスコープを返す．
const VlScope*
Evaluator::evaluate_disable(const VlStmt* stmt)
{
  return stmt->target_scope();
}

// @brief 式の評価を行う．
// @param[in] expr 対象の式を表す構文木要素
VlValue
Evaluator::evaluate_expr(const VlExpr* expr)
{
  switch ( expr->type() ) {
  case VpiObjType::Operation:   return evaluate_opr(expr);
  case VpiObjType::Constant:    return evaluate_const(expr);
  case VpiObjType::FuncCall:    return evaluate_funccall(expr);
  case VpiObjType::SysFuncCall: return VlValue(); // 定数式の中では無視
  default: // ここに来たらプライマリ系
           // bitselect, partselect も馥府．
    return evaluate_primary(expr);
  }
  ASSERT_NOT_REACHED;
  return VlValue();
}

// @brief 演算子に対して式の値を評価する．
// @param[in] expr 対象の式
VlValue
Evaluator::evaluate_opr(const VlExpr* expr)
{
  SizeType n_op{expr->operand_num()};
  // オペランドの値を評価する．
  vector<VlValue> opr_vals(n_op);
  for ( SizeType i = 0; i < n_op; ++ i ) {
    opr_vals[i] = evaluate_expr(expr->operand(i));
  }

  // ほとんど VlValue の該当の関数を呼び出すだけ．
  switch ( expr->op_type() ) {
  case VpiOpType::Minus:
    return - opr_vals[0];
  case VpiOpType::Plus:
    return opr_vals[0];
  case VpiOpType::Not:
    return log_not(opr_vals[0]);
  case VpiOpType::BitNeg:
    return bit_negate(opr_vals[0]);
  case VpiOpType::UnaryAnd:
    return reduction_and(opr_vals[0]);
  case VpiOpType::UnaryNand:
    return reduction_nand(opr_vals[0]);
  case VpiOpType::UnaryOr:
    return reduction_or(opr_vals[0]);
  case VpiOpType::UnaryNor:
    return reduction_nor(opr_vals[0]);
  case VpiOpType::UnaryXor:
    return reduction_xor(opr_vals[0]);
  case VpiOpType::UnaryXNor:
    return reduction_xnor(opr_vals[0]);
  case VpiOpType::Sub:
    return opr_vals[0] - opr_vals[1];
  case VpiOpType::Div:
    return opr_vals[0] / opr_vals[1];
  case VpiOpType::Mod:
    return opr_vals[0] % opr_vals[1];
  case VpiOpType::Eq:
    return eq(opr_vals[0], opr_vals[1]);
  case VpiOpType::Neq:
    return ne(opr_vals[0], opr_vals[1]);
  case VpiOpType::CaseEq:
    return eq_with_x(opr_vals[0], opr_vals[1]);
  case VpiOpType::CaseNeq:
    return log_not(eq_with_x(opr_vals[0], opr_vals[1]));
  case VpiOpType::Gt:
    return gt(opr_vals[0], opr_vals[1]);
  case VpiOpType::Ge:
    return ge(opr_vals[0], opr_vals[1]);
  case VpiOpType::Lt:
    return lt(opr_vals[0], opr_vals[1]);
  case VpiOpType::Le:
    return le(opr_vals[0], opr_vals[1]);
  case VpiOpType::LShift:
    return opr_vals[0] << opr_vals[1];
  case VpiOpType::RShift:
    return opr_vals[0] >> opr_vals[1];
  case VpiOpType::Add:
    return opr_vals[0] + opr_vals[1];
  case VpiOpType::Mult:
    return opr_vals[0] * opr_vals[1];
  case VpiOpType::LogAnd:
    return log_and(opr_vals[0], opr_vals[1]);
  case VpiOpType::LogOr:
    return log_or(opr_vals[0], opr_vals[1]);
  case VpiOpType::BitAnd:
    return bit_and(opr_vals[0], opr_vals[1]);
  case VpiOpType::BitOr:
    return bit_or(opr_vals[0], opr_vals[1]);
  case VpiOpType::BitXor:
    return bit_xor(opr_vals[0], opr_vals[1]);
  case VpiOpType::BitXNor:
    return bit_xnor(opr_vals[0], opr_vals[1]);
  case VpiOpType::Condition:
    return ite(opr_vals[0], opr_vals[1], opr_vals[2]);
  case VpiOpType::Concat:
    return concat(opr_vals);
  case VpiOpType::MultiConcat:
    return multi_concat(opr_vals);
  case VpiOpType::ArithLShift:
    // 算術左シフトは普通の左シフトと同じ
    return opr_vals[0] << opr_vals[1];
  case VpiOpType::ArithRShift:
    return arshift(opr_vals[0], opr_vals[1]);
  case VpiOpType::Power:
    return power(opr_vals[0], opr_vals[1]);

  case VpiOpType::EventOr:
  case VpiOpType::Null:
  case VpiOpType::List:
  case VpiOpType::MinTypMax:
  case VpiOpType::Posedge:
  case VpiOpType::Negedge:
    // これらは使えない．
    ASSERT_NOT_REACHED;
    return VlValue();
  }
}

// @brief 定数に対して式の値を評価する．
// @param[in] expr 対象の式
VlValue
Evaluator::evaluate_const(const VlExpr* expr)
{
  return expr->constant_value();
}

// @brief プライマリに対して式の値を評価する．
// @param[in] expr 対象の式
VlValue
Evaluator::evaluate_primary(const VlExpr* expr)
{
  auto decl{expr->decl_obj()};
  auto declarray{expr->declarray_obj()};
  VlValue base_val;
  if ( decl ) {
    base_val = get_val(decl);
  }
  else if ( declarray ) {
    // インデックスの値を求める．
    SizeType n{expr->declarray_dimension()};
    vector<int> index_array(n);
    for ( SizeType i = 0; i < n; ++ i ) {
      auto index_expr = expr->declarray_index(i);
      index_array[i] = evaluate_int(index_expr);
    }
    SizeType offset;
    bool stat = declarray->calc_array_offset(index_array, offset);
    ASSERT_COND( stat );
    base_val = get_val(declarray, offset);
  }
  else {
    ASSERT_NOT_REACHED;
  }

  if ( expr->is_primary() ) {
    return base_val;
  }
  else if ( expr->is_bitselect() ) {
    auto index_expr = expr->index();
    int index = evaluate_int(index_expr);
    auto bv = base_val.bitvector_value();
    return VlValue(bv.bit_select_op(index));
  }
  else if ( expr->is_partselect() ) {
    auto left_expr = expr->left_range();
    int left_index = evaluate_int(left_expr);
    auto right_expr = expr->right_range();
    int right_index = evaluate_int(right_expr);
    auto bv = base_val.bitvector_value();
    return VlValue(bv.part_select_op(left_index, right_index));
  }

  ASSERT_NOT_REACHED;
  return VlValue();
}

// @brief 関数呼び出しに対して式の値を評価する．
// @param[in] expr 対象の式
VlValue
Evaluator::evaluate_funccall(const VlExpr* expr)
{
  auto func{expr->function()};

  // 引数の値を評価する．
  SizeType n_io{expr->argument_num()};
  ASSERT_COND( n_io == func->io_num() );
  vector<VlValue> arg_list(n_io);
  for ( SizeType i = 0; i < n_io; ++ i ) {
    arg_list[i] = evaluate_expr(expr->argument(i));
  }

  Evaluator eval(func, mPutError);
  return eval(arg_list);
}

// @brief 式を評価して整数値を返す．
// @param[in] expr 対象の式
int
Evaluator::evaluate_int(const VlExpr* expr)
{
  auto val = evaluate_expr(expr);
  if ( !val.is_int_compat() ) {
    if ( mPutError ) {
      MsgMgr::put_msg(__FILE__, __LINE__,
		      expr->file_region(),
		      MsgType::Error,
		      "ELAB",
		      "Integer value required.");
    }
    return 0;
  }
  return val.int_value();
}

// @brief 式を評価して整数値を返す．
// @param[in] expr 対象の式
bool
Evaluator::evaluate_bool(const VlExpr* expr)
{
  auto val = evaluate_expr(expr);
  if ( !val.is_int_compat() ) {
    if ( mPutError ) {
      MsgMgr::put_msg(__FILE__, __LINE__,
		      expr->file_region(),
		      MsgType::Error,
		      "ELAB",
		      "Integer value required.");
    }
    return 0;
  }
  return val.logic_value().to_bool();
}

// @brief 値を登録する．
// @param[in] obj 対象のオブジェクト
// @param[in] val 値
//
// 単独のオブジェクト用
void
Evaluator::reg_val(const VlDeclBase* obj,
		   const VlValue& val)
{
  reg_val(obj, 0, val);
}

// @brief 値を登録する．
// @param[in] obj 対象のオブジェクト
// @param[in] offset オフセット
// @param[in] val 値
//
// 配列要素用
void
Evaluator::reg_val(const VlDeclBase* obj,
		   SizeType offset,
		   const VlValue& val)
{
  Key key{obj, offset};
  mValMap.emplace(key, val);
}

// @brief 値を登録する．
// @param[in] obj 対象のオブジェクト
// @param[in] val 値
// @param[in] index ビット選択インデックス
//
// 単独のオブジェクト用
void
Evaluator::reg_val(const VlDeclBase* obj,
		   const VlValue& val,
		   int index)
{
  reg_val(obj, 0, val, index);
}

// @brief 値を登録する．
// @param[in] obj 対象のオブジェクト
// @param[in] offset オフセット
// @param[in] val 値
// @param[in] index ビット選択インデックス
//
// 配列要素用
void
Evaluator::reg_val(const VlDeclBase* obj,
		   SizeType offset,
		   const VlValue& val,
		   int index)
{
  Key key{obj, offset};
  BitVector bv;
  if ( mValMap.count(key) > 0 ) {
    auto val0 = mValMap.at(key);
    ASSERT_COND( val.is_bitvector_compat() );
    bv = val0.bitvector_value();
  }
  else {
    bv = BitVector(VlScalarVal::x(), obj->bit_size());
  }

  auto sv = val.scalar_value();
  bv.bit_select_op(index, sv);
  mValMap.emplace(key, VlValue(bv));
}

// @brief 値を登録する．
// @param[in] obj 対象のオブジェクト
// @param[in] val 値
// @param[in] left, right 範囲選択のインデックス
//
// 単独のオブジェクト用
void
Evaluator::reg_val(const VlDeclBase* obj,
		   const VlValue& val,
		   int left,
		   int right)
{
  reg_val(obj, 0, val, left, right);
}

// @brief 値を登録する．
// @param[in] obj 対象のオブジェクト
// @param[in] offset オフセット
// @param[in] val 値
// @param[in] left, right 範囲選択のインデックス
//
// 配列要素用
void
Evaluator::reg_val(const VlDeclBase* obj,
		   SizeType offset,
		   const VlValue& val,
		   int left,
		   int right)
{
  Key key{obj, offset};
  BitVector bv;
  if ( mValMap.count(key) > 0 ) {
    auto val0 = mValMap.at(key);
    ASSERT_COND( val.is_bitvector_compat() );
    bv = val0.bitvector_value();
  }
  else {
    bv = BitVector(VlScalarVal::x(), obj->bit_size());
  }

  bv.part_select_op(left, right, val.bitvector_value());
  mValMap.emplace(key, VlValue(bv));
}

// @brief 値を取り出す．
// @param[in] obj 対象のオブジェクト
//
// 単独のオブジェクト用
VlValue
Evaluator::get_val(const VlDeclBase* obj)
{
  return get_val(obj, 0);
}

// @brief 値を取り出す．
// @param[in] obj 対象のオブジェクト
// @param[in] offset オフセット
//
// 配列要素用
VlValue
Evaluator::get_val(const VlDeclBase* obj,
		   SizeType offset)
{
  Key key{obj, offset};
  ASSERT_COND( mValMap.count(key) > 0 );
  return mValMap.at(key);
}

END_NAMESPACE_YM_VERILOG
