﻿#ifndef YM_VLLINEWATCHER_H
#define YM_VLLINEWATCHER_H

/// @file ym/VlLineWatcher.h
/// @brief VlLineWatcher のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym/verilog.h"
#include "ym/Binder.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class VlLineWatcher VlLineWatcher.h <ym/VlLineWatcher.h>
/// @ingroup VlParser
/// @brief Verilog-HDL 用の Lex 中で行番号を監視するクラス
///
/// このクラスは純粋仮想クラスなので継承したクラスを作らなければ
/// 使えない．
///
/// 具体的には
/// void event_proc(int ) というメンバ関数を実装する必要がある．
///
/// 作成したオブジェクトは VlParser::reg_watcher(VlLineWatcher*)
/// でパーサーに登録する．
/// パーサーは改行文字を読み込む度に登録された VlLineWatcher に対して
/// event_proc(int) を呼び出す．この時の引数は現在の行番号
//////////////////////////////////////////////////////////////////////
class VlLineWatcher :
  public T1Binder<int>
{
};

END_NAMESPACE_YM_VERILOG

#endif // YM_VLLINEWATCHER_H
