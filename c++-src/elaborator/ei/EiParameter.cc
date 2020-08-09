
/// @file EiParameter.cc
/// @brief EiParameter の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "EiFactory.h"
#include "EiParameter.h"

#include "elaborator/ElbExpr.h"

#include "ym/BitVector.h"

#include "ym/pt/PtDecl.h"
#include "ym/pt/PtItem.h"
#include "ym/pt/PtMisc.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief parameter 宣言のヘッダを生成する(範囲指定なし)．
// @param[in] parent 親のスコープ
ElbParamHead*
EiFactory::new_ParamHead(const VlNamedObj* parent,
			 const PtDeclHead* pt_head)
{
  EiParamHead* head = new EiParamHead(parent, pt_head);
  return head;
}

// @brief parameter 宣言のヘッダを生成する．
// @param[in] parent 親のスコープ
// @param[in] pt_head パース木の宣言ヘッダ
// @param[in] left 範囲の左側の式
// @param[in] right 範囲の右側の式
ElbParamHead*
EiFactory::new_ParamHead(const VlNamedObj* parent,
			 const PtDeclHead* pt_head,
			 const PtExpr* left,
			 const PtExpr* right,
			 int left_val,
			 int right_val)
{
  ASSERT_COND( left != nullptr );
  ASSERT_COND( right != nullptr );

  EiParamHead* head = new EiParamHeadV(parent, pt_head,
				       left, right,
				       left_val, right_val);
  return head;
}

// @brief parameter 宣言を生成する．
// @param[in] head ヘッダ
// @param[in] pt_item パース木の宣言要素
// @param[in] init 初期割り当て式
// @param[in] is_local localparam の時 true
ElbParameter*
EiFactory::new_Parameter(ElbParamHead* head,
			 const PtNamedBase* pt_item,
			 bool is_local)
{
  EiParameter* param = nullptr;

  switch ( head->type() ) {
  case VpiObjType::Parameter:
  case VpiObjType::SpecParam:
    if ( is_local ) {
      param = new EiLocalParam(head, pt_item);
    }
    else {
      param = new EiParameter(head, pt_item);
    }
    break;

  default:
    ASSERT_NOT_REACHED;
    break;
  }

  return param;
}


//////////////////////////////////////////////////////////////////////
// クラス EiParamHead
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のスコープ
// @param[in] pt_head パース木の宣言ヘッダ
EiParamHead::EiParamHead(const VlNamedObj* parent,
			 const PtDeclHead* pt_head) :
  mParent(parent),
  mPtHead(pt_head)
{
}

// @brief デストラクタ
EiParamHead::~EiParamHead()
{
}

// @brief 型の取得
VpiObjType
EiParamHead::type() const
{
  switch ( mPtHead->type() ) {
  case PtDeclType::Param:
  case PtDeclType::LocalParam:
    return VpiObjType::Parameter;

  case PtDeclType::SpecParam:
    return VpiObjType::SpecParam;

  default:
    ASSERT_NOT_REACHED;
    break;
  }
  return VpiObjType::Parameter;
}

// @brief このオブジェクトの属しているスコープを返す．
const VlNamedObj*
EiParamHead::parent() const
{
  return mParent;
}

// @brief 符号の取得
// @param[in] val 値
// @retval true 符号つき
// @retval false 符号なし
// @note ヘッダに型指定がない時は値から情報を得る．
bool
EiParamHead::is_signed(const VlValue& val) const
{
  if ( mPtHead->data_type() == VpiVarType::None ) {
    return val.is_signed();
  }
  else {
    return mPtHead->is_signed();
  }
}

// @brief 範囲指定を持つとき true を返す．
bool
EiParamHead::has_range() const
{
  return false;
}

// @brief 範囲の MSB の値を返す．
// @note 範囲を持たないときの値は不定
int
EiParamHead::left_range_val() const
{
  switch ( mPtHead->data_type() ) {
  case VpiVarType::Integer:
    return kVpiSizeInteger - 1;

  case VpiVarType::Real:
  case VpiVarType::Realtime:
    return 0;

  case VpiVarType::Time:
    return kVpiSizeTime - 1;

  default:
    break;
  }
  // int とみなす．
  return kVpiSizeInteger - 1;
}

// @brief 範囲の LSB の値を返す．
// @note 範囲を持たないときの値は不定
int
EiParamHead::right_range_val() const
{
  return 0;
}

// @brief 範囲のMSBを表す文字列の取得
// @note 範囲を持たない時の値は不定
string
EiParamHead::left_range_string() const
{
  return string();
}

