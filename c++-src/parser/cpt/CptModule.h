#ifndef CPTMODULE_H
#define CPTMODULE_H

/// @file CptModule.h
/// @brief CptModule のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.

#include "ym/pt/PtModule.h"
#include "ym/pt/PtP.h"
#include "ym/FileRegion.h"

#include "parser/PtiArray.h"
#include "parser/PtiDecl.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @brief module を表すノード
//////////////////////////////////////////////////////////////////////
class CptModule :
  public PtModule
{
public:

  /// @brief コンストラクタ
  CptModule(
    const FileRegion& file_region,
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
    PtiIOHeadArray&& iohead_array,
    PtiDeclHeadArray&& declhead_array,
    PtiItemArray&& item_array
  );

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

  /// @brief パラメータポート宣言配列の要素数の取得
  SizeType
  paramport_num() const override;

  /// @brief パラメータポート宣言の取得
  const PtDeclHead*
  paramport(
    SizeType pos ///< [in] 位置 ( 0 <= pos < paramport_num() )
  ) const override;

  /// @brief ポート数を取り出す．
  SizeType
  port_num() const override;

  /// @brief ポートを取り出す．
  const PtPort*
  port(
    SizeType pos ///< [in] 位置 ( 0 <= pos < port_num() )
  ) const override;

  /// @brief 入出力宣言ヘッダ配列の要素数の取得
  SizeType
  iohead_num() const override;

  /// @brief 入出力宣言の取得
  const PtIOHead*
  iohead(
    SizeType pos ///< [in] 位置 ( 0 <= pos < iohead_num() )
  ) const override;

  /// @brief 入出力宣言の要素数の取得
  SizeType
  iodecl_num() const override;

  /// @brief 宣言ヘッダ配列の要素数の取得
  SizeType
  declhead_num() const override;

  /// @brief 宣言ヘッダの取得
  const PtDeclHead*
  declhead(
    SizeType pos ///< [in] 位置 ( 0 <= pos < declhead_num() )
  ) const override;

  /// @brief item 配列の要素数の取得
  SizeType
  item_num() const override;

  /// @brief item の取得
  const PtItem*
  item(
    SizeType pos ///< [in] 位置 ( 0 <= pos < item_num() )
  ) const override;

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
  std::uint32_t mFlags;

  // decay time
  std::int32_t mDefDecayTime;

  // config 情報
  string mConfig;

  // library 情報
  string mLibrary;

  // cell 情報
  string mCell;

  // パラメータポート宣言のリスト
  PtiDeclHeadArray mParamPortArray;

  // ポートの配列
  PtiPortArray mPortArray;

  // 入出力宣言リスト
  PtiIOHeadArray mIOHeadArray;

  // 入出力宣言の要素数
  int mIODeclNum;

  // 宣言リスト
  PtiDeclHeadArray mDeclHeadArray;

  // 要素のリスト
  PtiItemArray mItemArray;

};


//////////////////////////////////////////////////////////////////////
/// @brief port を表すクラス
//////////////////////////////////////////////////////////////////////
class CptPort :
  public PtiPort
{
public:

  /// @brief コンストラクタ
  CptPort(
    const FileRegion& file_region,
    const char* ext_name
  );

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
  SizeType
  portref_size() const override;

  /// @brief 内部のポート結線リストの取得
  const PtExpr*
  portref_elem(
    int pos ///< [in] 位置番号 ( 0 <= pos < portref_num() )
  ) const override;

  /// @brief 内部ポート結線の方向の取得
  VpiDir
  portref_dir(
    int pos ///< [in] 位置番号 ( 0 <= pos < portref_num() )
  ) const override;


public:
  //////////////////////////////////////////////////////////////////////
  // 設定用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief portref の方向を設定する．
  void
  _set_portref_dir(
    int pos,   ///< [in] 位置番号 ( 0 <= pos < portref_num() )
    VpiDir dir ///< [in] 方向
  ) override;


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
public:

  /// @brief コンストラクタ
  CptPort1(
    const FileRegion& file_region,
    const PtExpr* portref,
    const char* ext_name
  );

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
  SizeType
  portref_size() const override;

  /// @brief 内部のポート結線リストの取得
  const PtExpr*
  portref_elem(
    int pos ///< [in] 位置番号 ( 0 <= pos < portref_num() )
  ) const override;

  ///@brief 内部ポート結線の方向の取得
  VpiDir
  portref_dir(
    int pos ///< [in] 位置番号 ( 0 <= pos < portref_num() )
  ) const override;


public:
  //////////////////////////////////////////////////////////////////////
  // 設定用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief portref の方向を設定する．
  void
  _set_portref_dir(
    int pos,   ///< [in] 位置番号 ( 0 <= pos < portref_num() )
    VpiDir dir ///< [in] 方向
  ) override;


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
public:

  /// @brief コンストラクタ
  CptPort2(
    const FileRegion& file_region,
    const PtExpr* portref,
    PtiExprArray&& portref_array,
    const char* ext_name,
    void* q
  );

  /// @brief デストラクタ
  ~CptPort2();


public:
  //////////////////////////////////////////////////////////////////////
  // PtPort の継承クラスが実装しなければならない仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内部のポート結線リストのサイズの取得
  SizeType
  portref_size() const override;

  /// @brief 内部のポート結線リストの取得
  const PtExpr*
  portref_elem(
    int pos ///< [in] 位置番号 ( 0 <= pos < portref_num() )
  ) const override;

  ///@brief 内部ポート結線の方向の取得
  VpiDir
  portref_dir(
    int pos ///< [in] 位置番号 ( 0 <= pos < portref_num() )
  ) const override;


public:
  //////////////////////////////////////////////////////////////////////
  // 設定用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief portref の方向を設定する．
  void
  _set_portref_dir(
    int pos,   ///< [in] 位置番号 ( 0 <= pos < portref_num() )
    VpiDir dir ///< [in] 方向
  ) override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ポート参照式の配列
  PtiExprArray mPortRefArray;

  // 方向の配列
  VpiDir* mDirArray;

};

END_NAMESPACE_YM_VERILOG

#endif // CPTMODULE_H
