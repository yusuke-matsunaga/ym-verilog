
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
// @param[in] file_region ファイル位置の情報
// @param[in] type 式の種類を表す型
SptExpr::SptExpr(const FileRegion& file_region,
		 PtExprType type) :
  mFileRegion{file_region},
  mType{type}
{
}

// @brief デストラクタ
SptExpr::~SptExpr()
{
}

// @brief ファイル位置の取得
// @return ファイル位置
FileRegion
SptExpr::file_region() const
{
  return mFileRegion;
}

// @brief 式の型の取得
// @return 式の型
PtExprType
SptExpr::type() const
{
  return mType;
}

// @brief 演算子の種類の取得
// @return 演算子の種類
// このクラスでは vpiNullOp を返す．
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
// @param[in] pos 位置 ( 0 <= pos < namebranch_num() )
const PtNameBranch*
SptExpr::namebranch(SizeType pos) const
{
  ASSERT_NOT_REACHED;
  return nullptr;
}

// @brief 末尾の名前の取得
// @return 末尾の名前
// このクラスでは nullptr を返す．
const char*
SptExpr::name() const
{
  return nullptr;
}

// @brief オペランドの数の取得
// @return 子供の数
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
// @param[in] pos 取り出すオペランンドの位置(最初の位置は 0)
// @return pos 番目のオペランド
const PtExpr*
SptExpr::operand(SizeType pos) const
{
  return nullptr;
}

// @brief 定数インデックスのチェック
// @retval true インデックスもしくは範囲が定数にならなければならないとき
// @retval false 上記以外
// このクラスでは false を返す．
bool
SptExpr::is_const_index() const
{
  return false;
}

// @brief インデックスリストのサイズの取得
// @return インデックスリストのサイズ
SizeType
SptExpr::index_num() const
{
  return 0;
}

// @brief インデックスの取得
// @param[in] pos 位置番号 ( 0 <= pos < index_num() )
const PtExpr*
SptExpr::index(SizeType pos) const
{
  return nullptr;
}

// @brief 範囲指定モードの取得
// @return 範囲指定モード
// このクラスでは kVpiNoRange を返す．
VpiRangeMode
SptExpr::range_mode() const
{
  return VpiRangeMode::No;
}

// @brief 範囲の左側の式の取得
// @return 範囲の左側の式
// このクラスでは nullptr を返す．
const PtExpr*
SptExpr::left_range() const
{
  return nullptr;
}

// @brief 範囲の右側の式の取得
// @return 範囲の右側の式
// このクラスでは nullptr を返す．
const PtExpr*
SptExpr::right_range() const
{
  return nullptr;
}

// @brief 定数の種類の取得
// @return 定数の種類
// このクラスでは VpiConstType::Int を返す．
VpiConstType
SptExpr::const_type() const
{
  return VpiConstType::Int; // ダミー
}

// @brief 整数型の定数のサイズの取得
// @return サイズ\n
// サイズ指定の無い場合と整数型の定数でない場合には 0 を返す．
// このクラスでは 0 を返す．
SizeType
SptExpr::const_size() const
{
  return 0;
}

// @brief 整数型の値の取得
// @return 値
// このクラスでは 0 を返す．
unsigned int
SptExpr::const_uint() const
{
  return 0;
}

// @brief 整数型および文字列型の定数の文字列表現の取得
// @return 値の文字列表現\n
// 整数型のサイズと基数は含まない．
// このクラスでは nullptr を返す．
const char*
SptExpr::const_str() const
{
  return nullptr;
}

// @brief 実数型の値の取得
// @return 値
// このクラスでは 0.0 を返す．
double
SptExpr::const_real() const
{
  return 0.0;
}

// @brief インデックスとして使える式のチェック
// @retval true 階層名の添字として使える式
// @retval false 使えない式
// このクラスでは false を返す．
bool
SptExpr::is_index_expr() const
{
  return false;
}

// @brief インデックスの値の取得
// @return 階層名の添字として使える式の時にその値を返す．
// このクラスでは const_uint() をキャストして返す．
int
SptExpr::index_value() const
{
  return static_cast<int>(const_uint());
}

