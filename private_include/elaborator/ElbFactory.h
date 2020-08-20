#ifndef ELBFACTORY_H
#define ELBFACTORY_H

/// @file ElbFactory.h
/// @brief ElbFactory のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.


#include "ym/verilog.h"
#include "ym/pt/PtP.h"
#include "ym/clib.h"
#include "ElbFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ElbFactory ElbFactory "ElbFactory.h"
/// @breif ElbObj の派生クラスを生成するファクトリクラス
//////////////////////////////////////////////////////////////////////
class ElbFactory
{
public:

  /// @brief オブジェクトを生成する関数
  static
  ElbFactory*
  new_obj();

  /// @brief デストラクタ
  virtual
  ~ElbFactory() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 管理用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内訳を表示する．
  virtual
  void
  dump_prof(ostream& s) = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // インスタンス生成関係のメソッド
  //////////////////////////////////////////////////////////////////////

  /// @brief トップレベルのスコープを生成する
  virtual
  const VlScope*
  new_Toplevel() = 0;

  /// @brief ステートメントブロックのスコープを生成する．
  /// @param[in] parent 親のスコープ環境
  /// @param[in] pt_stmt 対応するパース木の要素
  virtual
  const VlScope*
  new_StmtBlockScope(const VlScope* parent,
		     const PtStmt* pt_stmt) = 0;

  /// @brief generate block を生成する．
  /// @param[in] parent 親のスコープ環境
  /// @param[in] pt_item 対応するパース木の要素
  virtual
  const VlScope*
  new_GenBlock(const VlScope* parent,
	       const PtItem* pt_item) = 0;

  /// @brief GfBlock 検索用の親の名前付きオブジェクトを作る．
  virtual
  ElbGfRoot*
  new_GfRoot(const VlScope* parent,
	     const PtItem* pt_item) = 0;

  /// @brief generate for block を生成する．
  /// @param[in] parent 親のスコープ環境
  /// @param[in] pt_item 対応するパース木の要素
  /// @param[in] int gvi 対応する genvar の値
  virtual
  const VlScope*
  new_GfBlock(const VlScope* parent,
	      const PtItem* pt_item,
	      int gvi) = 0;

  /// @brief UDP定義を生成する．
  /// @param[in] pt_udp パース木の UDP 定義
  /// @param[in] is_protected プロテクト属性
  virtual
  ElbUdpDefn*
  new_UdpDefn(const PtUdp* pt_udp,
	      bool is_protected) = 0;

  /// @brief module を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_module モジュールテンプレート
  /// @param[in] pt_head パース木のヘッダ定義
  /// @param[in] pt_inst パース木のインスタンス定義
  virtual
  ElbModule*
  new_Module(const VlScope* parent,
	     const PtModule* pt_module,
	     const PtItem* pt_head,
	     const PtInst* pt_inst) = 0;

  /// @brief module array を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_module モジュールテンプレート
  /// @param[in] pt_head パース木のヘッダ定義
  /// @param[in] pt_inst パース木のインスタンス定義
  /// @param[in] left 範囲の MSB の式
  /// @param[in] right 範囲の LSB の式
  /// @param[in] left_val 範囲の MSB の値
  /// @param[in] right_val 範囲の LSB の値
  virtual
  ElbModuleArray*
  new_ModuleArray(const VlScope* parent,
		  const PtModule* pt_module,
		  const PtItem* pt_head,
		  const PtInst* pt_inst,
		  const PtExpr* left,
		  const PtExpr* right,
		  int left_val,
		  int right_val) = 0;

  /// @brief module IO ヘッダを生成する．
  /// @param[in] module 親のモジュール
  /// @param[in] pt_header パース木のIO宣言ヘッダ
  virtual
  ElbIOHead*
  new_IOHead(const VlModule* module,
	     const PtIOHead* pt_header) = 0;

  /// @brief タスク/関数 IO ヘッダを生成する．
  /// @param[in] taskfunc 親のタスク/関数
  /// @param[in] pt_header パース木のIO宣言ヘッダ
  virtual
  ElbIOHead*
  new_IOHead(const VlTaskFunc* module,
	     const PtIOHead* pt_header) = 0;

