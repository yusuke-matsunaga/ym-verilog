#ifndef PTIFACTORY_H
#define PTIFACTORY_H

/// @file PtiFactory.h
/// @brief PtiFactory のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym/FileRegion.h"
#include "PtiFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

class Alloc;

//////////////////////////////////////////////////////////////////////
/// @class PtiFactory PtiFactory.h "PtiFactory.h"
/// @brief Verilog-HDL のパース木を生成するファクトリクラス
///
/// このクラスは純粋仮想基底クラスなのでインターフェイスの定義しか
/// していない．実際にはこれを継承したクラスを用意する必要がある．
//////////////////////////////////////////////////////////////////////
class PtiFactory
{
public:

  /// @brief PtiFactory の実装クラスのオブジェクトを作る関数
  static
  PtiFactory*
  make_obj(
    const string& type, ///< [in] 実装クラスを指定する文字列
    Alloc& alloc        ///< [in] メモリアロケータ
  );

  /// @brief コンストラクタ
  PtiFactory(
    Alloc& alloc ///< [in] メモリアロケータ
  );

  /// @brief デストラクタ
  virtual
  ~PtiFactory();


public:
  //////////////////////////////////////////////////////////////////////
  // UDP 関係
  //////////////////////////////////////////////////////////////////////

  /// @brief combinational UDP の生成
  /// @return 生成された UDP
  virtual
  const PtUdp*
  new_CmbUdp(
    const FileRegion& fr,                        ///< [in] ファイル位置情報
    const char* name,                            ///< [in] UDP名
    const vector<const PtPort*>& port_array,     ///< [in] ポート名の配列
    const vector<const PtIOHead*>& io_array,     ///< [in] IOの配列
    const vector<const PtUdpEntry*>& entry_array ///< [in] テーブルエントリの配列
  ) = 0;

  /// @brief sequential UDP の生成
  /// @return 生成された UDP
  virtual
  const PtUdp*
  new_SeqUdp(
    const FileRegion& fr,                        ///< [in] ファイル位置情報
    const char* name,				 ///< [in] UDP名
    const vector<const PtPort*>& port_array,	 ///< [in] ポート名の配列
    const vector<const PtIOHead*>& io_array,	 ///< [in] IOの配列
    const PtExpr* init_value,                    ///< [in] 初期値を表す式
    const vector<const PtUdpEntry*>& entry_array ///< [in] テーブルエントリの配列
  ) = 0;

  /// @brief combinational UDP 用のテーブルエントリの生成
  /// @return 生成されたテーブルエントリ
  virtual
  const PtUdpEntry*
  new_UdpEntry(
    const FileRegion& fr,                         ///< [in] ファイル位置の情報
    const vector<const PtUdpValue*>& input_array, ///< [in] 入力値の配列
    const PtUdpValue* output                      ///< [in] 出力値
  ) = 0;

  /// @brief sequential UDP 用のテーブルエントリの生成
  /// @return 生成されたテーブルエントリ
  virtual
  const PtUdpEntry*
  new_UdpEntry(
    const FileRegion& fr,                         ///< [in] ファイル位置の情報
    const vector<const PtUdpValue*>& input_array, ///< [in] 入力値の配列
    const PtUdpValue* current,			  ///< [in] 現状態値
    const PtUdpValue* output                      ///< [in] 出力値
  ) = 0;

  /// @brief UDP のテーブルエントリの要素の値の生成
  /// @return 生成された値
  virtual
  const PtUdpValue*
  new_UdpValue(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    char symbol           ///< [in] シンボル
  ) = 0;

  /// @brief UDP のテーブルエントリの要素の値の生成
  /// @return 生成された値
  virtual
  const PtUdpValue*
  new_UdpValue(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    char symbol1,         ///< [in] シンボル1
    char symbol2          ///< [in] シンボル2
  ) = 0;


  //////////////////////////////////////////////////////////////////////
  // モジュール関係
  //////////////////////////////////////////////////////////////////////

  /// @brief モジュールの生成
  /// @return 生成されたモジュール
  virtual
  const PtModule*
  new_Module(
    const FileRegion& fr,                             ///< [in] ファイル位置の情報
    const char* name,                                 ///< [in] モジュール名
    bool macro,                                       ///< [in] macromodule の時 true となるフラグ
    bool is_cell,                                     ///< [in] `celldefine - `endcelldefine で囲まれていた
                                                      ///<      ときに true となるフラグ
    bool is_protected,                                ///< [in] 保護されているときに true となるフラグ．
                                                      ///<      verilog の構文にこんな情報はない．
    int time_unit,                                    ///< [in] 時刻の単位を表す数値．
    int time_precision,                               ///< [in] 時刻の精度を表す数値．
                                                      ///<      有効な値は 2 - -15 で 2 の時に 100秒を表す．
                                                      ///<      以下，1減るごとに10分の1になる．-16 で未定義を表す．
    VpiNetType net_type,                              ///< [in] 未定義の識別子から暗黙のネットを生成するときのネット型．
    VpiUnconnDrive unconn,                            ///< [in] unconnected_drive の値．
    VpiDefDelayMode delay,                            ///< [in] delay_mode の値．
    int decay,                                        ///< [in] default_decay_time の値．
                                                      ///<      意味のある値は正の整数もしくは無限大をあらわす -1
    bool explicit_name,                               ///< [in] ポートがすべて名前付きのときに true となるフラグ
    bool portfaults,                                  ///< [in] Verifault 用．
    bool suppress_faults,                             ///< [in] Verifault 用
    const string& config,                             ///< [in] config 情報
    const string& library,                            ///< [in] library 情報
    const string& cell,                               ///< [in] cell 情報
    const vector<const PtDeclHead*>& paramport_array, ///< [in] パラメータポートのリスト
    const vector<const PtPort*>& port_array,          ///< [in] ポートのリスト
    const vector<const PtIOHead*>& iodecl_array,      ///< [in] 入出力のリスト
    const vector<const PtDeclHead*>& decl_array,      ///< [in] 宣言のリスト
    const vector<const PtItem*>& item_array           ///< [in] 要素のリスト
  ) = 0;


  //////////////////////////////////////////////////////////////////////
  // ポート関係
  //////////////////////////////////////////////////////////////////////

  /// @brief ポートの生成 (空のポート / 名前のみのポート)
  /// @return 生成されたポート
  virtual
  PtiPort*
  new_Port(
    const FileRegion& fr,          ///< [in] ファイル位置の情報
    const char* ext_name = nullptr ///< [in] ポート名
  ) = 0;

  /// @brief ポートの生成 (ポート参照式があるポート)
  /// @return 生成されたポート
  virtual
  PtiPort*
  new_Port(
    const FileRegion& fr,  ///< [in] ファイル位置の情報
    const PtExpr* portref, ///< [in] ポートに接続している式 (ポート参照式)
    const char* ext_name   ///< [in] ポート名 (空文字列の場合もある)
  ) = 0;

