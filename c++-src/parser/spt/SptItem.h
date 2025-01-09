#ifndef SPTITEM_H
#define SPTITEM_H

/// @file SptItem.h
/// @brief SptItem のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.

#include "ym/FileRegion.h"

#include "ym/pt/PtItem.h"
#include "parser/PtiArray.h"
#include "parser/PtiFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @brief item の基底クラス
//////////////////////////////////////////////////////////////////////
class SptItem :
  public PtItem
{
protected:

  /// @brief コンストラクタ
  SptItem(
    const FileRegion& file_region,
    PtItemType type
  );

  /// @brief デストラクタ
  ~SptItem();


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置の取得
  FileRegion
  file_region() const override;

  /// @brief 型の取得
  PtItemType
  type() const override;

  /// @brief プリミティブタイプの取得
  /// @return プリミティブタイプ
  ///
  /// このクラスでは kVpiAndPrim を返す．
  VpiPrimType
  prim_type() const override;

  /// @brief strength の取得
  /// @return 信号強度
  ///
  /// このクラスでは nullptr を返す．
  const PtStrength*
  strength() const override;

  /// @brief delay の取得
  /// @return 遅延
  ///
  /// このクラスでは nullptr を返す．
  const PtDelay*
  delay() const override;

  /// @brief パラメータ割り当て数の取得
  SizeType
  paramassign_num() const override;

  /// @brief パラメータ割り当ての取得
  const PtConnection*
  paramassign(
    SizeType pos ///< [in] 位置 ( 0 <= pos < paramassign_num() )
  ) const override;

  /// @brief defparam の要素数の取得
  SizeType
  defparam_num() const override;

  /// @brief defparam の取得
  const PtDefParam*
  defparam(
    SizeType pos ///< [in] 位置 ( 0 <= pos < defparam_num() )
  ) const override;

  /// @brief continuous assign の要素数の取得
  SizeType
  contassign_num() const override;

  /// @brief continuous assign の取得
  const PtContAssign*
  contassign(
    SizeType pos ///< [in] 位置 ( 0 <= pos < contassign_num() )
  ) const override;

  /// @brief module/UDP/gate instance の要素数の取得
  SizeType
  inst_num() const override;

  /// @brief module/UDP/gate instance の取得
  const PtInst*
  inst(
    SizeType pos ///< [in] 位置 ( 0 <= pos < inst_num() )
  ) const override;

  /// @brief 名前の取得
  /// @return 名前
  ///
  /// このクラスでは nullptr を返す．
  const char*
  name() const override;

  /// @brief automatic 情報の取得
  /// @retval true automatic 宣言された task/function
  /// @retval false 上記以外
  ///
  /// このクラスでは false を返す．
  bool
  automatic() const override;

  /// @brief IO宣言の要素数の取得
  SizeType
  ioitem_num() const override;

  /// @brief IO宣言ヘッダリストの要素数の取得
  SizeType
  iohead_num() const override;

  /// @brief IO宣言ヘッダの取得
  const PtIOHead*
  iohead(
    SizeType pos ///< [in] 位置 ( 0 <= pos < iohead_num() )
  ) const override;

  /// @brief 宣言ヘッダの要素数の取得
  SizeType
  declhead_num() const override;

  /// @brief 宣言ヘッダの取得
  const PtDeclHead*
  declhead(
    SizeType pos ///< [in] 位置 ( 0 <= pos < declhead_num() )
  ) const override;

  /// @brief item リストの要素数の取得
  SizeType
  item_num() const override;

  /// @brief item の取得
  const PtItem*
  item(
    SizeType pos ///< [in] 位置 ( 0 <= pos < item_num() )
  ) const override;

  /// @brief 本体のステートメントの取得
  /// @return 本体のステートメント
  ///
  /// このクラスでは nullptr を返す．
  const PtStmt*
  body() const override;

  /// @brief 符号の取得
  /// @retval true 符号つき
  /// @retval false 符号なし
  ///
  /// このクラスでは false を返す．
  bool
  is_signed() const override;

  /// @brief 範囲の左側の式の取得
  /// @return 範囲の左側の式
  ///
  /// このクラスでは nullptr を返す．
  const PtExpr*
  left_range() const override;

  /// @brief 範囲の右側の式の取得
  /// @return 範囲の右側の式
  ///
  /// このクラスでは nullptr を返す．
  const PtExpr*
  right_range() const override;

  /// @brief 戻値のデータ型の取得
  /// @return 戻値のデータ型
  ///
  /// このクラスでは kVpiVarNone を返す．
  VpiVarType
  data_type() const override;

  /// @brief constant function の展開中の印をつける．
  ///
  /// このクラスではなにもしない．
  void
  set_in_use() const override;

  /// @brief constant function の展開中の印を消す．
  ///
  /// このクラスではなにもしない．
  void
  clear_in_use() const override;

  /// @brief 使用中(constant function として展開中)のチェック
  /// @return 使用中の時 true を返す．
  ///
  /// このクラスでは false を返す．
  bool
  is_in_use() const override;

  /// @brief specify block item の種類の取得
  /// @return specify block item の種類
  ///
  /// このクラスでは kVpiPulsestyleOnEvent を返す．
  VpiSpecItemType
  specitem_type() const override;

  /// @brief specify block path の種類の取得
  /// @return specify block path の種類
  ///
  /// このクラスでは kVpiSpecPathNull を返す．
  VpiSpecPathType
  specpath_type() const override;

  /// @brief ターミナルの要素数の取得
  SizeType
  terminal_num() const override;

  /// @brief ターミナルの取得
  const PtExpr*
  terminal(
    SizeType pos ///< [in] 位置 ( 0 <= pos < terminal_num() )
  ) const override;

  /// @brief パス記述の取得
  /// @return パス記述
  ///
  /// このクラスでは nullptr を返す．
  const PtPathDecl*
  path_decl() const override;

  /// @brief 条件式の取得
  /// @return 条件式
  ///
  /// このクラスでは nullptr を返す．
  const PtExpr*
  expr() const override;

  /// @brief 条件が成り立ったときに生成される宣言ヘッダ配列の要素数の取得
  SizeType
  then_declhead_num() const override;

  /// @brief 条件が成り立ったときに生成される宣言ヘッダの取得
  const PtDeclHead*
  then_declhead(
    SizeType pos ///< [in] 位置 ( 0 <= pos < then_declhead_num() )
  ) const override;

  /// @brief 条件が成り立ったときに生成される item 配列の要素数の取得
  SizeType
  then_item_num() const override;

  /// @brief 条件が成り立ったときに生成される item の取得
  const PtItem*
  then_item(
    SizeType pos ///< [in] 位置 ( 0 <= pos < then_item_num() )
  ) const override;

  /// @brief 条件が成り立たなかったときに生成される宣言ヘッダ配列の要素数の取得
  SizeType
  else_declhead_num() const override;

  /// @brief 条件が成り立たなかったときに生成される宣言ヘッダの取得
  const PtDeclHead*
  else_declhead(
    SizeType pos ///< [in] 位置 ( 0 <= pos < else_declhead_num() )
  ) const override;

  /// @brief 条件が成り立たなかったときに生成される item 配列の要素数の取得
  SizeType
  else_item_num() const override;

  /// @brief 条件が成り立たなかったときに生成される item の取得
  const PtItem*
  else_item(
    SizeType pos ///< [in] 位置 ( 0 <= pos < else_item_num() )
  ) const override;

  /// @brief case item のリストの要素数の取得
  SizeType
  caseitem_num() const override;

  /// @brief case item の取得
  const PtGenCaseItem*
  caseitem(
    SizeType pos ///< [in] 位置 ( 0 <= pos < caseitem_num() )
  ) const override;

  /// @brief 繰り返し制御用の変数名の取得
  /// @return 繰り返し制御用の変数名
  ///
  /// このクラスでは nullptr を返す．
  const char*
  loop_var() const override;

  /// @brief 初期化文の右辺の取得
  /// @return 初期化文の右辺
  ///
  /// このクラスでは nullptr を返す．
  const PtExpr*
  init_expr() const override;

  /// @brief 増加文の右辺の取得
  /// @return 増加文の右辺
  ///
  /// このクラスでは nullptr を返す．
  const PtExpr*
  next_expr() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 型
  PtItemType mType;

};


