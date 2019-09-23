#ifndef YM_VERILOG_H
#define YM_VERILOG_H

/// @file ym/verilog.h
/// @brief ym-verilog に共通な定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2016 Yusuke Matsunaga
/// All rights reserved.

/// @namespace nsYm::nsVerilog
/// @brief Verilog パーサ関係のクラスが属する名前空間
///
/// ただし，主要なものは nsYm にインポートされている．
/// 実装用のローカルな要素だけがこの名前空間に存在する．

/// @defgroup ym-verilog Verilog パッケージ
///
/// Verilog-HDL(IEEE1364, IEEE1364-2001) フォーマットの
/// フロントエンド


#include "ym_config.h"
#include "vpi_user.h"

/// @brief ym_verilog 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_VERILOG \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(nsVerilog)

/// @brief ym_verilog 用の名前空間の終了
#define END_NAMESPACE_YM_VERILOG \
END_NAMESPACE(nsVerilog) \
END_NAMESPACE_YM


BEGIN_NAMESPACE_YM_VERILOG

class BitVector;
class VlLineWatcher;
class VlMgr;
class VlScalarVal;
class VlTime;
class VlUdpVal;
class VlValue;
class VlValueType;

END_NAMESPACE_YM_VERILOG

BEGIN_NAMESPACE_YM

/// @defgroup VlCommon Verilog 用基本ユーティリティ
/// @ingroup ym-verilog
/// @{

//////////////////////////////////////////////////////////////////////
// 列挙型の定義
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
/// @brief オブジェクトの型
/// @note この列挙型の値を強制的に int にキャストしても
/// vpi_user.h の定義値として使える．
//////////////////////////////////////////////////////////////////////
enum class VpiObjType {
  Always          = vpiAlways,
  AssignStmt      = vpiAssignStmt,
  Assignment      = vpiAssignment,
  Begin           = vpiBegin,
  Case            = vpiCase,
  CaseItem        = vpiCaseItem,
  Constant        = vpiConstant,
  ContAssign      = vpiContAssign,
  Deassign        = vpiDeassign,
  DefParam        = vpiDefParam,
  DelayControl    = vpiDelayControl,
  Disable         = vpiDisable,
  EventControl    = vpiEventControl,
  EventStmt       = vpiEventStmt,
  For             = vpiFor,
  Force           = vpiForce,
  Forever         = vpiForever,
  Fork            = vpiFork,
  FuncCall        = vpiFuncCall,
  Function        = vpiFunction,
  Gate            = vpiGate,
  If              = vpiIf,
  IfElse          = vpiIfElse,
  Initial         = vpiInitial,
  IntegerVar      = vpiIntegerVar,
  InterModPath    = vpiInterModPath,
  Iterator        = vpiIterator,
  IODecl          = vpiIODecl,
  Memory          = vpiMemory,
  MemoryWord      = vpiMemoryWord,
  ModPath         = vpiModPath,
  Module          = vpiModule,
  NamedBegin      = vpiNamedBegin,
  NamedEvent      = vpiNamedEvent,
  NamedFork       = vpiNamedFork,
  Net             = vpiNet,
  NetBit          = vpiNetBit,
  NullStmt        = vpiNullStmt,
  Operation       = vpiOperation,
  ParamAssign     = vpiParamAssign,
  Parameter       = vpiParameter,
  PartSelect      = vpiPartSelect,
  PathTerm        = vpiPathTerm,
  Port            = vpiPort,
  PortBit         = vpiPortBit,
  PrimTerm        = vpiPrimTerm,
  RealVar         = vpiRealVar,
  Reg             = vpiReg,
  RegBit          = vpiRegBit,
  Release         = vpiRelease,
  Repeat          = vpiRepeat,
  RepeatControl   = vpiRepeatControl,
  SchedEvent      = vpiSchedEvent,
  SpecParam       = vpiSpecParam,
  Switch          = vpiSwitch,
  SysFuncCall     = vpiSysFuncCall,
  SysTaskCall     = vpiSysTaskCall,
  TableEntry      = vpiTableEntry,
  Task            = vpiTask,
  TaskCall        = vpiTaskCall,
  Tchk            = vpiTchk,
  TchkTerm        = vpiTchkTerm,
  TimeVar         = vpiTimeVar,
  TimeQueue       = vpiTimeQueue,
  Udp             = vpiUdp,
  UdpDefn         = vpiUdpDefn,
  UserSystf       = vpiUserSystf,
  VarSelect       = vpiVarSelect,
  Wait            = vpiWait,
  While           = vpiWhile,
  Attribute       = vpiAttribute,
  BitSelect       = vpiBitSelect,
  Callback        = vpiCallback,
  DelayTerm       = vpiDelayTerm,
  DelayDevice     = vpiDelayDevice,
  Frame           = vpiFrame,
  GateArray       = vpiGateArray,
  ModuleArray     = vpiModuleArray,
  PrimitiveArray  = vpiPrimitiveArray,
  NetArray        = vpiNetArray,
  Range           = vpiRange,
  RegArray        = vpiRegArray,
  SwitchArray     = vpiSwitchArray,
  UdpArray        = vpiUdpArray,
  ContAssignBit   = vpiContAssignBit,
  NamedEventArray = vpiNamedEventArray,
  Scope           = vpiScope
};


