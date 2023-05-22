
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

// @brief val を表す文字を返す．
inline
char
hexchar(int val)
{
  ASSERT_COND( 0 <= val && val < 16 );

  if ( val <= 9 ) {
    return static_cast<char>('0' + val);
  }
  else {
    return static_cast<char>('A' + val - 10);
  }
}

// @brief val を10進数で表した文字列を得る．
string
size_to_dec_str(int val)
{
  string ans;
  if ( val >= 10 ) {
    int q = val / 10;
    ans = size_to_dec_str(q);
  }
  int r = val % 10;
  char c = static_cast<char>('0' + r);
  ans += c;
  return ans;
}

END_NONAMESPACE


// @brief src を 10 で割る．
// @param[in] src 元の値
// @param[out] q 求まった商を格納する変数
// @return 余りを返す．
int
BitVector::div10(const uword* src,
		 int n,
		 uword* q)
{
  int l = n * BLOCK_SIZE;
  int r = 0;
  for ( int i = l; i -- > 0; ) {
    r <<= 1;
    int blk = i / BLOCK_SIZE;
    int pos = i % BLOCK_SIZE;
    if ( (src[blk] >> pos) & 1 ) {
      r |= 1;
    }
    if ( r >= 10 ) {
      r -= 10;
      q[blk] |= 1 << pos;
    }
  }
  return r;
}

// @brief val を10進数で表した文字列を返す．
string
BitVector::dec_str_sub(const uword* val,
		       int n)
{
  uword val1[n];
  for ( int i = 0; i < n; ++ i ) {
    val1[i] = 0;
  }
  int r = div10(val, n, val1);
  bool zero = true;
  for ( int i = 0; i < n; ++ i ) {
    if ( val1[i] ) {
      zero = false;
      break;
    }
  }
  string ans;
  if ( !zero ) {
    ans = dec_str_sub(val1, n);
  }
  ans += hexchar(r);
  return ans;
}


//////////////////////////////////////////////////////////////////////
// その他のメンバ関数
//////////////////////////////////////////////////////////////////////

// pos ビット目の値を得る．
// pos が範囲を越えていたら kX を返す．
VlScalarVal
BitVector::value(int pos) const
{
  if ( pos < 0 || pos >= static_cast<int>(size()) ) {
    return VlScalarVal::x();
  }

  SizeType blk = pos / BLOCK_SIZE;
  SizeType sft = pos - blk * BLOCK_SIZE;
  uword msk = 1 << sft;
  if ( mVal1.get()[blk] & msk ) {
    if ( mVal0.get()[blk] & msk ) {
      return VlScalarVal::x();
    }
    else {
      return VlScalarVal::one();
    }
  }
  else {
    if ( mVal0.get()[blk] & msk ) {
      return VlScalarVal::zero();
    }
    else {
      return VlScalarVal::z();
    }
  }
}

// x 値を含んでいたら true を返す
bool
BitVector::has_x() const
{
  SizeType n = block(size());
  for ( SizeType i = 0; i < n - 1; ++ i ) {
    uword tmp = mVal0.get()[i] & mVal1.get()[i];
    if ( tmp ) {
      return true;
    }
  }

  uword m = mask(size());
  if ( mVal0.get()[n - 1] & mVal1.get()[n - 1] & m ) {
    return true;
  }
  return false;
}

// z 値を含んでいたら true を返す
bool
BitVector::has_z() const
{
  SizeType n = block(size());
  for ( SizeType i = 0; i < n - 1; ++ i ) {
    if ( (mVal0.get()[i] | mVal1.get()[i]) != ALL1 ) {
      return true;
    }
  }

  uword m = mask(size());
  if ( (mVal0.get()[n - 1] | mVal1.get()[n - 1] | ~m) != ALL1 ) {
    return true;
  }
  return false;
}

