﻿#ifndef EIIODECL_H
#define EIIODECL_H

/// @file EiIODecl.h
/// @brief EiIODecl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ElbIODecl.h"
#include "EiRange.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiIOHead  EiIODecl.h "EiIODecl.h"
/// @brief IO のヘッダの基底クラス
//////////////////////////////////////////////////////////////////////
class EiIOHead :
  public ElbIOHead
{
protected:

  /// @brief コンストラクタ
  /// @param[in] pt_header パース木のIO宣言ヘッダ
  EiIOHead(const PtIOHead* pt_header);

  /// @brief デストラクタ
  virtual
  ~EiIOHead();


public:
  //////////////////////////////////////////////////////////////////////
  // ElbIOHead の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 方向を返す．
  virtual
  tVlDirection
  direction() const;

  /// @brief 親のモジュールの取得
  /// @note このクラスでは nullptr を返す．
  virtual
  ElbModule*
  module() const;

  /// @brief 親のタスクの取得
  /// @note このクラスでは nullptr を返す．
  virtual
  ElbTaskFunc*
  task() const;

  /// @brief 親の関数の取得
  /// @note このクラスでは nullptr を返す．
  virtual
  ElbTaskFunc*
  function() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // パース木の IO 宣言ヘッダ
  const PtIOHead* mPtHead;

};


//////////////////////////////////////////////////////////////////////
/// @class EiModIOHead  EiIODecl.h "EiIODecl.h"
/// @brief module 用の IO のヘッダ
//////////////////////////////////////////////////////////////////////
class EiModIOHead :
  public EiIOHead
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] module 親のモジュール
  /// @param[in] pt_header パース木のIO宣言ヘッダ
  EiModIOHead(ElbModule* module,
	      const PtIOHead* pt_header);

  /// @brief デストラクタ
  virtual
  ~EiModIOHead();


public:
  //////////////////////////////////////////////////////////////////////
  // EiIOHead の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 親のモジュールの取得
  ElbModule*
  module() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親のモジュール
  ElbModule* mModule;

};


//////////////////////////////////////////////////////////////////////
/// @class EiTaskIOHead EiIODecl.h "EiIODecl.h"
/// @brief タスク用の IO のヘッダ
//////////////////////////////////////////////////////////////////////
class EiTaskIOHead :
  public EiIOHead
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] task 親のタスク
  /// @param[in] pt_header パース木のIO宣言ヘッダ
  EiTaskIOHead(ElbTaskFunc* task,
	     const PtIOHead* pt_header);

  /// @brief デストラクタ
  virtual
  ~EiTaskIOHead();


public:
  //////////////////////////////////////////////////////////////////////
  // EiIOHead の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 親のタスクの取得
  ElbTaskFunc*
  task() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親のタスク
  ElbTaskFunc* mTask;

};


//////////////////////////////////////////////////////////////////////
/// @class EiFunctionIOHead EiIODecl.h "EiIODecl.h"
/// @brief 関数用の IO のヘッダ
//////////////////////////////////////////////////////////////////////
class EiFunctionIOHead :
  public EiIOHead
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] func 親の関数
  /// @param[in] pt_header パース木のIO宣言ヘッダ
  EiFunctionIOHead(ElbTaskFunc* func,
		   const PtIOHead* pt_header);

  /// @brief デストラクタ
  virtual
  ~EiFunctionIOHead();


public:
  //////////////////////////////////////////////////////////////////////
  // EiIOHead の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 親の関数の取得
  ElbTaskFunc*
  function() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親の関数
  ElbTaskFunc* mFunction;

};


//////////////////////////////////////////////////////////////////////
/// @class EiIODecl EiIODecl.h "EiIODecl.h"
/// @brief IO 要素を表すクラス
//////////////////////////////////////////////////////////////////////
class EiIODecl :
  public ElbIODecl
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  EiIODecl();

  /// @brief デストラクタ
  virtual
  ~EiIODecl();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  virtual
  tVpiObjType
  type() const;

  /// @brief ファイル位置を返す．
  virtual
  FileRegion
  file_region() const;


public:
  //////////////////////////////////////////////////////////////////////
  // VlIODecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を返す．
  virtual
  const char*
  name() const;

  /// @brief 方向を返す．
  virtual
  tVlDirection
  direction() const;

  /// @brief 符号の取得
  /// @retval true 符号つき
  /// @retval false 符号なし
  virtual
  bool
  is_signed() const;

  /// @brief 範囲指定を持つとき true を返す．
  virtual
  bool
  has_range() const;

  /// @brief 範囲の MSB の値を返す．
  /// @note 範囲を持たないときの値は不定
  virtual
  int
  left_range_val() const;

  /// @brief 範囲の LSB の値を返す．
  /// @note 範囲を持たないときの値は不定
  virtual
  int
  right_range_val() const;

  /// @brief 範囲のMSBを表す文字列の取得
  /// @note 範囲を持たない時の値は不定
  virtual
  string
  left_range_string() const;

  /// @brief 範囲のLSBを表す文字列の取得
  /// @note 範囲を持たない時の値は不定
  virtual
  string
  right_range_string() const;

  /// @brief ビット幅を返す．
  virtual
  ymuint
  bit_size() const;

  /// @brief 対応する宣言要素を返す．
  virtual
  const VlDecl*
  decl() const;

  /// @brief 親のモジュールの取得
  virtual
  const VlModule*
  module() const;

  /// @brief 親の UDP の取得
  /// @note このクラスでは nullptr を返す．
  virtual
  const VlUdpDefn*
  udp_defn() const;

  /// @brief 親のタスクの取得
  virtual
  const VlTaskFunc*
  task() const;

  /// @brief 親の関数の取得
  virtual
  const VlTaskFunc*
  function() const;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbIODecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 対応する ElbDecl を返す．
  virtual
  ElbDecl*
  _decl() const;


public:
  //////////////////////////////////////////////////////////////////////
  // EiIODecl の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期化を行う．
  /// @param[in] head ヘッダ
  /// @param[in] pt_item パース木のIO宣言要素
  /// @param[in] decl 対応する宣言要素
  /// @param[in] 符号付き属性の補正値
  void
  init(ElbIOHead* head,
       const PtIOItem* pt_item,
       ElbDecl* decl);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ヘッダ
  ElbIOHead* mHead;

  // パース木の IO 宣言
  const PtIOItem* mPtItem;

  // 対応する宣言要素
  ElbDecl* mDecl;

};

END_NAMESPACE_YM_VERILOG

#endif // EIIODECL_H
