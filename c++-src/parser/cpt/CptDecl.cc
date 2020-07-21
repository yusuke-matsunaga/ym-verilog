
/// @file CptDecl.cc
/// @brief CptDecl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "CptDecl.h"
#include "parser/CptFactory.h"

#include "ym/pt/PtExpr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// 宣言要素のヘッダの基底クラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
// @param file_region ファイル位置の情報
CptDeclHead::CptDeclHead(const FileRegion& file_region) :
  mFileRegion(file_region)
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
// @retval true 符号つき
// @retval false 符号なし
// @note このクラスでは false を返す．
bool
CptDeclHead::is_signed() const
{
  return false;
}

// @brief 範囲のMSBの取得
// @retval 範囲のMSB 範囲を持つとき
// @retval nullptr 範囲を持たないとき
// @note このクラスでは nullptr を返す．
const PtExpr*
CptDeclHead::left_range() const
{
  return nullptr;
}

// @brief 範囲のLSBの取得
// @retval 範囲のLSB 範囲を持つとき
// @retval nullptr 範囲を持たないとき
// @note このクラスでは nullptr を返す．
const PtExpr*
CptDeclHead::right_range() const
{
  return nullptr;
}

// @brief データ型の取得
// @retval データ型 kParam, kLocalParam, kVar の場合
// @retval kVpiVarNone 上記以外
// @note このクラスでは kVpiVarNone を返す．
VpiVarType
CptDeclHead::data_type() const
{
  return VpiVarType::None;
}

// @brief net 型の取得
// @retval net 型 net 型の要素の場合
// @retval kVpiNone net 型の要素でない場合
// @note このクラスでは kVpiNone を返す．
VpiNetType
CptDeclHead::net_type() const
{
  return VpiNetType::None;
}

// @brief vectored|scalared 属性の取得
// @retval kVpiVsNone vectored|scalared 指定なし
// @retval kVpiVectored vectored 指定あり
// @retval kVpiScalared scalared 指定あり
// @note このクラスでは kVpiVsNone を返す．
VpiVsType
CptDeclHead::vs_type() const
{
  return VpiVsType::None;
}

// @brief strength の取得
// @retval strength
// @retval nullptr strength の指定なし
// @note このクラスでは nullptr を返す．
const PtStrength*
CptDeclHead::strength() const
{
  return nullptr;
}

// @brief delay の取得
// @retval delay
// @retval nullptr delay の指定なし
// @note このクラスでは nullptr を返す．
const PtDelay*
CptDeclHead::delay() const
{
  return nullptr;
}

// @brief 要素のリストの取得
PtDeclItemArray
CptDeclHead::item_list() const
{
  return mItemArray;
}

// @brief 要素リストの設定
// @param[in] elem_array 要素リスト(配列)
void
CptDeclHead::set_elem(PtDeclItemArray item_array)
{
  mItemArray = item_array;
}


//////////////////////////////////////////////////////////////////////
/// parameter 宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
// @param file_region ファイル位置の情報
CptParamH::CptParamH(const FileRegion& file_region) :
  CptDeclHead(file_region)
{
}

// デストラクタ
CptParamH::~CptParamH()
{
}

// 宣言要素の型の取得
// @return 宣言要素の型
PtDeclType
CptParamH::type() const
{
  return PtDeclType::Param;
}


//////////////////////////////////////////////////////////////////////
/// localparam 宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
// @param file_region ファイル位置の情報
CptLocalParamH::CptLocalParamH(const FileRegion& file_region) :
  CptDeclHead(file_region)
{
}

// デストラクタ
CptLocalParamH::~CptLocalParamH()
{
}

// 宣言要素の型の取得
// @return 宣言要素の型
PtDeclType
CptLocalParamH::type() const
{
  return PtDeclType::LocalParam;
}


//////////////////////////////////////////////////////////////////////
/// 符号なし範囲つき(ビットベクタタイプ)の parameter ヘッダのクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
// @param file_region ファイル位置の情報
// @param left 範囲の左側の式
// @param right 範囲の右側の式
CptParamHV::CptParamHV(const FileRegion& file_region,
		       const PtExpr* left,
		       const PtExpr* right) :
  CptParamH(file_region),
  mLeftRange(left),
  mRightRange(right)
{
}

