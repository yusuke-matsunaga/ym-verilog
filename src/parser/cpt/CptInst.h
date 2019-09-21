﻿#ifndef CPTINST_H
#define CPTINST_H

/// @file CptInst.h
/// @brief CptInst のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "CptItem.h"



BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @brief gate header のベース実装クラス
//////////////////////////////////////////////////////////////////////
class CptGateH :
  public CptItem
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  CptGateH(const FileRegion& file_region,
	   tVpiPrimType prim_type,
	   PtInstArray inst_array);

  /// @brief デストラクタ
  virtual
  ~CptGateH();


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  virtual
  FileRegion
  file_region() const;

  /// @brief 型を返す．
  /// @note ここでは kGateInst を返す．
  virtual
  tPtItemType
  type() const;

  /// @brief プリミティブタイプを返す．
  virtual
  tVpiPrimType
  prim_type() const;

  /// @brief 要素数を取り出す．
  virtual
  int
  size() const;

  /// @brief module/UDP/gate instance 要素の取得
  /// @param[in] pos 位置番号 ( 0 <= pos < size() )
  virtual
  const PtInst*
  inst(int pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // プリミティブタイプ
  tVpiPrimType mPrimType;

  // 要素の配列
  PtInstArray mInstArray;

};


//////////////////////////////////////////////////////////////////////
/// @brief strength をもつ gate instance のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////
class CptGateHS :
  public CptGateH
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  CptGateHS(const FileRegion& file_region,
	    tVpiPrimType prim_type,
	    const PtStrength* strength,
	    PtInstArray inst_array);

  /// @brief デストラクタ
  virtual
  ~CptGateHS();


public:
  //////////////////////////////////////////////////////////////////////
  // PtGateH の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief strength を返す．
  virtual
  const PtStrength*
  strength() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // strength
  const PtStrength* mStrength;

};


//////////////////////////////////////////////////////////////////////
/// @brief delay をもつ gate instance のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////
class CptGateHD :
  public CptGateH
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  CptGateHD(const FileRegion& file_region,
	    tVpiPrimType prim_type,
	    const PtDelay* delay,
	    PtInstArray inst_array);

  /// @brief デストラクタ
  virtual
  ~CptGateHD();


public:
  //////////////////////////////////////////////////////////////////////
  // PtGateH の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief delay を返す．
  virtual
  const PtDelay*
  delay() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // delay
  const PtDelay* mDelay;

};


//////////////////////////////////////////////////////////////////////
/// @brief strength と delay をもつ gate instance のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////
class CptGateHSD :
  public CptGateH
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  CptGateHSD(const FileRegion& file_region,
	     tVpiPrimType prim_type,
	     const PtStrength* strength,
	     const PtDelay* delay,
	     PtInstArray inst_array);

  /// @brief デストラクタ
  virtual
  ~CptGateHSD();


public:
  //////////////////////////////////////////////////////////////////////
  // PtGateH の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief strength を返す．
  virtual
  const PtStrength*
  strength() const;

  /// @brief delay を返す．
  virtual
  const PtDelay*
  delay() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // strength
  const PtStrength* mStrength;

  // delay
  const PtDelay* mDelay;

};


//////////////////////////////////////////////////////////////////////
/// @brief module/udp header のベース実装クラス
//////////////////////////////////////////////////////////////////////
class CptMuH :
  public CptItem
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  CptMuH(const FileRegion& file_region,
	 const char* def_name,
	 PtInstArray inst_array);

  /// @brief デストラクタ
  virtual
  ~CptMuH();


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  virtual
  FileRegion
  file_region() const;

  /// @brief 型を返す．
  /// @note ここでは kPtItem_MuInst を返す
  virtual
  tPtItemType
  type() const;

  /// @brief 定義名を返す．
  virtual
  const char*
  name() const;

  /// @brief 要素数を取り出す．
  virtual
  int
  size() const;

  /// @brief module/UDP/gate instance 要素の取得
  /// @param[in] pos 位置番号 ( 0 <= pos < size() )
  virtual
  const PtInst*
  inst(int pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 定義名
  const char* mName;

  // 要素の配列
  PtInstArray mInstArray;

};


//////////////////////////////////////////////////////////////////////
/// @brief パラメータ割り当てつきの CptMuH
//////////////////////////////////////////////////////////////////////
class CptMuHP :
  public CptMuH
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  CptMuHP(const FileRegion& file_region,
	  const char* def_name,
	  PtConnectionArray con_array,
	  PtInstArray inst_array);

  /// @brief デストラクタ
  virtual
  ~CptMuHP();


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief パラメータ割り当てリストの取得
  virtual
  PtConnectionArray
  paramassign_array() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // パラメータ割り当ての配列
  PtConnectionArray mParamAssignArray;

};


