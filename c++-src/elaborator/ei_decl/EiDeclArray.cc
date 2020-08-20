
/// @file EiDeclArray.cc
/// @brief EiDeclArray の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.


#include "ei/EiFactory.h"
#include "ei/EiDeclArray.h"

#include "elaborator/ElbDecl.h"
#include "elaborator/ElbExpr.h"

#include "ym/pt/PtDecl.h"
#include "ym/pt/PtItem.h"
#include "ym/pt/PtMisc.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief 宣言要素の配列を生成する．
// @param[in] parent 親のスコープ
// @param[in] head ヘッダ
// @param[in] pt_item パース木の宣言要素
// @param[in] range_src 範囲の配列
const VlDeclArray*
EiFactory::new_DeclArray(ElbDeclHead* head,
			 const PtNamedBase* pt_item,
			 const vector<ElbRangeSrc>& range_src)
{
  SizeType dim_size = range_src.size();
  vector<EiRange> range_array(dim_size);
  for ( SizeType i = 0; i < dim_size; ++ i ) {
    range_array[i].set(range_src[i]);
  }

  auto decl{new EiDeclArray(head, pt_item, range_array)};

  return decl;
}


//////////////////////////////////////////////////////////////////////
// クラス EiDeclArray
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のスコープ
// @param[in] head ヘッダ
// @param[in] pt_item パース木の宣言要素
// @param[in] range_array 範囲の配列
EiDeclArray::EiDeclArray(ElbDeclHead* head,
			 const PtNamedBase* pt_item,
			 const vector<EiRange>& range_array) :
  mHead{head},
  mPtItem{pt_item},
  mRangeList(range_array)
{
}

// @brief デストラクタ
EiDeclArray::~EiDeclArray()
{
}

// @brief 型の取得
// @return vpi_user.h で定義された型 (vpiModule など)
VpiObjType
EiDeclArray::type() const
{
  switch ( mHead->type() ) {
  case VpiObjType::Net:        return VpiObjType::NetArray;
  case VpiObjType::Reg:        return VpiObjType::RegArray;
  case VpiObjType::NamedEvent: return VpiObjType::NamedEventArray;
  case VpiObjType::IntegerVar: return VpiObjType::IntegerVar;
  case VpiObjType::RealVar:    return VpiObjType::RealVar;
  case VpiObjType::TimeVar:    return VpiObjType::TimeVar;
  default: break;
  }
  ASSERT_NOT_REACHED;
  return VpiObjType::NetArray;
}

// @brief ファイル位置を返す．
FileRegion
EiDeclArray::file_region() const
{
  return mPtItem->file_region();
}

// @brief このオブジェクトの属しているスコープを返す．
const VlScope*
EiDeclArray::parent_scope() const
{
  return mHead->parent_scope();
}

// @brief 名前の取得
string
EiDeclArray::name() const
{
  return mPtItem->name();
}

// @breif 値の型を返す．
// @note 値を持たないオブジェクトの場合には kVpiValueNone を返す．
VlValueType
EiDeclArray::value_type() const
{
  return VlValueType();
}

// @brief 符号の取得
// @retval true 符号つき
// @retval false 符号なし
bool
EiDeclArray::is_signed() const
{
  return mHead->is_signed();
}

// @brief 範囲指定を持つとき true を返す．
bool
EiDeclArray::has_range() const
{
  return mHead->has_range();
}

// @brief 範囲の MSB の値を返す．
// @note 範囲を持たないときの値は不定
int
EiDeclArray::left_range_val() const
{
  return mHead->left_range_val();
}

// @brief 範囲の LSB の値を返す．
// @note 範囲を持たないときの値は不定
int
EiDeclArray::right_range_val() const
{
  return mHead->right_range_val();
}

// @brief 範囲のMSBを表す文字列の取得
// @note 範囲を持たない時の値は不定
string
EiDeclArray::left_range_string() const
{
  return mHead->left_range_string();
}

// @brief 範囲のLSBを表す文字列の取得
// @note 範囲を持たない時の値は不定
string
EiDeclArray::right_range_string() const
{
  return mHead->right_range_string();
}

