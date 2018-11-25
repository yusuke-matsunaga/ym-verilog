﻿
/// @file EiDeclHead.cc
/// @brief EiDeclHead の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "EiFactory.h"
#include "EiDeclHead.h"

#include "ym/pt/PtDecl.h"
#include "ym/pt/PtItem.h"
#include "ym/pt/PtMisc.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief 宣言要素のヘッダを生成する．
// @param[in] parent 親のスコープ
// @param[in] pt_head パース木の宣言ヘッダ
// @param[in] left 範囲の左側の式
// @param[in] right 範囲の右側の式
// @param[in] delay 遅延値を持つとき true
ElbDeclHead*
EiFactory::new_DeclHead(const VlNamedObj* parent,
			const PtDeclHead* pt_head,
			const PtExpr* left,
			const PtExpr* right,
			int left_val,
			int right_val,
			bool delay)
{
  ASSERT_COND( left != nullptr && right != nullptr );

  EiDeclHead* head = nullptr;
  if ( delay ) {
    void* p = mAlloc.get_memory(sizeof(EiDeclHeadPtVD));
    head = new (p) EiDeclHeadPtVD(parent, pt_head,
				  left, right,
				  left_val, right_val);
  }
  else {
    void* p = mAlloc.get_memory(sizeof(EiDeclHeadPtV));
    head = new (p) EiDeclHeadPtV(parent, pt_head,
				 left, right,
				 left_val, right_val);
  }
  return head;
}

// @brief 宣言要素のヘッダを生成する．
// @param[in] parent 親のスコープ
// @param[in] pt_head パース木の宣言ヘッダ
// @param[in] delay 遅延値を持つとき true
ElbDeclHead*
EiFactory::new_DeclHead(const VlNamedObj* parent,
			const PtDeclHead* pt_head,
			bool delay)
{
  EiDeclHead* head = nullptr;
  if ( delay ) {
    void* p = mAlloc.get_memory(sizeof(EiDeclHeadPtD));
    head = new (p) EiDeclHeadPtD(parent, pt_head);
  }
  else {
    void* p = mAlloc.get_memory(sizeof(EiDeclHeadPt));
    head = new (p) EiDeclHeadPt(parent, pt_head);
  }
  return head;
}

// @brief 宣言要素のヘッダを生成する．(IODecl 中の宣言用)
// @param[in] parent 親のスコープ
// @param[in] pt_head パース木のIO宣言ヘッダ
// @param[in] aux_type 補助的なデータ型
// @param[in] left 範囲の左側の式
// @param[in] right 範囲の右側の式
// @param[in] left_val 範囲の左側の値
// @param[in] right_val 範囲の右側の値
ElbDeclHead*
EiFactory::new_DeclHead(const VlNamedObj* parent,
			const PtIOHead* pt_head,
			tVpiAuxType aux_type,
			const PtExpr* left,
			const PtExpr* right,
			int left_val,
			int right_val)
{
  ASSERT_COND( left != nullptr && right != nullptr );

  void* p = mAlloc.get_memory(sizeof(EiDeclHeadPt2V));
  EiDeclHead* head = new (p) EiDeclHeadPt2V(parent, pt_head, aux_type,
					    left, right,
					    left_val, right_val);
  return head;
}

// @brief 宣言要素のヘッダを生成する．(IODecl 中の宣言用)
// @param[in] parent 親のスコープ
// @param[in] pt_head パース木のIO宣言ヘッダ
// @param[in] aux_type 補助的なデータ型
ElbDeclHead*
EiFactory::new_DeclHead(const VlNamedObj* parent,
			const PtIOHead* pt_head,
			tVpiAuxType aux_type)
{
  void* p = mAlloc.get_memory(sizeof(EiDeclHeadPt2));
  EiDeclHead* head = new (p) EiDeclHeadPt2(parent, pt_head, aux_type);
  return head;
}