  /// @brief 宣言要素のヘッダを生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_head パース木の宣言ヘッダ
  /// @param[in] has_delay 遅延値を持つとき true
  virtual
  ElbDeclHead*
  new_DeclHead(const VlScope* parent,
	       const PtDeclHead* pt_head,
	       bool has_delay = false) = 0;

  /// @brief 宣言要素のヘッダを生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_head パース木の宣言ヘッダ
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @param[in] left_val 範囲の MSB の値
  /// @param[in] right_val 範囲の LSB の値
  /// @param[in] has_delay 遅延値を持つとき true
  virtual
  ElbDeclHead*
  new_DeclHead(const VlScope* parent,
	       const PtDeclHead* pt_head,
	       const PtExpr* left,
	       const PtExpr* right,
	       int left_val,
	       int right_val,
	       bool has_delay = false) = 0;

  /// @brief 宣言要素のヘッダを生成する．(IODecl 中の宣言用)
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_head パース木のIO宣言ヘッダ
  /// @param[in] aux_type 補助的なデータ型
  virtual
  ElbDeclHead*
  new_DeclHead(const VlScope* parent,
	       const PtIOHead* pt_head,
	       VpiAuxType aux_type) = 0;

  /// @brief 宣言要素のヘッダを生成する．(IODecl 中の宣言用)
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_head パース木のIO宣言ヘッダ
  /// @param[in] aux_type 補助的なデータ型
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @param[in] left_val 範囲の MSB の値
  /// @param[in] right_val 範囲の LSB の値
  virtual
  ElbDeclHead*
  new_DeclHead(const VlScope* parent,
	       const PtIOHead* pt_head,
	       VpiAuxType aux_type,
	       const PtExpr* left,
	       const PtExpr* right,
	       int left_val,
	       int right_val) = 0;

  /// @brief 宣言要素のヘッダを生成する．(function の暗黙宣言用)
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_item パース木の関数定義
  virtual
  ElbDeclHead*
  new_DeclHead(const VlScope* parent,
	       const PtItem* pt_item) = 0;

  /// @brief 宣言要素のヘッダを生成する．(function の暗黙宣言用)
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_item パース木の関数定義
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @param[in] left_val 範囲の MSB の値
  /// @param[in] right_val 範囲の LSB の値
  virtual
  ElbDeclHead*
  new_DeclHead(const VlScope* parent,
	       const PtItem* pt_item,
	       const PtExpr* left,
	       const PtExpr* right,
	       int left_val,
	       int right_val) = 0;

  /// @brief 宣言要素を生成する．
  /// @param[in] head ヘッダ
  /// @param[in] pt_item パース木の宣言要素
  /// @param[in] init 初期値
  virtual
  ElbDecl*
  new_Decl(ElbDeclHead* head,
	   const PtNamedBase* pt_item,
	   const VlExpr* init = nullptr) = 0;

  /// @brief 暗黙のネットを生成する．
  /// @param[in] pt_expr パース木のプライマリ式
  virtual
  ElbDecl*
  new_ImpNet(const VlScope* parent,
	     const PtExpr* pt_expr,
	     VpiNetType net_type) = 0;

  /// @brief 宣言要素の配列を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] head ヘッダ
  /// @param[in] pt_item パース木の宣言要素
  /// @param[in] range_src 範囲の配列
  virtual
  const VlDeclArray*
  new_DeclArray(ElbDeclHead* head,
		const PtNamedBase* pt_item,
		const vector<ElbRangeSrc>& range_src) = 0;

  /// @brief parameter 宣言のヘッダを生成する(範囲指定なし)．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_head パース木の宣言ヘッダ
  virtual
  ElbParamHead*
  new_ParamHead(const VlScope* parent,
		const PtDeclHead* pt_head) = 0;

  /// @brief parameter 宣言のヘッダを生成する(範囲指定あり)．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_head パース木の宣言ヘッダ
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @param[in] left_val 範囲の MSB の値
  /// @param[in] right_val 範囲の LSB の値
  virtual
  ElbParamHead*
  new_ParamHead(const VlScope* parent,
		const PtDeclHead* pt_head,
		const PtExpr* left,
		const PtExpr* right,
		int left_val,
		int right_val) = 0;

