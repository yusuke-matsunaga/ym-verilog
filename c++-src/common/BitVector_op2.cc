
/// @file BitVector.cc
/// @brief BitVector の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym/BitVector.h"


BEGIN_NAMESPACE_YM_VERILOG

BEGIN_NONAMESPACE

using uword = std::uint64_t;

/// @brief すべてが0のパタン
const uword ALL0 = 0x0000000000000000;

/// @brief すべてが1のパタン
const uword ALL1 = 0xFFFFFFFFFFFFFFFF;

const SizeType BLOCK_SIZE = sizeof(uword) * 8;

// @brief ビット長からブロック数を得る．
inline
SizeType
block(SizeType size)
{
  return (size + BLOCK_SIZE - 1) / BLOCK_SIZE;
}

// @brief ビット長から最後のブロックのシフト数を得る．
inline
SizeType
shift(SizeType size)
{
  return size - (block(size) - 1) * BLOCK_SIZE;
}

// @brief size からマスクパタンを作る
inline
uword
mask(SizeType size)
{
  return ALL1 >> (BLOCK_SIZE - shift(size));
}

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// 4.1.9 Logical operators (scalar)
// 結果が x になる場合があるので bool ではなく BitVector を返す．
// オペランドがスカラーで無い場合には強制的にスカラーに変換する．
// 具体的には最下位ビットをそのままスカラー値だと思う．
// 結果の型は以下のとおり
// - 符号なし
// - サイズあり(1)
// - 基数は2
//////////////////////////////////////////////////////////////////////

// 否定
VlScalarVal
operator!(const BitVector& src)
{
  return !src.to_logic();
}

// 論理積
VlScalarVal
operator&&(const BitVector& src1,
	   const BitVector& src2)
{
  VlScalarVal v1 = src1.to_logic();
  VlScalarVal v2 = src2.to_logic();
  return v1 && v2;
}

// 論理和
VlScalarVal
operator||(const BitVector& src1,
	   const BitVector& src2)
{
  VlScalarVal v1 = src1.to_logic();
  VlScalarVal v2 = src2.to_logic();
  return v1 || v2;
}


//////////////////////////////////////////////////////////////////////
// 4.1.10 Bit-wise operators
// 2つのオペランドの長さが合わないときは長い方に合わせる．
// この場合は常に0が埋められる．
// 結果の型は以下のとおり
// - 符号: ともに signed -> signed
//         上記以外      -> unsigned
// - size: unsized のオペランドは integer とおなじとみなす．
//         で，2つのオペランドの size の大きい方に合わせる．
//         ただし，どちらも unsized なら結果も unsized
// - 基数は2
//////////////////////////////////////////////////////////////////////

// 自分自身の値をビットごとに反転させる
const BitVector&
BitVector::negate()
{
  // 基本的には val0 と val1 を交換すればよいが，
  // z を含んでいたときが例外となる．
  // z のビットは x にするのでともに1と立てる
  SizeType n = block(size());
  for ( SizeType i = 0; i < n; ++ i ) {
    uword pat0 = mVal0.get()[i];
    uword pat1 = mVal1.get()[i];
    uword zpat = ~pat0 & ~pat1; // 1 なら Z
    mVal0.get()[i] = pat1 | zpat;
    mVal1.get()[i] = pat0 | zpat;
  }
  return *this;
}

// 論理積つき代入
const BitVector&
BitVector::operator&=(const BitVector& src)
{
  SizeType ans_size = size();
  if ( ans_size < src.size() ) {
    ans_size = src.size();
  }
  bool ans_sized = is_sized() || src.is_sized();
  bool ans_signed = is_signed() && src.is_signed();
  int ans_base = base();
  if ( ans_base != src.base() ) {
    // てきとー
    ans_base = 2;
  }

  if ( size() < ans_size ) {
    set(mVal0.get(), mVal1.get(), size(), ans_size, ans_sized, ans_signed, ans_base);
  }

  if ( src.size() < ans_size ) {
    return operator&=(BitVector(src, ans_size));
  }

  set_type(ans_sized, ans_signed, ans_base);

  // 基本的には val0 を or して val1 を and すればよいが，
  // z を含んでいたときが例外となる．
  SizeType n = block(size());
  for ( SizeType i = 0; i < n; ++ i ) {
    uword val1_0 = mVal0.get()[i];
    uword val1_1 = mVal1.get()[i];
    uword zpat;
    // z のパタンを x に直しておく．
    zpat = ~val1_0 & ~val1_1;
    val1_0 |= zpat;
    val1_1 |= zpat;
    uword val2_0 = src.mVal0.get()[i];
    uword val2_1 = src.mVal1.get()[i];
    // z のパタンを x に直しておく．
    zpat = ~val2_0 & ~val2_1;
    val2_0 |= zpat;
    val2_1 |= zpat;

    mVal0.get()[i] = val1_0 | val2_0;
    mVal1.get()[i] = val1_1 & val2_1;
  }

  return *this;
}

