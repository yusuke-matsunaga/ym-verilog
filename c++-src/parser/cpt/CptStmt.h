#ifndef CPTSTMT_H
#define CPTSTMT_H

/// @file CptStmt.h
/// @brief CptStmt のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.


#include "ym/pt/PtStmt.h"
#include "ym/FileRegion.h"
#include "parser/PtiArray.h"
#include "parser/PtiFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @brief statement の共通の親クラス
//////////////////////////////////////////////////////////////////////
class CptStmt :
  public PtStmt
{
public:

  /// @brief コンストラクタ
  CptStmt(const FileRegion& file_region);

  /// @brief デストラクタ
  ~CptStmt();


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置の取得
  /// @return ファイル位置
  FileRegion
  file_region() const override;

  /// @brief 階層ブランチの要素数の取得
  SizeType
  namebranch_num() const override;

  /// @brief 階層ブランチの取得
  /// @param[in] pos 位置 ( 0 <= pos < namebranch_num() )
  const PtNameBranch*
  namebranch(SizeType pos) const override;

  /// @brief 名前の取得
  /// @return 名前
  /// @note kDisable/kEnable/kSysEnable/kParBlock/kSeqBlock で意味のある関数
  /// @note このクラスでは nullptr を返す．
  const char*
  name() const override;

  /// @brief ステートメントの種類を表す文字列の取得
  /// @return ステートメントの種類を表す文字列
  const char*
  stmt_name() const override;

  /// @brief 引数の数の取得
  SizeType
  arg_num() const override;

  /// @brief 引数の取得
  /// @param[in] pos 位置 ( 0 <= pos < arg_num() )
  const PtExpr*
  arg(SizeType pos) const override;

  /// @brief コントロールの取得
  /// @return ディレイ/イベントコントロール
  /// @note kDc/kEc で意味のある関数
  /// @note このクラスでは nullptr を返す．
  const PtControl*
  control() const override;

  /// @brief 本体のステートメントの取得
  /// @return 本体のステートメント
  /// @note kDc/kEc/kWait/kForever/kRepeat/kWhile/kFor/kIf で意味のある関数
  /// @note このクラスでは nullptr を返す．
  const PtStmt*
  body() const override;

  /// @brief 式の取得
  /// @return 式
  /// @note kWait/kRepeat/kWhile/kFor/kIf/kCase/kCaseX/kCaseZ で意味のある関数
  /// @note このクラスでは nullptr を返す．
  const PtExpr*
  expr() const override;

  /// @brief 左辺式の取得
  /// @return 左辺式
  /// @note kAssign/kForce/kPcAssign/kRelease/kDeassign で意味のある関数
  /// @note このクラスでは nullptr を返す．
  const PtExpr*
  lhs() const override;

  /// @brief 右辺式の取得
  /// @return 右辺式
  /// @note kAssign/kForce/kPcAssign で意味のある関数
  /// @note このクラスでは nullptr を返す．
  const PtExpr*
  rhs() const override;

  /// @brief イベントプライマリの取得
  /// @return イベントプライマリ
  /// @note kEvent で意味のある関数
  /// @note このクラスでは nullptr を返す．
  const PtExpr*
  primary() const override;

  /// @brief 条件が成り立たなかったとき実行されるステートメントの取得
  /// @return 条件が成り立たなかったとき実行されるステートメント
  /// @note kIf で意味のある関数
  /// @note このクラスでは nullptr を返す．
  const PtStmt*
  else_body() const override;

  /// @brief case item のリストの要素数の取得
  SizeType
  caseitem_num() const override;

  /// @brief case item の取得
  /// @param[in] pos 位置 ( 0 <= pos < caseitem_num() )
  const PtCaseItem*
  caseitem(SizeType pos) const override;

  /// @brief 初期化代入文の取得
  /// @return 初期化代入文
  /// @note kFor で意味のある関数
  /// @note このクラスでは nullptr を返す．
  const PtStmt*
  init_stmt() const override;

  /// @brief 繰り返し代入文の取得
  /// @return 繰り返し代入文
  /// @note kFor で意味のある関数
  /// @note このクラスでは nullptr を返す．
  const PtStmt*
  next_stmt() const override;

  /// @brief 宣言ヘッダ配列の要素数の取得
  /// @note kNamedParBlock/kNamedSeqBlock で意味のある関数
  SizeType
  declhead_num() const override;

  /// @brief 宣言ヘッダの取得
  /// @param[in] pos 位置 ( 0 <= pos < declhead_num() )
  /// @note kNamedParBlock/kNamedSeqBlock で意味のある関数
  const PtDeclHead*
  declhead(SizeType pos) const override;

  /// @brief 子供のステートメント配列の要素数の取得
  /// @note kParBlock/kSeqBlock で意味のある関数
  SizeType
  stmt_num() const override;

  /// @brief 子供のステートメントの取得
  /// @param[in] pos 位置 ( 0 <= pos < stmt_num() )
  /// @note kParBlock/kSeqBlock で意味のある関数
  const PtStmt*
  stmt(SizeType pos) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

};


