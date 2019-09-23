#ifndef EIPRIMARY_H
#define EIPRIMARY_H

/// @file EiPrimary.h
/// @brief EiPrimary のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "EiExpr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiPrimary EiPrimary.h "EiPrimary.h"
/// @brief プライマリ式の基底クラス
//////////////////////////////////////////////////////////////////////
class EiPrimaryBase :
  public EiExprBase
{
  friend class EiFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] pt_expr パース木の定義要素
  EiPrimaryBase(const PtExpr* pt_expr);

  /// @brief デストラクタ
  ~EiPrimaryBase();


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief プライマリ(net/reg/variables/parameter)の時に true を返す．
  bool
  is_primary() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbExpr の仮想関数
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
/// @class EiPrimary EiPrimary.h "EiPrimary.h"
/// @brief プライマリ式を表すクラス
//////////////////////////////////////////////////////////////////////
class EiPrimary :
  public EiPrimaryBase
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] obj 本体のオブジェクト
  EiPrimary(const PtExpr* pt_expr,
	    ElbDecl* obj);

  /// @brief デストラクタ
  ~EiPrimary();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  VpiObjType
  type() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 式のタイプを返す．
  VlValueType
  value_type() const override;

  /// @brief 定数の時 true を返す．
  /// @note 参照している要素の型によって決まる．
  bool
  is_const() const override;

  /// @brief 宣言要素もしくは配列型宣言要素への参照を返す．
  /// @note それ以外では nullptr を返す．
  const VlDeclBase*
  decl_base() const override;

  /// @brief 宣言要素への参照の場合，対象のオブジェクトを返す．
  /// @note 宣言要素に対するビット選択，部分選択の場合にも意味を持つ．
  const VlDecl*
  decl_obj() const override;

  /// @brief 左辺式の要素数の取得
  /// @note 通常は1だが，連結演算子の場合はその子供の数となる．
  /// @note ただし，連結演算の入れ子はすべて平坦化して考える．
  /// @note このクラスでは 1 を返す．
  SizeType
  lhs_elem_num() const override;

  /// @brief 左辺式の要素の取得
  /// @param[in] pos 位置 ( 0 <= pos < lhs_elem_num() )
  /// @note 連結演算子の見かけと異なり LSB 側が0番めの要素となる．
  /// @note このクラスでは pos = 0 の時，自分自身を返す．
  const VlExpr*
  lhs_elem(SizeType pos) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象の宣言要素
  ElbDecl* mObj;

};


//////////////////////////////////////////////////////////////////////
/// @class EiDeclPrimary EiPrimary.h "EiPrimary.h"
/// @brief プライマリ式を表すクラス
//////////////////////////////////////////////////////////////////////
class EiDeclPrimary :
  public EiExpr
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] pt_item パース木の定義要素
  /// @param[in] obj 本体のオブジェクト
  EiDeclPrimary(const PtDeclItem* pt_item,
		ElbDecl* obj);

  /// @brief デストラクタ
  ~EiDeclPrimary();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  VpiObjType
  type() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 式のタイプを返す．
  VlValueType
  value_type() const override;

  /// @brief 定数の時 true を返す．
  /// @note 参照している要素の型によって決まる．
  bool
  is_const() const override;

  /// @brief プライマリ(net/reg/variables/parameter)の時に true を返す．
  bool
  is_primary() const override;

  /// @brief 宣言要素もしくは配列型宣言要素への参照を返す．
  /// @note それ以外では nullptr を返す．
  const VlDeclBase*
  decl_base() const override;

  /// @brief 宣言要素への参照の場合，対象のオブジェクトを返す．
  /// @note 宣言要素に対するビット選択，部分選択の場合にも意味を持つ．
  const VlDecl*
  decl_obj() const override;

  /// @brief Verilog-HDL の文字列を得る．
  string
  decompile() const override;

  /// @brief 左辺式の要素数の取得
  /// @note 通常は1だが，連結演算子の場合はその子供の数となる．
  /// @note ただし，連結演算の入れ子はすべて平坦化して考える．
  /// @note このクラスでは 1 を返す．
  SizeType
  lhs_elem_num() const override;

  /// @brief 左辺式の要素の取得
  /// @param[in] pos 位置 ( 0 <= pos < lhs_elem_num() )
  /// @note 連結演算子の見かけと異なり LSB 側が0番めの要素となる．
  /// @note このクラスでは pos = 0 の時に自分自身 を返す．
  const VlExpr*
  lhs_elem(SizeType pos) const override;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbExpr の仮想関数
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


