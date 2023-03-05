
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
const VlDeclArray*
EiFactory::new_DeclArray(
  ElbDeclHead* head,
  const PtNamedBase* pt_item,
  const vector<ElbRangeSrc>& range_src
)
{
  SizeType dim_size = range_src.size();
  vector<EiRange> range_array(dim_size);
  for ( SizeType i = 0; i < dim_size; ++ i ) {
    range_array[i].set(range_src[i]);
  }

  auto decl = new EiDeclArray{head, pt_item, range_array};
  return decl;
}


//////////////////////////////////////////////////////////////////////
// クラス EiDeclArray
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiDeclArray::EiDeclArray(
  ElbDeclHead* head,
  const PtNamedBase* pt_item,
  const vector<EiRange>& range_array
) : mHead{head},
    mPtItem{pt_item},
    mRangeList(range_array)
{
}

// @brief デストラクタ
EiDeclArray::~EiDeclArray()
{
}

// @brief 型の取得
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
VlValueType
EiDeclArray::value_type() const
{
  return VlValueType();
}

// @brief 符号の取得
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
int
EiDeclArray::left_range_val() const
{
  return mHead->left_range_val();
}

// @brief 範囲の LSB の値を返す．
int
EiDeclArray::right_range_val() const
{
  return mHead->right_range_val();
}

// @brief 範囲のMSBを表す文字列の取得
string
EiDeclArray::left_range_string() const
{
  return mHead->left_range_string();
}

// @brief 範囲のLSBを表す文字列の取得
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
bool
EiDeclArray::calc_bit_offset(
  int index,
  SizeType& offset
) const
{
  return mHead->calc_bit_offset(index, offset);
}

// @brief データ型の取得
VpiVarType
EiDeclArray::data_type() const
{
  return mHead->data_type();
}

// @brief net 型の取得
VpiNetType
EiDeclArray::net_type() const
{
  return mHead->net_type();
}

// @brief vectored|scalared 属性の取得
VpiVsType
EiDeclArray::vs_type() const
{
  return mHead->vs_type();
}

// @brief drive0 strength の取得
VpiStrength
EiDeclArray::drive0() const
{
  return mHead->drive0();
}

// @brief drive1 strength の取得
VpiStrength
EiDeclArray::drive1() const
{
  return mHead->drive1();
}

// @brief charge strength の取得
VpiStrength
EiDeclArray::charge() const
{
  return mHead->charge();
}

// @brief delay の取得
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
const VlRange*
EiDeclArray::range(
  SizeType pos
) const
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
bool
EiDeclArray::calc_array_offset(
  int index,
  SizeType& offset
) const
{
  if ( mRangeList.size() == 1 ) {
    return mRangeList.range(0)->calc_offset(index, offset);
  }
  return false;
}

// @brief 他次元配列の場合にインデックスのリストからオフセットを計算する．
bool
EiDeclArray::calc_array_offset(
  const vector<int>& index_list,
  SizeType& offset
) const
{
  return mRangeList.calc_offset(index_list, offset);
}

END_NAMESPACE_YM_VERILOG
