#ifndef SPTMODULE_H
#define SPTMODULE_H

/// @file SptModule.h
/// @brief SptModule のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym/FileRegion.h"
#include "ym/pt/PtModule.h"
#include "parser/PtiDecl.h"
#include "parser/PtiArray.h"
#include "parser/PtiFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// module を表すノード
//////////////////////////////////////////////////////////////////////
class SptModule :
  public PtModule
{
  friend class SptFactory;

private:

  /// コンストラクタ
  SptModule(const FileRegion& file_region,
	    const char* name,
	    bool macro,
	    bool is_cell,
	    bool is_protected,
	    int time_unit,
	    int time_precision,
	    VpiNetType net_type,
	    VpiUnconnDrive unconn,
	    VpiDefDelayMode delay,
	    int decay,
	    bool explicit_name,
	    bool portfaults,
	    bool suppress_faults,
	    const string& config,
	    const string& library,
	    const string& cell,
	    PtiDeclHeadArray&& paramport_array,
	    PtiPortArray&& port_array,
	    PtiIOHeadArray&& iodecl_array,
	    PtiDeclHeadArray&& decl_array,
	    PtiItemArray&& item_array);

  /// デストラクタ
  ~SptModule();


public:
  //////////////////////////////////////////////////////////////////////
  // PtModule の継承クラスが実装しなければならない仮想関数
  //////////////////////////////////////////////////////////////////////

  /// ファイル位置の取得
  FileRegion
  file_region() const override;

  /// 名前の取得
  const char*
  name() const override;

  /// macromodule 情報の取得
  bool
  is_macromodule() const override;

  /// cell 情報の取得
  bool
  is_cell() const override;

  /// protect 情報の取得
  bool
  is_protected() const override;

  /// time unit の取得
  int
  time_unit() const override;

  /// time precision の取得
  int
  time_precision() const override;

  /// default net type の取得
  VpiNetType
  nettype() const override;

  /// unconnected drive の取得
  VpiUnconnDrive
  unconn_drive() const override;

  /// default delay mode の取得
  VpiDefDelayMode
  delay_mode() const override;

  /// default decay time の取得
  int
  decay_time() const override;

  /// portfaults 情報の取得
  bool
  portfaults() const override;

  /// suppress_faults 情報の取得
  bool
  suppress_faults() const override;

  /// config 情報の取得
  const string&
  config() const override;

  /// library 情報の取得
  const string&
  library() const override;

  /// cell 情報の取得
  const string&
  cell() const override;

  /// @brief パラメータポート宣言配列の要素数の取得
  SizeType
  paramport_num() const override;

  /// @brief パラメータポート宣言の取得
  /// @param[in] pos 位置 ( 0 <= pos < paramport_num() )
  const PtDeclHead*
  paramport(SizeType pos) const override;

  /// @brief ポート数を取り出す．
  SizeType
  port_num() const override;

  /// @brief ポートを取り出す．
  /// @param[in] pos 位置 ( 0 <= pos < port_num() )
  const PtPort*
  port(SizeType pos) const override;

  /// @brief 入出力宣言ヘッダ配列の要素数の取得
  SizeType
  iohead_num() const override;

  /// @brief 入出力宣言の取得
  /// @param[in] pos 位置 ( 0 <= pos < iohead_num() )
  const PtIOHead*
  iohead(SizeType pos) const override;

  /// @brief 入出力宣言の要素数の取得
  /// @note 個々のヘッダが持つ要素数の総和を計算する．
  SizeType
  iodecl_num() const override;

  /// @brief 宣言ヘッダ配列の要素数の取得
  SizeType
  declhead_num() const override;

  /// @brief 宣言ヘッダの取得
  /// @param[in] pos 位置 ( 0 <= pos < declhead_num() )
  const PtDeclHead*
  declhead(SizeType pos) const override;

  /// @brief item 配列の要素数の取得
  SizeType
  item_num() const override;

  /// @brief item の取得
  /// @param[in] pos 位置 ( 0 <= pos < item_num() )
  const PtItem*
  item(SizeType pos) const override;

  /// top_module フラグを下ろす
  void
  clear_topmodule() const override;

  /// top module のチェック
  bool
  is_topmodule() const override;

  /// @brief in_use フラグの設定
  void
  set_in_use() const override;

  /// @brief in_use フラグの解除
  void
  reset_in_use() const override;

  /// @brief in_use フラグの取得
  bool
  is_in_use() const override;


private:

  // すべてのポートが名前を持っていることを記録する．
  void
  set_named_port();

  // すべてのポートが外部名を持っているときに true を返す．
  // { a, b } のような名無しポートがあると false となる．
  // true の時しか名前による結合は行えない．
  bool
  explicit_name() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 名前
  const char* mName;

  // 様々な情報をパックしたもの
  mutable
  ymuint32 mFlags;

  // decay time
  ymint32 mDefDecayTime;

  // config 情報
  string mConfig;

  // library 情報
  string mLibrary;

  // cell 情報
  string mCell;

  // パラメータポート宣言の配列
  PtiDeclHeadArray mParamPortArray;

  // ポートの配列
  PtiPortArray mPortArray;

  // 入出力宣言の配列
  PtiIOHeadArray mIOHeadArray;

  // 入出力宣言の要素数
  SizeType mIODeclNum;

  // 宣言リスト
  PtiDeclHeadArray mDeclHeadArray;

  // 要素のリスト
  PtiItemArray mItemArray;

};


//////////////////////////////////////////////////////////////////////
/// port を表すクラス
//////////////////////////////////////////////////////////////////////
class SptPort :
  public PtiPort
{
  friend class SptFactory;

private:

  /// @brief コンストラクタ
  SptPort(const FileRegion& file_region,
	  const PtExpr* portref,
	  PtiExprArray&& portref_array,
	  const char* ext_name);

  /// @brief デストラクタ
  ~SptPort();


public:
  //////////////////////////////////////////////////////////////////////
  // PtPort の継承クラスが実装しなければならない仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置の取得
  FileRegion
  file_region() const override;

  /// @brief 外向の名前の取得
  const char*
  ext_name() const override;

  /// @brief 内側のポート結線を表す式の取得
  const PtExpr*
  portref() const override;

  /// @brief 内部のポート結線リストのサイズの取得
  SizeType
  portref_size() const override;

  /// @brief 内部のポート結線の取得
  const PtExpr*
  portref_elem(int pos) const override;

  /// @brief 内部のポート結線の方向の取得
  VpiDir
  portref_dir(int pos) const override;


public:
  //////////////////////////////////////////////////////////////////////
  // PtiPort の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief portref の方向を設定する．
  /// @param[in] pos 位置番号 ( 0 <= pos < portref_num() )
  /// @param[in] dir 方向
  void
  _set_portref_dir(int pos,
		   VpiDir dir) override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 外部向きの名前
  const char* mExtName;

  // portef
  const PtExpr* mPortRef;

  // portref の配列
  PtiExprArray mPortRefArray;

  // portref の方向の配列
  VpiDir* mDirArray;

};

END_NAMESPACE_YM_VERILOG

#endif // SPTMODULE_H
