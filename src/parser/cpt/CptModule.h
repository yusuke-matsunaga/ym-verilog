﻿#ifndef CPTMODULE_H
#define CPTMODULE_H

/// @file CptModule.h
/// @brief CptModule のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym/pt/PtModule.h"
#include "ym/pt/PtArray.h"
#include "ym/pt/PtP.h"

#include "ym/FileRegion.h"

#include "PtiDecl.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @brief module を表すノード
//////////////////////////////////////////////////////////////////////
class CptModule :
  public PtModule
{
  friend class CptFactory;

private:

  /// @brief コンストラクタ
  CptModule(const FileRegion& file_region,
	    const char* name,
	    bool macro,
	    bool is_cell,
	    bool is_protected,
	    int time_unit,
	    int time_precision,
	    tVpiNetType net_type,
	    tVpiUnconnDrive unconn,
	    tVpiDefDelayMode delay,
	    int decay,
	    bool explicit_name,
	    bool portfaults,
	    bool suppress_faults,
	    const string& config,
	    const string& library,
	    const string& cell,
	    PtDeclHeadArray paramport_array,
	    PtiPortArray port_array,
	    PtIOHeadArray iohead_array,
	    PtDeclHeadArray declhead_array,
	    PtItemArray item_array);

  /// @brief デストラクタ
  virtual
  ~CptModule();


public:
  //////////////////////////////////////////////////////////////////////
  // PtModule の継承クラスが実装しなければならない仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置の取得
  virtual
  FileRegion
  file_region() const;

  /// @brief 名前の取得
  virtual
  const char*
  name() const;

  /// @brief macromodule 情報の取得
  virtual
  bool
  is_macromodule() const;

  /// @brief cell 情報の取得
  virtual
  bool
  is_cell() const;

  /// @brief protect 情報の取得
  virtual
  bool
  is_protected() const;

  /// @brief time unit の取得
  virtual
  int
  time_unit() const;

  /// @brief time precision の取得
  virtual
  int
  time_precision() const;

  /// @brief default net type の取得
  virtual
  tVpiNetType
  nettype() const;

  /// @brief unconnected drive の取得
  virtual
  tVpiUnconnDrive
  unconn_drive() const;

  /// @brief default delay mode の取得
  virtual
  tVpiDefDelayMode
  delay_mode() const;

  /// @brief default decay time の取得
  virtual
  int
  decay_time() const;

  /// @brief portfaults 情報の取得
  virtual
  bool
  portfaults() const;

  /// @brief suppress_faults 情報の取得
  virtual
  bool
  suppress_faults() const;

  /// @brief config 情報の取得
  virtual
  const string&
  config() const;

  /// @brief library 情報の取得
  virtual
  const string&
  library() const;

  /// @brief cell 情報の取得
  virtual
  const string&
  cell() const;

  /// @brief パラメータポート宣言配列の取得
  virtual
  PtDeclHeadArray
  paramport_array() const;

  /// @brief ポート数の取得
  /// @return ポート数
  virtual
  int
  port_num() const;

  /// @brief ポートの取得
  virtual
  const PtPort*
  port(int pos) const;

  /// @brief 入出力宣言ヘッダ配列の取得
  virtual
  PtIOHeadArray
  iohead_array() const;

  /// @brief 入出力宣言の要素数の取得
  /// @note 個々のヘッダが持つ要素数の総和を計算する．
  virtual
  int
  iodecl_num() const;

  /// @brief 宣言ヘッダ配列の取得
  virtual
  PtDeclHeadArray
  declhead_array() const;

  /// @brief item 配列の取得
  virtual
  PtItemArray
  item_array() const;

  /// @brief 関数名から関数の検索
  virtual
  const PtItem*
  find_function(const char* name) const;

  /// @brief top_module フラグを下ろす．
  virtual
  void
  clear_topmodule() const;

  /// @brief top module のチェック
  virtual
  bool
  is_topmodule() const;

  /// @brief in_use フラグの設定
  virtual
  void
  set_in_use() const;

  /// @brief in_use フラグの解除
  virtual
  void
  reset_in_use() const;

  /// @brief in_use フラグの取得
  virtual
  bool
  is_in_use() const;


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

  // パラメータポート宣言のリスト
  PtDeclHeadArray mParamPortArray;

  // ポートの配列
  PtiPortArray mPortArray;

  // 入出力宣言リスト
  PtIOHeadArray mIOHeadArray;

  // 入出力宣言の要素数
  int mIODeclNum;

  // 宣言リスト
  PtDeclHeadArray mDeclHeadArray;

  // 要素のリスト
  PtItemArray mItemArray;

  // 関数定義の辞書
  unordered_map<string, const PtItem*> mFuncDic;

};


