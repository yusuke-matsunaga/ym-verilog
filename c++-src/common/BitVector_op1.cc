
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

// @brief a * b の結果を2ワードで返す．
// - <ret-val>.first が high-word
// - <ret-val>.second が low-word
pair<uword, uword>
mult32(uword a,
       uword b)
{
  uword al = a & 0xffff;
  uword ah = a >> 16;
  uword bl = b & 0xffff;
  uword bh = b >> 16;

  uword tmp_ll = al * bl;
  uword tmp_lh = al * bh;
  uword tmp_hl = ah * bl;
  uword tmp_hh = ah * bh;

  uword tmp_lll = tmp_ll & 0xffff;
  uword tmp_llh = tmp_ll >> 16;
  uword tmp_lhl = tmp_lh & 0xffff;
  uword tmp_lhh = tmp_lh >> 16;
  uword tmp_hll = tmp_hl & 0xffff;
  uword tmp_hlh = tmp_hl >> 16;
  uword tmp_hhl = tmp_hh & 0xffff;
  uword tmp_hhh = tmp_hh >> 16;

  uword c0 = tmp_lll;
  uword c1 = tmp_llh + tmp_hll + tmp_lhl;
  uword c1h = c1 >> 16;
  c1 &= 0xffff;
  uword c2 = c1h + tmp_hlh + tmp_lhh + tmp_hhl;
  uword c2h = c2 >> 16;
  c2 &= 0xffff;
  uword c3 = c2h + tmp_hhh;

  uword cl = (c1 << 16) + c0;
  uword ch = (c3 << 16) + c2;

  return make_pair(ch, cl);
}

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// 4.1.5 Arithmetic operators
// 結果の型は以下のように決定される．
// - 符号: ともに signed -> signed
//         上記以外      -> unsigned
// - size: unsized のオペランドは integer とおなじとみなす．
//         で，2つのオペランドの size の大きい方に合わせる．
//         ただし，どちらも unsized なら結果も unsized
// - 基数はなし(10)
//////////////////////////////////////////////////////////////////////

// 2の補数を計算し自分自身に代入する．
const BitVector&
BitVector::complement()
{
  // 中身はほとんど上と同じ
  if ( has_xz() ) {
    return *this = BitVector::x(size());
  }

  // 全ビットを反転して1を足す
  SizeType n = block(size());
  bool carry = true;
  uword m = mask(size());
  for ( SizeType i = 0; i < n; ++ i ) {
    mVal1.get()[i] = mVal0.get()[i];
    if ( carry ) {
      if ( mVal1.get()[i] == ALL1 ) {
	mVal1.get()[i] = ALL0;
	carry = true;
      }
      else {
	mVal1.get()[i] += 1;
	carry = false;
      }
    }
    mVal0.get()[i] = ~mVal1.get()[i];
    if ( i == n - 1) {
      mVal0.get()[i] |= ~m;
      mVal1.get()[i] &= m;
    }
  }

  return *this;
}

// 加算つき代入
const BitVector&
BitVector::operator+=(const BitVector& src)
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
    ans_base = 10;
  }

  if ( size() < ans_size ) {
    set(mVal0.get(), mVal1.get(), size(), ans_size, ans_sized, ans_signed, ans_base);
  }

  if ( src.size() < ans_size ) {
    return operator+=(BitVector(src, ans_size));
  }

  if ( has_xz() || src.has_xz() ) {
    return *this = BitVector::x(ans_size);
  }

  set_type(ans_sized, ans_signed, ans_base);

  SizeType n = block(size());
  uword carry = 0;
  for ( SizeType i = 0; i < n; ++ i ) {
    uword old_val = mVal1.get()[i];
    mVal1.get()[i] += src.mVal1.get()[i] + carry;
    mVal0.get()[i] = ~mVal1.get()[i];
    // carry 条件ってなに？
    // ans のほうが見掛け上小さくなったとき
    if ( mVal1.get()[i] < old_val ) {
      carry = 1;
    }
    else {
      carry = 0;
    }
  }

  return *this;
}

