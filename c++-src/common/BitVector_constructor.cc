
/// @file BitVector_constructor.cc
/// @brief BitVector の constructor の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym/BitVector.h"


BEGIN_NAMESPACE_YM_VERILOG

BEGIN_NONAMESPACE

using uword = std::uint64_t;

const SizeType BLOCK_SIZE = sizeof(uword) * 8;

/// @brief すべてが0のパタン
const uword ALL0 = 0x0000000000000000;

/// @brief すべてが1のパタン
const uword ALL1 = 0xFFFFFFFFFFFFFFFF;

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

// unsigned int からの変換コンストラクタ
BitVector::BitVector(
  unsigned int val
)
{
  // サイズなし，符号なし，基数10
  set(~val, val, kVpiSizeInteger, false, false, 10);
}

// @brief 符号なし整数からの代入演算子
BitVector&
BitVector::operator=(
  unsigned int val
)
{
  // サイズなし，符号なし，基数10
  set(~val, val, kVpiSizeInteger, false, false, 10);
  return *this;
}

// unsigned int からの変換コンストラクタ
BitVector::BitVector(
  SizeType val
)
{
  // サイズなし，符号なし，基数10
  set(~val, val, kVpiSizeInteger, false, false, 10);
}

// int からのキャスト用コンストラクタ
BitVector::BitVector(
  int val
)
{
  // サイズなし，符号あり，基数10
  set(~val, val, kVpiSizeInteger, false, true, 10);
}

// @brief int からの代入演算子
BitVector&
BitVector::operator=(
  int val
)
{
  // サイズなし，符号あり，基数10
  set(~val, val, kVpiSizeInteger, false, true, 10);
  return *this;
}

// bool からの変換コンストラクタ
BitVector::BitVector(
  bool value
)
{
  uword val0 = value ? 1 : 0;
  uword val1 = 1 - val0;
  // サイズあり(1)，符号なし，基数2
  set(val0, val1, 1, true, false, 2);
}

// @brief bool からの代入演算子
BitVector&
BitVector::operator=(
  bool value
)
{
  uword val0 = value ? 1 : 0;
  uword val1 = 1 - val0;
  // サイズあり(1)，符号なし，基数2
  set(val0, val1, 1, true, false, 2);
  return *this;
}

// @brief time 型からの変換コンストラクタ
BitVector::BitVector(
  VlTime time
)
{
  // サイズあり, 符号無し, 基数10
  resize(kVpiSizeTime);
  set_type(true, false, 10);
  mVal1.get()[0] = time.low();
  mVal1.get()[1] = time.high();
  mVal0.get()[0] = ~mVal1.get()[0];
  mVal0.get()[1] = ~mVal1.get()[1];
}

// @brief time 型からの代入演算子
BitVector&
BitVector::operator=(
  VlTime time
)
{
  // サイズあり, 符号無し, 基数10
  resize(kVpiSizeTime);
  set_type(true, false, 10);
  mVal1.get()[0] = time.low();
  mVal1.get()[1] = time.high();
  mVal0.get()[0] = ~mVal1.get()[0];
  mVal0.get()[1] = ~mVal1.get()[1];
  return *this;
}

// スカラ値からのキャスト用コンストラクタ
BitVector::BitVector(
  const VlScalarVal& value,
  SizeType size
)
{
  resize(size);
  set_type(true, false, 2);
  SizeType n = block(size);
  uword val0 = ALL0;
  uword val1 = ALL1;

  if ( value.is_zero() ) {
    val0 = ALL1;
    val1 = ALL0;
  }
  else if ( value.is_one() ) {
    val0 = ALL0;
    val1 = ALL1;
  }
  else if ( value.is_x() ) {
    val0 = ALL1;
    val1 = ALL1;
  }
  else if ( value.is_z() ) {
    val0 = ALL0;
    val1 = ALL0;
  }
  else {
    ASSERT_NOT_REACHED;
  }
  for ( SizeType i = 0; i < n - 1; ++ i ) {
    mVal0.get()[i] = val0;
    mVal1.get()[i] = val1;
  }
  // 最後のブロックだけはマスクをかける．
  uword m = mask(size);
  mVal0.get()[n - 1] = val0 | (~m);
  mVal1.get()[n - 1] = val1 & m;
}

// brief C文字列からの変換用コンストラクタ
BitVector::BitVector(
  const char* str
)
{
  operator=(str);
}

