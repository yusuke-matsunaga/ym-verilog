/// @file Parser_item.cc
/// @brief Parser の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.


#include "Parser_int.h"
#include "parser/PtiFactory.h"
#include "ym/pt/PtItem.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// defparam の生成
//////////////////////////////////////////////////////////////////////

// @brief defparam 文のヘッダの生成
// @param[in] fr ファイル位置の情報
const PtItem*
Parser::new_DefParamH(const FileRegion& fr)
{
  auto item{mFactory.new_DefParamH(fr, mDefParamList)};
  return item;
}

// @brief defparam リストを初期化する．
void
Parser::init_defparam()
{
  mDefParamList.clear();
}

// @brief defparam 文の要素の生成
// @param[in] fr ファイル位置の情報
// @param[in] name 名前
// @param[in] value 値を表す式
void
Parser::new_DefParam(const FileRegion& fr,
		     const char* name,
		     const PtExpr* value)
{
  auto defparam{mFactory.new_DefParam(fr, name, value)};
  add_defparam(defparam);
}

// @brief defparam 文の要素の生成 (階層つき識別子)
// @param[in] fr ファイル位置の情報
// @param[in] hname 階層名
// @param[in] value 値を表す式
void
Parser::new_DefParam(const FileRegion& fr,
		     PuHierName* hname,
		     const PtExpr* value)
{
  auto defparam{mFactory.new_DefParam(fr, hname, value)};
  add_defparam(defparam);
}

// @brief defparam リストに要素を追加する．
inline
void
Parser::add_defparam(const PtDefParam* defparam)
{
  mDefParamList.push_back(defparam);
}


//////////////////////////////////////////////////////////////////////
// continuous assign の生成
//////////////////////////////////////////////////////////////////////

// @brief continuous assign 文のヘッダの生成
// @param[in] fr ファイル位置の情報
const PtItem*
Parser::new_ContAssignH(const FileRegion& fr)
{
  auto item{mFactory.new_ContAssignH(fr, mContAssignList)};

  return item;
}

// @brief continuous assign 文のヘッダの生成 (strengthつき)
// @param[in] fr ファイル位置の情報
// @param[in] strength 信号強度
const PtItem*
Parser::new_ContAssignH(const FileRegion& fr,
			const PtStrength* strength)
{
  auto item{mFactory.new_ContAssignH(fr, strength, mContAssignList)};
  return item;
}

// @brief continuous assign 文のヘッダの生成 (遅延付き)
// @param[in] fr ファイル位置の情報
// @param[in] delay 遅延値
const PtItem*
Parser::new_ContAssignH(const FileRegion& fr,
			const PtDelay* delay)
{
  auto item{mFactory.new_ContAssignH(fr, delay, mContAssignList)};
  return item;
}

// @brief continuous assign 文のヘッダの生成 (strength, 遅延付き)
// @param[in] fr ファイル位置の情報
// @param[in] strength 信号強度
// @param[in] delay 遅延値
// @param[in] elem_array 要素のリスト
const PtItem*
Parser::new_ContAssignH(const FileRegion& fr,
			const PtStrength* strength,
			const PtDelay* delay)
{
  auto item{mFactory.new_ContAssignH(fr, strength, delay, mContAssignList)};
  return item;
}

// @brief contassign リストを初期化する．
void
Parser::init_contassign()
{
  mContAssignList.clear();
}

// @brief continuous assign 文の生成
// @param[in] fr ファイル位置の情報
// @param[in] lhs 左辺式
// @param[in] rhs 右辺式
void
Parser::new_ContAssign(const FileRegion& fr,
		       const PtExpr* lhs,
		       const PtExpr* rhs)
{
  auto ca{mFactory.new_ContAssign(fr, lhs, rhs)};
  add_contassign(ca);
}

// @brief contassign リストに要素を追加する．
inline
void
Parser::add_contassign(const PtContAssign* contassign)
{
  mContAssignList.push_back(contassign);
}


//////////////////////////////////////////////////////////////////////
// process の生成
//////////////////////////////////////////////////////////////////////

// @brief initial 文の生成
// @param[in] fr ファイル位置の情報
// @param[in] body 本体のステートメント
const PtItem*
Parser::new_Initial(const FileRegion& fr,
		    const PtStmt* body)
{
  auto item{mFactory.new_Initial(fr, body)};
  return item;
}

// @brief always 文の生成
// @param[in] fr ファイル位置の情報
// @param[in] body 本体のステートメント
const PtItem*
Parser::new_Always(const FileRegion& fr,
		   const PtStmt* body)
{
  auto item{mFactory.new_Always(fr, body)};
  return item;
}


//////////////////////////////////////////////////////////////////////
// task/function の生成
//////////////////////////////////////////////////////////////////////

