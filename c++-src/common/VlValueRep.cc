
/// @file VlValueRep.cc
/// @brief VlValueRep の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.

#include "VlValueRep.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス VlValueError
//////////////////////////////////////////////////////////////////////

// @brief 型を返す．
VlValue::Type
VlValueError::type() const
{
  return Type::ERROR;
}

// @brief 整数型に変換可能な時に true を返す．
bool
VlValueError::is_int_compat() const
{
  return false;
}

// @brief unsigned int 型に変換可能な時に true を返す．
bool
VlValueError::is_uint_compat() const
{
  return false;
}

// @brief 実数型に変換可能な時に true を返す．
bool
VlValueError::is_real_compat() const
{
  return false;
}

// @brief time 型に変換可能な時に true を返す．
bool
VlValueError::is_time_compat() const
{
  return false;
}

// @brief ビットベクタ型に変換可能な時に true を返す．
bool
VlValueError::is_bitvector_compat() const
{
  return false;
}

// @brief 整数型の値を返す．
int
VlValueError::int_value() const
{
  return 0;
}

// @brief unsigned int 型の値を返す．
unsigned int
VlValueError::uint_value() const
{
  return 0;
}

// @brief スカラー型の値を返す．
VlScalarVal
VlValueError::scalar_value() const
{
  return VlScalarVal::x();
}

// @brief 論理型の値を返す．
VlScalarVal
VlValueError::logic_value() const
{
  return VlScalarVal::x();
}

// @brief 実数型の値を返す．
double
VlValueError::real_value() const
{
  return 0.0;
}

// @brief time 型の値を返す．
VlTime
VlValueError::time_value() const
{
  return VlTime();
}

// @brief ビットベクタ型の値を返す．
BitVector
VlValueError::bitvector_value(
  const VlValueType& req
) const
{
  return BitVector();
}

// @brief 自身の複製を返す．
VlValueRep*
VlValueError::duplicate() const
{
  return new VlValueError{};
}


//////////////////////////////////////////////////////////////////////
// クラス VlValueInt
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
VlValueInt::VlValueInt(
  int val
) : mVal{val}
{
}

// @brief 型を返す．
VlValueRep::Type
VlValueInt::type() const
{
  return Type::INT;
}

// @brief 整数型に変換可能な時に true を返す．
bool
VlValueInt::is_int_compat() const
{
  return true;
}

// @brief unsigned int 型に変換可能な時に true を返す．
bool
VlValueInt::is_uint_compat() const
{
  return true;
}

// @brief 実数型に変換可能な時に true を返す．
bool
VlValueInt::is_real_compat() const
{
  return true;
}

// @brief time 型に変換可能な時に true を返す．
bool
VlValueInt::is_time_compat() const
{
  return true;
}

// @brief ビットベクタ型に変換可能な時に true を返す．
bool
VlValueInt::is_bitvector_compat() const
{
  return true;
}

// @brief 整数型の値を返す．
int
VlValueInt::int_value() const
{
  return mVal;
}

// @brief unsigned int 型の値を返す．
unsigned int
VlValueInt::uint_value() const
{
  return static_cast<unsigned int>(mVal);
}

// @brief スカラー型の値を返す．
VlScalarVal
VlValueInt::scalar_value() const
{
  return VlScalarVal(mVal);
}

// @brief 論理型の値を返す．
VlScalarVal
VlValueInt::logic_value() const
{
  if ( mVal != 0 ) {
    return VlScalarVal::one();
  }
  return VlScalarVal::zero();
}

// @brief 実数型の値を返す．
double
VlValueInt::real_value() const
{
  return static_cast<double>(mVal);
}

// @brief time 型の値を返す．
VlTime
VlValueInt::time_value() const
{
  return VlTime(uint_value());
}

// @brief ビットベクタ型の値を返す．
BitVector
VlValueInt::bitvector_value(
  const VlValueType& req
) const
{
  return BitVector(mVal).coerce(req);
}

// @brief 自身の複製を返す．
VlValueRep*
VlValueInt::duplicate() const
{
  return new VlValueInt{mVal};
}


//////////////////////////////////////////////////////////////////////
// クラス VlValueUint
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
VlValueUint::VlValueUint(
  unsigned int val
) : mVal{val}
{
}

// @brief 型を返す．
VlValueRep::Type
VlValueUint::type() const
{
  return Type::UINT;
}

// @brief 整数型に変換可能な時に true を返す．
bool
VlValueUint::is_int_compat() const
{
  return true;
}

// @brief unsigned int 型に変換可能な時に true を返す．
bool
VlValueUint::is_uint_compat() const
{
  return true;
}

// @brief 実数型に変換可能な時に true を返す．
bool
VlValueUint::is_real_compat() const
{
  return true;
}

// @brief time 型に変換可能な時に true を返す．
bool
VlValueUint::is_time_compat() const
{
  return true;
}

// @brief ビットベクタ型に変換可能な時に true を返す．
bool
VlValueUint::is_bitvector_compat() const
{
  return true;
}

