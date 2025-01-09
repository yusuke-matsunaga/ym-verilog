
/// @file VlValue.cc
/// @brief VlValue の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.

#include "ym/VlValue.h"
#include "VlValueRep.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス VlValue
//////////////////////////////////////////////////////////////////////

// @brief 空のコンストラクタ
VlValue::VlValue() :
  mRep{new VlValueError{}}
{
}

// @brief コピーコンストラクタ
VlValue::VlValue(
  const VlValue& src
) : mRep{src.mRep}
{
}

// @brief ムーブコンストラクタ
VlValue::VlValue(
  VlValue&& src
) : mRep{std::move(src.mRep)}
{
}

// @brief 整数値からのコンストラクタ
VlValue::VlValue(
  int val
) : mRep{new VlValueInt{val}}
{
}

// @brief std::uint32_t からのコンストラクタ
VlValue::VlValue(
  std::uint32_t val
) : mRep{new VlValueUint{val}}
{
}

// @brief スカラー値からのコンストラクタ
VlValue::VlValue(
  const VlScalarVal& val
) : mRep{new VlValueScalar{val}}
{
}

// @brief time からのコンストラクタ
VlValue::VlValue(
  VlTime val
) : mRep{new VlValueTime{val}}
{
}

// @brief 実数からのコンストラクタ
VlValue::VlValue(
  double val
) : mRep{new VlValueReal(val)}
{
}

// @brief ビットベクタからのコンストラクタ
VlValue::VlValue(
  const BitVector& val
) : mRep{new VlValueBitVector{val}}
{
}

// @brief 型変換を伴うコンストラクタ
VlValue::VlValue(
  const VlValue& src,
  const VlValueType& value_type
)
{
  if ( value_type.is_int_type() ) {
    mRep = shared_ptr<VlValueRep>{new VlValueInt{src.int_value()}};
  }
  else if ( value_type.is_real_type() ) {
    mRep = shared_ptr<VlValueRep>{new VlValueReal{src.real_value()}};
  }
  else if ( value_type.is_time_type() ) {
    mRep = shared_ptr<VlValueRep>{new VlValueTime{src.time_value()}};
  }
  else if ( value_type.is_no_type() ) {
    mRep = src.mRep;
  }
  else if ( value_type.is_bitvector_type() ) {
    auto src_bv = src.bitvector_value();
    auto value_p = new VlValueBitVector{BitVector{src_bv,
						  value_type.size(),
						  value_type.is_sized(),
						  value_type.is_signed(),
						  src_bv.base()}};
    mRep = shared_ptr<VlValueRep>{value_p};
  }
  else {
    ASSERT_NOT_REACHED;
  }
}

// @brief 型変換を伴うムーブコンストラクタ
VlValue::VlValue(
  VlValue&& src,
  const VlValueType& value_type
)
{
  if ( this->value_type() == value_type || value_type.is_no_type() ) {
    // 型が同じ場合だけムーブを使う．
    mRep = std::move(src.mRep);
  }
  else {
    if ( value_type.is_int_type() ) {
      mRep = shared_ptr<VlValueRep>{new VlValueInt{src.int_value()}};
    }
    else if ( value_type.is_real_type() ) {
      mRep = shared_ptr<VlValueRep>{new VlValueReal{src.real_value()}};
    }
    else if ( value_type.is_time_type() ) {
      mRep = shared_ptr<VlValueRep>{new VlValueTime{src.time_value()}};
    }
    else if ( value_type.is_bitvector_type() ) {
      auto src_bv = src.bitvector_value();
      auto value_p = new VlValueBitVector{BitVector{src_bv,
						    value_type.size(),
						    value_type.is_sized(),
						    value_type.is_signed(),
						    src_bv.base()}};
      mRep = shared_ptr<VlValueRep>{value_p};
    }
    else {
      ASSERT_NOT_REACHED;
    }
  }
}

// @brief コピー代入演算子
VlValue&
VlValue::operator=(
  const VlValue& src
)
{
  mRep = src.mRep;
  return *this;
}

// @brief ムーブ代入演算子
VlValue&
VlValue::operator=(
  VlValue&& src
)
{
  mRep = std::move(src.mRep);
  return *this;
}

// @brief デストラクタ
VlValue::~VlValue()
{
}

