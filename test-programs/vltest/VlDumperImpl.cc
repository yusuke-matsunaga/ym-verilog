
/// @file VlDumperImpl.cc
/// @brief VlDumperImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2009, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "VlDumperImpl.h"

#include "ym/FileRegion.h"
#include "ym/VlMgr.h"
#include "ym/VlValue.h"
#include "ym/vl/VlModule.h"
#include "ym/vl/VlUdp.h"

#include <queue>


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// VlObj の出力用クラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] s 出力ストリーム
VlDumperImpl::VlDumperImpl(ostream& s) :
  mStream(s),
  mIndent(0),
  mFileLocMode(false),
  mNullptrSuppressMode(true),
  mBitExpandMode(false)
{
}

// @brief デストラクタ
VlDumperImpl::~VlDumperImpl()
{
}

// @brief ファイル位置情報を出力するモードにする
void
VlDumperImpl::enable_file_loc_mode()
{
  mFileLocMode = true;
}

// @brief ファイル位置情報の出力を抑制するモードにする．
void
VlDumperImpl::disable_file_loc_mode()
{
  mFileLocMode = false;
}

// @brief ファイル位置情報を出力するモードのとき true を返す．
bool
VlDumperImpl::file_loc_mode() const
{
  return mFileLocMode;
}

// @brief nullptr ポインタの出力を抑止する．
void
VlDumperImpl::enable_nullptr_suppress_mode()
{
  mNullptrSuppressMode = true;
}

// @brief nullptr ポインタの出力を行う．
void
VlDumperImpl::disable_nullptr_suppress_mode()
{
  mNullptrSuppressMode = false;
}

// @brief nullptr ポインタの出力抑止モードの状態を得る．
bool
VlDumperImpl::nullptr_suppress_mode() const
{
  return mNullptrSuppressMode;
}

// @brief bit 展開した内容を出力する．
void
VlDumperImpl::enable_bitexpand_mode()
{
  mBitExpandMode = true;
}

// @brief bit 展開しない．
void
VlDumperImpl::disable_bitexpand_mode()
{
  mBitExpandMode = false;
}

// @brief bit 展開モードの時 true を返す．
bool
VlDumperImpl::bitexpand_mode() const
{
  return mBitExpandMode;
}

// @brief VlMgr の持っている内容を出力する
// @param[in] mgr VlMgr
void
VlDumperImpl::put(const VlMgr& mgr)
{
  // UDP を出力する．
  for ( auto udp: mgr.udp_list() ) {
    put_udp_defn("UDP", mgr, udp);
  }

  // トップモジュールから順にモジュールを出力する．
  std::queue<const VlModule*> tmp_queue;
  for ( auto module: mgr.topmodule_list() ) {
    tmp_queue.push(module);
  }
  while ( !tmp_queue.empty() ) {
    const VlModule* module = tmp_queue.front();
    tmp_queue.pop();
    put_module("MODULE", mgr, module);

    vector<const VlModule*> module_list;
    if ( mgr.find_module_list(module, module_list) ) {
      for ( auto module1: module_list ) {
	tmp_queue.push(module1);
      }
    }
    vector<const VlModuleArray*> modulearray_list;
    if ( mgr.find_modulearray_list(module, modulearray_list) ) {
      for ( auto module_array: modulearray_list ) {
	SizeType n = module_array->elem_num();
	for ( SizeType i = 0; i < n; ++ i ) {
	  const VlModule* module1 = module_array->elem_by_offset(i);
	  tmp_queue.push(module1);
	}
      }
    }
  }
}

