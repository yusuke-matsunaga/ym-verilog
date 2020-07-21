
/// @file InputMgr.cc
/// @brief InputMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014, 2019 Yusuke Matsunaga
/// All rights reserved.


#include "InputMgr.h"

#include "ym/FileInfo.h"


// ファイル末尾に改行がなくても warning としない時に 1
#define ALLOW_EOF_WITHOUT_NL 1


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// InputMgr のパブリックなメンバ関数
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
InputMgr::InputMgr()
{
}

// @brief デストラクタ
InputMgr::~InputMgr()
{
}

// @brief 初期状態に戻す．
// 読み込みの途中でこれを呼ぶと大変なことになる．
// 普通は正常に読み終わったあとか, エラーの後で呼ぶ．
void
InputMgr::clear()
{
  mFileStack.clear();
}


//////////////////////////////////////////////////////////////////////
// サーチパスとファイル関係
//////////////////////////////////////////////////////////////////////

// @brief サーチパスリストを設定する．
// @param[in] searchpath セットするサーチパス
void
InputMgr::set_searchpath(const SearchPathList& searchpath)
{
  mSearchPathList = searchpath;
}

// @brief 設定されているサーチパスを考慮して filename を探す．
// @param[in] filename ファイル名
// @return 実際の絶対パス
PathName
InputMgr::search_file(const string& filename)
{
  // filename が存在して読めなければならない．
  return mSearchPathList.search(PathName(filename));
}

// @brief ファイルをオープンする．
// @param[in] filename ファイル名
// @param[in] parent_file インクルード元のファイル情報
// @retval true オープンに成功した．
// @retval false ファイルが開けなかった
// 設定されているサーチパスを考慮して filename を探す．
bool
InputMgr::open_file(const string& filename,
		    const FileLoc& parent_file)
{
  auto pathname = search_file(filename);
  if ( !pathname.is_valid() ) {
    return false;
  }
  // 本当のパス名
  string realname = pathname.str();

  ifstream tmp_in(realname);
  if ( !tmp_in ) {
    return false;
  }
  mFsStack.push_back(ifstream()); // ダミー
  ifstream& in = mFsStack.back();
  in.swap(tmp_in);

  mFileStack.push_back(unique_ptr<InputFileObj>{new InputFileObj{in, {realname, parent_file}}});

  return true;
}

// @brief 現在のファイル位置を強制的に書き換える．
// @param[in] new_filename 新しいファイル名
// @param[in] line     新しい行番号
// @param[in] level
//           - 0 インクルード関係のレベル変化無し
//           - 1 新しいファイルはインクルードされたファイル
//           - 2 新しいファイルはインクルードもとのファイル
void
InputMgr::set_file_loc(const char* new_filename,
		       int line,
		       int level)
{
  ASSERT_COND( !mFileStack.empty() );

  FileInfo cur_fi = cur_file().file_info();
  switch ( level ) {
  case 0: // レベルの変化無し
    if ( cur_fi.filename() != new_filename ) {
      // 新しい FileInfo を作る．
      FileLoc flp = cur_fi.parent_loc();
      cur_fi = FileInfo(new_filename, flp);
    }
    break;

  case 1: // 新しいインクルードファイル．
    cur_fi = FileInfo{new_filename, cur_fi.parent_loc()};
    break;

  case 2: // インクルードの終り
    cur_fi = cur_fi.parent_loc().file_info();
    if ( cur_fi.filename() != new_filename ) {
      // 新しい FileInfo を作る．
      cur_fi = FileInfo(new_filename, cur_fi.parent_loc());
    }
    break;
  }
  cur_file().set_file_info(cur_fi);
}

// @brief 現在のファイルを返す．
InputFileObj&
InputMgr::cur_file() const
{
  ASSERT_COND( !mFileStack.empty() );
  return *(mFileStack.front().get());
}

// @brief 現在のファイル名を返す．
string
InputMgr::cur_filename() const
{
  return cur_file().file_info().filename();
}

// @brief 現在の InputFile が EOF を返したときの処理
// @return 処理を続けられる時 true を返す．
bool
InputMgr::wrap_up()
{
  for ( ; ; ) {
    mFsStack.pop_back();
    mFileStack.pop_back();
    if ( mFileStack.empty() ) {
      // もうファイルが残っていない．
      return false;
    }
    if ( !cur_file().is_eof() ) {
      return true;
    }
  }
}

// @brief ファイルのオープン済チェック
// @param[in] name チェックするファイル名
// @retval true name という名のファイルがオープンされている．
// @retval false name というなのファイルはオープンされていない．
bool
InputMgr::check_file(const char* name) const
{
  for ( auto& input_file: mFileStack ) {
    if ( input_file->file_info().filename() == name ) {
      return true;
    }
  }
  return false;
}

// @brief 一文字読み出す．
//
// 実際には peek(); acept() と等価
int
InputMgr::get()
{
  return cur_file().get();
}

// @brief 次の文字を読み出す．
//
// ファイル位置の情報等は変わらない
int
InputMgr::peek()
{
  return cur_file().peek();
}

// @brief 直前の peek() を確定させる．
void
InputMgr::accept()
{
  cur_file().accept();
}

// @brief ファイルの末尾の時にtrue を返す．
bool
InputMgr::is_eof() const
{
  return cur_file().is_eof();
}

// @brief 現在の位置を返す．
FileLoc
InputMgr::cur_loc() const
{
  return cur_file().cur_loc();
}

END_NAMESPACE_YM_VERILOG
