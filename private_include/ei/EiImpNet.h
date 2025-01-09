#ifndef EIIMPNET_H
#define EIIMPNET_H

/// @file EiImpNet.h
/// @brief EiImpNet のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.

#include "elaborator/ElbDecl.h"

#include "ym/VlScalarVal.h"
#include "ym/pt/PtP.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiImpNet EiImpNet.h "EiImpNet.h"
/// @brief 暗黙のネットを表すクラス
//////////////////////////////////////////////////////////////////////
class EiImpNet :
  public ElbDecl
{
public:

  /// @brief コンストラクタ
  EiImpNet(
    const VlScope* parent, ///< [in] 親のスコープ
    const PtExpr* pt_expr, ///< [in] パース木のプライマリ式
    VpiNetType net_type    ///< [in] ネットの型
  );

  /// @brief デストラクタ
  ~EiImpNet();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  VpiObjType
  type() const override;

  /// @brief ファイル位置を返す．
  FileRegion
  file_region() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // VlNamedObj の派生クラスに共通な仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief このオブジェクトの属しているスコープを返す．
  const VlScope*
  parent_scope() const override;

  /// @brief 名前の取得
  string
  name() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // VlDecl の関数
  //////////////////////////////////////////////////////////////////////

  /// @breif 値の型を返す．
  VlValueType
  value_type() const override;

  /// @brief 符号の取得
  /// @retval true 符号つき
  /// @retval false 符号なし
  bool
  is_signed() const override;

  /// @brief 範囲指定を持つとき true を返す．
  bool
  has_range() const override;

  /// @brief 範囲の MSB の値を返す．
  int
  left_range_val() const override;

  /// @brief 範囲の LSB の値を返す．
  int
  right_range_val() const override;

  /// @brief 範囲のMSBを表す文字列の取得
  string
  left_range_string() const override;

  /// @brief 範囲のLSBを表す文字列の取得
  string
  right_range_string() const override;

  /// @brief left_range >= right_range の時に true を返す．
  bool
  is_big_endian() const override;

  /// @brief left_range <= right_range の時に true を返す．
  bool
  is_little_endian() const override;

  /// @brief ビット幅を返す．
  SizeType
  bit_size() const override;

  /// @brief オフセット値の取得
  /// @retval true インデックスが範囲内に入っている時
  /// @retval false インデックスが範囲外の時
  bool
  calc_bit_offset(
    int index,       ///< [in] インデックス
    SizeType& offset ///< [out] インデックスに対するオフセット値
  ) const override;

  /// @brief データ型の取得
  /// @retval データ型 パラメータや変数の場合
  /// @retval kVpiVarNone 上記以外
  VpiVarType
  data_type() const override;

  /// @brief net 型の取得
  /// @retval net 型 net 型の要素の場合
  /// @retval kVpiNone net 型の要素でない場合
  VpiNetType
  net_type() const override;

  /// @brief vectored|scalared 属性の取得
  /// @retval kVpiVsNone vectored|scalared 指定なし
  /// @retval kVpiVectored vectored 指定あり
  /// @retval kVpiScalared scalared 指定あり
  VpiVsType
  vs_type() const override;

  /// @brief drive0 strength の取得
  /// @retval 0 の強度
  /// @retval kVpiNoStrength strength の指定なし
  VpiStrength
  drive0() const override;

  /// @brief drive1 strength の取得
  /// @retval 1 の強度
  /// @retval kVpiNoStrength strength の指定なし
  VpiStrength
  drive1() const override;

  /// @brief charge strength の取得
  /// @retval 電荷の強度
  /// @retval kVpiNoStrength strength の指定なし
  VpiStrength
  charge() const override;

  /// @brief delay の取得
  /// @retval delay
  /// @retval nullptr delay の指定なし
  const VlDelay*
  delay() const override;

  /// @brief 定数値を持つ型のときに true を返す．
  bool
  is_consttype() const override;

  /// @brief 初期値の取得
  /// @retval 初期値
  /// @retval nullptr 設定がない場合
  const VlExpr*
  init_value() const override;

  /// @brief localparam のときに true 返す．
  bool
  is_local_param() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 符号付きに補正する．
  void
  set_signed() override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親のスコープ
  const VlScope* mParent;

  // パース木のプライマリ式
  const PtExpr* mPtExpr;

  // ネット型
  VpiNetType mNetType;

  // 値
  VlScalarVal mVal;

};

END_NAMESPACE_YM_VERILOG

#endif // EIIMPNET_H
