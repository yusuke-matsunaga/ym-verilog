
/// @file CptDecl.cc
/// @brief CptDecl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.

#include "CptDecl.h"
#include "alloc/Alloc.h"
#include "parser/CptFactory.h"
#include "ym/pt/PtExpr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// 宣言要素のヘッダの基底クラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptDeclHead::CptDeclHead(
  const FileRegion& file_region
) : mFileRegion{file_region}
{
}

// デストラクタ
CptDeclHead::~CptDeclHead()
{
}

// ファイル位置の取得
FileRegion
CptDeclHead::file_region() const
{
  return mFileRegion;
}

// @brief 符号の取得
bool
CptDeclHead::is_signed() const
{
  return false;
}

// @brief 範囲のMSBの取得
const PtExpr*
CptDeclHead::left_range() const
{
  return nullptr;
}

// @brief 範囲のLSBの取得
const PtExpr*
CptDeclHead::right_range() const
{
  return nullptr;
}

// @brief データ型の取得
VpiVarType
CptDeclHead::data_type() const
{
  return VpiVarType::None;
}

// @brief net 型の取得
VpiNetType
CptDeclHead::net_type() const
{
  return VpiNetType::None;
}

// @brief vectored|scalared 属性の取得
VpiVsType
CptDeclHead::vs_type() const
{
  return VpiVsType::None;
}

// @brief strength の取得
const PtStrength*
CptDeclHead::strength() const
{
  return nullptr;
}

// @brief delay の取得
const PtDelay*
CptDeclHead::delay() const
{
  return nullptr;
}

// @brief 要素数の取得
SizeType
CptDeclHead::item_num() const
{
  return mItemArray.size();
}

// @brief 要素の取得
const PtDeclItem*
CptDeclHead::item(
  SizeType pos
) const
{
  return mItemArray[pos];
}

// @brief 要素リストの設定
void
CptDeclHead::set_elem(
  PtiDeclItemArray&& elem_array
)
{
  mItemArray = std::move(elem_array);
}


//////////////////////////////////////////////////////////////////////
/// parameter 宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptParamH::CptParamH(
  const FileRegion& file_region
) : CptDeclHead{file_region}
{
}

// デストラクタ
CptParamH::~CptParamH()
{
}

// 宣言要素の型の取得
PtDeclType
CptParamH::type() const
{
  return PtDeclType::Param;
}


//////////////////////////////////////////////////////////////////////
/// localparam 宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptLocalParamH::CptLocalParamH(
  const FileRegion& file_region
) : CptDeclHead{file_region}
{
}

// デストラクタ
CptLocalParamH::~CptLocalParamH()
{
}

// 宣言要素の型の取得
PtDeclType
CptLocalParamH::type() const
{
  return PtDeclType::LocalParam;
}


//////////////////////////////////////////////////////////////////////
/// 符号なし範囲つき(ビットベクタタイプ)の parameter ヘッダのクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptParamHV::CptParamHV(
  const FileRegion& file_region,
  const PtExpr* left,
  const PtExpr* right
) : CptParamH{file_region},
    mLeftRange{left},
    mRightRange{right}
{
}

// デストラクタ
CptParamHV::~CptParamHV()
{
}

// 符号の取得
bool
CptParamHV::is_signed() const
{
  // このクラスは常に false を返す．
  return false;
}

// 範囲のMSBの取得
const PtExpr*
CptParamHV::left_range() const
{
  return mLeftRange;
}

// 範囲のLSBの取得
const PtExpr*
CptParamHV::right_range() const
{
  return mRightRange;
}


//////////////////////////////////////////////////////////////////////
/// 符号付き範囲つき(ビットベクタタイプ)の parameter ヘッダのクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptParamHSV::CptParamHSV(
  const FileRegion& file_region,
  const PtExpr* left,
  const PtExpr* right
) : CptParamHV{file_region, left, right}
{
}

// デストラクタ
CptParamHSV::~CptParamHSV()
{
}

// 符号の取得
bool
CptParamHSV::is_signed() const
{
  // このクラスは常に true を返す．
  return true;
}