// 減算つき代入
const BitVector&
BitVector::operator-=(const BitVector& src)
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
    ans_base = 10;
  }

  if ( size() < ans_size ) {
    set(mVal0.get(), mVal1.get(), size(), ans_size, ans_sized, ans_signed, ans_base);
  }

  if ( src.size() < ans_size ) {
    return operator-=(BitVector(src, ans_size));
  }

  if ( has_xz() || src.has_xz() ) {
    return *this = BitVector::x(ans_size);
  }

  set_type(ans_sized, ans_signed, ans_base);

  SizeType n = block(size());
  uword carry = 1;
  for ( SizeType i = 0; i < n; ++ i ) {
    uword old_val = mVal1.get()[i];
    mVal1.get()[i] += ~src.mVal1.get()[i] + carry;
    mVal0.get()[i] = ~mVal1.get()[i];
    // carry 条件ってなに？
    // ans のほうが見掛け上小さくなったとき
    if ( mVal1.get()[i] < old_val ) {
      carry = 1;
    }
    else {
      carry = 0;
    }
  }

  return *this;
}

// 乗算つき代入
const BitVector&
BitVector::operator*=(const BitVector& src)
{
  SizeType ans_size = size();
  if ( ans_size < src.size() ) {
    ans_size = src.size();
  }
  bool ans_sized = is_sized() || src.is_sized();
  bool ans_signed = is_signed() && src.is_signed();
  int ans_base = base();
  if ( ans_base != src.base() ) {
    ans_base = 10;
  }

  if ( size() < ans_size ) {
    set(mVal0.get(), mVal1.get(), size(), ans_size, ans_sized, ans_signed, ans_base);
  }
  if ( src.size() < ans_size ) {
    return operator*=(BitVector(src, ans_size));
  }
  if ( has_xz() || src.has_xz() ) {
    return *this = BitVector::x(ans_size);
  }

  // 符号あわせをしておく．
  BitVector tmp1 = ans_signed && is_negative() ? - *this : *this;
  BitVector tmp2 = ans_signed && src.is_negative() ? - src : src;
  bool invert = ans_signed ? is_negative() ^ src.is_negative() : false;

  set_type(ans_sized, ans_signed, ans_base);

  SizeType n = block(size());
  uword carry = 0;
  for ( SizeType i = 0; i < n; ++ i ) {
    uword v = carry;
    carry = 0;
    for ( SizeType j = 0; j <= i; ++ j ) {
      uword old_v = v;
      pair<uword, uword> tmp = mult32(tmp1.mVal1.get()[j], tmp2.mVal1.get()[i - j]);
      v += tmp.second;
      if ( v < old_v ) {
	++ carry;
      }
    }
    for ( SizeType j = 0; j < i; ++ j ) {
      uword old_v = v;
      pair<uword, uword> tmp = mult32(tmp1.mVal1.get()[j], tmp2.mVal1.get()[i - j - 1]);
      v += tmp.first;
      if ( v < old_v ) {
	++ carry;
      }
    }
    mVal1.get()[i] = v;
    mVal0.get()[i] = ~v;
  }
  // 上位ビットをマスクしておく
  uword m = mask(ans_size);
  mVal0.get()[n - 1] |= ~m;
  mVal1.get()[n - 1] &= m;

  if ( invert ) {
    complement();
  }

  return *this;
}