// 論理和つき代入
const BitVector&
BitVector::operator|=(const BitVector& src)
{
  SizeType ans_size = size();
  if ( ans_size < src.size() ) {
    ans_size = src.size();
  }
  bool ans_sized = is_sized() || src.is_sized();
  bool ans_signed = is_signed() && src.is_signed();
  int ans_base = base();
  if ( ans_base != src.base() ) {
    // てきとー
    ans_base = 2;
  }

  if ( size() < ans_size ) {
    set(mVal0.get(), mVal1.get(), size(), ans_size, ans_sized, ans_signed, ans_base);
  }

  if ( src.size() < ans_size ) {
    return operator|=(BitVector(src, ans_size));
  }

  set_type(ans_sized, ans_signed, ans_base);

  // 基本的には val0 を and して val1 を or すればよいが，
  // z を含んでいたときが例外となる．
  SizeType n = block(size());
  for ( SizeType i = 0; i < n; ++ i ) {
    uword val1_0 = mVal0.get()[i];
    uword val1_1 = mVal1.get()[i];
    uword zpat;
    // z のパタンを x に直しておく．
    zpat = ~val1_0 & ~val1_1;
    val1_0 |= zpat;
    val1_1 |= zpat;
    uword val2_0 = src.mVal0.get()[i];
    uword val2_1 = src.mVal1.get()[i];
    // z のパタンを x に直しておく．
    zpat = ~val2_0 & ~val2_1;
    val2_0 |= zpat;
    val2_1 |= zpat;

    mVal0.get()[i] = val1_0 & val2_0;
    mVal1.get()[i] = val1_1 | val2_1;
  }

  return *this;
}

// 排他的論理和
const BitVector&
BitVector::operator^=(const BitVector& src)
{
  SizeType ans_size = size();
  if ( ans_size < src.size() ) {
    ans_size = src.size();
  }
  bool ans_sized = is_sized() || src.is_sized();
  bool ans_signed = is_signed() && src.is_signed();
  int ans_base = base();
  if ( ans_base != src.base() ) {
    // てきとー
    ans_base = 2;
  }

  if ( size() < ans_size ) {
    set(mVal0.get(), mVal1.get(), size(), ans_size, ans_sized, ans_signed, ans_base);
  }

  if ( src.size() < ans_size ) {
    return operator^=(BitVector(src, ans_size));
  }

  set_type(ans_sized, ans_signed, ans_base);

  // 計算の仕方は否定とANDとORを組み合わせたもの
  SizeType n = block(size());
  for ( SizeType i = 0; i < n; ++ i ) {
    uword val1_0 = mVal0.get()[i];
    uword val1_1 = mVal1.get()[i];
    uword zpat;
    // z のパタンを x に直しておく．
    zpat = ~val1_0 & ~val1_1;
    val1_0 |= zpat;
    val1_1 |= zpat;
    uword val2_0 = mVal0.get()[i];
    uword val2_1 = mVal1.get()[i];
    // z のパタンを x に直しておく．
    zpat = ~val2_0 & ~val2_1;
    val2_0 |= zpat;
    val2_1 |= zpat;

    mVal0.get()[i] = (val1_0 | val2_1) & (val1_1 | val2_0);
    mVal1.get()[i] = (val1_1 & val2_0) | (val1_0 & val2_1);
  }

  return *this;
}


//////////////////////////////////////////////////////////////////////
// 4.1.11 Reduction operators
// 結果の型は以下のとおり
// - 符号なし
// - サイズあり(1)
// - 基数は2
//////////////////////////////////////////////////////////////////////

