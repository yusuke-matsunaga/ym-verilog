#ifndef PARSER_H
#define PARSER_H

/// @file Parser.h
/// @brief Parser のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014, 2020, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym/pt/PtP.h"
#include "ym/File.h"
#include "PtiFwd.h"
#include "PtiFactory.h"
#include "PtiDecl.h"
#include "PtrList.h"
#include "PtMgr.h"


BEGIN_NAMESPACE_YM_VERILOG

class VlLineWatcher;
class Lex;

union YYSTYPE;

//////////////////////////////////////////////////////////////////////
/// @class Parser Parser.h "Parser.h"
/// @ingroup Parser
/// @brief Verilog-HDL のパーサークラス
//////////////////////////////////////////////////////////////////////
class Parser
{
public:

  /// @brief コンストラクタ
  Parser(
    PtMgr& ptmgr ///< [in] 読んだ結果のパース木を登録するマネージャ
  );

  /// @brief デストラクタ
  ~Parser();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイルを読み込む．
  /// @retval true 正常に終了した．
  /// @retval false エラーが起こった．
  bool
  read_file(
    const string& filename,                    ///< [in] 読み込むファイル名
    const SearchPathList& searchpath,          ///< [in] サーチパス
    const vector<VlLineWatcher*>& watcher_list ///< [in] 行番号ウオッチャーのリスト
  );


public:
  //////////////////////////////////////////////////////////////////////
  // UDP 関連の生成関数
  //////////////////////////////////////////////////////////////////////

  /// @brief Verilog1995 タイプのUDP を生成する．
  void
  new_Udp1995(
    const FileRegion& file_region,     ///< [in] ファイル上の位置
    const char* name,                  ///< [in] 名前
    const char* init_name,             ///< [in] 初期値の名前
    const FileRegion& init_loc,        ///< [in] 初期値の位置
    const PtExpr* init_value,          ///< [in] 初期値のパース木
    PtrList<const PtAttrInst>* ai_list ///< [in] 属性リスト
  );

  /// @brief Verilog2001 タイプのUDP を生成する．
  void
  new_Udp2001(
    const FileRegion& file_region,     ///< [in] ファイル上の位置
    const char* name,		       ///< [in] 名前
    const char* init_name,	       ///< [in] 初期値の名前
    const FileRegion& init_loc,	       ///< [in] 初期値の位置
    const PtExpr* init_value,	       ///< [in] 初期値のパース木
    PtrList<const PtAttrInst>* ai_list ///< [in] 属性リスト
  );

  /// @brief combinational UDP 用のテーブルエントリの生成
  void
  new_UdpEntry(
    const FileRegion& fr,         ///< [in] ファイル位置の情報
    const FileRegion& output_loc, ///< [in] 出力記号の位置
    char output_symbol            ///< [in] 出力記号
  );

  /// @brief sequential UDP 用のテーブルエントリの生成
  void
  new_UdpEntry(
    const FileRegion& fr,          ///< [in] ファイル位置の情報
    const FileRegion& current_loc, ///< [in] 現状態記号の位置
    char current_symbol,           ///< [in] 現状態記号
    const FileRegion& output_loc,  ///< [in] 出力記号の位置
    char output_symbol             ///< [in] 出力記号
  );

  /// @brief UDP のテーブルエントリの要素の値の生成
  void
  new_UdpValue(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    char symbol           ///< [in] シンボル
  );

  /// @brief UDP のテーブルエントリの要素の値の生成
  void
  new_UdpValue(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    char symbol1,         ///< [in] 1番目のシンボル
    char symbol2          ///< [in] 2番目のシンボル
  );


private:

  /// @brief new_Udp の下請け関数
  void
  new_Udp(
    const FileRegion& file_region,
    const char* udp_name,
    const char* init_name,
    const FileRegion& init_loc,
    const PtExpr* init_value,
    PtrList<const PtAttrInst>* ai_list,
    bool is_seq,
    const PtIOItem* out_item,
    const vector<const PtPort*>& port_array,
    const vector<const PtIOHead*>& iohead_array
  );


public:
  //////////////////////////////////////////////////////////////////////
  // モジュール関連の要素の生成関数
  //////////////////////////////////////////////////////////////////////

  /// @brief Verilog1995 タイプのモジュール(のテンプレート)を生成する．
  void
  new_Module1995(
    const FileRegion& file_region,
    bool is_macro,
    const char* name,
    PtrList<const PtAttrInst>* ai_list
  );

  /// @brief Verilog2001 タイプのモジュール(のテンプレート)を生成する．
  void
  new_Module2001(
    const FileRegion& file_region,
    bool is_macro,
    const char* name,
    PtrList<const PtAttrInst>* ai_list
  );


public:
  //////////////////////////////////////////////////////////////////////
  // ポート関連の要素の生成関数
  //////////////////////////////////////////////////////////////////////

  /// @brief PtiPort の vector からポート配列を作る．
  vector<const PtPort*>
  new_PortArray(
    const vector<PtiPort*>& port_vector
  );

  /// @brief 入出力宣言からポートを作る．
  vector<const PtPort*>
  new_PortArray(
    const vector<const PtIOHead*>& iohead_array
  );

  /// @brief 空のポートの生成
  void
  new_Port();

  /// @brief ポートの生成 (内側の式のみ指定するタイプ)
  void
  new_Port1(
    const FileRegion& file_region ///< [in] file_region ファイル位置
  );

  /// @brief ポートの生成 (外側の名前のみ指定するタイプ)
  void
  new_Port2(
    const FileRegion& file_region, ///< [in] ファイル位置
    const char* name               ///< [in] 外側の名前
  );

  /// @brief ポートの生成 (外側の名前と内側の式を指定するタイプ)
  ///
  /// 内側の式は new_PortRef() で内部に追加されているものとする．
  void
  new_Port3(
    const FileRegion& file_region, ///< [in] ファイル位置
    const char* name               ///< [in] 外側の名前
  );

  /// @brief ポート参照式の生成
  void
  new_PortRef(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* name      ///< [in] ポートに接続している内部の識別子名
  );

  /// @brief ビット指定つきポート参照式の生成
  void
  new_PortRef(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* name,     ///< [in] ポートに接続している内部の識別子名
    const PtExpr* index   ///< [in] ビット指定用の式
  );

  /// @brief 範囲指定付きポート参照式の生成
  void
  new_PortRef(
    const FileRegion& fr,    ///< [in] ファイル位置の情報
    const char* name,        ///< [in] ポートに接続している内部の識別子名
    VpiRangeMode range_mode, ///< [in] 範囲指定のモード
    const PtExpr* left,      ///< [in] 範囲指定の左側の式
    const PtExpr* right      ///< [in] 範囲指摘の右側の式
  );


public:
  //////////////////////////////////////////////////////////////////////
  // IO 宣言関係の要素の生成関数
  //////////////////////////////////////////////////////////////////////

  /// @brief IO 宣言のヘッダの生成
  PtiIOHead*
  new_IOHead(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    VpiDir dir,           ///< [in] IO の種類(方向)
    bool sign             ///< [in] 符号付きのとき true となるフラグ
  );

  /// @brief IO 宣言のヘッダの生成 (reg 型)
  PtiIOHead*
  new_RegIOHead(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    VpiDir dir,           ///< [in] IO の種類(方向)
    bool sign             ///< [in] 符号付きのとき true となるフラグ
  );

  /// @brief IO 宣言のヘッダの生成 (ネット型)
  PtiIOHead*
  new_NetIOHead(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    VpiDir dir,           ///< [in] IO の種類(方向)
    VpiNetType net_type,  ///< [in] 補助的なネット型
    bool sign
  );

  /// @brief IO 宣言のヘッダの生成 (変数型)
  PtiIOHead*
  new_VarIOHead(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    VpiDir dir,           ///< [in] IO の種類(方向)
    VpiVarType var_type   ///< [in] 補助的な変数型
  );

  /// @brief 範囲付きの IO 宣言のヘッダの生成
  PtiIOHead*
  new_IOHead(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    VpiDir dir,           ///< [in] IO の種類(方向)
    bool sign,            ///< [in] 符号付きのとき true となるフラグ
    const PtExpr* left,   ///< [in] 範囲の左側の式
    const PtExpr* right   ///< [in] 範囲の右側の式
  );

  /// @brief 範囲付きの IO 宣言のヘッダの生成 (reg 型)
  PtiIOHead*
  new_RegIOHead(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    VpiDir dir,           ///< [in] IO の種類(方向)
    bool sign,            ///< [in] 符号付きのとき true となるフラグ
    const PtExpr* left,   ///< [in] 範囲の左側の式
    const PtExpr* right   ///< [in] 範囲の右側の式
  );