// @brief 整数値を設定する．
void
VlValue::set(
  int val
)
{
  mRep = shared_ptr<VlValueRep>{new VlValueInt{val}};
}

// @brief unsigned int の値をセットする．
void
VlValue::set(
  unsigned int val
)
{
  mRep = shared_ptr<VlValueRep>{new VlValueUint{val}};
}

// @brief スカラー値をセットする．
void
VlValue::set(
  const VlScalarVal& val
)
{
  mRep = shared_ptr<VlValueRep>{new VlValueScalar{val}};
}

// @brief time の値をセットする．
void
VlValue::set(
  VlTime val
)
{
  mRep = shared_ptr<VlValueRep>{new VlValueTime{val}};
}

// @brief 実数値をセットする．
void
VlValue::set(
  double val
)
{
  mRep = shared_ptr<VlValueRep>{new VlValueReal{val}};
}

// @brief ビットベクタの値をセットする．
void
VlValue::set(
  const BitVector& val
)
{
  mRep = shared_ptr<VlValueRep>{new VlValueBitVector{val}};
}

// @relates VlValue
VlValue
operator-(
  const VlValue& src
)
{
  switch ( src.type() ) {
  case VlValue::Type::INT:
    return VlValue{ - src.int_value() };

  case VlValue::Type::UINT:
  case VlValue::Type::SCALAR:
    return VlValue{ static_cast<unsigned int>(- src.int_value()) };

  case VlValue::Type::REAL:
    return VlValue{ - src.real_value() };

  case VlValue::Type::TIME:
    // これは意味があるとは思えないのでビットベクタに変換する．
  case VlValue::Type::BITVECTOR:
    return VlValue{ - src.bitvector_value() };

  case VlValue::Type::ERROR:
    return src;
  }
  ASSERT_NOT_REACHED;
  return VlValue();
}

// @relates VlValue
VlValue
operator+(
  const VlValue& src1,
  const VlValue& src2
)
{
  if ( src1.is_error() || src2.is_error() ) {
    return VlValue{};
  }
  if ( src1.is_uint() ) {
    if ( src2.is_uint() ) {
      return VlValue{ src1.uint_value() + src2.uint_value() };
    }
    if ( src2.is_int() ) {
      return VlValue{ src1.int_value() + src2.int_value() };
    }
  }
  if ( src1.is_int() ) {
    if ( src2.is_int() || src2.is_int() ) {
      return VlValue{ src1.int_value() + src2.int_value() };
    }
  }
  if ( src1.is_real() || src2.is_real() ) {
    return VlValue{ src1.real_value() + src2.real_value() };
  }
  if ( src1.is_time() && src2.is_time() ) {
    return VlValue{ src1.time_value() + src2.time_value() };
  }
  return VlValue{ src1.bitvector_value() + src2.bitvector_value() };
}

// @relates VlValue
VlValue
operator-(
  const VlValue& src1,
  const VlValue& src2
)
{
  if ( src1.is_error() || src2.is_error() ) {
    return VlValue{};
  }
  if ( src1.is_uint() ) {
    if ( src2.is_uint() ) {
      return VlValue{ src1.uint_value() - src2.uint_value() };
    }
    if ( src2.is_int() ) {
      return VlValue{ src1.int_value() - src2.int_value() };
    }
  }
  if ( src1.is_int() ) {
    if ( src2.is_int() || src2.is_int() ) {
      return VlValue{ src1.int_value() - src2.int_value() };
    }
  }
  if ( src1.is_real() || src2.is_real() ) {
    return VlValue{ src1.real_value() - src2.real_value() };
  }
  return VlValue{ src1.bitvector_value() - src2.bitvector_value() };
}

// @relates VlValue
VlValue
operator*(
  const VlValue& src1,
  const VlValue& src2
)
{
  if ( src1.is_error() || src2.is_error() ) {
    return VlValue{};
  }
  if ( src1.is_uint() ) {
    if ( src2.is_uint() ) {
      return VlValue{ src1.uint_value() * src2.uint_value() };
    }
    if ( src2.is_int() ) {
      return VlValue{ src1.int_value() * src2.int_value() };
    }
  }
  if ( src1.is_int() ) {
    if ( src2.is_int() || src2.is_int() ) {
      return VlValue{ src1.int_value() * src2.int_value() };
    }
  }
  if ( src1.is_real() || src2.is_real() ) {
    return VlValue{ src1.real_value() * src2.real_value() };
  }
  return VlValue{ src1.bitvector_value() * src2.bitvector_value() };
}

