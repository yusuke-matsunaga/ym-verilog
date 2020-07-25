#ifndef CPTITEM_H
#define CPTITEM_H

/// @file CptItem.h
/// @brief CptItem のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym/pt/PtItem.h"
#include "ym/pt/PtArray.h"

#include "ym/FileRegion.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// item の基底クラス
//////////////////////////////////////////////////////////////////////
class CptItem :
  public PtItem
{
protected:

  /// @brief コンストラクタ
  CptItem();

  /// @brief デストラクタ
  ~CptItem();


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief プリミティブタイプの取得
  /// @return プリミティブタイプ
  /// @note このクラスでは kVpiAndPrim を返す．
  VpiPrimType
  prim_type() const override;

  /// @brief strength の取得
  /// @return 信号強度
  /// @note このクラスでは nullptr を返す．
  const PtStrength*
  strength() const override;

  /// @brief delay の取得
  /// @return 遅延
  /// @note このクラスでは nullptr を返す．
  const PtDelay*
  delay() const override;

  /// @brief パラメータ割り当てリストの取得
  const PtConnectionArray*
  paramassign_array() const override;

  /// @brief defparam のリストの取得
  const PtDefParamArray*
  defparam_list() const override;

  /// @brief continuous assign のリストの取得
  const PtContAssignArray*
  contassign_list() const override;

  /// @brief module/UDP/gate instance リストの取得
  const PtInstArray*
  inst_list() const override;

  /// @brief automatic 情報の取得
  /// @retval true automatic 宣言された task/function
  /// @retval false 上記以外
  bool
  automatic() const override;

  /// @brief 名前の取得
  /// @return 名前
  /// @note このクラスでは nullptr を返す．
  const char*
  name() const override;

  /// @brief IO宣言の要素数の取得
  int
  ioitem_num() const override;

  /// @brief IO宣言リストの配列の取得
  const PtIOHeadArray*
  iohead_array() const override;

  /// @brief 宣言ヘッダ配列の取得
  const PtDeclHeadArray*
  declhead_array() const override;

  /// @brief item 配列の取得
  const PtItemArray*
  item_array() const override;

  /// @brief 本体のステートメントの取得
  /// @return 本体のステートメント
  /// @note このクラスでは nullptr を返す．
  const PtStmt*
  body() const override;

  /// @brief 符号の取得
  /// @retval true 符号つき
  /// @retval false 符号なし
  /// @note このクラスでは false を返す．
  bool
  is_signed() const override;

  /// @brief 範囲の左側の式の取得
  /// @return 範囲の左側の式
  /// @note このクラスでは nullptr を返す．
  const PtExpr*
  left_range() const override;

  /// @brief 範囲の右側の式の取得
  /// @return 範囲の右側の式
  /// @note このクラスでは nullptr を返す．
  const PtExpr*
  right_range() const override;

  /// @brief 戻値のデータ型の取得
  /// @return 戻値のデータ型
  /// @note このクラスでは kVpiVarNone を返す．
  VpiVarType
  data_type() const override;

  /// @brief constant function の展開中の印をつける．
  /// @note このクラスではなにもしない．
  void
  set_in_use() const override;

  /// @brief constant function の展開中の印を消す．
  /// @note このクラスではなにもしない．
  void
  clear_in_use() const override;

  /// @brief 使用中(constant function として展開中)のチェック
  /// @return 使用中の時 true を返す．
  /// @note このクラスでは false を返す．
  bool
  is_in_use() const override;

  /// @brief specify block item の種類の取得
  /// @return specify block item の種類
  /// @note このクラスでは kVpiPulsestyleOnEvent を返す．
  VpiSpecItemType
  specitem_type() const override;

  /// @brief specify block path の種類の取得
  /// @return specify block path の種類
  /// @note このクラスでは kVpiSpecPathNull を返す．
  VpiSpecPathType
  specpath_type() const override;

  /// @brief ターミナルのリストの取得
  const PtExprArray*
  terminal_list() const override;

  /// @brief パス記述の取得
  /// @return パス記述
  /// @note このクラスでは nullptr を返す．
  const PtPathDecl*
  path_decl() const override;

  /// @brief 条件式の取得
  /// @return 条件式
  /// @note このクラスでは nullptr を返す．
  const PtExpr*
  expr() const override;

  /// @brief 条件が成り立ったときに生成される宣言ヘッダ配列の取得
  const PtDeclHeadArray*
  then_declhead_array() const override;

  /// @brief 条件が成り立ったときに生成される item 配列の取得
  const PtItemArray*
  then_item_array() const override;

  /// @brief 条件が成り立たなかったときに生成される宣言ヘッダ配列の取得
  const PtDeclHeadArray*
  else_declhead_array() const override;

  /// @brief 条件が成り立たなかったときに生成される item 配列の取得
  const PtItemArray*
  else_item_array() const override;

  /// @brief case item のリストの取得
  const PtGenCaseItemArray*
  caseitem_list() const override;

  /// @brief 繰り返し制御用の変数名の取得
  /// @return 繰り返し制御用の変数名
  /// @note このクラスでは nullptr を返す．
  const char*
  loop_var() const override;

  /// @brief 初期化文の右辺の取得
  /// @return 初期化文の右辺
  /// @note このクラスでは nullptr を返す．
  const PtExpr*
  init_expr() const override;

  /// @brief 増加文の右辺の取得
  /// @return 増加文の右辺
  /// @note このクラスでは nullptr を返す．
  const PtExpr*
  next_expr() const override;

};


