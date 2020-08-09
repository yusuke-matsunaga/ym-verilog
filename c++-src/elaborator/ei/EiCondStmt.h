#ifndef EICONDSTMT_H
#define EICONDSTMT_H

/// @file EiCondStmt.h
/// @brief EiCondStmt のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.

// ステートメントを表すクラス
// IEEE Std 1364-2001 26.6.32 While, repeat, wait
// IEEE Std 1364-2001 26.6.33 For
// IEEE Std 1364-2001 26.6.34 Forever
// IEEE Std 1364-2001 26.6.35 If, if-else
// IEEE Std 1364-2001 26.6.36 Case


#include "EiStmt.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiLoopStmt EiCondStmt.h "EiCondStmt.h"
/// @brief while 文/ repeat 文/ wait 文 の基底クラス
/// IEEE Std 1364-2001 26.6.32 While, repeat, wait
//////////////////////////////////////////////////////////////////////
class EiLoopStmt :
  public EiStmtBase
{
  friend class EiFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] cond 条件式
  /// @param[in] stmt 本体のステートメント
  EiLoopStmt(const VlNamedObj* parent,
	     const VlProcess* process,
	     const PtStmt* pt_stmt,
	     const VlExpr* cond,
	     const VlStmt* stmt);

  /// @brief デストラクタ
  ~EiLoopStmt();


public:
  //////////////////////////////////////////////////////////////////////
  // VlStmt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 条件式を返す．
  const VlExpr*
  expr() const override;

  /// @brief 本体のステートメントを返す．
  const VlStmt*
  body_stmt() const override;


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスで用いられる関数
  //////////////////////////////////////////////////////////////////////

  const VlStmt*
  _body_stmt() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 条件式
  const VlExpr* mCondition;

  // 本体
  const VlStmt* mBodyStmt;

};


//////////////////////////////////////////////////////////////////////
/// @class EiWhileStmt EiCondStmt.h "EiCondStmt.h"
/// @brief while 文を表すクラス
/// IEEE Std 1364-2001 26.6.32 While, repeat, wait
//////////////////////////////////////////////////////////////////////
class EiWhileStmt :
  public EiLoopStmt
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] cond 条件式
  /// @param[in] stmt 本体のステートメント
  EiWhileStmt(const VlNamedObj* parent,
	      const VlProcess* process,
	      const PtStmt* pt_stmt,
	      const VlExpr* cond,
	      const VlStmt* stmt);

  /// @brief デストラクタ
  ~EiWhileStmt();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  VpiObjType
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// @class EiRepeatStmt EiCondStmt.h "EiCondStmt.h"
/// @brief repeat 文を表すクラス
/// IEEE Std 1364-2001 26.6.32 While, repeat, wait
//////////////////////////////////////////////////////////////////////
class EiRepeatStmt :
  public EiLoopStmt
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] cond 条件式
  /// @param[in] stmt 本体のステートメント
  EiRepeatStmt(const VlNamedObj* parent,
	       const VlProcess* process,
	       const PtStmt* pt_stmt,
	       const VlExpr* cond,
	       const VlStmt* stmt);

  /// @brief デストラクタ
  ~EiRepeatStmt();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  VpiObjType
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// @class EiWaitStmt EiCondStmt.h "EiCondStmt.h"
/// @brief wait 文を表すクラス
/// IEEE Std 1364-2001 26.6.32 While, repeat, wait
//////////////////////////////////////////////////////////////////////
class EiWaitStmt :
  public EiLoopStmt
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] cond 条件式
  /// @param[in] stmt 本体のステートメント
  EiWaitStmt(const VlNamedObj* parent,
	     const VlProcess* process,
	     const PtStmt* pt_stmt,
	     const VlExpr* cond,
	     const VlStmt* stmt);

  /// @brief デストラクタ
  ~EiWaitStmt();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  VpiObjType
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// @class EiForStmt EiCondStmt.h "EiCondStmt.h"
/// @brief for statement を表すクラス
/// IEEE Std 1364-2001 26.6.33 For
//////////////////////////////////////////////////////////////////////
class EiForStmt :
  public EiLoopStmt
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] cond 条件式
  /// @param[in] init_stmt 初期化式
  /// @param[in] inc_stmt 増加式
  /// @param[in] stmt 本体のステートメント
  EiForStmt(const VlNamedObj* parent,
	    const VlProcess* process,
	    const PtStmt* pt_stmt,
	    const VlExpr* cond,
	    const VlStmt* init_stmt,
	    const VlStmt* inc_stmt,
	    const VlStmt* stmt);

  /// @brief デストラクタ
  ~EiForStmt();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  VpiObjType
  type() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // VlStmt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// 初期化文を返す．
  const VlStmt*
  init_stmt() const override;

  /// 繰り返し文を返す．
  const VlStmt*
  inc_stmt() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 初期化文
  const VlStmt* mInitStmt;

  // 増加文
  const VlStmt* mIncStmt;

};


//////////////////////////////////////////////////////////////////////
/// @class EiForeverStmt EiCondStmt.h "EiCondStmt.h"
/// @brief forever statement を表すクラス
/// IEEE Std 1364-2001 26.6.34 Forever
//////////////////////////////////////////////////////////////////////
class EiForeverStmt :
  public EiStmtBase
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] stmt 本体のステートメント
  EiForeverStmt(const VlNamedObj* parent,
		const VlProcess* process,
		const PtStmt* pt_stmt,
		const VlStmt* stmt);

  /// @brief デストラクタ
  ~EiForeverStmt();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  VpiObjType
  type() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // VlStmt に固有の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 本体のステートメントを返す．
  const VlStmt*
  body_stmt() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 本体
  const VlStmt* mBodyStmt;

};


