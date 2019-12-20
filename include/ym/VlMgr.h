﻿#ifndef YM_VLMGR_H
#define YM_VLMGR_H

/// @file ym/VlMgr.h
/// @brief VlMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym/verilog.h"
#include "ym/pt/PtP.h"
#include "ym/vl/VlFwd.h"
#include "ym/ClibCellLibrary.h"
#include "ym/File.h"


BEGIN_NAMESPACE_YM_VERILOG

class PtMgr;
class PtiFactory;
class ElbMgr;
class ElbFactory;

//////////////////////////////////////////////////////////////////////
/// @class VlMgr VlMgr.h "ym/VlMgr.h"
/// @brief Verilog-HDL の elaboration 結果を保持するクラス
//////////////////////////////////////////////////////////////////////
class VlMgr
{
public:

  /// @brief コンストラクタ
  VlMgr();

  /// @brief デストラクタ
  ~VlMgr();


public:

  /// @brief 内容をクリアする．
  void
  clear();


public:
  //////////////////////////////////////////////////////////////////////
  // parser 関係のメンバ関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイルを読み込む．
  /// @param[in] filename 読み込むファイル名
  /// @param[in] searchpath サーチパス
  /// @param[in] watcher_list 行番号ウォッチャーのリスト
  /// @retval true 正常に終了した．
  /// @retval false エラーが起こった．
  bool
  read_file(const string& filename,
	    const SearchPathList& searchpath = SearchPathList(),
	    const vector<VlLineWatcher*> watcher_list = vector<VlLineWatcher*>());

  /// @brief 登録されているモジュールのリストを返す．
  /// @return 登録されているモジュールのリスト
  const vector<const PtModule*>&
  pt_module_list() const;

  /// @brief 登録されている UDP のリストを返す．
  /// @return 登録されている UDP のリスト
  const vector<const PtUdp*>&
  pt_udp_list() const;


public:
  //////////////////////////////////////////////////////////////////////
  // elaboration 関係のメンバ関数
  //////////////////////////////////////////////////////////////////////

  /// @brief エラボレーションを行う．
  /// @param[in] cell_library セルライブラリ
  /// @return エラー数を返す．
  int
  elaborate(const ClibCellLibrary& cell_library = ClibCellLibrary());

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

  /// @brief スコープと名前から名前付き要素を取り出す．
  /// @param[in] parent 検索対象のスコープ
  /// @param[in] name 名前
  /// @return scope というスコープ内の name という要素を返す．
  /// @return なければ nullptr を返す．
  const VlNamedObj*
  find_obj(const VlNamedObj* parent,
	   const char* name) const;

  /// @brief スコープに属する internal scope のリストを取り出す．
  /// @param[in] parent 検索対象のスコープ
  /// @param[out] scope_list 結果を格納するリスト
  /// @retval true 該当する要素が1つ以上あった．
  /// @retval false 該当する要素がなかった．
  bool
  find_internalscope_list(const VlNamedObj* parent,
			  vector<const VlNamedObj*>& scope_list) const;

  /// @brief スコープとタグから宣言要素を取り出す．
  /// @param[in] parent 検索対象のスコープ
  /// @param[in] tag タグ
  /// @param[out] decl_list 結果を格納するリスト
  /// @retval true 該当する要素が1つ以上あった．
  /// @retval false 該当する要素がなかった．
  /// @note scope というスコープ内の tag というタグを持つ宣言要素を
  /// decl_list に入れる．
  bool
  find_decl_list(const VlNamedObj* parent,
		 int tag,
		 vector<const VlDecl*>& decl_list) const;

  /// @brief スコープとタグから宣言要素の配列を取り出す．
  /// @param[in] parent 検索対象のスコープ
  /// @param[in] tag タグ
  /// @param[out] declarray_list 結果を格納するリスト
  /// @retval true 該当する要素が1つ以上あった．
  /// @retval false 該当する要素がなかった．
  /// @note scope というスコープ内の tag というタグを持つ宣言要素を
  /// decl_list に入れる．
  bool
  find_declarray_list(const VlNamedObj* parent,
		      int tag,
		      vector<const VlDeclArray*>& declarray_list) const;

  /// @brief スコープに属する defparam のリストを取り出す．
  /// @param[in] parent 検索対象のスコープ
  /// @param[out] defparam_list 結果を格納するリスト
  /// @retval true 該当する要素が1つ以上あった．
  /// @retval false 該当する要素がなかった．
  bool
  find_defparam_list(const VlNamedObj* parent,
		     vector<const VlDefParam*>& defparam_list) const;