//////////////////////////////////////////////////////////////////////
// 符号なし範囲つき(ビットベクタタイプ)の localparam ヘッダのクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptLocalParamHV::CptLocalParamHV(
  const FileRegion& file_region,
  const PtExpr* left,
  const PtExpr* right
) : CptParamHV{file_region, left, right}
{
}

// デストラクタ
CptLocalParamHV::~CptLocalParamHV()
{
}

// 宣言要素の型の取得
PtDeclType
CptLocalParamHV::type() const
{
  return PtDeclType::LocalParam;
}


//////////////////////////////////////////////////////////////////////
// 符号付き範囲つき(ビットベクタタイプ)の localparam ヘッダのクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptLocalParamHSV::CptLocalParamHSV(
  const FileRegion& file_region,
  const PtExpr* left,
  const PtExpr* right
) : CptParamHSV{file_region, left, right}
{
}

// デストラクタ
CptLocalParamHSV::~CptLocalParamHSV()
{
}

// 宣言要素の型の取得
PtDeclType
CptLocalParamHSV::type() const
{
  return PtDeclType::LocalParam;
}


//////////////////////////////////////////////////////////////////////
// 組み込み型の parameter ヘッダのクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptParamHT::CptParamHT(
  const FileRegion& file_region,
  VpiVarType var_type
) : CptDeclHead{file_region},
    mVarType{var_type}
{
}

// デストラクタ
CptParamHT::~CptParamHT()
{
}

// 宣言要素の型の取得
PtDeclType
CptParamHT::type() const
{
  return PtDeclType::Param;
}

// @brief 符号の取得
bool
CptParamHT::is_signed() const
{
  switch ( data_type() ) {
  case VpiVarType::Integer:
  case VpiVarType::Real:
  case VpiVarType::Realtime:
    return true;
  default:
    return false;
  }
}

// データ型の取得
VpiVarType
CptParamHT::data_type() const
{
  return mVarType;
}


//////////////////////////////////////////////////////////////////////
// 組み込み型の localparam ヘッダのクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptLocalParamHT::CptLocalParamHT(
  const FileRegion& file_region,
  VpiVarType var_type
) : CptParamHT{file_region, var_type}
{
}

// デストラクタ
CptLocalParamHT::~CptLocalParamHT()
{
}

// 宣言要素の型の取得
PtDeclType
CptLocalParamHT::type() const
{
  return PtDeclType::LocalParam;
}


//////////////////////////////////////////////////////////////////////
// reg 宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptRegH::CptRegH(
  const FileRegion& file_region
) : CptDeclHead{file_region}
{
}

// デストラクタ
CptRegH::~CptRegH()
{
}

// 宣言要素の型の取得
PtDeclType
CptRegH::type() const
{
  return PtDeclType::Reg;
}


//////////////////////////////////////////////////////////////////////
/// 1ビット符号つき？？？ の reg 宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptRegHS::CptRegHS(
  const FileRegion& file_region
) : CptRegH{file_region}
{
}

// デストラクタ
CptRegHS::~CptRegHS()
{
}

// 符号の取得
// このクラスでは true を返す．
bool
CptRegHS::is_signed() const
{
  return true;
}


//////////////////////////////////////////////////////////////////////
// ビットベクタ型の reg 宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptRegHV::CptRegHV(
  const FileRegion& file_region,
  const PtExpr* left,
  const PtExpr* right
) : CptRegH{file_region},
    mLeftRange{left},
    mRightRange{right}
{
}

// デストラクタ
CptRegHV::~CptRegHV()
{
}

// 符号の取得
bool
CptRegHV::is_signed() const
{
  return false;
}

// 範囲のMSBの取得
const PtExpr*
CptRegHV::left_range() const
{
  return mLeftRange;
}

// 範囲のLSBの取得
const PtExpr*
CptRegHV::right_range() const
{
  return mRightRange;
}


//////////////////////////////////////////////////////////////////////
// 符号つきビットベクタ型の reg 宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptRegHSV::CptRegHSV(
  const FileRegion& file_region,
  const PtExpr* left,
  const PtExpr* right
) : CptRegHV{file_region, left, right}
{
}

// デストラクタ
CptRegHSV::~CptRegHSV()
{
}

// 符号の取得
bool
CptRegHSV::is_signed() const
{
  return true;
}


