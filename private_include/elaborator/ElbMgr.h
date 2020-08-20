#ifndef ELBMGR_H
#define ELBMGR_H

/// @file ElbMgr.h
/// @brief ElbMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.


#include "ym/pt/PtP.h"
#include "ym/vl/VlFwd.h"
#include "ym/clib.h"

#include "TagDict.h"
#include "AttrHash.h"

#include "parser/PtiFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ElbMgr ElbMgr.h "ElbMgr.h"
/// @brief ElbMgr の実装クラス
//////////////////////////////////////////////////////////////////////
class ElbMgr
{
public:

  /// @brief コンストラクタ
  ElbMgr();

  /// @brief デストラクタ
  ~ElbMgr();


public:

  /// @brief 内容をクリアする．
  void
  clear();


public:
  //////////////////////////////////////////////////////////////////////
  // 要素を検索する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief UDP 定義のリストを返す．
  const vector<const VlUdpDefn*>&
  udp_list() const;

  /// @brief 名前から UDP 定義を取出す．
  /// @param[in] name 名前
  /// @return name という名の UDP を返す．
  /// @return なければ nullptr を返す．
  const VlUdpDefn*
  find_udp(const char* name) const;

  /// @brief topmodule のリストを返す．
  const vector<const VlModule*>&
  topmodule_list() const;

  /// @brief 名前から UserSystf を取出す．
  /// @param[in] name 名前
  /// @return name という名のユーザー定義関数を返す．
  /// @return なければ nullptr を返す．
  const VlUserSystf*
  find_user_systf(const char* name) const;

  /// @brief スコープに属する internal scope のリストを取り出す．
  /// @param[in] parent 検索対象のスコープ
  /// @return 結果のリストを返す．
  vector<const VlScope*>
  find_internalscope_list(const VlScope* parent) const;

  /// @brief スコープとタグから宣言要素を取り出す．
  /// @param[in] parent 検索対象のスコープ
  /// @param[in] tag タグ
  /// @return 結果のリストを返す．
  ///
  /// parent のスコープ内の tag というタグを持つ要素のリストを返す．
  vector<const VlDecl*>
  find_decl_list(const VlScope* parent,
		 int tag) const;

  /// @brief スコープとタグから宣言要素の配列を取り出す．
  /// @param[in] parent 検索対象のスコープ
  /// @param[in] tag タグ
  /// @retrun 結果のリストを返す．
  ///
  /// parent というスコープ内の tag というタグを持つ要素のリストを返す．
  vector<const VlDeclArray*>
  find_declarray_list(const VlScope* parent,
		      int tag) const;

  /// @brief スコープに属する defparam のリストを取り出す．
  /// @param[in] parent 検索対象のスコープ
  /// @return 結果のリストを返す．
  vector<const VlDefParam*>
  find_defparam_list(const VlScope* parent) const;

  /// @brief スコープに属する param assign のリストを取り出す．
  /// @param[in] parent 検索対象のスコープ
  /// @return 結果のリストを返す．
  vector<const VlParamAssign*>
  find_paramassign_list(const VlScope* parent) const;

  /// @brief スコープに属する module のリストを取り出す．
  /// @param[in] parent 検索対象のスコープ
  /// @return 結果のリストを返す．
  vector<const VlModule*>
  find_module_list(const VlScope* parent) const;

  /// @brief スコープに属する module arrayのリストを取り出す．
  /// @param[in] parent 検索対象のスコープ
  /// @return 結果のリストを返す．
  vector<const VlModuleArray*>
  find_modulearray_list(const VlScope* parent) const;

  /// @brief スコープに属する primitive のリストを取り出す．
  /// @param[in] parent 検索対象のスコープ
  /// @return 結果のリストを返す．
  vector<const VlPrimitive*>
  find_primitive_list(const VlScope* parent) const;

  /// @brief スコープに属する primitive array のリストを取り出す．
  /// @param[in] parent 検索対象のスコープ
  /// @return 結果のリストを返す．
  vector<const VlPrimArray*>
  find_primarray_list(const VlScope* parent) const;

  /// @brief スコープに属する continuous assignment のリストを取り出す．
  /// @param[in] parent 検索対象のスコープ
  /// @return 結果のリストを返す．
  vector<const VlContAssign*>
  find_contassign_list(const VlScope* parent) const;

  /// @brief スコープに属するタスクのリストを取り出す．
  /// @param[in] parent 検索対象のスコープ
  /// @return 結果のリストを返す．
  vector<const VlTaskFunc*>
  find_task_list(const VlScope* parent) const;

  /// @brief スコープに属する関数のリストを取り出す．
  /// @param[in] parent 検索対象のスコープ
  /// @return 結果のリストを返す．
  vector<const VlTaskFunc*>
  find_function_list(const VlScope* parent) const;

  /// @brief スコープに属する process のリストを取り出す．
  /// @param[in] parent 検索対象のスコープ
  /// @return 結果のリストを返す．
  vector<const VlProcess*>
  find_process_list(const VlScope* parent) const;


public:
  //////////////////////////////////////////////////////////////////////
  // 要素を登録する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief システムタスク/システム関数を登録する．
  /// @param[in] systf 登録するシステムタスク/システム関数
  void
  reg_user_systf(const VlUserSystf* systf);

  /// @brief UDP を登録する．
  /// @param[in] def_name 定義名
  /// @param[in] udp 登録する UDP
  void
  reg_udp(const char* def_name,
	  const VlUdpDefn* udp);

  /// @brief グローバルスコープを登録する．
  void
  reg_toplevel(const VlScope* toplevel);

