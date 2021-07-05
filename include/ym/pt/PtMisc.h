#ifndef YM_PT_PTMISC_H
#define YM_PT_PTMISC_H

/// @file ym/pt/PtMisc.h
/// @brief その他のの部品クラスのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014, 2021 Yusuke Matsunaga
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
  //////////////////////////////////////////////////////////////////////
  // PtControl の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  /// @return 型
  virtual
  PtCtrlType
  type() const = 0;

  /// @brief 遅延式の取得
  /// @retval 遅延を表す式 delay control の場合
  /// @retval nullptr 上記以外
  virtual
  const PtExpr*
  delay() const = 0;

  /// @brief イベントリストの要素数の取得
  ///
  /// event control/repeat control の場合のみ意味を持つ
  virtual
  SizeType
  event_num() const = 0;

  /// @brief イベントリストの要素の取得
  ///
  /// event control/repeat control の場合のみ意味を持つ
  virtual
  const PtExpr*
  event(
    SizeType pos ///< [in] 位置 ( 0 <= pos < event_num() )
  ) const = 0;

  /// @brief イベントリストの取得
  vector<const PtExpr*>
  event_list() const
  {
    SizeType n = event_num();
    vector<const PtExpr*> vec(n);
    for ( SizeType i = 0; i < n; ++ i ) {
      vec[i] = event(i);
    }
    return vec;
  }

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
  //////////////////////////////////////////////////////////////////////
  // PtStrength の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief drive strength0 の取得
  /// @return 0 の強度
  virtual
  VpiStrength
  drive0() const = 0;

  /// @brief drive strength1 の取得
  /// @return 1 の強度
  virtual
  VpiStrength
  drive1() const = 0;

  /// @brief charge strength の取得
  /// @return 電荷の強度
  virtual
  VpiStrength
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
  //////////////////////////////////////////////////////////////////////
  // PtDelay の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値の取得
  /// @return pos 番目の遅延を表す式\n
  /// 該当する要素がなければ nullptr を返す．
  virtual
  const PtExpr*
  value(
    SizeType pos ///< [in] 取得する遅延値の位置(0 〜 2)
  ) const = 0;

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

  /// @brief デストラクタ
  virtual
  ~PtNameBranch() = default;


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

  /// @brief インデックスを含めた名前を返す．
  string
  expand_name() const
  {
    ostringstream buf;
    buf << name();
    if ( has_index() ) {
      buf << "[" << index() << "]";
    }
    return buf.str();
  }

};


//////////////////////////////////////////////////////////////////////
/// @class PtAttrInst PtMisc.h <ym/pt/PtMisc.h>
/// @ingroup VlParser
/// @ingroup PtGroup
/// @brief attribute_instance を表すクラス
//////////////////////////////////////////////////////////////////////
class PtAttrInst :
  public PtBase
{
public:
  //////////////////////////////////////////////////////////////////////
  // PtAttrInst の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素数の取得
  virtual
  SizeType
  attrspec_num() const = 0;

  /// @brief 要素の取得
  virtual
  const PtAttrSpec*
  attrspec(
    SizeType pos ///< [in] 位置 ( 0 <= pos < attrspec_num() )
  ) const = 0;

  /// @brief 要素のリストの取得
  vector<const PtAttrSpec*>
  attrspec_list() const
  {
    SizeType n = attrspec_num();
    vector<const PtAttrSpec*> vec(n);
    for ( SizeType i = 0; i < n; ++ i ) {
      vec[i] = attrspec(i);
    }
    return vec;
  }

};


//////////////////////////////////////////////////////////////////////
/// @class PtAttrSpec PtMisc.h <ym/pt/PtMisc.h>
/// @ingroup VlParser
/// @ingroup PtGroup
/// @brief attr_spec を表すクラス
//////////////////////////////////////////////////////////////////////
class PtAttrSpec :
  public PtBase
{
public:
  //////////////////////////////////////////////////////////////////////
  // PtAttrSpec の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

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