// リダクションAND
VlScalarVal
BitVector::reduction_and() const
{
  if ( has_xz() ) {
    return VlScalarVal::x();
  }

  SizeType n = block(size());
  uword m = mask(size());
  for ( SizeType i = 0; i < n - 1; ++ i ) {
    if ( mVal0.get()[i] ) {
      // 1 ビットでも 0 のビットがあれば結果は0
      return VlScalarVal::zero();
    }
  }
  if ( mVal0.get()[n - 1] & m ) {
    // 1 ビットでも 0 のビットがあれば結果は0
    return VlScalarVal::zero();
  }
  return VlScalarVal::one();
}

// リダクションNAND
VlScalarVal
BitVector::reduction_nand() const
{
  if ( has_xz() ) {
    return VlScalarVal::x();
  }

  SizeType n = block(size());
  uword m = mask(size());
  for ( SizeType i = 0; i < n - 1; ++ i ) {
    if ( mVal0.get()[i] ) {
      // 1 ビットでも 0 のビットがあれば結果は1
      return VlScalarVal::one();
    }
  }
  if ( mVal0.get()[n - 1] & m ) {
    // 1 ビットでも 0 のビットがあれば結果は1
    return VlScalarVal::one();
  }
  return VlScalarVal::zero();
}

// リダクションOR
VlScalarVal
BitVector::reduction_or() const
{
  if ( has_xz() ) {
    return VlScalarVal::x();
  }
  SizeType n = block(size());
  uword m = mask(size());
  for ( SizeType i = 0; i < n - 1; ++ i ) {
    if ( mVal1.get()[i] ) {
      // 1 ビットでも 1 のビットがあれば結果は1
      return VlScalarVal::one();
    }
  }
  if ( mVal1.get()[n - 1] & m ) {
    // 1 ビットでも 1 のビットがあれば結果は1
    return VlScalarVal::one();
  }
  return VlScalarVal::zero();
}

// リダクションNOR
VlScalarVal
BitVector::reduction_nor() const
{
  if ( has_xz() ) {
    return VlScalarVal::x();
  }
  SizeType n = block(size());
  uword m = mask(size());
  for ( SizeType i = 0; i < n - 1; ++ i ) {
    if ( mVal1.get()[i] ) {
      // 1 ビットでも 1 のビットがあれば結果は0
      return VlScalarVal::zero();
    }
  }
  if ( mVal1.get()[n - 1] & m ) {
    // 1 ビットでも 1 のビットがあれば結果は0
    return VlScalarVal::zero();
  }
  return VlScalarVal::one();
}

// リダクションXOR
VlScalarVal
BitVector::reduction_xor() const
{
  if ( has_xz() ) {
    return VlScalarVal::x();
  }
  SizeType n = block(size());
  // xor は地道に数えるしかない
  // 高速に行うには8ビット単位ぐらいで表引きをすればよい．
  uword v = 0;
  for ( SizeType i = 0; i < n - 1; ++ i ) {
    for ( int b = 0; b < BLOCK_SIZE; b ++ ) {
      if ( (mVal1.get()[i] >> b) & 1 ) {
	v ^= 1;
      }
    }
  }
  SizeType s = shift(size());
  for ( SizeType b = 0; b < s; b ++ ) {
    if ( (mVal1.get()[n - 1] >> b) & 1 ) {
      v ^= 1;
    }
  }
  if ( v ) {
    return VlScalarVal::one();
  }
  else {
    return VlScalarVal::zero();
  }
}

// リダクションXNOR
VlScalarVal
BitVector::reduction_xnor() const
{
  if ( has_xz() ) {
    return VlScalarVal::x();
  }
  SizeType n = block(size());
  // xor は地道に数えるしかない
  // 高速に行うには8ビット単位ぐらいで表引きをすればよい．
  uword v = 0;
  for ( SizeType i = 0; i < n - 1; ++ i ) {
    for ( SizeType b = 0; b < BLOCK_SIZE; b ++ ) {
      if ( (mVal1.get()[i] >> b) & 1 ) {
	v ^= 1;
      }
    }
  }
  SizeType s = shift(size());
  for ( SizeType b = 0; b < s; ++ b ) {
    if ( (mVal1.get()[n - 1] >> b) & 1 ) {
      v ^= 1;
    }
  }
  if ( v ) {
    return VlScalarVal::zero();
  }
  else {
    return VlScalarVal::one();
  }
}