  /// @brief internal scope を登録する．
  /// @param[in] obj 登録するオブジェクト
  void
  reg_internalscope(const VlScope* obj);

  /// @brief 宣言要素を登録する．
  /// @param[in] tag タグ
  /// @param[in] obj 登録するオブジェクト
  void
  reg_decl(int tag,
	   const VlDecl* obj);

  /// @brief 宣言要素を登録する．
  /// @param[in] tag タグ
  /// @param[in] obj 登録するオブジェクト
  void
  reg_declarray(int tag,
		const VlDeclArray* obj);

  /// @brief defparam を登録する．
  /// @param[in] obj 登録するオブジェクト
  void
  reg_defparam(const VlDefParam* obj);

  /// @brief paramassign を登録する．
  /// @param[in] obj 登録するオブジェクト
  void
  reg_paramassign(const VlParamAssign* obj);

  /// @brief モジュール配列を登録する．
  /// @param[in] obj 登録するオブジェクト
  void
  reg_modulearray(const VlModuleArray* obj);

  /// @brief モジュールを登録する．
  /// @param[in] module 登録するモジュール
  void
  reg_module(const VlModule* module);

  /// @brief プリミティブ配列を登録する．
  /// @param[in] obj 登録するオブジェクト
  void
  reg_primarray(const VlPrimArray* obj);

  /// @brief プリミティブを登録する．
  /// @param[in] obj 登録するオブジェクト
  void
  reg_primitive(const VlPrimitive* obj);

  /// @brief タスクを登録する．
  /// @param[in] obj 登録するオブジェクト
  void
  reg_task(const VlTaskFunc* obj);

  /// @brief 関数を登録する．
  /// @param[in] obj 登録するオブジェクト
  void
  reg_function(const VlTaskFunc* obj);

  /// @brief continuous assignment を登録する．
  /// @param[in] obj 登録するオブジェクト
  void
  reg_contassign(const VlContAssign* obj);

  /// @brief process を登録する．
  /// @param[in] obj 登録するオブジェクト
  void
  reg_process(const VlProcess* obj);


public:
  //////////////////////////////////////////////////////////////////////
  // 属性の検索/登録
  //////////////////////////////////////////////////////////////////////

  /// @brief 属性リストを得る．
  /// @param[in] obj 対象のオブジェクト
  /// @param[in] def 定義側の属性の時 true とするフラグ
  vector<const VlAttribute*>
  find_attr(const VlObj* obj,
	    bool def) const;

  /// @brief 属性リストを登録する．
  /// @param[in] obj 対象のオブジェクト
  /// @param[in] def 定義側の属性の時 true とするフラグ
  /// @param[in] attr_list 属性リスト
  void
  reg_attr(const VlObj* obj,
	   bool def,
	   const vector<const VlAttribute*>& attr_list);


public:
  //////////////////////////////////////////////////////////////////////
  // インスタンス生成関係のメソッド
  //////////////////////////////////////////////////////////////////////

  /// @brief トップレベルのスコープを生成する
  const VlScope*
  new_Toplevel();

  /// @brief generate block を生成する．
  /// @param[in] parent 親のスコープ環境
  /// @param[in] pt_item 対応するパース木の要素
  const VlScope*
  new_GenBlock(const VlScope* parent,
	       const PtItem* pt_item);

  /// @brief GfBlock 検索用の親の名前付きオブジェクトを作る．
  ElbGfRoot*
  new_GfRoot(const VlScope* parent,
	     const PtItem* pt_item);

  /// @brief generate for block を生成する．
  /// @param[in] parent 親のスコープ環境
  /// @param[in] pt_item 対応するパース木の要素
  /// @param[in] int gvi 対応する genvar の値
  const VlScope*
  new_GfBlock(const VlScope* parent,
	      const PtItem* pt_item,
	      int gvi);

  /// @brief UDP定義を生成する．
  /// @param[in] pt_udp パース木の UDP 定義
  /// @param[in] is_protected プロテクト属性
  ElbUdpDefn*
  new_UdpDefn(const PtUdp* pt_udp,
	      bool is_protected);

  /// @brief module を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_module モジュールテンプレート
  /// @param[in] pt_head パース木のヘッダ定義
  /// @param[in] pt_inst パース木のインスタンス定義
  ElbModule*
  new_Module(const VlScope* parent,
	     const PtModule* pt_module,
	     const PtItem* pt_head,
	     const PtInst* pt_inst);

  /// @brief module array を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_module モジュールテンプレート
  /// @param[in] pt_head パース木のヘッダ定義
  /// @param[in] pt_inst パース木のインスタンス定義
  /// @param[in] left 範囲の MSB の式
  /// @param[in] right 範囲の LSB の式
  /// @param[in] left_val 範囲の MSB の値
  /// @param[in] right_val 範囲の LSB の値
  const VlModuleArray*
  new_ModuleArray(const VlScope* parent,
		  const PtModule* pt_module,
		  const PtItem* pt_head,
		  const PtInst* pt_inst,
		  const PtExpr* left,
		  const PtExpr* right,
		  int left_val,
		  int right_val);

  /// @brief module IO ヘッダを生成する．
  /// @param[in] module 親のモジュール
  /// @param[in] pt_header パース木のIO宣言ヘッダ
  ElbIOHead*
  new_ModIOHead(const VlModule* module,
		const PtIOHead* pt_header);

  /// @brief タスク用の IO ヘッダを生成する．
  /// @param[in] task 親のタスク
  /// @param[in] pt_header パース木のIO宣言ヘッダ
  ElbIOHead*
  new_TaskIOHead(const VlTaskFunc* task,
		 const PtIOHead* pt_header);

