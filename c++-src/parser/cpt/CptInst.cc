
/// @file CptInst.cc
/// @brief インスタンス系オブジェクトの実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "CptInst.h"
#include "parser/CptFactory.h"


BEGIN_NAMESPACE_YM_VERILOG


//////////////////////////////////////////////////////////////////////
// gate instance のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptGateH::CptGateH(const FileRegion& file_region,
		   VpiPrimType prim_type,
		   PtInstArray inst_array) :
  mFileRegion(file_region),
  mPrimType(prim_type),
  mInstArray(inst_array)
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

// @brief module/UDP/gate instance リストの取得
PtInstArray
CptGateH::inst_list() const
{
  return mInstArray;
}


//////////////////////////////////////////////////////////////////////
// strength をもつ gate instance のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptGateHS::CptGateHS(const FileRegion& file_region,
		     VpiPrimType prim_type,
		     const PtStrength* strength,
		     PtInstArray inst_array) :
  CptGateH(file_region, prim_type, inst_array),
  mStrength(strength)
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
CptGateHD::CptGateHD(const FileRegion& file_region,
		     VpiPrimType prim_type,
		     const PtDelay* delay,
		     PtInstArray inst_array) :
  CptGateH(file_region, prim_type, inst_array),
  mDelay(delay)
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
CptGateHSD::CptGateHSD(const FileRegion& file_region,
		       VpiPrimType prim_type,
		       const PtStrength* strength,
		       const PtDelay* delay,
		       PtInstArray inst_array) :
  CptGateH(file_region, prim_type, inst_array),
  mStrength(strength),
  mDelay(delay)
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
CptMuH::CptMuH(const FileRegion& file_region,
	       const char* def_name,
	       PtInstArray inst_array) :
  mFileRegion(file_region),
  mName(def_name),
  mInstArray(inst_array)
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

// @brief module/UDP/gate instance リストの取得
PtInstArray
CptMuH::inst_list() const
{
  return mInstArray;
}


//////////////////////////////////////////////////////////////////////
// パラメータ割り当てつきの CptMuH
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptMuHP::CptMuHP(const FileRegion& file_region,
		 const char* def_name,
		 PtConnectionArray con_array,
		 PtInstArray inst_array) :
  CptMuH(file_region, def_name, inst_array),
  mParamAssignArray(con_array)
{
}

// デストラクタ
CptMuHP::~CptMuHP()
{
}

// @brief パラメータ割り当てリストの取得
PtConnectionArray
CptMuHP::paramassign_array() const
{
  return mParamAssignArray;
}


//////////////////////////////////////////////////////////////////////
// strength を持つ MuH
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptMuHS::CptMuHS(const FileRegion& file_region,
		 const char* def_name,
		 const PtStrength* strength,
		 PtInstArray inst_array) :
  CptMuH(file_region, def_name, inst_array),
  mStrength(strength)
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
CptMuHD::CptMuHD(const FileRegion& file_region,
		 const char* def_name,
		 const PtDelay* delay,
		 PtInstArray inst_array) :
  CptMuH(file_region, def_name, inst_array),
  mDelay(delay)
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
CptMuHSD::CptMuHSD(const FileRegion& file_region,
		   const char* def_name,
		   const PtStrength* strength,
		   const PtDelay* delay,
		   PtInstArray inst_array) :
  CptMuH(file_region, def_name, inst_array),
  mStrength(strength),
  mDelay(delay)
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
CptInst::CptInst(const FileRegion& file_region,
		 PtConnectionArray con_array) :
  mFileRegion(file_region),
  mPortArray(con_array)
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
// @return 名前
// このクラスでは nullptr を返す．
const char*
CptInst::name() const
{
  return nullptr;
}

// @brief 範囲の左側の式の取得
// @return 範囲の左側の式
// このクラスでは nullptr を返す．
const PtExpr*
CptInst::left_range() const
{
  return nullptr;
}

// @brief 範囲の右側の式の取得
// @return 範囲の右側の式
// このクラスでは nullptr を返す．
const PtExpr*
CptInst::right_range() const
{
  return nullptr;
}

// @brief ポートのリストの取得
PtConnectionArray
CptInst::port_list() const
{
  return mPortArray;
}


