
/// @file SptExpr.cc
/// @brief SptExpr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.

#include "SptExpr.h"
#include "parser/SptFactory.h"
#include "parser/PuHierName.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// expression を表す基底クラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SptExpr::SptExpr(
  const FileRegion& file_region,
  PtExprType type
) : mFileRegion{file_region},
    mType{type}
{
}

// @brief デストラクタ
SptExpr::~SptExpr()
{
}

// @brief ファイル位置の取得
FileRegion
SptExpr::file_region() const
{
  return mFileRegion;
}

// @brief 式の型の取得
PtExprType
SptExpr::type() const
{
  return mType;
}

// @brief 演算子の種類の取得
VpiOpType
SptExpr::op_type() const
{
  return VpiOpType::Null;
}

// @brief 階層ブランチの要素数の取得
SizeType
SptExpr::namebranch_num() const
{
  return 0;
}

// @brief 階層ブランチの取得
const PtNameBranch*
SptExpr::namebranch(
  SizeType pos
) const
{
  ASSERT_NOT_REACHED;
  return nullptr;
}

// @brief 末尾の名前の取得
const char*
SptExpr::name() const
{
  return nullptr;
}

// @brief オペランドの数の取得
SizeType
SptExpr::operand_num() const
{
  return 0;
}

// @brief 0番目のオペランドの取得
const PtExpr*
SptExpr::operand0() const
{
  return nullptr;
}

// @brief 1番目のオペランドの取得
const PtExpr*
SptExpr::operand1() const
{
  return nullptr;
}

// @brief 2番目のオペランドの取得
const PtExpr*
SptExpr::operand2() const
{
  return nullptr;
}

// @brief オペランドの取得
const PtExpr*
SptExpr::operand(
  SizeType pos
) const
{
  return nullptr;
}

// @brief 定数インデックスのチェック
bool
SptExpr::is_const_index() const
{
  return false;
}

// @brief インデックスリストのサイズの取得
SizeType
SptExpr::index_num() const
{
  return 0;
}

// @brief インデックスの取得
const PtExpr*
SptExpr::index(
  SizeType pos
) const
{
  return nullptr;
}

// @brief 範囲指定モードの取得
VpiRangeMode
SptExpr::range_mode() const
{
  return VpiRangeMode::No;
}

// @brief 範囲の左側の式の取得
const PtExpr*
SptExpr::left_range() const
{
  return nullptr;
}

// @brief 範囲の右側の式の取得
const PtExpr*
SptExpr::right_range() const
{
  return nullptr;
}

// @brief 定数の種類の取得
VpiConstType
SptExpr::const_type() const
{
  return VpiConstType::Int; // ダミー
}

// @brief 整数型の定数のサイズの取得
SizeType
SptExpr::const_size() const
{
  return 0;
}

// @brief 整数型の値の取得
std::uint32_t
SptExpr::const_uint32() const
{
  return 0;
}

// @brief 整数型および文字列型の定数の文字列表現の取得
const char*
SptExpr::const_str() const
{
  return nullptr;
}

// @brief 実数型の値の取得
double
SptExpr::const_real() const
{
  return 0.0;
}

// @brief インデックスとして使える式のチェック
bool
SptExpr::is_index_expr() const
{
  return false;
}

// @brief インデックスの値の取得
int
SptExpr::index_value() const
{
  return static_cast<int>(const_uint32());
}

// @brief simple primary のチェック
bool
SptExpr::is_simple() const
{
  if ( type() == PtExprType::Primary &&
       index_num() == 0 &&
       left_range() == nullptr ) {
    return true;
  }
  return false;
}


//////////////////////////////////////////////////////////////////////
// 演算子のベース実装クラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SptOpr1::SptOpr1(
  const FileRegion& file_region,
  VpiOpType op_type,
  const PtExpr* opr1,
  const PtExpr* opr2,
  const PtExpr* opr3
) : SptExpr{file_region, PtExprType::Opr},
    mOpType{op_type},
    mExprList{opr1, opr2, opr3}
{
  if ( opr2 ) {
    if ( opr3 ) {
      mSize = 3;
    }
    else {
      mSize = 2;
    }
  }
  else {
    mSize = 1;
  }
}

// @brief デストラクタ
SptOpr1::~SptOpr1()
{
}

// @brief 演算子の種類の取得
VpiOpType
SptOpr1::op_type() const
{
  return mOpType;
}