//////////////////////////////////////////////////////////////////////
/// @brief disable 文 を表すクラス
//////////////////////////////////////////////////////////////////////
class CptDisable :
  public CptStmt
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  CptDisable(const FileRegion& file_region,
	     const char* name);

  /// @brief デストラクタ
  ~CptDisable();


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  /// @note このクラスは PtStmtType::Disable を返す．
  PtStmtType
  type() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // PtDisable の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 末尾の名前を返す．
  const char*
  name() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 末尾の名前
  const char* mName;

};


//////////////////////////////////////////////////////////////////////
/// @brief 階層つき名前を持つ disable 文 を表すクラス
//////////////////////////////////////////////////////////////////////
class CptDisableH :
  public CptDisable
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  CptDisableH(const FileRegion& file_region,
	      PtiNameBranchArray&& nb_array,
	      const char* tail_name);

  /// @brief デストラクタ
  ~CptDisableH();


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 階層ブランチの要素数の取得
  SizeType
  namebranch_num() const override;

  /// @brief 階層ブランチの取得
  /// @param[in] pos 位置 ( 0 <= pos < namebranch_num() )
  const PtNameBranch*
  namebranch(SizeType pos) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 階層ブランチのリスト
  PtiNameBranchArray mNbArray;

};


//////////////////////////////////////////////////////////////////////
/// @brief task enable 文 / system task enable 文に共通な基底クラス
//////////////////////////////////////////////////////////////////////
class CptEnableBase :
  public CptStmt
{
protected:

  /// @brief コンストラクタ
  CptEnableBase(const FileRegion& file_region,
		const char* name,
		PtiExprArray&& arg_array);

  /// @brief デストラクタ
  ~CptEnableBase();


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 末尾の名前を返す．
  const char*
  name() const override;

  /// @brief 引数の数の取得
  SizeType
  arg_num() const override;

  /// @brief 引数の取得
  /// @param[in] pos 位置 ( 0 <= pos < arg_num() )
  const PtExpr*
  arg(SizeType pos) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 末尾の名前
  const char* mName;

  // 引数の配列
  PtiExprArray mArgArray;

};


//////////////////////////////////////////////////////////////////////
/// @brief 階層なしの名前を持った enable 文
//////////////////////////////////////////////////////////////////////
class CptEnable :
  public CptEnableBase
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  CptEnable(const FileRegion& file_region,
	    const char* name,
	    PtiExprArray&& arg_array);

  /// @brief デストラクタ
  ~CptEnable();


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  /// @note このクラスは PtStmtType::Enable を返す．
  PtStmtType
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// @brief 階層つきの名前をもった enable 文 を表すクラス
//////////////////////////////////////////////////////////////////////
class CptEnableH :
  public CptEnable
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  CptEnableH(const FileRegion& file_region,
	     PtiNameBranchArray&& nb_array,
	     const char* tail_name,
	     PtiExprArray&& arg_array);

  /// @brief デストラクタ
  ~CptEnableH();


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 階層ブランチの要素数の取得
  SizeType
  namebranch_num() const override;

  /// @brief 階層ブランチの取得
  /// @param[in] pos 位置 ( 0 <= pos < namebranch_num() )
  const PtNameBranch*
  namebranch(SizeType pos) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 階層ブランチのリスト
  PtiNameBranchArray mNbArray;

};


