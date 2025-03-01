﻿
/// @file CptUdp.cc
/// @brief CptUdp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.

#include "CptUdp.h"
#include "alloc/Alloc.h"
#include "parser/CptFactory.h"
#include "parser/PtiDecl.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// UDP を表すノード
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptUdp::CptUdp(
  const FileRegion& file_region,
  const char* name,
  PtiPortArray&& port_array,
  PtiIOHeadArray&& iohead_array,
  bool is_seq,
  const PtExpr* init_value,
  PtiUdpEntryArray&& entry_array
) : mFileRegion{file_region},
    mName{name},
    mPortArray{std::move(port_array)},
    mIOHeadArray{std::move(iohead_array)},
    mSeq{is_seq},
    mInitValue{init_value},
    mTableArray{std::move(entry_array)}
{
}

// デストラクタ
CptUdp::~CptUdp()
{
}

// ファイル位置を返す．
FileRegion
CptUdp::file_region() const
{
  return mFileRegion;
}

// primitivie type を返す．
VpiPrimType
CptUdp::prim_type() const
{
  if ( mSeq ) {
    return VpiPrimType::Seq;
  }
  else {
    return VpiPrimType::Comb;
  }
}

// 名前を返す．
const char*
CptUdp::name() const
{
  return mName;
}

// @brief ポート数を取り出す．
SizeType
CptUdp::port_num() const
{
  return mPortArray.size();
}

// @brief ポートを取り出す．
const PtPort*
CptUdp::port(
  SizeType pos
) const
{
  return mPortArray[pos];
}

// @brief 入出力宣言ヘッダ配列の要素数の取得
SizeType
CptUdp::iohead_num() const
{
  return mIOHeadArray.size();
}

// @brief 入出力宣言ヘッダの取得
const PtIOHead*
CptUdp::iohead(
  SizeType pos
) const
{
  return mIOHeadArray[pos];
}

// 初期値を取出す．
const PtExpr*
CptUdp::init_value() const
{
  return mInitValue;
}

// @brief テーブルの要素数を取り出す．
SizeType
CptUdp::table_num() const
{
  return mTableArray.size();
}

// @brief テーブルの要素を取り出す．
const PtUdpEntry*
CptUdp::table(
  SizeType pos
) const
{
  return mTableArray[pos];
}


//////////////////////////////////////////////////////////////////////
// UDP のテーブルの中身を表すクラス(組合せ回路用)
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptUdpEntry::CptUdpEntry(
  const FileRegion& file_region,
  PtiUdpValueArray&& input_array,
  const PtUdpValue* output
) : mFileRegion{file_region},
    mInputArray{std::move(input_array)},
    mOutput{output}
{
}

// デストラクタ
CptUdpEntry::~CptUdpEntry()
{
}

// ファイル位置を返す．
FileRegion
CptUdpEntry::file_region() const
{
  return mFileRegion;
}

// @brief 入力値の配列の要素数を取り出す．
SizeType
CptUdpEntry::input_num() const
{
  return mInputArray.size();
}

// @brief 入力値を取り出す．
const PtUdpValue*
CptUdpEntry::input(
  SizeType pos
) const
{
  return mInputArray[pos];
}

// @brief 現状態の値を取り出す．
const PtUdpValue*
CptUdpEntry::current() const
{
  return nullptr;
}

// 出力の値を取り出す．
const PtUdpValue*
CptUdpEntry::output() const
{
  return mOutput;
}


//////////////////////////////////////////////////////////////////////
// UDP のテーブルの中身を表すクラス(順序回路用)
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptUdpEntryS::CptUdpEntryS(
  const FileRegion& file_region,
  PtiUdpValueArray&& input_array,
  const PtUdpValue* current,
  const PtUdpValue* output
) : CptUdpEntry{file_region, std::move(input_array), output},
  mCurrent{current}
{
}

// デストラクタ
CptUdpEntryS::~CptUdpEntryS()
{
}

// 現状態の値を取り出す．
const PtUdpValue*
CptUdpEntryS::current() const
{
  return mCurrent;
}


//////////////////////////////////////////////////////////////////////
// UDP のテーブルの値を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptUdpValue::CptUdpValue(
  const FileRegion& file_region,
  char symbol
) : mFileRegion{file_region},
    mSymbol{symbol}
{
}

