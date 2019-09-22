﻿
/// @file PtDumper.cc
/// @brief PtDumper の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "PtDumper.h"

#include "ym/pt/PtModule.h"
#include "ym/pt/PtUdp.h"
#include "ym/pt/PtPort.h"
#include "ym/pt/PtDecl.h"
#include "ym/pt/PtItem.h"
#include "ym/pt/PtStmt.h"
#include "ym/pt/PtExpr.h"
#include "ym/pt/PtMisc.h"
#include "ym/pt/PtArray.h"
#include "ym/VlUdpVal.h"

#include "ym/StrBuff.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class PtHeader PtDumper.cc "PtDumper.cc"
/// @ingroup VlParser
/// @ingroup PtGroup
/// @brief マーカを出力するクラス
//////////////////////////////////////////////////////////////////////
class PtHeader
{
public:

  /// @brief コンストラクタ
  /// @param[in] s 出力ストリーム
  /// @param[in] name 名前
  /// @param[in] type 属性
  /// @param[in] need_cr マーカの直後に改行するとき true とするフラグ
  PtHeader(PtDumper& s,
	   const char* name,
	   const char* type,
	   bool need_cr = true);

  /// @brief デストラクタ
  ~PtHeader();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 出力先のストリーム
  PtDumper& mS;

  // マーカー名
  StrBuff mName;

};


//////////////////////////////////////////////////////////////////////
// Pt クラスの内容を出力するクラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] s 出力先のストリーム
PtDumper::PtDumper(ostream& s) :
  mStream(s),
  mIndent(0),
  mFileLocMode(false)
{
}

// @brief デストラクタ
PtDumper::~PtDumper()
{
}

// @brief ファイル位置情報を出力するモードににする
void
PtDumper::enable_file_loc_mode()
{
  mFileLocMode = true;
}

// @brief ファイル位置情報の出力を抑制するモードにする
void
PtDumper::disable_file_loc_mode()
{
  mFileLocMode = false;
}

// @brief ファイル位置情報の出力モードの取得
// @return ファイル位置情報を出力するモードのとき true を返す．
bool
PtDumper::file_loc_mode() const
{
  return mFileLocMode;
}

// @brief パーサーの内部情報の表示
// @param[in] parser パーサー
void
PtDumper::put(const vector<const PtUdp*>& udp_list,
	      const vector<const PtModule*>& module_list)
{
  for ( auto udp: udp_list ) {
    put(udp);
  }
  for ( auto module: module_list ) {
    put(module);
  }
}

// @brief UDP を表示する
// @param[in] udp UDP
void
PtDumper::put(const PtUdp* udp)
{
  PtHeader x(*this, "UDP", "UDP");

  put("mFileRegion", udp->file_region());
#if 0
  put("mAttrInst",  udp->attr_top());
#endif

  put("mPrimType", udp->prim_type());

  put("mName", udp->name());

  for ( auto port: udp->port_list() ) {
    put("mPort", port->ext_name());
  }
  for ( auto io: udp->iohead_array() ) {
    put("mIO", io);
  }

  put("mInitial", udp->init_value());

  for ( auto entry: udp->table_array() ) {
    PtHeader x(*this, "mTable", "UdpEntry");

    put("mFileRegion", entry->file_region());

    for ( auto v: entry->input_array() ) {
      put("mInput", v);
    }
    put("mCurrent", entry->current());
    put("mOutput", entry->output());
  }
}

// @brief UDP テーブルの値を表示する
// @param[in] label ラベル
// @param[in] v UDP テーブルの値
void
PtDumper::put(const char* label,
	      const PtUdpValue* v)
{
  if ( v == nullptr ) {
    return;
  }

  PtHeader x(*this, label, "UdpValue");

  put("mFileRegion", v->file_region());
  put("mSymbol", v->symbol().to_string());
}

BEGIN_NONAMESPACE

// 時間の単位を表す文字列を得る．
string
unit2str(int unit)
{
  if ( unit > 2 || unit < -15 ) {
    return "illegal time unit";
  }

  unit += 15;
  string ans;
  switch ( unit % 3 ) {
  case 0: ans = "1"; break;
  case 1: ans = "10"; break;
  case 2: ans = "100"; break;
  }
  switch ( unit / 3 ) {
  case 5: ans += "s"; break;
  case 4: ans += "ms"; break;
  case 3: ans += "us"; break;
  case 2: ans += "ns"; break;
  case 1: ans += "ps"; break;
  case 0: ans += "fs"; break;
  }
  return ans;
}

END_NONAMESPACE

// @brief module を表示する
// @param[in] m モジュール
void
PtDumper::put(const PtModule* m)
{
  PtHeader x(*this, "Module", "Module");

  put("mFileRegion", m->file_region());
#if 0
  put("mAttrInst", m->attr_top());
#endif

  put("mName", m->name());

  put("mCellDefine", m->is_cell());
  put("mProtected", m->is_protected());
  put("mMacroModule", m->is_macromodule());

  if ( m->time_unit() != -16 ) {
    put("mTimeUnit", unit2str(m->time_unit()));
    put("mTimePrecision", unit2str(m->time_precision()));
  }

  put("mDefNetType", m->nettype());
  put("mUnconnDrive", m->unconn_drive());
  put("mDelayMode", m->delay_mode());
  put("mDecayTime", m->decay_time());
  put("config", m->config());
  put("library", m->library());
  put("cell", m->cell());

  PtDeclHeadArray paramport_array = m->paramport_array();
  for (ymuint i = 0; i < paramport_array.size(); ++ i) {
    const PtDeclHead* param = paramport_array[i];
    put("mParamPort", param);
  }

  for ( auto port: m->port_list() ) {
    PtHeader x(*this, "mPort", "Port");

    put("mFileRegion", port->file_region());
    if ( port->ext_name() != nullptr ) {
      put("mExprname", port->ext_name());
    }

    for (ymuint j = 0; j < port->portref_size(); ++ j) {
      const PtExpr* pr = port->portref_elem(j);
      PtHeader x(*this, "mPortRef", "PortRef");

      put("mFileRegion", pr->file_region());
      put("mDir", port->portref_dir(j));
      put("mName", pr->name());
      if ( pr->index_num() == 1 ) {
	put("mIndex", pr->index(0));
      }
      else {
	ASSERT_COND( pr->index_num() == 0 );
      }
      if ( pr->range_mode() != kVpiNoRange ) {
	put("mRangeMode", pr->range_mode());
	put("mLeftRange", pr->left_range());
	put("mRightRange", pr->right_range());
      }
    }
  }

  put_decls(m->iohead_array(),
	    m->declhead_array());
  PtItemArray item_array = m->item_array();
  for (ymuint i = 0; i < item_array.size(); ++ i) {
    const PtItem* item = item_array[i];
    put("mItem", item);
  }
}