// デストラクタ
CptParamHV::~CptParamHV()
{
}

// 符号の取得
// @retval true 符号つき
// @retval false 符号なし
bool
CptParamHV::is_signed() const
{
  // このクラスは常に false を返す．
  return false;
}

// 範囲のMSBの取得
// @return 範囲のMSB
const PtExpr*
CptParamHV::left_range() const
{
  return mLeftRange;
}

// 範囲のLSBの取得
// @return 範囲のLSB
const PtExpr*
CptParamHV::right_range() const
{
  return mRightRange;
}


//////////////////////////////////////////////////////////////////////
/// 符号付き範囲つき(ビットベクタタイプ)の parameter ヘッダのクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
// @param file_region ファイル位置の情報
// @param left 範囲の左側の式
// @param right 範囲の右側の式
CptParamHSV::CptParamHSV(const FileRegion& file_region,
			 const PtExpr* left,
			 const PtExpr* right) :
  CptParamHV(file_region, left, right)
{
}

// デストラクタ
CptParamHSV::~CptParamHSV()
{
}

// 符号の取得
// @retval true 符号つき
// @retval false 符号なし
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
// @param file_region ファイル位置の情報
// @param left 範囲の左側の式
// @param right 範囲の右側の式
CptLocalParamHV::CptLocalParamHV(const FileRegion& file_region,
				 const PtExpr* left,
				 const PtExpr* right) :
  CptParamHV(file_region, left, right)
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
// @param file_region ファイル位置の情報
// @param left 範囲の左側の式
// @param right 範囲の右側の式
CptLocalParamHSV::CptLocalParamHSV(const FileRegion& file_region,
				   const PtExpr* left,
				   const PtExpr* right) :
  CptParamHSV(file_region, left, right)
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
// @param file_region ファイル位置の情報
// @param var_type データ型
CptParamHT::CptParamHT(const FileRegion& file_region,
		       VpiVarType var_type) :
  CptDeclHead(file_region),
  mVarType(var_type)
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
// @retval true 符号つき
// @retval false 符号なし
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
// @retval データ型 kParam, kLocalParam, kVar の場合
// @retval kVpiVarNone 上記以外
VpiVarType
CptParamHT::data_type() const
{
  return mVarType;
}


//////////////////////////////////////////////////////////////////////
// 組み込み型の localparam ヘッダのクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
// @param file_region ファイル位置の情報
// @param var_type データ型
CptLocalParamHT::CptLocalParamHT(const FileRegion& file_region,
				 VpiVarType var_type) :
  CptParamHT(file_region, var_type)
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
// @param file_region ファイル位置の情報
CptRegH::CptRegH(const FileRegion& file_region) :
  CptDeclHead(file_region)
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
CptRegHS::CptRegHS(const FileRegion& file_region) :
  CptRegH(file_region)
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
// @param file_region ファイル位置の情報
// @param left 範囲の左側の式
// @param right 範囲の右側の式
CptRegHV::CptRegHV(const FileRegion& file_region,
		   const PtExpr* left,
		   const PtExpr* right) :
  CptRegH(file_region),
  mLeftRange(left),
  mRightRange(right)
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
// @return 範囲のMSB
const PtExpr*
CptRegHV::left_range() const
{
  return mLeftRange;
}

// 範囲のLSBの取得
// @return 範囲のLSB
const PtExpr*
CptRegHV::right_range() const
{
  return mRightRange;
}


//////////////////////////////////////////////////////////////////////
// 符号つきビットベクタ型の reg 宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
// @param file_region ファイル位置の情報
// @param left 範囲の左側の式
// @param right 範囲の右側の式
CptRegHSV::CptRegHSV(const FileRegion& file_region,
		     const PtExpr* left,
		     const PtExpr* right) :
  CptRegHV(file_region, left, right)
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
// @param file_region ファイル位置の情報
// @param var_type データ型
CptVarH::CptVarH(const FileRegion& file_region,
		 VpiVarType var_type) :
  CptDeclHead(file_region),
  mVarType(var_type)
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
// @retval true 符号つき
// @retval false 符号なし
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
// @retval データ型 kParam, kLocalParam, kVar の場合
// @retval kVpiVarNone 上記以外
VpiVarType
CptVarH::data_type() const
{
  return mVarType;
}