// @brief 宣言要素のヘッダを生成する．(function の暗黙宣言用)
// @param[in] parent 親のスコープ
// @param[in] pt_item パース木の関数定義
// @param[in] left 範囲の左側の式
// @param[in] right 範囲の右側の式
// @param[in] left_val 範囲の左側の値
// @param[in] right_val 範囲の右側の値
ElbDeclHead*
EiFactory::new_DeclHead(const VlNamedObj* parent,
			const PtItem* pt_item,
			const PtExpr* left,
			const PtExpr* right,
			int left_val,
			int right_val)
{
  ASSERT_COND( left != nullptr && right != nullptr );

  void* p = mAlloc.get_memory(sizeof(EiDeclHeadPt3V));
  EiDeclHead* head = new (p) EiDeclHeadPt3V(parent, pt_item,
					    left, right,
					    left_val, right_val);
  return head;
}

// @brief 宣言要素のヘッダを生成する．(function の暗黙宣言用)
// @param[in] parent 親のスコープ
// @param[in] pt_item パース木の関数定義
ElbDeclHead*
EiFactory::new_DeclHead(const VlNamedObj* parent,
			const PtItem* pt_item)
{
  void* p = mAlloc.get_memory(sizeof(EiDeclHeadPt3));
  EiDeclHead* head = new (p) EiDeclHeadPt3(parent, pt_item);
  return head;
}


//////////////////////////////////////////////////////////////////////
// クラス EiDeclHead
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のスコープ
EiDeclHead::EiDeclHead(const VlNamedObj* parent) :
  mParent(parent)
{
}

// @brief デストラクタ
EiDeclHead::~EiDeclHead()
{
}

// @brief このオブジェクトの属しているスコープを返す．
const VlNamedObj*
EiDeclHead::parent() const
{
  return mParent;
}


//////////////////////////////////////////////////////////////////////
// クラス EiDeclHeadPt
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のスコープ
// @param[in] pt_header パース木の宣言ヘッダ
EiDeclHeadPt::EiDeclHeadPt(const VlNamedObj* parent,
			   const PtDeclHead* pt_header) :
  EiDeclHead(parent),
  mPtHead(pt_header)
{
}

// @brief デストラクタ
EiDeclHeadPt::~EiDeclHeadPt()
{
}

// @brief 型の取得
// @return vpi_user.h で定義された型 (vpiModule など)
tVpiObjType
EiDeclHeadPt::type() const
{
  switch ( mPtHead->type() ) {
  case kPtDecl_Param:
  case kPtDecl_LocalParam:
    return kVpiParameter;

  case kPtDecl_Reg:
    return kVpiReg;

  case kPtDecl_Var:
    switch ( mPtHead->data_type() ) {
    case kVpiVarInteger:  return kVpiIntegerVar;
    case kVpiVarReal:     return kVpiRealVar;
    case kVpiVarTime:     return kVpiTimeVar;
    default: break;
    }
    break;

  case kPtDecl_Net:
    return kVpiNet;

  case kPtDecl_Event:
    return kVpiNamedEvent;

  case kPtDecl_SpecParam:
    return kVpiSpecParam;

  default:
    break;
  }
  ASSERT_NOT_REACHED;
  return kVpiParameter;
}

// @brief 符号の取得
// @retval true 符号つき
// @retval false 符号なし
bool
EiDeclHeadPt::is_signed() const
{
  return mPtHead->is_signed();
}

// @brief 範囲指定を持つとき true を返す．
bool
EiDeclHeadPt::has_range() const
{
  return false;
}

// @brief 範囲の MSB の値を返す．
// @note 範囲を持たないときの値は不定
int
EiDeclHeadPt::left_range_val() const
{
  return 0;
}

// @brief 範囲の LSB の値を返す．
// @note 範囲を持たないときの値は不定
int
EiDeclHeadPt::right_range_val() const
{
  return 0;
}

// @brief 範囲のMSBを表す文字列の取得
// @note 範囲を持たない時の値は不定
string
EiDeclHeadPt::left_range_string() const
{
  return string();
}