//////////////////////////////////////////////////////////////////////
// 変数宣言 ヘッダのクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptVarH::CptVarH(
  const FileRegion& file_region,
  VpiVarType var_type
) : CptDeclHead{file_region},
    mVarType{var_type}
{
}

// デストラクタ
CptVarH::~CptVarH()
{
}

// 宣言要素の型の取得
PtDeclType
CptVarH::type() const
{
  return PtDeclType::Var;
}

// @brief 符号の取得
bool
CptVarH::is_signed() const
{
  switch ( data_type() ) {
  case VpiVarType::Integer:
  case VpiVarType::Real:
  case VpiVarType::Realtime:
    return true;
  default:
    return false;
  }
}

// データ型の取得
VpiVarType
CptVarH::data_type() const
{
  return mVarType;
}


//////////////////////////////////////////////////////////////////////
// genvar宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptGenvarH::CptGenvarH(
  const FileRegion& file_region
) : CptDeclHead{file_region}
{
}

// デストラクタ
CptGenvarH::~CptGenvarH()
{
}

// 宣言要素の型の取得
PtDeclType
CptGenvarH::type() const
{
  return PtDeclType::Genvar;
}


//////////////////////////////////////////////////////////////////////
// net宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptNetH::CptNetH(
  const FileRegion& file_region,
  VpiNetType net_type,
  bool sign
) : CptDeclHead{file_region}
{
  mFlags = (static_cast<std::uint32_t>(net_type) << 1) | static_cast<std::uint32_t>(sign);
}

// デストラクタ
CptNetH::~CptNetH()
{
}

// 宣言要素の型の取得
PtDeclType
CptNetH::type() const
{
  return PtDeclType::Net;
}

// net 型の取得
VpiNetType
CptNetH::net_type() const
{
  return static_cast<VpiNetType>((mFlags >> 1) & 15);
}

// 符号の有無の取得
bool
CptNetH::is_signed() const
{
  return static_cast<bool>(mFlags & 1);
}


//////////////////////////////////////////////////////////////////////
// strength つきの net宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptNetHS::CptNetHS(
  const FileRegion& file_region,
  VpiNetType net_type,
  bool sign,
  const PtStrength* strength
) : CptNetH{file_region, net_type, sign},
    mStrength{strength}
{
}

// デストラクタ
CptNetHS::~CptNetHS()
{
}

// strength の取得
const PtStrength*
CptNetHS::strength() const
{
  return mStrength;
}


//////////////////////////////////////////////////////////////////////
// delay つきの net宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptNetHD::CptNetHD(
  const FileRegion& file_region,
  VpiNetType net_type,
  bool sign,
  const PtDelay* delay
) : CptNetH{file_region, net_type, sign},
    mDelay{delay}
{
}

// デストラクタ
CptNetHD::~CptNetHD()
{
}

// delay の取得
const PtDelay*
CptNetHD::delay() const
{
  return mDelay;
}


//////////////////////////////////////////////////////////////////////
// strength と delay つきの net宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptNetHSD::CptNetHSD(
  const FileRegion& file_region,
  VpiNetType net_type,
  bool sign,
  const PtStrength* strength,
  const PtDelay* delay
) : CptNetH{file_region, net_type, sign},
    mStrength{strength},
    mDelay{delay}
{
}

// デストラクタ
CptNetHSD::~CptNetHSD()
{
}

// strength の取得
const PtStrength*
CptNetHSD::strength() const
{
  return mStrength;
}

// delay の取得
const PtDelay*
CptNetHSD::delay() const
{
  return mDelay;
}


//////////////////////////////////////////////////////////////////////
// ビットベクタ型の net宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptNetHV::CptNetHV(
  const FileRegion& file_region,
  VpiNetType net_type,
  VpiVsType vstype,
  bool sign,
  const PtExpr* left,
  const PtExpr* right
) : CptNetH{file_region, net_type, sign},
    mLeftRange{left},
    mRightRange{right}
{
  mFlags |= (static_cast<std::uint32_t>(vstype) << 5);
}

// デストラクタ
CptNetHV::~CptNetHV()
{
}

