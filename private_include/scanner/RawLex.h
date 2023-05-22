#ifndef RAWLEX_H
#define RAWLEX_H

/// @file RawLex.h
/// @brief RawLex のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym/verilog.h"

#include "ym/StrBuff.h"
#include "ym/File.h"
#include "ym/FileRegion.h"
#include "ym/Binder.h"


BEGIN_NAMESPACE_YM_VERILOG

class VlLineWatcher;
class LexPlugin;
class LexState;
class LexCondState;
class LexPluginDict;
class RsrvWordDic;
class InputMgr;
class MacroSource;
class TokenInfo;
class TokenList;

//////////////////////////////////////////////////////////////////////
/// @class RawLex RawLex.h "RawLex.h"
/// @ingroup VlParser
/// @brief Verilog 用の低レベルな字句解析ルーティン
///
/// 基本的なトークンの切り出しはここで行うが，compiler-directive
/// の処理は Lex で行う．
/// @sa InputFileObj Lex VlLineWatcher
//////////////////////////////////////////////////////////////////////
class RawLex
{
  friend class LexPlugin;

public:

  /// @brief 解析用のコンテキスト
  enum Context {
    /// @brief 通常
    NORMAL,
    /// @brief UDP 定義のテーブル記述内
    UDP,
    /// @brief 2進定数
    BIN,
    /// @brief 8進定数
    OCT,
    /// @brief 10進定数
    DEC,
    /// @brief 16進定数
    HEX
  };


public:

  /// @brief コンストラクタ
  RawLex();