  /// @brief 範囲付きの IO 宣言のヘッダの生成 (ネット型)
  PtiIOHead*
  new_NetIOHead(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    VpiDir dir,           ///< [in] IO の種類(方向)
    VpiNetType net_type,  ///< [in] 補助的なネット型
    bool sign,            ///< [in] 符号付きのとき true となるフラグ
    const PtExpr* left,   ///< [in] 範囲の左側の式
    const PtExpr* right   ///< [in] 範囲の右側の式
  );

  /// @brief IO 宣言の要素の生成
  void
  new_IOItem(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* name      ///< [in] 要素名
  );


  /// @brief 初期値付き IO 宣言の要素の生成
  void
  new_IOItem(
    const FileRegion& fr,    ///< [in] ファイル位置の情報
    const char* name,        ///< [in] 要素名
    const PtExpr* init_value ///< [in] 初期値を表す式
  );


public:
  //////////////////////////////////////////////////////////////////////
  // 宣言関連の要素の生成
  //////////////////////////////////////////////////////////////////////

  /// @brief パラメータ宣言のヘッダの生成 (型指定なし)
  PtiDeclHead*
  new_ParamH(
    const FileRegion& fr ///< [in] ファイル位置の情報
  );

  /// @brief 範囲指定型パラメータ宣言のヘッダの生成
  PtiDeclHead*
  new_ParamH(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    bool sign,            ///< [in] 符号付きのとき true となるフラグ
    const PtExpr* left,   ///< [in] 範囲の左側の式
    const PtExpr* right   ///< [in] 範囲の右側の式
  );

  /// @brief 組み込み型パラメータ宣言のヘッダの生成
  PtiDeclHead*
  new_ParamH(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    VpiVarType var_type   ///< [in] データ型
  );

  /// @brief local param 宣言のヘッダの生成 (型指定なし)
  PtiDeclHead*
  new_LocalParamH(
    const FileRegion& fr ///< [in] ファイル位置の情報
  );

  /// @brief 範囲指定型 local param 宣言のヘッダの生成
  PtiDeclHead*
  new_LocalParamH(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    bool sign,            ///< [in] 符号付きのとき true となるフラグ
    const PtExpr* left,   ///< [in] 範囲の左側の式
    const PtExpr* right   ///< [in] 範囲の右側の式
  );

  /// @brief 組み込み型パラメータ宣言のヘッダの生成
  PtiDeclHead*
  new_LocalParamH(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    VpiVarType var_type   ///< [in] データ型
  );

  /// @brief specparam 宣言のヘッダの生成
  /// @return 生成された specparam
  PtiDeclHead*
  new_SpecParamH(
    const FileRegion& fr ///< [in] ファイル位置の情報
  );

  /// @brief 範囲指定型 specparam 宣言のヘッダの生成
  PtiDeclHead*
  new_SpecParamH(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const PtExpr* left,   ///< [in] 範囲の左側の式
    const PtExpr* right   ///< [in] 範囲の右側の式
  );

  /// @brief イベント宣言のヘッダの生成
  /// @return 生成されたイベント
  PtiDeclHead*
  new_EventH(
    const FileRegion& fr ///< [in] ファイル位置の情報
  );

  /// @brief genvar 宣言のヘッダの生成
  PtiDeclHead*
  new_GenvarH(
    const FileRegion& fr ///< [in] ファイル位置の情報
  );

  /// @brief 変数宣言のヘッダの生成
  PtiDeclHead*
  new_VarH(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    VpiVarType var_type   ///< [in] データ型
  );

  /// @brief 1ビット型 reg 宣言のヘッダの生成
  PtiDeclHead*
  new_RegH(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    bool sign             ///< [in] 符号付きの時 true となるフラグ
  );

  /// @brief 範囲指定型 reg 宣言のヘッダの生成
  PtiDeclHead*
  new_RegH(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    bool sign,            ///< [in] 符号付きの時 true となるフラグ
    const PtExpr* left,   ///< [in] 範囲の左側の式
    const PtExpr* right   ///< [in] 範囲の右側の式
  );

  /// @brief 1ビット型 net 宣言のヘッダの生成
  PtiDeclHead*
  new_NetH(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    VpiNetType type,      ///< [in] net の型
    bool sign             ///< [in] 符号の有無を表すフラグ
  );

  /// @brief 1ビット型 net 宣言のヘッダの生成 (strength あり)
  PtiDeclHead*
  new_NetH(
    const FileRegion& fr,      ///< [in] ファイル位置の情報
    VpiNetType type,           ///< [in] net の型
    bool sign,                 ///< [in] 符号の有無を表すフラグ
    const PtStrength* strength ///< [in] 信号強度
  );

  /// @brief 1ビット型 net 宣言のヘッダの生成 (遅延あり)
  PtiDeclHead*
  new_NetH(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    VpiNetType type,      ///< [in] net の型
    bool sign,            ///< [in] 符号の有無を表すフラグ
    const PtDelay* delay  ///< [in] 遅延
  );

  /// @brief 1ビット型 net 宣言のヘッダの生成 (strength, 遅延あり)
  PtiDeclHead*
  new_NetH(
    const FileRegion& fr,       ///< [in] ファイル位置の情報
    VpiNetType type,            ///< [in] net の型
    bool sign,                  ///< [in] 符号の有無を表すフラグ
    const PtStrength* strength, ///< [in] 信号強度
    const PtDelay* delay        ///< [in] 遅延
  );

  /// @brief 範囲指定型 net 宣言のヘッダの生成
  PtiDeclHead*
  new_NetH(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    VpiNetType type,      ///< [in] net の型
    VpiVsType vstype,     ///< [in] vector/scalar 指定
    bool sign,            ///< [in] 符号の有無を表すフラグ
    const PtExpr* left,   ///< [in] 範囲の左側の式
    const PtExpr* right   ///< [in] 範囲の右側の式
  );

  /// @brief 範囲指定型 net 宣言のヘッダの生成 (strengthあり)
  PtiDeclHead*
  new_NetH(
    const FileRegion& fr,       ///< [in] ファイル位置の情報
    VpiNetType type,            ///< [in] net の型
    VpiVsType vstype,           ///< [in] vector/scalar 指定
    bool sign,                  ///< [in] 符号の有無を表すフラグ
    const PtExpr* left,         ///< [in] 範囲の左側の式
    const PtExpr* right,        ///< [in] 範囲の右側の式
    const PtStrength* strength  ///< [in] 信号強度
  );

  /// @brief 範囲指定型 net 宣言のヘッダの生成 (遅延あり)
  PtiDeclHead*
  new_NetH(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    VpiNetType type,	  ///< [in] net の型
    VpiVsType vstype,	  ///< [in] vector/scalar 指定
    bool sign,		  ///< [in] 符号の有無を表すフラグ
    const PtExpr* left,	  ///< [in] 範囲の左側の式
    const PtExpr* right,  ///< [in] 範囲の右側の式
    const PtDelay* delay  ///< [in] 遅延
  );

  /// @brief 範囲指定型 net 宣言のヘッダの生成 (strength, 遅延あり)
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
  );

  /// @brief 宣言要素の生成
  void
  new_DeclItem(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* name      ///< [in] 名前
  );

  /// @brief 初期値付き宣言要素の生成
  void
  new_DeclItem(
    const FileRegion& fr,    ///< [in] ファイル位置の情報
    const char* name,        ///< [in] 名前
    const PtExpr* init_value ///< [in] 初期値を表す式
  );

  /// @brief 配列型宣言要素の生成
  void
  new_DeclItem(
    const FileRegion& fr,              ///< [in] ファイル位置の情報
    const char* name,                  ///< [in] 名前
    PtrList<const PtRange>* range_list ///< [in] 配列の各次元の範囲のリスト
  );

  /// @brief 範囲の生成
  const PtRange*
  new_Range(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const PtExpr* msb,    ///< [in] MSB を表す式
    const PtExpr* lsb     ///< [in] LSB を表す式
  );


