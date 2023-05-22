
/// @file CptExpr.cc
/// @brief CptExpr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.

#include "CptExpr.h"
#include "alloc/Alloc.h"
#include "parser/CptFactory.h"
#include "parser/PuHierName.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// expression を表す基底クラス
//////////////////////////////////////////////////////////////////////

// @brief 演算子の種類の取得
VpiOpType
CptExpr::op_type() const
{
  return VpiOpType::Null;
}

// @brief 階層ブランチの要素数の取得
SizeType
CptExpr::namebranch_num() const
{
  return 0;
}

// @brief 階層ブランチの取得
const PtNameBranch*
CptExpr::namebranch(
  SizeType pos
) const
{
  ASSERT_NOT_REACHED;
  return nullptr;
}

// @brief 末尾の名前の取得
const char*
CptExpr::name() const
{
  return nullptr;
}

// @brief オペランドの数の取得
SizeType
CptExpr::operand_num() const
{
  return 0;
}

// @brief 0番目のオペランドの取得
const PtExpr*
CptExpr::operand0() const
{
  return nullptr;
}

// @brief 1番目のオペランドの取得
const PtExpr*
CptExpr::operand1() const
{
  return nullptr;
}

// @brief 2番目のオペランドの取得
const PtExpr*
CptExpr::operand2() const
{
  return nullptr;
}

// @brief オペランドの取得
const PtExpr*
CptExpr::operand(
  SizeType pos
) const
{
  return nullptr;
}

// @brief 定数インデックスのチェック
bool
CptExpr::is_const_index() const
{
  return false;
}

// @brief インデックスリストのサイズの取得
SizeType
CptExpr::index_num() const
{
  return 0;
}

// @brief インデックスの取得
const PtExpr*
CptExpr::index(
  SizeType pos
) const
{
  return nullptr;
}

// @brief 範囲指定モードの取得
VpiRangeMode
CptExpr::range_mode() const
{
  return VpiRangeMode::No;
}

// @brief 範囲の左側の式の取得
const PtExpr*
CptExpr::left_range() const
{
  return nullptr;
}

// @brief 範囲の右側の式の取得
const PtExpr*
CptExpr::right_range() const
{
  return nullptr;
}

// @brief 定数の種類の取得
VpiConstType
CptExpr::const_type() const
{
  return VpiConstType::Int; // ダミー
}

// @brief 整数型の定数のサイズの取得
SizeType
CptExpr::const_size() const
{
  return 0;
}

// @brief 整数型の値の取得
std::uint32_t
CptExpr::const_uint32() const
{
  return 0;
}

// @brief 整数型および文字列型の定数の文字列表現の取得
const char*
CptExpr::const_str() const
{
  return nullptr;
}

// @brief 実数型の値の取得
double
CptExpr::const_real() const
{
  return 0.0;
}

// @brief インデックスとして使える式のチェック
bool
CptExpr::is_index_expr() const
{
  return false;
}

// @brief インデックスの値の取得
int
CptExpr::index_value() const
{
  return 0;
}

// @brief simple primary のチェック
bool
CptExpr::is_simple() const
{
  return false;
}


//////////////////////////////////////////////////////////////////////
// 演算子のベース実装クラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptOpr::CptOpr(
  VpiOpType op_type
) : mOpType{op_type}
{
}

// デストラクタ
CptOpr::~CptOpr()
{
}

// クラスの型を返す．
PtExprType
CptOpr::type() const
{
  return PtExprType::Opr;
}

// 演算子のトークン番号を得る．
VpiOpType
CptOpr::op_type() const
{
  return mOpType;
}


//////////////////////////////////////////////////////////////////////
// 単項演算子を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptOpr1::CptOpr1(
  const FileRegion& file_region,
  VpiOpType op_type,
  const PtExpr* opr
) : CptOpr{op_type},
    mFileRegion{file_region},
    mOpr{opr}
{
  ASSERT_COND( opr );
}

// デストラクタ
CptOpr1::~CptOpr1()
{
}

// ファイル位置を返す．
FileRegion
CptOpr1::file_region() const
{
  return mFileRegion;
}

// 階層名の添字として使える式の時に true を返す．
bool
CptOpr1::is_index_expr() const
{
  // 算術演算はOKだけどめんどくさいので単項のマイナスのみOKとする．
  if ( op_type() == VpiOpType::Null || op_type() == VpiOpType::Minus ) {
    return operand0()->is_index_expr();
  }
  else {
    return false;
  }
}

