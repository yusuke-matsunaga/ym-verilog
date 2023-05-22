#ifndef TAGDICT_H
#define TAGDICT_H

/// @file TagDict.h
/// @brief TagDict のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2019, 2020, 2021 Yusuke Matsunaga
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
  void
  add_internalscope(
    const VlScope* scope ///< [in] 登録する要素
  );

  /// @brief internal scope のリストを取り出す．
  /// @return 結果のリストを返す．
  vector<const VlScope*>
  find_internalscope_list(
    const VlScope* parent ///< [in] 親のスコープ
  ) const;

  /// @brief 宣言要素を追加する．
  void
  add_decl(
    int tag,           ///< [in] 要素の型を表すタグ (vpi_user.h 参照)
    const VlDecl* decl ///< [in] 登録する要素
  );

  /// @brief タグから該当する宣言要素のリストを返す．
  /// @return 結果のリストを返す．
  ///
  /// parent のスコープ内の tag というタグを持つ要素のリストを返す．
  vector<const VlDecl*>
  find_decl_list(
    const VlScope* parent, ///< [in] 親のスコープ
    int tag                ///< [in] 要素の型を表すタグ (vpi_user.h 参照)
  ) const;

  /// @brief 宣言要素を追加する．
  void
  add_declarray(
    int tag,                ///< [in] 要素の型を表すタグ (vpi_user.h 参照)
    const VlDeclArray* decl ///< [in] 登録する要素
  );

  /// @brief タグから該当する宣言要素のリストを返す．
  /// @retrun 結果のリストを返す．
  ///
  /// parent というスコープ内の tag というタグを持つ要素のリストを返す．
  vector<const VlDeclArray*>
  find_declarray_list(
    const VlScope* parent, ///< [in] 親のスコープ
    int tag                ///< [in] 要素の型を表すタグ (vpi_user.h 参照)
  ) const;

  /// @brief defparam を追加する．
  void
  add_defparam(
    const VlDefParam* defparam ///< [in] 登録する要素
  );

  /// @brief defparam のリストを取り出す．
  /// @return 結果のリストを返す．
  vector<const VlDefParam*>
  find_defparam_list(
    const VlScope* parent ///< [in] 親のスコープ
  ) const;

  /// @brief param assign を追加する．
  void
  add_paramassign(
    const VlParamAssign* paramassign ///< [in] 登録する要素
  );

  /// @brief param assign のリストを取り出す．
  /// @return 結果のリストを返す．
  vector<const VlParamAssign*>
  find_paramassign_list(
    const VlScope* parent ///< [in] 親のスコープ
  ) const;

  /// @brief module array を追加する．
  void
  add_modulearray(
    const VlModuleArray* modulearray ///< [in] 登録する要素
  );

  /// @brief module array のリストを取り出す．
  /// @return 結果のリストを返す．
  vector<const VlModuleArray*>
  find_modulearray_list(
    const VlScope* parent ///< [in] 親のスコープ
  ) const;

  /// @brief module を追加する．
  void
  add_module(
    const VlModule* module ///< [in] 登録する要素
  );

  /// @brief module のリストを取り出す．
  /// @return 結果のリストを返す．
  vector<const VlModule*>
  find_module_list(
    const VlScope* parent ///< [in] 親のスコープ
  ) const;

  /// @brief primitive array を追加する．
  void
  add_primarray(
    const VlPrimArray* primarray ///< [in] 登録する要素
  );

  /// @brief primitive array のリストを取り出す．
  /// @return 結果のリストを返す．
  vector<const VlPrimArray*>
  find_primarray_list(
    const VlScope* parent ///< [in] 親のスコープ
  ) const;

  /// @brief primitive を追加する．
  void
  add_primitive(
    const VlPrimitive* primitive ///< [in] 登録する要素
  );

  /// @brief primitive のリストを取り出す．
  /// @return 結果のリストを返す．
  vector<const VlPrimitive*>
  find_primitive_list(
    const VlScope* parent ///< [in] 親のスコープ
  ) const;

  /// @brief タスクを追加する．
  void
  add_task(
    const VlTaskFunc* task ///< [in] 追加する要素
  );

  /// @brief タスクのリストを取り出す．
  /// @return 結果のリストを返す．
  vector<const VlTaskFunc*>
  find_task_list(
    const VlScope* parent ///< [in] 親のスコープ
  ) const;

  /// @brief 関数を追加する．
  void
  add_function(
    const VlTaskFunc* func ///< [in] 追加する要素
  );

  /// @brief 関数のリストを取り出す．
  /// @return 結果のリストを返す．
  vector<const VlTaskFunc*>
  find_function_list(
    const VlScope* parent ///< [in] 親のスコープ
  ) const;

  /// @brief continuous assignment を追加する．
  void
  add_contassign(
    const VlContAssign* contassign ///< [in] 登録する要素
  );

  /// @brief continuous assignment のリストを取り出す．
  /// @return 結果のリストを返す．
  vector<const VlContAssign*>
  find_contassign_list(
    const VlScope* parent ///< [in] 親のスコープ
  ) const;

  /// @brief process を追加する．
  void
  add_process(
    const VlProcess* process ///< [in] 登録する要素
  );

  /// @brief process のリストを取り出す．
  /// @return 結果のリストを返す．
  vector<const VlProcess*>
  find_process_list(
    const VlScope* parent ///< [in] 親のスコープ
  ) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で使用される下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief Cell を登録する．
  void
  put_cell(
    const VlScope* parent, ///< [in] 親のスコープ
    int tag,               ///< [in] 要素の型を表すタグ (vpi_user.h 参照)
    TagDictCell* cell      ///< [in] 対象の Cell
  );

  /// @brief タグから該当する Cell を探す．
  TagDictCell*
  find_cell(
    const VlScope* parent, ///< [in] 親のスコープ
    int tag                ///< [in] 要素の型を表すタグ (vpi_user.h 参照)
  ) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で使用されるデータ構造
  //////////////////////////////////////////////////////////////////////

  // 辞書のキーを表す型
  struct Key
  {
    // 親のスコープ
    const VlScope* mParent;

    // タグ
    int mTag;

  };

  // Key のハッシュ関数クラス
  struct Hash
  {
    SizeType
    operator()(
      const Key& key
    ) const
    {
      return ((reinterpret_cast<PtrIntType>(key.mParent) * key.mTag) >> 8);
    }
  };

  // Key の等価比較関数クラス
  struct Eq
  {
    bool
    operator()(
      const Key& left,
      const Key& right
    ) const
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

};

END_NAMESPACE_YM_VERILOG

#endif // TAGDICT_H
