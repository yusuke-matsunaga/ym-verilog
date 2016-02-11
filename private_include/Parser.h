﻿#ifndef PARSER_H
#define PARSER_H

/// @file Parser.h
/// @brief Parser のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym/pt/PtP.h"
#include "ym//File.h"
#include "ym//Alloc.h"
#include "ym//FragAlloc.h"
#include "ym//UnitAlloc.h"
#include "ym//HashSet.h"
#include "ym//HashMap.h"
#include "PtiFwd.h"
#include "PtiFactory.h"
#include "PtiDecl.h"
#include "PtrList.h"


BEGIN_NAMESPACE_YM_VERILOG

class VlLineWatcher;
class PtMgr;
class Lex;
class PuHierName;

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
  /// @param[in] ptmgr 読んだ結果のパース木を登録するマネージャ
  /// @param[in] alloc メモリアロケータ
  /// @param[in] ptifactory パース木の要素を生成するファクトリクラス
  Parser(PtMgr& ptmgr,
	 Alloc& alloc,
	 PtiFactory& ptifactory);

  /// @brief デストラクタ
  ~Parser();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイルを読み込む．
  /// @param[in] filename 読み込むファイル名
  /// @param[in] searchpath サーチパス
  /// @param[in] watcher_list 行番号ウオッチャーのリスト
  /// @retval true 正常に終了した．
  /// @retval false エラーが起こった．
  bool
  read_file(const string& filename,
	    const SearchPathList& searchpath,
	    const list<VlLineWatcher*>& watcher_list);


public:
  //////////////////////////////////////////////////////////////////////
  // UDP 関連の生成関数
  //////////////////////////////////////////////////////////////////////

  /// @brief Verilog1995 タイプのUDP を生成する．
  void
  new_Udp1995(const FileRegion& file_region,
	      const char* name,
	      const char* init_name,
	      const FileRegion& init_loc,
	      const PtExpr* init_value,
	      PtrList<const PtAttrInst>* ai_list);

  /// @brief Verilog2001 タイプのUDP を生成する．
  void
  new_Udp2001(const FileRegion& file_region,
	      const char* name,
	      const char* init_name,
	      const FileRegion& init_loc,
	      const PtExpr* init_value,
	      PtrList<const PtAttrInst>* ai_list);

  /// @brief combinational UDP 用のテーブルエントリの生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] output_loc 出力記号の位置
  /// @param[in] output_symbol 出力記号
  void
  new_UdpEntry(const FileRegion& fr,
	       const FileRegion& output_loc,
	       char output_symbol);

  /// @brief sequential UDP 用のテーブルエントリの生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] current_loc 現状態記号の位置
  /// @param[in] current_symbol 現状態記号
  /// @param[in] output_loc 出力記号の位置
  /// @param[in] output_symbol 出力記号
  void
  new_UdpEntry(const FileRegion& fr,
	       const FileRegion& current_loc,
	       char current_symbol,
	       const FileRegion& output_loc,
	       char output_symbol);

  /// @brief UDP のテーブルエントリの要素の値の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] symbol シンボル
  /// @return 生成された値
  void
  new_UdpValue(const FileRegion& fr,
	       char symbol);

  /// @brief UDP のテーブルエントリの要素の値の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] symbol1, symbol2 シンボル
  /// @return 生成された値
  void
  new_UdpValue(const FileRegion& fr,
	       char symbol1,
	       char symbol2);


private:

  /// @brief new_Udp の下請け関数
  void
  new_Udp(const FileRegion& file_region,
	  const char* udp_name,
	  const char* init_name,
	  const FileRegion& init_loc,
	  const PtExpr* init_value,
	  PtrList<const PtAttrInst>* ai_list,
	  bool is_seq,
	  const PtIOItem* out_item,
	  PtiPortArray port_array,
	  PtIOHeadArray iohead_array);


public:
  //////////////////////////////////////////////////////////////////////
  // モジュール関連の要素の生成関数
  //////////////////////////////////////////////////////////////////////

  /// @brief Verilog1995 タイプのモジュール(のテンプレート)を生成する．
  void
  new_Module1995(const FileRegion& file_region,
		 bool is_macro,
		 const char* name,
		 PtrList<const PtAttrInst>* ai_list);

  /// @brief Verilog2001 タイプのモジュール(のテンプレート)を生成する．
  void
  new_Module2001(const FileRegion& file_region,
		 bool is_macro,
		 const char* name,
		 PtrList<const PtAttrInst>* ai_list);


public:
  //////////////////////////////////////////////////////////////////////
  // ポート関連の要素の生成関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入出力宣言中の重複チェックを行う．
  bool
  check_PortArray(PtIOHeadArray iohead_array);

  /// @brief 入出力宣言からポートを作る．
  PtiPortArray
  new_PortArray(PtIOHeadArray iohead_array);

  /// @brief 空のポートの生成
  void
  new_Port();

  /// @brief ポートの生成 (内側の式のみ指定するタイプ)
  /// @param[in] file_region ファイル位置
  void
  new_Port1(const FileRegion& file_region);

  /// @brief ポートの生成 (外側の名前のみ指定するタイプ)
  /// @param[in] file_region ファイル位置
  /// @param[in] name 外側の名前
  void
  new_Port2(const FileRegion& file_region,
	    const char* name);

  /// @brief ポートの生成 (外側の名前と内側の式を指定するタイプ)
  /// @param[in] file_region ファイル位置
  /// @param[in] name 外側の名前
  /// @note 内側の式は new_PortRef() で内部に追加されているものとする．
  void
  new_Port3(const FileRegion& file_region,
	    const char* name);

  /// @brief ポート参照式の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name ポートに接続している内部の識別子名
  void
  new_PortRef(const FileRegion& fr,
	      const char* name);

  /// @brief ビット指定つきポート参照式の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name ポートに接続している内部の識別子名
  /// @param[in] index ビット指定用の式
  void
  new_PortRef(const FileRegion& fr,
	      const char* name,
	      const PtExpr* index);

  /// @brief 範囲指定付きポート参照式の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name ポートに接続している内部の識別子名
  /// @param[in] range_mode 範囲指定のモード
  /// @param[in] left 範囲指定の左側の式
  /// @param[in] right 範囲指摘の右側の式
  void
  new_PortRef(const FileRegion& fr,
	      const char* name,
	      tVpiRangeMode range_mode,
	      const PtExpr* left,
	      const PtExpr* right);


public:
  //////////////////////////////////////////////////////////////////////
  // IO 宣言関係の要素の生成関数
  //////////////////////////////////////////////////////////////////////

  /// @brief IO 宣言のヘッダの生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] type IO の種類
  /// @param[in] sign 符号付きのとき true となるフラグ
  PtiIOHead*
  new_IOHead(const FileRegion& fr,
	     tPtIOType type,
	     bool sign);

  /// @brief IO 宣言のヘッダの生成 (reg 型)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] type IO の種類
  /// @param[in] sign 符号付きのとき true となるフラグ
  PtiIOHead*
  new_RegIOHead(const FileRegion& fr,
		tPtIOType type,
		bool sign);

  /// @brief IO 宣言のヘッダの生成 (ネット型)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] type IO の種類
  /// @param[in] net_type 補助的なネット型
  /// @param[in] sign 符号付きのとき true となるフラグ
  PtiIOHead*
  new_NetIOHead(const FileRegion& fr,
		tPtIOType type,
		tVpiNetType net_type,
		bool sign);

  /// @brief IO 宣言のヘッダの生成 (変数型)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] type IO の種類
  /// @param[in] var_type 補助的な変数型
  PtiIOHead*
  new_VarIOHead(const FileRegion& fr,
		tPtIOType type,
		tVpiVarType var_type);

  /// @brief 範囲付きの IO 宣言のヘッダの生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] type IO の種類
  /// @param[in] sign 符号付きのとき true となるフラグ
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  PtiIOHead*
  new_IOHead(const FileRegion& fr,
	     tPtIOType type,
	     bool sign,
	     const PtExpr* left,
	     const PtExpr* right);

  /// @brief 範囲付きの IO 宣言のヘッダの生成 (reg 型)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] type IO の種類
  /// @param[in] sign 符号付きのとき true となるフラグ
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  PtiIOHead*
  new_RegIOHead(const FileRegion& fr,
		tPtIOType type,
		bool sign,
		const PtExpr* left,
		const PtExpr* right);

  /// @brief 範囲付きの IO 宣言のヘッダの生成 (ネット型)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] type IO の種類
  /// @param[in] net_type 補助的なネット型
  /// @param[in] sign 符号付きのとき true となるフラグ
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  PtiIOHead*
  new_NetIOHead(const FileRegion& fr,
		tPtIOType type,
		tVpiNetType net_type,
		bool sign,
		const PtExpr* left,
		const PtExpr* right);

  /// @brief IO 宣言の要素の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 要素名
  void
  new_IOItem(const FileRegion& fr,
	     const char* name);


  /// @brief 初期値付き IO 宣言の要素の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 要素名
  /// @param[in] init_value 初期値を表す式
  void
  new_IOItem(const FileRegion& fr,
	     const char* name,
	     const PtExpr* init_value);