public:
  //////////////////////////////////////////////////////////////////////
  // item 関連の要素の生成
  //////////////////////////////////////////////////////////////////////

  /// @brief defparam 文のヘッダの生成
  const PtItem*
  new_DefParamH(
    const FileRegion& fr ///< [in] ファイル位置の情報
  );

  /// @brief defparam 文の要素の生成
  void
  new_DefParam(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* name,     ///< [in] 名前
    const PtExpr* value   ///< [in] 値を表す式
  );

  /// @brief defparam 文の要素の生成 (階層つき識別子)
  void
  new_DefParam(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    PuHierName* hname,    ///< [in] 階層名
    const PtExpr* value   ///< [in] 値を表す式
  );

  /// @brief continuous assign 文のヘッダの生成
  const PtItem*
  new_ContAssignH(
    const FileRegion& fr ///< [in] ファイル位置の情報
  );

  /// @brief continuous assign 文のヘッダの生成 (strengthつき)
  const PtItem*
  new_ContAssignH(
    const FileRegion& fr,      ///< [in] ファイル位置の情報
    const PtStrength* strength ///< [in] 信号強度
  );

  /// @brief continuous assign 文のヘッダの生成 (遅延付き)
  const PtItem*
  new_ContAssignH(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const PtDelay* delay  ///< [in] 遅延値
  );

  /// @brief continuous assign 文のヘッダの生成 (strength, 遅延付き)
  const PtItem*
  new_ContAssignH(
    const FileRegion& fr,       ///< [in] ファイル位置の情報
    const PtStrength* strength, ///< [in] 信号強度
    const PtDelay* delay        ///< [in] 遅延値
  );

  /// @brief continuous assign 文の生成
  void
  new_ContAssign(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const PtExpr* lhs,    ///< [in] 左辺式
    const PtExpr* rhs     ///< [in] 右辺式
  );

  /// @brief initial 文の生成
  const PtItem*
  new_Initial(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const PtStmt* body    ///< [in] 本体のステートメント
  );

  /// @brief always 文の生成
  const PtItem*
  new_Always(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const PtStmt* body    ///< [in] 本体のステートメント
  );

  /// @brief task 文の生成
  const PtItem*
  new_Task(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* name,     ///< [in] task 名
    bool automatic,       ///< [in] automatic task の時に true となるフラグ
    const PtStmt* stmt    ///< [in] 本体のステートメント
  );

  /// @brief 1ビット型 function 文の生成
  const PtItem*
  new_Function(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* name,     ///< [in] function 名
    bool automatic,       ///< [in] automatic task の時に true となるフラグ
    bool sign,            ///< [in] signed 属性がついていたら true となるフラグ
    const PtStmt* stmt    ///< [in] 本体のステートメント
  );

  /// @brief 範囲指定型 function 文の生成
  const PtItem*
  new_SizedFunc(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* name,     ///< [in] function 名
    bool automatic,       ///< [in] automatic task の時に true となるフラグ
    bool sign,		  ///< [in] signed 属性がついていたら true となるフラグ
    const PtExpr* left,	  ///< [in] 範囲の左側の式
    const PtExpr* right,  ///< [in] 範囲の右側の式
    const PtStmt* stmt    ///< [in] 本体のステートメント
  );

  /// @brief 組み込み型 function 文の生成
  const PtItem*
  new_TypedFunc(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* name,	  ///< [in] function 名
    bool automatic,	  ///< [in] automatic task の時に true となるフラグ
    bool sign,		  ///< [in] signed 属性がついていたら true となるフラグ
    VpiVarType func_type, ///< [in] 関数の戻値の型
    const PtStmt* stmt    ///< [in] 本体のステートメント
  );

  /// @brief gate instance 文のヘッダの生成
  const PtItem*
  new_GateH(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    VpiPrimType type      ///< [in] primitive の型
  );

  /// @brief gate instance 文のヘッダの生成 (strength付き)
  const PtItem*
  new_GateH(
    const FileRegion& fr,      ///< [in] ファイル位置の情報
    VpiPrimType type,          ///< [in] primitive の型
    const PtStrength* strength ///< [in] 信号強度
  );

  /// @brief gate instance 文のヘッダの生成 (遅延付き)
  const PtItem*
  new_GateH(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    VpiPrimType type,	  ///< [in] primitive の型
    const PtDelay* delay  ///< [in] 遅延値
  );

  /// @brief gate instance 文のヘッダの生成 (strength, 遅延付き)
  const PtItem*
  new_GateH(
    const FileRegion& fr,       ///< [in] ファイル位置の情報
    VpiPrimType type,	        ///< [in] primitive の型
    const PtStrength* strength, ///< [in] 信号強度
    const PtDelay* delay        ///< [in] 遅延値
  );

  /// @brief module instance/UDP instance 文のヘッダの生成
  const PtItem*
  new_MuH(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* def_name  ///< [in] 定義名
  );

  /// @brief module instance/UDP instance 文のヘッダの生成 (strength付き)
  const PtItem*
  new_MuH(
    const FileRegion& fr,      ///< [in] ファイル位置の情報
    const char* def_name,      ///< [in] 定義名
    const PtStrength* strength ///< [in] 信号強度
  );

  /// @brief module instance/UDP instance 文のヘッダの生成 (遅延付き)
  const PtItem*
  new_MuH(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* def_name, ///< [in] 定義名
    const PtDelay* delay  ///< [in] 遅延値
  );

  /// @brief module instance/UDP instance 文のヘッダの生成 (strength, 遅延付き)
  const PtItem*
  new_MuH(
    const FileRegion& fr,       ///< [in] ファイル位置の情報
    const char* def_name,       ///< [in] 定義名
    const PtStrength* strength, ///< [in] 信号強度
    const PtDelay* delay        ///< [in] 遅延値
  );

  /// @brief module instance/UDP instance 文のヘッダの生成
  const PtItem*
  new_MuH(
    const FileRegion& fr,                 ///< [in] ファイル位置の情報
    const char* def_name,		  ///< [in] 定義名
    PtrList<const PtConnection>* con_list ///< [in] ポート割り当てリスト
  );

  /// @brief module instance/UDP/gate instance の要素の生成
  void
  new_Inst(
    const FileRegion& fr,                 ///< [in] ファイル位置の情報
    PtrList<const PtConnection>* con_list ///< [in] ポート割り当ての配列
  );

  /// @brief module instance/UDP/gate instance の要素の生成
  void
  new_Inst(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const PtExpr* expr1   ///< [in] ポート割り当て
  );

  /// @brief module instance/UDP/gate instance の要素の生成
  /// @param[in] fr
  /// @param[in] expr1, expr2
  void
  new_Inst(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const PtExpr* expr1,  ///< [in] ポート割り当て式1
    const PtExpr* expr2   ///< [in] ポート割り当て式2
  );

  /// @brief module instance/UDP/gate instance の要素の生成
  void
  new_Inst(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const PtExpr* expr1,  ///< [in] ポート割り当て式1
    const PtExpr* expr2,  ///< [in] ポート割り当て式2
    const PtExpr* expr3   ///< [in] ポート割り当て式3
  );

  /// @brief module instance/UDP/gate instance の要素の生成
  void
  new_Inst(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const PtExpr* expr1,  ///< [in] ポート割り当て式1
    const PtExpr* expr2,  ///< [in] ポート割り当て式2
    const PtExpr* expr3,  ///< [in] ポート割り当て式3
    const PtExpr* expr4   ///< [in] ポート割り当て式4
  );

  /// @brief 名前付き module instance/UDP/gate instance の要素の生成
  void
  new_InstN(
    const FileRegion& fr,                 ///< [in] ファイル位置の情報
    const char* name,                     ///< [in] 名前
    PtrList<const PtConnection>* con_list ///< [in] ポート割り当ての配列
  );

  /// @brief 名前付き module instance/UDP/gate instance の要素の生成
  void
  new_InstN(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* name,     ///< [in] 名前
    const PtExpr* expr1   ///< [in] ポート割り当て式1
  );

  /// @brief 名前付き module instance/UDP/gate instance の要素の生成
  void
  new_InstN(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* name,     ///< [in] 名前
    const PtExpr* expr1,  ///< [in] ポート割り当て式1
    const PtExpr* expr2   ///< [in] ポート割り当て式2
  );

  /// @brief 名前付き module instance/UDP/gate instance の要素の生成
  void
  new_InstN(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* name,     ///< [in] 名前
    const PtExpr* expr1,  ///< [in] ポート割り当て式1
    const PtExpr* expr2,  ///< [in] ポート割り当て式2
    const PtExpr* expr3   ///< [in] ポート割り当て式3
  );

  /// @brief 名前付き module instance/UDP/gate instance の要素の生成
  void
  new_InstN(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* name,     ///< [in] 名前
    const PtExpr* expr1,  ///< [in] ポート割り当て式1
    const PtExpr* expr2,  ///< [in] ポート割り当て式2
    const PtExpr* expr3,  ///< [in] ポート割り当て式3
    const PtExpr* expr4   ///< [in] ポート割り当て式4
  );

  /// @brief 名前と範囲付き module instance/UDP/gate instance の要素の生成
  void
  new_InstV(
    const FileRegion& fr,                 ///< [in] ファイル位置の情報
    const char* name,                     ///< [in] 名前
    const PtExpr* left,                   ///< [in] 範囲の左側の式
    const PtExpr* right,                  ///< [in] 範囲の右側の式
    PtrList<const PtConnection>* con_list ///< [in] ポート割り当ての配列
  );

  /// @brief 名前と範囲付き module instance/UDP/gate instance の要素の生成
  void
  new_InstV(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* name,     ///< [in] 名前
    const PtExpr* left,   ///< [in] 範囲の左側の式
    const PtExpr* right,  ///< [in] 範囲の右側の式
    const PtExpr* expr1   ///< [in] ポート割り当て式1
  );

  /// @brief 名前と範囲付き module instance/UDP/gate instance の要素の生成
  void
  new_InstV(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* name,     ///< [in] 名前
    const PtExpr* left,   ///< [in] 範囲の左側の式
    const PtExpr* right,  ///< [in] 範囲の右側の式
    const PtExpr* expr1,  ///< [in] ポート割り当て式1
    const PtExpr* expr2   ///< [in] ポート割り当て式2
  );

  /// @brief 名前と範囲付き module instance/UDP/gate instance の要素の生成
  void
  new_InstV(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* name,     ///< [in] 名前
    const PtExpr* left,   ///< [in] 範囲の左側の式
    const PtExpr* right,  ///< [in] 範囲の右側の式
    const PtExpr* expr1,  ///< [in] ポート割り当て式1
    const PtExpr* expr2,  ///< [in] ポート割り当て式2
    const PtExpr* expr3   ///< [in] ポート割り当て式3
  );

  /// @brief 名前と範囲付き module instance/UDP/gate instance の要素の生成
  void
  new_InstV(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* name,     ///< [in] 名前
    const PtExpr* left,   ///< [in] 範囲の左側の式
    const PtExpr* right,  ///< [in] 範囲の右側の式
    const PtExpr* expr1,  ///< [in] ポート割り当て式1
    const PtExpr* expr2,  ///< [in] ポート割り当て式2
    const PtExpr* expr3,  ///< [in] ポート割り当て式3
    const PtExpr* expr4   ///< [in] ポート割り当て式4
  );

  /// @brief generate 文の生成
  const PtItem*
  new_Generate(
    const FileRegion& fr ///< [in] ファイル位置の情報
  );

  /// @brief generate block 文の生成
  void
  new_GenBlock(
    const FileRegion& fr ///< [in] ファイル位置の情報
  );

  /// @brief 名前付き generate block 文の生成
  void
  new_GenBlock(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* name      ///< [in] 名前
  );

  /// @brief generate if 文の生成
  void
  new_GenIf(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const PtExpr* cond    ///< [in] 条件を表す式
  );

  /// @brief generate if 文の生成
  void
  new_GenIfElse(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const PtExpr* cond    ///< [in] 条件を表す式
  );

  /// @brief generate case 文の生成
  void
  new_GenCase(
    const FileRegion& fr,                   ///< [in] ファイル位置の情報
    const PtExpr* expr,                     ///< [in] 選択式
    PtrList<const PtGenCaseItem>* item_list ///< [in] generate case item のリスト
  );

  /// @brief generate case の要素の生成
  /// @return 生成された generate case item
  const PtGenCaseItem*
  new_GenCaseItem(
    const FileRegion& fr,             ///< [in] ファイル位置の情報
    PtrList<const PtExpr>* label_list ///< [in] 比較式のリスト
  );

  /// @brief generate for 文の生成
  void
  new_GenFor(
    const FileRegion& fr,    ///< [in] ファイル位置の情報
    const char* loop_var,    ///< [in] ループ変数
    const PtExpr* init_expr, ///< [in] 初期化式
    const PtExpr* cond,      ///< [in] ループ条件式
    const char* inc_var,     ///< [in] 増加式の左辺の変数
    const PtExpr* inc_expr,  ///< [in] 増加式
    const char* block_name   ///< [in] ブロック名
  );

  /// @brief specify block item の生成
  void
  new_SpecItem(
    const FileRegion& fr,                ///< [in] ファイル位置の情報
    VpiSpecItemType id,                  ///< [in] specify block item の種類
    PtrList<const PtExpr>* terminal_list ///< [in] 端子のリスト
  );

  /// @brief path 仕様を生成する．
  void
  new_SpecPath(
    const FileRegion& fr,       ///< [in] ファイル位置の情報
    VpiSpecPathType id,         ///< [in] spec path の種類
    const PtExpr* expr,         ///< [in] 条件式
    const PtPathDecl* path_decl ///< [in] パス記述
  );

  /// @brief パス記述の生成
  const PtPathDecl*
  new_PathDecl(
    const FileRegion& fr,               ///< [in] ファイル位置の情報
    int edge,                           ///< [in] エッジ
    PtrList<const PtExpr>* input_list,  ///< [in] 入力リスト
    int input_pol,                      ///< [in] 入力の極性
    VpiPathType op,                     ///< [in] パスタイプ
    PtrList<const PtExpr>* output_list, ///< [in] 出力リスト
    int output_pol,                     ///< [in] 出力の極性
    const PtExpr* expr,                 ///< [in] 条件式
    const PtPathDelay* path_delay       ///< [in] パス遅延
  );

  /// @brief パス記述の生成
  const PtPathDecl*
  new_PathDecl(
    const FileRegion& fr,               ///< [in] ファイル位置の情報
    int edge,			        ///< [in] エッジ
    PtrList<const PtExpr>* input_list,  ///< [in] 入力リスト
    int input_pol,		        ///< [in] 入力の極性
    VpiPathType op,		        ///< [in] パスタイプ
    const PtExpr* output,	        ///< [in] 出力
    int output_pol,		        ///< [in] 出力の極性
    const PtExpr* expr,		        ///< [in] 条件式
    const PtPathDelay* path_delay       ///< [in] パス遅延
  );

  /// @brief path delay value の生成 (値が1個)
  /// @return 生成された path delay value
  const PtPathDelay*
  new_PathDelay(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const PtExpr* value   ///< [in] 値
  );

  /// @brief path delay value の生成 (値が2個)
  /// @return 生成された path delay value
  const PtPathDelay*
  new_PathDelay(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const PtExpr* value1, ///< [in] 値1
    const PtExpr* value2  ///< [in] 値2
  );

  /// @brief path delay value の生成 (値が3個)
  /// @return 生成された path delay value
  const PtPathDelay*
  new_PathDelay(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const PtExpr* value1, ///< [in] 値1
    const PtExpr* value2, ///< [in] 値2
    const PtExpr* value3  ///< [in] 値3
  );

  /// @brief path delay value の生成 (値が6個)
  /// @return 生成された path delay value
  const PtPathDelay*
  new_PathDelay(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const PtExpr* value1, ///< [in] 値1
    const PtExpr* value2, ///< [in] 値2
    const PtExpr* value3, ///< [in] 値3
    const PtExpr* value4, ///< [in] 値4
    const PtExpr* value5, ///< [in] 値5
    const PtExpr* value6  ///< [in] 値6
  );

  /// @brief path delay value の生成 (値が12個)
  /// @param[in] fr ファイル位置の情報
  /// @return 生成された path delay value
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
  );


