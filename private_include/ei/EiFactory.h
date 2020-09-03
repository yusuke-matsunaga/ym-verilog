#ifndef EIFACTORY_H
#define EIFACTORY_H

/// @file EiFactory.h
/// @brief EiFactory のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.


#include "elaborator/ElbFactory.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// EiObj の派生クラスを生成するファクトリクラス
//////////////////////////////////////////////////////////////////////
class EiFactory :
  public ElbFactory
{
public:

  /// @brief コンストラクタ
  EiFactory();

  /// @brief デストラクタ
  ~EiFactory();


public:
  //////////////////////////////////////////////////////////////////////
  // 管理用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内訳を表示する．
  void
  dump_prof(ostream& s) override;


public:
  //////////////////////////////////////////////////////////////////////
  // インスタンス生成関係のメソッド
  //////////////////////////////////////////////////////////////////////

  /// @brief トップレベルのスコープを生成する
  const VlScope*
  new_Toplevel() override;

  /// @brief ステートメントブロックのスコープを生成する．
  /// @param[in] parent 親のスコープ環境
  /// @param[in] pt_stmt 対応するパース木の要素
  const VlScope*
  new_StmtBlockScope(const VlScope* parent,
		     const PtStmt* pt_stmt) override;

  /// @brief generate block を生成する．
  /// @param[in] parent 親のスコープ環境
  /// @param[in] pt_item 対応するパース木の要素
  const VlScope*
  new_GenBlock(const VlScope* parent,
	       const PtItem* pt_item) override;

  /// @brief GfBlock 検索用の親の名前付きオブジェクトを作る．
  ElbGfRoot*
  new_GfRoot(const VlScope* parent,
	     const PtItem* pt_item) override;

  /// @brief generate for block を生成する．
  /// @param[in] parent 親のスコープ環境
  /// @param[in] pt_item 対応するパース木の要素
  /// @param[in] int gvi 対応する genvar の値
  const VlScope*
  new_GfBlock(const VlScope* parent,
	      const PtItem* pt_item,
	      int gvi) override;

  /// @brief UDP定義を生成する．
  /// @param[in] pt_udp パース木の UDP 定義
  /// @param[in] is_protected プロテクト属性
  ElbUdpDefn*
  new_UdpDefn(const PtUdp* pt_udp,
	      bool is_protected) override;

  /// @brief module を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_module モジュールテンプレート
  /// @param[in] pt_head パース木のヘッダ定義
  /// @param[in] pt_inst パース木のインスタンス定義
  ElbModule*
  new_Module(const VlScope* parent,
	     const PtModule* pt_module,
	     const PtItem* pt_head,
	     const PtInst* pt_inst) override;

  /// @brief module array を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_module モジュールテンプレート
  /// @param[in] pt_head パース木のヘッダ定義
  /// @param[in] pt_inst パース木のインスタンス定義
  /// @param[in] left 範囲の MSB の式
  /// @param[in] right 範囲の LSB の式
  /// @param[in] left_val 範囲の MSB の値
  /// @param[in] right_val 範囲の LSB の値
  ElbModuleArray*
  new_ModuleArray(const VlScope* parent,
		  const PtModule* pt_module,
		  const PtItem* pt_head,
		  const PtInst* pt_inst,
		  const PtExpr* left,
		  const PtExpr* right,
		  int left_val,
		  int right_val) override;

  /// @brief module IO ヘッダを生成する．
  /// @param[in] module 親のモジュール
  /// @param[in] pt_header パース木のIO宣言ヘッダ
  ElbIOHead*
  new_IOHead(const VlModule* module,
	     const PtIOHead* pt_header) override;

  /// @brief タスク/関数 IO ヘッダを生成する．
  /// @param[in] taskfunc 親のタスク/関数
  /// @param[in] pt_header パース木のIO宣言ヘッダ
  ElbIOHead*
  new_IOHead(const VlTaskFunc* taskfunc,
	     const PtIOHead* pt_header) override;

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
	       bool has_delay = false) override;

  /// @brief 宣言要素のヘッダを生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_head パース木の宣言ヘッダ
  /// @param[in] has_delay 遅延値を持つとき true
  ElbDeclHead*
  new_DeclHead(const VlScope* parent,
	       const PtDeclHead* pt_head,
	       bool has_delay = false) override;

  /// @brief 宣言要素のヘッダを生成する．(IODecl 中の宣言用)
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_head パース木のIO宣言ヘッダ
  /// @param[in] aux_type 補助的なデータ型
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @param[in] left_val 範囲の MSB の値
  /// @param[in] right_val 範囲の LSB の値
  /// @note 範囲なしの時には left と right に nullptr を入れる．
  ElbDeclHead*
  new_DeclHead(const VlScope* parent,
	       const PtIOHead* pt_head,
	       VpiAuxType aux_type,
	       const PtExpr* left,
	       const PtExpr* right,
	       int left_val,
	       int right_val) override;

  /// @brief 宣言要素のヘッダを生成する．(IODecl 中の宣言用)
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_head パース木のIO宣言ヘッダ
  /// @param[in] aux_type 補助的なデータ型
  ElbDeclHead*
  new_DeclHead(const VlScope* parent,
	       const PtIOHead* pt_head,
	       VpiAuxType aux_type) override;

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
	       int right_val) override;

  /// @brief 宣言要素のヘッダを生成する．(function の暗黙宣言用)
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_item パース木の関数定義
  ElbDeclHead*
  new_DeclHead(const VlScope* parent,
	       const PtItem* pt_item) override;

  /// @brief 宣言要素を生成する．
  /// @param[in] head ヘッダ
  /// @param[in] pt_item パース木の宣言要素
  /// @param[in] init 初期値
  /// @param[in] is_local localparam の時 true
  ElbDecl*
  new_Decl(ElbDeclHead* head,
	   const PtNamedBase* pt_item,
	   const VlExpr* init = nullptr) override;

  /// @brief 暗黙のネットを生成する．
  /// @param[in] pt_expr パース木のプライマリ式
  ElbDecl*
  new_ImpNet(const VlScope* parent,
	     const PtExpr* pt_expr,
	     VpiNetType net_type) override;

  /// @brief 宣言要素の配列を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] head ヘッダ
  /// @param[in] pt_item パース木の宣言要素
  /// @param[in] range_src 範囲の配列
  const VlDeclArray*
  new_DeclArray(ElbDeclHead* head,
		const PtNamedBase* pt_item,
		const vector<ElbRangeSrc>& range_src) override;

  /// @brief parameter 宣言のヘッダを生成する(範囲指定なし)．
  /// @param[in] parent 親のスコープ
  ElbParamHead*
  new_ParamHead(const VlScope* parent,
		const PtDeclHead* pt_head) override;

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
		int right_val) override;

  /// @brief parameter 宣言を生成する．
  /// @param[in] head ヘッダ
  /// @param[in] pt_item パース木の宣言要素
  /// @param[in] init 初期値
  /// @param[in] is_local localparam の時 true
  ElbParameter*
  new_Parameter(ElbParamHead* head,
		const PtNamedBase* pt_item,
		bool is_local = false) override;

  /// @brief genvar を生成する．
  /// @param[in] parent 親のスコープ環境
  /// @param[in] pt_item 対応するパース木の要素
  /// @param[in] val 初期値
  ElbGenvar*
  new_Genvar(const VlScope* parent,
	     const PtDeclItem* pt_item,
	     int val) override;

  /// @brief continuous assignment のヘッダを生成する．
  /// @param[in] module 親のモジュール
  /// @param[in] pt_head パース木のヘッダ定義
  /// @param[in] delay 遅延値
  /// @note 遅延を持たないときは nullptr を与える．
  ElbCaHead*
  new_CaHead(const VlModule* module,
	     const PtItem* pt_head,
	     const VlDelay* delay = nullptr) override;

  /// @brief continuous assignment を生成する．
  /// @param[in] head ヘッダ
  /// @param[in] pt_obj 対応するパース木中の定義要素
  /// @param[in] lhs 左辺式
  /// @param[in] rhs 右辺式
  const VlContAssign*
  new_ContAssign(ElbCaHead* head,
		 const PtBase* pt_obj,
		 const VlExpr* lhs,
		 const VlExpr* rhs) override;

  /// @brief net 宣言中の continuous assignment を生成する．
  /// @param[in] module 親のモジュール
  /// @param[in] pt_obj パース木の定義要素
  /// @param[in] lhs 左辺式
  /// @param[in] rhs 右辺式
  const VlContAssign*
  new_ContAssign(const VlModule* module,
		 const PtBase* pt_obj,
		 const VlExpr* lhs,
		 const VlExpr* rhs) override;

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
		  const VlValue& rhs_value) override;

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
		       const VlValue& rhs_value) override;

  /// @brief defparam 文を生成する．
  /// @param[in] module 親のモジュール
  /// @param[in] pt_header パース木の defparam ヘッダ
  /// @param[in] pt_item パース木の defparam 定義
  /// @param[in] param 対象の parameter
  /// @param[in] rhs_expr 割り当て式の右辺
  /// @param[in] rhs_value 右辺の値
  const VlDefParam*
  new_DefParam(const VlModule* module,
	       const PtItem* pt_header,
	       const PtDefParam* pt_defparam,
	       ElbParameter* param,
	       const PtExpr* rhs_expr,
	       const VlValue& rhs_value) override;

  /// @brief ゲートプリミティブのヘッダを生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_header パース木の定義
  /// @param[in] has_delay 遅延値を持つとき true
  ElbPrimHead*
  new_PrimHead(const VlScope* parent,
	       const PtItem* pt_header,
	       bool has_delay) override;

  /// @brief UDPプリミティブのヘッダを生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_header パース木の定義
  /// @param[in] udp UDP 定義
  /// @param[in] has_delay 遅延値を持つとき true
  ElbPrimHead*
  new_UdpHead(const VlScope* parent,
	      const PtItem* pt_header,
	      const VlUdpDefn* udp,
	      bool has_delay) override;

  /// @brief セルプリミティブのヘッダを生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_header パース木の定義
  /// @param[in] cell_id セル番号
  ElbPrimHead*
  new_CellHead(const VlScope* parent,
	       const PtItem* pt_header,
	       int cell_id) override;

  /// @brief プリミティブインスタンスを生成する．
  /// @param[in] head ヘッダ
  /// @param[in] pt_inst インスタンス定義
  ElbPrimitive*
  new_Primitive(ElbPrimHead* head,
		const PtInst* pt_inst) override;

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
		     int right_val) override;

  /// @brief セルプリミティブインスタンスを生成する．
  /// @param[in] head ヘッダ
  /// @param[in] cell セル
  /// @param[in] pt_inst インスタンス定義
  ElbPrimitive*
  new_CellPrimitive(ElbPrimHead* head,
		    const ClibCell& cell,
		    const PtInst* pt_inst) override;

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
			 int right_val) override;

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
	       bool const_func) override;

  /// @brief function を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_item パース木の定義
  /// @param[in] const_func 定数関数フラグ
  ElbTaskFunc*
  new_Function(const VlScope* parent,
	       const PtItem* pt_item,
	       bool const_func) override;

  /// @brief task を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_item パース木の定義
  ElbTaskFunc*
  new_Task(const VlScope* parent,
	   const PtItem* pt_item) override;

  /// @brief プロセス文を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_item パース木の要素定義
  ElbProcess*
  new_Process(const VlScope* parent,
	      const PtItem* pt_item) override;

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
		 const VlControl* control = nullptr) override;

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
		 const VlExpr* rhs) override;

  /// @brief deassign ステートメントを生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] lhs 左辺の式
  const VlStmt*
  new_DeassignStmt(const VlScope* parent,
		   const VlProcess* process,
		   const PtStmt* pt_stmt,
		   const VlExpr* lhs) override;

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
		const VlExpr* rhs) override;

  /// @brief release ステートメントを生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] lhs 左辺の式
  const VlStmt*
  new_ReleaseStmt(const VlScope* parent,
		  const VlProcess* process,
		  const PtStmt* pt_stmt,
		  const VlExpr* lhs) override;

  /// @brief begin ブロックを生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] stmt_list 子のステートメントリスト
  const VlStmt*
  new_Begin(const VlScope* parent,
	    const VlProcess* process,
	    const PtStmt* pt_stmt,
	    const vector<const VlStmt*>& stmt_list) override;

  /// @brief fork ブロックを生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] stmt_list 子のステートメントリスト
  const VlStmt*
  new_Fork(const VlScope* parent,
	   const VlProcess* process,
	   const PtStmt* pt_stmt,
	   const vector<const VlStmt*>& stmt_list) override;

  /// @brief 名前付き begin ブロックを生成する．
  /// @param[in] block 自分自身に対応するスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] stmt_list 子のステートメントリスト
  const VlStmt*
  new_NamedBegin(const VlScope* block,
		 const VlProcess* process,
		 const PtStmt* pt_stmt,
		 const vector<const VlStmt*>& stmt_list) override;


  /// @brief 名前付き fork ブロックを生成する．
  /// @param[in] block 自分自身に対応するスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] stmt_list 子のステートメントリスト
  const VlStmt*
  new_NamedFork(const VlScope* block,
		const VlProcess* process,
		const PtStmt* pt_stmt,
		const vector<const VlStmt*>& stmt_list) override;

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
		const VlStmt* stmt) override;

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
		const VlStmt* stmt) override;

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
	       const VlStmt* stmt) override;

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
	      const VlStmt* stmt) override;

  /// @brief forever 文を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] stmt 本体のステートメント
  const VlStmt*
  new_ForeverStmt(const VlScope* parent,
		  const VlProcess* process,
		  const PtStmt* pt_stmt,
		  const VlStmt* stmt) override;

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
	     const VlStmt* else_stmt) override;

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
	       const vector<const VlCaseItem*>& caseitem_list) override;

  /// @brief caseitem を生成する．
  /// @param[in] pt_item パース木の caseitem 要素
  /// @param[in] label_list ラベルを表す式のリスト
  /// @param[in] body 本体のステートメント
  const VlCaseItem*
  new_CaseItem(const PtCaseItem* pt_item,
	       const vector<ElbExpr*>& label_list,
	       const VlStmt* body) override;

  /// @brief イベント文を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] named_event 対象のイベント
  const VlStmt*
  new_EventStmt(const VlScope* parent,
		const VlProcess* process,
		const PtStmt* pt_stmt,
		ElbExpr* named_event) override;

  /// @brief nullptr ステートメントを生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  const VlStmt*
  new_NullStmt(const VlScope* parent,
	       const VlProcess* process,
	       const PtStmt* pt_stmt) override;

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
	       const vector<ElbExpr*>& arg_array) override;

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
		  const vector<ElbExpr*>& arg_array) override;

  /// @brief disable 文を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] target 対象のスコープ
  const VlStmt*
  new_DisableStmt(const VlScope* parent,
		  const VlProcess* process,
		  const PtStmt* pt_stmt,
		  const VlScope* target) override;

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
	       const VlStmt* stmt) override;

  /// @brief 遅延コントロールを生成する．
  /// @param[in] pt_control パース木の定義要素
  /// @param[in] delay 遅延式
  const VlControl*
  new_DelayControl(const PtControl* pt_control,
		   ElbExpr* delay) override;

  /// @brief イベントコントロールを生成する．
  /// @param[in] pt_control パース木の定義要素
  /// @param[in] event_num イベントリストの要素数
  /// @param[in] event_list イベントリストを表す配列
  const VlControl*
  new_EventControl(const PtControl* pt_control,
		   const vector<ElbExpr*>& event_list) override;

  /// @brief リピートコントロールを生成する．
  /// @param[in] pt_control パース木の定義要素
  /// @param[in] rep 繰り返し式
  /// @param[in] event_num イベントリストの要素数
  /// @param[in] event_list イベントリストを表す配列
  const VlControl*
  new_RepeatControl(const PtControl* pt_control,
		    ElbExpr* rep,
		    const vector<ElbExpr*>& event_list) override;

  /// @brief 単項演算子を生成する．
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] op_type 演算子のタイプ
  /// @param[in] opr1 オペランド
  ElbExpr*
  new_UnaryOp(const PtExpr* pt_expr,
	      VpiOpType op_type,
	      ElbExpr* opr1) override;

  /// @brief 2項演算子を生成する．
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] op_type 演算子のタイプ
  /// @param[in] opr1 オペランド
  /// @param[in] opr2 オペランド
  ElbExpr*
  new_BinaryOp(const PtExpr* pt_expr,
	       VpiOpType op_type,
	       ElbExpr* opr1,
	       ElbExpr* opr2) override;

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
		ElbExpr* opr3) override;

  /// @brief 連結演算子を生成する．
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] opr_list オペランドのリスト
  ElbExpr*
  new_ConcatOp(const PtExpr* pt_expr,
	       const vector<ElbExpr*>& opr_list) override;

  /// @brief 反復連結演算子を生成する．
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] rep_expr 繰り返し数を表す式
  /// @param[in] rep_num 繰り返し数
  /// @param[in] opr_list オペランドのリスト
  ElbExpr*
  new_MultiConcatOp(const PtExpr* pt_expr,
		    SizeType rep_num,
		    ElbExpr* rep_expr,
		    const vector<ElbExpr*>& opr_list) override;

  /// @brief プライマリ式を生成する．
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] obj 本体のオブジェクト
  /// @param[in] index_list インデックスのリスト
  ElbExpr*
  new_Primary(const PtExpr* pt_expr,
	      const VlDecl* obj) override;

  /// @brief プライマリ式を生成する(net decl の初期値用)．
  /// @param[in] pt_item パース木の定義要素
  /// @param[in] obj 本体のオブジェクト
  /// @param[in] index_list インデックスのリスト
  ElbExpr*
  new_Primary(const PtDeclItem* pt_item,
	      const VlDecl* obj) override;

  /// @brief プライマリ式を生成する．
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] obj 本体のオブジェクト
  ElbExpr*
  new_Primary(const PtExpr* pt_expr,
	      ElbParameter* obj) override;

  /// @brief プライマリ式を生成する(配列要素版)．
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] obj 本体のオブジェクト
  /// @param[in] index_list インデックスのリスト
  ElbExpr*
  new_Primary(const PtExpr* pt_expr,
	      const VlDeclArray* obj,
	      const vector<ElbExpr*>& index_list) override;

  /// @brief プライマリ式を生成する(固定インデックスの配列要素版)．
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] obj 本体のオブジェクト
  /// @param[in] offset オフセット
  ElbExpr*
  new_Primary(const PtExpr* pt_expr,
	      const VlDeclArray* obj,
	      int offset) override;

  /// @brief 固定ビット選択式を生成する．
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] expr 本体のオブジェクト
  /// @param[in] bit_index ビット選択式
  /// @param[in] bit_index_val ビット選択式の値
  ElbExpr*
  new_BitSelect(const PtExpr* pt_expr,
		ElbExpr* expr,
		const PtExpr* bit_index,
		int bit_index_val) override;

  /// @brief 固定ビット選択式を生成する．
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] expr 本体の式
  /// @param[in] bit_index_val ビット選択式の値
  ElbExpr*
  new_BitSelect(const PtExpr* pt_expr,
		ElbExpr* expr,
		int bit_index_val) override;

  /// @brief 可変ビット選択式を生成する．
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] expr 本体のオブジェクト
  /// @param[in] bit_index ビット選択式
  ElbExpr*
  new_BitSelect(const PtExpr* pt_expr,
		ElbExpr* expr,
		ElbExpr* bit_index) override;

  /// @brief 固定部分選択式を生成する．
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] parent_expr 本体の式
  /// @param[in] index1, index2 パート選択式
  /// @param[in] index1_val, index2_val パート選択式の値
  ElbExpr*
  new_PartSelect(const PtExpr* pt_expr,
		 ElbExpr* parent_expr,
		 const PtExpr* index1,
		 const PtExpr* index2,
		 int index1_val,
		 int index2_val) override;

  /// @brief 固定部分選択式を生成する．
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] parent_expr 本体の式
  /// @param[in] index1, inde2 パート選択式
  ElbExpr*
  new_PartSelect(const PtExpr* pt_expr,
		 ElbExpr* parent_expr,
		 int index1,
		 int index2) override;

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
		     int range_val) override;

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
		      int range_val) override;

  /// @brief 定数式を生成する．
  /// @param[in] pt_expr パース木の定義要素
  ElbExpr*
  new_Constant(const PtExpr* pt_expr) override;

  /// @brief genvar 起因の定数式を生成する．
  /// @param[in] pt_primary パース木の定義要素
  /// @param[in] val 値
  ElbExpr*
  new_GenvarConstant(const PtExpr* pt_primary,
		     int val) override;

  /// @brief 関数呼び出し式を生成する．
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] func 関数
  /// @param[in] arg_list 引数のリスト
  ElbExpr*
  new_FuncCall(const PtExpr* pt_expr,
	       const VlTaskFunc* func,
	       const vector<ElbExpr*>& arg_list) override;

  /// @brief システム関数呼び出し式を生成する．
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] user_systf システム関数
  /// @param[in] arg_list 引数のリスト
  ElbExpr*
  new_SysFuncCall(const PtExpr* pt_obj,
		  const VlUserSystf* user_systf,
		  const vector<ElbExpr*>& arg_list) override;

  /// @brief システム関数/システムタスクの引数を生成する．
  /// @param[in] pt_expr パース木中で参照している要素
  /// @param[in] arg 引数本体
  ElbExpr*
  new_ArgHandle(const PtExpr* pt_expr,
		const VlScope* arg) override;

  /// @brief システム関数/システムタスクの引数を生成する．
  /// @param[in] pt_expr パース木中で参照している要素
  /// @param[in] arg 引数本体
  ElbExpr*
  new_ArgHandle(const PtExpr* pt_expr,
		const VlPrimitive* arg) override;

  /// @brief システム関数/システムタスクの引数を生成する．
  /// @param[in] pt_expr パース木中で参照している要素
  /// @param[in] arg 引数本体
  ElbExpr*
  new_ArgHandle(const PtExpr* pt_expr,
		const VlDeclArray* arg) override;

  /// @brief 連結演算子の左辺式を生成する．
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] opr_array オペランドを格納する配列
  /// @param[in] lhs_elem_array 左辺の要素の配列
  ElbExpr*
  new_Lhs(const PtExpr* pt_expr,
	  const vector<ElbExpr*>& opr_array,
	  const vector<ElbExpr*>& lhs_elem_array) override;

  /// @brief 遅延値を生成する．
  /// @param[in] pt_obj パース木の定義要素
  /// @param[in] elem_num 要素数
  /// @param[in] expr_list 式の配列
  const VlDelay*
  new_Delay(const PtBase* pt_obj,
	    const vector<ElbExpr*>& expr_list) override;

  /// @brief attribute instance のリストを生成する．
  /// @param[in] pt_attr パース木の定義要素
  /// @param[in] expr 値
  /// @param[in] def 定義側の属性の時 true
  const VlAttribute*
  new_Attribute(const PtAttrSpec* pt_attr,
		const VlExpr* expr,
		bool def) override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  ymuint32 mUdpDefnNum;
  ymuint32 mModuleArrayNum;
  ymuint32 mModule1Num;
  ymuint32 mModule2Num;
  ymuint32 mPortNum;
  ymuint32 mIODeclMRNum;
  ymuint32 mIODeclMNum;
  ymuint32 mParameterNNum;
  ymuint32 mParameterRNum;
  ymuint32 mParameterTNum;
  ymuint32 mSpecParamNum;
  ymuint32 mSpecParamRNum;
  ymuint32 mNetArrayNum;
  ymuint32 mNetNum;
  ymuint32 mNetInfoNum;
  ymuint32 mNetInfoRNum;
  ymuint32 mRegArrayNum;
  ymuint32 mRegNum;
  ymuint32 mIntArrayNum;
  ymuint32 mRealArrayNum;
  ymuint32 mTimeArrayNum;
  ymuint32 mIntNum;
  ymuint32 mTimeNum;
  ymuint32 mRealNum;
  ymuint32 mNeArrayNum;
  ymuint32 mNamedEventNum;
  ymuint32 mContAssignNum;
  ymuint32 mDefParamNum;
  ymuint32 mParamAssignNum;
  ymuint32 mPrimArrayNum;
  ymuint32 mPrimitiveNum;
  ymuint32 mTaskNum;
  ymuint32 mFunctionNum;
  ymuint32 mFunctionRNum;
  ymuint32 mFunctionTNum;
  ymuint32 mIODeclTFRNum;
  ymuint32 mIODeclTFNum;
  ymuint32 mInitialNum;
  ymuint32 mAlwaysNum;
  ymuint32 mBeginNum;
  ymuint32 mForkNum;
  ymuint32 mNamedBeginNum;
  ymuint32 mNamedForkNum;
  ymuint32 mIfStmtNum;
  ymuint32 mIfElseStmtNum;
  ymuint32 mWhileStmtNum;
  ymuint32 mRepeatStmtNum;
  ymuint32 mWaitStmtNum;
  ymuint32 mCaseStmtNum;
  ymuint32 mCaseItemNum;
  ymuint32 mForStmtNum;
  ymuint32 mDelayCtrlStmtNum;
  ymuint32 mEventCtrlStmtNum;
  ymuint32 mEventStmtNum;
  ymuint32 mAssignmentNum;
  ymuint32 mAssignStmtNum;
  ymuint32 mDeassignStmtNum;
  ymuint32 mDisableStmtNum;
  ymuint32 mTaskCallNum;
  ymuint32 mSysTaskCallNum;
  ymuint32 mForeverStmtNum;
  ymuint32 mForceStmtNum;
  ymuint32 mReleaseStmtNum;
  ymuint32 mNullStmtNum;
  ymuint32 mPrimaryNum;
  ymuint32 mBitSelectNum;
  ymuint32 mGenericBitSelectNum;
  ymuint32 mConstPartSelectNum;
  ymuint32 mPlusPartSelectNum;
  ymuint32 mMinusPartSelectNum;
  ymuint32 mNotOpNum;
  ymuint32 mBitNegOpNum;
  ymuint32 mUnaryAndOpNum;
  ymuint32 mUnaryNandOpNum;
  ymuint32 mUnaryOrOpNum;
  ymuint32 mUnaryNorOpNum;
  ymuint32 mUnaryXorOpNum;
  ymuint32 mUnaryXNorOpNum;
  ymuint32 mPlusOpNum;
  ymuint32 mMinusOpNum;
  ymuint32 mPosedgeOpNum;
  ymuint32 mNegedgeOpNum;
  ymuint32 mEqOpNum;
  ymuint32 mNeqOpNum;
  ymuint32 mCaseEqOpNum;
  ymuint32 mCaseNeqOpNum;
  ymuint32 mGtOpNum;
  ymuint32 mGeOpNum;
  ymuint32 mLtOpNum;
  ymuint32 mLeOpNum;
  ymuint32 mLogAndOpNum;
  ymuint32 mLogOrOpNum;
  ymuint32 mBitAndOpNum;
  ymuint32 mBitOrOpNum;
  ymuint32 mBitXorOpNum;
  ymuint32 mBitXNorOpNum;
  ymuint32 mAddOpNum;
  ymuint32 mSubOpNum;
  ymuint32 mMultOpNum;
  ymuint32 mDivOpNum;
  ymuint32 mModOpNum;
  ymuint32 mPowerOpNum;
  ymuint32 mLShiftOpNum;
  ymuint32 mRShiftOpNum;
  ymuint32 mArithLShiftOpNum;
  ymuint32 mArithRShiftOpNum;
  ymuint32 mConditionOpNum;
  ymuint32 mMinTypMaxOpNum;
  ymuint32 mConcatOpNum;
  ymuint32 mMultiConcatOpNum;
  ymuint32 mListOpNum;
  ymuint32 mEventOrOpNum;
  ymuint32 mFuncCallNum;
  ymuint32 mSysFuncCallNum;
  ymuint32 mIntConstNum;
  ymuint32 mBitVectorConstNum;
  ymuint32 mRealConstNum;
  ymuint32 mStringConstNum;
  ymuint32 mGenvarConstantNum;
  ymuint32 mRangeNum;
  ymuint32 mDelayControlNum;
  ymuint32 mEventControlNum;
  ymuint32 mRepeatControlNum;
  ymuint32 mAttributeNum;
  ymuint32 mStmtBlockScopeNum;
  ymuint32 mGenBlockNum;
  ymuint32 mGenForBlockNum;
  ymuint32 mModuleInfoNum;
};

END_NAMESPACE_YM_VERILOG

#endif // EIFACTORY_H