// @relates VlValue
VlValue
operator/(
  const VlValue& src1,
  const VlValue& src2
)
{
  if ( src1.is_error() || src2.is_error() ) {
    return VlValue{};
  }
  if ( src1.is_uint() ) {
    if ( src2.is_uint() ) {
      return VlValue{ src1.uint_value() / src2.uint_value() };
    }
    if ( src2.is_int() ) {
      return VlValue{ src1.int_value() / src2.int_value() };
    }
  }
  if ( src1.is_int() ) {
    if ( src2.is_int() || src2.is_int() ) {
      return VlValue{ src1.int_value() / src2.int_value() };
    }
  }
  if ( src1.is_real() || src2.is_real() ) {
    return VlValue{ src1.real_value() / src2.real_value() };
  }
  return VlValue{ src1.bitvector_value() / src2.bitvector_value() };
}

// @relates VlValue
VlValue
operator%(
  const VlValue& src1,
  const VlValue& src2
)
{
  if ( src1.is_error() || src2.is_error() ) {
    return VlValue{};
  }
  if ( src1.is_uint() ) {
    if ( src2.is_uint() ) {
      return VlValue{ src1.uint_value() % src2.uint_value() };
    }
    if ( src2.is_int() ) {
      return VlValue{ src1.int_value() % src2.int_value() };
    }
  }
  if ( src1.is_int() ) {
    if ( src2.is_int() || src2.is_int() ) {
      return VlValue{ src1.int_value() % src2.int_value() };
    }
  }
  if ( src1.is_real() || src2.is_real() ) {
    return VlValue{};
  }
  return VlValue{ src1.bitvector_value() % src2.bitvector_value() };
}

// @relates VlValue
VlValue
power(
  const VlValue& src1,
  const VlValue& src2
)
{
  if ( src1.is_error() || src2.is_error() ) {
    return VlValue{};
  }
  if ( src1.is_real() || src1.is_int() ||
       src2.is_real() || src2.is_int() ) {
    double v1 = src1.real_value();
    double v2 = src2.real_value();
    if ( (v1 == 0.0 && v2 <= 0.0) || (v1 < 0.0 && rint(v2) != v2) ) {
      return VlValue{VlScalarVal::x()};
    }
    return VlValue{ pow(v1, v2) };
  }
  return VlValue{ power(src1.bitvector_value(), src2.bitvector_value()) };
}

// @relates VlValue
VlValue
lt(
  const VlValue& src1,
  const VlValue& src2
)
{
  if ( src1.is_error() || src2.is_error() ) {
    return VlValue{};
  }
  if ( src1.is_int() ) {
    if ( src2.is_int() || src2.is_uint() ) {
      if ( src1.int_value() < src2.int_value() ) {
	return VlValue{VlScalarVal::one()};
      }
      else {
	return VlValue{VlScalarVal::zero()};
      }
    }
  }
  else if ( src1.is_uint() ) {
    if ( src2.is_int() ) {
      if ( src1.int_value() < src2.int_value() ) {
	return VlValue{VlScalarVal::one()};
      }
      else {
	return VlValue{VlScalarVal::zero()};
      }
    }
    else if ( src2.is_uint() ) {
      if ( src1.uint_value() < src2.uint_value() ) {
	return VlValue{VlScalarVal::one()};
      }
      else {
	return VlValue{VlScalarVal::zero()};
      }
    }
  }
  else if ( src1.is_real() || src2.is_real() ) {
    if ( src1.real_value() < src2.real_value() ) {
      return VlValue{VlScalarVal::one()};
    }
    else {
      return VlValue{VlScalarVal::zero()};
    }
  }
  return VlValue{ lt(src1.bitvector_value(), src2.bitvector_value()) };
}

// @relates VlValue
VlValue
gt(
  const VlValue& src1,
  const VlValue& src2
)
{
  return lt(src2, src1);
}

// @relates VlValue
VlValue
le(
  const VlValue& src1,
  const VlValue& src2
)
{
  return log_not(lt(src2, src1));
}

// @relates VlValue
VlValue
ge(
  const VlValue& src1,
  const VlValue& src2
)
{
  return log_not(lt(src1, src2));
}

