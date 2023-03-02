
/// @file SptDecl.cc
/// @brief SptDecl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.

#include "SptDecl.h"
#include "parser/SptFactory.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// IO宣言のヘッダのベース実装クラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
SptIOHead::SptIOHead(
  const FileRegion& file_region,
  VpiDir dir,
  VpiAuxType aux_type,
  VpiNetType net_type,
  VpiVarType var_type,
  bool sign,
  const PtExpr* left,
  const PtExpr* right
) : mFileRegion{file_region},
    mDir{dir},
    mAuxType{aux_type},
    mNetType{net_type},
    mVarType{var_type},
    mSigned{sign},
    mLeftRange{left},
    mRightRange{right}
{
}

// デストラクタ
SptIOHead::~SptIOHead()
{
}

// ファイル位置の取得
FileRegion
SptIOHead::file_region() const
{
  return mFileRegion;
}

// 型の取得
VpiDir
SptIOHead::direction() const
{
  return mDir;
}

// 補助的な型の取得
VpiAuxType
SptIOHead::aux_type() const
{
  return mAuxType;
}

// 補助的なネット型の取得
VpiNetType
SptIOHead::net_type() const
{
  return mNetType;
}

// 補助的な変数型の取得
VpiVarType
SptIOHead::var_type() const
{
  return mVarType;
}

// 符号の取得
bool
SptIOHead::is_signed() const
{
  return mSigned;
}

// 範囲のMSBの取得
const PtExpr*
SptIOHead::left_range() const
{
  return mLeftRange;
}

// 範囲のLSBの取得
const PtExpr*
SptIOHead::right_range() const
{
  return mRightRange;
}

// @brief 要素数の取得
SizeType
SptIOHead::item_num() const
{
  return mItemArray.size();
}

// @brief 要素の取得
const PtIOItem*
SptIOHead::item(
  SizeType pos
) const
{
  return mItemArray[pos];
}

// @brief 要素リストの設定
void
SptIOHead::set_elem(
  PtiIOItemArray&& elem_array
)
{
  mItemArray = move(elem_array);
}


//////////////////////////////////////////////////////////////////////
// IO宣言要素を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
SptIOItem::SptIOItem(
  const FileRegion& file_region,
  const char* name,
  const PtExpr* init_value
) : mFileRegion{file_region},
    mName{name},
    mInitValue{init_value}
{
}

// デストラクタ
SptIOItem::~SptIOItem()
{
}

// ファイル位置の取得
FileRegion
SptIOItem::file_region() const
{
  return mFileRegion;
}

// 名前の取得
const char*
SptIOItem::name() const
{
  return mName;
}

// 初期値の取得
const PtExpr*
SptIOItem::init_value() const
{
  return mInitValue;
}


//////////////////////////////////////////////////////////////////////
// 宣言要素のヘッダの基底クラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
SptDeclHead::SptDeclHead(
  const FileRegion& file_region,
  PtDeclType type,
  bool sign,
  const PtExpr* left,
  const PtExpr* right,
  VpiVarType var_type,
  VpiNetType net_type,
  VpiVsType vs_type,
  const PtStrength* strength,
  const PtDelay* delay
) : mFileRegion{file_region},
    mType{type},
    mSigned{sign},
    mLeftRange{left},
    mRightRange{right},
    mVarType{var_type},
    mNetType{net_type},
    mVsType{vs_type},
    mStrength{strength},
    mDelay{delay}
{
}

// デストラクタ
SptDeclHead::~SptDeclHead()
{
}

// ファイル位置の取得
FileRegion
SptDeclHead::file_region() const
{
  return mFileRegion;
}

// 宣言要素の型の取得
PtDeclType
SptDeclHead::type() const
{
  return mType;
}

// 符号の取得
bool
SptDeclHead::is_signed() const
{
  return mSigned;
}

// 範囲のMSBの取得
const PtExpr*
SptDeclHead::left_range() const
{
  return mLeftRange;
}

// 範囲のLSBの取得
const PtExpr*
SptDeclHead::right_range() const
{
  return mRightRange;
}

// データ型の取得
VpiVarType
SptDeclHead::data_type() const
{
  return mVarType;
}

// net 型の取得
VpiNetType
SptDeclHead::net_type() const
{
  return mNetType;
}

// vectored|scalared 属性の取得
VpiVsType
SptDeclHead::vs_type() const
{
  return mVsType;
}

// strength の取得
const PtStrength*
SptDeclHead::strength() const
{
  return mStrength;
}

// delay の取得
const PtDelay*
SptDeclHead::delay() const
{
  return mDelay;
}

// @brief 要素数の取得
SizeType
SptDeclHead::item_num() const
{
  return mItemArray.size();
}

// @brief 要素の取得
const PtDeclItem*
SptDeclHead::item(
  SizeType pos
) const
{
  return mItemArray[pos];
}