// @brief インデックスとして使える式のチェック
bool
SptOpr1::is_index_expr() const
{
  // 算術演算なら本当はOKだけどめんどくさいので単項のマイナスのみOKとする．
  if ( mOpType == VpiOpType::Minus || mOpType == VpiOpType::Null ) {
    return operand(0)->is_index_expr();
  }
  else {
    return false;
  }
}

// @brief インデックスの値の取得
int
SptOpr1::index_value() const
{
  switch ( mOpType ) {
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
SptOpr1::operand_num() const
{
  return mSize;
}

// @brief 0番目のオペランドの取得
const PtExpr*
SptOpr1::operand0() const
{
  return mExprList[0];
}

// @brief 1番目のオペランドの取得
const PtExpr*
SptOpr1::operand1() const
{
  return mExprList[1];
}

// @brief 2番目のオペランドの取得
const PtExpr*
SptOpr1::operand2() const
{
  return mExprList[2];
}

// @brief オペランドを取り出す
// @param[in] pos 取り出すオペランドの位置（最初の位置は 0）
// @return pos 番目のオペランド
const PtExpr*
SptOpr1::operand(SizeType pos) const
{
  return mExprList[pos];
}


//////////////////////////////////////////////////////////////////////
// 演算子のベース実装クラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SptOpr2::SptOpr2(
  const FileRegion& file_region,
  VpiOpType op_type,
  PtiExprArray&& opr_array
) : SptExpr{file_region, PtExprType::Opr},
    mOpType{op_type},
    mExprArray{std::move(opr_array)}
{
}

// @brief デストラクタ
SptOpr2::~SptOpr2()
{
}

// @brief 演算子の種類の取得
// @return 演算子の種類
VpiOpType
SptOpr2::op_type() const
{
  return mOpType;
}

// @brief インデックスとして使える式のチェック
bool
SptOpr2::is_index_expr() const
{
  return false;
}

// @brief インデックスの値の取得
int
SptOpr2::index_value() const
{
  return 0;
}

// @brief オペランドの数の取得
SizeType
SptOpr2::operand_num() const
{
  return mExprArray.size();
}

// @brief 0番目のオペランドの取得
const PtExpr*
SptOpr2::operand0() const
{
  return mExprArray[0];
}

// @brief 1番目のオペランドの取得
const PtExpr*
SptOpr2::operand1() const
{
  return mExprArray[1];
}

// @brief 2番目のオペランドの取得
const PtExpr*
SptOpr2::operand2() const
{
  return mExprArray[2];
}

// @brief オペランドの取得
const PtExpr*
SptOpr2::operand(
  SizeType pos
) const
{
  return mExprArray[pos];
}


//////////////////////////////////////////////////////////////////////
// function call / system function call に共通の基底クラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SptFuncCall::SptFuncCall(
  const FileRegion& file_region,
  PtExprType type,
  PtiNameBranchArray&& nb_array,
  const char* name,
  PtiExprArray&& arg_array
) : SptExpr{file_region, type},
    mNbArray{std::move(nb_array)},
    mName{name},
    mArgArray{std::move(arg_array)}
{
}

// @brief デストラクタ
SptFuncCall::~SptFuncCall()
{
}

// @brief 階層ブランチの要素数の取得
SizeType
SptFuncCall::namebranch_num() const
{
  return mNbArray.size();
}

// @brief 階層ブランチの取得
const PtNameBranch*
SptFuncCall::namebranch(
  SizeType pos
) const
{
  return mNbArray[pos];
}

// @brief 末尾の名前の取得
const char*
SptFuncCall::name() const
{
  return mName;
}

// @brief 引数の数の取得
SizeType
SptFuncCall::operand_num() const
{
  return mArgArray.size();
}

// @brief 0番目のオペランドの取得
const PtExpr*
SptFuncCall::operand0() const
{
  return operand(0);
}

// @brief 1番目のオペランドの取得
const PtExpr*
SptFuncCall::operand1() const
{
  return operand(1);
}

// @brief 2番目のオペランドの取得
const PtExpr*
SptFuncCall::operand2() const
{
  return operand(2);
}

// @brief オペランドの取得
const PtExpr*
SptFuncCall::operand(
  SizeType pos
) const
{
  return mArgArray[pos];
}