// @brief IO のヘッダを出力する．
// @param[in] label ラベル
// @param[in] io IOヘッダ
void
PtDumper::put(const char* label,
	      const PtIOHead* io)
{
  const char* nm = nullptr;
  switch ( io->type() ) {
  case kPtIO_Input:  nm = "Input"; break;
  case kPtIO_Output: nm = "Output"; break;
  case kPtIO_Inout:  nm = "Inout"; break;
  default: ASSERT_NOT_REACHED;
  }
  PtHeader x(*this, label, nm);


  put("mFileRegion", io->file_region());
#if 0
  put("mAttrInst", io->attr_top());
#endif

  put("mAuxType", io->aux_type());
  put("mNetType", io->net_type());
  put("mVarType", io->var_type());
  put("mSigned", io->is_signed());
  put("mLeftRange", io->left_range());
  put("mRightRange", io->right_range());

  for ( auto item: io->item_list() ) {
    PtHeader x(*this, "mElem", "IOElem");

    put("mFileRegion", item->file_region());
    put("mName", item->name());
    put("mInitValue", item->init_value());
  }
}

// @brief 宣言のヘッダを出力する．
// @param[in] label ラベル
// @param[in] decl 宣言ヘッダ
void
PtDumper::put(const char* label,
	      const PtDeclHead* decl)
{
  const char* nm = nullptr;
  switch ( decl->type() ) {
  case kPtDecl_Param:
    switch ( decl->data_type() ) {
    case kVpiVarNone:            nm = "Parameter"; break;
    case kVpiVarInteger:         nm = "Parameter(integer)";  break;
    case kVpiVarReal:            nm = "Parameter(real)"; break;
    case kVpiVarTime:            nm = "Parameter(time)"; break;
    case kVpiVarRealtime:        nm = "Parameter(realtime)"; break;
    default: ASSERT_NOT_REACHED; break;
    }
    break;
  case kPtDecl_LocalParam:
    switch ( decl->data_type() ) {
    case kVpiVarNone:            nm = "Localparam"; break;
    case kVpiVarInteger:         nm = "Localparam(integer)";  break;
    case kVpiVarReal:            nm = "Localparam(real)"; break;
    case kVpiVarTime:            nm = "Localparam(time)"; break;
    case kVpiVarRealtime:        nm = "Localparam(realtime)"; break;
    default: ASSERT_NOT_REACHED; break;
    }
    break;
  case kPtDecl_Reg:            nm = "Reg";        break;
  case kPtDecl_Var:
    switch ( decl->data_type() ) {
    case kVpiVarInteger:         nm = "Integer";    break;
    case kVpiVarReal:            nm = "Real";       break;
    case kVpiVarTime:            nm = "Time";       break;
    case kVpiVarRealtime:        nm = "Realtime";  break;
    case kVpiVarNone: ASSERT_NOT_REACHED;
    default: ASSERT_NOT_REACHED; break;
    }
    break;
  case kPtDecl_Genvar:         nm = "Genvar";     break;
  case kPtDecl_Event:          nm = "Event";      break;
  case kPtDecl_SpecParam:      nm = "Specparam";  break;
  case kPtDecl_Net:
    switch ( decl->net_type() ) {
    case kVpiSupply0:            nm = "Supply0"; break;
    case kVpiSupply1:            nm = "Supply1"; break;
    case kVpiTri:                nm = "Tri"; break;
    case kVpiTriAnd:             nm = "Triand"; break;
    case kVpiTriOr:              nm = "Trior"; break;
    case kVpiTriReg:             nm = "Trireg"; break;
    case kVpiTri0:               nm = "Tri0"; break;
    case kVpiTri1:               nm = "Tri1"; break;
    case kVpiWire:               nm = "Wire"; break;
    case kVpiWand:               nm = "Wand"; break;
    case kVpiWor:                nm = "Wor"; break;
    case kVpiNone:               nm = "None"; break;
    default: ASSERT_NOT_REACHED; break;
    }
    break;
  default: ASSERT_NOT_REACHED; break;
  }
  PtHeader x(*this, label, nm);

  put("mFileRegion", decl->file_region());
#if 0
  put("mAttrInst", decl->attr_top());
#endif

  put("mSigned", decl->is_signed());
  put("mLeftRange", decl->left_range());
  put("mRightRange", decl->right_range());
  put("mVsType", decl->vs_type());
  put("mStrength", decl->strength());
  put("mDelay", decl->delay());

  for ( auto item: decl->item_list() ) {
    PtHeader x(*this, "mElem", "DeclItem");

    put("mFileRegion", item->file_region());
    put("mName", item->name());
    for ( auto range: item->range_list() ) {
      PtHeader x(*this, "mDimension", "Range");

      put("mLeftRange", range->left());
      put("mRightRange", range->right());
    }
    put("mInitValue", item->init_value());
  }
}