// @brief simple primary のチェック
// @retval true index_list も range も持たないとき
// @retval false 上記以外
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
// @param[in] file_reigon ファイル位置の情報
// @param[in] ai_list 属性インスタンスのリスト
// @param[in] op_type 演算子の種類
// @param[in] opr_list　オペランドのリスト
SptOpr1::SptOpr1(const FileRegion& file_region,
		 VpiOpType op_type,
		 const PtExpr* opr1,
		 const PtExpr* opr2,
		 const PtExpr* opr3) :
  SptExpr{file_region, PtExprType::Opr},
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
// @return 演算子の種類
VpiOpType
SptOpr1::op_type() const
{
  return mOpType;
}

// @brief インデックスとして使える式のチェック
// @retval true 階層名の添字として使える式
// @retval false 使えない式
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
// @return 階層名の添字として使える式の時にその値を返す．
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
// @return オペランドの数
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
// @param[in] file_reigon ファイル位置の情報
// @param[in] op_type 演算子の種類
// @param[in] opr_array オペランドのリスト
SptOpr2::SptOpr2(const FileRegion& file_region,
		 VpiOpType op_type,
		 PtiExprArray&& opr_array) :
  SptExpr{file_region, PtExprType::Opr},
  mOpType{op_type},
  mExprArray{move(opr_array)}
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
// @retval true 階層名の添字として使える式
// @retval false 使えない式
bool
SptOpr2::is_index_expr() const
{
  return false;
}

// @brief インデックスの値の取得
// @return 階層名の添字として使える式の時にその値を返す．
int
SptOpr2::index_value() const
{
  return 0;
}

// @brief オペランドの数の取得
// @return オペランドの数
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
// @param[in] pos 取り出すオペランンドの位置(最初の位置は 0)
// @return pos 番目のオペランド
const PtExpr*
SptOpr2::operand(SizeType pos) const
{
  return mExprArray[pos];
}


//////////////////////////////////////////////////////////////////////
// function call / system function call に共通の基底クラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] file_region ファイル位置の情報
// @param[in] ai_list 属性インスタンスのリスト
// @param[in] type 関数の型(kFuncCall or kSysFuncCall)
// @param[in] nblist 階層名のリスト
// @param[in] name 名前
// @param[in] arg_list 引数のリスト
SptFuncCall::SptFuncCall(const FileRegion& file_region,
			 PtExprType type,
			 PtiNameBranchArray&& nb_array,
			 const char* name,
			 PtiExprArray&& arg_array) :
  SptExpr{file_region, type},
  mNbArray{move(nb_array)},
  mName{name},
  mArgArray{move(arg_array)}
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
// @param[in] pos 位置 ( 0 <= pos < namebranch_num() )
const PtNameBranch*
SptFuncCall::namebranch(SizeType pos) const
{
  return mNbArray[pos];
}

// @brief 末尾の名前の取得
// @return 末尾の名前
const char*
SptFuncCall::name() const
{
  return mName;
}

// @brief 引数の数の取得
// @return 引数の数
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
// @param[in] pos 取り出すオペランンドの位置(最初の位置は 0)
// @return pos 番目のオペランド
const PtExpr*
SptFuncCall::operand(SizeType pos) const
{
  return mArgArray[pos];
}


//////////////////////////////////////////////////////////////////////
// PtPrimary の実装クラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] file_region ファイル位置の情報
// @param[in] nblist 階層名のリスト
// @param[in] tail_name 末尾のリスト
// @param[in] const_index インデックスが定数でなければいけないとき true となるフラグ
// @param[in] index_array インデックスのリスト
// @param[in] mode 範囲指定のモード
// @param[in] left 範囲の左側の式
// @param[in] right 範囲の右側の式
SptPrimary::SptPrimary(const FileRegion& file_region,
		       PtiNameBranchArray&& nb_array,
		       const char* tail_name,
		       bool const_index,
		       PtiExprArray&& index_array,
		       VpiRangeMode mode,
		       const PtExpr* left,
		       const PtExpr* right) :
  SptExpr{file_region, PtExprType::Primary},
  mNbArray{move(nb_array)},
  mName{tail_name},
  mConstIndex{const_index},
  mIndexArray{move(index_array)},
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
// @param[in] pos 位置 ( 0 <= pos < namebranch_num() )
const PtNameBranch*
SptPrimary::namebranch(SizeType pos) const
{
  return mNbArray[pos];
}

// @brief 末尾の名前の取得
// @return 末尾の名前
const char*
SptPrimary::name() const
{
  return mName;
}

// @brief 定数インデックスのチェック
// @retval true インデックスもしくは範囲が定数にならなければならないとき
// @retval false 上記以外
bool
SptPrimary::is_const_index() const
{
  return mConstIndex;
}

