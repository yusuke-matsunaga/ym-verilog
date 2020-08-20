
/// @file CptItem.cc
/// @brief CptItem の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "CptItem.h"
#include "parser/CptFactory.h"
#include "parser/PuHierName.h"
#include "parser/PtiArray.h"
#include "ym/pt/PtDecl.h"
#include "ym/pt/PtStmt.h"
#include "ym/pt/PtExpr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// item の基底クラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CptItem::CptItem()
{
}

// @brief デストラクタ
CptItem::~CptItem()
{
}

// @brief プリミティブタイプの取得
// @return プリミティブタイプ
// このクラスでは kVpiAndPrim を返す．
VpiPrimType
CptItem::prim_type() const
{
  return VpiPrimType::And; // ダミー
}

// @brief strength の取得
// @return 信号強度
// このクラスでは nullptr を返す．
const PtStrength*
CptItem::strength() const
{
  return nullptr;
}

// @brief delay の取得
// @return 遅延
// このクラスでは nullptr を返す．
const PtDelay*
CptItem::delay() const
{
  return nullptr;
}

// @brief パラメータ割り当て数の取得
SizeType
CptItem::paramassign_num() const
{
  return 0;
}

// @brief パラメータ割り当ての取得
// @param[in] pos 位置 ( 0 <= pos < paramassign_num() )
const PtConnection*
CptItem::paramassign(SizeType pos) const
{
  ASSERT_NOT_REACHED;
  return nullptr;
}

// @brief defparam の要素数の取得
SizeType
CptItem::defparam_num() const
{
  return 0;
}

// @brief defparam の取得
// @param[in] pos 位置 ( 0 <= pos < defparam_num() )
const PtDefParam*
CptItem::defparam(SizeType pos) const
{
  ASSERT_NOT_REACHED;
  return nullptr;
}

// @brief continuous assign の要素数の取得
SizeType
CptItem::contassign_num() const
{
  return 0;
}

// @brief continuous assign の取得
// @param[in] pos 位置 ( 0 <= pos < contassign_num() )
const PtContAssign*
CptItem::contassign(SizeType pos) const
{
  ASSERT_NOT_REACHED;
  return nullptr;
}

// @brief module/UDP/gate instance の要素数の取得
SizeType
CptItem::inst_num() const
{
  return 0;
}

// @brief module/UDP/gate instance の取得
// @param[in] pos 位置 ( 0 <= pos < inst_num() )
const PtInst*
CptItem::inst(SizeType pos) const
{
  ASSERT_NOT_REACHED;
  return nullptr;
}

// @brief 名前の取得
// @return 名前
// このクラスでは nullptr を返す．
const char*
CptItem::name() const
{
  return nullptr;
}

// @brief automatic 情報の取得
// @retval true automatic 宣言された task/function
// @retval false 上記以外
// このクラスでは false を返す．
bool
CptItem::automatic() const
{
  return false;
}

// @brief IO宣言の要素数の取得
SizeType
CptItem::ioitem_num() const
{
  return 0;
}

// @brief IO宣言ヘッダリストの要素数の取得
SizeType
CptItem::iohead_num() const
{
  return 0;
}

// @brief IO宣言ヘッダの取得
// @param[in] pos 位置 ( 0 <= pos < iohead_num() )
const PtIOHead*
CptItem::iohead(SizeType pos) const
{
  ASSERT_NOT_REACHED;
  return nullptr;
}

// @brief 宣言ヘッダの要素数の取得
SizeType
CptItem::declhead_num() const
{
  return 0;
}

// @brief 宣言ヘッダの取得
// @param[in] pos 位置 ( 0 <= pos < declhead_num() )
const PtDeclHead*
CptItem::declhead(SizeType pos) const
{
  ASSERT_NOT_REACHED;
  return nullptr;
}

// @brief item リストの要素数の取得
SizeType
CptItem::item_num() const
{
  return 0;
}

// @brief item の取得
// @param[in] pos 位置 ( 0 <= pos < item_num() )
const PtItem*
CptItem::item(SizeType pos) const
{
  ASSERT_NOT_REACHED;
  return nullptr;
}

// @brief 本体のステートメントの取得
// @return 本体のステートメント
// このクラスでは nullptr を返す．
const PtStmt*
CptItem::body() const
{
  return nullptr;
}