#if 0
// @brief VlObj* を出力する関数
// @param[in] label ラベル
// @param[in] obj オブジェクト
void
VlDumperImpl::put(const char* label,
		  const VlMgr& mgr,
		  const VlObj* obj)
{
  if ( obj == nullptr ) {
    if ( !nullptr_suppress_mode() ) {
      VlDumpHeader x(*this, label, "null-pointer");
    }
    return;
  }

  switch ( obj->type() ) {
  case kVpiModule:          put_module(label, mgr, obj); break;
  case kVpiModuleArray:     put_module_array(label, mgr, obj); break;
  case kVpiScope:           put_scope(label, mgr, obj); break;

  case kVpiGateArray:
  case kVpiSwitchArray:
  case kVpiUdpArray:        put_prim_array(label, mgr, obj); break;

  case kVpiNet:             put_decl(label, mgr, obj); break;
  case kVpiNetArray:        put_decl(label, mgr, obj); break;
  case kVpiReg:             put_decl(label, mgr, obj); break;
  case kVpiRegArray:        put_decl(label, mgr, obj); break;
  case kVpiIntegerVar:
  case kVpiRealVar:
  case kVpiTimeVar:
  case kVpiVarSelect:       put_decl(label, mgr, obj); break;
  case kVpiNamedEvent:      put_decl(label, mgr, obj); break;
  case kVpiNamedEventArray: put_decl(label, mgr, obj); break;
  case kVpiParameter:       put_decl(label, mgr, obj); break;
  case kVpiSpecParam:       put_decl(label, mgr, obj); break;
  case kVpiDefParam:        put_def_param(label, mgr, obj); break;
  case kVpiParamAssign:     put_param_assign(label, mgr, obj); break;

  case kVpiRange:           put_range(label, mgr, obj); break;

  case kVpiGate:
  case kVpiSwitch:
  case kVpiUdp:             put_primitive(label, mgr, obj); break;
  case kVpiPrimTerm:        put_prim_term(label, mgr, obj); break;

  case kVpiUdpDefn:         put_udp_defn(label, mgr, obj); break;

  case kVpiTask:            put_task(label, mgr, obj); break;
  case kVpiFunction:        put_function(label, mgr, obj); break;

#if 0
  case kVpiFrame:           put_frame(label, mgr, obj); break;
#endif
  case kVpiContAssign:      put_cont_assign(label, mgr, obj); break;
#if 0
  case kVpiContAssignBit:   put_cont_assign_bit(label, mgr, obj); break;
#endif

  case kVpiInitial:         put_process(label, mgr, obj); break;
  case kVpiAlways:          put_process(label, mgr, obj); break;

  case kVpiBegin:           put_stmt(label, mgr, obj); break;
  case kVpiFork:            put_stmt(label, mgr, obj); break;
  case kVpiNamedBegin:      put_stmt(label, mgr, obj); break;
  case kVpiNamedFork:       put_stmt(label, mgr, obj); break;
  case kVpiNullStmt:        put_stmt(label, mgr, obj); break;
  case kVpiEventStmt:       put_stmt(label, mgr, obj); break;
  case kVpiAssignment:      put_stmt(label, mgr, obj); break;
  case kVpiWhile:           put_stmt(label, mgr, obj); break;
  case kVpiRepeat:          put_stmt(label, mgr, obj); break;
  case kVpiWait:            put_stmt(label, mgr, obj); break;
  case kVpiFor:             put_stmt(label, mgr, obj); break;
  case kVpiForever:         put_stmt(label, mgr, obj); break;
  case kVpiIf:              put_stmt(label, mgr, obj); break;
  case kVpiIfElse:          put_stmt(label, mgr, obj); break;
  case kVpiCase:            put_stmt(label, mgr, obj); break;
  case kVpiForce:           put_stmt(label, mgr, obj); break;
  case kVpiAssignStmt:      put_stmt(label, mgr, obj); break;
  case kVpiDeassign:        put_stmt(label, mgr, obj); break;
  case kVpiRelease:         put_stmt(label, mgr, obj); break;
  case kVpiTaskCall:        put_stmt(label, mgr, obj); break;
  case kVpiSysTaskCall:     put_stmt(label, mgr, obj); break;
  case kVpiDisable:         put_stmt(label, mgr, obj); break;

#if 0
  case kVpiDelayControl:    put_control(label, mgr, obj); break;
  case kVpiEventControl:    put_control(label, mgr, obj); break;
  case kVpiRepeatControl:   put_control(label, mgr, obj); break;
#endif

  case kVpiFuncCall:
  case kVpiSysFuncCall:
  case kVpiBitSelect:
  case kVpiPartSelect:
  case kVpiOperation:
  case kVpiConstant:
    {
      const VlExpr* expr = dynamic_cast<const VlExpr*>(obj);
      ASSERT_COND( expr  );
      put_expr(label, mgr, expr);
    }
    break;

  default: put("illegal type", obj->type() );
  }
}
#endif


