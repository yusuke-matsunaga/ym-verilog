#ifndef ELBSTUBLIST_H
#define ELBSTUBLIST_H

/// @file ElbStubList.h
/// @brief ElbStubList のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.


#include "ym/verilog.h"


BEGIN_NAMESPACE_YM_VERILOG

class ElbStub;

//////////////////////////////////////////////////////////////////////
/// @class ElbStubList ElbStub.h "ElbStub.h"
/// @brief ElbStub のリストを表すクラス
//////////////////////////////////////////////////////////////////////
class ElbStubList
{
public:

  /// @brief コンストラクタ
  ElbStubList() = default;

  /// @brief コピーコンストラクタ
  /// @param[in] src コピー元のオブジェクト
  ElbStubList(const ElbStubList& src) = default;

  /// @brief ムーブコンストラクタ
  /// @param[in] src ムーブ元のオブジェクト
  ElbStubList(ElbStubList&& src) = default;

  /// @brief コピー代入演算子
  /// @param[in] src コピー元のオブジェクト
  ElbStubList&
  operator=(const ElbStubList& src) = default;

  /// @breif ムーブ代入演算子
  /// @param[in] src ムーブ元のオブジェクト
  ElbStubList&
  operator=(ElbStubList&& src) = default;

  /// @brief デストラクタ
  ~ElbStubList();


public:

  /// @brief 末尾に要素を追加する．
  void
  push_back(ElbStub* elem);

  /// @brief 空の時 true を返す．
  bool
  empty() const;

  /// @brief 要素の stub を評価する．
  ///
  /// 結果としてリストは空になる．
  void
  eval();

private:

  /// @brief 内容を空にする．
  void
  clear();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // リストの本体
  vector<ElbStub*> mList;

};

END_NAMESPACE_YM_VERILOG

#endif // ELBSTUB_H
