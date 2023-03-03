#ifndef ELBENV_H
#define ELBENV_H

/// @file ElbEnv.h
/// @brief ElbEnv のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.

#include "ym/verilog.h"
#include "ym/vl/VlFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ElbEnv ElbEnv.h "ElbEnv.h"
/// @brief Elaboration 時の環境を表すクラス
///
/// 具体的には
/// - 定数式
/// - 関数内の式
/// - 定数関数内の式
/// - システム関数/システムタスクの引数
/// - イベント式
/// - net 型の左辺式
/// - reg/variables 型の左辺式
/// - procedural continuous assignment 文の左辺式
/// - force 文の左辺式
/// の種類がある．
/// 実際にはそれぞれの環境に対応した派生クラスを用いる．
//////////////////////////////////////////////////////////////////////
class ElbEnv
{
public:

  /// @brief コンストラクタ
  ElbEnv();

  /// @brief コピーコンストラクタ
  ElbEnv(
    const ElbEnv& src
  );

  /// @brief 代入演算子
  const ElbEnv&
  operator=(
    const ElbEnv& src
  );

  /// @brief デストラクタ
  ~ElbEnv() = default;


protected:
  //////////////////////////////////////////////////////////////////////
  // ElbEnv の状態を設定する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 定数式を要求する．
  void
  set_constant();

  /// @brief 親の function を設定する．
  void
  set_constant_function(
    const VlScope* function ///< [in] 設定する function
  );

  /// @brief 親の function を設定する．
  void
  set_function();

  /// @brief 親の task を設定する．
  void
  set_task();

  /// @brief system task/system function の引数の印をつける．
  void
  set_system_tf_arg();

  /// @brief イベント式の印をつける．
  void
  set_event_expr();

  /// @brief net 型の左辺式の印をつける．
  void
  set_net_lhs();

  /// @brief reg/var 型の左辺式の印をつける．
  void
  set_var_lhs();

  /// @brief pca 代入文の左辺式の印をつける．
  void
  set_pca_lhs();

  /// @brief force 代入文の左辺式の印をつける．
  void
  set_force_lhs();


public:
  //////////////////////////////////////////////////////////////////////
  // ElbEnv の状態を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 定数式が要求されている時に true を返す．
  bool
  is_constant() const;

  /// @brief constant function 内の生成の時に true を返す．
  bool
  inside_constant_function() const;

  /// @brief constant function 内の生成の時に親の function を返す．
  const VlScope*
  constant_function() const;

  /// @brief function 内の生成時に true を返す．
  bool
  inside_function() const;

  /// @brief system task/system function の引数の時に true を返す．
  bool
  is_system_tf_arg() const;

  /// @brief イベント式の時に true を返す．
  bool
  is_event_expr() const;

  /// @brief 左辺式の時に true を返す．
  bool
  is_lhs() const;

  /// @brief net 型の左辺式の時に true を返す．
  bool
  is_net_lhs() const;

  /// @brief reg/var 型の左辺式の時に true を返す．
  bool
  is_var_lhs() const;

  /// @brief pca 代入文の左辺式の時に true を返す．
  bool
  is_pca_lhs() const;

  /// @brief force 代入文の左辺式の時に true を返す．
  bool
  is_force_lhs() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる定数
  //////////////////////////////////////////////////////////////////////

  // 定数式の環境
  static const int CONSTANT = 0;

  // 関数内の環境
  static const int FUNCTION = 1;

  // 定数関数内の環境
  static const int CONSTFUNC = 2;

  // タスク内の環境
  static const int TASK = 3;

  // 引数の環境
  static const int ARG = 4;

  // イベント式の環境
  static const int EVENT = 5;

  // 左辺式の環境
  static const int LHS = 6;

  // ネットの環境
  static const int NET = 7;

  // 変数の環境
  static const int VAR = 8;

  // PCA の環境
  static const int PCA = 9;

  // FORCE の環境
  static const int FORCE = 10;

  // フラグ数
  static const int NUM_FLAG = FORCE + 1;

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 種々のフラグ
  bitset<NUM_FLAG> mFlags;