public:
  //////////////////////////////////////////////////////////////////////
  // 宣言関連の要素の生成
  //////////////////////////////////////////////////////////////////////

  /// @brief パラメータ宣言のヘッダの生成 (型指定なし)
  /// @param[in] fr ファイル位置の情報
  PtiDeclHead*
  new_ParamH(const FileRegion& fr);

  /// @brief 範囲指定型パラメータ宣言のヘッダの生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] sign 符号付きのとき true となるフラグ
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  PtiDeclHead*
  new_ParamH(const FileRegion& fr,
	     bool sign,
	     const PtExpr* left,
	     const PtExpr* right);

  /// @brief 組み込み型パラメータ宣言のヘッダの生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] var_type データ型
  PtiDeclHead*
  new_ParamH(const FileRegion& fr,
	     tVpiVarType var_type);

  /// @brief local param 宣言のヘッダの生成 (型指定なし)
  /// @param[in] fr ファイル位置の情報
  PtiDeclHead*
  new_LocalParamH(const FileRegion& fr);

  /// @brief 範囲指定型 local param 宣言のヘッダの生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] sign 符号付きのとき true となるフラグ
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  PtiDeclHead*
  new_LocalParamH(const FileRegion& fr,
		  bool sign,
		  const PtExpr* left,
		  const PtExpr* right);

  /// @brief 組み込み型パラメータ宣言のヘッダの生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] var_type データ型
  PtiDeclHead*
  new_LocalParamH(const FileRegion& fr,
		  tVpiVarType var_type);

  /// @brief specparam 宣言のヘッダの生成
  /// @param[in] fr ファイル位置の情報
  /// @return 生成された specparam
  PtiDeclHead*
  new_SpecParamH(const FileRegion& fr);

  /// @brief 範囲指定型 specparam 宣言のヘッダの生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  PtiDeclHead*
  new_SpecParamH(const FileRegion& fr,
		 const PtExpr* left,
		 const PtExpr* right);

  /// @brief イベント宣言のヘッダの生成
  /// @param[in] fr ファイル位置の情報
  /// @return 生成されたイベント
  PtiDeclHead*
  new_EventH(const FileRegion& fr);

  /// @brief genvar 宣言のヘッダの生成
  /// @param[in] fr ファイル位置の情報
  PtiDeclHead*
  new_GenvarH(const FileRegion& fr);

  /// @brief 変数宣言のヘッダの生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] var_type データ型
  PtiDeclHead*
  new_VarH(const FileRegion& fr,
	   tVpiVarType var_type);

  /// @brief 1ビット型 reg 宣言のヘッダの生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] sign 符号付きの時 true となるフラグ
  PtiDeclHead*
  new_RegH(const FileRegion& fr,
	   bool sign);

  /// @brief 範囲指定型 reg 宣言のヘッダの生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] sign 符号付きの時 true となるフラグ
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  PtiDeclHead*
  new_RegH(const FileRegion& fr,
	   bool sign,
	   const PtExpr* left,
	   const PtExpr* right);

  /// @brief 1ビット型 net 宣言のヘッダの生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] type net の型
  /// @param[in] sign 符号の有無を表すフラグ
  PtiDeclHead*
  new_NetH(const FileRegion& fr,
	   tVpiNetType type,
	   bool sign);

  /// @brief 1ビット型 net 宣言のヘッダの生成 (strength あり)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] type net の型
  /// @param[in] sign 符号の有無を表すフラグ
  /// @param[in] strength 信号強度
  PtiDeclHead*
  new_NetH(const FileRegion& fr,
	   tVpiNetType type,
	   bool sign,
	   const PtStrength* strength);

  /// @brief 1ビット型 net 宣言のヘッダの生成 (遅延あり)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] type net の型
  /// @param[in] sign 符号の有無を表すフラグ
  /// @param[in] delay 遅延
  PtiDeclHead*
  new_NetH(const FileRegion& fr,
	   tVpiNetType type,
	   bool sign,
	   const PtDelay* delay);

  /// @brief 1ビット型 net 宣言のヘッダの生成 (strength, 遅延あり)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] type net の型
  /// @param[in] sign 符号の有無を表すフラグ
  /// @param[in] strength 信号強度
  /// @param[in] delay 遅延
  PtiDeclHead*
  new_NetH(const FileRegion& fr,
	   tVpiNetType type,
	   bool sign,
	   const PtStrength* strength,
	   const PtDelay* delay);

  /// @brief 範囲指定型 net 宣言のヘッダの生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] type net の型
  /// @param[in] vstype vector/scalar 指定
  /// @param[in] sign 符号の有無を表すフラグ
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  PtiDeclHead*
  new_NetH(const FileRegion& fr,
	   tVpiNetType type,
	   tVpiVsType vstype,
	   bool sign,
	   const PtExpr* left,
	   const PtExpr* right);

  /// @brief 範囲指定型 net 宣言のヘッダの生成 (strengthあり)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] type net の型
  /// @param[in] vstype vector/scalar 指定
  /// @param[in] sign 符号の有無を表すフラグ
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @param[in] strength 信号強度
  PtiDeclHead*
  new_NetH(const FileRegion& fr,
	   tVpiNetType type,
	   tVpiVsType vstype,
	   bool sign,
	   const PtExpr* left,
	   const PtExpr* right,
	   const PtStrength* strength);

  /// @brief 範囲指定型 net 宣言のヘッダの生成 (遅延あり)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] type net の型
  /// @param[in] vstype vector/scalar 指定
  /// @param[in] sign 符号の有無を表すフラグ
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @param[in] delay 遅延
  PtiDeclHead*
  new_NetH(const FileRegion& fr,
	   tVpiNetType type,
	   tVpiVsType vstype,
	   bool sign,
	   const PtExpr* left,
	   const PtExpr* right,
	   const PtDelay* delay);

  /// @brief 範囲指定型 net 宣言のヘッダの生成 (strength, 遅延あり)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] type net の型
  /// @param[in] vstype vector/scalar 指定
  /// @param[in] sign 符号の有無を表すフラグ
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @param[in] strength 信号強度
  /// @param[in] delay 遅延
  PtiDeclHead*
  new_NetH(const FileRegion& fr,
	   tVpiNetType type,
	   tVpiVsType vstype,
	   bool sign,
	   const PtExpr* left,
	   const PtExpr* right,
	   const PtStrength* strength,
	   const PtDelay* delay);

  /// @brief 宣言要素の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 名前
  void
  new_DeclItem(const FileRegion& fr,
	       const char* name);

  /// @brief 初期値付き宣言要素の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 名前
  /// @param[in] init_value 初期値を表す式
  void
  new_DeclItem(const FileRegion& fr,
	       const char* name,
	       const PtExpr* init_value);

  /// @brief 配列型宣言要素の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 名前
  /// @param[in] range_list 配列の各次元の範囲のリスト
  void
  new_DeclItem(const FileRegion& fr,
	       const char* name,
	       PtrList<const PtRange>* range_list);

  /// @brief 範囲の生成
  /// @param[in] msb MSB を表す式
  /// @param[in] lsb LSB を表す式
  const PtRange*
  new_Range(const FileRegion& fr,
	    const PtExpr* msb,
	    const PtExpr* lsb);