// @brief C文字列からの代入演算子
BitVector&
BitVector::operator=(
  const char* str
)
{
  if ( str ) {
    SizeType strsize = strlen(str);
    set_from_string(strsize, str);
  }
  else {
    // "\0" という文字列にしておく (4.2.3.3)
    set_from_string(1, "\0");
  }
  return *this;
}

// string 文字列からの変換用コンストラクタ
BitVector::BitVector(
  const string& str
)
{
  operator=(str);
}

// @brief string 文字列からの代入演算子
BitVector&
BitVector::operator=(
  const string& str
)
{
  SizeType strsize = str.size();
  if ( strsize == 0 ) {
    set_from_string(1, "\0");
  }
  else {
    // 上の set_from_string() を使ってもできるけど
    // c_str() を呼びたくない．
    SizeType s = strsize * 8;

    resize(s);
    set_type(true, false, 2);

    int j = 0;
    int k = 0;
    uword tmp = 0;
    for ( int i = strsize; i -- > 0; ) {
      char c = str[i];
      tmp += (static_cast<uword>(c) << (k * 8));
      ++ k;
      if ( k == 4 ) {
	mVal0.get()[j] = ~tmp;
	mVal1.get()[j] =  tmp;
	++ j;
	k = 0;
	tmp = 0;
      }
    }
    if ( k != 0 ) {
      uword mask = ALL1 << (k * 8);
      mVal0.get()[j] = ~tmp | mask;
      mVal1.get()[j] =  tmp;
    }
  }
  return *this;
}

// 浮動小数点数を整数に変換してからビットベクタにする
BitVector::BitVector(
  double val
)
{
  double r = rint(val);
  int intval = static_cast<int>(r);
  set(~intval, intval, BLOCK_SIZE, false, true, 10);
}

// @brief 浮動小数点数からの代入演算子
BitVector&
BitVector::operator=(
  double val
)
{
  double r = rint(val);
  int intval = static_cast<int>(r);
  set(~intval, intval, BLOCK_SIZE, false, true, 10);
  return *this;
}

// Verilog-HDL 形式の文字列からの変換コンストラクタ
BitVector::BitVector(
  SizeType size,
  bool is_signed,
  SizeType base,
  const string& str
)
{
  bool is_sized = true;
  if ( size == 0 ) {
    size = BLOCK_SIZE;
    is_sized = false;
  }
  switch ( base ) {
  case  2: set_from_binstring(size, is_sized, is_signed, str, 0); break;
  case  8: set_from_octstring(size, is_sized, is_signed, str, 0); break;
  case 10: set_from_decstring(size, is_sized, is_signed, str, 0); break;
  case 16: set_from_hexstring(size, is_sized, is_signed, str, 0); break;
  default: cerr << "illegal base : " << base << endl;
  }
}

// @brief 連結演算用のコンストラクタ
BitVector::BitVector(
  const vector<BitVector>& src_list
)
{
  SizeType tl = 0;
  for ( const auto& bv: src_list ) {
    tl += bv.size();
  }

  resize(tl);
  set_type(true, false, 2);

  SizeType blk = 0;
  SizeType pos = 0;
  for ( const auto& bv: src_list ) {
    SizeType l = bv.size();
    SizeType b = block(l);
    SizeType s = shift(l);
    uword m = mask(l);
    if ( pos == 0 ) {
      for ( SizeType i = 0; i < b - 1; ++ i ) {
	mVal0.get()[blk] = bv.mVal0.get()[i];
	mVal1.get()[blk] = bv.mVal1.get()[i];
	++ blk;
      }
      mVal0.get()[blk] = bv.mVal0.get()[b - 1] & m;
      mVal1.get()[blk] = bv.mVal1.get()[b - 1] & m;
      pos = s;
      if ( pos == BLOCK_SIZE ) {
	pos = 0;
	++ blk;
      }
    }
    else {
      SizeType rpos = BLOCK_SIZE - pos;
      for ( SizeType i = 0; i < b - 1; ++ i ) {
	mVal0.get()[blk] |= (bv.mVal0.get()[i] << pos);
	mVal1.get()[blk] |= (bv.mVal1.get()[i] << pos);
	++ blk;
	mVal0.get()[blk] |= (bv.mVal0.get()[i] >> rpos);
	mVal1.get()[blk] |= (bv.mVal1.get()[i] >> rpos);
      }
      uword val0 = bv.mVal0.get()[b - 1] & m;
      uword val1 = bv.mVal1.get()[b - 1] & m;
      mVal0.get()[blk] |= (val0 << pos);
      mVal1.get()[blk] |= (val1 << pos);
      pos += s;
      if ( pos >= BLOCK_SIZE ) {
	++ blk;
	pos -= BLOCK_SIZE;
	if ( pos > 0 ) {
	  mVal0.get()[blk] |= (val0 >> rpos);
	  mVal1.get()[blk] |= (val1 >> rpos);
	}
      }
    }
  }
}