public:
  //////////////////////////////////////////////////////////////////////
  // ステートメント関連の要素の生成
  //////////////////////////////////////////////////////////////////////

  /// @brief disable 文の生成
  /// @return 生成された disable 文
  const PtStmt*
  new_Disable(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* name      ///< [in] 名前
  );

  /// @brief disable 文の生成 (階層付き識別子)
  /// @return 生成された disable 文
  const PtStmt*
  new_Disable(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    PuHierName* hname     ///< [in] 階層名
  );

  /// @brief enable 文の生成
  /// @return 生成された enable 文
  const PtStmt*
  new_Enable(
    const FileRegion& fr,           ///< [in] ファイル位置の情報
    const char* name,               ///< [in] 名前
    PtrList<const PtExpr>* arg_list ///< [in] 引数のリスト
  );

  /// @brief enable 文の生成 (階層付き識別子)
  /// @return 生成された enable 文
  const PtStmt*
  new_Enable(
    const FileRegion& fr,           ///< [in] ファイル位置の情報
    PuHierName* hname,              ///< [in] 階層名
    PtrList<const PtExpr>* arg_list ///< [in] 引数のリスト
  );

  /// @brief system task enable 文の生成
  const PtStmt*
  new_SysEnable(
    const FileRegion& fr,           ///< [in] ファイル位置の情報
    const char* name,               ///< [in] 名前
    PtrList<const PtExpr>* arg_list ///< [in] 引数のリスト
  );

  /// @brief delay control 文の生成
  /// @return 生成された delay control 文
  const PtStmt*
  new_DcStmt(
    const FileRegion& fr,   ///< [in] ファイル位置の情報
    const PtControl* delay, ///< [in] 遅延コントロール
    const PtStmt* body      ///< [in] 本体のステートメント
  );

  /// @brief event control 文の生成
  /// @return 生成された event control 文
  const PtStmt*
  new_EcStmt(
    const FileRegion& fr,   ///< [in] ファイル位置の情報
    const PtControl* event, ///< [in] イベントコントロール
    const PtStmt* body      ///< [in] 本体のステートメント
  );

  /// @brief wait 文の生成
  /// @return 生成された wait 文
  const PtStmt*
  new_Wait(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const PtExpr* cond,   ///< [in] 条件式
    const PtStmt* body    ///< [in] 本体のステートメント
  );

  /// @brief assign 文の生成
  /// @return 生成された assign 文
  const PtStmt*
  new_Assign(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const PtExpr* lhs,    ///< [in] 左辺式
    const PtExpr* rhs     ///< [in] 右辺式
  );

  /// @brief control 付き assign 文の生成
  /// @return 生成された assign 文
  const PtStmt*
  new_Assign(
    const FileRegion& fr,    ///< [in] ファイル位置の情報
    const PtExpr* lhs,       ///< [in] 左辺式
    const PtExpr* rhs,       ///< [in] 右辺式
    const PtControl* control ///< [in] 制御式
  );

  /// @brief nonblocking assign 文の生成
  /// @return 生成された nonblocking assign 文
  const PtStmt*
  new_NbAssign(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const PtExpr* lhs,    ///< [in] 左辺式
    const PtExpr* rhs     ///< [in] 右辺式
  );

  /// @brief control 付き nonblocking assign 文の生成
  /// @return 生成された nonblocking assign 文
  const PtStmt*
  new_NbAssign(
    const FileRegion& fr,    ///< [in] ファイル位置の情報
    const PtExpr* lhs,       ///< [in] 左辺式
    const PtExpr* rhs,       ///< [in] 右辺式
    const PtControl* control ///< [in] 制御式
  );

  /// @brief event 文の生成
  /// @return 生成された event 文
  const PtStmt*
  new_EventStmt(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const PtExpr* event   ///< [in] イベント名
  );

  /// @brief null 文の生成
  /// @return 生成された null 文
  const PtStmt*
  new_NullStmt(
    const FileRegion& fr ///< [in] ファイル位置の情報
  );

  /// @brief if 文の生成
  /// @return 生成された if 文
  const PtStmt*
  new_If(
    const FileRegion& fr,   ///< [in] ファイル位置の情報
    const PtExpr* expr,     ///< [in] 条件を表す式
    const PtStmt* then_body ///< [in] 成り立ったときに実行されるステートメント
  );

  /// @brief if 文の生成
  /// @return 生成された if 文
  const PtStmt*
  new_If(
    const FileRegion& fr,    ///< [in] ファイル位置の情報
    const PtExpr* expr,	     ///< [in] 条件を表す式
    const PtStmt* then_body, ///< [in] 成り立ったときに実行されるステートメント
    const PtStmt* else_body  ///< [in] 成り立たなかったときに実行されるステートメント
  );

  /// @brief case 文の生成
  /// @return 生成された case 文
  const PtStmt*
  new_Case(
    const FileRegion& fr,                    ///< [in] ファイル位置の情報
    const PtExpr* expr,                      ///< [in] 条件を表す式
    PtrList<const PtCaseItem>* caseitem_list ///< [in] case item のリスト
  );

  /// @brief casex 文の生成
  /// @return 生成された case 文
  const PtStmt*
  new_CaseX(
    const FileRegion& fr,                    ///< [in] ファイル位置の情報
    const PtExpr* expr,			     ///< [in] 条件を表す式
    PtrList<const PtCaseItem>* caseitem_list ///< [in] case item のリスト
  );

  /// @brief casez 文の生成
  /// @return 生成された case 文
  const PtStmt*
  new_CaseZ(
    const FileRegion& fr,                    ///< [in] ファイル位置の情報
    const PtExpr* expr,			     ///< [in] 条件を表す式
    PtrList<const PtCaseItem>* caseitem_list ///< [in] case item のリスト
  );

  /// @brief case item の生成
  /// @return 生成された case item
  const PtCaseItem*
  new_CaseItem(
    const FileRegion& fr,              ///< [in] ファイル位置の情報
    PtrList<const PtExpr>* label_list, ///< [in] ラベルのリスト
    const PtStmt* body                 ///< [in] 本体のステートメント
  );

  /// @brief forever 文の生成
  /// @return 生成された forever 文
  const PtStmt*
  new_Forever(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const PtStmt* body    ///< [in] 本体のステートメント
  );

  /// @brief repeat 文の生成
  /// @return 生成された repeat 文
  const PtStmt*
  new_Repeat(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const PtExpr* expr,   ///< [in] 繰り返し数を表す式
    const PtStmt* body    ///< [in] 本体のステートメント
  );

  /// @brief while 文の生成
  /// @return 生成された while 文
  const PtStmt*
  new_While(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const PtExpr* cond,   ///< [in] 繰り返し条件を表す式
    const PtStmt* body    ///< [in] 本体のステートメント
  );

  /// @brief for 文の生成
  /// @return 生成された for 文
  const PtStmt*
  new_For(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const PtStmt* init,   ///< [in] 初期化文
    const PtExpr* cond,   ///< [in] 繰り返し条件を表す式
    const PtStmt* next,   ///< [in] 増加文
    const PtStmt* body    ///< [in] 本体のステートメント
  );

  /// @brief procedural assign 文の生成
  /// @return 生成された procedural assign 文
  const PtStmt*
  new_PcAssign(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const PtExpr* lhs,    ///< [in] 左辺式
    const PtExpr* rhs     ///< [in] 右辺式
  );

  /// @brief deassign 文の生成
  /// @return 生成された deassign 文
  const PtStmt*
  new_Deassign(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const PtExpr* lhs     ///< [in] 左辺式
  );

  /// @brief force 文の生成
  /// @return 生成された force 文
  const PtStmt*
  new_Force(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const PtExpr* lhs,    ///< [in] 左辺式
    const PtExpr* rhs     ///< [in] 右辺式
  );

  /// @brief release 文の生成
  /// @return 生成された release 文
  const PtStmt*
  new_Release(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const PtExpr* lhs     ///< [in] 左辺式
  );

  /// @brief parallel block の生成
  /// @return 生成された parallel block
  const PtStmt*
  new_ParBlock(
    const FileRegion& fr,            ///< [in] ファイル位置の情報
    PtrList<const PtStmt>* stmt_list ///< [in] 本体のステートメントのリスト
  );

  /// @brief 名前付き parallel block の生成
  /// @return 生成された parallel block
  const PtStmt*
  new_NamedParBlock(
    const FileRegion& fr,            ///< [in] ファイル位置の情報
    const char* name,                ///< [in] 名前
    PtrList<const PtStmt>* stmt_list ///< [in] 本体のステートメントのリスト
  );

  /// @brief sequential block の生成
  /// @return 生成された sequential block
  const PtStmt*
  new_SeqBlock(
    const FileRegion& fr,            ///< [in] ファイル位置の情報
    PtrList<const PtStmt>* stmt_list ///< [in] 本体のステートメントのリスト
  );

  /// @brief 名前付き sequential block の生成
  /// @return 生成された sequential block
  const PtStmt*
  new_NamedSeqBlock(
    const FileRegion& fr,            ///< [in] ファイル位置の情報
    const char* name,                ///< [in] 名前
    PtrList<const PtStmt>* stmt_list ///< [in] 本体のステートメントのリスト
  );