// vectored|scalared 属性の取得
VpiVsType
CptNetHV::vs_type() const
{
  return static_cast<VpiVsType>(mFlags >> 5);
}

// 範囲のMSBの取得
const PtExpr*
CptNetHV::left_range() const
{
  return mLeftRange;
}

// 範囲のLSBの取得
const PtExpr*
CptNetHV::right_range() const
{
  return mRightRange;
}


//////////////////////////////////////////////////////////////////////
// strength つきの CptNetHV
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptNetHVS::CptNetHVS(
  const FileRegion& file_region,
  VpiNetType net_type,
  VpiVsType vstype,
  bool sign,
  const PtExpr* left,
  const PtExpr* right,
  const PtStrength* strength
) : CptNetHV{file_region, net_type,
	     vstype, sign, left, right},
    mStrength{strength}
{
}

// デストラクタ
CptNetHVS::~CptNetHVS()
{
}

// strength の取得
const PtStrength*
CptNetHVS::strength() const
{
  return mStrength;
}


//////////////////////////////////////////////////////////////////////
// delay つきの CptNetHV
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptNetHVD::CptNetHVD(
  const FileRegion& file_region,
  VpiNetType net_type,
  VpiVsType vstype,
  bool sign,
  const PtExpr* left,
  const PtExpr* right,
  const PtDelay* delay
) : CptNetHV{file_region, net_type,
	     vstype, sign, left, right},
    mDelay{delay}
{
}

// デストラクタ
CptNetHVD::~CptNetHVD()
{
}

// delay の取得
const PtDelay*
CptNetHVD::delay() const
{
  return mDelay;
}


//////////////////////////////////////////////////////////////////////
// strength と delay つきの CptNetHV
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptNetHVSD::CptNetHVSD(
  const FileRegion& file_region,
  VpiNetType net_type,
  VpiVsType vstype,
  bool sign,
  const PtExpr* left,
  const PtExpr* right,
  const PtStrength* strength,
  const PtDelay* delay
) : CptNetHV{file_region, net_type,
	     vstype, sign, left, right},
    mStrength{strength},
    mDelay{delay}
{
}

// デストラクタ
CptNetHVSD::~CptNetHVSD()
{
}

// strength の取得
const PtStrength*
CptNetHVSD::strength() const
{
  return mStrength;
}

// delay の取得
const PtDelay*
CptNetHVSD::delay() const
{
  return mDelay;
}


//////////////////////////////////////////////////////////////////////
// event 宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptEventH::CptEventH(
  const FileRegion& file_region
) : CptDeclHead{file_region}
{
}

// デストラクタ
CptEventH::~CptEventH()
{
}

// 宣言要素の型の取得
PtDeclType
CptEventH::type() const
{
  return PtDeclType::Event;
}


//////////////////////////////////////////////////////////////////////
// specparam 宣言のヘッダのベース実装クラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptSpecParamH::CptSpecParamH(
  const FileRegion& file_region
) : CptDeclHead{file_region}
{
}

// デストラクタ
CptSpecParamH::~CptSpecParamH()
{
}

// 宣言要素の型の取得
PtDeclType
CptSpecParamH::type() const
{
  return PtDeclType::SpecParam;
}


//////////////////////////////////////////////////////////////////////
// ビットベクタ型の specparam
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptSpecParamHV::CptSpecParamHV(
  const FileRegion& file_region,
  const PtExpr* left,
  const PtExpr* right
) : CptSpecParamH{file_region},
    mLeftRange{left},
    mRightRange{right}
{
}

// デストラクタ
CptSpecParamHV::~CptSpecParamHV()
{
}

// 範囲のMSBの取得
const PtExpr*
CptSpecParamHV::left_range() const
{
  return mLeftRange;
}

// 範囲のLSBの取得
const PtExpr*
CptSpecParamHV::right_range() const
{
  return mRightRange;
}


//////////////////////////////////////////////////////////////////////
// 宣言要素のベース実装クラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptDeclItemBase::CptDeclItemBase(
  const char* name
) : mName{name}
{
}

// デストラクタ
CptDeclItemBase::~CptDeclItemBase()
{
}

// 名前の取得
const char*
CptDeclItemBase::name() const
{
  return mName;
}