  /// @brief ポートの生成 (連結型のポート参照式があるポート)
  /// @return 生成されたポート
  virtual
  PtiPort*
  new_Port(
    const FileRegion& fr,                       ///< [in] ファイル位置の情報
    const PtExpr* portref,                      ///< [in] ポートに接続している式 (ポート参照式)
    const vector<const PtExpr*>& portref_array, ///< [in] ポートに接続している式 (ポート参照式)の配列
    const char* ext_name                        ///< [in] ポート名 (空文字列の場合もある)
  ) = 0;


  //////////////////////////////////////////////////////////////////////
  // IO 宣言関係
  //////////////////////////////////////////////////////////////////////

  /// @brief 範囲付きの IO 宣言のヘッダの生成
  /// @return 生成された IO宣言ヘッダ
  virtual
  PtiIOHead*
  new_IOHead(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    VpiDir dir,		  ///< [in] IO の種類 (input, inout, output)
    bool sign,		  ///< [in] 符号付きのとき true となるフラグ
    const PtExpr* left,   ///< [in] 範囲の左側の式
    const PtExpr* right   ///< [in] 範囲の右側の式
  ) = 0;

  /// @brief 範囲付きの IO 宣言のヘッダの生成 (reg 型)
  /// @return 生成された IO宣言ヘッダ
  virtual
  PtiIOHead*
  new_RegIOHead(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    VpiDir dir,		  ///< [in] IO の種類 (input, inout, output)
    bool sign,		  ///< [in] 符号付きのとき true となるフラグ
    const PtExpr* left,	  ///< [in] 範囲の左側の式
    const PtExpr* right	  ///< [in] 範囲の右側の式
  ) = 0;

  /// @brief 範囲付きの IO 宣言のヘッダの生成 (ネット型)
  /// @return 生成された IO宣言ヘッダ
  virtual
  PtiIOHead*
  new_NetIOHead(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    VpiDir dir,		  ///< [in] IO の種類 (input, inout, output)
    VpiNetType net_type,  ///< [in] 補助的なネット型
    bool sign,            ///< [in] 符号付きのとき true となるフラグ
    const PtExpr* left,	  ///< [in] 範囲の左側の式
    const PtExpr* right	  ///< [in] 範囲の右側の式
  ) = 0;

  /// @brief IO 宣言のヘッダの生成 (変数型)
  /// @return 生成された IO宣言ヘッダ
  virtual
  PtiIOHead*
  new_VarIOHead(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    VpiDir dir,           ///< [in] IO の種類 (input, inout, output)
    VpiVarType var_type   ///< [in] 補助的な変数型
  ) = 0;

  /// @brief 初期値付き IO 宣言の要素の生成
  /// @return 生成された要素
  virtual
  const PtIOItem*
  new_IOItem(
    const FileRegion& fr,    ///< [in] ファイル位置の情報
    const char* name,        ///< [in] 要素名
    const PtExpr* init_value ///< [in] 初期値を表す式
  ) = 0;


  //////////////////////////////////////////////////////////////////////
  // その他の宣言関係
  //////////////////////////////////////////////////////////////////////

  /// @brief 範囲指定型パラメータ宣言のヘッダの生成
  /// @return 生成されたパラメータ
  virtual
  PtiDeclHead*
  new_ParamH(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    bool sign,            ///< [in] 符号付きのとき true となるフラグ
    const PtExpr* left,   ///< [in] 範囲の左側の式
    const PtExpr* right,  ///< [in] 範囲の右側の式
    bool local = false    ///< [in] local param の時 true にするフラグ
  ) = 0;

  /// @brief 組み込み型パラメータ宣言のヘッダの生成
  /// @return 生成されたパラメータ
  virtual
  PtiDeclHead*
  new_ParamH(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    VpiVarType var_type,  ///< [in] データ型
    bool local = false    ///< [in] local param の時 true にするフラグ
  ) = 0;

  /// @brief 範囲指定型 specparam 宣言のヘッダの生成
  /// @return 生成された specparam
  virtual
  PtiDeclHead*
  new_SpecParamH(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const PtExpr* left,   ///< [in] 範囲の左側の式
    const PtExpr* right   ///< [in] 範囲の右側の式
  ) = 0;

  /// @brief イベント宣言のヘッダの生成
  /// @return 生成されたイベント
  virtual
  PtiDeclHead*
  new_EventH(
    const FileRegion& fr ///< [in] ファイル位置の情報
  ) = 0;

  /// @brief genvar 宣言のヘッダの生成
  /// @return 生成された genvar
  virtual
  PtiDeclHead*
  new_GenvarH(
    const FileRegion& fr ///< [in] ファイル位置の情報
  ) = 0;

  /// @brief 変数宣言のヘッダの生成
  /// @return 生成された変数
  virtual
  PtiDeclHead*
  new_VarH(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    VpiVarType var_type   ///< [in] データ型
  ) = 0;

  /// @brief 範囲指定型 reg 宣言のヘッダの生成
  /// @return 生成された reg
  virtual
  PtiDeclHead*
  new_RegH(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    bool sign,            ///< [in] 符号付きの時 true となるフラグ
    const PtExpr* left,   ///< [in] 範囲の左側の式
    const PtExpr* right   ///< [in] 範囲の右側の式
  ) = 0;

  /// @brief 範囲指定型 net 宣言のヘッダの生成 (strength, 遅延あり)
  /// @return 生成されたネット
  virtual
  PtiDeclHead*
  new_NetH(
    const FileRegion& fr,       ///< [in] ファイル位置の情報
    VpiNetType type,	        ///< [in] net の型
    VpiVsType vstype,	        ///< [in] vector/scalar 指定
    bool sign,		        ///< [in] 符号の有無を表すフラグ
    const PtExpr* left,	        ///< [in] 範囲の左側の式
    const PtExpr* right,        ///< [in] 範囲の右側の式
    const PtStrength* strength, ///< [in] 信号強度
    const PtDelay* delay        ///< [in] 遅延
  ) = 0;

  /// @brief 宣言要素の生成
  /// @return 生成された要素
  virtual
  const PtDeclItem*
  new_DeclItem(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* name      ///< [in] 名前
  ) = 0;

  /// @brief 初期値付き宣言要素の生成
  /// @return 生成された要素
  virtual
  const PtDeclItem*
  new_DeclItem(
    const FileRegion& fr,    ///< [in] ファイル位置の情報
    const char* name,        ///< [in] 名前
    const PtExpr* init_value ///< [in] 初期値を表す式
  ) = 0;