// 除算つき代入
const BitVector&
BitVector::operator/=(const BitVector& src)
{
  SizeType ans_size = size();
  if ( ans_size < src.size() ) {
    ans_size = src.size();
  }
  bool ans_sized = is_sized() || src.is_sized();
  bool ans_signed = is_signed() && src.is_signed();
  int ans_base = base();
  if ( ans_base != src.base() ) {
    ans_base = 10;
  }

  if ( size() < ans_size ) {
    set(mVal0.get(), mVal1.get(), size(), ans_size, ans_sized, ans_signed, ans_base);
  }
  if ( src.size() < ans_size ) {
    return operator/=(BitVector(src, ans_size));
  }
  if ( has_xz() || src.has_xz() ) {
    return *this = BitVector::x(ans_size);
  }

  // 符号あわせをしておく．
  BitVector tmp1 = ans_signed && is_negative() ? - *this : *this;
  BitVector tmp2 = ans_signed && src.is_negative() ? - src : src;
  bool invert = ans_signed ? is_negative() ^ src.is_negative() : false;

  set_type(ans_sized, ans_signed, ans_base);

  tmp1.set_type(tmp1.is_sized(), false, tmp1.base());
  tmp2.set_type(tmp2.is_sized(), false, tmp2.base());

  BitVector p(VlScalarVal::zero(), ans_size);
  for ( SizeType i = ans_size; i -- > 0; ) {
    p <<= 1;
    if ( tmp1.value(i).is_one() ) {
      p.mVal0.get()[0] &= ~1U;
      p.mVal1.get()[0] |= 1U;
    }
    if ( p >= tmp2 ) {
      p -= tmp2;
      mVal0.get()[i] &= ~1U;
      mVal1.get()[i] |= 1U;
    }
  }

  if ( invert ) {
    complement();
  }

  return *this;
}

// 剰余算
const BitVector&
BitVector::operator%=(const BitVector& src)
{
  SizeType ans_size = size();
  if ( ans_size < src.size() ) {
    ans_size = src.size();
  }
  bool ans_sized = is_sized() || src.is_sized();
  bool ans_signed = is_signed() && src.is_signed();
  int ans_base = src.base();
  if ( ans_base != src.base() ) {
    ans_base = 10;
  }

  if ( size() < ans_size ) {
    set(mVal0.get(), mVal1.get(), size(), ans_size, ans_sized, ans_signed, ans_base);
  }
  if ( src.size() < ans_size ) {
    return operator%=(BitVector(src, ans_size));
  }
  if ( has_xz() || src.has_xz() ) {
    return *this = BitVector::x(ans_size);
  }

  // 符号あわせをしておく．
  BitVector tmp1 = ans_signed && is_negative() ? - *this : *this;
  BitVector tmp2 = ans_signed && src.is_negative() ? - src : src;
  bool invert = ans_signed ? is_negative() : false;

  tmp2.set_type(tmp2.is_sized(), false, tmp2.base());

  set_type(ans_sized, ans_signed, ans_base);
  *this = BitVector(VlScalarVal::zero(), ans_size);

  for ( SizeType i = ans_size; i -- > 0; ) {
    operator<<=(1);
    if ( tmp1.value(i).is_one() ) {
      mVal0.get()[0] &= ~1U;
      mVal1.get()[0] |= 1U;
    }
    if ( *this >= tmp2 ) {
      operator-=(tmp2);
    }
  }

  if ( invert ) {
    complement();
  }

  return *this;
}

// 巾乗
const BitVector&
BitVector::power(const BitVector& src)
{
  SizeType ans_size = size();
  if ( ans_size < src.size() ) {
    ans_size = src.size();
  }
  bool ans_sized = is_sized() || src.is_sized();
  bool ans_signed = is_signed() && src.is_signed();
  int ans_base = base();
  if ( ans_base != src.base() ) {
    ans_base = 10;
  }

  if ( size() < ans_size ) {
    set(mVal0.get(), mVal1.get(), size(), ans_size, ans_sized, ans_signed, ans_base);
  }
  if ( src.size() < ans_size ) {
    return power(BitVector(src, ans_size));
  }
  if ( has_xz() || src.has_xz() ) {
    return *this = BitVector::x(ans_size);
  }

  // 特殊な例: 自分が2ならシフトで代用できる．
  if ( *this == BitVector(2) ) {
    *this = 1;
    return alshift(src);
  }

  // 馬鹿げた実装
  BitVector ans(*this);
  BitVector i(src);
  BitVector c1(1);
  BitVector c0(0);
  for ( i = i - c1; i > c0; i = i - c1 ) {
    ans *= *this;
  }
  return *this = ans;
}