  /// @brief 関数用の IO ヘッダを生成する．
  /// @param[in] func 親の関数
  /// @param[in] pt_header パース木のIO宣言ヘッダ
  ElbIOHead*
  new_FunctionIOHead(const VlTaskFunc* func,
		     const PtIOHead* pt_header);

  /// @brief 宣言要素のヘッダを生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_head パース木の宣言ヘッダ
  /// @param[in] has_delay 遅延値を持つとき true
  ElbDeclHead*
  new_DeclHead(const VlScope* parent,
	       const PtDeclHead* pt_head,
	       bool has_delay = false);

  /// @brief 宣言要素のヘッダを生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_head パース木の宣言ヘッダ
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @param[in] left_val 範囲の MSB の値
  /// @param[in] right_val 範囲の LSB の値
  /// @param[in] has_delay 遅延値を持つとき true
  ElbDeclHead*
  new_DeclHead(const VlScope* parent,
	       const PtDeclHead* pt_head,
	       const PtExpr* left,
	       const PtExpr* right,
	       int left_val,
	       int right_val,
	       bool has_delay = false);

  /// @brief 宣言要素のヘッダを生成する．(IODecl 中の宣言用)
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_head パース木のIO宣言ヘッダ
  /// @param[in] aux_type 補助的なデータ型
  ElbDeclHead*
  new_DeclHead(const VlScope* parent,
	       const PtIOHead* pt_head,
	       VpiAuxType aux_type);

  /// @brief 宣言要素のヘッダを生成する．(IODecl 中の宣言用)
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_head パース木のIO宣言ヘッダ
  /// @param[in] aux_type 補助的なデータ型
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @param[in] left_val 範囲の MSB の値
  /// @param[in] right_val 範囲の LSB の値
  ElbDeclHead*
  new_DeclHead(const VlScope* parent,
	       const PtIOHead* pt_head,
	       VpiAuxType aux_type,
	       const PtExpr* left,
	       const PtExpr* right,
	       int left_val,
	       int right_val);

  /// @brief 宣言要素のヘッダを生成する．(function の暗黙宣言用)
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_item パース木の関数定義
  ElbDeclHead*
  new_DeclHead(const VlScope* parent,
	       const PtItem* pt_item);

  /// @brief 宣言要素のヘッダを生成する．(function の暗黙宣言用)
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_item パース木の関数定義
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @param[in] left_val 範囲の MSB の値
  /// @param[in] right_val 範囲の LSB の値
  ElbDeclHead*
  new_DeclHead(const VlScope* parent,
	       const PtItem* pt_item,
	       const PtExpr* left,
	       const PtExpr* right,
	       int left_val,
	       int right_val);

  /// @brief 宣言要素を生成する．
  /// @param[in] head ヘッダ
  /// @param[in] pt_item パース木の宣言要素
  /// @param[in] init 初期値
  ElbDecl*
  new_Decl(ElbDeclHead* head,
	   const PtNamedBase* pt_item,
	   const VlExpr* init = nullptr);

  /// @brief 暗黙のネットを生成する．
  /// @param[in] pt_expr パース木のプライマリ式
  ElbDecl*
  new_ImpNet(const VlScope* parent,
	     const PtExpr* pt_expr,
	     VpiNetType net_type);

  /// @brief 宣言要素の配列を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] head ヘッダ
  /// @param[in] pt_item パース木の宣言要素
  /// @param[in] range_src 範囲の配列
  const VlDeclArray*
  new_DeclArray(ElbDeclHead* head,
		const PtNamedBase* pt_item,
		const vector<ElbRangeSrc>& range_src);

  /// @brief parameter 宣言のヘッダを生成する(範囲指定なし)．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_head パース木の宣言ヘッダ
  ElbParamHead*
  new_ParamHead(const VlScope* parent,
		const PtDeclHead* pt_head);

  /// @brief parameter 宣言のヘッダを生成する(範囲指定あり)．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_head パース木の宣言ヘッダ
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @param[in] left_val 範囲の MSB の値
  /// @param[in] right_val 範囲の LSB の値
  ElbParamHead*
  new_ParamHead(const VlScope* parent,
		const PtDeclHead* pt_head,
		const PtExpr* left,
		const PtExpr* right,
		int left_val,
		int right_val);

  /// @brief parameter 宣言を生成する．
  /// @param[in] head ヘッダ
  /// @param[in] pt_item パース木の宣言要素
  /// @param[in] init 初期値
  /// @param[in] is_local localparam の時 true
  ElbParameter*
  new_Parameter(ElbParamHead* head,
		const PtNamedBase* pt_item,
		bool is_local = false);

  /// @brief genvar を生成する．
  /// @param[in] parent 親のスコープ環境
  /// @param[in] pt_item 対応するパース木の要素
  /// @param[in] val 初期値
  ElbGenvar*
  new_Genvar(const VlScope* parent,
	     const PtDeclItem* pt_item,
	     int val);

  /// @brief continuous assignment のヘッダを生成する．
  /// @param[in] module 親のモジュール
  /// @param[in] pt_head パース木のヘッダ定義
  /// @param[in] delay 遅延値
  /// @note 遅延を持たないときは nullptr を与える．
  ElbCaHead*
  new_CaHead(const VlModule* module,
	     const PtItem* pt_head,
	     const VlDelay* delay = nullptr);