// @brief 範囲のLSBを表す文字列の取得
// @note 範囲を持たない時の値は不定
string
EiParamHead::right_range_string() const
{
  return string();
}

// @brief left_range >= right_range の時に true を返す．
bool
EiParamHead::is_big_endian() const
{
  return true;
}

// @brief left_range <= right_range の時に true を返す．
bool
EiParamHead::is_little_endian() const
{
  return false;
}

// @brief ビット幅を返す．
// @param[in] val 値
// @note ヘッダに型指定がない時は値から情報を得る．
SizeType
EiParamHead::bit_size(const VlValue& val) const
{
  switch ( mPtHead->data_type() ) {
  case VpiVarType::Integer:
    return kVpiSizeInteger;

  case VpiVarType::Real:
  case VpiVarType::Realtime:
    return kVpiSizeReal;

  case VpiVarType::Time:
    return kVpiSizeTime;

  case VpiVarType::None:
    return val.bit_size();

  default:
    break;
  }
  ASSERT_NOT_REACHED;
  return kVpiSizeInteger;
}

// @brief オフセット値の取得
// @param[in] index インデックス
// @param[out] offset インデックスに対するオフセット値
// @param[in] val 値
// @retval true インデックスが範囲内に入っている時
// @retval false インデックスが範囲外の時
// @note ヘッダに型指定がない時は値から情報を得る．
bool
EiParamHead::calc_bit_offset(int index,
			     SizeType& offset,
			     const VlValue& val) const
{
  switch ( mPtHead->data_type() ) {
  case VpiVarType::Real:
  case VpiVarType::Realtime:
    return false;

  case VpiVarType::Time:
    if ( index >= 0 && index < static_cast<int>(kVpiSizeTime) ) {
      offset = index;
      return true;
    }
    return false;

  case VpiVarType::Integer:
    if ( index >= 0 && index < static_cast<int>(kVpiSizeInteger) ) {
      offset = index;
      return true;
    }
    return false;

  case VpiVarType::None:
    if ( index >= 0 && index < static_cast<int>(val.bit_size()) ) {
      offset = index;
      return true;
    }
    return false;

  default:
    break;
  }
  ASSERT_NOT_REACHED;
  return false;
}

// @breif 値の型を返す．
// @param[in] val 値
// @note ヘッダに型指定がない時は値から情報を得る．
VlValueType
EiParamHead::value_type(const VlValue& val) const
{
  switch ( mPtHead->data_type() ) {
  case VpiVarType::Real:
  case VpiVarType::Realtime:
    return VlValueType::real_type();

  case VpiVarType::Time:
    return VlValueType::time_type();

  case VpiVarType::Integer:
    return VlValueType::int_type();

  case VpiVarType::None:
    return val.value_type();

  default:
    break;
  }
  ASSERT_NOT_REACHED;
  return VlValueType();
}

// @brief データ型の取得
VpiVarType
EiParamHead::data_type() const
{
  return mPtHead->data_type();
}

// @brief パース木の宣言ヘッダを返す．
const PtDeclHead*
EiParamHead::pt_head() const
{
  return mPtHead;
}


//////////////////////////////////////////////////////////////////////
// クラス EiParamHeadV
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のスコープ
// @param[in] pt_head パース木の宣言ヘッダ
// @param[in] left 範囲の左側の式
// @param[in] right 範囲の右側の式
// @param[in] left_val 範囲の左側の値
// @param[in] right_val 範囲の右側の値
EiParamHeadV::EiParamHeadV(const VlNamedObj* parent,
			   const PtDeclHead* pt_head,
			   const PtExpr* left,
			   const PtExpr* right,
			   int left_val,
			   int right_val) :
  EiParamHead(parent, pt_head)
{
  mRange.set(left, right, left_val, right_val);
}

// @brief デストラクタ
EiParamHeadV::~EiParamHeadV()
{
}

// @brief 符号の取得
// @param[in] val 値
// @retval true 符号つき
// @retval false 符号なし
// @note ヘッダに型指定がない時は値から情報を得る．
bool
EiParamHeadV::is_signed(const VlValue& val) const
{
  return pt_head()->is_signed();
}

// @brief 範囲指定を持つとき true を返す．
bool
EiParamHeadV::has_range() const
{
  return true;
}

// @brief 範囲の MSB の値を返す．
// @note 範囲を持たないときの値は不定
int
EiParamHeadV::left_range_val() const
{
  return mRange.left_range_val();
}

