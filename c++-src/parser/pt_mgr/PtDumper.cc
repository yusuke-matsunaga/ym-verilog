
/// @file PtDumper.cc
/// @brief PtDumper の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "parser/PtDumper.h"

#include "ym/pt/PtModule.h"
#include "ym/pt/PtUdp.h"
#include "ym/pt/PtPort.h"
#include "ym/pt/PtDecl.h"
#include "ym/pt/PtItem.h"
#include "ym/pt/PtStmt.h"
#include "ym/pt/PtExpr.h"
#include "ym/pt/PtMisc.h"
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
  for ( auto io: udp->iohead_list() ) {
    put("mIO", io);
  }

  put("mInitial", udp->init_value());

  for ( auto entry: udp->table_list() ) {
    PtHeader x(*this, "mTable", "UdpEntry");

    put("mFileRegion", entry->file_region());

    for ( auto v: entry->input_list() ) {
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

  for ( auto param: m->paramport_list() ) {
    put("mParamPort", param);
  }

  for ( auto port: m->port_list() ) {
    PtHeader x(*this, "mPort", "Port");

    put("mFileRegion", port->file_region());
    if ( port->ext_name() != nullptr ) {
      put("mExprname", port->ext_name());
    }

    for ( int j = 0; j < port->portref_size(); ++ j ) {
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
      if ( pr->range_mode() != VpiRangeMode::No ) {
	put("mRangeMode", pr->range_mode());
	put("mLeftRange", pr->left_range());
	put("mRightRange", pr->right_range());
      }
    }
  }

  put_decls(m->iohead_list(), m->declhead_list());
  for ( auto item: m->item_list() ) {
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
  switch ( io->direction() ) {
  case VpiDir::Input:  nm = "Input"; break;
  case VpiDir::Output: nm = "Output"; break;
  case VpiDir::Inout:  nm = "Inout"; break;
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
  case PtDeclType::Param:
    switch ( decl->data_type() ) {
    case VpiVarType::None:            nm = "Parameter"; break;
    case VpiVarType::Integer:         nm = "Parameter(integer)";  break;
    case VpiVarType::Real:            nm = "Parameter(real)"; break;
    case VpiVarType::Time:            nm = "Parameter(time)"; break;
    case VpiVarType::Realtime:        nm = "Parameter(realtime)"; break;
    default: ASSERT_NOT_REACHED; break;
    }
    break;
  case PtDeclType::LocalParam:
    switch ( decl->data_type() ) {
    case VpiVarType::None:            nm = "Localparam"; break;
    case VpiVarType::Integer:         nm = "Localparam(integer)";  break;
    case VpiVarType::Real:            nm = "Localparam(real)"; break;
    case VpiVarType::Time:            nm = "Localparam(time)"; break;
    case VpiVarType::Realtime:        nm = "Localparam(realtime)"; break;
    default: ASSERT_NOT_REACHED; break;
    }
    break;
  case PtDeclType::Reg:            nm = "Reg";        break;
  case PtDeclType::Var:
    switch ( decl->data_type() ) {
    case VpiVarType::Integer:         nm = "Integer";    break;
    case VpiVarType::Real:            nm = "Real";       break;
    case VpiVarType::Time:            nm = "Time";       break;
    case VpiVarType::Realtime:        nm = "Realtime";  break;
    case VpiVarType::None: ASSERT_NOT_REACHED;
    default: ASSERT_NOT_REACHED; break;
    }
    break;
  case PtDeclType::Genvar:         nm = "Genvar";     break;
  case PtDeclType::Event:          nm = "Event";      break;
  case PtDeclType::SpecParam:      nm = "Specparam";  break;
  case PtDeclType::Net:
    switch ( decl->net_type() ) {
    case VpiNetType::Supply0:          nm = "Supply0"; break;
    case VpiNetType::Supply1:          nm = "Supply1"; break;
    case VpiNetType::Tri:              nm = "Tri"; break;
    case VpiNetType::TriAnd:           nm = "Triand"; break;
    case VpiNetType::TriOr:            nm = "Trior"; break;
    case VpiNetType::TriReg:           nm = "Trireg"; break;
    case VpiNetType::Tri0:             nm = "Tri0"; break;
    case VpiNetType::Tri1:             nm = "Tri1"; break;
    case VpiNetType::Wire:             nm = "Wire"; break;
    case VpiNetType::Wand:             nm = "Wand"; break;
    case VpiNetType::Wor:              nm = "Wor"; break;
    case VpiNetType::None:             nm = "None"; break;
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
  case PtItemType::DefParam:        nm = "DefparamHeader"; break;
  case PtItemType::ContAssign:      nm = "ContAssignHeader"; break;
  case PtItemType::Initial:         nm = "Initial"; break;
  case PtItemType::Always:          nm = "Always"; break;
  case PtItemType::Task:            nm = "Task"; break;
  case PtItemType::Func:
    switch ( item->data_type() ) {
    case VpiVarType::None:            nm = "Function"; break;
    case VpiVarType::Integer:         nm = "Function(integer)"; break;
    case VpiVarType::Real:            nm = "Function(real)"; break;
    case VpiVarType::Time:            nm = "Function(time)"; break;
    case VpiVarType::Realtime:        nm = "Function(realtime)"; break;
    default: ASSERT_NOT_REACHED; break;
    }
    break;
  case PtItemType::GateInst:        nm = "GateHeader"; break;
  case PtItemType::MuInst:          nm = "MuHeader"; break;
  case PtItemType::SpecItem:
    switch ( item->specitem_type() ) {
    case VpiSpecItemType::PulsestyleOnEvent:  nm = "Pulse_onevent"; break;
    case VpiSpecItemType::PulsestyleOnDetect: nm = "Pulse_ondetect"; break;
    case VpiSpecItemType::Showcancelled:      nm = "Showcancelled"; break;
    case VpiSpecItemType::Noshowcancelled:    nm = "Noshowcancelld"; break;
    default: ASSERT_NOT_REACHED; break;
    }
  case PtItemType::SpecPath:        nm = "SpecPath"; break;
  case PtItemType::Generate:        nm = "Generate"; break;
  case PtItemType::GenBlock:        nm = "GenBlock"; break;
  case PtItemType::GenIf:           nm = "GenIf"; break;
  case PtItemType::GenCase:         nm = "GenCase"; break;
  case PtItemType::GenFor:          nm = "GenFor"; break;
  default: ASSERT_NOT_REACHED; break;
  }
  PtHeader x(*this, label, nm);

  put("mFileRegions", item->file_region());
#if 0
  put("mAttrInst", item->attr_top());
#endif

  switch ( item->type() ) {
  case PtItemType::DefParam:
    for ( auto dp: item->defparam_list() ) {
      PtHeader x(*this, "mElem", "DefParam");

      put("mFileRegion", dp->file_region());
      put(dp->namebranch_list());
      put("mName", dp->name());
      put("mValue", dp->expr());
    }
    break;

  case PtItemType::ContAssign:
    put("mStrength", item->strength());
    put("mDelay", item->delay());
    for ( auto ca: item->contassign_list() ) {
      PtHeader x(*this, "mElem", "ContAssign");

      put("mFileRegion", ca->file_region());
      put("mLhs", ca->lhs());
      put("mRhs", ca->rhs());
    }
    break;

  case PtItemType::Initial:
  case PtItemType::Always:
    put("mBody", item->body());
    break;

  case PtItemType::Task:
  case PtItemType::Func:
    put("mAutomatic", item->automatic());
    if ( item->left_range() ) {
      put("mSigned", item->is_signed());
      put("mLeftRange", item->left_range());
      put("mRightRange", item->right_range());
    }
    put("mName", item->name());
    put_decls(item->iohead_list(),
	      item->declhead_list());
    put("mBody", item->body());
    break;

  case PtItemType::GateInst:
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

  case PtItemType::MuInst:
    put("mDefName", item->name());
    for ( auto con: item->paramassign_list() ) {
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

  case PtItemType::SpecItem:
    for ( auto expr: item->terminal_list() ) {
      put("mTerminal", expr);
    }
    break;

  case PtItemType::SpecPath:
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

  case PtItemType::Generate:
  case PtItemType::GenBlock:
    if ( item->name() != nullptr ) {
      put("mName", item->name());
    }
    put_decl_item("mBody", item->declhead_list(), item->item_list());
    break;

  case PtItemType::GenIf:
    put("mCond", item->expr());
    put_decl_item("mThenBody", item->then_declhead_list(), item->then_item_list());
    put_decl_item("mElseBody", item->else_declhead_list(), item->else_item_list());
    break;

  case PtItemType::GenCase:
    put("mExpr", item->expr());
    for ( auto gci: item->caseitem_list() ) {
      PtHeader x(*this, "mCaseItem", "GenCaseItem");

      put("mFileRegion", gci->file_region());
      for ( auto expr: gci->label_list() ) {
	put("mLabel", expr);
      }
      put_decl_item("mBody", gci->declhead_list(), gci->item_list());
    }
    break;

  case PtItemType::GenFor:
    put("mLoopVar", item->loop_var());
    put("mInitehExpr", item->init_expr());
    put("mCond", item->expr());
    put("mNext", item->next_expr());
    put("mName", item->name());
    put_decl_item("mBody", item->declhead_list(), item->item_list());
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
  for ( auto ei = item->input_top(); ei; ei = ei->next(), ++ i ) {
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
  for ( auto ei = item->output_top(); ei; ei = ei->next(), ++ i) {
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

  for ( int i = 0; i < 12; ++ i ) {
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
  case PtStmtType::Disable:       nm = "Disable"; break;
  case PtStmtType::Enable:        nm = "Enable"; break;
  case PtStmtType::SysEnable:     nm = "SysEnable"; break;
  case PtStmtType::DelayControl:            nm = "ControlStmt"; break;
  case PtStmtType::EventControl:            nm = "ControlStmt"; break;
  case PtStmtType::Assign:        nm = "Assign"; break;
  case PtStmtType::NbAssign:      nm = "NonBlockingAssign"; break;
  case PtStmtType::Event:         nm = "EventTrigger"; break;
  case PtStmtType::Null:          nm = "NullStmt"; break;
  case PtStmtType::If:            nm = "If"; break;
  case PtStmtType::Case:          nm = "Case"; break;
  case PtStmtType::CaseX:         nm = "Casex"; break;
  case PtStmtType::CaseZ:         nm = "Casez"; break;
  case PtStmtType::Wait:          nm = "Wait"; break;
  case PtStmtType::Forever:       nm = "Forever"; break;
  case PtStmtType::Repeat:        nm = "Repeat"; break;
  case PtStmtType::White:         nm = "While"; break;
  case PtStmtType::For:           nm = "For"; break;
  case PtStmtType::PcAssign:      nm = "PcAssign"; break;
  case PtStmtType::Deassign:      nm = "Deassign"; break;
  case PtStmtType::Force:         nm = "Force"; break;
  case PtStmtType::Release:       nm = "Release"; break;
  case PtStmtType::ParBlock:      nm = "Fork"; break;
  case PtStmtType::SeqBlock:      nm = "Begin"; break;
  case PtStmtType::NamedParBlock: nm = "Fork"; break;
  case PtStmtType::NamedSeqBlock: nm = "Begin"; break;
  default: ASSERT_NOT_REACHED; break;
  }
  PtHeader x(*this, label, nm);

  put("mFileRegion", stmt->file_region());
#if 0
  put("mAttrInst", stmt->attr_top());
#endif

  switch ( stmt->type() ) {
  case PtStmtType::Disable:
    put(stmt->namebranch_list());
    put("mName", stmt->name());
    break;

  case PtStmtType::Enable:
  case PtStmtType::SysEnable:
    put(stmt->namebranch_list());
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

  case PtStmtType::DelayControl:
  case PtStmtType::EventControl:
    put("mControl", stmt->control());
    put("mBody", stmt->body());
    break;

  case PtStmtType::Wait:
    put("mExpr", stmt->expr());
    put("mBody", stmt->body());
    break;

  case PtStmtType::Assign:
  case PtStmtType::NbAssign:
  case PtStmtType::PcAssign:
  case PtStmtType::Force:
  case PtStmtType::Deassign:
  case PtStmtType::Release:
    put("mControl", stmt->control());
    put("mLhs", stmt->lhs());
    put("mRhs", stmt->rhs());
    break;

  case PtStmtType::Event:
    put("mEventName", stmt->primary());
    break;

  case PtStmtType::Null:
    break;

  case PtStmtType::If:
    put("mCond", stmt->expr());
    put("mThen", stmt->body());
    put("mElse", stmt->else_body());
    break;

  case PtStmtType::Case:
  case PtStmtType::CaseX:
  case PtStmtType::CaseZ:
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

  case PtStmtType::Forever:
  case PtStmtType::Repeat:
  case PtStmtType::White:
  case PtStmtType::For:
    put("mInit", stmt->init_stmt());
    put("mExpr", stmt->expr());
    put("mNext", stmt->next_stmt());
    put("mBody", stmt->body());
    break;

  case PtStmtType::NamedParBlock:
  case PtStmtType::NamedSeqBlock:
    put("mName", stmt->name());
    // 次の case にわざと継続する．

  case PtStmtType::ParBlock:
  case PtStmtType::SeqBlock:
    for ( auto head: stmt->declhead_list() ) {
      put("mDecl", head);
    }
    for ( auto stmt1: stmt->stmt_list() ) {
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
  case PtExprType::Opr:
    if ( expr->op_type() == VpiOpType::Null ) {
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
      for ( int i = 0; i < expr->operand_num(); ++ i ) {
	put("mOperand",  expr->operand(i));
      }
    }
    break;

  case PtExprType::Const:
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

  case PtExprType::FuncCall:
  case PtExprType::SysFuncCall:
    {
      const char* nm = nullptr;
      if ( expr->type() == PtExprType::FuncCall ) {
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
      put(expr->namebranch_list());
      put("mName", expr->name());
      for ( int i = 0; i < expr->operand_num(); ++ i ) {
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

  case PtExprType::Primary:
    {
      PtHeader x(*this, label, "Primary");

      put("mFileRegion", expr->file_region());
      put(expr->namebranch_list());
      put("mName", expr->name());
      for ( int i = 0; i < expr->index_num(); ++ i ) {
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
  auto file_list{file_loc.parent_loc_list()};
  for ( auto loc: file_list ) {
    PtHeader x(*this, "mParentFile", "IncFile", false);
    mStream << "name = " << loc.filename()
	    << ", line = " << loc.line();
  }
}

/// @brief 宣言を出力する．
void
PtDumper::put_decls(const vector<const PtIOHead*>& iohead_array,
		    const vector<const PtDeclHead*>& declhead_array)
{
  for ( auto io: iohead_array ) {
    put("mIODecl", io);
  }
  for ( auto decl: declhead_array ) {
    put("mDecl", decl);
  }
}

// @brief 宣言と要素を出力する．
// @param[in] label ラベル
// @param[in] decl_array 宣言の配列
// @param[in] item_array 要素の配列
void
PtDumper::put_decl_item(const char* label,
			const vector<const PtDeclHead*>& decl_array,
			const vector<const PtItem*>& item_array)
{
  PtHeader x(*this, label, "GenBlock");

  for ( auto decl: decl_array ) {
    put("mDecl", decl);
  }
  for ( auto item: item_array ) {
    put("mItem", item);
  }
}

// @brief 階層名の出力
void
PtDumper::put(const vector<const PtNameBranch*>& nb_array)
{
  for ( auto nb: nb_array ) {
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
	      VpiAuxType aux_type)
{
  PtHeader x(*this, label, "aux_type", false);
  switch ( aux_type ) {
  case VpiAuxType::None: mStream << "none"; break;
  case VpiAuxType::Net:  mStream << "net"; break;
  case VpiAuxType::Reg:  mStream << "reg"; break;
  case VpiAuxType::Var:  mStream << "var"; break;
  default: ASSERT_NOT_REACHED; break;
  }
}

// @brief ネット型の出力
// @param[in] label ラベル
// @param[in] net_type ネット型
void
PtDumper::put(const char* label,
	      VpiNetType net_type)
{
  PtHeader x(*this, label, "net_type", false);
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
  default: ASSERT_NOT_REACHED; break;
  }
}

// @brief 変数型の出力
// @param[in] label ラベル
// @param[in] var_type 変数型
void
PtDumper::put(const char* label,
	      VpiVarType var_type)
{
  PtHeader x(*this, label, "var_type", false);
  switch ( var_type ) {
  case VpiVarType::Integer:  mStream << "integer"; break;
  case VpiVarType::Real:     mStream << "real"; break;
  case VpiVarType::Time:     mStream << "time"; break;
  case VpiVarType::Realtime: mStream << "realtime"; break;
  case VpiVarType::None:     mStream << "none"; break;
  default: ASSERT_NOT_REACHED; break;
  }
}

// @brief 方向の出力
// @param[in] label ラベル
// @param[in] direction 方向
void
PtDumper::put(const char* label,
	      VpiDir direction)
{
  PtHeader x(*this, label, "direction", false);
  mStream << direction;
}

// @brief unconnected drive の出力
// @param[in] label ラベル
// @param[in] ud unconnected drive
void
PtDumper::put(const char* label,
	      VpiUnconnDrive ud)
{
  PtHeader x(*this, label, "unconn_drive", false);
  switch ( ud ) {
  case VpiUnconnDrive::HighZ: mStream << "high-Z"; break;
  case VpiUnconnDrive::Pull1: mStream << "pull-1"; break;
  case VpiUnconnDrive::Pull0: mStream << "pull-0"; break;
  default: ASSERT_NOT_REACHED; break;
  }
}

// @brief default delay mode の出力
// @param[in] label ラベル
// @param[in] delay_mode default delay mode
void
PtDumper::put(const char* label,
	      VpiDefDelayMode delay_mode)
{
  PtHeader x(*this, label, "def_delay_mode", false);
  switch ( delay_mode ) {
  case VpiDefDelayMode::None:    mStream << "none"; break;
  case VpiDefDelayMode::Path:    mStream << "path"; break;
  case VpiDefDelayMode::Distrib: mStream << "distrib"; break;
  case VpiDefDelayMode::Unit:    mStream << "unit"; break;
  case VpiDefDelayMode::Zero:    mStream << "zero"; break;
  case VpiDefDelayMode::MTM:     mStream << "min-typ-max"; break;
  default: ASSERT_NOT_REACHED; break;
  }
}

// @brief primitive型の出力
// @param[in] label ラベル
// @param[in] prim_type primitive 型
void
PtDumper::put(const char* label,
	      VpiPrimType prim_type)
{
  PtHeader x(*this, label, "prim_type", false);
  switch ( prim_type ) {
  case VpiPrimType::And:      mStream << "and"; break;
  case VpiPrimType::Nand:     mStream << "nand"; break;
  case VpiPrimType::Nor:      mStream << "nor"; break;
  case VpiPrimType::Or:       mStream << "or"; break;
  case VpiPrimType::Xor:      mStream << "xor"; break;
  case VpiPrimType::Xnor:     mStream << "xnor"; break;
  case VpiPrimType::Buf:      mStream << "buf"; break;
  case VpiPrimType::Not:      mStream << "not"; break;
  case VpiPrimType::Bufif0:   mStream << "bufif0"; break;
  case VpiPrimType::Bufif1:   mStream << "bufif1"; break;
  case VpiPrimType::Notif0:   mStream << "notif0"; break;
  case VpiPrimType::Notif1:   mStream << "notif1"; break;
  case VpiPrimType::Nmos:     mStream << "nmos"; break;
  case VpiPrimType::Pmos:     mStream << "pmos"; break;
  case VpiPrimType::Cmos:     mStream << "cmos"; break;
  case VpiPrimType::Rnmos:    mStream << "rnmos"; break;
  case VpiPrimType::Rpmos:    mStream << "rpmos"; break;
  case VpiPrimType::Rcmos:    mStream << "rcmos"; break;
  case VpiPrimType::Rtran:    mStream << "rtran"; break;
  case VpiPrimType::Rtranif0: mStream << "rtranif0"; break;
  case VpiPrimType::Rtranif1: mStream << "rtranif1"; break;
  case VpiPrimType::Tran:     mStream << "tran"; break;
  case VpiPrimType::Tranif0:  mStream << "tranif0"; break;
  case VpiPrimType::Tranif1:  mStream << "tranif1"; break;
  case VpiPrimType::Pullup:   mStream << "pullup"; break;
  case VpiPrimType::Pulldown: mStream << "pulldown"; break;
  case VpiPrimType::Seq:      mStream << "seq"; break;
  case VpiPrimType::Comb:     mStream << "comb"; break;
  default: ASSERT_NOT_REACHED; break;
  }
}

// @brief 演算子型の出力
// @param[in] label ラベル
// @param[in] op_type 演算子型
void
PtDumper::put(const char* label,
	      VpiOpType op_type)
{
  PtHeader x(*this, label, "op_type", false);
  switch ( op_type ) {
  case VpiOpType::Minus:       mStream << "minus"; break;
  case VpiOpType::Plus:        mStream << "plus"; break;
  case VpiOpType::Not:         mStream << "not"; break;
  case VpiOpType::BitNeg:      mStream << "bitneg"; break;
  case VpiOpType::UnaryAnd:    mStream << "unary and"; break;
  case VpiOpType::UnaryNand:   mStream << "unary nand"; break;
  case VpiOpType::UnaryOr:     mStream << "unary or"; break;
  case VpiOpType::UnaryNor:    mStream << "unary nor"; break;
  case VpiOpType::UnaryXor:    mStream << "unary xor"; break;
  case VpiOpType::UnaryXNor:   mStream << "unary xnor"; break;
  case VpiOpType::Sub:         mStream << "sub"; break;
  case VpiOpType::Div:         mStream << "div"; break;
  case VpiOpType::Mod:         mStream << "mod"; break;
  case VpiOpType::Eq:          mStream << "eq"; break;
  case VpiOpType::Neq:         mStream << "neq"; break;
  case VpiOpType::CaseEq:      mStream << "caseeq"; break;
  case VpiOpType::CaseNeq:     mStream << "caseneq"; break;
  case VpiOpType::Gt:          mStream << "gt"; break;
  case VpiOpType::Ge:          mStream << "ge"; break;
  case VpiOpType::Lt:          mStream << "lt"; break;
  case VpiOpType::Le:          mStream << "le"; break;
  case VpiOpType::LShift:      mStream << "left shift"; break;
  case VpiOpType::RShift:      mStream << "right shift"; break;
  case VpiOpType::Add:         mStream << "add"; break;
  case VpiOpType::Mult:        mStream << "mult"; break;
  case VpiOpType::LogAnd:      mStream << "logical and"; break;
  case VpiOpType::LogOr:       mStream << "logical or"; break;
  case VpiOpType::BitAnd:      mStream << "bit and"; break;
  case VpiOpType::BitOr:       mStream << "bit or"; break;
  case VpiOpType::BitXor:      mStream << "bit xor"; break;
  case VpiOpType::BitXNor:     mStream << "bit xnor"; break;
  case VpiOpType::Condition:   mStream << "conditional"; break;
  case VpiOpType::Concat:      mStream << "concat"; break;
  case VpiOpType::MultiConcat: mStream << "multi concat"; break;
  case VpiOpType::EventOr:     mStream << "event or"; break;
  case VpiOpType::Null:        mStream << "null"; break;
  case VpiOpType::List:        mStream << "list"; break;
  case VpiOpType::MinTypMax:   mStream << "min-typ-max"; break;
  case VpiOpType::Posedge:     mStream << "posedge"; break;
  case VpiOpType::Negedge:     mStream << "negedge"; break;
  case VpiOpType::ArithLShift: mStream << "arithmetic left shift"; break;
  case VpiOpType::ArithRShift: mStream << "arithmetic right shift"; break;
  case VpiOpType::Power:       mStream << "power"; break;
  default: ASSERT_NOT_REACHED; break;
  }
}

// @brief 定数型の出力
// @param[in] label ラベル
// @param[in] const_type 定数型
void
PtDumper::put(const char* label,
	      VpiConstType const_type)
{
  PtHeader x(*this, label, "const_type", false);
  switch ( const_type ) {
  case VpiConstType::Dec:          mStream << "dec"; break;
  case VpiConstType::Real:         mStream << "real"; break;
  case VpiConstType::Binary:       mStream << "binary"; break;
  case VpiConstType::Oct:          mStream << "oct"; break;
  case VpiConstType::Hex:          mStream << "hex"; break;
  case VpiConstType::String:       mStream << "string"; break;
  case VpiConstType::Int:          mStream << "int"; break;
  case VpiConstType::SignedDec:    mStream << "signed dec"; break;
  case VpiConstType::SignedBinary: mStream << "signed binary"; break;
  case VpiConstType::SignedOct:    mStream << "signed oct"; break;
  case VpiConstType::SignedHex:    mStream << "signed hex"; break;
  default: ASSERT_NOT_REACHED; break;
  }
}

// @brief strength の出力
// @param[in] label ラベル
// @param[in] strength strength
void
PtDumper::put(const char* label,
	      VpiStrength strength)
{
  PtHeader x(*this, label, "strength", false);
  switch ( strength ) {
  case VpiStrength::NoStrength:   mStream << "none"; break;
  case VpiStrength::SupplyDrive:  mStream << "supply"; break;
  case VpiStrength::StrongDrive:  mStream << "strong"; break;
  case VpiStrength::PullDrive:    mStream << "pull"; break;
  case VpiStrength::WeakDrive:    mStream << "weak"; break;
  case VpiStrength::LargeCharge:  mStream << "large"; break;
  case VpiStrength::MediumCharge: mStream << "medium"; break;
  case VpiStrength::SmallCharge:  mStream << "small"; break;
  case VpiStrength::HiZ:          mStream << "high-Z"; break;
  default: ASSERT_NOT_REACHED; break;
  }
}

// @brief vectored/scalared 属性の出力
// @param[in] label ラベル
// @param[in] vs_type vectored/scalared 属性
void
PtDumper::put(const char* label,
	      VpiVsType vs_type)
{
  PtHeader x(*this, label, "vs_type", false);
  switch ( vs_type ) {
  case VpiVsType::None:     mStream << "none"; break;
  case VpiVsType::Vectored: mStream << "vectored"; break;
  case VpiVsType::Scalared: mStream << "scalared"; break;
  default: ASSERT_NOT_REACHED; break;
  }
}

// @brief 範囲指定モードの出力
// @param[in] label ラベル
// @param[in] range_mode
void
PtDumper::put(const char* label,
	      VpiRangeMode range_mode)
{
  PtHeader x(*this, label, "range_mode", false);
  switch ( range_mode ) {
  case VpiRangeMode::No:    mStream << "no range"; break;
  case VpiRangeMode::Const: mStream << "constant range"; break;
  case VpiRangeMode::Plus:  mStream << "plus range"; break;
  case VpiRangeMode::Minus: mStream << "minus range"; break;
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
  case PtCtrlType::Delay:  nm = "DelayControl"; break;
  case PtCtrlType::Event:  nm = "EventControl"; break;
  case PtCtrlType::Repeat: nm = "RepeatControl"; break;
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
  if ( str->drive0() != VpiStrength::NoStrength ) {
    put("mDrive0", str->drive0());
  }
  if ( str->drive1() != VpiStrength::NoStrength ) {
    put("mDrive1", str->drive1());
  }
  if ( str->charge() != VpiStrength::NoStrength ) {
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