//////////////////////////////////////////////////////////////////////
/// @brief system task enable 文 を表すクラス
//////////////////////////////////////////////////////////////////////
class CptSysEnable :
  public CptEnableBase
{
  friend class CptFactory;

private:

  /// @brief コンストラクタ
  CptSysEnable(const FileRegion& file_region,
	       const char* task_name,
	       PtiExprArray&& arg_array);

  /// @brief デストラクタ
  ~CptSysEnable();


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  /// @note このクラスは PtStmtType::SysEnable を返す．
  PtStmtType
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// @brief CtrlStmt の基底クラス
//////////////////////////////////////////////////////////////////////
class CptCtrlStmt :
  public CptStmt
{
protected:

  /// @brief コンストラクタ
  CptCtrlStmt(const FileRegion& file_region,
	      const PtControl* delay,
	      const PtStmt* body);

  /// @brief デストラクタ
  ~CptCtrlStmt();


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief コントロールを返す．
  const PtControl*
  control() const override;

  /// @brief 本体を取り出す．
  const PtStmt*
  body() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // コントロール
  const PtControl* mDelay;

  // 本体
  const PtStmt* mBody;

};


//////////////////////////////////////////////////////////////////////
/// @brief delay control statement
//////////////////////////////////////////////////////////////////////
class CptDcStmt :
  public CptCtrlStmt
{
  friend class CptFactory;

private:

  /// @brief コンストラクタ
  CptDcStmt(const FileRegion& file_region,
	    const PtControl* delay,
	    const PtStmt* body);

  /// @brief デストラクタ
  ~CptDcStmt();


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  /// @note このクラスは PtStmtType::DelayControl を返す．
  PtStmtType
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// @brief event control statement
//////////////////////////////////////////////////////////////////////
class CptEcStmt :
  public CptCtrlStmt
{
  friend class CptFactory;

private:

  /// @brief コンストラクタ
  CptEcStmt(const FileRegion& file_region,
	    const PtControl* event,
	    const PtStmt* body);

  /// @brief デストラクタ
  ~CptEcStmt();


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  /// @note このクラスは PtStmtType::EventControl を返す．
  PtStmtType
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// @brief wait 文 を表すクラス
//////////////////////////////////////////////////////////////////////
class CptWait :
  public CptStmt
{
  friend class CptFactory;

private:

  /// @brief コンストラクタ
  CptWait(const FileRegion& file_region,
	  const PtExpr* expr,
	  const PtStmt* body);

  /// @brief デストラクタ
  ~CptWait() override;


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  /// @note このクラスは PtStmtType::Wait を返す．
  PtStmtType
  type() const override;

  /// @brief 条件を返す．
  const PtExpr*
  expr() const override;

  /// @brief 実行すべき本体を返す．
  const PtStmt*
  body() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 条件
  const PtExpr* mExpr;

  // 実行すべき本体
  const PtStmt* mBody;

};


//////////////////////////////////////////////////////////////////////
/// @brief 代入文系の基底クラス
//////////////////////////////////////////////////////////////////////
class CptAssignBase :
  public CptStmt
{
protected:

  /// @brief コンストラクタ
  CptAssignBase(const FileRegion& file_region,
		const PtExpr* lhs);

  /// @brief デストラクタ
  ~CptAssignBase();


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief lhs を得る．
  const PtExpr*
  lhs() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 左辺
  const PtExpr* mLhs;

};


//////////////////////////////////////////////////////////////////////
/// @brief 代入文
//////////////////////////////////////////////////////////////////////
class CptAssign :
  public CptAssignBase
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  CptAssign(const FileRegion& file_region,
	    const PtExpr* lhs,
	    const PtExpr* rhs);

  /// @brief デストラクタ
  ~CptAssign();


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  /// @note このクラスは PtStmtType::Assign を返す．
  PtStmtType
  type() const override;

  /// @brief rhs を得る．
  const PtExpr*
  rhs() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 右辺
  const PtExpr* mRhs;

};


//////////////////////////////////////////////////////////////////////
/// @brief コントロールつきの代入文
//////////////////////////////////////////////////////////////////////
class CptAssignC :
  public CptAssign
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  CptAssignC(const FileRegion& file_region,
	     const PtExpr* lhs,
	     const PtExpr* rhs,
	     const PtControl* control);

  /// @brief デストラクタ
  ~CptAssignC();


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief コントロールを返す．
  const PtControl*
  control() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // コントロール
  const PtControl* mControl;

};


//////////////////////////////////////////////////////////////////////
/// @brief ノンブロッキング代入文
//////////////////////////////////////////////////////////////////////
class CptNbAssign :
  public CptAssign
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  CptNbAssign(const FileRegion& file_region,
	      const PtExpr* lhs,
	      const PtExpr* rhs);

  /// @brief デストラクタ
  ~CptNbAssign();


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  /// @note このクラスは PtStmtType::NbAssign を返す．
  PtStmtType
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// @brief コントロールつきノンブロッキング代入文
//////////////////////////////////////////////////////////////////////
class CptNbAssignC :
  public CptAssignC
{
  friend class CptFactory;

private:

  /// @brief コンストラクタ
  CptNbAssignC(const FileRegion& file_region,
	       const PtExpr* lhs,
	       const PtExpr* rhs,
	       const PtControl* control);

  /// @brief デストラクタ
  ~CptNbAssignC();


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  /// @note このクラスは PtStmtType::NbAssign を返す．
  PtStmtType
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// @brief procedural continuous assigment 文
//////////////////////////////////////////////////////////////////////
class CptPcAssign :
  public CptAssignBase
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  CptPcAssign(const FileRegion& file_region,
	      const PtExpr* lhs,
	      const PtExpr* rhs);

  /// @brief デストラクタ
  ~CptPcAssign();


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  /// @note このクラスは kPtPcaAssignStmt を返す．
  PtStmtType
  type() const override;

  /// @brief 右辺式を返す．
  const PtExpr*
  rhs() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 右辺
  const PtExpr* mRhs;

};


//////////////////////////////////////////////////////////////////////
/// @brief deassigment 文
//////////////////////////////////////////////////////////////////////
class CptDeassign :
  public CptAssignBase
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  CptDeassign(const FileRegion& file_region,
	      const PtExpr* lhs);

  /// @brief デストラクタ
  ~CptDeassign();


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  /// @note このクラスは PtStmtType::Deassign を返す．
  PtStmtType
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// @brief force 文
//////////////////////////////////////////////////////////////////////
class CptForce :
  public CptPcAssign
{
  friend class CptFactory;

private:

  /// @brief コンストラクタ
  CptForce(const FileRegion& file_region,
	   const PtExpr* lhs,
	   const PtExpr* rhs);

  /// @brief デストラクタ
  ~CptForce();


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  /// @note このクラスは PtStmtType::Force を返す．
  PtStmtType
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// @brief release 文
//////////////////////////////////////////////////////////////////////
class CptRelease :
  public CptDeassign
{
  friend class CptFactory;

private:

  /// @brief コンストラクタ
  CptRelease(const FileRegion& file_region,
	     const PtExpr* lhs);

  /// @brief デストラクタ
  ~CptRelease();


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  /// @note このクラスは PtStmtType::Release を返す．
  PtStmtType
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// @brief event statement を表すノード
//////////////////////////////////////////////////////////////////////
class CptEventStmt :
  public CptStmt
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  CptEventStmt(const FileRegion& file_region,
	       const PtExpr* event);

  /// @brief デストラクタ
  ~CptEventStmt();


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  /// @note このクラスは PtStmtType::Event を返す．
  PtStmtType
  type() const override;

  /// @brief イベントプライマリを返す．
  const PtExpr*
  primary() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // イベント名を表すプライマリ
  const PtExpr* mPrimary;

};


//////////////////////////////////////////////////////////////////////
/// @brief null statement を表すノード
//////////////////////////////////////////////////////////////////////
class CptNullStmt :
  public CptStmt
{
  friend class CptFactory;

private:

  /// @brief コンストラクタ
  CptNullStmt(const FileRegion& file_region);

  /// @brief デストラクタ
  ~CptNullStmt();


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  /// @note このクラスは PtStmtType::Null を返す．
  PtStmtType
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// @brief if 文 を表すクラス
//////////////////////////////////////////////////////////////////////
class CptIf :
  public CptStmt
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  CptIf(const FileRegion& file_region,
	const PtExpr* expr,
	const PtStmt* then_body);

  /// @brief デストラクタ
  ~CptIf();


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  /// @note このクラスは PtStmtType::If を返す．
  PtStmtType
  type() const override;

  /// @brief 条件式を返す．
  const PtExpr*
  expr() const override;

  /// @brief 成り立ったとき実行されるステートメント
  const PtStmt*
  body() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 条件式
  const PtExpr* mCond;

  // 成り立ったとき実行されるステートメント
  const PtStmt* mThen;

};


//////////////////////////////////////////////////////////////////////
/// @brief else 節付きの if 文 を表すクラス
//////////////////////////////////////////////////////////////////////
class CptIfElse :
  public CptIf
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  CptIfElse(const FileRegion& file_region,
	    const PtExpr* expr,
	    const PtStmt* then_body,
	    const PtStmt* else_body);

  /// @brief デストラクタ
  ~CptIfElse();


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 成り立たなかったとき実行されるステートメント
  const PtStmt*
  else_body() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 成り立たなかったとき実行されるステートメント
  const PtStmt* mElse;

};


//////////////////////////////////////////////////////////////////////
/// @brief caseitemを表すクラス
//////////////////////////////////////////////////////////////////////
class CptCaseItem :
  public PtCaseItem
{
  friend class CptFactory;

private:

  /// @brief コンストラクタ
  CptCaseItem(const FileRegion& file_region,
	      PtiExprArray&& label_array,
	      const PtStmt* body);

  /// @brief デストラクタ
  ~CptCaseItem();


public:
  //////////////////////////////////////////////////////////////////////
  // PtCaseItem の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  FileRegion
  file_region() const override;

  /// @brief ラベルのリストの要素数の取得
  SizeType
  label_num() const override;

  /// @brief ラベルの取得
  /// @param[in] pos 位置 ( 0 <= pos < label_num() )
  const PtExpr*
  label(SizeType pos) const override;

  /// @brief 本体のステートメント得る．
  const PtStmt*
  body() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // ラベルの配列
  PtiExprArray mLabelArray;

  // ラベルが一致したときに実行されるステートメント
  const PtStmt* mBody;

};


//////////////////////////////////////////////////////////////////////
/// @brief case 文 を表すクラス
//////////////////////////////////////////////////////////////////////
class CptCase :
  public CptStmt
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  CptCase(const FileRegion& file_region,
	  const PtExpr* expr,
	  PtiCaseItemArray&& caseitem_array);

  /// @brief デストラクタ
  ~CptCase();


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  /// @note このクラスは PtStmtType::Case を返す．
  PtStmtType
  type() const override;

  /// @brief 比較される式を返す．
  const PtExpr*
  expr() const override;

  /// @brief case item のリストの要素数の取得
  SizeType
  caseitem_num() const override;

  /// @brief case item の取得
  /// @param[in] pos 位置 ( 0 <= pos < caseitem_num() )
  const PtCaseItem*
  caseitem(SizeType pos) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 比較される式
  const PtExpr* mExpr;

  // case item の配列
  PtiCaseItemArray mCaseItemArray;

};