//////////////////////////////////////////////////////////////////////
// 4.1.7 Relational operators
// 結果が x になる場合があるので bool ではなく BitVector を返す．
// 2つのオペランドのサイズが異なっていたら長い方に合わせる．
// ともに符号付きの場合には符号付き数として比較する．
// そうでなければ符号なし数として比較する．
// 結果の型は以下のとおり
// - 符号なし
// - サイズあり(1)
// - 基数は2
//////////////////////////////////////////////////////////////////////

// 小なり
bool
BitVector::lt_base(const BitVector& src1,
		   const BitVector& src2)
{
  bool invert = false;
  if ( src1.is_signed() && src2.is_signed() ) {
    // 符号だけでわかる場合もある．
    bool neg1 = src1.is_negative();
    bool neg2 = src2.is_negative();
    if ( neg1 && !neg2 ) {
      return true;
    }
    if ( !neg1 && neg2 ) {
      return false;
    }
    if ( neg1 && neg2 ) {
      invert = true;
    }
  }

  SizeType n = block(src1.size());
  for ( int i = n; i -- > 0; ) {
    if ( src1.mVal1.get()[i] < src2.mVal1.get()[i] ) {
      return !invert;
    }
    else if ( src1.mVal1.get()[i] > src2.mVal1.get()[i] ) {
      return invert;
    }
  }
  return invert;
}

// 小なり
VlScalarVal
lt(const BitVector& src1,
   const BitVector& src2)
{
  if ( src1.has_xz() || src2.has_xz() ) {
    return VlScalarVal::x();
  }

  bool ans = false;
  if ( src1.size() < src2.size() ) {
    ans = BitVector::lt_base(BitVector(src1, src2.size()), src2);
  }
  else if ( src1.size() > src2.size() ) {
    ans = BitVector::lt_base(src1, BitVector(src2, src1.size()));
  }
  else {
    ans = BitVector::lt_base(src1, src2);
  }
  if ( ans ) {
    return VlScalarVal::one();
  }
  return VlScalarVal::zero();
}

// 小なり
// こちらは bool にキャストしたバージョン
bool
operator<(const BitVector& src1,
	  const BitVector& src2)
{
  if ( src1.has_xz() || src2.has_xz() ) {
    return false;
  }

  if ( src1.size() < src2.size() ) {
    return BitVector::lt_base(BitVector(src1, src2.size()), src2);
  }
  if ( src1.size() > src2.size() ) {
    return BitVector::lt_base(src1, BitVector(src2, src1.size()));
  }
  return BitVector::lt_base(src1, src2);
}


//////////////////////////////////////////////////////////////////////
// 4.1.8 Equality operators
// 結果が x になる場合があるので bool ではなく BitVector を返す．
// 2つのオペランドのサイズが異なっていたら長い方に合わせる．
// 結果の型は以下のとおり
// - 符号なし
// - サイズあり(1)
// - 基数は2
//////////////////////////////////////////////////////////////////////

