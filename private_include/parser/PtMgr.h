#ifndef PTMGR_H
#define PTMGR_H

/// @file PtMgr.h
/// @brief PtMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.


#include "ym/pt/PtP.h"
#include "alloc/Alloc.h"
#include "parser/PtrList.h"
#include "PtiAttrInfo.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class PtMgr PtMgr.h <ym/PtMgr.h>
/// @ingroup PtMgr
/// @brief Verilog のパース木を管理するクラス
//////////////////////////////////////////////////////////////////////
class PtMgr
{
  friend class Parser;

public:

  /// @brief コンストラクタ
  PtMgr();

  /// @brief デストラクタ
  ~PtMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 登録されているモジュールのリストを返す．
  /// @return 登録されているモジュールのリスト
  const vector<const PtModule*>&
  pt_module_list() const;

  /// @brief 登録されている UDP のリストを返す．
  /// @return 登録されている UDP のリスト
  const vector<const PtUdp*>&
  pt_udp_list() const;

  /// @brief インスタンス記述で用いられている名前かどうか調べる．
  /// @param[in] name 調べる名前
  /// @return 用いられていたら true を返す．
  bool
  check_def_name(const string& name) const;

  /// @brief 関数を探す．
  /// @param[in] module 親のモジュール
  /// @param[in] name 関数名
  ///
  /// なければ nullptr を返す．
  const PtItem*
  find_function(const PtModule* module,
		const string& name) const;

  /// @brief attribute instance を取り出す．
  /// @param[in] pt_obj 対象の構文木の要素
  /// @return PtAttrInst のリスト
  ///
  /// 空の場合もある．
  vector<const PtAttrInst*>
  find_attr_list(const PtBase* pt_obj) const;

  /// @brief 全ての属性リストのリストを返す．
  vector<const PtiAttrInfo>
  all_attr_list() const;


public:
  //////////////////////////////////////////////////////////////////////
  // データをセットする関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 今までに生成したインスタンスをすべて破壊する．
  void
  clear();

  /// @brief UDP 定義を追加する．
  ///
  /// 内部で reg_pt() を呼んでいる．
  void
  reg_udp(const PtUdp* udp);

  /// @brief モジュール定義を追加する．
  ///
  /// 内部で reg_pt() を呼んでいる．
  void
  reg_module(const PtModule* module);

  /// @brief インスタンス定義名を追加する．
  void
  reg_defname(const string& name);

  /// @brief attribute instance を登録する．
  void
  reg_attrinst(const PtBase* pt_obj,
	       PtrList<const PtAttrInst>* ai_list,
	       bool def = false);

  /// @brief 文字列領域を確保する．
  /// @param[in] str 文字列
  /// @return 文字列を返す．
  ///
  /// 同一の文字列は共有する．
  const char*
  save_string(const char* str);


public:
  //////////////////////////////////////////////////////////////////////
  // メンバにアクセスする関数
  //////////////////////////////////////////////////////////////////////

  /// @brief メモリアロケーターを返す．
  Alloc&
  alloc();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // メモリアロケーター
  unique_ptr<Alloc> mAlloc;

  // UDP 定義のリスト
  vector<const PtUdp*> mUdpList;

  // モジュール定義のリスト
  vector<const PtModule*> mModuleList;

  // インスタンス記述で用いられている名前
  // たぶんモジュール名か UDP名のはず
  unordered_set<string> mDefNames;

  // 文字列の辞書
  unordered_set<string> mStringPool;

  struct AttrHash
  {
    SizeType
    operator()(const PtiAttrInfo& attr_info) const
    {
      SizeType tmp{reinterpret_cast<SizeType>(attr_info.obj())};
      return (tmp * tmp) >> 16;
    }
  };

  struct AttrEq
  {
    bool
    operator()(const PtiAttrInfo& attr_info1,
	       const PtiAttrInfo& attr_info2) const
    {
      return attr_info1.obj() == attr_info2.obj();
    }
  };

  // 属性リストの辞書
  // PtBase* をキーにする．
  unordered_set<PtiAttrInfo, AttrHash, AttrEq> mAttrDict;

};

END_NAMESPACE_YM_VERILOG

#endif // PTMGR_H
