#ifndef YM_VLVALUE_H
#define YM_VLVALUE_H

/// @file ym/VlValue.h
/// @brief VlValue のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym/verilog.h"
#include "ym/BitVector.h"
#include "ym/VlTime.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class VlValueRep VlValue.h "ym/VlValue.h"
/// @brief VlValue の実体を表す仮想クラス
///
/// ここではインターフェイスの定義のみを行っている．
//////////////////////////////////////////////////////////////////////
class VlValueRep
{
public:

  /// @brief 派生クラスの型
  enum class Type {
    INT,
    UINT,
    SCALAR,
    REAL,
    TIME,
    BITVECTOR,
    ERROR
  };


public:

  VlValueRep() = default;

  virtual
  ~VlValueRep() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // VlValueRep の継承クラスが実装しなければならない仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  virtual
  VlValueRep::Type
  type() const = 0;

  /// @brief 整数型に変換可能な時に true を返す．
  virtual
  bool
  is_int_compat() const = 0;

  /// @brief unsigned int 型に変換可能な時に true を返す．
  virtual
  bool
  is_uint_compat() const = 0;

  /// @brief 実数型に変換可能な時に true を返す．
  virtual
  bool
  is_real_compat() const = 0;

  /// @brief time 型に変換可能な時に true を返す．
  virtual
  bool
  is_time_compat() const = 0;

  /// @brief ビットベクタ型に変換可能な時に true を返す．
  virtual
  bool
  is_bitvector_compat() const = 0;

  /// @brief 整数型の値を返す．
  /// @note 値が整数型に変換できない時の値は不定
  virtual
  int
  int_value() const = 0;

  /// @brief unsigned int 型の値を返す．
  /// @note 値が整数型に変換できない時の値は不定
  virtual
  unsigned int
  uint_value() const = 0;

  /// @brief スカラー型の値を返す．
  virtual
  VlScalarVal
  scalar_value() const = 0;

  /// @brief 論理型の値を返す．
  virtual
  VlScalarVal
  logic_value() const = 0;

  /// @brief 実数型の値を返す．
  /// @note 値が整数型に変換できない時の値は不定
  virtual
  double
  real_value() const = 0;

  /// @brief time 型の値を返す．
  virtual
  VlTime
  time_value() const = 0;

  /// @brief ビットベクタ型の値を返す．
  virtual
  BitVector
  bitvector_value(
    const VlValueType& req_type ///< [in] 要求されるデータの型
  ) const = 0;

  /// @brief 自身の複製を返す．
  virtual
  VlValueRep*
  duplicate() const = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class VlValue VlValue.h "ym/VlValue.h"
/// @brief 値を表すクラス
///
/// 型に応じた VlValueRep の派生クラスが実際の値を保持する．
/// このクラスはそのオブジェクとへの shared_ptr を持つ．
//////////////////////////////////////////////////////////////////////
class VlValue
{
public:
  //////////////////////////////////////////////////////////////////////
  // 保持している値の種類
  //////////////////////////////////////////////////////////////////////

  using Type = VlValueRep::Type;


public:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ/デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief 空のコンストラクタ
  ///
  /// 値は不定
  VlValue();

  /// @brief コピーコンストラクタ
  VlValue(
    const VlValue& src ///< [in] コピー元のオブジェクト
  );

  /// @brief ムーブコンストラクタ
  VlValue(
    VlValue&& src ///< [in] ムーブ元のオブジェクト
  );

  /// @brief 整数値からのコンストラクタ
  explicit
  VlValue(
    int val ///< [in] 値
  );

  /// @brief unsined int からのコンストラクタ
  explicit
  VlValue(
    std::uint32_t val ///< [in] 値
  );

  /// @brief スカラー値からのコンストラクタ
  explicit
  VlValue(
    const VlScalarVal& val ///< [in] 値
  );

  /// @brief time からのコンストラクタ
  explicit
  VlValue(
    VlTime val ///< [in] 値
  );

  /// @brief 実数からのコンストラクタ
  explicit
  VlValue(
    double val ///< [in] 値
  );

  /// @brief ビットベクタからのコンストラクタ
  explicit
  VlValue(
    const BitVector& val ///< [in] 値
  );

  /// @brief 型変換を伴うコンストラクタ
  VlValue(
    const VlValue& src,           ///< [in] コピー元のオブジェクト
    const VlValueType& value_type ///< [in] 型
  );