// @brief 符号の取得
// @retval true 符号つき
// @retval false 符号なし
// このクラスでは false を返す．
bool
CptItem::is_signed() const
{
  return false;
}

// @brief 範囲の左側の式の取得
// @return 範囲の左側の式
// このクラスでは nullptr を返す．
const PtExpr*
CptItem::left_range() const
{
  return nullptr;
}

// @brief 範囲の右側の式の取得
// @return 範囲の右側の式
// このクラスでは nullptr を返す．
const PtExpr*
CptItem::right_range() const
{
  return nullptr;
}

// @brief 戻値のデータ型の取得
// @return 戻値のデータ型
// このクラスでは kVpiVarNone を返す．
VpiVarType
CptItem::data_type() const
{
  return VpiVarType::None;
}

// @brief constant function の展開中の印をつける．
// このクラスではなにもしない．
void
CptItem::set_in_use() const
{
}

// @brief constant function の展開中の印を消す．
// このクラスではなにもしない．
void
CptItem::clear_in_use() const
{
}

// @brief 使用中(constant function として展開中)のチェック
// @return 使用中の時 true を返す．
// このクラスでは false を返す．
bool
CptItem::is_in_use() const
{
  return false;
}

// @brief specify block item の種類の取得
// @return specify block item の種類
// このクラスでは kVpiPulsestyleOnEvent を返す．
VpiSpecItemType
CptItem::specitem_type() const
{
  return VpiSpecItemType::PulsestyleOnEvent; // ダミー
}

// @brief specify block path の種類の取得
// @retun specify block path の種類
// このクラスでは kVpiSpecPathNull を返す．
VpiSpecPathType
CptItem::specpath_type() const
{
  return VpiSpecPathType::Null; // ダミー
}

// @brief ターミナルの要素数の取得
SizeType
CptItem::terminal_num() const
{
  return 0;
}

// @brief ターミナルの取得
// @param[in] pos 位置 ( 0 <= pos < terminal_num() )
const PtExpr*
CptItem::terminal(SizeType pos) const
{
  ASSERT_NOT_REACHED;
  return nullptr;
}

// @brief パス記述の取得
// @return パス記述
// このクラスでは nullptr を返す．
const PtPathDecl*
CptItem::path_decl() const
{
  return nullptr;
}

// @brief 条件式の取得
// @return 条件式
// このクラスでは nullptr を返す．
const PtExpr*
CptItem::expr() const
{
  return nullptr;
}

// @brief 条件が成り立ったときに生成される宣言ヘッダ配列の要素数の取得
SizeType
CptItem::then_declhead_num() const
{
  return 0;
}

// @brief 条件が成り立ったときに生成される宣言ヘッダの取得
// @param[in] pos 位置 ( 0 <= pos < then_declhead_num() )
const PtDeclHead*
CptItem::then_declhead(SizeType pos) const
{
  ASSERT_NOT_REACHED;
  return nullptr;
}

// @brief 条件が成り立ったときに生成される item 配列の要素数の取得
SizeType
CptItem::then_item_num() const
{
  return 0;
}

// @brief 条件が成り立ったときに生成される item の取得
// @param[in] pos 位置 ( 0 <= pos < then_item_num() )
const PtItem*
CptItem::then_item(SizeType pos) const
{
  ASSERT_NOT_REACHED;
  return nullptr;
}

// @brief 条件が成り立たなかったときに生成される宣言ヘッダ配列の要素数の取得
SizeType
CptItem::else_declhead_num() const
{
  return 0;
}

// @brief 条件が成り立たなかったときに生成される宣言ヘッダの取得
// @param[in] pos 位置 ( 0 <= pos < else_declhead_num() )
const PtDeclHead*
CptItem::else_declhead(SizeType pos) const
{
  ASSERT_NOT_REACHED;
  return nullptr;
}

// @brief 条件が成り立たなかったときに生成される item 配列の要素数の取得
SizeType
CptItem::else_item_num() const
{
  return 0;
}

// @brief 条件が成り立たなかったときに生成される item の取得
// @param[in] pos 位置 ( 0 <= pos < else_item_num() )
const PtItem*
CptItem::else_item(SizeType pos) const
{
  ASSERT_NOT_REACHED;
  return nullptr;
}