  /// @brief continuous assignment を生成する．
  /// @param[in] head ヘッダ
  /// @param[in] pt_obj 対応するパース木中の定義要素
  /// @param[in] lhs 左辺式
  /// @param[in] rhs 右辺式
  const VlContAssign*
  new_ContAssign(ElbCaHead* head,
		 const PtBase* pt_obj,
		 const VlExpr* lhs,
		 const VlExpr* rhs);

  /// @brief net 宣言中の continuous assignment を生成する．
  /// @param[in] module 親のモジュール
  /// @param[in] pt_obj パース木の定義要素
  /// @param[in] lhs 左辺式
  /// @param[in] rhs 右辺式
  const VlContAssign*
  new_ContAssign(const VlModule* module,
		 const PtBase* pt_obj,
		 const VlExpr* lhs,
		 const VlExpr* rhs);

  /// @brief パラメータ割り当て文を生成する．
  /// @param[in] module 親のモジュール
  /// @param[in] param 対象の parameter
  /// @param[in] rhs_expr 割り当て式の右辺
  /// @param[in] rhs_value 右辺の値
  const VlParamAssign*
  new_ParamAssign(const VlModule* module,
		  const PtBase* pt_obj,
		  ElbParameter* param,
		  const PtExpr* rhs_expr,
		  const VlValue& rhs_value);

  /// @brief 名前によるパラメータ割り当て文を生成する．
  /// @param[in] module 親のモジュール
  /// @param[in] param 対象の parameter
  /// @param[in] rhs_expr 割り当て式の右辺
  /// @param[in] rhs_value 右辺の値
  const VlParamAssign*
  new_NamedParamAssign(const VlModule* module,
		       const PtBase* pt_obj,
		       ElbParameter* param,
		       const PtExpr* rhs_expr,
		       const VlValue& rhs_value);

  /// @brief defparam 文を生成する．
  /// @param[in] module 親のモジュール
  /// @param[in] pt_header パース木の defparam ヘッダ
  /// @param[in] pt_item パース木の defparam 定義
  /// @param[in] param 対象の parameter
  /// @param[in] rhs_expr 割り当て式の右辺式
  /// @param[in] rhs_value 右辺の値
  const VlDefParam*
  new_DefParam(const VlModule* module,
	       const PtItem* pt_header,
	       const PtDefParam* pt_defparam,
	       ElbParameter* param,
	       const PtExpr* rhs_expr,
	       const VlValue& rhs_value);

  /// @brief ゲートプリミティブのヘッダを生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_header パース木の定義
  /// @param[in] has_delay 遅延値を持つとき true
  ElbPrimHead*
  new_PrimHead(const VlScope* parent,
	       const PtItem* pt_header,
	       bool has_delay);

  /// @brief UDPプリミティブのヘッダを生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_header パース木の定義
  /// @param[in] udp UDP 定義
  /// @param[in] has_delay 遅延値を持つとき true
  ElbPrimHead*
  new_UdpHead(const VlScope* parent,
	      const PtItem* pt_header,
	      const VlUdpDefn* udp,
	      bool has_delay);

  /// @brief セルプリミティブのヘッダを生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_header パース木の定義
  /// @param[in] cell_id セル番号
  ElbPrimHead*
  new_CellHead(const VlScope* parent,
	       const PtItem* pt_header,
	       int cell_id);

  /// @brief プリミティブインスタンスを生成する．
  /// @param[in] head ヘッダ
  /// @param[in] pt_inst インスタンス定義
  ElbPrimitive*
  new_Primitive(ElbPrimHead* head,
		const PtInst* pt_inst);

  /// @brief プリミティブ配列インスタンスを生成する．
  /// @param[in] head ヘッダ
  /// @param[in] pt_inst インスタンス定義
  /// @param[in] left 範囲の MSB の式
  /// @param[in] right 範囲の LSB の式
  /// @param[in] left_val 範囲の MSB の値
  /// @param[in] right_val 範囲の LSB の値
  ElbPrimArray*
  new_PrimitiveArray(ElbPrimHead* head,
		     const PtInst* pt_inst,
		     const PtExpr* left,
		     const PtExpr* right,
		     int left_val,
		     int right_val);

  /// @brief セルプリミティブインスタンスを生成する．
  /// @param[in] head ヘッダ
  /// @param[in] cell セル
  /// @param[in] pt_inst インスタンス定義
  ElbPrimitive*
  new_CellPrimitive(ElbPrimHead* head,
		    const ClibCell& cell,
		    const PtInst* pt_inst);

  /// @brief セルプリミティブ配列インスタンスを生成する．
  /// @param[in] head ヘッダ
  /// @param[in] cell セル
  /// @param[in] pt_inst インスタンス定義
  /// @param[in] left 範囲の MSB の式
  /// @param[in] right 範囲の LSB の式
  /// @param[in] left_val 範囲の MSB の値
  /// @param[in] right_val 範囲の LSB の値
  ElbPrimArray*
  new_CellPrimitiveArray(ElbPrimHead* head,
			 const ClibCell& cell,
			 const PtInst* pt_inst,
			 const PtExpr* left,
			 const PtExpr* right,
			 int left_val,
			 int right_val);

  /// @brief function を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_item パース木の定義
  /// @param[in] const_func 定数関数フラグ
  ElbTaskFunc*
  new_Function(const VlScope* parent,
	       const PtItem* pt_item,
	       bool const_func);

  /// @brief function を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_item パース木の定義
  /// @param[in] left 範囲の MSB の式
  /// @param[in] right 範囲の LSB の式
  /// @param[in] left_val 範囲の MSB の値
  /// @param[in] right_val 範囲の LSB の値
  /// @param[in] const_func 定数関数フラグ
  ElbTaskFunc*
  new_Function(const VlScope* parent,
	       const PtItem* pt_item,
	       const PtExpr* left,
	       const PtExpr* right,
	       int left_val,
	       int right_val,
	       bool const_func);