// @brief task/function 定義の開始
// - iohead list の初期化
// - paramhead list の初期化
// - localparamhead list の初期化
// - declhead list の初期化
// を行う．
void
Parser::init_tf()
{
  mCurIOHeadList = &mTfIOHeadList;
  push_declhead_list();

  mCurIOHeadList->clear();
  mIOItemList.clear();
  cur_declhead_list().clear();
  mDeclItemList.clear();
}

// @brief task/function 定義の終了
void
Parser::end_tf()
{
  mCurIOHeadList = &mModuleIOHeadList;
  mCurDeclArray = pop_declhead_list();
}

// @brief task 文の生成
// @param[in] fr ファイル位置の情報
// @param[in] name task 名
// @param[in] automatic automatic task の時に true となるフラグ
// @param[in] stmt 本体のステートメント
const PtItem*
Parser::new_Task(const FileRegion& fr,
		 const char* name,
		 bool automatic,
		 const PtStmt* stmt)
{
  auto item{mFactory.new_Task(fr, name, automatic,
			      get_tf_io_array(),
			      mCurDeclArray,
			      stmt)};
  return item;
}

// @brief 1ビット型 function 文の生成
// @param[in] fr ファイル位置の情報
// @param[in] name function 名
// @param[in] automatic automatic task の時に true となるフラグ
// @param[in] sign signed 属性がついていたら true となるフラグ
// @param[in] stmt 本体のステートメント
const PtItem*
Parser::new_Function(const FileRegion& fr,
		     const char* name,
		     bool automatic,
		     bool sign,
		     const PtStmt* stmt)
{
  auto item{mFactory.new_Function(fr, name, automatic,
				  sign,
				  get_tf_io_array(),
				  mCurDeclArray,
				  stmt)};
  return item;
}

// @brief 範囲指定型 function 文の生成
// @param[in] fr ファイル位置の情報
// @param[in] name function 名
// @param[in] automatic automatic task の時に true となるフラグ
// @param[in] sign 符号つきの時に true となるフラグ
// @param[in] left 範囲の左側の式
// @param[in] right 範囲の右側の式
// @param[in] stmt 本体のステートメント
const PtItem*
Parser::new_SizedFunc(const FileRegion& fr,
		      const char* name,
		      bool automatic,
		      bool sign,
		      const PtExpr* left,
		      const PtExpr* right,
		      const PtStmt* stmt)
{
  auto item{mFactory.new_SizedFunc(fr, name, automatic,
				   sign, left, right,
				   get_tf_io_array(),
				   mCurDeclArray,
				   stmt)};
  return item;
}

// @brief 組み込み型 function 文の生成
// @param[in] fr ファイル位置の情報
// @param[in] name function 名
// @param[in] automatic automatic task の時に true となるフラグ
// @param[in] sign signed 属性がついていたら true となるフラグ
// @param[in] func_type 関数の戻値の型
// @param[in] stmt 本体のステートメント
const PtItem*
Parser::new_TypedFunc(const FileRegion& fr,
		      const char* name,
		      bool automatic,
		      bool sign,
		      VpiVarType func_type,
		      const PtStmt* stmt)
{
  auto item{mFactory.new_TypedFunc(fr, name, automatic,
				   sign, func_type,
				   get_tf_io_array(),
				   mCurDeclArray,
				   stmt)};
  return item;
}


//////////////////////////////////////////////////////////////////////
// spec item の生成
//////////////////////////////////////////////////////////////////////

// @brief specify block item の生成
// @param[in] fr ファイル位置の情報
// @param[in] id specify block item の種類
// @param[in] terminal_list 端子のリスト
void
Parser::new_SpecItem(const FileRegion& fr,
		     VpiSpecItemType id,
		     PtrList<const PtExpr>* terminal_list)
{
  auto item{mFactory.new_SpecItem(fr, id, terminal_list->to_vector())};
  add_item(item);
}

// @brief path 仕様を生成する．
// @param[in] fr ファイル位置の情報
// @param[in] id spec path の種類
// @param[in] expr 条件式
// @param[in] path_decl パス記述
void
Parser::new_SpecPath(const FileRegion& fr,
		     VpiSpecPathType id,
		     const PtExpr* expr,
		     const PtPathDecl* path_decl)
{
  auto item{mFactory.new_SpecPath(fr, id, expr, path_decl)};
  add_item(item);
}