//////////////////////////////////////////////////////////////////////
// その他のデータ型の出力
//////////////////////////////////////////////////////////////////////

// @brief VlValue の情報を出力する．
// @param[in] label ラベル
// @param[in] value 値
void
VlDumperImpl::put(const char* label,
		  const VlValue& value)
{
  VlDumpHeader x(this, label, "VlValue");
  switch ( value.type() ) {
  case VlValue::kIntType:
    put("int_value()", value.int_value());
    break;

  case VlValue::kUintType:
    put("uint_value()", value.uint_value());
    break;

  case VlValue::kScalarType:
    put("scalar_value()", value.scalar_value());
    break;

  case VlValue::kRealType:
    put("real_value()", value.real_value());
    break;

  case VlValue::kTimeType:
    put("time_value()", value.time_value());
    break;

  case VlValue::kBitVectorType:
    put("bitvector_value()", value.bitvector_value());
    break;

  case VlValue::kErrorType:
    mStream << "ErrorType";
    break;
  }
}

// @brief FileLoc の情報を出力する．
// @param[in] label ラベル
// @param[in] file_loc ファイルの位置情報
void
VlDumperImpl::put(const char* label,
		  const FileLoc& file_loc)
{
  if ( !file_loc_mode() ) {
    return;
  }

  VlDumpHeader x(this, label, "FileLoc");
  if ( file_loc.is_valid() ) {
    put_parent_file(file_loc);

    VlDumpHeader x(this, "mPosition", "FilePos", false);
    mStream << "name = " << file_loc.filename()
	    << ", line " << file_loc.line()
	    << ", column " << file_loc.column();
  }
  else {
    VlDumpHeader x(this, "mPosition", "FilePos", false);
    mStream << "invalid file_loc";
  }
}

// @brief FileRegion の情報をに出力する．
// @param[in] label ラベル
// @param[in] file_region ファイル領域の情報
void
VlDumperImpl::put(const char* label,
		  const FileRegion& file_region)
{
  if ( !file_loc_mode() ) {
    return;
  }

  VlDumpHeader x1(this, label, "FileRegion");

  const FileLoc& first = file_region.start_loc();
  const FileLoc& last = file_region.end_loc();

  if ( first.file_info() == last.file_info() ) {
    if ( first.is_valid() ) {
      // 同じファイル
      put_parent_file(first);

      VlDumpHeader x(this, "mRegion", "FilePos", false);
      mStream << "name = " << first.filename();
      if ( first.line() == last.line() ) {
	// 同じ行番号
	mStream << ", line = " << first.line();
	if ( first.column() == last.column() ) {
	  // 同じコラム位置
	  mStream << ", column = " << first.column();
	}
	else {
	  // 異なるコラム
	  mStream << ", column = " << first.column()
		  << " - " << last.column();
	}
      }
      else {
	// 異なる行
	mStream << ", line = " << first.line()
		<< ", column = " << first.column()
		<< " - line = " << last.line()
		<< ", column = " << last.column();
      }
    }
    else {
      VlDumpHeader x(this, "mRegion", "FilePos", false);
      mStream << "---";
    }
  }
  else {
    // 異なるファイル
    put("mFirstLoc", first);
    put("mLastLoc", last);
  }
}

// @brief file_loc の親のファイル情報を出力する．
// @param[in] file_loc ファイル位置の情報
void
VlDumperImpl::put_parent_file(const FileLoc& file_loc)
{
  vector<FileLoc> file_list;
  file_loc.parent_loc_list(file_list);
  for ( const auto& loc: file_list ) {
    VlDumpHeader x(this, "mParentFile", "IncFile", false);
    mStream << "name = " << loc.filename()
	    << ", line " << loc.line();
  }
}

// @brief direction を出力する関数
// @param[in] label ラベル
// @param[in] direction 向き
void
VlDumperImpl::put(const char* label,
		  VpiDir direction)
{
  VlDumpHeader x(this, label, "vpiDirection", false);
  mStream << direction;
}