// 階層名の添字として使える式の時にその値を返す．
int
CptOpr1::index_value() const
{
  switch ( op_type() ) {
  case VpiOpType::Null:
    return operand0()->index_value();

  case VpiOpType::Minus:
    return - operand0()->index_value();

  default:
    break;
  }
  return 0;
}

// @brief オペランドの数の取得
SizeType
CptOpr1::operand_num() const
{
  return 1;
}

// @brief 0番目のオペランドの取得
const PtExpr*
CptOpr1::operand0() const
{
  return mOpr;
}

// @brief 1番目のオペランドの取得
const PtExpr*
CptOpr1::operand1() const
{
  return nullptr;
}

// @brief 2番目のオペランドの取得
const PtExpr*
CptOpr1::operand2() const
{
  return nullptr;
}

// @brief オペランドの取得
const PtExpr*
CptOpr1::operand(
  SizeType pos
) const
{
  if ( pos == 0 ) {
    return mOpr;
  }
  else {
    return nullptr;
  }
}


//////////////////////////////////////////////////////////////////////
// 二項演算子を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptOpr2::CptOpr2(
  VpiOpType op_type,
  const PtExpr* opr1,
  const PtExpr* opr2
) : CptOpr{op_type},
    mOpr{opr1, opr2}
{
  ASSERT_COND( opr1 );
  ASSERT_COND( opr2 );
}

// デストラクタ
CptOpr2::~CptOpr2()
{
}

// ファイル位置を返す．
FileRegion
CptOpr2::file_region() const
{
  return FileRegion{mOpr[0]->file_region(), mOpr[1]->file_region()};
}

// @brief オペランドの数の取得
SizeType
CptOpr2::operand_num() const
{
  return 2;
}

// @brief 0番目のオペランドの取得
const PtExpr*
CptOpr2::operand0() const
{
  return mOpr[0];
}

// @brief 1番目のオペランドの取得
const PtExpr*
CptOpr2::operand1() const
{
  return mOpr[1];
}

// @brief 2番目のオペランドの取得
const PtExpr*
CptOpr2::operand2() const
{
  return nullptr;
}

// @brief オペランドの取得
const PtExpr*
CptOpr2::operand(
  SizeType pos
) const
{
  if ( pos < 2 ) {
    return mOpr[pos];
  }
  else {
    return nullptr;
  }
}


//////////////////////////////////////////////////////////////////////
// 三項演算子を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptOpr3::CptOpr3(
  VpiOpType op_type,
  const PtExpr* opr1,
  const PtExpr* opr2,
  const PtExpr* opr3
) : CptOpr{op_type},
    mOpr{opr1, opr2, opr3}
{
  ASSERT_COND( opr1 );
  ASSERT_COND( opr2 );
  ASSERT_COND( opr3 );
}

// デストラクタ
CptOpr3::~CptOpr3()
{
}

// ファイル位置を返す．
FileRegion
CptOpr3::file_region() const
{
  return FileRegion{mOpr[0]->file_region(), mOpr[2]->file_region()};
}

// @brief オペランドの数の取得
SizeType
CptOpr3::operand_num() const
{
  return 3;
}

// @brief 0番目のオペランドの取得
const PtExpr*
CptOpr3::operand0() const
{
  return mOpr[0];
}

// @brief 1番目のオペランドの取得
const PtExpr*
CptOpr3::operand1() const
{
  return mOpr[1];
}

// @brief 2番目のオペランドの取得
const PtExpr*
CptOpr3::operand2() const
{
  return mOpr[2];
}

// @brief オペランドの取得
const PtExpr*
CptOpr3::operand(
  SizeType pos
) const
{
  if ( pos < 3 ) {
    return mOpr[pos];
  }
  else {
    return nullptr;
  }
}


//////////////////////////////////////////////////////////////////////
// concatenation を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptConcat::CptConcat(
  const FileRegion& file_region,
  PtiExprArray&& expr_array
) : mFileRegion{file_region},
    mExprArray{move(expr_array)}
{
}

// デストラクタ
CptConcat::~CptConcat()
{
}

// ファイル位置を返す．
FileRegion
CptConcat::file_region() const
{
  return mFileRegion;
}

// クラスの型を返す．
PtExprType
CptConcat::type() const
{
  return PtExprType::Opr;
}