//////////////////////////////////////////////////////////////////////
/// @brief strength を持つ MuH
//////////////////////////////////////////////////////////////////////
class CptMuHS :
  public CptMuH
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  CptMuHS(const FileRegion& file_region,
	  const char* def_name,
	  const PtStrength* strength,
	  PtInstArray inst_array);

  /// @brief デストラクタ
  virtual
  ~CptMuHS();


public:
  //////////////////////////////////////////////////////////////////////
  // PtMuH の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief strength を返す．
  virtual
  const PtStrength*
  strength() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // strength
  const PtStrength* mStrength;

};


//////////////////////////////////////////////////////////////////////
/// @brief delay を持つ MuH
//////////////////////////////////////////////////////////////////////
class CptMuHD :
  public CptMuH
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  CptMuHD(const FileRegion& file_region,
	  const char* def_name,
	  const PtDelay* delay,
	  PtInstArray inst_array);

  /// @brief デストラクタ
  virtual
  ~CptMuHD();


public:
  //////////////////////////////////////////////////////////////////////
  // PtMuH の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief delay を返す．
  virtual
  const PtDelay*
  delay() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // delay
  const PtDelay* mDelay;

};


//////////////////////////////////////////////////////////////////////
/// @brief strength と delay を持つ MuH
//////////////////////////////////////////////////////////////////////
class CptMuHSD :
  public CptMuH
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  CptMuHSD(const FileRegion& file_region,
	   const char* def_name,
	   const PtStrength* strength,
	   const PtDelay* delay,
	   PtInstArray inst_array);

  /// @brief デストラクタ
  virtual
  ~CptMuHSD();


public:
  //////////////////////////////////////////////////////////////////////
  // PtMuH の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief strength を返す．
  virtual
  const PtStrength*
  strength() const;

  /// @brief delay を返す．
  virtual
  const PtDelay*
  delay() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // strength
  const PtStrength* mStrength;

  // delay
  const PtDelay* mDelay;

};


//////////////////////////////////////////////////////////////////////
/// @brief module instance/UDP/gate instance のベース実装クラス
//////////////////////////////////////////////////////////////////////
class CptInst :
  public PtInst
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  CptInst(const FileRegion& file_region,
	  PtConnectionArray con_array);

  /// @brief デストラクタ
  virtual
  ~CptInst();


public:
  //////////////////////////////////////////////////////////////////////
  // PtMuInst の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  virtual
  FileRegion
  file_region() const;

  /// @brief 名前の取得
  /// @return 名前
  /// @note このクラスでは nullptr を返す．
  virtual
  const char*
  name() const;

  /// @brief 範囲の左側の式の取得
  /// @return 範囲の左側の式
  /// @note このクラスでは nullptr を返す．
  virtual
  const PtExpr*
  left_range() const;

  /// @brief 範囲の右側の式の取得
  /// @return 範囲の右側の式
  /// @note このクラスでは nullptr を返す．
  virtual
  const PtExpr*
  right_range() const;

  /// @brief ポート数の取得
  /// @return ポート数
  virtual
  int
  port_num() const;

  /// @brief ポートの取得
  /// @param[in] pos 位置番号 ( 0 <= pos < port_num() )
  virtual
  const PtConnection*
  port(int pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // ポート割り当ての配列
  PtConnectionArray mPortArray;

};


//////////////////////////////////////////////////////////////////////
/// @brief 名前を持つ CptInst
//////////////////////////////////////////////////////////////////////
class CptInstN :
  public CptInst
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  CptInstN(const FileRegion& file_region,
	   const char* name,
	   PtConnectionArray con_array);

  /// @brief デストラクタ
  virtual
  ~CptInstN();


public:
  //////////////////////////////////////////////////////////////////////
  // PtInst の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を返す．
  virtual
  const char*
  name() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  const char* mName;

};


//////////////////////////////////////////////////////////////////////
/// @brief 名前と範囲指定を持つ CptInst
//////////////////////////////////////////////////////////////////////
class CptInstR :
  public CptInstN
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  CptInstR(const FileRegion& file_region,
	   const char* name,
	   const PtExpr* left,
	   const PtExpr* right,
	   PtConnectionArray con_array);

  /// @brief デストラクタ
  virtual
  ~CptInstR();


public:
  //////////////////////////////////////////////////////////////////////
  // PtInst の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief range の MSB を取出す．
  virtual
  const PtExpr*
  left_range() const;

  /// @brief range の LSB を取出す．
  virtual
  const PtExpr*
  right_range() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 範囲のMSB
  const PtExpr* mLeftRange;

  // 範囲のLSB
  const PtExpr* mRightRange;

};

END_NAMESPACE_YM_VERILOG

#endif // CPTINST_H