// x か z を含んでいたら true を返す
bool
BitVector::has_xz() const
{
  SizeType n = block(size());
  for ( SizeType i = 0; i < n - 1; ++ i ) {
    if ( (mVal0.get()[i] ^ mVal1.get()[i]) != ALL1 ) {
      return true;
    }
  }

  uword m = mask(size());
  if ( ((mVal0.get()[n - 1] ^ mVal1.get()[n - 1]) | ~m) != ALL1 ) {
    return true;
  }
  return false;
}

// z を x に変える．
// ほとんどの演算において入力の z は x と区別されない
void
BitVector::z_to_x()
{
  SizeType n = block(size());
  for ( SizeType i = 0; i < n - 1; ++ i ) {
    uword zpat = ~mVal0.get()[i] & ~mVal1.get()[i];
    mVal0.get()[i] |= zpat;
    mVal1.get()[i] |= zpat;
  }
  uword m = mask(size());
  uword zpat = ~mVal0.get()[n - 1] & ~mVal1.get()[n - 1] & m;
  mVal0.get()[n - 1] |= zpat;
  mVal1.get()[n - 1] |= zpat;
}

// x/z を 0 に変える．
// Verilog-HDL で4値を2値に強引に変換するときのやり方
void
BitVector::xz_to_0()
{
  SizeType n = block(size());
  for ( SizeType i = 0; i < n - 1; ++ i ) {
    uword pat = ~mVal0.get()[i] & mVal1.get()[i];
    mVal0.get()[i] = ~pat;
    mVal1.get()[i] = pat;
  }
  uword m = mask(size());
  uword pat = ~mVal0.get()[n - 1] & mVal1.get()[n - 1];
  uword andpat = ~pat | ~m;
  uword orpat = pat & m;
  mVal0.get()[n - 1] = (mVal0.get()[n - 1] | orpat) & andpat;
  mVal1.get()[n - 1] = (mVal1.get()[n - 1] | orpat) & andpat;
}

// 値を近い double 型に変換する．
// X/Z は0と見なす．
double
BitVector::to_real() const
{
  BitVector tmp = *this;
  tmp.xz_to_0();
  SizeType n = block(size());
  double ans = 0.0;
  for ( SizeType i = 0; i < n; ++ i ) {
    const double mag = static_cast<double>(1 << (i * BLOCK_SIZE));
    ans += static_cast<double>(mVal1.get()[i]) * mag;
  }
  return ans;
}

// @brief 論理値として評価する．
// @retval VlScalarVal::zero() 0 の時
// @retval VlScalarVal::one() 0 以外の確定値の時
// @retval VlScalarVal::x() 不定値を1ビットでも含む場合
VlScalarVal
BitVector::to_logic() const
{
  SizeType n = block(size());
  for ( SizeType i = 0; i < n - 1; ++ i ) {
    uword pat0 = mVal0.get()[i];
    uword pat_xor = pat0 ^ mVal1.get()[i];
    if ( pat_xor != ALL1 ) {
      // X/Z のパタンがあった
      return VlScalarVal::x();
    }
    if ( pat0 != ALL0 ) {
      return VlScalarVal::one();
    }
  }
  uword m = mask(size());
  uword pat0 = mVal0.get()[n - 1];
  uword pat_xor = (pat0 ^ mVal1.get()[n - 1]) | ~m;
  if ( pat_xor != ALL1 ) {
    // X/Z のパタンがあった
    return VlScalarVal::x();
  }
  if ( pat0 != ALL0 ) {
    return VlScalarVal::one();
  }
  return VlScalarVal::zero();
}

// 値をバイトベクターと見なして文字列に変換する．
// 長さが8の倍数でない場合にはパディングする．
string
BitVector::to_string() const
{
  SizeType n = size();
  SizeType nc = (n + 7) / 8;
  string ans;
  for ( int i = nc; i -- > 0; ) {
    char c = 0;
    int base = i << 3;
    for ( int j = 0; j < 8; ++ j ) {
      if ( value(base + j).is_one() ) {
	c |= (1 << j);
      }
    }
    ans += c;
  }

  return ans;
}

