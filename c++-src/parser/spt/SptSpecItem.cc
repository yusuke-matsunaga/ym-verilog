
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
SptSpecItem::SptSpecItem(
  const FileRegion& file_region,
  VpiSpecItemType id,
  PtiExprArray&& terminal_array
) : SptItem{file_region, PtItemType::SpecItem},
    mId{id},
    mTerminals{std::move(terminal_array)}
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

// @brief ターミナルの要素数の取得
SizeType
SptSpecItem::terminal_num() const
{
  return mTerminals.size();
}

// @brief ターミナルの取得
const PtExpr*
SptSpecItem::terminal(
  SizeType pos
) const
{
  return mTerminals[pos];
}


//////////////////////////////////////////////////////////////////////
// specify_block の path_declaration を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
SptSpecPath::SptSpecPath(
  const FileRegion& file_region,
  VpiSpecPathType id,
  const PtExpr* expr,
  const PtPathDecl* path_decl
) : SptItem{file_region, PtItemType::SpecPath},
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
SptPathDecl::SptPathDecl(
  const FileRegion& file_region,
  int edge,
  PtiExprArray&& input_array,
  int input_pol,
  VpiPathType op,
  PtiExprArray&& output_array,
  int output_pol,
  const PtExpr* expr,
  const PtPathDelay* path_delay
) : mFileRegion{file_region},
    mEdge{edge},
    mInputs{std::move(input_array)},
    mInputPol{input_pol},
    mOp{op},
    mOutputs{std::move(output_array)},
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
int
SptPathDecl::edge() const
{
  return mEdge;
}

// @brief 入力のリストの要素数の取得
SizeType
SptPathDecl::input_num() const
{
  return mInputs.size();
}

// @brief 入力の取得
const PtExpr*
SptPathDecl::input(
  SizeType pos
) const
{
  return mInputs[pos];
}

// 入力の極性を取り出す．
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

// @brief 出力のリストの要素数の取得
SizeType
SptPathDecl::output_num() const
{
  return mOutputs.size();
}

// @brief 出力の取得
const PtExpr*
SptPathDecl::output(
  SizeType pos
) const
{
  return mOutputs[pos];
}

// 出力の極性を取り出す．
int
SptPathDecl::output_pol() const
{
  return mOutputPol;
}

// 式を取り出す．
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
SptPathDelay::SptPathDelay(
  const FileRegion& file_region,
  const PtExpr* value
) : mFileRegion{file_region},
    mValues{value, nullptr, nullptr,
	    nullptr, nullptr, nullptr,
	    nullptr, nullptr, nullptr,
	    nullptr, nullptr, nullptr}
{
}

SptPathDelay::SptPathDelay(
  const FileRegion& file_region,
  const PtExpr* value1,
  const PtExpr* value2
) : mFileRegion{file_region},
    mValues{value1, value2, nullptr,
	    nullptr, nullptr, nullptr,
	    nullptr, nullptr, nullptr,
	    nullptr, nullptr, nullptr}
{
}

SptPathDelay::SptPathDelay(
  const FileRegion& file_region,
  const PtExpr* value1,
  const PtExpr* value2,
  const PtExpr* value3
) : mFileRegion{file_region},
    mValues{value1, value2, value3,
	    nullptr, nullptr, nullptr,
	    nullptr, nullptr, nullptr,
	    nullptr, nullptr, nullptr}
{
}

SptPathDelay::SptPathDelay(
  const FileRegion& file_region,
  const PtExpr* value1,
  const PtExpr* value2,
  const PtExpr* value3,
  const PtExpr* value4,
  const PtExpr* value5,
  const PtExpr* value6
) : mFileRegion{file_region},
    mValues{value1, value2, value3,
	    value4, value5, value6,
	    nullptr, nullptr, nullptr,
	    nullptr, nullptr, nullptr}
{
}

SptPathDelay::SptPathDelay(
  const FileRegion& file_region,
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
  const PtExpr* value12
) : mFileRegion{file_region},
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
const PtExpr*
SptPathDelay::value(
  SizeType pos
) const
{
  return mValues[pos];
}


//////////////////////////////////////////////////////////////////////
// item 関係の生成
//////////////////////////////////////////////////////////////////////

// @brief specify block item の生成
const PtItem*
SptFactory::new_SpecItem(
  const FileRegion& file_region,
  VpiSpecItemType id,
  const vector<const PtExpr*>& terminal_array
)
{
  auto node = new SptSpecItem{file_region, id,
			      PtiExprArray(mAlloc, terminal_array)};
  return node;
}

// @brief path 仕様を生成する．
const PtItem*
SptFactory::new_SpecPath(
  const FileRegion& file_region,
  VpiSpecPathType id,
  const PtExpr* expr,
  const PtPathDecl* path_decl
)
{
  auto node = new SptSpecPath{file_region, id, expr, path_decl};
  return node;
}

// @brief パス記述の生成
const PtPathDecl*
SptFactory::new_PathDecl(
  const FileRegion& file_region,
  int edge,
  const vector<const PtExpr*>& input_array,
  int input_pol,
  VpiPathType op,
  const vector<const PtExpr*>& output_array,
  int output_pol,
  const PtExpr* expr,
  const PtPathDelay* path_delay
)
{
  auto node = new SptPathDecl{file_region, edge,
			      PtiExprArray(mAlloc, input_array),
			      input_pol,
			      op,
			      PtiExprArray(mAlloc, output_array),
			      output_pol,
			      expr, path_delay};
  return node;
}

// @brief path delay value の生成 (値が1つ)
const PtPathDelay*
SptFactory::new_PathDelay(
  const FileRegion& file_region,
  const PtExpr* value
)
{
  auto node = new SptPathDelay{file_region, value};
  return node;
}

// @brief path delay value の生成 (値が2つ)
const PtPathDelay*
SptFactory::new_PathDelay(
  const FileRegion& file_region,
  const PtExpr* value1,
  const PtExpr* value2
)
{
  auto node = new SptPathDelay{file_region, value1, value2};
  return node;
}

// @brief path delay value の生成 (値が3つ)
const PtPathDelay*
SptFactory::new_PathDelay(
  const FileRegion& file_region,
  const PtExpr* value1,
  const PtExpr* value2,
  const PtExpr* value3
)
{
  auto node = new SptPathDelay{file_region,
			       value1, value2, value3};
  return node;
}

// @brief path delay value の生成 (値が6つ)
const PtPathDelay*
SptFactory::new_PathDelay(
  const FileRegion& file_region,
  const PtExpr* value1,
  const PtExpr* value2,
  const PtExpr* value3,
  const PtExpr* value4,
  const PtExpr* value5,
  const PtExpr* value6
)
{
  auto node = new SptPathDelay{file_region,
			       value1, value2, value3,
			       value4, value5, value6};
  return node;
}

// @brief path delay value の生成 (値が12個)
const PtPathDelay*
SptFactory::new_PathDelay(
  const FileRegion& file_region,
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
  const PtExpr* value12
)
{
  auto node = new SptPathDelay{file_region,
			       value1, value2, value3,
			       value4, value5, value6,
			       value7, value8, value9,
			       value10, value11, value12};
  return node;
}

END_NAMESPACE_YM_VERILOG