// コピーコンストラクタ
BitVector::BitVector(
  const BitVector& src
) : mSize{src.mSize},
    mFlags{src.mFlags},
    mVal0{unique_ptr<uword>{new uword[block(mSize)]}},
    mVal1{unique_ptr<uword>{new uword[block(mSize)]}}
{
  int n = block(mSize);
  for ( int i = 0; i < n; ++ i ) {
    mVal0.get()[i] = src.mVal0.get()[i];
    mVal1.get()[i] = src.mVal1.get()[i];
  }
}

// @brief ムーブコンストラクタ
BitVector::BitVector(
  BitVector&& src
) : mSize{src.mSize},
    mFlags{src.mFlags},
    mVal0{std::move(src.mVal0)},
    mVal1{std::move(src.mVal1)}
{
}

// コピー代入演算子
BitVector&
BitVector::operator=(
  const BitVector& src
)
{
  if ( &src != this ) {
    mSize = src.mSize;
    mFlags = src.mFlags;
    SizeType n = block(mSize);
    mVal0 = unique_ptr<uword>{new uword[n]};
    mVal1 = unique_ptr<uword>{new uword[n]};
    for ( int i = 0; i < n; ++ i ) {
      mVal0.get()[i] = src.mVal0.get()[i];
      mVal1.get()[i] = src.mVal1.get()[i];
    }
  }
  return *this;
}

// ムーブ代入演算子
BitVector&
BitVector::operator=(
  BitVector&& src
)
{
  mSize = src.mSize;
  mFlags = src.mFlags;
  mVal0 = std::move(src.mVal0);
  mVal1 = std::move(src.mVal1);
  return *this;
}

// ビット長の変換を行うコピーコンストラクタもどき
BitVector::BitVector(
  const BitVector& src,
  SizeType size
)
{
  set(src.mVal0.get(), src.mVal1.get(), src.size(), size, true,
      src.is_signed(), src.base());
}

// ビット長の変換と属性の変更を行うコピーコンストラクタもどき
BitVector::BitVector(
  const BitVector& src,
  SizeType size,
  bool is_sized,
  bool is_signed,
  SizeType base
)
{
  set(src.mVal0.get(), src.mVal1.get(), src.size(), size, is_sized, is_signed, base);
}

// @brief スカラ値からの代入演算子
BitVector&
BitVector::operator=(
  const VlScalarVal& value
)
{
  resize(1);
  set_type(true, false, 2);

  if ( value.is_zero() ) {
    mVal0.get()[0] = 1;
    mVal1.get()[0] = 0;
  }
  else if ( value.is_one() ) {
    mVal0.get()[0] = 0;
    mVal1.get()[0] = 1;
  }
  else if ( value.is_x() ) {
    mVal0.get()[0] = 1;
    mVal1.get()[0] = 1;
  }
  else if ( value.is_z() ) {
    mVal0.get()[0] = 0;
    mVal1.get()[0] = 0;
  }
  else {
    ASSERT_NOT_REACHED;
  }
  return *this;
}

// @brief ビット長の変換と属性の変更を行う代入演算子もどき
void
BitVector::set_with_attr(
  const BitVector& src,
  SizeType size,
  bool is_sized,
  bool is_signed,
  SizeType base
)
{
  set(src.mVal0.get(), src.mVal1.get(), src.size(), size, is_sized, is_signed, base);
}