private:
  //////////////////////////////////////////////////////////////////////
  // EiExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief パース木の定義要素を返す．
  const PtBase*
  pt_obj() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // パース木の定義要素
  const PtDeclItem* mPtObj;

  // 対象の宣言要素
  ElbDecl* mObj;

};


//////////////////////////////////////////////////////////////////////
/// @class EiParamPrimary EiPrimary.h "EiPrimary.h"
/// @brief パラメータ用のプライマリ式を表すクラス
//////////////////////////////////////////////////////////////////////
class EiParamPrimary :
  public EiPrimaryBase
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] obj 本体のオブジェクト
  EiParamPrimary(const PtExpr* pt_expr,
		 ElbParameter* obj);

  /// @brief デストラクタ
  ~EiParamPrimary();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  VpiObjType
  type() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 式のタイプを返す．
  VlValueType
  value_type() const override;

  /// @brief 定数の時 true を返す．
  /// @note このクラスでは true を返す．
  bool
  is_const() const override;

  /// @brief 定数値を返す．
  /// @note kVpiConstant の時，意味を持つ．
  /// @note それ以外では動作は不定
  VlValue
  constant_value() const override;

  /// @brief 宣言要素もしくは配列型宣言要素への参照を返す．
  /// @note それ以外では nullptr を返す．
  const VlDeclBase*
  decl_base() const override;

  /// @brief 宣言要素への参照の場合，対象のオブジェクトを返す．
  /// @note 宣言要素に対するビット選択，部分選択の場合にも意味を持つ．
  const VlDecl*
  decl_obj() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象の宣言要素
  ElbParameter* mObj;

};


//////////////////////////////////////////////////////////////////////
/// @class EiArrayElemPrimary EiPrimary.h "EiPrimary.h"
/// @brief 配列要素のプライマリ式を表すクラス
//////////////////////////////////////////////////////////////////////
class EiArrayElemPrimary :
  public EiPrimaryBase
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] obj 本体のオブジェクト
  /// @param[in] dim 配列の次元
  /// @param[in] index_list インデックスのリスト
  EiArrayElemPrimary(const PtExpr* pt_expr,
		     ElbDeclArray* obj,
		     int dim,
		     ElbExpr** index_list);

  /// @brief デストラクタ
  ~EiArrayElemPrimary();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  VpiObjType
  type() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 式のタイプを返す．
  VlValueType
  value_type() const override;

  /// @brief 定数の時 true を返す．
  /// @note 参照している要素の型によって決まる．
  bool
  is_const() const override;

  /// @brief 宣言要素もしくは配列型宣言要素への参照を返す．
  /// @note それ以外では nullptr を返す．
  const VlDeclBase*
  decl_base() const override;

  /// @brief 宣言要素への参照の場合，対象のオブジェクトを返す．
  /// @note 宣言要素に対するビット選択，部分選択の場合にも意味を持つ．
  const VlDeclArray*
  declarray_obj() const override;

  /// @brief 配列型宣言要素への参照の場合，配列の次元を返す．
  /// @note それ以外では 0 を返す．
  SizeType
  declarray_dimension() const override;

  /// @brief 配列型宣言要素への参照の場合，配列のインデックスを返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < declarray_dimension() )
  /// @note それ以外では nullptr を返す．
  const VlExpr*
  declarray_index(SizeType pos) const override;

  /// @brief 左辺式の要素数の取得
  /// @note 通常は1だが，連結演算子の場合はその子供の数となる．
  /// @note ただし，連結演算の入れ子はすべて平坦化して考える．
  /// @note このクラスでは 1 を返す．
  SizeType
  lhs_elem_num() const override;

  /// @brief 左辺式の要素の取得
  /// @param[in] pos 位置 ( 0 <= pos < lhs_elem_num() )
  /// @note 連結演算子の見かけと異なり LSB 側が0番めの要素となる．
  /// @note このクラスでは pos = 0 の時に自分自身 を返す．
  const VlExpr*
  lhs_elem(SizeType pos) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象の宣言要素
  ElbDeclArray* mObj;

  // 配列の次元
  int mDim;

  // インデックスのリスト
  ElbExpr** mIndexList;

};