public:
  //////////////////////////////////////////////////////////////////////
  // item 関連の要素の生成
  //////////////////////////////////////////////////////////////////////

  /// @brief defparam 文のヘッダの生成
  /// @param[in] fr ファイル位置の情報
  const PtItem*
  new_DefParamH(const FileRegion& fr);

  /// @brief defparam 文の要素の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 名前
  /// @param[in] value 値を表す式
  void
  new_DefParam(const FileRegion& fr,
	       const char* name,
	       const PtExpr* value);

  /// @brief defparam 文の要素の生成 (階層つき識別子)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] hname 階層名
  /// @param[in] value 値を表す式
  void
  new_DefParam(const FileRegion& fr,
	       PuHierName* hname,
	       const PtExpr* value);

  /// @brief continuous assign 文のヘッダの生成
  /// @param[in] fr ファイル位置の情報
  const PtItem*
  new_ContAssignH(const FileRegion& fr);

  /// @brief continuous assign 文のヘッダの生成 (strengthつき)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] strength 信号強度
  const PtItem*
  new_ContAssignH(const FileRegion& fr,
		  const PtStrength* strength);

  /// @brief continuous assign 文のヘッダの生成 (遅延付き)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] delay 遅延値
  const PtItem*
  new_ContAssignH(const FileRegion& fr,
		  const PtDelay* delay);

  /// @brief continuous assign 文のヘッダの生成 (strength, 遅延付き)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] strength 信号強度
  /// @param[in] delay 遅延値
  /// @param[in] elem_array 要素のリスト
  const PtItem*
  new_ContAssignH(const FileRegion& fr,
		  const PtStrength* strength,
		  const PtDelay* delay);

  /// @brief continuous assign 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] lhs 左辺式
  /// @param[in] rhs 右辺式
  void
  new_ContAssign(const FileRegion& fr,
		 const PtExpr* lhs,
		 const PtExpr* rhs);

  /// @brief initial 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] body 本体のステートメント
  const PtItem*
  new_Initial(const FileRegion& fr,
	      const PtStmt* body);

  /// @brief always 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] body 本体のステートメント
  const PtItem*
  new_Always(const FileRegion& fr,
	     const PtStmt* body);

  /// @brief task 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name task 名
  /// @param[in] automatic automatic task の時に true となるフラグ
  /// @param[in] stmt 本体のステートメント
  const PtItem*
  new_Task(const FileRegion& fr,
	   const char* name,
	   bool automatic,
	   const PtStmt* stmt);

  /// @brief 1ビット型 function 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name function 名
  /// @param[in] automatic automatic task の時に true となるフラグ
  /// @param[in] sign signed 属性がついていたら true となるフラグ
  /// @param[in] stmt 本体のステートメント
  const PtItem*
  new_Function(const FileRegion& fr,
	       const char* name,
	       bool automatic,
	       bool sign,
	       const PtStmt* stmt);

  /// @brief 範囲指定型 function 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name function 名
  /// @param[in] automatic automatic task の時に true となるフラグ
  /// @param[in] sign 符号つきの時に true となるフラグ
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @param[in] stmt 本体のステートメント
  const PtItem*
  new_SizedFunc(const FileRegion& fr,
		const char* name,
		bool automatic,
		bool sign,
		const PtExpr* left,
		const PtExpr* right,
		const PtStmt* stmt);

  /// @brief 組み込み型 function 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name function 名
  /// @param[in] automatic automatic task の時に true となるフラグ
  /// @param[in] sign signed 属性がついていたら true となるフラグ
  /// @param[in] func_type 関数の戻値の型
  /// @param[in] stmt 本体のステートメント
  const PtItem*
  new_TypedFunc(const FileRegion& fr,
		const char* name,
		bool automatic,
		bool sign,
		tVpiVarType func_type,
		const PtStmt* stmt);

  /// @brief gate instance 文のヘッダの生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] type primitive の型
  const PtItem*
  new_GateH(const FileRegion& fr,
	    tVpiPrimType type);

  /// @brief gate instance 文のヘッダの生成 (strength付き)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] type primitive の型
  /// @param[in] strength 信号強度
  const PtItem*
  new_GateH(const FileRegion& fr,
	    tVpiPrimType type,
	    const PtStrength* strength);

  /// @brief gate instance 文のヘッダの生成 (遅延付き)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] type primitive の型
  /// @param[in] delay 遅延値
  const PtItem*
  new_GateH(const FileRegion& fr,
	    tVpiPrimType type,
	    const PtDelay* delay);

  /// @brief gate instance 文のヘッダの生成 (strength, 遅延付き)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] type primitive の型
  /// @param[in] strength 信号強度
  /// @param[in] delay 遅延値
  const PtItem*
  new_GateH(const FileRegion& fr,
	    tVpiPrimType type,
	    const PtStrength* strength,
	    const PtDelay* delay);

  /// @brief module instance/UDP instance 文のヘッダの生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] def_name 定義名
  const PtItem*
  new_MuH(const FileRegion& fr,
	  const char* def_name);

  /// @brief module instance/UDP instance 文のヘッダの生成 (strength付き)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] def_name 定義名
  /// @param[in] strength 信号強度
  const PtItem*
  new_MuH(const FileRegion& fr,
	  const char* def_name,
	  const PtStrength* strength);

  /// @brief module instance/UDP instance 文のヘッダの生成 (遅延付き)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] def_name 定義名
  /// @param[in] delay 遅延値
  const PtItem*
  new_MuH(const FileRegion& fr,
	  const char* def_name,
	  const PtDelay* delay);

  /// @brief module instance/UDP instance 文のヘッダの生成 (strength, 遅延付き)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] def_name 定義名
  /// @param[in] strength 信号強度
  /// @param[in] delay 遅延値
  const PtItem*
  new_MuH(const FileRegion& fr,
	  const char* def_name,
	  const PtStrength* strength,
	  const PtDelay* delay);

  /// @brief module instance/UDP instance 文のヘッダの生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] def_name 定義名
  /// @param[in] con_list ポート割り当てリスト
  const PtItem*
  new_MuH(const FileRegion& fr,
	  const char* def_name,
	  PtrList<const PtConnection>* con_list);

  /// @brief module instance/UDP/gate instance の要素の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] con_list ポート割り当ての配列
  void
  new_Inst(const FileRegion& fr,
	   PtrList<const PtConnection>* con_list);

  /// @brief module instance/UDP/gate instance の要素の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] expr1 ポート割り当て
  void
  new_Inst(const FileRegion& fr,
	   const PtExpr* expr1);

  /// @brief module instance/UDP/gate instance の要素の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] expr1, expr2 ポート割り当て
  void
  new_Inst(const FileRegion& fr,
	   const PtExpr* expr1,
	   const PtExpr* expr2);

  /// @brief module instance/UDP/gate instance の要素の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] expr1, expr2, expr3 ポート割り当て
  void
  new_Inst(const FileRegion& fr,
	   const PtExpr* expr1,
	   const PtExpr* expr2,
	   const PtExpr* expr3);

  /// @brief module instance/UDP/gate instance の要素の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] expr1, expr2, expr3, expr4 ポート割り当て
  void
  new_Inst(const FileRegion& fr,
	   const PtExpr* expr1,
	   const PtExpr* expr2,
	   const PtExpr* expr3,
	   const PtExpr* expr4);

  /// @brief 名前付き module instance/UDP/gate instance の要素の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 名前
  /// @param[in] con_list ポート割り当ての配列
  void
  new_InstN(const FileRegion& fr,
	    const char* name,
	    PtrList<const PtConnection>* con_list);

  /// @brief 名前付き module instance/UDP/gate instance の要素の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 名前
  /// @param[in] expr1 ポート割り当て
  void
  new_InstN(const FileRegion& fr,
	    const char* name,
	    const PtExpr* expr1);

  /// @brief 名前付き module instance/UDP/gate instance の要素の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 名前
  /// @param[in] expr1, expr2 ポート割り当て
  void
  new_InstN(const FileRegion& fr,
	    const char* name,
	    const PtExpr* expr1,
	    const PtExpr* expr2);

  /// @brief 名前付き module instance/UDP/gate instance の要素の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 名前
  /// @param[in] expr1, expr2, expr3 ポート割り当て
  void
  new_InstN(const FileRegion& fr,
	    const char* name,
	    const PtExpr* expr1,
	    const PtExpr* expr2,
	    const PtExpr* expr3);

  /// @brief 名前付き module instance/UDP/gate instance の要素の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 名前
  /// @param[in] expr1, expr2, expr3, expr4 ポート割り当て
  void
  new_InstN(const FileRegion& fr,
	    const char* name,
	    const PtExpr* expr1,
	    const PtExpr* expr2,
	    const PtExpr* expr3,
	    const PtExpr* expr4);

  /// @brief 名前と範囲付き module instance/UDP/gate instance の要素の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 名前
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @param[in] con_list ポート割り当ての配列
  void
  new_InstV(const FileRegion& fr,
	    const char* name,
	    const PtExpr* left,
	    const PtExpr* right,
	    PtrList<const PtConnection>* con_list);

  /// @brief 名前と範囲付き module instance/UDP/gate instance の要素の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 名前
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @param[in] expr1 ポート割り当て
  void
  new_InstV(const FileRegion& fr,
	    const char* name,
	    const PtExpr* left,
	    const PtExpr* right,
	    const PtExpr* expr1);

  /// @brief 名前と範囲付き module instance/UDP/gate instance の要素の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 名前
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @param[in] expr1, expr2 ポート割り当て
  void
  new_InstV(const FileRegion& fr,
	    const char* name,
	    const PtExpr* left,
	    const PtExpr* right,
	    const PtExpr* expr1,
	    const PtExpr* expr2);

  /// @brief 名前と範囲付き module instance/UDP/gate instance の要素の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 名前
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @param[in] expr1, expr2, expr3 ポート割り当て
  void
  new_InstV(const FileRegion& fr,
	    const char* name,
	    const PtExpr* left,
	    const PtExpr* right,
	    const PtExpr* expr1,
	    const PtExpr* expr2,
	    const PtExpr* expr3);

  /// @brief 名前と範囲付き module instance/UDP/gate instance の要素の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 名前
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @param[in] expr1, expr2, expr3, expr4 ポート割り当て
  void
  new_InstV(const FileRegion& fr,
	    const char* name,
	    const PtExpr* left,
	    const PtExpr* right,
	    const PtExpr* expr1,
	    const PtExpr* expr2,
	    const PtExpr* expr3,
	    const PtExpr* expr4);

  /// @brief generate 文の生成
  /// @param[in] fr ファイル位置の情報
  const PtItem*
  new_Generate(const FileRegion& fr);

  /// @brief generate block 文の生成
  /// @param[in] fr ファイル位置の情報
  void
  new_GenBlock(const FileRegion& fr);

  /// @brief 名前付き generate block 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 名前
  void
  new_GenBlock(const FileRegion& fr,
	       const char* name);

  /// @brief generate if 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] cond 条件を表す式
  void
  new_GenIf(const FileRegion& fr,
	    const PtExpr* cond);

  /// @brief generate if 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] cond 条件を表す式
  void
  new_GenIfElse(const FileRegion& fr,
		const PtExpr* cond);

  /// @brief generate case 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] expr 選択式
  /// @param[in] item_list generate case item のリスト
  void
  new_GenCase(const FileRegion& fr,
	      const PtExpr* expr,
	      PtrList<const PtGenCaseItem>* item_list);

  /// @brief generate case の要素の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] label_list 比較式のリスト
  /// @return 生成された generate case item
  const PtGenCaseItem*
  new_GenCaseItem(const FileRegion& fr,
		  PtrList<const PtExpr>* label_list);

  /// @brief generate for 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] loop_var ループ変数
  /// @param[in] init_expr 初期化式
  /// @param[in] cond ループ条件式
  /// @param[in] inc_var 増加式の左辺の変数
  /// @param[in] inc_expr 増加式
  /// @param[in] block_name ブロック名
  void
  new_GenFor(const FileRegion& fr,
	     const char* loop_var,
	     const PtExpr* init_expr,
	     const PtExpr* cond,
	     const char* inc_var,
	     const PtExpr* inc_expr,
	     const char* block_name);

  /// @brief specify block item の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] id specify block item の種類
  /// @param[in] terminal_list 端子のリスト
  void
  new_SpecItem(const FileRegion& fr,
	       tVpiSpecItemType id,
	       PtrList<const PtExpr>* terminal_list);

  /// @brief path 仕様を生成する．
  /// @param[in] fr ファイル位置の情報
  /// @param[in] id spec path の種類
  /// @param[in] expr 条件式
  /// @param[in] path_decl パス記述
  void
  new_SpecPath(const FileRegion& fr,
	       tVpiSpecPathType id,
	       const PtExpr* expr,
	       const PtPathDecl* path_decl);

  /// @brief パス記述の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] edge
  /// @param[in] input_list
  /// @param[in] input_pol
  /// @param[in] op
  /// @param[in] output_list
  /// @param[in] output_pol
  /// @param[in] expr
  /// @param[in] path_delay
  const PtPathDecl*
  new_PathDecl(const FileRegion& fr,
	       int edge,
	       PtrList<const PtExpr>* input_list,
	       int input_pol,
	       int op,
	       PtrList<const PtExpr>* output_list,
	       int output_pol,
	       const PtExpr* expr,
	       const PtPathDelay* path_delay);

  /// @brief パス記述の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] edge
  /// @param[in] input_list
  /// @param[in] input_pol
  /// @param[in] op
  /// @param[in] output
  /// @param[in] output_pol
  /// @param[in] expr
  /// @param[in] path_delay
  const PtPathDecl*
  new_PathDecl(const FileRegion& fr,
	       int edge,
	       PtrList<const PtExpr>* input_list,
	       int input_pol,
	       int op,
	       const PtExpr* output,
	       int output_pol,
	       const PtExpr* expr,
	       const PtPathDelay* path_delay);

  /// @brief path delay value の生成 (値が1個)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] value 値
  /// @return 生成された path delay value
  const PtPathDelay*
  new_PathDelay(const FileRegion& fr,
		const PtExpr* value);

  /// @brief path delay value の生成 (値が2個)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] value1 値1
  /// @param[in] value2 値2
  /// @return 生成された path delay value
  const PtPathDelay*
  new_PathDelay(const FileRegion& fr,
		const PtExpr* value1,
		const PtExpr* value2);

  /// @brief path delay value の生成 (値が3個)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] value1 値1
  /// @param[in] value2 値2
  /// @param[in] value3 値3
  /// @return 生成された path delay value
  const PtPathDelay*
  new_PathDelay(const FileRegion& fr,
		const PtExpr* value1,
		const PtExpr* value2,
		const PtExpr* value3);

  /// @brief path delay value の生成 (値が6個)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] value1 値1
  /// @param[in] value2 値2
  /// @param[in] value3 値3
  /// @param[in] value4 値4
  /// @param[in] value5 値5
  /// @param[in] value6 値6
  /// @return 生成された path delay value
  const PtPathDelay*
  new_PathDelay(const FileRegion& fr,
		const PtExpr* value1,
		const PtExpr* value2,
		const PtExpr* value3,
		const PtExpr* value4,
		const PtExpr* value5,
		const PtExpr* value6);

  /// @brief path delay value の生成 (値が12個)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] value1 値1
  /// @param[in] value2 値2
  /// @param[in] value3 値3
  /// @param[in] value4 値4
  /// @param[in] value5 値5
  /// @param[in] value6 値6
  /// @param[in] value7 値7
  /// @param[in] value8 値8
  /// @param[in] value9 値9
  /// @param[in] value10 値10
  /// @param[in] value11 値11
  /// @param[in] value12 値12
  /// @return 生成された path delay value
  const PtPathDelay*
  new_PathDelay(const FileRegion& fr,
		const PtExpr* value1,
		const PtExpr* value2,
		const PtExpr* value3,
		const PtExpr* value4,
		const PtExpr* value5,
		const PtExpr* value6,
		const PtExpr* value7,
		const PtExpr* value8,
		const PtExpr* value9,
		const PtExpr* value10,
		const PtExpr* value11,
		const PtExpr* value12);