  /// @brief 配列型宣言要素の生成
  /// @return 生成された要素
  virtual
  const PtDeclItem*
  new_DeclItem(
    const FileRegion& fr,                     ///< [in] ファイル位置の情報
    const char* name,                         ///< [in] 名前
    const vector<const PtRange*>& range_array ///< [in] 配列の各次元の範囲のリスト
  ) = 0;

  /// @brief 範囲の生成
  virtual
  const PtRange*
  new_Range(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const PtExpr* msb,    ///< [in] MSB を表す式
    const PtExpr* lsb     ///< [in] LSB を表す式
  ) = 0;


  //////////////////////////////////////////////////////////////////////
  // item 関係
  //////////////////////////////////////////////////////////////////////

  /// @brief defparam 文のヘッダの生成
  /// @return 生成された defparam ヘッダ
  virtual
  const PtItem*
  new_DefParamH(
    const FileRegion& fr,                       ///< [in] ファイル位置の情報
    const vector<const PtDefParam*>& elem_array ///< [in] 要素のリスト
  ) = 0;

  /// @brief defparam 文の要素の生成
  /// @return 生成された要素
  virtual
  const PtDefParam*
  new_DefParam(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* name,     ///< [in] 名前
    const PtExpr* value   ///< [in] 値を表す式
  ) = 0;

  /// @brief defparam 文の要素の生成 (階層つき識別子)
  /// @return 生成された要素
  virtual
  const PtDefParam*
  new_DefParam(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    PuHierName* hname,    ///< [in] 階層名
    const PtExpr* value   ///< [in] 値を表す式
  ) = 0;

  /// @brief continuous assign 文のヘッダの生成 (strength, 遅延付き)
  /// @return 生成された continuous assign 文のヘッダ
  virtual
  const PtItem*
  new_ContAssignH(
    const FileRegion& fr,                         ///< [in] ファイル位置の情報
    const PtStrength* strength,                   ///< [in] 信号強度
    const PtDelay* delay,                         ///< [in] 遅延値
    const vector<const PtContAssign*>& elem_array ///< [in] 要素のリスト
  ) = 0;

  /// @brief continuous assign 文の生成
  /// @return 生成された continuous assign 文
  virtual
  const PtContAssign*
  new_ContAssign(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const PtExpr* lhs,    ///< [in] 左辺式
    const PtExpr* rhs     ///< [in] 右辺式
  ) = 0;

  /// @brief initial 文の生成
  /// @return 生成された initial 文
  virtual
  const PtItem*
  new_Initial(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const PtStmt* body    ///< [in] 本体のステートメント
  ) = 0;

  /// @brief always 文の生成
  /// @return 生成された always 文
  virtual
  const PtItem*
  new_Always(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const PtStmt* body    ///< [in] 本体のステートメント
  ) = 0;

  /// @brief task 文の生成
  /// @return 生成された task
  virtual
  const PtItem*
  new_Task(
    const FileRegion& fr,                        ///< [in] ファイル位置の情報
    const char* name,                            ///< [in] task 名
    bool automatic,                              ///< [in] automatic task の時に true となるフラグ
    const vector<const PtIOHead*>& iodecl_array, ///< [in] IO宣言のリスト
    const vector<const PtDeclHead*>& decl_array, ///< [in] 宣言のリスト
    const PtStmt* stmt                           ///< [in] 本体のステートメント
  ) = 0;

  /// @brief 1ビット型 function 文の生成
  /// @return 生成された function
  virtual
  const PtItem*
  new_Function(
    const FileRegion& fr,                        ///< [in] ファイル位置の情報
    const char* name,				 ///< [in] task 名
    bool automatic,				 ///< [in] automatic task の時に true となるフラグ
    bool sign,					 ///< [in] signed 属性がついていたら true となるフラグ
    const vector<const PtIOHead*>& iodecl_array, ///< [in] IO宣言のリスト
    const vector<const PtDeclHead*>& decl_array, ///< [in] 宣言のリスト
    const PtStmt* stmt				 ///< [in] 本体のステートメント
  ) = 0;

  /// @brief 範囲指定型 function 文の生成
  /// @return 生成された function
  virtual
  const PtItem*
  new_SizedFunc(
    const FileRegion& fr,                        ///< [in] ファイル位置の情報
    const char* name,				 ///< [in] task 名
    bool automatic,				 ///< [in] automatic task の時に true となるフラグ
    bool sign,					 ///< [in] signed 属性がついていたら true となるフラグ
    const PtExpr* left,                          ///< [in] 範囲の左側の式
    const PtExpr* right,                         ///< [in] 範囲の右側の式
    const vector<const PtIOHead*>& iodecl_array, ///< [in] IO宣言のリスト
    const vector<const PtDeclHead*>& decl_array, ///< [in] 宣言のリスト
    const PtStmt* stmt				 ///< [in] 本体のステートメント
  ) = 0;

  /// @brief 組み込み型 function 文の生成
  /// @return 生成された task 文
  virtual
  const PtItem*
  new_TypedFunc(
    const FileRegion& fr,                        ///< [in] ファイル位置の情報
    const char* name,				 ///< [in] task 名
    bool automatic,				 ///< [in] automatic task の時に true となるフラグ
    bool sign,					 ///< [in] signed 属性がついていたら true となるフラグ
    VpiVarType func_type,                        ///< [in] 関数の戻値の型
    const vector<const PtIOHead*>& iodecl_array, ///< [in] IO宣言のリスト
    const vector<const PtDeclHead*>& decl_array, ///< [in] 宣言のリスト
    const PtStmt* stmt				 ///< [in] 本体のステートメント
  ) = 0;

  /// @brief gate instance 文のヘッダの生成 (strength, 遅延付き)
  /// @return 生成された gate instance 文のヘッダ
  virtual
  const PtItem*
  new_GateH(
    const FileRegion& fr,                   ///< [in] ファイル位置の情報
    VpiPrimType type,			    ///< [in] primitive の型
    const PtStrength* strength,		    ///< [in] 信号強度
    const PtDelay* delay,		    ///< [in] 遅延値
    const vector<const PtInst*>& elem_array ///< [in] 要素の配列
  ) = 0;

  /// @brief module instance/UDP instance 文のヘッダの生成 (strength, 遅延付き)
  /// @return 生成された module instance/UDP instance 文のヘッダ
  virtual
  const PtItem*
  new_MuH(
    const FileRegion& fr,                   ///< [in] ファイル位置の情報
    const char* def_name,		    ///< [in] 定義名
    const PtStrength* strength,		    ///< [in] 信号強度
    const PtDelay* delay,                   ///< [in] 遅延値
    const vector<const PtInst*>& elem_array ///< [in] 要素の配列
  ) = 0;