// @brief 要素の出力
// @param[in] label ラベル
// @param[in] item 要素
void
PtDumper::put(const char* label,
	      const PtItem* item)
{
  if ( item == nullptr ) {
    return;
  }

  const char* nm = nullptr;
  switch ( item->type() ) {
  case kPtItem_DefParam:        nm = "DefparamHeader"; break;
  case kPtItem_ContAssign:      nm = "ContAssignHeader"; break;
  case kPtItem_Initial:         nm = "Initial"; break;
  case kPtItem_Always:          nm = "Always"; break;
  case kPtItem_Task:            nm = "Task"; break;
  case kPtItem_Func:
    switch ( item->data_type() ) {
    case kVpiVarNone:            nm = "Function"; break;
    case kVpiVarInteger:         nm = "Function(integer)"; break;
    case kVpiVarReal:            nm = "Function(real)"; break;
    case kVpiVarTime:            nm = "Function(time)"; break;
    case kVpiVarRealtime:        nm = "Function(realtime)"; break;
    default: ASSERT_NOT_REACHED; break;
    }
    break;
  case kPtItem_GateInst:        nm = "GateHeader"; break;
  case kPtItem_MuInst:          nm = "MuHeader"; break;
  case kPtItem_SpecItem:
    switch ( item->specitem_type() ) {
    case kVpiPulsestyleOnEvent:  nm = "Pulse_onevent"; break;
    case kVpiPulsestyleOnDetect: nm = "Pulse_ondetect"; break;
    case kVpiShowcancelled:      nm = "Showcancelled"; break;
    case kVpiNoshowcancelled:    nm = "Noshowcancelld"; break;
    default: ASSERT_NOT_REACHED; break;
    }
  case kPtItem_SpecPath:        nm = "SpecPath"; break;
  case kPtItem_Generate:        nm = "Generate"; break;
  case kPtItem_GenBlock:        nm = "GenBlock"; break;
  case kPtItem_GenIf:           nm = "GenIf"; break;
  case kPtItem_GenCase:         nm = "GenCase"; break;
  case kPtItem_GenFor:          nm = "GenFor"; break;
  default: ASSERT_NOT_REACHED; break;
  }
  PtHeader x(*this, label, nm);

  put("mFileRegions", item->file_region());
#if 0
  put("mAttrInst", item->attr_top());
#endif

  switch ( item->type() ) {
  case kPtItem_DefParam:
    for ( auto dp: item->defparam_list() ) {
      PtHeader x(*this, "mElem", "DefParam");

      put("mFileRegion", dp->file_region());
      put(dp->namebranch_array());
      put("mName", dp->name());
      put("mValue", dp->expr());
    }
    break;

  case kPtItem_ContAssign:
    put("mStrength", item->strength());
    put("mDelay", item->delay());
    for ( auto ca: item->contassign_list() ) {
      PtHeader x(*this, "mElem", "ContAssign");

      put("mFileRegion", ca->file_region());
      put("mLhs", ca->lhs());
      put("mRhs", ca->rhs());
    }
    break;

  case kPtItem_Initial:
  case kPtItem_Always:
    put("mBody", item->body());
    break;

  case kPtItem_Task:
  case kPtItem_Func:
    put("mAutomatic", item->automatic());
    if ( item->left_range() ) {
      put("mSigned", item->is_signed());
      put("mLeftRange", item->left_range());
      put("mRightRange", item->right_range());
    }
    put("mName", item->name());
    put_decls(item->iohead_array(),
	      item->declhead_array());
    put("mBody", item->body());
    break;

  case kPtItem_GateInst:
    put("mPrimType", item->prim_type());
    put("mStrength", item->strength());
    put("mDelay", item->delay());
    for ( auto gi: item->inst_list() ) {
      PtHeader x(*this, "mElem", "GateInst");

      put("mFileRegion", gi->file_region());
      if ( gi->name() != nullptr ) {
	put("mName", gi->name());
      }
      if ( gi->left_range() ) {
	put("mLeftRange", gi->left_range());
	put("mRightrange", gi->right_range());
      }
      for ( auto con: gi->port_list() ) {
	put("mPortCon", con);
      }
    }
    break;

  case kPtItem_MuInst:
    put("mDefName", item->name());
    for ( auto con: item->paramassign_array() ) {
      put("mParamCon", con);
    }
    put("mStrength", item->strength());
    put("mDelay", item->delay());
    for ( auto mui: item->inst_list() ) {
      PtHeader x(*this, "mElem", "MuInst");

      put("mFileRegion", mui->file_region());
      put("mName", mui->name());
      if ( mui->left_range() ) {
	put("mLeftRange", mui->left_range());
	put("mRightRange", mui->right_range());
      }
      for ( auto con: mui->port_list() ) {
	put("mPortCon", con);
      }
    }
    break;

  case kPtItem_SpecItem:
    for ( auto expr: item->terminal_list() ) {
      put("mTerminal", expr);
    }
    break;

  case kPtItem_SpecPath:
#if 0 // PATH_DECL
    switch ( item->specpath_type() ) {
    case kVpiSpecPathNull:
      break;
    case kVpiSpecPathIf:
      put("mIf", item->expr());
      break;
    case kVpiSpecPathIfnone:
      {
	PtHeader x3(s, "mIfnone");
      }
      break;
    }
    {
      PtHeader x4(s, "mPathDecl");
      s << item->path_decl();
    }
#endif
    break;

  case kPtItem_Generate:
  case kPtItem_GenBlock:
    if ( item->name() != nullptr ) {
      put("mName", item->name());
    }
    put_decl_item("mBody", item->declhead_array(), item->item_array());
    break;

  case kPtItem_GenIf:
    put("mCond", item->expr());
    put_decl_item("mThenBody", item->then_declhead_array(), item->then_item_array());
    put_decl_item("mElseBody", item->else_declhead_array(), item->else_item_array());
    break;

  case kPtItem_GenCase:
    put("mExpr", item->expr());
    for ( auto gci: item->caseitem_list() ) {
      PtHeader x(*this, "mCaseItem", "GenCaseItem");

      put("mFileRegion", gci->file_region());
      for ( auto expr: gci->label_list() ) {
	put("mLabel", expr);
      }
      put_decl_item("mBody", gci->declhead_array(), gci->item_array());
    }
    break;

  case kPtItem_GenFor:
    put("mLoopVar", item->loop_var());
    put("mInitehExpr", item->init_expr());
    put("mCond", item->expr());
    put("mNext", item->next_expr());
    put("mName", item->name());
    put_decl_item("mBody", item->declhead_array(), item->item_array());
    break;

  default: ASSERT_NOT_REACHED; break;
  }
}