//////////////////////////////////////////////////////////////////////
// 名前を持つ CptInst
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptInstN::CptInstN(const FileRegion& file_region,
		   const char* name,
		   PtConnectionArray con_array) :
  CptInst(file_region, con_array),
  mName(name)
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
CptInstR::CptInstR(const FileRegion& file_region,
		       const char* name,
		       const PtExpr* left,
		       const PtExpr* right,
		       PtConnectionArray con_array) :
  CptInstN(file_region, name, con_array),
  mLeftRange(left),
  mRightRange(right)
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
CptFactory::new_GateH(const FileRegion& file_region,
		      VpiPrimType type,
		      PtInstArray inst_array)
{
  ++ mNumGateH;
  auto obj = new CptGateH(file_region, type, inst_array);
  return obj;
}

// gate instance 文のヘッダを生成する．
const PtItem*
CptFactory::new_GateH(const FileRegion& file_region,
		      VpiPrimType type,
		      const PtStrength* strength,
		      PtInstArray inst_array)
{
  ++ mNumGateHS;
  auto obj = new CptGateHS(file_region, type, strength, inst_array);
  return obj;
}

// gate instance 文のヘッダを生成する．
const PtItem*
CptFactory::new_GateH(const FileRegion& file_region,
		      VpiPrimType type,
		      const PtDelay* delay,
		      PtInstArray inst_array)
{
  ++ mNumGateHD;
  auto obj = new CptGateHD(file_region, type, delay, inst_array);
  return obj;
}

// gate instance 文のヘッダを生成する．
const PtItem*
CptFactory::new_GateH(const FileRegion& file_region,
		      VpiPrimType type,
		      const PtStrength* strength,
		      const PtDelay* delay,
		      PtInstArray inst_array)
{
  ++ mNumGateHSD;
  auto obj = new CptGateHSD(file_region,
			    type, strength, delay, inst_array);
  return obj;
}

// module instance/UDP instance 文のヘッダを生成する．
const PtItem*
CptFactory::new_MuH(const FileRegion& file_region,
		    const char* def_name,
		    PtInstArray inst_array)
{
  ++ mNumMuH;
  auto obj = new CptMuH(file_region, def_name,
			inst_array);
  return obj;
}

// module instance/UDP instance 文のヘッダを生成する．
const PtItem*
CptFactory::new_MuH(const FileRegion& file_region,
		    const char* def_name,
		    const PtStrength* strength,
		    PtInstArray inst_array)
{
  ++ mNumMuHS;
  auto obj = new CptMuHS(file_region, def_name,
			 strength, inst_array);
  return obj;
}

// module instance/UDP instance 文のヘッダを生成する．
const PtItem*
CptFactory::new_MuH(const FileRegion& file_region,
		    const char* def_name,
		    const PtDelay* delay,
		    PtInstArray inst_array)
{
  ++ mNumMuHD;
  auto obj = new CptMuHD(file_region, def_name,
			 delay, inst_array);
  return obj;
}

// module instance/UDP instance 文のヘッダを生成する．
const PtItem*
CptFactory::new_MuH(const FileRegion& file_region,
		    const char* def_name,
		    const PtStrength* strength,
		    const PtDelay* delay,
		    PtInstArray inst_array)
{
  ++ mNumMuHSD;
  auto obj = new CptMuHSD(file_region, def_name,
			  strength, delay, inst_array);
  return obj;
}

// module instance/UDP instance 文のヘッダを生成する．
const PtItem*
CptFactory::new_MuH(const FileRegion& file_region,
		    const char* def_name,
		    PtConnectionArray con_array,
		    PtInstArray inst_array)
{
  ++ mNumMuHP;
  auto obj = new CptMuHP(file_region, def_name,
			 con_array, inst_array);
  return obj;
}

// module instance/UDP instance の要素を生成する．
const PtInst*
CptFactory::new_Inst(const FileRegion& file_region,
		     PtConnectionArray con_array)
{
  ++ mNumInst;
  auto obj = new CptInst(file_region, con_array);
  return obj;
}

// module instance/UDP instance の要素を生成する．
const PtInst*
CptFactory::new_InstN(const FileRegion& file_region,
		      const char* name,
		      PtConnectionArray con_array)
{
  ++ mNumInstN;
  auto obj = new CptInstN(file_region, name, con_array);
  return obj;
}

// module instance/UDP instance の要素を生成する．
const PtInst*
CptFactory::new_InstV(const FileRegion& file_region,
		      const char* name,
		      const PtExpr* left,
		      const PtExpr* right,
		      PtConnectionArray con_array)
{
  ++ mNumInstR;
  auto obj = new CptInstR(file_region, name, left, right, con_array);
  return obj;
}

END_NAMESPACE_YM_VERILOG