  /// @brief parameter 宣言を生成する．
  /// @param[in] head ヘッダ
  /// @param[in] pt_item パース木の宣言要素
  /// @param[in] init 初期値
  /// @param[in] is_local localparam の時 true
  virtual
  ElbParameter*
  new_Parameter(ElbParamHead* head,
		const PtNamedBase* pt_item,
		bool is_local = false) = 0;

  /// @brief genvar を生成する．
  /// @param[in] parent 親のスコープ環境
  /// @param[in] pt_item 対応するパース木の要素
  /// @param[in] val 初期値
  virtual
  ElbGenvar*
  new_Genvar(const VlScope* parent,
	     const PtDeclItem* pt_item,
	     int val) = 0;

  /// @brief continuous assignment のヘッダを生成する．
  /// @param[in] module 親のモジュール
  /// @param[in] pt_head パース木のヘッダ定義
  /// @param[in] delay 遅延値
  /// @note 遅延を持たないときは nullptr を与える．
  virtual
  ElbCaHead*
  new_CaHead(const VlModule* module,
	     const PtItem* pt_head,
	     const VlDelay* delay = nullptr) = 0;

  /// @brief continuous assignment を生成する．
  /// @param[in] head ヘッダ
  /// @param[in] pt_obj 対応するパース木中の定義要素
  /// @param[in] lhs 左辺式
  /// @param[in] rhs 右辺式
  virtual
  const VlContAssign*
  new_ContAssign(ElbCaHead* head,
		 const PtBase* pt_obj,
		 const VlExpr* lhs,
		 const VlExpr* rhs) = 0;

  /// @brief net 宣言中の continuous assignment を生成する．
  /// @param[in] module 親のモジュール
  /// @param[in] pt_obj パース木の定義要素
  /// @param[in] lhs 左辺式
  /// @param[in] rhs 右辺式
  virtual
  const VlContAssign*
  new_ContAssign(const VlModule* module,
		 const PtBase* pt_obj,
		 const VlExpr* lhs,
		 const VlExpr* rhs) = 0;

  /// @brief パラメータ割り当て文を生成する．
  /// @param[in] module 親のモジュール
  /// @param[in] param 対象の parameter
  /// @param[in] rhs_expr 割り当て式の右辺
  /// @param[in] rhs_value 右辺の値
  virtual
  const VlParamAssign*
  new_ParamAssign(const VlModule* module,
		  const PtBase* pt_obj,
		  ElbParameter* param,
		  const PtExpr* rhs_expr,
		  const VlValue& rhs_value) = 0;

  /// @brief 名前によるパラメータ割り当て文を生成する．
  /// @param[in] module 親のモジュール
  /// @param[in] param 対象の parameter
  /// @param[in] rhs_expr 割り当て式の右辺
  /// @param[in] rhs_value 右辺の値
  virtual
  const VlParamAssign*
  new_NamedParamAssign(const VlModule* module,
		       const PtBase* pt_obj,
		       ElbParameter* param,
		       const PtExpr* rhs_expr,
		       const VlValue& rhs_value) = 0;

  /// @brief defparam 文を生成する．
  /// @param[in] module 親のモジュール
  /// @param[in] pt_header パース木の defparam ヘッダ
  /// @param[in] pt_item パース木の defparam 定義
  /// @param[in] param 対象の parameter
  /// @param[in] rhs_expr 割り当て式の右辺式
  /// @param[in] rhs_value 右辺の値
  virtual
  const VlDefParam*
  new_DefParam(const VlModule* module,
	       const PtItem* pt_header,
	       const PtDefParam* pt_defparam,
	       ElbParameter* param,
	       const PtExpr* rhs_expr,
	       const VlValue& rhs_value) = 0;

  /// @brief ゲートプリミティブのヘッダを生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_header パース木の定義
  /// @param[in] has_delay 遅延値を持つとき true
  virtual
  ElbPrimHead*
  new_PrimHead(const VlScope* parent,
	       const PtItem* pt_header,
	       bool has_delay) = 0;

  /// @brief UDPプリミティブのヘッダを生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_header パース木の定義
  /// @param[in] udp UDP 定義
  /// @param[in] has_delay 遅延値を持つとき true
  virtual
  ElbPrimHead*
  new_UdpHead(const VlScope* parent,
	      const PtItem* pt_header,
	      const VlUdpDefn* udp,
	      bool has_delay) = 0;

