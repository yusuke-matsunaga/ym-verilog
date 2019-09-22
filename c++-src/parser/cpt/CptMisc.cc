﻿
/// @file CptMisc.cc
/// @brief CptMisc の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "CptMisc.h"
#include "CptFactory.h"

#include "ym/pt/PtExpr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// delay control/event control/repeat control を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptControl::CptControl()
{
}

// デストラクタ
CptControl::~CptControl()
{
}

// @brief  遅延式の取得
// @retval 遅延を表す式 delay control の場合
// @retval nullptr 上記以外
// デフォルトでは nullptr を返す．
const PtExpr*
CptControl::delay() const
{
  return nullptr;
}

// @brief イベントリストの取得
// @note event control/repeat control の場合のみ意味を持つ
PtExprArray
CptControl::event_list() const
{
  return PtExprArray{};
}

// @brief 繰り返し数の取得
// @retval 繰り返し数を表す式 repeat control の場合
// @retval nullptr 上記以外
// デフォルトでは nullptr を返す．
const PtExpr*
CptControl::rep_expr() const
{
  return nullptr;
}


//////////////////////////////////////////////////////////////////////
// delay control を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptDelayControl::CptDelayControl(const FileRegion& file_region,
				 const PtExpr* delay) :
  mTopLoc(file_region.start_loc()),
  mDelay(delay)
{
  ASSERT_COND(delay );
}

// デストラクタ
CptDelayControl::~CptDelayControl()
{
}

// ファイル位置を返す．
FileRegion
CptDelayControl::file_region() const
{
  return FileRegion(mTopLoc, mDelay->file_region().end_loc());
}

// 型を返す．
tPtCtrlType
CptDelayControl::type() const
{
  return kPtDelayControl;
}

// 遅延式を返す．
const PtExpr*
CptDelayControl::delay() const
{
  return mDelay;
}


//////////////////////////////////////////////////////////////////////
// event を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptEventControl::CptEventControl(const FileRegion& file_region,
				 PtExprArray event_array) :
  mFileRegion(file_region),
  mEventArray(event_array)
{
}

// デストラクタ
CptEventControl::~CptEventControl()
{
}

// ファイル位置を返す．
FileRegion
CptEventControl::file_region() const
{
  return mFileRegion;
}

// 型を返す．
tPtCtrlType
CptEventControl::type() const
{
  return kPtEventControl;
}

// @brief イベントリストの取得
// @note event control/repeat control の場合のみ意味を持つ
PtExprArray
CptEventControl::event_list() const
{
  return mEventArray;
}


//////////////////////////////////////////////////////////////////////
// repeat 形式の event を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptRepeatControl::CptRepeatControl(const FileRegion& file_region,
				   const PtExpr* expr,
				   PtExprArray event_array) :
  mFileRegion(file_region),
  mRepExpr(expr),
  mEventArray(event_array)
{
  ASSERT_COND( expr );
}

// デストラクタ
CptRepeatControl::~CptRepeatControl()
{
}

// ファイル位置を返す．
FileRegion
CptRepeatControl::file_region() const
{
  return mFileRegion;
}

// 型を返す．
tPtCtrlType
CptRepeatControl::type() const
{
  return kPtRepeatControl;
}

// 繰り返し数を得る．
const PtExpr*
CptRepeatControl::rep_expr() const
{
  return mRepExpr;
}

// @brief イベントリストの取得
// @note event control/repeat control の場合のみ意味を持つ
PtExprArray
CptRepeatControl::event_list() const
{
  return mEventArray;
}


//////////////////////////////////////////////////////////////////////
// ordered_connection/named_connection を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptConnection::CptConnection(const FileRegion& file_region,
			     const PtExpr* expr) :
  mFileRegion(file_region),
  mExpr(expr)
{
}

// デストラクタ
CptConnection::~CptConnection()
{
}

// ファイル位置を取出す．
FileRegion
CptConnection::file_region() const
{
  return mFileRegion;
}

// @brief 名前の取得
// @retval 名前 named connection の場合
// @retval "" ordered connection の場合
// デフォルトでは nullptr を返す．
const char*
CptConnection::name() const
{
  return nullptr;
}

// 式を取出す
const PtExpr*
CptConnection::expr() const
{
  return mExpr;
}


//////////////////////////////////////////////////////////////////////
// ordered_connection を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptOrderedCon::CptOrderedCon(const FileRegion& file_region,
			     const PtExpr* expr) :
  CptConnection(file_region, expr)
{
}

// デストラクタ
CptOrderedCon::~CptOrderedCon()
{
}


//////////////////////////////////////////////////////////////////////
// named_connection を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptNamedCon::CptNamedCon(const FileRegion& file_region,
			 const char* name,
			 const PtExpr* expr) :
  CptConnection(file_region, expr),
  mName(name)
{
}

// デストラクタ
CptNamedCon::~CptNamedCon()
{
}

// 名前を取り出す．
const char*
CptNamedCon::name() const
{
  return mName;
}