///演算子の種類の取得
VpiOpType
CptConcat::op_type() const
{
  return VpiOpType::Concat;
}

// @brief オペランドの数の取得
SizeType
CptConcat::operand_num() const
{
  return mExprArray.size();
}

// @brief 0番目のオペランドの取得
const PtExpr*
CptConcat::operand0() const
{
  if ( operand_num() > 0 ) {
    return mExprArray[0];
  }
  else {
    return nullptr;
  }
}

// @brief 1番目のオペランドの取得
const PtExpr*
CptConcat::operand1() const
{
  if ( operand_num() > 1 ) {
    return mExprArray[1];
  }
  else {
    return nullptr;
  }
}

// @brief 2番目のオペランドの取得
const PtExpr*
CptConcat::operand2() const
{
  if ( operand_num() > 2 ) {
    return mExprArray[2];
  }
  else {
    return nullptr;
  }
}

// @brief オペランドの取得
const PtExpr*
CptConcat::operand(
  SizeType pos
) const
{
  if ( operand_num() > pos ) {
    return mExprArray[pos];
  }
  else {
    return nullptr;
  }
}


//////////////////////////////////////////////////////////////////////
// multiple concatenation を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptMultiConcat::CptMultiConcat(
  const FileRegion& file_region,
  PtiExprArray&& expr_array
) : CptConcat{file_region, move(expr_array)}
{
}

// デストラクタ
CptMultiConcat::~CptMultiConcat()
{
}

// 演算子の種類の取得
VpiOpType
CptMultiConcat::op_type() const
{
  return VpiOpType::MultiConcat;
}


//////////////////////////////////////////////////////////////////////
// min/typ/max delay のベース実装クラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptMinTypMax::CptMinTypMax(
  const PtExpr* val0,
  const PtExpr* val1,
  const PtExpr* val2
) : mValue{val0, val1, val2}
{
  ASSERT_COND( val0 );
  ASSERT_COND( val1 );
  ASSERT_COND( val2 );
}

// デストラクタ
CptMinTypMax::~CptMinTypMax()
{
}

// ファイル位置を返す．
FileRegion
CptMinTypMax::file_region() const
{
  return FileRegion{mValue[0]->file_region(), mValue[2]->file_region()};
}

// クラスの型を返す．
PtExprType
CptMinTypMax::type() const
{
  return PtExprType::Opr;
}

// 演算子の種類の取得
VpiOpType
CptMinTypMax::op_type() const
{
  return VpiOpType::MinTypMax;
}

// 子供の数の取得
SizeType
CptMinTypMax::operand_num() const
{
  return 3;
}

// @brief 0番目のオペランドの取得
const PtExpr*
CptMinTypMax::operand0() const
{
  return mValue[0];
}

// @brief 1番目のオペランドの取得
const PtExpr*
CptMinTypMax::operand1() const
{
  return mValue[1];
}

// @brief 2番目のオペランドの取得
const PtExpr*
CptMinTypMax::operand2() const
{
  return mValue[2];
}

// 値(式)を取出す．
const PtExpr*
CptMinTypMax::operand(
  SizeType idx
) const
{
  if ( idx < 3 ) {
    return mValue[idx];
  }
  else {
    return nullptr;
  }
}


//////////////////////////////////////////////////////////////////////
// function call / system function call に共通の基底クラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptFuncCallBase::CptFuncCallBase(
  const FileRegion& file_region,
  const char* name,
  PtiExprArray&& arg_array
) : mFileRegion{file_region},
    mName{name},
    mArgArray{move(arg_array)}
{
}

// デストラクタ
CptFuncCallBase::~CptFuncCallBase()
{
}

// ファイル位置を返す．
FileRegion
CptFuncCallBase::file_region() const
{
  return mFileRegion;
}

// 末尾の名前を返す．
const char*
CptFuncCallBase::name() const
{
  return mName;
}

// @brief オペランドの数の取得
SizeType
CptFuncCallBase::operand_num() const
{
  return mArgArray.size();
}

// @brief 0番目のオペランドの取得
const PtExpr*
CptFuncCallBase::operand0() const
{
  if ( operand_num() > 0 ) {
    return mArgArray[0];
  }
  else {
    return nullptr;
  }
}