// @brief 範囲リストのサイズの取得
SizeType
CptDeclItemBase::range_num() const
{
  return 0;
}

// @brief 範囲の取得
const PtRange*
CptDeclItemBase::range(
  SizeType pos
) const
{
  ASSERT_NOT_REACHED;
  return nullptr;
}

// 初期値の取得
const PtExpr*
CptDeclItemBase::init_value() const
{
  return nullptr;
}


//////////////////////////////////////////////////////////////////////
// 宣言要素のベース実装クラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptDeclItem::CptDeclItem(
  const FileRegion& file_region,
  const char* name
) : CptDeclItemBase{name},
    mLoc{file_region}
{
}

// デストラクタ
CptDeclItem::~CptDeclItem()
{
}

// ファイル位置の取得
FileRegion
CptDeclItem::file_region() const
{
  return mLoc;
}


//////////////////////////////////////////////////////////////////////
// 範囲をもった宣言要素のクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptDeclItemR::CptDeclItemR(
  const FileRegion& file_region,
  const char* name,
  PtiRangeArray&& range_array
) : CptDeclItemBase{name},
    mFileRegion{file_region},
    mRangeArray{std::move(range_array)}
{
}

// デストラクタ
CptDeclItemR::~CptDeclItemR()
{
}

// ファイル位置の取得
FileRegion
CptDeclItemR::file_region() const
{
  return mFileRegion;
}

// @brief 範囲リストのサイズの取得
SizeType
CptDeclItemR::range_num() const
{
  return mRangeArray.size();
}

// @brief 範囲の取得
const PtRange*
CptDeclItemR::range(
  SizeType pos
) const
{
  return mRangeArray[pos];
}


//////////////////////////////////////////////////////////////////////
// 初期値をもった宣言要素のクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptDeclItemI::CptDeclItemI(
  const FileRegion& file_region,
  const char* name,
  const PtExpr* init_value
) : CptDeclItem{file_region, name},
    mInitValue{init_value}
{
  ASSERT_COND( init_value );
}

// デストラクタ
CptDeclItemI::~CptDeclItemI()
{
}

// ファイル位置の取得
FileRegion
CptDeclItemI::file_region() const
{
  return FileRegion{CptDeclItem::file_region(), mInitValue->file_region()};
}

// 初期値の取得
const PtExpr*
CptDeclItemI::init_value() const
{
  return mInitValue;
}


//////////////////////////////////////////////////////////////////////
// @class CptRange CptDecl.h "CptDecl.h"
// @brief 範囲を表すクラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CptRange::CptRange(
  const FileRegion& fr,
  const PtExpr* msb,
  const PtExpr* lsb
) : mFileRegion{fr},
    mMsb{msb},
    mLsb{lsb}
{
}

// @brief デストラクタ
CptRange::~CptRange()
{
}

// ファイル位置の取得
FileRegion
CptRange::file_region() const
{
  return mFileRegion;
}

// @brief 範囲の MSB を取り出す．
const PtExpr*
CptRange::left() const
{
  return mMsb;
}

// @brief 範囲の LSB を取り出す．
const PtExpr*
CptRange::right() const
{
  return mLsb;
}


//////////////////////////////////////////////////////////////////////
// その他の宣言関係
//////////////////////////////////////////////////////////////////////

// 範囲指定型パラメータ宣言のヘッダの生成
PtiDeclHead*
CptFactory::new_ParamH(
  const FileRegion& file_region,
  bool sign,
  const PtExpr* left,
  const PtExpr* right,
  bool local
)
{
  if ( local ) {
    if ( left == nullptr ) {
      ASSERT_COND( right == nullptr );
      ++ mNumLocalParamH;
      void* p = mAlloc.get_memory(sizeof(CptLocalParamH));
      auto obj = new (p) CptLocalParamH{file_region};
      return obj;
    }
    else {
      ASSERT_COND( right != nullptr );
      ++ mNumLocalParamHV;
      if ( sign ) {
	void* p = mAlloc.get_memory(sizeof(CptLocalParamHSV));
	auto obj = new (p) CptLocalParamHSV{file_region, left, right};
	return obj;
      }
      else {
	void* p = mAlloc.get_memory(sizeof(CptLocalParamHV));
	auto obj = new (p) CptLocalParamHV{file_region, left, right};
	return obj;
      }
    }
  }
  else {
    if ( left == nullptr ) {
      ASSERT_COND( right == nullptr );
      ++ mNumParamH;
      void* p = mAlloc.get_memory(sizeof(CptParamH));
      auto obj = new (p) CptParamH{file_region};
      return obj;
    }
    else {
      ASSERT_COND( right != nullptr );
      ++ mNumParamHV;
      if ( sign ) {
	void* p = mAlloc.get_memory(sizeof(CptParamHSV));
	auto obj = new (p) CptParamHSV{file_region, left, right};
	return obj;
      }
      else {
	void* p = mAlloc.get_memory(sizeof(CptParamHV));
	auto obj = new (p) CptParamHV{file_region, left, right};
	return obj;
      }
    }
  }
}