#if 0 // PATH_DECL
// path decl を表示する．
void
PtDumper::put(const char* label,
	      const PtPathDecl* item)
{
  PtHeader x(*this, label, "PathDecl");

  put("mFileRegion", item->file_region());

  switch ( item->edge() ) {
  case vpiPosedgeOp:
    { PtHeader x2(s, "mPosedge"); }
    break;
  case vpiNegedgeOp:
    { PtHeader x3(s, "mNegedge"); }
    break;
  }

  ymuint i = 0;
  for (const PtExprIter* ei = item->input_top();
       ei; ei = ei->next(), ++ i) {
    PtHeader x4(s, "mInput", i);
    s << ei->expr();
  }

  if ( item->input_pol() ) {
    PtHeader x5(s, "InputPol");
    s << static_cast<char>(item->input_pol());
  }

  {
    PtHeader x6(s, "mOp");
    switch ( item->op() ) {
    case vpiPathParallel: s << "PathParallel(=>)"; break;
    case vpiPathFull:     s << "PathFull(*>)"; break;
    }
  }

  i = 0;
  for (const PtExprIter* ei = item->output_top();
       ei; ei = ei->next(), ++ i) {
    PtHeader x7(s, "mOutput", i);
    s << ei->expr();
  }

  if ( item->output_pol() ) {
    PtHeader x8(s, "mOutputPol");
    s << static_cast<char>(item->output_pol());
  }

  if ( item->expr() ) {
    PtHeader x9(s, "mExpr");
    s << item->expr();
  }

  {
    PtHeader x10(s, "mPathDelay");
    s << item->path_delay();
  }
}

// path delay を表示する．
PtDumper&
operator<<(PtDumper& s,
	   const PtPathDelay* item)
{
  PtHeader x1(s, "PathDelay");

  s << item->file_region();

  for (ymuint i = 0; i < 12; ++ i) {
    if ( item->value(i) == nullptr ) {
      break;
    }
    PtHeader x2(s, "mValue", i);
    s << item->value(i);
  }

  return s;
}
#endif

// @brief ステートメントの出力
// @param[in] label ラベル
// @param[in] stmt ステートメント
void
PtDumper::put(const char* label,
	      const PtStmt* stmt)
{
  if ( stmt == nullptr ) {
    return;
  }

  const char* nm = nullptr;
  switch ( stmt->type() ) {
  case kPtDisableStmt:       nm = "Disable"; break;
  case kPtEnableStmt:        nm = "Enable"; break;
  case kPtSysEnableStmt:     nm = "SysEnable"; break;
  case kPtDcStmt:            nm = "ControlStmt"; break;
  case kPtEcStmt:            nm = "ControlStmt"; break;
  case kPtAssignStmt:        nm = "Assign"; break;
  case kPtNbAssignStmt:      nm = "NonBlockingAssign"; break;
  case kPtEventStmt:         nm = "EventTrigger"; break;
  case kPtNullStmt:          nm = "NullStmt"; break;
  case kPtIfStmt:            nm = "If"; break;
  case kPtCaseStmt:          nm = "Case"; break;
  case kPtCaseXStmt:         nm = "Casex"; break;
  case kPtCaseZStmt:         nm = "Casez"; break;
  case kPtWaitStmt:          nm = "Wait"; break;
  case kPtForeverStmt:       nm = "Forever"; break;
  case kPtRepeatStmt:        nm = "Repeat"; break;
  case kPtWhileStmt:         nm = "While"; break;
  case kPtForStmt:           nm = "For"; break;
  case kPtPcAssignStmt:      nm = "PcAssign"; break;
  case kPtDeassignStmt:      nm = "Deassign"; break;
  case kPtForceStmt:         nm = "Force"; break;
  case kPtReleaseStmt:       nm = "Release"; break;
  case kPtParBlockStmt:      nm = "Fork"; break;
  case kPtSeqBlockStmt:      nm = "Begin"; break;
  case kPtNamedParBlockStmt: nm = "Fork"; break;
  case kPtNamedSeqBlockStmt: nm = "Begin"; break;
  default: ASSERT_NOT_REACHED; break;
  }
  PtHeader x(*this, label, nm);

  put("mFileRegion", stmt->file_region());
#if 0
  put("mAttrInst", stmt->attr_top());
#endif

  switch ( stmt->type() ) {
  case kPtDisableStmt:
    put(stmt->namebranch_array());
    put("mName", stmt->name());
    break;

  case kPtEnableStmt:
  case kPtSysEnableStmt:
    put(stmt->namebranch_array());
    put("mName", stmt->name());
    for ( auto arg: stmt->arg_list() ) {
      if ( arg ) {
	put("mArg", arg);
      }
      else {
	put("mArg", "null");
      }
    }
    break;

  case kPtDcStmt:
  case kPtEcStmt:
    put("mControl", stmt->control());
    put("mBody", stmt->body());
    break;

  case kPtWaitStmt:
    put("mExpr", stmt->expr());
    put("mBody", stmt->body());
    break;

  case kPtAssignStmt:
  case kPtNbAssignStmt:
  case kPtPcAssignStmt:
  case kPtForceStmt:
  case kPtDeassignStmt:
  case kPtReleaseStmt:
    put("mControl", stmt->control());
    put("mLhs", stmt->lhs());
    put("mRhs", stmt->rhs());
    break;

  case kPtEventStmt:
    put("mEventName", stmt->primary());
    break;

  case kPtNullStmt:
    break;

  case kPtIfStmt:
    put("mCond", stmt->expr());
    put("mThen", stmt->body());
    put("mElse", stmt->else_body());
    break;

  case kPtCaseStmt:
  case kPtCaseXStmt:
  case kPtCaseZStmt:
    put("mExpr", stmt->expr());
    for ( auto ci: stmt->caseitem_list() ) {
      PtHeader x(*this, "mCaseItem", "CaseItem");

      put("mFileRegion", ci->file_region());
      for ( auto expr: ci->label_list() ) {
	put("mLabel", expr);
      }
      put("mBody", ci->body());
    }
    break;

  case kPtForeverStmt:
  case kPtRepeatStmt:
  case kPtWhileStmt:
  case kPtForStmt:
    put("mInit", stmt->init_stmt());
    put("mExpr", stmt->expr());
    put("mNext", stmt->next_stmt());
    put("mBody", stmt->body());
    break;

  case kPtNamedParBlockStmt:
  case kPtNamedSeqBlockStmt:
    put("mName", stmt->name());
    // 次の case にわざと継続する．

  case kPtParBlockStmt:
  case kPtSeqBlockStmt:
    for (ymuint i = 0; i < stmt->declhead_array().size(); ++ i) {
      const PtDeclHead* head = stmt->declhead_array()[i];
      put("mDecl", head);
    }
    for (ymuint i = 0; i < stmt->stmt_array().size(); ++ i) {
      const PtStmt* stmt1 = stmt->stmt_array()[i];
      put("mStatement", stmt1);
    }
    break;

  default: ASSERT_NOT_REACHED; break;
  }
}

