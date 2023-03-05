#ifndef CPTFACTORY_H
#define CPTFACTORY_H

/// @file CptFactory.h
/// @brief CptFactory のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.

#include "PtiFactory.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class CptFactory CptFactory.h "CptFactory.h"
/// @brief 省メモリ版 Pt クラス(Cpt クラス)を生成するファクトリクラス
//////////////////////////////////////////////////////////////////////
class CptFactory :
  public PtiFactory
{
public:

  /// @brief コンストラクタ
  CptFactory(
    Alloc& alloc ///< [in] メモリアロケータ
  );

  /// @brief デストラクタ
  ~CptFactory();


public:
  //////////////////////////////////////////////////////////////////////
  // UDP 関係
  //////////////////////////////////////////////////////////////////////

  /// @brief combinational UDP の生成
  const PtUdp*
  new_CmbUdp(
    const FileRegion& fr,
    const char* name,
    const vector<const PtPort*>& port_array,
    const vector<const PtIOHead*>& io_array,
    const vector<const PtUdpEntry*>& entry_array
  ) override;

  /// @brief sequential UDP の生成
  const PtUdp*
  new_SeqUdp(
    const FileRegion& fr,
    const char* name,
    const vector<const PtPort*>& port_array,
    const vector<const PtIOHead*>& io_array,
    const PtExpr* init_value,
    const vector<const PtUdpEntry*>& entry_array
  ) override;

  /// @brief combinational UDP 用のテーブルエントリの生成
  const PtUdpEntry*
  new_UdpEntry(
    const FileRegion& fr,
    const vector<const PtUdpValue*>& input_array,
    const PtUdpValue* output
  ) override;

  /// @brief sequential UDP 用のテーブルエントリの生成
  const PtUdpEntry*
  new_UdpEntry(
    const FileRegion& fr,
    const vector<const PtUdpValue*>& input_array,
    const PtUdpValue* current,
    const PtUdpValue* output
  ) override;

  /// @brief UDP のテーブルエントリの要素の値の生成
  const PtUdpValue*
  new_UdpValue(
    const FileRegion& fr,
    char symbol
  ) override;

  /// @brief UDP のテーブルエントリの要素の値の生成
  const PtUdpValue*
  new_UdpValue(
    const FileRegion& fr,
    char symbol1,
    char symbol2
  ) override;


  //////////////////////////////////////////////////////////////////////
  // モジュール関係
  //////////////////////////////////////////////////////////////////////

  /// @brief モジュールの生成
  const PtModule*
  new_Module(
    const FileRegion& fr,
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
    const vector<const PtDeclHead*>& paramport_array,
    const vector<const PtPort*>& port_array,
    const vector<const PtIOHead*>& iohead_array,
    const vector<const PtDeclHead*>& declhead_array,
    const vector<const PtItem*>& item_array
  ) override;


  //////////////////////////////////////////////////////////////////////
  // ポート関係
  //////////////////////////////////////////////////////////////////////

  /// @brief ポートの生成
  PtiPort*
  new_Port(
    const FileRegion& file_region,
    const char* ext_name
  ) override;

  /// @brief ポートの生成 (ポート参照式があるポート)
  PtiPort*
  new_Port(
    const FileRegion& fr,
    const PtExpr* portref,
    const char* ext_name
  ) override;

  /// @brief ポートの生成 (連結型のポート参照式があるポート)
  PtiPort*
  new_Port(
    const FileRegion& fr,
    const PtExpr* portref,
    const vector<const PtExpr*>& port_ref_array,
    const char* ext_name
  ) override;


  //////////////////////////////////////////////////////////////////////
  // IO 宣言関係
  //////////////////////////////////////////////////////////////////////

  /// @brief 範囲付きの IO 宣言のヘッダの生成
  PtiIOHead*
  new_IOHead(
    const FileRegion& fr,
    VpiDir dir,
    bool sign,
    const PtExpr* left,
    const PtExpr* right
  ) override;

  /// @brief 範囲付きの IO 宣言のヘッダの生成 (reg 型)
  PtiIOHead*
  new_RegIOHead(
    const FileRegion& fr,
    VpiDir dir,
    bool sign,
    const PtExpr* left,
    const PtExpr* right
  ) override;

  /// @brief 範囲付きの IO 宣言のヘッダの生成 (ネット型)
  PtiIOHead*
  new_NetIOHead(
    const FileRegion& fr,
    VpiDir dir,
    VpiNetType net_type,
    bool sign,
    const PtExpr* left,
    const PtExpr* right
  ) override;

  /// @brief IO 宣言のヘッダの生成 (変数型)
  PtiIOHead*
  new_VarIOHead(
    const FileRegion& fr,
    VpiDir dir,
    VpiVarType var_type
  ) override;

  /// @brief 初期値付き IO 宣言の要素の生成
  PtIOItem*
  new_IOItem(
    const FileRegion& file_region,
    const char* name,
    const PtExpr* init_value
  ) override;


  //////////////////////////////////////////////////////////////////////
  // その他の宣言関係
  //////////////////////////////////////////////////////////////////////

  /// @brief 範囲指定型パラメータ宣言のヘッダの生成
  PtiDeclHead*
  new_ParamH(
    const FileRegion& fr,
    bool sign,
    const PtExpr* left,
    const PtExpr* right,
    bool local
  ) override;

  /// @brief 組み込み型パラメータ宣言のヘッダの生成
  PtiDeclHead*
  new_ParamH(
    const FileRegion& fr,
    VpiVarType var_type,
    bool local
  ) override;

  /// @brief 範囲指定型 specparam 宣言のヘッダの生成
  PtiDeclHead*
  new_SpecParamH(
    const FileRegion& fr,
    const PtExpr* left,
    const PtExpr* right
  ) override;

  /// @brief イベント宣言のヘッダの生成
  PtiDeclHead*
  new_EventH(
    const FileRegion& fr
  ) override;

  /// @brief genvar 宣言のヘッダの生成
  PtiDeclHead*
  new_GenvarH(
    const FileRegion& fr
  ) override;

  /// @brief 変数宣言のヘッダの生成
  PtiDeclHead*
  new_VarH(
    const FileRegion& fr,
    VpiVarType var_type
  ) override;

  /// @brief ビットベクタ型 reg 宣言のヘッダの生成
  PtiDeclHead*
  new_RegH(
    const FileRegion& fr,
    bool sign,
    const PtExpr* left,
    const PtExpr* right
  ) override;

  /// @brief 範囲指定型 net 宣言のヘッダの生成 (strength, 遅延あり)
  PtiDeclHead*
  new_NetH(
    const FileRegion& fr,
    VpiNetType type,
    VpiVsType vstype,
    bool sign,
    const PtExpr* left,
    const PtExpr* right,
    const PtStrength* strength,
    const PtDelay* delay
  ) override;

  /// @brief 宣言要素の生成
  const PtDeclItem*
  new_DeclItem(
    const FileRegion& fr,
    const char* name
  ) override;

  /// @brief 初期値付き宣言要素の生成
  const PtDeclItem*
  new_DeclItem(
    const FileRegion& fr,
    const char* name,
    const PtExpr* init_value
  ) override;

  /// @brief 配列型宣言要素の生成
  const PtDeclItem*
  new_DeclItem(
    const FileRegion& fr,
    const char* name,
    const vector<const PtRange*>& range_array
  ) override;

  /// @brief 範囲の生成
  const PtRange*
  new_Range(
    const FileRegion& fr,
    const PtExpr* msb,
    const PtExpr* lsb
  ) override;


  //////////////////////////////////////////////////////////////////////
  // item 関係
  //////////////////////////////////////////////////////////////////////

  /// @brief defparam 文のヘッダの生成
  const PtItem*
  new_DefParamH(
    const FileRegion& fr,
    const vector<const PtDefParam*>& elem_array
  ) override;

  /// @brief defparam 文の要素の生成
  const PtDefParam*
  new_DefParam(
    const FileRegion& fr,
    const char* name,
    const PtExpr* value
  ) override;

  /// @brief defparam 文の要素の生成 (階層つき識別子)
  const PtDefParam*
  new_DefParam(
    const FileRegion& fr,
    PuHierName* hname,
    const PtExpr* value
  ) override;

  /// @brief continuous assign 文のヘッダの生成 (strength, 遅延付き)
  const PtItem*
  new_ContAssignH(
    const FileRegion& fr,
    const PtStrength* strength,
    const PtDelay* delay,
    const vector<const PtContAssign*>& elem_array
  ) override;

  /// @brief continuous assign 文の生成
  const PtContAssign*
  new_ContAssign(
    const FileRegion& fr,
    const PtExpr* lhs,
    const PtExpr* rhs
  ) override;

  /// @brief initial 文の生成
  const PtItem*
  new_Initial(
    const FileRegion& fr,
    const PtStmt* body
  ) override;

  /// @brief always 文の生成
  const PtItem*
  new_Always(
    const FileRegion& fr,
    const PtStmt* body
  ) override;

  /// @brief task 文の生成
  const PtItem*
  new_Task(
    const FileRegion& fr,
    const char* name,
    bool automatic,
    const vector<const PtIOHead*>& iohead_array,
    const vector<const PtDeclHead*>& declhead_array,
    const PtStmt* stmt
  ) override;

  /// @brief 1ビット型 function 文の生成
  const PtItem*
  new_Function(
    const FileRegion& fr,
    const char* name,
    bool automatic,
    bool sign,
    const vector<const PtIOHead*>& iohead_array,
    const vector<const PtDeclHead*>& declhead_array,
    const PtStmt* stmt
  ) override;

  /// @brief 範囲指定型 function 文の生成
  const PtItem*
  new_SizedFunc(
    const FileRegion& fr,
    const char* name,
    bool automatic,
    bool sign,
    const PtExpr* left,
    const PtExpr* right,
    const vector<const PtIOHead*>& iohead_array,
    const vector<const PtDeclHead*>& declhead_array,
    const PtStmt* stmt
  ) override;

  /// @brief 組み込み型 function 文の生成
  const PtItem*
  new_TypedFunc(
    const FileRegion& fr,
    const char* name,
    bool automatic,
    bool sign,
    VpiVarType func_type,
    const vector<const PtIOHead*>& iohead_array,
    const vector<const PtDeclHead*>& declhead_array,
    const PtStmt* stmt
  ) override;

  /// @brief gate instance 文のヘッダの生成 (strength, 遅延付き)
  const PtItem*
  new_GateH(
    const FileRegion& fr,
    VpiPrimType type,
    const PtStrength* strength,
    const PtDelay* delay,
    const vector<const PtInst*>& elem_array
  ) override;

  /// @brief module instance/UDP instance 文のヘッダの生成 (strength, 遅延付き)
  const PtItem*
  new_MuH(
    const FileRegion& fr,
    const char* def_name,
    const PtStrength* strength,
    const PtDelay* delay,
    const vector<const PtInst*>& elem_array
  ) override;

  /// @brief module instance/UDP instance 文のヘッダの生成
  const PtItem*
  new_MuH(
    const FileRegion& fr,
    const char* def_name,
    const vector<const PtConnection*>& con_array,
    const vector<const PtInst*>& elem_array
  ) override;

  /// @brief 名前と範囲付き module instance/UDP/gate instance の要素の生成
  const PtInst*
  new_Inst(
    const FileRegion& fr,
    const char* name,
    const PtExpr* left,
    const PtExpr* right,
    const vector<const PtConnection*>& con_array
  ) override;

  /// @brief generate 文の生成
  const PtItem*
  new_Generate(
    const FileRegion& fr,
    const vector<const PtDeclHead*>& declhead_array,
    const vector<const PtItem*>& item_array
  ) override;

  /// @brief generate block 文の生成
  const PtItem*
  new_GenBlock(
    const FileRegion& fr,
    const vector<const PtDeclHead*>& declhead_array,
    const vector<const PtItem*>& item_array
  ) override;

  /// @brief 名前付き generate block 文の生成
  const PtItem*
  new_GenBlock(
    const FileRegion& fr,
    const char* name,
    const vector<const PtDeclHead*>& declhead_array,
    const vector<const PtItem*>& item_array
  ) override;

  /// @brief generate if 文の生成
  const PtItem*
  new_GenIf(
    const FileRegion& fr,
    const PtExpr* cond,
    const vector<const PtDeclHead*>& then_declhead_array,
    const vector<const PtItem*>& then_item_array,
    const vector<const PtDeclHead*>& else_declhead_array,
    const vector<const PtItem*>& else_item_array
  ) override;

  /// @brief generate case 文の生成
  const PtItem*
  new_GenCase(
    const FileRegion& fr,
    const PtExpr* expr,
    const vector<const PtGenCaseItem*>& item_array
  ) override;

  /// @brief generate case の要素の生成
  const PtGenCaseItem*
  new_GenCaseItem(
    const FileRegion& fr,
    const vector<const PtExpr*>& label_array,
    const vector<const PtDeclHead*>& declhead_array,
    const vector<const PtItem*>& item_array
  ) override;

  /// @brief generate for 文の生成
  const PtItem*
  new_GenFor(
    const FileRegion& fr,
    const char* loop_var,
    const PtExpr* init_expr,
    const PtExpr* cond,
    const PtExpr* next_expr,
    const char* block_name,
    const vector<const PtDeclHead*>& declhead_array,
    const vector<const PtItem*>& item_array
  ) override;

  /// @brief specify block item の生成
  const PtItem*
  new_SpecItem(
    const FileRegion& fr,
    VpiSpecItemType id,
    const vector<const PtExpr*>& terminal_top
  ) override;

  /// @brief path 仕様を生成する．
  const PtItem*
  new_SpecPath(
    const FileRegion& fr,
    VpiSpecPathType id,
    const PtExpr* expr,
    const PtPathDecl* path_decl
  ) override;

  /// @brief パス記述の生成
  const PtPathDecl*
  new_PathDecl(
    const FileRegion& fr,
    int edge,
    const vector<const PtExpr*>& input_array,
    int input_pol,
    VpiPathType op,
    const vector<const PtExpr*>& output_array,
    int output_pol,
    const PtExpr* expr,
    const PtPathDelay* path_delay
  ) override;

  /// @brief path delay value の生成 (値が1個)
  const PtPathDelay*
  new_PathDelay(
    const FileRegion& fr,
    const PtExpr* value
  ) override;

  /// @brief path delay value の生成 (値が2個)
  const PtPathDelay*
  new_PathDelay(
    const FileRegion& fr,
    const PtExpr* value1,
    const PtExpr* value2
  ) override;

  /// @brief path delay value の生成 (値が3個)
  const PtPathDelay*
  new_PathDelay(
    const FileRegion& fr,
    const PtExpr* value1,
    const PtExpr* value2,
    const PtExpr* value3
  ) override;

  /// @brief path delay value の生成 (値が6個)
  const PtPathDelay*
  new_PathDelay(
    const FileRegion& fr,
    const PtExpr* value1,
    const PtExpr* value2,
    const PtExpr* value3,
    const PtExpr* value4,
    const PtExpr* value5,
    const PtExpr* value6
  ) override;

  /// @brief path delay value の生成 (値が12個)
  const PtPathDelay*
  new_PathDelay(
    const FileRegion& fr,
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
    const PtExpr* value12
  ) override;


  //////////////////////////////////////////////////////////////////////
  // statement 関係
  //////////////////////////////////////////////////////////////////////

  /// @brief disable 文の生成
  const PtStmt*
  new_Disable(
    const FileRegion& fr,
    const char* name
  ) override;

  /// @brief disable 文の生成 (階層付き識別子)
  const PtStmt*
  new_Disable(
    const FileRegion& fr,
    PuHierName* hname
  ) override;

  /// @brief enable 文の生成
  const PtStmt*
  new_Enable(
    const FileRegion& fr,
    const char* name,
    const vector<const PtExpr*>& arg_array
  ) override;

  /// @brief enable 文の生成 (階層付き識別子)
  const PtStmt*
  new_Enable(
    const FileRegion& fr,
    PuHierName* hname,
    const vector<const PtExpr*>& arg_array
  ) override;

  /// @brief system task enable 文の生成
  const PtStmt*
  new_SysEnable(
    const FileRegion& fr,
    const char* task_name,
    const vector<const PtExpr*>& arg_array
  ) override;

  /// @brief delay control 文の生成
  const PtStmt*
  new_DcStmt(
    const FileRegion& fr,
    const PtControl* delay,
    const PtStmt* body
  ) override;

  /// @brief event control 文の生成
  const PtStmt*
  new_EcStmt(
    const FileRegion& fr,
    const PtControl* event,
    const PtStmt* body
  ) override;

  /// @brief wait 文の生成
  const PtStmt*
  new_Wait(
    const FileRegion& fr,
    const PtExpr* cond,
    const PtStmt* body
  ) override;

  /// @brief assign 文の生成
  const PtStmt*
  new_Assign(
    const FileRegion& fr,
    const PtExpr* lhs,
    const PtExpr* rhs
  ) override;

  /// @brief control 付き assign 文の生成
  const PtStmt*
  new_Assign(
    const FileRegion& fr,
    const PtExpr* lhs,
    const PtExpr* rhs,
    const PtControl* control
  ) override;

  /// @brief nonblocking assign 文の生成
  const PtStmt*
  new_NbAssign(
    const FileRegion& fr,
    const PtExpr* lhs,
    const PtExpr* rhs
  ) override;

  /// @brief control 付き nonblocking assign 文の生成
  const PtStmt*
  new_NbAssign(
    const FileRegion& fr,
    const PtExpr* lhs,
    const PtExpr* rhs,
    const PtControl* control
  ) override;

  /// @brief event 文の生成
  const PtStmt*
  new_EventStmt(
    const FileRegion& fr,
    const PtExpr* event
  ) override;

  /// @brief null 文の生成
  const PtStmt*
  new_NullStmt(
    const FileRegion& fr
  ) override;

  /// @brief if 文の生成
  const PtStmt*
  new_If(
    const FileRegion& fr,
    const PtExpr* expr,
    const PtStmt* then_body
  ) override;

  /// @brief if 文の生成
  const PtStmt*
  new_If(
    const FileRegion& fr,
    const PtExpr* expr,
    const PtStmt* then_body,
    const PtStmt* else_body
  ) override;

  /// @brief case 文の生成
  const PtStmt*
  new_Case(
    const FileRegion& fr,
    const PtExpr* expr,
    const vector<const PtCaseItem*>& caseitem_array
  ) override;

  /// @brief casex 文の生成
  const PtStmt*
  new_CaseX(
    const FileRegion& fr,
    const PtExpr* expr,
    const vector<const PtCaseItem*>& caseitem_array
  ) override;

  /// @brief casez 文の生成
  const PtStmt*
  new_CaseZ(
    const FileRegion& fr,
    const PtExpr* expr,
    const vector<const PtCaseItem*>& caseitem_array
  ) override;

  /// @brief case item の生成
  const PtCaseItem*
  new_CaseItem(
    const FileRegion& fr,
    const vector<const PtExpr*>& label_array,
    const PtStmt* body
  ) override;

  /// @brief forever 文の生成
  const PtStmt*
  new_Forever(
    const FileRegion& fr,
    const PtStmt* body
  ) override;

  /// @brief repeat 文の生成
  const PtStmt*
  new_Repeat(
    const FileRegion& fr,
    const PtExpr* expr,
    const PtStmt* body
  ) override;

  /// @brief while 文の生成
  const PtStmt*
  new_While(
    const FileRegion& fr,
    const PtExpr* cond,
    const PtStmt* body
  ) override;

  /// @brief for 文の生成
  const PtStmt*
  new_For(
    const FileRegion& fr,
    const PtStmt* init,
    const PtExpr* cond,
    const PtStmt* next,
    const PtStmt* body
  ) override;

  /// @brief procedural assign 文の生成
  const PtStmt*
  new_PcAssign(
    const FileRegion& fr,
    const PtExpr* lhs,
    const PtExpr* rhs
  ) override;

  /// @brief deassign 文の生成
  const PtStmt*
  new_Deassign(
    const FileRegion& fr,
    const PtExpr* lhs
  ) override;

  /// @brief force 文の生成
  const PtStmt*
  new_Force(
    const FileRegion& fr,
    const PtExpr* lhs,
    const PtExpr* rhs
  ) override;

  /// @brief release 文の生成
  const PtStmt*
  new_Release(
    const FileRegion& fr,
    const PtExpr* lhs
  ) override;

  /// @brief parallel block の生成
  const PtStmt*
  new_ParBlock(
    const FileRegion& fr,
    const vector<const PtStmt*>& stmt_array
  ) override;

  /// @brief 名前付き parallel block の生成
  const PtStmt*
  new_NamedParBlock(
    const FileRegion& fr,
    const char* name,
    const vector<const PtDeclHead*>& declhead_array,
    const vector<const PtStmt*>& stmt_array
  ) override;

  /// @brief sequential block の生成
  const PtStmt*
  new_SeqBlock(
    const FileRegion& fr,
    const vector<const PtStmt*>& stmt_array
  ) override;

  /// @brief 名前付き sequential block の生成
  const PtStmt*
  new_NamedSeqBlock(
    const FileRegion& fr,
    const char* name,
    const vector<const PtDeclHead*>& declhead_array,
    const vector<const PtStmt*>& stmt_array
  ) override;


  //////////////////////////////////////////////////////////////////////
  // expression 関係
  //////////////////////////////////////////////////////////////////////

  /// @brief 単項演算子の生成
  const PtExpr*
  new_Opr(
    const FileRegion& fr,
    VpiOpType type,
    const PtExpr* opr
  ) override;

  /// @brief 二項演算子の生成
  const PtExpr*
  new_Opr(
    const FileRegion& fr,
    VpiOpType type,
    const PtExpr* opr1,
    const PtExpr* opr2
  ) override;

  /// @brief 三項演算子の生成
  const PtExpr*
  new_Opr(
    const FileRegion& fr,
    VpiOpType type,
    const PtExpr* opr1,
    const PtExpr* opr2,
    const PtExpr* opr3
  ) override;

  /// @brief concatination 演算子の生成
  const PtExpr*
  new_Concat(
    const FileRegion& fr,
    const vector<const PtExpr*>& expr_array
  ) override;

  /// @brief multi-concatination 演算子の生成
  const PtExpr*
  new_MultiConcat(
    const FileRegion& fr,
    const vector<const PtExpr*>& expr_array
  ) override;

  /// @brief min/typ/max delay 演算子の生成
  const PtExpr*
  new_MinTypMax(
    const FileRegion& fr,
    const PtExpr* val0,
    const PtExpr* val1,
    const PtExpr* val2
  ) override;

  /// @brief primary の生成
  const PtExpr*
  new_Primary(
    const FileRegion& fr,
    const char* name
  ) override;

  /// @brief インデックス付き primary の生成
  const PtExpr*
  new_Primary(
    const FileRegion& fr,
    const char* name,
    const vector<const PtExpr*>& index_array
  ) override;

  /// @brief 範囲指定付き primary の生成
  const PtExpr*
  new_Primary(
    const FileRegion& fr,
    const char* name,
    VpiRangeMode mode,
    const PtExpr* left,
    const PtExpr* right
  ) override;

  /// @brief インデックスと範囲指定付き primary の生成
  const PtExpr*
  new_Primary(
    const FileRegion& fr,
    const char* name,
    const vector<const PtExpr*>& index_array,
    VpiRangeMode mode,
    const PtExpr* left,
    const PtExpr* right
  ) override;

  /// @brief primary の生成 (階層付き)
  const PtExpr*
  new_Primary(
    const FileRegion& fr,
    PuHierName* hname
  ) override;

  /// @brief インデックス付き primary の生成 (階層付き)
  const PtExpr*
  new_Primary(
    const FileRegion& fr,
    PuHierName* hname,
    const vector<const PtExpr*>& index_array
  ) override;

  /// @brief 範囲指定付き primary の生成 (階層付き)
  const PtExpr*
  new_Primary(
    const FileRegion& fr,
    PuHierName* hname,
    VpiRangeMode mode,
    const PtExpr* left,
    const PtExpr* right
  ) override;

  /// @brief インデックスと範囲指定付き primary の生成 (階層付き)
  const PtExpr*
  new_Primary(
    const FileRegion& fr,
    PuHierName* hname,
    const vector<const PtExpr*>& index_array,
    VpiRangeMode mode,
    const PtExpr* left,
    const PtExpr* right
  ) override;

  /// @brief constant primary の生成
  const PtExpr*
  new_CPrimary(
    const FileRegion& fr,
    const char* name,
    const vector<const PtExpr*>& index_array
  ) override;

  /// @brief 範囲指定付き constant primary の生成
  const PtExpr*
  new_CPrimary(
    const FileRegion& fr,
    const char* name,
    VpiRangeMode mode,
    const PtExpr* left,
    const PtExpr* right
  ) override;

  /// @brief インデックス付き constant primary の生成 (階層付き)
  const PtExpr*
  new_CPrimary(
    const FileRegion& fr,
    PuHierName* hname,
    const vector<const PtExpr*>& index_array
  ) override;

  /// @brief function call の生成
  const PtExpr*
  new_FuncCall(
    const FileRegion& fr,
    const char* name,
    const vector<const PtExpr*>& arg_array
  ) override;

  /// @brief function call の生成 (階層付き)
  const PtExpr*
  new_FuncCall(
    const FileRegion& fr,
    PuHierName* hname,
    const vector<const PtExpr*>& arg_array
  ) override;

  /// @brief system function call の生成
  const PtExpr*
  new_SysFuncCall(
    const FileRegion& fr,
    const char* name,
    const vector<const PtExpr*>& arg_array
  ) override;

  /// @brief 整数型の定数の生成
  const PtExpr*
  new_IntConst(
    const FileRegion& fr,
    ymuint32 value
  ) override;

  /// @brief 整数型の定数の生成
  const PtExpr*
  new_IntConst(
    const FileRegion& fr,
    const char* value
  ) override;

  /// @brief 基底付き整数型の定数の生成
  const PtExpr*
  new_IntConst(
    const FileRegion& fr,
    VpiConstType const_type,
    const char* value
  ) override;

  /// @brief サイズと基底付き定数の生成
  const PtExpr*
  new_IntConst(
    const FileRegion& fr,
    SizeType size,
    VpiConstType const_type,
    const char* value
  ) override;

  /// @brief 実数型の定数の生成
  const PtExpr*
  new_RealConst(
    const FileRegion& fr,
    double value
  ) override;

  /// @brief 文字列型の定数の生成
  const PtExpr*
  new_StringConst(
    const FileRegion& fr,
    const char* value
  ) override;


  //////////////////////////////////////////////////////////////////////
  // その他
  //////////////////////////////////////////////////////////////////////

  /// @brief ディレイコントロールの生成
  const PtControl*
  new_DelayControl(
    const FileRegion& fr,
    const PtExpr* value
  ) override;

  /// @brief イベントコントロールの生成
  const PtControl*
  new_EventControl(
    const FileRegion& fr,
    const vector<const PtExpr*>& event_array
  ) override;

  /// @brief リピートコントロールの生成
  const PtControl*
  new_RepeatControl(
    const FileRegion& fr,
    const PtExpr* expr,
    const vector<const PtExpr*>& event_array
  ) override;

  /// @brief 順序つき結合子の生成
  const PtConnection*
  new_OrderedCon(
    const FileRegion& fr,
    const PtExpr* expr
  ) override;

  /// @brief 順序つき結合子の生成
  const PtConnection*
  new_OrderedCon(
    const PtExpr* expr
  ) override;

  /// @brief 名前付き結合子の生成
  const PtConnection*
  new_NamedCon(
    const FileRegion& fr,
    const char* name,
    const PtExpr* expr
  ) override;

  /// @brief drive strength の生成
  const PtStrength*
  new_Strength(
    const FileRegion& fr,
    VpiStrength value1,
    VpiStrength value2
  ) override;

  /// @brief charge strength の生成
  const PtStrength*
  new_Strength(
    const FileRegion& fr,
    VpiStrength value
  ) override;

  /// @brief 遅延値の生成 (1つの値)
  const PtDelay*
  new_Delay(
    const FileRegion& fr,
    const PtExpr* value1
  ) override;

  /// @brief 遅延値の生成 (2つの値)
  const PtDelay*
  new_Delay(
    const FileRegion& fr,
    const PtExpr* value1,
    const PtExpr* value2
  ) override;

  /// @brief 遅延値の生成 (3つの値)
  const PtDelay*
  new_Delay(
    const FileRegion& fr,
    const PtExpr* value1,
    const PtExpr* value2,
    const PtExpr* value3
  ) override;

  /// @brief 階層名の生成
  const PtNameBranch*
  new_NameBranch(
    const char* name
  ) override;

  /// @brief インデックス付きの階層名の生成
  const PtNameBranch*
  new_NameBranch(
    const char* name,
    int index
  ) override;


  //////////////////////////////////////////////////////////////////////
  // attribute instance 関係
  //////////////////////////////////////////////////////////////////////

  /// @brief attribute instance の生成
  const PtAttrInst*
  new_AttrInst(
    const FileRegion& fr,
    const vector<const PtAttrSpec*>& as_array
  ) override;

  /// @brief attribute spec の生成
  const PtAttrSpec*
  new_AttrSpec(
    const FileRegion& fr,
    const char* name,
    const PtExpr* expr
  ) override;


public:
  //////////////////////////////////////////////////////////////////////
  // おもにデバッグ用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 確保したオブジェクトに関する情報を出力する．
  void
  dump_profile(
    ostream& s
  ) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 確保したオブジェクトの数
  int mNumDefStub;
  int mNumUdp;
  int mNumUdpEntry;
  int mNumUdpEntryS;
  int mNumUdpValue;
  int mNumModule;
  int mNumPort;
  int mNumPortRef;
  int mNumPortRefI;
  int mNumPortRefR;
  int mNumIOH;
  int mNumIOHV;
  int mNumIOItem;
  int mNumIOItemI;
  int mNumParamH;
  int mNumParamHV;
  int mNumParamHT;
  int mNumLocalParamH;
  int mNumLocalParamHV;
  int mNumLocalParamHT;
  int mNumSpecParamH;
  int mNumSpecParamHV;
  int mNumEventH;
  int mNumGenvarH;
  int mNumVarH;
  int mNumRegH;
  int mNumRegHV;
  int mNumNetH;
  int mNumNetHS;
  int mNumNetHD;
  int mNumNetHSD;
  int mNumNetHV;
  int mNumNetHVS;
  int mNumNetHVD;
  int mNumNetHVSD;
  int mNumDeclItem;
  int mNumDeclItemI;
  int mNumDeclItemR;
  int mNumDefParamH;
  int mNumDefParam;
  int mNumContAssignH;
  int mNumContAssignHS;
  int mNumContAssignHD;
  int mNumContAssignHSD;
  int mNumContAssign;
  int mNumInitial;
  int mNumAlways;
  int mNumTask;
  int mNumFunction;
  int mNumSizedFunc;
  int mNumTypedFunc;
  int mNumGateH;
  int mNumGateHS;
  int mNumGateHD;
  int mNumGateHSD;
  int mNumMuH;
  int mNumMuHS;
  int mNumMuHD;
  int mNumMuHSD;
  int mNumMuHP;
  int mNumInst;
  int mNumInstN;
  int mNumInstR;
  int mNumGenerate;
  int mNumGenBlock;
  int mNumGenBlockN;
  int mNumGenIf;
  int mNumGenCase;
  int mNumGenCaseItem;
  int mNumGenFor;
  int mNumSpecItem;
  int mNumSpecPath;
  int mNumPathDecl;
  int mNumPathDelay;
  int mNumDisable;
  int mNumDisableH;
  int mNumEnable;
  int mNumEnableH;
  int mNumSysEnable;
  int mNumDcStmt;
  int mNumEcStmt;
  int mNumWait;
  int mNumAssign;
  int mNumAssignC;
  int mNumNbAssign;
  int mNumNbAssignC;
  int mNumEventStmt;
  int mNumNullStmt;
  int mNumIf;
  int mNumCase;
  int mNumCaseX;
  int mNumCaseZ;
  int mNumCaseItem;
  int mNumForever;
  int mNumRepeat;
  int mNumWhile;
  int mNumFor;
  int mNumPcAssign;
  int mNumDeassign;
  int mNumForce;
  int mNumRelease;
  int mNumParBlock;
  int mNumParBlockN;
  int mNumSeqBlock;
  int mNumSeqBlockN;
  int mNumOpr1;
  int mNumOpr2;
  int mNumOpr3;
  int mNumConcat;
  int mNumMultiConcat;
  int mNumMinTypMax3;
  int mNumPrimary;
  int mNumPrimaryI;
  int mNumPrimaryR;
  int mNumPrimaryIR;
  int mNumPrimaryH;
  int mNumPrimaryHI;
  int mNumPrimaryHR;
  int mNumPrimaryHIR;
  int mNumPrimaryCI;
  int mNumPrimaryCR;
  int mNumPrimaryHCI;
  int mNumFuncCall;
  int mNumFuncCallH;
  int mNumSysFuncCall;
  int mNumIntConstant1;
  int mNumIntConstant2;
  int mNumIntConstant3;
  int mNumRealConstant;
  int mNumStringConstant;
  int mNumDelayControl;
  int mNumEventControl;
  int mNumRepeatControl;
  int mNumOrderedCon;
  int mNumAiOrderedCon;
  int mNumNamedCon;
  int mNumAiNamedCon;
  int mNumStrength;
  int mNumDelay;
  int mNumNameBranch;
  int mNumNameBranchI;
  int mNumRange;
  int mNumAttrInst;
  int mNumAttrSpec;
};

END_NAMESPACE_YM_VERILOG

#endif // CPTFACTORY_H
