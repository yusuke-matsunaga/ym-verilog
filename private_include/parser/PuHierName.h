#ifndef LIBYM_YM_PARSER_PUHIERNAME_H
#define LIBYM_YM_PARSER_PUHIERNAME_H

/// @file parser/PuHierName.h
/// @brief PuHierName のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2006, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "parser/PtiFwd.h"
#include "parser/PtrList.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class PuHeirName Putils.h "parser/Putils.h"
/// @ingroup VlParser
/// @brief 階層つき名を表すクラス
///
/// 中身は階層部分を表す PtNameBranch のリストと末尾の名前を表す
/// 文字列から成る．
///
/// コンストラクタでは最下層の名前の設定のみを行う．
/// 階層の追加は add(const char*), add(int index, const char*)
/// で行う．これは内部で PtNameBranch を生成していることに因る．
//////////////////////////////////////////////////////////////////////
class PuHierName
{
public:

  /// @brief コンストラクタ
  PuHierName(
    const PtNameBranch* nb, ///< [in] 階層ブランチ
    const char* name        ///< [in] 名前
  ) : mNbList{new PtrList<const PtNameBranch>},
      mTailName(name)
  {
    mNbList->push_back(nb);
  }

  /// @brief デストラクタ
  ~PuHierName()
  {
    delete mNbList;
  }


public:

  /// @brief 階層を追加する．
  void
  add(
    const PtNameBranch* nb, ///< [in] 追加する階層ブランチ
    const char* tail_name   ///< [in] 追加する最下層の名前
  )
  {
    mNbList->push_back(nb);
    mTailName = tail_name;
  }


public:

  /// @brief 階層ブランチを PtNameBranchArray の形で取り出す．
  ///
  /// この関数を呼ぶと mNbList は破壊される．
  vector<const PtNameBranch*>
  name_branch_to_vector()
  {
    auto ans{mNbList->to_vector()};
    delete mNbList;
    mNbList = nullptr;
    return ans;
  }

  /// @brief 最下層の名前を取り出す．
  /// @return 最下層の名前
  const char*
  tail_name() const
  {
    return mTailName;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 階層ブランチのリスト
  PtrList<const PtNameBranch>* mNbList;

  // 最下層の名前
  const char* mTailName;

};

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_YM_PARSER_PUHIERNAME_H