//////////////////////////////////////////////////////////////////////
/// @brief casex 文 を表すクラス
//////////////////////////////////////////////////////////////////////
class CptCaseX :
  public CptCase
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  CptCaseX(const FileRegion& file_region,
	   const PtExpr* expr,
	   PtiCaseItemArray&& caseitem_array);

  /// @brief デストラクタ
  ~CptCaseX();


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  /// @note ここでは PtStmtType::CaseX を返す．
  PtStmtType
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// @brief casez 文 を表すクラス
//////////////////////////////////////////////////////////////////////
class CptCaseZ :
  public CptCase
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  CptCaseZ(const FileRegion& file_region,
	   const PtExpr* expr,
	   PtiCaseItemArray&& caseitem_array);

  /// @brief デストラクタ
  ~CptCaseZ();


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  /// @note ここでは PtStmtType::CaseZ を返す．
  PtStmtType
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// @brief LoopStmt のベースクラス
//////////////////////////////////////////////////////////////////////
class CptLoopStmt :
  public CptStmt
{
protected:

  /// @brief コンストラクタ
  CptLoopStmt(const FileRegion& file_region,
	      const PtStmt* body);

  /// @brief デストラクタ
  ~CptLoopStmt();


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 実行すべき本体を返す．
  const PtStmt*
  body() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実行すべき本体
  const PtStmt* mBody;

};


