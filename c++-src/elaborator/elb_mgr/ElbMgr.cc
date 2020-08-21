
/// @file ElbMgr.cc
/// @brief ElbMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.


#include "elaborator/ElbMgr.h"
#include "ym/vl/VlContAssign.h"
#include "ym/vl/VlControl.h"
#include "ym/vl/VlDecl.h"
#include "ym/vl/VlDeclArray.h"
#include "ym/vl/VlDelay.h"
#include "ym/vl/VlParamAssign.h"
#include "ym/vl/VlPrimitive.h"
#include "ym/vl/VlProcess.h"
#include "ym/vl/VlStmt.h"
#include "ym/vl/VlTaskFunc.h"
#include "ym/vl/VlUserSystf.h"
#include "ym/vl/VlUdp.h"
#include "elaborator/ElbGfRoot.h"
#include "elaborator/ElbUdp.h"
#include "elaborator/ElbModule.h"
#include "elaborator/ElbModuleArray.h"
#include "elaborator/ElbIOHead.h"
#include "elaborator/ElbDecl.h"
#include "elaborator/ElbParameter.h"
#include "elaborator/ElbGenvar.h"
#include "elaborator/ElbCaHead.h"
#include "elaborator/ElbPrimitive.h"
#include "elaborator/ElbTaskFunc.h"
#include "elaborator/ElbProcess.h"
#include "elaborator/ElbExpr.h"


BEGIN_NAMESPACE_YM_VERILOG

// @brief コンストラクタ
ElbMgr::ElbMgr() :
  mFactory{ElbFactory::new_obj()}
{
}

// @brief デストラクタ
ElbMgr::~ElbMgr()
{
  clear();
}

// @brief 内容をクリアする．
void
ElbMgr::clear()
{
  for ( auto head: mHeadList ) {
    delete head;
  }
  mHeadList.clear();

  for ( auto obj: mObjList ) {
    delete obj;
  }
  mObjList.clear();

  mUdpList.clear();
  mUdpHash.clear();
  mTopmoduleList.clear();
  mSystfHash.clear();
  mTagDict.clear();
  mAttrHash.clear();
  mTopLevel = nullptr;
}

// @brief UDP 定義のリストを返す．
const vector<const VlUdpDefn*>&
ElbMgr::udp_list() const
{
  return mUdpList;
}

// @brief 名前から UDP 定義を取出す．
// @param[in] name 名前
// @return name という名の UDP を返す．
// @return なければ nullptr を返す．
const VlUdpDefn*
ElbMgr::find_udp(const char* name) const
{
  if ( mUdpHash.count(name) > 0 ) {
    return mUdpHash.at(name);
  }
  else {
    return nullptr;
  }
}

// @brief UDP を登録する．
// @param[in] def_name 定義名
// @param[in] udp 登録する UDP
void
ElbMgr::reg_udp(const char* def_name,
		const VlUdpDefn* udp)
{
  mUdpList.push_back(udp);
  mUdpHash[def_name] = udp;
}

// @brief グローバルスコープを登録する．
void
ElbMgr::reg_toplevel(const VlScope* toplevel)
{
  mTopLevel = toplevel;
}

// @brief topmodule のリストを返す．
const vector<const VlModule*>&
ElbMgr::topmodule_list() const
{
  return mTopmoduleList;
}

// @brief 名前から UserSystf を取出す．
// @param[in] name 名前
// @return name という名のユーザー定義関数を返す．
// @return なければ nullptr を返す．
const VlUserSystf*
ElbMgr::find_user_systf(const char* name) const
{
  if ( mSystfHash.count(name) > 0 ) {
    return mSystfHash.at(name);
  }
  else {
    return nullptr;
  }
}

// @brief システムタスク/システム関数を登録する．
void
ElbMgr::reg_user_systf(const VlUserSystf* systf)
{
  mSystfHash[systf->name()] = systf;
}

// @brief internal scope を登録する．
// @param[in] obj 登録するオブジェクト
void
ElbMgr::reg_internalscope(const VlScope* obj)
{
  mTagDict.add_internalscope(obj);
}

// @brief 宣言要素を登録する．
// @param[in] tag タグ
// @param[in] obj 登録するオブジェクト
void
ElbMgr::reg_decl(int tag,
		 const VlDecl* obj)
{
  if ( tag ) {
    mTagDict.add_decl(tag, obj);
  }
}

// @brief 配列型の宣言要素を登録する．
// @param[in] tag タグ
// @param[in] obj 登録するオブジェクト
void
ElbMgr::reg_declarray(int tag,
		      const VlDeclArray* obj)
{
  if ( tag ) {
    if ( tag == vpiVariables ) {
      // ちょっと汚い補正
      tag += 100;
    }
    mTagDict.add_declarray(tag, obj);
  }
}

// @brief defparam を登録する．
// @param[in] obj 登録するオブジェクト
void
ElbMgr::reg_defparam(const VlDefParam* obj)
{
  mTagDict.add_defparam(obj);
}

// @brief paramassign を登録する．
// @param[in] obj 登録するオブジェクト
void
ElbMgr::reg_paramassign(const VlParamAssign* obj)
{
  mTagDict.add_paramassign(obj);
}

// @brief モジュール配列を登録する．
// @param[in] obj 登録するオブジェクト
void
ElbMgr::reg_modulearray(const VlModuleArray* obj)
{
  mTagDict.add_modulearray(obj);
}

// @brief モジュールを登録する．
void
ElbMgr::reg_module(const VlModule* obj)
{
  mTagDict.add_module(obj);
  if ( obj->parent_scope() == mTopLevel ) {
    mTopmoduleList.push_back(obj);
  }
}

// @brief プリミティブ配列を登録する．
// @param[in] obj 登録するオブジェクト
void
ElbMgr::reg_primarray(const VlPrimArray* obj)
{
  mTagDict.add_primarray(obj);
}

// @brief プリミティブを登録する．
// @param[in] obj 登録するオブジェクト
void
ElbMgr::reg_primitive(const VlPrimitive* obj)
{
  mTagDict.add_primitive(obj);
}

// @brief タスクを登録する．
// @param[in] obj 登録するオブジェクト
void
ElbMgr::reg_task(const VlTaskFunc* obj)
{
  mTagDict.add_task(obj);
}

// @brief 関数を登録する．
// @param[in] obj 登録するオブジェクト
void
ElbMgr::reg_function(const VlTaskFunc* obj)
{
  mTagDict.add_function(obj);
}

// @brief continuous assignment を登録する．
// @param[in] obj 登録するオブジェクト
void
ElbMgr::reg_contassign(const VlContAssign* obj)
{
  mTagDict.add_contassign(obj);
}

// @brief process を登録する．
// @param[in] obj 登録するオブジェクト
void
ElbMgr::reg_process(const VlProcess* obj)
{
  mTagDict.add_process(obj);
}