public:
  //////////////////////////////////////////////////////////////////////
  // ステートメント関連の要素の生成
  //////////////////////////////////////////////////////////////////////

  /// @brief disable 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 停止するスコープ名
  /// @return 生成された disable 文
  const PtStmt*
  new_Disable(const FileRegion& fr,
	      const char* name);

  /// @brief disable 文の生成 (階層付き識別子)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] hname 階層名
  /// @return 生成された disable 文
  const PtStmt*
  new_Disable(const FileRegion& fr,
	      PuHierName* hname);

  /// @brief enable 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 起動するタスク名
  /// @param[in] arg_array 引数のリスト
  /// @return 生成された enable 文
  const PtStmt*
  new_Enable(const FileRegion& fr,
	     const char* name,
	     PtrList<const PtExpr>* arg_list);

  /// @brief enable 文の生成 (階層付き識別子)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] hname 階層名
  /// @param[in] name 末尾の名前
  /// @param[in] arg_array 引数のリスト
  /// @return 生成された enable 文
  const PtStmt*
  new_Enable(const FileRegion& fr,
	     PuHierName* hname,
	     PtrList<const PtExpr>* arg_list);

  /// @brief system task enable 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 起動するシステムタスク名
  /// @param[in] arg_array 引数のリスト
  const PtStmt*
  new_SysEnable(const FileRegion& fr,
		const char* name,
		PtrList<const PtExpr>* arg_list);

  /// @brief delay control 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] delay delay control (delay control 文とは別物)
  /// @param[in] body 本体のステートメント
  /// @return 生成された delay control 文
  const PtStmt*
  new_DcStmt(const FileRegion& fr,
	     const PtControl* delay,
	     const PtStmt* body);

  /// @brief event control 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] event event control (event control 文とは別物)
  /// @param[in] body 本体のステートメント
  /// @return 生成された event control 文
  const PtStmt*
  new_EcStmt(const FileRegion& fr,
	     const PtControl* event,
	     const PtStmt* body);

  /// @brief wait 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] cond 条件式
  /// @param[in] body 本体のステートメント
  /// @return 生成された wait 文
  const PtStmt*
  new_Wait(const FileRegion& fr,
	   const PtExpr* cond,
	   const PtStmt* body);

  /// @brief assign 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] lhs 左辺式
  /// @param[in] rhs 右辺式
  /// @return 生成された assign 文
  const PtStmt*
  new_Assign(const FileRegion& fr,
	     const PtExpr* lhs,
	     const PtExpr* rhs);

  /// @brief control 付き assign 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] lhs 左辺式
  /// @param[in] rhs 右辺式
  /// @param[in] control 制御式
  /// @return 生成された assign 文
  const PtStmt*
  new_Assign(const FileRegion& fr,
	     const PtExpr* lhs,
	     const PtExpr* rhs,
	     const PtControl* control);

  /// @brief nonblocking assign 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] lhs 左辺式
  /// @param[in] rhs 右辺式
  /// @return 生成された nonblocking assign 文
  const PtStmt*
  new_NbAssign(const FileRegion& fr,
	       const PtExpr* lhs,
	       const PtExpr* rhs);

  /// @brief control 付き nonblocking assign 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] lhs 左辺式
  /// @param[in] rhs 右辺式
  /// @param[in] control 制御式
  /// @return 生成された nonblocking assign 文
  const PtStmt*
  new_NbAssign(const FileRegion& fr,
	       const PtExpr* lhs,
	       const PtExpr* rhs,
	       const PtControl* control);

  /// @brief event 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] event イベント名
  /// @return 生成された event 文
  const PtStmt*
  new_EventStmt(const FileRegion& fr,
		const PtExpr* event);

  /// @brief null 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @return 生成された null 文
  const PtStmt*
  new_NullStmt(const FileRegion& fr);

  /// @brief if 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] expr 条件式
  /// @param[in] then_body 成り立ったときに実行されるステートメント
  /// @return 生成された if 文
  const PtStmt*
  new_If(const FileRegion& fr,
	 const PtExpr* expr,
	 const PtStmt* then_body);

  /// @brief if 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] expr 条件式
  /// @param[in] then_body 成り立ったときに実行されるステートメント
  /// @param[in] else_body 成り立たなかったときに実行されるステートメント
  /// @return 生成された if 文
  const PtStmt*
  new_If(const FileRegion& fr,
	 const PtExpr* expr,
	 const PtStmt* then_body,
	 const PtStmt* else_body);

  /// @brief case 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] expr 条件式
  /// @param[in] caseitem_list case item のリスト
  /// @return 生成された case 文
  const PtStmt*
  new_Case(const FileRegion& fr,
	   const PtExpr* expr,
	   PtrList<const PtCaseItem>* caseitem_list);

  /// @brief casex 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] expr 条件式
  /// @param[in] caseitem_list case item のリスト
  /// @return 生成された case 文
  const PtStmt*
  new_CaseX(const FileRegion& fr,
	    const PtExpr* expr,
	    PtrList<const PtCaseItem>* caseitem_list);

  /// @brief casez 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] expr 条件式
  /// @param[in] caseitem_list case item のリスト
  /// @return 生成された case 文
  const PtStmt*
  new_CaseZ(const FileRegion& fr,
	    const PtExpr* expr,
	    PtrList<const PtCaseItem>* caseitem_list);

  /// @brief case item の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] label_list ラベルのリスト
  /// @param[in] body 本体のステートメント
  /// @return 生成された case item
  const PtCaseItem*
  new_CaseItem(const FileRegion& fr,
	       PtrList<const PtExpr>* label_list,
	       const PtStmt* body);

  /// @brief forever 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] body 本体のステートメント
  /// @return 生成された forever 文
  const PtStmt*
  new_Forever(const FileRegion& fr,
	      const PtStmt* body);

  /// @brief repeat 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] expr 繰り返し数を表す式
  /// @param[in] body 本体のステートメント
  /// @return 生成された repeat 文
  const PtStmt*
  new_Repeat(const FileRegion& fr,
	     const PtExpr* expr,
	     const PtStmt* body);

  /// @brief while 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] cond 繰り返し条件式
  /// @param[in] body 本体のステートメント
  /// @return 生成された while 文
  const PtStmt*
  new_While(const FileRegion& fr,
	    const PtExpr* cond,
	    const PtStmt* body);

  /// @brief for 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] init 初期化文
  /// @param[in] cond 繰り返し条件式
  /// @param[in] next 増加文
  /// @param[in] body 本体のステートメント
  /// @return 生成された for 文
  const PtStmt*
  new_For(const FileRegion& fr,
	  const PtStmt* init,
	  const PtExpr* cond,
	  const PtStmt* next,
	  const PtStmt* body);

  /// @brief procedural assign 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] lhs 左辺式
  /// @param[in] rhs 右辺式
  /// @return 生成された procedural assign 文
  const PtStmt*
  new_PcAssign(const FileRegion& fr,
	       const PtExpr* lhs,
	       const PtExpr* rhs);

  /// @brief deassign 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] lhs 左辺式
  /// @return 生成された deassign 文
  const PtStmt*
  new_Deassign(const FileRegion& fr,
	       const PtExpr* lhs);

  /// @brief force 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] lhs 左辺式
  /// @param[in] rhs 右辺式
  /// @return 生成された force 文
  const PtStmt*
  new_Force(const FileRegion& fr,
	    const PtExpr* lhs,
	    const PtExpr* rhs);

  /// @brief release 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] lhs 左辺式
  /// @return 生成された release 文
  const PtStmt*
  new_Release(const FileRegion& fr,
	      const PtExpr* lhs);

  /// @brief parallel block の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] stmt_list 本体のステートメントのリスト
  /// @return 生成された parallel block
  const PtStmt*
  new_ParBlock(const FileRegion& fr,
	       PtrList<const PtStmt>* stmt_list);

  /// @brief 名前付き parallel block の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name ブロック名
  /// @param[in] stmt_list 本体のステートメントのリスト
  /// @return 生成された parallel block
  const PtStmt*
  new_NamedParBlock(const FileRegion& fr,
		    const char* name,
		    PtrList<const PtStmt>* stmt_list);

  /// @brief sequential block の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] stmt_list 本体のステートメントのリスト
  /// @return 生成された sequential block
  const PtStmt*
  new_SeqBlock(const FileRegion& fr,
	       PtrList<const PtStmt>* stmt_list);

  /// @brief 名前付き sequential block の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name ブロック名
  /// @param[in] stmt_list 本体のステートメントのリスト
  /// @return 生成された sequential block
  const PtStmt*
  new_NamedSeqBlock(const FileRegion& fr,
		    const char* name,
		    PtrList<const PtStmt>* stmt_list);


