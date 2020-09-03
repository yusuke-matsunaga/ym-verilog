
/// @file Parser.cc
/// @brief Parser の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.


#include "parser/Parser.h"
#include "scanner/Lex.h"
#include "parser/PtiFactory.h"
#include "parser/PtiArray.h"
#include "parser/PtMgr.h"
#include "parser/PuHierName.h"

#include "ym/pt/PtModule.h"
#include "ym/pt/PtUdp.h"
#include "ym/pt/PtItem.h"
#include "ym/pt/PtStmt.h"
#include "ym/MsgMgr.h"


const int debug = 0;
#define dout cout


BEGIN_NAMESPACE_YM_VERILOG

#include "verilog_grammer.hh"

// メモリリークのチェックを行うとき 1 にする．
const int check_memory_leak = 0;

//////////////////////////////////////////////////////////////////////
// Verilog-HDL のパーサークラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] ptmgr 読んだ結果のパース木を登録するマネージャ
Parser::Parser(PtMgr& ptmgr) :
  mAlloc{ptmgr.alloc()},
  mPtMgr{ptmgr},
  mFactory{PtiFactory::make_obj("cpt", mAlloc)},
  mLex{new Lex}
{
}

// @brief デストラクタ
Parser::~Parser()
{
  // mLex は unique_ptr のデストラクタで破壊される．
}

// @brief ファイルを読み込む．
// @param[in] filename 読み込むファイル名
// @param[in] searchpath サーチパス
// @param[in] watcher_list 行番号ウオッチャーのリスト
// @retval true 正常に終了した．
// @retval false エラーが起こった．
bool
Parser::read_file(const string& filename,
		  const SearchPathList& searchpath,
		  const vector<VlLineWatcher*>& watcher_list)
{
  // YACC の生成するパーズ関数
  extern int yyparse(Parser&);

  lex().set_searchpath(searchpath);

  for ( auto watcher: watcher_list ) {
    lex().reg_watcher(watcher);
  }

  if ( !lex().open_file(filename) ) {
    ostringstream buf;
    buf << filename << " : No such file.";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    FileRegion(),
		    MsgType::Failure,
		    "VLPARSER",
		    buf.str());
    return false;
  }

  int stat = yyparse(*this);

  return (stat == 0);
}

// @brief yylex とのインターフェイス
// @param[out] lvalp 値を格納する変数
// @param[out] llocp 位置情報を格納する変数
// @return 読み込んだトークンの id を返す．
int
Parser::yylex(YYSTYPE& lval,
	      FileRegion& lloc)
{
  int id = lex().get_token();

  switch ( id ) {
  case IDENTIFIER:
  case SYS_IDENTIFIER:
  case STRING:
  case UNUMBER:
  case UNUM_BIG:
    // 文字列領域は PtMgr が管理する．
    lval.strtype = mPtMgr.save_string(lex().cur_string());
    break;

  case UNUM_INT:
    lval.uinttype = lex().cur_uint();
    break;

  case RNUMBER:
    lval.dbltype = lex().cur_rnumber();
    break;

  default:
    break;
  }
  lloc = lex().cur_token_loc();
  return id;
}

// @brief 使用されているモジュール名を登録する．
// @param[in] name 登録する名前
void
Parser::reg_defname(const char* name)
{
  mPtMgr.reg_defname(name);
}

// @brief attribute instance を登録する．
void
Parser::reg_attrinst(const PtBase* ptobj,
		     PtrList<const PtAttrInst>* attr_list,
		     bool def)
{
  mPtMgr.reg_attrinst(ptobj, attr_list, def);
}

// 関数内で使えるステートメントかどうかのチェック
bool
Parser::check_function_statement(const PtStmt* stmt)
{
  switch ( stmt->type() ) {
  case PtStmtType::Disable:
  case PtStmtType::SysEnable:
  case PtStmtType::Null:
    return true;

  case PtStmtType::Assign:
    if ( stmt->control() == nullptr) return true;
    break;

  case PtStmtType::If:
    if ( stmt->body() ) {
      if ( !check_function_statement(stmt->body()) ) {
	return false;
      }
    }
    if ( stmt->else_body() ) {
      if ( !check_function_statement(stmt->else_body()) ) {
	return false;
      }
    }
    return true;

  case PtStmtType::Case:
  case PtStmtType::CaseX:
  case PtStmtType::CaseZ:
    for ( auto item: stmt->caseitem_list() ) {
      if ( !check_function_statement(item->body()) ) {
	return false;
      }
    }
    return true;

  case PtStmtType::Forever:
  case PtStmtType::Repeat:
  case PtStmtType::White:
  case PtStmtType::For:
    if ( stmt->init_stmt() ) {
      if ( !check_function_statement(stmt->init_stmt()) ) {
	return false;
      }
    }
    if ( stmt->next_stmt() ) {
      if ( !check_function_statement(stmt->next_stmt()) ) {
	return false;
      }
    }
    return check_function_statement(stmt->body());

  case PtStmtType::SeqBlock:
  case PtStmtType::NamedSeqBlock:
    for ( auto stmt1: stmt->stmt_list() ) {
      if ( !check_function_statement(stmt1) ) {
	return false;
      }
    }
    return true;

  default:
    break;
  }
  ostringstream buf;
  buf << stmt->stmt_name()
      << " cannot be used in function declaration.";
  MsgMgr::put_msg(__FILE__, __LINE__,
		  stmt->file_region(),
		  MsgType::Error,
		  "PARS",
		  buf.str());
  return false;
}