// コンストラクタ
CptUdpValue::CptUdpValue(
  const FileRegion& file_region,
  char symbol1,
  char symbol2
) : mFileRegion{file_region},
    mSymbol{symbol1, symbol2}
{
}

// デストラクタ
CptUdpValue::~CptUdpValue()
{
}

// ファイル位置を返す．
FileRegion
CptUdpValue::file_region() const
{
  return mFileRegion;
}

// シンボルを取り出す．
VlUdpVal
CptUdpValue::symbol() const
{
  return mSymbol;
}


//////////////////////////////////////////////////////////////////////
// UDP 関係
//////////////////////////////////////////////////////////////////////

// combinational UDP の生成
const PtUdp*
CptFactory::new_CmbUdp(
  const FileRegion& file_region,
  const char* name,
  const vector<const PtPort*>& port_array,
  const vector<const PtIOHead*>& iohead_array,
  const vector<const PtUdpEntry*>& entry_array
)
{
  ++ mNumUdp;
  void* p = mAlloc.get_memory(sizeof(CptUdp));
  auto obj = new (p) CptUdp{file_region,
			    name,
			    PtiArray<const PtPort>{mAlloc, port_array},
			    PtiArray<const PtIOHead>{mAlloc, iohead_array},
			    false, nullptr,
			    PtiArray<const PtUdpEntry>{mAlloc, entry_array}};
  return obj;
}

// sequential UDP の生成
const PtUdp*
CptFactory::new_SeqUdp(
  const FileRegion& file_region,
  const char* name,
  const vector<const PtPort*>& port_array,
  const vector<const PtIOHead*>& iohead_array,
  const PtExpr* init_value,
  const vector<const PtUdpEntry*>& entry_array
)
{
  ++ mNumUdp;
  void* p = mAlloc.get_memory(sizeof(CptUdp));
  auto obj = new (p) CptUdp{file_region,
			    name,
			    PtiArray<const PtPort>{mAlloc, port_array},
			    PtiArray<const PtIOHead>{mAlloc, iohead_array},
			    true,
			    init_value,
			    PtiArray<const PtUdpEntry>{mAlloc, entry_array}};
  return obj;
}

// combinational UDP 用のテーブルエントリの生成
const PtUdpEntry*
CptFactory::new_UdpEntry(
  const FileRegion& file_region,
  const vector<const PtUdpValue*>& input_array,
  const PtUdpValue* output
)
{
  ++ mNumUdpEntry;
  void* p = mAlloc.get_memory(sizeof(CptUdpEntry));
  auto obj = new (p) CptUdpEntry{file_region,
				 PtiArray<const PtUdpValue>{mAlloc, input_array},
				 output};
  return obj;
}

// sequential UDP 用のテーブルエントリの生成
const PtUdpEntry*
CptFactory::new_UdpEntry(
  const FileRegion& file_region,
  const vector<const PtUdpValue*>& input_array,
  const PtUdpValue* current,
  const PtUdpValue* output
)
{
  ++ mNumUdpEntryS;
  void* p = mAlloc.get_memory(sizeof(CptUdpEntryS));
  auto obj = new (p) CptUdpEntryS{file_region,
				  PtiArray<const PtUdpValue>{mAlloc, input_array},
				  current,
				  output};
  return obj;
}

// UDP のテーブルエントリの要素の値の生成 (1つの値)
const PtUdpValue*
CptFactory::new_UdpValue(
  const FileRegion& file_region,
  char symbol
)
{
  ++ mNumUdpValue;
  void* p = mAlloc.get_memory(sizeof(CptUdpValue));
  auto obj = new (p) CptUdpValue{file_region, symbol};
  return obj;
}

// @brief UDP のテーブルエントリの要素の値の生成
const PtUdpValue*
CptFactory::new_UdpValue(
  const FileRegion& file_region,
  char symbol1,
  char symbol2
)
{
  ++ mNumUdpValue;
  void* p = mAlloc.get_memory(sizeof(CptUdpValue));
  auto obj = new (p) CptUdpValue{file_region, symbol1, symbol2};
  return obj;
}

END_NAMESPACE_YM_VERILOG
