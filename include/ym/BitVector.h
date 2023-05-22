#ifndef YM_BITVECTOR_H
#define YM_BITVECTOR_H

/// @file ym/BitVector.h
/// @brief BitVector のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014, 2020, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym/verilog.h"
#include "ym/VlValueType.h"
#include "ym/VlScalarVal.h"
#include "ym/VlTime.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class BitVector BitVector.h <ym/BitVector.h>
/// @ingroup VlCommon
/// @brief Verilog-HDL のビットベクタ値を表すクラス
///
/// 各ビットは 0, 1, X, Z の4値をとりうる
///
/// それ以外の属性としては
///   - サイズ
///   - 符号の有無
///   - 基数
/// を持つ．
//////////////////////////////////////////////////////////////////////
class BitVector
{
public:

  //////////////////////////////////////////////////////////////////////
  /// @name コンストラクタ/デストラクタ/代入演算子
  /// @{

  /// @brief unsigned int からのコンストラクタ
  ///
  /// 空のコンストラクタでもある<br>
  /// 結果の型は
  /// - サイズは無し
  /// - 符号なし
  /// - 基数は10
  explicit
  BitVector(
    unsigned int val = 0 ///< [in] 値
  );

  /// @brief SizeType からのコンストラクタ
  ///
  /// 結果の型は
  /// - サイズは無し
  /// - 符号なし
  /// - 基数は10
  explicit
  BitVector(
    SizeType val ///< [in] 値
  );

  /// @brief int からのキャスト用コンストラクタ
  ///
  /// 結果の型は
  /// - サイズは無し
  /// - 符号あり
  /// - 基数は10
  ///
  explicit
  BitVector(
    int val ///< [in] 値
  );

  /// @brief bool からの変換コンストラクタ
  ///
  /// 結果の型は
  /// - サイズはあり(1ビット)
  /// - 符号なし
  /// - 基数は2
  ///
  explicit
  BitVector(
    bool value ///< [in] ブール値
  );

  /// @brief time 型からの変換コンストラクタ
  ///
  /// 結果の型は
  /// - サイズはあり(64ビット)
  /// - 符号なし
  /// - 基数は10
  explicit
  BitVector(
    VlTime time ///< [in] time 値
  );

  /// @brief スカラ値からのキャスト用コンストラクタ
  ///
  /// size を指定するとその数分 value を繰り返す．<br>
  /// 結果の型は
  /// - サイズはあり(= size)
  /// - 符号なし
  /// - 基数は2
  explicit
  BitVector(
    const VlScalarVal& value, ///< [in] 値 (kVpiScalar{0, 1, X, Z}
    SizeType size = 1         ///< [in] サイズ (ビット幅)
  );

  /// @brief C文字列からの変換用コンストラクタ
  ///
  /// 結果の型は
  /// - サイズはあり
  /// - 符号なし
  /// - 基数は2
  explicit
  BitVector(
    const char* str ///< [in] 文字列 (C文字列)
  );

  /// @brief string 文字列からの変換用コンストラクタ
  ///
  /// 結果の型は
  /// - サイズはあり
  /// - 符号なし
  /// - 基数は2
  explicit
  BitVector(
    const string& str ///< [in] 文字列 (string)
  );

  /// @brief 浮動小数点数をビットベクタにする
  /// @warning 整数で表せない範囲の場合には値は不定
  /// @todo 整数を経由しない方法に書き換えること
  explicit
  BitVector(
    double val ///< [in] 浮動小数点数
  );

  /// @brief Verilog-HDL 形式の文字列からの変換コンストラクタ
  ///
  /// とはいってもサイズと基数は str に含まれていない
  BitVector(
    SizeType size,    ///< [in] サイズ
    bool is_signed,   ///< [in] 符号の有無を表すフラグ
    SizeType base,    ///< [in] 基数を表す数字 (2, 8, 10, 16 のみが妥当な値)
    const string& str ///< [in] 値の内容を表すVerilog-HDL形式の文字列
  );

  /// @brief 連結演算用のコンストラクタ
  ///
  /// src_list の内容を連結したものをセットする
  BitVector(
    const vector<BitVector>& src_list ///< [in] 連結する値のリスト
  );

  /// @brief コピーコンストラクタ
  BitVector(
    const BitVector& src ///< [in] コピー元のオブジェクト
  );

  /// @brief ムーブコンストラクタ
  BitVector(
    BitVector&& src ///< [in] ムーブ元のオブジェクト
  );

  /// @brief ビット長の変換を行うコピーコンストラクタもどき
  ///
  /// 変換ルールは以下の通り
  /// - src のビット長が size よりも短ければ適切な拡張を行う．
  ///   * unsigned なら上位に0を補う．signed なら符号拡張を行う．
  ///   * MSB が x か z の場合には x または z を補う．
  /// - src のビット長が size よりも長ければ切り捨てる．
  BitVector(
    const BitVector& src, ///< [in] 返還元ののビットベクタ
    SizeType size         ///< [in] 指定サイズ
  );

  /// @brief ビット長の変換と属性の変更を行うコピーコンストラクタもどき
  BitVector(
    const BitVector& src, ///< [in] 変換元のオブジェクト
    SizeType size,        ///< [in] 指定サイズ
    bool is_sized,        ///< [in] サイズの有無
    bool is_signed,       ///< [in] 符号の有無
    SizeType base         ///< [in] 基数
  );

  /// @brief コピー代入演算子
  /// @return 代入後の自分自身の参照を返す．
  BitVector&
  operator=(
    const BitVector& src ///< [in] コピー元のオブジェクト
  );

