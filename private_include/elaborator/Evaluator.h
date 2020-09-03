#ifndef EVALUATOR_H
#define EVALUATOR_H

/// @file Evaluator.h
/// @brief Evaluator のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2020 Yusuke Matsunaga
/// All rights reserved.

#include "ym/verilog.h"
#include "ym/pt/PtP.h"
#include "ym/vl/VlFwd.h"
#include "ym/VlValue.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class Evaluator Evaluator.h "Evaluator.h"
/// @brief 関数の値の評価を行うクラス
///
/// constant expression を elaboration 中に評価するために用いる．
/// constant expression は constant function call を含むので中には
/// ステートメントの実行を伴う．
/// しかし再帰呼び出しはないので一つの変数に対するオブジェクトは
/// 唯一に定まる．
/// そこで，変数に対する値割り当てを unordered_map で保持する．
///
/// 注意が必要なのは変数の中には配列型のものがあるということ．
/// 一括して扱うために単独の変数も要素数１でオフセットが０の配列
/// 要素とみなす．
/// ただし，これは値の割り当てマップのなかだけの話．
//////////////////////////////////////////////////////////////////////
class Evaluator
{
public:

  /// @brief コンストラクタ
  /// @param[in] function 関数
  /// @param[in] put_error エラーを出力する時 true にするフラグ
  Evaluator(const VlTaskFunc* function,
	    bool put_error);

  /// @brief デストラクタ
  ~Evaluator();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 関数を評価する．
  /// @param[in] arg_list 引数のリスト
  VlValue
  operator()(const vector<VlValue>& arg_list);


private:
  //////////////////////////////////////////////////////////////////////
  // evaluate_stmt() で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ステートメントの実行を行う．
  /// @param[in] stmt 対象のステートメント
  /// @return disable が実行された場合はスコープを返す．
  const VlScope*
  evaluate_stmt(const VlStmt* stmt);

  /// @brief begin-end ブロックの実行を行う．
  /// @param[in] stmt 対象のステートメント
  /// @return break 対象のスコープを返す．
  const VlScope*
  evaluate_seqblock(const VlStmt* stmt);

  /// @brief 名前付き begin-end ブロックの実行を行う．
  /// @param[in] stmt 対象のステートメント
  /// @return break 対象のスコープを返す．
  const VlScope*
  evaluate_namedseqblock(const VlStmt* stmt);

  /// @brief 代入文の実行を行う．
  /// @param[in] stmt 対象のステートメント
  /// @return break 対象のスコープを返す．
  const VlScope*
  evaluate_assign(const VlStmt* stmt);

  /// @brief while 文の実行を行う．
  /// @param[in] stmt 対象のステートメント
  /// @return break 対象のスコープを返す．
  const VlScope*
  evaluate_while(const VlStmt* stmt);

  /// @brief repeat 文の実行を行う．
  /// @param[in] stmt 対象のステートメント
  /// @return break 対象のスコープを返す．
  const VlScope*
  evaluate_repeat(const VlStmt* stmt);

  /// @brief for 文の実行を行う．
  /// @param[in] stmt 対象のステートメント
  /// @return break 対象のスコープを返す．
  const VlScope*
  evaluate_for(const VlStmt* stmt);

  /// @brief forever 文の実行を行う．
  /// @param[in] stmt 対象のステートメント
  /// @return break 対象のスコープを返す．
  const VlScope*
  evaluate_forever(const VlStmt* stmt);

  /// @brief if 文の実行を行う．
  /// @param[in] stmt 対象のステートメント
  /// @return break 対象のスコープを返す．
  const VlScope*
  evaluate_if(const VlStmt* stmt);

  /// @brief if-else 文の実行を行う．
  /// @param[in] stmt 対象のステートメント
  /// @return break 対象のスコープを返す．
  const VlScope*
  evaluate_ifelse(const VlStmt* stmt);

  /// @brief case 文の実行を行う．
  /// @param[in] stmt 対象のステートメント
  /// @return break 対象のスコープを返す．
  const VlScope*
  evaluate_case(const VlStmt* stmt);

  /// @brief disable 文の実行を行う．
  /// @param[in] stmt 対象のステートメント
  /// @return break 対象のスコープを返す．
  const VlScope*
  evaluate_disable(const VlStmt* stmt);

  /// @brief caseitem のラベルと一致するか調べる．
  /// @param[in] case_type case 文の種類
  /// @param[in] val case 文の値
  /// @param[in] caseitem 対象の caseitem
  bool
  match(VpiCaseType case_type,
	const VlValue& val,
	const VlCaseItem* caseitem);