// @brief 属性リストを登録する．
// @param[in] obj 対象のオブジェクト
// @param[in] def 定義側の属性の時 true とするフラグ
// @param[in] attr_list 属性リスト
void
ElbMgr::reg_attr(const VlObj* obj,
		 bool def,
		 const vector<const VlAttribute*>& attr_list)
{
  mAttrHash.add(obj, def, attr_list);
}

// @brief トップレベルのスコープを生成する
const VlScope*
ElbMgr::new_Toplevel()
{
  auto scope{factory().new_Toplevel()};
  mObjList.push_back(scope);
  return scope;
}

// @brief ステートメントブロックのスコープを生成する．
// @param[in] parent 親のスコープ環境
// @param[in] pt_stmt 対応するパース木の要素
const VlScope*
ElbMgr::new_StmtBlockScope(const VlScope* parent,
			   const PtStmt* pt_stmt)
{
  auto scope{factory().new_StmtBlockScope(parent, pt_stmt)};
  mObjList.push_back(scope);
  return scope;
}

// @brief generate block を生成する．
// @param[in] parent 親のスコープ環境
// @param[in] pt_item 対応するパース木の要素
const VlScope*
ElbMgr::new_GenBlock(const VlScope* parent,
		     const PtItem* pt_item)
{
  auto scope{factory().new_GenBlock(parent, pt_item)};
  mObjList.push_back(scope);
  return scope;
}

// @brief GfBlock 検索用の親の名前付きオブジェクトを作る．
ElbGfRoot*
ElbMgr::new_GfRoot(const VlScope* parent,
		   const PtItem* pt_item)
{
  auto gfroot{factory().new_GfRoot(parent, pt_item)};
  mObjList.push_back(gfroot);
  return gfroot;
}

// @brief generate for block を生成する．
// @param[in] parent 親のスコープ環境
// @param[in] pt_item 対応するパース木の要素
// @param[in] int gvi 対応する genvar の値
const VlScope*
ElbMgr::new_GfBlock(const VlScope* parent,
		    const PtItem* pt_item,
		    int gvi)
{
  auto gfblock{factory().new_GfBlock(parent, pt_item, gvi)};
  mObjList.push_back(gfblock);
  return gfblock;
}

// @brief UDP定義を生成する．
// @param[in] pt_udp パース木の UDP 定義
// @param[in] is_protected プロテクト属性
ElbUdpDefn*
ElbMgr::new_UdpDefn(const PtUdp* pt_udp,
		    bool is_protected)
{
  auto udp{factory().new_UdpDefn(pt_udp, is_protected)};
  mObjList.push_back(udp);
  return udp;
}

// @brief module を生成する．
// @param[in] parent 親のスコープ
// @param[in] pt_module モジュールテンプレート
// @param[in] pt_head パース木のヘッダ定義
// @param[in] pt_inst パース木のインスタンス定義
ElbModule*
ElbMgr::new_Module(const VlScope* parent,
		   const PtModule* pt_module,
		   const PtItem* pt_head,
		   const PtInst* pt_inst)
{
  auto module{factory().new_Module(parent, pt_module, pt_head, pt_inst)};
  mObjList.push_back(module);
  return module;
}

// @brief module array を生成する．
// @param[in] parent 親のスコープ
// @param[in] pt_module モジュールテンプレート
// @param[in] pt_head パース木のヘッダ定義
// @param[in] pt_inst パース木のインスタンス定義
// @param[in] left 範囲の MSB の式
// @param[in] right 範囲の LSB の式
// @param[in] left_val 範囲の MSB の値
// @param[in] right_val 範囲の LSB の値
ElbModuleArray*
ElbMgr::new_ModuleArray(const VlScope* parent,
			const PtModule* pt_module,
			const PtItem* pt_head,
			const PtInst* pt_inst,
			const PtExpr* left,
			const PtExpr* right,
			int left_val,
			int right_val)
{
  auto modulearray{factory().new_ModuleArray(parent, pt_module, pt_head, pt_inst,
					     left, right, left_val, right_val)};
  mObjList.push_back(modulearray);
  return modulearray;
}

// @brief module IO ヘッダを生成する．
// @param[in] module 親のモジュール
// @param[in] pt_header パース木のIO宣言ヘッダ
ElbIOHead*
ElbMgr::new_IOHead(const VlModule* module,
		      const PtIOHead* pt_header)
{
  auto head{factory().new_IOHead(module, pt_header)};
  mHeadList.push_back(head);
  return head;
}

// @brief タスク/関数用の IO ヘッダを生成する．
// @param[in] taskfunc 親のタスク/関数
// @param[in] pt_header パース木のIO宣言ヘッダ
ElbIOHead*
ElbMgr::new_IOHead(const VlTaskFunc* taskfunc,
		   const PtIOHead* pt_header)
{
  auto head{factory().new_IOHead(taskfunc, pt_header)};
  mHeadList.push_back(head);
  return head;
}

// @brief 宣言要素のヘッダを生成する．
// @param[in] parent 親のスコープ
// @param[in] pt_head パース木の宣言ヘッダ
// @param[in] has_delay 遅延値を持つとき true
ElbDeclHead*
ElbMgr::new_DeclHead(const VlScope* parent,
		     const PtDeclHead* pt_head,
		     bool has_delay)
{
  auto head{factory().new_DeclHead(parent, pt_head, has_delay)};
  mHeadList.push_back(head);
  return head;
}

// @brief 宣言要素のヘッダを生成する．
// @param[in] parent 親のスコープ
// @param[in] pt_head パース木の宣言ヘッダ
// @param[in] left 範囲の左側の式
// @param[in] right 範囲の右側の式
// @param[in] left_val 範囲の MSB の値
// @param[in] right_val 範囲の LSB の値
// @param[in] has_delay 遅延値を持つとき true
ElbDeclHead*
ElbMgr::new_DeclHead(const VlScope* parent,
		     const PtDeclHead* pt_head,
		     const PtExpr* left,
		     const PtExpr* right,
		     int left_val,
		     int right_val,
		     bool has_delay)
{
  auto head{factory().new_DeclHead(parent, pt_head, left, right,
				   left_val, right_val, has_delay)};
  mHeadList.push_back(head);
  return head;
}

// @brief 宣言要素のヘッダを生成する．(IODecl 中の宣言用)
// @param[in] parent 親のスコープ
// @param[in] pt_head パース木のIO宣言ヘッダ
// @param[in] aux_type 補助的なデータ型
ElbDeclHead*
ElbMgr::new_DeclHead(const VlScope* parent,
		     const PtIOHead* pt_head,
		     VpiAuxType aux_type)
{
  auto head{factory().new_DeclHead(parent, pt_head, aux_type)};
  mHeadList.push_back(head);
  return head;
}