// @brief case item のリストの要素数の取得
SizeType
CptItem::caseitem_num() const
{
  return 0;
}

// @brief case item の取得
// @param[in] pos 位置 ( 0 <= pos < caseitem_num() )
const PtGenCaseItem*
CptItem::caseitem(SizeType pos) const
{
  ASSERT_NOT_REACHED;
  return nullptr;
}

// @brief 繰り返し制御用の変数名の取得
// @return 繰り返し制御用の変数名
// このクラスでは nullptr を返す．
const char*
CptItem::loop_var() const
{
  return nullptr;
}

// @brief 初期化文の右辺の取得
// @return 初期化文の右辺
// このクラスでは nullptr を返す．
const PtExpr*
CptItem::init_expr() const
{
  return nullptr;
}

// @brief 増加文の右辺の取得
// @return 増加文の右辺
// このクラスでは nullptr を返す．
const PtExpr*
CptItem::next_expr() const
{
  return nullptr;
}


//////////////////////////////////////////////////////////////////////
// defparam 文のヘッダ
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptDefParamH::CptDefParamH(const FileRegion& file_region,
			   PtiDefParamArray&& dp_array) :
  mFileRegion(file_region),
  mArray{move(dp_array)}
{
}

// デストラクタ
CptDefParamH::~CptDefParamH()
{
}

// ファイル位置を返す．
FileRegion
CptDefParamH::file_region() const
{
  return mFileRegion;
}

// 型を返す．
PtItemType
CptDefParamH::type() const
{
  return PtItemType::DefParam;
}

// @brief defparam の要素数の取得
SizeType
CptDefParamH::defparam_num() const
{
  return mArray.size();
}

// @brief defparam の取得
// @param[in] pos 位置 ( 0 <= pos < defparam_num() )
const PtDefParam*
CptDefParamH::defparam(SizeType pos) const
{
  return mArray[pos];
}


//////////////////////////////////////////////////////////////////////
// defparam 文のベース実装クラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptDefParam::CptDefParam(const FileRegion& file_region,
			 PtiNameBranchArray&& nb_array,
			 const char* tail_name,
			 const PtExpr* value) :
  mTopLoc(file_region.start_loc()),
  mNbArray{move(nb_array)},
  mName(tail_name),
  mExpr(value)
{
  ASSERT_COND( value );
}

// デストラクタ
CptDefParam::~CptDefParam()
{
}

// ファイル位置を返す．
FileRegion
CptDefParam::file_region() const
{
  return FileRegion(mTopLoc, mExpr->file_region().end_loc());
}

// @brief 階層ブランチの要素数の取得
SizeType
CptDefParam::namebranch_num() const
{
  return mNbArray.size();
}

// @brief 階層ブランチの取得
// @param[in] pos 位置 ( 0 <= pos < namebranch_num() )
const PtNameBranch*
CptDefParam::namebranch(SizeType pos) const
{
  return mNbArray[pos];
}

// 末尾の名前を返す．
const char*
CptDefParam::name() const
{
  return mName;
}

// 値を返す．
const PtExpr*
CptDefParam::expr() const
{
  return mExpr;
}


//////////////////////////////////////////////////////////////////////
// continuous assign 文のヘッダ
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptContAssignH::CptContAssignH(const FileRegion& file_region,
			       PtiContAssignArray&& ca_array) :
  mFileRegion(file_region),
  mArray{move(ca_array)}
{
}

// デストラクタ
CptContAssignH::~CptContAssignH()
{
}

// ファイル位置を返す．
FileRegion
CptContAssignH::file_region() const
{
  return mFileRegion;
}

// 型を返す．
PtItemType
CptContAssignH::type() const
{
  return PtItemType::ContAssign;
}

// @brief continuous assign の要素数の取得
SizeType
CptContAssignH::contassign_num() const
{
  return mArray.size();
}

// @brief continuous assign の取得
// @param[in] pos 位置 ( 0 <= pos < contassign_num() )
const PtContAssign*
CptContAssignH::contassign(SizeType pos) const
{
  return mArray[pos];
}