// @brief 範囲のLSBを表す文字列の取得
// @note 範囲を持たない時の値は不定
string
EiDeclHeadPt::right_range_string() const
{
  return string();
}

// @brief left_range >= right_range の時に true を返す．
bool
EiDeclHeadPt::is_big_endian() const
{
  return true;
}

// @brief left_range <= right_range の時に true を返す．
bool
EiDeclHeadPt::is_little_endian() const
{
  return true;
}

// @brief ビット幅を返す．
int
EiDeclHeadPt::bit_size() const
{
  switch ( mPtHead->type() ) {
  case kPtDecl_Reg:
  case kPtDecl_Net:
    // この型は範囲指定を含まないので 1ビットとなる．
    return 1;

  case kPtDecl_Param:
  case kPtDecl_LocalParam:
  case kPtDecl_Var:
    switch ( mPtHead->data_type() ) {
    case kVpiVarInteger:
      return kVpiSizeInteger;

    case kVpiVarReal:
      return kVpiSizeReal;

    case kVpiVarTime:
      return kVpiSizeTime;

    default:
      // int とみなす．
      return kVpiSizeInteger;
    }
    break;

  case kPtDecl_Event:
    return 0;

  case kPtDecl_SpecParam:
    return kVpiSizeInteger;

  default:
    break;
  }
  ASSERT_NOT_REACHED;
  return 0;
}

// @brief オフセット値の取得
// @param[in] index インデックス
// @param[out] offset インデックスに対するオフセット値
// @retval true インデックスが範囲内に入っている時
// @retval false インデックスが範囲外の時
bool
EiDeclHeadPt::calc_bit_offset(int index,
			      int& offset) const
{
  switch ( mPtHead->type() ) {
  case kPtDecl_Reg:
  case kPtDecl_Net:
    // この型は範囲指定を含まないので 1ビットとなる．
    if ( index == 0 ) {
      offset = 0;
      return true;
    }
    // 0 以外のインデックスは無効
    return false;

  case kPtDecl_Param:
  case kPtDecl_LocalParam:
  case kPtDecl_Var:
    switch ( mPtHead->data_type() ) {
    case kVpiVarReal:
      // 実数タイプの部分ビット指定は無効
      return false;

    case kVpiVarTime:
      if ( index >= 0 && index < static_cast<int>(kVpiSizeTime) ) {
	offset = index;
	return true;
      }
      // 範囲外は無効
      return false;

    case kVpiVarInteger:
    default:
      // int とみなす．
      if ( index >= 0 && index < static_cast<int>(kVpiSizeInteger) ) {
	offset = index;
	return true;
      }
      // 範囲外は無効
      return false;
    }
    break;

  case kPtDecl_Event:
    // イベントオブジェクトは部分指定できない．
    // というかたぶん，ここには来ないはず．
    ASSERT_NOT_REACHED;
    return false;

  case kPtDecl_SpecParam:
    // int とみなす．
    if ( index >= 0 && index < static_cast<int>(kVpiSizeInteger) ) {
      offset = index;
      return true;
    }
    // 範囲外は無効
    return false;

  default:
    break;
  }
  ASSERT_NOT_REACHED;
  return false;
}

// @brief データ型の取得
// @retval データ型 kParam, kLocalParam, kVar の場合
// @retval kVpiVarNone 上記以外
tVpiVarType
EiDeclHeadPt::data_type() const
{
  return mPtHead->data_type();
}

// @brief net 型の取得
// @retval net 型 net 型の要素の場合
// @retval kVpiNone net 型の要素でない場合
tVpiNetType
EiDeclHeadPt::net_type() const
{
  return mPtHead->net_type();
}

// @brief vectored|scalared 属性の取得
// @retval kVpiVsNone vectored|scalared 指定なし
// @retval kVpiVectored vectored 指定あり
// @retval kVpiScalared scalared 指定あり
tVpiVsType
EiDeclHeadPt::vs_type() const
{
  return mPtHead->vs_type();
}

