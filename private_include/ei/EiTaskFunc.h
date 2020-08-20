#ifndef EITASKFUNC_H
#define EITASKFUNC_H

/// @file EiTaskFunc.h
/// @brief EiTaskFunc のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.


#include "elaborator/ElbTaskFunc.h"
#include "ei/EiRange.h"


BEGIN_NAMESPACE_YM_VERILOG

class EiIODecl;

//////////////////////////////////////////////////////////////////////
/// @class EiTask EiTaskFunc.h "EiTaskFunc.h"
/// @brief ElbTaskFunc の実装クラス
//////////////////////////////////////////////////////////////////////
class EiTaskFunc :
  public ElbTaskFunc
{
public:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_item パース木の定義
  /// @param[in] io_num IOの数
  /// @param[in] io_array IO の配列
  EiTaskFunc(const VlScope* parent,
	     const PtItem* pt_item,
	     SizeType io_num);

  /// @brief デストラクタ
  ~EiTaskFunc();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  FileRegion
  file_region() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // VlNamedObj の派生クラスに共通な仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief このオブジェクトの属しているスコープを返す．
  const VlScope*
  parent_scope() const override;

  /// @brief 名前の取得
  string
  name() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // VlTaskFunc の仮想関数(タスク/関数共通)
  //////////////////////////////////////////////////////////////////////

  /// @brief automatic 宣言されていたら true を返す．
  bool
  automatic() const override;

  /// @brief 入出力数を得る．
  SizeType
  io_num() const override;

  /// @brief 入出力の取得
  /// @param[in] pos 位置番号 ( 0 <= pos < io_num() )
  const VlIODecl*
  io(SizeType pos) const override;

  /// @brief 本体のステートメントを得る．
  const VlStmt*
  stmt() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbTaskFunc の仮想関数
  //////////////////////////////////////////////////////////////////////

#if 0
  /// @brief 入出力用のヘッダを生成する．
  // @param[in] pt_header パース木のIO宣言ヘッダ
  ElbIOHead*
  new_IOHead(const PtIOHead* pt_header) override;
#endif

  /// @brief 入出力の初期設定を行う．
  /// @param[in] head ヘッダ
  /// @param[in] pt_item パース木のIO宣言要素
  /// @param[in] decl 対応する宣言要素
  void
  add_iodecl(ElbIOHead* head,
	     const PtIOItem* pt_item,
	     const VlDecl* decl) override;

#if 0
  /// @brief 宣言要素用のヘッダを生成する．
  /// @param[in] pt_header パース木の宣言ヘッダ
  ElbDeclHead*
  new_DeclHead(const PtDeclHead* pt_header) override;

  /// @brief 宣言要素のヘッダを生成する．(IODecl 中の宣言用)
  /// @param[in] pt_head パース木のIO宣言ヘッダ
  /// @param[in] aux_type 補助的なデータ型
  ElbDeclHead*
  new_DeclHead(const PtIOHead* pt_head,
	       VpiAuxType aux_type) override;

  /// @brief 宣言要素のヘッダを生成する．(IODecl 中の宣言用)
  /// @param[in] pt_head パース木のIO宣言ヘッダ
  /// @param[in] aux_type 補助的なデータ型
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @param[in] left_val 範囲の MSB の値
  /// @param[in] right_val 範囲の LSB の値
  ElbDeclHead*
  new_DeclHead(const PtIOHead* pt_head,
	       VpiAuxType aux_type,
	       const PtExpr* left,
	       const PtExpr* right,
	       int left_val,
	       int right_val) override;

  /// @brief 宣言要素のヘッダを生成する．(function の暗黙宣言用)
  /// @param[in] pt_item パース木の関数定義
  ElbDeclHead*
  new_DeclHead(const PtItem* pt_item) override;

  /// @brief 宣言要素のヘッダを生成する．(function の暗黙宣言用)
  /// @param[in] pt_item パース木の関数定義
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @param[in] left_val 範囲の MSB の値
  /// @param[in] right_val 範囲の LSB の値
  ElbDeclHead*
  new_DeclHead(const PtItem* pt_item,
	       const PtExpr* left,
	       const PtExpr* right,
	       int left_val,
	       int right_val) override;
#endif

  /// @brief 本体のステートメントをセットする．
  void
  set_stmt(const VlStmt* stmt) override;


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief PtItem を取り出す．
  const PtItem*
  pt_item() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親のスコープ
  const VlScope* mParent;

  // パース木のタスク/関数定義
  const PtItem* mPtItem;

  // 入出力用ヘッダのリスト
  vector<ElbIOHead*> mIOHeadList;

  // 入出力のリスト
  vector<EiIODecl> mIODeclList;

  // 本体のステートメント
  const VlStmt* mStmt;

};


