#ifndef YM_PT_PTBASE_H
#define YM_PT_PTBASE_H

/// @file ym/pt/PtBase.h
/// @brief Pt の基底クラスのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym/verilog.h"
#include "ym/pt/PtP.h"
#include "ym/FileRegion.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class PtBase PtBase.h <ym/pt/PtBase.h>
/// @ingroup VlParser
/// @ingroup PtGroup
/// @brief 全ての Pt クラスの基底クラス
//////////////////////////////////////////////////////////////////////
class PtBase
{
public:

  /// @brief デストラクタ
  virtual
  ~PtBase() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // PtBase の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置の取得
  /// @return ファイル位置
  virtual
  FileRegion
  file_region() const = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class PtNamedBase PtBase.h <ym/pt/PtBase.h>
/// @ingroup VlParser
/// @ingroup PtGroup
/// @brief 名前を持つ PtBase
//////////////////////////////////////////////////////////////////////
class PtNamedBase :
  public PtBase
{
public:
  //////////////////////////////////////////////////////////////////////
  // PtNamedBase の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前の取得
  /// @return 名前
  virtual
  const char*
  name() const = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class PtHierNamedBase PtBase.h <ym/pt/PtBase.h>
/// @ingroup VlParser
/// @ingroup PtGroup
/// @brief 階層付き名前を持つ PtBase
//////////////////////////////////////////////////////////////////////
class PtHierNamedBase :
  public PtNamedBase
{
public:
  //////////////////////////////////////////////////////////////////////
  // PtHierNamedBase の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 階層ブランチの要素数の取得
  virtual
  SizeType
  namebranch_num() const = 0;

  /// @brief 階層ブランチの取得
  /// @param[in] pos 位置 ( 0 <= pos < namebranch_num() )
  virtual
  const PtNameBranch*
  namebranch(SizeType pos) const = 0;

  /// @brief 階層名を展開する．
  string
  fullname() const;

  /// @brief 階層ブランチのリストを返す．
  vector<const PtNameBranch*>
  namebranch_list() const
  {
    SizeType n = namebranch_num();
    vector<const PtNameBranch*> vec(n);
    for ( SizeType i = 0; i < n; ++ i ) {
      vec[i] = namebranch(i);
    }
    return vec;
  }

};

END_NAMESPACE_YM_VERILOG

#endif // YM_PT_PTBASE_H