//////////////////////////////////////////////////////////////////////
// 4.1.12 Shift operators
// 結果の型は以下のとおり
// - 符号   : 第1オペランドと同じ
// - サイズ : 第1オペランドと同じ
// - 基数   : 第1オペランドと同じ
//////////////////////////////////////////////////////////////////////

// 論理左シフトつき代入
const BitVector&
BitVector::operator<<=(const BitVector& src)
{
  if ( src.has_xz() ) {
    return *this = BitVector::x(size());
  }

  if ( !src.is_uint32() ) {
    cerr << "error in operator<<: right operand too big" << endl;
    return *this = BitVector::x(size());
  }

  return operator<<=(src.to_uint32());
}

// 論理左シフトつき代入
// src2 が int のバージョン
const BitVector&
BitVector::operator<<=(int sft)
{
  if ( sft == 0 ) {
    // シフト量が0のときはそのまま返す
    return *this;
  }

  SizeType n = block(size());
  SizeType bit_sft = sft % BLOCK_SIZE; // ブロック内のシフト量
  SizeType blk_sft = sft / BLOCK_SIZE; // ブロック単位のシフト量
  SizeType rbit_sft = BLOCK_SIZE - bit_sft;
  SizeType en = n - blk_sft;

  if ( bit_sft == 0 ) {
    // この向きでコピーしないと破壊してしまう．
    for ( int i = en; i -- > 0; ) {
      mVal0.get()[i + blk_sft] = mVal0.get()[i];
      mVal1.get()[i + blk_sft] = mVal1.get()[i];
    }
  }
  else {
    for ( int i = en; i -- > 0; ) {
      uword lval0 = (mVal0.get()[i] << bit_sft);
      uword lval1 = (mVal1.get()[i] << bit_sft);
      uword rval0 = ((i > 0) ? mVal0.get()[i - 1] : ALL1) >> rbit_sft;
      uword rval1 = ((i > 0) ? mVal1.get()[i - 1] : ALL0) >> rbit_sft;
      mVal0.get()[i + blk_sft] = lval0 | rval0;
      mVal1.get()[i + blk_sft] = lval1 | rval1;
    }
  }
  // 空のブロックを詰める
  for ( SizeType i = 0; i < blk_sft; ++ i ) {
    mVal0.get()[i] = ALL1;
    mVal1.get()[i] = ALL0;
  }

  return *this;
}

// 論理右シフトつき代入
const BitVector&
BitVector::operator>>=(const BitVector& src)
{
  if ( src.has_xz() ) {
    return *this = BitVector::x(size());
  }

  if ( !src.is_uint32() ) {
    cerr << "error in operator>>: right operand too big" << endl;
    return *this = BitVector::x(size());
  }

  return operator>>=(src.to_uint32());
}

// 論理右シフトつき代入
// src2 が ind のバージョン
const BitVector&
BitVector::operator>>=(int sft)
{
  if ( sft == 0 ) {
    // シフト量が0の時はそのまま返す．
    return *this;
  }

  SizeType n = block(size());
  SizeType bit_sft = sft % BLOCK_SIZE; // ブロック内のシフト量
  SizeType blk_sft = sft / BLOCK_SIZE; // ブロック単位のシフト量
  SizeType lbit_sft = BLOCK_SIZE - bit_sft;

  if ( bit_sft == 0 ) {
    for ( SizeType i = blk_sft; i < n; ++ i ) {
      mVal0.get()[i - blk_sft] = mVal0.get()[i];
      mVal1.get()[i - blk_sft] = mVal1.get()[i];
    }
  }
  else {
    for ( SizeType i = blk_sft; i < n; ++ i ) {
      uword r0 = mVal0.get()[i] >> bit_sft;
      uword r1 = mVal1.get()[i] >> bit_sft;
      uword l0 = ((i < n - 1) ? mVal0.get()[i + 1] : ALL1) << lbit_sft;
      uword l1 = ((i < n - 1) ? mVal1.get()[i + 1] : ALL0) << lbit_sft;
      mVal0.get()[i - blk_sft] = l0 | r0;
      mVal1.get()[i - blk_sft] = l1 | r1;
    }
  }
  // 上位ビットに0を詰めておく．
  for ( SizeType i = n - blk_sft; i < n; ++ i ) {
    mVal0.get()[i] = ALL1;
    mVal1.get()[i] = ALL0;
  }
  return *this;
}