// @brief 宣言要素のヘッダを生成する．(IODecl 中の宣言用)
// @param[in] parent 親のスコープ
// @param[in] pt_head パース木のIO宣言ヘッダ
// @param[in] aux_type 補助的なデータ型
// @param[in] left 範囲の左側の式
// @param[in] right 範囲の右側の式
// @param[in] left_val 範囲の MSB の値
// @param[in] right_val 範囲の LSB の値
ElbDeclHead*
ElbMgr::new_DeclHead(const VlScope* parent,
		     const PtIOHead* pt_head,
		     VpiAuxType aux_type,
		     const PtExpr* left,
		     const PtExpr* right,
		     int left_val,
		     int right_val)
{
  auto head{factory().new_DeclHead(parent, pt_head, aux_type,
				   left, right, left_val, right_val)};
  mHeadList.push_back(head);
  return head;
}

// @brief 宣言要素のヘッダを生成する．(function の暗黙宣言用)
// @param[in] parent 親のスコープ
// @param[in] pt_item パース木の関数定義
ElbDeclHead*
ElbMgr::new_DeclHead(const VlScope* parent,
		     const PtItem* pt_item)
{
  auto head{factory().new_DeclHead(parent, pt_item)};
  mHeadList.push_back(head);
  return head;
}

// @brief 宣言要素のヘッダを生成する．(function の暗黙宣言用)
// @param[in] parent 親のスコープ
// @param[in] pt_item パース木の関数定義
// @param[in] left 範囲の左側の式
// @param[in] right 範囲の右側の式
// @param[in] left_val 範囲の MSB の値
// @param[in] right_val 範囲の LSB の値
ElbDeclHead*
ElbMgr::new_DeclHead(const VlScope* parent,
		     const PtItem* pt_item,
		     const PtExpr* left,
		     const PtExpr* right,
		     int left_val,
		     int right_val)
{
  auto head{factory().new_DeclHead(parent, pt_item, left, right,
				   left_val, right_val)};
  mHeadList.push_back(head);
  return head;
}

// @brief 宣言要素を生成する．
// @param[in] head ヘッダ
// @param[in] pt_item パース木の宣言要素
// @param[in] init 初期値
ElbDecl*
ElbMgr::new_Decl(ElbDeclHead* head,
		 const PtNamedBase* pt_item,
		 const VlExpr* init)
{
  auto decl{factory().new_Decl(head, pt_item, init)};
  mObjList.push_back(decl);
  return decl;
}

// @brief 暗黙のネットを生成する．
// @param[in] pt_expr パース木のプライマリ式
ElbDecl*
ElbMgr::new_ImpNet(const VlScope* parent,
		   const PtExpr* pt_expr,
		   VpiNetType net_type)
{
  auto decl{factory().new_ImpNet(parent, pt_expr, net_type)};
  mObjList.push_back(decl);
  return decl;
}

// @brief 宣言要素の配列を生成する．
// @param[in] parent 親のスコープ
// @param[in] head ヘッダ
// @param[in] pt_item パース木の宣言要素
// @param[in] range_src 範囲の配列
const VlDeclArray*
ElbMgr::new_DeclArray(ElbDeclHead* head,
		      const PtNamedBase* pt_item,
		      const vector<ElbRangeSrc>& range_src)
{
  auto decl{factory().new_DeclArray(head, pt_item, range_src)};
  mObjList.push_back(decl);
  return decl;
}

// @brief parameter 宣言のヘッダを生成する(範囲指定なし)．
// @param[in] parent 親のスコープ
// @param[in] pt_head パース木の宣言ヘッダ
ElbParamHead*
ElbMgr::new_ParamHead(const VlScope* parent,
		      const PtDeclHead* pt_head)
{
  auto head{factory().new_ParamHead(parent, pt_head)};
  mHeadList.push_back(head);
  return head;
}

// @brief parameter 宣言のヘッダを生成する(範囲指定あり)．
// @param[in] parent 親のスコープ
// @param[in] pt_head パース木の宣言ヘッダ
// @param[in] left 範囲の左側の式
// @param[in] right 範囲の右側の式
// @param[in] left_val 範囲の MSB の値
// @param[in] right_val 範囲の LSB の値
ElbParamHead*
ElbMgr::new_ParamHead(const VlScope* parent,
		      const PtDeclHead* pt_head,
		      const PtExpr* left,
		      const PtExpr* right,
		      int left_val,
		      int right_val)
{
  auto head{factory().new_ParamHead(parent, pt_head, left, right,
				    left_val, right_val)};
  mHeadList.push_back(head);
  return head;
}

// @brief parameter 宣言を生成する．
// @param[in] head ヘッダ
// @param[in] pt_item パース木の宣言要素
// @param[in] init 初期値
// @param[in] is_local localparam の時 true
ElbParameter*
ElbMgr::new_Parameter(ElbParamHead* head,
		      const PtNamedBase* pt_item,
		      bool is_local)
{
  auto param{factory().new_Parameter(head, pt_item, is_local)};
  mObjList.push_back(param);
  return param;
}

// @brief genvar を生成する．
// @param[in] parent 親のスコープ環境
// @param[in] pt_item 対応するパース木の要素
// @param[in] val 初期値
ElbGenvar*
ElbMgr::new_Genvar(const VlScope* parent,
		   const PtDeclItem* pt_item,
		   int val)
{
  auto genvar{factory().new_Genvar(parent, pt_item, val)};
  mObjList.push_back(genvar);
  return genvar;
}

// @brief continuous assignment のヘッダを生成する．
// @param[in] module 親のモジュール
// @param[in] pt_head パース木のヘッダ定義
// @param[in] delay 遅延値
// @note 遅延を持たないときは nullptr を与える．
ElbCaHead*
ElbMgr::new_CaHead(const VlModule* module,
		   const PtItem* pt_head,
		   const VlDelay* delay)
{
  auto head{factory().new_CaHead(module, pt_head, delay)};
  mHeadList.push_back(head);
  return head;
}

// @brief continuous assignment を生成する．
// @param[in] head ヘッダ
// @param[in] pt_obj 対応するパース木中の定義要素
// @param[in] lhs 左辺式
// @param[in] rhs 右辺式
const VlContAssign*
ElbMgr::new_ContAssign(ElbCaHead* head,
		       const PtBase* pt_obj,
		       const VlExpr* lhs,
		       const VlExpr* rhs)
{
  auto contassign{factory().new_ContAssign(head, pt_obj, lhs, rhs)};
  mObjList.push_back(contassign);
  return contassign;
}

