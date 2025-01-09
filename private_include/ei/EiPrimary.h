#ifndef EIPRIMARY_H
#define EIPRIMARY_H

/// @file EiPrimary.h
/// @brief EiPrimary のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.

#include "ei/EiExpr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiPrimary EiPrimary.h "EiPrimary.h"
/// @brief プライマリ式の基底クラス
//////////////////////////////////////////////////////////////////////
class EiPrimaryBase :
  public EiExprBase
{
public:

  /// @brief コンストラクタ
  EiPrimaryBase(
    const PtExpr* pt_expr ///< [in] パース木の定義要素
  );

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
  void
  _set_reqsize(
    const VlValueType& type ///< [in] 要求される式の型
  ) override;

};


//////////////////////////////////////////////////////////////////////
/// @class EiPrimary EiPrimary.h "EiPrimary.h"
/// @brief プライマリ式を表すクラス
//////////////////////////////////////////////////////////////////////
class EiPrimary :
  public EiPrimaryBase
{
public:

  /// @brief コンストラクタ
  EiPrimary(
    const PtExpr* pt_expr, ///< [in] パース木の定義要素
    const VlDecl* obj      ///< [in] 本体のオブジェクト
  );

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
  bool
  is_const() const override;

  /// @brief 宣言要素もしくは配列型宣言要素への参照を返す．
  const VlDeclBase*
  decl_base() const override;

  /// @brief 宣言要素への参照の場合，対象のオブジェクトを返す．
  const VlDecl*
  decl_obj() const override;

  /// @brief 左辺式の要素数の取得
  SizeType
  lhs_elem_num() const override;

  /// @brief 左辺式の要素の取得
  const VlExpr*
  lhs_elem(
    SizeType pos ///< [in] 位置 ( 0 <= pos < lhs_elem_num() )
  ) const override;

  /// @brief 左辺式の要素のリストの取得
  vector<const VlExpr*>
  lhs_elem_list() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象の宣言要素
  const VlDecl* mObj;

};


//////////////////////////////////////////////////////////////////////
/// @class EiDeclPrimary EiPrimary.h "EiPrimary.h"
/// @brief プライマリ式を表すクラス
//////////////////////////////////////////////////////////////////////
class EiDeclPrimary :
  public EiExpr
{
public:

  /// @brief コンストラクタ
  EiDeclPrimary(
    const PtDeclItem* pt_item, ///< [in] パース木の定義要素
    const VlDecl* obj          ///< [in] 本体のオブジェクト
  );

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
  bool
  is_const() const override;

  /// @brief プライマリ(net/reg/variables/parameter)の時に true を返す．
  bool
  is_primary() const override;

  /// @brief 宣言要素もしくは配列型宣言要素への参照を返す．
  const VlDeclBase*
  decl_base() const override;

  /// @brief 宣言要素への参照の場合，対象のオブジェクトを返す．
  const VlDecl*
  decl_obj() const override;

  /// @brief Verilog-HDL の文字列を得る．
  string
  decompile() const override;

  /// @brief 左辺式の要素数の取得
  SizeType
  lhs_elem_num() const override;

  /// @brief 左辺式の要素の取得
  const VlExpr*
  lhs_elem(
    SizeType pos ///< [in] 位置 ( 0 <= pos < lhs_elem_num() )
  ) const override;

  /// @brief 左辺式の要素のリストの取得
  vector<const VlExpr*>
  lhs_elem_list() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要求される式の型を計算してセットする．
  void
  _set_reqsize(
    const VlValueType& type ///< [in] 要求される式の型
  ) override;


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
  const VlDecl* mObj;

};


//////////////////////////////////////////////////////////////////////
/// @class EiDeclPrimary EiPrimary.h "EiPrimary.h"
/// @brief プライマリ式を表すクラス
//////////////////////////////////////////////////////////////////////
class EiDeclArrayPrimary :
  public EiExpr
{
public:

  /// @brief コンストラクタ
  EiDeclArrayPrimary(
    const PtExpr* pt_expr, ///< [in] パース木の定義要素
    const VlDeclArray* obj ///< [in] 本体のオブジェクト
  );

  /// @brief デストラクタ
  ~EiDeclArrayPrimary();


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
  bool
  is_const() const override;

  /// @brief プライマリ(net/reg/variables/parameter)の時に true を返す．
  bool
  is_primary() const override;

  /// @brief 宣言要素もしくは配列型宣言要素への参照を返す．
  const VlDeclBase*
  decl_base() const override;

  /// @brief 宣言要素への参照の場合，対象のオブジェクトを返す．
  const VlDecl*
  decl_obj() const override;

  /// @brief Verilog-HDL の文字列を得る．
  string
  decompile() const override;

  /// @brief 左辺式の要素数の取得
  SizeType
  lhs_elem_num() const override;

  /// @brief 左辺式の要素の取得
  const VlExpr*
  lhs_elem(
    SizeType pos ///< [in] 位置 ( 0 <= pos < lhs_elem_num() )
  ) const override;

  /// @brief 左辺式の要素のリストの取得
  vector<const VlExpr*>
  lhs_elem_list() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要求される式の型を計算してセットする．
  void
  _set_reqsize(
    const VlValueType& type ///< [in] 要求される式の型
  ) override;


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
  const PtExpr* mPtObj;

  // 対象の宣言要素
  const VlDeclArray* mObj;

};