  /// @brief 型変換を伴うコンストラクタ
  VlValue(
    VlValue&& src,                ///< [in] ムーブ元のオブジェクト
    const VlValueType& value_type ///< [in] 型
  );

  /// @brief コピー代入演算子
  VlValue&
  operator=(
    const VlValue& src ///< [in] コピー元のオブジェクト
  );

  /// @brief ムーブ代入演算子
  VlValue&
  operator=(
    VlValue&& src ///< [in] ムーブ元のオブジェクト
  );

  /// @brief デストラクタ
  ~VlValue();


public:
  //////////////////////////////////////////////////////////////////////
  // 値を設定する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 整数値を設定する．
  void
  set(
    int val ///< [in] 値
  );

  /// @brief unsigned int の値をセットする．
  void
  set(
    unsigned int val  ///< [in] 値
  );

  /// @brief スカラー値をセットする．
  void
  set(
    const VlScalarVal& val ///< [in] 値
  );

  /// @brief time の値をセットする．
  void
  set(
    VlTime val ///< [in] 値
  );

  /// @brief 実数値をセットする．
  void
  set(
    double val ///< [in] 値
  );

  /// @brief ビットベクタの値をセットする．
  void
  set(
    const BitVector& val ///< [in] 値
  );


public:
  //////////////////////////////////////////////////////////////////////
  // 値を取り出す関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  Type
  type() const { return mRep->type(); }

  /// @brief 整数型の時に true を返す．
  bool
  is_int() const { return type() == Type::INT; }

  /// @brief 符号なし整数型の時に true 返す．
  bool
  is_uint() const { return type() == Type::UINT; }

  /// @brief 実数型の時に true を返す．
  bool
  is_real() const { return type() == Type::REAL; }

  /// @brief スカラー型の時に true を返す．
  bool
  is_scalar() const { return type() == Type::SCALAR; }

  /// @brief time 型の時に true を返す．
  bool
  is_time() const { return type() == Type::TIME; }

  /// @brief ビットベクタ型の時に true を返す．
  bool
  is_bitvector() const { return type() == Type::BITVECTOR; }

  /// @brief エラー状態の時に true を返す．
  bool
  is_error() const { return type() == Type::ERROR; }

  /// @brief 整数型に変換可能な時に true を返す．
  bool
  is_int_compat() const { return mRep->is_int_compat(); }

  /// @brief unsigned uint 型に変換可能な時に true を返す．
  bool
  is_uint_compat() const { return mRep->is_uint_compat(); }

  /// @brief 実数型に変換可能な時に true を返す．
  bool
  is_real_compat() const { return mRep->is_real_compat(); }

  /// @brief time 型に変換可能な時に true を返す．
  bool
  is_time_compat() const { return mRep->is_time_compat(); }

  /// @brief ビットベクタ型に変換可能な時に true を返す．
  bool
  is_bitvector_compat() const { return mRep->is_bitvector_compat(); }

  /// @brief 符号付きの型の時に true を返す．
  bool
  is_signed() const
  {
    switch ( type() ) {
    case Type::INT:
    case Type::REAL:
      return true;

    case Type::UINT:
    case Type::SCALAR:
    case Type::TIME:
    case Type::ERROR:
      return false;

    case Type::BITVECTOR:
      return bitvector_value().is_signed();
    }
    ASSERT_NOT_REACHED;
    return false;
  }

  /// @brief ビットサイズを返す．
  SizeType
  bit_size() const
  {
    switch ( type() ) {
    case Type::INT:
    case Type::UINT:
      return kVpiSizeInteger;

    case Type::REAL:
      return kVpiSizeReal;

    case Type::SCALAR:
      return 1;

    case Type::TIME:
      return kVpiSizeTime;

    case Type::BITVECTOR:
      return bitvector_value().size();

    case Type::ERROR:
      return 0;
    }
    ASSERT_NOT_REACHED;
    return 0;
  }

  /// @brief 値の型を返す．
  VlValueType
  value_type() const
  {
    switch ( type() ) {
    case Type::INT:
      return VlValueType::int_type();

    case Type::UINT:
      return VlValueType(false, true, kVpiSizeInteger);

    case Type::REAL:
      return VlValueType::real_type();

    case Type::SCALAR:
      return VlValueType(false, true, 1);

    case Type::TIME:
      return VlValueType::time_type();

    case Type::BITVECTOR:
      return bitvector_value().value_type();

    case Type::ERROR:
      return VlValueType();
    }
    ASSERT_NOT_REACHED;
    return VlValueType();
  }

