﻿
/// @file vltest_main.cc
/// @brief Verilog-HDL フロントエンドのテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2006, 2014 Yusuke Matsunaga
/// All rights reserved.

#include "ym/verilog.h"
#include "ym/PoptMainApp.h"


BEGIN_NAMESPACE_YM_VERILOG

void
rawlex_mode(
  const vector<string>& filename_list,
  const char* spath,
  int watch_line,
  int loop,
  bool verbose,
  bool dump_token
);

void
lex_mode(
  const vector<string>& filename_list,
  const char* spath,
  int watch_line,
  int loop,
  bool verbose,
  bool dump_token
);

END_NAMESPACE_YM_VERILOG


int
main(
  int argc,
  const char** argv
)
{
  using namespace nsYm;
  using namespace nsYm::nsVerilog;

  int mode = 4;
  int verbose = 0;
  int dump = 0;
  int bit_expand = 0;
  int all_msg = 0;
  const char* spath = nullptr;
  int watch_line = 0;
  int loop = 0;
  int profile = 0;

  PoptMainApp popt;

  PoptNone popt_verbose("verbose", 'v', "enable verbose mode");
  PoptNone popt_rawlex("rawlex", '1', "enable rawlex mode");
  PoptNone popt_lex("lex", '2', "enable lex mode");
  PoptNone popt_dump("dump", 'd', "set dump-flag");
  PoptNone popt_allmsg("all-msg", 'a', "display all kind of messages");
  PoptStr popt_path("search-path", 'p', "set search path", "\"path list \"");
  PoptInt popt_loop("loop", 'l', "loop test", "loop count");
  PoptInt popt_watch("watch-line", 'w', "enable line watcher", "line number");
  PoptNone popt_prof("profile", 'q', "show memory profile");

  popt.add_option(&popt_verbose);
  popt.add_option(&popt_rawlex);
  popt.add_option(&popt_lex);
  popt.add_option(&popt_dump);
  popt.add_option(&popt_allmsg);
  popt.add_option(&popt_path);
  popt.add_option(&popt_loop);
  popt.add_option(&popt_watch);
  popt.add_option(&popt_prof);

  popt.set_other_option_help("[OPTIONS]* <file-name> ...");

  auto popt_stat = popt.parse_options(argc, argv, 0);
  if ( popt_stat == PoptStat::Abort ) {
    return -1;
  }

  // 残りの引数はすべてファイル名と見なす
  vector<string> filename_list;
  SizeType n_files = popt.get_args(filename_list);

  if ( n_files == 0 ) {
    popt.print_usage(stderr, 0);
    return 1;
  }

  if ( popt_verbose.is_specified() ) {
    verbose = true;
  }
  if ( popt_rawlex.is_specified() ) {
    mode = 1;
  }
  if ( popt_lex.is_specified() ) {
    mode = 2;
  }
  if ( popt_dump.is_specified() ) {
    dump = 1;
  }
  if ( popt_allmsg.is_specified() ) {
    all_msg = 1;
  }
  if ( popt_path.is_specified() ) {
    spath = popt_path.val().c_str();
  }
  if ( popt_loop.is_specified() ) {
    loop = popt_loop.val();
  }
  if ( popt_watch.is_specified() ) {
    watch_line = popt_watch.val();
  }
  if ( popt_prof.is_specified() ) {
    profile = 1;
  }

#if 0
#if HAVE_POPT
  // オプション解析用のデータ
  const struct poptOption options[] = {
    // long-option
    // short-option
    // argument type
    // variable address
    // option tag
    // docstr
    // argstr
    { "verbose", 'v', POPT_ARG_NONE, &verbose, 0,
      "enable verbose mode", nullptr },

    { "rawlex", '1', POPT_ARG_NONE, nullptr, 1,
      "enable rawlex mode", nullptr },

    { "lex", '2', POPT_ARG_NONE, nullptr, 2,
      "enable lex mode", nullptr },

    { "yacc", '3', POPT_ARG_NONE, nullptr, 3,
      "enable yacc mode", nullptr },

    { "elaborate", '4', POPT_ARG_NONE, nullptr, 4,
      "enable elaborate mode", nullptr },

    { "dump", 'd', POPT_ARG_NONE, &dump, 0,
      "set dump-flag", nullptr },

    { "all-msg", 'a', POPT_ARG_NONE, &all_msg, 0,
      "display all kind of messages", nullptr },

    { "search-path", 'p', POPT_ARG_STRING, &spath, 0,
      "set search path", "\"path list\"" },

    { "loop", 'l', POPT_ARG_INT, &loop, 0,
      "loop test", nullptr },

    { "watch-line", 'w', POPT_ARG_INT, &watch_line, 0,
      "enable line watcher", "line number" },

    { "profile", 'q', POPT_ARG_NONE, &profile, 0,
      "show memory profile", nullptr },

    { "liberty", 0, POPT_ARG_STRING, &liberty_name, 0,
      "specify liberty library", "\"file name\"" },

    { "mislib", 0, POPT_ARG_STRING, &mislib_name, 0,
      "specify mislib library", "\"file name\"" },

    POPT_AUTOHELP

    { nullptr, '\0', 0, nullptr, 0, nullptr, nullptr }
  };

  // オプション解析用のコンテキストを生成する．
  poptContext popt_context = poptGetContext(nullptr, argc, argv, options, 0);
  poptSetOtherOptionHelp(popt_context, "<file-name> ...");

  // オプション解析を行う．
  for ( ; ; ) {
    int rc = poptGetNextOpt(popt_context);
    if ( rc == -1 ) {
      break;
    }
    if ( rc < -1 ) {
      // エラーが起きた．
      fprintf(stderr, "%s: %s\n",
	      poptBadOption(popt_context, POPT_BADOPTION_NOALIAS),
	      poptStrerror(rc));
      return 1;
    }
    mode = rc;
  }

  // 残りの引数はすべてファイル名と見なす
  list<string> filename_list;
  for ( ; ; ) {
    const char* str = poptGetArg(popt_context);
    if ( str == nullptr ) break;
    filename_list.push_back(string(str));
  }

  if ( filename_list.empty() ) {
    poptPrintUsage(popt_context, stderr, 0);
    return 1;
  }
#else
  // オプション解析なし
  list<string> filename_list;
  for (int i = 1; i < argc; ++ i) {
    if ( argv[i][0] == '-' ) {
      if ( strcmp(argv[i], "-1") == 0 ) {
	mode = 1;
	continue;
      }
      if ( strcmp(argv[i], "-2") == 0 ) {
	mode = 2;
	continue;
      }
      if ( strcmp(argv[i], "-3") == 0 ) {
	mode = 3;
	continue;
      }
      if ( strcmp(argv[i], "-4") == 0 ) {
	mode = 4;
	continue;
      }
      if ( strcmp(argv[i], "-d") == 0 ) {
	dump_pt = true;
	continue;
      }
      if ( strcmp(argv[i], "-D") == 0 ) {
	dump_vpi = true;
      }
      if ( strcmp(argv[i], "-v") == 0 ) {
	verbose = true;
      }
    }
    else {
      filename_list.push_back(argv[i]);
    }
  }
#endif
#endif

  switch ( mode ) {
  case 1:
    rawlex_mode(filename_list,
		spath,
		watch_line,
		loop,
		verbose,
		dump);
    break;

  case 2:
    lex_mode(filename_list,
	     spath,
	     watch_line,
	     loop,
	     verbose,
	     dump);
    break;
  }

  return 0;
}