// @brief drive0 strength の取得
// @retval 0 の強度
// @retval kVpiNoStrength strength の指定なし
tVpiStrength
EiDeclHeadPt::drive0() const
{
  if ( mPtHead->strength() ) {
    return mPtHead->strength()->drive0();
  }
  else {
    return kVpiNoStrength;
  }
}

// @brief drive1 strength の取得
// @retval 1 の強度
// @retval kVpiNoStrength strength の指定なし
tVpiStrength
EiDeclHeadPt::drive1() const
{
  if ( mPtHead->strength() ) {
    return mPtHead->strength()->drive1();
  }
  else {
    return kVpiNoStrength;
  }
}

// @brief charge strength の取得
// @retval 電荷の強度
// @retval kVpiNoStrength strength の指定なし
tVpiStrength
EiDeclHeadPt::charge() const
{
  if ( mPtHead->strength() ) {
    return mPtHead->strength()->charge();
  }
  else {
    return kVpiNoStrength;
  }
}


//////////////////////////////////////////////////////////////////////
// クラス EiDeclHeadPtD
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のスコープ
// @param[in] pt_header パース木の宣言ヘッダ
EiDeclHeadPtD::EiDeclHeadPtD(const VlNamedObj* parent,
			     const PtDeclHead* pt_header) :
  EiDeclHeadPt(parent, pt_header)
{
}

// @brief デストラクタ
EiDeclHeadPtD::~EiDeclHeadPtD()
{
}

// @brief 遅延式の取得
// @retval delay
// @retval nullptr delay の指定なし
ElbDelay*
EiDeclHeadPtD::delay() const
{
  return mDelay;
}

// @brief 遅延式の設定
void
EiDeclHeadPtD::set_delay(ElbDelay* delay)
{
  mDelay = delay;
}


//////////////////////////////////////////////////////////////////////
// クラス EiDeclHeadPtV
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のスコープ
// @param[in] pt_header パース木の宣言ヘッダ
// @param[in] left 範囲の左側の式
// @param[in] right 範囲の右側の式
// @param[in] left_val 範囲の左側の値
// @param[in] right_val 範囲の右側の値
EiDeclHeadPtV::EiDeclHeadPtV(const VlNamedObj* parent,
			     const PtDeclHead* pt_header,
			     const PtExpr* left,
			     const PtExpr* right,
			     int left_val,
			     int right_val) :
  EiDeclHeadPt(parent, pt_header)
{
  mRange.set(left, right, left_val, right_val);
}

// @brief デストラクタ
EiDeclHeadPtV::~EiDeclHeadPtV()
{
}

// @brief 範囲指定を持つとき true を返す．
bool
EiDeclHeadPtV::has_range() const
{
  return true;
}

// @brief 範囲の MSB の値を返す．
// @note 範囲を持たないときの値は不定
int
EiDeclHeadPtV::left_range_val() const
{
  return mRange.left_range_val();
}

// @brief 範囲の LSB の値を返す．
// @note 範囲を持たないときの値は不定
int
EiDeclHeadPtV::right_range_val() const
{
  return mRange.right_range_val();
}

// @brief 範囲のMSBを表す文字列の取得
// @note 範囲を持たない時の値は不定
string
EiDeclHeadPtV::left_range_string() const
{
  return mRange.left_range_string();
}

// @brief 範囲のLSBを表す文字列の取得
// @note 範囲を持たない時の値は不定
string
EiDeclHeadPtV::right_range_string() const
{
  return mRange.right_range_string();
}

// @brief left_range >= right_range の時に true を返す．
bool
EiDeclHeadPtV::is_big_endian() const
{
  return mRange.is_big_endian();
}

// @brief left_range <= right_range の時に true を返す．
bool
EiDeclHeadPtV::is_little_endian() const
{
  return mRange.is_little_endian();
}

// @brief ビット幅を返す．
int
EiDeclHeadPtV::bit_size() const
{
  return mRange.size();
}

