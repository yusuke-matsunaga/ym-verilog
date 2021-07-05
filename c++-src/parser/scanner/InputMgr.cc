
/// @file InputMgr.cc
/// @brief InputMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014, 2019, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "InputMgr.h"

#include "ym/FileInfo.h"


// ファイル末尾に改行がなくても warning としない時に 1
#define ALLOW_EOF_WITHOUT_NL 1


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// InputMgr のパブリックなメンバ関数
//////////////////////////////////////////////////////////////////////

// @brief 初期状態に戻す．
void
InputMgr::clear()
{
  mFsStack.clear();
  mFileStack.clear();
}


//////////////////////////////////////////////////////////////////////
// サーチパスとファイル関係
//////////////////////////////////////////////////////////////////////

// @brief ファイルをオープンする．
bool
InputMgr::open_file(
  const string& filename,
  const FileLoc& parent_file
)
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

  mFileStack.push_back(unique_ptr<Scanner>{new Scanner{in, {realname, parent_file}}});

  return true;
}

// @brief 現在のファイル位置を強制的に書き換える．
void
InputMgr::set_file_loc(
  const char* new_filename,
  int line,
  int level
)
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
Scanner&
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
bool
InputMgr::check_file(
  const char* name
) const
{
  for ( auto& file: mFileStack ) {
    if ( file->file_info().filename() == name ) {
      return true;
    }
  }
  return false;
}

END_NAMESPACE_YM_VERILOG