// @brief net 宣言中の continuous assignment を生成する．
// @param[in] module 親のモジュール
// @param[in] pt_obj パース木の定義要素
// @param[in] lhs 左辺式
// @param[in] rhs 右辺式
const VlContAssign*
ElbMgr::new_ContAssign(const VlModule* module,
		       const PtBase* pt_obj,
		       const VlExpr* lhs,
		       const VlExpr* rhs)
{
  auto contassign{factory().new_ContAssign(module, pt_obj, lhs, rhs)};
  mObjList.push_back(contassign);
  return contassign;
}

// @brief パラメータ割り当て文を生成する．
// @param[in] module 親のモジュール
// @param[in] param 対象の parameter
// @param[in] rhs_expr 割り当て式の右辺
// @param[in] rhs_value 右辺の値
const VlParamAssign*
ElbMgr::new_ParamAssign(const VlModule* module,
			const PtBase* pt_obj,
			ElbParameter* param,
			const PtExpr* rhs_expr,
			const VlValue& rhs_value)
{
  auto paramassign{factory().new_ParamAssign(module, pt_obj, param,
					     rhs_expr, rhs_value)};
  mObjList.push_back(paramassign);
  return paramassign;
}

// @brief 名前によるパラメータ割り当て文を生成する．
// @param[in] module 親のモジュール
// @param[in] param 対象の parameter
// @param[in] rhs_expr 割り当て式の右辺
// @param[in] rhs_value 右辺の値
const VlParamAssign*
ElbMgr::new_NamedParamAssign(const VlModule* module,
			     const PtBase* pt_obj,
			     ElbParameter* param,
			     const PtExpr* rhs_expr,
			     const VlValue& rhs_value)
{
  auto paramassign{factory().new_NamedParamAssign(module, pt_obj, param,
						  rhs_expr, rhs_value)};
  mObjList.push_back(paramassign);
  return paramassign;
}

// @brief defparam 文を生成する．
// @param[in] module 親のモジュール
// @param[in] pt_header パース木の defparam ヘッダ
// @param[in] pt_item パース木の defparam 定義
// @param[in] param 対象の parameter
// @param[in] rhs_expr 割り当て式の右辺式
// @param[in] rhs_value 右辺の値
const VlDefParam*
ElbMgr::new_DefParam(const VlModule* module,
		     const PtItem* pt_header,
		     const PtDefParam* pt_defparam,
		     ElbParameter* param,
		     const PtExpr* rhs_expr,
		     const VlValue& rhs_value)
{
  auto defparam{factory().new_DefParam(module, pt_header, pt_defparam,
				       param, rhs_expr, rhs_value)};
  mObjList.push_back(defparam);
  return defparam;
}

// @brief ゲートプリミティブのヘッダを生成する．
// @param[in] parent 親のスコープ
// @param[in] pt_header パース木の定義
// @param[in] has_delay 遅延値を持つとき true
ElbPrimHead*
ElbMgr::new_PrimHead(const VlScope* parent,
		     const PtItem* pt_header,
		     bool has_delay)
{
  auto head{factory().new_PrimHead(parent, pt_header, has_delay)};
  mHeadList.push_back(head);
  return head;
}

// @brief UDPプリミティブのヘッダを生成する．
// @param[in] parent 親のスコープ
// @param[in] pt_header パース木の定義
// @param[in] udp UDP 定義
// @param[in] has_delay 遅延値を持つとき true
ElbPrimHead*
ElbMgr::new_UdpHead(const VlScope* parent,
		    const PtItem* pt_header,
		    const VlUdpDefn* udp,
		    bool has_delay)
{
  auto head{factory().new_UdpHead(parent, pt_header, udp, has_delay)};
  mHeadList.push_back(head);
  return head;
}

// @brief セルプリミティブのヘッダを生成する．
// @param[in] parent 親のスコープ
// @param[in] pt_header パース木の定義
// @param[in] cell_id セル番号
ElbPrimHead*
ElbMgr::new_CellHead(const VlScope* parent,
		     const PtItem* pt_header,
		     int cell_id)
{
  auto head{factory().new_CellHead(parent, pt_header, cell_id)};
  mHeadList.push_back(head);
  return head;
}

// @brief プリミティブインスタンスを生成する．
// @param[in] head ヘッダ
// @param[in] pt_inst インスタンス定義
ElbPrimitive*
ElbMgr::new_Primitive(ElbPrimHead* head,
		      const PtInst* pt_inst)
{
  auto prim{factory().new_Primitive(head, pt_inst)};
  mObjList.push_back(prim);
  return prim;
}

// @brief プリミティブ配列インスタンスを生成する．
// @param[in] head ヘッダ
// @param[in] pt_inst インスタンス定義
// @param[in] left 範囲の MSB の式
// @param[in] right 範囲の LSB の式
// @param[in] left_val 範囲の MSB の値
// @param[in] right_val 範囲の LSB の値
ElbPrimArray*
ElbMgr::new_PrimitiveArray(ElbPrimHead* head,
			   const PtInst* pt_inst,
			   const PtExpr* left,
			   const PtExpr* right,
			   int left_val,
			   int right_val)
{
  auto prim{factory().new_PrimitiveArray(head, pt_inst, left, right,
					 left_val, right_val)};
  mObjList.push_back(prim);
  return prim;
}

// @brief セルプリミティブインスタンスを生成する．
// @param[in] head ヘッダ
// @param[in] cell セル
// @param[in] pt_inst インスタンス定義
ElbPrimitive*
ElbMgr::new_CellPrimitive(ElbPrimHead* head,
			  const ClibCell& cell,
			  const PtInst* pt_inst)
{
  auto prim{factory().new_CellPrimitive(head, cell, pt_inst)};
  mObjList.push_back(prim);
  return prim;
}

// @brief セルプリミティブ配列インスタンスを生成する．
// @param[in] head ヘッダ
// @param[in] cell セル
// @param[in] pt_inst インスタンス定義
// @param[in] left 範囲の MSB の式
// @param[in] right 範囲の LSB の式
// @param[in] left_val 範囲の MSB の値
// @param[in] right_val 範囲の LSB の値
ElbPrimArray*
ElbMgr::new_CellPrimitiveArray(ElbPrimHead* head,
			       const ClibCell& cell,
			       const PtInst* pt_inst,
			       const PtExpr* left,
			       const PtExpr* right,
			       int left_val,
			       int right_val)
{
  auto prim{factory().new_CellPrimitiveArray(head, cell, pt_inst,
					     left, right,
					     left_val, right_val)};
  mObjList.push_back(prim);
  return prim;
}

// @brief function を生成する．
// @param[in] parent 親のスコープ
// @param[in] pt_item パース木の定義
// @param[in] const_func 定数関数フラグ
ElbTaskFunc*
ElbMgr::new_Function(const VlScope* parent,
		     const PtItem* pt_item,
		     bool const_func)
{
  auto func{factory().new_Function(parent, pt_item, const_func)};
  mObjList.push_back(func);
  return func;
}

