﻿
/// @file CptSpecItem.cc
/// @brief CptSpecItem の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.

#include "CptSpecItem.h"
#include "parser/CptFactory.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// specify_block の item を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptSpecItem::CptSpecItem(
  const FileRegion& file_region,
  VpiSpecItemType id,
  PtiExprArray&& terminal_array
) : mFileRegion{file_region},
    mId{id},
    mTerminalArray{std::move(terminal_array)}
{
}

// デストラクタ
CptSpecItem::~CptSpecItem()
{
}

// ファイル位置を返す．
FileRegion
CptSpecItem::file_region() const
{
  return mFileRegion;
}

// 型を返す．
PtItemType
CptSpecItem::type() const
{
  return PtItemType::SpecItem;
}

// トークン番号を返す．??? たぶん変更される．
VpiSpecItemType
CptSpecItem::specitem_type() const
{
  return mId;
}

// @brief ターミナルの要素数の取得
SizeType
CptSpecItem::terminal_num() const
{
  return mTerminalArray.size();
}

// @brief ターミナルの取得
const PtExpr*
CptSpecItem::terminal(
  SizeType pos
) const
{
  return mTerminalArray[pos];
}


//////////////////////////////////////////////////////////////////////
// specify_block の path_declaration を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptSpecPath::CptSpecPath(
  const FileRegion& file_region,
  VpiSpecPathType id,
  const PtExpr* expr,
  const PtPathDecl* path_decl
) : mFileRegion{file_region},
    mId{id},
    mExpr{expr},
    mPathDecl{path_decl}
{
}

// デストラクタ
CptSpecPath::~CptSpecPath()
{
}

// ファイル位置を返す．
FileRegion
CptSpecPath::file_region() const
{
  return mFileRegion;
}

// 型を返す．
PtItemType
CptSpecPath::type() const
{
  return PtItemType::SpecPath;
}

// トークン番号を返す．??? たぶん変更される．
VpiSpecPathType
CptSpecPath::specpath_type() const
{
  return mId;
}

// モジュールパスの式を返す．
const PtExpr*
CptSpecPath::expr() const
{
  return mExpr;
}

// パス記述を返す．
const PtPathDecl*
CptSpecPath::path_decl() const
{
  return mPathDecl;
}


//////////////////////////////////////////////////////////////////////
// path_decl を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptPathDecl::CptPathDecl(
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
    mInputArray{std::move(input_array)},
    mInputPol{input_pol},
    mOp{op},
    mOutputArray{std::move(output_array)},
    mOutputPol{output_pol},
    mExpr{expr},
    mPathDelay{path_delay}
{
}

// デストラクタ
CptPathDecl::~CptPathDecl()
{
}

// ファイル位置を返す．
FileRegion
CptPathDecl::file_region() const
{
  return mFileRegion;
}

// edge_descriptor を取り出す．
int
CptPathDecl::edge() const
{
  return mEdge;
}

// @brief 入力のリストの要素数の取得
SizeType
CptPathDecl::input_num() const
{
  return mInputArray.size();
}

// @brief 入力の取得
const PtExpr*
CptPathDecl::input(
  SizeType pos
) const
{
  return mInputArray[pos];
}

// 入力の極性を取り出す．
int
CptPathDecl::input_pol() const
{
  return mInputPol;
}

// パス記述子(?)を得る．vpiParallel か vpiFull
VpiPathType
CptPathDecl::op() const
{
  return mOp;
}

// @brief 出力のリストの要素数の取得
SizeType
CptPathDecl::output_num() const
{
  return mOutputArray.size();
}

// @brief 出力の取得
const PtExpr*
CptPathDecl::output(
  SizeType pos
) const
{
  return mOutputArray[pos];
}

// 出力の極性を取り出す．
int
CptPathDecl::output_pol() const
{
  return mOutputPol;
}

// 式を取り出す．
const PtExpr*
CptPathDecl::expr() const
{
  return mExpr;
}

// path_delay_value を取り出す．
const PtPathDelay*
CptPathDecl::path_delay() const
{
  return mPathDelay;
}


//////////////////////////////////////////////////////////////////////
// path_delay_value を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptPathDelay::CptPathDelay(
  const FileRegion& file_region,
  const PtExpr* value
) : mFileRegion{file_region},
    mValues{value,   nullptr, nullptr,
	    nullptr, nullptr, nullptr,
	    nullptr, nullptr, nullptr,
	    nullptr, nullptr, nullptr}
{
}

CptPathDelay::CptPathDelay(
  const FileRegion& file_region,
  const PtExpr* value1,
  const PtExpr* value2
) : mFileRegion{file_region},
    mValues{value1,  value2,  nullptr,
	    nullptr, nullptr, nullptr,
	    nullptr, nullptr, nullptr,
	    nullptr, nullptr, nullptr}