// Verilog-HDL (IEEE1364-2001) の形式の文字列からの値をセットする．
bool
BitVector::set_from_verilog_string(
  const string& str
)
{
  SizeType size = BLOCK_SIZE; // デフォルト値
  bool is_sized = false; // デフォルト値
  bool is_signed = true; // デフォルト値
  SizeType base = 10; // デフォルト値

  // ビット長を求める．
  string::size_type pos = str.find_first_of('\'');
  if ( pos != string::npos ) {
    if ( pos != 0 ) {
      // ' よりも前の部分はビット長を表す文字列
      string lenstr = str.substr(0, pos);
      size = atoi(lenstr.c_str()); // 手抜き
      is_sized = true;
    }
    pos ++; // ' の次の位置を指す．

    // 基数を求める．
    is_signed = false;
    switch ( str[pos] ) {
    case 'b':
    case 'B': base =  2; pos ++; break;
    case 'o':
    case 'O': base =  8; pos ++; break;
    case 'd':
    case 'D': base = 10; pos ++; break;
    case 'h':
    case 'H': base = 16; pos ++; break;
    case 's':
    case 'S':
      {
	pos ++;
	is_signed = true;
	switch ( str[pos] ) {
	case 'b':
	case 'B': base =  2; pos ++; break;
	case 'o':
	case 'O': base =  8; pos ++; break;
	case 'd':
	case 'D': base = 10; pos ++; break;
	case 'h':
	case 'H': base = 16; pos ++; break;
	default:
	  // 本当は例外を投げるのがいいな．
	  cerr << "illegal character (" << str[pos] << ") in string "
	       << str << endl;
	  return false;
	}
      }
      break;
    default:
      // 本当は例外を投げるのがいいな．
      cerr << "illegal character (" << str[pos] << ") in string "
	   << str << endl;
      return false;
    }
  }
  else {
    pos = 0;
  }

  switch ( base ) {
  case  2: set_from_binstring(size, is_sized, is_signed, str, 0); break;
  case  8: set_from_octstring(size, is_sized, is_signed, str, 0); break;
  case 10: set_from_decstring(size, is_sized, is_signed, str, 0); break;
  case 16: set_from_hexstring(size, is_sized, is_signed, str, 0); break;
  default: cerr << "illegal base : " << base << endl;
  }

  return true;
}

// @brief 型変換を行う．
const BitVector&
BitVector::coerce(
  const VlValueType& type
)
{
  if ( !type.is_no_type() ) {
    bool is_signed = type.is_signed();
    bool is_sized = type.is_sized();
    int req_size = type.size();

    if ( size() == req_size ) {
      set_type(is_sized, is_signed, base());
    }
    else {
      set(mVal0.get(), mVal1.get(), size(), req_size, is_sized, is_signed, base());
    }
  }

  return *this;
}

// 0 を表すオブジェクトを生成する
BitVector
BitVector::zero(
  SizeType size
)
{
  return BitVector{VlScalarVal::zero(), size};
}

// 1 を表すオブジェクトを生成する
BitVector
BitVector::one(
  SizeType size
)
{
  return BitVector{VlScalarVal::one(), size};
}

// X を表すオブジェクトを生成する
BitVector
BitVector::x(
  SizeType size
)
{
  return BitVector{VlScalarVal::x(), size};
}

// Z を表すオブジェクトを生成する
BitVector
BitVector::z(
  SizeType size
)
{
  return BitVector{VlScalarVal::z(), size};
}

// Verilog 形式の2進数から変換するための共通ルーティン
void
BitVector::set_from_binstring(
  SizeType size,
  bool is_sized,
  bool is_signed,
  const string& str,
  int pos
)
{
  // 文字列の表している値を求める．
  // 実際に格納するものはサイズなどの属性が異なるので set() を用いる．
  string::size_type end = str.size();
  SizeType src_size = end - pos;
  SizeType src_n = block(src_size);
  uword val0[src_n];
  uword val1[src_n];
  for ( SizeType i = 0; i < src_n; ++ i ) {
    val0[i] = ALL1;
    val1[i] = ALL0;
  }
  SizeType vpos = src_size - 1;
  for ( ; pos < end; pos ++, vpos -- ) {
    SizeType blk = vpos / BLOCK_SIZE;
    SizeType sft = vpos - blk * BLOCK_SIZE;
    uword ppat = 1 << sft;
    uword npat = ~ppat;
    char c = str[pos];
    if ( c == '0' ) {
      val0[blk] |= ppat;
      val1[blk] &= npat;
    }
    else if ( c == '1' ) {
      val0[blk] &= npat;
      val1[blk] |= ppat;
    }
    else if ( c == 'x' || c == 'X' ) {
      val0[blk] |= ppat;
      val1[blk] |= ppat;
    }
    else if ( c == 'z' || c == 'Z' || c == '?' ) {
      val0[blk] &= npat;
      val1[blk] &= npat;
    }
    else {
      // 本当は例外を投げるのがいいな．
      cerr << "illegal character (" << c << ") in string "
	   << str << endl;
      return;
    }
  }

  // この文字列の先頭に - はつかないので結果は必ず非負の数だが，
  // MSBが1の場合にこれをそのまま符号つき数とみなすと符号拡張して
  // しまうのでいったん符号なし数として拡張した後で符号付きに変えている．
  set(val0,val1, src_size, size, is_sized, false, 2);
  if ( is_signed ) {
    set_type(is_sized, true, 2);
  }
}

