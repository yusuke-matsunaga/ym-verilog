﻿
/// @file SptItem.cc
/// @brief SptItem の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "SptItem.h"
#include "parser/SptFactory.h"
#include "parser/PuHierName.h"
#include "ym/pt/PtDecl.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// item の基底クラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
// @param file_region ファイル位置の情報
// @param type 種類
SptItem::SptItem(const FileRegion& file_region,
		 PtItemType type) :
  mFileRegion{file_region},
  mType{type}
{
}

// デストラクタ
SptItem::~SptItem()
{
}

// ファイル位置の取得
// @return ファイル位置
FileRegion
SptItem::file_region() const
{
  return mFileRegion;
}

// 型の取得
// @return 型
PtItemType
SptItem::type() const
{
  return mType;
}

// @brief プリミティブタイプの取得
// @return プリミティブタイプ
// このクラスでは VpiPrimType::And を返す．
VpiPrimType
SptItem::prim_type() const
{
  return VpiPrimType::And; // ダミー
}

// @brief strength の取得
// @return 信号強度
// このクラスでは nullptr を返す．
const PtStrength*
SptItem::strength() const
{
  return nullptr;
}

// @brief delay の取得
// @return 遅延
// このクラスでは nullptr を返す．
const PtDelay*
SptItem::delay() const
{
  return nullptr;
}

// @brief パラメータ割り当て数の取得
SizeType
SptItem::paramassign_num() const
{
  return 0;
}

// @brief パラメータ割り当ての取得
// @param[in] pos 位置 ( 0 <= pos < paramassign_num() )
const PtConnection*
SptItem::paramassign(SizeType pos) const
{
  ASSERT_NOT_REACHED;
  return nullptr;
}

// @brief defparam の要素数の取得
SizeType
SptItem::defparam_num() const
{
  return 0;
}

// @brief defparam の取得
// @param[in] pos 位置 ( 0 <= pos < defparam_num() )
const PtDefParam*
SptItem::defparam(SizeType pos) const
{
  ASSERT_NOT_REACHED;
  return nullptr;
}

// @brief continuous assign の要素数の取得
SizeType
SptItem::contassign_num() const
{
  return 0;
}

// @brief continuous assign の取得
// @param[in] pos 位置 ( 0 <= pos < contassign_num() )
const PtContAssign*
SptItem::contassign(SizeType pos) const
{
  ASSERT_NOT_REACHED;
  return nullptr;
}

// @brief module/UDP/gate instance の要素数の取得
SizeType
SptItem::inst_num() const
{
  return 0;
}

// @brief module/UDP/gate instance の取得
// @param[in] pos 位置 ( 0 <= pos < inst_num() )
const PtInst*
SptItem::inst(SizeType pos) const
{
  ASSERT_NOT_REACHED;
  return nullptr;
}

// @brief 名前の取得
// @return 名前
// このクラスでは nullptr を返す．
const char*
SptItem::name() const
{
  return nullptr;
}

// @brief automatic 情報の取得
// @retval true automatic 宣言された task/function
// @retval false 上記以外
// このクラスでは false を返す．
bool
SptItem::automatic() const
{
  return false;
}

// @brief IO宣言の要素数の取得
SizeType
SptItem::ioitem_num() const
{
  return 0;
}

// @brief IO宣言ヘッダリストの要素数の取得
SizeType
SptItem::iohead_num() const
{
  return 0;
}

// @brief IO宣言ヘッダの取得
// @param[in] pos 位置 ( 0 <= pos < iohead_num() )
const PtIOHead*
SptItem::iohead(SizeType pos) const
{
  ASSERT_NOT_REACHED;
  return nullptr;
}

// @brief 宣言ヘッダの要素数の取得
SizeType
SptItem::declhead_num() const
{
  return 0;
}

// @brief 宣言ヘッダの取得
// @param[in] pos 位置 ( 0 <= pos < declhead_num() )
const PtDeclHead*
SptItem::declhead(SizeType pos) const
{
  ASSERT_NOT_REACHED;
  return nullptr;
}

// @brief item リストの要素数の取得
SizeType
SptItem::item_num() const
{
  return 0;
}

// @brief item の取得
// @param[in] pos 位置 ( 0 <= pos < item_num() )
const PtItem*
SptItem::item(SizeType pos) const
{
  ASSERT_NOT_REACHED;
  return nullptr;
}

// @brief 本体のステートメントの取得
// @return 本体のステートメント
// このクラスでは nullptr を返す．
const PtStmt*
SptItem::body() const
{
  return nullptr;
}

// @brief 符号の取得
// @retval true 符号つき
// @retval false 符号なし
// このクラスでは false を返す．
bool
SptItem::is_signed() const
{
  return false;
}

// @brief 範囲の左側の式の取得
// @return 範囲の左側の式
// このクラスでは nullptr を返す．
const PtExpr*
SptItem::left_range() const
{
  return nullptr;
}

// @brief 範囲の右側の式の取得
// @return 範囲の右側の式
// このクラスでは nullptr を返す．
const PtExpr*
SptItem::right_range() const
{
  return nullptr;
}

