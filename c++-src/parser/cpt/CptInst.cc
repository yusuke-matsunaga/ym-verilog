
/// @file CptInst.cc
/// @brief インスタンス系オブジェクトの実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.

#include "CptInst.h"
#include "parser/CptFactory.h"
#include "parser/PtiArray.h"


BEGIN_NAMESPACE_YM_VERILOG


//////////////////////////////////////////////////////////////////////
// gate instance のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptGateH::CptGateH(
  const FileRegion& file_region,
  VpiPrimType prim_type,
  PtiInstArray&& inst_array
) : mFileRegion{file_region},
    mPrimType{prim_type},
    mInstArray{move(inst_array)}
{
}

// デストラクタ
CptGateH::~CptGateH()
{
}

// ファイル位置を返す．
FileRegion
CptGateH::file_region() const
{
  return mFileRegion;
}

// 型を返す．
PtItemType
CptGateH::type() const
{
  return PtItemType::GateInst;
}

// プリミティブタイプ(vpiAndPrim など)を返す．
VpiPrimType
CptGateH::prim_type() const
{
  return mPrimType;
}

// @brief module/UDP/gate instance の要素数の取得
SizeType
CptGateH::inst_num() const
{
  return mInstArray.size();
}

// @brief module/UDP/gate instance の取得
const PtInst*
CptGateH::inst(
  SizeType pos
) const
{
  return mInstArray[pos];
}


//////////////////////////////////////////////////////////////////////
// strength をもつ gate instance のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptGateHS::CptGateHS(
  const FileRegion& file_region,
  VpiPrimType prim_type,
  const PtStrength* strength,
  PtiInstArray&& inst_array
) : CptGateH{file_region, prim_type, move(inst_array)},
    mStrength{strength}
{
}

// デストラクタ
CptGateHS::~CptGateHS()
{
}

// strength を返す．
const PtStrength*
CptGateHS::strength() const
{
  return mStrength;
}


//////////////////////////////////////////////////////////////////////
// delay をもつ gate instance のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptGateHD::CptGateHD(
  const FileRegion& file_region,
  VpiPrimType prim_type,
  const PtDelay* delay,
  PtiInstArray&& inst_array
) : CptGateH{file_region, prim_type, move(inst_array)},
    mDelay{delay}
{
}

// デストラクタ
CptGateHD::~CptGateHD()
{
}

// delay を返す．
const PtDelay*
CptGateHD::delay() const
{
  return mDelay;
}


//////////////////////////////////////////////////////////////////////
// strength と delay をもつ gate instance のヘッダを表すクラス      //
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptGateHSD::CptGateHSD(
  const FileRegion& file_region,
  VpiPrimType prim_type,
  const PtStrength* strength,
  const PtDelay* delay,
  PtiInstArray&& inst_array
) : CptGateH{file_region, prim_type, move(inst_array)},
    mStrength{strength},
    mDelay{delay}
{
}

// デストラクタ
CptGateHSD::~CptGateHSD()
{
}

// strength を返す．
const PtStrength*
CptGateHSD::strength() const
{
  return mStrength;
}

// delay を返す．
const PtDelay*
CptGateHSD::delay() const
{
  return mDelay;
}


//////////////////////////////////////////////////////////////////////
// module instance/UDP instance のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptMuH::CptMuH(
  const FileRegion& file_region,
  const char* def_name,
  PtiInstArray&& inst_array
) : mFileRegion{file_region},
    mName{def_name},
    mInstArray{move(inst_array)}
{
}

// デストラクタ
CptMuH::~CptMuH()
{
}

// ファイル位置を返す．
FileRegion
CptMuH::file_region() const
{
  return mFileRegion;
}

// 型を返す．
PtItemType
CptMuH::type() const
{
  return PtItemType::MuInst;
}

// 定義名を返す．
const char*
CptMuH::name() const
{
  return mName;
}

// @brief module/UDP/gate instance の要素数の取得
SizeType
CptMuH::inst_num() const
{
  return mInstArray.size();
}

// @brief module/UDP/gate instance の取得
const PtInst*
CptMuH::inst(
  SizeType pos
) const
{
  return mInstArray[pos];
}


//////////////////////////////////////////////////////////////////////
// パラメータ割り当てつきの CptMuH
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptMuHP::CptMuHP(
  const FileRegion& file_region,
  const char* def_name,
  PtiConnectionArray&& con_array,
  PtiInstArray&& inst_array
) : CptMuH{file_region, def_name, move(inst_array)},
    mParamAssignArray{move(con_array)}
{
}

