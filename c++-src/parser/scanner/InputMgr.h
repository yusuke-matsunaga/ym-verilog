#ifndef INPUTMGR_H
#define INPUTMGR_H

/// @file InputMgr.h
/// @brief InputMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2013-2014, 2019, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym/verilog.h"

#include "ym/File.h"
#include "ym/FileRegion.h"
#include "ym/Scanner.h"
#include "ym/MsgHandler.h"
#include "ym/StrBuff.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class InputMgr InputMgr.h "InputMgr.h"
/// @ingroup VlParser
/// @brief 入力ファイルを管理するクラス
/// @sa InputFile FileInfo
//////////////////////////////////////////////////////////////////////
class InputMgr
{
public:

  /// @brief コンストラクタ
  InputMgr() = default;

  /// @brief デストラクタ
  ~InputMgr() = default;


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 初期化およびファイル関係
  /// @{

  /// @brief 初期状態に戻す．
  ///
  /// 読み込みの途中でこれを呼ぶと大変なことになる．
  /// 普通は正常に読み終わったあとか, エラーの後で呼ぶ．
  void
  clear();

  /// @brief サーチパスリストを設定する．
  void
  set_searchpath(
    const SearchPathList& searchpath ///< [in] セットするサーチパス
  )
  {
    mSearchPathList = searchpath;
  }

  /// @brief 設定されているサーチパスを考慮して filename を探す．
  /// @return 実際の絶対パス
  ///
  /// PathName::is_valid() で結果がわかる．
  PathName
  search_file(
    const string& filename ///< [in] ファイル名
  )
  {
    // filename が存在して読めなければならない．
    return mSearchPathList.search(PathName(filename));
  }

  /// @brief ファイルをオープンする．
  /// @retval true オープンに成功した．
  /// @retval false ファイルが開けなかった
  bool
  open_file(
    const string& filename,                ///< [in] ファイル名
    const FileLoc& parent_file = FileLoc() ///< [in] インクルード元のファイル情報
  );

  /// @brief ファイルのオープン済チェック
  /// @retval true name という名のファイルがオープンされている．
  /// @retval false name というなのファイルはオープンされていない．
  bool
  check_file(
    const char* filename ///< [in] チェックするファイル名
  ) const;

  /// @brief 現在のファイル位置を強制的に書き換える．
  void
  set_file_loc(
    const char* new_filename, ///< [in] 新しいファイル名
    int line,                 ///< [in] 新しい行番号
    int level                 ///< [in] インクルードレベル
                              ///<   - 0 インクルード関係のレベル変化無し
                              ///<   - 1 新しいファイルはインクルードされたファイル
                              ///<   - 2 新しいファイルはインクルードもとのファイル
  );

  /// @brief 現在のファイルを返す．
  Scanner&
  cur_file() const;

  /// @brief 現在のファイル名を返す．
  string
  cur_filename() const;

  /// @brief 現在の InputFile が EOF を返したときの処理
  /// @return 処理を続けられる時 true を返す．
  bool
  wrap_up();

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name データの入力関係の関数
  /// @{

  /// @brief 一文字読み出す．
  ///
  /// 実際には peek(); acept() と等価
  int
  get() { return cur_file().get(); }

  /// @brief 次の文字を読み出す．
  ///
  /// ファイル位置の情報等は変わらない
  int
  peek() { return cur_file().peek(); }

  /// @brief 直前の peek() を確定させる．
  void
  accept() { cur_file().accept(); }

  /// @brief ファイルの末尾の時にtrue を返す．
  bool
  is_eof() const { return cur_file().is_eof(); }

  /// @brief 現在の位置を返す．
  FileLoc
  cur_loc() const { return cur_file().cur_pos(); }

  /// @}
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる下請け関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // サーチパス
  SearchPathList mSearchPathList;

  // ファイルストリームのスタック
  vector<ifstream> mFsStack;

  // ファイルごとのスキャナーのスタック
  vector<unique_ptr<Scanner>> mFileStack;

};

END_NAMESPACE_YM_VERILOG

#endif // INPUTMGR_H
