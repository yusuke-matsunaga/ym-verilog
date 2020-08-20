
/// @file EiFactory.cc
/// @brief EiFactory の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ei/EiFactory.h"


BEGIN_NAMESPACE_YM_VERILOG

// @brief オブジェクトを生成する関数
ElbFactory*
ElbFactory::new_obj()
{
  return new EiFactory();
}


//////////////////////////////////////////////////////////////////////
// クラス EiFactory
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiFactory::EiFactory()
{
}

// @brief デストラクタ
EiFactory::~EiFactory()
{
}

// @brief 内訳を表示する．
void
EiFactory::dump_prof(ostream& s)
{
}

END_NAMESPACE_YM_VERILOG
