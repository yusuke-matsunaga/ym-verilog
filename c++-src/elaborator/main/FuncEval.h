#ifndef FUNCEVAL_H
#define FUNCEVAL_H

/// @file FuncEval.h
/// @brief FuncEval のヘッダファイル
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
/// @class FuncEval FuncEval.h "FuncEval.h"
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
class FuncEval
{
public:

  /// @brief コンストラクタ
  FuncEval(
    const VlTaskFunc* function ///< [in] 関数
  );

  /// @brief デストラクタ
  ~FuncEval();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 関数を評価する．
  VlValue
  operator()(
    const vector<VlValue>& arg_list ///< [in] 引数のリスト
  );


private:
  //////////////////////////////////////////////////////////////////////
  // evaluate_stmt() で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ステートメントの実行を行う．
  /// @return disable が実行された場合はスコープを返す．
  const VlScope*
  evaluate_stmt(
    const VlStmt* stmt ///< [in] 対象のステートメント
  );

  /// @brief begin-end ブロックの実行を行う．
  /// @return break 対象のスコープを返す．
  const VlScope*
  evaluate_seqblock(
    const VlStmt* stmt ///< [in] 対象のステートメント
  );

  /// @brief 名前付き begin-end ブロックの実行を行う．
  /// @return break 対象のスコープを返す．
  const VlScope*
  evaluate_namedseqblock(
    const VlStmt* stmt ///< [in] 対象のステートメント
  );

  /// @brief 代入文の実行を行う．
  /// @return break 対象のスコープを返す．
  const VlScope*
  evaluate_assign(
    const VlStmt* stmt ///< [in] 対象のステートメント
  );

  /// @brief while 文の実行を行う．
  /// @return break 対象のスコープを返す．
  const VlScope*
  evaluate_while(
    const VlStmt* stmt ///< [in] 対象のステートメント
  );

  /// @brief repeat 文の実行を行う．
  /// @return break 対象のスコープを返す．
  const VlScope*
  evaluate_repeat(
    const VlStmt* stmt ///< [in] 対象のステートメント
  );

  /// @brief for 文の実行を行う．
  /// @return break 対象のスコープを返す．
  const VlScope*
  evaluate_for(
    const VlStmt* stmt ///< [in] 対象のステートメント
  );

  /// @brief forever 文の実行を行う．
  /// @return break 対象のスコープを返す．
  const VlScope*
  evaluate_forever(
    const VlStmt* stmt ///< [in] 対象のステートメント
  );

  /// @brief if 文の実行を行う．
  /// @return break 対象のスコープを返す．
  const VlScope*
  evaluate_if(
    const VlStmt* stmt ///< [in] 対象のステートメント
  );

  /// @brief if-else 文の実行を行う．
  /// @return break 対象のスコープを返す．
  const VlScope*
  evaluate_ifelse(
    const VlStmt* stmt ///< [in] 対象のステートメント
  );

  /// @brief case 文の実行を行う．
  /// @return break 対象のスコープを返す．
  const VlScope*
  evaluate_case(
    const VlStmt* stmt ///< [in] 対象のステートメント
  );

  /// @brief disable 文の実行を行う．
  /// @return break 対象のスコープを返す．
  const VlScope*
  evaluate_disable(
    const VlStmt* stmt ///< [in] 対象のステートメント
  );

  /// @brief caseitem のラベルと一致するか調べる．
  bool
  match(
    VpiCaseType case_type,     ///< [in] case 文の種類
    const VlValue& val,        ///< [in] case 文の値
    const VlCaseItem* caseitem ///< [in] 対象の caseitem
  );

  /// @brief 左辺の要素を値を代入する．
  ///
  /// expr は primary か bit-select, part-select
  void
  assign_value(
    const VlExpr* expr, ///< [in] 左辺式
    const VlValue& val  ///< [in] 値
  );


private:
  //////////////////////////////////////////////////////////////////////
  // evaluate_expr() の下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 式の評価を行う．
  VlValue
  evaluate_expr(
    const VlExpr* expr ///< [in] 対象の式
  );