  /// @brief スコープに属する param assign のリストを取り出す．
  /// @param[in] parent 検索対象のスコープ
  /// @param[out] paramassign_list 結果を格納するリスト
  /// @retval true 該当する要素が1つ以上あった．
  /// @retval false 該当する要素がなかった．
  bool
  find_paramassign_list(const VlNamedObj* parent,
			vector<const VlParamAssign*>& paramassign_list) const;

  /// @brief スコープに属する module のリストを取り出す．
  /// @param[in] parent 検索対象のスコープ
  /// @param[out] module_list 結果を格納するリスト
  /// @retval true 該当する要素が1つ以上あった．
  /// @retval false 該当する要素がなかった．
  bool
  find_module_list(const VlNamedObj* parent,
		   vector<const VlModule*>& module_list) const;

  /// @brief スコープに属する module arrayのリストを取り出す．
  /// @param[in] parent 検索対象のスコープ
  /// @param[out] modulearray_list 結果を格納するリスト
  /// @retval true 該当する要素が1つ以上あった．
  /// @retval false 該当する要素がなかった．
  bool
  find_modulearray_list(const VlNamedObj* parent,
			vector<const VlModuleArray*>& modulearray_list) const;

  /// @brief スコープに属する primitive のリストを取り出す．
  /// @param[in] parent 検索対象のスコープ
  /// @param[out] primitive_list 結果を格納するリスト
  /// @retval true 該当する要素が1つ以上あった．
  /// @retval false 該当する要素がなかった．
  bool
  find_primitive_list(const VlNamedObj* parent,
		      vector<const VlPrimitive*>& primitive_list) const;

  /// @brief スコープに属する primitive array のリストを取り出す．
  /// @param[in] parent 検索対象のスコープ
  /// @param[out] primarray_list 結果を格納するリスト
  /// @retval true 該当する要素が1つ以上あった．
  /// @retval false 該当する要素がなかった．
  bool
  find_primarray_list(const VlNamedObj* parent,
		      vector<const VlPrimArray*>& primarray_list) const;

  /// @brief スコープに属するタスクのリストを取り出す．
  /// @param[in] parent 検索対象のスコープ
  /// @param[out] task_list 結果を格納するリスト
  /// @retval true 該当する要素が1つ以上あった．
  /// @retval false 該当する要素がなかった．
  bool
  find_task_list(const VlNamedObj* parent,
		 vector<const VlTaskFunc*>& task_list) const;

  /// @brief スコープに属する関数のリストを取り出す．
  /// @param[in] parent 検索対象のスコープ
  /// @param[out] func_list 結果を格納するリスト
  /// @retval true 該当する要素が1つ以上あった．
  /// @retval false 該当する要素がなかった．
  bool
  find_function_list(const VlNamedObj* parent,
		     vector<const VlTaskFunc*>& func_list) const;

  /// @brief スコープに属する continuous assignment のリストを取り出す．
  /// @param[in] parent 検索対象のスコープ
  /// @param[out] contassign_list 結果を格納するリスト
  /// @retval true 該当する要素が1つ以上あった．
  /// @retval false 該当する要素がなかった．
  bool
  find_contassign_list(const VlNamedObj* parent,
		       vector<const VlContAssign*>& contassign_list) const;

  /// @brief スコープに属する process のリストを取り出す．
  /// @param[in] parent 検索対象のスコープ
  /// @param[out] process_list 結果を格納するリスト
  /// @retval true 該当する要素が1つ以上あった．
  /// @retval false 該当する要素がなかった．
  bool
  find_process_list(const VlNamedObj* parent,
		    vector<const VlProcess*>& process_list) const;

  /// @brief 属性リストを得る．
  /// @param[in] obj 対象のオブジェクト
  /// @param[in] def 定義側の属性の時 true とするフラグ
  VlAttrList*
  find_attr(const VlObj* obj,
	    bool def = false) const;

  /// @brief このオブジェクトが確保したメモリの総量を返す．
  ymuint
  allocated_size() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // Pt オブジェクトを管理するクラス
  unique_ptr<PtMgr> mPtMgr;

  // Ptオブジェクトの生成を行うファクトリクラス
  unique_ptr<PtiFactory> mPtiFactory;

  // Elb オブジェクトを管理するクラス
  unique_ptr<ElbMgr> mElbMgr;

  // Elbオブジェクトの生成を行うファクトリクラス
  unique_ptr<ElbFactory> mElbFactory;

};

END_NAMESPACE_YM_VERILOG

#endif // YM_VLMGR_H
