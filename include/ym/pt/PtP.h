#ifndef YM_PT_PTP_H
#define YM_PT_PTP_H

/// @file ym/pt/PtP.h
/// @brief Pt クラスの名前の宣言のみおこなうヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.


#include "ym/verilog.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// 列挙型の定義
//////////////////////////////////////////////////////////////////////

/// @brief PtDeclH の派生クラスを区別するための列挙型
enum class PtDeclType {
  Param,      //< @brief parameter
  LocalParam, //< @brief localparam
  Reg,        //< @brief reg
  Var,        //< @brief variables
  Genvar,     //< @brief genvar
  Net,        //< @brief net
  Event,      //< @brief named event
  SpecParam   //< @brief specparam
};

/// @brief PtItem の派生クラスの識別コード
enum class PtItemType {
  DefParam,
  ContAssign,
  Initial,
  Always,
  Task,
  Func,
  GateInst,
  MuInst,
  SpecItem,
  SpecPath,
  Generate,
  GenBlock,
  GenIf,
  GenCase,
  GenFor
};

/// @brief PtStmt の派生クラスの型を表す列挙型
enum class PtStmtType {
  Disable,
  Enable,
  SysEnable,
  DelayControl,
  EventControl,
  Assign,
  NbAssign,
  Event,
  Null,
  If,
  Case,
  CaseX,
  CaseZ,
  Wait,
  Forever,
  Repeat,
  White,
  For,
  PcAssign,
  Deassign,
  Force,
  Release,
  ParBlock,
  SeqBlock,
  NamedParBlock,
  NamedSeqBlock
};

/// @brief PtExpr の派生クラスを識別するための列挙型
enum class PtExprType {
  /// @brief 演算子
  Opr,
  /// @brief 関数呼び出し
  FuncCall,
  /// @brief システム関数呼び出し
  SysFuncCall,
  /// @brief プライマリ
  Primary,
  /// @brief 定数
  Const
};

/// @brief コントロールの型を区別するための列挙型
enum class PtCtrlType {
  /// @brief delay control
  Delay,
  /// @brief event control
  Event,
  /// @brief repeat control
  Repeat
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