//////////////////////////////////////////////////////////////////////
/// @brief defparam 文のヘッダ
//////////////////////////////////////////////////////////////////////
class CptDefParamH :
  public CptItem
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  CptDefParamH(const FileRegion& file_region,
	       const PtDefParamArray* dp_array);

  /// @brief デストラクタ
  ~CptDefParamH();


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置の取得
  FileRegion
  file_region() const override;

  /// @brief 型を返す．
  /// @note ここでは PtItemType::DefParam を返す．
  PtItemType
  type() const override;

  /// @brief defparam リストの取得
  const PtDefParamArray*
  defparam_list() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 要素の配列
  const PtDefParamArray* mArray;

};


//////////////////////////////////////////////////////////////////////
/// @brief defparam 文を表すクラス
//////////////////////////////////////////////////////////////////////
class CptDefParam :
  public PtDefParam
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  /// @note value は常に定数式
  CptDefParam(const FileRegion& file_region,
	      const PtNameBranchArray* nb_array,
	      const char* tail_name,
	      const PtExpr* value);

  /// @brief デストラクタ
  ~CptDefParam();


public:
  //////////////////////////////////////////////////////////////////////
  // PtDefParam の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置の取得
  /// @return ファイル位置
  FileRegion
  file_region() const override;

  /// @brief 階層ブランチの取得
  const PtNameBranchArray*
  namebranch_array() const override;

  /// @brief 末尾の名前の取得
  /// @return 末尾の名前
  const char*
  name() const override;

  /// @brief 値の取得
  /// @return 値を表す式
  const PtExpr*
  expr() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 先頭の位置
  FileLoc mTopLoc;

  // 階層ブランチのリスト
  const PtNameBranchArray* mNbArray;

  // 末尾の名前
  const char* mName;

  // 値
  const PtExpr* mExpr;

};


//////////////////////////////////////////////////////////////////////
/// @brief continuous assign 文のヘッダ
//////////////////////////////////////////////////////////////////////
class CptContAssignH :
  public CptItem
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  CptContAssignH(const FileRegion& file_region,
		 const PtContAssignArray* ca_array);

  /// @brief デストラクタ
  ~CptContAssignH();


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  FileRegion
  file_region() const override;

  /// @brief 型を返す．
  /// @note ここでは PtItemType::ContAssign を返す．
  PtItemType
  type() const override;

  /// @brief continuous assign リストの取得
  /// @param[in] pos 位置番号 ( 0 <= pos < size() )
  const PtContAssignArray*
  contassign_list() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 要素の配列
  const PtContAssignArray* mArray;

};


//////////////////////////////////////////////////////////////////////
/// @brief strength つきの continuous assign 文のヘッダ
//////////////////////////////////////////////////////////////////////
class CptContAssignHS :
  public CptContAssignH
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  CptContAssignHS(const FileRegion& file_region,
		  const PtStrength* str,
		  const PtContAssignArray* ca_array);

  /// @brief デストラクタ
  ~CptContAssignHS();