  /// @brief セルプリミティブのヘッダを生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_header パース木の定義
  /// @param[in] cell_id セル番号
  virtual
  ElbPrimHead*
  new_CellHead(const VlScope* parent,
	       const PtItem* pt_header,
	       int cell_id) = 0;

  /// @brief プリミティブインスタンスを生成する．
  /// @param[in] head ヘッダ
  /// @param[in] pt_inst インスタンス定義
  virtual
  ElbPrimitive*
  new_Primitive(ElbPrimHead* head,
		const PtInst* pt_inst) = 0;

  /// @brief プリミティブ配列インスタンスを生成する．
  /// @param[in] head ヘッダ
  /// @param[in] pt_inst インスタンス定義
  /// @param[in] left 範囲の MSB の式
  /// @param[in] right 範囲の LSB の式
  /// @param[in] left_val 範囲の MSB の値
  /// @param[in] right_val 範囲の LSB の値
  virtual
  ElbPrimArray*
  new_PrimitiveArray(ElbPrimHead* head,
		     const PtInst* pt_inst,
		     const PtExpr* left,
		     const PtExpr* right,
		     int left_val,
		     int right_val) = 0;

  /// @brief セルプリミティブインスタンスを生成する．
  /// @param[in] head ヘッダ
  /// @param[in] cell セル
  /// @param[in] pt_inst インスタンス定義
  virtual
  ElbPrimitive*
  new_CellPrimitive(ElbPrimHead* head,
		    const ClibCell& cell,
		    const PtInst* pt_inst) = 0;

  /// @brief セルプリミティブ配列インスタンスを生成する．
  /// @param[in] head ヘッダ
  /// @param[in] cell セル
  /// @param[in] pt_inst インスタンス定義
  /// @param[in] left 範囲の MSB の式
  /// @param[in] right 範囲の LSB の式
  /// @param[in] left_val 範囲の MSB の値
  /// @param[in] right_val 範囲の LSB の値
  virtual
  ElbPrimArray*
  new_CellPrimitiveArray(ElbPrimHead* head,
			 const ClibCell& cell,
			 const PtInst* pt_inst,
			 const PtExpr* left,
			 const PtExpr* right,
			 int left_val,
			 int right_val) = 0;

  /// @brief function を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_item パース木の定義
  /// @param[in] const_func 定数関数フラグ
  virtual
  ElbTaskFunc*
  new_Function(const VlScope* parent,
	       const PtItem* pt_item,
	       bool const_func) = 0;

  /// @brief function を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_item パース木の定義
  /// @param[in] left 範囲の MSB の式
  /// @param[in] right 範囲の LSB の式
  /// @param[in] left_val 範囲の MSB の値
  /// @param[in] right_val 範囲の LSB の値
  /// @param[in] const_func 定数関数フラグ
  virtual
  ElbTaskFunc*
  new_Function(const VlScope* parent,
	       const PtItem* pt_item,
	       const PtExpr* left,
	       const PtExpr* right,
	       int left_val,
	       int right_val,
	       bool const_func) = 0;

  /// @brief task を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_item パース木の定義
  virtual
  ElbTaskFunc*
  new_Task(const VlScope* parent,
	   const PtItem* pt_item) = 0;

  /// @brief プロセス文を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_item パース木の要素定義
  virtual
  ElbProcess*
  new_Process(const VlScope* parent,
	      const PtItem* pt_item) = 0;

  /// @brief 代入文を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] lhs 左辺の式
  /// @param[in] rhs 右辺の式
  /// @param[in] block ブロッキング代入の時 true
  /// @param[in] control コントロール
  virtual
  const VlStmt*
  new_Assignment(const VlScope* parent,
		 const VlProcess* process,
		 const PtStmt* pt_stmt,
		 const VlExpr* lhs,
		 const VlExpr* rhs,
		 bool block,
		 const VlControl* control = nullptr) = 0;

  /// @brief assign ステートメントを生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] lhs 左辺の式
  /// @param[in] rhs 右辺の式
  virtual
  const VlStmt*
  new_AssignStmt(const VlScope* parent,
		 const VlProcess* process,
		 const PtStmt* pt_stmt,
		 const VlExpr* lhs,
		 const VlExpr* rhs) = 0;

