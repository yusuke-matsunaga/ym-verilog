#ifndef PTMGR_H
#define PTMGR_H

/// @file PtMgr.h
/// @brief PtMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym/pt/PtP.h"
#include "alloc/SimpleAlloc.h"


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

  /// @brief アロケータを返す．
  PtAlloc&
  alloc();

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
  check_def_name(const char* name) const;


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
  reg_defname(const char* name);

  /// @brief 文字列領域を確保する．
  /// @param[in] str 文字列
  /// @return 文字列を返す．
  ///
  /// 同一の文字列は共有する．
  const char*
  save_string(const char* str);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // メモリアロケータ
  SimpleAlloc mAlloc;

  // UDP 定義のリスト
  vector<const PtUdp*> mUdpList;

  // モジュール定義のリスト
  vector<const PtModule*> mModuleList;

  // インスタンス記述で用いられている名前
  // たぶんモジュール名か UDP名のはず
  unordered_set<string> mDefNames;

  // 文字列の辞書
  unordered_set<string> mStringPool;

};

END_NAMESPACE_YM_VERILOG

#endif // PTMGR_H