// @brief 要素リストの設定
void
SptDeclHead::set_elem(
  PtiDeclItemArray&& elem_array
)
{
  mItemArray = move(elem_array);
}


//////////////////////////////////////////////////////////////////////
// 宣言要素のベース実装クラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
SptDeclItem::SptDeclItem(
  const FileRegion& file_region,
  const char* name,
  PtiRangeArray&& range_array,
  const PtExpr* init_value
) : mFileRegion{file_region},
    mName{name},
    mRangeArray{move(range_array)},
    mInitValue{init_value}
{
}

// デストラクタ
SptDeclItem::~SptDeclItem()
{
}

// ファイル位置の取得
FileRegion
SptDeclItem::file_region() const
{
  return mFileRegion;
}

// 名前の取得
const char*
SptDeclItem::name() const
{
  return mName;
}

// @brief dimension list のサイズの取得
SizeType
SptDeclItem::range_num() const
{
  return mRangeArray.size();
}

// @brief 範囲の取得
const PtRange*
SptDeclItem::range(
  SizeType pos
) const
{
  return mRangeArray[pos];
}

// 初期値の取得
const PtExpr*
SptDeclItem::init_value() const
{
  return mInitValue;
}


//////////////////////////////////////////////////////////////////////
// @class SptRange SptDecl.h "SptDecl.h"
// @brief 範囲を表すクラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SptRange::SptRange(
  const FileRegion& file_region,
  const PtExpr* msb,
  const PtExpr* lsb
) : mFileRegion{file_region},
    mMsb{msb},
    mLsb{lsb}
{
}

// @brief デストラクタ
SptRange::~SptRange()
{
}

// ファイル位置の取得
FileRegion
SptRange::file_region() const
{
  return mFileRegion;
}

// @brief 範囲の MSB を取り出す．
const PtExpr*
SptRange::left() const
{
  return mMsb;
}

// @brief 範囲の LSB を取り出す．
const PtExpr*
SptRange::right() const
{
  return mLsb;
}


//////////////////////////////////////////////////////////////////////
// IO 宣言関係のオブジェクトを生成する関数
//////////////////////////////////////////////////////////////////////

// @brief 範囲付きの IO 宣言のヘッダの生成
PtiIOHead*
SptFactory::new_IOHead(
  const FileRegion& file_region,
  VpiDir dir,
  bool sign,
  const PtExpr* left,
  const PtExpr* right)
{
  auto node = new SptIOHead{file_region,
			    dir, VpiAuxType::None, VpiNetType::None,
			    VpiVarType::None,
			    sign, left, right};
  return node;
}

// @brief 範囲付きの IO 宣言のヘッダの生成 (reg 型)
PtiIOHead*
SptFactory::new_RegIOHead(
  const FileRegion& file_region,
  VpiDir dir,
  bool sign,
  const PtExpr* left,
  const PtExpr* right
)
{
  auto node = new SptIOHead{file_region,
			    dir, VpiAuxType::Reg, VpiNetType::None,
			    VpiVarType::None,
			    sign, left, right};
  return node;
}

// @brief 範囲付きの IO 宣言のヘッダの生成 (ネット型)
PtiIOHead*
SptFactory::new_NetIOHead(
  const FileRegion& file_region,
  VpiDir dir,
  VpiNetType net_type,
  bool sign,
  const PtExpr* left,
  const PtExpr* right
)
{
  auto node = new SptIOHead{file_region,
			    dir, VpiAuxType::Net, net_type,
			    VpiVarType::None,
			    sign, left, right};
  return node;
}

// @brief IO 宣言のヘッダの生成 (変数型)
PtiIOHead*
SptFactory::new_VarIOHead(
  const FileRegion& file_region,
  VpiDir dir,
  VpiVarType var_type
)
{
  auto node = new SptIOHead{file_region,
			    dir, VpiAuxType::Var, VpiNetType::None,
			    var_type,
			    false, nullptr, nullptr};
  return node;
}

// @brief 初期値付き IO 宣言の要素の生成
const PtIOItem*
SptFactory::new_IOItem(
  const FileRegion& file_region,
  const char* name,
  const PtExpr* init_value
)
{
  auto node = new SptIOItem{file_region, name, init_value};
  return node;
}


//////////////////////////////////////////////////////////////////////
// その他の宣言関係
//////////////////////////////////////////////////////////////////////

// @brief 範囲指定型パラメータ宣言のヘッダの生成
PtiDeclHead*
SptFactory::new_ParamH(
  const FileRegion& file_region,
  bool sign,
  const PtExpr* left,
  const PtExpr* right,
  bool local
)
{
  auto type = local ? PtDeclType::LocalParam : PtDeclType::Param;
  auto node = new SptDeclHead{file_region,
			      type,
			      sign, left, right,
			      VpiVarType::None, VpiNetType::None, VpiVsType::None,
			      nullptr, nullptr};
  return node;
}

