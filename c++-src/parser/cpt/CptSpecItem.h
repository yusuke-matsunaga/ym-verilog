﻿#ifndef CPTSPECITEM_H
#define CPTSPECITEM_H

/// @file CptSpecItem.h
/// @brief CptSpecItem のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.

#include "CptItem.h"
#include "parser/PtiArray.h"
#include "parser/PtiFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @brief specify_block item を表すクラス
//////////////////////////////////////////////////////////////////////
class CptSpecItem :
  public CptItem
{
public:

  /// @brief コンストラクタ
  CptSpecItem(
    const FileRegion& file_region,
    VpiSpecItemType id,
    PtiExprArray&& terminal_array
  );

  /// @brief デストラクタ
  ~CptSpecItem();


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  FileRegion
  file_region() const override;

  /// @brief 型を返す．
  PtItemType
  type() const override;

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

  // ファイル位置
  FileRegion mFileRegion;

  // トークン番号
  VpiSpecItemType mId;

  // ターミナルの配列
  PtiExprArray mTerminalArray;

};


//////////////////////////////////////////////////////////////////////
/// @brief path 仕様 を表すクラス
//////////////////////////////////////////////////////////////////////
class CptSpecPath :
  public CptItem
{
public:

  /// @brief コンストラクタ
  CptSpecPath(
    const FileRegion& file_region,
    VpiSpecPathType id,
    const PtExpr* expr,
    const PtPathDecl* path_decl
  );

  /// @brief デストラクタ
  ~CptSpecPath();


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  FileRegion
  file_region() const override;

  /// @brief 型を返す．
  PtItemType
  type() const override;

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

  // ファイル位置
  FileRegion mFileRegion;

  // トークン番号
  VpiSpecPathType mId;

  // モジュールパスの式
  const PtExpr* mExpr;

  // パス記述
  const PtPathDecl* mPathDecl;

};


//////////////////////////////////////////////////////////////////////
/// @brief path_delay_declaration を表すクラス
//////////////////////////////////////////////////////////////////////
class CptPathDecl :
  public PtPathDecl
{
public:

  /// @brief コンストラクタ
  CptPathDecl(
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
  ~CptPathDecl();


public:
  //////////////////////////////////////////////////////////////////////
  // PtPathDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  FileRegion
  file_region() const override;

  /// @brief edge_descriptor を取り出す．
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
  int
  output_pol() const override;

  /// @brief 式を取り出す．
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
  PtiExprArray mInputArray;
  int mInputPol;
  VpiPathType mOp;
  PtiExprArray mOutputArray;
  int mOutputPol;
  const PtExpr* mExpr;
  const PtPathDelay* mPathDelay;

};


//////////////////////////////////////////////////////////////////////
/// @brief path_delay_value を表すクラス
//////////////////////////////////////////////////////////////////////
class CptPathDelay :
  public PtPathDelay
{
public:

  /// @brief コンストラクタ
  CptPathDelay(
    const FileRegion& file_region,
    const PtExpr* value1
  );

  /// @brief コンストラクタ
  CptPathDelay(
    const FileRegion& file_region,
    const PtExpr* value1,
    const PtExpr* value2
  );

  /// @brief コンストラクタ
  CptPathDelay(
    const FileRegion& file_region,
    const PtExpr* value1,
    const PtExpr* value2,
    const PtExpr* value3
  );

  /// @brief コンストラクタ
  CptPathDelay(
    const FileRegion& file_region,
    const PtExpr* value1,
    const PtExpr* value2,
    const PtExpr* value3,
    const PtExpr* value4,
    const PtExpr* value5,
    const PtExpr* value6
  );

  /// @brief コンストラクタ
  CptPathDelay(
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
  ~CptPathDelay();


public:
  //////////////////////////////////////////////////////////////////////
  // PtPathDelay の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
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

  // ディレイ値
  const PtExpr* mValues[12];

};

END_NAMESPACE_YM_VERILOG

#endif // CPTSPECITEM_H