public:
  //////////////////////////////////////////////////////////////////////
  // 式の生成
  //////////////////////////////////////////////////////////////////////

  /// @brief 単項演算子の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] type 演算の種類
  /// @param[in] opr オペランド
  /// @return 生成された演算子
  const PtExpr*
  new_Opr(const FileRegion& fr,
	  tVlOpType type,
	  const PtExpr* opr,
	  PtrList<const PtAttrInst>* ai_list);

  /// @brief 二項演算子の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] type 演算の種類
  /// @param[in] opr1 オペランド1
  /// @param[in] opr2 オペランド2
  /// @return 生成された演算子
  const PtExpr*
  new_Opr(const FileRegion& fr,
	  tVlOpType type,
	  const PtExpr* opr1,
	  const PtExpr* opr2,
	  PtrList<const PtAttrInst>* ai_list);

  /// @brief 三項演算子の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] type 演算の種類
  /// @param[in] opr1 オペランド1
  /// @param[in] opr2 オペランド2
  /// @param[in] opr3 オペランド3
  /// @return 生成された演算子
  const PtExpr*
  new_Opr(const FileRegion& fr,
	  tVlOpType type,
	  const PtExpr* opr1,
	  const PtExpr* opr2,
	  const PtExpr* opr3,
	  PtrList<const PtAttrInst>* ai_list);

  /// @brief concatination 演算子の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] expr_list オペランドのリスト
  /// @return 生成された concatination 演算子
  const PtExpr*
  new_Concat(const FileRegion& fr,
	     PtrList<const PtExpr>* expr_list);

  /// @brief multi-concatination 演算子の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] rep 繰り返し数
  /// @param[in] expr_list 結合するオペランドのリスト
  /// @return 生成された multi-concatination 演算子
  const PtExpr*
  new_MultiConcat(const FileRegion& fr,
		  const PtExpr* rep,
		  PtrList<const PtExpr>* expr_list);

  /// @brief min/typ/max delay 演算子の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] val0 minimum 値
  /// @param[in] val1 typical 値
  /// @param[in] val2 maximum 値
  /// @return 生成された min/typ/max 演算子
  const PtExpr*
  new_MinTypMax(const FileRegion& fr,
		const PtExpr* val0,
		const PtExpr* val1,
		const PtExpr* val2);

  /// @brief primary の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 識別子名
  /// @return 生成された primary
  const PtExpr*
  new_Primary(const FileRegion& fr,
	      const char* name);

  /// @brief インデックス付き primary の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 識別子名
  /// @param[in] index_list インデックスのリスト
  /// @return 生成された primary
  const PtExpr*
  new_Primary(const FileRegion& fr,
	      const char* name,
	      PtrList<const PtExpr>* index_array);

  /// @brief 範囲指定付き primary の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 識別子名
  /// @param[in] mode 範囲指定のモード
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @return 生成された primary
  const PtExpr*
  new_Primary(const FileRegion& fr,
	      const char* name,
	      tVpiRangeMode mode,
	      const PtExpr* left,
	      const PtExpr* right);

  /// @brief インデックスと範囲指定付き primary の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 識別子名
  /// @param[in] index_list インデックスのリスト
  /// @param[in] mode 範囲指定のモード
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @return 生成された primary
  const PtExpr*
  new_Primary(const FileRegion& fr,
	      const char* name,
	      PtrList<const PtExpr>* index_list,
	      tVpiRangeMode mode,
	      const PtExpr* left,
	      const PtExpr* right);

  /// @brief primary の生成 (階層付き)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] hname 階層名
  /// @return 生成された primary
  const PtExpr*
  new_Primary(const FileRegion& fr,
	      PuHierName* hname);

  /// @brief インデックス付き primary の生成 (階層付き)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] hname 階層名
  /// @param[in] index_list インデックスのリスト
  /// @return 生成された primary
  const PtExpr*
  new_Primary(const FileRegion& fr,
	      PuHierName* hname,
	      PtrList<const PtExpr>* index_list);

  /// @brief 範囲指定付き primary の生成 (階層付き)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] hname 階層名
  /// @param[in] mode 範囲指定のモード
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @return 生成された primary
  const PtExpr*
  new_Primary(const FileRegion& fr,
	      PuHierName* hname,
	      tVpiRangeMode mode,
	      const PtExpr* left,
	      const PtExpr* right);

  /// @brief インデックスと範囲指定付き primary の生成 (階層付き)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] hname 階層名
  /// @param[in] index_list インデックスのリスト
  /// @param[in] mode 範囲指定のモード
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @return 生成された primary
  const PtExpr*
  new_Primary(const FileRegion& fr,
	      PuHierName* hname,
	      PtrList<const PtExpr>* index_list,
	      tVpiRangeMode mode,
	      const PtExpr* left,
	      const PtExpr* right);

  /// @brief constant primary の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 識別子名
  /// @param[in] index インデックス
  /// @return 生成された const primary
  const PtExpr*
  new_CPrimary(const FileRegion& fr,
	       const char* name,
	       const PtExpr* index);

  /// @brief constant primary の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 識別子名
  /// @param[in] index_list インデックスのリスト
  /// @return 生成された const primary
  const PtExpr*
  new_CPrimary(const FileRegion& fr,
	       const char* name,
	       PtrList<const PtExpr>* index_list);

  /// @brief 範囲指定付き constant primary の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 識別子名
  /// @param[in] mode 範囲指定のモード
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @return 生成された constant primary
  const PtExpr*
  new_CPrimary(const FileRegion& fr,
	       const char* name,
	       tVpiRangeMode mode,
	       const PtExpr* left,
	       const PtExpr* right);

  /// @brief インデックス付き constant primary の生成 (階層付き)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] hname 階層名
  /// @param[in] index_list インデックスのリスト
  /// @return 生成された constant primary
  const PtExpr*
  new_CPrimary(const FileRegion& fr,
	       PuHierName* hname,
	       PtrList<const PtExpr>* index_list);

  /// @brief function call の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 関数名
  /// @param[in] arg_list 引数のリスト
  /// @return 生成された function call
  const PtExpr*
  new_FuncCall(const FileRegion& fr,
	       const char* name,
	       PtrList<const PtExpr>* arg_list,
	       PtrList<const PtAttrInst>* ai_list);

  /// @brief function call の生成 (階層付き)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] hname 階層名
  /// @param[in] arg_list 引数のリスト
  /// @return 生成された function call
  const PtExpr*
  new_FuncCall(const FileRegion& fr,
	       PuHierName* hname,
	       PtrList<const PtExpr>* arg_list,
	       PtrList<const PtAttrInst>* ai_list);

  /// @brief system function call の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 関数名
  /// @param[in] arg_array 引数のリスト
  /// @return 生成された function call
  const PtExpr*
  new_SysFuncCall(const FileRegion& fr,
		  const char* name,
		  PtrList<const PtExpr>* arg_list);

  /// @brief 整数型の定数の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] value 定数を表す整数値
  /// @return 生成された定数
  const PtExpr*
  new_IntConst(const FileRegion& fr,
	       ymuint value);

  /// @brief 整数型の定数の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] value 定数の文字列表現
  /// @return 生成された定数
  const PtExpr*
  new_IntConst(const FileRegion& fr,
	       const char* value);

  /// @brief 基底付き整数型の定数の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] const_type 定数の種類
  /// @param[in] value 定数の文字列表現
  /// @return 生成された定数
  const PtExpr*
  new_IntConst(const FileRegion& fr,
	       tVpiConstType const_type,
	       const char* value);

  /// @brief サイズと基底付き定数の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] size サイズ
  /// @param[in] const_type 定数の種類
  /// @param[in] value 定数の文字列表現
  /// @return 生成された定数
  const PtExpr*
  new_IntConst(const FileRegion& fr,
	       ymuint size,
	       tVpiConstType const_type,
	       const char* value);

  /// @brief 実数型の定数の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] value 定数の文字列表現
  /// @return 生成された定数
  const PtExpr*
  new_RealConst(const FileRegion& fr,
		double value);

  /// @brief 文字列型の定数の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] value 文字列
  /// @return 生成された定数
  const PtExpr*
  new_StringConst(const FileRegion& fr,
		  const char* value);