  /// @brief module instance/UDP instance 文のヘッダの生成
  /// @return 生成された module instance/UDP instance 文のヘッダ
  virtual
  const PtItem*
  new_MuH(
    const FileRegion& fr,                         ///< [in]ファイル位置の情報
    const char* def_name,                         ///< [in] 定義名
    const vector<const PtConnection*>& con_array, ///< [in] ポート割り当てリスト
    const vector<const PtInst*>& elem_array       ///< [in] 要素の配列
  ) = 0;

  /// @brief 名前と範囲付き module instance/UDP/gate instance の要素の生成
  /// @return 生成された module instance/UDP instance の要素
  virtual
  const PtInst*
  new_Inst(
    const FileRegion& fr,                        ///< [in] ファイル位置の情報
    const char* name,				 ///< [in] 名前
    const PtExpr* left,                          ///< [in] 範囲の左側の式
    const PtExpr* right,                         ///< [in] 範囲の右側の式
    const vector<const PtConnection*>& con_array ///< [in] ポート割り当ての配列
  ) = 0;

  /// @brief generate 文の生成
  /// @return 生成された generate 文
  virtual
  const PtItem*
  new_Generate(
    const FileRegion& fr,                        ///< [in] ファイル位置の情報
    const vector<const PtDeclHead*>& decl_array, ///< [in] 宣言のリスト
    const vector<const PtItem*>& item_array      ///< [in] 要素のリスト
  ) = 0;

  /// @brief generate block 文の生成
  /// @return 生成された generate block 文
  virtual
  const PtItem*
  new_GenBlock(
    const FileRegion& fr,                        ///< [in] ファイル位置の情報
    const vector<const PtDeclHead*>& decl_array, ///< [in] 宣言のリスト
    const vector<const PtItem*>& item_array      ///< [in] 要素のリスト
  ) = 0;

  /// @brief 名前付き generate block 文の生成
  /// @return 生成された generate block 文
  virtual
  const PtItem*
  new_GenBlock(
    const FileRegion& fr,                        ///< [in] ファイル位置の情報
    const char* name,                            ///< [in] 名前
    const vector<const PtDeclHead*>& decl_array, ///< [in] 宣言のリスト
    const vector<const PtItem*>& item_array      ///< [in] 要素のリスト
  ) = 0;

  /// @brief generate if 文の生成
  /// @return 生成された generate if 文
  virtual
  const PtItem*
  new_GenIf(
    const FileRegion& fr,                             ///< [in] ファイル位置の情報
    const PtExpr* cond,                               ///< [in] 条件を表す式
    const vector<const PtDeclHead*>& then_decl_array, ///< [in] 条件が成り立った時に生成する宣言のリスト
    const vector<const PtItem*>& then_item_array,     ///< [in] 条件が成り立った時に生成する要素のリスト
    const vector<const PtDeclHead*>& else_decl_array, ///< [in] 条件が成り立たなかった時に生成する宣言のリスト
    const vector<const PtItem*>& else_item_array      ///< [in] 条件が成り立たなかった時に生成する要素のリスト
  ) = 0;

  /// @brief generate case 文の生成
  /// @return 生成された generate case 文
  virtual
  const PtItem*
  new_GenCase(
    const FileRegion& fr,                          ///< [in] ファイル位置の情報
    const PtExpr* expr,                            ///< [in] 選択式
    const vector<const PtGenCaseItem*>& item_array ///< [in] generate case item のリスト
  ) = 0;

  /// @brief generate case の要素の生成
  /// @return 生成された generate case item
  virtual
  const PtGenCaseItem*
  new_GenCaseItem(
    const FileRegion& fr,                        ///< [in] ファイル位置の情報
    const vector<const PtExpr*>& label_array,    ///< [in] 比較式のリスト
    const vector<const PtDeclHead*>& decl_array, ///< [in] 宣言のリスト
    const vector<const PtItem*>& item_array      ///< [in] 要素のリスト
  ) = 0;

  /// @brief generate for 文の生成
  /// @return 生成された generate for 文
  virtual
  const PtItem*
  new_GenFor(
    const FileRegion& fr,                        ///< [in] ファイル位置の情報
    const char* loop_var,                        ///< [in] ループ変数
    const PtExpr* init_expr,                     ///< [in] 初期化式
    const PtExpr* cond,                          ///< [in] ループ条件式
    const PtExpr* next_expr,                     ///< [in] 増加式
    const char* block_name,                      ///< [in] ブロック名
    const vector<const PtDeclHead*>& decl_array, ///< [in] 宣言のリスト
    const vector<const PtItem*>& item_array      ///< [in] 要素のリスト
  ) = 0;

  /// @brief specify block item の生成
  /// @return 生成された specify block item
  virtual
  const PtItem*
  new_SpecItem(
    const FileRegion& fr,                       ///< [in] ファイル位置の情報
    VpiSpecItemType id,                         ///< [in] specify block item の種類
    const vector<const PtExpr*>& terminal_array ///< [in] 端子のリスト
  ) = 0;

  /// @brief path 仕様を生成する．
  /// @return 生成された spec path
  virtual
  const PtItem*
  new_SpecPath(
    const FileRegion& fr,       ///< [in] ファイル位置の情報
    VpiSpecPathType id,         ///< [in] spec path の種類
    const PtExpr* expr,         ///< [in] 条件式
    const PtPathDecl* path_decl ///< [in] パス記述
  ) = 0;

  /// @brief パス記述の生成
  /// @return 生成されたパス記述
  virtual
  const PtPathDecl*
  new_PathDecl(
    const FileRegion& fr,                      ///< [in] ファイル位置の情報
    int edge,
    const vector<const PtExpr*>& input_array,
    int input_pol,
    VpiPathType op,
    const vector<const PtExpr*>& output_array,
    int output_pol,
    const PtExpr* expr,
    const PtPathDelay* path_delay
  ) = 0;

  /// @brief path delay value の生成 (値が1個)
  /// @return 生成された path delay value
  virtual
  const PtPathDelay*
  new_PathDelay(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const PtExpr* value   ///< [in] 値
  ) = 0;

  /// @brief path delay value の生成 (値が2個)
  /// @return 生成された path delay value
  virtual
  const PtPathDelay*
  new_PathDelay(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const PtExpr* value1, ///< [in] 値1
    const PtExpr* value2  ///< [in] 値2
  ) = 0;

  /// @brief path delay value の生成 (値が3個)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] value1 値1
  /// @param[in] value2 値2
  /// @param[in] value3 値3
  /// @return 生成された path delay value
  virtual
  const PtPathDelay*
  new_PathDelay(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const PtExpr* value1, ///< [in] 値1
    const PtExpr* value2, ///< [in] 値2
    const PtExpr* value3  ///< [in] 値3
  ) = 0;