// @brief 整数型の値を返す．
int
VlValueUint::int_value() const
{
  return static_cast<int>(mVal);
}

// @brief unsigned int 型の値を返す．
unsigned int
VlValueUint::uint_value() const
{
  return mVal;
}

// @brief スカラー型の値を返す．
VlScalarVal
VlValueUint::scalar_value() const
{
  return VlScalarVal(mVal);
}

// @brief 論理型の値を返す．
VlScalarVal
VlValueUint::logic_value() const
{
  if ( mVal != 0 ) {
    return VlScalarVal::one();
  }
  return VlScalarVal::zero();
}

// @brief 実数型の値を返す．
double
VlValueUint::real_value() const
{
  return static_cast<double>(mVal);
}

// @brief time 型の値を返す．
VlTime
VlValueUint::time_value() const
{
  return VlTime{mVal};
}

// @brief ビットベクタ型の値を返す．
BitVector
VlValueUint::bitvector_value(
  const VlValueType& req
) const
{
  return BitVector{mVal}.coerce(req);
}

// @brief 自身の複製を返す．
VlValueRep*
VlValueUint::duplicate() const
{
  return new VlValueUint{mVal};
}


//////////////////////////////////////////////////////////////////////
// クラス VlValueScalar
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
VlValueScalar::VlValueScalar(
  const VlScalarVal& val
) : mVal{val}
{
}

// @brief 型を返す．
VlValueRep::Type
VlValueScalar::type() const
{
  return Type::SCALAR;
}

// @brief 整数型に変換可能な時に true を返す．
bool
VlValueScalar::is_int_compat() const
{
  return !mVal.is_xz();
}

// @brief unsigned int 型に変換可能な時に true を返す．
bool
VlValueScalar::is_uint_compat() const
{
  return is_int_compat();
}

// @brief 実数型に変換可能な時に true を返す．
bool
VlValueScalar::is_real_compat() const
{
  return is_int_compat();
}

// @brief time 型に変換可能な時に true を返す．
bool
VlValueScalar::is_time_compat() const
{
  return is_int_compat();
}

// @brief ビットベクタ型に変換可能な時に true を返す．
bool
VlValueScalar::is_bitvector_compat() const
{
  return true;
}

// @brief 整数型の値を返す．
int
VlValueScalar::int_value() const
{
  return mVal.to_int();
}

// @brief unsigned int 型の値を返す．
unsigned int
VlValueScalar::uint_value() const
{
  return static_cast<unsigned int>(int_value());
}

// @brief スカラー型の値を返す．
VlScalarVal
VlValueScalar::scalar_value() const
{
  return mVal;
}

// @brief 論理型の値を返す．
VlScalarVal
VlValueScalar::logic_value() const
{
  if ( mVal.is_z() ) {
    return VlScalarVal::x();
  }
  return mVal;
}

// @brief 実数型の値を返す．
double
VlValueScalar::real_value() const
{
  return static_cast<double>(int_value());
}

// @brief time 型の値を返す．
VlTime
VlValueScalar::time_value() const
{
  return VlTime(uint_value());
}

// @brief ビットベクタ型の値を返す．
BitVector
VlValueScalar::bitvector_value(
  const VlValueType& req
) const
{
  return BitVector{mVal}.coerce(req);
}

// @brief 自身の複製を返す．
VlValueRep*
VlValueScalar::duplicate() const
{
  return new VlValueScalar{mVal};
}


//////////////////////////////////////////////////////////////////////
// クラス VlValueReal
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
VlValueReal::VlValueReal(
  double val
) : mVal{val}
{
}

// @brief 型を返す．
VlValueRep::Type
VlValueReal::type() const
{
  return Type::REAL;
}

// @brief 整数型に変換可能な時に true を返す．
bool
VlValueReal::is_int_compat() const
{
  return true;
}

// @brief unsigned int 型に変換可能な時に true を返す．
bool
VlValueReal::is_uint_compat() const
{
  return true;
}

// @brief 実数型に変換可能な時に true を返す．
bool
VlValueReal::is_real_compat() const
{
  return true;
}

// @brief time 型に変換可能な時に true を返す．
bool
VlValueReal::is_time_compat() const
{
  return true;
}

// @brief ビットベクタ型に変換可能な時に true を返す．
bool
VlValueReal::is_bitvector_compat() const
{
  return false;
}

// @brief 整数型の値を返す．
int
VlValueReal::int_value() const
{
  return static_cast<int>(mVal);
}

// @brief unsigned int 型の値を返す．
unsigned int
VlValueReal::uint_value() const
{
  return static_cast<unsigned int>(mVal);
}

// @brief スカラー型の値を返す．
VlScalarVal
VlValueReal::scalar_value() const
{
  return VlScalarVal(mVal);
}

// @brief 論理型の値を返す．
VlScalarVal
VlValueReal::logic_value() const
{
  if ( mVal != 0.0 ) {
    return VlScalarVal::one();
  }
  return VlScalarVal::zero();
}