public:
  //////////////////////////////////////////////////////////////////////
  // その他
  //////////////////////////////////////////////////////////////////////

  /// @brief ディレイコントロールの生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] value 遅延を表す式
  /// @return 生成されたディレイコントロール
  const PtControl*
  new_DelayControl(const FileRegion& fr,
		   const PtExpr* value);

  /// @brief イベントコントロールの生成 (any-event)
  /// @param[in] fr ファイル位置の情報
  /// @return 生成されたイベントコントロール
  const PtControl*
  new_EventControl(const FileRegion& fr);

  /// @brief イベントコントロールの生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] event_name イベントを表す名前
  /// @return 生成されたイベントコントロール
  const PtControl*
  new_EventControl(const FileRegion& fr,
		   const char* event_name,
		   const FileRegion& name_loc);

  /// @brief イベントコントロールの生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] event_name イベントを表す階層名
  /// @return 生成されたイベントコントロール
  const PtControl*
  new_EventControl(const FileRegion& fr,
		   PuHierName* event_name,
		   const FileRegion& name_loc);

  /// @brief イベントコントロールの生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] event_list イベントのリスト
  /// @return 生成されたイベントコントロール
  const PtControl*
  new_EventControl(const FileRegion& fr,
		   PtrList<const PtExpr>* event_array);

  /// @brief リピートコントロールの生成 (any-event)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] rep 繰り返し数を表す式
  /// @return 生成されたリピートコントロール
  const PtControl*
  new_RepeatControl(const FileRegion& fr,
		    const PtExpr* rep);

  /// @brief リピートコントロールの生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] rep 繰り返し数を表す式
  /// @param[in] event 繰り返しの単位となるイベント
  /// @return 生成されたリピートコントロール
  const PtControl*
  new_RepeatControl(const FileRegion& fr,
		    const PtExpr* rep,
		    const char* event_name,
		    const FileRegion& name_loc);

  /// @brief リピートコントロールの生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] rep 繰り返し数を表す式
  /// @param[in] event 繰り返しの単位となるイベント
  /// @return 生成されたリピートコントロール
  const PtControl*
  new_RepeatControl(const FileRegion& fr,
		    const PtExpr* rep,
		    PuHierName* event_name,
		    const FileRegion& name_loc);

  /// @brief リピートコントロールの生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] rep 繰り返し数を表す式
  /// @param[in] event_list 繰り返しの単位となるイベントのリスト
  /// @return 生成されたリピートコントロール
  const PtControl*
  new_RepeatControl(const FileRegion& fr,
		    const PtExpr* rep,
		    PtrList<const PtExpr>* event_list);

  /// @brief 順序つき結合子の生成
  /// @param[in] expr 結合する式
  /// @return 生成された結合子
  const PtConnection*
  new_OrderedCon(const PtExpr* expr);

  /// @brief 順序つき結合子の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] expr 結合する式
  /// @return 生成された結合子
  const PtConnection*
  new_OrderedCon(const FileRegion& fr,
		 const PtExpr* expr,
		 PtrList<const PtAttrInst>* ai_list);

  /// @brief 名前付き結合子の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 名前
  /// @param[in] expr 結合する式
  /// @return 生成された結合子
  const PtConnection*
  new_NamedCon(const FileRegion& fr,
	       const char* name,
	       const PtExpr* expr = nullptr,
	       PtrList<const PtAttrInst>* ai_list = nullptr);

  /// @brief strength の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] value0 '0' の強度
  /// @param[in] value1 '1' の強度
  /// @return 生成された strength
  const PtStrength*
  new_Strength(const FileRegion& fr,
	       tVpiStrength value0,
	       tVpiStrength value1);

  /// @brief charge strength の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] value 強度
  /// @return 生成された strength
  const PtStrength*
  new_Strength(const FileRegion& fr,
	       tVpiStrength value);

  /// @brief 遅延値の生成 (1つの値)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] value1 値1
  /// @return 生成された遅延値
  const PtDelay*
  new_Delay(const FileRegion& fr,
	    const PtExpr* value1);

  /// @brief 遅延値の生成 (2つの値)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] value1 値1
  /// @param[in] value2 値2
  /// @return 生成された遅延値
  const PtDelay*
  new_Delay(const FileRegion& fr,
	    const PtExpr* value1,
	    const PtExpr* value2);

  /// @brief 遅延値の生成 (3つの値)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] value1 値1
  /// @param[in] value2 値2
  /// @param[in] value3 値3
  /// @return 生成された遅延値
  const PtDelay*
  new_Delay(const FileRegion& fr,
	    const PtExpr* value1,
	    const PtExpr* value2,
	    const PtExpr* value3);

  /// @brief 階層名の生成
  /// @param[in] head_name 階層の上位部分
  /// @param[in] name 階層の最下位部分
  PuHierName*
  new_HierName(const char* head_name,
	       const char* name);

  /// @brief 階層名の生成
  /// @param[in] head_name 階層の上位部分
  /// @param[in] index インデックス
  /// @param[in] name 階層の最下位部分
  PuHierName*
  new_HierName(const char* head_name,
	       int index,
	       const char* name);

  /// @brief 階層名の追加
  /// @aram[in] hname 階層名の上位部分
  /// @param[in] name 追加する名前
  void
  add_HierName(PuHierName* hname,
		 const char* name);

  /// @brief 階層名の追加
  /// @aram[in] hname 階層名の上位部分
  /// @param[in] index インデックス
  /// @param[in] name 追加する名前
  void
  add_HierName(PuHierName* hname,
	       int index,
	       const char* name);

  /// @brief 階層名の取得
  /// @param[in] hname 階層名
  /// @param[out] 階層の上位部分の配列
  /// @return 階層の下位名
  /// @note この関数のなかで hname は削除される．
  const char*
  extract_HierName(PuHierName* hname,
		   PtNameBranchArray& nb_array);


  //////////////////////////////////////////////////////////////////////
  // attribute instance 関係
  //////////////////////////////////////////////////////////////////////

  /// @brief attribute instance の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] as_array attribute spec のリスト
  /// @return 生成された attribute instance
  const PtAttrInst*
  new_AttrInst(const FileRegion& fr,
	       PtrList<const PtAttrSpec>* as_list);

  /// @brief attribute spec の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 名前
  /// @param[in] expr 値
  /// @return 生成された attribute spec
  const PtAttrSpec*
  new_AttrSpec(const FileRegion& fr,
	       const char* name,
	       const PtExpr* expr);


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

  /// @brief ポートリストを配列に変換する．
  PtiPortArray
  get_port_array();

  /// @brief ポート参照リストを初期化する．
  void
  init_portref_list();

  /// @brief ポート参照リストに要素を追加する．
  void
  add_portref(const PtExpr* portref);

  /// @brief parameter port 宣言ヘッダを追加する．
  void
  add_paramport_head(PtiDeclHead* head,
		     PtrList<const PtAttrInst>* attr_list);

  /// @brief parameter port 宣言の終わり
  void
  flush_paramport();

  /// @brief parameter port リストを配列に変換する．
  PtDeclHeadArray
  get_paramport_array();

  /// @brief IOポート宣言リストにIO宣言ヘッダを追加する．
  void
  add_ioport_head(PtiIOHead* head,
		  PtrList<const PtAttrInst>* attr_list);

  /// @brief IO宣言の終わり
  void
  flush_io();

  /// @brief IO宣言リストにIO宣言ヘッダを追加する．
  void
  add_io_head(PtiIOHead* head,
	      PtrList<const PtAttrInst>* attr_list);

  /// @brief IO宣言リストにIO宣言要素を追加する．
  void
  add_io_item(const PtIOItem* item);

  /// @brief module用の IO宣言リストを配列に変換する．
  PtIOHeadArray
  get_module_io_array();

  /// @brief task/function 用の IO宣言リストを配列に変換する．
  PtIOHeadArray
  get_tf_io_array();

  /// @brief module 用の parameter リストを配列に変換する．
  PtDeclHeadArray
  get_module_param_array();

  /// @brief task/function 用の parameter リストを配列に変換する．
  PtDeclHeadArray
  get_tf_param_array();

  /// @brief module 用の localparam リストを配列に変換する．
  PtDeclHeadArray
  get_module_localparam_array();

  /// @brief task/function 用の localparam リストを配列に変換する．
  PtDeclHeadArray
  get_tf_localparam_array();

  /// @brief 宣言リストに宣言ヘッダを追加する．
  void
  add_decl_head(PtiDeclHead* head,
		PtrList<const PtAttrInst>* attr_list);

  /// @brief 宣言リストに宣言要素を追加する．
  void
  add_decl_item(const PtDeclItem* item);

  /// @brief 宣言リストを配列に変換する．
  PtDeclHeadArray
  get_decl_array();

  /// @brief module 用の宣言リストを配列に変換する．
  PtDeclHeadArray
  get_module_decl_array();

  /// @brief task/function 用の宣言リストを配列に変換する．
  PtDeclHeadArray
  get_tf_decl_array();

  /// @brief item リストに要素を追加する．
  void
  add_item(const PtItem* item,
	   PtrList<const PtAttrInst>* attr_list);

  /// @brief item リストを配列に変換する．
  PtItemArray
  get_item_array();

  /// @brief module 用の item リストを配列に変換する．
  PtItemArray
  get_module_item_array();

  /// @brief task/function 用の item リストを配列に変換する．
  PtItemArray
  get_tf_item_array();

  /// @brief UdpEntry を追加する．
  void
  add_udp_entry(const PtUdpEntry* entry);

  /// @brief UdpEntry のリストを配列に変換する．
  PtUdpEntryArray
  get_udp_entry_array();

  /// @brief UdpValue のリストを初期化する．
  void
  init_udp_value_list();

  /// @brief UdpValue を追加する．
  void
  add_udp_value(const PtUdpValue* value);

  /// @brief UdpValue のリストを配列に変換する．
  PtUdpValueArray
  get_udp_value_array();

  /// @brief defparam リストを初期化する．
  void
  init_defparam();

  /// @brief defparam リストに要素を追加する．
  void
  add_defparam(const PtDefParam* defparam);

  /// @brief defparam リストを配列に変換する．
  PtDefParamArray
  get_defparam_array();

  /// @brief contassign リストを初期化する．
  void
  init_contassign();

  /// @brief contassign リストに要素を追加する．
  void
  add_contassign(const PtContAssign* contassign);

  /// @brief contassign リストを配列に変換する．
  PtContAssignArray
  get_contassign_array();

  /// @brief instance リストを初期化する．
  void
  init_inst();

  /// @brief instance リストに要素を追加する．
  void
  add_inst(const PtInst* inst);

  /// @brief instance リストを配列に変換する．
  PtInstArray
  get_inst_array();


