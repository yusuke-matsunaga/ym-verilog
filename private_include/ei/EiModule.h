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
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_module モジュールテンプレート
  /// @param[in] pt_head パース木のヘッダ定義
  /// @param[in] pt_inst インスタンス定義
  EiModuleHead(const VlScope* parent,
	       const PtModule* pt_module,
	       const PtItem* pt_head,
	       const PtInst* pt_inst);

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
  /// @param[in] port_num ポート数
  /// @param[in] io_num IO 数
  void
  init(SizeType port_num,
       SizeType io_num);


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
  /// @param[in] pos 取得するポートの位置 (0 <= pos < port_num())
  /// @return pos 番目のポートを返す．
  const VlPort*
  port(SizeType pos) const override;

  /// @brief 入出力数を得る．
  SizeType
  io_num() const override;

  /// @brief 入出力の取得
  /// @param[in] pos 位置番号 ( 0 <= pos < io_num() )
  const VlIODecl*
  io(SizeType pos) const override;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbModule の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入出力を追加する．
  /// @param[in] head ヘッダ
  /// @param[in] pt_item パース木のIO宣言要素
  /// @param[in] decl 対応する宣言要素
  void
  add_iodecl(ElbIOHead* head,
	     const PtIOItem* pt_item,
	     const VlDecl* decl) override;

  /// @brief ポートの初期設定を行う．
  /// @param[in] index ポート番号
  /// @param[in] pt_port パース木のポート定義
  /// @param[in] low_conn 下位の接続
  /// @param[in] dir 向き
  void
  init_port(SizeType index,
	    const PtPort* pt_port,
	    ElbExpr* low_conn,
	    VpiDir dir) override;

  /// @brief ポートの high_conn を接続する．
  /// @param[in] index ポート番号
  /// @param[in] high_conn 上位の接続の式
  /// @param[in] conn_by_name 名前による割り当て時に true とするフラグ
  void
  set_port_high_conn(SizeType index,
		     ElbExpr* high_conn,
		     bool conn_by_name) override;


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
  /// @param[in] port_array ポート配列
  /// @param[in] io_array IO 配列
  /// @param[in] module_array 親の配列
  /// @param[in] index 配列中のインデックス
  void
  init(SizeType port_num,
       SizeType io_num,
       EiModuleArray* module_array,
       int index);


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
  /// @note 配列要素の時のみ意味を持つ．
  int
  index() const override;

  /// @brief 親の配列を返す．
  /// @note 配列要素の時のみ意味を持つ．
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
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_module モジュールテンプレート
  /// @param[in] pt_head パース木のヘッダ定義
  /// @param[in] pt_inst パース木のインスタンス定義
  EiModule2(const VlScope* parent,
	    const PtModule* pt_module,
	    const PtItem* pt_head,
	    const PtInst* pt_inst);

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
  /// @note 配列要素の時のみ意味を持つ．
  int
  index() const override;

  /// @brief 親の配列を返す．
  /// @note 配列要素の時のみ意味を持つ．
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
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_module モジュールテンプレート
  /// @param[in] pt_head パース木のヘッダ定義
  /// @param[in] pt_inst パース木のインスタンス定義
  /// @param[in] range 範囲
  /// @param[in] array モジュール配列
  EiModuleArray(const VlScope* parent,
		const PtModule* pt_module,
		const PtItem* pt_head,
		const PtInst* pt_inst,
		const EiRangeImpl& range);

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
  /// @param[in] offset 位置番号 (0 <= offset < elem_num())
  const VlModule*
  elem_by_offset(SizeType offset) const override;

  /// @brief 要素を返す．
  /// @param[in] index インデックス
  const VlModule*
  elem_by_index(int index) const override;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbModuleArray の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素を取り出す．
  /// @param[in] index モジュール番号
  ElbModule*
  elem(SizeType index) override;


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