// @brief インデックスリストのサイズの取得
// @return インデックスリストのサイズ
SizeType
SptPrimary::index_num() const
{
  return mIndexArray.size();
}

// @brief インデックスの取得
// @param[in] pos 位置番号 ( 0 <= pos < index_num() )
const PtExpr*
SptPrimary::index(SizeType pos) const
{
  return mIndexArray[pos];
}

// @brief 範囲指定モードの取得
// @return 範囲指定モード
VpiRangeMode
SptPrimary::range_mode() const
{
  return mMode;
}

// @brief 範囲の左側の式の取得
// @return 範囲の左側の式
const PtExpr*
SptPrimary::left_range() const
{
  return mLeftRange;
}

// @brief 範囲の右側の式の取得
// @return 範囲の右側の式
const PtExpr*
SptPrimary::right_range() const
{
  return mRightRange;
}


//////////////////////////////////////////////////////////////////////
// 定数
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] file_region ファイル位置の情報
// @param[in] const_type 定数の型
// @param[in] size ビットサイズ
// @param[in] base 基数
// @param[in] svalue 値の文字列表現
// @param[in] rvalue 実数型の時の値
SptConstant::SptConstant(const FileRegion& file_region,
			 VpiConstType const_type,
			 SizeType size,
			 unsigned int uvalue,
			 const char* svalue,
			 double rvalue) :
  SptExpr{file_region, PtExprType::Const},
  mConstType{const_type},
  mSize{size},
  mUintValue{uvalue},
  mStrValue{svalue},
  mRealValue{rvalue}
{
}

// @brief デストラクタ
SptConstant::~SptConstant()
{
}

// @brief 整数型の定数のサイズの取得
// @return サイズ
SizeType
SptConstant::const_size() const
{
  return mSize;
}

// @brief 整数型の値の取得
// @return 整数型の値
unsigned int
SptConstant::const_uint() const
{
  return mUintValue;
}

// @brief 整数型および文字列型の定数の文字列表現の取得
// @return 文字列型の値
const char*
SptConstant::const_str() const
{
  return mStrValue;
}

// @brief 実数型の値の取得
// @return 実数値
double
SptConstant::const_real() const
{
  return mRealValue;
}

// @brief インデックスとして使える式のチェック
// @retval true インデックスとして使える場合
// @retval false 使えない場合
bool
SptConstant::is_index_expr() const
{
  // ただの整数の場合のみが使える．
  return const_type() == VpiConstType::Int && const_str() == nullptr;
}

// @brief 定数の種類の取得
// @return 定数の種類
VpiConstType
SptConstant::const_type() const
{
  return mConstType;
}


//////////////////////////////////////////////////////////////////////
// expression 関係
//////////////////////////////////////////////////////////////////////

// @brief 単項演算子の生成
// @param[in] file_region ファイル位置の情報
// @param[in] type 演算の種類
// @param[in] opr オペランド
// @return 生成された演算子
const PtExpr*
SptFactory::new_Opr(const FileRegion& file_region,
		    VpiOpType type,
		    const PtExpr* opr)
{
  auto node = new SptOpr1(file_region, type,
			  opr, nullptr, nullptr);
  return node;
}

// @brief 二項演算子の生成
// @param[in] file_region ファイル位置の情報
// @param[in] type 演算の種類
// @param[in] opr1 オペランド1
// @param[in] opr2 オペランド2
// @return 生成された演算子
const PtExpr*
SptFactory::new_Opr(const FileRegion& file_region,
		    VpiOpType type,
		    const PtExpr* opr1,
		    const PtExpr* opr2)
{
  auto node = new SptOpr1(file_region, type,
			  opr1, opr2, nullptr);
  return node;
}

// @brief 三項演算子の生成
// @param[in] file_region ファイル位置の情報
// @param[in] type 演算の種類
// @param[in] opr1 オペランド1
// @param[in] opr2 オペランド2
// @param[in] opr3 オペランド3
// @return 生成された演算子
const PtExpr*
SptFactory::new_Opr(const FileRegion& file_region,
		    VpiOpType type,
		    const PtExpr* opr1,
		    const PtExpr* opr2,
		    const PtExpr* opr3)
{
  auto node = new SptOpr1(file_region, type,
			  opr1, opr2, opr3);
  return node;
}

