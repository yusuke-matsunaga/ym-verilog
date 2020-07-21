#ifndef CPTMODULE_H
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

#include "parser/PtiDecl.h"


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
	    PtIOHeadArray iohead_array,
	    PtDeclHeadArray declhead_array,
	    PtItemArray item_array);

  /// @brief デストラクタ
  ~CptModule();


public:
  //////////////////////////////////////////////////////////////////////
  // PtModule の継承クラスが実装しなければならない仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置の取得
  FileRegion
  file_region() const override;

  /// @brief 名前の取得
  const char*
  name() const override;

  /// @brief macromodule 情報の取得
  bool
  is_macromodule() const override;

  /// @brief cell 情報の取得
  bool
  is_cell() const override;

  /// @brief protect 情報の取得
  bool
  is_protected() const override;

  /// @brief time unit の取得
  int
  time_unit() const override;

  /// @brief time precision の取得
  int
  time_precision() const override;

  /// @brief default net type の取得
  VpiNetType
  nettype() const override;

  /// @brief unconnected drive の取得
  VpiUnconnDrive
  unconn_drive() const override;

  /// @brief default delay mode の取得
  VpiDefDelayMode
  delay_mode() const override;

  /// @brief default decay time の取得
  int
  decay_time() const override;

  /// @brief portfaults 情報の取得
  bool
  portfaults() const override;

  /// @brief suppress_faults 情報の取得
  bool
  suppress_faults() const override;

  /// @brief config 情報の取得
  const string&
  config() const override;

  /// @brief library 情報の取得
  const string&
  library() const override;

  /// @brief cell 情報の取得
  const string&
  cell() const override;

  /// @brief パラメータポート宣言配列の取得
  PtDeclHeadArray
  paramport_array() const override;

  /// @brief ポートのリストを取り出す．
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

  /// @brief 関数名から関数の検索
  const PtItem*
  find_function(const char* name) const override;

  /// @brief top_module フラグを下ろす．
  void
  clear_topmodule() const override;

  /// @brief top module のチェック
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

  // パラメータポート宣言のリスト
  PtDeclHeadArray mParamPortArray;

  // ポートの配列
  PtPortArray mPortArray;

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
  FileRegion
  file_region() const override;

  /// @brief 外向の名前の取得
  /// @return 外向の名前(本当のポート名)
  /// @return 無い場合は nullptr を返す
  const char*
  ext_name() const override;

  /// @brief 内側のポート結線を表す式の取得
  const PtExpr*
  portref() const override;

  /// @brief 内部のポート結線リストのサイズの取得
  int
  portref_size() const override;

  /// @brief 内部のポート結線リストの取得
  /// @param[in] pos 位置番号 ( 0 <= pos < portref_num() )
  const PtExpr*
  portref_elem(int pos) const override;

  /// @brief 内部ポート結線の方向の取得
  /// @param[in] pos 位置番号 ( 0 <= pos < portref_num() )
  VpiDir
  portref_dir(int pos) const override;


public:
  //////////////////////////////////////////////////////////////////////
  // 設定用の関数
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
  ~CptPort1();


public:
  //////////////////////////////////////////////////////////////////////
  // PtPort の継承クラスが実装しなければならない仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内側のポート結線を表す式の取得
  const PtExpr*
  portref() const override;

  /// @brief 内部のポート結線リストのサイズの取得
  int
  portref_size() const override;

  /// @brief 内部のポート結線リストの取得
  /// @param[in] pos 位置番号 ( 0 <= pos < portref_num() )
  const PtExpr*
  portref_elem(int pos) const override;

  ///@brief 内部ポート結線の方向の取得
  /// @param[in] pos 位置番号 ( 0 <= pos < portref_num() )
  VpiDir
  portref_dir(int pos) const override;


public:
  //////////////////////////////////////////////////////////////////////
  // 設定用の関数
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

  // 内部向きの接続を表す式
  const PtExpr* mPortRef;

  // 方向
  VpiDir mDir;

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
	   const char* ext_name);

  /// @brief デストラクタ
  ~CptPort2();


public:
  //////////////////////////////////////////////////////////////////////
  // PtPort の継承クラスが実装しなければならない仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内部のポート結線リストのサイズの取得
  int
  portref_size() const override;

  /// @brief 内部のポート結線リストの取得
  /// @param[in] pos 位置番号 ( 0 <= pos < portref_num() )
  const PtExpr*
  portref_elem(int pos) const override;

  ///@brief 内部ポート結線の方向の取得
  /// @param[in] pos 位置番号 ( 0 <= pos < portref_num() )
  VpiDir
  portref_dir(int pos) const override;


public:
  //////////////////////////////////////////////////////////////////////
  // 設定用の関数
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

  // ポート参照式の配列
  PtExprArray mPortRefArray;

  // 方向の配列
  VpiDir* mDirArray;

};

END_NAMESPACE_YM_VERILOG

#endif // CPTMODULE_H