//////////////////////////////////////////////////////////////////////
/// @brief defparam 文のヘッダ
//////////////////////////////////////////////////////////////////////
class SptDefParamH :
  public SptItem
{
  friend class SptFactory;

private:

  /// @brief コンストラクタ
  SptDefParamH(
    const FileRegion& file_region,
    PtiDefParamArray&& dp_array
  );

  /// @brief デストラクタ
  ~SptDefParamH();


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief defparam の要素数の取得
  SizeType
  defparam_num() const override;

  /// @brief defparam の取得
  const PtDefParam*
  defparam(
    SizeType pos ///< [in] 位置 ( 0 <= pos < defparam_num() )
  ) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 要素の配列
  PtiDefParamArray mArray;

};


//////////////////////////////////////////////////////////////////////
/// @brief defparam 文を表すクラス
//////////////////////////////////////////////////////////////////////
class SptDefParam :
  public PtDefParam
{
  friend class SptFactory;

private:

  /// @brief コンストラクタ
  ///
  /// value は常に定数式
  SptDefParam(
    const FileRegion& file_region,
    PtiNameBranchArray&& nb_array,
    const char* tail_name,
    const PtExpr* value
  );

  /// @brief デストラクタ
  ~SptDefParam();


public:
  //////////////////////////////////////////////////////////////////////
  // PtDefParam の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  FileRegion
  file_region() const override;

  /// @brief 階層ブランチの要素数の取得
  SizeType
  namebranch_num() const override;

  /// @brief 階層ブランチの取得
  const PtNameBranch*
  namebranch(
    SizeType pos ///< [in] 位置 ( 0 <= pos < namebranch_num() )
  ) const override;

  /// @brief 末尾の名前を返す．
  const char*
  name() const override;

  /// @brief 値を返す．
  const PtExpr*
  expr() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 階層ブランチの配列
  PtiNameBranchArray&& mNbArray;

  // 末尾の名前
  const char* mName;

  // 値
  const PtExpr* mExpr;

};