//////////////////////////////////////////////////////////////////////
/// @class EiParamPrimary EiPrimary.h "EiPrimary.h"
/// @brief パラメータ用のプライマリ式を表すクラス
//////////////////////////////////////////////////////////////////////
class EiParamPrimary :
  public EiPrimaryBase
{
public:

  /// @brief コンストラクタ
  EiParamPrimary(
    const PtExpr* pt_expr, ///< [in] パース木の定義要素
    ElbParameter* obj      ///< [in] 本体のオブジェクト
  );

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
  bool
  is_const() const override;

  /// @brief 定数値を返す．
  VlValue
  constant_value() const override;

  /// @brief 宣言要素もしくは配列型宣言要素への参照を返す．
  const VlDeclBase*
  decl_base() const override;

  /// @brief 宣言要素への参照の場合，対象のオブジェクトを返す．
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
public:

  /// @brief コンストラクタ
  EiArrayElemPrimary(
    const PtExpr* pt_expr,             ///< [in] パース木の定義要素
    const VlDeclArray* obj,            ///< [in] 本体のオブジェクト
    const vector<ElbExpr*>& index_list ///< [in] インデックスのリスト
  );

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
  bool
  is_const() const override;

  /// @brief 宣言要素もしくは配列型宣言要素への参照を返す．
  const VlDeclBase*
  decl_base() const override;

  /// @brief 宣言要素への参照の場合，対象のオブジェクトを返す．
  const VlDeclArray*
  declarray_obj() const override;

  /// @brief 配列型宣言要素への参照の場合，配列の次元を返す．
  SizeType
  declarray_dimension() const override;

  /// @brief 配列型宣言要素への参照の場合，配列のインデックスを返す．
  const VlExpr*
  declarray_index(
    SizeType pos ///< [in] 位置番号 ( 0 <= pos < declarray_dimension() )
  ) const override;

  /// @brief 左辺式の要素数の取得
  SizeType
  lhs_elem_num() const override;

  /// @brief 左辺式の要素の取得
  const VlExpr*
  lhs_elem(
    SizeType pos ///< [in] 位置 ( 0 <= pos < lhs_elem_num() )
  ) const override;

  /// @brief 左辺式の要素のリストの取得
  vector<const VlExpr*>
  lhs_elem_list() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象の宣言要素
  const VlDeclArray* mObj;

  // インデックスのリスト
  vector<ElbExpr*> mIndexList;

};


//////////////////////////////////////////////////////////////////////
/// @class EiArrayElemPrimary EiPrimary.h "EiPrimary.h"
/// @brief 固定インデックスの配列要素のプライマリ式を表すクラス
//////////////////////////////////////////////////////////////////////
class EiConstArrayElemPrimary :
  public EiPrimaryBase
{
public:

  /// @brief コンストラクタ
  EiConstArrayElemPrimary(
    const PtExpr* pt_expr,  ///< [in] パース木の定義要素
    const VlDeclArray* obj, ///< [in] 本体のオブジェクト
    SizeType offset         ///< [in] オフセット
  );

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
  bool
  is_const() const override;

  /// @brief 部分/ビット指定が定数の時 true を返す．
  bool
  is_constant_select() const override;

  /// @brief 宣言要素もしくは配列型宣言要素への参照を返す．
  const VlDeclBase*
  decl_base() const override;

  /// @brief 宣言要素への参照の場合，対象のオブジェクトを返す．
  const VlDeclArray*
  declarray_obj() const override;

  /// @brief 配列型宣言要素への参照の場合，配列の次元を返す．
  SizeType
  declarray_dimension() const override;

  /// @brief 配列型宣言要素への参照の場合，配列のインデックスを返す．
  const VlExpr*
  declarray_index(
    SizeType pos ///< [in] 位置番号 ( 0 <= pos < declarray_dimension() )
  ) const override;

  /// @brief 配列型宣言要素への参照のオフセットを返す．
  SizeType
  declarray_offset() const override;

  /// @brief 左辺式の要素数の取得
  SizeType
  lhs_elem_num() const override;

  /// @brief 左辺式の要素の取得
  const VlExpr*
  lhs_elem(
    SizeType pos ///< [in] 位置 ( 0 <= pos < lhs_elem_num() )
  ) const override;

  /// @brief 左辺式の要素のリストの取得
  vector<const VlExpr*>
  lhs_elem_list() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象の宣言要素
  const VlDeclArray* mObj;

  // オフセット
  SizeType mOffset;

};


//////////////////////////////////////////////////////////////////////
/// @class EiScopePrimary EiPrimary.h "EiPrimary.h"
/// @brief VlNamedObj のプライマリ式を表すクラス
//////////////////////////////////////////////////////////////////////
class EiScopePrimary :
  public EiPrimaryBase
{
public:

  /// @brief コンストラクタ
  EiScopePrimary(
    const PtExpr* pt_expr, ///< [in] パース木の定義要素
    const VlScope* obj     ///< [in] 本体のオブジェクト
  );

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
  bool
  is_const() const override;

  /// @brief 対象のオブジェクトを返す．
  const VlScope*
  scope_obj() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象のスコープ
  const VlScope* mObj;

};


//////////////////////////////////////////////////////////////////////
/// @class EiPrimitivePrimary EiPrimary.h "EiPrimary.h"
/// @brief VlPrimitive のプライマリ式を表すクラス
//////////////////////////////////////////////////////////////////////
class EiPrimitivePrimary :
  public EiPrimaryBase
{
public:

  /// @brief コンストラクタ
  EiPrimitivePrimary(
    const PtExpr* pt_expr, ///< [in] パース木の定義要素
    const VlPrimitive* obj ///< [in] 本体のオブジェクト
  );

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
  const VlPrimitive* mObj;

};

END_NAMESPACE_YM_VERILOG

#endif // EIPRIMARY_H