// @brief 式の内容を出力する
// @param[in] label ラベル
// @param[in] expr 式
void
PtDumper::put(const char* label,
	      const PtExpr* expr)
{
  if ( expr == nullptr ) {
    return;
  }

  switch ( expr->type() ) {
  case kPtOprExpr:
    if ( expr->op_type() == kVlNullOp ) {
      // '(' expression ')' なので無視
      return put(label, expr->operand(0));
    }
    {
      PtHeader x(*this, label, "Opr");

      put("mFileRegion", expr->file_region());
#if 0
      put("mAttrInst", expr->attr_top());
#endif

      put("mOprType", expr->op_type());
      for (ymuint i = 0; i < expr->operand_num(); ++ i) {
	put("mOperand",  expr->operand(i));
      }
    }
    break;

  case kPtConstExpr:
    {
      PtHeader x(*this, label, "Constant");

      put("mFileRegion", expr->file_region());
      put("mConstType", expr->const_type());
      if ( 0 ) {
	PtHeader x(*this, "mConstUint", "uint");
	mStream << expr->const_uint();
      }
      put("mConstUint", static_cast<int>(expr->const_uint()));
      put("mConstStr", expr->const_str());
      if ( 0 ) {
	PtHeader x(*this, "mConstReal", "double");
	mStream << expr->const_real();
      }
      put("mConstReal", expr->const_real());
    }
    break;

  case kPtFuncCallExpr:
  case kPtSysFuncCallExpr:
    {
      const char* nm = nullptr;
      if ( expr->type() == kPtFuncCallExpr ) {
	nm = "FuncCall";
      }
      else {
	nm = "SysFuncCall";
      }
      PtHeader x(*this, label, nm);

      put("mFileRegion", expr->file_region());
#if 0
      put("mAttrInst", expr->attr_top());
#endif
      put(expr->namebranch_array());
      put("mName", expr->name());
      for (ymuint i = 0; i < expr->operand_num(); ++ i) {
	const PtExpr* opr = expr->operand(i);
	if ( opr ) {
	  put("mOperand", opr);
	}
	else {
	  put("mOperand", "null");
	}
      }
    }
    break;

  case kPtPrimaryExpr:
    {
      PtHeader x(*this, label, "Primary");

      put("mFileRegion", expr->file_region());
      put(expr->namebranch_array());
      put("mName", expr->name());
      for (ymuint i = 0; i < expr->index_num(); ++ i) {
	const PtExpr* index = expr->index(i);
	put("mIndex", index);
      }
      if ( expr->left_range() ) {
	put("mRangeMode", expr->range_mode());
	put("mLeftRange", expr->left_range());
	put("mRightRange", expr->right_range());
      }
    }
    break;

  default: ASSERT_NOT_REACHED; break;
  }
}

// @brief インクルード元のファイル情報の出力
// @param[in] file_loc ファイルの位置情報
void
PtDumper::put_parent_file(const FileLoc& file_loc)
{
  vector<FileLoc> file_list;
  file_loc.parent_loc_list(file_list);
  for ( auto loc: file_list ) {
    PtHeader x(*this, "mParentFile", "IncFile", false);
    mStream << "name = " << loc.filename()
	    << ", line = " << loc.line();
  }
}

/// @brief 宣言を出力する．
void
PtDumper::put_decls(PtIOHeadArray iohead_array,
		    PtDeclHeadArray declhead_array)
{
  for (ymuint i = 0; i < iohead_array.size(); ++ i) {
    const PtIOHead* io = iohead_array[i];
    put("mIODecl", io);
  }
  for (ymuint i = 0; i < declhead_array.size(); ++ i) {
    const PtDeclHead* decl = declhead_array[i];
    put("mDecl", decl);
  }
}

// @brief 宣言と要素を出力する．
// @param[in] label ラベル
// @param[in] decl_array 宣言の配列
// @param[in] item_array 要素の配列
void
PtDumper::put_decl_item(const char* label,
			PtDeclHeadArray decl_array,
			PtItemArray item_array)
{
  PtHeader x(*this, label, "GenBlock");

  for (ymuint i = 0; i < decl_array.size(); ++ i) {
    put("mDecl", decl_array[i]);
  }
  for (ymuint i = 0; i < item_array.size(); ++ i) {
    put("mItem", item_array[i]);
  }
}

// @brief 階層名の出力
void
PtDumper::put(PtNameBranchArray nb_array)
{
  for (ymuint i = 0; i < nb_array.size(); ++ i) {
    const PtNameBranch* nb = nb_array[i];
    put("mNameBranch", nb);
  }
}

// @brief ブール型データの出力
// @param[in] label ラベル
// @param[in] b ブール値
void
PtDumper::put(const char* label,
	      bool b)
{
  PtHeader x(*this, label, "bool", false);
  if ( b ) {
    mStream << "true";
  }
  else {
    mStream << "false";
  }
}

// @brief 整数型データの出力
// @param[in] label ラベル
// @param[in] d 整数値
void
PtDumper::put(const char* label,
	      int d)
{
  PtHeader x(*this, label, "int", false);
  mStream << d;
}

// @brief 実数型データの出力
// @param[in] label ラベル
// @param[in] r 実数値
void
PtDumper::put(const char* label,
	      double r)
{
  PtHeader x(*this, label, "real", false);
  mStream << r;
}

// @brief 文字列型データの出力
// @param[in] label ラベル
// @param[in] str 文字列
void
PtDumper::put(const char* label,
	      const char* str)
{
  PtHeader x(*this, label, "string", false);
  if ( str ) {
    mStream << str;
  }
}

