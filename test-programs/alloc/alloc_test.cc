
/// @file alloc_test.cc
/// @brief alloc_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2020 Yusuke Matsunaga
/// All rights reserved.


#include "parser/SimpleAlloc.h"

int
main(int argc,
     char** argv)
{
  using namespace nsYm;

  for ( int i = 0; i < 10000; ++ i ) {
    SimpleAlloc alloc(4096);
    vector<char*> p_list;
    int a = 0;
    for ( int j = 0; j < 10000; ++ j ) {
      void* p{alloc.get_memory(1000)};
      a += j * j;
      char* q{new char[4096]};
      p_list.push_back(q);
    }
    for ( auto p: p_list ) {
      delete [] p;
    }
  }

  return 0;
}