// 算術右シフトつき代入
const BitVector&
BitVector::arshift(const BitVector& src)
{
  if ( src.has_xz() ) {
    return *this = BitVector::x(size());
  }

  if ( !src.is_uint32() ) {
    cerr << "error in operator<<: right operand too big" << endl;
    return *this = BitVector::x(size());
  }

  return arshift(src.to_uint32());
}

// 算術右シフトつき代入
// src が int のバージョン
const BitVector&
BitVector::arshift(int sft)
{
  if ( sft == 0 ) {
    // シフト量が0の時はそのまま返す．
    return *this;
  }

  SizeType n = block(size());
  SizeType bit_sft = sft % BLOCK_SIZE; // ブロック内のシフト量
  SizeType blk_sft = sft / BLOCK_SIZE; // ブロック単位のシフト量
  SizeType lbit_sft = BLOCK_SIZE - bit_sft;

  uword pad0 = ALL1;
  uword pad1 = ALL0;
  if ( is_negative() ) {
    pad0 = ALL0;
    pad1 = ALL1;
  }

  if ( bit_sft == 0 ) {
    for ( SizeType i = blk_sft; i < n; ++ i ) {
      mVal0.get()[i - blk_sft] = mVal0.get()[i];
      mVal1.get()[i - blk_sft] = mVal1.get()[i];
    }
  }
  else {
    for ( SizeType i = blk_sft; i < n; ++ i ) {
      uword r0 = mVal0.get()[i] >> bit_sft;
      uword r1 = mVal1.get()[i] >> bit_sft;
      uword l0 = ((i < n - 1) ? mVal0.get()[i + 1] : pad0) << lbit_sft;
      uword l1 = ((i < n - 1) ? mVal1.get()[i + 1] : pad1) << lbit_sft;
      mVal0.get()[i - blk_sft] = l0 | r0;
      mVal1.get()[i - blk_sft] = l1 | r1;
    }
  }
  for ( SizeType i = n - blk_sft; i < n; ++ i ) {
    mVal0.get()[i] = pad0;
    mVal1.get()[i] = pad1;
  }
  return *this;
}


//////////////////////////////////////////////////////////////////////
// 4.1.13 Conditional operators
// 第1オペランドは強制的にスカラーに変換される．
// 結果の型は以下のとおり
// - 符号   : 第2,第3オペランドがともに signed -> signed
//            上記以外 -> unsigned
// - サイズ : 第2,第3オペランドの長い方に合わせる．
//            ただし unsized なら integer と同じと見なす．
//            ともに unsized なら 結果も unsized
// - 基数   : 第1オペランドが 1 の時には 第2オペランドの基数
//            0 の時には第3オペランドの基数
//            x, z の時に第2オペランドと第3オペランドの基数が異
//            なっていたら2とする．
//////////////////////////////////////////////////////////////////////

// 条件演算
BitVector
ite(const BitVector& src1,
    const BitVector& src2,
    const BitVector& src3)
{
  bool has_size = src1.is_sized() || src2.is_sized();
  SizeType size = src1.size();
  if ( size < src2.size() ) {
    size = src2.size();
  }
  bool has_sign = src1.is_signed() && src2.is_signed();
  VlScalarVal lval = src1.to_logic();
  if ( lval.is_zero() ) {
    return BitVector(src3, size, has_size, has_sign, src3.base());
  }
  if ( lval.is_one() ) {
    return BitVector(src2, size, has_size, has_sign, src2.base());
  }
  return BitVector(src2).merge(src3);
}


//////////////////////////////////////////////////////////////////////
// 4.1.14 Concatenation
// 結果の型は以下の通り
// - 符号   : 常に unsigned
// - サイズ : オペランドのサイズの和．もしもオペランドに unsized
//            な値が含まれていたらエラー(今はintegerと見なす)
// - 基数   : 2
//////////////////////////////////////////////////////////////////////

// @brief 連結演算
// @param[in] src_list 連結する値のリスト
// @return 連結した値
BitVector
concat(const vector<BitVector>& src_list)
{
  return BitVector(src_list);
}

