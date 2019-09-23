#ifndef EICONSTANT_H
#define EICONSTANT_H

/// @file EiConstant.h
/// @brief EiExpr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "EiExpr.h"

#include "ym/BitVector.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiConstant EiConstant.h "EiConstant.h"
/// @brief 定数を表すクラス
//////////////////////////////////////////////////////////////////////
class EiConstant :
  public EiExprBase
{
protected:

  /// @brief コンストラクタ
  /// @param[in] pt_expr パース木の定義要素
  EiConstant(const PtExpr* pt_expr);

  /// @brief デストラクタ
  ~EiConstant();


public:
  //////////////////////////////////////////////////////////////////////
  // ElbObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  VpiObjType
  type() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 定数の時 true を返す．
  /// @note このクラスは常に true を返す．
  bool
  is_const() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // EiExpr の設定用の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要求される式の型を計算してセットする．
  /// @param[in] type 要求される式の型
  /// @note 必要であればオペランドに対して再帰的に処理を行なう．
  void
  _set_reqsize(const VlValueType& type) override;

  /// @brief オペランドを返す．
  /// @param[in] pos 位置番号
  /// @note 演算子の時，意味を持つ．
  /// @note このクラスでは nullptr を返す．
  ElbExpr*
  _operand(SizeType pos) const override;

};


//////////////////////////////////////////////////////////////////////
/// @class EiIntConst EiConstant.h "EiConstant.h"
/// @brief 整数型の定数を表すクラス
//////////////////////////////////////////////////////////////////////
class EiIntConst :
  public EiConstant
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] value 値
  EiIntConst(const PtExpr* pt_expr,
	     int value);

  /// @brief デストラクタ
  ~EiIntConst();


public:
  //////////////////////////////////////////////////////////////////////
  // ElbExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 式のタイプを返す．
  VlValueType
  value_type() const override;

  /// @brief 定数の型を返す．
  /// @note 定数の時，意味を持つ．
  VpiConstType
  constant_type() const override;

  /// @brief 定数値を返す．
  /// @note kVpiConstant の時，意味を持つ．
  /// @note それ以外では動作は不定
  VlValue
  constant_value() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値
  int mValue;

};


//////////////////////////////////////////////////////////////////////
/// @class EiBitVectorConst EiConstant.h "EiConstant.h"
/// @brief ビットベクタ型の定数を表すクラス
//////////////////////////////////////////////////////////////////////
class EiBitVectorConst :
  public EiConstant
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] const_type 定数型
  /// @param[in] value 値
  EiBitVectorConst(const PtExpr* pt_expr,
		   VpiConstType const_type,
		   const BitVector& value);

  /// @brief デストラクタ
  ~EiBitVectorConst();


public:
  //////////////////////////////////////////////////////////////////////
  // ElbExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 式のタイプを返す．
  VlValueType
  value_type() const override;

  /// @brief 定数の型を返す．
  /// @note 定数の時，意味を持つ．
  VpiConstType
  constant_type() const override;

  /// @brief 定数値を返す．
  /// @note kVpiConstant の時，意味を持つ．
  /// @note それ以外では動作は不定
  VlValue
  constant_value() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 定数の型
  VpiConstType mConstType;

  // 値
  BitVector mValue;

};


//////////////////////////////////////////////////////////////////////
/// @class EiRealConst EiConstant.h "EiConstant.h"
/// @brief 実数型の定数を表すクラス
//////////////////////////////////////////////////////////////////////
class EiRealConst :
  public EiConstant
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] value 値
  EiRealConst(const PtExpr* pt_expr,
	      double value);

  /// @brief デストラクタ
  ~EiRealConst();


public:
  //////////////////////////////////////////////////////////////////////
  // ElbExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 式のタイプを返す．
  VlValueType
  value_type() const override;

  /// @brief 定数の型を返す．
  /// @note 定数の時，意味を持つ．
  VpiConstType
  constant_type() const override;

  /// @brief 定数値を返す．
  /// @note kVpiConstant の時，意味を持つ．
  /// @note それ以外では動作は不定
  VlValue
  constant_value() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値
  double mValue;

};


//////////////////////////////////////////////////////////////////////
/// @class EiStringConst EiConstant.h "EiConstant.h"
/// @brief 文字列型の定数を表すクラス
//////////////////////////////////////////////////////////////////////
class EiStringConst :
  public EiConstant
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] value 値
  EiStringConst(const PtExpr* pt_expr,
		const string& value);

  /// @brief デストラクタ
  ~EiStringConst();


public:
  //////////////////////////////////////////////////////////////////////
  // ElbExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 式のタイプを返す．
  VlValueType
  value_type() const override;

  /// @brief 定数の型を返す．
  /// @note 定数の時，意味を持つ．
  VpiConstType
  constant_type() const override;

  /// @brief 定数値を返す．
  /// @note kVpiConstant の時，意味を持つ．
  /// @note それ以外では動作は不定
  VlValue
  constant_value() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値
  BitVector mValue;

};

END_NAMESPACE_YM_VERILOG

#endif // EICONSTANT_H