// 組み込み型パラメータ宣言のヘッダの生成
PtiDeclHead*
CptFactory::new_ParamH(
  const FileRegion& file_region,
  VpiVarType var_type,
  bool local
)
{
  if ( local ) {
    ++ mNumLocalParamHT;
    void* p = mAlloc.get_memory(sizeof(CptLocalParamHT));
    auto obj = new (p) CptLocalParamHT{file_region, var_type};
    return obj;
  }
  else {
    ++ mNumParamHT;
    void* p = mAlloc.get_memory(sizeof(CptParamHT));
    auto obj = new (p) CptParamHT{file_region, var_type};
    return obj;
  }
}

// specparam 宣言のヘッダを生成する．
PtiDeclHead*
CptFactory::new_SpecParamH(
  const FileRegion& file_region,
  const PtExpr* left,
  const PtExpr* right
)
{
  if ( left == nullptr ) {
    ASSERT_COND( right == nullptr );
    ++ mNumSpecParamH;
    void* p = mAlloc.get_memory(sizeof(CptSpecParamH));
    auto obj = new (p) CptSpecParamH(file_region);
    return obj;
  }
  else {
    ASSERT_COND( right != nullptr );
    ++ mNumSpecParamHV;
    void* p = mAlloc.get_memory(sizeof(CptSpecParamHV));
    auto obj = new (p) CptSpecParamHV{file_region, left, right};
    return obj;
  }
}

// event 宣言のヘッダを生成する．
PtiDeclHead*
CptFactory::new_EventH(
  const FileRegion& file_region
)
{
  ++ mNumEventH;
  void* p = mAlloc.get_memory(sizeof(CptEventH));
  auto obj = new (p) CptEventH{file_region};
  return obj;
}

// genvar 宣言のヘッダを生成する．
PtiDeclHead*
CptFactory::new_GenvarH(
  const FileRegion& file_region
)
{
  ++ mNumGenvarH;
  void* p = mAlloc.get_memory(sizeof(CptGenvarH));
  auto obj = new (p) CptGenvarH{file_region};
  return obj;
}

// 変数宣言のヘッダを生成する．
PtiDeclHead*
CptFactory::new_VarH(
  const FileRegion& file_region,
  VpiVarType var_type
)
{
  ++ mNumVarH;
  void* p = mAlloc.get_memory(sizeof(CptVarH));
  auto obj = new (p) CptVarH{file_region, var_type};
  return obj;
}

// reg 宣言のヘッダを生成する．
PtiDeclHead*
CptFactory::new_RegH(
  const FileRegion& file_region,
  bool sign,
  const PtExpr* left,
  const PtExpr* right
)
{
  if ( left == nullptr ) {
    ASSERT_COND( right == nullptr );
    ++ mNumRegH;
    if ( sign ) {
      void* p = mAlloc.get_memory(sizeof(CptRegHS));
      auto obj = new (p) CptRegHS(file_region);
      return obj;
    }
    else {
      void* p = mAlloc.get_memory(sizeof(CptRegH));
      auto obj = new (p) CptRegH{file_region};
      return obj;
    }
  }
  else {
    ASSERT_COND( right != nullptr );
    ++ mNumRegHV;
    if ( sign ) {
      void* p = mAlloc.get_memory(sizeof(CptRegHSV));
      auto obj = new (p) CptRegHSV{file_region, left, right};
      return obj;
    }
    else {
      void* p = mAlloc.get_memory(sizeof(CptRegHV));
      auto obj = new (p) CptRegHV{file_region, left, right};
      return obj;
    }
  }
}

