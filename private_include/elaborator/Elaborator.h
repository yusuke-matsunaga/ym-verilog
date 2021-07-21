#ifndef ELABORATOR_H
#define ELABORATOR_H

/// @file Elaborator.h
/// @brief Elaborator のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2020, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym/verilog.h"
#include "ym/pt/PtP.h"
#include "ym/vl/VlFwd.h"
#include "ym/ClibCellLibrary.h"

#include "ObjDict.h"
#include "AttrDict.h"
#include "ElbStubList.h"

#include "ElbFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

class PtMgr;

class ElbMgr;

class UdpGen;
class ModuleGen;
class DeclGen;
class ItemGen;
class StmtGen;
class ExprGen;
class ExprEval;
class AttrGen;
class DefParamStub;

//////////////////////////////////////////////////////////////////////
/// @class Elaborator Elaborator.h "Elaborator.h"
/// @brief elaboration を行うクラス
/// 基本的には 内部に状態を持たないファンクタークラス
/// 実際には elaboration 途中でオブジェクトを保持しておくハッシュ表
/// などを持つ．最終結果は引数の ElbMgr に格納される．
//////////////////////////////////////////////////////////////////////
class Elaborator
{
  friend class ElbProxy;

public:

  /// @brief コンストラクタ
  Elaborator(
    ElbMgr& elb_mgr,                    ///< [in] Elbオブジェクトを管理するクラス
    const ClibCellLibrary& cell_library ///< [in] セルライブラリ
  );

  /// @brief デストラクタ
  ~Elaborator();


public:

  /// @brief エラボレーションを行う．
  /// @return エラー数を返す．
  ///
  /// この関数は一度しか呼べない．
  int
  operator()(
    const PtMgr& pt_mgr ///< [in] パース木を管理するクラス
  );


private:
  //////////////////////////////////////////////////////////////////////
  // elaboration で用いられる下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 後で処理する defparam 文を登録する．
  void
  add_defparamstub(
    const VlModule* module, ///< [in] アイテムテンプレートのヘッダ (defparam を持つ)
    const PtItem* header    ///< [in] defparam 文のテンプレート
  );

  /// @brief phase1 で行う処理を登録する．
  void
  add_phase1stub(
    ElbStub* stub ///< [in] phase1 で行う処理を表すスタブ
  );

  /// @brief phase2 で行う処理を登録する．
  void
  add_phase2stub(
    ElbStub* stub ///< [in] phase2 で行う処理を表すスタブ
  );

  /// @brief phase3 で行う処理を登録する．
  void
  add_phase3stub(
    ElbStub* stub ///< [in] phase3 で行う処理を表すスタブ
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 要素を検索する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前からモジュール定義を取り出す．
  /// @return name という名のモジュール定義
  /// @return なければ nullptr を返す．
  const PtModule*
  find_moduledef(
    const string& name ///< [in] 名前
  ) const;

  /// @brief 関数定義を探す．
  const PtItem*
  find_funcdef(
    const VlModule* module, ///< [in] 親のモジュール
    const string& name      ///< [in] 関数名
  ) const;

  /// @brief constant function を取り出す．
  /// @return parent というスコープ内の name という関数を返す．
  /// @return なければ nullptr を返す．
  const VlTaskFunc*
  find_constant_function(
    const VlScope* parent, ///< [in] 検索対象のスコープ
    const string& name     ///< [in] 名前
  ) const;

  /// @brief セルの探索
  /// @return name という名のセル番号を返す．
  ///
  /// なければ -1 を返す．
  int
  find_cell_id(
    const string& name ///< [in] セル名
  ) const;


public:
  //////////////////////////////////////////////////////////////////////
  // 要素を生成・登録する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief トップレベルのスコープを生成する
  const VlScope*
  new_Toplevel();


private:
  //////////////////////////////////////////////////////////////////////
  // 要素を登録する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief constant function を登録する．
  void
  reg_constant_function(
    const VlTaskFunc* func ///< [in] 関数
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // エラボレーションを行ったことを示すフラグ
  bool mDone;

  // 生成したオブジェクトを管理するクラス
  ElbMgr& mMgr;

  // セルライブラリ
  ClibCellLibrary mCellLibrary;

  // UDP 生成用のオブジェクト
  unique_ptr<UdpGen> mUdpGen;

  // モジュール生成用のオブジェクト
  unique_ptr<ModuleGen> mModuleGen;

  // 宣言要素生成用のオブジェクト
  unique_ptr<DeclGen> mDeclGen;

  // 構成要素生成用のオブジェクト
  unique_ptr<ItemGen> mItemGen;

  // ステートメント生成用のオブジェクト
  unique_ptr<StmtGen> mStmtGen;

  // 式生成用のオブジェクト
  unique_ptr<ExprGen> mExprGen;

  // 定数式評価用のオブジェクト
  unique_ptr<ExprEval> mExprEval;

  // attribute instance 生成用のオブジェクト
  unique_ptr<AttrGen> mAttrGen;

  // 関数定義の辞書
  unordered_map<string, const PtItem*> mFuncDict;

  // constant function の辞書
  ObjDict mCfDict;

  // モジュールテンプレートの辞書
  unordered_map<string, const PtModule*> mModuleDict;

  // attribute instance の辞書
  AttrDict mAttrDict;

  // defparam 文の元のリスト
  list<DefParamStub> mDefParamStubList;

  // phase1 で生成するオブジェクトを追加するリスト
  ElbStubList mPhase1StubList1;

  // phase1 で生成するオブジェクトを読み出すリスト
  ElbStubList mPhase1StubList2;

  // phase2 生成するオブジェクトを入れたリスト
  ElbStubList mPhase2StubList;

  // phase3 で link するオブジェクトを入れたリスト
  ElbStubList mPhase3StubList;


protected:
  //////////////////////////////////////////////////////////////////////
  // コンパイルオプション
  //////////////////////////////////////////////////////////////////////

  // IOに範囲がなく宣言のみに範囲を持つ場合を許すとき true
  bool mAllowEmptyIORange;

};

END_NAMESPACE_YM_VERILOG

#endif // ELABORATOR_H