  /// @brief task を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_item パース木の定義
  ElbTaskFunc*
  new_Task(const VlScope* parent,
	   const PtItem* pt_item);

  /// @brief プロセス文を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_item パース木の要素定義
  ElbProcess*
  new_Process(const VlScope* parent,
	      const PtItem* pt_item);

  /// @brief 代入文を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] lhs 左辺の式
  /// @param[in] rhs 右辺の式
  /// @param[in] block ブロッキング代入の時 true
  /// @param[in] control コントロール
  const VlStmt*
  new_Assignment(const VlScope* parent,
		 const VlProcess* process,
		 const PtStmt* pt_stmt,
		 const VlExpr* lhs,
		 const VlExpr* rhs,
		 bool block,
		 const VlControl* control = nullptr);

  /// @brief assign ステートメントを生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] lhs 左辺の式
  /// @param[in] rhs 右辺の式
  const VlStmt*
  new_AssignStmt(const VlScope* parent,
		 const VlProcess* process,
		 const PtStmt* pt_stmt,
		 const VlExpr* lhs,
		 const VlExpr* rhs);

  /// @brief deassign ステートメントを生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] lhs 左辺の式
  const VlStmt*
  new_DeassignStmt(const VlScope* parent,
		   const VlProcess* process,
		   const PtStmt* pt_stmt,
		   const VlExpr* lhs);

  /// @brief force ステートメントを生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] lhs 左辺の式
  /// @param[in] rhs 右辺の式
  const VlStmt*
  new_ForceStmt(const VlScope* parent,
		const VlProcess* process,
		const PtStmt* pt_stmt,
		const VlExpr* lhs,
		const VlExpr* rhs);

  /// @brief release ステートメントを生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] lhs 左辺の式
  const VlStmt*
  new_ReleaseStmt(const VlScope* parent,
		  const VlProcess* process,
		  const PtStmt* pt_stmt,
		  const VlExpr* lhs);

  /// @brief begin ブロックを生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] stmt_list 子のステートメントリスト
  const VlStmt*
  new_Begin(const VlScope* parent,
	    const VlProcess* process,
	    const PtStmt* pt_stmt,
	    const vector<const VlStmt*>& stmt_list);

  /// @brief fork ブロックを生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] stmt_list 子のステートメントリスト
  const VlStmt*
  new_Fork(const VlScope* parent,
	   const VlProcess* process,
	   const PtStmt* pt_stmt,
	   const vector<const VlStmt*>& stmt_list);

  /// @breif statement block を生成する．
  /// @param[in] parent 親のスコープ環境
  /// @param[in] pt_stmt 対応するパース木の要素
  const VlNamedObj*
  new_StmtScope(const VlScope* parent,
		const PtStmt* pt_stmt);

  /// @brief 名前付き begin ブロックを生成する．
  /// @param[in] block 自分自身に対応するスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] stmt_list 子のステートメントリスト
  const VlStmt*
  new_NamedBegin(const VlNamedObj* block,
		 const VlProcess* process,
		 const PtStmt* pt_stmt,
		 const vector<const VlStmt*>& stmt_list);


  /// @brief 名前付き fork ブロックを生成する．
  /// @param[in] block 自分自身に対応するスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] stmt_list 子のステートメントリスト
  const VlStmt*
  new_NamedFork(const VlNamedObj* block,
		const VlProcess* process,
		const PtStmt* pt_stmt,
		const vector<const VlStmt*>& stmt_list);

  /// @brief while 文を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] cond 条件式
  /// @param[in] stmt 本体のステートメント
  const VlStmt*
  new_WhileStmt(const VlScope* parent,
		const VlProcess* process,
		const PtStmt* pt_stmt,
		const VlExpr* cond,
		const VlStmt* stmt);

  /// @brief repeat 文を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] cond 条件式
  /// @param[in] stmt 本体のステートメント
  const VlStmt*
  new_RepeatStmt(const VlScope* parent,
		const VlProcess* process,
		const PtStmt* pt_stmt,
		const VlExpr* cond,
		const VlStmt* stmt);

  /// @brief wait 文を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] cond 条件式
  /// @param[in] stmt 本体のステートメント
  const VlStmt*
  new_WaitStmt(const VlScope* parent,
	       const VlProcess* process,
	       const PtStmt* pt_stmt,
	       const VlExpr* cond,
	       const VlStmt* stmt);

  /// @brief for 文を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] cond 条件式
  /// @param[in] init_stmt 初期化式
  /// @param[in] inc_stmt 増加式
  /// @param[in] stmt 本体のステートメント
  const VlStmt*
  new_ForStmt(const VlScope* parent,
	      const VlProcess* process,
	      const PtStmt* pt_stmt,
	      const VlExpr* cond,
	      const VlStmt* init_stmt,
	      const VlStmt* inc_stmt,
	      const VlStmt* stmt);

  /// @brief forever 文を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] stmt 本体のステートメント
  const VlStmt*
  new_ForeverStmt(const VlScope* parent,
		  const VlProcess* process,
		  const PtStmt* pt_stmt,
		  const VlStmt* stmt);

  /// @brief if 文を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] cond 条件式
  /// @param[in] stmt 本体のステートメント
  /// @param[in] else_stmt else節のステートメント
  /// @note else_stmt は nullptr もありうる．
  const VlStmt*
  new_IfStmt(const VlScope* parent,
	     const VlProcess* process,
	     const PtStmt* pt_stmt,
	     const VlExpr* cond,
	     const VlStmt* stmt,
	     const VlStmt* else_stmt);