// @brief 範囲の LSB の値を返す．
// @note 範囲を持たないときの値は不定
int
EiParamHeadV::right_range_val() const
{
  return mRange.right_range_val();
}

// @brief 範囲のMSBを表す文字列の取得
// @note 範囲を持たない時の値は不定
string
EiParamHeadV::left_range_string() const
{
  return mRange.left_range_string();
}

// @brief 範囲のLSBを表す文字列の取得
// @note 範囲を持たない時の値は不定
string
EiParamHeadV::right_range_string() const
{
  return mRange.right_range_string();
}

// @brief left_range >= right_range の時に true を返す．
bool
EiParamHeadV::is_big_endian() const
{
  return mRange.is_big_endian();
}

// @brief left_range <= right_range の時に true を返す．
bool
EiParamHeadV::is_little_endian() const
{
  return mRange.is_little_endian();
}

// @brief ビット幅を返す．
// @param[in] val 値
// @note ヘッダに型指定がない時は値から情報を得る．
SizeType
EiParamHeadV::bit_size(const VlValue& val) const
{
  return mRange.size();
}

// @brief オフセット値の取得
// @param[in] index インデックス
// @param[out] offset インデックスに対するオフセット値
// @param[in] val 値
// @retval true インデックスが範囲内に入っている時
// @retval false インデックスが範囲外の時
// @note ヘッダに型指定がない時は値から情報を得る．
bool
EiParamHeadV::calc_bit_offset(int index,
			      SizeType& offset,
			      const VlValue& val) const
{
  return mRange.calc_offset(index, offset);
}

// @breif 値の型を返す．
// @param[in] val 値
// @note ヘッダに型指定がない時は値から情報を得る．
VlValueType
EiParamHeadV::value_type(const VlValue& val) const
{
  return VlValueType(pt_head()->is_signed(), true, mRange.size());
}


//////////////////////////////////////////////////////////////////////
// クラス EiParameter
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] head ヘッダ
// @param[in] pt_item パース木の宣言要素
EiParameter::EiParameter(ElbParamHead* head,
			 const PtNamedBase* pt_item) :
  mHead(head),
  mPtItem(pt_item),
  mExpr(nullptr)
{
}

// @brief デストラクタ
EiParameter::~EiParameter()
{
}

// @brief 型の取得
// @return vpi_user.h で定義された型 (vpiModule など)
VpiObjType
EiParameter::type() const
{
  return mHead->type();
}

// @brief ファイル位置の取得
FileRegion
EiParameter::file_region() const
{
  return mPtItem->file_region();
}

// @brief このオブジェクトの属しているスコープを返す．
const VlNamedObj*
EiParameter::parent() const
{
  return mHead->parent();
}

// @brief 名前の取得
const char*
EiParameter::name() const
{
  return mPtItem->name();
}

// @breif 値の型を返す．
// @note 値を持たないオブジェクトの場合には kVpiValueNone を返す．
VlValueType
EiParameter::value_type() const
{
  // (1) with no type or range specification の場合，そのパラメータに
  //     割り当てられる最終的な値の type と range を持つ．
  //
  // (2) with a range, but with no type の場合，その range を持つ
  //     unsigned 型と定義される．
  //     初期値および override された値はこの型に変換される．
  //
  // (3) with a type specification, but with no range specification の場合，
  //     その型に定義される．range は最終的な値の range をとる．
  //
  // (4) a signed parameter (signed but with no range のこと？) は最終的な値の
  //     範囲を持つ．
  //
  // (5) with a signed type specification and with a range specification
  //     の場合，そのままの型を持つ．値はこの型に変換される．
  //
  // (6) with no range specification, and with either a signed type
  //     specification or no type specification は lsb = 0, msb = size -1
  //     という implied range を持つ．
  //     ただし，最終的な値も unsized の場合，lsb = 0, msb は最低31以上の
  //     実装依存の値をとる．

  return mHead->value_type(mValue);
}

// @brief 符号の取得
// @retval true 符号つき
// @retval false 符号なし
bool
EiParameter::is_signed() const
{
  return mHead->is_signed(mValue);
}

// @brief 範囲指定を持つとき true を返す．
bool
EiParameter::has_range() const
{
  return mHead->has_range();
}

// @brief 範囲の MSB の値を返す．
// @note 範囲を持たないときの値は不定
int
EiParameter::left_range_val() const
{
  return mHead->left_range_val();
}

// @brief 範囲の LSB の値を返す．
// @note 範囲を持たないときの値は不定
int
EiParameter::right_range_val() const
{
  return mHead->right_range_val();
}