//////////////////////////////////////////////////////////////////////
// PtPrimary の実装クラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SptPrimary::SptPrimary(
  const FileRegion& file_region,
  PtiNameBranchArray&& nb_array,
  const char* tail_name,
  bool const_index,
  PtiExprArray&& index_array,
  VpiRangeMode mode,
  const PtExpr* left,
  const PtExpr* right
) : SptExpr{file_region, PtExprType::Primary},
    mNbArray{std::move(nb_array)},
    mName{tail_name},
    mConstIndex{const_index},
    mIndexArray{std::move(index_array)},
    mMode{mode},
    mLeftRange{left},
    mRightRange{right}
{
}

// @brief デストラクタ
SptPrimary::~SptPrimary()
{
}

// @brief 階層ブランチの要素数の取得
SizeType
SptPrimary::namebranch_num() const
{
  return mNbArray.size();
}

// @brief 階層ブランチの取得
const PtNameBranch*
SptPrimary::namebranch(
  SizeType pos
) const
{
  return mNbArray[pos];
}

// @brief 末尾の名前の取得
const char*
SptPrimary::name() const
{
  return mName;
}

// @brief 定数インデックスのチェック
bool
SptPrimary::is_const_index() const
{
  return mConstIndex;
}

// @brief インデックスリストのサイズの取得
SizeType
SptPrimary::index_num() const
{
  return mIndexArray.size();
}

// @brief インデックスの取得
const PtExpr*
SptPrimary::index(
  SizeType pos
) const
{
  return mIndexArray[pos];
}

// @brief 範囲指定モードの取得
VpiRangeMode
SptPrimary::range_mode() const
{
  return mMode;
}

// @brief 範囲の左側の式の取得
const PtExpr*
SptPrimary::left_range() const
{
  return mLeftRange;
}

// @brief 範囲の右側の式の取得
const PtExpr*
SptPrimary::right_range() const
{
  return mRightRange;
}


//////////////////////////////////////////////////////////////////////
// 定数
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SptConstant::SptConstant(
  const FileRegion& file_region,
  VpiConstType const_type,
  SizeType size,
  std::uint32_t ivalue,
  const char* svalue,
  double rvalue
) : SptExpr{file_region, PtExprType::Const},
    mConstType{const_type},
    mSize{size},
    mIntValue{ivalue},
    mStrValue{svalue},
    mRealValue{rvalue}
{
}

// @brief デストラクタ
SptConstant::~SptConstant()
{
}

// @brief 整数型の定数のサイズの取得
SizeType
SptConstant::const_size() const
{
  return mSize;
}

// @brief 整数型の値の取得
std::uint32_t
SptConstant::const_uint32() const
{
  return mIntValue;
}

// @brief 整数型および文字列型の定数の文字列表現の取得
const char*
SptConstant::const_str() const
{
  return mStrValue;
}

// @brief 実数型の値の取得
double
SptConstant::const_real() const
{
  return mRealValue;
}

// @brief インデックスとして使える式のチェック
bool
SptConstant::is_index_expr() const
{
  // ただの整数の場合のみが使える．
  return const_type() == VpiConstType::Int && const_str() == nullptr;
}

// @brief 定数の種類の取得
VpiConstType
SptConstant::const_type() const
{
  return mConstType;
}


//////////////////////////////////////////////////////////////////////
// expression 関係
//////////////////////////////////////////////////////////////////////

// @brief 単項演算子の生成
const PtExpr*
SptFactory::new_Opr(
  const FileRegion& file_region,
  VpiOpType type,
  const PtExpr* opr
)
{
  auto node = new SptOpr1{file_region, type,
			  opr, nullptr, nullptr};
  return node;
}

// @brief 二項演算子の生成
const PtExpr*
SptFactory::new_Opr(
  const FileRegion& file_region,
  VpiOpType type,
  const PtExpr* opr1,
  const PtExpr* opr2
)
{
  auto node = new SptOpr1{file_region, type,
			  opr1, opr2, nullptr};
  return node;
}

// @brief 三項演算子の生成
const PtExpr*
SptFactory::new_Opr(
  const FileRegion& file_region,
  VpiOpType type,
  const PtExpr* opr1,
  const PtExpr* opr2,
  const PtExpr* opr3
)
{
  auto node = new SptOpr1{file_region, type,
			  opr1, opr2, opr3};
  return node;
}

// @brief concatination 演算子の生成
const PtExpr*
SptFactory::new_Concat(
  const FileRegion& file_region,
  const vector<const PtExpr*>& expr_array
)
{
  auto node = new SptOpr2{file_region, VpiOpType::Concat,
			  PtiArray<const PtExpr>{mAlloc, expr_array}};
  return node;
}