  /// @brief 左辺の要素を値を代入する．
  /// @param[in] expr 左辺式
  /// @param[in] val 値
  ///
  /// expr は primary か bit-select, part-select
  void
  assign_value(const VlExpr* expr,
	       const VlValue& val);


private:
  //////////////////////////////////////////////////////////////////////
  // evaluate_expr() の下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 式の評価を行う．
  /// @param[in] expr 対象の式を表す構文木要素
  VlValue
  evaluate_expr(const VlExpr* expr);

  /// @brief 演算子に対して式の値を評価する．
  /// @param[in] expr 対象の式
  VlValue
  evaluate_opr(const VlExpr* expr);

  /// @brief 定数に対して式の値を評価する．
  /// @param[in] expr 対象の式
  VlValue
  evaluate_const(const VlExpr* expr);

  /// @brief 関数呼び出しに対して式の値を評価する．
  /// @param[in] expr 対象の式
  VlValue
  evaluate_funccall(const VlExpr* expr);

  /// @brief プライマリに対して式の値を評価する．
  /// @param[in] expr 対象の式
  VlValue
  evaluate_primary(const VlExpr* expr);

  /// @brief 式を評価して整数値を返す．
  /// @param[in] expr 対象の式
  int
  evaluate_int(const VlExpr* expr);

  /// @brief 式を評価して整数値を返す．
  /// @param[in] expr 対象の式
  bool
  evaluate_bool(const VlExpr* expr);


private:
  //////////////////////////////////////////////////////////////////////
  // 値の管理する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値を登録する．
  /// @param[in] obj 対象のオブジェクト
  /// @param[in] val 値
  ///
  /// 単独のオブジェクト用
  void
  reg_val(const VlDeclBase* obj,
	  const VlValue& val);

  /// @brief 値を登録する．
  /// @param[in] obj 対象のオブジェクト
  /// @param[in] offset オフセット
  /// @param[in] val 値
  ///
  /// 配列要素用
  void
  reg_val(const VlDeclBase* obj,
	  SizeType offset,
	  const VlValue& val);

  /// @brief 値を登録する．
  /// @param[in] obj 対象のオブジェクト
  /// @param[in] val 値
  /// @param[in] index ビット選択インデックス
  ///
  /// 単独のオブジェクト用
  void
  reg_val(const VlDeclBase* obj,
	  const VlValue& val,
	  int index);

  /// @brief 値を登録する．
  /// @param[in] obj 対象のオブジェクト
  /// @param[in] offset オフセット
  /// @param[in] val 値
  /// @param[in] index ビット選択インデックス
  ///
  /// 配列要素用
  void
  reg_val(const VlDeclBase* obj,
	  SizeType offset,
	  const VlValue& val,
	  int intdex);

  /// @brief 値を登録する．
  /// @param[in] obj 対象のオブジェクト
  /// @param[in] val 値
  /// @param[in] left, right 範囲選択のインデックス
  ///
  /// 単独のオブジェクト用
  void
  reg_val(const VlDeclBase* obj,
	  const VlValue& val,
	  int left,
	  int right);

  /// @brief 値を登録する．
  /// @param[in] obj 対象のオブジェクト
  /// @param[in] offset オフセット
  /// @param[in] val 値
  /// @param[in] left, right 範囲選択のインデックス
  ///
  /// 配列要素用
  void
  reg_val(const VlDeclBase* obj,
	  SizeType offset,
	  const VlValue& val,
	  int left,
	  int right);

  /// @brief 値を取り出す．
  /// @param[in] obj 対象のオブジェクト
  ///
  /// 単独のオブジェクト用
  VlValue
  get_val(const VlDeclBase* obj);

  /// @brief 値を取り出す．
  /// @param[in] obj 対象のオブジェクト
  /// @param[in] offset オフセット
  ///
  /// 配列要素用
  VlValue
  get_val(const VlDeclBase* obj,
	  SizeType offset);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ構造
  //////////////////////////////////////////////////////////////////////

  // キーを表すクラス
  struct Key
  {
    const VlDeclBase* mObj{nullptr};
    SizeType mOffset{0};
  };

  // ハッシュ関数を表すクラス
  struct Hash
  {
    SizeType
    operator()(const Key& key) const
    {
      auto tmp = reinterpret_cast<SizeType>(key.mObj);
      return ((tmp * tmp) >> 18) ^ key.mOffset;
    }
  };

  // 等価比較を行うクラス
  struct Eq
  {
    bool
    operator()(const Key& key1,
	       const Key& key2) const
    {
      return key1.mObj == key2.mObj && key1.mOffset == key2.mOffset;
    }
  };


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象の関数
  const VlTaskFunc* mFunction;

  // エラー出力フラグ
  bool mPutError;

  // 値割り当ての辞書
  unordered_map<Key, VlValue, Hash, Eq> mValMap;

};

END_NAMESPACE_YM_VERILOG

#endif // EVALUATOR_H