//////////////////////////////////////////////////////////////////////
// strength つきの continuous assign 文のヘッダ
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptContAssignHS::CptContAssignHS(const FileRegion& file_region,
				 const PtStrength* strength,
				 PtiContAssignArray&& ca_array) :
  CptContAssignH(file_region, move(ca_array)),
  mStrength(strength)
{
}

// デストラクタ
CptContAssignHS::~CptContAssignHS()
{
}

// strength を返す．
const PtStrength*
CptContAssignHS::strength() const
{
  return mStrength;
}


//////////////////////////////////////////////////////////////////////
// delay つきの continuous assign 文のヘッダ
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptContAssignHD::CptContAssignHD(const FileRegion& file_region,
				 const PtDelay* delay,
				 PtiContAssignArray&& ca_array) :
  CptContAssignH(file_region, move(ca_array)),
  mDelay(delay)
{
}

// デストラクタ
CptContAssignHD::~CptContAssignHD()
{
}

// delay を返す．
const PtDelay*
CptContAssignHD::delay() const
{
  return mDelay;
}


//////////////////////////////////////////////////////////////////////
// strength と delay つきの continuous assign 文のヘッダ
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptContAssignHSD::CptContAssignHSD(const FileRegion& file_region,
				   const PtStrength* strength,
				   const PtDelay* delay,
				   PtiContAssignArray&& ca_array) :
  CptContAssignH(file_region, move(ca_array)),
  mStrength(strength),
  mDelay(delay)
{
}

// デストラクタ
CptContAssignHSD::~CptContAssignHSD()
{
}

// strength を返す．
const PtStrength*
CptContAssignHSD::strength() const
{
  return mStrength;
}

// delay を返す．
const PtDelay*
CptContAssignHSD::delay() const
{
  return mDelay;
}


//////////////////////////////////////////////////////////////////////
// continuous assign文 のベース実装クラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptContAssign::CptContAssign(const PtExpr* lhs,
			     const PtExpr* rhs) :
  mLhs(lhs),
  mRhs(rhs)
{
}

// デストラクタ
CptContAssign::~CptContAssign()
{
}

// ファイル位置を取り出す．
FileRegion
CptContAssign::file_region() const
{
  return FileRegion(lhs()->file_region(), rhs()->file_region());
}

// 左辺式を取り出す．
const PtExpr*
CptContAssign::lhs() const
{
  return mLhs;
}

// 右辺式を取り出す．
const PtExpr*
CptContAssign::rhs() const
{
  return mRhs;
}


//////////////////////////////////////////////////////////////////////
// initial文/always文に共通なクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptProcess::CptProcess(const FileRegion& file_region,
		       const PtStmt* body) :
  mTopLoc(file_region.start_loc()),
  mBody(body)
{
  ASSERT_COND(body );
}

// デストラクタ
CptProcess::~CptProcess()
{
}

// ファイル位置を返す．
FileRegion
CptProcess::file_region() const
{
  return FileRegion(mTopLoc, mBody->file_region().end_loc());
}

// 本体のステートメントを返す．
const PtStmt*
CptProcess::body() const
{
  return mBody;
}


//////////////////////////////////////////////////////////////////////
// initial文 を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptInitial::CptInitial(const FileRegion& file_region,
		       const PtStmt* body) :
  CptProcess(file_region, body)
{
}

// デストラクタ
CptInitial::~CptInitial()
{
}

// 型を返す．
PtItemType
CptInitial::type() const
{
  return PtItemType::Initial;
}


//////////////////////////////////////////////////////////////////////
// always文 を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptAlways::CptAlways(const FileRegion& file_region,
		     const PtStmt* body) :
  CptProcess(file_region, body)
{
}

// デストラクタ
CptAlways::~CptAlways()
{
}

// 型を返す．
PtItemType
CptAlways::type() const
{
  return PtItemType::Always;
}


//////////////////////////////////////////////////////////////////////
// function と task の共通の親クラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptTf::CptTf(const FileRegion& file_region,
	     const char* name,
	     bool automatic,
	     PtiIOHeadArray&& iohead_array,
	     PtiDeclHeadArray&& declhead_array,
	     const PtStmt* stmt) :
  mFileRegion(file_region),
  mName(name),
  mAutomatic(automatic),
  mIOHeadArray{move(iohead_array)},
  mDeclHeadArray{move(declhead_array)},
  mBody(stmt)
{
  int n = 0;
  for ( auto head: mIOHeadArray ) {
    n += head->item_num();
  }
  mIOItemNum = n;
}