  /// @brief path delay value の生成 (値が6個)
  /// @return 生成された path delay value
  virtual
  const PtPathDelay*
  new_PathDelay(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const PtExpr* value1, ///< [in] 値1
    const PtExpr* value2, ///< [in] 値2
    const PtExpr* value3, ///< [in] 値3
    const PtExpr* value4, ///< [in] 値4
    const PtExpr* value5, ///< [in] 値5
    const PtExpr* value6  ///< [in] 値6
  ) = 0;

  /// @brief path delay value の生成 (値が12個)
  /// @return 生成された path delay value
  virtual
  const PtPathDelay*
  new_PathDelay(
    const FileRegion& fr,  ///< [in] ファイル位置の情報
    const PtExpr* value1,  ///< [in] 値1
    const PtExpr* value2,  ///< [in] 値2
    const PtExpr* value3,  ///< [in] 値3
    const PtExpr* value4,  ///< [in] 値4
    const PtExpr* value5,  ///< [in] 値5
    const PtExpr* value6,  ///< [in] 値6
    const PtExpr* value7,  ///< [in] 値7
    const PtExpr* value8,  ///< [in] 値8
    const PtExpr* value9,  ///< [in] 値9
    const PtExpr* value10, ///< [in] 値10
    const PtExpr* value11, ///< [in] 値11
    const PtExpr* value12  ///< [in] 値12
  ) = 0;


  //////////////////////////////////////////////////////////////////////
  // statement 関係
  //////////////////////////////////////////////////////////////////////

  /// @brief disable 文の生成
  /// @return 生成された disable 文
  virtual
  const PtStmt*
  new_Disable(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* name      ///< [in] 停止するスコープ名
  ) = 0;

  /// @brief disable 文の生成 (階層付き識別子)
  /// @return 生成された disable 文
  virtual
  const PtStmt*
  new_Disable(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    PuHierName* hname     ///< [in] 階層名
  ) = 0;

  /// @brief enable 文の生成
  /// @return 生成された enable 文
  virtual
  const PtStmt*
  new_Enable(
    const FileRegion& fr,                  ///< [in] ファイル位置の情報
    const char* name,                      ///< [in] 起動するタスク名
    const vector<const PtExpr*>& arg_array ///< [in] 引数のリスト
  ) = 0;

  /// @brief enable 文の生成 (階層付き識別子)
  /// @return 生成された enable 文
  virtual
  const PtStmt*
  new_Enable(
    const FileRegion& fr,                  ///< [in] ファイル位置の情報
    PuHierName* hname,			   ///< [in] 起動するタスクの階層付き名
    const vector<const PtExpr*>& arg_array ///< [in] 引数のリスト
  ) = 0;

  /// @brief system task enable 文の生成
  /// @return 生成された system task enable 文
  virtual
  const PtStmt*
  new_SysEnable(
    const FileRegion& fr,                  ///< [in] ファイル位置の情報
    const char* name,			   ///< [in] 起動するシステムタスク名
    const vector<const PtExpr*>& arg_array ///< [in] 引数のリスト
  ) = 0;

  /// @brief delay control 文の生成
  /// @return 生成された delay control 文
  virtual
  const PtStmt*
  new_DcStmt(
    const FileRegion& fr,   ///< [in] ファイル位置の情報
    const PtControl* delay, ///< [in] delay control (delay control 文とは別物)
    const PtStmt* body      ///< [in] 本体のステートメント
  ) = 0;

  /// @brief event control 文の生成
  /// @return 生成された event control 文
  virtual
  const PtStmt*
  new_EcStmt(
    const FileRegion& fr,   ///< [in] ファイル位置の情報
    const PtControl* event, ///< [in] event control (event control 文とは別物)
    const PtStmt* body      ///< [in] 本体のステートメント
  ) = 0;

  /// @brief wait 文の生成
  /// @return 生成された wait 文
  virtual
  const PtStmt*
  new_Wait(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const PtExpr* cond,   ///< [in] 条件式
    const PtStmt* body    ///< [in] 本体のステートメント
  ) = 0;

  /// @brief assign 文の生成
  /// @return 生成された assign 文
  virtual
  const PtStmt*
  new_Assign(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const PtExpr* lhs,    ///< [in] 左辺式
    const PtExpr* rhs     ///< [in] 右辺式
  ) = 0;

  /// @brief control 付き assign 文の生成
  /// @return 生成された assign 文
  virtual
  const PtStmt*
  new_Assign(
    const FileRegion& fr,    ///< [in] ファイル位置の情報
    const PtExpr* lhs,       ///< [in] 左辺式
    const PtExpr* rhs,       ///< [in] 右辺式
    const PtControl* control ///< [in] 制御式
  ) = 0;

  /// @brief nonblocking assign 文の生成
  /// @return 生成された nonblocking assign 文
  virtual
  const PtStmt*
  new_NbAssign(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const PtExpr* lhs,	  ///< [in] 左辺式
    const PtExpr* rhs	  ///< [in] 右辺式
  ) = 0;

  /// @brief control 付き nonblocking assign 文の生成
  /// @return 生成された nonblocking assign 文
  virtual
  const PtStmt*
  new_NbAssign(
    const FileRegion& fr,    ///< [in] ファイル位置の情報
    const PtExpr* lhs,	     ///< [in] 左辺式
    const PtExpr* rhs,	     ///< [in] 右辺式
    const PtControl* control ///< [in] 制御式
  ) = 0;

  /// @brief event 文の生成
  /// @return 生成された event 文
  virtual
  const PtStmt*
  new_EventStmt(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const PtExpr* event   ///< [in] イベント
  ) = 0;

  /// @brief null 文の生成
  /// @return 生成された null 文
  virtual
  const PtStmt*
  new_NullStmt(
    const FileRegion& fr ///< [in] ファイル位置の情報
  ) = 0;

  /// @brief if 文の生成
  /// @return 生成された if 文
  virtual
  const PtStmt*
  new_If(
    const FileRegion& fr,   ///< [in] ファイル位置の情報
    const PtExpr* expr,     ///< [in] 条件式
    const PtStmt* then_body ///< [in] 成り立ったときに実行されるステートメント
  ) = 0;

  /// @brief if 文の生成
  /// @return 生成された if 文
  virtual
  const PtStmt*
  new_If(
    const FileRegion& fr,    ///< [in] ファイル位置の情報
    const PtExpr* expr,	     ///< [in] 条件式
    const PtStmt* then_body, ///< [in] 成り立ったときに実行されるステートメント
    const PtStmt* else_body  ///< [in] 成り立たなかったときに実行されるステートメント
  ) = 0;

