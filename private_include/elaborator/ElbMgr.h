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
  vector<const VlNamedObj*>
  find_internalscope_list(const VlNamedObj* parent) const;

  /// @brief スコープとタグから宣言要素を取り出す．
  /// @param[in] parent 検索対象のスコープ
  /// @param[in] tag タグ
  /// @return 結果のリストを返す．
  ///
  /// parent のスコープ内の tag というタグを持つ要素のリストを返す．
  vector<const VlDecl*>
  find_decl_list(const VlNamedObj* parent,
		 int tag) const;

  /// @brief スコープとタグから宣言要素の配列を取り出す．
  /// @param[in] parent 検索対象のスコープ
  /// @param[in] tag タグ
  /// @retrun 結果のリストを返す．
  ///
  /// parent というスコープ内の tag というタグを持つ要素のリストを返す．
  vector<const VlDeclArray*>
  find_declarray_list(const VlNamedObj* parent,
		      int tag) const;

  /// @brief スコープに属する defparam のリストを取り出す．
  /// @param[in] parent 検索対象のスコープ
  /// @return 結果のリストを返す．
  vector<const VlDefParam*>
  find_defparam_list(const VlNamedObj* parent) const;

  /// @brief スコープに属する param assign のリストを取り出す．
  /// @param[in] parent 検索対象のスコープ
  /// @return 結果のリストを返す．
  vector<const VlParamAssign*>
  find_paramassign_list(const VlNamedObj* parent) const;

  /// @brief スコープに属する module のリストを取り出す．
  /// @param[in] parent 検索対象のスコープ
  /// @return 結果のリストを返す．
  vector<const VlModule*>
  find_module_list(const VlNamedObj* parent) const;

  /// @brief スコープに属する module arrayのリストを取り出す．
  /// @param[in] parent 検索対象のスコープ
  /// @return 結果のリストを返す．
  vector<const VlModuleArray*>
  find_modulearray_list(const VlNamedObj* parent) const;

  /// @brief スコープに属する primitive のリストを取り出す．
  /// @param[in] parent 検索対象のスコープ
  /// @return 結果のリストを返す．
  vector<const VlPrimitive*>
  find_primitive_list(const VlNamedObj* parent) const;

  /// @brief スコープに属する primitive array のリストを取り出す．
  /// @param[in] parent 検索対象のスコープ
  /// @return 結果のリストを返す．
  vector<const VlPrimArray*>
  find_primarray_list(const VlNamedObj* parent) const;

  /// @brief スコープに属する continuous assignment のリストを取り出す．
  /// @param[in] parent 検索対象のスコープ
  /// @return 結果のリストを返す．
  vector<const VlContAssign*>
  find_contassign_list(const VlNamedObj* parent) const;

  /// @brief スコープに属するタスクのリストを取り出す．
  /// @param[in] parent 検索対象のスコープ
  /// @return 結果のリストを返す．
  vector<const VlTaskFunc*>
  find_task_list(const VlNamedObj* parent) const;

  /// @brief スコープに属する関数のリストを取り出す．
  /// @param[in] parent 検索対象のスコープ
  /// @return 結果のリストを返す．
  vector<const VlTaskFunc*>
  find_function_list(const VlNamedObj* parent) const;

  /// @brief スコープに属する process のリストを取り出す．
  /// @param[in] parent 検索対象のスコープ
  /// @return 結果のリストを返す．
  vector<const VlProcess*>
  find_process_list(const VlNamedObj* parent) const;


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
  reg_toplevel(const VlNamedObj* toplevel);

  /// @brief internal scope を登録する．
  /// @param[in] obj 登録するオブジェクト
  void
  reg_internalscope(const VlNamedObj* obj);

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
  ElbAttrList*
  find_attr(const VlObj* obj,
	    bool def) const;

  /// @brief 属性リストを登録する．
  /// @param[in] obj 対象のオブジェクト
  /// @param[in] def 定義側の属性の時 true とするフラグ
  /// @param[in] attr_list 属性リスト
  void
  reg_attr(const VlObj* obj,
	   bool def,
	   ElbAttrList* attr_list);


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

  // タグをキーにした各スコープごとのオブジェクトのリストの辞書
  TagDict mTagDict;

  // 属性リストの辞書
  AttrHash mAttrHash;

  // トップレベルスコープ
  const VlNamedObj* mTopLevel;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief スコープに属する internal scope のリストを取り出す．