{
}
CptPathDelay::CptPathDelay(
  const FileRegion& file_region,
  const PtExpr* value1,
  const PtExpr* value2,
  const PtExpr* value3
) : mFileRegion{file_region},
    mValues{value1,  value2,  value3,
	    nullptr, nullptr, nullptr,
	    nullptr, nullptr, nullptr,
	    nullptr, nullptr, nullptr}
{
}

CptPathDelay::CptPathDelay(
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

CptPathDelay::CptPathDelay(
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
    mValues{value1,  value2,  value3,
	    value4,  value5,  value6,
	    value7,  value8,  value9,
	    value10, value11, value12}
{
}

// デストラクタ
CptPathDelay::~CptPathDelay()
{
}

// ファイル位置を返す．
FileRegion
CptPathDelay::file_region() const
{
  return mFileRegion;
}

// 値を取り出す．
const PtExpr*
CptPathDelay::value(
  SizeType pos
) const
{
  if ( 0 <= pos && pos < 12 ) {
    return mValues[pos];
  }
  else {
    return nullptr;
  }
}


//////////////////////////////////////////////////////////////////////
// spec item 関係
//////////////////////////////////////////////////////////////////////

// specify block item を生成する．
const PtItem*
CptFactory::new_SpecItem(
  const FileRegion& file_region,
  VpiSpecItemType id,
  const vector<const PtExpr*>& terminal_array
)
{
  ++ mNumSpecItem;
  void* p = mAlloc.get_memory(sizeof(CptSpecItem));
  auto obj = new (p) CptSpecItem{file_region, id,
				 PtiArray<const PtExpr>{mAlloc, terminal_array}};
  return obj;
}

// path 仕様を生成する．
const PtItem*
CptFactory::new_SpecPath(
  const FileRegion& file_region,
  VpiSpecPathType id,
  const PtExpr* expr,
  const PtPathDecl* path_decl
)
{
  ++ mNumSpecPath;
  void* p = mAlloc.get_memory(sizeof(CptSpecPath));
  auto obj = new (p) CptSpecPath{file_region, id, expr, path_decl};
  return obj;
}

// path 記述を生成する．
const PtPathDecl*
CptFactory::new_PathDecl(
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
  ++ mNumPathDecl;
  void* p = mAlloc.get_memory(sizeof(CptPathDecl));
  auto obj = new (p) CptPathDecl{file_region, edge,
				 PtiArray<const PtExpr>{mAlloc, input_array},
				 input_pol,
				 op,
				 PtiArray<const PtExpr>{mAlloc, output_array},
				 output_pol,
				 expr, path_delay};
  return obj;
}

// path delay value を生成する．
const PtPathDelay*
CptFactory::new_PathDelay(
  const FileRegion& file_region,
  const PtExpr* value
)
{
  ++ mNumPathDelay;
  void* p = mAlloc.get_memory(sizeof(CptPathDelay));
  auto obj = new (p) CptPathDelay{file_region, value};
  return obj;
}

// path delay value を生成する．
const PtPathDelay*
CptFactory::new_PathDelay(
  const FileRegion& file_region,
  const PtExpr* value1,
  const PtExpr* value2
)
{
  ++ mNumPathDelay;
  void* p = mAlloc.get_memory(sizeof(CptPathDelay));
  auto obj = new (p) CptPathDelay{file_region, value1, value2};
  return obj;
}

// path delay value を生成する．
const PtPathDelay*
CptFactory::new_PathDelay(
  const FileRegion& file_region,
  const PtExpr* value1,
  const PtExpr* value2,
  const PtExpr* value3
)
{
  ++ mNumPathDelay;
  void* p = mAlloc.get_memory(sizeof(CptPathDelay));
  auto obj = new (p) CptPathDelay{file_region, value1, value2, value3};
  return obj;
}

// path delay value を生成する．
const PtPathDelay*
CptFactory::new_PathDelay(
  const FileRegion& file_region,
  const PtExpr* value1,
  const PtExpr* value2,
  const PtExpr* value3,
  const PtExpr* value4,
  const PtExpr* value5,
  const PtExpr* value6
)
{
  ++ mNumPathDelay;
  void* p = mAlloc.get_memory(sizeof(CptPathDelay));
  auto obj = new (p) CptPathDelay{file_region,
				  value1, value2, value3,
				  value4, value5, value6};
  return obj;
}

// path delay value を生成する．
const PtPathDelay*
CptFactory::new_PathDelay(
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
  ++ mNumPathDelay;
  void* p = mAlloc.get_memory(sizeof(CptPathDelay));
  auto obj = new (p) CptPathDelay{file_region,
				  value1, value2, value3,
				  value4, value5, value6,
				  value7, value8, value9,
				  value10, value11, value12};
  return obj;
}

END_NAMESPACE_YM_VERILOG