// Verilog-HDL の形式で出力する．
// 内容を Verilog-HDL (IEEE1364-2001) の形式の文字列に変換する．
string
BitVector::verilog_string(
  SizeType opt_base
) const
{
  // まず最初に符号付きでマイナスの場合には補数を取る．
  if ( is_negative() ) {
    BitVector tmp = -(*this);
    return "-" + tmp.verilog_string(opt_base);
  }

  if ( opt_base != 2 &&
       opt_base != 8 &&
       opt_base != 10 &&
       opt_base != 16 ) {
    // 安全策
    opt_base = base();
  }

  // サイズなしの場合には上位の0は無視する．
  bool skip_zeros = !is_sized();

  string ans;
  SizeType l = size();
  if ( l == 1 ) {
    VlScalarVal v = value(0);
    if ( v.is_zero() ) {
      ans += '0';
    }
    else if ( v.is_one() ) {
      ans += '1';
    }
    else if ( v.is_x() ) {
      ans += 'X';
    }
    else if ( v.is_z() ) {
      ans += 'Z';
    }
    else {
      ASSERT_NOT_REACHED;
    }
  }
  else {
    if ( is_sized() ) {
      ans += size_to_dec_str(l);
    }
    switch ( opt_base ) {
    case 2:
      if ( is_signed() ) {
	ans += "\'sb";
      }
      else {
	ans += "\'b";
      }
      ans += bin_str(skip_zeros);
      break;

    case 8:
      if ( is_signed() ) {
	ans += "\'so";
      }
      else {
	ans += "\'o";
      }
      ans += oct_str(skip_zeros);
      break;

    case 10:
      if ( has_xz() ) {
	if ( is_signed() ) {
	  ans += "\'sb";
	}
	else {
	  ans += "\'b";
	}
	ans += bin_str(skip_zeros);
      }
      else {
	if ( is_signed() ) {
	  if ( is_sized() ) {
	    ans += "\'sd";
	  }
	}
	else {
	  ans += "\'d";
	}
	ans += dec_str();
      }
      break;

    case 16:
      if ( is_signed() ) {
	ans += "\'sh";
      }
      else {
	ans += "\'h";
      }
      ans += hex_str(skip_zeros);
      break;
    }
  }
  return ans;
}

// 内容を10進数で表した文字列を返す．
string
BitVector::dec_str() const
{
  return dec_str_sub(mVal1.get(), block(size()));
}

// 内容を2進数で表した文字列を返す．
string
BitVector::bin_str(bool skip_zeros) const
{
  SizeType l = size();
  bool first = true;
  string ans;
  for ( int i = l; i -- > 0; ) {
    VlScalarVal v = value(i);
    if ( skip_zeros && v.is_zero() ) {
      continue;
    }
    skip_zeros = false;
    if ( i % 4 == 3 && !first ) {
      ans += '_';
    }
    if ( v.is_zero() ) {
      ans += '0';
    }
    else if ( v.is_one() ) {
      ans += '1';
    }
    else if ( v.is_x() ) {
      ans += 'X';
    }
    else if ( v.is_z() ) {
      ans += 'Z';
    }
    else {
      ASSERT_NOT_REACHED;
    }
    first = false;
  }
  if ( skip_zeros ) {
    // 全部0だった
    ans += '0';
  }
  return ans;
}

