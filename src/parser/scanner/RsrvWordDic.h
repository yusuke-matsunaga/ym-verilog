﻿#ifndef RSRVWORDDIC_H
#define RSRVWORDDIC_H

/// @file RsrvWordDic.h
/// @brief RsrvWordDic のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym/verilog.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class RsrvWordDic RsrvWordDic.h "RsrvWordDic.h"
/// @ingroup VlParser
/// @brief Verilog-HDL の予約語テーブル
//////////////////////////////////////////////////////////////////////
class RsrvWordDic
{
public:

  /// @brief 唯一のインスタンスを取ってくる関数
  /// @return 唯一のインスタンスを返す．
  static
  const RsrvWordDic&
  the_dic();

  /// @brief str が予約語ならそのトークン番号を返す．
  /// @param[in] str 文字列
  /// @return str が予約語ならそのトークン番号を返す．
  /// そうでないときは
  /// - 先頭が '$' なら SYS_IDENTIFIER を返す．
  /// - 先頭が '`' なら CD_DEFINED_SYMBOL を返す．
  /// - 上記以外ならば IDENTIFIER を返す．
  int
  token(const char* str) const;

  /// @brief トークンから文字列を取り出す．
  /// @param[in] token トークン番号
  /// @return token に対応した文字列を返す．\n
  /// トークンが正しくなければ nullptr を返す．
  const char*
  str(int token) const;


private:

  /// @brief コンストラクタ
  RsrvWordDic();

  /// @brief デストラクタ
  ~RsrvWordDic();


private:

  // 値を覚えておくためのセル
  struct Cell
  {
    // 文字列
    const char* mStr;

    // トークン
    int mTok;

    // 文字列をキーとしたハッシュ用のリンク
    Cell* mLink1;

    // トークンをキーとしたハッシュ用のリンク
    Cell* mLink2;

  };


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // Cell の本体の配列
  Cell* mCellArray;

  // 文字列をキーとしてトークンを持つハッシュ表
  Cell** mTable1;

  // トークンをキーとして文字列を持つハッシュ表
  Cell** mTable2;

  // テーブルサイズ
  ymuint32 mSize;

};

END_NAMESPACE_YM_VERILOG

#endif // RSRVWORDDIC_H