// @brief multi-concatination 演算子の生成
const PtExpr*
SptFactory::new_MultiConcat(
  const FileRegion& file_region,
  const vector<const PtExpr*>& expr_array
)
{
  auto node = new SptOpr2{file_region, VpiOpType::MultiConcat,
			  PtiArray<const PtExpr>{mAlloc, expr_array}};
  return node;
}

// @brief min/typ/max delay 演算子の生成
const PtExpr*
SptFactory::new_MinTypMax(
  const FileRegion& file_region,
  const PtExpr* val0,
  const PtExpr* val1,
  const PtExpr* val2
)
{
  auto node = new SptOpr1{file_region, VpiOpType::MinTypMax,
			  val0, val1, val2};
  return node;
}

// @brief primary の生成
const PtExpr*
SptFactory::new_Primary(
  const FileRegion& file_region,
  const char* name
)
{
  auto node = new SptPrimary{file_region,
                             PtiNameBranchArray{},
			     name,
			     false};
  return node;
}

// @brief インデックス付き primary の生成
const PtExpr*
SptFactory::new_Primary(
  const FileRegion& file_region,
  const char* name,
  const vector<const PtExpr*>& index_array
)
{
  auto node = new SptPrimary{file_region,
			     PtiNameBranchArray(),
			     name,
			     false,
			     PtiArray<const PtExpr>{mAlloc, index_array}};
  return node;
}

// @brief 範囲指定付き primary の生成
const PtExpr*
SptFactory::new_Primary(
  const FileRegion& file_region,
  const char* name,
  VpiRangeMode mode,
  const PtExpr* left,
  const PtExpr* right
)
{
  auto node = new SptPrimary{file_region,
			     PtiNameBranchArray(),
			     name,
			     false,
			     PtiExprArray(),
			     mode, left, right};
  return node;
}

// @brief インデックスと範囲指定付き primary の生成
const PtExpr*
SptFactory::new_Primary(
  const FileRegion& file_region,
  const char* name,
  const vector<const PtExpr*>& index_array,
  VpiRangeMode mode,
  const PtExpr* left,
  const PtExpr* right
)
{
  auto node = new SptPrimary{file_region,
			     PtiNameBranchArray(),
			     name,
			     false,
			     PtiExprArray(mAlloc, index_array),
			     mode, left, right};
  return node;
}

// @brief primary の生成 (階層付き)
const PtExpr*
SptFactory::new_Primary(
  const FileRegion& file_region,
  PuHierName* hname
)
{
  auto nb_array = hname->name_branch_to_vector();
  auto tail_name = hname->tail_name();
  auto node = new SptPrimary{file_region,
			     PtiNameBranchArray{mAlloc, nb_array},
			     tail_name,
			     false};
  return node;
}

// @brief インデックス付き primary の生成 (階層付き)
const PtExpr*
SptFactory::new_Primary(
  const FileRegion& file_region,
  PuHierName* hname,
  const vector<const PtExpr*>& index_array
)
{
  auto nb_array = hname->name_branch_to_vector();
  auto tail_name = hname->tail_name();
  auto node = new SptPrimary{file_region,
			     PtiNameBranchArray{mAlloc, nb_array},
			     tail_name,
			     false,
			     PtiExprArray{mAlloc, index_array}};
  return node;
}

// @brief 範囲指定付き primary の生成 (階層付き)
const PtExpr*
SptFactory::new_Primary(
  const FileRegion& file_region,
  PuHierName* hname,
  VpiRangeMode mode,
  const PtExpr* left,
  const PtExpr* right
)
{
  auto nb_array = hname->name_branch_to_vector();
  auto tail_name = hname->tail_name();
  auto node = new SptPrimary{file_region,
			     PtiNameBranchArray{mAlloc, nb_array},
			     tail_name,
			     false,
			     PtiExprArray(),
			     mode, left, right};
  return node;
}

// @brief インデックスと範囲指定付き primary の生成 (階層付き)
const PtExpr*
SptFactory::new_Primary(
  const FileRegion& file_region,
  PuHierName* hname,
  const vector<const PtExpr*>& index_array,
  VpiRangeMode mode,
  const PtExpr* left,
  const PtExpr* right
)
{
  auto nb_array = hname->name_branch_to_vector();
  auto tail_name = hname->tail_name();
  auto node = new SptPrimary{file_region,
			     PtiNameBranchArray{mAlloc, nb_array},
			     tail_name,
			     false,
			     PtiExprArray{mAlloc, index_array},
			     mode, left, right};
  return node;
}

