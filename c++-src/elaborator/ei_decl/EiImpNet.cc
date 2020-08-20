
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
// @param[in] pt_expr パース木のプライマリ式
ElbDecl*
EiFactory::new_ImpNet(const VlScope* parent,
		      const PtExpr* pt_expr,
		      VpiNetType net_type)
{
  auto decl{new EiImpNet(parent, pt_expr, net_type)};

  return decl;
}


//////////////////////////////////////////////////////////////////////
// クラス EiImpNet
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_expr パース木のプライマリ式
EiImpNet::EiImpNet(const VlScope* parent,
		   const PtExpr* pt_expr,
		   VpiNetType net_type) :
  mParent{parent},
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
// @note 値を持たないオブジェクトの場合には kVpiValueNone を返す．
VlValueType
EiImpNet::value_type() const
{
  return VlValueType(false, true, 1);
}

// @brief 符号の取得
// @retval true 符号つき
// @retval false 符号なし
// @note このクラスでは false を返す．
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
// @note 範囲を持たないときの値は不定
int
EiImpNet::left_range_val() const
{
  return 0;
}

// @brief 範囲の LSB の値を返す．
// @note 範囲を持たないときの値は不定
int
EiImpNet::right_range_val() const
{
  return 0;
}

// @brief 範囲のMSBを表す文字列の取得
// @note 範囲を持たない時の値は不定
string
EiImpNet::left_range_string() const
{
  return string();
}

// @brief 範囲のLSBを表す文字列の取得
// @note 範囲を持たない時の値は不定
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
// @param[in] index インデックス
// @param[out] offset インデックスに対するオフセット値
// @retval true インデックスが範囲内に入っている時
// @retval false インデックスが範囲外の時
bool
EiImpNet::calc_bit_offset(int index,
			  SizeType& offset) const
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
// @retval データ型 パラメータや変数の場合
// @retval kVpiVarNone 上記以外
VpiVarType
EiImpNet::data_type() const
{
  return VpiVarType::None;
}

// @brief net 型の取得
// @retval net 型 net 型の要素の場合
// @retval kVpiNone net 型の要素でない場合
VpiNetType
EiImpNet::net_type() const
{
  return mNetType;
}

// @brief vectored|scalared 属性の取得
// @retval kVpiVsNone vectored|scalared 指定なし
// @retval kVpiVectored vectored 指定あり
// @retval kVpiScalared scalared 指定あり
VpiVsType
EiImpNet::vs_type() const
{
  return VpiVsType::None;
}

// @brief drive0 strength の取得
// @retval 0 の強度
// @retval kVpiNoStrength strength の指定なし
VpiStrength
EiImpNet::drive0() const
{
  return VpiStrength::NoStrength;
}

// @brief drive1 strength の取得
// @retval 1 の強度
// @retval kVpiNoStrength strength の指定なし
VpiStrength
EiImpNet::drive1() const
{
  return VpiStrength::NoStrength;
}

// @brief charge strength の取得
// @retval 電荷の強度
// @retval kVpiNoStrength strength の指定なし
VpiStrength
EiImpNet::charge() const
{
  return VpiStrength::NoStrength;
}

// @brief delay の取得
// @retval delay
// @retval nullptr delay の指定なし
const VlDelay*
EiImpNet::delay() const
{
  return nullptr;
}

// @brief 定数値を持つ型のときに true を返す．
// @note このクラスは false を返す．
bool
EiImpNet::is_consttype() const
{
  return false;
}

// @brief 初期値の取得
// @retval 初期値
// @retval nullptr 設定がない場合
const VlExpr*
EiImpNet::init_value() const
{
  return nullptr;
}

// @brief localparam のときに true 返す．
// @note このクラスは false を返す．
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