  /// @brief case 文を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] expr 条件式
  /// @param[in] caseitem_list caseitem のリスト
  const VlStmt*
  new_CaseStmt(const VlScope* parent,
	       const VlProcess* process,
	       const PtStmt* pt_stmt,
	       const VlExpr* expr,
	       const vector<const VlCaseItem*>& caseitem_list);

  /// @brief caseitem を生成する．
  /// @param[in] pt_item パース木の caseitem 要素
  /// @param[in] label_list ラベルを表す式のリスト
  /// @param[in] body 本体のステートメント
  const VlCaseItem*
  new_CaseItem(const PtCaseItem* pt_item,
	       const vector<ElbExpr*>& label_list,
	       const VlStmt* body);

  /// @brief イベント文を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] named_event 対象のイベント
  const VlStmt*
  new_EventStmt(const VlScope* parent,
		const VlProcess* process,
		const PtStmt* pt_stmt,
		ElbExpr* named_event);

  /// @brief nullptr ステートメントを生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  const VlStmt*
  new_NullStmt(const VlScope* parent,
	       const VlProcess* process,
	       const PtStmt* pt_stmt);

  /// @brief タスクコール文を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] task 対象のタスク
  /// @param[in] arg_array 引数の配列
  const VlStmt*
  new_TaskCall(const VlScope* parent,
	       const VlProcess* process,
	       const PtStmt* pt_stmt,
	       const VlTaskFunc* task,
	       const vector<ElbExpr*>& arg_array);

  /// @brief システムタスクコール文を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] user_systf システムタスク
  /// @param[in] arg_array 引数の配列
  const VlStmt*
  new_SysTaskCall(const VlScope* parent,
		  const VlProcess* process,
		  const PtStmt* pt_stmt,
		  const VlUserSystf* user_systf,
		  const vector<ElbExpr*>& arg_array);

  /// @brief disable 文を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] target 対象のスコープ
  const VlStmt*
  new_DisableStmt(const VlScope* parent,
		  const VlProcess* process,
		  const PtStmt* pt_stmt,
		  const VlNamedObj* target);

  /// @brief コントロール文を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] control コントロール
  /// @param[in] stmt 本体のステートメント
  const VlStmt*
  new_CtrlStmt(const VlScope* parent,
	       const VlProcess* process,
	       const PtStmt* pt_stmt,
	       const VlControl* control,
	       const VlStmt* stmt);

  /// @brief 遅延コントロールを生成する．
  /// @param[in] pt_control パース木の定義要素
  /// @param[in] delay 遅延式
  const VlControl*
  new_DelayControl(const PtControl* pt_control,
		   ElbExpr* delay);

  /// @brief イベントコントロールを生成する．
  /// @param[in] pt_control パース木の定義要素
  /// @param[in] event_list イベントリストを表す配列
  const VlControl*
  new_EventControl(const PtControl* pt_control,
		   const vector<ElbExpr*>& event_list);

  /// @brief リピートコントロールを生成する．
  /// @param[in] pt_control パース木の定義要素
  /// @param[in] rep 繰り返し式
  /// @param[in] event_list イベントリストを表す配列
  const VlControl*
  new_RepeatControl(const PtControl* pt_control,
		    ElbExpr* rep,
		    const vector<ElbExpr*>& event_list);

