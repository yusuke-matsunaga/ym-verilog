﻿#ifndef MODULEGEN_H
#define MODULEGEN_H

/// @file ModuleGen.h
/// @brief ModuleGen のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym/verilog.h"
#include "ym/pt/PtP.h"
#include "ElbProxy.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ModuleGen ModuleGen.h "ModuleGen.h"
/// @brief ElbModule を生成するクラス
//////////////////////////////////////////////////////////////////////
class ModuleGen :
  public ElbProxy
{
public:

  /// @brief コンストラクタ
  /// @param[in] elab 生成器
  /// @param[in] elb_mgr Elbオブジェクトを管理するクラス
  ModuleGen(Elaborator& elab,
	    ElbMgr& elb_mgr);

  /// @brief デストラクタ
  ~ModuleGen();


public:
  //////////////////////////////////////////////////////////////////////
  // ModuleGen の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief top module のインスタンス化を行う．
  /// @param[in] toplevel トップレベルのスコープ
  /// @param[in] pt_module モジュール定義
  void
  phase1_topmodule(const VlScope* toplevel,
		   const PtModule* pt_module);

  /// @brief module の中身のうちスコープに関係するインスタンス化を行う．
  /// @param[in] modle モジュール
  /// @param[in] pt_module モジュール定義
  /// @param[in] param_con パラメータ割り当ての情報
  void
  phase1_module_item(ElbModule* module,
		     const PtModule* pt_module,
		     const vector<ElbParamCon>& param_con_list);


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief phase2 で処理する内容をキューに積む．
  /// @param[in] modle モジュール
  /// @param[in] pt_module モジュール定義
  void
  add_phase2stub(ElbModule* module,
		 const PtModule* pt_module);

  /// @brief module の中身のインスタンス化を行う．
  /// @param[in] modle モジュール
  /// @param[in] pt_module モジュール定義
  void
  phase2_module_item(ElbModule* module,
		     const PtModule* pt_module);

  /// @brief port の生成を行う．
  /// @param[in] module 親のモジュール
  /// @param[in] pt_module モジュール定義
  void
  instantiate_port(ElbModule* module,
		   const PtModule* pt_module);

  /// @brief PtPortRef から expression を生成する．
  /// @param[in] module 親のモジュール
  /// @param[in] pt_portref パース木の portref 定義
  ElbExpr*
  instantiate_portref(ElbModule* module,
		      const PtExpr* pt_portref);


private:
  //////////////////////////////////////////////////////////////////////
  // エラーメッセージを出力する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief パラメータポートの割り当て数が多すぎる．
  /// @param[in] param_con_list パラメータポートの割り当てリスト
  void
  error_too_many_param(const vector<ElbParamCon>& param_con_list);

  /// @brief パラメータポートに現れるパラメータが存在しない．
  /// @param[in] pt_con パラメータポート割り当てのパース木
  /// @param[in] name パラメータ名
  void
  error_no_param(const PtConnection* pt_con,
		 const char* name);

  /// @brief 対象の要素が見つからない．
  /// @param[in] file_region ファイル位置
  /// @param[in] name 名前
  void
  error_not_found(const FileRegion& file_region,
		  const char* name);

  /// @brief ポートに配列が使われている．
  /// @param[in] file_region ファイル位置
  /// @param[in] array 配列
  void
  error_port_array(const FileRegion& file_region,
		   const VlDeclArray* array);

  /// @brief ポートに使われている要素が宣言要素でなかった．
  /// @param[in] file_region ファイル位置
  /// @param[in] name 名前
  void
  error_illegal_port(const FileRegion& file_region,
		     const char* name);

  /// @brief 添字が範囲外
  /// @param[in] file_region ファイル位置
  void
  warning_index_out_of_range(const FileRegion& file_region);

  /// @brief 左の範囲が範囲外
  /// @param[in] file_region ファイル位置
  void
  warning_left_index_out_of_range(const FileRegion& file_region);

  /// @brief 右の範囲が範囲外
  /// @param[in] file_region ファイル位置
  void
  warning_right_index_out_of_range(const FileRegion& file_region);

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief phase2 で処理する内容をキューに積む．
// @param[in] modle モジュール
// @param[in] pt_module モジュール定義
inline
void
ModuleGen::add_phase2stub(ElbModule* module,
			  const PtModule* pt_module)
{
  ElbStub* stub = make_stub(this, &ModuleGen::phase2_module_item,
			    module, pt_module);
  ElbProxy::add_phase2stub(stub);
}

END_NAMESPACE_YM_VERILOG

#endif // MODULEGEN_H