//////////////////////////////////////////////////////////////////////
/// @brief unconnected port drive の値
/// @note この列挙型の値を強制的に int にキャストしても
/// vpi_user.h の定義値として使える．
//////////////////////////////////////////////////////////////////////
enum class VpiUnconnDrive {
  HighZ = vpiHighZ,
  Pull1 = vpiPull1,
  Pull0 = vpiPull0
};


//////////////////////////////////////////////////////////////////////
/// @brief default delay Mode の値
/// @note この列挙型の値を強制的に int にキャストしても
/// vpi_user.h の定義値として使える．
//////////////////////////////////////////////////////////////////////
enum class VpiDefDelayMode {
  None    = vpiDelayModeNone,
  Path    = vpiDelayModePath,
  Distrib = vpiDelayModeDistrib,
  Unit    = vpiDelayModeUnit,
  Zero    = vpiDelayModeZero,
  MTM     = vpiDelayModeMTM
};


//////////////////////////////////////////////////////////////////////
/// @brief direction の値
/// @note この列挙型の値を強制的に int にキャストしても
/// vpi_user.h の定義値として使える．
//////////////////////////////////////////////////////////////////////
enum class VpiDir {
  Input       = vpiInput,
  Output      = vpiOutput,
  Inout       = vpiInout,
  MixedIO     = vpiMixedIO,
  NoDirection = vpiNoDirection
};

/// @brief VpiDir のストリーム出力
ostream&
operator<<(ostream& s,
	   VpiDir dir);


//////////////////////////////////////////////////////////////////////
/// @brief net の型
/// @note この列挙型の値を強制的に int にキャストしても
/// vpi_user.h の定義値として使える．
//////////////////////////////////////////////////////////////////////
enum class VpiNetType {
  Wire     = vpiWire,
  Wand     = vpiWand,
  Wor      = vpiWor,
  Tri      = vpiTri,
  Tri0     = vpiTri0,
  Tri1     = vpiTri1,
  TriReg   = vpiTriReg,
  TriAnd   = vpiTriAnd,
  TriOr    = vpiTriOr,
  Supply1  = vpiSupply1,
  Supply0  = vpiSupply0,
  None     = vpiNone
};


//////////////////////////////////////////////////////////////////////
/// @brief primitive の型
/// @note この列挙型の値を強制的に int にキャストしても
/// vpi_user.h の定義値として使える．
//////////////////////////////////////////////////////////////////////
enum class VpiPrimType {
  And       = vpiAndPrim,
  Nand      = vpiNandPrim,
  Nor       = vpiNorPrim,
  Or        = vpiOrPrim,
  Xor       = vpiXorPrim,
  Xnor      = vpiXnorPrim,
  Buf       = vpiBufPrim,
  Not       = vpiNotPrim,
  Bufif0    = vpiBufif0Prim,
  Bufif1    = vpiBufif1Prim,
  Notif0    = vpiNotif0Prim,
  Notif1    = vpiNotif1Prim,
  Nmos      = vpiNmosPrim,
  Pmos      = vpiPmosPrim,
  Cmos      = vpiCmosPrim,
  Rnmos     = vpiRnmosPrim,
  Rpmos     = vpiRpmosPrim,
  Rcmos     = vpiRcmosPrim,
  Rtran     = vpiRtranPrim,
  Rtranif0  = vpiRtranif0Prim,
  Rtranif1  = vpiRtranif1Prim,
  Tran      = vpiTranPrim,
  Tranif0   = vpiTranif0Prim,
  Tranif1   = vpiTranif1Prim,
  Pullup    = vpiPullupPrim,
  Pulldown  = vpiPulldownPrim,
  Seq       = vpiSeqPrim,
  Comb      = vpiCombPrim,
  // この値だけ vpi_user.h にはない
  // 値の重複に注意すること．
  Cell      = 29
};


//////////////////////////////////////////////////////////////////////
/// @brief module path/data path の極性
/// @note この列挙型の値を強制的に int にキャストしても
/// vpi_user.h の定義値として使える．
//////////////////////////////////////////////////////////////////////
enum class VpiPolarity {
  Positive = vpiPositive,
  Negative = vpiNegative,
  Unknown  = vpiUnknown
};


