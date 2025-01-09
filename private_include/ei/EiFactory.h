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
  dump_prof(
    ostream& s ///< [in] 出力先のストリーム
  ) override;


public:
  //////////////////////////////////////////////////////////////////////
  // インスタンス生成関係のメソッド
  //////////////////////////////////////////////////////////////////////

  /// @brief トップレベルのスコープを生成する
  const VlScope*
  new_Toplevel() override;

  /// @brief ステートメントブロックのスコープを生成する．
  const VlScope*
  new_StmtBlockScope(
    const VlScope* parent, ///< [in] 親のスコープ環境
    const PtStmt* pt_stmt  ///< [in] 対応するパース木の要素
  ) override;

  /// @brief generate block を生成する．
  const VlScope*
  new_GenBlock(
    const VlScope* parent, ///< [in] 親のスコープ環境
    const PtItem* pt_item  ///< [in] 対応するパース木の要素
  ) override;

  /// @brief GfBlock 検索用の親の名前付きオブジェクトを作る．
  ElbGfRoot*
  new_GfRoot(
    const VlScope* parent, ///< [in] 親のスコープ環境
    const PtItem* pt_item  ///< [in] 対応するパース木の要素
  ) override;

  /// @brief generate for block を生成する．
  const VlScope*
  new_GfBlock(
    const VlScope* parent, ///< [in] 親のスコープ環境
    const PtItem* pt_item, ///< [in] 対応するパース木の要素
    int gvi                ///< [in] 対応する genvar の値
  ) override;

  /// @brief UDP定義を生成する．
  ElbUdpDefn*
  new_UdpDefn(
    const PtUdp* pt_udp, ///< [in] パース木の UDP 定義
    bool is_protected    ///< [in] プロテクト属性
  ) override;

  /// @brief module を生成する．
  ElbModule*
  new_Module(
    const VlScope* parent,     ///< [in] 親のスコープ
    const PtModule* pt_module, ///< [in] モジュールテンプレート
    const PtItem* pt_head,     ///< [in] パース木のヘッダ定義
    const PtInst* pt_inst      ///< [in] パース木のインスタンス定義
  ) override;

  /// @brief module array を生成する．
  ElbModuleArray*
  new_ModuleArray(
    const VlScope* parent,     ///< [in] 親のスコープ
    const PtModule* pt_module, ///< [in] モジュールテンプレート
    const PtItem* pt_head,     ///< [in] パース木のヘッダ定義
    const PtInst* pt_inst,     ///< [in] パース木のインスタンス定義
    const PtExpr* left,        ///< [in] 範囲の MSB の式
    const PtExpr* right,       ///< [in] 範囲の LSB の式
    int left_val,              ///< [in] 範囲の MSB の値
    int right_val              ///< [in] 範囲の LSB の値
  ) override;

  /// @brief module IO ヘッダを生成する．
  ElbIOHead*
  new_IOHead(
    const VlModule* module,   ///< [in] 親のモジュール
    const PtIOHead* pt_header ///< [in] パース木のIO宣言ヘッダ
  ) override;

  /// @brief タスク/関数 IO ヘッダを生成する．
  ElbIOHead*
  new_IOHead(
    const VlTaskFunc* taskfunc, ///< [in] 親のタスク/関数
    const PtIOHead* pt_header   ///< [in] パース木のIO宣言ヘッダ
  ) override;

  /// @brief 宣言要素のヘッダを生成する．
  ElbDeclHead*
  new_DeclHead(
    const VlScope* parent,     ///< [in] 親のスコープ
    const PtDeclHead* pt_head, ///< [in] パース木の宣言ヘッダ
    const PtExpr* left,        ///< [in] 範囲の左側の式
    const PtExpr* right,       ///< [in] 範囲の右側の式
    int left_val,              ///< [in] 範囲の MSB の値
    int right_val,             ///< [in] 範囲の LSB の値
    bool has_delay = false     ///< [in] 遅延値を持つとき true
  ) override;

  /// @brief 宣言要素のヘッダを生成する．
  ElbDeclHead*
  new_DeclHead(
    const VlScope* parent,     ///< [in] 親のスコープ
    const PtDeclHead* pt_head, ///< [in] パース木の宣言ヘッダ
    bool has_delay = false     ///< [in] 遅延値を持つとき true
  ) override;

  /// @brief 宣言要素のヘッダを生成する．(IODecl 中の宣言用)
  ElbDeclHead*
  new_DeclHead(
    const VlScope* parent,   ///< [in] 親のスコープ
    const PtIOHead* pt_head, ///< [in] パース木のIO宣言ヘッダ
    VpiAuxType aux_type,     ///< [in] 補助的なデータ型
    const PtExpr* left,      ///< [in] 範囲の左側の式
    const PtExpr* right,     ///< [in] 範囲の右側の式
    int left_val,            ///< [in] 範囲の MSB の値
    int right_val            ///< [in] 範囲の LSB の値
  ) override;

  /// @brief 宣言要素のヘッダを生成する．(IODecl 中の宣言用)
  ElbDeclHead*
  new_DeclHead(
    const VlScope* parent,   ///< [in] 親のスコープ
    const PtIOHead* pt_head, ///< [in] パース木のIO宣言ヘッダ
    VpiAuxType aux_type      ///< [in] 補助的なデータ型
  ) override;

  /// @brief 宣言要素のヘッダを生成する．(function の暗黙宣言用)
  ElbDeclHead*
  new_DeclHead(
    const VlScope* parent, ///< [in] 親のスコープ
    const PtItem* pt_item, ///< [in] パース木の関数定義
    const PtExpr* left,    ///< [in] 範囲の左側の式
    const PtExpr* right,   ///< [in] 範囲の右側の式
    int left_val,          ///< [in] 範囲の MSB の値
    int right_val          ///< [in] 範囲の LSB の値
  ) override;

  /// @brief 宣言要素のヘッダを生成する．(function の暗黙宣言用)
  ElbDeclHead*
  new_DeclHead(
    const VlScope* parent, ///< [in] 親のスコープ
    const PtItem* pt_item  ///< [in] パース木の関数定義
  ) override;

  /// @brief 宣言要素を生成する．
  ElbDecl*
  new_Decl(
    ElbDeclHead* head,           ///< [in] ヘッダ
    const PtNamedBase* pt_item,  ///< [in] パース木の宣言要素
    const VlExpr* init = nullptr ///< [in] 初期値
  ) override;

  /// @brief 暗黙のネットを生成する．
  ElbDecl*
  new_ImpNet(
    const VlScope* parent, ///< [in] 親のスコープ
    const PtExpr* pt_expr, ///< [in] パース木のプライマリ式
    VpiNetType net_type    ///< [in] ネットの型
  ) override;

  /// @brief 宣言要素の配列を生成する．
  const VlDeclArray*
  new_DeclArray(
    ElbDeclHead* head,                   ///< [in] ヘッダ
    const PtNamedBase* pt_item,          ///< [in] パース木の宣言要素
    const vector<ElbRangeSrc>& range_src ///< [in] 範囲の配列
  ) override;

  /// @brief parameter 宣言のヘッダを生成する(範囲指定なし)．
  ElbParamHead*
  new_ParamHead(
    const VlScope* parent,    ///< [in] 親のスコープ
    const PtDeclHead* pt_head ///< [in] パース木の宣言ヘッダ
  ) override;

  /// @brief parameter 宣言のヘッダを生成する(範囲指定あり)．
  ElbParamHead*
  new_ParamHead(
    const VlScope* parent,     ///< [in] 親のスコープ
    const PtDeclHead* pt_head, ///< [in] パース木の宣言ヘッダ
    const PtExpr* left,        ///< [in] 範囲の左側の式
    const PtExpr* right,       ///< [in] 範囲の右側の式
    int left_val,              ///< [in] 範囲の MSB の値
    int right_val              ///< [in] 範囲の LSB の値
  ) override;

  /// @brief parameter 宣言を生成する．
  ElbParameter*
  new_Parameter(
    ElbParamHead* head,         ///< [in] ヘッダ
    const PtNamedBase* pt_item, ///< [in] パース木の宣言要素
    bool is_local = false       ///< [in] localparam の時 true
  ) override;

  /// @brief genvar を生成する．
  ElbGenvar*
  new_Genvar(
    const VlScope* parent,     ///< [in] 親のスコープ環境
    const PtDeclItem* pt_item, ///< [in] 対応するパース木の要素
    int val                    ///< [in] 初期値
  ) override;

  /// @brief continuous assignment のヘッダを生成する．
  ElbCaHead*
  new_CaHead(
    const VlModule* module,        ///< [in] 親のモジュール
    const PtItem* pt_head,         ///< [in] パース木のヘッダ定義
    const VlDelay* delay = nullptr ///< [in] 遅延値
  ) override;

  /// @brief continuous assignment を生成する．
  const VlContAssign*
  new_ContAssign(
    ElbCaHead* head,      ///< [in] ヘッダ
    const PtBase* pt_obj, ///< [in] 対応するパース木中の定義要素
    const VlExpr* lhs,    ///< [in] 左辺式
    const VlExpr* rhs     ///< [in] 右辺式
  ) override;

  /// @brief net 宣言中の continuous assignment を生成する．
  const VlContAssign*
  new_ContAssign(
    const VlModule* module, ///< [in] 親のモジュール
    const PtBase* pt_obj,   ///< [in] パース木の定義要素
    const VlExpr* lhs,      ///< [in] 左辺式
    const VlExpr* rhs       ///< [in] 右辺式
  ) override;

  /// @brief パラメータ割り当て文を生成する．
  const VlParamAssign*
  new_ParamAssign(
    const VlModule* module,  ///< [in] 親のモジュール
    const PtBase* pt_obj,    ///< [in] パース木の定義要素
    ElbParameter* param,     ///< [in] 対象の parameter
    const PtExpr* rhs_expr,  ///< [in] 割り当て式の右辺
    const VlValue& rhs_value ///< [in] 右辺の値
  ) override;

  /// @brief 名前によるパラメータ割り当て文を生成する．
  const VlParamAssign*
  new_NamedParamAssign(
    const VlModule* module,  ///< [in] 親のモジュール
    const PtBase* pt_obj,    ///< [in] パース木の定義要素
    ElbParameter* param,     ///< [in] 対象の parameter
    const PtExpr* rhs_expr,  ///< [in] 割り当て式の右辺
    const VlValue& rhs_value ///< [in] 右辺の値
  ) override;

  /// @brief defparam 文を生成する．
  const VlDefParam*
  new_DefParam(
    const VlModule* module,        ///< [in] 親のモジュール
    const PtItem* pt_header,       ///< [in] パース木の defparam ヘッダ
    const PtDefParam* pt_defparam, ///< [in] パース木の defparam 定義
    ElbParameter* param,           ///< [in] 対象の parameter
    const PtExpr* rhs_expr,        ///< [in] 割り当て式の右辺
    const VlValue& rhs_value       ///< [in] 右辺の値
  ) override;

  /// @brief ゲートプリミティブのヘッダを生成する．
  ElbPrimHead*
  new_PrimHead(
    const VlScope* parent,   ///< [in] 親のスコープ
    const PtItem* pt_header, ///< [in] パース木の定義
    bool has_delay           ///< [in] 遅延値を持つとき true
  ) override;

  /// @brief UDPプリミティブのヘッダを生成する．
  ElbPrimHead*
  new_UdpHead(
    const VlScope* parent,   ///< [in] 親のスコープ
    const PtItem* pt_header, ///< [in] パース木の定義
    const VlUdpDefn* udp,    ///< [in] 定義
    bool has_delay           ///< [in] 遅延値を持つとき true
  ) override;

  /// @brief セルプリミティブのヘッダを生成する．
  ElbPrimHead*
  new_CellHead(
    const VlScope* parent,   ///< [in] 親のスコープ
    const PtItem* pt_header, ///< [in] パース木の定義
    const ClibCell& cell     ///< [in] セル
  ) override;

  /// @brief プリミティブインスタンスを生成する．
  ElbPrimitive*
  new_Primitive(
    ElbPrimHead* head,    ///< [in] ヘッダ
    const PtInst* pt_inst ///< [in] インスタンス定義
  ) override;

  /// @brief プリミティブ配列インスタンスを生成する．
  ElbPrimArray*
  new_PrimitiveArray(
    ElbPrimHead* head,     ///< [in] ヘッダ
    const PtInst* pt_inst, ///< [in] インスタンス定義
    const PtExpr* left,    ///< [in] 範囲の MSB の式
    const PtExpr* right,   ///< [in] 範囲の LSB の式
    int left_val,          ///< [in] 範囲の MSB の値
    int right_val          ///< [in] 範囲の LSB の値
  ) override;

  /// @brief セルプリミティブインスタンスを生成する．
  ElbPrimitive*
  new_CellPrimitive(
    ElbPrimHead* head,    ///< [in] ヘッダ
    const ClibCell& cell, ///< [in] セル
    const PtInst* pt_inst ///< [in] インスタンス定義
  ) override;

  /// @brief セルプリミティブ配列インスタンスを生成する．
  ElbPrimArray*
  new_CellPrimitiveArray(
    ElbPrimHead* head,     ///< [in] ヘッダ
    const ClibCell& cell,  ///< [in] セル
    const PtInst* pt_inst, ///< [in] インスタンス定義
    const PtExpr* left,    ///< [in] 範囲の MSB の式
    const PtExpr* right,   ///< [in] 範囲の LSB の式
    int left_val,          ///< [in] 範囲の MSB の値
    int right_val          ///< [in] 範囲の LSB の値
  ) override;

  /// @brief function を生成する．
  ElbTaskFunc*
  new_Function(
    const VlScope* parent, ///< [in] 親のスコープ
    const PtItem* pt_item, ///< [in] パース木の定義
    const PtExpr* left,    ///< [in] 範囲の MSB の式
    const PtExpr* right,   ///< [in] 範囲の LSB の式
    int left_val,          ///< [in] 範囲の MSB の値
    int right_val,         ///< [in] 範囲の LSB の値
    bool const_func        ///< [in] 定数関数フラグ
  ) override;

  /// @brief function を生成する．
  ElbTaskFunc*
  new_Function(
    const VlScope* parent, ///< [in] 親のスコープ
    const PtItem* pt_item, ///< [in] パース木の定義
    bool const_func        ///< [in] 定数関数フラグ
  ) override;

  /// @brief task を生成する．
  ElbTaskFunc*
  new_Task(
    const VlScope* parent, ///< [in] 親のスコープ
    const PtItem* pt_item  ///< [in] パース木の定義
  ) override;

  /// @brief プロセス文を生成する．
  ElbProcess*
  new_Process(
    const VlScope* parent, ///< [in] 親のスコープ
    const PtItem* pt_item  ///< [in] パース木の要素定義
  ) override;

  /// @brief 代入文を生成する．
  const VlStmt*
  new_Assignment(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in]	親のプロセス (or nullptr)
    const PtStmt* pt_stmt,    ///< [in]	パース木のステートメント定義
    const VlExpr* lhs,        ///< [in]	左辺の式
    const VlExpr* rhs,        ///< [in]	右辺の式
    bool block,               ///< [in]	ブロッキング代入の時 true
    const VlControl* control  ///< [in]	コントロール
    = nullptr
  ) override;

  /// @brief assign ステートメントを生成する．
  const VlStmt*
  new_AssignStmt(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in]	親のプロセス (or nullptr)
    const PtStmt* pt_stmt,    ///< [in]	パース木のステートメント定義
    const VlExpr* lhs,        ///< [in]	左辺の式
    const VlExpr* rhs         ///< [in]	右辺の式
  ) override;

  /// @brief deassign ステートメントを生成する．
  const VlStmt*
  new_DeassignStmt(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in]	親のプロセス (or nullptr)
    const PtStmt* pt_stmt,    ///< [in]	パース木のステートメント定義
    const VlExpr* lhs         ///< [in]	左辺の式
  ) override;

  /// @brief force ステートメントを生成する．
  const VlStmt*
  new_ForceStmt(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in]	親のプロセス (or nullptr)
    const PtStmt* pt_stmt,    ///< [in]	パース木のステートメント定義
    const VlExpr* lhs,        ///< [in]	左辺の式
    const VlExpr* rhs         ///< [in]	右辺の式
  ) override;

  /// @brief release ステートメントを生成する．
  const VlStmt*
  new_ReleaseStmt(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in]	親のプロセス (or nullptr)
    const PtStmt* pt_stmt,    ///< [in]	パース木のステートメント定義
    const VlExpr* lhs         ///< [in]	左辺の式
  ) override;

  /// @brief begin ブロックを生成する．
  const VlStmt*
  new_Begin(
    const VlScope* parent,                 ///< [in] 親のスコープ
    const VlProcess* process,              ///< [in] 親のプロセス (or nullptr)
    const PtStmt* pt_stmt,                 ///< [in] パース木のステートメント定義
    const vector<const VlStmt*>& stmt_list ///< [in] 子のステートメントリスト
  ) override;

  /// @brief fork ブロックを生成する．
  const VlStmt*
  new_Fork(
    const VlScope* parent,                 ///< [in] 親のスコープ
    const VlProcess* process,		   ///< [in] 親のプロセス (or nullptr)
    const PtStmt* pt_stmt,		   ///< [in] パース木のステートメント定義
    const vector<const VlStmt*>& stmt_list ///< [in] 子のステートメントリスト
  ) override;

  /// @brief 名前付き begin ブロックを生成する．
  const VlStmt*
  new_NamedBegin(
    const VlScope* block,                  ///< [in] 親のスコープ
    const VlProcess* process,		   ///< [in] 親のプロセス (or nullptr)
    const PtStmt* pt_stmt,		   ///< [in] パース木のステートメント定義
    const vector<const VlStmt*>& stmt_list ///< [in] 子のステートメントリスト
  ) override;


  /// @brief 名前付き fork ブロックを生成する．
  const VlStmt*
  new_NamedFork(
    const VlScope* block,                  ///< [in] 親のスコープ
    const VlProcess* process,		   ///< [in] 親のプロセス (or nullptr)
    const PtStmt* pt_stmt,		   ///< [in] パース木のステートメント定義
    const vector<const VlStmt*>& stmt_list ///< [in] 子のステートメントリスト
  ) override;

  /// @brief while 文を生成する．
  const VlStmt*
  new_WhileStmt(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in] 親のプロセス (or nullptr)
    const PtStmt* pt_stmt,    ///< [in] パース木のステートメント定義
    const VlExpr* cond,	      ///< [in] 条件式
    const VlStmt* stmt        ///< [in] 子のステートメントリスト
  ) override;

  /// @brief repeat 文を生成する．
  const VlStmt*
  new_RepeatStmt(
    const VlScope* parent,     ///< [in] 親のスコープ
    const VlProcess* process,  ///< [in] 親のプロセス (or nullptr)
    const PtStmt* pt_stmt,     ///< [in] パース木のステートメント定義
    const VlExpr* cond,	       ///< [in] 条件式
    const VlStmt* stmt	       ///< [in] 子のステートメントリスト
  ) override;

  /// @brief wait 文を生成する．
  const VlStmt*
  new_WaitStmt(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in] 親のプロセス (or nullptr)
    const PtStmt* pt_stmt,    ///< [in] パース木のステートメント定義
    const VlExpr* cond,	      ///< [in] 条件式
    const VlStmt* stmt	      ///< [in] 子のステートメントリスト
  ) override;

  /// @brief for 文を生成する．
  const VlStmt*
  new_ForStmt(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in] 親のプロセス (or nullptr)
    const PtStmt* pt_stmt,    ///< [in] パース木のステートメント定義
    const VlExpr* cond,	      ///< [in] 条件式
    const VlStmt* init_stmt,  ///< [in] 初期化式
    const VlStmt* inc_stmt,   ///< [in]	増加式
    const VlStmt* stmt        ///< [in] 本体のステートメント
  ) override;

  /// @brief forever 文を生成する．
  const VlStmt*
  new_ForeverStmt(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in]	親のプロセス (or nullptr)
    const PtStmt* pt_stmt,    ///< [in]	パース木のステートメント定義
    const VlStmt* stmt        ///< [in]	本体のステートメント
  ) override;

  /// @brief if 文を生成する．
  const VlStmt*
  new_IfStmt(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in]	親のプロセス (or nullptr)
    const PtStmt* pt_stmt,    ///< [in]	パース木のステートメント定義
    const VlExpr* cond,       ///< [in]	条件式
    const VlStmt* stmt,       ///< [in]	本体のステートメント
    const VlStmt* else_stmt   ///< [in]	else節のステートメント
  ) override;

  /// @brief case 文を生成する．
  const VlStmt*
  new_CaseStmt(
    const VlScope* parent,                     ///< [in] 親のスコープ
    const VlProcess* process,                  ///< [in] 親のプロセス (or nullptr)
    const PtStmt* pt_stmt,                     ///< [in] パース木のステートメント定義
    const VlExpr* expr,                        ///< [in] 条件式
    const vector<const VlCaseItem*>& item_list ///< [in] caseitem のリスト
  ) override;

  /// @brief caseitem を生成する．
  const VlCaseItem*
  new_CaseItem(
    const PtCaseItem* pt_item,          ///< [in] パース木の caseitem 要素
    const vector<ElbExpr*>& label_list, ///< [in] ラベルを表す式のリスト
    const VlStmt* body                  ///< [in] 本体のステートメント
  ) override;

  /// @brief イベント文を生成する．
  const VlStmt*
  new_EventStmt(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in]	親のプロセス (or nullptr)
    const PtStmt* pt_stmt,    ///< [in]	パース木のステートメント定義
    ElbExpr* named_event      ///< [in]	対象のイベント
  ) override;

  /// @brief nullptr ステートメントを生成する．
  const VlStmt*
  new_NullStmt(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in]	親のプロセス (or nullptr)
    const PtStmt* pt_stmt     ///< [in] パース木のステートメント定義
  ) override;

  /// @brief タスクコール文を生成する．
  const VlStmt*
  new_TaskCall(
    const VlScope* parent,            ///< [in] 親のスコープ
    const VlProcess* process,         ///< [in]	親のプロセス (or nullptr)
    const PtStmt* pt_stmt,            ///< [in]	パース木のステートメント定義
    const VlTaskFunc* task,           ///< [in]	対象のタスク
    const vector<ElbExpr*>& arg_array ///< [in]	引数の配列
  ) override;

  /// @brief システムタスクコール文を生成する．
  const VlStmt*
  new_SysTaskCall(
    const VlScope* parent,            ///< [in] 親のスコープ
    const VlProcess* process,         ///< [in]	親のプロセス (or nullptr)
    const PtStmt* pt_stmt,            ///< [in]	パース木のステートメント定義
    const VlUserSystf* user_systf,    ///< [in]	システムタスク
    const vector<ElbExpr*>& arg_array ///< [in]	引数の配列
  ) override;

  /// @brief disable 文を生成する．
  const VlStmt*
  new_DisableStmt(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in]	親のプロセス (or nullptr)
    const PtStmt* pt_stmt,    ///< [in]	パース木のステートメント定義
    const VlScope* target     ///< [in]	対象のスコープ
  ) override;

  /// @brief コントロール文を生成する．
  const VlStmt*
  new_CtrlStmt(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in]	親のプロセス (or nullptr)
    const PtStmt* pt_stmt,    ///< [in]	パース木のステートメント定義
    const VlControl* control, ///< [in]	コントロール
    const VlStmt* stmt        ///< [in]	本体のステートメント
  ) override;

  /// @brief 遅延コントロールを生成する．
  /// @param[in] pt_control パース木の定義要素
  /// @param[in] delay 遅延式
  const VlControl*
  new_DelayControl(
    const PtControl* pt_control,
    ElbExpr* delay
  ) override;

  /// @brief イベントコントロールを生成する．
  const VlControl*
  new_EventControl(
    const PtControl* pt_control,       ///< [in] パース木の定義要素
    const vector<ElbExpr*>& event_list ///< [in] イベントリスト
  ) override;

  /// @brief リピートコントロールを生成する．
  const VlControl*
  new_RepeatControl(
    const PtControl* pt_control,       ///< [in] パース木の定義要素
    ElbExpr* rep,                      ///< [in] 繰り返し式
    const vector<ElbExpr*>& event_list ///< [in] イベントリスト
  ) override;

  /// @brief 単項演算子を生成する．
  ElbExpr*
  new_UnaryOp(
    const PtExpr* pt_expr, ///< [in] パース木の定義要素
    VpiOpType op_type,     ///< [in] 演算子のタイプ
    ElbExpr* opr1          ///< [in] オペランド
  ) override;

  /// @brief 2項演算子を生成する．
  ElbExpr*
  new_BinaryOp(
    const PtExpr* pt_expr, ///< [in] パース木の定義要素
    VpiOpType op_type,     ///< [in] 演算子のタイプ
    ElbExpr* opr1,         ///< [in] オペランド
    ElbExpr* opr2          ///< [in] オペランド
  ) override;

  /// @brief 3項演算子を生成する．
  ElbExpr*
  new_TernaryOp(
    const PtExpr* pt_expr, ///< [in] パース木の定義要素
    VpiOpType op_type,	   ///< [in] 演算子のタイプ
    ElbExpr* opr1,	   ///< [in] オペランド
    ElbExpr* opr2,	   ///< [in] オペランド
    ElbExpr* opr3          ///< [in] オペランド
  ) override;

  /// @brief 連結演算子を生成する．
  ElbExpr*
  new_ConcatOp(
    const PtExpr* pt_expr,           ///< [in] パース木の定義要素
    const vector<ElbExpr*>& opr_list ///< [in] オペランドのリスト
  ) override;

  /// @brief 反復連結演算子を生成する．
  ElbExpr*
  new_MultiConcatOp(
    const PtExpr* pt_expr,           ///< [in] パース木の定義要素
    SizeType rep_num,                ///< [in] 繰り返し数を表す式
    ElbExpr* rep_expr,               ///< [in] 繰り返し数
    const vector<ElbExpr*>& opr_list ///< [in] オペランドのリスト
  ) override;

  /// @brief プライマリ式を生成する．
  ElbExpr*
  new_Primary(
    const PtExpr* pt_expr, ///< [in] パース木の定義要素
    const VlDecl* obj      ///< [in] 本体のオブジェクト
  ) override;

  /// @brief プライマリ式を生成する(net decl の初期値用)．
  ElbExpr*
  new_Primary(
    const PtDeclItem* pt_item, ///< [in] パース木の定義要素
    const VlDecl* obj          ///< [in] 本体のオブジェクト
  ) override;

  /// @brief プライマリ式を生成する．
  ElbExpr*
  new_Primary(
    const PtExpr* pt_expr, ///< [in] パース木の定義要素
    ElbParameter* obj      ///< [in] 本体のオブジェクト
  ) override;

  /// @brief プライマリ式を生成する(配列要素版)．
  ElbExpr*
  new_Primary(
    const PtExpr* pt_expr,             ///< [in] パース木の定義要素
    const VlDeclArray* obj,            ///< [in] 本体のオブジェクト
    const vector<ElbExpr*>& index_list ///< [in] インデックスのリスト
  ) override;

  /// @brief プライマリ式を生成する(固定インデックスの配列要素版)．
  ElbExpr*
  new_Primary(
    const PtExpr* pt_expr,  ///< [in] パース木の定義要素
    const VlDeclArray* obj, ///< [in] 本体のオブジェクト
    SizeType offset         ///< [in] オフセット
  ) override;

  /// @brief 固定ビット選択式を生成する．
  ElbExpr*
  new_BitSelect(
    const PtExpr* pt_expr,   ///< [in] パース木の定義要素
    ElbExpr* expr,           ///< [in] 本体のオブジェクト
    const PtExpr* bit_index, ///< [in] ビット選択式
    int bit_index_val        ///< [in] ビット選択式の値
  ) override;

  /// @brief 固定ビット選択式を生成する．
  ElbExpr*
  new_BitSelect(
    const PtExpr* pt_expr, ///< [in] パース木の定義要素
    ElbExpr* expr,         ///< [in] 本体の式
    int bit_index_val      ///< [in] ビット選択式の値
  ) override;

  /// @brief 可変ビット選択式を生成する．
  ElbExpr*
  new_BitSelect(
    const PtExpr* pt_expr, ///< [in] パース木の定義要素
    ElbExpr* expr,         ///< [in] 本体のオブジェクト
    ElbExpr* bit_index     ///< [in] ビット選択式
  ) override;

  /// @brief 固定部分選択式を生成する．
  ElbExpr*
  new_PartSelect(
    const PtExpr* pt_expr, ///< [in] パース木の定義要素
    ElbExpr* parent_expr,  ///< [in] 本体の式
    const PtExpr* index1,  ///< [in] パート選択式1
    const PtExpr* index2,  ///< [in] パート選択式2
    int index1_val,        ///< [in] パート選択式1の値
    int index2_val         ///< [in] パート選択式2の値
  ) override;

  /// @brief 固定部分選択式を生成する．
  ElbExpr*
  new_PartSelect(
    const PtExpr* pt_expr, ///< [in] パース木の定義要素
    ElbExpr* parent_expr,  ///< [in] 本体の式
    int index1,            ///< [in] パート選択式1
    int index2             ///< [in] パート選択式2
  ) override;

  /// @brief 可変部分選択式を生成する．
  ElbExpr*
  new_PlusPartSelect(
    const PtExpr* pt_expr,    ///< [in] パース木の定義要素
    ElbExpr* obj,             ///< [in]	本体のオブジェクト
    ElbExpr* base,            ///< [in]	範囲のベースアドレスを表す式
    const PtExpr* range_expr, ///< [in]	範囲を表す式
    SizeType range_val        ///< [in]	範囲の値
  ) override;

  /// @brief 可変部分選択式を生成する．
  ElbExpr*
  new_MinusPartSelect(
    const PtExpr* pt_expr,    ///< [in] パース木の定義要素
    ElbExpr* obj,             ///< [in]	本体のオブジェクト
    ElbExpr* base,            ///< [in]	範囲のベースアドレスを表す式
    const PtExpr* range_expr, ///< [in]	範囲を表す式
    SizeType range_val        ///< [in]	範囲の値
  ) override;

  /// @brief 定数式を生成する．
  ElbExpr*
  new_Constant(
    const PtExpr* pt_expr ///< [in] パース木の定義要素
  ) override;

  /// @brief genvar 起因の定数式を生成する．
  ElbExpr*
  new_GenvarConstant(
    const PtExpr* pt_primary, ///< [in] パース木の定義要素
    int val                   ///< [in]	値
  ) override;

  /// @brief 関数呼び出し式を生成する．
  ElbExpr*
  new_FuncCall(
    const PtExpr* pt_expr,           ///< [in] パース木の定義要素
    const VlTaskFunc* func,          ///< [in] 関数
    const vector<ElbExpr*>& arg_list ///< [in] 引数のリスト
  ) override;

  /// @brief システム関数呼び出し式を生成する．
  ElbExpr*
  new_SysFuncCall(
    const PtExpr* pt_obj,            ///< [in] パース木の定義要素
    const VlUserSystf* user_systf,   ///< [in] システム関数
    const vector<ElbExpr*>& arg_list ///< [in] 引数のリスト
  ) override;

  /// @brief システム関数/システムタスクの引数を生成する．
  ElbExpr*
  new_ArgHandle(
    const PtExpr* pt_expr, ///< [in] パース木中で参照している要素
    const VlScope* arg     ///< [in] 引数本体
  ) override;

  /// @brief システム関数/システムタスクの引数を生成する．
  ElbExpr*
  new_ArgHandle(
    const PtExpr* pt_expr, ///< [in] パース木中で参照している要素
    const VlPrimitive* arg ///< [in] 引数本体
  ) override;

  /// @brief システム関数/システムタスクの引数を生成する．
  ElbExpr*
  new_ArgHandle(
    const PtExpr* pt_expr, ///< [in] パース木中で参照している要素
    const VlDeclArray* arg ///< [in] 引数本体
  ) override;

  /// @brief 連結演算子の左辺式を生成する．
  ElbExpr*
  new_Lhs(
    const PtExpr* pt_expr,                 ///< [in] パース木の定義要素
    const vector<ElbExpr*>& opr_array,     ///< [in] オペランドを格納する配列
    const vector<ElbExpr*>& lhs_elem_array ///< [in] 左辺の要素の配列
  ) override;

  /// @brief 遅延値を生成する．
  const VlDelay*
  new_Delay(
    const PtBase* pt_obj,             ///< [in] パース木の定義要素
    const vector<ElbExpr*>& expr_list ///< [in] 式のリスト
  ) override;

  /// @brief attribute instance のリストを生成する．
  const VlAttribute*
  new_Attribute(
    const PtAttrSpec* pt_attr, ///< [in] パース木の定義要素
    const VlExpr* expr,        ///< [in] 値
    bool def                   ///< [in] 定義側の属性の時 true
  ) override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  std::uint32_t mUdpDefnNum;
  std::uint32_t mModuleArrayNum;
  std::uint32_t mModule1Num;
  std::uint32_t mModule2Num;
  std::uint32_t mPortNum;
  std::uint32_t mIODeclMRNum;
  std::uint32_t mIODeclMNum;
  std::uint32_t mParameterNNum;
  std::uint32_t mParameterRNum;
  std::uint32_t mParameterTNum;
  std::uint32_t mSpecParamNum;
  std::uint32_t mSpecParamRNum;
  std::uint32_t mNetArrayNum;
  std::uint32_t mNetNum;
  std::uint32_t mNetInfoNum;
  std::uint32_t mNetInfoRNum;
  std::uint32_t mRegArrayNum;
  std::uint32_t mRegNum;
  std::uint32_t mIntArrayNum;
  std::uint32_t mRealArrayNum;
  std::uint32_t mTimeArrayNum;
  std::uint32_t mIntNum;
  std::uint32_t mTimeNum;
  std::uint32_t mRealNum;
  std::uint32_t mNeArrayNum;
  std::uint32_t mNamedEventNum;
  std::uint32_t mContAssignNum;
  std::uint32_t mDefParamNum;
  std::uint32_t mParamAssignNum;
  std::uint32_t mPrimArrayNum;
  std::uint32_t mPrimitiveNum;
  std::uint32_t mTaskNum;
  std::uint32_t mFunctionNum;
  std::uint32_t mFunctionRNum;
  std::uint32_t mFunctionTNum;
  std::uint32_t mIODeclTFRNum;
  std::uint32_t mIODeclTFNum;
  std::uint32_t mInitialNum;
  std::uint32_t mAlwaysNum;
  std::uint32_t mBeginNum;
  std::uint32_t mForkNum;
  std::uint32_t mNamedBeginNum;
  std::uint32_t mNamedForkNum;
  std::uint32_t mIfStmtNum;
  std::uint32_t mIfElseStmtNum;
  std::uint32_t mWhileStmtNum;
  std::uint32_t mRepeatStmtNum;
  std::uint32_t mWaitStmtNum;
  std::uint32_t mCaseStmtNum;
  std::uint32_t mCaseItemNum;
  std::uint32_t mForStmtNum;
  std::uint32_t mDelayCtrlStmtNum;
  std::uint32_t mEventCtrlStmtNum;
  std::uint32_t mEventStmtNum;
  std::uint32_t mAssignmentNum;
  std::uint32_t mAssignStmtNum;
  std::uint32_t mDeassignStmtNum;
  std::uint32_t mDisableStmtNum;
  std::uint32_t mTaskCallNum;
  std::uint32_t mSysTaskCallNum;
  std::uint32_t mForeverStmtNum;
  std::uint32_t mForceStmtNum;
  std::uint32_t mReleaseStmtNum;
  std::uint32_t mNullStmtNum;
  std::uint32_t mPrimaryNum;
  std::uint32_t mBitSelectNum;
  std::uint32_t mGenericBitSelectNum;
  std::uint32_t mConstPartSelectNum;
  std::uint32_t mPlusPartSelectNum;
  std::uint32_t mMinusPartSelectNum;
  std::uint32_t mNotOpNum;
  std::uint32_t mBitNegOpNum;
  std::uint32_t mUnaryAndOpNum;
  std::uint32_t mUnaryNandOpNum;
  std::uint32_t mUnaryOrOpNum;
  std::uint32_t mUnaryNorOpNum;
  std::uint32_t mUnaryXorOpNum;
  std::uint32_t mUnaryXNorOpNum;
  std::uint32_t mPlusOpNum;
  std::uint32_t mMinusOpNum;
  std::uint32_t mPosedgeOpNum;
  std::uint32_t mNegedgeOpNum;
  std::uint32_t mEqOpNum;
  std::uint32_t mNeqOpNum;
  std::uint32_t mCaseEqOpNum;
  std::uint32_t mCaseNeqOpNum;
  std::uint32_t mGtOpNum;
  std::uint32_t mGeOpNum;
  std::uint32_t mLtOpNum;
  std::uint32_t mLeOpNum;
  std::uint32_t mLogAndOpNum;
  std::uint32_t mLogOrOpNum;
  std::uint32_t mBitAndOpNum;
  std::uint32_t mBitOrOpNum;
  std::uint32_t mBitXorOpNum;
  std::uint32_t mBitXNorOpNum;
  std::uint32_t mAddOpNum;
  std::uint32_t mSubOpNum;
  std::uint32_t mMultOpNum;
  std::uint32_t mDivOpNum;
  std::uint32_t mModOpNum;
  std::uint32_t mPowerOpNum;
  std::uint32_t mLShiftOpNum;
  std::uint32_t mRShiftOpNum;
  std::uint32_t mArithLShiftOpNum;
  std::uint32_t mArithRShiftOpNum;
  std::uint32_t mConditionOpNum;
  std::uint32_t mMinTypMaxOpNum;
  std::uint32_t mConcatOpNum;
  std::uint32_t mMultiConcatOpNum;
  std::uint32_t mListOpNum;
  std::uint32_t mEventOrOpNum;
  std::uint32_t mFuncCallNum;
  std::uint32_t mSysFuncCallNum;
  std::uint32_t mIntConstNum;
  std::uint32_t mBitVectorConstNum;
  std::uint32_t mRealConstNum;
  std::uint32_t mStringConstNum;
  std::uint32_t mGenvarConstantNum;
  std::uint32_t mRangeNum;
  std::uint32_t mDelayControlNum;
  std::uint32_t mEventControlNum;
  std::uint32_t mRepeatControlNum;
  std::uint32_t mAttributeNum;
  std::uint32_t mStmtBlockScopeNum;
  std::uint32_t mGenBlockNum;
  std::uint32_t mGenForBlockNum;
  std::uint32_t mModuleInfoNum;
};

END_NAMESPACE_YM_VERILOG

#endif // EIFACTORY_H
