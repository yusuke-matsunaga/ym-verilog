#ifndef CPTIO_H
#define CPTIO_H

/// @file CptIO.h
/// @brief CptIO のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.

#include "ym/pt/PtDecl.h"
#include "ym/FileRegion.h"
#include "parser/PtiArray.h"
#include "parser/PtiDecl.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @brief IO宣言のヘッダのベース実装クラス
//////////////////////////////////////////////////////////////////////
class CptIOHBase :
  public PtiIOHead
{
protected:

  /// @brief コンストラクタ
  CptIOHBase(
    const FileRegion& file_region, ///< [in] ファイル位置の情報
    VpiDir dir,                    ///< [in] IOの種類
    VpiAuxType aux_type,           ///< [in] 補助的な型
    VpiNetType net_type,           ///< [in] 補助的なネット型
    VpiVarType var_type,           ///< [in] 補助的な変数型
    bool sign                      ///< [in] 符号つきの時 true にするフラグ
  );

  /// @brief デストラクタ
  ~CptIOHBase();


public:
  //////////////////////////////////////////////////////////////////////
  // PtIOH の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置の取得
  FileRegion
  file_region() const override;

  /// @brief 型の取得
  VpiDir
  direction() const override;

  /// @brief 補助的な型の取得
  VpiAuxType
  aux_type() const override;

  /// @brief 補助的なネット型の取得
  VpiNetType
  net_type() const override;

  /// @brief 補助的な変数型の取得
  VpiVarType
  var_type() const override;

  /// @brief 符号の取得
  /// @retval true 符号付き
  /// @retval false 符号なし
  bool
  is_signed() const override;

  /// @brief 範囲のMSBの取得
  /// @retval 範囲のMSB
  /// @retval nullptr 範囲を持たないとき
  const PtExpr*
  left_range() const override;

  /// @brief 範囲のLSBの取得
  /// @retval 範囲のLSB
  /// @retval nullptr 範囲を持たないとき
  const PtExpr*
  right_range() const override;

  /// @brief 要素数の取得
  SizeType
  item_num() const override;

  /// @brief 要素の取得
  const PtIOItem*
  item(
    SizeType pos ///< [in] 位置 ( 0 <= pos < item_num() )
  ) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // PtiIOHead の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素リストの設定
  void
  set_elem(
    PtiIOItemArray&& elem_array ///< [in] 要素の配列
  ) override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // type と aux_type と符号を格納するメンバ
  std::uint32_t mAttr;

  // 要素の配列
  PtiIOItemArray mItemArray;

};


//////////////////////////////////////////////////////////////////////
/// @brief 範囲を持たない IO宣言のヘッダ
//////////////////////////////////////////////////////////////////////
class CptIOH :
  public CptIOHBase
{
public:

  /// @brief コンストラクタ
  CptIOH(
    const FileRegion& file_region, ///< [in] ファイル位置の情報
    VpiDir dir,			   ///< [in] IOの種類
    VpiAuxType aux_type,	   ///< [in] 補助的な型
    VpiNetType net_type,	   ///< [in] 補助的なネット型
    VpiVarType var_type,	   ///< [in] 補助的な変数型
    bool sign			   ///< [in] 符号つきの時 true にするフラグ
  );

  /// @brief デストラクタ
  ~CptIOH();

};


//////////////////////////////////////////////////////////////////////
/// @brief ビットベクタ型の IO宣言のヘッダ
//////////////////////////////////////////////////////////////////////
class CptIOHV :
  public CptIOHBase
{
public:

  /// @brief コンストラクタ
  CptIOHV(
    const FileRegion& file_region, ///< [in] ファイル位置の情報
    VpiDir dir,			   ///< [in] IOの種類
    VpiAuxType aux_type,	   ///< [in] 補助的な型
    VpiNetType net_type,	   ///< [in] 補助的なネット型
    bool sign,                     ///< [in] 符号つきの時 true にするフラグ
    const PtExpr* left,            ///< [in] 範囲の左側の式
    const PtExpr* right            ///< [in] 範囲の右側の式
  );

  /// @brief デストラクタ
  ~CptIOHV();


public:
  //////////////////////////////////////////////////////////////////////
  // PtIOH の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 範囲のMSBの取得
  const PtExpr*
  left_range() const override;

  /// @brief 範囲のLSBの取得
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
/// @brief IO宣言要素のベース実装クラス
//////////////////////////////////////////////////////////////////////
class CptIOItem :
  public PtIOItem
{
public:

  /// @brief コンストラクタ
  CptIOItem(
    const FileRegion& file_region, ///< [in] ファイル位置の情報
    const char* name               ///< [in] 名前
  );

  /// @brief デストラクタ
  ~CptIOItem();


public:
  //////////////////////////////////////////////////////////////////////
  // PtIOItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置の取得
  FileRegion
  file_region() const override;

  /// @brief 名前の取得
  const char*
  name() const override;

  /// @brief 初期値の取得
  /// @retval 初期値
  /// @retval nullptr 初期値を持たないとき
  const PtExpr*
  init_value() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // トークンの位置
  FileRegion mLoc;

  // 名前
  const char* mName;

};


//////////////////////////////////////////////////////////////////////
/// @brief 初期値をもった IO宣言要素の基底クラス
//////////////////////////////////////////////////////////////////////
class CptIOItemI :
  public CptIOItem
{
public:

  /// @brief コンストラクタ
  CptIOItemI(
    const FileRegion& file_region, ///< [in] ファイル位置の情報
    const char* name,              ///< [in] 名前
    const PtExpr* init_value       ///< [in] 初期値
  );

  /// @brief デストラクタ
  ~CptIOItemI();


public:
  //////////////////////////////////////////////////////////////////////
  // PtIOItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置の取得
  FileRegion
  file_region() const override;

  /// @brief 初期値の取得
  const PtExpr*
  init_value() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 初期値
  const PtExpr* mInitValue;

};

END_NAMESPACE_YM_VERILOG

#endif // CPTIO_H