// net 宣言のヘッダを生成する．
PtiDeclHead*
CptFactory::new_NetH(
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
  if ( left == nullptr ) {
    ASSERT_COND( right == nullptr );
    if ( strength == nullptr ) {
      if ( delay == nullptr ) {
	++ mNumNetH;
	void* p = mAlloc.get_memory(sizeof(CptNetH));
	auto obj = new (p) CptNetH{file_region, type, sign};
	return obj;
      }
      else {
	++ mNumNetHD;
	void* p = mAlloc.get_memory(sizeof(CptNetHD));
	auto obj = new (p) CptNetHD{file_region,
				    type, sign,
				    delay};
	return obj;
      }
    }
    else {
      if ( delay == nullptr ) {
	++ mNumNetHS;
	void* p = mAlloc.get_memory(sizeof(CptNetHS));
	auto obj = new (p) CptNetHS{file_region,
				    type, sign,
				    strength};
	return obj;
      }
      else {
	++mNumNetHSD;
	void* p = mAlloc.get_memory(sizeof(CptNetHSD));
	auto obj = new (p) CptNetHSD{file_region,
				     type, sign,
				     strength, delay};
	return obj;
      }
    }
  }
  else {
    ASSERT_COND( right != nullptr );
    if ( strength == nullptr ) {
      if ( delay == nullptr ) {
	++ mNumNetHV;
	void* p = mAlloc.get_memory(sizeof(CptNetHV));
	auto obj = new (p) CptNetHV{file_region,
				    type, vstype, sign, left, right};
	return obj;
      }
      else {
	++ mNumNetHVD;
	void* p = mAlloc.get_memory(sizeof(CptNetHVD));
	auto obj = new (p) CptNetHVD{file_region,
				     type, vstype, sign, left, right,
				     delay};
	return obj;
      }
    }
    else {
      if ( delay == nullptr ) {
	++ mNumNetHVS;
	void* p = mAlloc.get_memory(sizeof(CptNetHVS));
	auto obj = new (p) CptNetHVS{file_region,
				     type, vstype, sign, left, right,
				     strength};
	return obj;
      }
      else {
	++ mNumNetHVSD;
	void* p = mAlloc.get_memory(sizeof(CptNetHVSD));
	auto obj = new (p) CptNetHVSD{file_region,
				      type, vstype, sign, left, right,
				      strength, delay};
	return obj;
      }
    }
  }
}

// 宣言要素を生成する．
const PtDeclItem*
CptFactory::new_DeclItem(
  const FileRegion& file_region,
  const char* name
)
{
  ++ mNumDeclItem;
  void* p = mAlloc.get_memory(sizeof(CptDeclItem));
  auto obj = new (p) CptDeclItem{file_region, name};
  return obj;
}

const PtDeclItem*
CptFactory::new_DeclItem(
  const FileRegion& file_region,
  const char* name,
  const PtExpr* init_value
)
{
  ++ mNumDeclItemI;
  void* p = mAlloc.get_memory(sizeof(CptDeclItemI));
  auto obj = new (p) CptDeclItemI{file_region, name, init_value};
  return obj;
}

const PtDeclItem*
CptFactory::new_DeclItem(
  const FileRegion& file_region,
  const char* name,
  const vector<const PtRange*>& range_array
)
{
  ++ mNumDeclItemR;
  void* p = mAlloc.get_memory(sizeof(CptDeclItemR));
  auto obj = new (p) CptDeclItemR{file_region, name,
				  PtiArray<const PtRange>(mAlloc, range_array)};
  return obj;
}

const PtRange*
CptFactory::new_Range(
  const FileRegion& fr,
  const PtExpr* msb,
  const PtExpr* lsb
)
{
  ++ mNumRange;
  void* p = mAlloc.get_memory(sizeof(CptRange));
  auto obj = new (p) CptRange{fr, msb, lsb};
  return obj;
}

END_NAMESPACE_YM_VERILOG
