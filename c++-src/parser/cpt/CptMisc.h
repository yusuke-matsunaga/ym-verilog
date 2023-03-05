#ifndef CPTMISC_H
#define CPTMISC_H

/// @file CptMisc.h
/// @brief CptMisc のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.

#include "ym/pt/PtMisc.h"
#include "ym/FileRegion.h"
#include "parser/PtiArray.h"
#include "parser/PtiFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @brief delay control/event control/repeat control を表すベースクラス
//////////////////////////////////////////////////////////////////////
class CptControl :
  public PtControl
{
protected:

  /// @brief コンストラクタ
  CptControl();

  /// @brief デストラクタ
  ~CptControl();


public:
  //////////////////////////////////////////////////////////////////////
  // PtControl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 遅延式の取得
  /// @retval 遅延を表す式 delay control の場合
  /// @retval nullptr 上記以外
  const PtExpr*
  delay() const override;

  /// @brief イベントリストの要素数の取得
  ///
  /// event control/repeat control の場合のみ意味を持つ
  SizeType
  event_num() const override;

  /// @brief イベントリストの要素の取得
  ///
  /// event control/repeat control の場合のみ意味を持つ
  const PtExpr*
  event(
    SizeType pos ///< [in] 位置 ( 0 <= pos < event_num() )
  ) const override;

  /// @brief 繰り返し数の取得
  /// @retval 繰り返し数を表す式 repeat control の場合
  /// @retval nullptr 上記以外
  const PtExpr*
  rep_expr() const override;

};


//////////////////////////////////////////////////////////////////////
/// @brief delay control を表すクラス
//////////////////////////////////////////////////////////////////////
class CptDelayControl :
  public CptControl
{
public:

  /// @brief コンストラクタ
  CptDelayControl(
    const FileRegion& file_region,
    const PtExpr* value
  );

  /// @brief デストラクタ
  ~CptDelayControl();


public:
  //////////////////////////////////////////////////////////////////////
  // PtControl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  FileRegion
  file_region() const override;

  /// @brief 型を返す．
  PtCtrlType
  type() const override;

  /// @brief 遅延式を返す．
  const PtExpr*
  delay() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 先頭の位置
  FileLoc mTopLoc;

  // 遅延を表す式
  const PtExpr* mDelay;

};


//////////////////////////////////////////////////////////////////////
/// @brief event control を表すクラス
//////////////////////////////////////////////////////////////////////
class CptEventControl :
  public CptControl
{
public:

  /// @brief コンストラクタ
  CptEventControl(
    const FileRegion& file_region,
    PtiExprArray&& event_array
  );

  /// @brief デストラクタ
  ~CptEventControl();


public:
  //////////////////////////////////////////////////////////////////////
  // PtControl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  FileRegion
  file_region() const override;

  /// @brief 型を返す．
  PtCtrlType
  type() const override;

  /// @brief イベントリストの要素数の取得
  ///
  /// event control/repeat control の場合のみ意味を持つ
  SizeType
  event_num() const override;

  /// @brief イベントリストの要素の取得
  ///
  /// event control/repeat control の場合のみ意味を持つ
  const PtExpr*
  event(
    SizeType pos ///< [in] 位置 ( 0 <= pos < event_num() )
  ) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // イベントの配列
  PtiExprArray mEventArray;

};


//////////////////////////////////////////////////////////////////////
/// @brief repeat 形式の event を表すクラス
//////////////////////////////////////////////////////////////////////
class CptRepeatControl :
  public CptEventControl
{
public:

  /// @brief コンストラクタ
  CptRepeatControl(
    const FileRegion& file_region,
    const PtExpr* expr,
    PtiExprArray&& event_array
  );

  /// @brief デストラクタ
  ~CptRepeatControl();


public:
  //////////////////////////////////////////////////////////////////////
  // PtControl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  PtCtrlType
  type() const override;

  /// @brief 繰り返し数を得る．
  const PtExpr*
  rep_expr() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 繰り返し数を表す式
  const PtExpr* mRepExpr;

};


//////////////////////////////////////////////////////////////////////
/// @brief ordered_connection/named_connection を表すクラス
//////////////////////////////////////////////////////////////////////
class CptConnection :
  public PtConnection
{
protected:

  /// @brief コンストラクタ
  CptConnection(
    const FileRegion& file_region,
    const PtExpr* expr
  );

  /// @brief デストラクタ
  ~CptConnection();


public:
  //////////////////////////////////////////////////////////////////////
  // PtConnection に固有の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を取出す．
  FileRegion
  file_region() const override;

  /// @brief 名前の取得
  /// @retval 名前 named connection の場合
  /// @retval "" ordered connection の場合
  const char*
  name() const override;

  /// @brief 式を取り出す．
  const PtExpr*
  expr() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 接続を表す式
  const PtExpr* mExpr;

};


//////////////////////////////////////////////////////////////////////
/// @brief orered_connection を表すクラス
//////////////////////////////////////////////////////////////////////
class CptOrderedCon :
  public CptConnection
{
public:

  /// @brief コンストラクタ
  CptOrderedCon(
    const FileRegion& file_region,
    const PtExpr* expr
  );

  /// @brief デストラクタ
  ~CptOrderedCon();

};