  /// @brief デストラクタ
  ~RawLex();


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
    const SearchPathList& searchpath = SearchPathList() ///< [in] セットするサーチパス
  );

  /// @brief ファイルをオープンする．
  /// @retval true オープンに成功した．
  /// @retval false ファイルが開けなかった
  bool
  open_file(
    const string& filename ///< [in] ファイル名
  );

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name トークン読み出しおよび関連した情報の取得
  /// @{

  /// @brief トークンの読み出し
  /// @return 読み込んだトークンの id を返す．
  int
  get_token();

  /// @brief 生のトークンの読み出し
  /// @return 読み出されたトークンの id を返す．
  int
  get_raw_token();

  /// @brief 最後に読んだトークンの位置を返す．
  /// @return 最後に読んだトークンのファイル上の位置
  FileRegion
  cur_token_loc() const
  {
    return mCurPos;
  }

  /// @brief 最後に読んだ文字列を返す．
  /// @return 最後に読んだ文字列
  const char*
  cur_string() const
  {
    return mCurString;
  }

  /// @brief 最後に読んだ文字列を unsigned integer に変換する．
  /// @return 最後に読んだ文字列を unsigned integer に変換したもの
  std::uint32_t
  cur_uint() const
  {
    return mCurUint;
  }

  /// @brief 最後に読んだ文字列を real number に変換する．
  /// @return 最後に読んだ文字列を real number に変換したもの
  double
  cur_rnumber() const
  {
    return strtod(mCurString, static_cast<char**>(nullptr));
  }

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 行番号ウォッチャーの設定
  /// @{

  /// @brief 行番号ウオッチャーを設定する．
  ///
  /// watcher がすでに登録されていればなにもしない．
  void
  reg_watcher(
    VlLineWatcher* watcher ///< [in] 登録対象のウオッチャー
  );

  /// @brief 行番号ウオッチャーの登録を解除する．
  ///
  /// watcher が登録されていなければなにもしない．
  void
  unreg_watcher(
    VlLineWatcher* watcher ///< [in] 登録削除対象のウオッチャー
  );

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 機能拡張用のインターフェイス
  /// @{

  /// @brief プラグインの追加
  void
  add_plugin(
    const char* name, ///< [in] プラグインを起動するコンパイラディレクティブ名
    LexPlugin* plugin ///< [in] 登録するプラグイン
  );

  /// @brief プラグインの削除
  /// @retval true 削除が成功した．
  /// @retval false name という名のプラグインが存在しなかった．
  bool
  erase_plugin(
    const char* name ///< [in] 削除するプラグイン名
  );

  /// @brief プラグインを登録できるか調べる．
  /// @return true 登録可能
  /// @return false 同名の定義済みプラグインがあり，登録不可能
  bool
  check_pluginname(
    const char* name ///< [in] プラグイン名
  );

  /// @brief 内部状態の追加
  void
  add_state(
    LexState* state ///< [in] 登録する内部状態
  );

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name デバッグモードの制御
  /// @{

  /// @brief デバッグモードのセット
  void
  set_debug(
    bool flag = true ///< [in] true ならすべてのトークンの情報を標準エラーに出力する
  );

  /// @brief デバッグモードの取得
  bool
  debug();

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name その他
  /// @{

  /// @brief `resetall の処理
  void
  resetall(
    const FileRegion& file_region ///< [in] ファイル位置の情報
  );

  /// @brief マクロ定義の検査
  /// @return name という名のマクロが定義されていたら true を返す．
  bool
  is_macro_defined(
    const char* name ///< [in] 名前
  ) const;

  /// @brief マクロの多重展開のチェック
  /// @return マクロ展開が自己ループしていたら true を返す．
  bool
  check_macro(
    const char* name ///< [in] 名前
  ) const;

  /// @brief マクロの追加
  void
  push_macro(
    const char* name,      ///< [in] 名前
    const TokenInfo* top,  ///< [in] トークンの先頭
    TokenList* param_array ///< [in] 引数のリスト
  );

  /// @brief `ifdef/`ifndef 文の現在の条件の取得
  /// @retval true 条件が成り立っている
  /// @retval false 条件が成り立っていない
  bool
  cond() const;

  /// @brief condition の書き換え
  void
  set_cond(
    bool flag ///< [in] 設定する値
  );

  /// @brief condition の反転
  void
  flip_cond();

  /// @brief 改行文字を読み込んだときに呼ばれる関数
  void
  check_line(
    int line ///< [in] 行番号
  );

  /// @brief コンテキストを返す．
  Context
  context()
  {
    return mContext;
  }

  /// @}
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief トークンの読み出しを行う．
  int
  _read_token();

  /// @brief 2進数モードの読み込みを行う．
  /// @return トークンを返す．
  int
  read_bin_str(
    int c ///< [in] 最初の文字
  );

  /// @brief 8進数モードの読み込みを行う．
  /// @return トークンを返す．
  int
  read_oct_str(
    int c ///< [in] 最初の文字
  );

  /// @brief 10進数モードの読み込みを行う．
  /// @return トークンを返す．
  int
  read_dec_str(
    int c ///< [in] 最初の文字
  );

  /// @brief 16進数モードの読み込みを行う．
  /// @return トークンを返す．
  int
  read_hex_str(
    int c ///< [in] 最初の文字
  );

  /// @brief 識別子に用いられる文字([a-zA-Z0-9_$])が続く限り読みつづける．
  void
  read_str();

  /// @brief 二重引用符用の読み込み
  /// @return トークン番号を返す．
  ///
  /// 可能性のあるトークンは
  ///  - STRING
  ///  - ERROR
  int
  read_dq_str();

  /// @brief escaped identifier 用の読み込み
  /// @return トークン番号を返す．
  ///
  /// 可能性のあるトークンは
  ///  - SPACE
  ///  - IDENTIFIER
  ///  - ERROR
  int
  read_esc_str();

  /// @brief 数字を読み込む．
  /// @return トークン番号を返す．
  ///
  /// 可能性のあるトークンは
  ///  - UNUM_INT
  ///  - RNUMBER
  ///  - ERROR
  int
  read_num();

  /// @brief 空白文字を読み飛ばす
  void
  read_space();

  /// @brief '/' を読み込んだ後の処理
  /// @return トークン番号を返す．
  ///
  /// 可能性のあるトークンは
  ///  - COMMENT1
  ///  - COMMENT2
  ///  - '/'
  ///  - ERROR
  int
  read_comment();

  /// @brief 一文字読み出す．
  ///
  /// 実際には peek(); acept() と等価
  int
  get();

  /// @brief 次の文字を読み出す．
  ///
  /// ファイル位置の情報等は変わらない
  int
  peek();

  /// @brief 直前の peek() を確定させる．
  void
  accept();

  /// @brief ファイルの末尾の時にtrue を返す．
  bool
  is_eof() const;

  /// @brief 現在の位置を返す．
  FileLoc
  cur_loc() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力ファイルを管理するオブジェクト
  unique_ptr<InputMgr> mInputMgr;

  // 予約語辞書の参照
  const RsrvWordDic& mDic;

  // 内部状態用のリスト
  vector<unique_ptr<LexState>> mStates;

  // 条件コンパイル用の状態
  LexCondState* mCondState;

  // 現在のトークンの位置
  FileRegion mCurPos;

  // 現在のトークンの文字列
  const char* mCurString;

  // 現在のトークンが整数型のとき対応する整数値
  std::uint32_t mCurUint;

  // module 定義中のとき true となるフラグ
  bool mModuleMode;

  // マクロのスタック
  vector<MacroSource> mMacroStack;

  // 行番号ウオッチャーを管理するクラス
  T1BindMgr<int> mWatcherMgr;

  // プラグイン用の辞書
  unique_ptr<LexPluginDict> mPluginDict;

  // コンテキスト
  Context mContext;

  // read_token の結果の文字列を格納するバッファ
  StrBuff mStringBuff;

  // read_token の読み始めの位置
  FileLoc mFirstLoc;

  // デバッグ時に true にするフラグ
  bool mDebug;

};

END_NAMESPACE_YM_VERILOG

#endif // RAWLEX_H