//////////////////////////////////////////////////////////////////////
// genvar宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
// @param file_region ファイル位置の情報
CptGenvarH::CptGenvarH(const FileRegion& file_region) :
  CptDeclHead(file_region)
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
// @param file_region ファイル位置の情報
// @param net_type net の型
CptNetH::CptNetH(const FileRegion& file_region,
		 VpiNetType net_type,
		 bool sign) :
  CptDeclHead(file_region)
{
  mFlags = (static_cast<ymuint32>(net_type) << 1) | static_cast<ymuint32>(sign);
}

// デストラクタ
CptNetH::~CptNetH()
{
}

// 宣言要素の型の取得
// @return 宣言要素の型
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
// @param file_region ファイル位置の情報
// @param net_type net の型
// @param strength 信号強度
CptNetHS::CptNetHS(const FileRegion& file_region,
		   VpiNetType net_type,
		   bool sign,
		   const PtStrength* strength) :
  CptNetH(file_region, net_type, sign),
  mStrength(strength)
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
// @param file_region ファイル位置の情報
// @param net_type net の型
// @param delay 遅延
CptNetHD::CptNetHD(const FileRegion& file_region,
		   VpiNetType net_type,
		   bool sign,
		   const PtDelay* delay) :
  CptNetH(file_region, net_type, sign),
  mDelay(delay)
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
// @param file_region ファイル位置の情報
// @param net_type net の型
// @param strength 信号強度
// @param delay 遅延
CptNetHSD::CptNetHSD(const FileRegion& file_region,
		     VpiNetType net_type,
		     bool sign,
		     const PtStrength* strength,
		     const PtDelay* delay) :
  CptNetH(file_region, net_type, sign),
  mStrength(strength),
  mDelay(delay)
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
// @param file_region ファイル位置の情報
// @param net_type net の型
// @param vstype vectored/scalared 属性を表す値
// @param sign 符号の有無を表すフラグ
// @param left 範囲の左側の式
// @param right 範囲の右側の式
CptNetHV::CptNetHV(const FileRegion& file_region,
		   VpiNetType net_type,
		   VpiVsType vstype,
		   bool sign,
		   const PtExpr* left,
		   const PtExpr* right) :
  CptNetH(file_region, net_type, sign),
  mLeftRange(left),
  mRightRange(right)
{
  mFlags |= (static_cast<ymuint32>(vstype) << 5);
}

// デストラクタ
CptNetHV::~CptNetHV()
{
}

