#ifndef MODULEGEN_H
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
  ModuleGen(
    Elaborator& elab, ///< [in] 生成器
    ElbMgr& elb_mgr   ///< [in] Elbオブジェクトを管理するクラス
  );

  /// @brief デストラクタ
  ~ModuleGen();


public:
  //////////////////////////////////////////////////////////////////////
  // ModuleGen の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief top module のインスタンス化を行う．
  void
  phase1_topmodule(
    const VlScope* toplevel,  ///< [in] トップレベルのスコープ
    const PtModule* pt_module ///< [in] モジュール定義
  );

  /// @brief module の中身のうちスコープに関係するインスタンス化を行う．
  void
  phase1_module_item(
    ElbModule* module,                        ///< [in] モジュール
    const PtModule* pt_module,                ///< [in] モジュール定義
    const vector<ElbParamCon>& param_con_list ///< [in] パラメータ割り当ての情報
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief phase2 で処理する内容をキューに積む．
  void
  add_phase2stub(
    ElbModule* module,        ///< [in] モジュール
    const PtModule* pt_module ///< [in] モジュール定義
  )
  {
    auto stub = make_stub(this, &ModuleGen::phase2_module_item,
			  module, pt_module);
    ElbProxy::add_phase2stub(stub);
  }

  /// @brief module の中身のインスタンス化を行う．
  void
  phase2_module_item(
    ElbModule* module,        ///< [in] モジュール
    const PtModule* pt_module ///< [in] モジュール定義
  );

  /// @brief port の生成を行う．
  void
  instantiate_port(
    ElbModule* module,        ///< [in] 親のモジュール
    const PtModule* pt_module ///< [in] モジュール定義
  );

  /// @brief PtPortRef から expression を生成する．
  ElbExpr*
  instantiate_portref(
    ElbModule* module,       ///< [in] 親のモジュール
    const PtExpr* pt_portref ///< [in] パース木の portref 定義
  );


private:
  //////////////////////////////////////////////////////////////////////
  // エラーメッセージを出力する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief パラメータポートの割り当て数が多すぎる．
  void
  error_too_many_param(
    const vector<ElbParamCon>& param_con_list ///< [in] パラメータポートの割り当てリスト
  );

  /// @brief パラメータポートに現れるパラメータが存在しない．
  void
  error_no_param(
    const PtConnection* pt_con, ///< [in] パラメータポート割り当てのパース木
    const char* name            ///< [in] パラメータ名
  );

  /// @brief 対象の要素が見つからない．
  void
  error_not_found(
    const FileRegion& file_region, ///< [in] ファイル位置
    const char* name               ///< [in] 名前
  );

  /// @brief ポートに配列が使われている．
  void
  error_port_array(
    const FileRegion& file_region, ///< [in] ファイル位置
    const VlDeclArray* array       ///< [in] 配列
  );

  /// @brief ポートに使われている要素が宣言要素でなかった．
  void
  error_illegal_port(
    const FileRegion& file_region, ///< [in] ファイル位置
    const char* name               ///< [in] 名前
  );

  /// @brief 添字が範囲外
  void
  warning_index_out_of_range(
    const FileRegion& file_region ///< [in] ファイル位置
  );

  /// @brief 左の範囲が範囲外
  void
  warning_left_index_out_of_range(
    const FileRegion& file_region ///< [in] ファイル位置
  );

  /// @brief 右の範囲が範囲外
  void
  warning_right_index_out_of_range(
    const FileRegion& file_region ///< [in] ファイル位置
  );

};

END_NAMESPACE_YM_VERILOG

#endif // MODULEGEN_H