// デストラクタ
CptMuHP::~CptMuHP()
{
}

// @brief パラメータ割り当て数の取得
SizeType
CptMuHP::paramassign_num() const
{
  return mParamAssignArray.size();
}

// @brief パラメータ割り当ての取得
const PtConnection*
CptMuHP::paramassign(
  SizeType pos
) const
{
  return mParamAssignArray[pos];
}


//////////////////////////////////////////////////////////////////////
// strength を持つ MuH
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptMuHS::CptMuHS(
  const FileRegion& file_region,
  const char* def_name,
  const PtStrength* strength,
  PtiInstArray&& inst_array
) : CptMuH{file_region, def_name, move(inst_array)},
    mStrength{strength}
{
}

// デストラクタ
CptMuHS::~CptMuHS()
{
}

// strength を返す．
const PtStrength*
CptMuHS::strength() const
{
  return mStrength;
}


//////////////////////////////////////////////////////////////////////
// delay を持つ MuH
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptMuHD::CptMuHD(
  const FileRegion& file_region,
  const char* def_name,
  const PtDelay* delay,
  PtiInstArray&& inst_array
) : CptMuH{file_region, def_name, move(inst_array)},
    mDelay{delay}
{
}

// デストラクタ
CptMuHD::~CptMuHD()
{
}

// delay を返す．
const PtDelay*
CptMuHD::delay() const
{
  return mDelay;
}


//////////////////////////////////////////////////////////////////////
// strength と delay を持つ MuH
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptMuHSD::CptMuHSD(
  const FileRegion& file_region,
  const char* def_name,
  const PtStrength* strength,
  const PtDelay* delay,
  PtiInstArray&& inst_array
) : CptMuH{file_region, def_name, move(inst_array)},
    mStrength{strength},
    mDelay{delay}
{
}

// デストラクタ
CptMuHSD::~CptMuHSD()
{
}

// strength を返す．
const PtStrength*
CptMuHSD::strength() const
{
  return mStrength;
}

// delay を返す．
const PtDelay*
CptMuHSD::delay() const
{
  return mDelay;
}


//////////////////////////////////////////////////////////////////////
// module instance/UDP instance のベース実装クラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptInst::CptInst(
  const FileRegion& file_region,
  PtiConnectionArray&& con_array
) : mFileRegion{file_region},
    mPortArray{move(con_array)}
{
}

// デストラクタ
CptInst::~CptInst()
{
}

// ファイル位置を返す．
FileRegion
CptInst::file_region() const
{
  return mFileRegion;
}

// @brief 名前の取得
const char*
CptInst::name() const
{
  return nullptr;
}

// @brief 範囲の左側の式の取得
const PtExpr*
CptInst::left_range() const
{
  return nullptr;
}

// @brief 範囲の右側の式の取得
const PtExpr*
CptInst::right_range() const
{
  return nullptr;
}

// @brief ポートの要素数の取得
SizeType
CptInst::port_num() const
{
  return mPortArray.size();
}

// @brief ポートの取得
const PtConnection*
CptInst::port(
  SizeType pos
) const
{
  return mPortArray[pos];
}


//////////////////////////////////////////////////////////////////////
// 名前を持つ CptInst
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptInstN::CptInstN(
  const FileRegion& file_region,
  const char* name,
  PtiConnectionArray&& con_array
) : CptInst{file_region, move(con_array)},
    mName{name}
{
}

// デストラクタ
CptInstN::~CptInstN()
{
}

// 名前を返す．
const char*
CptInstN::name() const
{
  return mName;
}


//////////////////////////////////////////////////////////////////////
// 名前と範囲指定を持つ CptInst
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptInstR::CptInstR(
  const FileRegion& file_region,
  const char* name,
  const PtExpr* left,
  const PtExpr* right,
  PtiConnectionArray&& con_array
) : CptInstN{file_region, name, move(con_array)},
    mLeftRange{left},
    mRightRange{right}
{
}

// デストラクタ
CptInstR::~CptInstR()
{
}

// range の MSB を取出す．
const PtExpr*
CptInstR::left_range() const
{
  return mLeftRange;
}

// range の LSB を取出す．
const PtExpr*
CptInstR::right_range() const
{
  return mRightRange;
}


//////////////////////////////////////////////////////////////////////
// インスタンス文
//////////////////////////////////////////////////////////////////////

