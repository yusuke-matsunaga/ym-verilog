
/// @file VlMgr.cc
/// @brief VlMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym/VlMgr.h"

#include "parser/Parser.h"
#include "parser/PtMgr.h"
#include "parser/PtiFactory.h"

#include "elaborator/Elaborator.h"
#include "elaborator/ElbMgr.h"
//#include "elaborator/ElbUdp.h"
//#include "elaborator/ElbUserSystf.h"
#include "elaborator/ElbAttribute.h"
#include "elaborator/ElbFactory.h"


BEGIN_NAMESPACE_YM_VERILOG

// @brief コンストラクタ
VlMgr::VlMgr() :
  mPtMgr{new PtMgr},
  mPtiFactory{PtiFactory::make_obj("cpt", mPtMgr->alloc())},
  mElbMgr{new ElbMgr()},
  mElbFactory{ElbFactory::new_obj()}
{
}

// @brief デストラクタ
VlMgr::~VlMgr()
{
}

// @brief 内容をクリアする．
void
VlMgr::clear()
{
  mPtMgr->clear();
  mElbMgr->clear();
}

// @brief ファイルを読み込む．
// @param[in] filename 読み込むファイル名
// @param[in] searchpath サーチパス
// @param[in] watcher_list 行番号ウォッチャーのリスト
// @retval true 正常に終了した．
// @retval false エラーが起こった．
bool
VlMgr::read_file(const string& filename,
		 const SearchPathList& searchpath,
		 const vector<VlLineWatcher*> watcher_list)
{
  Parser parser(*mPtMgr, *mPtiFactory);

  return parser.read_file(filename, searchpath, watcher_list);
}

// @brief 登録されているモジュールのリストを返す．
// @return 登録されているモジュールのリスト
const vector<const PtModule*>&
VlMgr::pt_module_list() const
{
  return mPtMgr->pt_module_list();
}

// @brief 登録されている UDP のリストを返す．
// @return 登録されている UDP のリスト
const vector<const PtUdp*>&
VlMgr::pt_udp_list() const
{
  return mPtMgr->pt_udp_list();
}

// @brief エラボレーションを行う．
// @param[in] cell_library セルライブラリ
// @param[in] エラー数を返す．
int
VlMgr::elaborate(const ClibCellLibrary& cell_library)
{
  Elaborator elab(*mPtMgr, *mElbMgr, *mElbFactory, cell_library);

  return elab();
}

// @brief UDP 定義のリストを返す．
const vector<const VlUdpDefn*>&
VlMgr::udp_list() const
{
  return mElbMgr->udp_list();
}

// @brief 名前から UDP 定義を取出す．
// @param[in] name 名前
// @return name という名の UDP を返す．
// @return なければ nullptr を返す．
const VlUdpDefn*
VlMgr::find_udp(const char* name) const
{
  return mElbMgr->find_udp(name);
}

// @brief topmodule のリストを返す．
const vector<const VlModule*>&
VlMgr::topmodule_list() const
{
  return mElbMgr->topmodule_list();
}

// @brief 名前から UserSystf を取出す．
// @param[in] name 名前
// @return name という名のユーザー定義関数を返す．
// @return なければ nullptr を返す．
const VlUserSystf*
VlMgr::find_user_systf(const char* name) const
{
  return mElbMgr->find_user_systf(name);
}

// @brief スコープに属する internal scope のリストを取り出す．
// @param[in] parent 検索対象のスコープ
// @return 結果のリストを返す．
vector<const VlNamedObj*>
VlMgr::find_internalscope_list(const VlNamedObj* parent) const
{
  return mElbMgr->find_internalscope_list(parent);
}

// @brief スコープとタグから宣言要素を取り出す．
// @param[in] parent 検索対象のスコープ
// @param[in] tag タグ
// @return 結果のリストを返す．
//
// parent のスコープ内の tag というタグを持つ要素のリストを返す．
vector<const VlDecl*>
VlMgr::find_decl_list(const VlNamedObj* parent,
		      int tag) const
{
  return mElbMgr->find_decl_list(parent, tag);
}