//////////////////////////////////////////////////////////////////////
/// @brief port を表すクラス
//////////////////////////////////////////////////////////////////////
class CptPort :
  public PtiPort
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  CptPort(const FileRegion& file_region,
	  const char* ext_name);

  /// @brief デストラクタ
  ~CptPort();


public:
  //////////////////////////////////////////////////////////////////////
  // PtPort の継承クラスが実装しなければならない仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置の取得
  /// @return ファイル位置
  virtual
  FileRegion
  file_region() const;

  /// @brief 外向の名前の取得
  /// @return 外向の名前(本当のポート名)
  /// @return 無い場合は nullptr を返す
  virtual
  const char*
  ext_name() const;

  /// @brief 内側のポート結線を表す式の取得
  virtual
  const PtExpr*
  portref() const;

  /// @brief 内部のポート結線リストのサイズの取得
  virtual
  int
  portref_size() const;

  /// @brief 内部のポート結線リストの取得
  /// @param[in] pos 位置番号 ( 0 <= pos < portref_num() )
  virtual
  const PtExpr*
  portref_elem(int pos) const;

  /// @brief 内部ポート結線の方向の取得
  /// @param[in] pos 位置番号 ( 0 <= pos < portref_num() )
  virtual
  tVlDirection
  portref_dir(int pos) const;


public:
  //////////////////////////////////////////////////////////////////////
  // 設定用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief portref の方向を設定する．
  /// @param[in] pos 位置番号 ( 0 <= pos < portref_num() )
  /// @param[in] dir 方向
  virtual
  void
  _set_portref_dir(int pos,
		   tVlDirection dir);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 外部向きの名前
  const char* mExtName;

};


//////////////////////////////////////////////////////////////////////
/// @brief port を表すクラス (portref リスト付き)
//////////////////////////////////////////////////////////////////////
class CptPort1 :
  public CptPort
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  CptPort1(const FileRegion& file_region,
	   const PtExpr* portref,
	   const char* ext_name);

  /// @brief デストラクタ
  virtual
  ~CptPort1();


public:
  //////////////////////////////////////////////////////////////////////
  // PtPort の継承クラスが実装しなければならない仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内側のポート結線を表す式の取得
  virtual
  const PtExpr*
  portref() const;

  /// @brief 内部のポート結線リストのサイズの取得
  virtual
  int
  portref_size() const;

  /// @brief 内部のポート結線リストの取得
  /// @param[in] pos 位置番号 ( 0 <= pos < portref_num() )
  virtual
  const PtExpr*
  portref_elem(int pos) const;

  ///@brief 内部ポート結線の方向の取得
  /// @param[in] pos 位置番号 ( 0 <= pos < portref_num() )
  virtual
  tVlDirection
  portref_dir(int pos) const;


public:
  //////////////////////////////////////////////////////////////////////
  // 設定用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief portref の方向を設定する．
  /// @param[in] pos 位置番号 ( 0 <= pos < portref_num() )
  /// @param[in] dir 方向
  virtual
  void
  _set_portref_dir(int pos,
		   tVlDirection dir);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 内部向きの接続を表す式
  const PtExpr* mPortRef;

  // 方向
  tVlDirection mDir;

};


//////////////////////////////////////////////////////////////////////
/// @brief port を表すクラス (portref リスト付き)
//////////////////////////////////////////////////////////////////////
class CptPort2 :
  public CptPort1
{
  friend class CptFactory;

private:

  /// @brief コンストラクタ
  CptPort2(const FileRegion& file_region,
	   const PtExpr* portref,
	   PtExprArray portref_array,
	   tVlDirection* dir_array,
	   const char* ext_name);

  /// @brief デストラクタ
  virtual
  ~CptPort2();


public:
  //////////////////////////////////////////////////////////////////////
  // PtPort の継承クラスが実装しなければならない仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内部のポート結線リストのサイズの取得
  virtual
  int
  portref_size() const;

  /// @brief 内部のポート結線リストの取得
  /// @param[in] pos 位置番号 ( 0 <= pos < portref_num() )
  virtual
  const PtExpr*
  portref_elem(int pos) const;

  ///@brief 内部ポート結線の方向の取得
  /// @param[in] pos 位置番号 ( 0 <= pos < portref_num() )
  virtual
  tVlDirection
  portref_dir(int pos) const;


public:
  //////////////////////////////////////////////////////////////////////
  // 設定用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief portref の方向を設定する．
  /// @param[in] pos 位置番号 ( 0 <= pos < portref_num() )
  /// @param[in] dir 方向
  virtual
  void
  _set_portref_dir(int pos,
		   tVlDirection dir);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ポート参照式
  const PtExpr* mPortRef;

  // ポート参照式の配列
  PtExprArray mPortRefArray;

  // 方向の配列
  tVlDirection* mDirArray;

};

END_NAMESPACE_YM_VERILOG

#endif // CPTMODULE_H
