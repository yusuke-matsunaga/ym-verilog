#ifndef OBJDICT_H
#define OBJDICT_H

/// @file ObjDict.h
/// @brief ObjDict のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2020 Yusuke Matsunaga
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
  ObjDict();

  /// @brief デストラクタ
  ~ObjDict();


public:
  //////////////////////////////////////////////////////////////////////
  // ObjDict の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を空にする．
  void
  clear();

  /// @brief スコープを追加する．
  void
  add(const VlScope* obj);

  /// @brief 要素を追加する．
  void
  add(const VlTaskFunc* obj);

  /// @brief 要素を追加する．
  void
  add(ElbDecl* obj);

  /// @brief 要素を追加する．
  void
  add(const VlDeclArray* obj);

  /// @brief 要素を追加する．
  void
  add(ElbParameter* obj);

  /// @brief 要素を追加する．
  void
  add(const VlModule* obj);

  /// @brief 要素を追加する．
  void
  add(const VlModuleArray* obj);

  /// @brief 要素を追加する．
  void
  add(const VlPrimArray* obj);

  /// @brief 要素を追加する．
  void
  add(const VlPrimitive* obj);

  /// @brief 要素を追加する．
  void
  add(ElbGfRoot* obj);

  /// @brief 要素を追加する．
  void
  add(ElbGenvar* obj);

  /// @brief 名前から該当する要素を検索する．
  /// @note なければ nullptr を返す．
  ObjHandle*
  find(const VlScope* parent,
       const string& name) const;


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

};


END_NAMESPACE_YM_VERILOG

#endif // OBJDICT_H