public:
  //////////////////////////////////////////////////////////////////////
  // リスト関係
  //////////////////////////////////////////////////////////////////////

  template <typename T>
  PtrList<T, T>*
  new_list();

  template <typename T>
  PtArray<T>
  to_array(PtrList<T, T>* list);

  /// @brief 式のリストから配列を生成する．(multi_concat用)
  /// @param[in] pre_expr list の前に挿入する式
  /// @note 結果として list は削除される．
  PtExprArray
  ExprArray(const PtExpr* pre_expr,
	    PtrList<const PtExpr>* list);


public:

  /// @brief 関数内で使えるステートメントかどうかのチェック
  bool
  check_function_statement(const PtStmt* stmt);

  /// @brief default ラベルが2つ以上含まれていないかどうかのチェック
  bool
  check_default_label(const PtrList<const PtCaseItem>* ci_list);

  /// @brief 使用されているモジュール名を登録する．
  /// @param[in] name 登録する名前
  void
  reg_defname(const char* name);

  /// @brief attribute instance を登録する．
  void
  reg_attrinst(const PtBase* ptobj,
	       PtrList<const PtAttrInst>* attr_list);


public:

  /// @brief lex オブジェクトの取得
  /// @return lex オブジェクトを返す．
  Lex&
  lex();

  /// @brief yylex とのインターフェイス
  /// @param[out] lvalp 値を格納する変数
  /// @param[out] llocp 位置情報を格納する変数
  /// @return 読み込んだトークンの id を返す．
  int
  yylex(YYSTYPE& lval,
	FileRegion& lloc);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ型の定義
  //////////////////////////////////////////////////////////////////////

  typedef PtrList<PtiIOHead, const PtIOHead> PtIOHeadList;
  typedef PtrList<PtiDeclHead, const PtDeclHead> PtDeclHeadList;
  typedef PtrList<const PtItem, const PtItem> PtItemList;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 現在の declhead リストをスタックに積む．
  /// @param[in] new_declhead 新しく設定する declhead
  /// @note new_declhead が nullptr の場合，新たに生成する．
  void
  push_declhead_list(PtDeclHeadList* new_declhead);

  /// @brief スタックの末尾を declhead リストに戻す．
  /// @param[in] delete_top true なら昔の declhead を削除する．
  void
  pop_declhead_list(bool delete_top);

  /// @brief 現在の item リストをスタックに積む．
  /// @param[in] new_item 新しく設定する item リスト
  void
  push_item_list(PtItemList* new_item);

  /// @brief スタックの末尾を item リストに戻す．
  /// @param[in] delete_top true なら昔の declhead を削除する．
  void
  pop_item_list(bool delete_top);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // パース木を保持するクラス
  PtMgr& mPtMgr;

  // 本体のメモリアロケータ(配列確保用)
  Alloc& mAlloc;

  // パース木の要素の生成を行うクラス
  PtiFactory& mFactory;

  // 字句解析を行うオブジェクト
  Lex* mLex;

  // PuList<> のメモリ確保用オブジェクト
  FragAlloc mTmpAlloc;

  // PtrList 用のアロケータ
  UnitAlloc mCellAlloc;