// @brief 比較演算のベースとなる関数
// @param[in] src1, src2 オペランド
// @param[in] mode:
//   - 1 普通の等価比較
//   - 2 x をワイルドカードと見なした等価比較
//   - 3 x と z をワイルドカードと見なした等価比較
// @return src1 と src2 が等しいと見なせるとき true を返す．
bool
BitVector::eq_base(const BitVector& src1,
		   const BitVector& src2,
		   int mode)
{
  if ( mode == 1 ) {
    // 通常の等価比較
    return src1.mVal0 == src2.mVal0;
  }

  if ( mode == 2 ) {
    // x を 0 または 1 と見なす等価比較
    SizeType n = block(src1.size());
    for ( SizeType i = 0; i < n - 1; ++ i ) {
      if ( ((src1.mVal0.get()[i] & src2.mVal0.get()[i]) | (src1.mVal1.get()[i] & src2.mVal1.get()[i]))
	   != ALL1 ) {
	return false;
      }
    }
    uword m = mask(src1.size());
    if ( ((src1.mVal0.get()[n - 1] & src2.mVal0.get()[n - 1]) |
	  (src1.mVal1.get()[n - 1] & src2.mVal1.get()[n - 1]) |
	  ~m) != ALL1 ) {
      return false;
    }
    return true;
  }

  // x と z を 0 または 1 と見なす等価比較
  SizeType n = block(src1.size());
  for ( SizeType i = 0; i < n - 1; ++ i ) {
    uword val01 = src1.mVal0.get()[i];
    uword val11 = src1.mVal1.get()[i];
    uword val02 = src2.mVal0.get()[i];
    uword val12 = src2.mVal1.get()[i];
    if ( ((val01 & val02) | (val11 & val12)) != ALL1 &&
	 ((val01 | val02) & (val11 | val12)) != ALL0 ) {
      return false;
    }
  }

  uword m = mask(src1.size());
  uword val01 = src1.mVal0.get()[n - 1];
  uword val11 = src1.mVal1.get()[n - 1];
  uword val02 = src2.mVal0.get()[n - 1];
  uword val12 = src2.mVal1.get()[n - 1];
  if ( ((val01 & val02) | (val11 & val12) | ~m ) != ALL1 &&
       ((val01 | val02) & (val11 | val12) &  m ) != ALL0 ) {
    return false;
  }
  return true;
}

// @brief 等価比較演算子
// @param[in] src1, src2 オペランド
// @retval 1 src1 == src2 の時
// @retval 0 src1 != src2 の時
// @retval X 比較不能の時
VlScalarVal
eq(const BitVector& src1,
   const BitVector& src2)
{
  if ( src1.has_xz() || src2.has_xz() ) {
    return VlScalarVal::x();
  }

  bool ans = false;
  if ( src1.size() < src2.size() ) {
    ans = BitVector::eq_base(BitVector(src1, src2.size()), src2, 1);
  }
  else if ( src1.size() > src2.size() ) {
    ans = BitVector::eq_base(src1, BitVector(src2, src1.size()), 1);
  }
  else {
    ans = BitVector::eq_base(src1, src2, 1);
  }
  if ( ans ) {
    return VlScalarVal::one();
  }
  return VlScalarVal::zero();
}

// @brief x が 0 および 1 と等価と見なせるとした場合の等価比較演算子
// @param[in] src1, src2 オペランド
// @return 等価と見なせるとき true を返す．
bool
eq_with_x(const BitVector& src1,
	  const BitVector& src2)
{
  if ( src1.size() < src2.size() ) {
    return BitVector::eq_base(BitVector(src1, src2.size()), src2, 2);
  }
  if ( src1.size() > src2.size() ) {
    return BitVector::eq_base(src1, BitVector(src2, src1.size()), 2);
  }
  return BitVector::eq_base(src1, src2, 2);
}

// @brief x および z が 0 および 1 と等価と見なせるとした場合の等価比較演算子
// @param[in] src1, src2 オペランド
// @return 等価と見なせるとき true を返す．
bool
eq_with_xz(const BitVector& src1,
	   const BitVector& src2)
{
  if ( src1.size() < src2.size() ) {
    return BitVector::eq_base(BitVector(src1, src2.size()), src2, 3);
  }
  if ( src1.size() > src2.size() ) {
    return BitVector::eq_base(src1, BitVector(src2, src1.size()), 3);
  }
  return BitVector::eq_base(src1, src2, 3);
}

// @brief 等価比較演算子 (bool)
// @param[in] src1, src2 オペランド
// @return 1 src1 == src2 を返す．
bool
operator==(const BitVector& src1,
	   const BitVector& src2)
{
  if ( src1.has_xz() || src2.has_xz() ) {
    return false;
  }

  if ( src1.size() < src2.size() ) {
    return BitVector::eq_base(BitVector(src1, src2.size()), src2, 1);
  }
  if ( src1.size() > src2.size() ) {
    return BitVector::eq_base(src1, BitVector(src2, src1.size()), 1);
  }
  return BitVector::eq_base(src1, src2, 1);
}

END_NAMESPACE_YM_VERILOG
