﻿#ifndef SPTITEM_H
#define SPTITEM_H

/// @file SptItem.h
/// @brief SptItem のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym//FileRegion.h"

#include "ym/pt/PtItem.h"
#include "ym/pt/PtArray.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// item の基底クラス
//////////////////////////////////////////////////////////////////////
class SptItem :
  public PtItem
{
protected:

  /// コンストラクタ
  SptItem(const FileRegion& file_region,
	  tPtItemType type);

  /// デストラクタ
  ~SptItem();


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// ファイル位置の取得
  virtual
  FileRegion
  file_region() const;

  /// 型の取得
  virtual
  tPtItemType
  type() const;

  /// @brief プリミティブタイプの取得
  /// @return プリミティブタイプ
  /// @note このクラスでは kVpiAndPrim を返す．
  virtual
  tVpiPrimType
  prim_type() const;

  /// @brief strength の取得
  /// @return 信号強度
  /// @note このクラスでは nullptr を返す．
  virtual
  const PtStrength*
  strength() const;

  /// @brief delay の取得
  /// @return 遅延
  /// @note このクラスでは nullptr を返す．
  virtual
  const PtDelay*
  delay() const;

  /// @brief パラメータ割り当てリストの取得
  virtual
  PtConnectionArray
  paramassign_array() const;

  /// @brief 要素数の取得
  /// @return 要素数
  /// @note このクラスでは 0 を返す．
  virtual
  ymuint
  size() const;

  /// @brief defparam 要素の取得
  /// @param[in] pos 位置番号 ( 0 <= pos < size() )
  virtual
  const PtDefParam*
  defparam(ymuint pos) const;

  /// @brief continuous assign 要素の取得
  /// @param[in] pos 位置番号 ( 0 <= pos < size() )
  virtual
  const PtContAssign*
  contassign(ymuint pos) const;

  /// @brief module/UDP/gate instance 要素の取得
  /// @param[in] pos 位置番号 ( 0 <= pos < size() )
  virtual
  const PtInst*
  inst(ymuint pos) const;

  /// @brief 名前の取得
  /// @return 名前
  /// @note このクラスでは nullptr を返す．
  virtual
  const char*
  name() const;

  /// @brief automatic 情報の取得
  /// @retval true automatic 宣言された task/function
  /// @retval false 上記以外
  /// @note このクラスでは false を返す．
  virtual
  bool
  automatic() const;

  /// @brief IO宣言の要素数の取得
  virtual
  ymuint
  ioitem_num() const;

  /// @brief IO宣言リストの配列の取得
  virtual
  PtIOHeadArray
  iohead_array() const;

  /// @brief 宣言ヘッダ配列の取得
  virtual
  PtDeclHeadArray
  declhead_array() const;

  /// @brief item 配列の取得
  virtual
  PtItemArray
  item_array() const;

  /// @brief 本体のステートメントの取得
  /// @return 本体のステートメント
  /// @note このクラスでは nullptr を返す．
  virtual
  const PtStmt*
  body() const;

  /// @brief 符号の取得
  /// @retval true 符号つき
  /// @retval false 符号なし
  /// @note このクラスでは false を返す．
  virtual
  bool
  is_signed() const;

  /// @brief 範囲の左側の式の取得
  /// @return 範囲の左側の式
  /// @note このクラスでは nullptr を返す．
  virtual
  const PtExpr*
  left_range() const;

  /// @brief 範囲の右側の式の取得
  /// @return 範囲の右側の式
  /// @note このクラスでは nullptr を返す．
  virtual
  const PtExpr*
  right_range() const;

  /// @brief 戻値のデータ型の取得
  /// @return 戻値のデータ型
  /// @note このクラスでは kVpiVarNone を返す．
  virtual
  tVpiVarType
  data_type() const;

  /// @brief constant function の展開中の印をつける．
  /// @note このクラスではなにもしない．
  virtual
  void
  set_in_use() const;

  /// @brief constant function の展開中の印を消す．
  /// @note このクラスではなにもしない．
  virtual
  void
  clear_in_use() const;

  /// @brief 使用中(constant function として展開中)のチェック
  /// @return 使用中の時 true を返す．
  /// @note このクラスでは false を返す．
  virtual
  bool
  is_in_use() const;

  /// @brief specify block item の種類の取得
  /// @return specify block item の種類
  /// @note このクラスでは kVpiPulsestyleOnEvent を返す．
  virtual
  tVpiSpecItemType
  specitem_type() const;

  /// @brief specify block path の種類の取得
  /// @return specify block path の種類
  /// @note このクラスでは kVpiSpecPathNull を返す．
  virtual
  tVpiSpecPathType
  specpath_type() const;

  /// @brief ターミナルの取得
  /// @param[in] pos 位置番号 ( 0 <= pos < size() )
  virtual
  const PtExpr*
  terminal(ymuint pos) const;

  /// @brief パス記述の取得
  /// @return パス記述
  /// @note このクラスでは nullptr を返す．
  virtual
  const PtPathDecl*
  path_decl() const;

  /// @brief 条件式の取得
  /// @return 条件式
  /// @note このクラスでは nullptr を返す．
  virtual
  const PtExpr*
  expr() const;

  /// @brief 条件が成り立ったときに生成される宣言ヘッダ配列の取得
  virtual
  PtDeclHeadArray
  then_declhead_array() const;

  /// @brief 条件が成り立ったときに生成される item 配列の取得
  virtual
  PtItemArray
  then_item_array() const;

  /// @brief 条件が成り立たなかったときに生成される宣言ヘッダ配列の取得
  virtual
  PtDeclHeadArray
  else_declhead_array() const;

  /// @brief 条件が成り立たなかったときに生成される item 配列の取得
  virtual
  PtItemArray
  else_item_array() const;

  /// @brief case item の取得
  /// @param[in] pos 位置番号 ( 0 <= pos < size() )
  virtual
  const PtGenCaseItem*
  caseitem(ymuint pos) const;

  /// @brief 繰り返し制御用の変数名の取得
  /// @return 繰り返し制御用の変数名
  /// @note このクラスでは nullptr を返す．
  virtual
  const char*
  loop_var() const;

  /// @brief 初期化文の右辺の取得
  /// @return 初期化文の右辺
  /// @note このクラスでは nullptr を返す．
  virtual
  const PtExpr*
  init_expr() const;

  /// @brief 増加文の右辺の取得
  /// @return 増加文の右辺
  /// @note このクラスでは nullptr を返す．
  virtual
  const PtExpr*
  next_expr() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 型
  tPtItemType mType;

};