  // constant function
  const VlScope* mCf;

};


//////////////////////////////////////////////////////////////////////
/// @class ElbConstantEnv ElbEnv.h "ElbEnv.h"
/// @brief 定数式を表す環境
//////////////////////////////////////////////////////////////////////
class ElbConstantEnv :
  public ElbEnv
{
public:

  /// @brief コンストラクタ
  ElbConstantEnv();

};


//////////////////////////////////////////////////////////////////////
/// @class ElbConstantFunctionEnv ElbEnv.h "ElbEnv.h"
/// @brief 定数関数内を表す環境
//////////////////////////////////////////////////////////////////////
class ElbConstantFunctionEnv :
  public ElbEnv
{
public:

  /// @brief コンストラクタ
  ElbConstantFunctionEnv(
    const VlScope* func ///< [in] 親の関数
  );

};


//////////////////////////////////////////////////////////////////////
/// @class ElbTfEnv ElbEnv.h "ElbEnv.h"
/// @brief 関数/タスク内を表す環境
//////////////////////////////////////////////////////////////////////
class ElbTfEnv :
  public ElbEnv
{
public:

  /// @brief コンストラクタ
  ElbTfEnv(
    const VlTaskFunc* taskfunc ///< [in] タスク/関数
  );

};


//////////////////////////////////////////////////////////////////////
/// @class ElbSystemTfArgEnv ElbEnv.h "ElbEnv.h"
/// @brief システムタスク/関数の引数を表す環境
//////////////////////////////////////////////////////////////////////
class ElbSystemTfArgEnv :
  public ElbEnv
{
public:

  /// @brief コンストラクタ
  ElbSystemTfArgEnv(
    const ElbEnv& env ///< [in] 親の環境
  );

};


//////////////////////////////////////////////////////////////////////
/// @class ElbEventExprEnv ElbEnv.h "ElbEnv.h"
/// @brief イベント式を表す環境
//////////////////////////////////////////////////////////////////////
class ElbEventExprEnv :
  public ElbEnv
{
public:

  /// @brief コンストラクタ
  ElbEventExprEnv(
    const ElbEnv& env ///< [in] 親の環境
  );

};


//////////////////////////////////////////////////////////////////////
/// @class ElbNetLhsEnv ElbEnv.h "ElbEnv.h"
/// @brief net 型の左辺式を表す環境
//////////////////////////////////////////////////////////////////////
class ElbNetLhsEnv :
  public ElbEnv
{
public:

  /// @brief コンストラクタ
  ElbNetLhsEnv(
    const ElbEnv& env ///< [in] 親の環境
  );

};


//////////////////////////////////////////////////////////////////////
/// @class ElbVarLhsEnv ElbEnv.h "ElbEnv.h"
/// @brief reg/var 型の左辺式を表す環境
//////////////////////////////////////////////////////////////////////
class ElbVarLhsEnv :
  public ElbEnv
{
public:

  /// @brief コンストラクタ
  ElbVarLhsEnv(
    const ElbEnv& env ///< [in] 親の環境
  );

};


//////////////////////////////////////////////////////////////////////
/// @class ElbPcaLhsEnv ElbEnv.h "ElbEnv.h"
/// @brief pca 代入文の左辺式を表す環境
//////////////////////////////////////////////////////////////////////
class ElbPcaLhsEnv :
  public ElbEnv
{
public:

  /// @brief コンストラクタ
  ElbPcaLhsEnv(
    const ElbEnv& env ///< [in] 親の環境
  );

};


//////////////////////////////////////////////////////////////////////
/// @class ElbForceLhsEnv ElbEnv.h "ElbEnv.h"
/// @brief force 代入文の左辺式を表す環境
//////////////////////////////////////////////////////////////////////
class ElbForceLhsEnv :
  public ElbEnv
{
public:

  /// @brief コンストラクタ
  ElbForceLhsEnv(
    const ElbEnv& env ///< [in] 親の環境
  );

};

END_NAMESPACE_YM_VERILOG

#endif // ELBENV_H
