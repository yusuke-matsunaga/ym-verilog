#ifndef ELBPRIMITIVE_H
#define ELBPRIMITIVE_H

/// @file ElbPrimitive.h
/// @brief ElbPrimitive のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym/vl/VlPrimitive.h"

#include "ElbFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ElbPrimHead ElbPrimitive.h "ElbPrimitive.h"
/// @brief primitive/primitive array のヘッダ情報
//////////////////////////////////////////////////////////////////////
class ElbPrimHead
{
protected:

  /// @brief コンストラクタ
  ElbPrimHead();

  /// @brief デストラクタ
  virtual
  ~ElbPrimHead();


public:
  //////////////////////////////////////////////////////////////////////
  // ElbPrimHead の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief このオブジェクトの属しているスコープを返す．
  virtual
  const VlNamedObj*
  parent() const = 0;

  /// @brief primitive type を返す．
  virtual
  VpiPrimType
  prim_type() const = 0;

  /// @brief プリミティブの定義名を返す．
  virtual
  const char*
  def_name() const = 0;

  /// @brief UDP 定義を返す．
  virtual
  const ElbUdpDefn*
  udp_defn() const = 0;

  /// @brief セル番号を返す．
  virtual
  int
  cell_id() const = 0;

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
  ElbDelay*
  delay() const = 0;

  /// @brief 遅延式を設定する．
  virtual
  void
  set_delay(ElbDelay* expr) = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class ElbPrimArray ElbPrimitive.h "ElbPrimitive.h"
/// @brief gate/UDP instance の配列を表すクラス
/// IEEE Std 1364-2001 26.6.13 Primitive, prim term
//////////////////////////////////////////////////////////////////////
class ElbPrimArray :
  public VlPrimArray
{
  friend class CellPrimArray;

protected:

  /// @brief コンストラクタ
  ElbPrimArray();

  /// @brief デストラクタ
  ~ElbPrimArray();


public:
  //////////////////////////////////////////////////////////////////////
  // ElbPrimArray の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素のプリミティブを取り出す．
  virtual
  ElbPrimitive*
  _primitive_by_offset(int offset) const = 0;

  /// @brief 要素のプリミティブを取り出す．
  virtual
  ElbPrimitive*
  _primitive_by_index(int index) const = 0;

  /// @brief ヘッダを得る．
  virtual
  ElbPrimHead*
  head() const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbPrimArray の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 次の要素を得る．
  const ElbPrimArray*
  next() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 次の要素を指すポインタ
  ElbPrimArray* mNext;

};


//////////////////////////////////////////////////////////////////////
/// @class ElbPrimitive ElbItem.h "ElbItem.h"
/// @brief gate/UDP instance を表すクラス
//////////////////////////////////////////////////////////////////////
class ElbPrimitive :
  public VlPrimitive
{
  friend class CellPrimitive;

protected:

  /// @brief コンストラクタ
  ElbPrimitive();

  /// @brief デストラクタ
  ~ElbPrimitive();


public:
  //////////////////////////////////////////////////////////////////////
  // ElbPrimitive の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 接続する．
  /// @param[in] pos ポート番号 (0 から始まる)
  /// @param[in] expr 接続する式
  virtual
  void
  connect(int pos,
	  ElbExpr* expr) = 0;

  /// @brief ヘッダを得る．
  virtual
  ElbPrimHead*
  head() const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbPrimitive の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 次の要素を得る．
  const ElbPrimitive*
  next() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 便利関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入出力ポート数を計算する．
  /// @param[in] type プリミティブの型
  /// @param[in] port_size 全ポート数
  /// @param[out] output_num 出力ポート数
  /// @param[out] inout_num 入出力ポート数
  /// @param[out] input_num 入力ポート数
  /// @retval 1 port_size が多すぎる．
  /// @retval 0 port_size が範囲内に収まっている．
  /// @retval -1 port_size が少なすぎる．
  static
  int
  get_port_size(VpiPrimType type,
		SizeType port_size,
		SizeType& output_num,
		SizeType& inout_num,
		SizeType& input_num);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 次の要素を指すポインタ
  ElbPrimitive* mNext;

};


//////////////////////////////////////////////////////////////////////
/// @class ElbPrimTerm ElbPrimitive.h <ym/vl/VlPrimitive.h>
/// @brief プリミティブインスタンスのポート端子を表すクラス
/// IEEE Std 1364-2001 26.6.13 Primitive, prim term
//////////////////////////////////////////////////////////////////////
class ElbPrimTerm :
  public VlPrimTerm
{
protected:

  /// @brief コンストラクタ
  ElbPrimTerm() { }

  /// @brief デストラクタ
  ~ElbPrimTerm() { }


public:
  //////////////////////////////////////////////////////////////////////
  // ElbPrimTerm の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を設定する．
  virtual
  void
  set(ElbPrimitive* primitive,
      int index,
      VpiDir dir) = 0;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 次の要素を得る．
inline
const ElbPrimArray*
ElbPrimArray::next() const
{
  return mNext;
}

// @brief 次の要素を得る．
inline
const ElbPrimitive*
ElbPrimitive::next() const
{
  return mNext;
}


END_NAMESPACE_YM_VERILOG

#endif // ELBPRIMITIVE_H