// @brief オフセット値の取得
// @param[in] index インデックス
// @param[out] offset インデックスに対するオフセット値
// @retval true インデックスが範囲内に入っている時
// @retval false インデックスが範囲外の時
bool
EiDeclHeadPtV::calc_bit_offset(int index,
			       int& offset) const
{
  return mRange.calc_offset(index, offset);
}


//////////////////////////////////////////////////////////////////////
// クラス EiDeclHeadPtVD
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のスコープ
// @param[in] pt_header パース木の宣言ヘッダ
// @param[in] left 範囲の左側の式
// @param[in] right 範囲の右側の式
// @param[in] left_val 範囲の左側の値
// @param[in] right_val 範囲の右側の値
EiDeclHeadPtVD::EiDeclHeadPtVD(const VlNamedObj* parent,
			       const PtDeclHead* pt_header,
			       const PtExpr* left,
			       const PtExpr* right,
			       int left_val,
			       int right_val) :
  EiDeclHeadPtV(parent, pt_header, left, right, left_val, right_val),
  mDelay(nullptr)
{
}

// @brief デストラクタ
EiDeclHeadPtVD::~EiDeclHeadPtVD()
{
}

// @brief delay の取得
// @retval delay
// @retval nullptr delay の指定なし
ElbDelay*
EiDeclHeadPtVD::delay() const
{
  return mDelay;
}

// @brief 遅延式の設定
void
EiDeclHeadPtVD::set_delay(ElbDelay* delay)
{
  mDelay = delay;
}


//////////////////////////////////////////////////////////////////////
// クラス EiDeclHeadPt2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のスコープ
// @param[in] pt_header パース木のIO宣言ヘッダ
// @param[in] aux_type 補助的なデータ型
EiDeclHeadPt2::EiDeclHeadPt2(const VlNamedObj* parent,
			     const PtIOHead* pt_header,
			     tVpiAuxType aux_type) :
  EiDeclHead(parent),
  mPtHead(pt_header),
  mAuxType(aux_type)
{
}

// @brief デストラクタ
EiDeclHeadPt2::~EiDeclHeadPt2()
{
}

// @brief 型の取得
tVpiObjType
EiDeclHeadPt2::type() const
{
  switch ( mAuxType ) {
  case kVpiAuxNet: return kVpiNet;
  case kVpiAuxReg: return kVpiReg;
  case kVpiAuxVar:
    switch ( mPtHead->var_type() ) {
    case kVpiVarInteger:  return kVpiIntegerVar;
    case kVpiVarReal:     return kVpiRealVar;
    case kVpiVarTime:     return kVpiTimeVar;
    default: break;
    }
    break;

  default:
    break;
  }
  ASSERT_NOT_REACHED;
  return kVpiNet;
}

// @brief 符号の取得
// @retval true 符号つき
// @retval false 符号なし
bool
EiDeclHeadPt2::is_signed() const
{
  return mPtHead->is_signed();
}

// @brief 範囲指定を持つとき true を返す．
bool
EiDeclHeadPt2::has_range() const
{
  return false;
}

// @brief 範囲の MSB の値を返す．
// @note 範囲を持たないときの値は不定
int
EiDeclHeadPt2::left_range_val() const
{
  return 0;
}

// @brief 範囲の LSB の値を返す．
// @note 範囲を持たないときの値は不定
int
EiDeclHeadPt2::right_range_val() const
{
  return 0;
}

// @brief 範囲のMSBを表す文字列の取得
// @note 範囲を持たない時の値は不定
string
EiDeclHeadPt2::left_range_string() const
{
  return string();
}

// @brief 範囲のLSBを表す文字列の取得
// @note 範囲を持たない時の値は不定
string
EiDeclHeadPt2::right_range_string() const
{
  return string();
}

// @brief left_range >= right_range の時に true を返す．
bool
EiDeclHeadPt2::is_big_endian() const
{
  return true;
}