// @brief 文字列型データの出力
// @param[in] label ラベル
// @param[in] str 文字列
void
PtDumper::put(const char* label,
	      const string& str)
{
  put(label, str.c_str());
}

// @brief 補助的なデータ型の出力
// @param[in] label ラベル
// @param[in] aux_type 補助的なデータ型
void
PtDumper::put(const char* label,
	      tVpiAuxType aux_type)
{
  PtHeader x(*this, label, "aux_type", false);
  switch ( aux_type ) {
  case kVpiAuxNone: mStream << "none"; break;
  case kVpiAuxNet:  mStream << "net"; break;
  case kVpiAuxReg:  mStream << "reg"; break;
  case kVpiAuxVar:  mStream << "var"; break;
  default: ASSERT_NOT_REACHED; break;
  }
}

// @brief ネット型の出力
// @param[in] label ラベル
// @param[in] net_type ネット型
void
PtDumper::put(const char* label,
	      tVpiNetType net_type)
{
  PtHeader x(*this, label, "net_type", false);
  switch ( net_type ) {
  case kVpiSupply0: mStream << "supply0"; break;
  case kVpiSupply1: mStream << "supply1"; break;
  case kVpiTri:     mStream << "tri"; break;
  case kVpiTriAnd:  mStream << "triand"; break;
  case kVpiTriOr:   mStream << "trior"; break;
  case kVpiTriReg:  mStream << "trireg"; break;
  case kVpiTri0:    mStream << "tri0"; break;
  case kVpiTri1:    mStream << "tri1"; break;
  case kVpiWire:    mStream << "wire"; break;
  case kVpiWand:    mStream << "wand"; break;
  case kVpiWor:     mStream << "wor"; break;
  case kVpiNone:    mStream << "none"; break;
  default: ASSERT_NOT_REACHED; break;
  }
}

// @brief 変数型の出力
// @param[in] label ラベル
// @param[in] var_type 変数型
void
PtDumper::put(const char* label,
	      tVpiVarType var_type)
{
  PtHeader x(*this, label, "var_type", false);
  switch ( var_type ) {
  case kVpiVarInteger:  mStream << "integer"; break;
  case kVpiVarReal:     mStream << "real"; break;
  case kVpiVarTime:     mStream << "time"; break;
  case kVpiVarRealtime: mStream << "realtime"; break;
  case kVpiVarNone:     mStream << "none"; break;
  default: ASSERT_NOT_REACHED; break;
  }
}

// @brief 方向の出力
// @param[in] label ラベル
// @param[in] direction 方向
void
PtDumper::put(const char* label,
	      tVlDirection direction)
{
  PtHeader x(*this, label, "direction", false);
  mStream << direction;
}

// @brief unconnected drive の出力
// @param[in] label ラベル
// @param[in] ud unconnected drive
void
PtDumper::put(const char* label,
	      tVpiUnconnDrive ud)
{
  PtHeader x(*this, label, "unconn_drive", false);
  switch ( ud ) {
  case kVpiHighZ: mStream << "high-Z"; break;
  case kVpiPull1: mStream << "pull-1"; break;
  case kVpiPull0: mStream << "pull-0"; break;
  default: ASSERT_NOT_REACHED; break;
  }
}

// @brief default delay mode の出力
// @param[in] label ラベル
// @param[in] delay_mode default delay mode
void
PtDumper::put(const char* label,
	      tVpiDefDelayMode delay_mode)
{
  PtHeader x(*this, label, "def_delay_mode", false);
  switch ( delay_mode ) {
  case kVpiDelayModeNone:    mStream << "none"; break;
  case kVpiDelayModePath:    mStream << "path"; break;
  case kVpiDelayModeDistrib: mStream << "distrib"; break;
  case kVpiDelayModeUnit:    mStream << "unit"; break;
  case kVpiDelayModeZero:    mStream << "zero"; break;
  case kVpiDelayModeMTM:     mStream << "min-typ-max"; break;
  default: ASSERT_NOT_REACHED; break;
  }
}

// @brief primitive型の出力
// @param[in] label ラベル
// @param[in] prim_type primitive 型
void
PtDumper::put(const char* label,
	      tVpiPrimType prim_type)
{
  PtHeader x(*this, label, "prim_type", false);
  switch ( prim_type ) {
  case kVpiAndPrim:      mStream << "and"; break;
  case kVpiNandPrim:     mStream << "nand"; break;
  case kVpiNorPrim:      mStream << "nor"; break;
  case kVpiOrPrim:       mStream << "or"; break;
  case kVpiXorPrim:      mStream << "xor"; break;
  case kVpiXnorPrim:     mStream << "xnor"; break;
  case kVpiBufPrim:      mStream << "buf"; break;
  case kVpiNotPrim:      mStream << "not"; break;
  case kVpiBufif0Prim:   mStream << "bufif0"; break;
  case kVpiBufif1Prim:   mStream << "bufif1"; break;
  case kVpiNotif0Prim:   mStream << "notif0"; break;
  case kVpiNotif1Prim:   mStream << "notif1"; break;
  case kVpiNmosPrim:     mStream << "nmos"; break;
  case kVpiPmosPrim:     mStream << "pmos"; break;
  case kVpiCmosPrim:     mStream << "cmos"; break;
  case kVpiRnmosPrim:    mStream << "rnmos"; break;
  case kVpiRpmosPrim:    mStream << "rpmos"; break;
  case kVpiRcmosPrim:    mStream << "rcmos"; break;
  case kVpiRtranPrim:    mStream << "rtran"; break;
  case kVpiRtranif0Prim: mStream << "rtranif0"; break;
  case kVpiRtranif1Prim: mStream << "rtranif1"; break;
  case kVpiTranPrim:     mStream << "tran"; break;
  case kVpiTranif0Prim:  mStream << "tranif0"; break;
  case kVpiTranif1Prim:  mStream << "tranif1"; break;
  case kVpiPullupPrim:   mStream << "pullup"; break;
  case kVpiPulldownPrim: mStream << "pulldown"; break;
  case kVpiSeqPrim:      mStream << "seq"; break;
  case kVpiCombPrim:     mStream << "comb"; break;
  default: ASSERT_NOT_REACHED; break;
  }
}