public:
  //////////////////////////////////////////////////////////////////////
  // 式の生成
  //////////////////////////////////////////////////////////////////////

  /// @brief 単項演算子の生成
  /// @return 生成された演算子
  const PtExpr*
  new_Opr(
    const FileRegion& fr,              ///< [in] ファイル位置の情報
    VpiOpType type,                    ///< [in] 演算の種類
    const PtExpr* opr,                 ///< [in] オペランド
    PtrList<const PtAttrInst>* ai_list ///< [in] 属性リスト
  );

  /// @brief 二項演算子の生成
  /// @return 生成された演算子
  const PtExpr*
  new_Opr(
    const FileRegion& fr,              ///< [in] ファイル位置の情報
    VpiOpType type,                    ///< [in] 演算の種類
    const PtExpr* opr1,                ///< [in] オペランド1
    const PtExpr* opr2,                ///< [in] オペランド2
    PtrList<const PtAttrInst>* ai_list ///< [in] 属性リスト
  );

  /// @brief 三項演算子の生成
  /// @return 生成された演算子
  const PtExpr*
  new_Opr(
    const FileRegion& fr,               ///< [in] ファイル位置の情報
    VpiOpType type,		        ///< [in] 演算の種類
    const PtExpr* opr1,		        ///< [in] オペランド1
    const PtExpr* opr2,		        ///< [in] オペランド2
    const PtExpr* opr3,		        ///< [in] オペランド3
    PtrList<const PtAttrInst>* ai_list  ///< [in] 属性リスト
  );

  /// @brief concatination 演算子の生成
  /// @return 生成された concatination 演算子
  const PtExpr*
  new_Concat(
    const FileRegion& fr,            ///< [in] ファイル位置の情報
    PtrList<const PtExpr>* expr_list ///< [in] オペランドのリスト
  );

  /// @brief multi-concatination 演算子の生成
  /// @return 生成された multi-concatination 演算子
  const PtExpr*
  new_MultiConcat(
    const FileRegion& fr,            ///< [in] ファイル位置の情報
    const PtExpr* rep,               ///< [in] 繰り返し数
    PtrList<const PtExpr>* expr_list ///< [in] オペランドのリスト
  );

  /// @brief min/typ/max delay 演算子の生成
  /// @param[in] fr ファイル位置の情報
  /// @return 生成された min/typ/max 演算子
  const PtExpr*
  new_MinTypMax(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const PtExpr* val0,   ///< [in] minimum 値
    const PtExpr* val1,   ///< [in] typical 値
    const PtExpr* val2    ///< [in] maximum 値
  );

  /// @brief primary の生成
  /// @return 生成された primary
  const PtExpr*
  new_Primary(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* name      ///< [in] 識別子名
  );

  /// @brief インデックス付き primary の生成
  /// @return 生成された primary
  const PtExpr*
  new_Primary(
    const FileRegion& fr,              ///< [in] ファイル位置の情報
    const char* name,                  ///< [in] 名前
    PtrList<const PtExpr>* index_array ///< [in] インデックスのリスト
  );

  /// @brief 範囲指定付き primary の生成
  /// @return 生成された primary
  const PtExpr*
  new_Primary(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* name,     ///< [in] 名前
    VpiRangeMode mode,    ///< [in] 範囲指定のモード
    const PtExpr* left,   ///< [in] 範囲の左側の式
    const PtExpr* right	  ///< [in] 範囲の右側の式
  );

  /// @brief インデックスと範囲指定付き primary の生成
  /// @return 生成された primary
  const PtExpr*
  new_Primary(
    const FileRegion& fr,              ///< [in] ファイル位置の情報
    const char* name,                  ///< [in] 名前
    PtrList<const PtExpr>* index_list, ///< [in] インデックスのリスト
    VpiRangeMode mode,                 ///< [in] 範囲指定のモード
    const PtExpr* left,                ///< [in] 範囲の左側の式
    const PtExpr* right		       ///< [in] 範囲の右側の式
  );

  /// @brief primary の生成 (階層付き)
  /// @return 生成された primary
  const PtExpr*
  new_Primary(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    PuHierName* hname     ///< [in] 階層名
  );

  /// @brief インデックス付き primary の生成 (階層付き)
  /// @return 生成された primary
  const PtExpr*
  new_Primary(
    const FileRegion& fr,             ///< [in] ファイル位置の情報
    PuHierName* hname,                ///< [in] 階層名
    PtrList<const PtExpr>* index_list ///< [in] インデックスのリスト
  );

  /// @brief 範囲指定付き primary の生成 (階層付き)
  /// @return 生成された primary
  const PtExpr*
  new_Primary(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    PuHierName* hname,    ///< [in] 階層名
    VpiRangeMode mode,    ///< [in] 範囲指定のモード
    const PtExpr* left,   ///< [in] 範囲の左側の式
    const PtExpr* right	  ///< [in] 範囲の右側の式
  );

  /// @brief インデックスと範囲指定付き primary の生成 (階層付き)
  /// @return 生成された primary
  const PtExpr*
  new_Primary(
    const FileRegion& fr,              ///< [in] ファイル位置の情報
    PuHierName* hname,                 ///< [in] 階層名
    PtrList<const PtExpr>* index_list, ///< [in] インデックスのリスト
    VpiRangeMode mode,                 ///< [in] 範囲指定のモード
    const PtExpr* left,                ///< [in] 範囲の左側の式
    const PtExpr* right		       ///< [in] 範囲の右側の式
  );

  /// @brief constant primary の生成
  /// @return 生成された const primary
  const PtExpr*
  new_CPrimary(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* name,     ///< [in] 名前
    const PtExpr* index   ///< [in] インデックス
  );

  /// @brief constant primary の生成
  /// @return 生成された const primary
  const PtExpr*
  new_CPrimary(
    const FileRegion& fr,             ///< [in] ファイル位置の情報
    const char* name,                 ///< [in] 名前
    PtrList<const PtExpr>* index_list ///< [in] インデックスのリスト
  );

  /// @brief 範囲指定付き constant primary の生成
  /// @return 生成された constant primary
  const PtExpr*
  new_CPrimary(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* name,     ///< [in] 名前
    VpiRangeMode mode,    ///< [in] 範囲指定のモード
    const PtExpr* left,   ///< [in] 範囲の左側の式
    const PtExpr* right	  ///< [in] 範囲の右側の式
  );

  /// @brief インデックス付き constant primary の生成 (階層付き)
  /// @return 生成された constant primary
  const PtExpr*
  new_CPrimary(
    const FileRegion& fr,             ///< [in] ファイル位置の情報
    PuHierName* hname,                ///< [in] 階層名
    PtrList<const PtExpr>* index_list ///< [in] インデックスのリスト
  );

  /// @brief function call の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 関数名
  /// @param[in] arg_list 引数のリスト
  /// @return 生成された function call
  const PtExpr*
  new_FuncCall(
    const FileRegion& fr,              ///< [in] ファイル位置の情報
    const char* name,                  ///< [in] 名前
    PtrList<const PtExpr>* arg_list,   ///< [in] 引数のリスト
    PtrList<const PtAttrInst>* ai_list ///< [in] 属性リスト
  );

  /// @brief function call の生成 (階層付き)
  /// @return 生成された function call
  const PtExpr*
  new_FuncCall(
    const FileRegion& fr,              ///< [in] ファイル位置の情報
    PuHierName* hname,                 ///< [in] 階層名
    PtrList<const PtExpr>* arg_list,   ///< [in] 引数のリスト
    PtrList<const PtAttrInst>* ai_list ///< [in] 属性リスト
  );

  /// @brief system function call の生成
  /// @return 生成された function call
  const PtExpr*
  new_SysFuncCall(
    const FileRegion& fr,           ///< [in] ファイル位置の情報
    const char* name,               ///< [in] 名前
    PtrList<const PtExpr>* arg_list ///< [in] 引数のリスト
  );

  /// @brief 整数型の定数の生成
  /// @return 生成された定数
  const PtExpr*
  new_IntConst(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    ymuint value          ///< [in] 定数を表す整数値
  );

  /// @brief 整数型の定数の生成
  /// @return 生成された定数
  const PtExpr*
  new_IntConst(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* value     ///< [in] 定数の文字列表現
  );

  /// @brief 基底付き整数型の定数の生成
  /// @return 生成された定数
  const PtExpr*
  new_IntConst(
    const FileRegion& fr,    ///< [in] ファイル位置の情報
    VpiConstType const_type, ///< [in] 定数の種類
    const char* value        ///< [in] 定数の文字列表現
  );

  /// @brief サイズと基底付き定数の生成
  /// @return 生成された定数
  const PtExpr*
  new_IntConst(
    const FileRegion& fr,    ///< [in] ファイル位置の情報
    SizeType size,           ///< [in] サイズ
    VpiConstType const_type, ///< [in] 定数の種類
    const char* value        ///< [in] 定数の文字列表現
  );

  /// @brief 実数型の定数の生成
  /// @return 生成された定数
  const PtExpr*
  new_RealConst(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    double value          ///< [in] 定数の文字列表現
  );

  /// @brief 文字列型の定数の生成
  /// @return 生成された定数
  const PtExpr*
  new_StringConst(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* value     ///< [in] 文字列
  );