// @brief function を生成する．
// @param[in] parent 親のスコープ
// @param[in] pt_item パース木の定義
// @param[in] left 範囲の MSB の式
// @param[in] right 範囲の LSB の式
// @param[in] left_val 範囲の MSB の値
// @param[in] right_val 範囲の LSB の値
// @param[in] const_func 定数関数フラグ
ElbTaskFunc*
ElbMgr::new_Function(const VlScope* parent,
		     const PtItem* pt_item,
		     const PtExpr* left,
		     const PtExpr* right,
		     int left_val,
		     int right_val,
		     bool const_func)
{
  auto func{factory().new_Function(parent, pt_item,
				   left, right,
				   left_val, right_val,
				   const_func)};
  mObjList.push_back(func);
  return func;
}

// @brief task を生成する．
// @param[in] parent 親のスコープ
// @param[in] pt_item パース木の定義
ElbTaskFunc*
ElbMgr::new_Task(const VlScope* parent,
		 const PtItem* pt_item)
{
  auto task{factory().new_Task(parent, pt_item)};
  mObjList.push_back(task);
  return task;
}

// @brief プロセス文を生成する．
// @param[in] parent 親のスコープ
// @param[in] pt_item パース木の要素定義
ElbProcess*
ElbMgr::new_Process(const VlScope* parent,
		    const PtItem* pt_item)
{
  auto process{factory().new_Process(parent, pt_item)};
  mObjList.push_back(process);
  return process;
}

// @brief 代入文を生成する．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] lhs 左辺の式
// @param[in] rhs 右辺の式
// @param[in] block ブロッキング代入の時 true
// @param[in] control コントロール
const VlStmt*
ElbMgr::new_Assignment(const VlScope* parent,
		       const VlProcess* process,
		       const PtStmt* pt_stmt,
		       const VlExpr* lhs,
		       const VlExpr* rhs,
		       bool block,
		       const VlControl* control)
{
  auto stmt{factory().new_Assignment(parent, process, pt_stmt,
				     lhs, rhs, block, control)};
  mObjList.push_back(stmt);
  return stmt;
}

// @brief assign ステートメントを生成する．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] lhs 左辺の式
// @param[in] rhs 右辺の式
const VlStmt*
ElbMgr::new_AssignStmt(const VlScope* parent,
		       const VlProcess* process,
		       const PtStmt* pt_stmt,
		       const VlExpr* lhs,
		       const VlExpr* rhs)
{
  auto stmt{factory().new_AssignStmt(parent, process, pt_stmt,
				     lhs, rhs)};
  mObjList.push_back(stmt);
  return stmt;
}

// @brief deassign ステートメントを生成する．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] lhs 左辺の式
const VlStmt*
ElbMgr::new_DeassignStmt(const VlScope* parent,
			 const VlProcess* process,
			 const PtStmt* pt_stmt,
			 const VlExpr* lhs)
{
  auto stmt{factory().new_DeassignStmt(parent, process, pt_stmt, lhs)};
  mObjList.push_back(stmt);
  return stmt;
}

// @brief force ステートメントを生成する．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] lhs 左辺の式
// @param[in] rhs 右辺の式
const VlStmt*
ElbMgr::new_ForceStmt(const VlScope* parent,
		     const VlProcess* process,
		     const PtStmt* pt_stmt,
		     const VlExpr* lhs,
		     const VlExpr* rhs)
{
  auto stmt{factory().new_ForceStmt(parent, process, pt_stmt, lhs, rhs)};
  mObjList.push_back(stmt);
  return stmt;
}

// @brief release ステートメントを生成する．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] lhs 左辺の式
const VlStmt*
ElbMgr::new_ReleaseStmt(const VlScope* parent,
			const VlProcess* process,
			const PtStmt* pt_stmt,
			const VlExpr* lhs)
{
  auto stmt{factory().new_ReleaseStmt(parent, process, pt_stmt, lhs)};
  mObjList.push_back(stmt);
  return stmt;
}

// @brief begin ブロックを生成する．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] stmt_list 子のステートメントリスト
const VlStmt*
ElbMgr::new_Begin(const VlScope* parent,
		  const VlProcess* process,
		  const PtStmt* pt_stmt,
		  const vector<const VlStmt*>& stmt_list)
{
  auto stmt{factory().new_Begin(parent, process, pt_stmt, stmt_list)};
  mObjList.push_back(stmt);
  return stmt;
}

// @brief fork ブロックを生成する．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] stmt_list 子のステートメントリスト
const VlStmt*
ElbMgr::new_Fork(const VlScope* parent,
		 const VlProcess* process,
		 const PtStmt* pt_stmt,
		 const vector<const VlStmt*>& stmt_list)
{
  auto stmt{factory().new_Fork(parent, process, pt_stmt, stmt_list)};
  mObjList.push_back(stmt);
  return stmt;
}

// @brief 名前付き begin ブロックを生成する．
// @param[in] block 自分自身に対応するスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] stmt_list 子のステートメントリスト
const VlStmt*
ElbMgr::new_NamedBegin(const VlScope* block,
		       const VlProcess* process,
		       const PtStmt* pt_stmt,
		       const vector<const VlStmt*>& stmt_list)
{
  auto stmt{factory().new_NamedBegin(block, process, pt_stmt, stmt_list)};
  mObjList.push_back(stmt);
  return stmt;
}

// @brief 名前付き fork ブロックを生成する．
// @param[in] block 自分自身に対応するスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] stmt_list 子のステートメントリスト
const VlStmt*
ElbMgr::new_NamedFork(const VlScope* block,
		      const VlProcess* process,
		      const PtStmt* pt_stmt,
		      const vector<const VlStmt*>& stmt_list)
{
  auto stmt{factory().new_NamedFork(block, process, pt_stmt, stmt_list)};
  mObjList.push_back(stmt);
  return stmt;
}

// @brief while 文を生成する．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] cond 条件式
// @param[in] body 本体のステートメント
const VlStmt*
ElbMgr::new_WhileStmt(const VlScope* parent,
		      const VlProcess* process,
		      const PtStmt* pt_stmt,
		      const VlExpr* cond,
		      const VlStmt* body)
{
  auto stmt{factory().new_WhileStmt(parent, process, pt_stmt, cond, body)};
  mObjList.push_back(stmt);
  return stmt;
}

// @brief repeat 文を生成する．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] cond 条件式
// @param[in] body 本体のステートメント
const VlStmt*
ElbMgr::new_RepeatStmt(const VlScope* parent,
		       const VlProcess* process,
		       const PtStmt* pt_stmt,
		       const VlExpr* cond,
		       const VlStmt* body)
{
  auto stmt{factory().new_RepeatStmt(parent, process, pt_stmt, cond, body)};
  mObjList.push_back(stmt);
  return stmt;
}