// @brief 戻値のデータ型の取得
// @return 戻値のデータ型
// このクラスでは VpiVarType::None を返す．
VpiVarType
SptItem::data_type() const
{
  return VpiVarType::None;
}

// @brief constant function の展開中の印をつける．
// このクラスではなにもしない．
void
SptItem::set_in_use() const
{
}

// @brief constant function の展開中の印を消す．
// このクラスではなにもしない．
void
SptItem::clear_in_use() const
{
}

// @brief 使用中(constant function として展開中)のチェック
// @return 使用中の時 true を返す．
// このクラスでは false を返す．
bool
SptItem::is_in_use() const
{
  return false;
}

// @brief specify block item の種類の取得
// @return specify block item の種類
// このクラスでは PulsestyleOnEvent を返す．
VpiSpecItemType
SptItem::specitem_type() const
{
  return VpiSpecItemType::PulsestyleOnEvent; // ダミー
}

// @brief specify block path の種類の取得
// @retun specify block path の種類
// このクラスでは Null を返す．
VpiSpecPathType
SptItem::specpath_type() const
{
  return VpiSpecPathType::Null; // ダミー
}

// @brief ターミナルの要素数の取得
SizeType
SptItem::terminal_num() const
{
  return 0;
}

// @brief ターミナルの取得
// @param[in] pos 位置 ( 0 <= pos < terminal_num() )
const PtExpr*
SptItem::terminal(SizeType pos) const
{
  ASSERT_NOT_REACHED;
  return nullptr;
}

// @brief パス記述の取得
// @return パス記述
// このクラスでは nullptr を返す．
const PtPathDecl*
SptItem::path_decl() const
{
  return nullptr;
}

// @brief 条件式の取得
// @return 条件式
// このクラスでは nullptr を返す．
const PtExpr*
SptItem::expr() const
{
  return nullptr;
}

// @brief 条件が成り立ったときに生成される宣言ヘッダ配列の要素数の取得
SizeType
SptItem::then_declhead_num() const
{
  return 0;
}

// @brief 条件が成り立ったときに生成される宣言ヘッダの取得
// @param[in] pos 位置 ( 0 <= pos < then_declhead_num() )
const PtDeclHead*
SptItem::then_declhead(SizeType pos) const
{
  ASSERT_NOT_REACHED;
  return nullptr;
}

// @brief 条件が成り立ったときに生成される item 配列の要素数の取得
SizeType
SptItem::then_item_num() const
{
  return 0;
}

// @brief 条件が成り立ったときに生成される item の取得
// @param[in] pos 位置 ( 0 <= pos < then_item_num() )
const PtItem*
SptItem::then_item(SizeType pos) const
{
  ASSERT_NOT_REACHED;
  return nullptr;
}

// @brief 条件が成り立たなかったときに生成される宣言ヘッダ配列の要素数の取得
SizeType
SptItem::else_declhead_num() const
{
  return 0;
}

// @brief 条件が成り立たなかったときに生成される宣言ヘッダの取得
// @param[in] pos 位置 ( 0 <= pos < else_declhead_num() )
const PtDeclHead*
SptItem::else_declhead(SizeType pos) const
{
  ASSERT_NOT_REACHED;
  return nullptr;
}

// @brief 条件が成り立たなかったときに生成される item 配列の要素数の取得
SizeType
SptItem::else_item_num() const
{
  return 0;
}

// @brief 条件が成り立たなかったときに生成される item の取得
// @param[in] pos 位置 ( 0 <= pos < else_item_num() )
const PtItem*
SptItem::else_item(SizeType pos) const
{
  ASSERT_NOT_REACHED;
  return nullptr;
}

// @brief case item のリストの要素数の取得
SizeType
SptItem::caseitem_num() const
{
  return 0;
}

// @brief case item の取得
// @param[in] pos 位置 ( 0 <= pos < caseitem_num() )
const PtGenCaseItem*
SptItem::caseitem(SizeType pos) const
{
  ASSERT_NOT_REACHED;
  return nullptr;
}

// @brief 繰り返し制御用の変数名の取得
// @return 繰り返し制御用の変数名
// このクラスでは nullptr を返す．
const char*
SptItem::loop_var() const
{
  return nullptr;
}

// @brief 初期化文の右辺の取得
// @return 初期化文の右辺
// このクラスでは nullptr を返す．
const PtExpr*
SptItem::init_expr() const
{
  return nullptr;
}

// @brief 増加文の右辺の取得
// @return 増加文の右辺
// このクラスでは nullptr を返す．
const PtExpr*
SptItem::next_expr() const
{
  return nullptr;
}


//////////////////////////////////////////////////////////////////////
// defparam 文のヘッダ
//////////////////////////////////////////////////////////////////////

// コンストラクタ
// @param file_region ファイル位置の情報
// @param dplist defparam のリスト
// dplist は削除される．
SptDefParamH::SptDefParamH(const FileRegion& file_region,
			   PtiDefParamArray&& dp_array) :
  SptItem{file_region, PtItemType::DefParam},
  mArray{move(dp_array)}
{
}

// デストラクタ
SptDefParamH::~SptDefParamH()
{
}