public:
  //////////////////////////////////////////////////////////////////////
  // その他
  //////////////////////////////////////////////////////////////////////

  /// @brief ディレイコントロールの生成
  /// @return 生成されたディレイコントロール
  const PtControl*
  new_DelayControl(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const PtExpr* value   ///< [in] 遅延を表す式
  );

  /// @brief イベントコントロールの生成 (any-event)
  /// @return 生成されたイベントコントロール
  const PtControl*
  new_EventControl(
    const FileRegion& fr ///< [in] ファイル位置の情報
  );

  /// @brief イベントコントロールの生成
  /// @return 生成されたイベントコントロール
  const PtControl*
  new_EventControl(
    const FileRegion& fr,      ///< [in] ファイル位置の情報
    const char* event_name,    ///< [in] イベントを表す名前
    const FileRegion& name_loc ///< [in] event_name の位置
  );

  /// @brief イベントコントロールの生成
  /// @return 生成されたイベントコントロール
  const PtControl*
  new_EventControl(
    const FileRegion& fr,      ///< [in] ファイル位置の情報
    PuHierName* event_name,    ///< [in] イベントを表す名前
    const FileRegion& name_loc ///< [in] event_name の位置
  );

  /// @brief イベントコントロールの生成
  /// @return 生成されたイベントコントロール
  const PtControl*
  new_EventControl(
    const FileRegion& fr,              ///< [in] ファイル位置の情報
    PtrList<const PtExpr>* event_array ///< [in] イベントのリスト
  );

  /// @brief リピートコントロールの生成 (any-event)
  /// @return 生成されたリピートコントロール
  const PtControl*
  new_RepeatControl(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const PtExpr* rep     ///< [in] 繰り返し数を表す式
  );

  /// @brief リピートコントロールの生成
  /// @return 生成されたリピートコントロール
  const PtControl*
  new_RepeatControl(
    const FileRegion& fr,      ///< [in] ファイル位置の情報
    const PtExpr* rep,         ///< [in] 繰り返し数を表す式
    const char* event_name,    ///< [in] 繰り返しの単位となるイベント
    const FileRegion& name_loc ///< [in] event_name の位置
  );

  /// @brief リピートコントロールの生成
  /// @return 生成されたリピートコントロール
  const PtControl*
  new_RepeatControl(
    const FileRegion& fr,      ///< [in] ファイル位置の情報
    const PtExpr* rep,	       ///< [in] 繰り返し数を表す式
    PuHierName* event_name,    ///< [in] 繰り返しの単位となるイベント
    const FileRegion& name_loc ///< [in] event_name の位置
  );

  /// @brief リピートコントロールの生成
  /// @return 生成されたリピートコントロール
  const PtControl*
  new_RepeatControl(
    const FileRegion& fr,             ///< [in] ファイル位置の情報
    const PtExpr* rep,                ///< [in] 繰り返し数を表す式
    PtrList<const PtExpr>* event_list ///< [in] 繰り返しの単位となるイベントのリスト
  );

  /// @brief 順序つき結合子の生成
  /// @return 生成された結合子
  const PtConnection*
  new_OrderedCon(
    const PtExpr* expr ///< [in] 結合する式
  );

  /// @brief 順序つき結合子の生成
  /// @return 生成された結合子
  const PtConnection*
  new_OrderedCon(
    const FileRegion& fr,              ///< [in] ファイル位置の情報
    const PtExpr* expr,                ///< [in] 結合する式
    PtrList<const PtAttrInst>* ai_list ///< [in] 属性リスト
  );

  /// @brief 名前付き結合子の生成
  /// @return 生成された結合子
  const PtConnection*
  new_NamedCon(
    const FileRegion& fr,                        ///< [in] ファイル位置の情報
    const char* name,                            ///< [in] 名前
    const PtExpr* expr = nullptr,                ///< [in] 結合する式
    PtrList<const PtAttrInst>* ai_list = nullptr ///< [in] 属性リスト
  );

  /// @brief strength の生成
  /// @return 生成された strength
  const PtStrength*
  new_Strength(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    VpiStrength value0,   ///< [in] '0' の強度
    VpiStrength value1    ///< [in] '1' の強度
  );

  /// @brief charge strength の生成
  /// @return 生成された strength
  const PtStrength*
  new_Strength(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    VpiStrength value     ///< [in] 強度
  );

  /// @brief 遅延値の生成 (1つの値)
  /// @return 生成された遅延値
  const PtDelay*
  new_Delay(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const PtExpr* value1  ///< [in] 値1
  );

  /// @brief 遅延値の生成 (2つの値)
  /// @return 生成された遅延値
  const PtDelay*
  new_Delay(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const PtExpr* value1, ///< [in] 値1
    const PtExpr* value2  ///< [in] 値2
  );

  /// @brief 遅延値の生成 (3つの値)
  /// @return 生成された遅延値
  const PtDelay*
  new_Delay(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const PtExpr* value1, ///< [in] 値1
    const PtExpr* value2, ///< [in] 値2
    const PtExpr* value3  ///< [in] 値3
  );

  /// @brief 階層名の生成
  PuHierName*
  new_HierName(
    const char* head_name, ///< [in] 階層の上位部分
    const char* name       ///< [in] 階層の最下位部分
  );

  /// @brief 階層名の生成
  PuHierName*
  new_HierName(
    const char* head_name, ///< [in] 階層の上位部分
    int index,             ///< [in] インデックス
    const char* name       ///< [in] 階層の最下位部分
  );

  /// @brief 階層名の追加
  void
  add_HierName(
    PuHierName* hname, ///< [in] 階層名の上位部分
    const char* name   ///< [in] 追加する名前
  );

  /// @brief 階層名の追加
  void
  add_HierName(
    PuHierName* hname, ///< [in] 階層名の上位部分
    int index,         ///< [in] インデックス
    const char* name   ///< [in] 追加する名前
  );


  //////////////////////////////////////////////////////////////////////
  // attribute instance 関係
  //////////////////////////////////////////////////////////////////////

  /// @brief attribute instance の生成
  /// @return 生成された attribute instance
  const PtAttrInst*
  new_AttrInst(
    const FileRegion& fr,              ///< [in] ファイル位置の情報
    PtrList<const PtAttrSpec>* as_list ///< [in] attribute spec のリスト
  );

  /// @brief attribute spec の生成
  /// @return 生成された attribute spec
  const PtAttrSpec*
  new_AttrSpec(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* name,     ///< [in] 名前
    const PtExpr* expr    ///< [in] 値
  );