// デストラクタ
CptTf::~CptTf()
{
}

// ファイル位置を返す．
FileRegion
CptTf::file_region() const
{
  return mFileRegion;
}

// 名前を返す．
const char*
CptTf::name() const
{
  return mName;
}

// automatic なら true
bool
CptTf::automatic() const
{
  return mAutomatic;
}

// @brief IO宣言の要素数の取得
SizeType
CptTf::ioitem_num() const
{
  return mIOItemNum;
}

// @brief IO宣言ヘッダリストの要素数の取得
SizeType
CptTf::iohead_num() const
{
  return mIOHeadArray.size();
}

// @brief IO宣言ヘッダの取得
// @param[in] pos 位置 ( 0 <= pos < iohead_num() )
const PtIOHead*
CptTf::iohead(SizeType pos) const
{
  return mIOHeadArray[pos];
}

// @brief 宣言ヘッダの要素数の取得
SizeType
CptTf::declhead_num() const
{
  return mDeclHeadArray.size();
}

// @brief 宣言ヘッダの取得
// @param[in] pos 位置 ( 0 <= pos < declhead_num() )
const PtDeclHead*
CptTf::declhead(SizeType pos) const
{
  return mDeclHeadArray[pos];
}

// 本体を取り出す．
const PtStmt*
CptTf::body() const
{
  return mBody;
}


//////////////////////////////////////////////////////////////////////
// task を表すノード
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptTask::CptTask(const FileRegion& file_region,
		 const char* name,
		 bool automatic,
		 PtiIOHeadArray&& iohead_array,
		 PtiDeclHeadArray&& declhead_array,
		 const PtStmt* stmt) :
  CptTf(file_region,
	name, automatic,
	move(iohead_array),
	move(declhead_array),
	stmt)
{
}

// デストラクタ
CptTask::~CptTask()
{
}

// 型を返す．
PtItemType
CptTask::type() const
{
  return PtItemType::Task;
}


//////////////////////////////////////////////////////////////////////
// function を表すノード
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptFunction::CptFunction(const FileRegion& file_region,
			 const char* name,
			 bool automatic,
			 bool sign,
			 PtiIOHeadArray&& iohead_array,
			 PtiDeclHeadArray&& declhead_array,
			 const PtStmt* stmt) :
  CptTf(file_region,
	name, automatic,
	move(iohead_array),
	move(declhead_array),
	stmt),
  mSigned(sign)
{
  mInUse = false;
}

// デストラクタ
CptFunction::~CptFunction()
{
}

// 型を返す．
PtItemType
CptFunction::type() const
{
  return PtItemType::Func;
}

// 符号の有無を返す．
bool
CptFunction::is_signed() const
{
  return mSigned;
}

// constant function の展開中の印をつける．
void
CptFunction::set_in_use() const
{
  mInUse = true;
}

// constant function の展開中の印を消す．
void
CptFunction::clear_in_use() const
{
  mInUse = false;
}

// 使用中(constant function として展開中)の時 true を返す．
bool
CptFunction::is_in_use() const
{
  return mInUse;
}


//////////////////////////////////////////////////////////////////////
// ビットベクタタイプの function を表すノード
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptSizedFunc::CptSizedFunc(const FileRegion& file_region,
			   const char* name,
			   bool automatic,
			   bool sign,
			   const PtExpr* left,
			   const PtExpr* right,
			   PtiIOHeadArray&& iohead_array,
			   PtiDeclHeadArray&& declhead_array,
			   const PtStmt* stmt) :
  CptFunction(file_region,
	      name, automatic, sign,
	      move(iohead_array),
	      move(declhead_array),
	      stmt),
  mLeftRange(left),
  mRightRange(right)
{
}

// デストラクタ
CptSizedFunc::~CptSizedFunc()
{
}

// 範囲の MSB を得る．
const PtExpr*
CptSizedFunc::left_range() const
{
  return mLeftRange;
}

// 範囲の LSB を得る．
const PtExpr*
CptSizedFunc::right_range() const
{
  return mRightRange;
}