// @brief case type を出力する関数
// @param[in] label ラベル
// @param[in] case_type CASE文の型
void
VlDumperImpl::put(const char* label,
		  VpiCaseType case_type)
{
  VlDumpHeader x(this, label, "vpiCaseType", false);
  switch ( case_type ) {
  case VpiCaseType::Exact:  mStream << "vpiCaseExact"; break;
  case VpiCaseType::X:      mStream << "vpiCaseX"; break;
  case VpiCaseType::Z:      mStream << "vpiCaseZ"; break;
  }
}

// @brief const type を出力する関数
// @param[in] label ラベル
// @param[in] const_type 定数の型
void
VlDumperImpl::put(const char* label,
		  VpiConstType const_type)
{
  VlDumpHeader x(this, label, "vpiConstType", false);
  switch ( const_type ) {
  case VpiConstType::Dec:    mStream << "vpiDecConst"; break;
  case VpiConstType::Real:   mStream << "vpiRealConst"; break;
  case VpiConstType::Binary: mStream << "vpiBinaryConst"; break;
  case VpiConstType::Oct:    mStream << "vpiOctConst"; break;
  case VpiConstType::Hex:    mStream << "vpiHexConst"; break;
  case VpiConstType::String: mStream << "vpiStringConst"; break;
  case VpiConstType::Int:    mStream << "vpiIntConst"; break;
  default:   mStream << "illegal value: " << static_cast<int>(const_type);
  }
}

// @brief prim type を出力する関数
// @param[in] label ラベル
// @param[in] prim_type プリミティブの型
void
VlDumperImpl::put(const char* label,
		  VpiPrimType prim_type)
{
  VlDumpHeader x(this, label, "vpiPrimiType", false);
  switch ( prim_type ) {
  case VpiPrimType::And:      mStream << "vpiAndPrim"; break;
  case VpiPrimType::Nand:     mStream << "vpiNandPrim"; break;
  case VpiPrimType::Or:       mStream << "vpiOrPrim"; break;
  case VpiPrimType::Nor:      mStream << "vpiNorPrim"; break;
  case VpiPrimType::Xor:      mStream << "vpiXorPrim"; break;
  case VpiPrimType::Xnor:     mStream << "vpiXnorPrim"; break;
  case VpiPrimType::Buf:      mStream << "vpiBufPrim"; break;
  case VpiPrimType::Not:      mStream << "vpiNotPrim"; break;
  case VpiPrimType::Bufif0:   mStream << "vpiBufif0Prim"; break;
  case VpiPrimType::Bufif1:   mStream << "vpiBufif1Prim"; break;
  case VpiPrimType::Notif0:   mStream << "vpiNotif0Prim"; break;
  case VpiPrimType::Notif1:   mStream << "vpiNotif1Prim"; break;
  case VpiPrimType::Nmos:     mStream << "vpiNmosPrim"; break;
  case VpiPrimType::Pmos:     mStream << "vpiPmosPrim"; break;
  case VpiPrimType::Cmos:     mStream << "vpiCmosPrim"; break;
  case VpiPrimType::Rnmos:    mStream << "vpiRnmosPrim"; break;
  case VpiPrimType::Rpmos:    mStream << "vpiRpmosPrim"; break;
  case VpiPrimType::Rcmos:    mStream << "vpiRcmosPrim"; break;
  case VpiPrimType::Rtran:    mStream << "vpiRtranPrim"; break;
  case VpiPrimType::Rtranif0: mStream << "vpiRtranif0Prim"; break;
  case VpiPrimType::Rtranif1: mStream << "vpiRtranif1Prim"; break;
  case VpiPrimType::Tran:     mStream << "vpiTranPrim"; break;
  case VpiPrimType::Tranif0:  mStream << "vpiTranif0Prim"; break;
  case VpiPrimType::Tranif1:  mStream << "vpiTranif1Prim"; break;
  case VpiPrimType::Pullup:   mStream << "vpiPullupPrim"; break;
  case VpiPrimType::Pulldown: mStream << "vpiPulldownPrim"; break;
  case VpiPrimType::Seq:      mStream << "vpiSeqPrim"; break;
  case VpiPrimType::Comb:     mStream << "vpiCombPrim"; break;
  default: mStream << "illegal value: " << static_cast<int>(prim_type);
  }
}