//////////////////////////////////////////////////////////////////////
/// @brief forever 文 を表すクラス
//////////////////////////////////////////////////////////////////////
class CptForever :
  public CptLoopStmt
{
  friend class CptFactory;

private:

  /// @brief コンストラクタ
  CptForever(const FileRegion& file_region,
	     const PtStmt* body);

  /// @brief デストラクタ
  ~CptForever();


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  /// @note このクラスは PtStmtType::Forever を返す．
  PtStmtType
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// @brief repeat 文 を表すクラス
//////////////////////////////////////////////////////////////////////
class CptRepeat :
  public CptLoopStmt
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  CptRepeat(const FileRegion& file_region,
	    const PtExpr* expr,
	    const PtStmt* body);

  /// @brief デストラクタ
  ~CptRepeat();


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  /// @note このクラスは PtStmtType::Repeat を返す．
  PtStmtType
  type() const override;

  /// @brief 繰り返し式を返す．
  const PtExpr*
  expr() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 繰り返し式
  const PtExpr* mExpr;

};


//////////////////////////////////////////////////////////////////////
/// @brief while 文 を表すクラス
//////////////////////////////////////////////////////////////////////
class CptWhile :
  public CptRepeat // ちょっと汚い継承
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  CptWhile(const FileRegion& file_region,
	   const PtExpr* expr,
	   const PtStmt* body);

  /// @brief デストラクタ
  ~CptWhile();


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  /// @note このクラスは PtStmtType::White を返す．
  PtStmtType
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// @brief for 文 を表すクラス
//////////////////////////////////////////////////////////////////////
class CptFor :
  public CptRepeat
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  CptFor(const FileRegion& file_region,
	 const PtStmt* init,
	 const PtExpr* cond,
	 const PtStmt* next,
	 const PtStmt* body);

  /// @brief デストラクタ
  ~CptFor();


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  /// @note このクラスは PtStmtType::For を返す．
  PtStmtType
  type() const override;

  /// @brief 初期化代入式を取出す
  const PtStmt*
  init_stmt() const override;

  /// @brief 繰り返し代入式を取出す
  const PtStmt*
  next_stmt() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 初期化文
  const PtStmt* mInit;

  // 増加処理文
  const PtStmt* mNext;

};