// @brief 繰り返し連結演算
// @param[in] rep 繰り返し数
// @param[in] src_list 連結する値のリスト
// @return 連結した値
BitVector
multi_concat(const BitVector& rep,
	     const vector<BitVector>& src_list)
{
  if ( !rep.is_uint32() ) {
    // 手抜き
    return BitVector::x();
  }

  // まず src_list の連結を行う．
  BitVector unit(src_list);

  // それを rep 回繰り返す．
  std::uint32_t n = rep.to_uint32();
  vector<BitVector> tmp_list(n, unit);
  return BitVector(tmp_list);
}


//////////////////////////////////////////////////////////////////////
// 4.2.1 Vector bit-select and part-select addressing
// 範囲外は x を返す．
// 結果の型は以下の通り
// - 符号   : 常に unsigned
// - サイズ : 指定された値
// - 基数   : 2
//////////////////////////////////////////////////////////////////////

// part-select 演算子
BitVector
BitVector::part_select_op(int msb,
			  int lsb) const
{
  if ( msb < lsb ) {
    // 狂ってる
    return BitVector::x();
  }

  int new_size = msb - lsb + 1;
  if ( lsb >= static_cast<int>(size()) || msb < 0 ) {
    // 完全に範囲外
    return BitVector::x(new_size);
  }

  BitVector ans(VlScalarVal::x(), new_size);
  // とりあえず参照コードということでビット単位に転送してます．
  // 本当は part_select_op 書き込みのようなブロック転送のコードの方が速い
  SizeType start = 0;
  SizeType end = new_size - 1;
  if ( lsb < 0 ) {
    start = -lsb;
  }
  if ( msb >= static_cast<int>(size()) ) {
    end -= msb - size() + 1;
  }
  for ( SizeType i = start; i <= end; ++ i ) {
    ans.bit_select_op(i, value(i + lsb));
  }
  return ans;
}

// part-select 書き込み
// [msb:lsb] の範囲に val を書き込む
// 範囲外ならなにもしない．
void
BitVector::part_select_op(int msb,
			  int lsb,
			  const BitVector& val)
{
  if ( msb < lsb ) {
    // 狂ってる．
    return;
  }

  SizeType l = msb - lsb + 1;
  SizeType src_blk = block(l);
  uword src_mask = mask(l);

  uword blk0 = lsb / BLOCK_SIZE;
  uword sft0 = lsb - blk0 * BLOCK_SIZE;

  if ( sft0 == 0 ) {
    for ( SizeType i = 0; i < src_blk - 1; ++ i ) {
      mVal0.get()[i + blk0] = val.mVal0.get()[i];
      mVal1.get()[i + blk0] = val.mVal1.get()[i];
    }
    mVal0.get()[src_blk + blk0 - 1] &= ~src_mask;
    mVal0.get()[src_blk + blk0 - 1] |=  src_mask & val.mVal0.get()[src_blk - 1];
    mVal1.get()[src_blk + blk0 - 1] &= ~src_mask;
    mVal1.get()[src_blk + blk0 - 1] |=  src_mask & val.mVal1.get()[src_blk - 1];
  }
  else {
    if ( src_blk == 1 ) {
      uword val0 = val.mVal0.get()[0] & src_mask;
      uword val1 = val.mVal1.get()[0] & src_mask;
      uword lval0 = val0 << sft0;
      uword rval0 = val0 >> (BLOCK_SIZE - sft0);
      uword lval1 = val1 << sft0;
      uword rval1 = val1 >> (BLOCK_SIZE - sft0);
      uword lsrc_mask = src_mask << sft0;
      uword rsrc_mask = src_mask >> (BLOCK_SIZE - sft0);
      mVal0.get()[blk0] &= ~lsrc_mask;
      mVal0.get()[blk0] |=  lval0;
      mVal1.get()[blk0] &= ~lsrc_mask;
      mVal1.get()[blk0] |=  lval1;
      if ( rsrc_mask ) {
	mVal0.get()[blk0 + 1] &= ~rsrc_mask;
	mVal0.get()[blk0 + 1] |=  rval0;
	mVal1.get()[blk0 + 1] &= ~rsrc_mask;
	mVal1.get()[blk0 + 1] |=  rval1;
      }
    }
    else {
      uword val0 = val.mVal0.get()[0];
      uword val1 = val.mVal1.get()[0];
      uword lval0 = val0 << sft0;
      uword rval0 = val0 >> (BLOCK_SIZE - sft0);
      uword lval1 = val1 << sft0;
      uword rval1 = val1 >> (BLOCK_SIZE - sft0);
      uword lmask = ALL1 << sft0;
      uword rmask = ALL1 >> (BLOCK_SIZE - sft0);
      mVal0.get()[blk0] &= ~lmask;
      mVal0.get()[blk0] |= lval0;
      mVal1.get()[blk0] &= ~lmask;
      mVal1.get()[blk0] |= lval1;
      for ( SizeType i = 1; i < src_blk - 1; ++ i ) {
	val0 = val.mVal0.get()[i];
	val1 = val.mVal1.get()[i];
	lval0 = val0 << sft0;
	lval1 = val1 << sft0;
	mVal0.get()[blk0 + i] = lval0 | rval0;
	mVal1.get()[blk0 + i] = lval1 | rval1;
	rval0 = val0 >> (BLOCK_SIZE - sft0);
	rval1 = val1 >> (BLOCK_SIZE - sft0);
      }
      lval0 = (val.mVal0.get()[src_blk - 1] & src_mask) << sft0;
      lval1 = (val.mVal1.get()[src_blk - 1] & src_mask) << sft0;
      uword lsrc_mask = src_mask << sft0;
      mVal0.get()[blk0 + src_blk - 1] &= ~(lsrc_mask | rmask);
      mVal0.get()[blk0 + src_blk - 1] |= lval0 | rval0;
      mVal1.get()[blk0 + src_blk - 1] &= ~(lsrc_mask | rmask);
      mVal1.get()[blk0 + src_blk - 1] |= lval1 | rval1;
    }
  }
}

