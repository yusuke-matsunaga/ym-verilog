#ifndef EIMODULE_H
#define EIMODULE_H

/// @file EiModule.h
/// @brief EiModule のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.

#include "elaborator/ElbModule.h"
#include "elaborator/ElbModuleArray.h"
#include "ei/EiRange.h"

#include "ym/StrBuff.h"


BEGIN_NAMESPACE_YM_VERILOG

class EiModuleArray;
class EiPort;
class EiIODecl;
class EiExpr;

//////////////////////////////////////////////////////////////////////
/// @calss EiModuleHead EiModule.h "EiModule.h"
/// @brief EiModuleArray と EiModule に共通なヘッダ部分
//////////////////////////////////////////////////////////////////////
class EiModuleHead
{
public:

  /// @brief コンストラクタ
  EiModuleHead(
    const VlScope* parent,     ///< [in] 親のスコープ
    const PtModule* pt_module, ///< [in] モジュールテンプレート
    const PtItem* pt_head,     ///< [in] パース木のヘッダ定義
    const PtInst* pt_inst      ///< [in] インスタンス定義
  );

  /// @brief デストラクタ
  ~EiModuleHead();


public:
  //////////////////////////////////////////////////////////////////////
  // EiModuleHead の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief このオブジェクトの属しているスコープを返す．
  const VlScope*
  parent() const;

  /// @brief ファイル位置の取得
  FileRegion
  file_region() const;

  /// @brief インスタンス名を返す．
  string
  name() const;

  /// @brief definition location を返す．
  /// @return 定義側のファイル位置の情報を返す．
  FileRegion
  def_file_region() const;

  /// @brief definition name を返す．
  /// @return 定義名を返す．
  string
  def_name() const;

  /// @brief ポート数を返す．
  SizeType
  port_num() const;

  /// @brief 入出力宣言数を返す．
  SizeType
  io_num() const;

  /// @brief cell instance のチェック
  /// @return cell instance の場合に true を返す．
  bool
  is_cell_instance() const;

  /// @brief protect のチェック
  /// @return protect されていたら true を返す．
  bool
  is_protected() const;

  /// @brief top module の時 true を返す．
  bool
  is_top_module() const;

  /// @brief time unit を返す．
  /// @return 結果は 2 〜 -15 の整数
  /// @return もしくは未定義を表す -16
  int
  time_unit() const;

  /// @brief time precision を返す．
  /// @return 結果は 2 〜 -15 の整数
  /// @return もしくは未定義を表す -16
  int
  time_precision() const;

  /// @brief default net type を返す．
  VpiNetType
  def_net_type() const;

  /// @brief unconnected drive を返す．
  VpiUnconnDrive
  unconn_drive() const;

  /// @brief default delay mode を返す．
  VpiDefDelayMode
  def_delay_mode() const;

  /// @brief default decay time を返す．
  int
  def_decay_time() const;

  /// @brief config 情報を返す．
  string
  config() const;

  /// @brief library 情報を返す．
  string
  library() const;

  /// @brief cell 情報を返す．
  string
  cell() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親のスコープ
  const VlScope* mParent;

  // パース木のモジュール定義
  const PtModule* mPtModule;

  // パース木のヘッダ定義
  const PtItem* mPtHead;

  // パース木のモジュールインスタンス定義
  const PtInst* mPtInst;

};