// @brief defparam の要素数の取得
SizeType
SptDefParamH::defparam_num() const
{
  return mArray.size();
}

// @brief defparam の取得
// @param[in] pos 位置 ( 0 <= pos < defparam_num() )
const PtDefParam*
SptDefParamH::defparam(SizeType pos) const
{
  return mArray[pos];
}


//////////////////////////////////////////////////////////////////////
// defparam 文のベース実装クラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
// @param file_region ファイル位置の情報
// @param nblist 階層名のリスト
// @param tail_name 末尾の名前
// @param value 値を表す式
SptDefParam::SptDefParam(const FileRegion& file_region,
			 PtiNameBranchArray&& nb_array,
			 const char* tail_name,
			 const PtExpr* value) :
  mFileRegion{file_region},
  mNbArray{move(nb_array)},
  mName{tail_name},
  mExpr{value}
{
  ASSERT_COND( value );
}

// デストラクタ
SptDefParam::~SptDefParam()
{
}

// ファイル位置の取得
// @return ファイル位置
FileRegion
SptDefParam::file_region() const
{
  return mFileRegion;
}

// @brief 階層ブランチの要素数の取得
SizeType
SptDefParam::namebranch_num() const
{
  return mNbArray.size();
}

// @brief 階層ブランチの取得
// @param[in] pos 位置 ( 0 <= pos < namebranch_num() )
const PtNameBranch*
SptDefParam::namebranch(SizeType pos) const
{
  return mNbArray[pos];
}

// 末尾の名前の取得
// @return 末尾の名前
const char*
SptDefParam::name() const
{
  return mName;
}

// 値の取得
// @return 値を表す式
const PtExpr*
SptDefParam::expr() const
{
  return mExpr;
}


//////////////////////////////////////////////////////////////////////
// continuous assign 文のヘッダ
//////////////////////////////////////////////////////////////////////

// コンストラクタ
// @param file_region ファイル位置の情報
// @param strength 信号強度
// @param delay 遅延
// @param ca_list continuous assignment のリスト
SptContAssignH::SptContAssignH(const FileRegion& file_region,
			       const PtStrength* strength,
			       const PtDelay* delay,
			       PtiContAssignArray&& ca_array) :
  SptItem{file_region, PtItemType::ContAssign},
  mStrength{strength},
  mDelay{delay},
  mArray{move(ca_array)}
{
}

// デストラクタ
SptContAssignH::~SptContAssignH()
{
}

// strength の取得
// @return 信号強度
const PtStrength*
SptContAssignH::strength() const
{
  return mStrength;
}

// delay の取得
// @return 遅延
const PtDelay*
SptContAssignH::delay() const
{
  return mDelay;
}

// @brief continuous assign の要素数の取得
SizeType
SptContAssignH::contassign_num() const
{
  return mArray.size();
}

// @brief continuous assign の取得
// @param[in] pos 位置 ( 0 <= pos < contassign_num() )
const PtContAssign*
SptContAssignH::contassign(SizeType pos) const
{
  return mArray[pos];
}


//////////////////////////////////////////////////////////////////////
// continuous assign文 のベース実装クラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
// @param file_region ファイル位置の情報
// @param lhs 左辺式
// @param rhs 右辺式
SptContAssign::SptContAssign(const FileRegion& file_region,
			     const PtExpr* lhs,
			     const PtExpr* rhs) :
  mFileRegion{file_region},
  mLhs{lhs},
  mRhs{rhs}
{
}

// デストラクタ
SptContAssign::~SptContAssign()
{
}

// ファイル位置の取得
// @return ファイル位置
FileRegion
SptContAssign::file_region() const
{
  return mFileRegion;
}

// 左辺式の取得
// @return 左辺式
const PtExpr*
SptContAssign::lhs() const
{
  return mLhs;
}

// 右辺式の取得
// @return 右辺式
const PtExpr*
SptContAssign::rhs() const
{
  return mRhs;
}


//////////////////////////////////////////////////////////////////////
// initial文/always文に共通なクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
// @param file_region ファイル位置の情報
// @param type 型(kInitial/kAlways)
// @param body 本体のステートメント
SptProcess::SptProcess(const FileRegion& file_region,
		       PtItemType type,
		       const PtStmt* body) :
  SptItem{file_region, type},
  mBody{body}
{
  ASSERT_COND( body );
}

// デストラクタ
SptProcess::~SptProcess()
{
}

// 本体のステートメントの取得
// @return 本体のステートメント
const PtStmt*
SptProcess::body() const
{
  return mBody;
}