// @brief 演算子型の出力
// @param[in] label ラベル
// @param[in] op_type 演算子型
void
PtDumper::put(const char* label,
	      tVlOpType op_type)
{
  PtHeader x(*this, label, "op_type", false);
  switch ( op_type ) {
  case kVlMinusOp:       mStream << "minus"; break;
  case kVlPlusOp:        mStream << "plus"; break;
  case kVlNotOp:         mStream << "not"; break;
  case kVlBitNegOp:      mStream << "bitneg"; break;
  case kVlUnaryAndOp:    mStream << "unary and"; break;
  case kVlUnaryNandOp:   mStream << "unary nand"; break;
  case kVlUnaryOrOp:     mStream << "unary or"; break;
  case kVlUnaryNorOp:    mStream << "unary nor"; break;
  case kVlUnaryXorOp:    mStream << "unary xor"; break;
  case kVlUnaryXNorOp:   mStream << "unary xnor"; break;
  case kVlSubOp:         mStream << "sub"; break;
  case kVlDivOp:         mStream << "div"; break;
  case kVlModOp:         mStream << "mod"; break;
  case kVlEqOp:          mStream << "eq"; break;
  case kVlNeqOp:         mStream << "neq"; break;
  case kVlCaseEqOp:      mStream << "caseeq"; break;
  case kVlCaseNeqOp:     mStream << "caseneq"; break;
  case kVlGtOp:          mStream << "gt"; break;
  case kVlGeOp:          mStream << "ge"; break;
  case kVlLtOp:          mStream << "lt"; break;
  case kVlLeOp:          mStream << "le"; break;
  case kVlLShiftOp:      mStream << "left shift"; break;
  case kVlRShiftOp:      mStream << "right shift"; break;
  case kVlAddOp:         mStream << "add"; break;
  case kVlMultOp:        mStream << "mult"; break;
  case kVlLogAndOp:      mStream << "logical and"; break;
  case kVlLogOrOp:       mStream << "logical or"; break;
  case kVlBitAndOp:      mStream << "bit and"; break;
  case kVlBitOrOp:       mStream << "bit or"; break;
  case kVlBitXorOp:      mStream << "bit xor"; break;
  case kVlBitXNorOp:     mStream << "bit xnor"; break;
  case kVlConditionOp:   mStream << "conditional"; break;
  case kVlConcatOp:      mStream << "concat"; break;
  case kVlMultiConcatOp: mStream << "multi concat"; break;
  case kVlEventOrOp:     mStream << "event or"; break;
  case kVlNullOp:        mStream << "null"; break;
  case kVlListOp:        mStream << "list"; break;
  case kVlMinTypMaxOp:   mStream << "min-typ-max"; break;
  case kVlPosedgeOp:     mStream << "posedge"; break;
  case kVlNegedgeOp:     mStream << "negedge"; break;
  case kVlArithLShiftOp: mStream << "arithmetic left shift"; break;
  case kVlArithRShiftOp: mStream << "arithmetic right shift"; break;
  case kVlPowerOp:       mStream << "power"; break;
  default: ASSERT_NOT_REACHED; break;
  }
}

// @brief 定数型の出力
// @param[in] label ラベル
// @param[in] const_type 定数型
void
PtDumper::put(const char* label,
	      tVpiConstType const_type)
{
  PtHeader x(*this, label, "const_type", false);
  switch ( const_type ) {
  case kVpiDecConst:          mStream << "dec"; break;
  case kVpiRealConst:         mStream << "real"; break;
  case kVpiBinaryConst:       mStream << "binary"; break;
  case kVpiOctConst:          mStream << "oct"; break;
  case kVpiHexConst:          mStream << "hex"; break;
  case kVpiStringConst:       mStream << "string"; break;
  case kVpiIntConst:          mStream << "int"; break;
  case kVpiSignedDecConst:    mStream << "signed dec"; break;
  case kVpiSignedBinaryConst: mStream << "signed binary"; break;
  case kVpiSignedOctConst:    mStream << "signed oct"; break;
  case kVpiSignedHexConst:    mStream << "signed hex"; break;
  default: ASSERT_NOT_REACHED; break;
  }
}

// @brief strength の出力
// @param[in] label ラベル
// @param[in] strength strength
void
PtDumper::put(const char* label,
	      tVpiStrength strength)
{
  PtHeader x(*this, label, "strength", false);
  switch ( strength ) {
  case kVpiNoStrength:   mStream << "none"; break;
  case kVpiSupplyDrive:  mStream << "supply"; break;
  case kVpiStrongDrive:  mStream << "strong"; break;
  case kVpiPullDrive:    mStream << "pull"; break;
  case kVpiWeakDrive:    mStream << "weak"; break;
  case kVpiLargeCharge:  mStream << "large"; break;
  case kVpiMediumCharge: mStream << "medium"; break;
  case kVpiSmallCharge:  mStream << "small"; break;
  case kVpiHiZ:          mStream << "high-Z"; break;
  default: ASSERT_NOT_REACHED; break;
  }
}

// @brief vectored/scalared 属性の出力
// @param[in] label ラベル
// @param[in] vs_type vectored/scalared 属性
void
PtDumper::put(const char* label,
	      tVpiVsType vs_type)
{
  PtHeader x(*this, label, "vs_type", false);
  switch ( vs_type ) {
  case kVpiVsNone:   mStream << "none"; break;
  case kVpiVectored: mStream << "vectored"; break;
  case kVpiScalared: mStream << "scalared"; break;
  default: ASSERT_NOT_REACHED; break;
  }
}

// @brief 範囲指定モードの出力
// @param[in] label ラベル
// @param[in] range_mode
void
PtDumper::put(const char* label,
	      tVpiRangeMode range_mode)
{
  PtHeader x(*this, label, "range_mode", false);
  switch ( range_mode ) {
  case kVpiNoRange:    mStream << "no range"; break;
  case kVpiConstRange: mStream << "constant range"; break;
  case kVpiPlusRange:  mStream << "plus range"; break;
  case kVpiMinusRange: mStream << "minus range"; break;
  default: ASSERT_NOT_REACHED; break;
  }
}