//////////////////////////////////////////////////////////////////////
/// @brief continuous assign 文のヘッダ
//////////////////////////////////////////////////////////////////////
class SptContAssignH :
  public SptItem
{
  friend class SptFactory;

private:

  /// @brief コンストラクタ
  SptContAssignH(
    const FileRegion& file_region,
    const PtStrength* strength,
    const PtDelay* delay,
    PtiContAssignArray&& ca_array
  );

  /// @brief デストラクタ
  ~SptContAssignH();


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief strength を返す．
  const PtStrength*
  strength() const override;

  /// @brief delay を返す．
  const PtDelay*
  delay() const override;

  /// @brief continuous assign の要素数の取得
  SizeType
  contassign_num() const override;

  /// @brief continuous assign の取得
  const PtContAssign*
  contassign(
    SizeType pos ///< [in] 位置 ( 0 <= pos < contassign_num() )
  ) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // strength
  const PtStrength* mStrength;

  // delay
  const PtDelay* mDelay;

  // 要素の配列
  PtiContAssignArray mArray;

};


//////////////////////////////////////////////////////////////////////
// continuous assign 文
//////////////////////////////////////////////////////////////////////
class SptContAssign :
  public PtContAssign
{
  friend class SptFactory;

private:

  /// @brief コンストラクタ
  SptContAssign(
    const FileRegion& fr,
    const PtExpr* lhs,
    const PtExpr* rhs
  );

  /// @brief デストラクタ
  ~SptContAssign();


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

  /// @brief コンストラクタ
  SptProcess(
    const FileRegion& file_region,
    PtItemType type,
    const PtStmt* body
  );

  /// @brief デストラクタ
  ~SptProcess();


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 本体のステートメントを返す．
  const PtStmt*
  body() const override;


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
  SptTf(
    const FileRegion& file_region,     ///< [in] ファイル位置の情報
    PtItemType type,                   ///< [in] 型
    const char* name,                  ///< [in] 名前
    bool automatic,                    ///< [in] automatic 属性
    bool sign,                         ///< [in] 符号の有無
    const PtExpr* left,                ///< [in] 範囲の左側の式
    const PtExpr* right,               ///< [in] 範囲の右側の式
    VpiVarType var_type,               ///< [in] 関数の戻値の型
    PtiIOHeadArray&& iohead_array,     ///< [in] IO宣言のリスト
    PtiDeclHeadArray&& declhead_array, ///< [in] 宣言のリスト
    const PtStmt* stmt                 ///< [in] 本体のステートメント
  );

  /// @brief デストラクタ
  ~SptTf();


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を取り出す．
  const char*
  name() const override;

  /// @brief automatic なら true
  bool
  automatic() const override;

  /// @brief IO宣言の要素数の取得
  SizeType
  ioitem_num() const override;

  /// @brief IO宣言ヘッダリストの要素数の取得
  SizeType
  iohead_num() const override;

  /// @brief IO宣言ヘッダの取得
  const PtIOHead*
  iohead(
    SizeType pos ///< [in] 位置 ( 0 <= pos < iohead_num() )
  ) const override;

  /// @brief 宣言ヘッダの要素数の取得
  SizeType
  declhead_num() const override;

  /// @brief 宣言ヘッダの取得
  const PtDeclHead*
  declhead(
    SizeType pos ///< [in] 位置 ( 0 <= pos < declhead_num() )
  ) const override;

  /// @brief 本体を取り出す．
  const PtStmt*
  body() const override;

  /// @brief 符号の有無の取得
  bool
  is_signed() const override;

  /// @brief 範囲の MSB を得る． 指定なしなら nullptr
  const PtExpr*
  left_range() const override;

  /// @brief 範囲の LSB を得る． 指定なしなら nullptr
  const PtExpr*
  right_range() const override;

  /// @brief データ型の取得
  VpiVarType
  data_type() const override;

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
  VpiVarType mDataType;

  // IO宣言の要素数
  int mIOItemNum;

  // IO宣言の配列
  PtiIOHeadArray mIOHeadArray;

  // その他の宣言の配列
  PtiDeclHeadArray mDeclHeadArray;

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
  SptGateH(
    const FileRegion& file_region,
    VpiPrimType prim_type,
    const PtStrength* strength,
    const PtDelay* delay,
    PtiInstArray&& elem_array
  );

  // デストラクタ
  ~SptGateH();


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief プリミティブタイプ(vpiAndPrim など)を返す．
  VpiPrimType
  prim_type() const override;

  /// @brief strength を返す．
  const PtStrength*
  strength() const override;

  /// @brief delay を返す．
  const PtDelay*
  delay() const override;

  /// @brief module/UDP/gate instance の要素数の取得
  SizeType
  inst_num() const override;

  /// @brief module/UDP/gate instance の取得
  const PtInst*
  inst(
    SizeType pos ///< [in] 位置 ( 0 <= pos < inst_num() )
  ) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // プリミティブタイプ
  VpiPrimType mPrimType;

  // strength
  const PtStrength* mStrength;

  // delay
  const PtDelay* mDelay;

  // 要素の配列
  PtiInstArray mElemArray;

};