// vectored|scalared 属性の取得
// @retval kVpiVsNone vectored|scalared 指定なし
// @retval kVpiVectored vectored 指定あり
// @retval kVpiScalared scalared 指定あり
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
// @param file_region ファイル位置の情報
// @param net_type net の型
// @param vstype vectored/scalared 属性を表す値
// @param sign 符号の有無を表すフラグ
// @param left 範囲の左側の式
// @param right 範囲の右側の式
// @param strength 信号強度
CptNetHVS::CptNetHVS(const FileRegion& file_region,
		     VpiNetType net_type,
		     VpiVsType vstype,
		     bool sign,
		     const PtExpr* left,
		     const PtExpr* right,
		     const PtStrength* strength) :
  CptNetHV(file_region, net_type,
	   vstype, sign, left, right),
  mStrength(strength)
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
// @param file_region ファイル位置の情報
// @param net_type net の型
// @param vstype vectored/scalared 属性を表す値
// @param sign 符号の有無を表すフラグ
// @param left 範囲の左側の式
// @param right 範囲の右側の式
// @param delay 遅延
CptNetHVD::CptNetHVD(const FileRegion& file_region,
		     VpiNetType net_type,
		     VpiVsType vstype,
		     bool sign,
		     const PtExpr* left,
		     const PtExpr* right,
		     const PtDelay* delay) :
  CptNetHV(file_region, net_type,
	   vstype, sign, left, right),
  mDelay(delay)
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
// @param file_region ファイル位置の情報
// @param net_type net の型
// @param vstype vectored/scalared 属性を表す値
// @param sign 符号の有無を表すフラグ
// @param left 範囲の左側の式
// @param right 範囲の右側の式
// @param strength 信号強度
// @param delay 遅延
CptNetHVSD::CptNetHVSD(const FileRegion& file_region,
		       VpiNetType net_type,
		       VpiVsType vstype,
		       bool sign,
		       const PtExpr* left,
		       const PtExpr* right,
		       const PtStrength* strength,
		       const PtDelay* delay) :
  CptNetHV(file_region, net_type,
	   vstype, sign, left, right),
  mStrength(strength),
  mDelay(delay)
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
// @param file_region ファイル位置の情報
CptEventH::CptEventH(const FileRegion& file_region) :
  CptDeclHead(file_region)
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
// @param file_region ファイル位置の情報
CptSpecParamH::CptSpecParamH(const FileRegion& file_region) :
  CptDeclHead(file_region)
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
// @param file_region ファイル位置の情報
// @param left 範囲の左側の式
// @param right 範囲の右側の式
CptSpecParamHV::CptSpecParamHV(const FileRegion& file_region,
			       const PtExpr* left,
			       const PtExpr* right) :
  CptSpecParamH(file_region),
  mLeftRange(left),
  mRightRange(right)
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
// @param name 名前
CptDeclItemBase::CptDeclItemBase(const char* name) :
  mName(name)
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

// dimension list のサイズの取得
// @return ここでは常に 0 を返す．
int
CptDeclItemBase::dimension_list_size() const
{
  return 0;
}

// 範囲のリストの取得
PtRangeArray
CptDeclItemBase::range_list() const
{
  return PtRangeArray{};
}

// 初期値の取得
// @return ここでは常に nullptr を返す．
const PtExpr*
CptDeclItemBase::init_value() const
{
  return nullptr;
}


//////////////////////////////////////////////////////////////////////
// 宣言要素のベース実装クラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
// @param file_region ファイル位置の情報
// @param name 名前
CptDeclItem::CptDeclItem(const FileRegion& file_region,
			 const char* name) :
  CptDeclItemBase(name),
  mLoc(file_region)
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
// @param file_region ファイル位置の情報
// @param name 名前
// @param range_array 範囲のリスト
CptDeclItemR::CptDeclItemR(const FileRegion& file_region,
			   const char* name,
			   PtRangeArray range_array) :
  CptDeclItemBase(name),
  mFileRegion(file_region),
  mRangeArray(range_array)
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

// dimension list のサイズの取得
int
CptDeclItemR::dimension_list_size() const
{
  return mRangeArray.size();
}

// 範囲のリストの取得
PtRangeArray
CptDeclItemR::range_list() const
{
  return mRangeArray;
}


//////////////////////////////////////////////////////////////////////
// 初期値をもった宣言要素のクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
// @param file_region ファイル位置の情報
// @param name 名前
// @param init_value 初期値
CptDeclItemI::CptDeclItemI(const FileRegion& file_region,
			   const char* name,
			   const PtExpr* init_value) :
  CptDeclItem(file_region, name),
  mInitValue(init_value)
{
  ASSERT_COND(init_value );
}

// デストラクタ
CptDeclItemI::~CptDeclItemI()
{
}

// ファイル位置の取得
FileRegion
CptDeclItemI::file_region() const
{
  return FileRegion(CptDeclItem::file_region(), mInitValue->file_region());
}

// 初期値の取得
// @retval 初期値
// @retval nullptr 設定がない場合
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
CptRange::CptRange(const FileRegion& fr,
		   const PtExpr* msb,
		   const PtExpr* lsb) :
  mFileRegion(fr),
  mMsb(msb),
  mLsb(lsb)
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

// パラメータ宣言のヘッダの生成 (型指定なし)
PtiDeclHead*
CptFactory::new_ParamH(const FileRegion& file_region)
{
  ++ mNumParamH;
  auto obj = new CptParamH(file_region);
  return obj;
}