// 内容を8進数で表した文字列を返す．
string
BitVector::oct_str(bool skip_zeros) const
{
  SizeType l = size();
  SizeType blk = block(l);
  SizeType pos = shift(l);
  uword tmp0 = 7;
  uword tmp1 = 0;
  string ans;
  while ( blk -- > 0 ) {
    for ( int i = pos; i -- > 0; ) {
      uword bit0 = (mVal0.get()[blk] >> i) & 1;
      uword bit1 = (mVal1.get()[blk] >> i) & 1;
      tmp0 = ((tmp0 << 1) + bit0) & 7;
      tmp1 = ((tmp1 << 1) + bit1) & 7;
      l --;
      if ( l % 3 == 0 ) {
	if ( tmp0 + tmp1 == 7 ) {
	  if ( tmp1 ) {
	    // 0 ではない．
	    skip_zeros = false;
	  }
	  if ( !skip_zeros ) {
	    ans += hexchar(tmp1);
	  }
	}
	else if ( tmp0 == 0 && tmp1 == 0 ) {
	  skip_zeros = false;
	  ans += 'z';
	}
	else if ( tmp0 == 7 && tmp1 == 7 ) {
	  skip_zeros = false;
	  ans += 'x';
	}
	else {
	  skip_zeros = false;
	  // あってはならない．
	  // というかこの基底では表現できない
	  ans += '#';
	}
	tmp0 = 0;
	tmp1 = 0;
      }
    }
    pos = BLOCK_SIZE;
  }
  if ( skip_zeros ) {
    // 全部0だった
    ans += '0';
  }
  return ans;
}

// 内容を16進で表した文字列をかえす．
string
BitVector::hex_str(bool skip_zeros) const
{
  SizeType l = size();
  SizeType blk = block(l);
  SizeType pos = shift(l);
  uword tmp0 = 15;
  uword tmp1 = 0;
  string ans;
  while ( blk -- > 0 ) {
    for ( int i = pos; i -- > 0; ) {
      uword bit0 = (mVal0.get()[blk] >> i) & 1;
      uword bit1 = (mVal1.get()[blk] >> i) & 1;
      tmp0 = ((tmp0 << 1) + bit0) & 15;
      tmp1 = ((tmp1 << 1) + bit1) & 15;
      l --;
      if ( l % 4 == 0 ) {
	if ( tmp0 + tmp1 == 15 ) {
	  if ( tmp1 ) {
	    // 0 ではない．
	    skip_zeros = false;
	  }
	  if ( !skip_zeros ) {
	    ans += hexchar(tmp1);
	  }
	}
	else if ( tmp0 == 0 && tmp1 == 0 ) {
	  skip_zeros = false;
	  ans += 'z';
	}
	else if ( tmp0 == 15 && tmp1 == 15 ) {
	  skip_zeros = false;
	  ans += 'x';
	}
	else {
	  skip_zeros = false;
	  // あってはならない．
	  // というかこの基底では表現できない
	  ans += '#';
	}
	tmp0 = 15;
	tmp1 = 0;
      }
    }
    pos = BLOCK_SIZE;
  }
  if ( skip_zeros ) {
    // 全部0だった
    ans += '0';
  }
  return ans;
}

// 値をセットする関数
// これは1語に収まる時に用いる．
void
BitVector::set(
  uword val0,
  uword val1,
  SizeType size,
  bool has_size,
  bool has_sign,
  SizeType base
)
{
  ASSERT_COND( size <= BLOCK_SIZE );

  resize(size);
  set_type(has_size, has_sign, base);
  uword m = mask(size);
  mVal0.get()[0] = val0 | ~m;
  mVal1.get()[0] = val1 & m;
}

