#ifndef EIPORT_H
#define EIPORT_H

/// @file EiPort.h
/// @brief EiPort のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ElbPort.h"
#include "ym/pt/PtP.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiPort EiPort.h "EiPort.h"
/// @brief ElbPort の実装クラス
//////////////////////////////////////////////////////////////////////
class EiPort :
  public ElbPort
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  EiPort();

  /// @brief デストラクタ
  ~EiPort();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  VpiObjType
  type() const override;

  /// @brief ファイル位置を返す．
  FileRegion
  file_region() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // VlPort の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入出力の区別を得る．
  VpiDir
  direction() const override;

  /// @brief ビット幅を返す．
  SizeType
  bit_size() const override;

  /// @brief 名前による接続を持つとき true を返す．
  bool
  is_conn_by_name() const override;

  /// @brief 明示的に名前がついているとき true を返す．
  bool
  is_explicit_name() const override;

  /// @brief 名前を返す．
  const char*
  name() const override;

  /// @brief 親のモジュールを取出す
  const VlModule*
  module() const override;

  /// @brief ポートリストの何番目のポートかを表すインデックスを返す．
  int
  port_index() const override;

  /// @brief 上位の接続先を返す．
  const VlExpr*
  high_conn() const override;

  /// @brief 下位の接続先を返す．
  const VlExpr*
  low_conn() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // EiPortの関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期設定を行う．
  /// @param[in] module 親のモジュール
  /// @param[in] pt_port パース木のポート定義
  /// @param[in] index ポート番号
  /// @param[in] low_conn 下位の接続
  /// @param[in] dir 向き
  void
  init(ElbModule* parent,
       const PtPort* pt_port,
       int index,
       ElbExpr* low_conn,
       VpiDir dir);

  /// @brief high_conn を接続する．
  void
  set_high_conn(ElbExpr* high_conn,
		bool conn_by_name);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親のモジュール
  ElbModule* mModule;

  // パース木のポート定義
  const PtPort* mPtPort;

  // ポート番号
  int mIndex;

  // 上位の接続先
  ElbExpr* mHighConn;

  // 下位の接続先
  ElbExpr* mLowConn;

  // 向き
  VpiDir mDirection;

  // 名前による結合の時 true となるフラグ
  bool mConnByName;

};

END_NAMESPACE_YM_VERILOG

#endif // EIPORT_H