//////////////////////////////////////////////////////////////////////
/// @class EiTask EiTaskFunc.h "EiTaskFunc.h"
/// @brief タスクを表す EiTaskFunc の実装クラス
//////////////////////////////////////////////////////////////////////
class EiTask :
  public EiTaskFunc
{
public:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_item パース木の定義
  /// @param[in] io_num IOの数
  /// @param[in] io_array IO の配列
  EiTask(const VlScope* parent,
	 const PtItem* pt_item,
	 SizeType io_num);

  /// @brief デストラクタ
  ~EiTask();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  VpiObjType
  type() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // VlTaskFunc の仮想関数(関数の時のみ意味を持つもの)
  //////////////////////////////////////////////////////////////////////

  /// @brief function type を返す．
  VpiFuncType
  func_type() const override;

  /// @brief 符号付きの時 true を返す．
  bool
  is_signed() const override;

  /// @brief 範囲指定を持つとき true を返す．
  bool
  has_range() const override;

  /// @brief 範囲の MSB の値を返す．
  /// @note 範囲を持たないときの値は不定
  int
  left_range_val() const override;

  /// @brief 範囲の LSB の値を返す．
  /// @note 範囲を持たないときの値は不定
  int
  right_range_val() const override;

  /// @brief 範囲のMSBを表す文字列の取得
  /// @note 範囲を持たない時の値は不定
  string
  left_range_string() const override;

  /// @brief 範囲のLSBを表す文字列の取得
  /// @note 範囲を持たない時の値は不定
  string
  right_range_string() const override;

  /// @brief 出力のビット幅を返す．
  SizeType
  bit_size() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbTaskFunc の仮想関数
  // ただし Function の時しか意味を持たない．
  //////////////////////////////////////////////////////////////////////

  /// @brief 出力変数をセットする．
  /// @param[in] ovar 出力変数
  void
  set_ovar(ElbDecl* ovar) override;

  /// @brief constant function の時に true を返す．
  bool
  is_constant_function() const override;

};


//////////////////////////////////////////////////////////////////////
/// @class EiFunction EiTaskFunc.h "EiTaskFunc.h"
/// @brief 関数を表す EiTaskFunc の実装クラス
//////////////////////////////////////////////////////////////////////
class EiFunction :
  public EiTaskFunc
{
public:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_item パース木の定義
  /// @param[in] io_num IOの数
  /// @param[in] io_array IO の配列
  /// @param[in] const_func constant function の時 true にする．
  EiFunction(const VlScope* parent,
	     const PtItem* pt_item,
	     SizeType io_num,
	     bool const_func);

  /// @brief デストラクタ
  ~EiFunction();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  VpiObjType
  type() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // VlTaskFunc の仮想関数(関数の時のみ意味を持つもの)
  //////////////////////////////////////////////////////////////////////

  /// @brief function type を返す．
  VpiFuncType
  func_type() const override;

  /// @brief 符号付きの時 true を返す．
  bool
  is_signed() const override;

  /// @brief 範囲指定を持つとき true を返す．
  bool
  has_range() const override;

  /// @brief 範囲の MSB の値を返す．
  /// @note 範囲を持たないときの値は不定
  int
  left_range_val() const override;

  /// @brief 範囲の LSB の値を返す．
  /// @note 範囲を持たないときの値は不定
  int
  right_range_val() const override;

  /// @brief 範囲のMSBを表す文字列の取得
  /// @note 範囲を持たない時の値は不定
  string
  left_range_string() const override;

  /// @brief 範囲のLSBを表す文字列の取得
  /// @note 範囲を持たない時の値は不定
  string
  right_range_string() const override;

  /// @brief 出力のビット幅を返す．
  SizeType
  bit_size() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbTaskFunc の仮想関数
  // ただし Function の時しか意味を持たない．
  //////////////////////////////////////////////////////////////////////

  /// @brief 出力変数をセットする．
  /// @param[in] ovar 出力変数
  void
  set_ovar(ElbDecl* ovar) override;

  /// @brief constant function の時に true を返す．
  bool
  is_constant_function() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 出力変数
  ElbDecl* mOvar;

  // 定数関数フラグ
  bool mConstFunc;

};


//////////////////////////////////////////////////////////////////////
/// @class EiFunctionV EiaskFunc.h "EiTaskFunc.h"
/// @brief ベクタ型の関数を表すクラス
//////////////////////////////////////////////////////////////////////
class EiFunctionV :
  public EiFunction
{
public:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_item パース木の定義
  /// @param[in] io_num IOの数
  /// @param[in] io_array IO の配列
  /// @param[in] left 範囲の MSB の式
  /// @param[in] right 範囲の LSB の式
  /// @param[in] left_val 範囲の MSB の値
  /// @param[in] right_val 範囲の LSB の値
  /// @param[in] const_func 定数関数フラグ
  EiFunctionV(const VlScope* parent,
	      const PtItem* pt_item,
	      SizeType io_num,
	      const PtExpr* left,
	      const PtExpr* right,
	      int left_val,
	      int right_val,
	      bool const_func);

  /// @brief デストラクタ
  ~EiFunctionV();


public:
  //////////////////////////////////////////////////////////////////////
  // VlFunction の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 範囲指定を持つとき true を返す．
  bool
  has_range() const override;

  /// @brief 範囲の MSB の値を返す．
  /// @note 範囲を持たないときの値は不定
  int
  left_range_val() const override;

  /// @brief 範囲の LSB の値を返す．
  /// @note 範囲を持たないときの値は不定
  int
  right_range_val() const override;

  /// @brief 範囲のMSBを表す文字列の取得
  /// @note 範囲を持たない時の値は不定
  string
  left_range_string() const override;

  /// @brief 範囲のLSBを表す文字列の取得
  /// @note 範囲を持たない時の値は不定
  string
  right_range_string() const override;

  /// @brief 出力のビット幅を返す．
  SizeType
  bit_size() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 範囲
  EiRangeImpl mRange;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief PtItem を取り出す．
inline
const PtItem*
EiTaskFunc::pt_item() const
{
  return mPtItem;
}

END_NAMESPACE_YM_VERILOG

#endif // EITASKFUNC_H