// @brief 実数型の値を返す．
double
VlValueReal::real_value() const
{
  return mVal;
}

// @brief time 型の値を返す．
VlTime
VlValueReal::time_value() const
{
  return VlTime(mVal);
}

// @brief ビットベクタ型の値を返す．
BitVector
VlValueReal::bitvector_value(
  const VlValueType& req
) const
{
  return BitVector{};
}

// @brief 自身の複製を返す．
VlValueRep*
VlValueReal::duplicate() const
{
  return new VlValueReal{mVal};
}


//////////////////////////////////////////////////////////////////////
// クラス VlValueTime
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
VlValueTime::VlValueTime(
  VlTime val
) : mVal{val}
{
}

// @brief 型を返す．
VlValueRep::Type
VlValueTime::type() const
{
  return Type::TIME;
}

// @brief 整数型に変換可能な時に true を返す．
bool
VlValueTime::is_int_compat() const
{
  return mVal.value() <= 0x7FFFFFFFUL;
}

// @brief unsigned int 型に変換可能な時に true を返す．
bool
VlValueTime::is_uint_compat() const
{
  return mVal.value() <= 0xFFFFFFFFUL;
}

// @brief 実数型に変換可能な時に true を返す．
bool
VlValueTime::is_real_compat() const
{
  return true;
}

// @brief time 型に変換可能な時に true を返す．
bool
VlValueTime::is_time_compat() const
{
  return true;
}

// @brief ビットベクタ型に変換可能な時に true を返す．
bool
VlValueTime::is_bitvector_compat() const
{
  return true;
}

// @brief 整数型の値を返す．
int
VlValueTime::int_value() const
{
  return static_cast<int>(mVal.to_uint());
}

// @brief unsigned int 型の値を返す．
unsigned int
VlValueTime::uint_value() const
{
  return mVal.to_uint();
}

// @brief スカラー型の値を返す．
VlScalarVal
VlValueTime::scalar_value() const
{
  return VlScalarVal(mVal.low());
}

// @brief 論理型の値を返す．
VlScalarVal
VlValueTime::logic_value() const
{
  if ( mVal.value() != 0UL ) {
    return VlScalarVal::one();
  }
  return VlScalarVal::zero();
}

// @brief 実数型の値を返す．
double
VlValueTime::real_value() const
{
  return mVal.to_real();
}

// @brief time 型の値を返す．
VlTime
VlValueTime::time_value() const
{
  return mVal;
}

// @brief ビットベクタ型の値を返す．
BitVector
VlValueTime::bitvector_value(
  const VlValueType& req
) const
{
  return BitVector{mVal}.coerce(req);
}

// @brief 自身の複製を返す．
VlValueRep*
VlValueTime::duplicate() const
{
  return new VlValueTime{mVal};
}


//////////////////////////////////////////////////////////////////////
// クラス VlValueBitVector
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
VlValueBitVector::VlValueBitVector(
  const BitVector& val
) : mVal{val}
{
}

// @brief 型を返す．
VlValueRep::Type
VlValueBitVector::type() const
{
  return Type::BITVECTOR;
}

// @brief 整数型に変換可能な時に true を返す．
bool
VlValueBitVector::is_int_compat() const
{
  return mVal.is_int();
}

// @brief unsigned int 型に変換可能な時に true を返す．
bool
VlValueBitVector::is_uint_compat() const
{
  return mVal.is_uint32();
}

// @brief 実数型に変換可能な時に true を返す．
bool
VlValueBitVector::is_real_compat() const
{
  return true;
}

// @brief time 型に変換可能な時に true を返す．
bool
VlValueBitVector::is_time_compat() const
{
  return mVal.is_time();
}

// @brief ビットベクタ型に変換可能な時に true を返す．
bool
VlValueBitVector::is_bitvector_compat() const
{
  return true;
}

// @brief 整数型の値を返す．
int
VlValueBitVector::int_value() const
{
  return mVal.to_int();
}

// @brief unsigned int 型の値を返す．
unsigned int
VlValueBitVector::uint_value() const
{
  return mVal.to_uint32();
}

// @brief スカラー型の値を返す．
VlScalarVal
VlValueBitVector::scalar_value() const
{
  return mVal.to_scalar();
}

// @brief 論理型の値を返す．
VlScalarVal
VlValueBitVector::logic_value() const
{
  return mVal.to_logic();
}

// @brief 実数型の値を返す．
double
VlValueBitVector::real_value() const
{
  return mVal.to_real();
}

// @brief time 型の値を返す．
VlTime
VlValueBitVector::time_value() const
{
  return mVal.to_time();
}

// @brief ビットベクタ型の値を返す．
BitVector
VlValueBitVector::bitvector_value(
  const VlValueType& req
) const
{
  return BitVector{mVal}.coerce(req);
}

// @brief 自身の複製を返す．
VlValueRep*
VlValueBitVector::duplicate() const
{
  return new VlValueBitVector{mVal};
}

END_NAMESPACE_YM_VERILOG