//////////////////////////////////////////////////////////////////////
// module/udp header のベース実装クラス
//////////////////////////////////////////////////////////////////////
class SptMuH :
  public SptItem
{
  friend class SptFactory;

private:

  /// @brief コンストラクタ
  SptMuH(
    const FileRegion& file_region,
    const char* def_name,
    PtiConnectionArray&& con_array,
    const PtStrength* strength,
    const PtDelay* delay,
    PtiInstArray&& elem_array
  );

  /// @brief デストラクタ
  ~SptMuH();


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 定義名を返す．
  const char*
  name() const override;

  /// @brief strength を返す．
  const PtStrength*
  strength() const override;

  /// @brief delay を返す．
  const PtDelay*
  delay() const override;

  /// @brief パラメータ割り当て数の取得
  SizeType
  paramassign_num() const override;

  /// @brief パラメータ割り当ての取得
  const PtConnection*
  paramassign(
    SizeType pos ///< [in] 位置 ( 0 <= pos < paramassign_num() )
  ) const override;

  /// @brief module/UDP/gate instance の要素数の取得
  SizeType
  inst_num() const override;

  /// @brief module/UDP/gate instance の取得
  const PtInst*
  inst(
    SizeType pos ///< [in] 位置 ( 0 <= pos < inst_num() )
  ) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 定義名
  const char* mName;

  // パラメータ割り当ての配列
  PtiConnectionArray mParamArray;

  // strength
  const PtStrength* mStrength;

  // delay
  const PtDelay* mDelay;

  // 要素のリスト
  PtiInstArray mElemArray;

};