// @relates VlValue
VlValue
eq(
  const VlValue& src1,
  const VlValue& src2
)
{
  if ( src1.is_error() || src2.is_error() ) {
    return VlValue{};
  }
  if ( (src1.is_int() || src1.is_uint()) &&
       (src2.is_int() || src2.is_uint()) ) {
    if ( src1.int_value() == src2.int_value() ) {
      return VlValue{VlScalarVal::one()};
    }
    else {
      return VlValue{VlScalarVal::zero()};
    }
  }
  if ( src1.is_real() || src2.is_real() ) {
    if ( src1.real_value() == src2.real_value() ) {
      return VlValue{VlScalarVal::one()};
    }
    else {
      return VlValue{VlScalarVal::zero()};
    }
  }
  return VlValue{ eq(src1.bitvector_value(), src2.bitvector_value()) };
}

// @relates VlValue
VlValue
eq_with_x(
  const VlValue& src1,
  const VlValue& src2
)
{
  if ( src1.is_error() || src2.is_error() ) {
    return VlValue{};
  }
  if ( (src1.is_int() || src1.is_uint()) &&
       (src2.is_int() || src2.is_uint()) ) {
    if ( src1.int_value() == src2.int_value() ) {
      return VlValue{VlScalarVal::one()};
    }
    else {
      return VlValue{VlScalarVal::zero()};
    }
  }
  if ( src1.is_real() || src2.is_real() ) {
    if ( src1.real_value() == src2.real_value() ) {
      return VlValue{VlScalarVal::one()};
    }
    else {
      return VlValue{VlScalarVal::zero()};
    }
  }
  return VlValue{ eq_with_x(src1.bitvector_value(), src2.bitvector_value()) };
}

// @relates VlValue
VlValue
eq_with_xz(
  const VlValue& src1,
  const VlValue& src2
)
{
  if ( src1.is_error() || src2.is_error() ) {
    return VlValue{};
  }
  if ( (src1.is_int() || src1.is_uint()) &&
       (src2.is_int() || src2.is_uint()) ) {
    if ( src1.int_value() == src2.int_value() ) {
      return VlValue{VlScalarVal::one()};
    }
    else {
      return VlValue{VlScalarVal::zero()};
    }
  }
  if ( src1.is_real() || src2.is_real() ) {
    if ( src1.real_value() == src2.real_value() ) {
      return VlValue{VlScalarVal::one()};
    }
    else {
      return VlValue{VlScalarVal::zero()};
    }
  }
  return VlValue{ eq_with_xz(src1.bitvector_value(), src2.bitvector_value()) };
}

// @relates VlValue
VlValue
ne(
  const VlValue& src1,
  const VlValue& src2
)
{
  return log_not(eq(src1, src2));
}

// @relates VlValue
VlValue
log_not(
  const VlValue& src
)
{
  if ( src.is_error() ) {
    return VlValue{};
  }
  return VlValue{ !src.logic_value() };
}

// @relates VlValue
VlValue
log_and(
  const VlValue& src1,
  const VlValue& src2
)
{
  if ( src1.is_error() || src2.is_error() ) {
    return VlValue{};
  }
  return VlValue{ src1.logic_value() && src2.logic_value() };
}

// @relates VlValue
VlValue
log_or(
  const VlValue& src1,
  const VlValue& src2
)
{
  if ( src1.is_error() || src2.is_error() ) {
    return VlValue{};
  }
  return VlValue{ src1.logic_value() || src2.logic_value() };
}

// @relates VlValue
VlValue
bit_negate(
  const VlValue& src
)
{
  if ( src.is_error() ) {
    return VlValue{};
  }
  if ( !src.is_bitvector_compat() ) {
    return VlValue{};
  }
  return VlValue{ ~src.bitvector_value() };
}

// @relates VlValue
VlValue
bit_and(
  const VlValue& src1,
  const VlValue& src2
)
{
  if ( src1.is_error() || src2.is_error() ) {
    return VlValue{};
  }
  if ( !src1.is_bitvector_compat() || !src2.is_bitvector_compat() ) {
    return VlValue{};
  }
  return VlValue{ src1.bitvector_value() & src2.bitvector_value() };
}