  /// @brief case 文の生成
  /// @return 生成された case 文
  virtual
  const PtStmt*
  new_Case(
    const FileRegion& fr,                           ///< [in] ファイル位置の情報
    const PtExpr* expr,                             ///< [in] 条件式
    const vector<const PtCaseItem*>& caseitem_array ///< [in] case item のリスト
  ) = 0;

  /// @brief casex 文の生成
  /// @return 生成された case 文
  virtual
  const PtStmt*
  new_CaseX(
    const FileRegion& fr,                           ///< [in] ファイル位置の情報
    const PtExpr* expr,				    ///< [in] 条件式
    const vector<const PtCaseItem*>& caseitem_array ///< [in] case item のリスト
  ) = 0;

  /// @brief casez 文の生成
  /// @return 生成された case 文
  virtual
  const PtStmt*
  new_CaseZ(
    const FileRegion& fr,                           ///< [in] ファイル位置の情報
    const PtExpr* expr,				    ///< [in] 条件式
    const vector<const PtCaseItem*>& caseitem_array ///< [in] case item のリスト
  ) = 0;

  /// @brief case item の生成
  /// @return 生成された case item
  virtual
  const PtCaseItem*
  new_CaseItem(
    const FileRegion& fr,                     ///< [in] ファイル位置の情報
    const vector<const PtExpr*>& label_array, ///< [in] ラベルのリスト
    const PtStmt* body                        ///< [in] 本体のステートメント
  ) = 0;

  /// @brief forever 文の生成
  /// @return 生成された forever 文
  virtual
  const PtStmt*
  new_Forever(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const PtStmt* body    ///< [in] 本体のステートメント
  ) = 0;

  /// @brief repeat 文の生成
  /// @return 生成された repeat 文
  virtual
  const PtStmt*
  new_Repeat(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const PtExpr* expr,   ///< [in] 繰り返し数を表す式
    const PtStmt* body    ///< [in] 本体のステートメント
  ) = 0;

  /// @brief while 文の生成
  /// @return 生成された while 文
  virtual
  const PtStmt*
  new_While(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const PtExpr* cond,   ///< [in] 繰り返し条件式
    const PtStmt* body    ///< [in] 本体のステートメント
  ) = 0;

  /// @brief for 文の生成
  /// @return 生成された for 文
  virtual
  const PtStmt*
  new_For(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const PtStmt* init,   ///< [in] 初期化文
    const PtExpr* cond,   ///< [in] 繰り返し条件式
    const PtStmt* next,   ///< [in] 増加文
    const PtStmt* body    ///< [in] 本体のステートメント
  ) = 0;

  /// @brief procedural assign 文の生成
  /// @return 生成された procedural assign 文
  virtual
  const PtStmt*
  new_PcAssign(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const PtExpr* lhs,    ///< [in] 左辺式
    const PtExpr* rhs     ///< [in] 右辺式
  ) = 0;

  /// @brief deassign 文の生成
  /// @return 生成された deassign 文
  virtual
  const PtStmt*
  new_Deassign(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const PtExpr* lhs     ///< [in] 左辺式
  ) = 0;

  /// @brief force 文の生成
  /// @return 生成された force 文
  virtual
  const PtStmt*
  new_Force(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const PtExpr* lhs,    ///< [in] 左辺式
    const PtExpr* rhs     ///< [in] 右辺式
  ) = 0;

  /// @brief release 文の生成
  /// @return 生成された release 文
  virtual
  const PtStmt*
  new_Release(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const PtExpr* lhs     ///< [in] 左辺式
  ) = 0;

  /// @brief parallel block の生成
  /// @return 生成された parallel block
  virtual
  const PtStmt*
  new_ParBlock(
    const FileRegion& fr,                 ///< [in] ファイル位置の情報
    const vector<const PtStmt*>& stmt_lit ///< [in] 本体のステートメントのリスト
  ) = 0;

  /// @brief 名前付き parallel block の生成
  /// @return 生成された parallel block
  virtual
  const PtStmt*
  new_NamedParBlock(
    const FileRegion& fr,                        ///< [in] ファイル位置の情報
    const char* name,                            ///< [in] ブロック名
    const vector<const PtDeclHead*>& decl_array, ///< [in] 宣言のリスト
    const vector<const PtStmt*>& stmt_lit        ///< [in] 本体のステートメントのリスト
  ) = 0;

  /// @brief sequential block の生成
  /// @return 生成された sequential block
  virtual
  const PtStmt*
  new_SeqBlock(
    const FileRegion& fr,                 ///< [in] ファイル位置の情報
    const vector<const PtStmt*>& stmt_lit ///< [in] 本体のステートメントのリスト
  ) = 0;

  /// @brief 名前付き sequential block の生成
  /// @return 生成された sequential block
  virtual
  const PtStmt*
  new_NamedSeqBlock(
    const FileRegion& fr,                        ///< [in] ファイル位置の情報
    const char* name,                            ///< [in] ブロック名
    const vector<const PtDeclHead*>& decl_array, ///< [in] 宣言のリスト
    const vector<const PtStmt*>& stmt_lit        ///< [in] 本体のステートメントのリスト
  ) = 0;


  //////////////////////////////////////////////////////////////////////
  // expression 関係
  //////////////////////////////////////////////////////////////////////

  /// @brief 単項演算子の生成
  /// @return 生成された演算子
  virtual
  const PtExpr*
  new_Opr(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    VpiOpType type,       ///< [in] 演算の種類
    const PtExpr* opr     ///< [in] オペランド
  ) = 0;

  /// @brief 二項演算子の生成
  /// @return 生成された演算子
  virtual
  const PtExpr*
  new_Opr(
    const FileRegion& fr, ///< [in]  ファイル位置の情報
    VpiOpType type,	  ///< [in]  演算の種類
    const PtExpr* opr1,	  ///< [in]  オペランド1
    const PtExpr* opr2	  ///< [in]  オペランド2
  ) = 0;

  /// @brief 三項演算子の生成
  /// @return 生成された演算子
  virtual
  const PtExpr*
  new_Opr(
    const FileRegion& fr, ///< [in]  ファイル位置の情報
    VpiOpType type,	  ///< [in]  演算の種類
    const PtExpr* opr1,	  ///< [in]  オペランド1
    const PtExpr* opr2,	  ///< [in]  オペランド2
    const PtExpr* opr3 	  ///< [in]  オペランド3
  ) = 0;

  /// @brief concatination 演算子の生成
  /// @return 生成された concatination 演算子
  virtual
  const PtExpr*
  new_Concat(
    const FileRegion& fr,                   ///< [in] ファイル位置の情報
    const vector<const PtExpr*>& expr_array ///< [in] オペランドのリスト
  ) = 0;

  /// @brief multi-concatination 演算子の生成
  /// @return 生成された multi-concatination 演算子
  virtual
  const PtExpr*
  new_MultiConcat(
    const FileRegion& fr,                   ///< [in] ファイル位置の情報
    const vector<const PtExpr*>& expr_array ///< [in] 結合するオペランドのリスト
  ) = 0;

