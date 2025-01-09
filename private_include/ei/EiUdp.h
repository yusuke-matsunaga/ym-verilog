#ifndef EIUDP_H
#define EIUDP_H

/// @file EiUdpImpl.h
/// @brief EiUdpImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.

// UDP定義を表すクラス
// IEEE Std 1364-2001 26.6.14 UDP

#include "elaborator/ElbUdp.h"
#include "ym/vl/VlIODecl.h"
#include "ym/VlScalarVal.h"


BEGIN_NAMESPACE_YM_VERILOG

class EiUdpIO;
class EiTableEntry;
class EiExpr;

//////////////////////////////////////////////////////////////////////
/// @class EiUdpDefn ElbUdpImpl.h "ElbUdpImpl.h"
/// @brief ElbUdpDefn の実装クラス
//////////////////////////////////////////////////////////////////////
class EiUdpDefn :
  public ElbUdpDefn
{
public:

  /// @brief コンストラクタ
  EiUdpDefn(
    const PtUdp* pt_udp, ///< [in] パース木の UDP 定義
    bool is_protected,   ///< [in] プロテクト属性
    SizeType io_num,     ///< [in] ポート数
    SizeType table_num   ///< [in] テーブルの行数
  );

  /// @brief デストラクタ
  ~EiUdpDefn();


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
  // VlUdpDefn の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 定義された名前を返す．
  string
  def_name() const override;

  /// @brief primitive type を返す．
  VpiPrimType
  prim_type() const override;

  /// @brief ポート数を返す．
  SizeType
  port_num() const override;

  /// @brief 入力の宣言要素を返す．
  const VlIODecl*
  input(
    SizeType pos ///< [in] 入力番号 ( 0 <= pos < port_num() - 1 )
  ) const override;

  /// @brief 出力の宣言要素を返す．
  const VlIODecl*
  output() const override;

  /// @brief protected かどうかを返す．
  bool
  is_protected() const override;

  /// @brief 初期値を返す．
  /// @return 0/1/X を返す．
  VlScalarVal
  init_val() const override;

  /// @brief 初期値を表す文字列を返す．
  string
  init_val_string() const override;

  /// @brief table entry の行数を返す．
  SizeType
  table_size() const override;

  /// @brief table entry を返す．
  const VlTableEntry*
  table_entry(
    SizeType pos ///< [in] 行番号
  ) const override;


public:
  //////////////////////////////////////////////////////////////////////
  // 設定用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入出力オブジェクトの内容を設定する．
  void
  set_io(
    SizeType pos,              ///< [in] ポート中の位置
    const PtIOHead* pt_header, ///< [in] パース木の宣言ヘッダ
    const PtIOItem* pt_item    ///< [in] パース木の宣言要素
  ) override;

  /// @brief 初期値を設定する．
  void
  set_initial(
    const PtExpr* init_expr,    ///< [in] 初期値を表す式
    const VlScalarVal& init_val ///< [in] 初期値
  ) override;

  /// @brief table entry の内容を設定する．
  void
  set_tableentry(
    SizeType pos,                   ///< [in] 行番号
    const PtUdpEntry* pt_udp_entry, ///< [in] パース木の一行分の定義
    const vector<VlUdpVal>& vals    ///< [in] シンボル値の配列
  ) override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // パース木の UDP 定義
  const PtUdp* mPtUdp;

  // ポート数
  SizeType mPortNum;

  // protect されているとき true にするフラグ
  bool mProtected;

  // IO decl のリスト(配列)
  vector<EiUdpIO> mIODeclList;

  // 初期値の式
  const PtExpr* mInitExpr;

  // 初期値
  VlScalarVal mInitVal;

  // table entry の配列
  vector<EiTableEntry> mTableEntryList;

};


//////////////////////////////////////////////////////////////////////
/// UDP 用 IO decl の基底クラス
//////////////////////////////////////////////////////////////////////
class EiUdpIO :
  public VlIODecl
{
public:

  /// @brief コンストラクタ
  EiUdpIO();

  /// @brief デストラクタ
  ~EiUdpIO();


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
  // VlIODecl に固有の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を返す．
  string
  name() const override;

  /// @brief 方向を返す．
  VpiDir
  direction() const override;

  /// @brief 符号の属性の取得
  /// @return 符号付きのとき true を返す．
  bool
  is_signed() const override;

  /// @brief 範囲指定を持つとき true を返す．
  bool
  has_range() const override;

  /// @brief MSB の値を返す．
  int
  left_range_val() const override;

  /// @brief LSB の値を返す．
  int
  right_range_val() const override;

  /// @brief 範囲のMSBを表す文字列の取得
  string
  left_range_string() const override;

  /// @brief 範囲のLSBを表す文字列の取得
  string
  right_range_string() const override;

  /// @brief サイズを返す．
  /// このクラスは 1 を返す．
  SizeType
  bit_size() const override;

  /// @brief 対応する宣言要素を返す．
  const VlDecl*
  decl() const override;

  /// @brief 親のモジュールの取得
  const VlModule*
  module() const override;

  /// @brief 親のUDP定義を返す．
  const VlUdpDefn*
  udp_defn() const override;

  /// @brief 親のタスクの取得
  const VlTaskFunc*
  task() const override;

  /// @brief 親の関数の取得
  const VlTaskFunc*
  function() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // 設定用の関数 (EiUdpDefn が使う)
  //////////////////////////////////////////////////////////////////////

  /// @brief 親のUDPを設定する．
  void
  set_udp(
    ElbUdpDefn* udp
  );

  /// @brief 内容を設定する．
  void
  set(
    const PtIOHead* pt_header, ///< [in] パース木のIO宣言ヘッダ
    const PtIOItem* pt_item    ///< [in] パース木のIO宣言定義
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親の UDP
  ElbUdpDefn* mUdp;

  // パース木のIO宣言ヘッダ
  const PtIOHead* mPtHeader;

  // パース木のIO宣言定義
  const PtIOItem* mPtItem;

};


//////////////////////////////////////////////////////////////////////
/// @class EiTableEntry ElbUdpImpl.h "ElbUdpImpl.h"
/// @brief UDP の table entry を表すクラス
//////////////////////////////////////////////////////////////////////
class EiTableEntry :
  public ElbTableEntry
{
public:

  /// @brief コンストラクタ
  EiTableEntry();

  /// @brief デストラクタ
  ~EiTableEntry();


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
  // VlTableEntry の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 一行の要素数を返す．
  SizeType
  size() const override;

  /// @brief pos 番目の位置の値を返す．
  VlUdpVal
  val(
    SizeType pos
  ) const override;

  /// @brief 一行文の内容を表す文字列をつくる．
  string
  str() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbTableEntry の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期化する．
  void
  init(
    ElbUdpDefn* udp
  );

  /// @brief 設定する．
  void
  set(
    const PtUdpEntry* pt_entry,
    const vector<VlUdpVal>& vals
  ) override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親の UDP
  ElbUdpDefn* mUdp;

  // パース木の定義
  const PtUdpEntry* mPtUdpEntry;

  // 値の配列
  vector<VlUdpVal> mValArray;

};

END_NAMESPACE_YM_VERILOG

#endif // EIUDP_H