  /// @brief ムーブ代入演算子
  /// @return 代入後の自分自身の参照を返す．
  BitVector&
  operator=(
    BitVector&& src ///< [in] ムーブ元のオブジェクト
  );

  /// @brief 符号なし整数からの代入演算子
  /// @return 代入後の自分自身の参照を返す．
  ///
  /// 結果の型は
  /// - サイズは無し
  /// - 符号なし
  /// - 基数は10
  BitVector&
  operator=(
    unsigned int val ///< [in] 値
  );

  /// @brief int からの代入演算子
  /// @return 代入後の自分自身の参照を返す．
  ///
  /// 結果の型は
  /// - サイズは無し
  /// - 符号あり
  /// - 基数は10
  BitVector&
  operator=(
    int val ///< [in] 値
  );

  /// @brief bool からの代入演算子
  /// @return 代入後の自分自身の参照を返す．
  ///
  /// 結果の型は
  /// - サイズはあり(1ビット)
  /// - 符号なし
  /// - 基数は2
  BitVector&
  operator=(
    bool value ///< [in] 値
  );

  /// @brief time 型からの代入演算子
  /// @return 代入後の自分自身の参照を返す．
  ///
  /// 結果の型は
  /// - サイズはあり(64ビット)
  /// - 符号なし
  /// - 基数は10
  BitVector&
  operator=(
    VlTime time ///< [in] time 値
  );

  /// @brief スカラ値からの代入演算子
  /// @return 代入後の自分自身の参照を返す．
  ///
  /// 結果の型は
  /// - サイズはあり(1ビット)
  /// - 符号なし
  /// - 基数は2
  BitVector&
  operator=(
    const VlScalarVal& value ///< [in] 値 (0, 1, X, Z)
  );

  /// @brief C文字列からの代入演算子
  /// @return 代入後の自分自身の参照を返す．
  ///
  /// 結果の型は
  /// - サイズはあり
  /// - 符号なし
  /// - 基数は2
  BitVector&
  operator=(
    const char* str ///< [in] C文字列
  );

  /// @brief string 文字列からの代入演算子
  /// @return 代入後の自分自身の参照を返す．
  ///
  /// 結果の型は
  /// - サイズはあり
  /// - 符号なし
  /// - 基数は2
  BitVector&
  operator=(
    const string& str ///< [in] 文字列 (string)
  );

  /// @brief 浮動小数点数からの代入演算子
  /// @return 代入後の自分自身の参照を返す．
  /// @warning 整数で表せない範囲の場合には値は不定
  /// @todo 整数を経由しない方法に書き換えること
  BitVector&
  operator=(
    double val ///< [in] 浮動小数点数
  );

  /// @brief ビット長の変換と属性の変更を行う代入演算子もどき
  void
  set_with_attr(
    const BitVector& src, ///< [in] 変換元のオブジェクト
    SizeType size,        ///< [in] 指定サイズ
    bool is_sized,        ///< [in] サイズの有無
    bool is_signed,       ///< [in] 符号の有無
    SizeType base         ///< [in] 基数
  );

  /// @brief Verilog-HDL (IEEE1364-2001) 形式の文字列から値をセットする関数．
  /// @retval true 正しく変換できた
  /// @retval false str が形式に沿っていなかった
  ///
  /// もちろん IEEE1364-1995 の形式も OK
  bool
  set_from_verilog_string(
    const string& str ///< [in] Verilog形式の文字列
  );

  /// @brief 型変換を行う．
  /// @return 自分自身への参照を返す．
  const BitVector&
  coerce(
    const VlValueType& type ///< [in] 要求される型(サイズも含む)
  );

  /// @}
  //////////////////////////////////////////////////////////////////////


  //////////////////////////////////////////////////////////////////////
  /// @name スカラ値の繰り返しを作るクラスメソッド
  /// 上のコンストラクタを使うためには VlScalarVal の値を指定しなければならず
  /// かっこ悪いので
  /// @{

  /// @brief 0 を表すオブジェクトを生成する
  /// @return 生成されたオブジェクト
  static
  BitVector
  zero(
    SizeType size = 1 ///< [in] ビット数．デフォルトは 1
  );

  /// @brief 1 を表すオブジェクトを生成する
  /// @return 生成されたオブジェクト
  static
  BitVector
  one(
    SizeType size = 1 ///< [in] ビット数．デフォルトは 1
  );

  /// @brief X を表すオブジェクトを生成する
  /// @return 生成されたオブジェクト
  static
  BitVector
  x(
    SizeType size = 1 ///< [in] ビット数．デフォルトは 1
  );

  /// @brief Z を表すオブジェクトを生成する
  /// @return 生成されたオブジェクト
  static
  BitVector
  z(
    SizeType size = 1 ///< [in] ビット数．デフォルトは 1
  );

  /// @}
  //////////////////////////////////////////////////////////////////////


  //////////////////////////////////////////////////////////////////////
  // 以下の演算は IEEE1364-2001 section 4.1 準拠
  //////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////
  /// @name Arithmetic member operators (IEEE1364-2001 4.1.5)
  /// 結果の型は以下のように決定される．
  /// - 符号:
  ///   - ともに signed -> signed
  ///   - 上記以外      -> unsigned
  /// - size:
  ///   unsized のオペランドは integer とおなじとみなす．
  ///   で，2つのオペランドの size の大きい方に合わせる．
  ///   ただし，どちらも unsized なら結果も unsized
  /// - 基数: なし(10)
  /// @{