  /// @brief 単項演算子を生成する．
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] op_type 演算子のタイプ
  /// @param[in] opr1 オペランド
  ElbExpr*
  new_UnaryOp(const PtExpr* pt_expr,
	      VpiOpType op_type,
	      ElbExpr* opr1);

  /// @brief 2項演算子を生成する．
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] op_type 演算子のタイプ
  /// @param[in] opr1 オペランド
  /// @param[in] opr2 オペランド
  ElbExpr*
  new_BinaryOp(const PtExpr* pt_expr,
	       VpiOpType op_type,
	       ElbExpr* opr1,
	       ElbExpr* opr2);

  /// @brief 3項演算子を生成する．
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] op_type 演算子のタイプ
  /// @param[in] opr1 オペランド
  /// @param[in] opr2 オペランド
  /// @param[in] opr3 オペランド
  ElbExpr*
  new_TernaryOp(const PtExpr* pt_expr,
		VpiOpType op_type,
		ElbExpr* opr1,
		ElbExpr* opr2,
		ElbExpr* opr3);

  /// @brief 連結演算子を生成する．
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] opr_list オペランドのリスト
  ElbExpr*
  new_ConcatOp(const PtExpr* pt_expr,
	       const vector<ElbExpr*>& opr_list);

  /// @brief 反復連結演算子を生成する．
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] rep_expr 繰り返し数を表す式
  /// @param[in] rep_num 繰り返し数
  /// @param[in] opr_list オペランドのリスト
  ElbExpr*
  new_MultiConcatOp(const PtExpr* pt_expr,
		    SizeType rep_num,
		    ElbExpr* rep_expr,
		    const vector<ElbExpr*>& opr_list);

  /// @brief プライマリ式を生成する．
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] obj 本体のオブジェクト
  ElbExpr*
  new_Primary(const PtExpr* pt_expr,
	      const VlDecl* obj);

  /// @brief プライマリ式を生成する(net decl の初期値用)．
  /// @param[in] pt_item パース木の定義要素
  /// @param[in] obj 本体のオブジェクト
  /// @param[in] index_list インデックスのリスト
  ElbExpr*
  new_Primary(const PtDeclItem* pt_item,
	      const VlDecl* obj);

  /// @brief プライマリ式を生成する．
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] obj 本体のオブジェクト
  ElbExpr*
  new_Primary(const PtExpr* pt_expr,
	      ElbParameter* obj);

  /// @brief プライマリ式を生成する(配列要素版)．
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] obj 本体のオブジェクト
  /// @param[in] index_list インデックスのリスト
  ElbExpr*
  new_Primary(const PtExpr* pt_expr,
	      const VlDeclArray* obj,
	      const vector<ElbExpr*>& index_list);

  /// @brief プライマリ式を生成する(固定インデックスの配列要素版)．
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] obj 本体のオブジェクト
  /// @param[in] offset オフセット
  ElbExpr*
  new_Primary(const PtExpr* pt_expr,
	      const VlDeclArray* obj,
	      int offset);

  /// @brief 固定ビット選択式を生成する．
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] expr 本体のオブジェクト
  /// @param[in] bit_index ビット選択式
  /// @param[in] bit_index_val ビット選択式の値
  ElbExpr*
  new_BitSelect(const PtExpr* pt_expr,
		ElbExpr* expr,
		const PtExpr* bit_index,
		int bit_index_val);

  /// @brief 固定ビット選択式を生成する．
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] expr 本体の式
  /// @param[in] bit_index_val ビット選択式の値
  ElbExpr*
  new_BitSelect(const PtExpr* pt_expr,
		ElbExpr* expr,
		int bit_index_val);

  /// @brief 可変ビット選択式を生成する．
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] expr 本体のオブジェクト
  /// @param[in] bit_index ビット選択式
  ElbExpr*
  new_BitSelect(const PtExpr* pt_expr,
		ElbExpr* expr,
		ElbExpr* bit_index);

  /// @brief 固定部分選択式を生成する．
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] obj 本体のオブジェクト
  /// @param[in] index1, index2 パート選択式
  /// @param[in] index1_val, index2_val パート選択式の値
  ElbExpr*
  new_PartSelect(const PtExpr* pt_expr,
		 ElbExpr* obj,
		 const PtExpr* index1,
		 const PtExpr* index2,
		 int index1_val,
		 int index2_val);

  /// @brief 固定部分選択式を生成する．
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] expr 本体の式
  /// @param[in] index1, inde2 パート選択式
  ElbExpr*
  new_PartSelect(const PtExpr* pt_expr,
		 ElbExpr* expr,
		 int index1,
		 int index2);

  /// @brief 可変部分選択式を生成する．
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] obj 本体のオブジェクト
  /// @param[in] base 範囲のベースアドレスを表す式
  /// @param[in] range 範囲を表す式
  /// @param[in] range_val 範囲の値
  ElbExpr*
  new_PlusPartSelect(const PtExpr* pt_expr,
		     ElbExpr* obj,
		     ElbExpr* base,
		     const PtExpr* range_expr,
		     int range_val);

  /// @brief 可変部分選択式を生成する．
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] obj 本体のオブジェクト
  /// @param[in] base 範囲のベースアドレスを表す式
  /// @param[in] range 範囲を表す式
  /// @param[in] range_val 範囲の値
  ElbExpr*
  new_MinusPartSelect(const PtExpr* pt_expr,
		      ElbExpr* obj,
		      ElbExpr* base,
		      const PtExpr* range_expr,
		      int range_val);

  /// @brief 定数式を生成する．
  /// @param[in] pt_expr パース木の定義要素
  ElbExpr*
  new_Constant(const PtExpr* pt_expr);

  /// @brief genvar 起因の定数式を生成する．
  /// @param[in] pt_primary パース木の定義要素
  /// @param[in] val 値
  ElbExpr*
  new_GenvarConstant(const PtExpr* pt_primary,
		     int val);

  /// @brief 関数呼び出し式を生成する．
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] func 関数
  /// @param[in] arg_list 引数のリスト
  ElbExpr*
  new_FuncCall(const PtExpr* pt_expr,
	       const VlTaskFunc* func,
	       const vector<ElbExpr*>& arg_list);

  /// @brief システム関数呼び出し式を生成する．
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] user_systf システム関数
  /// @param[in] arg_list 引数のリスト
  ElbExpr*
  new_SysFuncCall(const PtExpr* pt_expr,
		  const VlUserSystf* user_systf,
		  const vector<ElbExpr*>& arg_list);

  /// @brief システム関数/システムタスクの引数を生成する．
  /// @param[in] pt_expr パース木中で参照している要素
  /// @param[in] arg 引数本体
  ElbExpr*
  new_ArgHandle(const PtExpr* pt_expr,
		const VlNamedObj* arg);

  /// @brief システム関数/システムタスクの引数を生成する．
  /// @param[in] pt_expr パース木中で参照している要素
  /// @param[in] arg 引数本体
  ElbExpr*
  new_ArgHandle(const PtExpr* pt_expr,
		ElbPrimitive* arg);

  /// @brief 連結演算子の左辺式を生成する．
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] opr_array オペランドを格納する配列
  /// @param[in] lhs_elem_array 左辺の要素の配列
  ElbExpr*
  new_Lhs(const PtExpr* pt_expr,
	  const vector<ElbExpr*>& opr_array,
	  const vector<ElbExpr*>& lhs_elem_array);

  /// @brief 遅延値を生成する．
  /// @param[in] pt_obj パース木の定義要素
  /// @param[in] expr_list 式の配列
  const VlDelay*
  new_Delay(const PtBase* pt_obj,
	    const vector<ElbExpr*>& expr_list);

  /// @brief attribute instance のリストを生成する．
  /// @param[in] n 要素数
  ElbAttrList*
  new_AttrList(SizeType n);


