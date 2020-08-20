#ifndef YM_VLMGR_H
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

class Alloc;
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

  /// @brief スコープに属する internal scope のリストを取り出す．
  /// @param[in] parent 検索対象のスコープ
  /// @return 結果のリストを返す．
  vector<const VlScope*>
  find_internalscope_list(const VlScope* parent) const;

  /// @brief スコープとタグから宣言要素を取り出す．
  /// @param[in] parent 検索対象のスコープ
  /// @param[in] tag タグ
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

  /// @brief スコープに属する continuous assignment のリストを取り出す．
  /// @param[in] parent 検索対象のスコープ
  /// @return 結果のリストを返す．
  vector<const VlContAssign*>
  find_contassign_list(const VlScope* parent) const;

  /// @brief スコープに属する process のリストを取り出す．
  /// @param[in] parent 検索対象のスコープ
  /// @return 結果のリストを返す．
  vector<const VlProcess*>
  find_process_list(const VlScope* parent) const;

  /// @brief 属性リストを得る．
  /// @param[in] obj 対象のオブジェクト
  /// @param[in] def 定義側の属性の時 true とするフラグ
  vector<const VlAttribute*>
  find_attr(const VlObj* obj,
	    bool def = false) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // メモリアロケータ
  unique_ptr<Alloc> mAlloc;

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