  /// @brief deassign ステートメントを生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] lhs 左辺の式
  virtual
  const VlStmt*
  new_DeassignStmt(const VlScope* parent,
		   const VlProcess* process,
		   const PtStmt* pt_stmt,
		   const VlExpr* lhs) = 0;

  /// @brief force ステートメントを生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] lhs 左辺の式
  /// @param[in] rhs 右辺の式
  virtual
  const VlStmt*
  new_ForceStmt(const VlScope* parent,
		const VlProcess* process,
		const PtStmt* pt_stmt,
		const VlExpr* lhs,
		const VlExpr* rhs) = 0;

  /// @brief release ステートメントを生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] lhs 左辺の式
  virtual
  const VlStmt*
  new_ReleaseStmt(const VlScope* parent,
		  const VlProcess* process,
		  const PtStmt* pt_stmt,
		  const VlExpr* lhs) = 0;

  /// @brief begin ブロックを生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] stmt_list 子のステートメントリスト
  virtual
  const VlStmt*
  new_Begin(const VlScope* parent,
	    const VlProcess* process,
	    const PtStmt* pt_stmt,
	    const vector<const VlStmt*>& stmt_list) = 0;

  /// @brief fork ブロックを生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] stmt_list 子のステートメントリスト
  virtual
  const VlStmt*
  new_Fork(const VlScope* parent,
	   const VlProcess* process,
	   const PtStmt* pt_stmt,
	   const vector<const VlStmt*>& stmt_list) = 0;

  /// @brief 名前付き begin ブロックを生成する．
  /// @param[in] block 自分自身に対応するスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] stmt_list 子のステートメントリスト
  virtual
  const VlStmt*
  new_NamedBegin(const VlScope* block,
		 const VlProcess* process,
		 const PtStmt* pt_stmt,
		 const vector<const VlStmt*>& stmt_list) = 0;


  /// @brief 名前付き fork ブロックを生成する．
  /// @param[in] block 自分自身に対応するスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] stmt_list 子のステートメントリスト
  virtual
  const VlStmt*
  new_NamedFork(const VlScope* block,
		const VlProcess* process,
		const PtStmt* pt_stmt,
		const vector<const VlStmt*>& stmt_list) = 0;

  /// @brief while 文を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] cond 条件式
  /// @param[in] stmt 本体のステートメント
  virtual
  const VlStmt*
  new_WhileStmt(const VlScope* parent,
		const VlProcess* process,
		const PtStmt* pt_stmt,
		const VlExpr* cond,
		const VlStmt* stmt) = 0;

  /// @brief repeat 文を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] cond 条件式
  /// @param[in] stmt 本体のステートメント
  virtual
  const VlStmt*
  new_RepeatStmt(const VlScope* parent,
		const VlProcess* process,
		const PtStmt* pt_stmt,
		const VlExpr* cond,
		const VlStmt* stmt) = 0;

  /// @brief wait 文を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] cond 条件式
  /// @param[in] stmt 本体のステートメント
  virtual
  const VlStmt*
  new_WaitStmt(const VlScope* parent,
	       const VlProcess* process,
	       const PtStmt* pt_stmt,
	       const VlExpr* cond,
	       const VlStmt* stmt) = 0;

  /// @brief for 文を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] cond 条件式
  /// @param[in] init_stmt 初期化式
  /// @param[in] inc_stmt 増加式
  /// @param[in] stmt 本体のステートメント
  virtual
  const VlStmt*
  new_ForStmt(const VlScope* parent,
	      const VlProcess* process,
	      const PtStmt* pt_stmt,
	      const VlExpr* cond,
	      const VlStmt* init_stmt,
	      const VlStmt* inc_stmt,
	      const VlStmt* stmt) = 0;

  /// @brief forever 文を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] stmt 本体のステートメント
  virtual
  const VlStmt*
  new_ForeverStmt(const VlScope* parent,
		  const VlProcess* process,
		  const PtStmt* pt_stmt,
		  const VlStmt* stmt) = 0;