// 値をセットする関数
void
BitVector::set(
  const uword* val0,
  const uword* val1,
  SizeType src_size,
  SizeType size,
  bool has_size,
  bool has_sign,
  SizeType base
)
{
  resize(size);
  set_type(has_size, has_sign, base);

  SizeType n = block(size);
  SizeType src_n = block(src_size);

  // val0, val1 の最後のビットの値
  // デフォルトでは 0 を表すビットパタンだが x や z の場合もある．
  // 最上位ビットが1で符号付きの場合には1を詰める．
  uword last_val0 = ALL1;
  uword last_val1 = ALL0;

  SizeType src_s = shift(src_size);
  uword last_bit0 = (val0[src_n - 1] >> (src_s - 1)) & 1;
  uword last_bit1 = (val1[src_n - 1] >> (src_s - 1)) & 1;
  if ( last_bit0 && last_bit1 ) { // x
    last_val1 = ALL1;
  }
  else if ( !last_bit0 && !last_bit1 ) { // z
    last_val0 = ALL0;
  }
  else if ( !last_bit0 && last_bit1 && is_signed() ) { // 1
    last_val0 = ALL0;
    last_val1 = ALL1;
  }

  // コピーする
  for ( SizeType i = 0; i < n; ++ i ) {
    if ( i < src_n - 1 ) {
      mVal0.get()[i] = val0[i];
      mVal1.get()[i] = val1[i];
    }
    else if ( i == src_n - 1 ) {
      uword sm = mask(src_size);
      mVal0.get()[i] = (last_val0 & ~sm) | (val0[i] & sm);
      mVal1.get()[i] = (last_val1 & ~sm) | (val1[i] & sm);
    }
    else {
      mVal0.get()[i] = last_val0;
      mVal1.get()[i] = last_val1;
    }
  }

  // 上位ビットをトリミングしておく
  uword m = mask(size);
  mVal0.get()[n - 1] |= ~m;
  mVal1.get()[n - 1] &= m;
}

// 値をセットする関数
void
BitVector::set(
  const vector<uword>& val0,
  const vector<uword>& val1,
  SizeType src_size,
  SizeType size,
  bool has_size,
  bool has_sign,
  SizeType base
)
{
  resize(size);
  set_type(has_size, has_sign, base);

  SizeType n = block(size);
  SizeType src_n = block(src_size);

  // val0, val1 の最後のビットの値
  // デフォルトでは 0 を表すビットパタンだが x や z の場合もある．
  // 最上位ビットが1で符号付きの場合には1を詰める．
  uword last_val0 = ALL1;
  uword last_val1 = ALL0;

  SizeType src_s = shift(src_size);
  uword last_bit0 = (val0[src_n - 1] >> (src_s - 1)) & 1;
  uword last_bit1 = (val1[src_n - 1] >> (src_s - 1)) & 1;
  if ( last_bit0 && last_bit1 ) { // x
    last_val1 = ALL1;
  }
  else if ( !last_bit0 && !last_bit1 ) { // z
    last_val0 = ALL0;
  }
  else if ( !last_bit0 && last_bit1 && is_signed() ) { // 1
    last_val0 = ALL0;
    last_val1 = ALL1;
  }

  // コピーする
  for ( SizeType i = 0; i < n; ++ i ) {
    if ( i < src_n - 1 ) {
      mVal0.get()[i] = val0[i];
      mVal1.get()[i] = val1[i];
    }
    else if ( i == src_n - 1 ) {
      uword sm = mask(src_size);
      mVal0.get()[i] = (last_val0 & ~sm) | (val0[i] & sm);
      mVal1.get()[i] = (last_val1 & ~sm) | (val1[i] & sm);
    }
    else {
      mVal0.get()[i] = last_val0;
      mVal1.get()[i] = last_val1;
    }
  }

  // 上位ビットをトリミングしておく
  uword m = mask(size);
  mVal0.get()[n - 1] |= ~m;
  mVal1.get()[n - 1] &= m;
}

// mVal0, mVal1 のリサイズをする．
void
BitVector::resize(SizeType size)
{
  SizeType old_bsize = block(mSize);
  mSize = size;
  SizeType new_bsize = block(mSize);
  if ( new_bsize > old_bsize ) {
    mVal0 = unique_ptr<uword>{new uword[new_bsize]};
    mVal1 = unique_ptr<uword>{new uword[new_bsize]};
  }
}

END_NAMESPACE_YM_VERILOG
