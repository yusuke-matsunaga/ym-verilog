#ifndef TAGDICT_H
#define TAGDICT_H

/// @file TagDict.h
/// @brief TagDict のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2019 Yusuke Matsunaga
/// All rights reserved.


#include "ym/verilog.h"
#include "ym/vl/VlFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

class TagDictCell;

//////////////////////////////////////////////////////////////////////
/// @class TagDict TagDict.h "TagDict.h"
/// @brief 各スコープの構成要素リストを格納するハッシュ表
//////////////////////////////////////////////////////////////////////
class TagDict
{
public:

  /// @brief コンストラクタ
  TagDict();

  /// @brief デストラクタ
  ~TagDict();


public:

  /// @brief 内容を空にする
  void
  clear();

  /// @brief internal scope を追加する．
  /// @param[in] scope 登録する要素
  void
  add_internalscope(const VlNamedObj* scope);

  /// @brief internal scope のリストを取り出す．
  /// @param[in] parent 親のスコープ
  /// @return 結果のリストを返す．
  vector<const VlNamedObj*>
  find_internalscope_list(const VlNamedObj* parent) const;

  /// @brief 宣言要素を追加する．
  /// @param[in] tag 要素の型を表すタグ (vpi_user.h 参照)
  /// @param[in] decl 登録する要素
  void
  add_decl(int tag,
	   const VlDecl* decl);

  /// @brief タグから該当する宣言要素のリストを返す．
  /// @param[in] parent 親のスコープ
  /// @param[in] tag 要素の型を表すタグ (vpi_user.h 参照)
  /// @return 結果のリストを返す．
  ///
  /// parent のスコープ内の tag というタグを持つ要素のリストを返す．
  vector<const VlDecl*>
  find_decl_list(const VlNamedObj* parent,
		 int tag) const;

  /// @brief 宣言要素を追加する．
  /// @param[in] tag 要素の型を表すタグ (vpi_user.h 参照)
  /// @param[in] decl 登録する要素
  void
  add_declarray(int tag,
		const VlDeclArray* decl);

  /// @brief タグから該当する宣言要素のリストを返す．
  /// @param[in] parent 親のスコープ
  /// @param[in] tag 要素の型を表すタグ (vpi_user.h 参照)
  /// @retrun 結果のリストを返す．
  ///
  /// parent というスコープ内の tag というタグを持つ要素のリストを返す．
  vector<const VlDeclArray*>
  find_declarray_list(const VlNamedObj* parent,
		      int tag) const;

  /// @brief defparam を追加する．
  /// @param[in] defparam 登録する要素
  void
  add_defparam(const VlDefParam* defparam);

  /// @brief defparam のリストを取り出す．
  /// @param[in] parent 親のスコープ
  /// @return 結果のリストを返す．
  vector<const VlDefParam*>
  find_defparam_list(const VlNamedObj* parent) const;

  /// @brief param assign を追加する．
  /// @param[in] paramassign 登録する要素
  void
  add_paramassign(const VlParamAssign* paramassign);

  /// @brief param assign のリストを取り出す．
  /// @param[in] parent 親のスコープ
  /// @return 結果のリストを返す．
  vector<const VlParamAssign*>
  find_paramassign_list(const VlNamedObj* parent) const;

  /// @brief module array を追加する．
  /// @param[in] modulearray 登録する要素
  void
  add_modulearray(const VlModuleArray* modulearray);

  /// @brief module array のリストを取り出す．
  /// @param[in] parent 親のスコープ
  /// @return 結果のリストを返す．
  vector<const VlModuleArray*>
  find_modulearray_list(const VlNamedObj* parent) const;

  /// @brief module を追加する．
  /// @param[in] module 登録する要素
  void
  add_module(const VlModule* module);

  /// @brief module のリストを取り出す．
  /// @param[in] parent 親のスコープ
  /// @return 結果のリストを返す．
  vector<const VlModule*>
  find_module_list(const VlNamedObj* parent) const;

  /// @brief primitive array を追加する．
  /// @param[in] primarray 登録する要素
  void
  add_primarray(const VlPrimArray* primarray);

  /// @brief primitive array のリストを取り出す．
  /// @param[in] parent 親のスコープ
  /// @return 結果のリストを返す．
  vector<const VlPrimArray*>
  find_primarray_list(const VlNamedObj* parent) const;

  /// @brief primitive を追加する．
  /// @param[in] primitive 登録する要素
  void
  add_primitive(const VlPrimitive* primitive);

  /// @brief primitive のリストを取り出す．
  /// @param[in] parent 親のスコープ
  /// @return 結果のリストを返す．
  vector<const VlPrimitive*>
  find_primitive_list(const VlNamedObj* parent) const;

  /// @brief タスクを追加する．
  /// @param[in] task 追加する要素
  void
  add_task(const VlTaskFunc* task);

  /// @brief タスクのリストを取り出す．
  /// @param[in] parent 親のスコープ
  /// @return 結果のリストを返す．
  vector<const VlTaskFunc*>
  find_task_list(const VlNamedObj* parent) const;

  /// @brief 関数を追加する．
  /// @param[in] func 追加する要素
  void
  add_function(const VlTaskFunc* func);

  /// @brief 関数のリストを取り出す．
  /// @param[in] parent 親のスコープ
  /// @return 結果のリストを返す．
  vector<const VlTaskFunc*>
  find_function_list(const VlNamedObj* parent) const;

  /// @brief continuous assignment を追加する．
  /// @param[in] contassign 登録する要素
  void
  add_contassign(const VlContAssign* contassign);

  /// @brief continuous assignment のリストを取り出す．
  /// @param[in] parent 親のスコープ
  /// @return 結果のリストを返す．
  vector<const VlContAssign*>
  find_contassign_list(const VlNamedObj* parent) const;

  /// @brief process を追加する．
  /// @param[in] process 登録する要素
  void
  add_process(const VlProcess* process);

  /// @brief process のリストを取り出す．
  /// @param[in] parent 親のスコープ
  /// @return 結果のリストを返す．
  vector<const VlProcess*>
  find_process_list(const VlNamedObj* parent) const;


private:

  /// @brief Cell を登録する．
  /// @param[in] parent 親のスコープ
  /// @param[in] tag 要素の型を表すタグ (vpi_user.h 参照)
  /// @param[in] cell 対象の Cell
  void
  put_cell(const VlNamedObj* parent,
	   int tag,
	   TagDictCell* cell);

  /// @brief タグから該当する Cell を探す．
  /// @param[in] parent 親のスコープ
  /// @param[in] tag 要素の型を表すタグ (vpi_user.h 参照)
  TagDictCell*
  find_cell(const VlNamedObj* parent,
	    int tag) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で使用されるデータ構造
  //////////////////////////////////////////////////////////////////////

  struct Key
  {
    // 親のスコープ
    const VlNamedObj* mParent;

    // タグ
    int mTag;

  };

  struct Hash
  {
    SizeType
    operator()(const Key& key) const
    {
      return ((reinterpret_cast<ympuint>(key.mParent) * key.mTag) >> 8);
    }
  };

  struct Eq
  {
    bool
    operator()(const Key& left,
	       const Key& right) const
    {
      return left.mParent == right.mParent && left.mTag == right.mTag;
    }
  };


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ハッシュ表
  unordered_map<Key, TagDictCell*, Hash, Eq> mHash;

  // セルのリスト
  vector<unique_ptr<TagDictCell>> mCellList;

};

END_NAMESPACE_YM_VERILOG

#endif // TAGDICT_H