// @brief func type の出力
// @param[in] label ラベル
// @param[in] func_type
void
VlDumperImpl::put(const char* label,
		  VpiFuncType func_type)
{
  VlDumpHeader x(this, label, "vpiFuncType", false);
  switch ( func_type ) {
  case VpiFuncType::Int:         mStream << "vpiIntFunc"; break;
  case VpiFuncType::Real:        mStream << "vpiRealFunc"; break;
  case VpiFuncType::Time:        mStream << "vpiTimeFunc"; break;
  case VpiFuncType::Sized:       mStream << "vpiSizedFunc"; break;
  case VpiFuncType::SizedSigned: mStream << "vpiSizedSignedFunc"; break;
  default: mStream << "illegal value: " << static_cast<int>(func_type);
  }
}

// @brief op type の出力
// @param[in] label ラベル
// @param[in] op_type 演算子の種類
void
VlDumperImpl::put(const char* label,
		  VpiOpType op_type)
{
  VlDumpHeader x(this, label, "vpiOpType", false);
  switch ( op_type ) {
  case VpiOpType::Minus:       mStream << "vpiMinusOp"; break;
  case VpiOpType::Plus:        mStream << "vpiPlusOp"; break;
  case VpiOpType::Not:         mStream << "vpiNotOp"; break;
  case VpiOpType::BitNeg:      mStream << "vpiBitNegOp"; break;
  case VpiOpType::UnaryAnd:    mStream << "vpiUnaryAndOp"; break;
  case VpiOpType::UnaryNand:   mStream << "vpiUnaryNandOp"; break;
  case VpiOpType::UnaryOr:     mStream << "vpiUnaryOrOp"; break;
  case VpiOpType::UnaryNor:    mStream << "vpiUnaryNorOp"; break;
  case VpiOpType::UnaryXor:    mStream << "vpiUnaryXorOp"; break;
  case VpiOpType::UnaryXNor:   mStream << "vpiUnaryXNorOp"; break;
  case VpiOpType::Sub:         mStream << "vpiSubOp"; break;
  case VpiOpType::Div:         mStream << "vpiDivOp"; break;
  case VpiOpType::Mod:         mStream << "vpiModOp"; break;
  case VpiOpType::Eq:          mStream << "vpiEqOp"; break;
  case VpiOpType::Neq:         mStream << "vpiNeqOp"; break;
  case VpiOpType::CaseEq:      mStream << "vpiCaseEqOp"; break;
  case VpiOpType::CaseNeq:     mStream << "vpiCaseNeqOp"; break;
  case VpiOpType::Gt:          mStream << "vpiGtOp"; break;
  case VpiOpType::Ge:          mStream << "vpiGeOp"; break;
  case VpiOpType::Lt:          mStream << "vpiLtOp"; break;
  case VpiOpType::Le:          mStream << "vpiLeOp"; break;
  case VpiOpType::LShift:      mStream << "vpiLShiftOp"; break;
  case VpiOpType::RShift:      mStream << "vpiRShiftOp"; break;
  case VpiOpType::Add:         mStream << "vpiAddOp"; break;
  case VpiOpType::Mult:        mStream << "vpiMultOp"; break;
  case VpiOpType::LogAnd:      mStream << "vpiLogAndOp"; break;
  case VpiOpType::LogOr:       mStream << "vpiLogOrOp"; break;
  case VpiOpType::BitAnd:      mStream << "vpiBitAndOp"; break;
  case VpiOpType::BitOr:       mStream << "vpiBitOrOp"; break;
  case VpiOpType::BitXor:      mStream << "vpiBitXorOp"; break;
  case VpiOpType::BitXNor:     mStream << "vpiBitXNorOp"; break;
  case VpiOpType::Condition:   mStream << "vpiConditionOp"; break;
  case VpiOpType::Concat:      mStream << "vpiConcatOp"; break;
  case VpiOpType::MultiConcat: mStream << "vpiMultiConcatOp"; break;
  case VpiOpType::EventOr:     mStream << "vpiEventOrOp"; break;
  case VpiOpType::Null:        mStream << "vpiNullOp"; break;
  case VpiOpType::List:        mStream << "vpiListOp"; break;
  case VpiOpType::MinTypMax:   mStream << "vpiMinTypMaxOp"; break;
  case VpiOpType::Posedge:     mStream << "vpiPosedgeOp"; break;
  case VpiOpType::Negedge:     mStream << "vpiNegedgeOp"; break;
  case VpiOpType::ArithLShift: mStream << "vpiArithLShiftOp"; break;
  case VpiOpType::ArithRShift: mStream << "vpiArithRShiftOp"; break;
  case VpiOpType::Power:       mStream << "vpiPowerOp"; break;
  default: mStream << "illegal value: " << static_cast<int>(op_type); break;
  }
}