//////////////////////////////////////////////////////////////////////
// strength を表すクラス
//////////////////////////////////////////////////////////////////////

// drive strength 用のコンストラクタ
CptStrength::CptStrength(const FileRegion& file_region,
			 tVpiStrength value1,
			 tVpiStrength value2) :
  mFileRegion(file_region),
  mValue{value1, value2, kVpiNoStrength}
{
}

// charge strength 用のコンストラクタ
CptStrength::CptStrength(const FileRegion& file_region,
			 tVpiStrength value1) :
  mFileRegion(file_region),
  mValue{kVpiNoStrength, kVpiNoStrength, value1}
{
}

// デストラクタ
CptStrength::~CptStrength()
{
}

// ファイル位置を取出す．
FileRegion
CptStrength::file_region() const
{
  return mFileRegion;
}

// drive strength0 を返す．
tVpiStrength
CptStrength::drive0() const
{
  return mValue[0];
}

// drive strength1 を返す．
tVpiStrength
CptStrength::drive1() const
{
  return mValue[1];
}

// charge strength を返す．
tVpiStrength
CptStrength::charge() const
{
  return mValue[2];
}


//////////////////////////////////////////////////////////////////////
// delay を表すクラス
//////////////////////////////////////////////////////////////////////

// 一つの値をとるコンストラクタ
CptDelay::CptDelay(const FileRegion& file_region,
		   const PtExpr* value1) :
  mFileRegion(file_region),
  mValue{value1, nullptr, nullptr}
{
}

// 二つの値をとるコンストラクタ
CptDelay::CptDelay(const FileRegion& file_region,
		   const PtExpr* value1,
		   const PtExpr* value2) :
  mFileRegion(file_region),
  mValue{value1, value2, nullptr}
{
}

// 三つの値をとるコンストラクタ
CptDelay::CptDelay(const FileRegion& file_region,
		   const PtExpr* value1,
		   const PtExpr* value2,
		   const PtExpr* value3) :
  mFileRegion(file_region),
  mValue{value1, value2, value3}
{
}

// デストラクタ
CptDelay::~CptDelay()
{
}

// ファイル位置を取出す．
FileRegion
CptDelay::file_region() const
{
  return mFileRegion;
}

// 値を取り出す．
const PtExpr*
CptDelay::value(SizeType pos) const
{
  if ( 0 <= pos && pos < 3 ) {
    return mValue[pos];
  }
  return nullptr;
}


//////////////////////////////////////////////////////////////////////
// 階層名を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptNameBranch::CptNameBranch(const char* name) :
  mName(name)
{
}

// デストラクタ
CptNameBranch::~CptNameBranch()
{
}

// 名前を取り出す．
const char*
CptNameBranch::name() const
{
  return mName;
}

// @brief インデックスの有無のチェック
// @retval true インデックスを持っている時
// @retval false インデックスを持っていない時
// デフォルトで false を返す．
bool
CptNameBranch::has_index() const
{
  return false;
}

// インデックスの取得
// @return インデックスの値
// デフォルトで 0 を返す．
int
CptNameBranch::index() const
{
  return 0;
}


//////////////////////////////////////////////////////////////////////
// インデックスつきの階層名を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptNameBranchI::CptNameBranchI(const char* name,
			       int index) :
  CptNameBranch(name),
  mIndex(index)
{
}

// デストラクタ
CptNameBranchI::~CptNameBranchI()
{
}

// インデックスを持っている時 true を返す．
bool
CptNameBranchI::has_index() const
{
  return true;
}

// インデックスを取り出す．
int
CptNameBranchI::index() const
{
  return mIndex;
}


//////////////////////////////////////////////////////////////////////
// attribute_instance を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptAttrInst::CptAttrInst(PtAttrSpecArray as_array) :
  mAttrSpecArray(as_array)
{
}

// デストラクタ
CptAttrInst::~CptAttrInst()
{
}

// @brief 要素のリストの取得
PtAttrSpecArray
CptAttrInst::attrspec_list() const
{
  return mAttrSpecArray;
}


//////////////////////////////////////////////////////////////////////
// attr_spec を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptAttrSpec::CptAttrSpec(const FileRegion& file_region,
			 const char* name,
			 const PtExpr* expr) :
  mFileRegion(file_region),
  mName(name),
  mExpr(expr)
{
}

// デストラクタ
CptAttrSpec::~CptAttrSpec()
{
}

// ファイル位置を返す．
FileRegion
CptAttrSpec::file_region() const
{
  return mFileRegion;
}

// 名前を取り出す．
const char*
CptAttrSpec::name() const
{
  return mName;
}

// 式を取り出す．nullptr の場合もある．
const PtExpr*
CptAttrSpec::expr() const
{
  return mExpr;
}


//////////////////////////////////////////////////////////////////////
// その他
//////////////////////////////////////////////////////////////////////