// @brief 範囲のMSBを表す文字列の取得
// @note 範囲を持たない時の値は不定
string
EiParameter::left_range_string() const
{
  return mHead->left_range_string();
}

// @brief 範囲のLSBを表す文字列の取得
// @note 範囲を持たない時の値は不定
string
EiParameter::right_range_string() const
{
  return mHead->right_range_string();
}

// @brief left_range >= right_range の時に true を返す．
bool
EiParameter::is_big_endian() const
{
  return mHead->is_big_endian();
}

// @brief left_range <= right_range の時に true を返す．
bool
EiParameter::is_little_endian() const
{
  return mHead->is_little_endian();
}

// @brief ビット幅を返す．
SizeType
EiParameter::bit_size() const
{
  return mHead->bit_size(mValue);
}

// @brief オフセット値の取得
// @param[in] index インデックス
// @param[out] offset インデックスに対するオフセット値
// @retval true インデックスが範囲内に入っている時
// @retval false インデックスが範囲外の時
bool
EiParameter::calc_bit_offset(int index,
			     SizeType& offset) const
{
  return mHead->calc_bit_offset(index, offset, mValue);
}

// @brief データ型の取得
// @retval データ型 kParam, kLocalParam, kVar の場合
// @retval kVpiVarNone 上記以外
VpiVarType
EiParameter::data_type() const
{
  return mHead->data_type();
}

// @brief net 型の取得
// @retval net 型 net 型の要素の場合
// @retval kVpiNone net 型の要素でない場合
// @note このクラスでは kVpiNone を返す．
VpiNetType
EiParameter::net_type() const
{
  return VpiNetType::None;
}

// @brief vectored|scalared 属性の取得
// @retval kVpiVsNone vectored|scalared 指定なし
// @retval kVpiVectored vectored 指定あり
// @retval kVpiScalared scalared 指定あり
// @note このクラスでは kVpiVsNone を返す．
VpiVsType
EiParameter::vs_type() const
{
  return VpiVsType::None;
}

// @brief drive0 strength の取得
// @retval 0 の強度
// @retval kVpiNoStrength strength の指定なし
// @note このクラスでは kVpiNoStrength を返す．
VpiStrength
EiParameter::drive0() const
{
  return VpiStrength::NoStrength;
}

// @brief drive1 strength の取得
// @retval 1 の強度
// @retval kVpiNoStrength strength の指定なし
// @note kVpiNoStrength を返す．
VpiStrength
EiParameter::drive1() const
{
  return VpiStrength::NoStrength;
}

// @brief charge strength の取得
// @retval 電荷の強度
// @retval kVpiNoStrength strength の指定なし
// @note このクラスでは kVpiNoStrength を返す．
VpiStrength
EiParameter::charge() const
{
  return VpiStrength::NoStrength;
}

// @brief delay の取得
// @retval delay
// @retval nullptr delay の指定なし
// @note このクラスでは nullptr を返す．
const VlDelay*
EiParameter::delay() const
{
  return nullptr;
}

// @brief 定数値を持つ型のときに true を返す．
// @note このクラスは true を返す．
bool
EiParameter::is_consttype() const
{
  return true;
}

// @brief 初期値の取得
// @retval 初期値
// @retval nullptr 設定がない場合
const VlExpr*
EiParameter::init_value() const
{
  return nullptr;
}

// @brief localparam のときに true 返す．
// @note このクラスでは false を返す．
bool
EiParameter::is_local_param() const
{
  return false;
}

// @brief 値の取得
VlValue
EiParameter::get_value() const
{
  return mValue;
}

// @brief 値の設定
// @param[in] expr 値を表す式
// @param[in] value 値
void
EiParameter::set_expr(const PtExpr* expr,
		      const VlValue& value)
{
  mExpr = expr;
  mValue = VlValue(value, mHead->value_type(value));
}


//////////////////////////////////////////////////////////////////////
// クラス EiLocalParam
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] head ヘッダ
// @param[in] pt_item パース木の宣言要素
EiLocalParam::EiLocalParam(ElbParamHead* head,
			   const PtNamedBase* pt_item) :
  EiParameter(head, pt_item)
{
}

// @brief デストラクタ
EiLocalParam::~EiLocalParam()
{
}

// @brief localparam のときに true 返す．
// @note このクラスでは true を返す．
bool
EiLocalParam::is_local_param() const
{
  return true;
}

END_NAMESPACE_YM_VERILOG
