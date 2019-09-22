﻿#ifndef YM_PT_PTMISC_H
#define YM_PT_PTMISC_H

/// @file ym/pt/PtMisc.h
/// @brief その他のの部品クラスのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym/pt/PtBase.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class PtControl PtMisc.h <ym/pt/PtMisc.h>
/// @ingroup VlParser
/// @ingroup PtGroup
/// @brief delay control/event control/repeat control を表すクラス
//////////////////////////////////////////////////////////////////////
class PtControl :
  public PtBase
{
public:

  /// @brief 仮想デストラクタ
  virtual
  ~PtControl() { }


public:
  //////////////////////////////////////////////////////////////////////
  // PtControl の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  /// @return 型
  virtual
  tPtCtrlType
  type() const = 0;

  /// @brief 遅延式の取得
  /// @retval 遅延を表す式 delay control の場合
  /// @retval nullptr 上記以外
  virtual
  const PtExpr*
  delay() const = 0;

  /// @brief イベントリストの取得
  /// @note event control/repeat control の場合のみ意味を持つ
  virtual
  PtExprArray
  event_list() const = 0;

  /// @brief 繰り返し数の取得
  /// @retval 繰り返し数を表す式 repeat control の場合
  /// @retval nullptr 上記以外
  virtual
  const PtExpr*
  rep_expr() const = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class PtConnection PtMisc.h <ym/pt/PtMisc.h>
/// @ingroup VlParser
/// @ingroup PtGroup
/// @brief orered_connection/named_connection を表すクラス
//////////////////////////////////////////////////////////////////////
class PtConnection :
  public PtBase
{
public:

  /// @brief 仮想デストラクタ
  virtual
  ~PtConnection() { }


public:
  //////////////////////////////////////////////////////////////////////
  // PtConnection の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前の取得
  /// @retval 名前 named connection の場合
  /// @retval "" ordered connection の場合
  virtual
  const char*
  name() const = 0;

  /// @brief 式の取得
  /// @return 式
  virtual
  const PtExpr*
  expr() const = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class PtStrength PtMisc.h <ym/pt/PtMisc.h>
/// @ingroup VlParser
/// @ingroup PtGroup
/// @brief strength を表すクラス
//////////////////////////////////////////////////////////////////////
class PtStrength :
  public PtBase
{
public:

  /// @brief 仮想デストラクタ
  virtual
  ~PtStrength() { }


public:
  //////////////////////////////////////////////////////////////////////
  // PtStrength の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief drive strength0 の取得
  /// @return 0 の強度
  virtual
  tVpiStrength
  drive0() const = 0;

  /// @brief drive strength1 の取得
  /// @return 1 の強度
  virtual
  tVpiStrength
  drive1() const = 0;

  /// @brief charge strength の取得
  /// @return 電荷の強度
  virtual
  tVpiStrength
  charge() const = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class PtDelay PtMisc.h <ym/pt/PtMisc.h>
/// @ingroup VlParser
/// @ingroup PtGroup
/// @brief delay を表すクラス
//////////////////////////////////////////////////////////////////////
class PtDelay :
  public PtBase
{
public:

  /// @brief 仮想デストラクタ
  virtual
  ~PtDelay() { }


public:
  //////////////////////////////////////////////////////////////////////
  // PtDelay の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値の取得
  /// @param[in] pos 取得する遅延値の位置(0 〜 2)
  /// @return pos 番目の遅延を表す式\n
  /// 該当する要素がなければ nullptr を返す．
  virtual
  const PtExpr*
  value(SizeType pos) const = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class PtNameBranch PtMisc.h <ym/pt/PtMisc.h>
/// @ingroup VlParser
/// @ingroup PtGroup
/// @brief 階層名を表すクラス
//////////////////////////////////////////////////////////////////////
class PtNameBranch
{
public:

  /// @brief 仮想デストラクタ
  virtual
  ~PtNameBranch() { }


public:

  /// @brief 名前の取得
  /// @return 名前
  virtual
  const char*
  name() const = 0;

  /// @brief インデックスの有無のチェック
  /// @retval true インデックスを持っている時
  /// @retval false インデックスを持っていない時
  /// @note デフォルトで false を返す．
  virtual
  bool
  has_index() const = 0;

  /// @brief インデックスの取得
  /// @return インデックスの値
  /// @note デフォルトで 0 を返す．
  virtual
  int
  index() const = 0;

};

/// @relates PtNameBranch
/// @brief 階層名を作り出す関数
/// @param[in] nb_array 階層ブランチリスト
/// @param[in] name 末尾の名前
/// @return 階層名を展開したものを返す．
string
expand_full_name(const PtNameBranchArray& nb_array,
		 const char* name);


//////////////////////////////////////////////////////////////////////
/// @class PtAttrInst PtMisc.h <ym/pt/PtMisc.h>
/// @ingroup VlParser
/// @ingroup PtGroup
/// @brief attribute_instance を表すクラス
//////////////////////////////////////////////////////////////////////
class PtAttrInst
{
public:

  /// @brief 仮想デストラクタ
  virtual
  ~PtAttrInst() { }


public:
  //////////////////////////////////////////////////////////////////////
  // PtAttrInst の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素のリストの取得
  virtual
  PtAttrSpecArray
  attrspec_list() const = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class PtAttrSpec PtMisc.h <ym/pt/PtMisc.h>
/// @ingroup VlParser
/// @ingroup PtGroup
/// @brief attr_spec を表すクラス
//////////////////////////////////////////////////////////////////////
class PtAttrSpec
{
public:

  /// @brief 仮想デストラクタ
  virtual
  ~PtAttrSpec() { }


public:
  //////////////////////////////////////////////////////////////////////
  // PtAttrSpec の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置の取得
  /// @return ファイル位置
  virtual
  FileRegion
  file_region() const = 0;

  /// @brief 名前の取得
  /// @return 名前
  virtual
  const char*
  name() const = 0;

  /// @brief 式の取得
  /// @return 式
  virtual
  const PtExpr*
  expr() const = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // YM_PT_PTMISC_H