//////////////////////////////////////////////////////////////////////
/// 組み込み型の function を表すノード
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptTypedFunc::CptTypedFunc(const FileRegion& file_region,
			   const char* name,
			   bool automatic,
			   bool sign,
			   VpiVarType data_type,
			   PtiIOHeadArray&& iohead_array,
			   PtiDeclHeadArray&& declhead_array,
			   const PtStmt* stmt) :
  CptFunction(file_region,
	      name, automatic, sign,
	      move(iohead_array),
	      move(declhead_array),
	      stmt),
  mDataType(data_type)
{
}

/// デストラクタ
CptTypedFunc::~CptTypedFunc()
{
}

// 戻値の型を返す．
VpiVarType
CptTypedFunc::data_type() const
{
  return mDataType;
}


//////////////////////////////////////////////////////////////////////
// item 関係
//////////////////////////////////////////////////////////////////////

// defparam 文のヘッダを生成する．
const PtItem*
CptFactory::new_DefParamH(const FileRegion& file_region,
			  const vector<const PtDefParam*>& elem_array)
{
  ++ mNumDefParamH;
  void* p{mAlloc.get_memory(sizeof(CptDefParamH))};
  auto obj{new (p) CptDefParamH(file_region,
				PtiArray<const PtDefParam>(mAlloc, elem_array))};
  return obj;
}

// defparam 文の要素を生成する．
const PtDefParam*
CptFactory::new_DefParam(const FileRegion& file_region,
			 const char* name,
			 const PtExpr* value)
{
  ++ mNumDefParam;
  void* p{mAlloc.get_memory(sizeof(CptDefParam))};
  auto obj{new (p) CptDefParam(file_region,
			       PtiArray<const PtNameBranch>(),
			       name, value)};
  return obj;
}

const PtDefParam*
CptFactory::new_DefParam(const FileRegion& file_region,
			 PuHierName* hname,
			 const PtExpr* value)
{
  ++ mNumDefParam;
  void* p{mAlloc.get_memory(sizeof(CptDefParam))};
  auto nb_array = hname->name_branch();
  auto tail_name = hname->tail_name();
  auto obj{new (p) CptDefParam(file_region,
			       PtiArray<const PtNameBranch>(mAlloc, nb_array),
			       tail_name, value)};
  return obj;
}

// continuous assign 文のヘッダを生成する．
const PtItem*
CptFactory::new_ContAssignH(const FileRegion& file_region,
			    const vector<const PtContAssign*>& elem_array)
{
  ++ mNumContAssignH;
  void* p{mAlloc.get_memory(sizeof(CptContAssignH))};
  auto obj{new (p) CptContAssignH(file_region,
				  PtiArray<const PtContAssign>(mAlloc, elem_array))};
  return obj;
}

// continuous assign 文のヘッダを生成する．
const PtItem*
CptFactory::new_ContAssignH(const FileRegion& file_region,
			    const PtStrength* strength,
			    const vector<const PtContAssign*>& elem_array)
{
  ++ mNumContAssignHS;
  void* p{mAlloc.get_memory(sizeof(CptContAssignHS))};
  auto obj{new (p) CptContAssignHS(file_region, strength,
				   PtiArray<const PtContAssign>(mAlloc, elem_array))};
  return obj;
}

// continuous assign 文のヘッダを生成する．
const PtItem*
CptFactory::new_ContAssignH(const FileRegion& file_region,
			    const PtDelay* delay,
			    const vector<const PtContAssign*>& elem_array)
{
  ++ mNumContAssignHD;
  void* p{mAlloc.get_memory(sizeof(CptContAssignHD))};
  auto obj{new (p) CptContAssignHD(file_region, delay,
				   PtiArray<const PtContAssign>(mAlloc, elem_array))};
  return obj;
}

// continuous assign 文のヘッダを生成する．
const PtItem*
CptFactory::new_ContAssignH(const FileRegion& file_region,
			    const PtStrength* strength,
			    const PtDelay* delay,
			    const vector<const PtContAssign*>& elem_array)
{
  ++ mNumContAssignHSD;
  void* p{mAlloc.get_memory(sizeof(CptContAssignHSD))};
  auto obj{new (p) CptContAssignHSD(file_region, strength, delay,
				    PtiArray<const PtContAssign>(mAlloc, elem_array))};
  return obj;
}

