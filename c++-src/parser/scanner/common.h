#ifndef PARSER_COMMON_H
#define PARSER_COMMON_H

/// @file parser_common.h
/// @brief YACC の生成したヘッダファイルをインクルードする為のファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym/pt/PtP.h"

#include "parser/PtrList.h"


BEGIN_NAMESPACE_YM_VERILOG

class Parser;
class PuHierName;

#include "verilog_grammer.hh"

END_NAMESPACE_YM_VERILOG

#endif // PARSER_COMMON_H
