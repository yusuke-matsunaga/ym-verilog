#ifndef EIUDP_H
#define EIUDP_H

/// @file EiUdpImpl.h
/// @brief EiUdpImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.

// UDP定義を表すクラス
// IEEE Std 1364-2001 26.6.14 UDP


#include "ElbUdp.h"
#include "ElbIODecl.h"
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
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] pt_udp パース木の UDP 定義
  /// @param[in] is_protected プロテクト属性
  /// @param[in] io_num ポート数
  /// @param[in] io_array IOの配列
  /// @param[in] table_num テーブルの行数
  /// @param[in] table テーブル
  /// @param[in] val_array テーブル中の値を納める配列
  EiUdpDefn(const PtUdp* pt_udp,
	    bool is_protected,
	    SizeType io_num,
	    EiUdpIO* io_array,
	    SizeType table_num,
	    EiTableEntry* table,
	    VlUdpVal* val_array);

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
  const char*
  def_name() const override;

  /// @brief primitive type を返す．
  VpiPrimType
  prim_type() const override;

  /// @brief ポート数を返す．
  SizeType
  port_num() const override;

  /// @brief 入力の宣言要素を返す．
  /// @param[in] pos 入力番号 ( 0 <= pos < port_num() - 1 )
  const VlIODecl*
  input(SizeType pos) const override;

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
  /// @param[in] pos 行番号
  const VlTableEntry*
  table_entry(SizeType pos) const override;


public:
  //////////////////////////////////////////////////////////////////////
  // 設定用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入出力オブジェクトの内容を設定する．
  /// @param[in] pos ポート中の位置
  /// @param[in] file_region ソースファイル上の位置
  /// @param[in] name 名前
  /// @param[in] dir 向き
  void
  set_io(SizeType pos,
	 const PtIOHead* pt_header,
	 const PtIOItem* pt_item) override;

  /// @brief 初期値を設定する．
  /// @param[in] init_expr 初期値を表す式
  /// @param[in] init_val 初期値
  void
  set_initial(const PtExpr* init_expr,
	      const VlScalarVal& init_val) override;

  /// @brief table entry の内容を設定する．
  /// @param[in] pos 行番号
  /// @param[in] pt_udp_entry パース木の一行分の定義
  /// @param[in] vals シンボル値の配列
  void
  set_tableentry(SizeType pos,
		 const PtUdpEntry* pt_udp_entry,
		 const vector<VlUdpVal>& vals) override;


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
  EiUdpIO* mIODeclList;

  // 初期値の式
  const PtExpr* mInitExpr;

  // 初期値
  VlScalarVal mInitVal;

  // table entry のサイズ
  SizeType mTableEntrySize;

  // table entry のリスト
  EiTableEntry* mTableEntryList;

  // table entry の値の配列
  VlUdpVal* mValArray;

};


//////////////////////////////////////////////////////////////////////
/// UDP 用 IO decl の基底クラス
//////////////////////////////////////////////////////////////////////
class EiUdpIO :
  public VlIODecl
{
  friend class EiFactory;
  friend class EiUdpDefn;

private:

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
  const char*
  name() const override;

  /// @brief 方向を返す．
  VpiDir
  direction() const override;

  /// @brief 符号の属性の取得
  /// @return 符号付きのとき true を返す．
  /// @note このクラスは false を返す．
  bool
  is_signed() const override;

  /// @brief 範囲指定を持つとき true を返す．
  bool
  has_range() const override;

  /// @brief MSB の値を返す．
  /// @note 範囲を持たないときの値は不定
  int
  left_range_val() const override;

  /// @brief LSB の値を返す．
  /// @note 範囲を持たないときの値は不定
  int
  right_range_val() const override;

  /// @brief 範囲のMSBを表す文字列の取得
  /// @note 範囲を持たない時の値は不定
  string
  left_range_string() const override;

  /// @brief 範囲のLSBを表す文字列の取得
  /// @note 範囲を持たない時の値は不定
  string
  right_range_string() const override;

  /// @brief サイズを返す．
  /// このクラスは 1 を返す．
  SizeType
  bit_size() const override;

  /// @brief 対応する宣言要素を返す．
  /// @note このクラスでは nullptr を返す．
  const VlDecl*
  decl() const override;

  /// @brief 親のモジュールの取得
  /// @return このクラスは nullptr を返す．
  const VlModule*
  module() const override;

  /// @brief 親のUDP定義を返す．
  const VlUdpDefn*
  udp_defn() const override;

  /// @brief 親のタスクの取得
  /// @return このクラスは nullptr を返す．
  const VlTaskFunc*
  task() const override;

  /// @brief 親の関数の取得
  /// @return このクラスは nullptr を返す．
  const VlTaskFunc*
  function() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // 設定用の関数 (EiUdpDefn が使う)
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を設定する．
  /// @param[in] pt_header パース木のIO宣言ヘッダ
  /// @param[in] pt_item パース木のIO宣言定義
  void
  set(const PtIOHead* pt_header,
      const PtIOItem* pt_item);


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
  friend class EiFactory;
  friend class EiUdpDefn;

private:

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
  val(SizeType pos) const override;

  /// @brief 一行文の内容を表す文字列をつくる．
  string
  str() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbTableEntry の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 設定する．
  void
  set(const PtUdpEntry* pt_entry,
      const vector<VlUdpVal>& vals) override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親の UDP
  ElbUdpDefn* mUdp;

  // パース木の定義
  const PtUdpEntry* mPtUdpEntry;

  // 値の配列 (実体は EiUdpDefn が持っている)
  VlUdpVal* mValArray;

};

END_NAMESPACE_YM_VERILOG

#endif // EIUDP_H
