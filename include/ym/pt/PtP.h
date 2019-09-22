#ifndef YM_PT_PTP_H
#define YM_PT_PTP_H

/// @file ym/pt/PtP.h
/// @brief Pt クラスの名前の宣言のみおこなうヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym/verilog.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// 列挙型の定義
//////////////////////////////////////////////////////////////////////

/// @brief IO の種類を表す列挙型
enum tPtIOType {
  /// @brief 入力
  kPtIO_Input  = 0,
  /// @brief 出力
  kPtIO_Output = 1,
  /// @brief 入出力
  kPtIO_Inout  = 2
};

/// @brief PtDeclH の派生クラスを区別するための列挙型
enum tPtDeclType {
  kPtDecl_Param,      //< @brief parameter
  kPtDecl_LocalParam, //< @brief localparam
  kPtDecl_Reg,        //< @brief reg
  kPtDecl_Var,        //< @brief variables
  kPtDecl_Genvar,     //< @brief genvar
  kPtDecl_Net,        //< @brief net
  kPtDecl_Event,      //< @brief named event
  kPtDecl_SpecParam   //< @brief specparam
};

/// @brief PtItem の派生クラスの識別コード
enum tPtItemType {
  kPtItem_DefParam,
  kPtItem_ContAssign,
  kPtItem_Initial,
  kPtItem_Always,
  kPtItem_Task,
  kPtItem_Func,
  kPtItem_GateInst,
  kPtItem_MuInst,
  kPtItem_SpecItem,
  kPtItem_SpecPath,
  kPtItem_Generate,
  kPtItem_GenBlock,
  kPtItem_GenIf,
  kPtItem_GenCase,
  kPtItem_GenFor
};

/// @brief PtStmt の派生クラスの型を表す列挙型
enum tPtStmtType {
  kPtDisableStmt,
  kPtEnableStmt,
  kPtSysEnableStmt,
  kPtDcStmt,
  kPtEcStmt,
  kPtAssignStmt,
  kPtNbAssignStmt,
  kPtEventStmt,
  kPtNullStmt,
  kPtIfStmt,
  kPtCaseStmt,
  kPtCaseXStmt,
  kPtCaseZStmt,
  kPtWaitStmt,
  kPtForeverStmt,
  kPtRepeatStmt,
  kPtWhileStmt,
  kPtForStmt,
  kPtPcAssignStmt,
  kPtDeassignStmt,
  kPtForceStmt,
  kPtReleaseStmt,
  kPtParBlockStmt,
  kPtSeqBlockStmt,
  kPtNamedParBlockStmt,
  kPtNamedSeqBlockStmt
};

/// @brief PtExpr の派生クラスを識別するための列挙型
enum tPtExprType {
  /// @brief 演算子
  kPtOprExpr,
  /// @brief 関数呼び出し
  kPtFuncCallExpr,
  /// @brief システム関数呼び出し
  kPtSysFuncCallExpr,
  /// @brief プライマリ
  kPtPrimaryExpr,
  /// @brief 定数
  kPtConstExpr
};

/// @brief コントロールの型を区別するための列挙型
enum tPtCtrlType {
  /// @brief delay control
  kPtDelayControl,
  /// @brief event control
  kPtEventControl,
  /// @brief repeat control
  kPtRepeatControl
};


//////////////////////////////////////////////////////////////////////
// クラス名の前方宣言
//////////////////////////////////////////////////////////////////////

// in PtBase.h
class PtBase;
class PtNamedBase;

// in PtUdp.h
class PtUdp;
class PtUdpEntry;
class PtUdpValue;

// in PtModule.h
class PtModule;

// in PtPort.h
class PtPort;

// in PtDecl.h
class PtIOHead;
class PtIOItem;
class PtDeclHead;
class PtDeclItem;
class PtRange;

// in PtItem.h
class PtItem;
class PtDefParam;
class PtContAssign;
class PtInst;
class PtGenCaseItem;
class PtPathDecl;
class PtPathDelay;

// in PtStmt.h
class PtStmt;
class PtCaseItem;

// in PtExpr.h
class PtExpr;

// in PtMisc.h
class PtStrength;
class PtDelay;
class PtControl;
class PtConnection;
class PtNameBranch;
class PtAttrInst;
class PtAttrSpec;

// in PtArray.h
template <typename T>
class PtArray;

using PtAttrInstArray = PtArray<const PtAttrInst>;
using PtAttrSpecArray = PtArray<const PtAttrSpec>;
using PtCaseItemArray = PtArray<const PtCaseItem>;
using PtConnectionArray = PtArray<const PtConnection>;
using PtContAssignArray = PtArray<const PtContAssign>;
using PtDeclHeadArray = PtArray<const PtDeclHead>;
using PtDeclItemArray = PtArray<const PtDeclItem>;
using PtDefParamArray = PtArray<const PtDefParam>;
using PtExprArray = PtArray<const PtExpr>;
using PtGenCaseItemArray = PtArray<const PtGenCaseItem>;
using PtInstArray = PtArray<const PtInst>;
using PtIOHeadArray = PtArray<const PtIOHead>;
using PtIOItemArray = PtArray<const PtIOItem>;
using PtItemArray = PtArray<const PtItem>;
using PtNameBranchArray = PtArray<const PtNameBranch>;
using PtRangeArray = PtArray<const PtRange>;
using PtPortArray = PtArray<const PtPort>;
using PtStmtArray = PtArray<const PtStmt>;
using PtUdpEntryArray = PtArray<const PtUdpEntry>;
using PtUdpValueArray = PtArray<const PtUdpValue>;

END_NAMESPACE_YM_VERILOG

BEGIN_NAMESPACE_STD

// PtModule へのポインタをキーにしたハッシュ関数クラスの定義
template <>
struct hash<YM_NAMESPACE::nsVerilog::PtModule*>
{
  SizeType
  operator()(YM_NAMESPACE::nsVerilog::PtModule* module) const
  {
    ympuint tmp = reinterpret_cast<ympuint>(module)/sizeof(void*);
    return static_cast<SizeType>(tmp);
  }
};

// const PtModule へのポインタをキーにしたハッシュ関数クラスの定義
template <>
struct hash<const YM_NAMESPACE::nsVerilog::PtModule*>
{
  SizeType
  operator()(const YM_NAMESPACE::nsVerilog::PtModule* module) const
  {
    ympuint tmp = reinterpret_cast<ympuint>(module)/sizeof(void*);
    return static_cast<SizeType>(tmp);
  }
};

END_NAMESPACE_STD

#endif // YM_PT_PTP_H