// 範囲指定型パラメータ宣言のヘッダの生成
PtiDeclHead*
CptFactory::new_ParamH(const FileRegion& file_region,
		       bool sign,
		       const PtExpr* left,
		       const PtExpr* right)
{
  ++ mNumParamHV;
  if ( sign ) {
    auto obj = new CptParamHSV(file_region, left, right);
    return obj;
  }
  else {
    auto obj = new CptParamHV(file_region, left, right);
    return obj;
  }
}

// 組み込み型パラメータ宣言のヘッダの生成
PtiDeclHead*
CptFactory::new_ParamH(const FileRegion& file_region,
		       VpiVarType var_type)
{
  ++ mNumParamHT;
  auto obj = new CptParamHT(file_region, var_type);
  return obj;
}

// local param 宣言のヘッダの生成 (型指定なし)
PtiDeclHead*
CptFactory::new_LocalParamH(const FileRegion& file_region)
{
  ++ mNumLocalParamH;
  auto obj = new CptLocalParamH(file_region);
  return obj;
}

// 範囲指定型 local param 宣言のヘッダの生成
PtiDeclHead*
CptFactory::new_LocalParamH(const FileRegion& file_region,
			    bool sign,
			    const PtExpr* left,
			    const PtExpr* right)
{
  ++ mNumLocalParamHV;
  if ( sign ) {
    auto obj = new CptLocalParamHSV(file_region, left, right);
    return obj;
  }
  else {
    auto obj = new CptLocalParamHV(file_region, left, right);
    return obj;
  }
}

// 組み込み型 local param 宣言のヘッダの生成
PtiDeclHead*
CptFactory::new_LocalParamH(const FileRegion& file_region,
			    VpiVarType var_type)
{
  ++ mNumLocalParamHT;
  auto obj = new CptLocalParamHT(file_region, var_type);
  return obj;
}

// specparam 宣言のヘッダを生成する．
PtiDeclHead*
CptFactory::new_SpecParamH(const FileRegion& file_region)
{
  ++ mNumSpecParamH;
  auto obj = new CptSpecParamH(file_region);
  return obj;
}

// specparam 宣言のヘッダを生成する．
PtiDeclHead*
CptFactory::new_SpecParamH(const FileRegion& file_region,
			   const PtExpr* left,
			   const PtExpr* right)
{
  ++ mNumSpecParamHV;
  auto obj = new CptSpecParamHV(file_region, left, right);
  return obj;
}

// event 宣言のヘッダを生成する．
PtiDeclHead*
CptFactory::new_EventH(const FileRegion& file_region)
{
  ++ mNumEventH;
  auto obj = new CptEventH(file_region);
  return obj;
}

// genvar 宣言のヘッダを生成する．
PtiDeclHead*
CptFactory::new_GenvarH(const FileRegion& file_region)
{
  ++ mNumGenvarH;
  auto obj = new CptGenvarH(file_region);
  return obj;
}

// 変数宣言のヘッダを生成する．
PtiDeclHead*
CptFactory::new_VarH(const FileRegion& file_region,
		     VpiVarType var_type)
{
  ++ mNumVarH;
  auto obj = new CptVarH(file_region, var_type);
  return obj;
}

// reg 宣言のヘッダを生成する．
PtiDeclHead*
CptFactory::new_RegH(const FileRegion& file_region,
		     bool sign)
{
  ++ mNumRegH;
  if ( sign ) {
    auto obj = new CptRegHS(file_region);
    return obj;
  }
  else {
    auto obj = new CptRegH(file_region);
    return obj;
  }
}

// reg 宣言のヘッダを生成する．
PtiDeclHead*
CptFactory::new_RegH(const FileRegion& file_region,
		     bool sign,
		     const PtExpr* left,
		     const PtExpr* right)
{
  ++ mNumRegHV;
  if ( sign ) {
    auto obj = new CptRegHSV(file_region, left, right);
    return obj;
  }
  else {
    auto obj = new CptRegHV(file_region, left, right);
    return obj;
  }
}

// net 宣言のヘッダを生成する．
PtiDeclHead*
CptFactory::new_NetH(const FileRegion& file_region,
		     VpiNetType type,
		     bool sign)
{
  ++ mNumNetH;
  auto obj = new CptNetH(file_region, type, sign);
  return obj;
}