//////////////////////////////////////////////////////////////////////
/// @brief StmtBlock の基底クラス
//////////////////////////////////////////////////////////////////////
class CptStmtBlock :
  public CptStmt
{
protected:

  /// @brief コンストラクタ
  CptStmtBlock(const FileRegion& file_region,
	       PtiStmtArray&& stmt_array);

  /// @brief デストラクタ
  ~CptStmtBlock();


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmtBlock の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 子供のステートメント配列の要素数の取得
  /// @note kParBlock/kSeqBlock で意味のある関数
  SizeType
  stmt_num() const override;

  /// @brief 子供のステートメントの取得
  /// @param[in] pos 位置 ( 0 <= pos < stmt_num() )
  /// @note kParBlock/kSeqBlock で意味のある関数
  const PtStmt*
  stmt(SizeType pos) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ステートメントの配列
  PtiStmtArray mStmtArray;

};


//////////////////////////////////////////////////////////////////////
/// @brief 名前と宣言要素を持つ StmtBlock
//////////////////////////////////////////////////////////////////////
class CptStmtBlockN :
  public CptStmtBlock
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  CptStmtBlockN(const FileRegion& file_region,
		const char* name,
		PtiDeclHeadArray&& declhead_array,
		PtiStmtArray&& stmt_array);

  /// @brief デストラクタ
  ~CptStmtBlockN();


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を取り出す．
  const char*
  name() const override;

  /// @brief 宣言ヘッダ配列の要素数の取得
  /// @note kNamedParBlock/kNamedSeqBlock で意味のある関数
  SizeType
  declhead_num() const override;

  /// @brief 宣言ヘッダの取得
  /// @param[in] pos 位置 ( 0 <= pos < declhead_num() )
  /// @note kNamedParBlock/kNamedSeqBlock で意味のある関数
  const PtDeclHead*
  declhead(SizeType pos) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  const char* mName;

  // 宣言の配列
  PtiDeclHeadArray mDeclHeadArray;

};