  /// @brief 演算子に対して式の値を評価する．
  VlValue
  evaluate_opr(
    const VlExpr* expr ///< [in] 対象の式
  );

  /// @brief 定数に対して式の値を評価する．
  VlValue
  evaluate_const(
    const VlExpr* expr ///< [in] 対象の式
  );

  /// @brief 関数呼び出しに対して式の値を評価する．
  VlValue
  evaluate_funccall(
    const VlExpr* expr ///< [in] 対象の式
  );

  /// @brief プライマリに対して式の値を評価する．
  VlValue
  evaluate_primary(
    const VlExpr* expr ///< [in] 対象の式
  );

  /// @brief 式を評価して整数値を返す．
  int
  evaluate_int(
    const VlExpr* expr ///< [in] 対象の式
  );

  /// @brief 式を評価して整数値を返す．
  bool
  evaluate_bool(
    const VlExpr* expr ///< [in] 対象の式
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 値を管理する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値を登録する．
  ///
  /// 単独のオブジェクト用
  void
  reg_val(
    const VlDeclBase* obj, ///< [in] 対象のオブジェクト
    const VlValue& val     ///< [in] 値
  );

  /// @brief 値を登録する．
  ///
  /// 配列要素用
  void
  reg_val(
    const VlDeclBase* obj, ///< [in] 対象のオブジェクト
    SizeType offset,       ///< [in] オフセット
    const VlValue& val     ///< [in] 値
  );

  /// @brief 値を登録する．
  ///
  /// 単独のオブジェクト用
  void
  reg_val(
    const VlDeclBase* obj, ///< [in] 対象のオブジェクト
    const VlValue& val,    ///< [in] 値
    int index              ///< [in] ビット選択インデックス
  );

  /// @brief 値を登録する．
  ///
  /// 配列要素用
  void
  reg_val(
    const VlDeclBase* obj, ///< [in] 対象のオブジェクト
    SizeType offset,	   ///< [in] オフセット
    const VlValue& val,	   ///< [in] 値
    int intdex             ///< [in] ビット選択インデックス
  );

  /// @brief 値を登録する．
  ///
  /// 単独のオブジェクト用
  void
  reg_val(
    const VlDeclBase* obj, ///< [in] 対象のオブジェクト
    const VlValue& val,	   ///< [in] 値
    int left,              ///< [in] 範囲選択の左のインデックス
    int right              ///< [in] 範囲選択の右のインデックス
  );

  /// @brief 値を登録する．
  ///
  /// 配列要素用
  void
  reg_val(
    const VlDeclBase* obj, ///< [in] 対象のオブジェクト
    SizeType offset,	   ///< [in] オフセット
    const VlValue& val,	   ///< [in] 値
    int left,              ///< [in] 範囲選択の左のインデックス
    int right		   ///< [in] 範囲選択の右のインデックス
  );

  /// @brief 値を取り出す．
  ///
  /// 単独のオブジェクト用
  VlValue
  get_val(
    const VlDeclBase* obj ///< [in] 対象のオブジェクト
  );

  /// @brief 値を取り出す．
  ///
  /// 配列要素用
  VlValue
  get_val(
    const VlDeclBase* obj, ///< [in] 対象のオブジェクト
    SizeType offset        ///< [in] オフセット
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ構造
  //////////////////////////////////////////////////////////////////////

  // キーを表すクラス
  // 単体の場合と配列+オフセットの2通りがある．
  struct Key
  {
    const VlDeclBase* mObj{nullptr};
    SizeType mOffset{0};
  };

  // ハッシュ関数を表すクラス
  struct Hash
  {
    SizeType
    operator()(
      const Key& key
    ) const
    {
      auto tmp = reinterpret_cast<SizeType>(key.mObj);
      return ((tmp * tmp) >> 18) ^ key.mOffset;
    }
  };

  // 等価比較を行うクラス
  struct Eq
  {
    bool
    operator()(
      const Key& key1,
      const Key& key2
    ) const
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

  // 値割り当ての辞書
  unordered_map<Key, VlValue, Hash, Eq> mValMap;

};

END_NAMESPACE_YM_VERILOG

#endif // FUNCEVAL_H
