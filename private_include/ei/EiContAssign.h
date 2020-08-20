#ifndef EICONTASSIGN_H
#define EICONTASSIGN_H

/// @file EiContAssign.h
/// @brief EiContAssign のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2008, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.


#include "ym/vl/VlContAssign.h"
#include "elaborator/ElbCaHead.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiCaHead EiContAssign.h "EiContAssign.h"
/// @brief 継続的代入文のヘッダ
//////////////////////////////////////////////////////////////////////
class EiCaHead :
  public ElbCaHead
{
public:

  /// @brief コンストラクタ
  /// @param[in] module 親のモジュール
  /// @param[in] pt_head パース木のヘッダ定義
  EiCaHead(const VlModule* module,
	   const PtItem* pt_head);

  /// @brief デストラクタ
  ~EiCaHead();


public:
  //////////////////////////////////////////////////////////////////////
  // EiCaHead の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 親のモジュールを返す．
  const VlModule*
  module() const override;

  /// @brief 0の強さを返す．
  VpiStrength
  drive0() const override;

  /// @brief 1の強さを返す．
  VpiStrength
  drive1() const override;

  /// @brief 遅延を表す式を返す．
  /// @note このクラスでは nullptr を返す．
  const VlDelay*
  delay() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親のモジュール
  const VlModule* mModule;

  // パース木のヘッダ定義
  const PtItem* mPtHead;

};


//////////////////////////////////////////////////////////////////////
/// @class EiCaHeadD EiContAssign.h "EiContAssign.h"
/// @brief 遅延付きの継続的代入文のヘッダ
//////////////////////////////////////////////////////////////////////
class EiCaHeadD :
  public EiCaHead
{
public:

  /// @brief コンストラクタ
  /// @param[in] module 親のモジュール
  /// @param[in] pt_head パース木のヘッダ定義
  /// @param[in] delay 遅延値
  EiCaHeadD(const VlModule* module,
	    const PtItem* pt_head,
	    const VlDelay* delay);

  /// @brief デストラクタ
  ~EiCaHeadD();


public:
  //////////////////////////////////////////////////////////////////////
  // EiCaHead の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 遅延を表す式を返す．
  const VlDelay*
  delay() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 遅延値
  const VlDelay* mDelay;

};


//////////////////////////////////////////////////////////////////////
/// @class EiContAssign EiContAssign.h "EiContAssign.h"
/// @brief 継続的代入文の実装クラス
//////////////////////////////////////////////////////////////////////
class EiContAssign :
  public VlContAssign
{
protected:

  /// @brief コンストラクタ
  /// @param[in] pt_obj 対応するパース木中の定義要素
  /// @param[in] lhs 左辺式
  /// @param[in] rhs 右辺式
  EiContAssign(const PtBase* pt_obj,
	       const VlExpr* lhs,
	       const VlExpr* rhs);

  /// @brief デストラクタ
  ~EiContAssign();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の派生クラスに共通な仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  VpiObjType
  type() const override;

  /// @brief ファイル位置の取得
  FileRegion
  file_region() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // VlContAssign の派生クラスに共通な仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ビット幅を返す．
  int
  bit_size() const override;

  /// @brief 左辺を返す．
  const VlExpr*
  lhs() const override;

  /// @brief 右辺を返す．
  const VlExpr*
  rhs() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対応するパース木の定義要素
  const PtBase* mPtObj;

  // 左辺式
  const VlExpr* mLhs;

  // 右辺式
  const VlExpr* mRhs;

};


//////////////////////////////////////////////////////////////////////
/// @class EiContAssign EiContAssign.h "EiContAssign.h"
/// @brief 継続的代入文の実装クラス
//////////////////////////////////////////////////////////////////////
class EiContAssign1 :
  public EiContAssign
{
public:

  /// @brief コンストラクタ
  /// @param[in] head ヘッダ
  /// @param[in] pt_obj 対応するパース木中の定義要素
  /// @param[in] lhs 左辺式
  /// @param[in] rhs 右辺式
  EiContAssign1(ElbCaHead* head,
		const PtBase* pt_obj,
		const VlExpr* lhs,
		const VlExpr* rhs);

  /// @brief デストラクタ
  ~EiContAssign1();


public:
  //////////////////////////////////////////////////////////////////////
  // VlContAssign の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 親のモジュールを返す．
  const VlModule*
  module() const override;

  /// @brief 0の強さを返す．
  VpiStrength
  drive0() const override;

  /// @brief 1の強さを返す．
  VpiStrength
  drive1() const override;

  /// @brief 遅延を表す式を返す．
  const VlDelay*
  delay() const override;

  /// @brief ネット宣言中の assignment の時に true を返す．
  bool
  has_net_decl_assign() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ヘッダ
  const ElbCaHead* mHead;

};


//////////////////////////////////////////////////////////////////////
/// @class EiContAssign2 EiContAssign.h "EiContAssign.h"
/// @brief net 宣言中の継続的代入文
//////////////////////////////////////////////////////////////////////
class EiContAssign2 :
  public EiContAssign
{
public:

  /// @brief コンストラクタ
  /// @param[in] module 親のモジュール
  /// @param[in] pt_obj パース木の定義要素
  /// @param[in] lhs 左辺式
  /// @param[in] rhs 右辺式
  EiContAssign2(const VlModule* module,
		const PtBase* pt_obj,
		const VlExpr* lhs,
		const VlExpr* rhs);

  /// @brief デストラクタ
  ~EiContAssign2();


public:
  //////////////////////////////////////////////////////////////////////
  // VlContAssign の派生クラスに共通な仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 親のモジュールを返す．
  const VlModule*
  module() const override;

  /// @brief 0の強さを返す．
  VpiStrength
  drive0() const override;

  /// @brief 1の強さを返す．
  VpiStrength
  drive1() const override;

  /// @brief 遅延を表す式を返す．
  const VlDelay*
  delay() const override;

  /// @brief ネット宣言中の assignment の時に true を返す．
  bool
  has_net_decl_assign() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親のモジュール
  const VlModule* mModule;

};

END_NAMESPACE_YM_VERILOG

#endif // ELBCONTASSIGN_H
