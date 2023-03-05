
/// @file SptFactory.cc
/// @brief SptFactory の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "parser/SptFactory.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// Pt クラス(の継承クラス)を生成するファクトリクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
SptFactory::SptFactory(
  Alloc& alloc
) : PtiFactory{alloc}
{
}

// デストラクタ
SptFactory::~SptFactory()
{
}

// 確保したオブジェクトに関する情報を出力する．
void
SptFactory::dump_profile(
  ostream& s
) const
{
  // 未実装
}

END_NAMESPACE_YM_VERILOG