// @brief concatination 演算子の生成
// @param[in] file_region ファイル位置の情報
// @param[in] expr_list オペランドのリスト
// @return 生成された concatination 演算子
const PtExpr*
SptFactory::new_Concat(const FileRegion& file_region,
		       const vector<const PtExpr*>& expr_array)
{
  auto node = new SptOpr2(file_region, VpiOpType::Concat,
			  PtiArray<const PtExpr>(mAlloc, expr_array));
  return node;
}

// @brief multi-concatination 演算子の生成
// @param[in] file_region ファイル位置の情報
// @param[in] expr_list 結合するオペランドのリスト
// @return 生成された multi-concatination 演算子
const PtExpr*
SptFactory::new_MultiConcat(const FileRegion& file_region,
			    const vector<const PtExpr*>& expr_array)
{
  auto node = new SptOpr2(file_region, VpiOpType::MultiConcat,
			  PtiArray<const PtExpr>(mAlloc, expr_array));
  return node;
}

// @brief min/typ/max delay 演算子の生成
// @param[in] file_region ファイル位置の情報
// @param[in] val0 minimum 値
// @param[in] val1 typical 値
// @param[in] val2 maximum 値
// @return 生成された min/typ/max 演算子
const PtExpr*
SptFactory::new_MinTypMax(const FileRegion& file_region,
			  const PtExpr* val0,
			  const PtExpr* val1,
			  const PtExpr* val2)
{
  auto node = new SptOpr1(file_region, VpiOpType::MinTypMax,
			  val0, val1, val2);
  return node;
}

// @brief primary の生成
// @param[in] file_region ファイル位置の情報
// @param[in] name 識別子名
// @return 生成された primary
const PtExpr*
SptFactory::new_Primary(const FileRegion& file_region,
			const char* name)
{
  auto node = new SptPrimary(file_region,
			     PtiNameBranchArray(),
			     name,
			     false);
  return node;
}

// @brief インデックス付き primary の生成
// @param[in] file_region ファイル位置の情報
// @param[in] name 識別子名
// @param[in] index_array インデックスのリスト
// @return 生成された primary
const PtExpr*
SptFactory::new_Primary(const FileRegion& file_region,
			const char* name,
			const vector<const PtExpr*>& index_array)
{
  auto node = new SptPrimary(file_region,
			     PtiNameBranchArray(),
			     name,
			     false,
			     PtiArray<const PtExpr>(mAlloc, index_array));
  return node;
}

// @brief 範囲指定付き primary の生成
// @param[in] file_region ファイル位置の情報
// @param[in] name 識別子名
// @param[in] mode 範囲指定のモード
// @param[in] left 範囲の左側の式
// @param[in] right 範囲の右側の式
// @return 生成された primary
const PtExpr*
SptFactory::new_Primary(const FileRegion& file_region,
			const char* name,
			VpiRangeMode mode,
			const PtExpr* left,
			const PtExpr* right)
{
  auto node = new SptPrimary(file_region,
			     PtiNameBranchArray(),
			     name,
			     false,
			     PtiExprArray(),
			     mode, left, right);
  return node;
}

// @brief インデックスと範囲指定付き primary の生成
// @param[in] file_region ファイル位置の情報
// @param[in] name 識別子名
// @param[in] index_array インデックスのリスト
// @param[in] mode 範囲指定のモード
// @param[in] left 範囲の左側の式
// @param[in] right 範囲の右側の式
// @return 生成された primary
const PtExpr*
SptFactory::new_Primary(const FileRegion& file_region,
			const char* name,
			const vector<const PtExpr*>& index_array,
			VpiRangeMode mode,
			const PtExpr* left,
			const PtExpr* right)
{
  auto node = new SptPrimary(file_region,
			     PtiNameBranchArray(),
			     name,
			     false,
			     PtiExprArray(mAlloc, index_array),
			     mode, left, right);
  return node;
}

// @brief primary の生成 (階層付き)
// @param[in] file_region ファイル位置の情報
// @param[in] nb_array 識別子の階層部分のリスト
// @param[in] tail_name 識別子の末尾
// @return 生成された primary
const PtExpr*
SptFactory::new_Primary(const FileRegion& file_region,
			PuHierName* hname)
{
  auto nb_array = hname->name_branch();
  auto tail_name = hname->tail_name();
  auto node = new SptPrimary(file_region,
			     PtiNameBranchArray(mAlloc, nb_array),
			     tail_name,
			     false);
  return node;
}

