﻿#ifndef ELBPARAMCON_H
#define ELBPARAMCON_H

/// @file ElbParamCon.h
/// @brief ElbParamCon のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym/ym_verilog.h"
#include "ym/FileRegion.h"
#include "ym/pt/PtMisc.h"
#include "ym/VlValue.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ElbParamCon ElbParamCon.h "ElbParamCon.h"
/// @brief インスタンス化で用いる stub クラス
//////////////////////////////////////////////////////////////////////
class ElbParamCon
{
public:

  /// @brief コンストラクタ
  /// @param[in] file_region ファイル位置
  /// @param[in] num 要素数
  /// @param[in] named_con 名前による割り当ての時 true となるフラグ
  ElbParamCon(const FileRegion& file_region,
	      ymuint num,
	      bool named_con);

  /// @brief 仮想デストラクタ
  virtual
  ~ElbParamCon();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部からアクセスするための関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  const FileRegion&
  file_region() const;

  /// @brief 名前による割り当ての時 true を返す．
  bool
  named_con() const;

  /// @brief 要素数を返す．
  ymuint
  elem_num() const;

  /// @brief pos 番目の要素に対応するパース木の要素を返す．
  /// @param[in] pos 位置
  const PtConnection*
  pt_con(ymuint pos) const;

  /// @brief pos 番目の要素の名前を返す．
  /// @param[in] pos 位置
  const char*
  name(ymuint pos) const;

  /// @brief pos 番目の要素の式を返す．
  /// @param[in] pos 位置
  const PtExpr*
  expr(ymuint pos) const;

  /// @brief pos 番目の要素の値を返す．
  /// @param[in] pos 位置
  VlValue
  value(ymuint pos) const;

  /// @brief 名前と値を設定する．
  void
  set(ymuint pos,
      const PtConnection* pt_con,
      const PtExpr* expr,
      VlValue value);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で使うデータ構造
  //////////////////////////////////////////////////////////////////////

  struct Unit {

    // パース木の要素
    const PtConnection* mPtCon;

    // 式
    const PtExpr* mExpr;

    // 値
    VlValue mValue;

  };


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 名前による割り当ての時 true となるフラグ
  bool mNamedCon;

  // 要素のベクタ
  vector<Unit> mList;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] file_region ファイル位置
// @param[in] num 要素数
/// @param[in] named_con 名前による割り当ての時 true となるフラグ
inline
ElbParamCon::ElbParamCon(const FileRegion& file_region,
			 ymuint num,
			 bool named_con) :
  mFileRegion(file_region),
  mNamedCon(named_con),
  mList(num)
{
}

// @brief 仮想デストラクタ
inline
ElbParamCon::~ElbParamCon()
{
}

// @brief ファイル位置を返す．
inline
const FileRegion&
ElbParamCon::file_region() const
{
  return mFileRegion;
}

// @brief 名前による割り当ての時 true を返す．
inline
bool
ElbParamCon::named_con() const
{
  return mNamedCon;
}

// @brief 要素数を返す．
inline
ymuint
ElbParamCon::elem_num() const
{
  return mList.size();
}

// @brief pos 番目の要素に対応するパース木の要素を返す．
// @param[in] pos 位置
inline
const PtConnection*
ElbParamCon::pt_con(ymuint pos) const
{
  return mList[pos].mPtCon;
}

// @brief 名前を返す．
inline
const char*
ElbParamCon::name(ymuint pos) const
{
  return mList[pos].mPtCon->name();
}

// @brief 式を返す．
inline
const PtExpr*
ElbParamCon::expr(ymuint pos) const
{
  return mList[pos].mExpr;
}

// @brief 値を返す．
inline
VlValue
ElbParamCon::value(ymuint pos) const
{
  return mList[pos].mValue;
}

// @brief 名前と値を設定する．
inline
void
ElbParamCon::set(ymuint pos,
		 const PtConnection* pt_con,
		 const PtExpr* expr,
		 VlValue value)
{
  Unit& unit = mList[pos];
  unit.mPtCon = pt_con;
  unit.mExpr = expr;
  unit.mValue = value;
}

END_NAMESPACE_YM_VERILOG

#endif // ELBPARAMCON_H