//////////////////////////////////////////////////////////////////////
/// @class EiArrayElemPrimary EiPrimary.h "EiPrimary.h"
/// @brief 固定インデックスの配列要素のプライマリ式を表すクラス
//////////////////////////////////////////////////////////////////////
class EiConstArrayElemPrimary :
  public EiPrimaryBase
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] obj 本体のオブジェクト
  /// @param[in] offset オフセット
  EiConstArrayElemPrimary(const PtExpr* pt_expr,
			  ElbDeclArray* obj,
			  int offset);

  /// @brief デストラクタ
  ~EiConstArrayElemPrimary();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  VpiObjType
  type() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 式のタイプを返す．
  VlValueType
  value_type() const override;

  /// @brief 定数の時 true を返す．
  /// @note 参照している要素の型によって決まる．
  bool
  is_const() const override;

  /// @brief 部分/ビット指定が定数の時 true を返す．
  /// @note kVpiPartSelect/kVpiBitSelect の時，意味を持つ．
  /// @note それ以外では常に false を返す．
  bool
  is_constant_select() const override;

  /// @brief 宣言要素もしくは配列型宣言要素への参照を返す．
  /// @note それ以外では nullptr を返す．
  const VlDeclBase*
  decl_base() const override;

  /// @brief 宣言要素への参照の場合，対象のオブジェクトを返す．
  /// @note 宣言要素に対するビット選択，部分選択の場合にも意味を持つ．
  const VlDeclArray*
  declarray_obj() const override;

  /// @brief 配列型宣言要素への参照の場合，配列の次元を返す．
  /// @note それ以外では 0 を返す．
  SizeType
  declarray_dimension() const override;

  /// @brief 配列型宣言要素への参照の場合，配列のインデックスを返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < declarray_dimension() )
  /// @note それ以外では nullptr を返す．
  const VlExpr*
  declarray_index(SizeType pos) const override;

  /// @brief 配列型宣言要素への参照のオフセットを返す．
  /// @note 固定インデックスの場合のみ意味を持つ．
  int
  declarray_offset() const override;

  /// @brief 左辺式の要素数の取得
  /// @note 通常は1だが，連結演算子の場合はその子供の数となる．
  /// @note ただし，連結演算の入れ子はすべて平坦化して考える．
  /// @note このクラスでは 1 を返す．
  SizeType
  lhs_elem_num() const override;

  /// @brief 左辺式の要素の取得
  /// @param[in] pos 位置 ( 0 <= pos < lhs_elem_num() )
  /// @note 連結演算子の見かけと異なり LSB 側が0番めの要素となる．
  /// @note このクラスでは pos = 0 の時に自分自身 を返す．
  const VlExpr*
  lhs_elem(SizeType pos) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象の宣言要素
  ElbDeclArray* mObj;

  // オフセット
  int mOffset;

};


//////////////////////////////////////////////////////////////////////
/// @class EiScopePrimary EiPrimary.h "EiPrimary.h"
/// @brief VlNamedObj のプライマリ式を表すクラス
//////////////////////////////////////////////////////////////////////
class EiScopePrimary :
  public EiPrimaryBase
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] obj 本体のオブジェクト
  EiScopePrimary(const PtExpr* pt_expr,
		 const VlNamedObj* obj);

  /// @brief デストラクタ
  ~EiScopePrimary();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  VpiObjType
  type() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 式のタイプを返す．
  VlValueType
  value_type() const override;

  /// @brief 定数の時 true を返す．
  /// @note 参照している要素の型によって決まる．
  bool
  is_const() const override;

  /// @brief 対象のオブジェクトを返す．
  const VlNamedObj*
  scope_obj() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象の宣言要素
  const VlNamedObj* mObj;

};


//////////////////////////////////////////////////////////////////////
/// @class EiPrimitivePrimary EiPrimary.h "EiPrimary.h"
/// @brief VlPrimitive のプライマリ式を表すクラス
//////////////////////////////////////////////////////////////////////
class EiPrimitivePrimary :
  public EiPrimaryBase
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] obj 本体のオブジェクト
  EiPrimitivePrimary(const PtExpr* pt_expr,
		     ElbPrimitive* obj);

  /// @brief デストラクタ
  ~EiPrimitivePrimary();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  VpiObjType
  type() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 式のタイプを返す．
  VlValueType
  value_type() const override;

  /// @brief 定数の時 true を返す．
  /// @note 参照している要素の型によって決まる．
  bool
  is_const() const override;

  /// @brief 対象のオブジェクトを返す．
  const VlPrimitive*
  primitive_obj() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象の宣言要素
  ElbPrimitive* mObj;

};

END_NAMESPACE_YM_VERILOG

#endif // EIPRIMARY_H