//////////////////////////////////////////////////////////////////////
// module instance/UDP instance のベース実装クラス
//////////////////////////////////////////////////////////////////////
class SptInst :
  public PtInst
{
  friend class SptFactory;

private:

  /// @brief コンストラクタ
  SptInst(
    const FileRegion& file_region,
    const char* name,
    const PtExpr* left,
    const PtExpr* right,
    PtiConnectionArray&& con_array
  );

  /// @brief デストラクタ
  ~SptInst();


public:
  //////////////////////////////////////////////////////////////////////
  // PtInst の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  FileRegion
  file_region() const override;

  /// @brief 名前を返す．
  const char*
  name() const override;

  /// @brief range の MSB を取出す．
  const PtExpr*
  left_range() const override;

  /// @brief range の LSB を取出す．
  const PtExpr*
  right_range() const override;

  /// @brief ポートの要素数の取得
  SizeType
  port_num() const override;

  /// @brief ポートの取得
  const PtConnection*
  port(
    SizeType pos ///< [in] 位置 ( 0 <= pos < port_num() )
  ) const override;


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
  PtiConnectionArray mPortArray;

};


//////////////////////////////////////////////////////////////////////
// generate 文で生成される要素を持つ本体
//////////////////////////////////////////////////////////////////////
class SptGenBody
{
public:

  /// @brief コンストラクタ
  SptGenBody(
    PtiDeclHeadArray&& decl_array,
    PtiItemArray&& item_array
  );

  /// @brief デストラクタ
  ~SptGenBody();


public:

  /// @brief 宣言ヘッダの要素数の取得
  SizeType
  declhead_num() const
  {
    return mDeclArray.size();
  }

  /// @brief 宣言ヘッダの取得
  const PtDeclHead*
  declhead(
    SizeType pos ///< [in] 位置 ( 0 <= pos < declhead_num() )
  ) const
  {
    return mDeclArray[pos];
  }

  /// @brief item リストの要素数の取得
  SizeType
  item_num() const
  {
    return mItemArray.size();
  }