  /// @brief min/typ/max delay 演算子の生成
  /// @return 生成された min/typ/max 演算子
  virtual
  const PtExpr*
  new_MinTypMax(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const PtExpr* val0,   ///< [in] minimum 値
    const PtExpr* val1,   ///< [in] typical 値
    const PtExpr* val2    ///< [in] maximum 値
  ) = 0;

  /// @brief primary の生成
  /// @return 生成された primary
  virtual
  const PtExpr*
  new_Primary(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* name      ///< [in] 識別子名
  ) = 0;

  /// @brief インデックス付き primary の生成
  /// @return 生成された primary
  virtual
  const PtExpr*
  new_Primary(
    const FileRegion& fr,                    ///< [in] ファイル位置の情報
    const char* name,                        ///< [in] 識別子名
    const vector<const PtExpr*>& index_array ///< [in] インデックスのリスト
  ) = 0;

  /// @brief 範囲指定付き primary の生成
  /// @return 生成された primary
  virtual
  const PtExpr*
  new_Primary(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* name,     ///< [in] 識別子名
    VpiRangeMode mode,    ///< [in] 範囲指定のモード
    const PtExpr* left,   ///< [in] 範囲の左側の式
    const PtExpr* right   ///< [in] 範囲の右側の式
  ) = 0;

  /// @brief インデックスと範囲指定付き primary の生成
  /// @return 生成された primary
  virtual
  const PtExpr*
  new_Primary(
    const FileRegion& fr,                     ///< [in] ファイル位置の情報
    const char* name,                         ///< [in] 識別子名
    const vector<const PtExpr*>& index_array, ///< [in] インデックスのリスト
    VpiRangeMode mode,                        ///< [in] 範囲指定のモード
    const PtExpr* left,                       ///< [in] 範囲の左側の式
    const PtExpr* right                       ///< [in] 範囲の右側の式
  ) = 0;

  /// @brief primary の生成 (階層付き)
  /// @return 生成された primary
  virtual
  const PtExpr*
  new_Primary(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    PuHierName* hname     ///< [in] 階層付き識別子
  ) = 0;

  /// @brief インデックス付き primary の生成 (階層付き)
  /// @return 生成された primary
  virtual
  const PtExpr*
  new_Primary(
    const FileRegion& fr,                    ///< [in] ファイル位置の情報
    PuHierName* hname,			     ///< [in] 階層付き識別子
    const vector<const PtExpr*>& index_array ///< [in] インデックスのリスト
  ) = 0;

  /// @brief 範囲指定付き primary の生成 (階層付き)
  /// @return 生成された primary
  virtual
  const PtExpr*
  new_Primary(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    PuHierName* hname,	  ///< [in] 階層付き識別子
    VpiRangeMode mode,    ///< [in] 範囲指定のモード
    const PtExpr* left,   ///< [in] 範囲の左側の式
    const PtExpr* right   ///< [in] 範囲の右側の式
  ) = 0;

  /// @brief インデックスと範囲指定付き primary の生成 (階層付き)
  /// @return 生成された primary
  virtual
  const PtExpr*
  new_Primary(
    const FileRegion& fr,                     ///< [in] ファイル位置の情報
    PuHierName* hname,			      ///< [in] 階層付き識別子
    const vector<const PtExpr*>& index_array, ///< [in] インデックスのリスト
    VpiRangeMode mode,                        ///< [in] 範囲指定のモード
    const PtExpr* left,			      ///< [in] 範囲の左側の式
    const PtExpr* right			      ///< [in] 範囲の右側の式
  ) = 0;

  /// @brief constant primary の生成
  /// @return 生成された const primary
  virtual
  const PtExpr*
  new_CPrimary(
    const FileRegion& fr,                    ///< [in] ファイル位置の情報
    const char* name,                        ///< [in] 識別子名
    const vector<const PtExpr*>& index_array ///< [in] インデックスのリスト
  ) = 0;

  /// @brief 範囲指定付き constant primary の生成
  /// @return 生成された constant primary
  virtual
  const PtExpr*
  new_CPrimary(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* name,	  ///< [in] 識別子名
    VpiRangeMode mode,	  ///< [in] 範囲指定のモード
    const PtExpr* left,	  ///< [in] 範囲の左側の式
    const PtExpr* right	  ///< [in] 範囲の右側の式
  ) = 0;

  /// @brief インデックス付き constant primary の生成 (階層付き)
  /// @return 生成された constant primary
  virtual
  const PtExpr*
  new_CPrimary(
    const FileRegion& fr,                    ///< [in] ファイル位置の情報
    PuHierName* hname,			     ///< [in] 階層付き識別子
    const vector<const PtExpr*>& index_array ///< [in] インデックスのリスト
  ) = 0;

  /// @brief function call の生成
  /// @return 生成された function call
  virtual
  const PtExpr*
  new_FuncCall(
    const FileRegion& fr,                  ///< [in] ファイル位置の情報
    const char* name,                      ///< [in] 関数名
    const vector<const PtExpr*>& arg_array ///< [in] 引数のリスト
  ) = 0;

  /// @brief function call の生成 (階層付き)
  /// @return 生成された function call
  virtual
  const PtExpr*
  new_FuncCall(
    const FileRegion& fr,                  ///< [in] ファイル位置の情報
    PuHierName* hname,                     ///< [in] 階層付き関数名
    const vector<const PtExpr*>& arg_array ///< [in] 引数のリスト
  ) = 0;

  /// @brief system function call の生成
  /// @return 生成された function call
  virtual
  const PtExpr*
  new_SysFuncCall(
    const FileRegion& fr,                  ///< [in] ファイル位置の情報
    const char* name,                      ///< [in] 関数名
    const vector<const PtExpr*>& arg_array ///< [in] 引数のリスト
  ) = 0;

  /// @brief 整数型の定数の生成
  /// @return 生成された定数
  virtual
  const PtExpr*
  new_IntConst(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    std::uint32_t value        ///< [in] 定数を表す整数値
  ) = 0;

  /// @brief 整数型の定数の生成
  /// @return 生成された定数
  virtual
  const PtExpr*
  new_IntConst(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* value     ///< [in] 定数の文字列表現
  ) = 0;

  /// @brief 基底付き整数型の定数の生成
  /// @return 生成された定数
  virtual
  const PtExpr*
  new_IntConst(
    const FileRegion& fr,    ///< [in] ファイル位置の情報
    VpiConstType const_type, ///< [in] 定数の種類
    const char* value        ///< [in] 定数の文字列表現
  ) = 0;