// default ラベルが2つ以上含まれていないかどうかのチェック
bool
Parser::check_default_label(const PtrList<const PtCaseItem>* ci_list)
{
  SizeType n = 0;
  for ( auto ci: *ci_list ) {
    if ( ci->label_num() == 0 ) {
      ++ n;
      if ( n > 1 ) {
	MsgMgr::put_msg(__FILE__, __LINE__,
			ci->file_region(),
			MsgType::Error,
			"PARS",
			" more than one 'default' labels.");
	return false;
      }
    }
  }
  return true;
}

// @brief 階層名の生成
// @param[in] head_name 階層の上位部分
// @param[in] name 階層の最下位部分
PuHierName*
Parser::new_HierName(const char* head_name,
		     const char* name)
{
  auto nb{mFactory->new_NameBranch(head_name)};
  auto hname{new_HierName(nb, name)};
  return hname;
}

// @brief 階層名の生成
// @param[in] head_name 階層の上位部分
// @param[in] index インデックス
// @param[in] name 階層の最下位部分
PuHierName*
Parser::new_HierName(const char* head_name,
		     int index,
		     const char* name)
{
  auto nb{mFactory->new_NameBranch(head_name, index)};
  auto hname{new_HierName(nb, name)};
  return hname;
}

// @brief 階層名の生成
// @param[in] head_name 階層の上位部分
// @param[in] index インデックス
// @param[in] name 階層の最下位部分
PuHierName*
Parser::new_HierName(const PtNameBranch* nb,
		     const char* name)
{
  auto hname{mFactory->new_HierName(nb, name)};
  return hname;
}

// @brief 階層名の追加
// @aram[in] hname 階層名の上位部分
// @param[in] name 追加する名前
void
Parser::add_HierName(PuHierName* hname,
		     const char* name)
{
  auto nb{mFactory->new_NameBranch(hname->tail_name())};
  hname->add(nb, name);
}

// @brief 階層名の追加
// @aram[in] hname 階層名の上位部分
// @param[in] index インデックス
// @param[in] name 追加する名前
void
Parser::add_HierName(PuHierName* hname,
		     int index,
		     const char* name)
{
  auto nb{mFactory->new_NameBranch(hname->tail_name(), index)};
  hname->add(nb, name);
}

// @brief parameter port 宣言ヘッダを追加する．
void
Parser::add_paramport_head(PtiDeclHead* head,
			   PtrList<const PtAttrInst>* attr_list)
{
  if ( head ) {
    reg_attrinst(head, attr_list);
    mParamPortHeadList.push_back(head);
  }
}

// @brief parameter port 宣言の終わり
void
Parser::flush_paramport()
{
  if ( !mDeclItemList.empty() ) {
    ASSERT_COND( !mParamPortHeadList.empty() );
    auto last{mParamPortHeadList.back()};
    last->set_elem(PtiDeclItemArray(mAlloc, mDeclItemList));
    mDeclItemList.clear();
  }
}

// @brief IOポート宣言リストにIO宣言ヘッダを追加する．
void
Parser::add_ioport_head(PtiIOHead* head,
			PtrList<const PtAttrInst>* attr_list)
{
  if ( head ) {
    reg_attrinst(head, attr_list);
    mCurIOHeadList->push_back(head);
  }
}

// @brief IO宣言の終わり
void
Parser::flush_io()
{
  if ( !mIOItemList.empty() ) {
    ASSERT_COND( !mCurIOHeadList->empty() );
    auto last{mCurIOHeadList->back()};
    last->set_elem(PtiIOItemArray(mAlloc, mIOItemList));
    mIOItemList.clear();
  }
}

// @brief IO宣言リストにIO宣言ヘッダを追加する．
void
Parser::add_io_head(PtiIOHead* head,
		    PtrList<const PtAttrInst>* attr_list)
{
  add_ioport_head(head, attr_list);
  flush_io();
}

// @brief IO宣言リストにIO宣言要素を追加する．
void
Parser::add_io_item(const PtIOItem* item)
{
  mIOItemList.push_back(item);
}

// @brief 宣言リストに宣言ヘッダを追加する．
void
Parser::add_decl_head(PtiDeclHead* head,
		      PtrList<const PtAttrInst>* attr_list)
{
  if ( head ) {
    reg_attrinst(head, attr_list);
    cur_declhead_list().push_back(head);
    if ( !mDeclItemList.empty() ) {
      head->set_elem(PtiDeclItemArray(mAlloc, mDeclItemList));
    }
  }
  mDeclItemList.clear();
}

// @brief 宣言リストに宣言要素を追加する．
void
Parser::add_decl_item(const PtDeclItem* item)
{
  mDeclItemList.push_back(item);
}

// @brief item リストに要素を追加する．
void
Parser::add_item(const PtItem* item,
		 PtrList<const PtAttrInst>* attr_list)
{
  if ( item ) {
    reg_attrinst(item, attr_list);
    cur_item_list().push_back(item);
  }
}

// @brief block-statment の開始
void
Parser::init_block()
{
  push_declhead_list();
}

// @brief block-statement の終了
void
Parser::end_block()
{
  mCurDeclArray = pop_declhead_list();
}

END_NAMESPACE_YM_VERILOG
