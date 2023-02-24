#ifndef ELBPRIMITIVE_H
#define ELBPRIMITIVE_H

/// @file ElbPrimitive.h
/// @brief ElbPrimitive のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/VlPrimitive.h"
#include "ym/clib.h"
#include "elaborator/ElbHead.h"
#include "elaborator/ElbFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ElbPrimHead ElbPrimitive.h "ElbPrimitive.h"
/// @brief primitive/primitive array のヘッダ情報
//////////////////////////////////////////////////////////////////////
class ElbPrimHead :
  public ElbHead
{
public:
  //////////////////////////////////////////////////////////////////////
  // ElbPrimHead の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief このオブジェクトの属しているスコープを返す．
  virtual
  const VlScope*
  parent_scope() const = 0;

  /// @brief primitive type を返す．
  virtual
  VpiPrimType
  prim_type() const = 0;

  /// @brief プリミティブの定義名を返す．
  virtual
  string
  def_name() const = 0;

  /// @brief UDP 定義を返す．
  virtual
  const VlUdpDefn*
  udp_defn() const = 0;

  /// @brief セルを返す．
  virtual
  ClibCell
  cell() const = 0;

  /// @brief 0 の強さを得る．
  virtual
  VpiStrength
  drive0() const = 0;

  /// @brief 1 の強さを得る．
  virtual
  VpiStrength
  drive1() const = 0;

  /// @brief 遅延式を得る．
  virtual
  const VlDelay*
  delay() const = 0;

  /// @brief 遅延式を設定する．
  virtual
  void
  set_delay(
    const VlDelay* expr ///< [in] 遅延式
  ) = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class ElbPrimArray ElbPrimitive.h "ElbPrimitive.h"
/// @brief gate/UDP instance の配列を表すクラス
/// IEEE Std 1364-2001 26.6.13 Primitive, prim term
//////////////////////////////////////////////////////////////////////
class ElbPrimArray :
  public VlPrimArray
{
protected:

  /// @brief コンストラクタ
  ElbPrimArray() = default;

  /// @brief デストラクタ
  ~ElbPrimArray() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbPrimArray の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素のプリミティブを取り出す．
  virtual
  ElbPrimitive*
  _primitive_by_offset(
    SizeType offset ///< [in] オフセット
  ) = 0;

  /// @brief 要素のプリミティブを取り出す．
  virtual
  ElbPrimitive*
  _primitive_by_index(
    int index ///< [in] インデックス
  ) = 0;;

  /// @brief ヘッダを得る．
  virtual
  ElbPrimHead*
  head() const = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class ElbPrimitive ElbItem.h "ElbItem.h"
/// @brief gate/UDP instance を表すクラス
//////////////////////////////////////////////////////////////////////
class ElbPrimitive :
  public VlPrimitive
{
protected:

  /// @brief コンストラクタ
  ElbPrimitive() = default;

  /// @brief デストラクタ
  ~ElbPrimitive() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbPrimitive の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 接続する．
  virtual
  void
  connect(
    SizeType pos,      ///< [in] ポート番号 (0 から始まる)
    const VlExpr* expr ///< [in] 接続する式
  ) = 0;

  /// @brief ヘッダを得る．
  virtual
  ElbPrimHead*
  head() const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // 便利関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入出力ポート数を計算する．
  /// @retval 1 port_size が多すぎる．
  /// @retval 0 port_size が範囲内に収まっている．
  /// @retval -1 port_size が少なすぎる．
  static
  int
  get_port_size(
    VpiPrimType type,     ///< [in] プリミティブの型
    SizeType port_size,   ///< [in] 全ポート数
    SizeType& output_num, ///< [out] 出力ポート数
    SizeType& inout_num,  ///< [out] 入出力ポート数
    SizeType& input_num   ///< [out] 入力ポート数
  );

};

END_NAMESPACE_YM_VERILOG

#endif // ELBPRIMITIVE_H
