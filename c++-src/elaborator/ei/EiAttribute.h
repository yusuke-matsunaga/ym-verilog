#ifndef EIATTRIBUTE_H
#define EIATTRIBUTE_H

/// @file EiAttribute.h
/// @brief EiAttribute のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "elb/ElbAttribute.h"
#include "ym/pt/PtP.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiAttribute EiAttribute.h "EiAttribute.h"
/// @brief genvar を表すクラス
//////////////////////////////////////////////////////////////////////
class EiAttribute :
  public ElbAttribute
{
  friend class EiFactory;
  friend class EiAttrList;

private:

  /// @brief コンストラクタ
  EiAttribute();

  /// @brief デストラクタ
  ~EiAttribute();


public:
  //////////////////////////////////////////////////////////////////////
  // ElbObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  VpiObjType
  type() const override;

  /// @brief ファイル位置を返す．
  FileRegion
  file_region() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // VlAttribute の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 属性名を返す．
  const char*
  name() const override;

  /// @brief def_attribute なら true を返す．
  bool
  def_attribute() const override;

  /// @brief 値を表す式を返す．
  VlExpr*
  expr() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // パース木の定義要素
  const PtAttrSpec* mPtAttrSpec;

  // 値
  ElbExpr* mExpr;

  // 定義側の属性の時 true をなるフラグ
  bool mDef;

};


//////////////////////////////////////////////////////////////////////
/// @class EiAttrList EiAttribute.h "EiAttribute.h"
/// @brief ElbAttrList の実装クラス
//////////////////////////////////////////////////////////////////////
class EiAttrList :
  public ElbAttrList
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  EiAttrList(SizeType n,
	     EiAttribute* array);

  /// @brief デストラクタ
  ~EiAttrList();


public:
  //////////////////////////////////////////////////////////////////////
  // VlAttrList の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素数を返す．
  SizeType
  size() const override;

  /// @brief 内容を返す．
  /// @param[in] pos 位置番号 (0 <= pos < size() )
  VlAttribute*
  elem(SizeType pos) const override;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbAttrList の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素を設定する．
  /// @param[in] pos 位置番号
  /// @param[in] pt_as パース木の定義要素
  /// @param[in] expr 値を表す式
  /// @param[in] def 定義側の属性のとき true とするフラグ
  void
  set(SizeType pos,
      const PtAttrSpec* pt_as,
      ElbExpr* expr,
      bool def) override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 要素数
  SizeType mSize;

  // 要素の配列
  EiAttribute* mArray;

};

END_NAMESPACE_YM_VERILOG

#endif // EIATTRIBUTE_H