// @brief strength を出力する関数
// @param[in] label ラベル
// @param[in] str 信号強度
// @param[in] put_on_null true の時, 空でも出力する．
void
VlDumperImpl::put(const char* label,
		  VpiStrength str,
		  bool put_on_null)
{
  if ( str == VpiStrength::NoStrength && !put_on_null ) {
    return;
  }
  VlDumpHeader x(this, label, "vpiStrength", false);
  switch ( str ) {
  case VpiStrength::NoStrength:   mStream << "none"; break;
  case VpiStrength::SupplyDrive:  mStream << "supply"; break;
  case VpiStrength::StrongDrive:  mStream << "strong"; break;
  case VpiStrength::PullDrive:    mStream << "pull"; break;
  case VpiStrength::WeakDrive:    mStream << "weak"; break;
  case VpiStrength::LargeCharge:  mStream << "large"; break;
  case VpiStrength::MediumCharge: mStream << "medium"; break;
  case VpiStrength::SmallCharge:  mStream << "small"; break;
  case VpiStrength::HiZ:          mStream << "high-Z"; break;
  default: mStream << "illegal strength"; break;
  }
}

// @brief unconnected drive の出力
// @param[in] label ラベル
// @param[in] ud unconnected drive
void
VlDumperImpl::put(const char* label,
		  VpiUnconnDrive ud)
{
  VlDumpHeader x(this, label, "unconn_drive", false);
  switch ( ud ) {
  case VpiUnconnDrive::HighZ: mStream << "high-Z"; break;
  case VpiUnconnDrive::Pull1: mStream << "pull-1"; break;
  case VpiUnconnDrive::Pull0: mStream << "pull-0"; break;
  }
}

// @brief default delay mode の出力
// @param[in] label ラベル
// @param[in] delay_mode default delay mode
void
VlDumperImpl::put(const char* label,
		  VpiDefDelayMode delay_mode)
{
  VlDumpHeader x(this, label, "def_delay_mode", false);
  switch ( delay_mode ) {
  case VpiDefDelayMode::None:    mStream << "none"; break;
  case VpiDefDelayMode::Path:    mStream << "path"; break;
  case VpiDefDelayMode::Distrib: mStream << "distrib"; break;
  case VpiDefDelayMode::Unit:    mStream << "unit"; break;
  case VpiDefDelayMode::Zero:    mStream << "zero"; break;
  case VpiDefDelayMode::MTM:     mStream << "min-typ-max"; break;
  }
}

// @brief ネット型の出力
// @param[in] label ラベル
// @param[in] net_type ネット型
void
VlDumperImpl::put(const char* label,
		  VpiNetType net_type)
{
  VlDumpHeader x(this, label, "net_type", false);
  switch ( net_type ) {
  case VpiNetType::Supply0: mStream << "supply0"; break;
  case VpiNetType::Supply1: mStream << "supply1"; break;
  case VpiNetType::Tri:     mStream << "tri"; break;
  case VpiNetType::TriAnd:  mStream << "triand"; break;
  case VpiNetType::TriOr:   mStream << "trior"; break;
  case VpiNetType::TriReg:  mStream << "trireg"; break;
  case VpiNetType::Tri0:    mStream << "tri0"; break;
  case VpiNetType::Tri1:    mStream << "tri1"; break;
  case VpiNetType::Wire:    mStream << "wire"; break;
  case VpiNetType::Wand:    mStream << "wand"; break;
  case VpiNetType::Wor:     mStream << "wor"; break;
  case VpiNetType::None:    mStream << "none"; break;
  }
}

// @brief ブール値の出力
// @param[in] label ラベル
// @param[in] b ブール値
void
VlDumperImpl::put(const char* label,
		  bool b)
{
  VlDumpHeader x(this, label, "bool", false);
  if ( b ) {
    mStream << "true";
  }
  else {
    mStream << "false";
  }
}

