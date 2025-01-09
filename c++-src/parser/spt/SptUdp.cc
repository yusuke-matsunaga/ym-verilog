
/// @file SptUdp.cc
/// @brief SptUdp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.

#include "SptUdp.h"
#include "parser/SptFactory.h"

#include "parser/PtiDecl.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// UDP を表すノード
//////////////////////////////////////////////////////////////////////

// コンストラクタ
SptUdp::SptUdp(
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
SptUdp::~SptUdp()
{
}

// ファイル位置を返す．
FileRegion
SptUdp::file_region() const
{
  return mFileRegion;
}

// primitivie type を返す．
VpiPrimType
SptUdp::prim_type() const
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
SptUdp::name() const
{
  return mName;
}

// @brief ポート数を取り出す．
SizeType
SptUdp::port_num() const
{
  return mPortArray.size();
}

// @brief ポートを取り出す．
const PtPort*
SptUdp::port(
  SizeType pos
) const
{
  return mPortArray[pos];
}

// @brief 入出力宣言ヘッダ配列の要素数の取得
SizeType
SptUdp::iohead_num() const
{
  return mIOHeadArray.size();
}

// @brief 入出力宣言ヘッダの取得
const PtIOHead*
SptUdp::iohead(
  SizeType pos
) const
{
  return mIOHeadArray[pos];
}

// 初期値を取出す．
const PtExpr*
SptUdp::init_value() const
{
  return mInitValue;
}

// @brief テーブルの要素数を取り出す．
SizeType
SptUdp::table_num() const
{
  return mTableArray.size();
}

// @brief テーブルの要素を取り出す．
const PtUdpEntry*
SptUdp::table(
  SizeType pos
) const
{
  return mTableArray[pos];
}


//////////////////////////////////////////////////////////////////////
// UDP のテーブルの中身を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
SptUdpEntry::SptUdpEntry(
  const FileRegion& file_region,
  PtiUdpValueArray&& input_array,
  const PtUdpValue* current,
  const PtUdpValue* output
) : mFileRegion{file_region},
    mInputArray{std::move(input_array)},
    mCurrent{current},
    mOutput{output}
{
}

// デストラクタ
SptUdpEntry::~SptUdpEntry()
{
}

// ファイル位置を返す．
FileRegion
SptUdpEntry::file_region() const
{
  return mFileRegion;
}

// @brief 入力値の配列の要素数を取り出す．
SizeType
SptUdpEntry::input_num() const
{
  return mInputArray.size();
}

// @brief 入力値を取り出す．
const PtUdpValue*
SptUdpEntry::input(
  SizeType pos
) const
{
  return mInputArray[pos];
}

// 現状態の値を取り出す．
const PtUdpValue*
SptUdpEntry::current() const
{
  return mCurrent;
}

// 出力の値を取り出す．
const PtUdpValue*
SptUdpEntry::output() const
{
  return mOutput;
}


//////////////////////////////////////////////////////////////////////
// UDP のテーブルの値を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
SptUdpValue::SptUdpValue(
  const FileRegion& file_region,
  char symbol1
) : mFileRegion{file_region},
    mSymbol{symbol1}
{
}

// コンストラクタ
SptUdpValue::SptUdpValue(
  const FileRegion& file_region,
  char symbol1,
  char symbol2
) : mFileRegion{file_region},
    mSymbol{symbol1, symbol2}
{
}

// デストラクタ
SptUdpValue::~SptUdpValue()
{
}

// ファイル位置を返す．
FileRegion
SptUdpValue::file_region() const
{
  return mFileRegion;
}

// 1つめのシンボルを取り出す．
VlUdpVal
SptUdpValue::symbol() const
{
  return mSymbol;
}


//////////////////////////////////////////////////////////////////////
// UDP 関係のオブジェクトの生成
//////////////////////////////////////////////////////////////////////

// @brief combinational UDP の生成
const PtUdp*
SptFactory::new_CmbUdp(
  const FileRegion& file_region,
  const char* name,
  const vector<const PtPort*>& port_array,
  const vector<const PtIOHead*>& iohead_array,
  const vector<const PtUdpEntry*>& entry_array
)
{
  auto node = new SptUdp{file_region,
			 name,
			 PtiPortArray{mAlloc, port_array},
			 PtiIOHeadArray{mAlloc, iohead_array},
			 false, nullptr,
			 PtiUdpEntryArray{mAlloc, entry_array}};
  return node;
}

// @brief sequential UDP の生成
const PtUdp*
SptFactory::new_SeqUdp(
  const FileRegion& file_region,
  const char* name,
  const vector<const PtPort*>& port_array,
  const vector<const PtIOHead*>& iohead_array,
  const PtExpr* init_value,
  const vector<const PtUdpEntry*>& entry_array
)
{
  auto node = new SptUdp{file_region,
			 name,
			 PtiPortArray{mAlloc, port_array},
			 PtiIOHeadArray{mAlloc, iohead_array},
			 true, init_value,
			 PtiUdpEntryArray{mAlloc, entry_array}};
  return node;
}

// @brief combinational UDP 用のテーブルエントリの生成
const PtUdpEntry*
SptFactory::new_UdpEntry(
  const FileRegion& file_region,
  const vector<const PtUdpValue*>& input_array,
  const PtUdpValue* output
)
{
  auto node = new SptUdpEntry{file_region,
			      PtiUdpValueArray{mAlloc, input_array},
			      nullptr,
			      output};
  return node;
}

// @brief sequential UDP 用のテーブルエントリの生成
const PtUdpEntry*
SptFactory::new_UdpEntry(
  const FileRegion& file_region,
  const vector<const PtUdpValue*>& input_array,
  const PtUdpValue* current,
  const PtUdpValue* output
)
{
  auto node = new SptUdpEntry{file_region,
			      PtiUdpValueArray{mAlloc, input_array},
			      current,
			      output};
  return node;
}

// @brief UDP のテーブルエントリの要素の値の生成
const PtUdpValue*
SptFactory::new_UdpValue(
  const FileRegion& file_region,
  char symbol
)
{
  auto node = new SptUdpValue{file_region, symbol};
  return node;
}

// @brief UDP のテーブルエントリの要素の値の生成
const PtUdpValue*
SptFactory::new_UdpValue(
  const FileRegion& file_region,
  char symbol1,
  char symbol2
)
{
  auto node = new SptUdpValue{file_region, symbol1, symbol2};
  return node;
}

END_NAMESPACE_YM_VERILOG
