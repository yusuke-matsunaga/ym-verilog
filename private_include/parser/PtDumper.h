#ifndef PTDUMPER_H
#define PTDUMPER_H

/// @file PtDumper.h
/// @brief PtDumper のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.

#include "ym/pt/PtP.h"
#include "ym/FileLoc.h"
#include "ym/FileRegion.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class PtDumper PtDumper.h <ym_veriog/pt/PtDumper.h>
/// @ingroup VlParser
/// @ingroup PtGroup
/// @brief Pt クラスの内容を出力するクラス
//////////////////////////////////////////////////////////////////////
class PtDumper
{
  friend class PtHeader;
public:

  /// @brief コンストラクタ
  PtDumper(
    ostream& s ///< [in] 出力先のストリーム
  );

  /// @brief デストラクタ
  ~PtDumper();


public:

  /// @brief UDP とモジュールの内部情報の表示
  void
  put(
    const vector<const PtUdp*>& udp_list,      ///< [in] UDP のリスト
    const vector<const PtModule*>& module_list ///< [in] モジュールのリスト
  );


  /// @brief UDP を表示する
  void
  put(
    const PtUdp* udp ///< [in] UDP
  );

  /// @brief UDP テーブルの値を表示する
  void
  put(
    const char* label,  ///< [in] ラベル
    const PtUdpValue* v ///< [in] テーブルの値
  );

  /// @brief module を表示する
  void
  put(
    const PtModule* m ///< [in] モジュール
  );

  /// @brief IO のヘッダを出力する．
  void
  put(
    const char* label, ///< [in] ラベル
    const PtIOHead* io ///< [in] IOヘッダ
  );

  /// @brief 宣言のヘッダを出力する．
  void
  put(
    const char* label,     ///< [in] ラベル
    const PtDeclHead* decl ///< [in] 宣言ヘッダ
  );

  /// @brief 要素の出力
  void
  put(
    const char* label, ///< [in] ラベル
    const PtItem* item ///< [in] 要素
  );

  /// @brief ステートメントの出力
  void
  put(
    const char* label, ///< [in] ラベル
    const PtStmt* stmt ///< [in] ステートメント
  );

  /// @brief 式の内容を出力する
  void
  put(
    const char* label, ///< [in] ラベル
    const PtExpr* expr ///< [in] 式
  );


public:

  /// @brief ファイル位置情報を出力するモードににする
  void
  enable_file_loc_mode();

  /// @brief ファイル位置情報の出力を抑制するモードにする
  void
  disable_file_loc_mode();

  /// @brief ファイル位置情報の出力モードの取得
  /// @return ファイル位置情報を出力するモードのとき true を返す．
  bool
  file_loc_mode() const;


private:

  /// @brief インクルード元のファイル情報の出力
  void
  put_parent_file(
    const FileLoc& file_loc ///< [in] ファイルの位置情報
  );

  /// @brief 宣言を出力する．
  void
  put_decls(
    const vector<const PtIOHead*>& iohead_array,
    const vector<const PtDeclHead*>& declhead_array
  );

  /// @brief 宣言と要素を出力する．
  void
  put_decl_item(
    const char* label,                           ///< [in] ラベル
    const vector<const PtDeclHead*>& decl_array, ///< [in] 宣言の配列
    const vector<const PtItem*>& item_array      ///< [in] 要素の配列
  );

  /// @brief 階層名の出力
  void
  put(
    const vector<const PtNameBranch*>& nb_array
  );

  /// @brief ブール型データの出力
  void
  put(
    const char* label, ///< [in] ラベル
    bool b             ///< [in] ブール値
  );

  /// @brief 整数型データの出力
  void
  put(
    const char* label, ///< [in] ラベル
    int d              ///< [in] 整数値
  );

  /// @brief 実数型データの出力
  void
  put(
    const char* label, ///< [in] ラベル
    double r           ///< [in] 実数値
  );

  /// @brief 文字列型データの出力
  void
  put(
    const char* label, ///< [in] ラベル
    const char* str    ///< [in] 文字列
  );

  /// @brief 文字列型データの出力
  void
  put(
    const char* label, ///< [in] ラベル
    const string& str  ///< [in] 文字列
  );