  /// @brief サイズと基底付き定数の生成
  /// @return 生成された定数
  virtual
  const PtExpr*
  new_IntConst(
    const FileRegion& fr,    ///< [in] ファイル位置の情報
    SizeType size,           ///< [in] サイズ
    VpiConstType const_type, ///< [in] 定数の種類
    const char* value        ///< [in] 定数の文字列表現
  ) = 0;

  /// @brief 実数型の定数の生成
  /// @return 生成された定数
  virtual
  const PtExpr*
  new_RealConst(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    double value          ///< [in] 定数の文字列表現
  ) = 0;

  /// @brief 文字列型の定数の生成
  /// @return 生成された定数
  virtual
  const PtExpr*
  new_StringConst(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* value     ///< [in] 文字列
  ) = 0;


  //////////////////////////////////////////////////////////////////////
  // その他
  //////////////////////////////////////////////////////////////////////

  /// @brief ディレイコントロールの生成
  /// @return 生成されたディレイコントロール
  virtual
  const PtControl*
  new_DelayControl(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const PtExpr* value   ///< [in] 遅延を表す式
  ) = 0;

  /// @brief イベントコントロールの生成
  /// @return 生成されたイベントコントロール
  virtual
  const PtControl*
  new_EventControl(
    const FileRegion& fr,                    ///< [in] ファイル位置の情報
    const vector<const PtExpr*>& event_array ///< [in] イベントのリスト
  ) = 0;

  /// @brief リピートコントロールの生成
  /// @return 生成されたリピートコントロール
  virtual
  const PtControl*
  new_RepeatControl(
    const FileRegion& fr,                    ///< [in] ファイル位置の情報
    const PtExpr* expr,                      ///< [in] 繰り返し数を表す式
    const vector<const PtExpr*>& event_array ///< [in] 繰り返しの単位となるイベントのリスト
  ) = 0;

  /// @brief 順序つき結合子の生成
  /// @return 生成された結合子
  virtual
  const PtConnection*
  new_OrderedCon(
    const PtExpr* expr ///< [in] 結合する式
  ) = 0;

  /// @brief 順序つき結合子の生成
  /// @return 生成された結合子
  virtual
  const PtConnection*
  new_OrderedCon(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const PtExpr* expr    ///< [in] 結合する式
  ) = 0;

  /// @brief 順序つき結合子のベクタの生成
  /// @return 生成された結合子のベクタ
  vector<const PtConnection*>
  new_OrderedConVector(
    const PtExpr* expr ///< [in] 結合する式
  );

  /// @brief 順序つき結合子のベクタの生成
  /// @return 生成された結合子のベクタ
  vector<const PtConnection*>
  new_OrderedConVector(
    const PtExpr* expr1, ///< [in] 結合する式1
    const PtExpr* expr2  ///< [in] 結合する式2
  );

  /// @brief 順序つき結合子のベクタの生成
  /// @return 生成された結合子のベクタ
  vector<const PtConnection*>
  new_OrderedConVector(
    const PtExpr* expr1, ///< [in] 結合する式1
    const PtExpr* expr2, ///< [in] 結合する式2
    const PtExpr* expr3  ///< [in] 結合する式3
  );

  /// @brief 順序つき結合子のベクタの生成
  /// @return 生成された結合子のベクタ
  vector<const PtConnection*>
  new_OrderedConVector(
    const PtExpr* expr1, ///< [in] 結合する式1
    const PtExpr* expr2, ///< [in] 結合する式2
    const PtExpr* expr3, ///< [in] 結合する式3
    const PtExpr* expr4  ///< [in] 結合する式4
  );

  /// @brief 名前付き結合子の生成
  /// @return 生成された結合子
  virtual
  const PtConnection*
  new_NamedCon(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* name,     ///< [in] 名前
    const PtExpr* expr    ///< [in] 結合する式
  ) = 0;

  /// @brief strength の生成
  /// @return 生成された strength
  virtual
  const PtStrength*
  new_Strength(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    VpiStrength value0,   ///< [in] '0' の強度
    VpiStrength value1    ///< [in] '1' の強度
  ) = 0;

  /// @brief charge strength の生成
  /// @return 生成された strength
  virtual
  const PtStrength*
  new_Strength(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    VpiStrength value     ///< [in] 強度
  ) = 0;

  /// @brief 遅延値の生成 (1つの値)
  /// @return 生成された遅延値
  virtual
  const PtDelay*
  new_Delay(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const PtExpr* value1  ///< [in] 値1
  ) = 0;

  /// @brief 遅延値の生成 (2つの値)
  /// @return 生成された遅延値
  virtual
  const PtDelay*
  new_Delay(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const PtExpr* value1, ///< [in] 値1
    const PtExpr* value2  ///< [in] 値2
  ) = 0;

  /// @brief 遅延値の生成 (3つの値)
  /// @return 生成された遅延値
  virtual
  const PtDelay*
  new_Delay(
    const FileRegion& fr,  ///< [in] ファイル位置の情報
    const PtExpr* value1,  ///< [in] 値1
    const PtExpr* value2,  ///< [in] 値2
    const PtExpr* value3   ///< [in] 値3
  ) = 0;

  /// @brief 階層名の生成
  /// @return 生成された階層名
  virtual
  const PtNameBranch*
  new_NameBranch(
    const char* name ///< [in] 名前
  ) = 0;

  /// @brief インデックス付きの階層名の生成
  /// @return 生成された階層名
  virtual
  const PtNameBranch*
  new_NameBranch(
    const char* name, ///< [in] 名前
    int index         ///< [in] インデックス
  ) = 0;

  /// @brief 階層名の生成
  PuHierName*
  new_HierName(
    const PtNameBranch* nb, ///< [in] 階層の上位部分
    const char* name        ///< [in] 階層の最下位部分
  );


  //////////////////////////////////////////////////////////////////////
  // attribute instance 関係
  //////////////////////////////////////////////////////////////////////

  /// @brief attribute instance の生成
  /// @return 生成された attribute instance
  virtual
  const PtAttrInst*
  new_AttrInst(
    const FileRegion& fr,                     ///< [in] ファイル位置の情報
    const vector<const PtAttrSpec*>& as_array ///< [in] attribute spec のリスト
  ) = 0;

  /// @brief attribute spec の生成
  /// @return 生成された attribute spec
  virtual
  const PtAttrSpec*
  new_AttrSpec(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* name,     ///< [in] 名前
    const PtExpr* expr    ///< [in] 値
  ) = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // おもにデバッグ用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 確保したオブジェクトに関する情報を出力する．
  virtual
  void
  dump_profile(
    ostream& s ///< [in] 出力先のストリーム
  ) const = 0;


protected:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // メモリアロケータ
  Alloc& mAlloc;

};

END_NAMESPACE_YM_VERILOG

#endif // PTFACTORY_H