// @brief wait 文を生成する．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] cond 条件式
// @param[in] body 本体のステートメント
const VlStmt*
ElbMgr::new_WaitStmt(const VlScope* parent,
		     const VlProcess* process,
		     const PtStmt* pt_stmt,
		     const VlExpr* cond,
		     const VlStmt* body)
{
  auto stmt{factory().new_WaitStmt(parent, process, pt_stmt, cond, body)};
  mObjList.push_back(stmt);
  return stmt;
}

// @brief for 文を生成する．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] cond 条件式
// @param[in] init_stmt 初期化式
// @param[in] inc_stmt 増加式
// @param[in] body 本体のステートメント
const VlStmt*
ElbMgr::new_ForStmt(const VlScope* parent,
		    const VlProcess* process,
		    const PtStmt* pt_stmt,
		    const VlExpr* cond,
		    const VlStmt* init_stmt,
		    const VlStmt* inc_stmt,
		    const VlStmt* body)
{
  auto stmt{factory().new_ForStmt(parent, process, pt_stmt, cond,
				  init_stmt, inc_stmt, body)};
  mObjList.push_back(stmt);
  return stmt;
}

// @brief forever 文を生成する．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] body 本体のステートメント
const VlStmt*
ElbMgr::new_ForeverStmt(const VlScope* parent,
			const VlProcess* process,
			const PtStmt* pt_stmt,
			const VlStmt* body)
{
  auto stmt{factory().new_ForeverStmt(parent, process, pt_stmt, body)};
  mObjList.push_back(stmt);
  return stmt;
}

// @brief if 文を生成する．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] cond 条件式
// @param[in] then_stmt 本体のステートメント
// @param[in] else_stmt else節のステートメント
// @note else_stmt は nullptr もありうる．
const VlStmt*
ElbMgr::new_IfStmt(const VlScope* parent,
		   const VlProcess* process,
		   const PtStmt* pt_stmt,
		   const VlExpr* cond,
		   const VlStmt* then_stmt,
		   const VlStmt* else_stmt)
{
  auto stmt{factory().new_IfStmt(parent, process, pt_stmt,
				 cond, then_stmt, else_stmt)};
  mObjList.push_back(stmt);
  return stmt;
}

// @brief case 文を生成する．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] expr 条件式
// @param[in] caseitem_list caseitem のリスト
const VlStmt*
ElbMgr::new_CaseStmt(const VlScope* parent,
		     const VlProcess* process,
		     const PtStmt* pt_stmt,
		     const VlExpr* expr,
		     const vector<const VlCaseItem*>& caseitem_list)
{
  auto stmt{factory().new_CaseStmt(parent, process, pt_stmt,
				   expr, caseitem_list)};
  mObjList.push_back(stmt);
  return stmt;
}

// @brief caseitem を生成する．
// @param[in] pt_item パース木の caseitem 要素
// @param[in] label_list ラベルを表す式のリスト
// @param[in] body 本体のステートメント
const VlCaseItem*
ElbMgr::new_CaseItem(const PtCaseItem* pt_item,
		     const vector<ElbExpr*>& label_list,
		     const VlStmt* body)
{
  auto caseitem{factory().new_CaseItem(pt_item, label_list, body)};
  mObjList.push_back(caseitem);
  return caseitem;
}

// @brief イベント文を生成する．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] named_event 対象のイベント
const VlStmt*
ElbMgr::new_EventStmt(const VlScope* parent,
		      const VlProcess* process,
		      const PtStmt* pt_stmt,
		      ElbExpr* named_event)
{
  auto stmt{factory().new_EventStmt(parent, process, pt_stmt,
				    named_event)};
  mObjList.push_back(stmt);
  return stmt;
}

// @brief nullptr ステートメントを生成する．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] pt_stmt パース木のステートメント定義
const VlStmt*
ElbMgr::new_NullStmt(const VlScope* parent,
		     const VlProcess* process,
		     const PtStmt* pt_stmt)
{
  auto stmt{factory().new_NullStmt(parent, process, pt_stmt)};
  mObjList.push_back(stmt);
  return stmt;
}

// @brief タスクコール文を生成する．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] task 対象のタスク
// @param[in] arg_array 引数の配列
const VlStmt*
ElbMgr::new_TaskCall(const VlScope* parent,
		     const VlProcess* process,
		     const PtStmt* pt_stmt,
		     const VlTaskFunc* task,
		     const vector<ElbExpr*>& arg_array)
{
  auto stmt{factory().new_TaskCall(parent, process, pt_stmt,
				   task, arg_array)};
  mObjList.push_back(stmt);
  return stmt;
}

// @brief システムタスクコール文を生成する．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] user_systf システムタスク
// @param[in] arg_array 引数の配列
const VlStmt*
ElbMgr::new_SysTaskCall(const VlScope* parent,
			const VlProcess* process,
			const PtStmt* pt_stmt,
			const VlUserSystf* user_systf,
			const vector<ElbExpr*>& arg_array)
{
  auto stmt{factory().new_SysTaskCall(parent, process, pt_stmt,
				      user_systf, arg_array)};
  mObjList.push_back(stmt);
  return stmt;
}

// @brief disable 文を生成する．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] target 対象のスコープ
const VlStmt*
ElbMgr::new_DisableStmt(const VlScope* parent,
			const VlProcess* process,
			const PtStmt* pt_stmt,
			const VlScope* target)
{
  auto stmt{factory().new_DisableStmt(parent, process, pt_stmt, target)};
  mObjList.push_back(stmt);
  return stmt;
}

// @brief コントロール文を生成する．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or nullptr)
// @param[in] pt_stmt パース木のステートメント定義
// @param[in] control コントロール
// @param[in] body 本体のステートメント
const VlStmt*
ElbMgr::new_CtrlStmt(const VlScope* parent,
		     const VlProcess* process,
		     const PtStmt* pt_stmt,
		     const VlControl* control,
		     const VlStmt* body)
{
  auto stmt{factory().new_CtrlStmt(parent, process, pt_stmt,
				   control, body)};
  mObjList.push_back(stmt);
  return stmt;
}

// @brief 遅延コントロールを生成する．
// @param[in] pt_control パース木の定義要素
// @param[in] delay 遅延式
const VlControl*
ElbMgr::new_DelayControl(const PtControl* pt_control,
			 ElbExpr* delay)
{
  auto control{factory().new_DelayControl(pt_control, delay)};
  mObjList.push_back(control);
  return control;
}

// @brief イベントコントロールを生成する．
// @param[in] pt_control パース木の定義要素
// @param[in] event_list イベントリストを表す配列
const VlControl*
ElbMgr::new_EventControl(const PtControl* pt_control,
			 const vector<ElbExpr*>& event_list)
{
  auto control{factory().new_EventControl(pt_control, event_list)};
  mObjList.push_back(control);
  return control;
}

