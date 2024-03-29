﻿#ifndef EIPRIMITIVE_H
#define EIPRIMITIVE_H

/// @file EiPrimitive.h
/// @brief EiPrimitive のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.

#include "elaborator/ElbPrimitive.h"
#include "ei/EiRange.h"
#include "ym/ClibCell.h"


BEGIN_NAMESPACE_YM_VERILOG

class EiPrimitive;
class EiPrimitive1;
class EiPrimArray;
class EiPrimTerm;

//////////////////////////////////////////////////////////////////////
/// @class EiPrimHead EiPrimitive.h "EiPrimitive.h"
/// @brief primitive/primitive array のヘッダ情報
//////////////////////////////////////////////////////////////////////
class EiPrimHead :
  public ElbPrimHead
{
public:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_header パース木の定義
  EiPrimHead(const VlScope* parent,
	     const PtItem* pt_header);

  /// @brief デストラクタ
  ~EiPrimHead();


public:
  //////////////////////////////////////////////////////////////////////
  // ElbPrimHead の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief このオブジェクトの属しているスコープを返す．
  const VlScope*
  parent_scope() const override;

  /// @brief primitive type を返す．
  VpiPrimType
  prim_type() const override;

  /// @brief プリミティブの定義名を返す．
  string
  def_name() const override;

  /// @brief UDP 定義を返す．
  /// @note このクラスでは nullptr を返す．
  const VlUdpDefn*
  udp_defn() const override;

  /// @brief セルを返す．
  ClibCell
  cell() const override;

  /// @brief 0 の強さを得る．
  VpiStrength
  drive0() const override;

  /// @brief 1 の強さを得る．
  VpiStrength
  drive1() const override;

  /// @brief 遅延式を得る．
  /// @note このクラスでは nullptr を返す．
  const VlDelay*
  delay() const override;

  /// @brief 遅延式を設定する．
  /// @note このクラスではなにもしない．
  void
  set_delay(const VlDelay* expr) override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親のスコープ
  const VlScope* mParent;

  // パース木の定義
  const PtItem* mPtHead;

};


//////////////////////////////////////////////////////////////////////
/// @class EiPrimHeadD EiPrimitive.h "EiPrimitive.h"
/// @brief 遅延付きの EiPrimHead
//////////////////////////////////////////////////////////////////////
class EiPrimHeadD :
  public EiPrimHead
{
public:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_header パース木の定義
  EiPrimHeadD(const VlScope* parent,
	      const PtItem* pt_header);

  /// @brief デストラクタ
  ~EiPrimHeadD();


public:
  //////////////////////////////////////////////////////////////////////
  // EiPrimHead の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 遅延式を得る．
  const VlDelay*
  delay() const override;

  /// @brief 遅延式を設定する．
  void
  set_delay(const VlDelay* expr) override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 遅延値
  const VlDelay* mDelay;

};


//////////////////////////////////////////////////////////////////////
/// @class EiPrimHeadU EiPrimitive.h "EiPrimitive.h"
/// @brief UDP 型の EiPrimHead
//////////////////////////////////////////////////////////////////////
class EiPrimHeadU :
  public EiPrimHead
{
public:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_header パース木の定義
  /// @param[in] udp UDP 定義
  EiPrimHeadU(const VlScope* parent,
	      const PtItem* pt_header,
	      const VlUdpDefn* udp);

  /// @brief デストラクタ
  ~EiPrimHeadU();


public:
  //////////////////////////////////////////////////////////////////////
  // EiPrimHead の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief primitive type を返す．
  VpiPrimType
  prim_type() const override;

  /// @brief プリミティブの定義名を返す．
  string
  def_name() const override;

  /// @brief UDP 定義を返す．
  const VlUdpDefn*
  udp_defn() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // UDP 定義
  const VlUdpDefn* mUdp;

};


//////////////////////////////////////////////////////////////////////
/// @class EiPrimHeadUD EiPrimitive.h "EiPrimitive.h"
/// @brief 遅延付きの EiPrimHeadU
//////////////////////////////////////////////////////////////////////
class EiPrimHeadUD :
  public EiPrimHeadU
{
public:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_header パース木の定義
  /// @param[in] udp UDP 定義
  EiPrimHeadUD(const VlScope* parent,
	       const PtItem* pt_header,
	       const VlUdpDefn* udp);

  /// @brief デストラクタ
  ~EiPrimHeadUD();


public:
  //////////////////////////////////////////////////////////////////////
  // EiPrimHead の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 遅延式を得る．
  const VlDelay*
  delay() const override;

  /// @brief 遅延式を設定する．
  void
  set_delay(const VlDelay* expr) override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 遅延値
  const VlDelay* mDelay;

};