// @brief 1番目のオペランドの取得
const PtExpr*
CptFuncCallBase::operand1() const
{
  if ( operand_num() > 1 ) {
    return mArgArray[1];
  }
  else {
    return nullptr;
  }
}

// @brief 2番目のオペランドの取得
const PtExpr*
CptFuncCallBase::operand2() const
{
  if ( operand_num() > 2 ) {
    return mArgArray[2];
  }
  else {
    return nullptr;
  }
}

// @brief オペランドの取得
const PtExpr*
CptFuncCallBase::operand(
  SizeType pos
) const
{
  if ( operand_num() > pos ) {
    return mArgArray[pos];
  }
  else {
    return nullptr;
  }
}


//////////////////////////////////////////////////////////////////////
// 階層なし名前を持つ function callを表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptFuncCall::CptFuncCall(
  const FileRegion& file_region,
  const char* name,
  PtiExprArray&& arg_array
) : CptFuncCallBase{file_region, name, move(arg_array)}
{
}

// デストラクタ
CptFuncCall::~CptFuncCall()
{
}

// クラスの型を返す．
PtExprType
CptFuncCall::type() const
{
  return PtExprType::FuncCall;
}


//////////////////////////////////////////////////////////////////////
// 階層つき名前を持つ function call を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptFuncCallH::CptFuncCallH(
  const FileRegion& file_region,
  PtiNameBranchArray&& nb_array,
  const char* tail_name,
  PtiExprArray&& arg_array
) : CptFuncCall{file_region, tail_name, move(arg_array)},
    mNbArray{move(nb_array)}
{
}

// デストラクタ
CptFuncCallH::~CptFuncCallH()
{
}

// @brief 階層ブランチの要素数の取得
SizeType
CptFuncCallH::namebranch_num() const
{
  return mNbArray.size();
}

// @brief 階層ブランチの取得
const PtNameBranch*
CptFuncCallH::namebranch(
  SizeType pos
) const
{
  return mNbArray[pos];
}


//////////////////////////////////////////////////////////////////////
// system function callを表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptSysFuncCall::CptSysFuncCall(
  const FileRegion& file_region,
  const char* name,
  PtiExprArray&& arg_array
) : CptFuncCallBase{file_region, name, move(arg_array)}
{
}

// デストラクタ
CptSysFuncCall::~CptSysFuncCall()
{
}

// クラスの型を返す．
PtExprType
CptSysFuncCall::type() const
{
  return PtExprType::SysFuncCall;
}


//////////////////////////////////////////////////////////////////////
// PtConstant のベース実装クラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptConstant::CptConstant(
  const FileRegion& file_region
) : mFileRegion{file_region}
{
}

// デストラクタ
CptConstant::~CptConstant()
{
}

// ファイル位置を返す．
FileRegion
CptConstant::file_region() const
{
  return mFileRegion;
}

// クラスの型を返す．
PtExprType
CptConstant::type() const
{
  return PtExprType::Const;
}


//////////////////////////////////////////////////////////////////////
// 整数型の定数(サイズ/基数の指定なし)
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptIntConstant1::CptIntConstant1(
  const FileRegion& file_region,
  std::uint32_t value
) : CptConstant{file_region},
    mValue{value}
{
}

// デストラクタ
CptIntConstant1::~CptIntConstant1()
{
}

// 定数の種類を表す型(vpiIntConst, vpiBinaryConst など) を返す．
VpiConstType
CptIntConstant1::const_type() const
{
  return VpiConstType::Int;
}

// 階層名の添字として使える式の時に true を返す．
bool
CptIntConstant1::is_index_expr() const
{
  return true;
}

// 整数型の値の取得
std::uint32_t
CptIntConstant1::const_uint32() const
{
  return mValue;
}

// @brief インデックスの値の取得
int
CptIntConstant1::index_value() const
{
  return static_cast<int>(mValue);
}


//////////////////////////////////////////////////////////////////////
// 整数型の定数(基数のみ指定あり)
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptIntConstant2::CptIntConstant2(
  const FileRegion& file_region,
  VpiConstType const_type,
  const char* value
) : CptConstant{file_region},
    mConstType{const_type},
    mValue{value}
{
}

// デストラクタ
CptIntConstant2::~CptIntConstant2()
{
}

// 定数の種類を表す型(vpiIntConst, vpiBinaryConst など) を返す．
VpiConstType
CptIntConstant2::const_type() const
{
  return mConstType;
}