public:
  //////////////////////////////////////////////////////////////////////
  // PtContAssignH の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief strength を返す．
  const PtStrength*
  strength() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // strength
  const PtStrength* mStrength;

};


//////////////////////////////////////////////////////////////////////
/// @brief delay つきの continuous assign 文のヘッダ
//////////////////////////////////////////////////////////////////////
class CptContAssignHD :
  public CptContAssignH
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  CptContAssignHD(const FileRegion& file_region,
		  const PtDelay* delay,
		  const PtContAssignArray* ca_array);

  /// @brief デストラクタ
  ~CptContAssignHD();


public:
  //////////////////////////////////////////////////////////////////////
  // PtContAssignH の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief delay を返す．
  const PtDelay*
  delay() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // delay
  const PtDelay* mDelay;

};


//////////////////////////////////////////////////////////////////////
/// @brief strength と delay つきの continuous assign 文のヘッダ
//////////////////////////////////////////////////////////////////////
class CptContAssignHSD :
  public CptContAssignH
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  CptContAssignHSD(const FileRegion& file_region,
		   const PtStrength* str,
		   const PtDelay* delay,
		   const PtContAssignArray* ca_array);

  /// @brief デストラクタ
  ~CptContAssignHSD();


public:
  //////////////////////////////////////////////////////////////////////
  // PtContAssignH の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief strength を返す．
  const PtStrength*
  strength() const override;

  /// @brief delay を返す．
  const PtDelay*
  delay() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // strength
  const PtStrength* mStrength;

  // delay
  const PtDelay* mDelay;

};


//////////////////////////////////////////////////////////////////////
/// @brief continuous assign 文
//////////////////////////////////////////////////////////////////////
class CptContAssign :
  public PtContAssign
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  CptContAssign(const PtExpr* lhs,
		const PtExpr* rhs);

  /// @brief デストラクタ
  ~CptContAssign();


public:
  //////////////////////////////////////////////////////////////////////
  // PtContAssign の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を取り出す．
  FileRegion
  file_region() const override;

  /// @brief 左辺式を取り出す．
  const PtExpr*
  lhs() const override;

  /// @brief 右辺式を取り出す．
  const PtExpr*
  rhs() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 左辺式
  const PtExpr* mLhs;

  // 右辺式
  const PtExpr* mRhs;

};


//////////////////////////////////////////////////////////////////////
/// @brief inititial 文/always 文に共通なクラス
//////////////////////////////////////////////////////////////////////
class CptProcess :
  public CptItem
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  CptProcess(const FileRegion& file_region,
	     const PtStmt* body);

  /// @brief デストラクタ
  ~CptProcess();


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を取り出す．
  FileRegion
  file_region() const override;

  /// @brief 本体のステートメントを返す．
  const PtStmt*
  body() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 先頭のファイル位置
  FileLoc mTopLoc;

  // 本体
  const PtStmt* mBody;

};


//////////////////////////////////////////////////////////////////////
/// @brief inititial 文
//////////////////////////////////////////////////////////////////////
class CptInitial :
  public CptProcess
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  CptInitial(const FileRegion& file_region,
	     const PtStmt* body);

  /// @brief デストラクタ
  ~CptInitial();


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  /// @note ここでは PtItemType::Initial を返す．
  PtItemType
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// @brief always 文
//////////////////////////////////////////////////////////////////////
class CptAlways :
  public CptProcess
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  CptAlways(const FileRegion& file_region,
	    const PtStmt* body);

  /// @brief デストラクタ
  ~CptAlways();


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  /// @note ここでは PtItemType::Always を返す．
  PtItemType
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// @brief task と function に共通な基底クラス
//////////////////////////////////////////////////////////////////////
class CptTf :
  public CptItem
{
protected:

  /// @brief コンストラクタ
  CptTf(const FileRegion& file_region,
	const char* name,
	bool automatic,
	const PtIOHeadArray* iohead_array,
	const PtDeclHeadArray* declhead_array,
	const PtStmt* stmt);

  /// @brief デストラクタ
  ~CptTf();


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  FileRegion
  file_region() const override;

  /// @brief 名前を取り出す．
  const char*
  name() const override;

  /// @brief automatic なら true
  bool
  automatic() const override;

  /// @brief IO宣言の要素数の取得
  int
  ioitem_num() const override;

  /// @brief IO宣言リストの配列の取得
  const PtIOHeadArray*
  iohead_array() const override;

  /// @brief 宣言ヘッダ配列の取得
  const PtDeclHeadArray*
  declhead_array() const override;

  /// @brief 本体を取り出す．
  const PtStmt*
  body() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 名前
  const char* mName;

  // automatic 属性
  bool mAutomatic;

  // IO宣言の要素数
  int mIOItemNum;

  // IO宣言の配列
  const PtIOHeadArray* mIOHeadArray;

  // その他の宣言の配列
  const PtDeclHeadArray* mDeclHeadArray;

  // 本体
  const PtStmt* mBody;

};