  /// @brief 補助的なデータ型の出力
  void
  put(
    const char* label,  ///< [in] ラベル
    VpiAuxType aux_type ///< [in] 補助的なデータ型
  );

  /// @brief ネット型の出力
  void
  put(
    const char* label,  ///< [in] ラベル
    VpiNetType net_type ///< [in] ネット型
  );

  /// @brief 変数型の出力
  void
  put(
    const char* label,  ///< [in] ラベル
    VpiVarType var_type ///< [in] 変数型
  );

  /// @brief 方向の出力
  void
  put(
    const char* label, ///< [in] ラベル
    VpiDir direction   ///< [in] 方向
  );

  /// @brief unconnected drive の出力
  void
  put(
    const char* label, ///< [in] ラベル
    VpiUnconnDrive ud  ///< [in] unconnected drive
  );

  /// @brief default delay mode の出力
  void
  put(
    const char* label,         ///< [in] ラベル
    VpiDefDelayMode delay_mode ///< [in] default delay mode
  );

  /// @brief primitive型の出力
  void
  put(
    const char* label,    ///< [in] ラベル
    VpiPrimType prim_type ///< [in] primitive 型
  );

  /// @brief 演算子型の出力
  void
  put(
    const char* label, ///< [in] ラベル
    VpiOpType op_type  ///< [in] 演算子型
  );

  /// @brief 定数型の出力
  void
  put(
    const char* label,      ///< [in] ラベル
    VpiConstType const_type ///< [in] 定数型
  );

  /// @brief strength の出力
  void
  put(
    const char* label,   ///< [in] ラベル
    VpiStrength strength ///< [in] strength
  );

  /// @brief vectored/scalared 属性の出力
  void
  put(
    const char* label, ///< [in] ラベル
    VpiVsType vs_type  ///< [in] vectored/scalared 属性
  );

  /// @brief 範囲指定モードの出力
  void
  put(
    const char* label,      ///< [in] ラベル
    VpiRangeMode range_mode ///< [in] 範囲指定モード
  );

  /// @brief FileLoc の情報を出力する．
  void
  put(
    const char* label,      ///< [in] ラベル
    const FileLoc& file_loc ///< [in] ファイルの位置情報
  );

  /// @brief FileRegion の情報を出力する．
  void
  put(
    const char* label,         ///< [in] ラベル
    const FileRegion& file_loc ///< [in] ファイル領域の情報
  );

  /// @brief 属性インスタンスを出力する．
  void
  put(
    const char* label,         ///< [in] ラベル
    const PtAttrInst* attr_top ///< [in] 属性インスタンスの先頭
  );

  /// @brief コントロールの内容を表示する．
  void
  put(
    const char* label,    ///< [in] ラベル
    const PtControl* ctrl ///< [in] コントロール
  );

  /// @brief strength の内容を表示する．
  void
  put(
    const char* label,    ///< [in] ラベル
    const PtStrength* str ///< [in] 信号強度
  );

  /// @brief 遅延情報を出力する．
  void
  put(
    const char* label,   ///< [in] ラベル
    const PtDelay* delay ///< [in] 遅延
  );

  /// @brief 接続情報を表示する．
  void
  put(
    const char* label,      ///< [in] ラベル
    const PtConnection* con ///< [in] 接続情報
  );

  /// @brief 階層ブランチの情報を出力する
  void
  put(
    const char* label,     ///< [in] ラベル
    const PtNameBranch* nb ///< [in] 階層ブランチ
  );

  /// @brief 開始マーカを出力する．
  void
  put_begin(
    const char* label,   ///< [in] マーカのラベル
    const char* type,    ///< [in] タイプ
    bool need_cr = true  ///< [in] true の時はマーカ出力後に改行する．
  );

  /// @brief 終了マーカを出力する．
  void
  put_end(
    const char* label ///< [in] マーカのラベル
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 出力ストリーム
  ostream& mStream;

  // マーカ出力後に改行したことを示すフラグ
  list<bool> mDoCR;

  // インデント量
  int mIndent;

  // ファイル位置情報の出力フラグ
  bool mFileLocMode;

};

END_NAMESPACE_YM_VERILOG

#endif // PTDUMPER_H