// Verilog 形式の8進数から変換するための共通ルーティン
void
BitVector::set_from_octstring(
  SizeType size,
  bool is_sized,
  bool is_signed,
  const string& str,
  int pos
)
{
  // 文字列の表している値を求める．
  // 実際に格納するものはサイズなどの属性が異なるので set() を用いる．
  string::size_type end = str.size();
  SizeType src_size = (end - pos) * 3;
  SizeType src_n = block(src_size);
  uword val0[src_n];
  uword val1[src_n];
  for ( SizeType i = 0; i < src_n; ++ i ) {
    val0[i] = ALL1;
    val1[i] = ALL0;
  }
  SizeType vpos = src_size - 3;
  for ( ; pos < end; pos ++, vpos -= 3 ) {
    SizeType blk = vpos / BLOCK_SIZE;
    SizeType sft = vpos - blk * BLOCK_SIZE;
    char c = str[pos];
    if ( '0' <= c && c <= '7' ) {
      int val = c - '0';
      uword lppat = val << sft;
      uword rppat = val >> (BLOCK_SIZE - sft);
      uword lnpat = ~lppat;
      uword rnpat = ~rppat;
      val0[blk] &= lnpat;
      val1[blk] |= lppat;
      if ( rppat ) {
	val0[blk + 1] &= rnpat;
	val1[blk + 1] |= rppat;
      }
    }
    else if ( c == 'x' || c == 'X' ) {
      uword lppat = 7 << sft;
      uword rppat = 7 >> (BLOCK_SIZE - sft);
      val0[blk] |= lppat;
      val1[blk] |= lppat;
      if ( rppat ) {
	val0[blk + 1] |= rppat;
	val1[blk + 1] |= rppat;
      }
    }
    else if ( c == 'z' || c == 'Z' || c == '?' ) {
      uword lnpat = ~(7 << sft);
      uword rnpat = ~(7 >> (BLOCK_SIZE - sft));
      val0[blk] &= lnpat;
      val1[blk] &= lnpat;
      if ( rnpat != ALL1 ) {
	val0[blk + 1] &= rnpat;
	val1[blk + 1] &= rnpat;
      }
    }
    else {
      // 本当は例外を投げるのがいいな．
      cerr << "illegal character (" << c << ") in string "
	   << str << endl;
      return;
    }
  }

  // この文字列の先頭に - はつかないので結果は必ず非負の数だが，
  // MSBが1の場合にこれをそのまま符号つき数とみなすと符号拡張して
  // しまうのでいったん符号なし数として拡張した後で符号付きに変えている．
  set(val0, val1, src_size, size, is_sized, false, 8);
  if ( is_signed ) {
    set_type(is_sized, true, 8);
  }
}

// Verilog 形式の10進数から変換するための共通ルーティン
void
BitVector::set_from_decstring(
  SizeType size,
  bool is_sized,
  bool is_signed,
  const string& str,
  int pos
)
{
  // 文字列の表している値を求める．
  // 実際に格納するものはサイズなどの属性が異なるので set() を用いる．
  vector<uword> val0;
  vector<uword> val1;
  SizeType src_size = 0;
  string::size_type end = str.size();
  const SizeType sft = BLOCK_SIZE - 4;
  for ( ; pos < end; ++ pos ) {
    char c = str[pos];
    if ( c < '0' || '9' < c ) {
      // 本当は例外を投げるのがいいな．
      cerr << "illegal character (" << c << ") in string "
	   << str << endl;
      return;
    }
    else {
      SizeType n = val1.size();
      uword carry = c - '0';
      for ( SizeType i = 0; i < n; ++ i ) {
	uword tmp = val1[i];
	uword u = tmp >> sft;
	uword l = tmp - (u << sft);
	uword u10 = u * 10;
	uword l10 = l * 10;
	val1[i] = carry + l10 + ((u10 % 16) << sft);
	val0[i] = ~val1[i];
	carry = u10 / 16;
      }
      if ( carry ) {
	val1.push_back(carry);
	val0.push_back(~carry);
      }
    }
  }
  SizeType last = val1.size();
  if ( last ) {
    last --;
    for ( int vpos = BLOCK_SIZE; vpos -- > 0; ) {
      if ( val1[last] & (1 << vpos) ) {
	src_size = vpos + last * BLOCK_SIZE + 1;
	break;
      }
    }
  }
  else {
    // 1ビットの0を入れておく
    val1.push_back(0);
    val0.push_back(1);
    src_size = 1;
  }

  // この文字列の先頭に - はつかないので結果は必ず非負の数だが，
  // MSBが1の場合にこれをそのまま符号つき数とみなすと符号拡張して
  // しまうのでいったん符号なし数として拡張した後で符号付きに変えている．
  set(val0, val1, src_size, size, is_sized, false, 10);
  if ( is_signed ) {
    set_type(is_sized, true, 10);
  }
}