// bit-select 書き込み
// bpos の位置に val を書き込む
// 範囲外ならなにもしない．
void
BitVector::bit_select_op(int bpos,
			 VlScalarVal val)
{
  if ( bpos < 0 || bpos >= static_cast<int>(size()) ) {
    return;
  }

  SizeType blk = bpos / BLOCK_SIZE;
  SizeType sft = bpos - blk * BLOCK_SIZE;
  uword msk = 1 << sft;
  if ( val.is_zero() ) {
    mVal0.get()[blk] |= msk;
    mVal1.get()[blk] &= ~msk;
  }
  else if ( val.is_one() ) {
    mVal0.get()[blk] &= ~msk;
    mVal1.get()[blk] |= msk;
  }
  else if ( val.is_x() ) {
    mVal0.get()[blk] |= msk;
    mVal1.get()[blk] |= msk;
  }
  else if ( val.is_z() ) {
    mVal0.get()[blk] &= ~msk;
    mVal1.get()[blk] &= ~msk;
  }
  else {
    ASSERT_NOT_REACHED;
  }
}


//////////////////////////////////////////////////////////////////////
// その他のメンバ関数
//////////////////////////////////////////////////////////////////////

// srcの値をビットごとにマージする．
// 異なっているビットは X となる．
const BitVector&
BitVector::merge(const BitVector& src)
{
  SizeType ans_size = size();
  if ( ans_size < src.size() ) {
    ans_size = src.size();
  }
  bool ans_sized = is_sized() || src.is_sized();
  bool ans_signed = is_signed() && src.is_signed();
  int ans_base = base();
  if ( ans_base != src.base() ) {
    ans_base = 2;
  }
  if ( size() < ans_size ) {
    set(mVal0.get(), mVal1.get(), size(), ans_size, ans_sized, ans_signed, ans_base);
  }
  if ( src.size() < ans_size ) {
    return merge(BitVector(src, ans_size));
  }

  set_type(ans_sized, ans_signed, ans_base);

  SizeType n = block(size());
  for ( SizeType i = 0; i < n; ++ i ) {
    uword val1_0 = mVal0.get()[i];
    uword val1_1 = mVal1.get()[i];
    uword val2_0 = src.mVal0.get()[i];
    uword val2_1 = src.mVal1.get()[i];
    // z を x に変換する
    uword zpat1 = ~val1_0 & ~val1_1;
    val1_0 |= zpat1;
    val1_1 |= zpat1;
    uword zpat2 = ~val2_0 & ~val2_1;
    val2_0 |= zpat2;
    val2_1 |= zpat2;
    mVal0.get()[i] = val1_0 | val2_0;
    mVal1.get()[i] = val1_1 | val2_1;
  }

  return *this;
}

END_NAMESPACE_YM_VERILOG
