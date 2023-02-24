#ifndef ITEMGEN_H
#define ITEMGEN_H

/// @file ItemGen.h
/// @brief ItemGen のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.


#include "ym/verilog.h"
#include "ym/pt/PtP.h"
#include "ym/clib.h"
#include "ElbProxy.h"
#include "elaborator/ElbFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

struct DefParamStub;

//////////////////////////////////////////////////////////////////////
/// @class ItemGen ItemGen.h "ItemGen.h"
/// @brief 構成要素を生成するクラス
//////////////////////////////////////////////////////////////////////
class ItemGen :
  public ElbProxy
{
public:

  /// @brief コンストラクタ
  /// @param[in] elab 生成器
  /// @param[in] elb_mgr Elbオブジェクトを管理するクラス
  ItemGen(Elaborator& elab,
	  ElbMgr& elb_mgr);

  /// @brief デストラクタ
  ~ItemGen();


public:
  //////////////////////////////////////////////////////////////////////
  // ItemGen の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief スコープに関係する要素を実体化する．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_item_array 要素定義のリスト
  void
  phase1_items(const VlScope* parent,
	       const vector<const PtItem*>& pt_item_array);

  /// @brief defparam 文によるパラメータ割り当てを行う．
  /// @param[in] stub defparam 文の情報
  /// @param[in] ulimit スコープの上限
  /// @return 該当するパラメータが見つかったら true を返す．
  bool
  defparam_override(const DefParamStub& stub,
		    const VlScope* ulimit);

  /// @brief constant function の生成を行う．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_function 関数定義
  const VlTaskFunc*
  instantiate_constant_function(const VlScope* parent,
				const PtItem* pt_function);


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief スコープに関係する要素を実体化する．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_item 要素定義
  void
  phase1_item(const VlScope* parent,
	      const PtItem* pt_item);

  /// @brief continous assignment に関連した式の名前解決を行う．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_head ヘッダ
  void
  instantiate_cont_assign(const VlScope* parent,
			  const PtItem* pt_head);

  /// @brief process 文の生成を行う．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_item パース木の定義
  void
  instantiate_process(const VlScope* parent,
		      const PtItem* pt_item);

  /// @brief task/function の生成を行う．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_tf タスク/関数定義
  void
  phase1_tf(const VlScope* parent,
	      const PtItem* pt_tf);

  /// @param[in] task/function 内の宣言要素の生成を行う．
  /// @param[in] taskfunc タスク/関数本体
  /// @param[in] pt_item パース木の関数定義
  void
  phase2_tf(ElbTaskFunc* taskfunc,
	    const PtItem* pt_item);

  /// @param[in] task/function 内のステートメントの生成を行う．
  /// @param[in] taskfunc タスク/関数本体
  /// @param[in] pt_item パース木の関数定義
  void
  phase3_tf(ElbTaskFunc* task,
	    const PtItem* pt_item);

  /// @brief module/Udp instance の生成を行う
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_head ヘッダ
  void
  phase1_muheader(const VlScope* parent,
		  const PtItem* pt_head);

  /// @brief module instance の生成を行う．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_head ヘッダ
  /// @param[in] pt_module モジュールの構文木要素
  void
  phase1_module(const VlScope* parent,
		const PtItem* pt_head,
		const PtModule* pt_module);

  /// @brief UDP instance の生成を行う．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_head ヘッダ
  /// @param[in] udpdefn UDP
  void
  phase1_udp(const VlScope* parent,
	     const PtItem* pt_head,
	     const VlUdpDefn* udpdefn);

  /// @brief cell instance の生成を行う．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_head ヘッダ
  /// @param[in] cell_id セル番号
  void
  phase1_cell(const VlScope* parent,
	      const PtItem* pt_head,
	      ClibCell cell);

  /// @brief module array のインスタンス化を行う．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_module モジュール定義
  /// @param[in] pt_head ヘッダ
  /// @param[in] pt_inst インスタンス定義
  void
  phase1_module_array(const VlScope* parent,
		      const PtModule* pt_module,
		      const PtItem* pt_head,
		      const PtInst* pt_inst);

  /// @brief module array instance の入出力端子の接続を行う．
  /// @param[in] module_array モジュール配列
  /// @param[in] pt_module モジュール定義
  /// @param[in] pt_inst インスタンス定義
  void
  link_module_array(ElbModuleArray* module_array,
		    const PtModule* pt_module,
		    const PtInst* pt_inst);

  /// @brief module instance の入出力端子の接続を行う．
  /// @param[in] module モジュール
  /// @param[in] pt_module モジュール定義
  /// @param[in] pt_inst インスタンス定義
  void
  link_module(ElbModule* module,
	      const PtModule* pt_module,
	      const PtInst* pt_inst);

  /// @brief gate instance の生成を行う
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_head ヘッダ
  void
  instantiate_gateheader(const VlScope* parent,
			 const PtItem* pt_head);

  /// @brief UDP instance の生成を行う
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_head ヘッダ
  /// @param[in] udpdefn UDP
  void
  instantiate_udpheader(const VlScope* parent,
			const PtItem* pt_head,
			const VlUdpDefn* udpdefn);

  /// @brief セル instance の生成を行う
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_head ヘッダ
  /// @param[in] cell セル
  void
  instantiate_cell(const VlScope* parent,
		   const PtItem* pt_head,
		   ClibCell cell);

  /// @brief gate delay の生成を行う
  /// @param[in] prim_head ゲートプリミティブのヘッダ
  /// @param[in] pt_delay パース木の遅延式
  void
  link_gate_delay(ElbPrimHead* prim_head,
		  const PtDelay* pt_delay);

  /// @brief UDP instance に付随する遅延式の生成を行う．
  /// @param[in] parent 親のスコープ
  /// @param[in] prim_head ヘッダ
  /// @param[in] pt_head パース木のインスタンス定義ヘッダ
  void
  link_udp_delay(ElbPrimHead* prim_head,
		 const PtItem* pt_head);

  /// @brief gate array instance で使われている式の名前解決を行う．
  /// @param[in] prim_array プリミティブ配列
  /// @param[in] pt_inst インスタンス定義
  void
  link_prim_array(ElbPrimArray* prim_array,
		  const PtInst* pt_inst);

  /// @brief gate instance で使われている式の名前解決を行う．
  /// @param[in] primitive プリミティブ配列
  /// @param[in] pt_inst インスタンス定義
  void
  link_primitive(ElbPrimitive* primitive,
		 const PtInst* pt_inst);

  /// @brief cell array instance で使われている式の名前解決を行う．
  /// @param[in] prim_array プリミティブ配列
  /// @param[in] pt_inst インスタンス定義
  void
  link_cell_array(ElbPrimArray* prim_array,
		  const PtInst* pt_inst);

  /// @brief cell instance で使われている式の名前解決を行う．
  /// @param[in] primitive プリミティブ配列
  /// @param[in] pt_inst インスタンス定義
  void
  link_cell(ElbPrimitive* primitive,
	    const PtInst* pt_inst);

  /// @brief generate block を実際にインスタンス化を行う．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_genblock generate block 定義
  void
  phase1_generate(const VlScope* parent,
		  const PtItem* pt_genblock);

  /// @brief PtGenBlock に対応するインスタンスの生成を行う
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_genblock generate block 定義
  void
  phase1_genblock(const VlScope* parent,
		  const PtItem* pt_genblock);

  /// @brief generate if に対応するインスタンスの生成を行う
  /// @param[in] parent 親のスコープ
  /// @parma[in] pt_genif generate if 定義
  void
  phase1_genif(const VlScope* parent,
	       const PtItem* pt_genif);

  /// @brief generate case に対応するインスタンスの生成を行う
  /// @param[in] parent 親のスコープ
  /// @parma[in] pt_gencase generate case 定義
  void
  phase1_gencase(const VlScope* parent,
		 const PtItem* pt_gencase);

  /// @brief generate for に対応するインスタンスの生成を行う
  /// @param[in] parent 親のスコープ
  /// @parma[in] pt_genfor generate for 定義
  void
  phase1_genfor(const VlScope* parent,
		const PtItem* pt_genfor);

  /// @brief generate block の要素でスコープに関連するものの生成を行う．
  /// @note と同時の残りの処理をキューに積む．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_decl_array パース木の宣言の配列
  /// @param[in] pt_item_array パース木の要素の配列
  void
  phase1_genitem(const VlScope* parent,
		 const vector<const PtDeclHead*>& pt_decl_array,
		 const vector<const PtItem*>& pt_item_array);

  /// @brief パラメータ割り当て情報を作る．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_head 構文木のヘッダ要素
  vector<ElbParamCon>
  gen_param_con_list(const VlScope* parent,
		     const PtItem* pt_head);

};

END_NAMESPACE_YM_VERILOG

#endif // ITEMGEN_H