// @relates VlValue
VlValue
bit_or(
  const VlValue& src1,
  const VlValue& src2
)
{
  if ( src1.is_error() || src2.is_error() ) {
    return VlValue{};
  }
  if ( !src1.is_bitvector_compat() || !src2.is_bitvector_compat() ) {
    return VlValue{};
  }
  return VlValue{ src1.bitvector_value() | src2.bitvector_value() };
}

// @relates VlValue
VlValue
bit_xor(
  const VlValue& src1,
  const VlValue& src2
)
{
  if ( src1.is_error() || src2.is_error() ) {
    return VlValue{};
  }
  if ( !src1.is_bitvector_compat() || !src2.is_bitvector_compat() ) {
    return VlValue{};
  }
  return VlValue{ src1.bitvector_value() ^ src2.bitvector_value() };
}

// @relates VlValue
VlValue
bit_xnor(
  const VlValue& src1,
  const VlValue& src2
)
{
  if ( src1.is_error() || src2.is_error() ) {
    return VlValue{};
  }
  if ( !src1.is_bitvector_compat() || !src2.is_bitvector_compat() ) {
    return VlValue{};
  }
  return VlValue{ ~(src1.bitvector_value() ^ src2.bitvector_value()) };
}

// @brief リダクションAND
VlValue
reduction_and(
  const VlValue& src
)
{
  if ( src.is_error() ) {
    return VlValue{};
  }
  if ( !src.is_bitvector_compat() ) {
    return VlValue{};
  }
  return VlValue{ src.bitvector_value().reduction_and() };
}

// @brief リダクションOR
VlValue
reduction_or(
  const VlValue& src
)
{
  if ( src.is_error() ) {
    return VlValue{};
  }
  if ( !src.is_bitvector_compat() ) {
    return VlValue{};
  }
  return VlValue{ src.bitvector_value().reduction_or() };
}

// @brief リダクションXOR
VlValue
reduction_xor(
  const VlValue& src
)
{
  if ( src.is_error() ) {
    return VlValue{};
  }
  if ( !src.is_bitvector_compat() ) {
    return VlValue{};
  }
  return VlValue{ src.bitvector_value().reduction_xor() };
}

// @brief リダクションNAND
VlValue
reduction_nand(
  const VlValue& src
)
{
  if ( src.is_error() ) {
    return VlValue{};
  }
  if ( !src.is_bitvector_compat() ) {
    return VlValue{};
  }
  return VlValue{ src.bitvector_value().reduction_nand() };
}

// @brief リダクションNOR
VlValue
reduction_nor(
  const VlValue& src
)
{
  if ( src.is_error() ) {
    return VlValue{};
  }
  if ( !src.is_bitvector_compat() ) {
    return VlValue{};
  }
  return VlValue{ src.bitvector_value().reduction_nor() };
}

// @brief リダクションXNOR
VlValue
reduction_xnor(
  const VlValue& src
)
{
  if ( src.is_error() ) {
    return VlValue{};
  }
  if ( !src.is_bitvector_compat() ) {
    return VlValue{};
  }
  return VlValue{ src.bitvector_value().reduction_xnor() };
}

// @relates VlValue
VlValue
operator<<(
  const VlValue& src1,
  const VlValue& src2
)
{
  if ( src1.is_error() || src2.is_error() ) {
    return VlValue{};
  }
  if ( !src1.is_bitvector_compat() || !src2.is_bitvector_compat() ) {
    return VlValue{};
  }
  return VlValue{ src1.bitvector_value() << src2.bitvector_value() };
}

// @relates VlValue
VlValue
operator<<(
  const VlValue& src1,
  std::uint32_t src2
)
{
  if ( src1.is_error() ) {
    return VlValue{};
  }
  if ( !src1.is_bitvector_compat() ) {
    return VlValue{};
  }
  return VlValue{ src1.bitvector_value() << src2 };
}

// @relates VlValue
VlValue
operator>>(
  const VlValue& src1,
  const VlValue& src2
)
{
  if ( src1.is_error() || src2.is_error() ) {
    return VlValue{};
  }
  if ( !src1.is_bitvector_compat() || !src2.is_bitvector_compat() ) {
    return VlValue{};
  }
  return VlValue{ src1.bitvector_value() >> src2.bitvector_value() };
}

// @relates VlValue
VlValue
operator>>(
  const VlValue& src1,
  std::uint32_t src2
)
{
  if ( src1.is_error() ) {
    return VlValue{};
  }
  if ( !src1.is_bitvector_compat() ) {
    return VlValue{};
  }
  return VlValue{ src1.bitvector_value() >> src2 };
}