// @param[in] parent 検索対象のスコープ
// @return 結果のリストを返す．
inline
vector<const VlNamedObj*>
ElbMgr::find_internalscope_list(const VlNamedObj* parent) const
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
ElbMgr::find_decl_list(const VlNamedObj* parent,
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
ElbMgr::find_declarray_list(const VlNamedObj* parent,
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
ElbMgr::find_defparam_list(const VlNamedObj* parent) const
{
  return mTagDict.find_defparam_list(parent);
}

// @brief スコープに属する param assign のリストを取り出す．
// @param[in] parent 検索対象のスコープ
// @return 結果のリストを返す．
inline
vector<const VlParamAssign*>
ElbMgr::find_paramassign_list(const VlNamedObj* parent) const
{
  return mTagDict.find_paramassign_list(parent);
}

// @brief スコープに属する module のリストを取り出す．
// @param[in] parent 検索対象のスコープ
// @return 結果のリストを返す．
inline
vector<const VlModule*>
ElbMgr::find_module_list(const VlNamedObj* parent) const
{
  return mTagDict.find_module_list(parent);
}

// @brief スコープに属する module arrayのリストを取り出す．
// @param[in] parent 検索対象のスコープ
// @return 結果のリストを返す．
inline
vector<const VlModuleArray*>
ElbMgr::find_modulearray_list(const VlNamedObj* parent) const
{
  return mTagDict.find_modulearray_list(parent);
}

// @brief スコープに属する primitive のリストを取り出す．
// @param[in] parent 検索対象のスコープ
// @return 結果のリストを返す．
inline
vector<const VlPrimitive*>
ElbMgr::find_primitive_list(const VlNamedObj* parent) const
{
  return mTagDict.find_primitive_list(parent);
}

// @brief スコープに属する primitive array のリストを取り出す．
// @param[in] parent 検索対象のスコープ
// @return 結果のリストを返す．
inline
vector<const VlPrimArray*>
ElbMgr::find_primarray_list(const VlNamedObj* parent) const
{
  return mTagDict.find_primarray_list(parent);
}

// @brief スコープに属する continuous assignment のリストを取り出す．
// @param[in] parent 検索対象のスコープ
// @return 結果のリストを返す．
inline
vector<const VlContAssign*>
ElbMgr::find_contassign_list(const VlNamedObj* parent) const
{
  return mTagDict.find_contassign_list(parent);
}

// @brief スコープに属するタスクのリストを取り出す．
// @param[in] parent 検索対象のスコープ
// @return 結果のリストを返す．
inline
vector<const VlTaskFunc*>
ElbMgr::find_task_list(const VlNamedObj* parent) const
{
  return mTagDict.find_task_list(parent);
}

// @brief スコープに属する関数のリストを取り出す．
// @param[in] parent 検索対象のスコープ
// @return 結果のリストを返す．
inline
vector<const VlTaskFunc*>
ElbMgr::find_function_list(const VlNamedObj* parent) const
{
  return mTagDict.find_function_list(parent);
}

// @brief スコープに属する process のリストを取り出す．
// @param[in] parent 検索対象のスコープ
// @return 結果のリストを返す．
inline
vector<const VlProcess*>
ElbMgr::find_process_list(const VlNamedObj* parent) const
{
  return mTagDict.find_process_list(parent);
}

// @brief 属性リストを得る．
// @param[in] obj 対象のオブジェクト
// @param[in] def 定義側の属性の時 true とするフラグ
inline
ElbAttrList*
ElbMgr::find_attr(const VlObj* obj,
		  bool def) const
{
  return mAttrHash.find(obj, def);
}

END_NAMESPACE_YM_VERILOG

#endif // ELBMGR_H
