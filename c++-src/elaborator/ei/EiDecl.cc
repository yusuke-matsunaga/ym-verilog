
/// @file EiDecl.cc
/// @brief EiDecl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.


#include "EiFactory.h"
#include "EiDecl.h"

#include "elaborator/ElbExpr.h"

#include "ym/pt/PtDecl.h"
#include "ym/pt/PtItem.h"
#include "ym/pt/PtMisc.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief 宣言要素を生成する．
// @param[in] head ヘッダ
// @param[in] pt_item パース木の宣言要素
// @param[in] init 初期割り当て式
ElbDecl*
EiFactory::new_Decl(ElbDeclHead* head,
		    const PtNamedBase* pt_item,
		    const VlExpr* init)
{
  ElbDecl* decl = nullptr;
  if ( init ) {
    decl = new EiDeclI(head, pt_item, init);
  }
  else {
    decl = new EiDecl(head, pt_item);
  }
  return decl;
}


//////////////////////////////////////////////////////////////////////
// クラス EiDecl
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] head ヘッダ
// @param[in] pt_item パース木の宣言要素
EiDecl::EiDecl(ElbDeclHead* head,
	       const PtNamedBase* pt_item) :
  mHead{head},
  mPtItem{pt_item},
  mAuxSign{false}
{
}

// @brief デストラクタ
EiDecl::~EiDecl()
{
}

// @brief 型の取得
// @return vpi_user.h で定義された型 (vpiModule など)
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
const VlNamedObj*
EiDecl::parent() const
{
  return mHead->parent();
}

// @brief 名前の取得
const char*
EiDecl::name() const
{
  return mPtItem->name();
}

// @breif 値の型を返す．
// @note 値を持たないオブジェクトの場合には kVpiValueNone を返す．
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
// @retval true 符号つき
// @retval false 符号なし
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
// @note 範囲を持たないときの値は不定
int
EiDecl::left_range_val() const
{
  return mHead->left_range_val();
}

// @brief 範囲の LSB の値を返す．
// @note 範囲を持たないときの値は不定
int
EiDecl::right_range_val() const
{
  return mHead->right_range_val();
}

// @brief 範囲のMSBを表す文字列の取得
// @note 範囲を持たない時の値は不定
string
EiDecl::left_range_string() const
{
  return mHead->left_range_string();
}

// @brief 範囲のLSBを表す文字列の取得
// @note 範囲を持たない時の値は不定
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
// @param[in] index インデックス
// @param[out] offset インデックスに対するオフセット値
// @retval true インデックスが範囲内に入っている時
// @retval false インデックスが範囲外の時
bool
EiDecl::calc_bit_offset(int index,
			SizeType& offset) const
{
  return mHead->calc_bit_offset(index, offset);
}

// @brief データ型の取得
// @retval データ型 kParam, kLocalParam, kVar の場合
// @retval kVpiVarNone 上記以外
VpiVarType
EiDecl::data_type() const
{
  return mHead->data_type();
}

// @brief net 型の取得
// @retval net 型 net 型の要素の場合
// @retval kVpiNone net 型の要素でない場合
VpiNetType
EiDecl::net_type() const
{
  return mHead->net_type();
}

// @brief vectored|scalared 属性の取得
// @retval kVpiVsNone vectored|scalared 指定なし
// @retval kVpiVectored vectored 指定あり
// @retval kVpiScalared scalared 指定あり
VpiVsType
EiDecl::vs_type() const
{
  return mHead->vs_type();
}

// @brief drive0 strength の取得
// @retval 0 の強度
// @retval kVpiNoStrength strength の指定なし
VpiStrength
EiDecl::drive0() const
{
  return mHead->drive0();
}

// @brief drive1 strength の取得
// @retval 1 の強度
// @retval kVpiNoStrength strength の指定なし
VpiStrength
EiDecl::drive1() const
{
  return mHead->drive1();
}

// @brief charge strength の取得
// @retval 電荷の強度
// @retval kVpiNoStrength strength の指定なし
VpiStrength
EiDecl::charge() const
{
  return mHead->charge();
}

// @brief delay の取得
// @retval delay
// @retval nullptr delay の指定なし
const VlDelay*
EiDecl::delay() const
{
  return mHead->delay();
}

// @brief 定数値を持つ型のときに true を返す．
// @note このクラスは false を返す．
bool
EiDecl::is_consttype() const
{
  return false;
}

// @brief 初期値の取得
// @retval 初期値
// @retval nullptr 設定がない場合
const VlExpr*
EiDecl::init_value() const
{
  return nullptr;
}

// @brief localparam のときに true 返す．
// @note このクラスは false を返す．
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
// @param[in] head ヘッダ
// @param[in] pt_item パース木の宣言要素
// @param[in] init 初期値
EiDeclI::EiDeclI(ElbDeclHead* head,
		 const PtNamedBase* pt_item,
		 const VlExpr* init) :
  EiDecl(head, pt_item),
  mInit{init}
{
}

// @brief デストラクタ
EiDeclI::~EiDeclI()
{
}

// @brief 初期値の取得
// @retval 初期値
// @retval nullptr 設定がない場合
const VlExpr*
EiDeclI::init_value() const
{
  return mInit;
}

// @brief 初期値の設定
// @param[in] expr 初期値
void
EiDeclI::set_init(const VlExpr* expr)
{
  mInit = expr;
}

END_NAMESPACE_YM_VERILOG