public:
  //////////////////////////////////////////////////////////////////////
  // その他の関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // UDP のリスト
  vector<const VlUdpDefn*> mUdpList;

  // UDP の辞書
  unordered_map<string, const VlUdpDefn*> mUdpHash;

  // topmodule のリスト
  vector<const VlModule*> mTopmoduleList;

  // UserSystf の辞書
  unordered_map<string, const VlUserSystf*> mSystfHash;

  // 全てのオブジェクトのリスト
  vector<const VlObj*> mObjList;

  // タグをキーにした各スコープごとのオブジェクトのリストの辞書
  TagDict mTagDict;

  // 属性リストの辞書
  AttrHash mAttrHash;

  // トップレベルスコープ
  const VlScope* mTopLevel;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief スコープに属する internal scope のリストを取り出す．
// @param[in] parent 検索対象のスコープ
// @return 結果のリストを返す．
inline
vector<const VlScope*>
ElbMgr::find_internalscope_list(const VlScope* parent) const
{
  return mTagDict.find_internalscope_list(parent);
}

// @brief スコープとタグから宣言要素を取り出す．
// @param[in] parent 検索対象のスコープ
// @param[in] tag タグ
// @return 結果のリストを返す．
//
// parent のスコープ内の tag というタグを持つ要素のリストを返す．
inline
vector<const VlDecl*>
ElbMgr::find_decl_list(const VlScope* parent,
		       int tag) const
{
  return mTagDict.find_decl_list(parent, tag);
}

// @brief スコープとタグから宣言要素の配列を取り出す．
// @param[in] parent 検索対象のスコープ
// @param[in] tag タグ
// @retrun 結果のリストを返す．
//
// parent というスコープ内の tag というタグを持つ要素のリストを返す．
inline
vector<const VlDeclArray*>
ElbMgr::find_declarray_list(const VlScope* parent,
			    int tag) const
{
  if ( tag == vpiVariables ) {
    // ちょっと汚い補正
    tag += 100;
  }
  return mTagDict.find_declarray_list(parent, tag);
}

// @brief スコープに属する defparam のリストを取り出す．
// @param[in] parent 検索対象のスコープ
// @return 結果のリストを返す．
inline
vector<const VlDefParam*>
ElbMgr::find_defparam_list(const VlScope* parent) const
{
  return mTagDict.find_defparam_list(parent);
}

// @brief スコープに属する param assign のリストを取り出す．
// @param[in] parent 検索対象のスコープ
// @return 結果のリストを返す．
inline
vector<const VlParamAssign*>
ElbMgr::find_paramassign_list(const VlScope* parent) const
{
  return mTagDict.find_paramassign_list(parent);
}

// @brief スコープに属する module のリストを取り出す．
// @param[in] parent 検索対象のスコープ
// @return 結果のリストを返す．
inline
vector<const VlModule*>
ElbMgr::find_module_list(const VlScope* parent) const
{
  return mTagDict.find_module_list(parent);
}

// @brief スコープに属する module arrayのリストを取り出す．
// @param[in] parent 検索対象のスコープ
// @return 結果のリストを返す．
inline
vector<const VlModuleArray*>
ElbMgr::find_modulearray_list(const VlScope* parent) const
{
  return mTagDict.find_modulearray_list(parent);
}

// @brief スコープに属する primitive のリストを取り出す．
// @param[in] parent 検索対象のスコープ
// @return 結果のリストを返す．
inline
vector<const VlPrimitive*>
ElbMgr::find_primitive_list(const VlScope* parent) const
{
  return mTagDict.find_primitive_list(parent);
}

// @brief スコープに属する primitive array のリストを取り出す．
// @param[in] parent 検索対象のスコープ
// @return 結果のリストを返す．
inline
vector<const VlPrimArray*>
ElbMgr::find_primarray_list(const VlScope* parent) const
{
  return mTagDict.find_primarray_list(parent);
}

// @brief スコープに属する continuous assignment のリストを取り出す．
// @param[in] parent 検索対象のスコープ
// @return 結果のリストを返す．
inline
vector<const VlContAssign*>
ElbMgr::find_contassign_list(const VlScope* parent) const
{
  return mTagDict.find_contassign_list(parent);
}

// @brief スコープに属するタスクのリストを取り出す．
// @param[in] parent 検索対象のスコープ
// @return 結果のリストを返す．
inline
vector<const VlTaskFunc*>
ElbMgr::find_task_list(const VlScope* parent) const
{
  return mTagDict.find_task_list(parent);
}

// @brief スコープに属する関数のリストを取り出す．
// @param[in] parent 検索対象のスコープ
// @return 結果のリストを返す．
inline
vector<const VlTaskFunc*>
ElbMgr::find_function_list(const VlScope* parent) const
{
  return mTagDict.find_function_list(parent);
}

// @brief スコープに属する process のリストを取り出す．
// @param[in] parent 検索対象のスコープ
// @return 結果のリストを返す．
inline
vector<const VlProcess*>
ElbMgr::find_process_list(const VlScope* parent) const
{
  return mTagDict.find_process_list(parent);
}

// @brief 属性リストを得る．
// @param[in] obj 対象のオブジェクト
// @param[in] def 定義側の属性の時 true とするフラグ
inline
vector<const VlAttribute*>
ElbMgr::find_attr(const VlObj* obj,
		  bool def) const
{
  return mAttrHash.find(obj, def);
}

END_NAMESPACE_YM_VERILOG

#endif // ELBMGR_H