// @brief リピートコントロールを生成する．
// @param[in] pt_control パース木の定義要素
// @param[in] rep 繰り返し式
// @param[in] event_list イベントリストを表す配列
const VlControl*
ElbMgr::new_RepeatControl(const PtControl* pt_control,
			  ElbExpr* rep,
			  const vector<ElbExpr*>& event_list)
{
  auto control{factory().new_RepeatControl(pt_control, rep, event_list)};
  mObjList.push_back(control);
  return control;
}

// @brief 単項演算子を生成する．
// @param[in] pt_expr パース木の定義要素
// @param[in] op_type 演算子のタイプ
// @param[in] opr1 オペランド
ElbExpr*
ElbMgr::new_UnaryOp(const PtExpr* pt_expr,
		    VpiOpType op_type,
		    ElbExpr* opr1)
{
  auto expr{factory().new_UnaryOp(pt_expr, op_type,
				  opr1)};
  mObjList.push_back(expr);
  return expr;
}

// @brief 2項演算子を生成する．
// @param[in] pt_expr パース木の定義要素
// @param[in] op_type 演算子のタイプ
// @param[in] opr1 オペランド
// @param[in] opr2 オペランド
ElbExpr*
ElbMgr::new_BinaryOp(const PtExpr* pt_expr,
		     VpiOpType op_type,
		     ElbExpr* opr1,
		     ElbExpr* opr2)
{
  auto expr{factory().new_BinaryOp(pt_expr, op_type,
				   opr1, opr2)};
  mObjList.push_back(expr);
  return expr;
}

// @brief 3項演算子を生成する．
// @param[in] pt_expr パース木の定義要素
// @param[in] op_type 演算子のタイプ
// @param[in] opr1 オペランド
// @param[in] opr2 オペランド
// @param[in] opr3 オペランド
ElbExpr*
ElbMgr::new_TernaryOp(const PtExpr* pt_expr,
		      VpiOpType op_type,
		      ElbExpr* opr1,
		      ElbExpr* opr2,
		      ElbExpr* opr3)
{
  auto expr{factory().new_TernaryOp(pt_expr, op_type,
				    opr1, opr2, opr3)};
  mObjList.push_back(expr);
  return expr;
}

// @brief 連結演算子を生成する．
// @param[in] pt_expr パース木の定義要素
// @param[in] opr_list オペランドのリスト
ElbExpr*
ElbMgr::new_ConcatOp(const PtExpr* pt_expr,
		     const vector<ElbExpr*>& opr_list)
{
  auto expr{factory().new_ConcatOp(pt_expr, opr_list)};
  mObjList.push_back(expr);
  return expr;
}

// @brief 反復連結演算子を生成する．
// @param[in] pt_expr パース木の定義要素
// @param[in] rep_expr 繰り返し数を表す式
// @param[in] rep_num 繰り返し数
// @param[in] opr_list オペランドのリスト
ElbExpr*
ElbMgr::new_MultiConcatOp(const PtExpr* pt_expr,
			  SizeType rep_num,
			  ElbExpr* rep_expr,
			  const vector<ElbExpr*>& opr_list)
{
  auto expr{factory().new_MultiConcatOp(pt_expr,
					rep_num, rep_expr,
					opr_list)};
  mObjList.push_back(expr);
  return expr;
}

// @brief プライマリ式を生成する．
// @param[in] pt_expr パース木の定義要素
// @param[in] obj 本体のオブジェクト
ElbExpr*
ElbMgr::new_Primary(const PtExpr* pt_expr,
		    const VlDecl* obj)
{
  auto expr{factory().new_Primary(pt_expr, obj)};
  mObjList.push_back(expr);
  return expr;
}

// @brief プライマリ式を生成する(net decl の初期値用)．
// @param[in] pt_item パース木の定義要素
// @param[in] obj 本体のオブジェクト
// @param[in] index_list インデックスのリスト
ElbExpr*
ElbMgr::new_Primary(const PtDeclItem* pt_item,
		    const VlDecl* obj)
{
  auto expr{factory().new_Primary(pt_item, obj)};
  mObjList.push_back(expr);
  return expr;
}

// @brief プライマリ式を生成する．
// @param[in] pt_expr パース木の定義要素
// @param[in] obj 本体のオブジェクト
ElbExpr*
ElbMgr::new_Primary(const PtExpr* pt_expr,
		    ElbParameter* obj)
{
  auto expr{factory().new_Primary(pt_expr, obj)};
  mObjList.push_back(expr);
  return expr;
}

// @brief プライマリ式を生成する(配列要素版)．
// @param[in] pt_expr パース木の定義要素
// @param[in] obj 本体のオブジェクト
// @param[in] index_list インデックスのリスト
ElbExpr*
ElbMgr::new_Primary(const PtExpr* pt_expr,
		    const VlDeclArray* obj,
		    const vector<ElbExpr*>& index_list)
{
  auto expr{factory().new_Primary(pt_expr, obj, index_list)};
  mObjList.push_back(expr);
  return expr;
}

// @brief プライマリ式を生成する(固定インデックスの配列要素版)．
// @param[in] pt_expr パース木の定義要素
// @param[in] obj 本体のオブジェクト
// @param[in] offset オフセット
ElbExpr*
ElbMgr::new_Primary(const PtExpr* pt_expr,
		    const VlDeclArray* obj,
		    int offset)
{
  auto expr{factory().new_Primary(pt_expr, obj, offset)};
  mObjList.push_back(expr);
  return expr;
}

// @brief 固定ビット選択式を生成する．
// @param[in] pt_expr パース木の定義要素
// @param[in] base 本体のオブジェクト
// @param[in] bit_index ビット選択式
// @param[in] bit_index_val ビット選択式の値
ElbExpr*
ElbMgr::new_BitSelect(const PtExpr* pt_expr,
		      ElbExpr* base,
		      const PtExpr* bit_index,
		      int bit_index_val)
{
  auto expr{factory().new_BitSelect(pt_expr, base,
				    bit_index, bit_index_val)};
  mObjList.push_back(expr);
  return expr;
}

// @brief 固定ビット選択式を生成する．
// @param[in] pt_expr パース木の定義要素
// @param[in] base 本体の式
// @param[in] bit_index_val ビット選択式の値
ElbExpr*
ElbMgr::new_BitSelect(const PtExpr* pt_expr,
		      ElbExpr* base,
		      int bit_index_val)
{
  auto expr{factory().new_BitSelect(pt_expr, base, bit_index_val)};
  mObjList.push_back(expr);
  return expr;
}