// @brief パス記述の生成
// @param[in] fr ファイル位置の情報
// @param[in] edge
// @param[in] input_list
// @param[in] input_pol
// @param[in] op
// @param[in] output_list
// @param[in] output_pol
// @param[in] expr
// @param[in] path_delay
const PtPathDecl*
Parser::new_PathDecl(const FileRegion& fr,
		     int edge,
		     PtrList<const PtExpr>* input_list,
		     int input_pol,
		     VpiPathType op,
		     PtrList<const PtExpr>* output_list,
		     int output_pol,
		     const PtExpr* expr,
		     const PtPathDelay* path_delay)
{
  auto item{mFactory.new_PathDecl(fr, edge,
				  input_list->to_vector(), input_pol,
				  op,
				  output_list->to_vector(), output_pol,
				  expr, path_delay)};
  return item;
}

// @brief パス記述の生成
// @param[in] fr ファイル位置の情報
// @param[in] edge
// @param[in] input_list
// @param[in] input_pol
// @param[in] op
// @param[in] output
// @param[in] output_pol
// @param[in] expr
// @param[in] path_delay
const PtPathDecl*
Parser::new_PathDecl(const FileRegion& fr,
		     int edge,
		     PtrList<const PtExpr>* input_list,
		     int input_pol,
		     VpiPathType op,
		     const PtExpr* output,
		     int output_pol,
		     const PtExpr* expr,
		     const PtPathDelay* path_delay)
{
  auto item{mFactory.new_PathDecl(fr, edge,
				  input_list->to_vector(), input_pol,
				  op,
				  output, output_pol,
				  expr, path_delay)};
  return item;
}

// @brief path delay value の生成 (値が1個)
// @param[in] fr ファイル位置の情報
// @param[in] value 値
// @return 生成された path delay value
const PtPathDelay*
Parser::new_PathDelay(const FileRegion& fr,
		      const PtExpr* value)
{
  auto item{mFactory.new_PathDelay(fr, value)};
  return item;
}

// @brief path delay value の生成 (値が2個)
// @param[in] fr ファイル位置の情報
// @param[in] value1 値1
// @param[in] value2 値2
// @return 生成された path delay value
const PtPathDelay*
Parser::new_PathDelay(const FileRegion& fr,
		      const PtExpr* value1,
		      const PtExpr* value2)
{
  auto item{mFactory.new_PathDelay(fr, value1, value2)};
  return item;
}

// @brief path delay value の生成 (値が3個)
// @param[in] fr ファイル位置の情報
// @param[in] value1 値1
// @param[in] value2 値2
// @param[in] value3 値3
// @return 生成された path delay value
const PtPathDelay*
Parser::new_PathDelay(const FileRegion& fr,
		      const PtExpr* value1,
		      const PtExpr* value2,
		      const PtExpr* value3)
{
  auto item{mFactory.new_PathDelay(fr, value1, value2, value3)};
  return item;
}

// @brief path delay value の生成 (値が6個)
// @param[in] fr ファイル位置の情報
// @param[in] value1 値1
// @param[in] value2 値2
// @param[in] value3 値3
// @param[in] value4 値4
// @param[in] value5 値5
// @param[in] value6 値6
// @return 生成された path delay value
const PtPathDelay*
Parser::new_PathDelay(const FileRegion& fr,
		      const PtExpr* value1,
		      const PtExpr* value2,
		      const PtExpr* value3,
		      const PtExpr* value4,
		      const PtExpr* value5,
		      const PtExpr* value6)
{
  auto item{mFactory.new_PathDelay(fr,
				   value1, value2, value3,
				   value4, value5, value6)};
  return item;
}

// @brief path delay value の生成 (値が12個)
// @param[in] fr ファイル位置の情報
// @param[in] value1 値1
// @param[in] value2 値2
// @param[in] value3 値3
// @param[in] value4 値4
// @param[in] value5 値5
// @param[in] value6 値6
// @param[in] value7 値7
// @param[in] value8 値8
// @param[in] value9 値9
// @param[in] value10 値10
// @param[in] value11 値11
// @param[in] value12 値12
// @return 生成された path delay value
const PtPathDelay*
Parser::new_PathDelay(const FileRegion& fr,
		      const PtExpr* value1,
		      const PtExpr* value2,
		      const PtExpr* value3,
		      const PtExpr* value4,
		      const PtExpr* value5,
		      const PtExpr* value6,
		      const PtExpr* value7,
		      const PtExpr* value8,
		      const PtExpr* value9,
		      const PtExpr* value10,
		      const PtExpr* value11,
		      const PtExpr* value12)
{
  auto item{mFactory.new_PathDelay(fr,
				   value1, value2, value3,
				   value4, value5, value6,
				   value7, value8, value9,
				   value10, value11, value12)};
  return item;
}

// @brief task/function 用の IO宣言リストを配列に変換する．
inline
vector<const PtIOHead*>
Parser::get_tf_io_array()
{
  return convert<const PtIOHead*, PtiIOHead*>(mTfIOHeadList);
}

END_NAMESPACE_YM_VERILOG
