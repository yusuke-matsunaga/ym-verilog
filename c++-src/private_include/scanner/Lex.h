#ifndef LEX_H
#define LEX_H

/// @file Lex.h
/// @brief Lex のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym/verilog.h"
#include "RawLex.h"


BEGIN_NAMESPACE_YM_VERILOG

class LexModuleState;

//////////////////////////////////////////////////////////////////////
/// @class Lex Lex.h "Lex.h"
/// @ingroup VlParser
/// @brief Verilog 用の字句解析器のインターフェイス
//////////////////////////////////////////////////////////////////////
class Lex :
  public RawLex
{
public:

  /// @brief コンストラクタ
  Lex();

  /// @brief デストラクタ
  ~Lex();


public:
  //////////////////////////////////////////////////////////////////////
  // 状態取得用のメンバ関数
  //////////////////////////////////////////////////////////////////////

  /// @brief cell_define の状態を取得する
  /// @retval true cell_define されている
  /// @retval false cell_define されていない
  bool
  cell_define() const;

  /// @brief net_type を取得する．
  VpiNetType
  default_nettype() const;

  /// @brief time_unit の取得
  int
  time_unit() const;

  /// @brief time_precision の取得
  int
  time_precision() const;

  /// @brief unconnected_drive の値を取得する．
  VpiUnconnDrive
  unconnected_drive() const;

  /// @brief decay_time の値を取得する．
  int
  default_decay_time() const;

  /// @brief trireg_strength の値を取得する．
  int
  default_trireg_strength() const;

  /// @brief delay_mode の値を取得する．
  VpiDefDelayMode
  delay_mode() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // モジュール関係のLEX状態
  LexModuleState* mModuleState;

};

END_NAMESPACE_YM_VERILOG

#endif // LEX_H