//////////////////////////////////////////////////////////////////////
/// @brief module path の edge type
/// @note この列挙型の値を強制的に int にキャストしても
/// vpi_user.h の定義値として使える．
//////////////////////////////////////////////////////////////////////
enum class VpiEdge {
  NoEdge  = vpiNoEdge,
  Edge01  = vpiEdge01,
  Edge10  = vpiEdge10,
  Edge0x  = vpiEdge0x,
  Edgex1  = vpiEdgex1,
  Edge1x  = vpiEdge1x,
  Edgex0  = vpiEdgex0,
  Posedge = vpiPosedge, // = (kVpiEdgex1 | kVpiEdge01 | kVpiEdge0x)
  Negedge = vpiNegedge, // = (kVpiEdgex0 | kVpiEdge10 | kVpiEdge1x)
  AnyEdge = vpiAnyEdge  // = (kVpiPosedge | kVpiNegedge)
};


//////////////////////////////////////////////////////////////////////
/// @brief path delay connection の型
/// @note この列挙型の値を強制的に int にキャストしても
/// vpi_user.h の定義値として使える．
//////////////////////////////////////////////////////////////////////
enum class VpiPathType {
  Full     = vpiPathFull,
  Parallel = vpiPathParallel
};


//////////////////////////////////////////////////////////////////////
/// @brief timing check の型
/// @note この列挙型の値を強制的に int にキャストしても
/// vpi_user.h の定義値として使える．
//////////////////////////////////////////////////////////////////////
enum class VpiTchkType {
  Setup      = vpiSetup,
  Hold       = vpiHold,
  Period     = vpiPeriod,
  Width      = vpiWidth,
  Skew       = vpiSkew,
  Recovery   = vpiRecovery,
  NoChange   = vpiNoChange,
  SetupHold  = vpiSetupHold,
  Fullskew   = vpiFullskew,
  Recrem     = vpiRecrem,
  Removal    = vpiRemoval,
  Timeskew   = vpiTimeskew
};


//////////////////////////////////////////////////////////////////////
/// @brief operation の型
/// @note この列挙型の値を強制的に int にキャストしても
/// vpi_user.h の定義値として使える．
//////////////////////////////////////////////////////////////////////
enum class VpiOpType {
  Minus        = vpiMinusOp,
  Plus         = vpiPlusOp,
  Not          = vpiNotOp,
  BitNeg       = vpiBitNegOp,
  UnaryAnd     = vpiUnaryAndOp,
  UnaryNand    = vpiUnaryNandOp,
  UnaryOr      = vpiUnaryOrOp,
  UnaryNor     = vpiUnaryNorOp,
  UnaryXor     = vpiUnaryXorOp,
  UnaryXNor    = vpiUnaryXNorOp,
  Sub          = vpiSubOp,
  Div          = vpiDivOp,
  Mod          = vpiModOp,
  Eq           = vpiEqOp,
  Neq          = vpiNeqOp,
  CaseEq       = vpiCaseEqOp,
  CaseNeq      = vpiCaseNeqOp,
  Gt           = vpiGtOp,
  Ge           = vpiGeOp,
  Lt           = vpiLtOp,
  Le           = vpiLeOp,
  LShift       = vpiLShiftOp,
  RShift       = vpiRShiftOp,
  Add          = vpiAddOp,
  Mult         = vpiMultOp,
  LogAnd       = vpiLogAndOp,
  LogOr        = vpiLogOrOp,
  BitAnd       = vpiBitAndOp,
  BitOr        = vpiBitOrOp,
  BitXor       = vpiBitXorOp,
  BitXNor      = vpiBitXNorOp,
  Condition    = vpiConditionOp,
  Concat       = vpiConcatOp,
  MultiConcat  = vpiMultiConcatOp,
  EventOr      = vpiEventOrOp,
  Null         = vpiNullOp,
  List         = vpiListOp,
  MinTypMax    = vpiMinTypMaxOp,
  Posedge      = vpiPosedgeOp,
  Negedge      = vpiNegedgeOp,
  ArithLShift  = vpiArithLShiftOp,
  ArithRShift  = vpiArithRShiftOp,
  Power        = vpiPowerOp
};


//////////////////////////////////////////////////////////////////////
/// @brief constant の型
/// @note この列挙型の値を強制的に int にキャストしても
/// vpi_user.h の定義値として使える．
/// ただし kVpiSignedXXXXX はない．
//////////////////////////////////////////////////////////////////////
enum class VpiConstType {
  Dec          = vpiDecConst,
  Real         = vpiRealConst,
  Binary       = vpiBinaryConst,
  Oct          = vpiOctConst,
  Hex          = vpiHexConst,
  String       = vpiStringConst,
  Int          = vpiIntConst,
  SignedDec    = vpiDecConst    | 8,
  SignedBinary = vpiBinaryConst | 8,
  SignedOct    = vpiOctConst    | 8,
  SignedHex    = vpiHexConst    | 8
};