//////////////////////////////////////////////////////////////////////
// function と task の共通の親クラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
// @param file_region ファイル位置の情報
// @param type 型
// @param name 名前
// @param automatic automatic 属性
// @param sign 符号の有無
// @param left 範囲の左側の式
// @param right 範囲の右側の式
// @param data_type 関数の戻値の型
// @param portdecl_list ポート宣言のリスト
// @param decl_list 宣言のリスト
// @param stmt 本体のステートメント
SptTf::SptTf(const FileRegion& file_region,
	     PtItemType type,
	     const char* name,
	     bool automatic,
	     bool sign,
	     const PtExpr* left,
	     const PtExpr* right,
	     VpiVarType data_type,
	     PtiIOHeadArray&& iohead_array,
	     PtiDeclHeadArray&& declhead_array,
	     const PtStmt* stmt) :
  SptItem{file_region, type},
  mName{name},
  mAutomatic{automatic},
  mSigned{sign},
  mLeftRange{left},
  mRightRange{right},
  mDataType{data_type},
  mIOHeadArray{move(iohead_array)},
  mDeclHeadArray{move(declhead_array)},
  mBody{stmt}
{
  mInUse = false;
  mIOItemNum = 0;
  for ( auto head: mIOHeadArray ) {
    mIOItemNum += head->item_num();
  }
}

// デストラクタ
SptTf::~SptTf()
{
}

// 名前の取得
// @return 名前
const char*
SptTf::name() const
{
  return mName;
}

// automatic 情報の取得
// @retval true automatic 宣言された task/function
// @retval false 上記以外
bool
SptTf::automatic() const
{
  return mAutomatic;
}

// @brief IO宣言の要素数の取得
SizeType
SptTf::ioitem_num() const
{
  return mIOItemNum;
}

// @brief IO宣言ヘッダリストの要素数の取得
SizeType
SptTf::iohead_num() const
{
  return mIOHeadArray.size();
}

// @brief IO宣言ヘッダの取得
// @param[in] pos 位置 ( 0 <= pos < iohead_num() )
const PtIOHead*
SptTf::iohead(SizeType pos) const
{
  return mIOHeadArray[pos];
}

// @brief 宣言ヘッダの要素数の取得
SizeType
SptTf::declhead_num() const
{
  return mDeclHeadArray.size();
}

// @brief 宣言ヘッダの取得
// @param[in] pos 位置 ( 0 <= pos < declhead_num() )
const PtDeclHead*
SptTf::declhead(SizeType pos) const
{
  return mDeclHeadArray[pos];
}

// 本体のステートメントの取得
// @return 本体のステートメントの取得
const PtStmt*
SptTf::body() const
{
  return mBody;
}

// 符号の取得
// @retval true 符号つき
// @retval false 符号なし
bool
SptTf::is_signed() const
{
  return mSigned;
}

// 範囲の左側の式の取得
// @return 範囲の左側の式
const PtExpr*
SptTf::left_range() const
{
  return mLeftRange;
}

// 範囲の右側の式の取得
// @return 範囲の右側の式
const PtExpr*
SptTf::right_range() const
{
  return mRightRange;
}

// データ型の取得
// @retval データ型 kParam, kLocalParam, kVar の場合
// @retval VpiVarType::None 上記以外
VpiVarType
SptTf::data_type() const
{
  return mDataType;
}

// constant function の展開中の印をつける．
void
SptTf::set_in_use() const
{
  mInUse = true;
}

// constant function の展開中の印を消す．
void
SptTf::clear_in_use() const
{
  mInUse = false;
}

// 使用中(constant function として展開中)のチェック
// @return 使用中の時 true を返す．
bool
SptTf::is_in_use() const
{
  return mInUse;
}


//////////////////////////////////////////////////////////////////////
// gate instance のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
// @param file_region ファイル位置の情報
// @param prim_type プリミティブタイプ
// @param strength 信号強度
// @param delay 遅延
// @param elem_list 要素のリスト
SptGateH::SptGateH(const FileRegion& file_region,
		   VpiPrimType prim_type,
		   const PtStrength* strength,
		   const PtDelay* delay,
		   PtiInstArray&& elem_array) :
  SptItem{file_region, PtItemType::GateInst},
  mPrimType{prim_type},
  mStrength{strength},
  mDelay{delay},
  mElemArray{move(elem_array)}
{
}

// デストラクタ
SptGateH::~SptGateH()
{
}

// プリミティブタイプの取得
// @return プリミティブタイプ
VpiPrimType
SptGateH::prim_type() const
{
  return mPrimType;
}

// strength の取得
// @return strength
const PtStrength*
SptGateH::strength() const
{
  return mStrength;
}

// delay の取得
// @return delay
const PtDelay*
SptGateH::delay() const
{
  return mDelay;
}

// @brief module/UDP/gate instance の要素数の取得
SizeType
SptGateH::inst_num() const
{
  return mElemArray.size();
}

// @brief module/UDP/gate instance の取得
// @param[in] pos 位置 ( 0 <= pos < inst_num() )
const PtInst*
SptGateH::inst(SizeType pos) const
{
  return mElemArray[pos];
}


//////////////////////////////////////////////////////////////////////
// module instance/UDP instance のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
// @param file_region ファイル位置の情報
// @param def_name 定義名
// @param con_array ポーと割当のリスト
// @param strength 信号強度
// @param delay 遅延
// @param elem_list
SptMuH::SptMuH(const FileRegion& file_region,
	       const char* def_name,
	       PtiConnectionArray&& con_array,
	       const PtStrength* strength,
	       const PtDelay* delay,
	       PtiInstArray&& elem_array) :
  SptItem{file_region, PtItemType::MuInst},
  mName{def_name},
  mParamArray{move(con_array)},
  mStrength{strength},
  mDelay{delay},
  mElemArray{move(elem_array)}
{
}