//////////////////////////////////////////////////////////////////////
/// @class EiPrimHeadC EiPrimitive.h "EiPrimitive.h"
/// @brief セル型の EiPrimHead
//////////////////////////////////////////////////////////////////////
class EiPrimHeadC :
  public EiPrimHead
{
public:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_header パース木の定義
  /// @param[in] cell セル
  EiPrimHeadC(const VlScope* parent,
	      const PtItem* pt_header,
	      const ClibCell& cell);

  /// @brief デストラクタ
  ~EiPrimHeadC();


public:
  //////////////////////////////////////////////////////////////////////
  // EiPrimHead の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief primitive type を返す．
  VpiPrimType
  prim_type() const override;

  /// @brief プリミティブの定義名を返す．
  string
  def_name() const override;

  /// @brief セルを返す．
  ClibCell
  cell() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // セル
  ClibCell mCell;

};


//////////////////////////////////////////////////////////////////////
/// @class EiPrimitive EiPrimitive.h "EiPrimitive.h"
/// @brief ElbPrimitive の実装クラス
//////////////////////////////////////////////////////////////////////
class EiPrimitive :
  public ElbPrimitive
{
public:

  /// @brief コンストラクタ
  EiPrimitive();

  /// @brief デストラクタ
  ~EiPrimitive();


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


public:
  //////////////////////////////////////////////////////////////////////
  // VlPrimitive の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief primitive type を返す．
  VpiPrimType
  prim_type() const override;

  /// @brief プリミティブの定義名を返す．
  string
  def_name() const override;

  /// @brief UPD 定義を返す．
  const VlUdpDefn*
  udp_defn() const override;

  /// @brief セルを返す．
  ClibCell
  cell() const override;

  /// @brief 0 の強さを得る．
  VpiStrength
  drive0() const override;

  /// @brief 1 の強さを得る．
  VpiStrength
  drive1() const override;

  /// @brief 遅延式を得る．
  const VlDelay*
  delay() const override;

  /// @brief ポート数を得る．
  SizeType
  port_num() const override;

  /// @brief ポート端子を得る．
  /// @param[in] pos 位置番号 (0 <= pos < port_num())
  const VlPrimTerm*
  prim_term(SizeType pos) const override;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbPrimitive の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 接続する．
  /// @param[in] pos ポート番号 (0 から始まる)
  /// @param[in] expr 接続する式
  void
  connect(SizeType pos,
	  const VlExpr* expr) override;


protected:
  //////////////////////////////////////////////////////////////////////
  // 初期化用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ポート配列を初期化する．
  /// @param[in] port_num ポート数
  void
  init_port(SizeType port_num);

  /// @brief ポート配列を初期化する．
  /// @param[in] port_num ポート数
  /// @param[in] cell セル
  void
  init_port(SizeType port_num,
	    const ClibCell& cell);


private:
  //////////////////////////////////////////////////////////////////////
  // EiPrimitive の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief パース木のインスタンス定義を得る．
  virtual
  const PtInst*
  pt_inst() const = 0;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ポートの実体の配列
  vector<EiPrimTerm> mPortArray;

};


//////////////////////////////////////////////////////////////////////
/// @class EiPrimitive1 EiPrimitive.h "EiPrimitive.h"
/// @brief 配列要素の EiPrimitive
//////////////////////////////////////////////////////////////////////
class EiPrimitive1 :
  public EiPrimitive
{
public:

  /// @brief コンストラクタ
  EiPrimitive1();

  /// @brief デストラクタ
  ~EiPrimitive1();


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を設定する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期設定を行う．
  /// @param[in] prim_array 親の配列
  /// @param[in] index インデックス番号
  /// @param[in] port_num 端子数
  void
  init(EiPrimArray* prim_array,
       SizeType index,
       SizeType port_num);

  /// @brief 初期設定を行う．
  /// @param[in] prim_array 親の配列
  /// @param[in] index インデックス番号
  /// @param[in] port_num 端子数
  /// @param[in] cell セル
  void
  init(EiPrimArray* prim_array,
       SizeType index,
       SizeType port_num,
       const ClibCell& cell);


public:
  //////////////////////////////////////////////////////////////////////
  // VlNamedObj の派生クラスに共通な仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前の取得
  string
  name() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // EiPrimitive の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ヘッダを得る．
  ElbPrimHead*
  head() const override;

  /// @brief パース木のインスタンス定義を得る．
  const PtInst*
  pt_inst() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親の配列
  EiPrimArray* mPrimArray;

  // インデックス番号
  SizeType mIndex;

};


//////////////////////////////////////////////////////////////////////
/// @class EiPrimitive2 EiPrimitive.h "EiPrimitive.h"
/// @brief 単一要素の EiPrimitive
//////////////////////////////////////////////////////////////////////
class EiPrimitive2 :
  public EiPrimitive
{
public:

  /// @brief コンストラクタ
  /// @param[in] head ヘッダ
  /// @param[in] pt_inst インスタンス定義
  /// @param[in] term_array 端子の配列
  EiPrimitive2(ElbPrimHead* head,
	       const PtInst* pt_inst);

  /// @brief コンストラクタ
  /// @param[in] head ヘッダ
  /// @param[in] cell セル
  /// @param[in] pt_inst インスタンス定義
  /// @param[in] term_array 端子の配列
  EiPrimitive2(ElbPrimHead* head,
	       const ClibCell& cell,
	       const PtInst* pt_inst);

  /// @brief デストラクタ
  ~EiPrimitive2();


public:
  //////////////////////////////////////////////////////////////////////
  // VlNamedObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前の取得
  string
  name() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // EiPrimitive の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ヘッダを得る．
  ElbPrimHead*
  head() const override;

  /// @brief パース木のインスタンス定義を得る．
  const PtInst*
  pt_inst() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ヘッダ
  ElbPrimHead* mHead;

  // パース木のインスタンス定義
  const PtInst* mPtInst;

};