  /// @brief item の取得
  const PtItem*
  item(
    SizeType pos ///< [in] 位置 ( 0 <= pos < item_num() )
  ) const
  {
    return mItemArray[pos];
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 宣言の配列
  PtiDeclHeadArray mDeclArray;

  // 要素の配列
  PtiItemArray mItemArray;

};


//////////////////////////////////////////////////////////////////////
// generate ブロックの基底クラス
//////////////////////////////////////////////////////////////////////
class SptGenerate :
  public SptItem
{
  friend class SptFactory;

private:

  /// @brief コンストラクタ
  SptGenerate(
    const FileRegion& file_region,
    PtItemType type,
    const char* name,
    PtiDeclHeadArray&& decl_array,
    PtiItemArray&& item_array
  );

  /// @brief デストラクタ
  ~SptGenerate();


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を返す．
  const char*
  name() const override;

  /// @brief 宣言ヘッダの要素数の取得
  SizeType
  declhead_num() const override;

  /// @brief 宣言ヘッダの取得
  const PtDeclHead*
  declhead(
    SizeType pos ///< [in] 位置 ( 0 <= pos < declhead_num() )
  ) const override;

  /// @brief item リストの要素数の取得
  SizeType
  item_num() const override;

  /// @brief item の取得
  const PtItem*
  item(
    SizeType pos ///< [in] 位置 ( 0 <= pos < item_num() )
  ) const override;


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

  /// @brief コンストラクタ
  SptGenIf(
    const FileRegion& file_region,
    const PtExpr* cond,
    PtiDeclHeadArray&& then_decl_array,
    PtiItemArray&& then_item_array,
    PtiDeclHeadArray&& else_decl_array,
    PtiItemArray&& else_item_array
  );

  /// @brief デストラクタ
  ~SptGenIf();


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 条件式を返す．
  const PtExpr*
  expr() const override;

  /// @brief 条件が成り立ったときに生成される宣言ヘッダの要素数の取得
  SizeType
  then_declhead_num() const override;

  /// @brief 条件が成り立ったときに生成される宣言ヘッダの取得
  const PtDeclHead*
  then_declhead(
    SizeType pos ///< [in] 位置 ( 0 <= pos < declhead_num() )
  ) const override;

  /// @brief 条件が成り立ったときに生成される item リストの要素数の取得
  SizeType
  then_item_num() const override;

  /// @brief 条件が成り立ったときに生成される item の取得
  const PtItem*
  then_item(
    SizeType pos ///< [in] 位置 ( 0 <= pos < item_num() )
  ) const override;

  /// @brief 条件が成り立たなかったときに生成される宣言ヘッダの要素数の取得
  SizeType
  else_declhead_num() const override;

  /// @brief 条件が成り立たなかったときに生成される宣言ヘッダの取得
  const PtDeclHead*
  else_declhead(
    SizeType pos ///< [in] 位置 ( 0 <= pos < declhead_num() )
  ) const override;

  /// @brief 条件が成り立たなかったときに生成される item リストの要素数の取得
  SizeType
  else_item_num() const override;

  /// @brief 条件が成り立たなかったときに生成される item の取得
  const PtItem*
  else_item(
    SizeType pos ///< [in] 位置 ( 0 <= pos < item_num() )
  ) const override;


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

  /// @brief コンストラクタ
  SptGenCase(
    const FileRegion& file_region,
    const PtExpr* expr,
    PtiGenCaseItemArray&& item_array
  );

  /// @brief デストラクタ
  ~SptGenCase();


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 比較式を返す．
  const PtExpr*
  expr() const override;

  /// @brief case item のリストの要素数の取得
  SizeType
  caseitem_num() const override;

  /// @brief case item の取得
  const PtGenCaseItem*
  caseitem(
    SizeType pos ///< [in] 位置 ( 0 <= pos < caseitem_num() )
  ) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 比較式
  const PtExpr* mExpr;

  // case item の配列
  PtiGenCaseItemArray mCaseItemArray;

};


//////////////////////////////////////////////////////////////////////
// gencaseitemを表すクラス
//////////////////////////////////////////////////////////////////////
class SptGenCaseItem :
  public PtGenCaseItem
{
  friend class SptFactory;

private:

  /// @brief コンストラクタ
  SptGenCaseItem(
    const FileRegion& file_region,
    PtiExprArray&& label_array,
    PtiDeclHeadArray&& decl_array,
    PtiItemArray&& item_array
  );

  /// @brief デストラクタ
  ~SptGenCaseItem();


public:
  //////////////////////////////////////////////////////////////////////
  // PtGenCaseItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  FileRegion
  file_region() const override;

  /// @brief ラベルの要素数の取得
  SizeType
  label_num() const override;

  /// @brief ラベルの取得
  const PtExpr*
  label(
    SizeType pos ///< [in] 位置 ( 0 <= pos < label_num() )
  ) const override;

  /// @brief 宣言ヘッダの要素数の取得
  SizeType
  declhead_num() const override;

  /// @brief 宣言ヘッダの取得
  const PtDeclHead*
  declhead(
    SizeType pos ///< [in] 位置 ( 0 <= pos < declhead_num() )
  ) const override;

  /// @brief item リストの要素数の取得
  SizeType
  item_num() const override;

  /// @brief item の取得
  const PtItem*
  item(
    SizeType pos ///< [in] 位置 ( 0 <= pos < item_num() )
  ) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // ラベルの配列
  PtiExprArray mLabelArray;

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

  /// @brief コンストラクタ
  SptGenFor(
    const FileRegion& file_region,
    const char* loop_var,
    const PtExpr* init_expr,
    const PtExpr* cond,
    const PtExpr* next_expr,
    const char* block_name,
    PtiDeclHeadArray&& decl_array,
    PtiItemArray&& item_array
  );

  /// @brief デストラクタ
  ~SptGenFor();


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を返す．
  const char*
  name() const override;

  /// @brief 宣言ヘッダの要素数の取得
  SizeType
  declhead_num() const override;

  /// @brief 宣言ヘッダの取得
  const PtDeclHead*
  declhead(
    SizeType pos ///< [in] 位置 ( 0 <= pos < declhead_num() )
  ) const override;

  /// @brief item リストの要素数の取得
  SizeType
  item_num() const override;

  /// @brief item の取得
  const PtItem*
  item(
    SizeType pos ///< [in] 位置 ( 0 <= pos < item_num() )
  ) const override;

  /// @brief 繰り返し制御用の変数名を返す．
  const char*
  loop_var() const override;

  /// @brief 初期化文の右辺を返す．
  const PtExpr*
  init_expr() const override;

  /// @brief 繰り返し条件を返す．
  const PtExpr*
  expr() const override;

  /// @brief 増加文の右辺を返す．
  const PtExpr*
  next_expr() const override;


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

  /// @brief コンストラクタ
  SptSpecItem(
    const FileRegion& file_region,
    VpiSpecItemType id,
    PtiExprArray&& terminal_array
  );

  /// @brief デストラクタ
  ~SptSpecItem();


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief specify block item の種類を返す．
  VpiSpecItemType
  specitem_type() const override;

  /// @brief ターミナルの要素数の取得
  SizeType
  terminal_num() const override;

  /// @brief ターミナルの取得
  const PtExpr*
  terminal(
    SizeType pos ///< [in] 位置 ( 0 <= pos < terminal_num() )
  ) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // トークン番号
  VpiSpecItemType mId;

  // ターミナルリスト
  PtiExprArray mTerminals;

};


//////////////////////////////////////////////////////////////////////
// path 仕様 を表すクラス
//////////////////////////////////////////////////////////////////////
class SptSpecPath :
  public SptItem
{
  friend class SptFactory;

private:

  /// @brief コンストラクタ
  SptSpecPath(
    const FileRegion& file_region,
    VpiSpecPathType id,
    const PtExpr* expr,
    const PtPathDecl* path_decl
  );

  /// @brief デストラクタ
  ~SptSpecPath();


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief specify block path の種類を返す．
  VpiSpecPathType
  specpath_type() const override;

  /// @brief モジュールパスの式を返す．
  const PtExpr*
  expr() const override;

  /// @brief パス記述を返す．
  const PtPathDecl*
  path_decl() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // トークン番号
  VpiSpecPathType mId;

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

  /// @brief コンストラクタ
  SptPathDecl(
    const FileRegion& file_region,
    int edge,
    PtiExprArray&& input_array,
    int input_pol,
    VpiPathType op,
    PtiExprArray&& output_array,
    int output_pol,
    const PtExpr* expr,
    const PtPathDelay* path_delay
  );

  /// @brief デストラクタ
  ~SptPathDecl();


public:
  //////////////////////////////////////////////////////////////////////
  // PtPathDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  FileRegion
  file_region() const override;

  /// @brief edge_descriptor を取り出す．
  ///
  /// 0の場合もある．
  int
  edge() const override;

  /// @brief 入力のリストの要素数の取得
  SizeType
  input_num() const override;

  /// @brief 入力の取得
  const PtExpr*
  input(
    SizeType pos ///< [in] 位置 ( 0 <= pos < input_num() )
  ) const override;

  /// @brief 入力の極性を取り出す．
  ///
  /// 0の場合もありうる．
  int
  input_pol() const override;

  /// @brief パス記述子(?)を得る．vpiParallel か vpiFull
  VpiPathType
  op() const override;

  /// @brief 出力のリストの要素数の取得
  SizeType
  output_num() const override;

  /// @brief 出力の取得
  const PtExpr*
  output(
    SizeType pos ///< [in] 位置 ( 0 <= pos < output_num() )
  ) const override;

  /// @brief 出力の極性を取り出す．
  ///
  /// 0の場合もありうる．
  int
  output_pol() const override;

  /// @brief 式を取り出す．
  ///
  /// nullptr の場合もありうる．
  const PtExpr*
  expr() const override;

  /// @brief path_delay_value を取り出す．
  const PtPathDelay*
  path_delay() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  int mEdge;
  PtiExprArray&& mInputs;
  int mInputPol;
  VpiPathType mOp;
  PtiExprArray&& mOutputs;
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

  /// @brief コンストラクタ
  SptPathDelay(
    const FileRegion& file_region,
    const PtExpr* value1
  );

  /// @brief コンストラクタ
  SptPathDelay(
    const FileRegion& file_region,
    const PtExpr* value1,
    const PtExpr* value2
  );

  /// @brief コンストラクタ
  SptPathDelay(
    const FileRegion& file_region,
    const PtExpr* value1,
    const PtExpr* value2,
    const PtExpr* value3
  );

  /// @brief コンストラクタ
  SptPathDelay(
    const FileRegion& file_region,
    const PtExpr* value1,
    const PtExpr* value2,
    const PtExpr* value3,
    const PtExpr* value4,
    const PtExpr* value5,
    const PtExpr* value6
  );

  /// @brief コンストラクタ
  SptPathDelay(
    const FileRegion& file_region,
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
    const PtExpr* value12
  );

  /// @brief デストラクタ
  ~SptPathDelay();


public:
  //////////////////////////////////////////////////////////////////////
  // PtPathDelay の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  FileRegion
  file_region() const override;

  /// @brief 値を取り出す．
  ///
  /// 0の場合もある．
  const PtExpr*
  value(SizeType pos) const override;


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