  /// @brief 2の補数を計算し自分自身に代入する．
  /// @return 自分自身を返す．
  const BitVector&
  complement();

  /// @brief 加算つき代入
  /// @return 自分自身を返す．
  ///
  /// 自分自身に src を足したものを代入する
  const BitVector&
  operator+=(
    const BitVector& src ///< [in] オペランド
  );

  /// @brief 減算つき代入
  /// @return 自分自身を返す．
  ///
  /// 自分自身から src を引いたものを代入する
  const BitVector&
  operator-=(
    const BitVector& src ///< [in] オペランド
  );

  /// @brief 乗算つき代入
  /// @return 自分自身を返す．
  ///
  /// 自分自身に src を掛けたものを代入する
  const BitVector&
  operator*=(
    const BitVector& src ///< [in] オペランド
  );

  /// @brief 除算つき代入
  /// @return 自分自身を返す．
  ///
  /// 自分自身を src で割ったものを代入する
  const BitVector&
  operator/=(
    const BitVector& src ///< [in] オペランド
  );

  /// @brief 剰余算つき代入
  /// @return 自分自身を返す．
  ///
  /// 自分自身を src で割った余りを代入する
  const BitVector&
  operator%=(
    const BitVector& src ///< [in] オペランド
  );

  /// 巾乗つき代入
  /// @return 自分自身を返す．
  ///
  /// 自分自身を src 乗したものを代入する
  const BitVector&
  power(
    const BitVector& src ///< [in] オペランド
  );

  /// @}
  //////////////////////////////////////////////////////////////////////


  //////////////////////////////////////////////////////////////////////
  /// @name Bit-wise logical member operators (IEEE1364-2001 4.1.10)
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

  /// @brief NOT
  /// @return 自分自身を返す．
  ///
  /// 自分自身の値をビットごとに反転させる
  const BitVector&
  negate();

  /// @brief AND つき代入
  /// @return 自分自身を返す．
  ///
  /// 自分自身と src との bit-wise AND を代入する
  const BitVector&
  operator&=(
    const BitVector& src ///< [in] オペランド
  );

  /// @brief Or つき代入
  /// @return 自分自身を返す．
  ///
  /// 自分自身と src との OR を代入する
  const BitVector&
  operator|=(
    const BitVector& src ///< [in] オペランド
  );

