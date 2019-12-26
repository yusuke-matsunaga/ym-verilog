
/// @file SptSpecItem.cc
/// @brief SptSpecItem の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "SptItem.h"
#include "parser/SptFactory.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// specify_block の item を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
SptSpecItem::SptSpecItem(const FileRegion& file_region,
			 VpiSpecItemType id,
			 PtExprArray terminal_array) :
  SptItem{file_region, kPtItem_SpecItem},
  mId{id},
  mTerminals{terminal_array}
{
}

// デストラクタ
SptSpecItem::~SptSpecItem()
{
}

// specify block item の種類の取得
VpiSpecItemType
SptSpecItem::specitem_type() const
{
  return mId;
}

// @brief ターミナルリストの取得
PtExprArray
SptSpecItem::terminal_list() const
{
  return mTerminals;
}


//////////////////////////////////////////////////////////////////////
// specify_block の path_declaration を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
SptSpecPath::SptSpecPath(const FileRegion& file_region,
			 VpiSpecPathType id,
			 const PtExpr* expr,
			 const PtPathDecl* path_decl) :
  SptItem{file_region, kPtItem_SpecPath},
  mId{id},
  mExpr{expr},
  mPathDecl{path_decl}
{
}

// デストラクタ
SptSpecPath::~SptSpecPath()
{
}

// specify block path の種類の取得
VpiSpecPathType
SptSpecPath::specpath_type() const
{
  return mId;
}

// モジュールパスの式を返す．
const PtExpr*
SptSpecPath::expr() const
{
  return mExpr;
}

// パス記述を返す．
const PtPathDecl*
SptSpecPath::path_decl() const
{
  return mPathDecl;
}


//////////////////////////////////////////////////////////////////////
// path_decl を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
SptPathDecl::SptPathDecl(const FileRegion& file_region,
			 int edge,
			 PtExprArray input_array,
			 int input_pol,
			 VpiPathType op,
			 PtExprArray output_array,
			 int output_pol,
			 const PtExpr* expr,
			 const PtPathDelay* path_delay) :
  mFileRegion{file_region},
  mEdge{edge},
  mInputs{input_array},
  mInputPol{input_pol},
  mOp{op},
  mOutputs{output_array},
  mOutputPol{output_pol},
  mExpr{expr},
  mPathDelay{path_delay}
{
}

// デストラクタ
SptPathDecl::~SptPathDecl()
{
}

// ファイル位置を返す．
FileRegion
SptPathDecl::file_region() const
{
  return mFileRegion;
}

// edge_descriptor を取り出す．
// 0の場合もある．
int
SptPathDecl::edge() const
{
  return mEdge;
}

// @brief 入力のリストの取得
PtExprArray
SptPathDecl::input_list() const
{
  return mInputs;
}

// 入力の極性を取り出す．
// 0の場合もありうる．
int
SptPathDecl::input_pol() const
{
  return mInputPol;
}

// パス記述子(?)を得る．vpiParallel か vpiFull
VpiPathType
SptPathDecl::op() const
{
  return mOp;
}

// @brief 出力のリストの取得
PtExprArray
SptPathDecl::output_list() const
{
  return mOutputs;
}

// 出力の極性を取り出す．
// 0の場合もありうる．
int
SptPathDecl::output_pol() const
{
  return mOutputPol;
}

// 式を取り出す．
// nullptr の場合もありうる．
const PtExpr*
SptPathDecl::expr() const
{
  return mExpr;
}

// path_delay_value を取り出す．
const PtPathDelay*
SptPathDecl::path_delay() const
{
  return mPathDelay;
}


//////////////////////////////////////////////////////////////////////
// path_delay_value を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
SptPathDelay::SptPathDelay(const FileRegion& file_region,
			   const PtExpr* value) :
  mFileRegion{file_region},
  mValues{value, nullptr, nullptr,
	  nullptr, nullptr, nullptr,
	  nullptr, nullptr, nullptr,
	  nullptr, nullptr, nullptr}
{
}

SptPathDelay::SptPathDelay(const FileRegion& file_region,
			   const PtExpr* value1,
			   const PtExpr* value2) :
  mFileRegion{file_region},
  mValues{value1, value2, nullptr,
	  nullptr, nullptr, nullptr,
	  nullptr, nullptr, nullptr,
	  nullptr, nullptr, nullptr}
{
}

SptPathDelay::SptPathDelay(const FileRegion& file_region,
			   const PtExpr* value1,
			   const PtExpr* value2,
			   const PtExpr* value3) :
  mFileRegion{file_region},
  mValues{value1, value2, value3,
	  nullptr, nullptr, nullptr,
	  nullptr, nullptr, nullptr,
	  nullptr, nullptr, nullptr}
{
}

SptPathDelay::SptPathDelay(const FileRegion& file_region,
			   const PtExpr* value1,
			   const PtExpr* value2,
			   const PtExpr* value3,
			   const PtExpr* value4,
			   const PtExpr* value5,
			   const PtExpr* value6) :
  mFileRegion{file_region},
  mValues{value1, value2, value3,
	  value4, value5, value6,
	  nullptr, nullptr, nullptr,
	  nullptr, nullptr, nullptr}
{
}