// デストラクタ
SptMuH::~SptMuH()
{
}

// 定義名の取得
// @return 定義名
const char*
SptMuH::name() const
{
  return mName;
}

// strength の取得
// @return strength
const PtStrength*
SptMuH::strength() const
{
  return mStrength;
}

// delay の取得
// @return delay
const PtDelay*
SptMuH::delay() const
{
  return mDelay;
}

// @brief パラメータ割り当て数の取得
SizeType
SptMuH::paramassign_num() const
{
  return mParamArray.size();
}

// @brief パラメータ割り当ての取得
// @param[in] pos 位置 ( 0 <= pos < paramassign_num() )
const PtConnection*
SptMuH::paramassign(SizeType pos) const
{
  return mParamArray[pos];
}

// @brief module/UDP/gate instance の要素数の取得
SizeType
SptMuH::inst_num() const
{
  return mElemArray.size();
}

// @brief module/UDP/gate instance の取得
// @param[in] pos 位置 ( 0 <= pos < inst_num() )
const PtInst*
SptMuH::inst(SizeType pos) const
{
  return mElemArray[pos];
}


//////////////////////////////////////////////////////////////////////
// module instance/UDP/gate instance のベース実装クラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
// @param file_region ファイル位置の情報
// @param name 名前
// @param left 範囲の左側の式
// @param right 範囲の右側の式
// @param con_array ポート割り当てリスト
SptInst::SptInst(const FileRegion& file_region,
		 const char* name,
		 const PtExpr* left,
		 const PtExpr* right,
		 PtiConnectionArray&& con_array) :
  mFileRegion{file_region},
  mName{name},
  mLeftRange{left},
  mRightRange{right},
  mPortArray{move(con_array)}
{
}

// デストラクタ
SptInst::~SptInst()
{
}

// ファイル位置の取得
// @return ファイル位置
FileRegion
SptInst::file_region() const
{
  return mFileRegion;
}

// 名前の取得
// @return 名前
const char*
SptInst::name() const
{
  return mName;
}

// 範囲の左側の式の取得
// @return 範囲の左側の式
const PtExpr*
SptInst::left_range() const
{
  return mLeftRange;
}

// 範囲の右側の式の取得
// @return 範囲の右側の式
const PtExpr*
SptInst::right_range() const
{
  return mRightRange;
}

// @brief ポートの要素数の取得
SizeType
SptInst::port_num() const
{
  return mPortArray.size();
}

// @brief ポートの取得
// @param[in] pos 位置 ( 0 <= pos < port_num() )
const PtConnection*
SptInst::port(SizeType pos) const
{
  return mPortArray[pos];
}


//////////////////////////////////////////////////////////////////////
// item 関係の生成
//////////////////////////////////////////////////////////////////////

// @brief defparam 文のヘッダの生成
// @param[in] file_region ファイル位置の情報
// @param[in] elem_array 要素のリスト
// @return 生成された defparam ヘッダ
const PtItem*
SptFactory::new_DefParamH(const FileRegion& file_region,
			  const vector<const PtDefParam*>& elem_array)
{
  auto node = new SptDefParamH(file_region,
			       PtiDefParamArray(mAlloc, elem_array));
  return node;
}

// @brief defparam 文の要素の生成
// @param[in] file_region ファイル位置の情報
// @param[in] name 名前
// @param[in] value 値を表す式
// @return 生成された要素
const PtDefParam*
SptFactory::new_DefParam(const FileRegion& file_region,
			 const char* name,
			 const PtExpr* value)
{
  auto node = new SptDefParam(file_region,
			      PtiNameBranchArray(),
			      name, value);
  return node;
}

// @brief defparam 文の要素の生成 (階層つき識別子)
// @param[in] file_region ファイル位置の情報
// @param[in] nb_array 階層名のリスト
// @param[in] tail_name 名前
// @param[in] value 値を表す式
// @return 生成された要素
const PtDefParam*
SptFactory::new_DefParam(const FileRegion& file_region,
			 PuHierName* hname,
			 const PtExpr* value)
{
  auto nb_array = hname->name_branch();
  auto tail_name = hname->tail_name();
  auto node = new SptDefParam(file_region,
			      PtiNameBranchArray(mAlloc, nb_array),
			      tail_name, value);
  return node;
}

// @brief continuous assign 文のヘッダの生成
// @param[in] file_region ファイル位置の情報
// @param[in] elem_array 要素のリスト
// @return 生成された continuous assign 文のヘッダ
const PtItem*
SptFactory::new_ContAssignH(const FileRegion& file_region,
			    const vector<const PtContAssign*>& elem_array)
{
  auto node = new SptContAssignH(file_region, nullptr, nullptr,
				 PtiContAssignArray(mAlloc, elem_array));
  return node;
}

