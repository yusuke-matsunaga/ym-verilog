
/// @file EiDecl.cc
/// @brief EiDecl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.

#include "ei/EiFactory.h"
#include "ei/EiDecl.h"

#include "elaborator/ElbExpr.h"

#include "ym/pt/PtDecl.h"
#include "ym/pt/PtItem.h"
#include "ym/pt/PtMisc.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief 宣言要素を生成する．
ElbDecl*
EiFactory::new_Decl(
  ElbDeclHead* head,
  const PtNamedBase* pt_item,
  const VlExpr* init
)
{
  ElbDecl* decl = nullptr;
  if ( init ) {
    decl = new EiDeclI{head, pt_item, init};
  }
  else {
    decl = new EiDecl{head, pt_item};
  }
  return decl;
}


//////////////////////////////////////////////////////////////////////
// クラス EiDecl
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiDecl::EiDecl(
  ElbDeclHead* head,
  const PtNamedBase* pt_item
) : mHead{head},
    mPtItem{pt_item},
    mAuxSign{false}
{
}

// @brief デストラクタ
EiDecl::~EiDecl()
{
}

// @brief 型の取得
VpiObjType
EiDecl::type() const
{
  return mHead->type();
}

// @brief ファイル位置の取得
FileRegion
EiDecl::file_region() const
{
  return mPtItem->file_region();
}

// @brief このオブジェクトの属しているスコープを返す．
const VlScope*
EiDecl::parent_scope() const
{
  return mHead->parent_scope();
}

// @brief 名前の取得
string
EiDecl::name() const
{
  return mPtItem->name();
}

// @breif 値の型を返す．
VlValueType
EiDecl::value_type() const
{
  switch ( type() ) {
  case VpiObjType::Net:
  case VpiObjType::Reg:
    return VlValueType(is_signed(), true, bit_size());

  case VpiObjType::IntegerVar:
    return VlValueType::int_type();

  case VpiObjType::RealVar:
    return VlValueType::real_type();

  case VpiObjType::TimeVar:
    return VlValueType::time_type();

  case VpiObjType::Parameter:
  case VpiObjType::SpecParam:
  case VpiObjType::Constant:
    // ここにはこない
    ASSERT_NOT_REACHED;
    break;

  default:
    // 上記以外は形無し
    break;
  }

  return VlValueType();
}

// @brief 符号の取得
bool
EiDecl::is_signed() const
{
  return mHead->is_signed() || mAuxSign;
}

// @brief 範囲指定を持つとき true を返す．
bool
EiDecl::has_range() const
{
  return mHead->has_range();
}

// @brief 範囲の MSB の値を返す．
int
EiDecl::left_range_val() const
{
  return mHead->left_range_val();
}

// @brief 範囲の LSB の値を返す．
int
EiDecl::right_range_val() const
{
  return mHead->right_range_val();
}

// @brief 範囲のMSBを表す文字列の取得
string
EiDecl::left_range_string() const
{
  return mHead->left_range_string();
}

// @brief 範囲のLSBを表す文字列の取得
string
EiDecl::right_range_string() const
{
  return mHead->right_range_string();
}

// @brief left_range >= right_range の時に true を返す．
bool
EiDecl::is_big_endian() const
{
  return mHead->is_big_endian();
}

// @brief left_range <= right_range の時に true を返す．
bool
EiDecl::is_little_endian() const
{
  return mHead->is_little_endian();
}

// @brief ビット幅を返す．
SizeType
EiDecl::bit_size() const
{
  return mHead->bit_size();
}

// @brief オフセット値の取得
bool
EiDecl::calc_bit_offset(
  int index,
  SizeType& offset
) const
{
  return mHead->calc_bit_offset(index, offset);
}

// @brief データ型の取得
VpiVarType
EiDecl::data_type() const
{
  return mHead->data_type();
}

// @brief net 型の取得
VpiNetType
EiDecl::net_type() const
{
  return mHead->net_type();
}

// @brief vectored|scalared 属性の取得
VpiVsType
EiDecl::vs_type() const
{
  return mHead->vs_type();
}

// @brief drive0 strength の取得
VpiStrength
EiDecl::drive0() const
{
  return mHead->drive0();
}

// @brief drive1 strength の取得
VpiStrength
EiDecl::drive1() const
{
  return mHead->drive1();
}

// @brief charge strength の取得
VpiStrength
EiDecl::charge() const
{
  return mHead->charge();
}

// @brief delay の取得
const VlDelay*
EiDecl::delay() const
{
  return mHead->delay();
}

// @brief 定数値を持つ型のときに true を返す．
bool
EiDecl::is_consttype() const
{
  return false;
}

// @brief 初期値の取得
const VlExpr*
EiDecl::init_value() const
{
  return nullptr;
}

// @brief localparam のときに true 返す．
bool
EiDecl::is_local_param() const
{
  return false;
}

// @brief 符号付きに補正する．
void
EiDecl::set_signed()
{
  mAuxSign = true;
}


//////////////////////////////////////////////////////////////////////
// クラス EiDeclI
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiDeclI::EiDeclI(
  ElbDeclHead* head,
  const PtNamedBase* pt_item,
  const VlExpr* init
) : EiDecl{head, pt_item},
    mInit{init}
{
}

// @brief デストラクタ
EiDeclI::~EiDeclI()
{
}

// @brief 初期値の取得
const VlExpr*
EiDeclI::init_value() const
{
  return mInit;
}

// @brief 初期値の設定
void
EiDeclI::set_init(
  const VlExpr* expr
)
{
  mInit = expr;
}

END_NAMESPACE_YM_VERILOG