//////////////////////////////////////////////////////////////////////
/// @brief task を表すクラス
//////////////////////////////////////////////////////////////////////
class CptTask :
  public CptTf
{
  friend class CptFactory;

private:

  /// @brief コンストラクタ
  CptTask(const FileRegion& file_region,
	  const char* name,
	  bool automatic,
	  const PtIOHeadArray* iohead_array,
	  const PtDeclHeadArray* declhead_array,
	  const PtStmt* stmt);

  /// @brief デストラクタ
  ~CptTask();


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  /// @note ここでは PtItemType::Task を返す．
  PtItemType
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// @brief function を表すクラス
//////////////////////////////////////////////////////////////////////
class CptFunction :
  public CptTf
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  CptFunction(const FileRegion& file_region,
	      const char* name,
	      bool automatic,
	      bool sign,
	      const PtIOHeadArray* iohead_array,
	      const PtDeclHeadArray* declhead_array,
	      const PtStmt* stmt);

  /// @brief デストラクタ
  ~CptFunction();


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  /// @note ここでは PtItemType::Function を返す．
  PtItemType
  type() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // PtTf の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 符号の有無
  bool
  is_signed() const override;

  /// @brief constant function の展開中の印をつける．
  void
  set_in_use() const override;

  /// @brief constant function の展開中の印を消す．
  void
  clear_in_use() const override;

  /// @brief 使用中(constant function として展開中)の時 true を返す．
  bool
  is_in_use() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 符号の有無
  bool mSigned;

  // 使用中かどうかを示すフラグ
  mutable bool mInUse;

};


//////////////////////////////////////////////////////////////////////
/// @brief ビットベクタタイプの function を表すノード
//////////////////////////////////////////////////////////////////////
class CptSizedFunc :
  public CptFunction
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  CptSizedFunc(const FileRegion& file_region,
	       const char* name,
	       bool automatic,
	       bool sign,
	       const PtExpr* left,
	       const PtExpr* right,
	       const PtIOHeadArray* iohead_array,
	       const PtDeclHeadArray* declhead_array,
	       const PtStmt* stmt);

  /// @brief デストラクタ
  ~CptSizedFunc();


public:
  //////////////////////////////////////////////////////////////////////
  // PtTf の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 範囲の MSB を得る．
  const PtExpr*
  left_range() const override;

  /// @brief 範囲の LSB を得る．
  const PtExpr*
  right_range() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 範囲のMSB
  const PtExpr* mLeftRange;

  // 範囲のLSB
  const PtExpr* mRightRange;

};


//////////////////////////////////////////////////////////////////////
/// @brief 組み込み型の function を表すノード
//////////////////////////////////////////////////////////////////////
class CptTypedFunc :
  public CptFunction
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  CptTypedFunc(const FileRegion& file_region,
	       const char* name,
	       bool automatic,
	       bool sign,
	       VpiVarType dat_type,
	       const PtIOHeadArray* iohead_array,
	       const PtDeclHeadArray* declhead_array,
	       const PtStmt* stmt);

  /// @brief デストラクタ
  ~CptTypedFunc();


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 戻値の型を返す．
  VpiVarType
  data_type() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 戻値の型
  VpiVarType mDataType;

};

END_NAMESPACE_YM_VERILOG

#endif // CPTITEM_H