// @brief constant primary の生成
const PtExpr*
SptFactory::new_CPrimary(
  const FileRegion& file_region,
  const char* name,
  const vector<const PtExpr*>& index_array
)
{
  auto node = new SptPrimary{file_region,
			     PtiNameBranchArray(),
			     name,
			     true,
			     PtiExprArray{mAlloc, index_array}};
  return node;
}

// @brief 範囲指定付き constant primary の生成
const PtExpr*
SptFactory::new_CPrimary(
  const FileRegion& file_region,
  const char* name,
  VpiRangeMode mode,
  const PtExpr* left,
  const PtExpr* right
)
{
  auto node = new SptPrimary{file_region,
			     PtiNameBranchArray(),
			     name,
			     true,
			     PtiExprArray(),
			     mode, left, right};
  return node;
}

// @brief インデックス付き constant primary の生成 (階層付き)
const PtExpr*
SptFactory::new_CPrimary(
  const FileRegion& file_region,
  PuHierName* hname,
  const vector<const PtExpr*>& index_array
)
{
  auto nb_array = hname->name_branch_to_vector();
  auto tail_name = hname->tail_name();
  auto node = new SptPrimary{file_region,
			     PtiNameBranchArray{mAlloc, nb_array},
			     tail_name,
			     true,
			     PtiExprArray{mAlloc, index_array}};
  return node;
}

// @brief function call の生成
const PtExpr*
SptFactory::new_FuncCall(
  const FileRegion& file_region,
  const char* name,
  const vector<const PtExpr*>& arg_array
)
{
  auto node = new SptFuncCall{file_region, PtExprType::FuncCall,
			      PtiNameBranchArray(),
			      name,
			      PtiExprArray{mAlloc, arg_array}};
  return node;
}

// @brief function call の生成 (階層付き)
const PtExpr*
SptFactory::new_FuncCall(
  const FileRegion& file_region,
  PuHierName* hname,
  const vector<const PtExpr*>& arg_array
)
{
  auto nb_array = hname->name_branch_to_vector();
  auto tail_name = hname->tail_name();
  auto node = new SptFuncCall{file_region, PtExprType::FuncCall,
			      PtiNameBranchArray{mAlloc, nb_array},
			      tail_name,
			      PtiExprArray{mAlloc, arg_array}};
  return node;
}

// @brief system function call の生成
const PtExpr*
SptFactory::new_SysFuncCall(
  const FileRegion& file_region,
  const char* name,
  const vector<const PtExpr*>& arg_array
)
{
  auto node = new SptFuncCall{file_region, PtExprType::SysFuncCall,
			      PtiNameBranchArray(),
			      name,
			      PtiExprArray{mAlloc, arg_array}};
  return node;
}

// @brief 整数型の定数の生成
const PtExpr*
SptFactory::new_IntConst(
  const FileRegion& file_region,
  std::uint32_t value
)
{
  auto node = new SptConstant{file_region, VpiConstType::Int,
			      0, value, nullptr, 0.0};
  return node;
}

// @brief 整数型の定数の生成
const PtExpr*
SptFactory::new_IntConst(
  const FileRegion& file_region,
  const char* value
)
{
  auto node = new SptConstant{file_region, VpiConstType::Int,
			      0, 0, value, 0.0};
  return node;
}

// @brief 基底付き整数型の定数の生成
const PtExpr*
SptFactory::new_IntConst(
  const FileRegion& file_region,
  VpiConstType const_type,
  const char* value
)
{
  auto node = new SptConstant{file_region, const_type,
			      0, 0, value, 0.0};
  return node;
}

// @brief サイズと基底付き定数の生成
const PtExpr*
SptFactory::new_IntConst(
  const FileRegion& file_region,
  SizeType size,
  VpiConstType const_type,
  const char* value
)
{
  auto node = new SptConstant{file_region, const_type,
			      size, 0, value, 0.0};
  return node;
}

// @brief 実数型の定数の生成
const PtExpr*
SptFactory::new_RealConst(
  const FileRegion& file_region,
  double value
)
{
  auto node = new SptConstant{file_region, VpiConstType::Real,
			      0, 0, nullptr, value};
  return node;
}

// @brief 文字列型の定数の生成
const PtExpr*
SptFactory::new_StringConst(
  const FileRegion& file_region,
  const char* value
)
{
  auto node = new SptConstant{file_region, VpiConstType::String,
			      0, 0, value, 0.0};
  return node;
}

END_NAMESPACE_YM_VERILOG