SptPathDelay::SptPathDelay(const FileRegion& file_region,
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
			   const PtExpr* value12) :
  mFileRegion{file_region},
  mValues{value1, value2, value3,
	  value4, value5, value6,
	  value7, value8, value9,
	  value10, value11, value12}
{
}

// デストラクタ
SptPathDelay::~SptPathDelay()
{
}

// ファイル位置を返す．
FileRegion
SptPathDelay::file_region() const
{
  return mFileRegion;
}

// 値を取り出す．
// 0の場合もある．
const PtExpr*
SptPathDelay::value(SizeType pos) const
{
  return mValues[pos];
}


//////////////////////////////////////////////////////////////////////
// item 関係の生成
//////////////////////////////////////////////////////////////////////

// @brief specify block item の生成
// @param[in] file_region ファイル位置の情報
// @param[in] id specify block item の種類
// @param[in] terminal_list 端子のリスト
// @return 生成された specify block item
const PtItem*
SptFactory::new_SpecItem(const FileRegion& file_region,
			 VpiSpecItemType id,
			 PtExprArray terminal_array)
{
  auto node = new SptSpecItem(file_region, id, terminal_array);
  return node;
}

// @brief path 仕様を生成する．
// @param[in] file_region ファイル位置の情報
// @param[in] id spec path の種類
// @param[in] expr 条件式
// @param[in] path_decl パス記述
// @return 生成された spec path
const PtItem*
SptFactory::new_SpecPath(const FileRegion& file_region,
			 VpiSpecPathType id,
			 const PtExpr* expr,
			 const PtPathDecl* path_decl)
{
  auto node = new SptSpecPath(file_region, id, expr, path_decl);
  return node;
}

// @brief パス記述の生成
// @param[in] file_region ファイル位置の情報
// @param[in] edge
// @param[in] input_array
// @param[in] input_pol
// @param[in] op
// @param[in] output_array
// @param[in] output_pol
// @param[in] expr
// @param[in] path_delay
// @return 生成されたパス記述
const PtPathDecl*
SptFactory::new_PathDecl(const FileRegion& file_region,
			 int edge,
			 PtExprArray input_array,
			 int input_pol,
			 VpiPathType op,
			 PtExprArray output_array,
			 int output_pol,
			 const PtExpr* expr,
			 const PtPathDelay* path_delay)
{
  auto node = new SptPathDecl(file_region, edge, input_array, input_pol,
			      op, output_array, output_pol,
			      expr, path_delay);
  return node;
}

// @brief path delay value の生成 (値が1つ)
// @param[in] file_region ファイル位置の情報
// @param[in] value 値
// @return 生成された path delay value
const PtPathDelay*
SptFactory::new_PathDelay(const FileRegion& file_region,
			  const PtExpr* value)
{
  auto node = new SptPathDelay(file_region, value);
  return node;
}

// @brief path delay value の生成 (値が2つ)
// @param[in] file_region ファイル位置の情報
// @param[in] value1 値1
// @param[in] value2 値2
// @return 生成された path delay value
const PtPathDelay*
SptFactory::new_PathDelay(const FileRegion& file_region,
			  const PtExpr* value1,
			  const PtExpr* value2)
{
  auto node = new SptPathDelay(file_region, value1, value2);
  return node;
}

// @brief path delay value の生成 (値が3つ)
// @param[in] file_region ファイル位置の情報
// @param[in] value1 値1
// @param[in] value2 値2
// @param[in] value3 値3
// @return 生成された path delay value
const PtPathDelay*
SptFactory::new_PathDelay(const FileRegion& file_region,
			  const PtExpr* value1,
			  const PtExpr* value2,
			  const PtExpr* value3)
{
  auto node = new SptPathDelay(file_region,
			       value1, value2, value3);
  return node;
}

// @brief path delay value の生成 (値が6つ)
// @param[in] file_region ファイル位置の情報
// @param[in] value1 値1
// @param[in] value2 値2
// @param[in] value3 値3
// @param[in] value4 値4
// @param[in] value5 値5
// @param[in] value6 値6
// @return 生成された path delay value
const PtPathDelay*
SptFactory::new_PathDelay(const FileRegion& file_region,
			  const PtExpr* value1,
			  const PtExpr* value2,
			  const PtExpr* value3,
			  const PtExpr* value4,
			  const PtExpr* value5,
			  const PtExpr* value6)
{
  auto node = new SptPathDelay(file_region,
			       value1, value2, value3,
			       value4, value5, value6);
  return node;
}

// @brief path delay value の生成 (値が12個)
// @param[in] file_region ファイル位置の情報
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
SptFactory::new_PathDelay(const FileRegion& file_region,
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
  auto node = new SptPathDelay(file_region,
			       value1, value2, value3,
			       value4, value5, value6,
			       value7, value8, value9,
			       value10, value11, value12);
  return node;
}

END_NAMESPACE_YM_VERILOG