//////////////////////////////////////////////////////////////////////
/// @class EiModule EiModule.h "EiModule.h"
/// @brief ElbModule の実装クラスの基底クラス
//////////////////////////////////////////////////////////////////////
class EiModule :
  public ElbModule
{
protected:

  /// @brief コンストラクタ
  EiModule();

  /// @brief デストラクタ
  ~EiModule();


public:

  /// @brief ポート配列とIO配列を初期化する．
  void
  init(
    SizeType port_num, ///< [in] ポート数
    SizeType io_num    ///< [in] IO 数
  );


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
  // VlNamedObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief このオブジェクトの属しているスコープを返す．
  const VlScope*
  parent_scope() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // VlModule の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief definition location を返す．
  /// @return 定義側のファイル位置の情報を返す．
  FileRegion
  def_file_region() const override;

  /// @brief definition name を返す．
  /// @return 定義名を返す．
  string
  def_name() const override;

  /// @brief cell instance のチェック
  /// @return cell instance の場合に true を返す．
  bool
  is_cell_instance() const override;

  /// @brief protect のチェック
  /// @return protect されていたら true を返す．
  bool
  is_protected() const override;

  /// @brief top module の時 true を返す．
  bool
  is_top_module() const override;

  /// @brief time unit を返す．
  /// @return 結果は 2 〜 -15 の整数
  /// @return もしくは未定義を表す -16
  int
  time_unit() const override;

  /// @brief time precision を返す．
  /// @return 結果は 2 〜 -15 の整数
  /// @return もしくは未定義を表す -16
  int
  time_precision() const override;

  /// @brief default net type を返す．
  VpiNetType
  def_net_type() const override;

  /// @brief unconnected drive を返す．
  VpiUnconnDrive
  unconn_drive() const override;

  /// @brief default delay mode を返す．
  VpiDefDelayMode
  def_delay_mode() const override;

  /// @brief default decay time を返す．
  int
  def_decay_time() const override;

  /// @brief config 情報を返す．
  string
  config() const override;

  /// @brief library 情報を返す．
  string
  library() const override;

  /// @brief cell 情報を返す．
  string
  cell() const override;

  /// @brief ポート数を返す．
  SizeType
  port_num() const override;

  /// @brief ポートの取得
  /// @return pos 番目のポートを返す．
  const VlPort*
  port(
    SizeType pos ///< [in] 取得するポートの位置 (0 <= pos < port_num())
  ) const override;

  /// @brief 入出力数を得る．
  SizeType
  io_num() const override;

  /// @brief 入出力の取得
  const VlIODecl*
  io(
    SizeType pos ///< [in] 位置番号 ( 0 <= pos < io_num() )
  ) const override;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbModule の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入出力を追加する．
  void
  add_iodecl(
    ElbIOHead* head,         ///< [in] ヘッダ
    const PtIOItem* pt_item, ///< [in] パース木のIO宣言要素
    const VlDecl* decl       ///< [in] 対応する宣言要素
  ) override;

  /// @brief ポートの初期設定を行う．
  void
  init_port(
    SizeType index,        ///< [in] ポート番号
    const PtPort* pt_port, ///< [in] パース木のポート定義
    ElbExpr* low_conn,     ///< [in] 下位の接続
    VpiDir dir             ///< [in] 向き
  ) override;

  /// @brief ポートの high_conn を接続する．
  void
  set_port_high_conn(
    SizeType index,     ///< [in] ポート番号
    ElbExpr* high_conn, ///< [in] 上位の接続の式
    bool conn_by_name   ///< [in] 名前による割り当て時に true とするフラグ
  ) override;


private:
  //////////////////////////////////////////////////////////////////////
  // EiModule の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ヘッダ情報を返す．
  virtual
  const EiModuleHead&
  head() const = 0;

  /// @brief ヘッダ情報を返す．
  virtual
  EiModuleHead&
  head() = 0;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ポートの配列
  vector<EiPort> mPortList;

  // 入出力の配列
  vector<EiIODecl> mIODeclList;

};


//////////////////////////////////////////////////////////////////////
/// @class EiModule1 EiModule.h "EiModule.h"
/// @brief 配列要素の EiModule
//////////////////////////////////////////////////////////////////////
class EiModule1 :
  public EiModule
{
public:

  /// @brief コンストラクタ
  EiModule1();

  /// @brief デストラクタ
  ~EiModule1();


public:
  //////////////////////////////////////////////////////////////////////
  // 設定用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期設定を行う．
  void
  init(
    SizeType port_num,           ///< [in] ポート配列
    SizeType io_num,             ///< [in] IO 配列
    EiModuleArray* module_array, ///< [in] 親の配列
    int index                    ///< [in] 配列中のインデックス
  );


public:
  //////////////////////////////////////////////////////////////////////
  // VlNamedObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前の取得
  string
  name() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // VlModule の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 配列要素の時 true を返す．
  bool
  is_array() const override;

  /// @brief インデックスの値を返す．
  int
  index() const override;

  /// @brief 親の配列を返す．
  const VlModuleArray*
  module_array() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // EiModule の派生クラスが実装しなければならない仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ヘッダ情報を返す．
  const EiModuleHead&
  head() const override;

  /// @brief ヘッダ情報を返す．
  EiModuleHead&
  head() override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親の配列
  EiModuleArray* mModuleArray;

  // インデックス
  int mIndex;

  // 名前
  StrBuff mName;

};


