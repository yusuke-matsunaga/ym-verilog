
/// @file ElbPrimitive.cc
/// @brief ElbPrimitive の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "elaborator/ElbPrimitive.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス ElbPrimHead
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// クラス ElbPrimArray
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// クラス ElbPrimitive
//////////////////////////////////////////////////////////////////////

// @brief 入出力ポート数を計算する．
// @param[in] type プリミティブの型
// @param[in] port_size 全ポート数
// @param[out] output_num 出力ポート数
// @param[out] inout_num 入出力ポート数
// @param[out] input_num 入力ポート数
// @retval 1 port_size が多すぎる．
// @retval 0 port_size が範囲内に収まっている．
// @retval -1 port_size が少なすぎる．
int
ElbPrimitive::get_port_size(VpiPrimType type,
			    SizeType port_size,
			    SizeType& output_num,
			    SizeType& inout_num,
			    SizeType& input_num)
{
  SizeType min_size = 0;
  SizeType max_size = port_size;

  switch ( type ) {
  case VpiPrimType::And:
  case VpiPrimType::Nand:
  case VpiPrimType::Nor:
  case VpiPrimType::Or:
  case VpiPrimType::Xor:
  case VpiPrimType::Xnor:
    min_size = 3;
    output_num = 1;
    inout_num = 0;
    input_num = port_size - 1;
    break;

  case VpiPrimType::Buf:
  case VpiPrimType::Not:
    min_size = 2;
    output_num = port_size - 1;
    inout_num = 0;
    input_num = 1;
    break;

  case VpiPrimType::Bufif0:
  case VpiPrimType::Bufif1:
  case VpiPrimType::Notif0:
  case VpiPrimType::Notif1:
    min_size = 3;
    max_size = 3;
    output_num = 1;
    inout_num = 0;
    input_num = 2;
    break;

  case VpiPrimType::Nmos:
  case VpiPrimType::Pmos:
  case VpiPrimType::Rnmos:
  case VpiPrimType::Rpmos:
    min_size = 3;
    max_size = 3;
    output_num = 1;
    inout_num = 0;
    input_num = 2;
    break;

  case VpiPrimType::Cmos:
  case VpiPrimType::Rcmos:
    min_size = 4;
    max_size = 4;
    output_num = 1;
    inout_num = 0;
    input_num = 3;
    break;

  case VpiPrimType::Tran:
  case VpiPrimType::Rtran:
    min_size = 2;
    max_size = 2;
    output_num = 0;
    inout_num = 2;
    input_num = 0;
    break;

  case VpiPrimType::Rtranif0:
  case VpiPrimType::Rtranif1:
  case VpiPrimType::Tranif0:
  case VpiPrimType::Tranif1:
    min_size = 3;
    max_size = 3;
    output_num = 0;
    inout_num = 2;
    input_num = 1;
    break;

  case VpiPrimType::Pullup:
  case VpiPrimType::Pulldown:
    min_size = 1;
    max_size = 1;
    output_num = 1;
    inout_num = 0;
    input_num = 0;
    break;

  default:
    // UDP
    min_size = 1;
    output_num = 1;
    inout_num = 0;
    input_num = port_size - 1;
    break;
  }
  if ( port_size < min_size ) {
    return -1;
  }
  if ( port_size > max_size ) {
    return 1;
  }
  return 0;
}

END_NAMESPACE_YM_VERILOG