//////////////////////////////////////////////////////////////////////
/// @brief named_connection を表すクラス
//////////////////////////////////////////////////////////////////////
class CptNamedCon :
  public CptConnection
{
public:

  /// @brief コンストラクタ
  CptNamedCon(
    const FileRegion& file_region,
    const char* name,
    const PtExpr* expr
  );

  /// @brief デストラクタ
  ~CptNamedCon();


public:
  //////////////////////////////////////////////////////////////////////
  // PtConnection の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を取り出す．
  const char*
  name() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // パラメータ名/ポート名
  const char* mName;

};


//////////////////////////////////////////////////////////////////////
/// @brief strength を表すクラス
//////////////////////////////////////////////////////////////////////
class CptStrength :
  public PtStrength
{
public:

  /// @brief drive strength を表すコンストラクタ
  CptStrength(
    const FileRegion& file_region,
    VpiStrength value1,
    VpiStrength value2
  );

  /// @brief charge strength を表すコンストラクタ
  CptStrength(
    const FileRegion& file_region,
    VpiStrength value1
  );

  /// @brief デストラクタ
  ~CptStrength();


public:
  //////////////////////////////////////////////////////////////////////
  // PtStrength の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を取出す．
  FileRegion
  file_region() const override;

  /// @brief drive strength0 を返す．
  VpiStrength
  drive0() const override;

  /// @brief drive strength1 を返す．
  VpiStrength
  drive1() const override;

  /// @brief charge strength を返す．
  VpiStrength
  charge() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 値の配列
  VpiStrength mValue[3];

};


//////////////////////////////////////////////////////////////////////
/// @brief delay を表すクラス
//////////////////////////////////////////////////////////////////////
class CptDelay :
  public PtDelay
{
public:

  /// @brief 一つの値をとるコンストラクタ
  CptDelay(
    const FileRegion& file_region,
    const PtExpr* value1
  );

  /// @brief 二つの値をとるコンストラクタ
  CptDelay(
    const FileRegion& file_region,
    const PtExpr* value1,
    const PtExpr* value2
  );

  /// @brief 三つの値をとるコンストラクタ
  CptDelay(
    const FileRegion& file_region,
    const PtExpr* value1,
    const PtExpr* value2,
    const PtExpr* value3
  );

  /// @brief デストラクタ
  ~CptDelay();


public:
  //////////////////////////////////////////////////////////////////////
  // PtDelay の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を取出す．
  FileRegion
  file_region() const override;

  /// @brief 値を取り出す．
  const PtExpr*
  value(
    SizeType pos
  ) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 値のリスト
  const PtExpr* mValue[3];

};


//////////////////////////////////////////////////////////////////////
/// @brief 階層名を表すクラス
//////////////////////////////////////////////////////////////////////
class CptNameBranch :
  public PtNameBranch
{
public:

  /// @brief コンストラクタ
  CptNameBranch(
    const char* name
  );

  /// @brief デストラクタ
  ~CptNameBranch();


public:
  //////////////////////////////////////////////////////////////////////
  // PtNameBranch の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を取り出す．
  const char*
  name() const override;

  /// @brief インデックスの有無のチェック
  /// @retval true インデックスを持っている時
  /// @retval false インデックスを持っていない時
  bool
  has_index() const override;

  /// @brief インデックスの取得
  /// @return インデックスの値
  int
  index() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  const char* mName;

};


//////////////////////////////////////////////////////////////////////
/// @brief インデックスつきの階層名を表すクラス
//////////////////////////////////////////////////////////////////////
class CptNameBranchI :
  public CptNameBranch
{
public:

  /// @brief コンストラクタ
  CptNameBranchI(
    const char* name,
    int index
  );

  /// @brief デストラクタ
  ~CptNameBranchI();


public:
  //////////////////////////////////////////////////////////////////////
  // PtNameBranch の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief インデックスを持っている時 true を返す．
  bool
  has_index() const override;

  /// @brief インデックスを取り出す．
  int
  index() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // インデックス
  int mIndex;

};


//////////////////////////////////////////////////////////////////////
/// @brief attribute_instance を表すクラス
//////////////////////////////////////////////////////////////////////
class CptAttrInst :
  public PtAttrInst
{
public:

  /// @brief コンストラクタ
  CptAttrInst(
    PtiAttrSpecArray&& as_array
  );

  /// @brief デストラクタ
  ~CptAttrInst();


public:
  //////////////////////////////////////////////////////////////////////
  // PtAttrInst の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  FileRegion
  file_region() const override;

  /// @brief 要素数の取得
  SizeType
  attrspec_num() const override;

  /// @brief 要素の取得
  const PtAttrSpec*
  attrspec(
    SizeType pos ///< [in] 位置 ( 0 <= pos < attrspec_num() )
  ) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // attr spec のリスト
  PtiAttrSpecArray mAttrSpecArray;

};


//////////////////////////////////////////////////////////////////////
/// @brief attr_spec を表すクラス
//////////////////////////////////////////////////////////////////////
class CptAttrSpec :
  public PtAttrSpec
{
public:

  /// @brief コンストラクタ
  CptAttrSpec(
    const FileRegion& file_region,
    const char* name,
    const PtExpr* expr
  );

  /// @brief デストラクタ
  ~CptAttrSpec();


public:
  //////////////////////////////////////////////////////////////////////
  // PtAttrSpec の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  FileRegion
  file_region() const override;

  /// @brief 名前を取り出す．
  const char*
  name() const override;

  /// @brief 式を取り出す．nullptr の場合もある．
  const PtExpr*
  expr() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 名前
  const char* mName;

  // 式
  const PtExpr* mExpr;

};

END_NAMESPACE_YM_VERILOG

#endif // CPTMISC_H