public:
  //////////////////////////////////////////////////////////////////////
  // Ver2.0 リスト関係
  //////////////////////////////////////////////////////////////////////

  /// @brief モジュール定義の開始
  /// - port list の初期化
  /// - paramport list の初期化
  /// - iohead list の初期化
  /// - paramhead list の初期化
  /// - localparamhead list の初期化
  /// - declhead list の初期化
  /// - item list の初期化
  /// を行う．
  void
  init_module();

  /// @brief モジュール定義の終了
  void
  end_module();

  /// @brief UDP定義の開始
  /// - port list の初期化
  /// - iohead list の初期化
  /// - declhead list の初期化
  /// - UDP entry list の初期化
  /// を行う．
  void
  init_udp();

  /// @brief UDP 定義の終了
  void
  end_udp();

  /// @brief task/function 定義の開始
  /// - iohead list の初期化
  /// - paramhead list の初期化
  /// - localparamhead list の初期化
  /// - declhead list の初期化
  /// を行う．
  void
  init_tf();

  /// @brief task/function 定義の終了
  void
  end_tf();

  /// @brief generate block の開始
  void
  init_generate();

  /// @brief generate block の終了
  void
  end_generate();

  /// @brief generate-if の開始
  void
  init_genif();

  /// @brief generate-if の終了
  void
  end_genif();

  /// @brief generate-if の else 節の開始
  void
  init_genelse();

  /// @grief generate-if の else 節の終了
  void
  end_genelse();

  /// @brief block-statment の開始
  void
  init_block();

  /// @brief block-statement の終了
  void
  end_block();

  /// @brief ポートリストにポートを追加する．
  void
  add_port(PtiPort* port);

  /// @brief ポートリストをvectorに変換する．
  vector<const PtPort*>
  get_port_vector()
  {
    return convert<const PtPort*, PtiPort*>(mPortList);
  }

  /// @brief ポート参照リストを初期化する．
  void
  init_portref_list();

  /// @brief ポート参照リストに要素を追加する．
  void
  add_portref(
    const PtExpr* portref
  );

  /// @brief parameter port 宣言ヘッダを追加する．
  void
  add_paramport_head(
    PtiDeclHead* head,
    PtrList<const PtAttrInst>* attr_list
  );

  /// @brief parameter port 宣言の終わり
  void
  flush_paramport();

  /// @brief parameter port リストを得る．
  vector<const PtDeclHead*>
  get_paramport_array()
  {
    return convert<const PtDeclHead*, PtiDeclHead*>(mParamPortHeadList);
  }

  /// @brief IOポート宣言リストにIO宣言ヘッダを追加する．
  void
  add_ioport_head(
    PtiIOHead* head,
    PtrList<const PtAttrInst>* attr_list
  );

  /// @brief IO宣言の終わり
  void
  flush_io();

  /// @brief IO宣言リストにIO宣言ヘッダを追加する．
  void
  add_io_head(
    PtiIOHead* head,
    PtrList<const PtAttrInst>* attr_list
  );

  /// @brief IO宣言リストにIO宣言要素を追加する．
  void
  add_io_item(
    const PtIOItem* item
  );

  /// @brief module用の IO宣言リストを得る．
  vector<const PtIOHead*>
  get_module_io_array()
  {
    return convert<const PtIOHead*, PtiIOHead*>(mModuleIOHeadList);
  }

  /// @brief task/function 用の IO宣言リストを得る．
  vector<const PtIOHead*>
  get_tf_io_array();

  /// @brief module 用の parameter リストを得る．
  vector<const PtDeclHead*>
  get_module_param_array();

  /// @brief task/function 用の parameter リストを得る．
  vector<const PtDeclHead*>
  get_tf_param_array();

  /// @brief module 用の localparam リストを得る．
  vector<const PtDeclHead*>
  get_module_localparam_array();

  /// @brief task/function 用の localparam リストを得る．
  vector<const PtDeclHead*>
  get_tf_localparam_array();

  /// @brief 宣言リストに宣言ヘッダを追加する．
  void
  add_decl_head(
    PtiDeclHead* head,
    PtrList<const PtAttrInst>* attr_list = nullptr
  );

  /// @brief 宣言リストに宣言要素を追加する．
  void
  add_decl_item(
    const PtDeclItem* item
  );

  /// @brief item リストに要素を追加する．
  void
  add_item(
    const PtItem* item,
    PtrList<const PtAttrInst>* attr_list = nullptr
  );

  /// @brief UdpEntry を追加する．
  void
  add_udp_entry(
    const PtUdpEntry* entry
  );

  /// @brief UdpValue のリストを初期化する．
  void
  init_udp_value_list();

  /// @brief UdpValue を追加する．
  void
  add_udp_value(
    const PtUdpValue* value
  )
  {
    mUdpValueList.push_back(value);
  }

  /// @brief UdpValue のリストを得る．
  vector<const PtUdpValue*>
  get_udp_value_array()
  {
    return mUdpValueList;
  }

  /// @brief defparam リストを初期化する．
  void
  init_defparam();

  /// @brief defparam リストに要素を追加する．
  void
  add_defparam(
    const PtDefParam* defparam
  );

  /// @brief contassign リストを初期化する．
  void
  init_contassign();

  /// @brief contassign リストに要素を追加する．
  void
  add_contassign(
    const PtContAssign* contassign
  );

  /// @brief instance リストを初期化する．
  void
  init_inst();

  /// @brief instance リストに要素を追加する．
  void
  add_inst(
    const PtInst* inst
  );


