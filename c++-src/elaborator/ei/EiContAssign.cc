
/// @file EiContAssign.cc
/// @brief EiContAssign の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "EiFactory.h"
#include "EiContAssign.h"

#include "elb/ElbExpr.h"

#include "ym/vl/VlDelay.h"

#include "ym/pt/PtItem.h"
#include "ym/pt/PtMisc.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief continuous assignment のヘッダを生成する．
// @param[in] module 親のモジュール
// @param[in] pt_head パース木のヘッダ定義
// @param[in] delay 遅延値
// @note 遅延を持たないときは nullptr を与える．
ElbCaHead*
EiFactory::new_CaHead(const VlModule* module,
		      const PtItem* pt_head,
		      const VlDelay* delay)
{
  EiCaHead* head = nullptr;
  if ( delay ) {
    head = new EiCaHeadD(module, pt_head, delay);
  }
  else {
    head = new EiCaHead(module, pt_head);
  }
  return head;
}

// @brief continuous assignment を生成する．
// @param[in] head ヘッダ
// @param[in] pt_obj 対応するパース木中の定義要素
// @param[in] lhs 左辺式
// @param[in] rhs 右辺式
const VlContAssign*
EiFactory::new_ContAssign(ElbCaHead* head,
			  const PtBase* pt_obj,
			  ElbExpr* lhs,
			  ElbExpr* rhs)
{
  EiContAssign* cont_assign = new EiContAssign1(head, pt_obj, lhs, rhs);

  return cont_assign;
}

// @brief net 宣言中の continuous assignment を生成する．
// @param[in] module 親のモジュール
// @param[in] pt_obj パース木の定義要素
// @param[in] lhs 左辺式
// @param[in] rhs 右辺式
const VlContAssign*
EiFactory::new_ContAssign(const VlModule* module,
			  const PtBase* pt_obj,
			  ElbExpr* lhs,
			  ElbExpr* rhs)
{
  EiContAssign* cont_assign = new EiContAssign2(module, pt_obj, lhs, rhs);

  return cont_assign;
}


//////////////////////////////////////////////////////////////////////
// クラス EiCaHead
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] pt_obj パース木のヘッダ定義
EiCaHead::EiCaHead(const VlModule* module,
		   const PtItem* pt_head) :
  mModule(module),
  mPtHead(pt_head)
{
}

// @brief デストラクタ
EiCaHead::~EiCaHead()
{
}

// @brief 親のスコープを返す．
const VlModule*
EiCaHead::module() const
{
  return mModule;
}

// @brief 0の強さを返す．
VpiStrength
EiCaHead::drive0() const
{
  if ( mPtHead->strength() ) {
    return mPtHead->strength()->drive0();
  }
  return VpiStrength::NoStrength;
}

// @brief 1の強さを返す．
VpiStrength
EiCaHead::drive1() const
{
  if ( mPtHead->strength() ) {
    return mPtHead->strength()->drive0();
  }
  return VpiStrength::NoStrength;
}

// @brief 遅延を表す式を返す．
// @note このクラスでは nullptr を返す．
const VlDelay*
EiCaHead::delay() const
{
  return nullptr;
}


//////////////////////////////////////////////////////////////////////
// クラス EiCaHeadD
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] pt_head パース木のヘッダ定義
// @param[in] delay 遅延値
EiCaHeadD::EiCaHeadD(const VlModule* module,
		     const PtItem* pt_head,
		     const VlDelay* delay) :
  EiCaHead(module, pt_head),
  mDelay(delay)
{
}

// @brief デストラクタ
EiCaHeadD::~EiCaHeadD()
{
}

// @brief 遅延を表す式を返す．
const VlDelay*
EiCaHeadD::delay() const
{
  return mDelay;
}


//////////////////////////////////////////////////////////////////////
// クラス EiContAssign
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_obj 対応するパース木中の定義要素
// @param[in] lhs 左辺式
// @param[in] rhs 右辺式
EiContAssign::EiContAssign(const PtBase* pt_obj,
			   ElbExpr* lhs,
			   ElbExpr* rhs) :
  mPtObj(pt_obj),
  mLhs(lhs),
  mRhs(rhs)
{
}

// @brief デストラクタ
EiContAssign::~EiContAssign()
{
}

// @brief 型の取得
VpiObjType
EiContAssign::type() const
{
  return VpiObjType::ContAssign;
}

// @brief ファイル位置を返す．
FileRegion
EiContAssign::file_region() const
{
  return mPtObj->file_region();
}

// @brief ビット幅を返す．
int
EiContAssign::bit_size() const
{
  return lhs()->bit_size();
}

// @brief 左辺を返す．
const VlExpr*
EiContAssign::lhs() const
{
  return mLhs;
}

// @brief 右辺を返す．
const VlExpr*
EiContAssign::rhs() const
{
  return mRhs;
}


//////////////////////////////////////////////////////////////////////
// クラス EiContAssign1
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] head ヘッダ
// @param[in] pt_obj 対応するパース木中の定義要素
// @param[in] lhs 左辺式
// @param[in] rhs 右辺式
EiContAssign1::EiContAssign1(ElbCaHead* head,
			     const PtBase* pt_obj,
			     ElbExpr* lhs,
			     ElbExpr* rhs) :
  EiContAssign(pt_obj, lhs, rhs),
  mHead(head)
{
}

// @brief デストラクタ
EiContAssign1::~EiContAssign1()
{
}

// @brief 親のスコープを返す．
const VlModule*
EiContAssign1::module() const
{
  return mHead->module();
}

// @brief 0の強さを返す．
VpiStrength
EiContAssign1::drive0() const
{
  return mHead->drive0();
}

// @brief 1の強さを返す．
VpiStrength
EiContAssign1::drive1() const
{
  return mHead->drive1();
}

// @brief 遅延を表す式を返す．
const VlDelay*
EiContAssign1::delay() const
{
  return mHead->delay();
}

// @brief ネット宣言中の assignment の時に true を返す．
bool
EiContAssign1::has_net_decl_assign() const
{
  return false;
}


//////////////////////////////////////////////////////////////////////
// クラス EiContAssign2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] pt_obj パース木の定義要素
// @param[in] lhs 左辺式
// @param[in] rhs 右辺式
EiContAssign2::EiContAssign2(const VlModule* module,
			     const PtBase* pt_obj,
			     ElbExpr* lhs,
			     ElbExpr* rhs) :
  EiContAssign(pt_obj, lhs, rhs),
  mModule(module)
{
}

// @brief デストラクタ
EiContAssign2::~EiContAssign2()
{
}

// @brief 親のスコープを返す．
const VlModule*
EiContAssign2::module() const
{
  return mModule;
}

// @brief 0の強さを返す．
VpiStrength
EiContAssign2::drive0() const
{
  return VpiStrength::NoStrength;
}

// @brief 1の強さを返す．
VpiStrength
EiContAssign2::drive1() const
{
  return VpiStrength::NoStrength;
}

// @brief 遅延を表す式を返す．
const VlDelay*
EiContAssign2::delay() const
{
  return nullptr;
}

// @brief ネット宣言中の assignment の時に true を返す．
bool
EiContAssign2::has_net_decl_assign() const
{
  return true;
}


END_NAMESPACE_YM_VERILOG