// @brief left_range <= right_range の時に true を返す．
bool
EiDeclHeadPt2::is_little_endian() const
{
  return true;
}

// @brief ビット幅を返す．
int
EiDeclHeadPt2::bit_size() const
{
  switch ( mAuxType ) {
  case kVpiAuxNet: return 1;
  case kVpiAuxReg: return 1;
  case kVpiAuxVar:
    switch ( mPtHead->var_type() ) {
    case kVpiVarInteger:  return kVpiSizeInteger;
    case kVpiVarReal:     return kVpiSizeReal;
    case kVpiVarTime:     return kVpiSizeTime;
    default: break;
    }
    break;

  default:
    break;
  }
  ASSERT_NOT_REACHED;
  return 0;
}

// @brief オフセット値の取得
// @param[in] index インデックス
// @param[out] offset インデックスに対するオフセット値
// @retval true インデックスが範囲内に入っている時
// @retval false インデックスが範囲外の時
bool
EiDeclHeadPt2::calc_bit_offset(int index,
			       int& offset) const
{
  switch ( mAuxType ) {
  case kVpiAuxNet:
  case kVpiAuxReg:
    // 範囲指定なしは1ビットとみなす．
    if ( index == 0 ) {
      offset = 0;
      return true;
    }
    // 0 以外は無効
    return false;

  case kVpiAuxVar:
    switch ( mPtHead->var_type() ) {
    case kVpiVarInteger:
      if ( index >= 0 && index < static_cast<int>(kVpiSizeInteger) ) {
	offset = index;
	return true;
      }
      // 範囲外は無効
      return false;

    case kVpiVarReal:
      // 実数の部分指定は無効
      return false;

    case kVpiVarTime:
      if ( index >= 0 && index < static_cast<int>(kVpiSizeTime) ) {
	return index;
      }
      // 範囲外は無効
      return false;

    default:
      break;
    }
    break;

  default:
    break;
  }
  ASSERT_NOT_REACHED;
  return false;
}

// @brief データ型の取得
// @retval データ型 kParam, kLocalParam, kVar の場合
// @retval kVpiVarNone 上記以外
tVpiVarType
EiDeclHeadPt2::data_type() const
{
  return mPtHead->var_type();
}

// @brief net 型の取得
// @retval net 型 net 型の要素の場合
// @retval kVpiNone net 型の要素でない場合
tVpiNetType
EiDeclHeadPt2::net_type() const
{
  return mPtHead->net_type();
}


//////////////////////////////////////////////////////////////////////
// クラス EiDeclHeadPt2V
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のスコープ
// @param[in] pt_header パース木のIO宣言ヘッダ
// @param[in] aux_type 補助的なデータ型
// @param[in] left 範囲の左側の式
// @param[in] right 範囲の右側の式
// @param[in] left_val 範囲の左側の値
// @param[in] right_val 範囲の右側の値
EiDeclHeadPt2V::EiDeclHeadPt2V(const VlNamedObj* parent,
			       const PtIOHead* pt_header,
			       tVpiAuxType aux_type,
			       const PtExpr* left,
			       const PtExpr* right,
			       int left_val,
			       int right_val) :
  EiDeclHeadPt2(parent, pt_header, aux_type)
{
  mRange.set(left, right, left_val, right_val);
}

// @brief デストラクタ
EiDeclHeadPt2V::~EiDeclHeadPt2V()
{
}

// @brief 範囲指定を持つとき true を返す．
bool
EiDeclHeadPt2V::has_range() const
{
  return true;
}

// @brief 範囲の MSB の値を返す．
// @note 範囲を持たないときの値は不定
int
EiDeclHeadPt2V::left_range_val() const
{
  return mRange.left_range_val();
}

// @brief 範囲の LSB の値を返す．
// @note 範囲を持たないときの値は不定
int
EiDeclHeadPt2V::right_range_val() const
{
  return mRange.right_range_val();
}