// ディレイコントロールを生成する．
const PtControl*
CptFactory::new_DelayControl(const FileRegion& file_region,
			     const PtExpr* value)
{
  ++ mNumDelayControl;
  void* p = alloc().get_memory(sizeof(CptDelayControl));
  return new (p) CptDelayControl(file_region, value);
}

// イベントコントロールを生成する．
const PtControl*
CptFactory::new_EventControl(const FileRegion& file_region,
			     PtExprArray event_array)
{
  ++ mNumEventControl;
  void* p = alloc().get_memory(sizeof(CptEventControl));
  return new (p) CptEventControl(file_region, event_array);
}

// リピートコントロールを生成する．
const PtControl*
CptFactory::new_RepeatControl(const FileRegion& file_region,
			      const PtExpr* expr,
			      PtExprArray event_array)
{
  ++ mNumRepeatControl;
  void* p = alloc().get_memory(sizeof(CptRepeatControl));
  return new (p) CptRepeatControl(file_region, expr, event_array);
}

// 順序つき結合子を生成する．
const PtConnection*
CptFactory::new_OrderedCon(const FileRegion& file_region,
			   const PtExpr* expr)
{
  ++ mNumAiOrderedCon;
  void* p = alloc().get_memory(sizeof(CptOrderedCon));
  return new (p) CptOrderedCon(file_region, expr);
}

// 順序つき結合子を生成する．
const PtConnection*
CptFactory::new_OrderedCon(const PtExpr* expr)
{
  ++ mNumOrderedCon;
  FileRegion file_region;
  if ( expr ) {
    file_region = expr->file_region();
  }
  void* p = alloc().get_memory(sizeof(CptOrderedCon));
  return new (p) CptOrderedCon(file_region, expr);
}

// 名前つき結合子を生成する．
const PtConnection*
CptFactory::new_NamedCon(const FileRegion& file_region,
			 const char* name,
			 const PtExpr* expr)
{
  ++ mNumNamedCon;
  void* p = alloc().get_memory(sizeof(CptNamedCon));
  return new (p) CptNamedCon(file_region, name, expr);
}

// strength を生成する．
const PtStrength*
CptFactory::new_Strength(const FileRegion& file_region,
			 tVpiStrength value1,
			 tVpiStrength value2)
{
  ++ mNumStrength;
  void* p = alloc().get_memory(sizeof(CptStrength));
  return new (p) CptStrength(file_region, value1, value2);
}

// strength を生成する．
const PtStrength*
CptFactory::new_Strength(const FileRegion& file_region,
			 tVpiStrength value1)
{
  ++ mNumStrength;
  void* p = alloc().get_memory(sizeof(CptStrength));
  return new (p) CptStrength(file_region, value1);
}

// delay 値を生成する．
const PtDelay*
CptFactory::new_Delay(const FileRegion& file_region,
		      const PtExpr* value1)
{
  ++ mNumDelay;
  void* p = alloc().get_memory(sizeof(CptDelay));
  return new (p) CptDelay(file_region, value1);
}

// delay 値を生成する．
const PtDelay*
CptFactory::new_Delay(const FileRegion& file_region,
		      const PtExpr* value1,
		      const PtExpr* value2)
{
  ++ mNumDelay;
  void* p = alloc().get_memory(sizeof(CptDelay));
  return new (p) CptDelay(file_region, value1, value2);
}

// delay 値を生成する．
const PtDelay*
CptFactory::new_Delay(const FileRegion& file_region,
		      const PtExpr* value1,
		      const PtExpr* value2,
		      const PtExpr* value3)
{
  ++ mNumDelay;
  void* p = alloc().get_memory(sizeof(CptDelay));
  return new (p) CptDelay(file_region, value1, value2, value3);
}

// 階層名を生成する．
const PtNameBranch*
CptFactory::new_NameBranch(const char* name)
{
  ++ mNumNameBranch;
  void* p = alloc().get_memory(sizeof(CptNameBranch));
  return new (p) CptNameBranch(name);
}

// 階層名を生成する．
const PtNameBranch*
CptFactory::new_NameBranch(const char* name,
			   int index)
{
  ++ mNumNameBranchI;
  void* p = alloc().get_memory(sizeof(CptNameBranchI));
  return new (p) CptNameBranchI(name, index);
}


//////////////////////////////////////////////////////////////////////
// attribute instance 関係
//////////////////////////////////////////////////////////////////////

// attribute instance を生成する．
const PtAttrInst*
CptFactory::new_AttrInst(const FileRegion& file_region,
			 PtAttrSpecArray as_array)
{
  // file_region は不要
  ++ mNumAttrInst;
  void* p = alloc().get_memory(sizeof(CptAttrInst));
  return new (p) CptAttrInst(as_array);
}

// attribute spec を生成する．
const PtAttrSpec*
CptFactory::new_AttrSpec(const FileRegion& file_region,
			 const char* name,
			 const PtExpr* expr)
{
  ++ mNumAttrSpec;
  void* p = alloc().get_memory(sizeof(CptAttrSpec));
  return new (p) CptAttrSpec(file_region, name, expr);
}

END_NAMESPACE_YM_VERILOG
