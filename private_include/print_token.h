﻿#ifndef PRINT_TOKEN_H
#define PRINT_TOKEN_H

/// @file print_token.h
/// @brief print_token() のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym/ym_verilog.h"


BEGIN_NAMESPACE_YM_VERILOG

/// @brief トークンの内容を出力する．
/// @param[in] s 出力先のストリーム
/// @param[in] id トークン番号
/// @param[in] str トークン文字列
void
print_token(ostream& s,
	    int id,
	    const char* str);

END_NAMESPACE_YM_VERILOG

#endif // PRINT_TOKEN_H