// 文字列型の値の取得
const char*
CptIntConstant2::const_str() const
{
  return mValue;
}


//////////////////////////////////////////////////////////////////////
// 整数型の定数(サイズと基数の指定あり)
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptIntConstant3::CptIntConstant3(
  const FileRegion& file_region,
  SizeType size,
  VpiConstType const_type,
  const char* value
) : CptConstant{file_region},
    mConstType{const_type},
    mSize{size},
    mValue{value}
{
}

// デストラクタ
CptIntConstant3::~CptIntConstant3()
{
}

// 定数の種類を表す型(vpiIntConst, vpiBinaryConst など) を返す．
VpiConstType
CptIntConstant3::const_type() const
{
  return mConstType;
}

// 整数型の定数のサイズの取得
SizeType
CptIntConstant3::const_size() const
{
  return mSize;
}

// 文字列型の値の取得
const char*
CptIntConstant3::const_str() const
{
  return mValue;
}


//////////////////////////////////////////////////////////////////////
// 実数型の定数(中身は PtConstant)
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptRealConstant::CptRealConstant(
  const FileRegion& file_region,
  double value
) : CptConstant{file_region},
    mValue{value}
{
}

// デストラクタ
CptRealConstant::~CptRealConstant()
{
}

// 定数の種類を表す型(vpiRealConst) を返す．
VpiConstType
CptRealConstant::const_type() const
{
  return VpiConstType::Real;
}

// 実数型の値の取得
double
CptRealConstant::const_real() const
{
  return mValue;
}


//////////////////////////////////////////////////////////////////////
// 文字列型の定数(中身は PtConstant)
//////////////////////////////////////////////////////////////////////

// 値を表す文字列を引数にとるコンストラクタ
CptStringConstant::CptStringConstant(
  const FileRegion& file_region,
  const char* value
) : CptConstant{file_region},
    mValue{value}
{
}

// デストラクタ
CptStringConstant::~CptStringConstant()
{
}

// 定数の種類を表す型(vpiStringConst) を返す．
VpiConstType
CptStringConstant::const_type() const
{
  return VpiConstType::String;
}

// 値を表示用の文字列の形で得る．
const char*
CptStringConstant::const_str() const
{
  return mValue;
}


//////////////////////////////////////////////////////////////////////
// expression 関係
//////////////////////////////////////////////////////////////////////

// 演算子を生成する．
const PtExpr*
CptFactory::new_Opr(
  const FileRegion& file_region,
  VpiOpType type,
  const PtExpr* opr
)
{
  ++ mNumOpr1;
  auto p = mAlloc.get_memory(sizeof(CptOpr1));
  auto obj = new (p) CptOpr1{file_region, type, opr};
  return obj;
}

const PtExpr*
CptFactory::new_Opr(
  const FileRegion& file_region,
  VpiOpType type,
  const PtExpr* opr1,
  const PtExpr* opr2
)
{
  // 実は file_region は不要
  ++ mNumOpr2;
  auto p = mAlloc.get_memory(sizeof(CptOpr2));
  auto obj = new (p) CptOpr2{type, opr1, opr2};
  return obj;
}

const PtExpr*
CptFactory::new_Opr(
  const FileRegion& file_region,
  VpiOpType type,
  const PtExpr* opr1,
  const PtExpr* opr2,
  const PtExpr* opr3
)
{
  // 実は file_region は不要
  ++ mNumOpr3;
  auto p = mAlloc.get_memory(sizeof(CptOpr3));
  auto obj = new (p) CptOpr3{type, opr1, opr2, opr3};
  return obj;
}

// concatination を生成する．
const PtExpr*
CptFactory::new_Concat(
  const FileRegion& file_region,
  const vector<const PtExpr*>& expr_array
)
{
  ++ mNumConcat;
  auto p = mAlloc.get_memory(sizeof(CptConcat));
  auto obj = new (p) CptConcat{file_region, PtiArray<const PtExpr>(mAlloc, expr_array)};
  return obj;
}

// multiple concatenation を生成する．
const PtExpr*
CptFactory::new_MultiConcat(
  const FileRegion& file_region,
  const vector<const PtExpr*>& expr_array
)
{
  ++ mNumMultiConcat;
  auto p = mAlloc.get_memory(sizeof(CptMultiConcat));
  auto obj = new (p) CptMultiConcat{file_region, PtiArray<const PtExpr>(mAlloc, expr_array)};
  return obj;
}

