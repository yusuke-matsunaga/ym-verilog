
/// @file EiImpNet.cc
/// @brief EiNet の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.

#include "ei/EiFactory.h"
#include "ei/EiImpNet.h"

#include "ym/pt/PtExpr.h"
#include "ym/BitVector.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief 暗黙のネットを生成する．
ElbDecl*
EiFactory::new_ImpNet(
  const VlScope* parent,
  const PtExpr* pt_expr,
  VpiNetType net_type
)
{
  auto decl = new EiImpNet{parent, pt_expr, net_type};
  return decl;
}


//////////////////////////////////////////////////////////////////////
// クラス EiImpNet
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiImpNet::EiImpNet(
  const VlScope* parent,
  const PtExpr* pt_expr,
  VpiNetType net_type
) : mParent{parent},
    mPtExpr{pt_expr},
    mNetType{net_type}
{
}

// @brief デストラクタ
EiImpNet::~EiImpNet()
{
}

// @brief 型の取得
VpiObjType
EiImpNet::type() const
{
  return VpiObjType::Net;
}

// @brief ファイル位置の取得
FileRegion
EiImpNet::file_region() const
{
  return mPtExpr->file_region();
}

// @brief このオブジェクトの属しているスコープを返す．
const VlScope*
EiImpNet::parent_scope() const
{
  return mParent;
}

// @brief 名前の取得
string
EiImpNet::name() const
{
  return mPtExpr->name();
}

// @breif 値の型を返す．
VlValueType
EiImpNet::value_type() const
{
  return VlValueType(false, true, 1);
}

// @brief 符号の取得
bool
EiImpNet::is_signed() const
{
  return false;
}

// @brief 範囲指定を持つとき true を返す．
bool
EiImpNet::has_range() const
{
  return false;
}

// @brief 範囲の MSB の値を返す．
int
EiImpNet::left_range_val() const
{
  return 0;
}

// @brief 範囲の LSB の値を返す．
int
EiImpNet::right_range_val() const
{
  return 0;
}

// @brief 範囲のMSBを表す文字列の取得
string
EiImpNet::left_range_string() const
{
  return string();
}

// @brief 範囲のLSBを表す文字列の取得
string
EiImpNet::right_range_string() const
{
  return string();
}

// @brief left_range >= right_range の時に true を返す．
bool
EiImpNet::is_big_endian() const
{
  return true;
}

// @brief left_range <= right_range の時に true を返す．
bool
EiImpNet::is_little_endian() const
{
  return true;
}

// @brief ビット幅を返す．
SizeType
EiImpNet::bit_size() const
{
  return 1;
}

// @brief オフセット値の取得
bool
EiImpNet::calc_bit_offset(
  int index,
  SizeType& offset
) const
{
  if ( index == 0 ) {
    offset = 0;
    return true;
  }
  else {
    return false;
  }
}

// @brief データ型の取得
VpiVarType
EiImpNet::data_type() const
{
  return VpiVarType::None;
}

// @brief net 型の取得
VpiNetType
EiImpNet::net_type() const
{
  return mNetType;
}

// @brief vectored|scalared 属性の取得
VpiVsType
EiImpNet::vs_type() const
{
  return VpiVsType::None;
}

// @brief drive0 strength の取得
VpiStrength
EiImpNet::drive0() const
{
  return VpiStrength::NoStrength;
}

// @brief drive1 strength の取得
VpiStrength
EiImpNet::drive1() const
{
  return VpiStrength::NoStrength;
}

// @brief charge strength の取得
VpiStrength
EiImpNet::charge() const
{
  return VpiStrength::NoStrength;
}

// @brief delay の取得
const VlDelay*
EiImpNet::delay() const
{
  return nullptr;
}

// @brief 定数値を持つ型のときに true を返す．
bool
EiImpNet::is_consttype() const
{
  return false;
}

// @brief 初期値の取得
const VlExpr*
EiImpNet::init_value() const
{
  return nullptr;
}

// @brief localparam のときに true 返す．
bool
EiImpNet::is_local_param() const
{
  return false;
}

// @brief 符号付きに補正する．
void
EiImpNet::set_signed()
{
  // なにもしない．
}

END_NAMESPACE_YM_VERILOG