// @brief FileLoc の情報を出力する．
// @param[in] label ラベル
// @param[in] file_loc ファイルの位置情報
void
PtDumper::put(const char* label,
	      const FileLoc& file_loc)
{
  if ( !file_loc_mode() ) {
    return;
  }

  PtHeader x(*this, label, "FileLoc");
  if ( file_loc.is_valid() ) {
    put_parent_file(file_loc);

    PtHeader x(*this, "mPosition", "FilePos", false);
    mStream << "name = " << file_loc.filename()
	    << ", line = " << file_loc.line()
	    << ", column = " << file_loc.column();
  }
  else {
    PtHeader x(*this, "mPosition", "FilePos", false);
    mStream << "invalid file_loc";
  }
}

// @brief FileRegion の情報を出力する．
// @param[in] label ラベル
// @param[in] file_region ファイル領域の情報
void
PtDumper::put(const char* label,
	      const FileRegion& file_region)
{
  if ( !file_loc_mode() ) {
    return;
  }

  PtHeader x(*this, label, "FileRegion");

  const FileLoc& first = file_region.start_loc();
  const FileLoc& last = file_region.end_loc();

  if ( first.file_info() == last.file_info() ) {
    if ( first.is_valid() ) {
      // 同じファイル
      put_parent_file(first);

      PtHeader x(*this, "mRegion", "FilePos", false);
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
      PtHeader x(*this, "mRegion", "FilePos", false);
      mStream << "---";
    }
  }
  else {
    // 異なるファイル
    put("mFirstLoc", first);
    put("mLastLoc", last);
  }
}

// @brief 属性インスタンスを出力する．
// @param[in] label ラベル
// @param[in] attr_top 属性インスタンスの先頭
void
PtDumper::put(const char* label,
	      const PtAttrInst* attr)
{
  if ( attr == nullptr ) {
    return;
  }

  PtHeader x(*this, label, "AttrInstList");
#if 0
  for ( ; attr; attr = attr->next()) {
    PtHeader x(*this, "mAttrInst", "AttrInst");
    for (const PtAttrSpec* as = attr->attr_spec_top();
	 as; as = as->next()) {
      PtHeader x(*this, "mAttrSpec", "AttrSpec");

      put("mFileRegion", as->file_region());
      put("mName", as->name());
      put("mExpr", as->expr());
    }
  }
#endif
}

// @brief コントロールの内容を表示する．
// @param[in] label ラベル
// @param[in] ctrl コントロール
void
PtDumper::put(const char* label,
	      const PtControl* ctrl)
{
  if ( ctrl == nullptr ) {
    return;
  }

  const char* nm = nullptr;
  switch ( ctrl->type() ) {
  case kPtDelayControl:  nm = "DelayControl"; break;
  case kPtEventControl:  nm = "EventControl"; break;
  case kPtRepeatControl: nm = "RepeatControl"; break;
  default: ASSERT_NOT_REACHED; break;
  }
  PtHeader x(*this, label, nm);

  put("mFileRegion", ctrl->file_region());
  put("mDelay", ctrl->delay());
  put("mRepExpr", ctrl->rep_expr());
  for ( auto expr: ctrl->event_list() ) {
    put("mEvent", expr);
  }
}

// @brief strength の内容を表示する．
// @param[in] label ラベル
// @param[in] str 信号強度
void
PtDumper::put(const char* label,
	      const PtStrength* str)
{
  if ( str == nullptr ) {
    return;
  }

  PtHeader x(*this, label, "Strengh");

  put("mFileRegion", str->file_region());
  if ( str->drive0() != kVpiNoStrength ) {
    put("mDrive0", str->drive0());
  }
  if ( str->drive1() != kVpiNoStrength ) {
    put("mDrive1", str->drive1());
  }
  if ( str->charge() != kVpiNoStrength ) {
    put("mCharge", str->charge());
  }
}

// @brief 遅延情報を出力する．
// @param[in] label ラベル
// @param[in] delay 遅延
void
PtDumper::put(const char* label,
	      const PtDelay* delay)
{
  if ( delay == nullptr ) {
    return;
  }

  PtHeader x(*this, label, "Delay");

  put("mFileRegion", delay->file_region());
  put("mDelay0", delay->value(0));
  put("mDelay1", delay->value(1));
  put("mDelay2", delay->value(2));
}

// @brief 接続情報を表示する．
// @param[in] label ラベル
// @param[in] con 接続情報
void
PtDumper::put(const char* label,
	      const PtConnection* con)
{
  if ( con == nullptr ) {
    return;
  }

  const char* nm = nullptr;
  if ( con->name() != nullptr ) {
    nm = "NamedCon";
  }
  else {
    nm = "OrderedCon";
  }
  PtHeader x(*this, label, nm);

  put("mFileRegion", con->file_region());
#if 0
  put("mAttrInst", con->attr_top());
#endif

  if ( con->name() != nullptr ) {
    put("mName", con->name());
  }
  if ( con->expr() ) {
    put("mExpr", con->expr());
  }
  else {
    put("mExpr", "null");
  }
}

// @brief 階層ブランチの情報を出力する
// @param[in] label ラベル
// @param[in] nm 階層ブランチ
void
PtDumper::put(const char* label,
	      const PtNameBranch* nb)
{
  PtHeader x(*this, label, "NameBranch");

  put("mName", nb->name());
  if ( nb->has_index() ) {
    put("mIndex", nb->index());
  }
}

// @brief 開始マーカを出力する．
// @param[in] label マーカのラベル
// @param[in] type タイプ
// @param[in] need_cr true の時はマーカ出力後に改行する．
void
PtDumper::put_begin(const char* label,
		    const char* type,
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
// @param[in] label マーカのラベル
void
PtDumper::put_end(const char* label)
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
// @param[in] s 出力ストリーム
// @param[in] name 名前
// @param[in] type 属性
// @param[in] need_cr マーカの直後に改行するとき true とするフラグ
PtHeader::PtHeader(PtDumper& s,
		   const char* name,
		   const char* type,
		   bool need_cr) :
  mS(s),
  mName(name)
{
  mS.put_begin(mName.c_str(), type, need_cr);
}

// @brief デストラクタ
PtHeader::~PtHeader()
{
  mS.put_end(mName.c_str());
}

END_NAMESPACE_YM_VERILOG