//////////////////////////////////////////////////////////////////////
/// @brief parallel block を表すノード
//////////////////////////////////////////////////////////////////////
class CptParBlock :
  public CptStmtBlock
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  CptParBlock(const FileRegion& file_region,
	      PtiStmtArray&& stmt_array);

  /// @brief デストラクタ
  ~CptParBlock();


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  /// @note このクラスは PtStmtType::ParBlock を返す．
  PtStmtType
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// @brief 名前と宣言要素を持つ parallel block を表すノード
//////////////////////////////////////////////////////////////////////
class CptParBlockN :
  public CptStmtBlockN
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  CptParBlockN(const FileRegion& file_region,
	       const char* name,
	       PtiDeclHeadArray&& declhead_array,
	       PtiStmtArray&& stmt_array);

  /// @brief デストラクタ
  ~CptParBlockN();


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  /// @note このクラスは PtStmtType::NamedParBlock を返す．
  PtStmtType
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// @brief sequential block を表すノード
//////////////////////////////////////////////////////////////////////
class CptSeqBlock :
  public CptStmtBlock
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  CptSeqBlock(const FileRegion& file_region,
	      PtiStmtArray&& stmt_array);

  /// @brief デストラクタ
  ~CptSeqBlock();


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  /// @note このクラスは PtStmtType::SeqBlock を返す．
  PtStmtType
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// @brief 名前と宣言要素を持つ sequential block を表すノード
//////////////////////////////////////////////////////////////////////
class CptSeqBlockN :
  public CptStmtBlockN
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  CptSeqBlockN(const FileRegion& file_region,
	       const char* name,
	       PtiDeclHeadArray&& declhead_array,
	       PtiStmtArray&& stmt_array);

  /// @brief デストラクタ
  ~CptSeqBlockN();


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  /// @note このクラスは PtStmtType::NamedSeqBlock を返す．
  PtStmtType
  type() const override;

};

END_NAMESPACE_YM_VERILOG

#endif // CPTSTMT_H