//////////////////////////////////////////////////////////////////////
/// @class EiModule2 EiModule.h "EiModule.h"
/// @brief 単独要素の EiModule
//////////////////////////////////////////////////////////////////////
class EiModule2 :
  public EiModule
{
public:

  /// @brief コンストラクタ
  EiModule2(
    const VlScope* parent,     ///< [in] 親のスコープ
    const PtModule* pt_module, ///< [in] モジュールテンプレート
    const PtItem* pt_head,     ///< [in] パース木のヘッダ定義
    const PtInst* pt_inst      ///< [in] パース木のインスタンス定義
  );

  /// @brief デストラクタ
  ~EiModule2();


public:
  //////////////////////////////////////////////////////////////////////
  // VlNamedObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前の取得
  string
  name() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // VlModule の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 配列要素の時 true を返す．
  bool
  is_array() const override;

  /// @brief インデックスの値を返す．
  int
  index() const override;

  /// @brief 親の配列を返す．
  const VlModuleArray*
  module_array() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // EiModule の派生クラスが実装しなければならない仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ヘッダ情報を返す．
  const EiModuleHead&
  head() const override;

  /// @brief ヘッダ情報を返す．
  EiModuleHead&
  head() override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ヘッダ情報
  EiModuleHead mHead;

};


//////////////////////////////////////////////////////////////////////
/// @class EiModuleArray EiModule.h "EiModule.h"
/// @brief VlModuleArray の実装クラス
//////////////////////////////////////////////////////////////////////
class EiModuleArray :
  public ElbModuleArray
{
public:

  /// @brief コンストラクタ
  EiModuleArray(
    const VlScope* parent,     ///< [in] 親のスコープ
    const PtModule* pt_module, ///< [in] モジュールテンプレート
    const PtItem* pt_head,     ///< [in] パース木のヘッダ定義
    const PtInst* pt_inst,     ///< [in] パース木のインスタンス定義
    const EiRangeImpl& range   ///< [in] 範囲
  );

  /// @brief デストラクタ
  ~EiModuleArray();


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
  // VlNamedObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief このオブジェクトの属しているスコープを返す．
  const VlScope*
  parent_scope() const override;

  /// @brief 名前の取得
  string
  name() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // VlModuleArray の仮想関数
  //////////////////////////////////////////////////////////////////////

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

  /// @brief 要素を返す．
  const VlModule*
  elem_by_offset(
    SizeType offset ///< [in] 位置番号 (0 <= offset < elem_num())
  ) const override;

  /// @brief 要素を返す．
  const VlModule*
  elem_by_index(
    int index ///< [in] インデックス
  ) const override;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbModuleArray の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素を取り出す．
  ElbModule*
  elem(
    SizeType index ///< [in] モジュール番号
  ) override;


public:
  //////////////////////////////////////////////////////////////////////
  // EiModuleArray の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ヘッダを返す．
  const EiModuleHead&
  head() const;

  /// @brief ヘッダを返す．
  EiModuleHead&
  head();

  /// @brief 範囲を返す．
  const EiRangeImpl&
  range() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ヘッダ
  EiModuleHead mHead;

  // 範囲
  EiRangeImpl mRange;

  // 要素の配列
  vector<EiModule1> mArray;

};

END_NAMESPACE_YM_VERILOG

#endif // EIMODULE_H