//////////////////////////////////////////////////////////////////////
// defparam 文のヘッダ
//////////////////////////////////////////////////////////////////////
class SptDefParamH :
  public SptItem
{
  friend class SptFactory;

private:

  /// コンストラクタ
  SptDefParamH(const FileRegion& file_region,
	       PtDefParamArray dp_array);

  /// デストラクタ
  virtual
  ~SptDefParamH();


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  // 要素数を返す．
  virtual
  ymuint
  size() const;

  /// @brief defparam 要素の取得
  /// @param[in] pos 位置番号 ( 0 <= pos < size() )
  virtual
  const PtDefParam*
  defparam(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 要素の配列
  PtDefParamArray mArray;

};


//////////////////////////////////////////////////////////////////////
// defparam 文を表すクラス
//////////////////////////////////////////////////////////////////////
class SptDefParam :
  public PtDefParam
{
  friend class SptFactory;

private:

  // コンストラクタ
  // value は常に定数式
  SptDefParam(const FileRegion& file_region,
	      PtNameBranchArray nb_array,
	      const char* tail_name,
	      const PtExpr* value);

  // デストラクタ
  virtual
  ~SptDefParam();


public:
  //////////////////////////////////////////////////////////////////////
  // PtDefParam の仮想関数
  //////////////////////////////////////////////////////////////////////

  // ファイル位置を返す．
  virtual
  FileRegion
  file_region() const;

  /// @brief 階層ブランチの取得
  virtual
  PtNameBranchArray
  namebranch_array() const;

  // 末尾の名前を返す．
  virtual
  const char*
  name() const;

  // 値を返す．
  virtual
  const PtExpr*
  expr() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 階層ブランチの配列
  PtNameBranchArray mNbArray;

  // 末尾の名前
  const char* mName;

  // 値
  const PtExpr* mExpr;

};


//////////////////////////////////////////////////////////////////////
// continuous assign 文のヘッダ
//////////////////////////////////////////////////////////////////////
class SptContAssignH :
  public SptItem
{
  friend class SptFactory;

private:

  // コンストラクタ
  SptContAssignH(const FileRegion& file_region,
		 const PtStrength* strength,
		 const PtDelay* delay,
		 PtContAssignArray ca_array);

  // デストラクタ
  virtual
  ~SptContAssignH();


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  // strength を返す．
  virtual
  const PtStrength*
  strength() const;

  // delay を返す．
  virtual
  const PtDelay*
  delay() const;

  // 要素数を得る．
  virtual
  ymuint
  size() const;

  /// @brief continuous assign 要素の取得
  /// @param[in] pos 位置番号 ( 0 <= pos < size() )
  virtual
  const PtContAssign*
  contassign(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // strength
  const PtStrength* mStrength;

  // delay
  const PtDelay* mDelay;

  // 要素の配列
  PtContAssignArray mArray;

};


//////////////////////////////////////////////////////////////////////
// continuous assign 文
//////////////////////////////////////////////////////////////////////
class SptContAssign :
  public PtContAssign
{
  friend class SptFactory;

private:

  // コンストラクタ
  SptContAssign(const FileRegion& fr,
		const PtExpr* lhs,
		const PtExpr* rhs);

  // デストラクタ
  virtual
  ~SptContAssign();


public:
  //////////////////////////////////////////////////////////////////////
  // PtContAssign の仮想関数
  //////////////////////////////////////////////////////////////////////

  // ファイル位置を取り出す．
  virtual
  FileRegion
  file_region() const;

  // 左辺式を取り出す．
  virtual
  const PtExpr*
  lhs() const;

  // 右辺式を取り出す．
  virtual
  const PtExpr*
  rhs() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 左辺式
  const PtExpr* mLhs;

  // 右辺式
  const PtExpr* mRhs;

};


//////////////////////////////////////////////////////////////////////
// inititial 文/always 文に共通なクラス
//////////////////////////////////////////////////////////////////////
class SptProcess :
  public SptItem
{
  friend class SptFactory;

private:

  // コンストラクタ
  SptProcess(const FileRegion& file_region,
	     tPtItemType type,
	     const PtStmt* body);

  // デストラクタ
  virtual
  ~SptProcess();


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  // 本体のステートメントを返す．
  virtual
  const PtStmt*
  body() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 本体
  const PtStmt* mBody;

};


//////////////////////////////////////////////////////////////////////
// task と function に共通な基底クラス
//////////////////////////////////////////////////////////////////////
class SptTf :
  public SptItem
{
  friend class SptFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] type 型
  /// @param[in] name 名前
  /// @param[in] automatic automatic 属性
  /// @param[in] sign 符号の有無
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @param[in] data_type 関数の戻値の型
  /// @param[in] iohead_array IO宣言のリスト
  /// @param[in] declhead_array 宣言のリスト
  /// @param[in] stmt 本体のステートメント
  SptTf(const FileRegion& file_region,
	tPtItemType type,
	const char* name,
	bool automatic,
	bool sign,
	const PtExpr* left,
	const PtExpr* right,
	tVpiVarType var_type,
	PtIOHeadArray iohead_array,
	PtDeclHeadArray declhead_array,
	const PtStmt* stmt);

  // デストラクタ
  virtual
  ~SptTf();


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  // 名前を取り出す．
  virtual
  const char*
  name() const;

  // automatic なら true
  virtual
  bool
  automatic() const;

  /// @brief IO宣言の要素数の取得
  virtual
  ymuint
  ioitem_num() const;

  /// @brief IO宣言リストの配列の取得
  virtual
  PtIOHeadArray
  iohead_array() const;

  /// @brief 宣言ヘッダ配列の取得
  virtual
  PtDeclHeadArray
  declhead_array() const;

  // 本体を取り出す．
  virtual
  const PtStmt*
  body() const;

  // 符号の有無の取得
  virtual
  bool
  is_signed() const;

  // 範囲の MSB を得る． 指定なしなら nullptr
  virtual
  const PtExpr*
  left_range() const;

  // 範囲の LSB を得る． 指定なしなら nullptr
  virtual
  const PtExpr*
  right_range() const;

  // データ型の取得
  virtual
  tVpiVarType
  data_type() const;

  // constant function の展開中の印をつける．
  virtual
  void
  set_in_use() const;

  // constant function の展開中の印を消す．
  virtual
  void
  clear_in_use() const;

  // 使用中(constant function として展開中)の時 true を返す．
  virtual
  bool
  is_in_use() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  const char* mName;

  // automatic 属性
  bool mAutomatic;

  // 符号
  bool mSigned;

  // 範囲のMSB
  const PtExpr* mLeftRange;

  // 範囲のLSB
  const PtExpr* mRightRange;

  // データ型
  tVpiVarType mDataType;

  // IO宣言の要素数
  ymuint32 mIOItemNum;

  // IO宣言の配列
  PtIOHeadArray mIOHeadArray;

  // その他の宣言の配列
  PtDeclHeadArray mDeclHeadArray;

  // 本体
  const PtStmt* mBody;

  // 使用中かどうかを示すフラグ
  mutable
  bool mInUse;

};


//////////////////////////////////////////////////////////////////////
// gate header のベース実装クラス
//////////////////////////////////////////////////////////////////////
class SptGateH :
  public SptItem
{
  friend class SptFactory;

private:

  // コンストラクタ
  SptGateH(const FileRegion& file_region,
	   tVpiPrimType prim_type,
	   const PtStrength* strength,
	   const PtDelay* delay,
	   PtInstArray elem_array);

  // デストラクタ
  virtual
  ~SptGateH();


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  // プリミティブタイプ(vpiAndPrim など)を返す．
  virtual
  tVpiPrimType
  prim_type() const;

  // strength を返す．
  virtual
  const PtStrength*
  strength() const;

  // delay を返す．
  virtual
  const PtDelay*
  delay() const;

  // 要素数を取り出す．
  virtual
  ymuint
  size() const;

  /// @brief module/UDP/gate instance 要素の取得
  /// @param[in] pos 位置番号 ( 0 <= pos < size() )
  virtual
  const PtInst*
  inst(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // プリミティブタイプ
  tVpiPrimType mPrimType;

  // strength
  const PtStrength* mStrength;

  // delay
  const PtDelay* mDelay;

  // 要素の配列
  PtInstArray mElemArray;

};


//////////////////////////////////////////////////////////////////////
// module/udp header のベース実装クラス
//////////////////////////////////////////////////////////////////////
class SptMuH :
  public SptItem
{
  friend class SptFactory;

private:

  // コンストラクタ
  SptMuH(const FileRegion& file_region,
	 const char* def_name,
	 PtConnectionArray con_array,
	 const PtStrength* strength,
	 const PtDelay* delay,
	 PtInstArray elem_array);

  // デストラクタ
  virtual
  ~SptMuH();


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  // 定義名を返す．
  virtual
  const char*
  name() const;

  // strength を返す．
  virtual
  const PtStrength*
  strength() const;

  // delay を返す．
  virtual
  const PtDelay*
  delay() const;

  /// @brief パラメータ割り当てリストの取得
  virtual
  PtConnectionArray
  paramassign_array() const;

  // 要素数を取り出す．
  virtual
  ymuint
  size() const;

  /// @brief module/UDP/gate instance 要素の取得
  /// @param[in] pos 位置番号 ( 0 <= pos < size() )
  virtual
  const PtInst*
  inst(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 定義名
  const char* mName;

  // パラメータ割り当ての配列
  PtConnectionArray mParamArray;

  // strength
  const PtStrength* mStrength;

  // delay
  const PtDelay* mDelay;

  // 要素のリスト
  PtInstArray mElemArray;

};


//////////////////////////////////////////////////////////////////////
// module instance/UDP instance のベース実装クラス
//////////////////////////////////////////////////////////////////////
class SptInst :
  public PtInst
{
  friend class SptFactory;

private:

  // コンストラクタ
  SptInst(const FileRegion& file_region,
	  const char* name,
	  const PtExpr* left,
	  const PtExpr* right,
	  PtConnectionArray con_array);

  // デストラクタ
  virtual
  ~SptInst();


public:
  //////////////////////////////////////////////////////////////////////
  // PtInst の仮想関数
  //////////////////////////////////////////////////////////////////////

  // ファイル位置を返す．
  virtual
  FileRegion
  file_region() const;

  // 名前を返す．
  virtual
  const char*
  name() const;

  // range の MSB を取出す．
  virtual
  const PtExpr*
  left_range() const;

  // range の LSB を取出す．
  virtual
  const PtExpr*
  right_range() const;

  /// @brief ポート数の取得
  /// @return ポート数
  virtual
  ymuint
  port_num() const;

  /// @brief ポートの取得
  /// @param[in] pos 位置番号 ( 0 <= pos < port_num() )
  virtual
  const PtConnection*
  port(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 名前
  const char* mName;

  // 範囲のMSB
  const PtExpr* mLeftRange;

  // 範囲のLSB
  const PtExpr* mRightRange;

  // ポート割り当ての配列
  PtConnectionArray mPortArray;

};


//////////////////////////////////////////////////////////////////////
// generate 文で生成される要素を持つ本体
//////////////////////////////////////////////////////////////////////
class SptGenBody
{
public:

  // コンストラクタ
  SptGenBody(PtDeclHeadArray decl_array,
	     PtItemArray item_array);

  // デストラクタ
  ~SptGenBody();


public:

  /// @brief 宣言ヘッダ配列の取得
  PtDeclHeadArray
  declhead_array() const { return mDeclArray; }

  /// @brief item 配列の取得
  PtItemArray
  item_array() const { return mItemArray; }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 宣言の配列
  PtDeclHeadArray mDeclArray;

  // 要素の配列
  PtItemArray mItemArray;

};


//////////////////////////////////////////////////////////////////////
// generate ブロックの基底クラス
//////////////////////////////////////////////////////////////////////
class SptGenerate :
  public SptItem
{
  friend class SptFactory;

private:

  // コンストラクタ
  SptGenerate(const FileRegion& file_region,
	      tPtItemType type,
	      const char* name,
	      PtDeclHeadArray decl_array,
	      PtItemArray item_array);

  // デストラクタ
  virtual
  ~SptGenerate();


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  // 名前を返す．
  virtual
  const char*
  name() const;

  /// @brief 宣言のリストの取得
  virtual
  PtDeclHeadArray
  declhead_array() const;

  /// @brief item リストの取得
  virtual
  PtItemArray
  item_array() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  const char* mName;

  // 本体
  SptGenBody mBody;

};


//////////////////////////////////////////////////////////////////////
// gen_if 文 を表すクラス
//////////////////////////////////////////////////////////////////////
class SptGenIf :
  public SptItem
{
  friend class SptFactory;

private:

  // コンストラクタ
  SptGenIf(const FileRegion& file_region,
	   const PtExpr* cond,
	   PtDeclHeadArray then_decl_array,
	   PtItemArray then_item_array,
	   PtDeclHeadArray else_decl_array,
	   PtItemArray else_item_array);

  // デストラクタ
  virtual
  ~SptGenIf();


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  // 条件式を返す．
  virtual
  const PtExpr*
  expr() const;

  /// @brief 条件が成り立ったときに生成される宣言ヘッダ配列の取得
  virtual
  PtDeclHeadArray
  then_declhead_array() const;

  /// @brief 条件が成り立ったときに生成される item 配列の取得
  virtual
  PtItemArray
  then_item_array() const;

  /// @brief 条件が成り立たなかったときに生成される宣言ヘッダ配列の取得
  virtual
  PtDeclHeadArray
  else_declhead_array() const;

  /// @brief 条件が成り立たなかったときに生成される item 配列の取得
  virtual
  PtItemArray
  else_item_array() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 条件式
  const PtExpr* mCond;

  // 成り立ったとき生成される本体
  SptGenBody mThenBody;

  // 成り立たなかったとき生成される本体
  SptGenBody mElseBody;

};


//////////////////////////////////////////////////////////////////////
// gen_case 文 を表すクラス
//////////////////////////////////////////////////////////////////////
class SptGenCase :
  public SptItem
{
  friend class SptFactory;

private:

  // コンストラクタ
  SptGenCase(const FileRegion& file_region,
	     const PtExpr* expr,
	     PtGenCaseItemArray item_array);

  // デストラクタ
  virtual
  ~SptGenCase();


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  // 比較式を返す．
  virtual
  const PtExpr*
  expr() const;

  // case item の要素数を返す．
  virtual
  ymuint
  size() const;

  // case item を返す．
  virtual
  const PtGenCaseItem*
  caseitem(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 比較式
  const PtExpr* mExpr;

  // case item の配列
  PtGenCaseItemArray mCaseItemArray;

};


//////////////////////////////////////////////////////////////////////
// gencaseitemを表すクラス
//////////////////////////////////////////////////////////////////////
class SptGenCaseItem :
  public PtGenCaseItem
{
  friend class SptFactory;

private:

  // コンストラクタ
  SptGenCaseItem(const FileRegion& file_region,
		 PtExprArray label_array,
		 PtDeclHeadArray decl_array,
		 PtItemArray item_array);

  // デストラクタ
  virtual
  ~SptGenCaseItem();


public:
  //////////////////////////////////////////////////////////////////////
  // PtGenCaseItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  // ファイル位置を返す．
  virtual
  FileRegion
  file_region() const;

  /// @brief ラベルの数の取得
  /// @return ラベル数\n
  /// 0 の時は default の意味
  virtual
  ymuint
  label_num() const;

  /// @brief ラベルの取得
  /// @param[in] pos 位置番号 ( 0 <= pos < label_num() )
  virtual
  const PtExpr*
  label(ymuint pos) const;

  /// @brief 宣言のリストの取得
  virtual
  PtDeclHeadArray
  declhead_array() const;

  /// @brief item リストの取得
  virtual
  PtItemArray
  item_array() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // ラベルの配列
  PtExprArray mLabelArray;

  // 生成される本体
  SptGenBody mBody;

};


//////////////////////////////////////////////////////////////////////
// gen_for 文 を表すクラス
//////////////////////////////////////////////////////////////////////
class SptGenFor :
  public SptItem
{
  friend class SptFactory;

private:

  // コンストラクタ
  SptGenFor(const FileRegion& file_region,
	    const char* loop_var,
	    const PtExpr* init_expr,
	    const PtExpr* cond,
	    const PtExpr* next_expr,
	    const char* block_name,
	    PtDeclHeadArray decl_array,
	    PtItemArray item_array);

  // デストラクタ
  virtual
  ~SptGenFor();


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  // 名前を返す．
  virtual
  const char*
  name() const;

  /// @brief 宣言のリストの取得
  virtual
  PtDeclHeadArray
  declhead_array() const;

  /// @brief item リストの取得
  virtual
  PtItemArray
  item_array() const;

  // 繰り返し制御用の変数名を返す．
  virtual
  const char*
  loop_var() const;

  // 初期化文の右辺を返す．
  virtual
  const PtExpr*
  init_expr() const;

  // 繰り返し条件を返す．
  virtual
  const PtExpr*
  expr() const;

  // 増加文の右辺を返す．
  virtual
  const PtExpr*
  next_expr() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  const char* mName;

  // 繰り返し制御用の変数名
  const char* mLoopVar;

  // 初期化文の右辺
  const PtExpr* mInitExpr;

  // 繰り返し条件
  const PtExpr* mCond;

  // 増加文の右辺
  const PtExpr* mNextExpr;

  // 生成される本体
  SptGenBody mBody;

};


//////////////////////////////////////////////////////////////////////
// specify_block item を表すクラス
//////////////////////////////////////////////////////////////////////
class SptSpecItem :
  public SptItem
{
  friend class SptFactory;

private:

  // コンストラクタ
  SptSpecItem(const FileRegion& file_region,
	      tVpiSpecItemType id,
	      PtExprArray terminal_array);

  // デストラクタ
  virtual
  ~SptSpecItem();


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  // specify block item の種類を返す．
  virtual
  tVpiSpecItemType
  specitem_type() const;

  // ターミナルリストの要素数を返す．
  virtual
  ymuint
  size() const;

  /// @brief ターミナルの取得
  /// @param[in] pos 位置番号 ( 0 <= pos < size() )
  virtual
  const PtExpr*
  terminal(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // トークン番号
  tVpiSpecItemType mId;

  // ターミナルリスト
  PtExprArray mTerminals;

};


//////////////////////////////////////////////////////////////////////
// path 仕様 を表すクラス
//////////////////////////////////////////////////////////////////////
class SptSpecPath :
  public SptItem
{
  friend class SptFactory;

private:

  // コンストラクタ
  SptSpecPath(const FileRegion& file_region,
	      tVpiSpecPathType id,
	      const PtExpr* expr,
	      const PtPathDecl* path_decl);

  // デストラクタ
  virtual
  ~SptSpecPath();


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  // specify block path の種類を返す．
  virtual
  tVpiSpecPathType
  specpath_type() const;

  // モジュールパスの式を返す．
  virtual
  const PtExpr*
  expr() const;

  // パス記述を返す．
  virtual
  const PtPathDecl*
  path_decl() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // トークン番号
  tVpiSpecPathType mId;

  // モジュールパスの式
  const PtExpr* mExpr;

  // パス記述
  const PtPathDecl* mPathDecl;

};


//////////////////////////////////////////////////////////////////////
// path_delay_declaration を表すクラス
//////////////////////////////////////////////////////////////////////
class SptPathDecl :
  public PtPathDecl
{
  friend class SptFactory;

private:

  // コンストラクタ
  SptPathDecl(const FileRegion& file_region,
	      int edge,
	      PtExprArray input_array,
	      int input_pol,
	      int op,
	      PtExprArray output_array,
	      int output_pol,
	      const PtExpr* expr,
	      const PtPathDelay* path_delay);

  // デストラクタ
  virtual
  ~SptPathDecl();


public:
  //////////////////////////////////////////////////////////////////////
  // PtPathDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  // ファイル位置を返す．
  virtual
  FileRegion
  file_region() const;

  // edge_descriptor を取り出す．
  // 0の場合もある．
  virtual
  int
  edge() const;

  /// @brief 入力リストの要素数の取得
  /// @return 入力リストの要素数
  virtual
  ymuint
  input_num() const;

  /// @brief 入力の取得
  /// @param[in] pos 位置番号 ( 0 <= pos < input_num() )
  virtual
  const PtExpr*
  input(ymuint pos) const;

  // 入力の極性を取り出す．
  // 0の場合もありうる．
  virtual
  int
  input_pol() const;

  // パス記述子(?)を得る．vpiParallel か vpiFull
  virtual
  int
  op() const;

  /// @brief 出力リストの要素数の取得
  /// @return 出力リストの要素数
  virtual
  ymuint
  output_num() const;

  /// @brief 出力の取得
  /// @param[in] pos 位置番号 ( 0 <= pos < output_num() )
  virtual
  const PtExpr*
  output(ymuint pos) const;

  // 出力の極性を取り出す．
  // 0の場合もありうる．
  virtual
  int
  output_pol() const;

  // 式を取り出す．
  // nullptr の場合もありうる．
  virtual
  const PtExpr*
  expr() const;

  // path_delay_value を取り出す．
  virtual
  const PtPathDelay*
  path_delay() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  int mEdge;
  PtExprArray mInputs;
  int mInputPol;
  int mOp;
  PtExprArray mOutputs;
  int mOutputPol;
  const PtExpr* mExpr;
  const PtPathDelay* mPathDelay;
};


//////////////////////////////////////////////////////////////////////
// path_delay_value を表すクラス
//////////////////////////////////////////////////////////////////////
class SptPathDelay :
  public PtPathDelay
{
  friend class SptFactory;

private:

  // コンストラクタ
  SptPathDelay(const FileRegion& file_region,
	       const PtExpr* value1);

  SptPathDelay(const FileRegion& file_region,
	       const PtExpr* value1,
	       const PtExpr* value2);

  SptPathDelay(const FileRegion& file_region,
	       const PtExpr* value1,
	       const PtExpr* value2,
	       const PtExpr* value3);

  SptPathDelay(const FileRegion& file_region,
	       const PtExpr* value1,
	       const PtExpr* value2,
	       const PtExpr* value3,
	       const PtExpr* value4,
	       const PtExpr* value5,
	       const PtExpr* value6);

  SptPathDelay(const FileRegion& file_region,
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
	       const PtExpr* value12);

  // デストラクタ
  virtual
  ~SptPathDelay();


public:
  //////////////////////////////////////////////////////////////////////
  // PtPathDelay の仮想関数
  //////////////////////////////////////////////////////////////////////

  // ファイル位置を返す．
  virtual
  FileRegion
  file_region() const;

  // 値を取り出す．
  // 0の場合もある．
  virtual
  const PtExpr*
  value(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // ディレイ値
  const PtExpr* mValues[12];

};

END_NAMESPACE_YM_VERILOG

#endif // SPTITEM_H
