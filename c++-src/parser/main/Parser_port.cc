/// @file Parser_port.cc
/// @brief Parser の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.


#include "parser/Parser.h"
#include "parser/PtiDecl.h"
#include "parser/PtiExpr.h"

#include "ym/MsgMgr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// PtiPort の生成
//////////////////////////////////////////////////////////////////////

// @brief 空のポートの生成
void
Parser::new_Port()
{
  add_port( mFactory.new_Port(FileRegion()) );
}

// @brief ポートの生成 (内側の式のみ指定するタイプ)
// @param[in] file_region ファイル位置
// @param[in] name 外側の名前
void
Parser::new_Port1(const FileRegion& file_region)
{
  // 明示的に外の名前がついていなくても内側の名前が1つで
  // 範囲指定が無いときには内側の名前を外側の名前とする．
  if ( mPortRefList.size() == 1 ) {
    auto portref = mPortRefList.front();
    const char* name = nullptr;
    if ( portref->index_num() == 0 && portref->left_range() == nullptr ) {
      name = portref->name();
    }
    mPortRefList.clear();
    add_port( mFactory.new_Port(file_region, portref, name) );
  }
  else {
    PtExprArray portref_array = mPortRefList.to_array();
    auto portref = mFactory.new_Concat(file_region, portref_array);
    auto port = mFactory.new_Port(file_region, portref, portref_array, nullptr);
    add_port(port);
  }
}

// @brief ポートの生成 (外側の名前のみ指定するタイプ)
// @param[in] file_region ファイル位置
// @param[in] name 外側の名前
void
Parser::new_Port2(const FileRegion& file_region,
		  const char* name)
{
  auto port = mFactory.new_Port(file_region, name);
  add_port(port);
}

// @brief ポートの生成 (外側の名前と内側の式を指定するタイプ)
// @param[in] file_region ファイル位置
// @param[in] name 外側の名前
void
Parser::new_Port3(const FileRegion& file_region,
		  const char* name)
{
  if ( mPortRefList.size() == 1 ) {
    auto port = mFactory.new_Port(file_region, mPortRefList.front(), name);
    add_port(port);
    mPortRefList.clear();
  }
  else {
    PtExprArray portref_array = mPortRefList.to_array();
    auto portref = mFactory.new_Concat(file_region, portref_array);
    auto port = mFactory.new_Port(file_region, portref, portref_array, name);
    add_port(port);
  }
}

// @brief ポートリストにポートを追加する．
inline
void
Parser::add_port(PtiPort* port)
{
  mPortList.push_back(port);
}


//////////////////////////////////////////////////////////////////////
// PtiPortArray の生成
//////////////////////////////////////////////////////////////////////

// @brief 入出力宣言中の重複チェックを行う．
bool
Parser::check_PortArray(PtIOHeadArray iohead_array)
{
  unordered_set<string> portref_dic;
  for ( auto head: iohead_array ) {
    for ( auto elem: head->item_list() ) {
      auto name = elem->name();
      if ( portref_dic.count(name) > 0 ) {
	ostringstream buf;
	buf << "\"" << name << "\" is redefined.";
	MsgMgr::put_msg(__FILE__, __LINE__,
			elem->file_region(),
			MsgType::Error,
			"ELAB",
			buf.str());
	return false;
      }
      portref_dic.insert(name);
    }
  }
  return true;
}

// @brief PtiPort の vector からポート配列を作る．
PtPortArray
Parser::new_PortArray(const vector<PtiPort*>& port_vector)
{
  // vector<PtiPort*> -> vector<const PtPort*> への変換
  SizeType n{port_vector.size()};
  vector<const PtPort*> tmp_vec(n);
  for ( int i = 0; i < n; ++ i ) {
    tmp_vec[i] = port_vector[i];
  }
  return PtPortArray(tmp_vec);
}

// @brief 入出力宣言からポートを作る．
PtPortArray
Parser::new_PortArray(PtIOHeadArray iohead_array)
{
  SizeType num = 0;
  for ( auto head: iohead_array ) {
    num += head->item_list().size();
  }

  // ポートを生成し vec に格納する．
  SizeType i = 0;
  vector<const PtPort*> vec(num);
  for ( auto head: iohead_array ) {
    for ( auto elem: head->item_list() ) {
      auto name = elem->name();
      auto portref = mFactory.new_Primary(elem->file_region(), name);
      auto port = mFactory.new_Port(elem->file_region(), portref, name);
      VpiDir dir = head->direction();
      port->_set_portref_dir(0, dir);
      vec[i] = port;
      ++ i;
    }
  }
  return PtPortArray(vec);
}


//////////////////////////////////////////////////////////////////////
// portref の生成
//////////////////////////////////////////////////////////////////////

// @brief ポート参照式の生成
// @param[in] fr ファイル位置の情報
// @param[in] name ポートに接続している内部の識別子名
void
Parser::new_PortRef(const FileRegion& fr,
		    const char* name)
{
  auto primary = mFactory.new_Primary(fr, name);
  add_portref(primary);
}

// @brief ビット指定つきポート参照式の生成
// @param[in] fr ファイル位置の情報
// @param[in] name ポートに接続している内部の識別子名
// @param[in] index ビット指定用の式
void
Parser::new_PortRef(const FileRegion& fr,
		    const char* name,
		    const PtExpr* index)
{
  PtrList<const PtExpr> index_list;
  index_list.push_back(index);
  PtExprArray index_array = to_array(&index_list);
  auto primary = mFactory.new_Primary(fr, name, index_array);
  add_portref(primary);
}

// @brief 範囲指定付きポート参照式の生成
// @param[in] fr ファイル位置の情報
// @param[in] name ポートに接続している内部の識別子名
// @param[in] range_mode 範囲指定のモード
// @param[in] left 範囲指定の左側の式
// @param[in] right 範囲指摘の右側の式
void
Parser::new_PortRef(const FileRegion& fr,
		    const char* name,
		    VpiRangeMode range_mode,
		    const PtExpr* left,
		    const PtExpr* right)
{
  auto primary = mFactory.new_Primary(fr, name, range_mode, left, right);
  add_portref(primary);
}

// @brief ポート参照リストを初期化する．
void
Parser::init_portref_list()
{
  mPortRefList.clear();
}

// @brief ポート参照リストに要素を追加する．
inline
void
Parser::add_portref(const PtExpr* portref)
{
  mPortRefList.push_back(portref);
}

END_NAMESPACE_YM_VERILOG
