#ifndef CPTUDP_H
#define CPTUDP_H

/// @file CptUdp.h
/// @brief CptUdp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym/pt/PtUdp.h"
#include "ym/pt/PtP.h"
#include "ym/VlUdpVal.h"
#include "ym/FileRegion.h"
#include "parser/PtiArray.h"
#include "parser/PtiFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @brief UDP を表すノード
//////////////////////////////////////////////////////////////////////
class CptUdp :
  public PtUdp
{
  friend class CptFactory;

private:

  /// @brief コンストラクタ
  CptUdp(const FileRegion& file_region,
	 const char* name,
	 PtiPortArray&& port_array,
	 PtiIOHeadArray&& iohead_array,
	 bool is_seq,
	 const PtExpr* init_value,
	 PtiUdpEntryArray&& entry_array);

  /// @brief デストラクタ
  ~CptUdp();


public:
  //////////////////////////////////////////////////////////////////////
  // PtUdp の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  FileRegion
  file_region() const override;

  /// @brief primitivie type を返す．
  VpiPrimType
  prim_type() const override;

  /// @brief 名前を返す．
  const char*
  name() const override;

  /// @brief ポート数を取り出す．
  SizeType
  port_num() const override;

  /// @brief ポートを取り出す．
  /// @param[in] pos 位置 ( 0 <= pos < port_num() )
  const PtPort*
  port(SizeType pos) const override;

  /// @brief 入出力宣言ヘッダ配列の要素数の取得
  SizeType
  iohead_num() const override;

  /// @brief 入出力宣言ヘッダの取得
  /// @param[in] pos 位置 ( 0 <= pos < iohead_num() )
  const PtIOHead*
  iohead(SizeType pos) const override;

  /// @brief 初期値を取出す．
  const PtExpr*
  init_value() const override;

  /// @brief テーブルの要素数を取り出す．
  SizeType
  table_num() const override;

  /// @brief テーブルの要素を取り出す．
  /// @param[in] pos 位置 ( 0 <= pos < table_num() )
  const PtUdpEntry*
  table(SizeType pos) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // プリミティブ名
  const char* mName;

  // ポートの配列
  PtiPortArray mPortArray;

  // 入出力宣言の配列
  PtiIOHeadArray mIOHeadArray;

  // sequential primitive の時 true
  bool mSeq;

  // 初期値
  const PtExpr* mInitValue;

  // テーブル要素の配列
  PtiUdpEntryArray mTableArray;

};


//////////////////////////////////////////////////////////////////////
/// @brief UDP の中身のテーブルを表すクラス(組合せ回路用)
//////////////////////////////////////////////////////////////////////
class CptUdpEntry :
  public PtUdpEntry
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  CptUdpEntry(const FileRegion& file_region,
	      PtiUdpValueArray&& input_array,
	      const PtUdpValue* output);

  /// @brief デストラクタ
  ~CptUdpEntry();


public:
  //////////////////////////////////////////////////////////////////////
  // PtUdpEntry の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  FileRegion
  file_region() const override;

  /// @brief 入力値の配列の要素数を取り出す．
  SizeType
  input_num() const override;

  /// @brief 入力値を取り出す．
  /// @param[in] pos 位置 ( 0 <= pos < input_num() )
  const PtUdpValue*
  input(SizeType pos) const override;

  /// @brief 現状態の値を取り出す．
  /// @note このクラスでは nullptr を返す．
  const PtUdpValue*
  current() const override;

  /// @brief 出力の値を取り出す．
  const PtUdpValue*
  output() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 入力パタンの配列
  PtiUdpValueArray mInputArray;

  // 出力のパタン
  const PtUdpValue* mOutput;

};


//////////////////////////////////////////////////////////////////////
/// @brief UDP の中身のテーブルを表すクラス(順序回路用)
//////////////////////////////////////////////////////////////////////
class CptUdpEntryS :
  public CptUdpEntry
{
  friend class CptFactory;

private:

  /// @brief コンストラクタ
  CptUdpEntryS(const FileRegion& file_region,
	       PtiUdpValueArray&& input_array,
	       const PtUdpValue* current,
	       const PtUdpValue* output);

  /// @brief デストラクタ
  ~CptUdpEntryS();


public:
  //////////////////////////////////////////////////////////////////////
  // PtUdpEntry の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 現状態の値を取り出す．
  const PtUdpValue*
  current() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 現状態のパタン
  const PtUdpValue* mCurrent;

};


//////////////////////////////////////////////////////////////////////
/// @brief UDP のテーブルの値を表すクラス
//////////////////////////////////////////////////////////////////////
class CptUdpValue :
  public PtUdpValue
{
  friend class CptFactory;

private:

  /// @brief コンストラクタ
  CptUdpValue(const FileRegion& file_region,
	      char symbol);

  /// @brief コンストラクタ
  CptUdpValue(const FileRegion& file_region,
	      char symbol1,
	      char symbol2);

  /// @brief デストラクタ
  ~CptUdpValue();


public:
  //////////////////////////////////////////////////////////////////////
  // PtUdpValue の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  FileRegion
  file_region() const override;

  /// @brief シンボルを取り出す．
  VlUdpVal
  symbol() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // シンボル
  VlUdpVal mSymbol;

};

END_NAMESPACE_YM_VERILOG

#endif // CPTUDP_H