  /// @brief 整数型の値を返す．
  /// @note 値が整数型に変換できない時の値は不定
  int
  int_value() const { return mRep->int_value(); }

  /// @brief unsigned int 型の値を返す．
  /// @note 値が整数型に変換できない時の値は不定
  unsigned int
  uint_value() const { return mRep->uint_value(); }

  /// @brief スカラー型の値を返す．
  /// @note スカラー型には常に変換可能
  VlScalarVal
  scalar_value() const { return mRep->scalar_value(); }

  /// @brief 論理型の値を返す．
  VlScalarVal
  logic_value() const { return mRep->logic_value(); }

  /// @brief 実数型の値を返す．
  /// @note 値が実数型に変換できない時の値は不定
  double
  real_value() const { return mRep->real_value(); }

  /// @brief time 型の値を返す．
  /// @note 値が time 型に変換できない時の値は不定
  VlTime
  time_value() const { return mRep->time_value(); }

  /// @brief ビットベクタ型の値を返す．
  BitVector
  bitvector_value(
    const VlValueType& req_type = {} ///< [in] 要求されるデータの型
  ) const
  {
    return mRep->bitvector_value(req_type);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値を持つ実体
  shared_ptr<VlValueRep> mRep;

};


//////////////////////////////////////////////////////////////////////
// VlValue の演算
//////////////////////////////////////////////////////////////////////

/// @relates VlValue
/// @brief 単項のマイナス(complement)演算子
/// @return 2の補数を返す．
///
/// 2の補数を求める．
/// パタンとして2の補数を求めるだけで
/// 符号の有無は変わらない．
VlValue
operator-(
  const VlValue& src ///< [in] オペランド
);

/// @relates VlValue
/// @brief 加算
/// @return src1 + src2 を返す
VlValue
operator+(
  const VlValue& src1, ///< [in] 第1オペランド
  const VlValue& src2  ///< [in] 第2オペランド
);

/// @relates VlValue
/// @brief 減算
/// @return src1 - src2 を返す
VlValue
operator-(
  const VlValue& src1, ///< [in] 第1オペランド
  const VlValue& src2  ///< [in] 第2オペランド
);

/// @relates VlValue
/// @brief 乗算
/// @return src1 * src2 を返す
VlValue
operator*(
  const VlValue& src1, ///< [in] 第1オペランド
  const VlValue& src2  ///< [in] 第2オペランド
);

/// @relates VlValue
/// @brief 除算
/// @return src1 / src2 を返す
VlValue
operator/(
  const VlValue& src1, ///< [in] 第1オペランド
  const VlValue& src2  ///< [in] 第2オペランド
);

/// @relates VlValue
/// @brief 剰余算
/// @return src1 % src2 を返す
VlValue
operator%(
  const VlValue& src1, ///< [in] 第1オペランド
  const VlValue& src2  ///< [in] 第2オペランド
);

/// @relates VlValue
/// @brief 巾乗
/// @return src1 の src2 乗を返す
VlValue
power(
  const VlValue& src1, ///< [in] 第1オペランド
  const VlValue& src2  ///< [in] 第2オペランド
);

/// @}
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
/// @name Relational operators (IEEE1364-2001 4.1.7)
/// 結果が x になる場合があるので bool ではなく VlValue を返す．
///
/// 2つのオペランドのサイズが異なっていたら長い方に合わせる．
///
/// ともに符号付きの場合には符号付き数として比較する．
///
/// そうでなければ符号なし数として比較する．
///
/// 結果の型は以下のとおり
/// - 符号なし
/// - サイズあり(1)
/// - 基数は2
/// @{

/// @relates VlValue
/// @brief less than 比較演算
/// @retval 1 src1 < src2 の時
/// @retval 0 src1 >= src2 の時
/// @retval X 比較不能の時
VlValue
lt(
  const VlValue& src1, ///< [in] 第1オペランド
  const VlValue& src2  ///< [in] 第2オペランド
);

/// @relates VlValue
/// @brief less than 比較演算 (bool)
/// @return src1 < src2 を返す
bool
operator<(
  const VlValue& src1, ///< [in] 第1オペランド
  const VlValue& src2  ///< [in] 第2オペランド
);

/// @relates VlValue
/// @brief greater than 比較演算
/// @retval 1 src1 > src2 の時
/// @retval 0 src1 <= src2 の時
/// @retval X 比較不能の時
VlValue
gt(
  const VlValue& src1, ///< [in] 第1オペランド
  const VlValue& src2  ///< [in] 第2オペランド
);

/// @relates VlValue
/// @brief greater than 比較演算 (bool)
/// @return src1 > src2 を返す
bool
operator>(
  const VlValue& src1, ///< [in] 第1オペランド
  const VlValue& src2  ///< [in] 第2オペランド
);

/// @relates VlValue
/// @brief less than or equal 比較演算
/// @retval 1 src1 <= src2 の時
/// @retval 0 src1 > src2 の時
/// @retval X 比較不能の時
VlValue
le(
  const VlValue& src1, ///< [in] 第1オペランド
  const VlValue& src2  ///< [in] 第2オペランド
);

/// @relates VlValue
/// @brief less than or equal 比較演算 (bool)
/// @return src1 <= src2 を返す
bool
operator<=(
  const VlValue& src1, ///< [in] 第1オペランド
  const VlValue& src2  ///< [in] 第2オペランド
);

/// @relates VlValue
/// @brief greater than or equal 比較演算
/// @retval 1 src1 >= src2 の時
/// @retval 0 src1 < src2 の時
/// @retval X 比較不能の時
VlValue
ge(
  const VlValue& src1, ///< [in] 第1オペランド
  const VlValue& src2  ///< [in] 第2オペランド
);

/// @relates VlValue
/// @brief greater than or equal 比較演算 (bool)
/// @return src1 >= src2 を返す
bool
operator>=(
  const VlValue& src1, ///< [in] 第1オペランド
  const VlValue& src2  ///< [in] 第2オペランド
);

/// @}
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
/// @name Equality operators (IEEE1364-2001 4.1.8)
/// 結果が x になる場合があるので bool ではなく VlValue を返す．
///
/// 2つのオペランドのサイズが異なっていたら長い方に合わせる．
///
/// 結果の型は以下のとおり
/// - 符号なし
/// - サイズあり(1)
/// - 基数は2
/// @{

/// @relates VlValue
/// @brief 等価比較演算子
/// @retval 1 src1 == src2 の時
/// @retval 0 src1 != src2 の時
/// @retval X 比較不能の時
VlValue
eq(
  const VlValue& src1, ///< [in] 第1オペランド
  const VlValue& src2  ///< [in] 第2オペランド
);

/// @relates VlValue
/// @brief x が 0 および 1 と等価と見なせるとした場合の等価比較演算子
/// @return 等価と見なせるとき true を返す．
VlValue
eq_with_x(
  const VlValue& src1, ///< [in] 第1オペランド
  const VlValue& src2  ///< [in] 第2オペランド
);

/// @relates VlValue
/// @brief x および z が 0 および 1 と等価と見なせるとした場合の等価比較演算子
/// @return 等価と見なせるとき true を返す．
VlValue
eq_with_xz(
  const VlValue& src1, ///< [in] 第1オペランド
  const VlValue& src2  ///< [in] 第2オペランド
);

/// @relates VlValue
/// @brief 等価比較演算子 (bool)
/// @return 1 src1 == src2 を返す．
bool
operator==(
  const VlValue& src1, ///< [in] 第1オペランド
  const VlValue& src2  ///< [in] 第2オペランド
);

/// @relates VlValue
/// @brief 非等価比較演算子
/// @retval 1 src1 != src2 の時
/// @retval 0 src1 == src2 の時
/// @retval X 比較不能の時
VlValue
ne(
  const VlValue& src1, ///< [in] 第1オペランド
  const VlValue& src2  ///< [in] 第2オペランド
);

/// @relates VlValue
/// @brief 非等価比較演算子 (bool)
/// @retval 1 src1 != src2 を返す．
bool
operator!=(
  const VlValue& src1, ///< [in] 第1オペランド
  const VlValue& src2  ///< [in] 第2オペランド
);

/// @}
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
/// @name Logical operators (scalar) (IEEE1364-2001 4.1.9)
/// 結果が x になる場合があるので bool ではなく VlValue を返す．
///
/// オペランドがスカラーで無い場合には強制的にスカラーに変換する．
/// 具体的には最下位ビットをそのままスカラー値だと思う．
///
/// 結果の型は以下のとおり
/// - 符号なし
/// - サイズあり(1)
/// - 基数は2
/// @{

/// @relates VlValue
/// @brief NOT演算
/// @retval 0 src が真の時
/// @retval 1 src が偽の時
/// @retval X 計算不能の時
VlValue
log_not(
  const VlValue& src ///< [in] オペランド
);

/// @relates VlValue
/// @brief AND演算
/// @retval 0 src1 と src2 のどちらか一方が偽の時
/// @retval 1 src1 と src2 がともに真の時
/// @retval X 計算不能の時
VlValue
log_and(
  const VlValue& src1, ///< [in] 第1オペランド
  const VlValue& src2  ///< [in] 第2オペランド
);

/// @relates VlValue
/// @brief OR演算
/// @retval 0 src1 と src2 がともに偽の時
/// @retval 1 src1 と src2 のどちらか一方が真の時
/// @retval X 計算不能の時
VlValue
log_or(
  const VlValue& src1, ///< [in] 第1オペランド
  const VlValue& src2  ///< [in] 第2オペランド
);

/// @}
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
/// @name Bit-wise logical operators (IEEE1364-2001 4.1.10)
/// 2つのオペランドの長さが合わないときは長い方に合わせる．
/// この場合は常に0が埋められる．
///
/// 結果の型は以下のとおり
/// - 符号:
///   - ともに signed -> signed
///   - 上記以外      -> unsigned
/// - size:
///   - unsized のオペランドは integer とおなじとみなす．
///   - 2つのオペランドの size の大きい方に合わせる．
///   - ただし，どちらも unsized なら結果も unsized
/// - 基数は2
/// @{

/// @relates VlValue
/// @brief bitwise NOT 演算
/// @return src のビットごとに否定したもの
VlValue
bit_negate(
  const VlValue& src ///< [in] オペランド
);

/// @relates VlValue
/// @brief bitwise AND 演算
/// @return src1 と src2 をビットごとに AND したもの
VlValue
bit_and(
  const VlValue& src1, ///< [in] 第1オペランド
  const VlValue& src2  ///< [in] 第2オペランド
);

/// @relates VlValue
/// @brief bitwise OR 演算
/// @return src1 と src2 をビットごとに OR したもの
VlValue
bit_or(
  const VlValue& src1, ///< [in] 第1オペランド
  const VlValue& src2  ///< [in] 第2オペランド
);

/// @relates VlValue
/// @brief bitwise XOR 演算
/// @return src1 と src2 をビットごとに XOR したもの
VlValue
bit_xor(
  const VlValue& src1, ///< [in] 第1オペランド
  const VlValue& src2  ///< [in] 第2オペランド
);

/// @relates VlValue
/// @brief bitwise XNOR 演算
/// @return src1 と src2 をビットごとに XNOR したもの
VlValue
bit_xnor(
  const VlValue& src1, ///< [in] 第1オペランド
  const VlValue& src2  ///< [in] 第2オペランド
);

/// @}
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
/// @name Reduction operators (IEEE1364-2001 4.1.11)
/// 結果の型は以下のとおり
/// - 符号なし
/// - サイズあり(1)
/// - 基数は2
/// @{

/// @brief リダクションAND
/// @return 演算結果を返す．
///
/// すべてのビットの AND を計算する
VlValue
reduction_and(
  const VlValue& src ///< [in] オペランド
);

/// @brief リダクションOR
/// @return 演算結果を返す．
///
/// すべてのビットの OR を計算する
VlValue
reduction_or(
  const VlValue& src ///< [in] オペランド
);

/// @brief リダクションXOR
/// @return 演算結果を返す．
///
/// すべてのビットの XOR を計算する
VlValue
reduction_xor(
  const VlValue& src ///< [in] オペランド
);

/// @brief リダクションNAND
/// @return 演算結果を返す．
///
/// すべてのビットの NAND を計算する
VlValue
reduction_nand(
  const VlValue& src ///< [in] オペランド
);

/// @brief リダクションNOR
/// @return 演算結果を返す．
///
/// すべてのビットの NOR を計算する
VlValue
reduction_nor(
  const VlValue& src ///< [in] オペランド
);

/// @brief リダクションXNOR
/// @return 演算結果を返す．
///
/// すべてのビットの XNOR を計算する
VlValue
reduction_xnor(
  const VlValue& src ///< [in] オペランド
);

/// @}
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
/// @name Shift operators (IEEE1364-2001 4.1.12)
/// 結果の型は以下のとおり
/// - 符号   : 第1オペランドと同じ
/// - サイズ : 第1オペランドと同じ
/// - 基数   : 第1オペランドと同じ
/// @{

/// @relates VlValue
/// @brief 論理左シフト
/// @return src1 を src2 だけ論理左シフトしたもの
VlValue
operator<<(
  const VlValue& src1, ///< [in] 元の値
  const VlValue& src2  ///< [in] シフト量 (VlValue)
);

/// @relates VlValue
/// @brief 論理左シフト
/// @return src1 を src2 だけ論理左シフトしたもの
VlValue
operator<<(
  const VlValue& src1, ///< [in] 元の値
  std::uint32_t src2        ///< [in] シフト量 (std::uint32_t)
);

/// @relates VlValue
/// @brief 論理右シフト
/// @return src1 を src2 だけ論理右シフトしたもの
VlValue
operator>>(
  const VlValue& src1, ///< [in] 元の値
  const VlValue& src2  ///< [in] シフト量 (VlValue)
);

/// @relates VlValue
/// @brief 論理右シフト
/// @return src1 を src2 だけ論理右シフトしたもの
VlValue
operator>>(
  const VlValue& src1, ///< [in] 元の値
  std::uint32_t src2        ///< [in] シフト量 (std::uint32_t)
);

/// @relates VlValue
/// @brief 算術左シフト
/// @return src1 を src2 だけ算術左シフトしたもの
/// 算術左シフト
VlValue
alshift(
  const VlValue& src1, ///< [in] 元の値
  const VlValue& src2  ///< [in] シフト量 (VlValue)
);

/// @relates VlValue
/// @brief 算術左シフト
/// @return src1 を src2 だけ算術左シフトしたもの
VlValue
alshift(
  const VlValue& src1, ///< [in] 元の値
  std::uint32_t src2        ///< [in] シフト量 (std::uint32_t)
);

/// @relates VlValue
/// @brief 算術右シフト
/// @return src1 を src2 だけ算術右シフトしたもの
VlValue
arshift(
  const VlValue& src1, ///< [in] 元の値
  const VlValue& src2  ///< [in] シフト量 (VlValue)
);

/// @relates VlValue
/// @brief 算術右シフト
/// @return src1 を src2 だけ算術右シフトしたもの
VlValue
arshift(
  const VlValue& src1, ///< [in] 元の値
  std::uint32_t src2        ///< [in] シフト量 (std::uint32_t)
);

/// @}
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
/// @name Conditional operators (IEEE1364-2001 4.1.13)
/// 第1オペランドは強制的にスカラーに変換される．
///
/// 結果の型は以下のとおり
/// - 符号:
///   - 第2,第3オペランドがともに signed -> signed
///   - 上記以外 -> unsigned
/// - サイズ:
///   - 第2,第3オペランドの長い方に合わせる．
///   - ただし unsized なら integer と同じと見なす．
///   - ともに unsized なら 結果も unsized
/// - 基数:
///   - 第1オペランドが 1 の時には第2オペランドの基数
///   - 第1オペランドが 0 の時には第3オペランドの基数
///   - 第1オペランドが x, z の時に第2オペランドと
///     第3オペランドの基数が異なっていたら2とする．
/// @{

/// @relates VlValue
/// @brief 条件演算
/// @return 演算結果
VlValue
ite(
  const VlValue& src1, ///< [in] 条件
  const VlValue& src2, ///< [in] src1 が真の時に選ばれる値
  const VlValue& src3  ///< [in] src1 が偽の時に選ばれる値
);

/// @relates VlValue
/// @brief 条件演算
/// @return 演算結果
VlValue
ite(
  const VlScalarVal& src1, ///< [in] 条件
  const VlValue& src2,     ///< [in] src1 が真の時に選ばれる値
  const VlValue& src3      ///< [in] src1 が偽の時に選ばれる値
);

/// @}
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
/// @name Concatenation (IEEE1364-2001 4.1.14)
/// 結果の型は以下の通り
/// - 符号: 常に unsigned
/// - サイズ: 連結するオペランドのサイズの和．
///           もしもオペランドに unsized な値が含まれていたらエラー
///           (今はintegerと見なす)
/// - 基数: 常に2
/// @{

/// @relates VlValue
/// @brief 連結演算
/// @return 連結した結果を返す．
VlValue
concat(
  const vector<VlValue>& src_list ///< [in] 連結する値のリスト
);

/// @relates VlValue
/// @brief 繰り返し連結演算
/// @return src_list[1~n-1] の内容を src_list[0] 回繰り返して連結したもの
VlValue
multi_concat(
  const vector<VlValue>& src_list ///< [in] 連結する値のリスト
);

/// @}
//////////////////////////////////////////////////////////////////////


END_NAMESPACE_YM_VERILOG

#endif // YM_VLVALUE_H
