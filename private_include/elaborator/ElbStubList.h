#ifndef ELBSTUBLIST_H
#define ELBSTUBLIST_H

/// @file ElbStubList.h
/// @brief ElbStubList のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2020, 2021 Yusuke Matsunaga
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
  ElbStubList(
    const ElbStubList& src ///< [in] コピー元のオブジェクト
  ) = default;

  /// @brief ムーブコンストラクタ
  ElbStubList(
    ElbStubList&& src ///< [in] ムーブ元のオブジェクト
  ) = default;

  /// @brief コピー代入演算子
  ElbStubList&
  operator=(
    const ElbStubList& src ///< [in] コピー元のオブジェクト
  ) = default;

  /// @breif ムーブ代入演算子
  ElbStubList&
  operator=(
    ElbStubList&& src ///< [in] ムーブ元のオブジェクト
  ) = default;

  /// @brief デストラクタ
  ~ElbStubList();


public:

  /// @brief 末尾に要素を追加する．
  void
  push_back(
    ElbStub* elem ///< [in] 追加する要素
  );

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