//////////////////////////////////////////////////////////////////////
/// @class EiPrimArray EiPrimitive.h "EiPrimitive.h"
/// @brief ElbPrimArray の実装クラス
//////////////////////////////////////////////////////////////////////
class EiPrimArray :
  public ElbPrimArray
{
public:

  /// @brief コンストラクタ
  /// @param[in] head ヘッダ
  /// @param[in] pt_inst インスタンス定義
  /// @param[in] range 範囲
  /// @param[in] elem_array 要素の配列
  /// @param[in] term_array 端子の配列
  EiPrimArray(ElbPrimHead* head,
	      const PtInst* pt_inst,
	      const EiRangeImpl& range);

  /// @brief コンストラクタ
  /// @param[in] head ヘッダ
  /// @param[in] cell セル
  /// @param[in] pt_inst インスタンス定義
  /// @param[in] range 範囲
  /// @param[in] elem_array 要素の配列
  /// @param[in] term_array 端子の配列
  EiPrimArray(ElbPrimHead* head,
	      const ClibCell& cell,
	      const PtInst* pt_inst,
	      const EiRangeImpl& range);

  /// @brief デストラクタ
  ~EiPrimArray();


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
  // VlPrimArray の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief primitive type を返す．
  VpiPrimType
  prim_type() const override;

  /// @brief プリミティブの定義名を返す．
  string
  def_name() const override;

  /// @brief UDP 定義を返す．
  const VlUdpDefn*
  udp_defn() const override;

  /// @brief セルを返す．
  ClibCell
  cell() const override;

  /// @brief 0 の強さを得る．
  VpiStrength
  drive0() const override;

  /// @brief 1 の強さを得る．
  VpiStrength
  drive1() const override;

  /// @brief 遅延式を得る．
  const VlDelay*
  delay() const override;

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

  /// @brief 要素数を返す．
  SizeType
  elem_num() const override;

  /// @brief 要素のプリミティブを返す．
  /// @param[in] offset 位置番号 ( 0 <= offset < elem_num() )
  const VlPrimitive*
  elem_by_offset(SizeType offset) const override;

  /// @brief 要素を返す．
  /// @param[in] index インデックス
  const VlPrimitive*
  elem_by_index(int index) const override;


public:
  //////////////////////////////////////////////////////////////////////
  // 設定用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素のプリミティブを取り出す．
  ElbPrimitive*
  _primitive_by_offset(SizeType offset) override;

  /// @brief 要素のプリミティブを取り出す．
  ElbPrimitive*
  _primitive_by_index(int index) override;

  /// @brief ヘッダを得る．
  ElbPrimHead*
  head() const override;

  /// @brief パース木のインスタンス定義を得る．
  const PtInst*
  pt_inst() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ヘッダ
  ElbPrimHead* mHead;

  // パース木のインスタンス定義
  const PtInst* mPtInst;

  // 範囲
  EiRangeImpl mRange;

  // 要素の配列
  vector<EiPrimitive1> mArray;

};


//////////////////////////////////////////////////////////////////////
/// @class EiPrimTerm EiPrimitive.h "EiPrimitive.h"
/// @brief ElbPrimTerm の実装クラス
//////////////////////////////////////////////////////////////////////
class EiPrimTerm :
  public VlPrimTerm
{
public:

  /// @brief コンストラクタ
  EiPrimTerm();

  /// @brief デストラクタ
  ~EiPrimTerm();


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
  // VlPrimTerm の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 親のプリミティブを返す．
  const VlPrimitive*
  primitive() const override;

  /// @brief 入出力の種類を返す．
  VpiDir
  direction() const override;

  /// @brief 端子番号を返す．
  SizeType
  term_index() const override;

  /// @brief 接続しているネットを表す式を返す．
  const VlExpr*
  expr() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // 設定用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を設定する．
  void
  set(const VlPrimitive* primitive,
      SizeType index,
      VpiDir dir);

  /// @brief 接続している式を設定する．
  void
  set_expr(const VlExpr* expr);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親のプリミティブ
  const VlPrimitive* mPrimitive;

  // インデックス + 方向(3bit)
  int mIndexDir;

  // 接続している式
  const VlExpr* mExpr;

};

END_NAMESPACE_YM_VERILOG

#endif // EIPRIMITIVE_H