// @brief 組み込み型パラメータ宣言のヘッダの生成
PtiDeclHead*
SptFactory::new_ParamH(
  const FileRegion& file_region,
  VpiVarType var_type,
  bool local
)
{
  auto type = local ? PtDeclType::LocalParam : PtDeclType::Param;
  auto node = new SptDeclHead{file_region,
			      type,
			      false, nullptr, nullptr,
			      var_type, VpiNetType::None, VpiVsType::None,
			      nullptr, nullptr};
  return node;
}

// @brief 範囲指定型 specparam 宣言のヘッダの生成
PtiDeclHead*
SptFactory::new_SpecParamH(
  const FileRegion& file_region,
  const PtExpr* left,
  const PtExpr* right
)
{
  auto node = new SptDeclHead{file_region,
			      PtDeclType::SpecParam,
			      false, left, right,
			      VpiVarType::None, VpiNetType::None, VpiVsType::None,
			      nullptr, nullptr};
  return node;
}

// @brief イベント宣言のヘッダの生成
PtiDeclHead*
SptFactory::new_EventH(
  const FileRegion& file_region
)
{
  auto node = new SptDeclHead{file_region,
			      PtDeclType::Event,
			      false, nullptr, nullptr,
			      VpiVarType::None, VpiNetType::None, VpiVsType::None,
			      nullptr, nullptr};
  return node;
}

// @brief genvar 宣言のヘッダの生成
PtiDeclHead*
SptFactory::new_GenvarH(
  const FileRegion& file_region
)
{
  auto node = new SptDeclHead{file_region,
			      PtDeclType::Genvar,
			      false, nullptr, nullptr,
			      VpiVarType::None, VpiNetType::None, VpiVsType::None,
			      nullptr, nullptr};
  return node;
}

// @brief 変数宣言のヘッダの生成
PtiDeclHead*
SptFactory::new_VarH(
  const FileRegion& file_region,
  VpiVarType var_type
)
{
  bool sign = false;
  switch ( var_type ) {
  case VpiVarType::Integer:
  case VpiVarType::Real:
  case VpiVarType::Realtime:
    sign = true;
  default:
    break;
  }
  auto node = new SptDeclHead{file_region,
			      PtDeclType::Var,
			      sign, nullptr, nullptr,
			      var_type, VpiNetType::None, VpiVsType::None,
			      nullptr, nullptr};
  return node;
}

// @brief 範囲指定型 reg 宣言のヘッダの生成
PtiDeclHead*
SptFactory::new_RegH(
  const FileRegion& file_region,
  bool sign,
  const PtExpr* left,
  const PtExpr* right
)
{
  auto node = new SptDeclHead{file_region,
			      PtDeclType::Reg,
			      sign, left, right,
			      VpiVarType::None, VpiNetType::None, VpiVsType::None,
			      nullptr, nullptr};
  return node;
}

// @brief 範囲指定型 net 宣言のヘッダの生成 (strength, 遅延あり)
PtiDeclHead*
SptFactory::new_NetH(
  const FileRegion& file_region,
  VpiNetType type,
  VpiVsType vstype,
  bool sign,
  const PtExpr* left,
  const PtExpr* right,
  const PtStrength* strength,
  const PtDelay* delay
)
{
  auto node = new SptDeclHead{file_region,
			      PtDeclType::Net,
			      sign, left, right,
			      VpiVarType::None, type, vstype,
			      strength, delay};
  return node;
}

// @brief 宣言要素の生成
const PtDeclItem*
SptFactory::new_DeclItem(
  const FileRegion& file_region,
  const char* name
)
{
  auto node = new SptDeclItem{file_region, name,
    PtiArray<const PtRange>{},
    nullptr};
  return node;
}

// @brief 初期値付き宣言要素の生成
const PtDeclItem*
SptFactory::new_DeclItem(
  const FileRegion& file_region,
  const char* name,
  const PtExpr* init_value
)
{
  auto node = new SptDeclItem{file_region, name,
    PtiArray<const PtRange>{},
    init_value};
  return node;
}

// @brief 配列型宣言要素の生成
const PtDeclItem*
SptFactory::new_DeclItem(
  const FileRegion& file_region,
  const char* name,
  const vector<const PtRange*>& range_array
)
{
  auto node = new SptDeclItem{file_region, name,
			      PtiArray<const PtRange>(mAlloc, range_array),
			      nullptr};
  return node;
}

// @brief 範囲の生成
const PtRange*
SptFactory::new_Range(
  const FileRegion& file_region,
  const PtExpr* msb,
  const PtExpr* lsb
)
{
  auto node = new SptRange{file_region, msb, lsb};
  return node;
}

END_NAMESPACE_YM_VERILOG
