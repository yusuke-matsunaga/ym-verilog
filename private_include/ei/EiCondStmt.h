#ifndef EICONDSTMT_H
#define EICONDSTMT_H

/// @file EiCondStmt.h
/// @brief EiCondStmt のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2020 Yusuke Matsunaga
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
public:

  /// @brief コンストラクタ
  EiLoopStmt(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in] 親のプロセス (or nullptr)
    const PtStmt* pt_stmt,    ///< [in] パース木のステートメント定義
    const VlExpr* cond,       ///< [in] 条件式
    const VlStmt* stmt        ///< [in] 本体のステートメント
  );

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
public:

  /// @brief コンストラクタ
  EiWhileStmt(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in] 親のプロセス (or nullptr)
    const PtStmt* pt_stmt,    ///< [in] パース木のステートメント定義
    const VlExpr* cond,       ///< [in] 条件式
    const VlStmt* stmt        ///< [in] 本体のステートメント
  );

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
public:

  /// @brief コンストラクタ
  EiRepeatStmt(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in] 親のプロセス (or nullptr)
    const PtStmt* pt_stmt,    ///< [in] パース木のステートメント定義
    const VlExpr* cond,	      ///< [in] 条件式
    const VlStmt* stmt	      ///< [in] 本体のステートメント
  );

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
public:

  /// @brief コンストラクタ
  EiWaitStmt(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in] 親のプロセス (or nullptr)
    const PtStmt* pt_stmt,    ///< [in] パース木のステートメント定義
    const VlExpr* cond,	      ///< [in] 条件式
    const VlStmt* stmt	      ///< [in] 本体のステートメント
  );

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
public:

  /// @brief コンストラクタ
  EiForStmt(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in] 親のプロセス (or nullptr)
    const PtStmt* pt_stmt,    ///< [in] パース木のステートメント定義
    const VlExpr* cond,	      ///< [in] 条件式
    const VlStmt* init_stmt,  ///< [in] 初期化式
    const VlStmt* inc_stmt,   ///< [in] 増加式
    const VlStmt* stmt        ///< [in] 本体のステートメント
  );

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
public:

  /// @brief コンストラクタ
  EiForeverStmt(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in] 親のプロセス (or nullptr)
    const PtStmt* pt_stmt,    ///< [in] パース木のステートメント定義
    const VlStmt* stmt	      ///< [in] 本体のステートメント
  );

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
public:

  /// @brief コンストラクタ
  EiIfStmt(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in] 親のプロセス (or nullptr)
    const PtStmt* pt_stmt,    ///< [in] パース木のステートメント定義
    const VlExpr* cond,	      ///< [in] 条件式
    const VlStmt* stmt	      ///< [in] 本体のステートメント
  );

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
public:

  /// @brief コンストラクタ
  EiIfElseStmt(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in] 親のプロセス (or nullptr)
    const PtStmt* pt_stmt,    ///< [in] パース木のステートメント定義
    const VlExpr* cond,	      ///< [in] 条件式
    const VlStmt* stmt,	      ///< [in] 本体のステートメント
    const VlStmt* else_stmt   ///< [in] else節のステートメント
  );

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
public:

  /// @brief コンストラクタ
  EiCaseItem(
    const PtCaseItem* pt_item,          ///< [in] パース木の caseitem 要素
    const vector<ElbExpr*>& label_list, ///< [in] ラベルを表す式のリスト
    const VlStmt* body                  ///< [in] 本体のステートメント
  );

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
  const VlExpr*
  expr(
    SizeType pos ///< [in] 位置番号
  ) const override;

  /// @brief 条件式のリストを返す．
  vector<const VlExpr*>
  expr_list() const override;

  /// @brief 本体のステートメントを返す．
  const VlStmt*
  body_stmt() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // パース木の caseitem 定義
  const PtCaseItem* mPtCaseItem;

  // ラベルのリスト
  vector<ElbExpr*> mExprList;

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
public:

  /// @brief コンストラクタ
  EiCaseStmt(
    const VlScope* parent,                     ///< [in] 親のスコープ
    const VlProcess* process,                  ///< [in] 親のプロセス (or nullptr)
    const PtStmt* pt_stmt,                     ///< [in] パース木のステートメント定義
    const VlExpr* expr,                        ///< [in] 条件式
    const vector<const VlCaseItem*>& item_list ///< [in] caseitem のリスト
  );

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
  const VlCaseItem*
  caseitem(
    SizeType pos ///< [in] 位置番号
  ) const override;

  /// @brief case item のリストを返す．
  vector<const VlCaseItem*>
  caseitem_list() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 条件式
  const VlExpr* mCondition;

  // case item のリスト
  vector<const VlCaseItem*> mCaseItemList;

};

END_NAMESPACE_YM_VERILOG

#endif // EICONDSTMT_H
