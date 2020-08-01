#ifndef OBJDICT_H
#define OBJDICT_H

/// @file ObjDict.h
/// @brief ObjDict のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym/verilog.h"
#include "ObjHandle.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ObjDict ObjDict.h "ObjDict.h"
/// @brief VlNamedObj を格納するハッシュ表
//////////////////////////////////////////////////////////////////////
class ObjDict
{
public:

  /// @brief コンストラクタ
  ObjDict() = default;

  /// @brief デストラクタ
  ~ObjDict() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // ObjDict の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を空にする．
  void
  clear();

  /// @brief 要素を追加する．
  void
  add(const VlNamedObj* obj);

  /// @brief 要素を追加する．
  void
  add(ElbTaskFunc* obj);

  /// @brief 要素を追加する．
  void
  add(ElbDecl* obj);

  /// @brief 要素を追加する．
  void
  add(ElbDeclArray* obj);

  /// @brief 要素を追加する．
  void
  add(ElbParameter* obj);

  /// @brief 要素を追加する．
  void
  add(ElbModuleArray* obj);

  /// @brief 要素を追加する．
  void
  add(ElbPrimArray* obj);

  /// @brief 要素を追加する．
  void
  add(ElbPrimitive* obj);

  /// @brief 要素を追加する．
  void
  add(ElbGfRoot* obj);

  /// @brief 要素を追加する．
  void
  add(ElbGenvar* obj);

  /// @brief 名前から該当する要素を検索する．
  /// @note なければ nullptr を返す．
  ObjHandle*
  find(const VlNamedObj* parent,
       const char* name) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ハンドルを追加する．
  void
  add_handle(ObjHandle* handle);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ObjHandle を納めるハッシュ表
  unordered_set<ObjHandle*, ObjHandleHash, ObjHandleEq> mHash;

  // ObjHandle のリスト
  vector<unique_ptr<ObjHandle>> mHandleList;

};


END_NAMESPACE_YM_VERILOG

#endif // OBJDICT_H
