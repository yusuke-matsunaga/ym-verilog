﻿
/// @file vltest_parse.cc
/// @brief parse モードのテスト
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2006, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym/Timer.h"
#include "ym/VlMgr.h"
#include "VlTestLineWatcher.h"
#include "parser/PtDumper.h"

#include "ym/MsgMgr.h"
#include "ym/StreamMsgHandler.h"


BEGIN_NAMESPACE_YM_VERILOG

void
parse_mode(const vector<string>& filename_list,
	   const char* spath,
	   int watch_line,
	   bool verbose,
	   bool profile,
	   int loop,
	   bool dump_pt)
{
  MsgHandler* tmh = new StreamMsgHandler(cerr);
  MsgMgr::attach_handler(tmh);

  SearchPathList splist;
  if ( spath ) {
    splist.set(spath);
  }

  VlTestLineWatcher watcher(watch_line);
  vector<VlLineWatcher*> watcher_list;
  if ( watch_line ) {
    watcher_list.push_back(&watcher);
  }

  int c = loop + 1;
  for ( int i = 0; i < c; ++ i ) {
#if 0
    try {
#endif
      Timer timer;
      timer.start();
      VlMgr vlmgr;
      for ( auto name: filename_list ) {
	if ( verbose ) {
	  cerr << "Reading " << name;
	  cerr.flush();
	}
	bool stat = vlmgr.read_file(name, splist, watcher_list);
	if ( verbose ) {
	  cerr << " end" << endl;
	}
      }
      timer.stop();
      auto time = timer.get_time();
      if ( verbose ) {
	cerr << "Parsing time: " << time << endl;
      }

      if ( dump_pt ) {
	const vector<const PtUdp*>& udp_list = vlmgr.pt_udp_list();
	const vector<const PtModule*>& module_list = vlmgr.pt_module_list();
	PtDumper dp(cout);
	dp.enable_file_loc_mode();
	dp.put(udp_list, module_list);
      }

      switch ( MsgMgr::error_num() ) {
      case 0:
	cerr << "No errors" << endl;
	break;
      case 1:
	cerr << "Total 1 error" << endl;
	break;

      default:
	cerr << "Total " << MsgMgr::error_num() << " errors" << endl;
	break;
      }
#if 0
    }
    catch ( AssertError x ) {
      cerr << x << endl;
    }
    catch ( std::bad_alloc x ) {
      cerr << "no more memory" << endl;
      exit(1);
    }
    catch (...) {
      cerr << "unkown exception" << endl;
    }
#endif
  }
}

END_NAMESPACE_YM_VERILOG