// @brief 範囲のMSBを表す文字列の取得
// @note 範囲を持たない時の値は不定
string
EiDeclHeadPt2V::left_range_string() const
{
  return mRange.left_range_string();
}

// @brief 範囲のLSBを表す文字列の取得
// @note 範囲を持たない時の値は不定
string
EiDeclHeadPt2V::right_range_string() const
{
  return mRange.right_range_string();
}

// @brief left_range >= right_range の時に true を返す．
bool
EiDeclHeadPt2V::is_big_endian() const
{
  return mRange.is_big_endian();
}

// @brief left_range <= right_range の時に true を返す．
bool
EiDeclHeadPt2V::is_little_endian() const
{
  return mRange.is_little_endian();
}

// @brief ビット幅を返す．
int
EiDeclHeadPt2V::bit_size() const
{
  return mRange.size();
}

// @brief オフセット値の取得
// @param[in] index インデックス
// @param[out] offset インデックスに対するオフセット値
// @retval true インデックスが範囲内に入っている時
// @retval false インデックスが範囲外の時
bool
EiDeclHeadPt2V::calc_bit_offset(int index,
				int& offset) const
{
  return mRange.calc_offset(index, offset);
}


//////////////////////////////////////////////////////////////////////
// クラス EiDeclHeadPt3
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のスコープ
// @param[in] pt_item パース木の関数定義
EiDeclHeadPt3::EiDeclHeadPt3(const VlNamedObj* parent,
			     const PtItem* pt_item) :
  EiDeclHead(parent),
  mPtItem(pt_item)
{
}

// @brief デストラクタ
EiDeclHeadPt3::~EiDeclHeadPt3()
{
}

// @brief 型の取得
tVpiObjType
EiDeclHeadPt3::type() const
{
  switch ( data_type() ) {
  case kVpiVarNone:     return kVpiReg;
  case kVpiVarInteger:  return kVpiIntegerVar;
  case kVpiVarReal:     return kVpiRealVar;
  case kVpiVarTime:     return kVpiTimeVar;
  default:
    break;
  }
  ASSERT_NOT_REACHED;
  return kVpiReg;
}

// @brief 符号の取得
// @retval true 符号つき
// @retval false 符号なし
bool
EiDeclHeadPt3::is_signed() const
{
  return mPtItem->is_signed();
}

// @brief 範囲指定を持つとき true を返す．
bool
EiDeclHeadPt3::has_range() const
{
  return false;
}

// @brief 範囲の MSB の値を返す．
// @note 範囲を持たないときの値は不定
int
EiDeclHeadPt3::left_range_val() const
{
  return 0;
}

// @brief 範囲の LSB の値を返す．
// @note 範囲を持たないときの値は不定
int
EiDeclHeadPt3::right_range_val() const
{
  return 0;
}

// @brief 範囲のMSBを表す文字列の取得
// @note 範囲を持たない時の値は不定
string
EiDeclHeadPt3::left_range_string() const
{
  return string();
}

// @brief 範囲のLSBを表す文字列の取得
// @note 範囲を持たない時の値は不定
string
EiDeclHeadPt3::right_range_string() const
{
  return string();
}

// @brief left_range >= right_range の時に true を返す．
bool
EiDeclHeadPt3::is_big_endian() const
{
  return true;
}

// @brief left_range <= right_range の時に true を返す．
bool
EiDeclHeadPt3::is_little_endian() const
{
  return true;
}

// @brief ビット幅を返す．
int
EiDeclHeadPt3::bit_size() const
{
  switch ( data_type() ) {
  case kVpiVarNone:     return 1;
  case kVpiVarInteger:  return kVpiSizeInteger;
  case kVpiVarReal:     return kVpiSizeReal;
  case kVpiVarTime:     return kVpiSizeTime;

  default:
    break;
  }
  ASSERT_NOT_REACHED;
  return 0;
}