//////////////////////////////////////////////////////////////////////
/// @class EiIfStmt EiCondStmt.h "EiCondStmt.h"
/// @brief if statement を表すクラス
/// IEEE Std 1364-2001 26.6.35 If, if-else
//////////////////////////////////////////////////////////////////////
class EiIfStmt :
  public EiStmtBase
{
  friend class EiFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] cond 条件式
  /// @param[in] stmt 本体のステートメント
  EiIfStmt(const VlNamedObj* parent,
	   const VlProcess* process,
	   const PtStmt* pt_stmt,
	   const VlExpr* cond,
	   const VlStmt* stmt);

  /// @brief デストラクタ
  ~EiIfStmt();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  VpiObjType
  type() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // VlStmt に固有の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 条件式を返す．
  const VlExpr*
  expr() const override;

  /// @brief 本体のステートメントを返す．
  const VlStmt*
  body_stmt() const override;


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスで用いられる関数
  //////////////////////////////////////////////////////////////////////

  const VlStmt*
  _body_stmt() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 条件式
  const VlExpr* mCondition;

  // 本体
  const VlStmt* mBodyStmt;

};


//////////////////////////////////////////////////////////////////////
/// @class EiIfElseStmt EiCondStmt.h "EiCondStmt.h"
/// @brief if else statement を表すクラス
/// IEEE Std 1364-2001 26.6.35 If, if-else
//////////////////////////////////////////////////////////////////////
class EiIfElseStmt :
  public EiIfStmt
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] cond 条件式
  /// @param[in] stmt 本体のステートメント
  /// @param[in] else_stmt else節のステートメント
  EiIfElseStmt(const VlNamedObj* parent,
	       const VlProcess* process,
	       const PtStmt* pt_stmt,
	       const VlExpr* cond,
	       const VlStmt* stmt,
	       const VlStmt* else_stmt);

  /// @brief デストラクタ
  ~EiIfElseStmt();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  VpiObjType
  type() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // VlStmt に固有の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief else ステートメントを返す．
  const VlStmt*
  else_stmt() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // else ステートメント
  const VlStmt* mElseStmt;

};


//////////////////////////////////////////////////////////////////////
/// @class EiCaseItem EiCondStmt.h "EiCondStmt.h"
/// @brief case item を表すクラス
/// IEEE Std 1364-2001 26.6.36 Case
//////////////////////////////////////////////////////////////////////
class EiCaseItem :
  public VlCaseItem
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] pt_item パース木の caseitem 要素
  /// @param[in] label_num ラベルの要素数
  /// @param[in] label_list ラベルを表す式のリスト
  /// @param[in] body 本体のステートメント
  EiCaseItem(const PtCaseItem* pt_item,
	     SizeType label_num,
	     ElbExpr** label_list,
	     const VlStmt* body);

  /// @brief デストラクタ
  ~EiCaseItem();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  VpiObjType
  type() const override;

  /// @brief ファイル位置を返す．
  FileRegion
  file_region() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // VlCaseItem に固有の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 条件式のリストの要素数を返す．
  SizeType
  expr_num() const override;

  /// @brief 条件式を返す．
  /// @param[in] pos 位置番号
  const VlExpr*
  expr(SizeType pos) const override;

  /// @brief 本体のステートメントを返す．
  const VlStmt*
  body_stmt() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // パース木の caseitem 定義
  const PtCaseItem* mPtCaseItem;

  // ラベルのリストの要素数
  SizeType mExprNum;

  // ラベルのリスト
  ElbExpr** mExprList;

  // 本文
  const VlStmt* mBodyStmt;

};


//////////////////////////////////////////////////////////////////////
/// @class EiCaseStmt EiCondStmt.h "EiCondStmt.h"
/// @brief case statement を表すクラス
/// IEEE Std 1364-2001 26.6.36 Case
//////////////////////////////////////////////////////////////////////
class EiCaseStmt :
  public EiStmtBase
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] expr 条件式
  /// @param[in] caseitem_num caseitem の要素数
  /// @param[in] caseitem_array caseitem の配列
  EiCaseStmt(const VlNamedObj* parent,
	     const VlProcess* process,
	     const PtStmt* pt_stmt,
	     const VlExpr* expr,
	     SizeType caseitem_num,
	     const VlCaseItem** caseitem_array);

  /// @brief デストラクタ
  ~EiCaseStmt();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  VpiObjType
  type() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // VlStmt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief case type を返す．
  VpiCaseType
  case_type() const override;

  /// @brief 条件式を返す．
  const VlExpr*
  expr() const override;

  /// @brief case item のリストの要素数を返す．
  SizeType
  caseitem_num() const override;

  /// @brief case item を返す．
  /// @param[in] pos 位置番号
  const VlCaseItem*
  caseitem(SizeType pos) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 条件式
  const VlExpr* mCondition;

  // case item の要素数
  SizeType mCaseItemNum;

  // case item のリスト
  const VlCaseItem** mCaseItemList;

};

END_NAMESPACE_YM_VERILOG

#endif // EICONDSTMT_H