//////////////////////////////////////////////////////////////////////
/// @brief case の型
/// @note この列挙型の値を強制的に int にキャストしても
/// vpi_user.h の定義値として使える．
//////////////////////////////////////////////////////////////////////
enum class VpiCaseType {
  Exact = vpiCaseExact,
  X     = vpiCaseX,
  Z     = vpiCaseZ
};


//////////////////////////////////////////////////////////////////////
/// @brief function の型
/// @note この列挙型の値を強制的に int にキャストしても
/// vpi_user.h の定義値として使える．
/// ただし kVpiRealtimeFunc はない．
//////////////////////////////////////////////////////////////////////
enum class VpiFuncType {
  Int         = vpiIntFunc,
  Real        = vpiRealFunc,
  Time        = vpiTimeFunc,
  Sized       = vpiSizedFunc,
  SizedSigned = vpiSizedSignedFunc,
  Realtime    = 6,
  SysInt      = vpiSysFuncInt,
  SysReal     = vpiSysFuncReal,
  SysTime     = vpiSysFuncTime,
  SysSized    = vpiSysFuncSized
};


//////////////////////////////////////////////////////////////////////
/// @brief delay の型
/// @note この列挙型の値を強制的に int にキャストしても
/// vpi_user.h の定義値として使える．
//////////////////////////////////////////////////////////////////////
enum class VpiDelayType {
  ModPath      = vpiModPathDelay,
  InterModPath = vpiInterModPathDelay,
  MIP          = vpiMIPDelay
};


//////////////////////////////////////////////////////////////////////
/// @brief strength の値
//////////////////////////////////////////////////////////////////////
enum class VpiStrength {
  NoStrength   = 0,
  SupplyDrive  = 1,
  StrongDrive  = 2,
  PullDrive    = 3,
  WeakDrive    = 4,
  LargeCharge  = 5,
  MediumCharge = 6,
  SmallCharge  = 7,
  HiZ          = 8
};


//////////////////////////////////////////////////////////////////////
/// @brief 変数のデータ型
//////////////////////////////////////////////////////////////////////
enum class VpiVarType {
  None     = 0,
  Integer  = 1,
  Real     = 2,
  Time     = 3,
  Realtime = 4
};


//////////////////////////////////////////////////////////////////////
/// @brief 範囲指定のモード
//////////////////////////////////////////////////////////////////////
enum class VpiRangeMode {
  /// @brief 範囲指定なし
  No    = 0,
  /// @brief 固定
  Const = 1, // [ a : b ]
  /// @brief 可変(プラス)
  Plus  = 2, // [ a +: b ]
  /// @brief 可変(マイナス)
  Minus = 3  // [ a -: b ]
};


//////////////////////////////////////////////////////////////////////
/// @brief IO宣言で用いられる補助的な型
//////////////////////////////////////////////////////////////////////
enum class VpiAuxType {
  /// @brief 型無し
  None = 0,
  /// @brief net 型
  Net  = 1,
  /// @brief reg 型
  Reg  = 2,
  /// @brief 変数型
  Var  = 3
};


//////////////////////////////////////////////////////////////////////
/// @brief vectored/scalared 指定の値
//////////////////////////////////////////////////////////////////////
enum class VpiVsType {
  /// @brief 指定なし
  None     = 0,
  /// @brief ベクター型
  Vectored = 1,
  /// @brief スカラー型
  Scalared = 2
};


//////////////////////////////////////////////////////////////////////
/// @brief specify block item の種類
//////////////////////////////////////////////////////////////////////
enum class VpiSpecItemType {
  PulsestyleOnEvent  = 0,
  PulsestyleOnDetect = 1,
  Showcancelled      = 2,
  Noshowcancelled    = 3
};


//////////////////////////////////////////////////////////////////////
/// @brief specify block path の種類
//////////////////////////////////////////////////////////////////////
enum class VpiSpecPathType {
  Null   = 0,
  If     = 1,
  Ifnone = 2
};

/// @}

//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief VpiDir のストリーム出力
inline
ostream&
operator<<(ostream& s,
	   VpiDir dir)
{
  switch ( dir ) {
  case VpiDir::Input:       s << "Input"; break;
  case VpiDir::Output:      s << "Output"; break;
  case VpiDir::Inout:       s << "Inout"; break;
  case VpiDir::MixedIO:     s << "Mixed IO"; break;
  case VpiDir::NoDirection: s << "No Direction"; break;
  }
  return s;
}


using nsVerilog::BitVector;
using nsVerilog::VlLineWatcher;
using nsVerilog::VlMgr;
using nsVerilog::VlScalarVal;
using nsVerilog::VlTime;
using nsVerilog::VlUdpVal;
using nsVerilog::VlValueType;
using nsVerilog::VlValue;

END_NAMESPACE_YM

#endif // YM_VERILOG_H