  /// @brief if 文を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] cond 条件式
  /// @param[in] stmt 本体のステートメント
  /// @param[in] else_stmt else節のステートメント
  /// @note else_stmt は nullptr もありうる．
  virtual
  const VlStmt*
  new_IfStmt(const VlScope* parent,
	     const VlProcess* process,
	     const PtStmt* pt_stmt,
	     const VlExpr* cond,
	     const VlStmt* stmt,
	     const VlStmt* else_stmt) = 0;

  /// @brief case 文を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] expr 条件式
  /// @param[in] caseitem_list caseitem のリスト
  virtual
  const VlStmt*
  new_CaseStmt(const VlScope* parent,
	       const VlProcess* process,
	       const PtStmt* pt_stmt,
	       const VlExpr* expr,
	       const vector<const VlCaseItem*>& caseitem_list) = 0;

  /// @brief caseitem を生成する．
  /// @param[in] pt_item パース木の caseitem 要素
  /// @param[in] label_list ラベルを表す式のリスト
  /// @param[in] body 本体のステートメント
  virtual
  const VlCaseItem*
  new_CaseItem(const PtCaseItem* pt_item,
	       const vector<ElbExpr*>& label_list,
	       const VlStmt* body) = 0;

  /// @brief イベント文を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] named_event 対象のイベント
  virtual
  const VlStmt*
  new_EventStmt(const VlScope* parent,
		const VlProcess* process,
		const PtStmt* pt_stmt,
		ElbExpr* named_event) = 0;

  /// @brief nullptr ステートメントを生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  virtual
  const VlStmt*
  new_NullStmt(const VlScope* parent,
	       const VlProcess* process,
	       const PtStmt* pt_stmt) = 0;

  /// @brief タスクコール文を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] task 対象のタスク
  /// @param[in] arg_array 引数の配列
  virtual
  const VlStmt*
  new_TaskCall(const VlScope* parent,
	       const VlProcess* process,
	       const PtStmt* pt_stmt,
	       const VlTaskFunc* task,
	       const vector<ElbExpr*>& arg_array) = 0;

  /// @brief システムタスクコール文を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] user_systf システムタスク
  /// @param[in] arg_array 引数の配列
  virtual
  const VlStmt*
  new_SysTaskCall(const VlScope* parent,
		  const VlProcess* process,
		  const PtStmt* pt_stmt,
		  const VlUserSystf* user_systf,
		  const vector<ElbExpr*>& arg_array) = 0;

  /// @brief disable 文を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] target 対象のスコープ
  virtual
  const VlStmt*
  new_DisableStmt(const VlScope* parent,
		  const VlProcess* process,
		  const PtStmt* pt_stmt,
		  const VlScope* target) = 0;

  /// @brief コントロール文を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] control コントロール
  /// @param[in] stmt 本体のステートメント
  virtual
  const VlStmt*
  new_CtrlStmt(const VlScope* parent,
	       const VlProcess* process,
	       const PtStmt* pt_stmt,
	       const VlControl* control,
	       const VlStmt* stmt) = 0;

  /// @brief 遅延コントロールを生成する．
  /// @param[in] pt_control パース木の定義要素
  /// @param[in] delay 遅延式
  virtual
  const VlControl*
  new_DelayControl(const PtControl* pt_control,
		   ElbExpr* delay) = 0;

  /// @brief イベントコントロールを生成する．
  /// @param[in] pt_control パース木の定義要素
  /// @param[in] event_list イベントリストを表す配列
  virtual
  const VlControl*
  new_EventControl(const PtControl* pt_control,
		   const vector<ElbExpr*>& event_list) = 0;

  /// @brief リピートコントロールを生成する．
  /// @param[in] pt_control パース木の定義要素
  /// @param[in] rep 繰り返し式
  /// @param[in] event_list イベントリストを表す配列
  virtual
  const VlControl*
  new_RepeatControl(const PtControl* pt_control,
		    ElbExpr* rep,
		    const vector<ElbExpr*>& event_list) = 0;