// net 宣言のヘッダを生成する．
PtiDeclHead*
CptFactory::new_NetH(const FileRegion& file_region,
		     VpiNetType type,
		     bool sign,
		     const PtStrength* strength)
{
  ++ mNumNetHS;
  auto obj = new CptNetHS(file_region,
			  type, sign,
			  strength);
  return obj;
}

// net 宣言のヘッダを生成する．
PtiDeclHead*
CptFactory::new_NetH(const FileRegion& file_region,
		     VpiNetType type,
		     bool sign,
		     const PtDelay* delay)
{
  ++ mNumNetHD;
  auto obj = new CptNetHD(file_region,
			  type, sign,
			  delay);
  return obj;
}

// net 宣言のヘッダを生成する．
PtiDeclHead*
CptFactory::new_NetH(const FileRegion& file_region,
		     VpiNetType type,
		     bool sign,
		     const PtStrength* strength,
		     const PtDelay* delay)
{
  ++mNumNetHSD;
  auto obj = new CptNetHSD(file_region,
			   type, sign,
			   strength, delay);
  return obj;
}

// net 宣言のヘッダを生成する．
PtiDeclHead*
CptFactory::new_NetH(const FileRegion& file_region,
		     VpiNetType type,
		     VpiVsType vstype,
		     bool sign,
		     const PtExpr* left,
		     const PtExpr* right)
{
  ++ mNumNetHV;
  auto obj = new CptNetHV(file_region,
			  type, vstype, sign, left, right);
  return obj;
}

// net 宣言のヘッダを生成する．
PtiDeclHead*
CptFactory::new_NetH(const FileRegion& file_region,
		     VpiNetType type,
		     VpiVsType vstype,
		     bool sign,
		     const PtExpr* left,
		     const PtExpr* right,
		     const PtStrength* strength)
{
  ++ mNumNetHVS;
  auto obj = new CptNetHVS(file_region,
			   type, vstype, sign, left, right,
			   strength);
  return obj;
}

// net 宣言のヘッダを生成する．
PtiDeclHead*
CptFactory::new_NetH(const FileRegion& file_region,
		     VpiNetType type,
		     VpiVsType vstype,
		     bool sign,
		     const PtExpr* left,
		     const PtExpr* right,
		     const PtDelay* delay)
{
  ++ mNumNetHVD;
  auto obj = new CptNetHVD(file_region,
			   type, vstype, sign, left, right,
			   delay);
  return obj;
}

// net 宣言のヘッダを生成する．
PtiDeclHead*
CptFactory::new_NetH(const FileRegion& file_region,
		     VpiNetType type,
		     VpiVsType vstype,
		     bool sign,
		     const PtExpr* left,
		     const PtExpr* right,
		     const PtStrength* strength,
		     const PtDelay* delay)
{
  ++ mNumNetHVSD;
  auto obj = new CptNetHVSD(file_region,
			    type, vstype, sign, left, right,
			    strength, delay);
  return obj;
}

// 宣言要素を生成する．
const PtDeclItem*
CptFactory::new_DeclItem(const FileRegion& file_region,
			 const char* name)
{
  ++ mNumDeclItem;
  auto obj = new CptDeclItem(file_region, name);
  return obj;
}

const PtDeclItem*
CptFactory::new_DeclItem(const FileRegion& file_region,
			 const char* name,
			 const PtExpr* init_value)
{
  ++ mNumDeclItemI;
  auto obj = new CptDeclItemI(file_region, name, init_value);
  return obj;
}

const PtDeclItem*
CptFactory::new_DeclItem(const FileRegion& file_region,
			 const char* name,
			 PtRangeArray range_array)
{
  ++ mNumDeclItemR;
  auto obj = new CptDeclItemR(file_region, name, range_array);
  return obj;
}

const PtRange*
CptFactory::new_Range(const FileRegion& fr,
		      const PtExpr* msb,
		      const PtExpr* lsb)
{
  auto obj = new CptRange(fr, msb, lsb);
  return obj;
}

END_NAMESPACE_YM_VERILOG