// multiple concatenation を生成する．
const PtExpr*
CptFactory::new_MinTypMax(
  const FileRegion& file_region,
  const PtExpr* val0,
  const PtExpr* val1,
  const PtExpr* val2
)
{
  // 実は file_region は不要
  ++ mNumMinTypMax3;
  auto p = mAlloc.get_memory(sizeof(CptMinTypMax));
  auto obj = new (p) CptMinTypMax{val0, val1, val2};
  return obj;
}

// function call を生成する．
const PtExpr*
CptFactory::new_FuncCall(
  const FileRegion& file_region,
  const char* name,
  const vector<const PtExpr*>& arg_array
)
{
  ++ mNumFuncCall;
  auto p = mAlloc.get_memory(sizeof(CptFuncCall));
  auto obj = new (p) CptFuncCall{file_region, name,
				 PtiArray<const PtExpr>(mAlloc, arg_array)};
  return obj;
}

// function call を生成する．
const PtExpr*
CptFactory::new_FuncCall(
  const FileRegion& file_region,
  PuHierName* hname,
  const vector<const PtExpr*>& arg_array
)
{
  ++ mNumFuncCallH;
  auto p = mAlloc.get_memory(sizeof(CptFuncCallH));
  auto nb_array = hname->name_branch_to_vector();
  auto tail_name = hname->tail_name();
  auto obj = new (p) CptFuncCallH{file_region,
				  PtiArray<const PtNameBranch>{mAlloc, nb_array},
				  tail_name,
				  PtiArray<const PtExpr>{mAlloc, arg_array}};
  return obj;
}

// system function call を生成する．
const PtExpr*
CptFactory::new_SysFuncCall(
  const FileRegion& file_region,
  const char* name,
  const vector<const PtExpr*>& arg_array
)
{
  ++ mNumSysFuncCall;
  auto p = mAlloc.get_memory(sizeof(CptSysFuncCall));
  auto obj = new (p) CptSysFuncCall{file_region, name,
				    PtiArray<const PtExpr>{mAlloc, arg_array}};
  return obj;
}

// 定数を生成する．
const PtExpr*
CptFactory::new_IntConst(
  const FileRegion& file_region,
  std::uint32_t value
)
{
  ++ mNumIntConstant1;
  auto p = mAlloc.get_memory(sizeof(CptIntConstant1));
  auto obj = new (p) CptIntConstant1{file_region, value};
  return obj;
}

// 定数を生成する．
const PtExpr*
CptFactory::new_IntConst(
  const FileRegion& file_region,
  const char* value
)
{
  ++ mNumIntConstant2;
  auto p = mAlloc.get_memory(sizeof(CptIntConstant2));
  auto obj = new (p) CptIntConstant2{file_region, VpiConstType::Int, value};
  return obj;
}

// 定数を生成する．
const PtExpr*
CptFactory::new_IntConst(
  const FileRegion& file_region,
  VpiConstType const_type,
  const char* value
)
{
  ++ mNumIntConstant2;
  auto p = mAlloc.get_memory(sizeof(CptIntConstant2));
  auto obj = new (p) CptIntConstant2{file_region, const_type, value};
  return obj;
}

// 定数を生成する．
const PtExpr*
CptFactory::new_IntConst(
  const FileRegion& file_region,
  SizeType size,
  VpiConstType const_type,
  const char* value
)
{
  ++ mNumIntConstant3;
  auto p = mAlloc.get_memory(sizeof(CptIntConstant3));
  auto obj = new (p) CptIntConstant3{file_region, size, const_type, value};
  return obj;
}

// 定数を生成する．
const PtExpr*
CptFactory::new_RealConst(
  const FileRegion& file_region,
  double value
)
{
  ++ mNumRealConstant;
  auto p = mAlloc.get_memory(sizeof(CptRealConstant));
  auto obj = new (p) CptRealConstant{file_region, value};
  return obj;
}

// 定数を生成する．
const PtExpr*
CptFactory::new_StringConst(
  const FileRegion& file_region,
  const char* value
)
{
  ++ mNumStringConstant;
  auto p = mAlloc.get_memory(sizeof(CptStringConstant));
  auto obj = new (p) CptStringConstant{file_region, value};
  return obj;
}

END_NAMESPACE_YM_VERILOG
