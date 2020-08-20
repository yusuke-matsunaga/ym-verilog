﻿
/// @file vltest_elaborate.cc
/// @brief elabrate モードのテスト
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2006, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym/StopWatch.h"
#include "VlTestLineWatcher.h"
#include "ym/VlMgr.h"
#include "VlDumper.h"

#include "ym/clib.h"

#include "ym/MsgMgr.h"
#include "ym/StreamMsgHandler.h"


BEGIN_NAMESPACE_YM_VERILOG

void
elaborate_mode(const vector<string>& filename_list,
	       bool all_msg,
	       const char* spath,
	       const ClibCellLibrary& cell_library,
	       int watch_line,
	       bool verbose,
	       bool profile,
	       int loop,
	       bool dump_vpi)
{
  MsgHandler* tmh = new StreamMsgHandler(&cerr);
  if ( all_msg ) {
    tmh->set_mask(kMsgMaskAll);
  }
  else {
    tmh->set_mask(kMsgMaskAll);
    tmh->delete_mask(MsgType::Info);
    tmh->delete_mask(MsgType::Debug);
  }
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
#if !defined(YM_DEBUG)
    try {
#endif
      StopWatch timer;
      timer.start();
      VlMgr vlmgr;
      for ( auto name: filename_list ) {
	if ( verbose ) {
	  cerr << "Reading " << name;
	  cerr.flush();
	}
	vlmgr.read_file(name, splist, watcher_list);
	if ( verbose ) {
	  cerr << " end" << endl;
	}
      }

      timer.stop();
      USTime time = timer.time();
      if ( verbose ) {
	cerr << "Parsing time: " << time << endl;
      }

      if ( !MsgMgr::error_num() ) {
	StopWatch timer;
	timer.start();

	vlmgr.elaborate(cell_library);

	timer.stop();
	USTime time = timer.time();
	if ( verbose ) {
	  cerr << "Elaborating time: " << time << endl;
	}

	if ( MsgMgr::error_num() == 0 && dump_vpi ) {
	  VlDumper dumper(cout);
	  dumper(vlmgr);
	}
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

#if !defined(YM_DEBUG)
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