// @brief 可変ビット選択式を生成する．
// @param[in] pt_expr パース木の定義要素
// @param[in] base 本体のオブジェクト
// @param[in] bit_index ビット選択式
ElbExpr*
ElbMgr::new_BitSelect(const PtExpr* pt_expr,
		      ElbExpr* base,
		      ElbExpr* bit_index)
{
  auto expr{factory().new_BitSelect(pt_expr, base, bit_index)};
  mObjList.push_back(expr);
  return expr;
}

// @brief 固定部分選択式を生成する．
// @param[in] pt_expr パース木の定義要素
// @param[in] obj 本体のオブジェクト
// @param[in] index1, index2 パート選択式
// @param[in] index1_val, index2_val パート選択式の値
ElbExpr*
ElbMgr::new_PartSelect(const PtExpr* pt_expr,
		       ElbExpr* obj,
		       const PtExpr* index1,
		       const PtExpr* index2,
		       int index1_val,
		       int index2_val)
{
  auto expr{factory().new_PartSelect(pt_expr, obj,
				     index1, index2,
				     index1_val, index2_val)};
  mObjList.push_back(expr);
  return expr;
}

// @brief 固定部分選択式を生成する．
// @param[in] pt_expr パース木の定義要素
// @param[in] base 本体の式
// @param[in] index1, inde2 パート選択式
ElbExpr*
ElbMgr::new_PartSelect(const PtExpr* pt_expr,
		       ElbExpr* base,
		       int index1,
		       int index2)
{
  auto expr{factory().new_PartSelect(pt_expr, base,
				     index1, index2)};
  mObjList.push_back(expr);
  return expr;
}

// @brief 可変部分選択式を生成する．
// @param[in] pt_expr パース木の定義要素
// @param[in] obj 本体のオブジェクト
// @param[in] base 範囲のベースアドレスを表す式
// @param[in] range 範囲を表す式
// @param[in] range_val 範囲の値
ElbExpr*
ElbMgr::new_PlusPartSelect(const PtExpr* pt_expr,
			   ElbExpr* obj,
			   ElbExpr* base,
			   const PtExpr* range_expr,
			   int range_val)
{
  auto expr{factory().new_PlusPartSelect(pt_expr, obj, base,
					 range_expr, range_val)};
  mObjList.push_back(expr);
  return expr;
}

// @brief 可変部分選択式を生成する．
// @param[in] pt_expr パース木の定義要素
// @param[in] obj 本体のオブジェクト
// @param[in] base 範囲のベースアドレスを表す式
// @param[in] range 範囲を表す式
// @param[in] range_val 範囲の値
ElbExpr*
ElbMgr::new_MinusPartSelect(const PtExpr* pt_expr,
			    ElbExpr* obj,
			    ElbExpr* base,
			    const PtExpr* range_expr,
			    int range_val)
{
  auto expr{factory().new_MinusPartSelect(pt_expr, obj, base,
					  range_expr, range_val)};
  mObjList.push_back(expr);
  return expr;
}

// @brief 定数式を生成する．
// @param[in] pt_expr パース木の定義要素
ElbExpr*
ElbMgr::new_Constant(const PtExpr* pt_expr)
{
  auto expr{factory().new_Constant(pt_expr)};
  mObjList.push_back(expr);
  return expr;
}

// @brief genvar 起因の定数式を生成する．
// @param[in] pt_primary パース木の定義要素
// @param[in] val 値
ElbExpr*
ElbMgr::new_GenvarConstant(const PtExpr* pt_primary,
			   int val)
{
  auto expr{factory().new_GenvarConstant(pt_primary, val)};
  mObjList.push_back(expr);
  return expr;
}

// @brief 関数呼び出し式を生成する．
// @param[in] pt_expr パース木の定義要素
// @param[in] func 関数
// @param[in] arg_list 引数のリスト
ElbExpr*
ElbMgr::new_FuncCall(const PtExpr* pt_expr,
		     const VlTaskFunc* func,
		     const vector<ElbExpr*>& arg_list)
{
  auto expr{factory().new_FuncCall(pt_expr, func, arg_list)};
  mObjList.push_back(expr);
  return expr;
}

// @brief システム関数呼び出し式を生成する．
// @param[in] pt_expr パース木の定義要素
// @param[in] user_systf システム関数
// @param[in] arg_list 引数のリスト
ElbExpr*
ElbMgr::new_SysFuncCall(const PtExpr* pt_expr,
			const VlUserSystf* user_systf,
			const vector<ElbExpr*>& arg_list)
{
  auto expr{factory().new_SysFuncCall(pt_expr, user_systf,
				      arg_list)};
  mObjList.push_back(expr);
  return expr;
}

// @brief システム関数/システムタスクの引数を生成する．
// @param[in] pt_expr パース木中で参照している要素
// @param[in] arg 引数本体
ElbExpr*
ElbMgr::new_ArgHandle(const PtExpr* pt_expr,
		      const VlScope* arg)
{
  auto expr{factory().new_ArgHandle(pt_expr, arg)};
  mObjList.push_back(expr);
  return expr;
}

// @brief システム関数/システムタスクの引数を生成する．
// @param[in] pt_expr パース木中で参照している要素
// @param[in] arg 引数本体
ElbExpr*
ElbMgr::new_ArgHandle(const PtExpr* pt_expr,
		      const VlPrimitive* arg)
{
  auto expr{factory().new_ArgHandle(pt_expr, arg)};
  mObjList.push_back(expr);
  return expr;
}

// @brief システム関数/システムタスクの引数を生成する．
// @param[in] pt_expr パース木中で参照している要素
// @param[in] arg 引数本体
ElbExpr*
ElbMgr::new_ArgHandle(const PtExpr* pt_expr,
		      const VlDeclArray* arg)
{
  auto expr{factory().new_ArgHandle(pt_expr, arg)};
  mObjList.push_back(expr);
  return expr;
}

// @brief 連結演算子の左辺式を生成する．
// @param[in] pt_expr パース木の定義要素
// @param[in] opr_array オペランドを格納する配列
// @param[in] lhs_elem_array 左辺の要素の配列
ElbExpr*
ElbMgr::new_Lhs(const PtExpr* pt_expr,
		const vector<ElbExpr*>& opr_array,
		const vector<ElbExpr*>& lhs_elem_array)
{
  auto expr{factory().new_Lhs(pt_expr, opr_array, lhs_elem_array)};
  mObjList.push_back(expr);
  return expr;
}

// @brief 遅延値を生成する．
// @param[in] pt_obj パース木の定義要素
// @param[in] expr_list 式の配列
const VlDelay*
ElbMgr::new_Delay(const PtBase* pt_obj,
		  const vector<ElbExpr*>& expr_list)
{
  auto delay{factory().new_Delay(pt_obj, expr_list)};
  mObjList.push_back(delay);
  return delay;
}

END_NAMESPACE_YM_VERILOG