  /// @brief XOR つき代入
  /// @return 自分自身を返す．
  ///
  /// 自分自身と src との XOR を代入する
  const BitVector&
  operator^=(
    const BitVector& src ///< [in] オペランド
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
  VlScalarVal
  reduction_and() const;

  /// @brief リダクションOR
  /// @return 演算結果を返す．
  ///
  /// すべてのビットの OR を計算する
  VlScalarVal
  reduction_or() const;

  /// @brief リダクションXOR
  /// @return 演算結果を返す．
  ///
  /// すべてのビットの XOR を計算する
  VlScalarVal
  reduction_xor() const;

  /// @brief リダクションNAND
  /// @return 演算結果を返す．
  ///
  /// すべてのビットの NAND を計算する
  VlScalarVal
  reduction_nand() const;

  /// @brief リダクションNOR
  /// @return 演算結果を返す．
  ///
  /// すべてのビットの NOR を計算する
  VlScalarVal
  reduction_nor() const;

  /// @brief リダクションXNOR
  /// @return 演算結果を返す．
  ///
  /// すべてのビットの XNOR を計算する
  VlScalarVal
  reduction_xnor() const;

  /// @}
  //////////////////////////////////////////////////////////////////////


  //////////////////////////////////////////////////////////////////////
  /// @name Shift member operators (IEEE1364-2001 4.1.12)
  /// 結果の型は以下のとおり
  /// - 符号   : 第1オペランドと同じ
  /// - サイズ : 第1オペランドと同じ
  /// - 基数   : 第1オペランドと同じ
  /// @{

  /// @brief 論理左シフトつき代入
  /// @return 自分自身を返す．
  ///
  /// 自分自身を src だけ論理左シフトしたものを代入する．
  const BitVector&
  operator<<=(
    const BitVector& src ///< [in] シフト量 (BitVector)
  );

  /// @brief 論理左シフトつき代入
  /// @return 自分自身を返す．
  ///
  /// 自分自身を src だけ論理左シフトしたものを代入する．
  const BitVector&
  operator<<=(
    int src ///< [in] シフト量 (int)
  );

  /// @brief 論理右シフトつき代入
  /// @return 自分自身を返す．
  ///
  /// 自分自身を src だけ論理右シフトしたものを代入する．
  const BitVector&
  operator>>=(
    const BitVector& src ///< [in] シフト量 (BitVector)
  );

  /// @brief 論理右シフトつき代入
  /// @return 自分自身を返す．
  ///
  /// 自分自身を src だけ論理右シフトしたものを代入する．
  const BitVector&
  operator>>=(
    int src ///< [in] シフト量 (int)
  );

  /// @brief 算術左シフトつき代入
  /// @return 自分自身を返す．
  ///
  /// 自分自身を src だけ算術左シフトしたものを代入する．
  const BitVector&
  alshift(
    const BitVector& src ///< [in] シフト量 (BitVector)
  )
  { // 実は論理左シフトそのもの
    return operator<<=(src);
  }

  /// @brief 算術左シフトつき代入
  /// @return 自分自身を返す．
  ///
  /// 自分自身を src だけ算術左シフトしたものを代入する．
  const BitVector&
  alshift(
    int src ///< [in] シフト量 (int)
  )
  { // 実は論理左シフトそのもの
    return operator<<=(src);
  }

  /// @brief 算術右シフトつき代入
  /// @return 自分自身を返す．
  ///
  /// 自分自身を src だけ算術右シフトしたものを代入する．
  const BitVector&
  arshift(
    const BitVector& src ///< [in] シフト量 (BitVector)
  );

  /// @brief 算術右シフトつき代入
  /// @return 自分自身を返す．
  ///
  /// 自分自身を src だけ算術右シフトしたものを代入する．
  const BitVector&
  arshift(
    int src ///< [in] シフト量 (int)
  );

  /// @}
  //////////////////////////////////////////////////////////////////////


  //////////////////////////////////////////////////////////////////////
  /// @name Vector bit-select and part-select addressing (IEEE1364-2001 4.2.1)
  /// 範囲外は x を返す．
  ///
  /// 結果の型は以下の通り
  /// - 符号   : 常に unsigned
  /// - サイズ : 指定された値
  /// - 基数   : 2
  /// @{

  /// @brief part-select 演算子
  /// @return 選択された範囲のビットベクタ
  BitVector
  part_select_op(
    int msb, ///< [in] 選択範囲の MSB
    int lsb  ///< [in] 選択範囲の LSB
  ) const;

  /// @brief part-select 書き込み
  ///
  /// - [msb:lsb] の範囲に val を書き込む
  /// - 範囲外ならなにもしない．
  void
  part_select_op(
    int msb,             ///< [in] 選択範囲の MSB
    int lsb,	         ///< [in] 選択範囲の LSB
    const BitVector& val ///< [in]書き込む値
  );

  /// @brief bit-select 演算子
  /// @return bpos で指定された位置の値
  VlScalarVal
  bit_select_op(
    int bpos ///< [in] 選択するビット位置
  ) const
  { // 範囲外のチェックは value() でやっている．
    return value(bpos);
  }

  /// @brief bit-select 書き込み
  ///
  /// - bpos の位置に val を書き込む．
  /// - 範囲外ならないもしない．
  void
  bit_select_op(
    int bpos,       ///< [in] 選択するビット位置
    VlScalarVal val ///< [in] 書き込む値
  );

  /// @}
  //////////////////////////////////////////////////////////////////////


  //////////////////////////////////////////////////////////////////////
  /// @name その他のメンバ関数
  /// @{

  /// @brief 値の型を返す．
  VlValueType
  value_type() const { return VlValueType(is_signed(), is_sized(), size()); }

  /// @brief srcの値をビットごとにマージする．
  /// @return 自分自身
  ///
  /// 異なっているビットは X となる．
  const BitVector&
  merge(
    const BitVector& src ///< [in] オペランド
  );

  /// @brief サイズを返す．
  SizeType
  size() const { return mSize; }

  /// @brief 実際のサイズはともかくサイズの指定があるかどうかを返す．
  bool
  is_sized() const { return mFlags[0]; }

  /// @brief 符号付きの場合に true を返す
  bool
  is_signed() const { return mFlags[1]; }

  /// @brief 表示用の基数を得る．
  ///
  /// 返り値は 2, 8, 10, 16 のいずれか．
  SizeType
  base() const
  {
    int v = static_cast<int>(mFlags[2]) * 2 + static_cast<int>(mFlags[3]);
    switch ( v ) {
    case 0: return 2;
    case 1: return 8;
    case 2: return 10;
    case 3: return 16;
    }
    ASSERT_NOT_REACHED;
    return 0;
  }

  /// @brief pos ビット目の値を得る．
  /// @return pos ビット目の値を返す．
  ///
  /// pos が範囲を越えていたら X を返す．
  VlScalarVal
  value(
    int pos ///< [in] 取得するビット位置
  ) const;

  /// @brief 符号付きかつ負数の時に true を返す
  bool
  is_negative() const
  {
    return is_signed() && value(size() - 1).is_one();
  }

  /// @brief x 値を含んでいたら true を返す
  bool
  has_x() const;

  /// @brief z 値を含んでいたら true を返す
  bool
  has_z() const;

  /// @brief x/z 値を含んでいたら true を返す
  bool
  has_xz() const;

  /// @brief z を x に変える．
  /// @note ほとんどの演算で z は x と区別されていない
  void
  z_to_x();

  /// @brief x/z を 0 に変える．
  /// @note Verilog-HDL で4値を2値に強引に変換するときのやり方
  void
  xz_to_0();

  /// @brief 32 ビットの符合なし数に変換可能なら true を返す．
  ///
  /// 具体的には size() が 32以下で x や z を含んでいないこと
  bool
  is_uint32() const { return (size() <= BLOCK_SIZE && !has_xz()); }

  /// @brief is_uint32 の条件を満たしているときに std::uint32_t に変換する．
  ///
  /// 上の条件を満たしていないときの値は不定
  /// (というか実際にどういう値を返すのかはソースコードを見ればわかる)
  std::uint32_t
  to_uint32() const { return static_cast<std::uint32_t>(mVal1.get()[0]); }

  /// @brief int の数値に変換可能なら true を返す．
  ///
  /// 実際には is_uint32() と同一
  bool
  is_int() const { return (size() <= BLOCK_SIZE && !has_xz()); }

  /// @brief is_int の条件を満たしているときに int に変換する．
  ///
  /// 上の条件を満たしていないときの値は不定
  /// (というか実際にどういう値を返すのかはソースコードを見ればわかる)
  int
  to_int() const { return static_cast<int>(mVal1.get()[0]); }

  /// @brief 値を double 型に変換する．
  ///
  /// X/Z は0と見なす．
  double
  to_real() const;

  /// @brief 値をバイトベクターと見なして文字列に変換する．
  ///
  /// 長さが8の倍数でない場合にはパディングする．
  string
  to_string() const;

  /// @brief 1ビットのスカラー値に変換する．
  ///
  /// 実際には LSB を返すだけ．
  VlScalarVal
  to_scalar() const { return value(0); }

  /// @brief 論理値として評価する．
  /// @retval 0 0 の時
  /// @retval 1 0 以外の確定値の時
  /// @retval X 不定値を1ビットでも含む場合
  VlScalarVal
  to_logic() const;

  /// @brief 内容をブール値に変換する．
  ///
  /// to_logic() と同様で，かつ X/Z を false と見なす．
  bool
  to_bool() const { return to_logic().is_one(); }

  /// @brief time 型に変換可能なら true を返す．
  ///
  /// 具体的には size() が 64 以下で X/Z を含んでいないこと
  bool
  is_time() const { return (size() <= 64 && !has_xz()); }

  /// @brief 内容を time 型に変換する．
  ///
  /// 具体的には下位64ビットを設定するだけなので
  /// オーバーフローしているときには値は正しくない．
  VlTime
  to_time() const
  {
    PLI_UINT32 l = static_cast<PLI_UINT32>(mVal1.get()[0]);
    PLI_UINT32 h = static_cast<PLI_UINT32>(mVal1.get()[1]);
    return VlTime(l, h);
  }

  /// @brief 内容を Verilog-HDL (IEEE1364-2001) の形式の文字列に変換する．
  /// @return Verilog-HDL 形式の文字列表現を返す．
  ///
  /// opt_base が 2, 8, 10, 16 の時には内部で持っている基数を無視して
  /// opt_base を基数と見なす．
  string
  verilog_string(
    SizeType opt_base = 0 ///< [in] 基数
  ) const;

  /// @brief 内容を10進数で表した文字列を返す．
  string
  dec_str() const;

  /// @brief 内容を2進数で表した文字列を返す．
  /// @return 内容を2進数で表した文字列を返す．
  ///
  /// skip_zeros が true なら上位は0は出力しない．
  string
  bin_str(
    bool skip_zeros = true ///< [in] 0スキップフラグ
  ) const;

  /// @brief 内容を8進数で表した文字列を返す．
  /// @return 内容を8進数で表した文字列を返す．
  ///
  /// skip_zeros が true なら上位は0は出力しない．
  string
  oct_str(
    bool skip_zeros = true ///< [in] 0スキップフラグ
  ) const;

  /// @brief 内容を16進数で表した文字列を返す．
  /// @return 内容を16進数で表した文字列を返す．
  ///
  /// skip_zeros が true なら上位は0は出力しない．
  string
  hex_str(
    bool skip_zeros = true ///< [in] 0スキップフラグ
  ) const;

  /// @}
  //////////////////////////////////////////////////////////////////////


public:

  //////////////////////////////////////////////////////////////////////
  /// @name 比較演算用の関数
  /// @{

  /// @brief 大小比較演算用の共通関数
  /// @return src1 < src2 の時 true を返す．
  static
  bool
  lt_base(
    const BitVector& src1, ///< [in] 第1オペランド
    const BitVector& src2  ///< [in] 第2オペランド
  );

  /// @brief 等価比較演算用の共通関数
  /// @return 等しいと見なすとき true を返す．
  ///
  /// モードの意味は以下の通り
  ///   - 1 普通の等価比較
  ///   - 2 x をワイルドカードと見なす等価比較
  ///   - 3 x と z をワイルドカードと見なす等価比較
  static
  bool
  eq_base(
    const BitVector& src1, ///< [in] 第1オペランド
    const BitVector& src2, ///< [in] 第2オペランド
    int mode               ///< [in] モード
  );

  /// @}
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  // ビットベクタ用の符号なし整数
  using uword = std::uint64_t;

  // mVal0, mVal1 のリサイズをする．
  void
  resize(
    SizeType size
  );

  // 属性(サイズの有無, 符号の有無, 基数)をセットする．
  void
  set_type(
    bool has_size,
    bool has_sign,
    SizeType base
  )
  {
    mFlags[0] = has_size;
    mFlags[1] = has_sign;
    mFlags[2] = (base == 10) || (base == 16);
    mFlags[3] = (base == 16) || (base == 8);
  }

  // 値をセットする．(1ワードバージョン)
  void
  set(
    uword val0,
    uword val1,
    SizeType size,
    bool is_sized,
    bool is_signed,
    SizeType base
  );

  // 値をセットする．(ベクタバージョン)
  void
  set(
    const uword* val0,
    const uword* val1,
    SizeType src_size,
    SizeType size,
    bool is_sized,
    bool is_signed,
    SizeType base
  );

  // 値をセットする．(ベクタバージョン)
  void
  set(
    const vector<uword>& val0,
    const vector<uword>& val1,
    SizeType src_size,
    SizeType size,
    bool is_sized,
    bool is_signed,
    SizeType base
  );

  // Verilog 形式の2進数から変換するための共通ルーティン
  void
  set_from_binstring(
    SizeType size,
    bool is_sized,
    bool is_signed,
    const string& str,
    int pos
  );

  // Verilog 形式の8進数から変換するための共通ルーティン
  void
  set_from_octstring(
    SizeType size,
    bool is_sized,
    bool is_signed,
    const string& str,
    int pos
  );

  // Verilog 形式の10進数から変換するための共通ルーティン
  void
  set_from_decstring(
    SizeType size,
    bool is_sized,
    bool is_signed,
    const string& str,
    int pos
  );

  // Verilog 形式の16進数から変換するための共通ルーティン
  void
  set_from_hexstring(
    SizeType size,
    bool is_sized,
    bool is_signed,
    const string& str,
    int pos
  );

  // 文字列からの変換用コンストラクタの共通ルーティン
  void
  set_from_string(
    SizeType strsize,
    const char* str
  );

  /// @brief src を 10 で割る．
  /// @return 余りを返す．
  static
  int
  div10(
    const uword* src, ///< [in] 元の値
    int n,            ///< [in]
    uword* q          ///< [out] 求まった商を格納する変数
  );

  /// @brief val を10進数で表した文字列を返す．
  static
  string
  dec_str_sub(
    const uword* val,
    int n
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ビット長
  SizeType mSize{0U};

  // sized, signed, base をパックした変数
  // base は 2ビットを使って 2, 8, 10, 16 を符号化する．
  bitset<4> mFlags{0};

  // 値を保持するベクタ
  // サイズは block(mSize)
  // mVal0:Val1 の組み合わせで値を表す．
  unique_ptr<uword> mVal0;
  unique_ptr<uword> mVal1;


public:
  //////////////////////////////////////////////////////////////////////
  // 定数
  //////////////////////////////////////////////////////////////////////

  /// @brief uword のビット長
  static
  const SizeType BLOCK_SIZE = sizeof(uword) * 8;

};


//////////////////////////////////////////////////////////////////////
/// @name Arithmetic operators (IEEE1364-2001 4.1.5)
/// 結果の型は以下のように決定される．
/// - 符号:
///   - ともに signed -> signed
///   - 上記以外      -> unsigned
/// - size:
///   - unsized のオペランドは integer とおなじとみなす．
///   - 2つのオペランドの size の大きい方に合わせる．
///   - ただし，どちらも unsized なら結果も unsized
/// - 基数はなし(10)
/// @{

/// @relates BitVector
/// @brief 単項のマイナス(complement)演算子
/// @return 2の補数を返す．
///
/// 2の補数を求める．
/// パタンとして2の補数を求めるだけで
/// 符号の有無は変わらない．
inline
BitVector
operator-(
  const BitVector& src ///< [in] オペランド
)
{
  return BitVector(src).complement();
}

/// @relates BitVector
/// @brief 加算
/// @return src1 + src2 を返す
inline
BitVector
operator+(
  const BitVector& src1, ///< [in] 第1オペランド
  const BitVector& src2  ///< [in] 第2オペランド
)
{
  return BitVector(src1) += src2;
}

/// @relates BitVector
/// @brief 減算
/// @return src1 - src2 を返す
inline
BitVector
operator-(
  const BitVector& src1, ///< [in] 第1オペランド
  const BitVector& src2  ///< [in] 第2オペランド
)
{
  return BitVector(src1) -= src2;
}

/// @relates BitVector
/// @brief 乗算
/// @return src1 * src2 を返す
inline
BitVector
operator*(
  const BitVector& src1, ///< [in] 第1オペランド
  const BitVector& src2  ///< [in] 第2オペランド
)
{
  return BitVector(src1) *= src2;
}

/// @relates BitVector
/// @brief 除算
/// @return src1 / src2 を返す
inline
BitVector
operator/(
  const BitVector& src1, ///< [in] 第1オペランド
  const BitVector& src2  ///< [in] 第2オペランド
)
{
  return BitVector(src1) /= src2;
}

/// @relates BitVector
/// @brief 剰余算
/// @return src1 % src2 を返す
inline
BitVector
operator%(
  const BitVector& src1, ///< [in] 第1オペランド
  const BitVector& src2  ///< [in] 第2オペランド
)
{
  return BitVector(src1) %= src2;
}

/// @relates BitVector
/// @brief 巾乗
/// @return src1 の src2 乗を返す
inline
BitVector
power(
  const BitVector& src1, ///< [in] 第1オペランド
  const BitVector& src2  ///< [in] 第2オペランド
)
{
  return BitVector(src1).power(src2);
}

/// @}
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
/// @name Relational operators (IEEE1364-2001 4.1.7)
/// 結果が x になる場合があるので bool ではなく BitVector を返す．
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

/// @relates BitVector
/// @brief less than 比較演算
/// @retval 1 src1 < src2 の時
/// @retval 0 src1 >= src2 の時
/// @retval X 比較不能の時
VlScalarVal
lt(
  const BitVector& src1, ///< [in] 第1オペランド
  const BitVector& src2  ///< [in] 第2オペランド
);

/// @relates BitVector
/// @param[in] src1, src2 オペランド
/// @return src1 < src2 を返す
bool
operator<(
  const BitVector& src1, ///< [in] 第1オペランド
  const BitVector& src2  ///< [in] 第2オペランド
);

/// @relates BitVector
/// @brief greater than 比較演算
/// @retval 1 src1 > src2 の時
/// @retval 0 src1 <= src2 の時
/// @retval X 比較不能の時
inline
VlScalarVal
gt(
  const BitVector& src1, ///< [in] 第1オペランド
  const BitVector& src2  ///< [in] 第2オペランド
)
{
  return lt(src2, src1);
}

/// @relates BitVector
/// @brief greater than 比較演算 (bool)
/// @return src1 > src2 を返す
inline
bool
operator>(
  const BitVector& src1, ///< [in] 第1オペランド
  const BitVector& src2  ///< [in] 第2オペランド
)
{
  return operator<(src2, src1);
}

/// @relates BitVector
/// @brief less than or equal 比較演算
/// @retval 1 src1 <= src2 の時
/// @retval 0 src1 > src2 の時
/// @retval X 比較不能の時
inline
VlScalarVal
le(
  const BitVector& src1, ///< [in] 第1オペランド
  const BitVector& src2  ///< [in] 第2オペランド
)
{
  return !lt(src2, src1);
}

/// @relates BitVector
/// @brief less than or equal 比較演算 (bool)
/// @return src1 <= src2 を返す
inline
bool
operator<=(
  const BitVector& src1, ///< [in] 第1オペランド
  const BitVector& src2  ///< [in] 第2オペランド
)
{
  return !operator<(src2, src1);
}

/// @relates BitVector
/// @brief greater than or equal 比較演算
/// @retval 1 src1 >= src2 の時
/// @retval 0 src1 < src2 の時
/// @retval X 比較不能の時
inline
VlScalarVal
ge(
  const BitVector& src1, ///< [in] 第1オペランド
  const BitVector& src2  ///< [in] 第2オペランド
)
{
  return !lt(src1, src2);
}

/// @relates BitVector
/// @brief greater than or equal 比較演算 (bool)
/// @return src1 >= src2 を返す
inline
bool
operator>=(
  const BitVector& src1, ///< [in] 第1オペランド
  const BitVector& src2  ///< [in] 第2オペランド
)
{
  return !operator<(src1, src2);
}

/// @}
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
/// @name Equality operators (IEEE1364-2001 4.1.8)
/// 結果が x になる場合があるので bool ではなく BitVector を返す．
///
/// 2つのオペランドのサイズが異なっていたら長い方に合わせる．
///
/// 結果の型は以下のとおり
/// - 符号なし
/// - サイズあり(1)
/// - 基数は2
/// @{

/// @relates BitVector
/// @brief 等価比較演算子
/// @retval 1 src1 == src2 の時
/// @retval 0 src1 != src2 の時
/// @retval X 比較不能の時
VlScalarVal
eq(
  const BitVector& src1, ///< [in] 第1オペランド
  const BitVector& src2  ///< [in] 第2オペランド
);

/// @relates BitVector
/// @brief x が 0 および 1 と等価と見なせるとした場合の等価比較演算子
/// @return 等価と見なせるとき true を返す．
bool
eq_with_x(
  const BitVector& src1, ///< [in] 第1オペランド
  const BitVector& src2  ///< [in] 第2オペランド
);

/// @relates BitVector
/// @brief x および z が 0 および 1 と等価と見なせるとした場合の等価比較演算子
/// @return 等価と見なせるとき true を返す．
bool
eq_with_xz(
  const BitVector& src1, ///< [in] 第1オペランド
  const BitVector& src2  ///< [in] 第2オペランド
);

/// @relates BitVector
/// @brief 等価比較演算子 (bool)
/// @return 1 src1 == src2 を返す．
bool
operator==(
  const BitVector& src1, ///< [in] 第1オペランド
  const BitVector& src2  ///< [in] 第2オペランド
);

/// @relates BitVector
/// @brief 非等価比較演算子
/// @retval kVpiScalar1 src1 != src2 の時
/// @retval kVpiScalar0 src1 == src2 の時
/// @retval kVpiScalarX 比較不能の時
inline
VlScalarVal
ne(
  const BitVector& src1, ///< [in] 第1オペランド
  const BitVector& src2  ///< [in] 第2オペランド
)
{
  return !eq(src1, src2);
}

/// @relates BitVector
/// @brief 非等価比較演算子 (bool)
/// @retval 1 src1 != src2 を返す．
inline
bool
operator!=(
  const BitVector& src1, ///< [in] 第1オペランド
  const BitVector& src2  ///< [in] 第2オペランド
)
{
  return !operator==(src1, src2);
}

/// @}
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
/// @name Logical operators (scalar) (IEEE1364-2001 4.1.9)
/// 結果が x になる場合があるので bool ではなく BitVector を返す．
///
/// オペランドがスカラーで無い場合には強制的にスカラーに変換する．
/// 具体的には最下位ビットをそのままスカラー値だと思う．
///
/// 結果の型は以下のとおり
/// - 符号なし
/// - サイズあり(1)
/// - 基数は2
/// @{

/// @relates BitVector
/// @brief NOT演算
/// @retval 0 src が真の時
/// @retval 1 src が偽の時
/// @retval X 計算不能の時
VlScalarVal
operator!(
  const BitVector& src ///< [in] オペランド
);

/// @relates BitVector
/// @brief AND演算
/// @retval 0 src1 と src2 のどちらか一方が偽の時
/// @retval 1 src1 と src2 がともに真の時
/// @retval X 計算不能の時
VlScalarVal
operator&&(
  const BitVector& src1, ///< [in] 第1オペランド
  const BitVector& src2  ///< [in] 第2オペランド
);

/// @relates BitVector
/// @brief OR演算
/// @retval 0 src1 と src2 がともに偽の時
/// @retval 1 src1 と src2 のどちらか一方が真の時
/// @retval X 計算不能の時
VlScalarVal
operator||(
  const BitVector& src1, ///< [in] 第1オペランド
  const BitVector& src2  ///< [in] 第2オペランド
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

/// @relates BitVector
/// @brief bitwise NOT 演算
/// @return src のビットごとに否定したもの
inline
BitVector
operator~(
  const BitVector& src ///< [in] オペランド
)
{
  return BitVector(src).negate();
}

/// @relates BitVector
/// @brief bitwise AND 演算
/// @return src1 と src2 をビットごとに AND したもの
inline
BitVector
operator&(
  const BitVector& src1, ///< [in] 第1オペランド
  const BitVector& src2  ///< [in] 第2オペランド
)
{
  return BitVector(src1) &= src2;
}

/// @relates BitVector
/// @brief bitwise OR 演算
/// @return src1 と src2 をビットごとに OR したもの
inline
BitVector
operator|(
  const BitVector& src1, ///< [in] 第1オペランド
  const BitVector& src2  ///< [in] 第2オペランド
)
{
  return BitVector(src1) |= src2;
}

/// @relates BitVector
/// @brief bitwise XOR 演算
/// @return src1 と src2 をビットごとに XOR したもの
inline
BitVector
operator^(
  const BitVector& src1, ///< [in] 第1オペランド
  const BitVector& src2  ///< [in] 第2オペランド
)
{
  return BitVector(src1) ^= src2;
}

/// @}
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
/// @name Shift operators (IEEE1364-2001 4.1.12)
/// 結果の型は以下のとおり
/// - 符号   : 第1オペランドと同じ
/// - サイズ : 第1オペランドと同じ
/// - 基数   : 第1オペランドと同じ
/// @{

/// @relates BitVector
/// @brief 論理左シフト
/// @return src1 を src2 だけ論理左シフトしたもの
inline
BitVector
operator<<(
  const BitVector& src1, ///< [in] 元の値
  const BitVector& src2  ///< [in] シフト量 (BitVector)
)
{
  return BitVector(src1) <<= src2;
}

/// @relates BitVector
/// @brief 論理左シフト
/// @return src1 を src2 だけ論理左シフトしたもの
inline
BitVector
operator<<(
  const BitVector& src1, ///< [in] 元の値
  int src2               ///< [in] シフト量 (int)
)
{
  return BitVector(src1) <<= src2;
}

/// @relates BitVector
/// @brief 論理右シフト
/// @return src1 を src2 だけ論理右シフトしたもの
inline
BitVector
operator>>(
  const BitVector& src1, ///< [in] 元の値
  const BitVector& src2 ///< [in] シフト量 (BitVector)
)
{
  return BitVector(src1) >>= src2;
}

/// @relates BitVector
/// @brief 論理右シフト
/// @return src1 を src2 だけ論理右シフトしたもの
inline
BitVector
operator>>(
  const BitVector& src1,  ///< [in] 元の値
  int src2  		  ///< [in] シフト量 (int)
)
{
  return BitVector(src1) >>= src2;
}

/// @relates BitVector
/// @brief 算術左シフト
/// @return src1 を src2 だけ算術左シフトしたもの
/// 算術左シフト
inline
BitVector
alshift(
  const BitVector& src1, ///< [in] 元の値
  const BitVector& src2  ///< [in] シフト量 (BitVector)
)
{
  // 実は論理左シフトそのもの
  return operator<<(src1, src2);
}

/// @relates BitVector
/// @brief 算術左シフト
/// @return src1 を src2 だけ算術左シフトしたもの
inline
BitVector
alshift(
  const BitVector& src1, ///< [in] 元の値
  int src2   	         ///< [in] シフト量 (int)
)
{
  // 実は論理左シフトそのもの
  return operator<<(src1, src2);
}

/// @relates BitVector
/// @brief 算術右シフト
/// @return src1 を src2 だけ算術右シフトしたもの
inline
BitVector
arshift(
  const BitVector& src1, ///< [in] 元の値
  const BitVector& src2  ///< [in] シフト量 (BitVector)
)
{
  return BitVector(src1).arshift(src2);
}

/// @relates BitVector
/// @brief 算術右シフト
/// @return src1 を src2 だけ算術右シフトしたもの
inline
BitVector
arshift(
  const BitVector& src1, ///< [in] 元の値
  int src2  	         ///< [in] シフト量 (int)
)
{
  return BitVector(src1).arshift(src2);
}

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

/// @relates BitVector
/// @brief 条件演算
/// @return 演算結果
BitVector
ite(
  const BitVector& src1, ///< [in] 条件
  const BitVector& src2, ///< [in] 条件が真の時に選ばれる値
  const BitVector& src3  ///< [in] 条件が偽の時に選ばれる値
);

/// @relates BitVector
/// @brief 条件演算
/// @return 演算結果
BitVector
ite(
  const VlScalarVal& src1, ///< [in] 条件
  const BitVector& src2,   ///< [in] 条件が真の時に選ばれる値
  const BitVector& src3    ///< [in] 条件が偽の時に選ばれる値
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

/// @relates BitVector
/// @brief 連結演算
/// @return 連結した結果を返す．
BitVector
concat(
  const vector<BitVector>& src_list ///< [in] 連結する値のリスト
);

/// @relates BitVector
/// @brief 繰り返し連結演算
/// @return src_list の内容を rep 回繰り返して連結したもの
BitVector
multi_concat(
  const BitVector& rep,             ///< [in] 繰り返し数
  const vector<BitVector>& src_list ///< [in] 連結する値のリスト
);

/// @}
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
/// @name ストリーム出力
/// @{

/// @relates BitVector
/// @brief Verilog-HDL (IEEE1364-2001) の形式で出力する．
/// @return s を返す．
inline
ostream&
operator<<(
  ostream& s,             ///< [in] 出力ストリーム
  const BitVector& bitval ///< [in] 出力対象のビットベクタ
)
{
  return s << bitval.verilog_string();
}

/// @}
//////////////////////////////////////////////////////////////////////

END_NAMESPACE_YM_VERILOG

#endif // YM_BITVECTOR_H