// @brief インデックス付き primary の生成 (階層付き)
// @param[in] file_region ファイル位置の情報
// @param[in] nb_array 識別子の階層部分のリスト
// @param[in] tail_name 識別子の末尾
// @param[in] index_array インデックスのリスト
// @return 生成された primary
const PtExpr*
SptFactory::new_Primary(const FileRegion& file_region,
			PuHierName* hname,
			const vector<const PtExpr*>& index_array)
{
  auto nb_array = hname->name_branch();
  auto tail_name = hname->tail_name();
  auto node = new SptPrimary(file_region,
			     PtiNameBranchArray(mAlloc, nb_array),
			     tail_name,
			     false,
			     PtiExprArray(mAlloc, index_array));
  return node;
}

// @brief 範囲指定付き primary の生成 (階層付き)
// @param[in] file_region ファイル位置の情報
// @param[in] nb_array 識別子の階層部分のリスト
// @param[in] tail_name 識別子の末尾
// @param[in] mode 範囲指定のモード
// @param[in] left 範囲の左側の式
// @param[in] right 範囲の右側の式
// @return 生成された primary
const PtExpr*
SptFactory::new_Primary(const FileRegion& file_region,
			PuHierName* hname,
			VpiRangeMode mode,
			const PtExpr* left,
			const PtExpr* right)
{
  auto nb_array = hname->name_branch();
  auto tail_name = hname->tail_name();
  auto node = new SptPrimary(file_region,
			     PtiNameBranchArray(mAlloc, nb_array),
			     tail_name,
			     false,
			     PtiExprArray(),
			     mode, left, right);
  return node;
}

// @brief インデックスと範囲指定付き primary の生成 (階層付き)
// @param[in] file_region ファイル位置の情報
// @param[in] nb_array 識別子の階層部分のリスト
// @param[in] tail_name 識別子の末尾
// @param[in] index_array インデックスのリスト
// @param[in] mode 範囲指定のモード
// @param[in] left 範囲の左側の式
// @param[in] right 範囲の右側の式
// @return 生成された primary
const PtExpr*
SptFactory::new_Primary(const FileRegion& file_region,
			PuHierName* hname,
			const vector<const PtExpr*>& index_array,
			VpiRangeMode mode,
			const PtExpr* left,
			const PtExpr* right)
{
  auto nb_array = hname->name_branch();
  auto tail_name = hname->tail_name();
  auto node = new SptPrimary(file_region,
			     PtiNameBranchArray(mAlloc, nb_array),
			     tail_name,
			     false,
			     PtiExprArray(mAlloc, index_array),
			     mode, left, right);
  return node;
}

// @brief constant primary の生成
// @param[in] file_region ファイル位置の情報
// @param[in] name 識別子名
// @param[in] index_array インデックスのリスト
// @return 生成された const primary
const PtExpr*
SptFactory::new_CPrimary(const FileRegion& file_region,
			 const char* name,
			 const vector<const PtExpr*>& index_array)
{
  auto node = new SptPrimary(file_region,
			     PtiNameBranchArray(),
			     name,
			     true,
			     PtiExprArray(mAlloc, index_array));
  return node;
}

// @brief 範囲指定付き constant primary の生成
// @param[in] file_region ファイル位置の情報
// @param[in] name 識別子名
// @param[in] mode 範囲指定のモード
// @param[in] left 範囲の左側の式
// @param[in] right 範囲の右側の式
// @return 生成された constant primary
const PtExpr*
SptFactory::new_CPrimary(const FileRegion& file_region,
			 const char* name,
			 VpiRangeMode mode,
			 const PtExpr* left,
			 const PtExpr* right)
{
  auto node = new SptPrimary(file_region,
			     PtiNameBranchArray(),
			     name,
			     true,
			     PtiExprArray(),
			     mode, left, right);
  return node;
}

// @brief インデックス付き constant primary の生成 (階層付き)
// @param[in] file_region ファイル位置の情報
// @param[in] nb_array 識別子の階層部分のリスト
// @param[in] tail_name 識別子の末尾
// @param[in] index_array インデックスのリスト
// @return 生成された constant primary
const PtExpr*
SptFactory::new_CPrimary(const FileRegion& file_region,
			 PuHierName* hname,
			 const vector<const PtExpr*>& index_array)
{
  auto nb_array = hname->name_branch();
  auto tail_name = hname->tail_name();
  auto node = new SptPrimary(file_region,
			     PtiNameBranchArray(mAlloc, nb_array),
			     tail_name,
			     true,
			     PtiExprArray(mAlloc, index_array));
  return node;
}