// @brief continuous assign 文のヘッダの生成 (strengthつき)
// @param[in] file_region ファイル位置の情報
// @param[in] strength 信号強度
// @param[in] elem_array 要素のリスト
// @return 生成された continuous assign 文のヘッダ
const PtItem*
SptFactory::new_ContAssignH(const FileRegion& file_region,
			    const PtStrength* strength,
			    const vector<const PtContAssign*>& elem_array)
{
  auto node = new SptContAssignH(file_region, strength, nullptr,
				 PtiContAssignArray(mAlloc, elem_array));
  return node;
}

// @brief continuous assign 文のヘッダの生成 (遅延付き)
// @param[in] file_region ファイル位置の情報
// @param[in] delay 遅延値
// @param[in] elem_array 要素のリスト
// @return 生成された continuous assign 文のヘッダ
const PtItem*
SptFactory::new_ContAssignH(const FileRegion& file_region,
			    const PtDelay* delay,
			    const vector<const PtContAssign*>& elem_array)
{
  auto node = new SptContAssignH(file_region, nullptr, delay,
				 PtiContAssignArray(mAlloc, elem_array));
  return node;
}

// @brief continuous assign 文のヘッダの生成 (strength, 遅延付き)
// @param[in] file_region ファイル位置の情報
// @param[in] strength 信号強度
// @param[in] delay 遅延値
// @param[in] elem_array 要素のリスト
// @return 生成された continuous assign 文のヘッダ
const PtItem*
SptFactory::new_ContAssignH(const FileRegion& file_region,
			    const PtStrength* strength,
			    const PtDelay* delay,
			    const vector<const PtContAssign*>& elem_array)
{
  auto node = new SptContAssignH(file_region, strength, delay,
				 PtiContAssignArray(mAlloc, elem_array));
  return node;
}

// @brief continuous assign 文の生成
// @param[in] file_region ファイル位置の情報
// @param[in] lhs 左辺式
// @param[in] rhs 右辺式
// @return 生成された continuous assign 文
const PtContAssign*
SptFactory::new_ContAssign(const FileRegion& file_region,
			   const PtExpr* lhs,
			   const PtExpr* rhs)
{
  auto node = new SptContAssign(file_region, lhs, rhs);
  return node;
}

// @brief initial 文の生成
// @param[in] file_region ファイル位置の情報
// @param[in] body 本体のステートメント
// @return 生成された initial 文
const PtItem*
SptFactory::new_Initial(const FileRegion& file_region,
			const PtStmt* body)
{
  auto node = new SptProcess(file_region,
			     PtItemType::Initial,
			     body);
  return node;
}

// @brief always 文の生成
// @param[in] file_region ファイル位置の情報
// @param[in] body 本体のステートメント
// @return 生成された always 文
const PtItem*
SptFactory::new_Always(const FileRegion& file_region,
		       const PtStmt* body)
{
  auto node = new SptProcess(file_region,
			     PtItemType::Always,
			     body);
  return node;
}

// @brief task 文の生成
// @param[in] file_region ファイル位置の情報
// @param[in] name task 名
// @param[in] automatic automatic task の時に true となるフラグ
// @param[in] iohead_array ポート宣言のリスト
// @param[in] paramhead_array parameter 宣言のリスト
// @param[in] localparamhead_array localparam 宣言のリスト
// @param[in] declhead_array 宣言のリスト (IO宣言を含む)
// @param[in] stmt 本体のステートメント
// @return 生成された task 文
const PtItem*
SptFactory::new_Task(const FileRegion& file_region,
		     const char* name,
		     bool automatic,
		     const vector<const PtIOHead*>& iohead_array,
		     const vector<const PtDeclHead*>& declhead_array,
		     const PtStmt* stmt)
{
  auto node = new SptTf(file_region,
			PtItemType::Task,
			name, automatic,
			false, nullptr, nullptr,
			VpiVarType::None,
			PtiIOHeadArray(mAlloc, iohead_array),
			PtiDeclHeadArray(mAlloc, declhead_array),
			stmt);
  return node;
}

// @brief 1ビット型 function 文の生成
// @param[in] file_region ファイル位置の情報
// @param[in] name function 名
// @param[in] automatic automatic task の時に true となるフラグ
// @param[in] sign 符号つきの時に true となるフラグ
// @param[in] iohead_array ポート宣言のリスト
// @param[in] paramhead_array parameter 宣言のリスト
// @param[in] lparamhead_array localparam 宣言のリスト
// @param[in] declhead_array 宣言のリスト (IO宣言を含む)
// @param[in] stmt 本体のステートメント
// @return 生成された func 文
const PtItem*
SptFactory::new_Function(const FileRegion& file_region,
			 const char* name,
			 bool automatic,
			 bool sign,
			 const vector<const PtIOHead*>& iohead_array,
			 const vector<const PtDeclHead*>& declhead_array,
			 const PtStmt* stmt)
{
  auto node = new SptTf(file_region,
			PtItemType::Func,
			name, automatic,
			sign, nullptr, nullptr,
			VpiVarType::None,
			PtiIOHeadArray(mAlloc, iohead_array),
			PtiDeclHeadArray(mAlloc, declhead_array),
			stmt);
  return node;
}

