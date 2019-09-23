﻿#ifndef SPTMODULE_H
#define SPTMODULE_H

/// @file SptModule.h
/// @brief SptModule のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym/FileRegion.h"
#include "ym/pt/PtArray.h"
#include "ym/pt/PtModule.h"
#include "parser/PtiDecl.h"


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
	    PtDeclHeadArray paramport_array,
	    PtPortArray port_array,
	    PtIOHeadArray iodecl_array,
	    PtDeclHeadArray decl_array,
	    PtItemArray item_array);

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

  /// @brief パラメータポート宣言配列の取得
  PtDeclHeadArray
  paramport_array() const override;

  /// @brief ポートのリストの取得
  PtPortArray
  port_list() const override;

  /// @brief 入出力宣言ヘッダ配列の取得
  PtIOHeadArray
  iohead_array() const override;

  /// @brief 入出力宣言の要素数の取得
  /// @note 個々のヘッダが持つ要素数の総和を計算する．
  SizeType
  iodecl_num() const override;

  /// @brief 宣言ヘッダ配列の取得
  PtDeclHeadArray
  declhead_array() const override;

  /// @brief item 配列の取得
  PtItemArray
  item_array() const override;

  /// 関数名から関数の検索
  const PtItem*
  find_function(const char* name) const override;

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
  PtDeclHeadArray mParamPortArray;

  // ポートの配列
  PtPortArray mPortArray;

  // 入出力宣言の配列
  PtIOHeadArray mIOHeadArray;

  // 入出力宣言の要素数
  SizeType mIODeclNum;

  // 宣言リスト
  PtDeclHeadArray mDeclHeadArray;

  // 要素のリスト
  PtItemArray mItemArray;

  // 関数定義の辞書
  unordered_map<string, const PtItem*> mFuncDic;

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
	  PtExprArray portref_array,
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
  int
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
  PtExprArray mPortRefArray;

  // portref の方向の配列
  VpiDir* mDirArray;

};

END_NAMESPACE_YM_VERILOG

#endif // SPTMODULE_H