// @brief function call の生成
// @param[in] file_region ファイル位置の情報
// @param[in] ai_top 属性インスタンスのリスト
// @param[in] name 関数名
// @param[in] arg_array 引数のリスト
// @return 生成された function call
const PtExpr*
SptFactory::new_FuncCall(const FileRegion& file_region,
			 const char* name,
			 const vector<const PtExpr*>& arg_array)
{
  auto node = new SptFuncCall(file_region, PtExprType::FuncCall,
			      PtiNameBranchArray(),
			      name,
			      PtiExprArray(mAlloc, arg_array));
  return node;
}

// @brief function call の生成 (階層付き)
// @param[in] file_region ファイル位置の情報
// @param[in] nb_array 関数名の階層部分のリスト
// @param[in] tail_name 関数名の末尾
// @param[in] arg_array 引数のリスト
// @return 生成された function call
const PtExpr*
SptFactory::new_FuncCall(const FileRegion& file_region,
			 PuHierName* hname,
			 const vector<const PtExpr*>& arg_array)
{
  auto nb_array = hname->name_branch();
  auto tail_name = hname->tail_name();
  auto node = new SptFuncCall(file_region, PtExprType::FuncCall,
			      PtiNameBranchArray(mAlloc, nb_array),
			      tail_name,
			      PtiExprArray(mAlloc, arg_array));
  return node;
}

// @brief system function call の生成
// @param[in] file_region ファイル位置の情報
// @param[in] name 関数名
// @param[in] arg_array 引数のリスト
// @return 生成された function call
const PtExpr*
SptFactory::new_SysFuncCall(const FileRegion& file_region,
			    const char* name,
			    const vector<const PtExpr*>& arg_array)
{
  auto node = new SptFuncCall(file_region, PtExprType::SysFuncCall,
			      PtiNameBranchArray(),
			      name,
			      PtiExprArray(mAlloc, arg_array));
  return node;
}

// @brief 整数型の定数の生成
// @param[in] file_region ファイル位置の情報
// @param[in] value 定数の文字列表現
// @return 生成された定数
const PtExpr*
SptFactory::new_IntConst(const FileRegion& file_region,
			 unsigned int value)
{
  auto node = new SptConstant(file_region, VpiConstType::Int,
			      0, value, nullptr, 0.0);
  return node;
}

// @brief 整数型の定数の生成
// @param[in] file_region ファイル位置の情報
// @param[in] value 定数の文字列表現
// @return 生成された定数
const PtExpr*
SptFactory::new_IntConst(const FileRegion& file_region,
			 const char* value)
{
  auto node = new SptConstant(file_region, VpiConstType::Int,
			      0, 0, value, 0.0);
  return node;
}

// @brief 基底付き整数型の定数の生成
// @param[in] file_region ファイル位置の情報
// @param[in] const_type 定数の種類
// @param[in] value 定数の文字列表現
// @return 生成された定数
const PtExpr*
SptFactory::new_IntConst(const FileRegion& file_region,
			 VpiConstType const_type,
			 const char* value)
{
  auto node = new SptConstant(file_region, const_type,
			      0, 0, value, 0.0);
  return node;
}

// @brief サイズと基底付き定数の生成
// @param[in] file_region ファイル位置の情報
// @param[in] size サイズ
// @param[in] const_type 定数の種類
// @param[in] value 定数の文字列表現
// @return 生成された定数
const PtExpr*
SptFactory::new_IntConst(const FileRegion& file_region,
			 int size,
			 VpiConstType const_type,
			 const char* value)
{
  auto node = new SptConstant(file_region, const_type,
			      size, 0, value, 0.0);
  return node;
}

// @brief 実数型の定数の生成
// @param[in] file_region ファイル位置の情報
// @param[in] value 定数の文字列表現
// @return 生成された定数
const PtExpr*
SptFactory::new_RealConst(const FileRegion& file_region,
			  double value)
{
  auto node = new SptConstant(file_region, VpiConstType::Real,
			      0, 0, nullptr, value);
  return node;
}

// @brief 文字列型の定数の生成
// @param[in] file_region ファイル位置の情報
// @param[in] value 文字列
// @return 生成された定数
const PtExpr*
SptFactory::new_StringConst(const FileRegion& file_region,
			    const char* value)
{
  auto node = new SptConstant(file_region, VpiConstType::String,
			      0, 0, value, 0.0);
  return node;
}

END_NAMESPACE_YM_VERILOG