// @brief 範囲指定型 function 文の生成
// @param[in] file_region ファイル位置の情報
// @param[in] name function 名
// @param[in] automatic automatic task の時に true となるフラグ
// @param[in] sign 符号つきの時に true にするフラグ
// @param[in] left 範囲の左側の式
// @param[in] right 範囲の右側の式
// @param[in] iohead_array ポート宣言のリスト
// @param[in] paramhead_array parameter 宣言のリスト
// @param[in] lparamhead_array localparam 宣言のリスト
// @param[in] declhead_array 宣言のリスト (IO宣言を含む)
// @param[in] stmt 本体のステートメント
// @return 生成された func 文
const PtItem*
SptFactory::new_SizedFunc(const FileRegion& file_region,
			  const char* name,
			  bool automatic,
			  bool sign,
			  const PtExpr* left,
			  const PtExpr* right,
			  const vector<const PtIOHead*>& iohead_array,
			  const vector<const PtDeclHead*>& declhead_array,
			  const PtStmt* stmt)
{
  auto node = new SptTf(file_region,
			PtItemType::Func,
			name, automatic,
			sign, left, right,
			VpiVarType::None,
			PtiIOHeadArray(mAlloc, iohead_array),
			PtiDeclHeadArray(mAlloc, declhead_array),
			stmt);
  return node;
}

// @brief 組み込み型 function 文の生成
// @param[in] file_region ファイル位置の情報
// @param[in] name function 名
// @param[in] automatic automatic task の時に true となるフラグ
// @param[in] sign 符号つきの時に true にするフラグ
// @param[in] func_type 関数の戻り値の型
// @param[in] iohead_array ポート宣言のリスト
// @param[in] paramhead_array parameter 宣言のリスト
// @param[in] lparamhead_array localparam 宣言のリスト
// @param[in] declhead_array 宣言のリスト (IO宣言を含む)
// @param[in] stmt 本体のステートメント
// @return 生成された func 文
const PtItem*
SptFactory::new_TypedFunc(const FileRegion& file_region,
			  const char* name,
			  bool automatic,
			  bool sign,
			  VpiVarType func_type,
			  const vector<const PtIOHead*>& iohead_array,
			  const vector<const PtDeclHead*>& declhead_array,
			  const PtStmt* stmt)
{
  auto node = new SptTf(file_region,
			PtItemType::Func,
			name, automatic,
			sign, nullptr, nullptr,
			func_type,
			PtiIOHeadArray(mAlloc, iohead_array),
			PtiDeclHeadArray(mAlloc, declhead_array),
			stmt);
  return node;
}

// @brief gate instance 文のヘッダの生成
// @param[in] file_region ファイル位置の情報
// @param[in] type primitive の型
// @param[in] elem_array 要素のリスト
// @return 生成された gate instance 文のヘッダ
const PtItem*
SptFactory::new_GateH(const FileRegion& file_region,
		      VpiPrimType type,
		      const vector<const PtInst*>& elem_array)
{
  auto node = new SptGateH(file_region,
			   type,
			   nullptr, nullptr,
			   PtiInstArray(mAlloc, elem_array));
  return node;
}

// @brief gate instance 文のヘッダの生成 (strength付き)
// @param[in] file_region ファイル位置の情報
// @param[in] type primitive の型
// @param[in] strength 信号強度
// @param[in] elem_array 要素のリスト
// @return 生成された gate instance 文のヘッダ
const PtItem*
SptFactory::new_GateH(const FileRegion& file_region,
		      VpiPrimType type,
		      const PtStrength* strength,
		      const vector<const PtInst*>& elem_array)
{
  auto node = new SptGateH(file_region,
			   type,
			   strength, nullptr,
			   PtiInstArray(mAlloc, elem_array));
  return node;
}

// @brief gate instance 文のヘッダの生成 (遅延付き)
// @param[in] file_region ファイル位置の情報
// @param[in] type primitive の型
// @param[in] delay 遅延値
// @param[in] elem_array 要素のリスト
// @return 生成された gate instance 文のヘッダ
const PtItem*
SptFactory::new_GateH(const FileRegion& file_region,
		      VpiPrimType type,
		      const PtDelay* delay,
		      const vector<const PtInst*>& elem_array)
{
  auto node = new SptGateH(file_region,
			   type,
			   nullptr, delay,
			   PtiInstArray(mAlloc, elem_array));
  return node;
}

// @brief gate instance 文のヘッダの生成 (strength, 遅延付き)
// @param[in] file_region ファイル位置の情報
// @param[in] type primitive の型
// @param[in] strength 信号強度
// @param[in] delay 遅延値
// @param[in] elem_array 要素のリスト
// @return 生成された gate instance 文のヘッダ
const PtItem*
SptFactory::new_GateH(const FileRegion& file_region,
		      VpiPrimType type,
		      const PtStrength* strength,
		      const PtDelay* delay,
		      const vector<const PtInst*>& elem_array)
{
  auto node = new SptGateH(file_region,
			   type,
			   strength, delay,
			   PtiInstArray(mAlloc, elem_array));
  return node;
}