public:
  //////////////////////////////////////////////////////////////////////
  // リスト関係
  //////////////////////////////////////////////////////////////////////

  // 要素を持たないリストを作る．
  template <typename T>
  PtrList<T>*
  new_list()
  {
    return new PtrList<T>();
  }


public:

  /// @brief 関数内で使えるステートメントかどうかのチェック
  bool
  check_function_statement(
    const PtStmt* stmt
  );

  /// @brief default ラベルが2つ以上含まれていないかどうかのチェック
  bool
  check_default_label(
    const PtrList<const PtCaseItem>* ci_list
  );

  /// @brief 使用されているモジュール名を登録する．
  /// @param[in] name 登録する名前
  void
  reg_defname(
    const char* name
  );

  /// @brief attribute instance を登録する．
  void
  reg_attrinst(
    const PtBase* ptobj,
    PtrList<const PtAttrInst>* attr_list,
    bool def = false
  );


public:

  /// @brief lex オブジェクトの取得
  /// @return lex オブジェクトを返す．
  Lex&
  lex()
  {
    return *mLex;
  }

  /// @brief yylex とのインターフェイス
  /// @param[out] lvalp 値を格納する変数
  /// @param[out] llocp 位置情報を格納する変数
  /// @return 読み込んだトークンの id を返す．
  int
  yylex(
    YYSTYPE& lval,
    FileRegion& lloc
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 現在の declhead リストをスタックに積む．
  void
  push_declhead_list()
  {
    mDeclHeadListStack.push_back(vector<PtiDeclHead*>());
  }

  /// @brief スタックのトップを取り出す．
  vector<const PtDeclHead*>
  pop_declhead_list()
  {
    auto vec{convert<const PtDeclHead*, PtiDeclHead*>(cur_declhead_list())};
    mDeclHeadListStack.pop_back();
    return vec;
  }

  /// @brief 現在の item リストをスタックに積む．
  void
  push_item_list()
  {
    mItemListStack.push_back(vector<const PtItem*>());
  }

  /// @brief スタックのトップを取り出す．
  vector<const PtItem*>
  pop_item_list()
  {
    auto vec{cur_item_list()};
    mItemListStack.pop_back();
    return vec;
  }

  /// @brief 現在の宣言ヘッダのリストを返す．
  vector<PtiDeclHead*>&
  cur_declhead_list()
  {
    return mDeclHeadListStack.back();
  }

  /// @brief 現在の item リストを返す．
  vector<const PtItem*>&
  cur_item_list()
  {
    return mItemListStack.back();
  }

  /// @brief 階層名の生成
  PuHierName*
  new_HierName(
    const PtNameBranch* nb, ///< [in] 上位の名前のリスト
    const char* name        ///< [in] 階層の最下位部分
  );

  /// @brief 入出力宣言中の重複チェックを行う．
  bool
  check_PortArray(
    const vector<const PtIOHead*>& iohead_array
  );

  /// @brief ポート宣言とIO宣言の齟齬をチェックする．
  /// @param[in] port_vector ポート宣言のリスト
  /// @param[in] iohead_array IO宣言のリスト
  /// @param[out] iodecl_dirs IO宣言名をキーとして向きを保持する辞書
  void
  check_IO(
    const vector<const PtPort*>& port_array,
    const vector<const PtIOHead*>& iohead_array,
    unordered_map<string, VpiDir>& iodecl_dirs
  );

  /// @brief vector を基底クラスの vector に変換する．
  template <typename T1,
	    typename T2>
  static
  vector<T1>
  convert(
    const vector<T2>& src
  )
  {
    SizeType n = src.size();
    vector<T1> vec(n);
    for ( SizeType i = 0; i < n; ++ i ) {
      vec[i] = src[i];
    }
    return vec;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // メモリアロケータ
  Alloc& mAlloc;

  // パース木を保持するクラス
  PtMgr& mPtMgr;

  // パース木の要素の生成を行うクラス
  unique_ptr<PtiFactory> mFactory;

  // 字句解析を行うオブジェクト
  unique_ptr<Lex> mLex;


public:
  //////////////////////////////////////////////////////////////////////
  // 静的に存在するリスト
  //////////////////////////////////////////////////////////////////////

  // ポートリスト
  vector<PtiPort*> mPortList;

  // ポート参照リスト
  vector<const PtExpr*> mPortRefList;

  // parameter port 宣言ヘッダリスト
  vector<PtiDeclHead*> mParamPortHeadList;

  // モジュール用 IO宣言ヘッダリスト
  vector<PtiIOHead*> mModuleIOHeadList;

  // task/function 用 IO宣言ヘッダリスト
  vector<PtiIOHead*> mTfIOHeadList;

  // IO宣言要素リスト
  vector<const PtIOItem*> mIOItemList;

  // 宣言要素リスト
  vector<const PtDeclItem*> mDeclItemList;

  // UDP エントリのリスト
  vector<const PtUdpEntry*> mUdpEntryList;

  // UDP のテーブルの値のリスト
  vector<const PtUdpValue*> mUdpValueList;

  // defparam 要素のリスト
  vector<const PtDefParam*> mDefParamList;

  // contassign リスト
  vector<const PtContAssign*> mContAssignList;

  // instance リスト
  vector<const PtInst*> mInstList;


private:
  //////////////////////////////////////////////////////////////////////
  // 現在，登録対象となっているリストを指すポインタ
  //////////////////////////////////////////////////////////////////////

  // 現在の IO宣言ヘッダリスト
  // 実際には mModuleIOHeadList か mTfIOHeadList を指す．
  vector<PtiIOHead*>* mCurIOHeadList;

  // 現在の宣言ヘッダの配列
  // スタックから取り出された最終結果
  vector<const PtDeclHead*> mCurDeclArray;

  // 現在の item の配列
  // スタックから取り出された最終結果
  vector<const PtItem*> mCurItemArray;

  // generate-if の then 節の宣言ヘッダリスト
  vector<const PtDeclHead*> mGenThenDeclArray;

  // generate-if の then 節の item リスト
  vector<const PtItem*> mGenThenItemArray;

  // generate-if の else 節の宣言ヘッダリスト
  vector<const PtDeclHead*> mGenElseDeclArray;

  // generate-if の else 節の item リスト
  vector<const PtItem*> mGenElseItemArray;


public:
  //////////////////////////////////////////////////////////////////////
  // mCurXXXList のスタック
  //////////////////////////////////////////////////////////////////////

  // 宣言ヘッダリストのスタック
  vector<vector<PtiDeclHead*>> mDeclHeadListStack;

  // item リストのスタック
  vector<vector<const PtItem*>> mItemListStack;

};

END_NAMESPACE_YM_VERILOG

#endif // PARSER_H
