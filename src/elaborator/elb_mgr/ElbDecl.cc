﻿
/// @file ElbDecl.cc
/// @brief ElbDecl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ElbDecl.h"
#include "ElbGenvar.h"
#include "ElbExpr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス ElbDeclHead
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ElbDeclHead::ElbDeclHead()
{
}

// @brief デストラクタ
ElbDeclHead::~ElbDeclHead()
{
}

// @brief データ型の取得
// @retval データ型 kParam, kLocalParam, kVar の場合
// @retval kVpiVarNone 上記以外
// @note このクラスでは kVpiVarNone を返す．
tVpiVarType
ElbDeclHead::data_type() const
{
  return kVpiVarNone;
}

// @brief net 型の取得
// @retval net 型 net 型の要素の場合
// @retval kVpiNone net 型の要素でない場合
// @note このクラスでは kVpiNone を返す．
tVpiNetType
ElbDeclHead::net_type() const
{
  return kVpiNone;
}

// @brief vectored|scalared 属性の取得
// @retval kVpiVsNone vectored|scalared 指定なし
// @retval kVpiVectored vectored 指定あり
// @retval kVpiScalared scalared 指定あり
// @note このクラスでは kVpiNsNone を返す．
tVpiVsType
ElbDeclHead::vs_type() const
{
  return kVpiVsNone;
}

// @brief drive0 strength の取得
// @retval 0 の強度
// @retval kVpiNoStrength strength の指定なし
// @note このクラスでは kVpiNoStrength を返す．
tVpiStrength
ElbDeclHead::drive0() const
{
  return kVpiNoStrength;
}

// @brief drive1 strength の取得
// @retval 1 の強度
// @retval kVpiNoStrength strength の指定なし
// @note このクラスでは kVpiNoStrength を返す．
tVpiStrength
ElbDeclHead::drive1() const
{
  return kVpiNoStrength;
}

// @brief charge strength の取得
// @retval 電荷の強度
// @retval kVpiNoStrength strength の指定なし
// @note このクラスでは kVpiNoStrength を返す．
tVpiStrength
ElbDeclHead::charge() const
{
  return kVpiNoStrength;
}

// @brief delay の取得
// @retval delay
// @retval nullptr delay の指定なし
// @note このクラスでは nullptr を返す．
ElbDelay*
ElbDeclHead::delay() const
{
  return nullptr;
}

// @brief 遅延式の設定
// @note このクラスではなにもしない．
void
ElbDeclHead::set_delay(ElbDelay* delay)
{
}


//////////////////////////////////////////////////////////////////////
// クラス ElbDecl
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ElbDecl::ElbDecl() :
  mNext(nullptr)
{
}

// @brief デストラクタ
ElbDecl::~ElbDecl()
{
}

// @brief 定数値を持つ型のときに true を返す．
// @note このクラスは false を返す．
bool
ElbDecl::is_consttype() const
{
  return false;
}

// @brief localparam のときに true 返す．
// @note このクラスでは false を返す．
bool
ElbDecl::is_local_param() const
{
  return false;
}

// @brief 初期値の設定
// @param[in] expr 初期値
// @note このクラスではなにもしない．
void
ElbDecl::set_init(ElbExpr* expr)
{
}


//////////////////////////////////////////////////////////////////////
// クラス ElbDeclArray
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ElbDeclArray::ElbDeclArray() :
  mNext(nullptr)
{
}

// @brief デストラクタ
ElbDeclArray::~ElbDeclArray()
{
}

END_NAMESPACE_YM_VERILOG