// @brief left_range >= right_range の時に true を返す．
bool
EiDeclArray::is_big_endian() const
{
  return mHead->is_big_endian();
}

// @brief left_range <= right_range の時に true を返す．
bool
EiDeclArray::is_little_endian() const
{
  return mHead->is_little_endian();
}

// @brief ビット幅を返す．
SizeType
EiDeclArray::bit_size() const
{
  return mHead->bit_size();
}

// @brief オフセット値の取得
// @param[in] index インデックス
// @param[out] offset インデックスに対するオフセット値
// @retval true インデックスが範囲内に入っている時
// @retval false インデックスが範囲外の時
bool
EiDeclArray::calc_bit_offset(int index,
			     SizeType& offset) const
{
  return mHead->calc_bit_offset(index, offset);
}

// @brief データ型の取得
// @retval データ型 kParam, kLocalParam, kVar の場合
// @retval kVpiVarNone 上記以外
VpiVarType
EiDeclArray::data_type() const
{
  return mHead->data_type();
}

// @brief net 型の取得
// @retval net 型 net 型の要素の場合
// @retval kVpiNone net 型の要素でない場合
VpiNetType
EiDeclArray::net_type() const
{
  return mHead->net_type();
}

// @brief vectored|scalared 属性の取得
// @retval kVpiVsNone vectored|scalared 指定なし
// @retval kVpiVectored vectored 指定あり
// @retval kVpiScalared scalared 指定あり
VpiVsType
EiDeclArray::vs_type() const
{
  return mHead->vs_type();
}

// @brief drive0 strength の取得
// @retval 0 の強度
// @retval kVpiNoStrength strength の指定なし
VpiStrength
EiDeclArray::drive0() const
{
  return mHead->drive0();
}

// @brief drive1 strength の取得
// @retval 1 の強度
// @retval kVpiNoStrength strength の指定なし
VpiStrength
EiDeclArray::drive1() const
{
  return mHead->drive1();
}

// @brief charge strength の取得
// @retval 電荷の強度
// @retval kVpiNoStrength strength の指定なし
VpiStrength
EiDeclArray::charge() const
{
  return mHead->charge();
}

// @brief delay の取得
// @retval delay
// @retval nullptr delay の指定なし
const VlDelay*
EiDeclArray::delay() const
{
  return mHead->delay();
}

// @brief 配列型オブジェクトの時に true を返す．
bool
EiDeclArray::is_array() const
{
  return true;
}

// @brief 多次元の配列型オブジェクトの時に true を返す．
bool
EiDeclArray::is_multi_array() const
{
  return dimension() > 1;
}

// @brief 配列型オブジェクトの場合の次元数の取得
SizeType
EiDeclArray::dimension() const
{
  return mRangeList.size();
}

// @brief 範囲の取得
// @param[in] pos 位置 ( 0 <= pos < dimension() )
const VlRange*
EiDeclArray::range(SizeType pos) const
{
  return mRangeList.range(pos);
}

// @brief 配列の要素数の取得
SizeType
EiDeclArray::array_size() const
{
  return mRangeList.elem_size();
}

// @brief 1次元配列の場合にインデックスからオフセットを計算する．
// @param[in] index インデックス
// @param[out] offset index に対するオフセット値
// @retval true index が範囲内だった．
// @retval false index が範囲外だった．
bool
EiDeclArray::calc_array_offset(int index,
			       SizeType& offset) const
{
  if ( mRangeList.size() == 1 ) {
    return mRangeList.range(0)->calc_offset(index, offset);
  }
  return false;
}

// @brief 他次元配列の場合にインデックスのリストからオフセットを計算する．
// @param[in] index_list インデックスのリスト
// @param[out] offset index_list に対するオフセット値
// @retval true オフセットが正しく計算できた．
// @retval false index_list のいずれかの値が範囲外だった．
bool
EiDeclArray::calc_array_offset(const vector<int>& index_list,
			       SizeType& offset) const
{
  return mRangeList.calc_offset(index_list, offset);
}

END_NAMESPACE_YM_VERILOG