// Verilog 形式の16進数から変換するための共通ルーティン
void
BitVector::set_from_hexstring(
  SizeType size,
  bool is_sized,
  bool is_signed,
  const string& str,
  int pos
)
{
  // 文字列の表している値を求める．
  // 実際に格納するものはサイズなどの属性が異なるので set() を用いる．
  string::size_type end = str.size();
  SizeType src_size = (end - pos) * 4;
  SizeType src_n = block(src_size);
  uword val0[src_n];
  uword val1[src_n];
  for ( SizeType i = 0; i < src_n; ++ i ) {
    val0[i] = ALL1;
    val1[i] = ALL0;
  }
  SizeType vpos = src_size - 4;
  for ( ; pos < end; pos ++, vpos -= 4) {
    SizeType blk = vpos / BLOCK_SIZE;
    SizeType sft = vpos - blk * BLOCK_SIZE;
    char c = str[pos];
    if ( '0' <= c && c <= '9' ) {
      uword val = c - '0';
      uword ppat = val << sft;
      uword npat = ~ppat;
      val0[blk] &= npat;
      val1[blk] |= ppat;
    }
    else if ( 'a' <= c && c <= 'f' ) {
      uword val = c - 'a' + 10;
      uword ppat = val << sft;
      uword npat = ~ppat;
      val0[blk] &= npat;
      val1[blk] |= ppat;
    }
    else if ( 'A' <= c && c <= 'F' ) {
      uword val = c - 'A' + 10;
      uword ppat = val << sft;
      uword npat = ~ppat;
      val0[blk] &= npat;
      val1[blk] |= ppat;
    }
    else if ( c == 'x' || c == 'X' ) {
      uword ppat = 15 << sft;
      val0[blk] |= ppat;
      val1[blk] |= ppat;
    }
    else if ( c == 'z' || c == 'Z' || c == '?' ) {
      uword npat = ~(15 << sft);
      val0[blk] &= npat;
      val1[blk] &= npat;
    }
    else {
      // 本当は例外を投げるのがいいな．
      cerr << "illegal character (" << c << ") in string "
	   << str << endl;
      return;
    }
  }

  // この文字列の先頭に - はつかないので結果は必ず非負の数だが，
  // MSBが1の場合にこれをそのまま符号つき数とみなすと符号拡張して
  // しまうのでいったん符号なし数として拡張した後で符号付きに変えている．
  set(val0, val1, src_size, size, is_sized, false, 16);
  if ( is_signed ) {
    set_type(is_sized, true, 16);
  }
}

// 文字列からの変換用コンストラクタの共通ルーティン
void
BitVector::set_from_string(
  SizeType strsize,
  const char* str
)
{
  SizeType s = strsize * 8;

  resize(s);
  set_type(true, false, 2);

  int j = 0;
  int k = 0;
  uword tmp = 0;
  for ( int i = strsize; i -- > 0; ) {
    char c = str[i];
    tmp += (static_cast<uword>(c) << (k * 8));
    ++ k;
    if ( k == 4 ) {
      mVal0.get()[j] = ~tmp;
      mVal1.get()[j] =  tmp;
      ++ j;
      k = 0;
      tmp = 0;
    }
  }
  if ( k != 0 ) {
    uword mask = ALL1 << (k * 8);
    mVal0.get()[j] = ~tmp | mask;
    mVal1.get()[j] = tmp;
  }
}

END_NAMESPACE_YM_VERILOG