// @brief module instance/UDP instance 文のヘッダの生成
// @param[in] file_region ファイル位置の情報
// @param[in] def_name 定義名
// @param[in] elem_array 要素のリスト
// @return 生成された module instance/UDP instance 文のヘッダ
const PtItem*
SptFactory::new_MuH(const FileRegion& file_region,
		    const char* def_name,
		    const vector<const PtInst*>& elem_array)
{
  auto node = new SptMuH(file_region, def_name,
			 PtiConnectionArray(),
			 nullptr, nullptr,
			 PtiInstArray(mAlloc, elem_array));
  return node;
}

// @brief module instance/UDP instance 文のヘッダの生成 (strength付き)
// @param[in] file_region ファイル位置の情報
// @param[in] def_name 定義名
// @param[in] strength 信号強度
// @param[in] elem_array 要素のリスト
// @return 生成された module instance/UDP instance 文のヘッダ
const PtItem*
SptFactory::new_MuH(const FileRegion& file_region,
		    const char* def_name,
		    const PtStrength* strength,
		    const vector<const PtInst*>& elem_array)
{
  auto node = new SptMuH(file_region, def_name,
			 PtiConnectionArray(),
			 strength, nullptr,
			 PtiInstArray(mAlloc, elem_array));
  return node;
}

// @brief module instance/UDP instance 文のヘッダの生成 (遅延付き)
// @param[in] file_region ファイル位置の情報
// @param[in] def_name 定義名
// @param[in] delay 遅延値
// @param[in] elem_array 要素のリスト
// @return 生成された module instance/UDP instance 文のヘッダ
const PtItem*
SptFactory::new_MuH(const FileRegion& file_region,
		    const char* def_name,
		    const PtDelay* delay,
		    const vector<const PtInst*>& elem_array)
{
  auto node = new SptMuH(file_region, def_name,
			 PtiConnectionArray(),
			 nullptr, delay,
			 PtiInstArray(mAlloc, elem_array));
  return node;
}

// @brief module instance/UDP instance 文のヘッダの生成 (strength, 遅延付き)
// @param[in] file_region ファイル位置の情報
// @param[in] def_name 定義名
// @param[in] strength 信号強度
// @param[in] delay 遅延値
// @param[in] elem_array 要素のリスト
// @return 生成された module instance/UDP instance 文のヘッダ
const PtItem*
SptFactory::new_MuH(const FileRegion& file_region,
		    const char* def_name,
		    const PtStrength* strength,
		    const PtDelay* delay,
		    const vector<const PtInst*>& elem_array)
{
  auto node = new SptMuH(file_region, def_name,
			 PtiConnectionArray(),
			 strength, delay,
			 PtiInstArray(mAlloc, elem_array));
  return node;
}

// @brief module instance/UDP instance 文のヘッダの生成
// @param[in] file_region ファイル位置の情報
// @param[in] def_name 定義名
// @param[in] con_array ポート割り当てリスト
// @param[in] elem_array 要素のリスト
// @return 生成された module instance/UDP instance 文のヘッダ
const PtItem*
SptFactory::new_MuH(const FileRegion& file_region,
		    const char* def_name,
		    const vector<const PtConnection*>& con_array,
		    const vector<const PtInst*>& elem_array)
{
  auto node = new SptMuH(file_region, def_name,
			 PtiConnectionArray(mAlloc, con_array),
			 nullptr, nullptr,
			 PtiInstArray(mAlloc, elem_array));
  return node;
}

// @brief module instance/UDP/gate instance の要素の生成
// @param[in] file_region ファイル位置の情報
// @param[in] con_list ポート割り当てリスト
// @return 生成された module instance/UDP instance の要素
const PtInst*
SptFactory::new_Inst(const FileRegion& file_region,
		     const vector<const PtConnection*>& con_array)
{
  auto node = new SptInst(file_region, nullptr, nullptr, nullptr,
			  PtiConnectionArray(mAlloc, con_array));
  return node;
}

// @brief 名前付き module instance/UDP/gate instance の要素の生成
// @param[in] file_region ファイル位置の情報
// @param[in] name 名前
// @param[in] con_list ポート割り当てリスト
// @return 生成された module instance/UDP instance の要素
const PtInst*
SptFactory::new_InstN(const FileRegion& file_region,
		      const char* name,
		      const vector<const PtConnection*>& con_array)
{
  auto node = new SptInst(file_region, name, nullptr, nullptr,
			  PtiConnectionArray(mAlloc, con_array));
  return node;
}

// @brief 名前と範囲付き module instance/UDP/gate instance の要素の生成
// @param[in] file_region ファイル位置の情報
// @param[in] name 名前
// @param[in] left 範囲の左側の式
// @param[in] right 範囲の右側の式
// @param[in] con_list ポート割り当てリスト
// @return 生成された module instance/UDP instance の要素
const PtInst*
SptFactory::new_InstV(const FileRegion& file_region,
		      const char* name,
		      const PtExpr* left,
		      const PtExpr* right,
		      const vector<const PtConnection*>& con_array)
{
  auto node = new SptInst(file_region, name, left, right,
			  PtiConnectionArray(mAlloc, con_array));
  return node;
}

END_NAMESPACE_YM_VERILOG