// @relates VlValue
VlValue
alshift(
  const VlValue& src1,
  const VlValue& src2
)
{
  if ( src1.is_error() || src2.is_error() ) {
    return VlValue{};
  }
  if ( !src1.is_bitvector_compat() || !src2.is_bitvector_compat() ) {
    return VlValue{};
  }
  return VlValue{ alshift(src1.bitvector_value(), src2.bitvector_value()) };
}

// @relates VlValue
VlValue
alshift(
  const VlValue& src1,
  std::uint32_t src2
)
{
  if ( src1.is_error() ) {
    return VlValue{};
  }
  if ( !src1.is_bitvector_compat() ) {
    return VlValue{};
  }
  return VlValue{ alshift(src1.bitvector_value(), src2) };
}

// @relates VlValue
VlValue
arshift(
  const VlValue& src1,
  const VlValue& src2
)
{
  if ( src1.is_error() || src2.is_error() ) {
    return VlValue{};
  }
  if ( !src1.is_bitvector_compat() || !src2.is_bitvector_compat() ) {
    return VlValue{};
  }
  return VlValue{ arshift(src1.bitvector_value(), src2.bitvector_value()) };
}

// @relates VlValue
VlValue
arshift(
  const VlValue& src1,
  std::uint32_t src2
)
{
  if ( src1.is_error() ) {
    return VlValue{};
  }
  if ( !src1.is_bitvector_compat() ) {
    return VlValue{};
  }
  return VlValue{ arshift(src1.bitvector_value(), src2) };
}

// @relates VlValue
VlValue
ite(
  const VlValue& src1,
  const VlValue& src2,
  const VlValue& src3
)
{
  if ( src1.logic_value().to_bool() ) {
    return src2;
  }
  else {
    return src3;
  }
}

// @relates VlValue
VlValue
ite(
  const VlScalarVal& src1,
  const VlValue& src2,
  const VlValue& src3
)
{
  if ( src1.to_bool() ) {
    return src2;
  }
  else {
    return src3;
  }
}

// @relates VlValue
VlValue
concat(
  const list<VlValue>& src_list
)
{
  vector<BitVector> bv_array;
  bv_array.reserve(src_list.size());
  for ( const auto& v: src_list ) {
    if ( !v.is_bitvector_compat() ) {
      return VlValue{};
    }
    bv_array.push_back(v.bitvector_value());
  }
  return VlValue{ concat(bv_array) };
}

// @relates VlValue
VlValue
concat(
  const vector<VlValue>& src_list
)
{
  vector<BitVector> bv_array;
  bv_array.reserve(src_list.size());
  for ( const auto& v: src_list ) {
    if ( !v.is_bitvector_compat() ) {
      return VlValue{};
    }
    bv_array.push_back(v.bitvector_value());
  }
  return VlValue{ concat(bv_array) };
}

// @relates VlValue
VlValue
multi_concat(
  const list<VlValue>& src_list
)
{
  vector<BitVector> bv_array;
  bv_array.reserve(src_list.size());
  auto p = src_list.begin();
  const auto& v0 = *p;
  if ( !v0.is_bitvector_compat() ) {
    return VlValue{};
  }
  for (++ p; p != src_list.end(); ++ p) {
    const auto& v = *p;
    if ( v.is_bitvector_compat() ) {
      return VlValue{};
    }
    bv_array.push_back(v.bitvector_value());
  }
  return VlValue{ multi_concat(v0.bitvector_value(), bv_array) };
}

// @relates VlValue
VlValue
multi_concat(
  const vector<VlValue>& src_list
)
{
  vector<BitVector> bv_array;
  bv_array.reserve(src_list.size());
  auto p = src_list.begin();
  const auto& v0 = *p;
  if ( !v0.is_bitvector_compat() ) {
    return VlValue{};
  }
  for (++ p; p != src_list.end(); ++ p) {
    const auto& v = *p;
    if ( v.is_bitvector_compat() ) {
      return VlValue{};
    }
    bv_array.push_back(v.bitvector_value());
  }
  return VlValue{ multi_concat(v0.bitvector_value(), bv_array) };
}

END_NAMESPACE_YM_VERILOG