// continuous assign 文の要素を生成する．
const PtContAssign*
CptFactory::new_ContAssign(const FileRegion& file_region,
			   const PtExpr* lhs,
			   const PtExpr* rhs)
{
  ++ mNumContAssign;
  // 実は file_region は不要
  void* p{mAlloc.get_memory(sizeof(CptContAssign))};
  auto obj{new (p) CptContAssign(lhs, rhs)};
  return obj;
}

// initial 文を生成する．
const PtItem*
CptFactory::new_Initial(const FileRegion& file_region,
			const PtStmt* body)
{
  ++ mNumInitial;
  void* p{mAlloc.get_memory(sizeof(CptInitial))};
  auto obj{new (p) CptInitial(file_region, body)};
  return obj;
}

// always 文を生成する．
const PtItem*
CptFactory::new_Always(const FileRegion& file_region,
		       const PtStmt* body)
{
  ++ mNumAlways;
  void* p{mAlloc.get_memory(sizeof(CptAlways))};
  auto obj{new (p) CptAlways(file_region, body)};
  return obj;
}

// task 文を生成する．
const PtItem*
CptFactory::new_Task(const FileRegion& file_region,
		     const char* name,
		     bool automatic,
		     const vector<const PtIOHead*>& iohead_array,
		     const vector<const PtDeclHead*>& declhead_array,
		     const PtStmt* stmt)
{
  ++ mNumTask;
  void* p{mAlloc.get_memory(sizeof(CptTask))};
  auto obj{new (p) CptTask(file_region, name, automatic,
			   PtiArray<const PtIOHead>(mAlloc, iohead_array),
			   PtiArray<const PtDeclHead>(mAlloc, declhead_array),
			   stmt)};
  return obj;
}

// function 文を生成する．
const PtItem*
CptFactory::new_Function(const FileRegion& file_region,
			 const char* name,
			 bool automatic,
			 bool sign,
			 const vector<const PtIOHead*>& iohead_array,
			 const vector<const PtDeclHead*>& declhead_array,
			 const PtStmt* stmt)
{
  ++ mNumFunction;
  void* p{mAlloc.get_memory(sizeof(CptFunction))};
  auto obj{new (p) CptFunction(file_region, name, automatic, sign,
			       PtiArray<const PtIOHead>(mAlloc, iohead_array),
			       PtiArray<const PtDeclHead>(mAlloc, declhead_array),
			       stmt)};
  return obj;
}

// ビットベクタ型 function の生成
const PtItem*
CptFactory::new_SizedFunc(const FileRegion& file_region,
			  const char* name,
			  bool automatic,
			  bool sign,
			  const PtExpr* left,
			  const PtExpr* right,
			  const vector<const PtIOHead*>& iohead_array,
			  const vector<const PtDeclHead*>& declhead_array,
			  const PtStmt* stmt)
{
  ++ mNumSizedFunc;
  void* p{mAlloc.get_memory(sizeof(CptSizedFunc))};
  auto obj{new (p) CptSizedFunc(file_region,
				name, automatic,
				sign, left, right,
				PtiArray<const PtIOHead>(mAlloc, iohead_array),
				PtiArray<const PtDeclHead>(mAlloc, declhead_array),
				stmt)};
  return obj;
}

// 組み込み型 function の生成
const PtItem*
CptFactory::new_TypedFunc(const FileRegion& file_region,
			  const char* name,
			  bool automatic,
			  bool sign,
			  VpiVarType func_type,
			  const vector<const PtIOHead*>& iohead_array,
			  const vector<const PtDeclHead*>& declhead_array,
			  const PtStmt* stmt)
{
  ++ mNumTypedFunc;
  void* p{mAlloc.get_memory(sizeof(CptTypedFunc))};
  auto obj{new (p) CptTypedFunc(file_region, name,
				automatic, sign,
				func_type,
				PtiArray<const PtIOHead>(mAlloc, iohead_array),
				PtiArray<const PtDeclHead>(mAlloc, declhead_array),
				stmt)};
  return obj;
}

END_NAMESPACE_YM_VERILOG