// @brief 整数値の出力
// @param[in] label ラベル
// @param[in] d 整数値
void
VlDumperImpl::put(const char* label,
		  int d)
{
  VlDumpHeader x(this, label, "int", false);
  mStream << d;
}

// @brief 整数値の出力
// @param[in] label ラベル
// @param[in] d 整数値
void
VlDumperImpl::put(const char* label,
		  unsigned int d)
{
  VlDumpHeader x(this, label, "unsigned int", false);
  mStream << d;
}

// @brief 整数値の出力
// @param[in] label ラベル
// @param[in] d 整数値
void
VlDumperImpl::put(const char* label,
		  SizeType d)
{
  VlDumpHeader x(this, label, "SizeType", false);
  mStream << d;
}

// @brief スカラー値の出力
// @param[in] label ラベル
// @param[in] val スカラー値
void
VlDumperImpl::put(const char* label,
		  const VlScalarVal& val)
{
  VlDumpHeader x(this, label, "scalar", false);
  if ( val.is_z() ) {
    mStream << "Z";
  }
  else if ( val.is_zero() ) {
    mStream << "0";
  }
  else if ( val.is_one() ) {
    mStream << "1";
  }
  else if ( val.is_x() ) {
    mStream << "X";
  }
  else {
    mStream << "Unexpected Value";
  }
}

// @brief time値の出力
// @param[in] label ラベル
// @param[in] val time値
void
VlDumperImpl::put(const char* label,
		  VlTime val)
{
  VlDumpHeader x(this, label, "time", false);
  put("high", val.high());
  put("low", val.low());

}

// @brief 実数値の出力
// @param[in] label ラベル
// @param[in] val 値
void
VlDumperImpl::put(const char* label,
		  double val)
{
  VlDumpHeader x(this, label, "real", false);
  mStream << val;
}

// @brief ビットベクタ値の出力
// @param[in] label ラベル
// @param[in] val 値
void
VlDumperImpl::put(const char* label,
		  const BitVector& val)
{
  VlDumpHeader x(this, label, "bitvector", false);
  mStream << val.verilog_string();
}

// @brief 文字列の出力
// @param[in] label ラベル
// @param[in] str 文字列
void
VlDumperImpl::put(const char* label,
		  const string& str)
{
  VlDumpHeader x(this, label, "string", false);
  mStream << str;
}

// @brief 文字列の出力
// @param[in] label ラベル
// @param[in] str 文字列
void
VlDumperImpl::put(const char* label,
		  const char* str)
{
  VlDumpHeader x(this, label, "string", false);
  if ( str ) {
    mStream << str;
  }
  else {
    mStream << "<null-ptr>";
  }
}

// @brief 開始マーカを出力する．
// @param[in] label ラベル
// @param[in] type 属性
// @param[in] need_cr マーカ出力直後に改行するときに true とするフラグ
void
VlDumperImpl::begin_marker(const char* label,
			   const string& type,
			   bool need_cr)
{
  for (int i = 0; i < mIndent; ++ i) {
    mStream << "  ";
  }
  mStream << "<" << label << " type = \"" << type << "\">";
  if ( need_cr ) {
    mStream << endl;
  }
  mDoCR.push_back(need_cr);
  ++ mIndent;
}

// @brief 終了マーカを出力する．
// @param[in] label ラベル
void
VlDumperImpl::end_marker(const char* label)
{
  -- mIndent;
  if ( mDoCR.back() ) {
    for (int i = 0; i < mIndent; ++ i) {
      mStream << "  ";
    }
  }
  mDoCR.pop_back();
  mStream << "</" << label << ">" << endl;
}


//////////////////////////////////////////////////////////////////////
// マーカを出力するクラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] dumper 出力ストリーム
// @param[in] label ラベル
// @param[in] type 属性
// @param[in] need_cr true の時マーカ出力直後に改行する
VlDumpHeader::VlDumpHeader(VlDumperImpl* dumper,
			   const char* label,
			   const char* type,
			   bool need_cr) :
  mDumper(dumper),
  mName(label)
{
  mDumper->begin_marker(mName, type, need_cr);
}

// @brief デストラクタ
VlDumpHeader::~VlDumpHeader()
{
  mDumper->end_marker(mName);
}

END_NAMESPACE_YM_VERILOG