// @brief スコープとタグから宣言要素の配列を取り出す．
// @param[in] parent 検索対象のスコープ
// @param[in] tag タグ
// @retrun 結果のリストを返す．
//
// parent というスコープ内の tag というタグを持つ要素のリストを返す．
vector<const VlDeclArray*>
VlMgr::find_declarray_list(const VlNamedObj* parent,
			   int tag) const
{
  return mElbMgr->find_declarray_list(parent, tag);
}

// @brief スコープに属する defparam のリストを取り出す．
// @param[in] parent 検索対象のスコープ
// @return 結果のリストを返す．
vector<const VlDefParam*>
VlMgr::find_defparam_list(const VlNamedObj* parent) const
{
  return mElbMgr->find_defparam_list(parent);
}

// @brief スコープに属する param assign のリストを取り出す．
// @param[in] parent 検索対象のスコープ
// @return 結果のリストを返す．
vector<const VlParamAssign*>
VlMgr::find_paramassign_list(const VlNamedObj* parent) const
{
  return mElbMgr->find_paramassign_list(parent);
}

// @brief スコープに属する module のリストを取り出す．
// @param[in] parent 検索対象のスコープ
// @return 結果のリストを返す．
vector<const VlModule*>
VlMgr::find_module_list(const VlNamedObj* parent) const
{
  return mElbMgr->find_module_list(parent);
}

// @brief スコープに属する module arrayのリストを取り出す．
// @param[in] parent 検索対象のスコープ
// @return 結果のリストを返す．
vector<const VlModuleArray*>
VlMgr::find_modulearray_list(const VlNamedObj* parent) const
{
  return mElbMgr->find_modulearray_list(parent);
}

// @brief スコープに属する primitive のリストを取り出す．
// @param[in] parent 検索対象のスコープ
// @return 結果のリストを返す．
vector<const VlPrimitive*>
VlMgr::find_primitive_list(const VlNamedObj* parent) const
{
  return mElbMgr->find_primitive_list(parent);
}

// @brief スコープに属する primitive array のリストを取り出す．
// @param[in] parent 検索対象のスコープ
// @return 結果のリストを返す．
vector<const VlPrimArray*>
VlMgr::find_primarray_list(const VlNamedObj* parent) const
{
  return mElbMgr->find_primarray_list(parent);
}

// @brief スコープに属するタスクのリストを取り出す．
// @param[in] parent 検索対象のスコープ
// @return 結果のリストを返す．
vector<const VlTaskFunc*>
VlMgr::find_task_list(const VlNamedObj* parent) const
{
  return mElbMgr->find_task_list(parent);
}

// @brief スコープに属する関数のリストを取り出す．
// @param[in] parent 検索対象のスコープ
// @return 結果のリストを返す．
vector<const VlTaskFunc*>
VlMgr::find_function_list(const VlNamedObj* parent) const
{
  return mElbMgr->find_function_list(parent);
}

// @brief スコープに属する continuous assignment のリストを取り出す．
// @param[in] parent 検索対象のスコープ
// @return 結果のリストを返す．
vector<const VlContAssign*>
VlMgr::find_contassign_list(const VlNamedObj* parent) const
{
  return mElbMgr->find_contassign_list(parent);
}

// @brief スコープに属する process のリストを取り出す．
// @param[in] parent 検索対象のスコープ
// @return 結果のリストを返す．
vector<const VlProcess*>
VlMgr::find_process_list(const VlNamedObj* parent) const
{
  return mElbMgr->find_process_list(parent);
}

// @brief 属性リストを得る．
// @param[in] obj 対象のオブジェクト
// @param[in] def 定義側の属性の時 true とするフラグ
VlAttrList*
VlMgr::find_attr(const VlObj* obj,
		 bool def) const
{
  return mElbMgr->find_attr(obj, def);
}

END_NAMESPACE_YM_VERILOG