// gate instance 文のヘッダを生成する．
const PtItem*
CptFactory::new_GateH(
  const FileRegion& file_region,
  VpiPrimType type,
  const PtStrength* strength,
  const PtDelay* delay,
  const vector<const PtInst*>& inst_array
)
{
  if ( strength == nullptr ) {
    if ( delay == nullptr ) {
      ++ mNumGateH;
      void* p = mAlloc.get_memory(sizeof(CptGateH));
      auto obj = new (p) CptGateH{file_region, type,
				  PtiArray<const PtInst>(mAlloc, inst_array)};
      return obj;
    }
    else {
      ++ mNumGateHD;
      void* p = mAlloc.get_memory(sizeof(CptGateHD));
      auto obj = new (p) CptGateHD{file_region, type, delay,
				   PtiArray<const PtInst>(mAlloc, inst_array)};
      return obj;
    }
  }
  else {
    if ( delay == nullptr ) {
      ++ mNumGateHS;
      void* p = mAlloc.get_memory(sizeof(CptGateHS));
      auto obj = new (p) CptGateHS{file_region, type, strength,
				   PtiArray<const PtInst>(mAlloc, inst_array)};
      return obj;
    }
    else {
      ++ mNumGateHSD;
      void* p = mAlloc.get_memory(sizeof(CptGateHSD));
      auto obj = new (p) CptGateHSD{file_region, type, strength, delay,
				    PtiArray<const PtInst>(mAlloc, inst_array)};
      return obj;
    }
  }
}

// module instance/UDP instance 文のヘッダを生成する．
const PtItem*
CptFactory::new_MuH(
  const FileRegion& file_region,
  const char* def_name,
  const PtStrength* strength,
  const PtDelay* delay,
  const vector<const PtInst*>& inst_array
)
{
  if ( strength == nullptr ) {
    if ( delay == nullptr ) {
      ++ mNumMuH;
      void* p = mAlloc.get_memory(sizeof(CptMuH));
      auto obj = new (p) CptMuH{file_region, def_name,
				PtiArray<const PtInst>(mAlloc, inst_array)};
      return obj;
    }
    else {
      ++ mNumMuHS;
      void* p = mAlloc.get_memory(sizeof(CptMuHS));
      auto obj = new (p) CptMuHS{file_region, def_name, strength,
				 PtiArray<const PtInst>(mAlloc, inst_array)};
      return obj;
    }
  }
  else {
    if ( delay == nullptr ) {
      ++ mNumMuHD;
      void* p = mAlloc.get_memory(sizeof(CptMuHD));
      auto obj = new (p) CptMuHD{file_region, def_name, delay,
				 PtiArray<const PtInst>(mAlloc, inst_array)};
      return obj;
    }
    else {
      ++ mNumMuHSD;
      void* p = mAlloc.get_memory(sizeof(CptMuHSD));
      auto obj = new (p) CptMuHSD{file_region, def_name, strength, delay,
				  PtiArray<const PtInst>(mAlloc, inst_array)};
      return obj;
    }
  }
}

// module instance/UDP instance 文のヘッダを生成する．
const PtItem*
CptFactory::new_MuH(
  const FileRegion& file_region,
  const char* def_name,
  const vector<const PtConnection*>& con_array,
  const vector<const PtInst*>& inst_array
)
{
  ++ mNumMuHP;
  void* p = mAlloc.get_memory(sizeof(CptMuHP));
  auto obj = new (p) CptMuHP{file_region, def_name,
			     PtiArray<const PtConnection>(mAlloc, con_array),
			     PtiArray<const PtInst>(mAlloc, inst_array)};
  return obj;
}

// module instance/UDP instance の要素を生成する．
const PtInst*
CptFactory::new_Inst(
  const FileRegion& file_region,
  const char* name,
  const PtExpr* left,
  const PtExpr* right,
  const vector<const PtConnection*>& con_array
)
{
  if ( name == nullptr ) {
    ++ mNumInst;
    void* p = mAlloc.get_memory(sizeof(CptInst));
    auto obj = new (p) CptInst{file_region,
			       PtiArray<const PtConnection>(mAlloc, con_array)};
    return obj;
  }

  if ( left == nullptr ) {
    ASSERT_COND( right == nullptr );
    ++ mNumInstN;
    void* p = mAlloc.get_memory(sizeof(CptInstN));
    auto obj = new (p) CptInstN{file_region, name,
				PtiArray<const PtConnection>(mAlloc, con_array)};
    return obj;
  }

  ASSERT_COND( right != nullptr );
  ++ mNumInstR;
  void* p = mAlloc.get_memory(sizeof(CptInstR));
  auto obj = new (p) CptInstR{file_region, name, left, right,
			      PtiArray<const PtConnection>(mAlloc, con_array)};
  return obj;
}

END_NAMESPACE_YM_VERILOG