public:
  //////////////////////////////////////////////////////////////////////
  // 静的に存在するリスト
  //////////////////////////////////////////////////////////////////////

  // ポートリスト
  PtrList<PtiPort> mPortList;

  // ポート参照リスト
  PtrList<const PtExpr> mPortRefList;

  // parameter port 宣言ヘッダリスト
  PtDeclHeadList mParamPortHeadList;

  // モジュール用 IO宣言ヘッダリスト
  PtIOHeadList mModuleIOHeadList;

  // task/function 用 IO宣言ヘッダリスト
  PtIOHeadList mTfIOHeadList;

  // モジュール用の宣言ヘッダリスト
  PtDeclHeadList mModuleDeclHeadList;

  // task/function 用の宣言ヘッダリスト
  PtDeclHeadList mTfDeclHeadList;

  // モジュール用の item リスト
  PtItemList mModuleItemList;

  // IO宣言要素リスト
  PtrList<const PtIOItem> mIOItemList;

  // 宣言要素リスト
  PtrList<const PtDeclItem> mDeclItemList;

  // UDP エントリのリスト
  PtrList<const PtUdpEntry> mUdpEntryList;

  // UDP のテーブルの値のリスト
  PtrList<const PtUdpValue> mUdpValueList;

  // defparam 要素のリスト
  PtrList<const PtDefParam> mDefParamList;

  // contassign リスト
  PtrList<const PtContAssign> mContAssignList;

  // instance リスト
  PtrList<const PtInst> mInstList;


public:
  //////////////////////////////////////////////////////////////////////
  // 現在，登録対象となっているリストを指すポインタ
  //////////////////////////////////////////////////////////////////////

  // 現在の IO宣言ヘッダリスト
  // 実際には mModuleIOHeadList か mTfIOHeadList を指す．
  PtIOHeadList* mCurIOHeadList;

  // 現在の宣言ヘッダリスト
  PtDeclHeadList* mCurDeclHeadList;

  // 現在の item リスト
  PtItemList* mCurItemList;

  // 現在の宣言ヘッダの配列
  PtDeclHeadArray mCurDeclArray;

  // 現在の item の配列
  PtItemArray mCurItemArray;

  // generate-if の then 節の宣言ヘッダリスト
  PtDeclHeadArray mGenThenDeclArray;

  // generate-if の then 節の item リスト
  PtItemArray mGenThenItemArray;

  // generate-if の else 節の宣言ヘッダリスト
  PtDeclHeadArray mGenElseDeclArray;

  // generate-if の else 節の item リスト
  PtItemArray mGenElseItemArray;


public:
  //////////////////////////////////////////////////////////////////////
  // mCurXXXList のスタック
  //////////////////////////////////////////////////////////////////////

  // 宣言ヘッダリストのスタック
  vector<PtDeclHeadList*> mDeclHeadListStack;

  // item リストのスタック
  vector<PtItemList*> mItemListStack;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief lex オブジェクトの取得
// @return lex オブジェクトを返す．
inline
Lex&
Parser::lex()
{
  return *mLex;
}

template <typename T>
inline
PtrList<T, T>*
Parser::new_list()
{
  void* p = mTmpAlloc.get_memory(sizeof(PtrList<T, T>));
  return new (p) PtrList<T, T>(mCellAlloc);
}

template <typename T>
inline
PtArray<T>
Parser::to_array(PtrList<T, T>* list)
{
  if ( list ) {
    PtArray<T> array = list->to_array(mAlloc);

    list->~PtrList<T, T>();
    mTmpAlloc.put_memory(sizeof(PtrList<T, T>), list);

    return array;
  }
  else {
    return PtArray<T>();
  }
}

END_NAMESPACE_YM_VERILOG

#endif // PARSER_H
