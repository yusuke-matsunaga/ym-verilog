﻿
/// @file vltest_lex.cc
/// @brief lex モードのテスト
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2009, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym/StopWatch.h"
#include "VlTestLineWatcher.h"

#include "ym/MsgMgr.h"
#include "ym/StreamMsgHandler.h"

#include "Lex.h"
#include "print_token.h"
#include "LexModuleState.h"

#include "parser_common.h"


BEGIN_NAMESPACE_YM_VERILOG

void
lex_mode(const vector<string>& filename_list,
	 const char* spath,
	 int watch_line,
	 bool verbose,
	 int loop,
	 bool dump_token)
{
  MsgHandler* handler = new StreamMsgHandler(&cerr);
  MsgMgr::attach_handler(handler);

  int c = loop + 1;
  for ( int i = 0; i < c; ++ i ) {

    Lex lex;

    if ( spath ) {
      SearchPathList splist(spath);
      lex.set_searchpath(splist);
    }
    if ( watch_line ) {
      VlTestLineWatcher* mw = new VlTestLineWatcher(watch_line);
      lex.reg_watcher(mw);
    }

    StopWatch timer;
    timer.start();

    int nt_id = 0;
    int nt_sid = 0;
    int nt_un = 0;
    int nt_unb = 0;
    int nt_uni = 0;
    int nt_rn = 0;
    int nt_s = 0;
    int ns = 0;
    for ( auto filename: filename_list ) {
      if ( !lex.open_file(filename) ) {
	cerr << filename << " : cannot open" << endl;
	return;
      }

      for ( ; ; ) {
	int id = lex.get_token();
	if ( id == EOF ) {
	  break;
	}
	if ( dump_token ) {
	  cout << lex.cur_token_loc() << ": ";
	  print_token(cout, id, lex.cur_string());
	  cout << endl;
	}

	switch ( id ) {
	case IDENTIFIER:
	  ++ nt_id;
	  break;
	case SYS_IDENTIFIER:
	  ++ nt_sid;
	  break;
	case UNUMBER:
	  ++ nt_un;
	  break;
	case UNUM_BIG:
	  ++ nt_unb;
	case UNUM_INT:
	  ++ nt_uni;
	  break;
	case RNUMBER:
	  ++ nt_rn;
	  break;
	case STRING:
	  ++ nt_s;
	  break;
	default:
	  ++ ns;
	  break;
	}
      }
    }

    sleep(10);

    timer.stop();
    USTime time = timer.time();

    if ( verbose ) {
      cout << "Total " << setw(10) << nt_id << " IDENTIFIER tokens" << endl
	   << "      " << setw(10) << nt_sid << " SYS_IDENTIFIER tokens" << endl
	   << "      " << setw(10) << nt_un << " UNUMBER tokens" << endl
	   << "      " << setw(10) << nt_unb << " UNUM BIG" << endl
	   << "      " << setw(10) << nt_uni << " UNUM INT" << endl
	   << "      " << setw(10) << nt_rn << " RNUMBER tokens" << endl
	   << "      " << setw(10) << nt_s << " STRING tokens" << endl
	   << "      " << setw(10) << ns << " symbols" << endl
	   << "Time: " << time << endl;
    }
  }
}

END_NAMESPACE_YM_VERILOG