  /// @brief 単項演算子を生成する．
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] op_type 演算子のタイプ
  /// @param[in] opr1 オペランド
  virtual
  ElbExpr*
  new_UnaryOp(const PtExpr* pt_expr,
	      VpiOpType op_type,
	      ElbExpr* opr1) = 0;

  /// @brief 2項演算子を生成する．
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] op_type 演算子のタイプ
  /// @param[in] opr1 オペランド
  /// @param[in] opr2 オペランド
  virtual
  ElbExpr*
  new_BinaryOp(const PtExpr* pt_expr,
	       VpiOpType op_type,
	       ElbExpr* opr1,
	       ElbExpr* opr2) = 0;

  /// @brief 3項演算子を生成する．
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] op_type 演算子のタイプ
  /// @param[in] opr1 オペランド
  /// @param[in] opr2 オペランド
  /// @param[in] opr3 オペランド
  virtual
  ElbExpr*
  new_TernaryOp(const PtExpr* pt_expr,
		VpiOpType op_type,
		ElbExpr* opr1,
		ElbExpr* opr2,
		ElbExpr* opr3) = 0;

  /// @brief 連結演算子を生成する．
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] opr_list オペランドのリスト
  virtual
  ElbExpr*
  new_ConcatOp(const PtExpr* pt_expr,
	       const vector<ElbExpr*>& opr_list) = 0;

  /// @brief 反復連結演算子を生成する．
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] rep_expr 繰り返し数を表す式
  /// @param[in] rep_num 繰り返し数
  /// @param[in] opr_list オペランドのリスト
  virtual
  ElbExpr*
  new_MultiConcatOp(const PtExpr* pt_expr,
		    SizeType rep_num,
		    ElbExpr* rep_expr,
		    const vector<ElbExpr*>& opr_list) = 0;

  /// @brief プライマリ式を生成する．
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] obj 本体のオブジェクト
  virtual
  ElbExpr*
  new_Primary(const PtExpr* pt_expr,
	      const VlDecl* obj) = 0;

  /// @brief プライマリ式を生成する(net decl の初期値用)．
  /// @param[in] pt_item パース木の定義要素
  /// @param[in] obj 本体のオブジェクト
  /// @param[in] index_list インデックスのリスト
  virtual
  ElbExpr*
  new_Primary(const PtDeclItem* pt_item,
	      const VlDecl* obj) = 0;

  /// @brief プライマリ式を生成する．
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] obj 本体のオブジェクト
  virtual
  ElbExpr*
  new_Primary(const PtExpr* pt_expr,
	      ElbParameter* obj) = 0;

  /// @brief プライマリ式を生成する(配列要素版)．
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] obj 本体のオブジェクト
  /// @param[in] index_list インデックスのリスト
  virtual
  ElbExpr*
  new_Primary(const PtExpr* pt_expr,
	      const VlDeclArray* obj,
	      const vector<ElbExpr*>& index_list) = 0;

  /// @brief プライマリ式を生成する(固定インデックスの配列要素版)．
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] obj 本体のオブジェクト
  /// @param[in] offset オフセット
  virtual
  ElbExpr*
  new_Primary(const PtExpr* pt_expr,
	      const VlDeclArray* obj,
	      int offset) = 0;

  /// @brief 固定ビット選択式を生成する．
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] expr 本体のオブジェクト
  /// @param[in] bit_index ビット選択式
  /// @param[in] bit_index_val ビット選択式の値
  virtual
  ElbExpr*
  new_BitSelect(const PtExpr* pt_expr,
		ElbExpr* expr,
		const PtExpr* bit_index,
		int bit_index_val) = 0;

  /// @brief 固定ビット選択式を生成する．
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] expr 本体の式
  /// @param[in] bit_index_val ビット選択式の値
  virtual
  ElbExpr*
  new_BitSelect(const PtExpr* pt_expr,
		ElbExpr* expr,
		int bit_index_val) = 0;

  /// @brief 可変ビット選択式を生成する．
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] expr 本体のオブジェクト
  /// @param[in] bit_index ビット選択式
  virtual
  ElbExpr*
  new_BitSelect(const PtExpr* pt_expr,
		ElbExpr* expr,
		ElbExpr* bit_index) = 0;

  /// @brief 固定部分選択式を生成する．
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] obj 本体のオブジェクト
  /// @param[in] index1, index2 パート選択式
  /// @param[in] index1_val, index2_val パート選択式の値
  virtual
  ElbExpr*
  new_PartSelect(const PtExpr* pt_expr,
		 ElbExpr* obj,
		 const PtExpr* index1,
		 const PtExpr* index2,
		 int index1_val,
		 int index2_val) = 0;

  /// @brief 固定部分選択式を生成する．
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] expr 本体の式
  /// @param[in] index1, inde2 パート選択式
  virtual
  ElbExpr*
  new_PartSelect(const PtExpr* pt_expr,
		 ElbExpr* expr,
		 int index1,
		 int index2) = 0;

  /// @brief 可変部分選択式を生成する．
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] obj 本体のオブジェクト
  /// @param[in] base 範囲のベースアドレスを表す式
  /// @param[in] range 範囲を表す式
  /// @param[in] range_val 範囲の値
  virtual
  ElbExpr*
  new_PlusPartSelect(const PtExpr* pt_expr,
		     ElbExpr* obj,
		     ElbExpr* base,
		     const PtExpr* range_expr,
		     int range_val) = 0;

  /// @brief 可変部分選択式を生成する．
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] obj 本体のオブジェクト
  /// @param[in] base 範囲のベースアドレスを表す式
  /// @param[in] range 範囲を表す式
  /// @param[in] range_val 範囲の値
  virtual
  ElbExpr*
  new_MinusPartSelect(const PtExpr* pt_expr,
		      ElbExpr* obj,
		      ElbExpr* base,
		      const PtExpr* range_expr,
		      int range_val) = 0;

  /// @brief 定数式を生成する．
  /// @param[in] pt_expr パース木の定義要素
  virtual
  ElbExpr*
  new_Constant(const PtExpr* pt_expr) = 0;

  /// @brief genvar 起因の定数式を生成する．
  /// @param[in] pt_primary パース木の定義要素
  /// @param[in] val 値
  virtual
  ElbExpr*
  new_GenvarConstant(const PtExpr* pt_primary,
		     int val) = 0;

  /// @brief 関数呼び出し式を生成する．
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] func 関数
  /// @param[in] arg_list 引数のリスト
  virtual
  ElbExpr*
  new_FuncCall(const PtExpr* pt_expr,
	       const VlTaskFunc* func,
	       const vector<ElbExpr*>& arg_list) = 0;

  /// @brief システム関数呼び出し式を生成する．
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] user_systf システム関数
  /// @param[in] arg_list 引数のリスト
  virtual
  ElbExpr*
  new_SysFuncCall(const PtExpr* pt_expr,
		  const VlUserSystf* user_systf,
		  const vector<ElbExpr*>& arg_list) = 0;

  /// @brief システム関数/システムタスクの引数を生成する．
  /// @param[in] pt_expr パース木中で参照している要素
  /// @param[in] arg 引数本体
  virtual
  ElbExpr*
  new_ArgHandle(const PtExpr* pt_expr,
		const VlScope* arg) = 0;

  /// @brief システム関数/システムタスクの引数を生成する．
  /// @param[in] pt_expr パース木中で参照している要素
  /// @param[in] arg 引数本体
  virtual
  ElbExpr*
  new_ArgHandle(const PtExpr* pt_expr,
		const VlPrimitive* arg) = 0;

  /// @brief システム関数/システムタスクの引数を生成する．
  /// @param[in] pt_expr パース木中で参照している要素
  /// @param[in] arg 引数本体
  virtual
  ElbExpr*
  new_ArgHandle(const PtExpr* pt_expr,
		const VlDeclArray* arg) = 0;

  /// @brief 連結演算子の左辺式を生成する．
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] opr_array オペランドを格納する配列
  /// @param[in] lhs_elem_array 左辺の要素の配列
  virtual
  ElbExpr*
  new_Lhs(const PtExpr* pt_expr,
	  const vector<ElbExpr*>& opr_array,
	  const vector<ElbExpr*>& lhs_elem_array) = 0;

  /// @brief 遅延値を生成する．
  /// @param[in] pt_obj パース木の定義要素
  /// @param[in] expr_list 式の配列
  virtual
  const VlDelay*
  new_Delay(const PtBase* pt_obj,
	    const vector<ElbExpr*>& expr_list) = 0;

  /// @brief attribute instance のリストを生成する．
  /// @param[in] pt_attr パース木の定義要素
  /// @param[in] def 定義側の属性の時 true
  virtual
  const VlAttribute*
  new_Attribute(const PtAttrInst* pt_attr,
		bool def) = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // ELBFACTORY_H
