#ifndef SPTMISC_H
#define SPTMISC_H

/// @file SptMisc.h
/// @brief その他の部品クラスのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym/FileRegion.h"

#include "ym/pt/PtMisc.h"
#include "ym/pt/PtArray.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// delay control/event control/repeat control を表すベースクラス
//////////////////////////////////////////////////////////////////////
class SptControl :
  public PtControl
{
  friend class SptFactory;

private:

  /// コンストラクタ
  SptControl(const FileRegion& file_region,
	     tPtCtrlType type,
	     const PtExpr* expr,
	     PtExprArray event_array);

  /// デストラクタ
  ~SptControl();


public:
  //////////////////////////////////////////////////////////////////////
  // PtControl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// ファイル位置の取得
  FileRegion
  file_region() const override;

  /// 型の取得
  tPtCtrlType
  type() const override;

  /// 遅延式の取得
  const PtExpr*
  delay() const override;

  /// @brief イベントリストの取得
  /// @note event control/repeat control の場合のみ意味を持つ
  PtExprArray
  event_list() const override;

  /// 繰り返し数の取得
  const PtExpr*
  rep_expr() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // クラスの型
  tPtCtrlType mType;

  // 遅延を表す式/繰り返し回数を表す式
  const PtExpr* mExpr;

  // イベントの配列
  PtExprArray mEventArray;

};


//////////////////////////////////////////////////////////////////////
/// ordered_connection/named_connection を表すクラス
//////////////////////////////////////////////////////////////////////
class SptConnection :
  public PtConnection
{
  friend class SptFactory;

private:

  /// コンストラクタ
  SptConnection(const FileRegion& file_region,
		const PtExpr* expr,
		const char* name = nullptr);

  /// デストラクタ
  ~SptConnection();


public:
  //////////////////////////////////////////////////////////////////////
  // PtConnection の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// ファイル位置の取得
  FileRegion
  file_region() const override;

  /// 名前の取得
  const char*
  name() const override;

  /// 式の取得
  const PtExpr*
  expr() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // パラメータ名/ポート名
  const char* mName;

  // 接続を表す式
  const PtExpr* mExpr;

};


//////////////////////////////////////////////////////////////////////
/// strength を表すクラス
//////////////////////////////////////////////////////////////////////
class SptStrength :
  public PtStrength
{
  friend class SptFactory;

private:

  /// コンストラクタ (0/1 の強度用)
  SptStrength(const FileRegion& file_region,
	      VpiStrength drive0,
	      VpiStrength drive1);

  /// コンストラクタ (電荷の強度用)
  SptStrength(const FileRegion& file_region,
	      VpiStrength charge);

  /// デストラクタ
  ~SptStrength();


public:
  //////////////////////////////////////////////////////////////////////
  // PtStrength の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// ファイル位置の取得
  FileRegion
  file_region() const override;

  /// drive strength0 の取得
  VpiStrength
  drive0() const override;

  /// drive strength1 の取得
  VpiStrength
  drive1() const override;

  /// charge strength の取得
  VpiStrength
  charge() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 値
  VpiStrength mDrive[3];
};


//////////////////////////////////////////////////////////////////////
/// delay を表すクラス
//////////////////////////////////////////////////////////////////////
class SptDelay :
  public PtDelay
{
  friend class SptFactory;

private:

  /// 一つの値をとるコンストラクタ
  SptDelay(const FileRegion& file_region,
	   const PtExpr* value1);

  /// 二つの値をとるコンストラクタ
  SptDelay(const FileRegion& file_region,
	   const PtExpr* value1,
	   const PtExpr* value2);

  /// 三つの値をとるコンストラクタ
  SptDelay(const FileRegion& file_region,
	   const PtExpr* value1,
	   const PtExpr* value2,
	   const PtExpr* value3);

  /// デストラクタ
  ~SptDelay();


public:
  //////////////////////////////////////////////////////////////////////
  // PtDelay の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// ファイル位置の取得
  FileRegion
  file_region() const override;

  /// 値の取得
  const PtExpr*
  value(SizeType pos) const override;


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
/// 階層名を表すクラス
//////////////////////////////////////////////////////////////////////
class SptNameBranch :
  public PtNameBranch
{
public:

  /// 名前のみのコンストラクタ
  SptNameBranch(const char* name);

  /// 名前とインデックスのコンストラクタ
  SptNameBranch(const char* name,
		int index);

  /// デストラクタ
  ~SptNameBranch();


public:
  //////////////////////////////////////////////////////////////////////
  // PtNameBranch の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// 名前の取得
  const char*
  name() const override;

  /// インデックスの有無のチェック
  bool
  has_index() const override;

  /// インデックスの取得
  int
  index() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  const char* mName;

  // インデックス
  // ただし使うときは1ビット左にシフトして最下位ビットに1を立てておく．
  // インデックス0とインデックスなしを区別するため．
  unsigned int mIndex;

};


//////////////////////////////////////////////////////////////////////
/// attribute_instance を表すクラス
//////////////////////////////////////////////////////////////////////
class SptAttrInst :
  public PtAttrInst
{
  friend class SptFactory;

private:

  /// コンストラクタ
  SptAttrInst(PtAttrSpecArray as_array);

  /// デストラクタ
  ~SptAttrInst();


public:
  //////////////////////////////////////////////////////////////////////
  // PtAttrInst の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素のリストの取得
  PtAttrSpecArray
  attrspec_list() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // attr spec の配列
  PtAttrSpecArray mAttrSpecArray;

};


//////////////////////////////////////////////////////////////////////
/// attr_spec を表すクラス
//////////////////////////////////////////////////////////////////////
class SptAttrSpec :
  public PtAttrSpec
{
  friend class SptFactory;

private:

  /// コンストラクタ
  SptAttrSpec(const FileRegion& file_region,
	      const char* name,
	      const PtExpr* expr);

  /// デストラクタ
  ~SptAttrSpec();


public:
  //////////////////////////////////////////////////////////////////////
  // PtAttrSpec の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// ファイル位置の取得
  FileRegion
  file_region() const override;

  /// 名前の取得
  const char*
  name() const override;

  /// 式の取得
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

#endif // SPTMISC_H
