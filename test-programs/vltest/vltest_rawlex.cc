﻿
/// @file vltest_rawlex.cc
/// @brief rawlex モードのテスト
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2009, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym/StopWatch.h"
#include "VlTestLineWatcher.h"

#include "ym/MsgMgr.h"
#include "ym/StreamMsgHandler.h"

#include "scanner/Lex.h"
#include "scanner/print_token.h"
#include "scanner/LexModuleState.h"
#include "scanner/common.h"


BEGIN_NAMESPACE_YM_VERILOG

void
rawlex_mode(const vector<string>& filename_list,
	    const char* spath,
	    int watch_line,
	    bool verbose,
	    bool dump_token)
{
  MsgHandler* handler = new StreamMsgHandler(&cerr);
  MsgMgr::attach_handler(handler);

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

  for ( auto filename: filename_list ) {
    if ( !lex.open_file(filename) ) {
      cerr << filename << " : cannot open" << endl;
      return;
    }

    for ( ; ; ) {
      int id = lex.get_raw_token();
      if ( id == EOF ) {
	break;
      }
      if ( dump_token ) {
	cout << lex.cur_token_loc() << ": ";
	print_token(cout, id, lex.cur_string());
	cout << endl;
      }
    }
  }

  timer.stop();
  if ( verbose ) {
    USTime time = timer.time();
    cout << "Time: " << time << endl;
  }
}

END_NAMESPACE_YM_VERILOG