// @brief オフセット値の取得
// @param[in] index インデックス
// @param[out] offset インデックスに対するオフセット値
// @retval true インデックスが範囲内に入っている時
// @retval false インデックスが範囲外の時
bool
EiDeclHeadPt3::calc_bit_offset(int index,
			       int& offset) const
{
  switch ( data_type() ) {
  case kVpiVarNone:
    // 指定なしは1ビットとみなす．
    if ( index == 0 ) {
      offset = 0;
      return true;
    }
    return false;

  case kVpiVarInteger:
    if ( index >= 0 && index < static_cast<int>(kVpiSizeInteger) ) {
      offset = index;
      return true;
    }
    // 範囲外は無効
    return false;

  case kVpiVarReal:
    // 実数の部分指定は無効
    return false;

  case kVpiVarTime:
    if ( index >= 0 && index < static_cast<int>(kVpiSizeTime) ) {
      offset = index;
      return true;
    }
    // 範囲外は無効
    return false;

  default:
    break;
  }
  ASSERT_NOT_REACHED;
  return false;
}

// @brief データ型の取得
// @retval データ型 kParam, kLocalParam, kVar の場合
// @retval kVpiVarNone 上記以外
tVpiVarType
EiDeclHeadPt3::data_type() const
{
  return mPtItem->data_type();
}

// @brief net 型の取得
// @retval net 型 net 型の要素の場合
// @retval kVpiNone net 型の要素でない場合
tVpiNetType
EiDeclHeadPt3::net_type() const
{
  return kVpiNone;
}


//////////////////////////////////////////////////////////////////////
// クラス EiDeclHeadPt3V
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のスコープ
// @param[in] pt_item パース木の関数定義
// @param[in] left 範囲の左側の式
// @param[in] right 範囲の右側の式
// @param[in] left_val 範囲の左側の値
// @param[in] right_val 範囲の右側の値
EiDeclHeadPt3V::EiDeclHeadPt3V(const VlNamedObj* parent,
			       const PtItem* pt_item,
			       const PtExpr* left,
			       const PtExpr* right,
			       int left_val,
			       int right_val) :
  EiDeclHeadPt3(parent, pt_item)
{
  mRange.set(left, right, left_val, right_val);
}

// @brief デストラクタ
EiDeclHeadPt3V::~EiDeclHeadPt3V()
{
}

// @brief 範囲指定を持つとき true を返す．
bool
EiDeclHeadPt3V::has_range() const
{
  return true;
}

// @brief 範囲の MSB の値を返す．
// @note 範囲を持たないときの値は不定
int
EiDeclHeadPt3V::left_range_val() const
{
  return mRange.left_range_val();
}

// @brief 範囲の LSB の値を返す．
// @note 範囲を持たないときの値は不定
int
EiDeclHeadPt3V::right_range_val() const
{
  return mRange.right_range_val();
}

// @brief 範囲のMSBを表す文字列の取得
// @note 範囲を持たない時の値は不定
string
EiDeclHeadPt3V::left_range_string() const
{
  return mRange.left_range_string();
}

// @brief 範囲のLSBを表す文字列の取得
// @note 範囲を持たない時の値は不定
string
EiDeclHeadPt3V::right_range_string() const
{
  return mRange.right_range_string();
}

// @brief left_range >= right_range の時に true を返す．
bool
EiDeclHeadPt3V::is_big_endian() const
{
  return mRange.is_big_endian();
}

// @brief left_range <= right_range の時に true を返す．
bool
EiDeclHeadPt3V::is_little_endian() const
{
  return mRange.is_little_endian();
}

// @brief ビット幅を返す．
int
EiDeclHeadPt3V::bit_size() const
{
  return mRange.size();
}

// @brief オフセット値の取得
// @param[in] index インデックス
// @param[out] offset インデックスに対するオフセット値
// @retval true インデックスが範囲内に入っている時
// @retval false インデックスが範囲外の時
bool
EiDeclHeadPt3V::calc_bit_offset(int index,
				int& offset) const
{
  return mRange.calc_offset(index, offset);
}

END_NAMESPACE_YM_VERILOG
