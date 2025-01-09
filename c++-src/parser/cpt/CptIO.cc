
/// @file CptIO.cc
/// @brief CptIO の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.

#include "CptIO.h"
#include "alloc/Alloc.h"
#include "parser/CptFactory.h"
#include "ym/pt/PtExpr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// IO宣言のヘッダのベース実装クラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CptIOHBase::CptIOHBase(
  const FileRegion& file_region,
  VpiDir dir,
  VpiAuxType aux_type,
  VpiNetType net_type,
  VpiVarType var_type,
  bool sign
) : mFileRegion{file_region}
{
  mAttr =
    static_cast<unsigned int>(sign) |
    (static_cast<unsigned int>(dir) << 1) |
    (static_cast<unsigned int>(aux_type) << 8) |
    (static_cast<unsigned int>(net_type) << 16) |
    (static_cast<unsigned int>(var_type) << 24);
}

// @brief デストラクタ
CptIOHBase::~CptIOHBase()
{
}

// @brief ファイル位置の取得
FileRegion
CptIOHBase::file_region() const
{
  return mFileRegion;
}

// @brief 型の取得
VpiDir
CptIOHBase::direction() const
{
  return static_cast<VpiDir>((mAttr & 0xff) >> 1);
}

// @brief 補助的な型の取得
VpiAuxType
CptIOHBase::aux_type() const
{
  return static_cast<VpiAuxType>((mAttr >> 8) & 0xff);
}

// @bief 補助的なネット型の取得
VpiNetType
CptIOHBase::net_type() const
{
  return static_cast<VpiNetType>((mAttr >> 16) & 0xff);
}

// @brief 補助的な変数型の取得
VpiVarType
CptIOHBase::var_type() const
{
  return static_cast<VpiVarType>((mAttr >> 24) & 0xff);
}

// @brief 符号の取得
bool
CptIOHBase::is_signed() const
{
  return static_cast<bool>(mAttr & 1);
}

// @brief 範囲のMSBの取得
const PtExpr*
CptIOHBase::left_range() const
{
  return nullptr;
}

// @brief 範囲のLSBの取得
const PtExpr*
CptIOHBase::right_range() const
{
  return nullptr;
}

// @brief 要素数の取得
SizeType
CptIOHBase::item_num() const
{
  return mItemArray.size();
}

// @brief 要素の取得
const PtIOItem*
CptIOHBase::item(
  SizeType pos
) const
{
  return mItemArray[pos];
}

// @brief 要素リストの設定
void
CptIOHBase::set_elem(
  PtiIOItemArray&& elem_array
)
{
  mItemArray = std::move(elem_array);
}


//////////////////////////////////////////////////////////////////////
// 範囲を持たない IO宣言のヘッダ
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CptIOH::CptIOH(
  const FileRegion& file_region,
  VpiDir dir,
  VpiAuxType aux_type,
  VpiNetType net_type,
  VpiVarType var_type,
  bool sign
) : CptIOHBase{file_region,
	       dir, aux_type,
	       net_type, var_type,
	       sign}
{
}

// @brief デストラクタ
CptIOH::~CptIOH()
{
}


//////////////////////////////////////////////////////////////////////
// ビットベクタ型のIO宣言のヘッダ
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CptIOHV::CptIOHV(
  const FileRegion& file_region,
  VpiDir dir,
  VpiAuxType aux_type,
  VpiNetType net_type,
  bool sign,
  const PtExpr* left,
  const PtExpr* right
) : CptIOHBase{file_region,
	       dir, aux_type,
	       net_type, VpiVarType::None,
	       sign},
    mLeftRange{left},
    mRightRange{right}
{
}

// @brief デストラクタ
CptIOHV::~CptIOHV()
{
}

// @brief 範囲のMSBの取得
const PtExpr*
CptIOHV::left_range() const
{
  return mLeftRange;
}

// @brief 範囲のLSBの取得
const PtExpr*
CptIOHV::right_range() const
{
  return mRightRange;
}


//////////////////////////////////////////////////////////////////////
// IO宣言要素を表すクラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CptIOItem::CptIOItem(
  const FileRegion& file_region,
  const char* name
) : mLoc{file_region},
    mName{name}
{
}

// @brief デストラクタ
CptIOItem::~CptIOItem()
{
}

// @brief ファイル位置の取得
FileRegion
CptIOItem::file_region() const
{
  return mLoc;
}

// @brief 名前の取得
const char*
CptIOItem::name() const
{
  return mName;
}

// @brief 初期値の取得
const PtExpr*
CptIOItem::init_value() const
{
  return nullptr;
}


//////////////////////////////////////////////////////////////////////
// 初期値をもった IO宣言要素の基底クラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CptIOItemI::CptIOItemI(
  const FileRegion& file_region,
  const char* name,
  const PtExpr* init_value
) : CptIOItem{file_region, name},
    mInitValue{init_value}
{
  ASSERT_COND( init_value );
}

// @brief デストラクタ
CptIOItemI::~CptIOItemI()
{
}

// @brief ファイル位置の取得
FileRegion
CptIOItemI::file_region() const
{
  return FileRegion(CptIOItem::file_region(), mInitValue->file_region());
}

// @brief 初期値の取得
const PtExpr*
CptIOItemI::init_value() const
{
  return mInitValue;
}


//////////////////////////////////////////////////////////////////////
// IO 宣言関係
//////////////////////////////////////////////////////////////////////

// @brief 範囲付きの IO 宣言のヘッダの生成
PtiIOHead*
CptFactory::new_IOHead(
  const FileRegion& file_region,
  VpiDir dir,
  bool sign,
  const PtExpr* left,
  const PtExpr* right
)
{
  if ( left == nullptr ) {
    ASSERT_COND( right == nullptr );
    ++ mNumIOH;
    void* p = mAlloc.get_memory(sizeof(CptIOH));
    auto obj = new (p) CptIOH{file_region, dir, VpiAuxType::None,
			      VpiNetType::None, VpiVarType::None, sign};
    return obj;
  }
  else {
    ASSERT_COND( right != nullptr );
    ++ mNumIOHV;
    void* p = mAlloc.get_memory(sizeof(CptIOHV));
    auto obj = new (p) CptIOHV{file_region, dir, VpiAuxType::None,
			       VpiNetType::None,
			       sign, left, right};
    return obj;
  }
}

// @brief 範囲付きの IO 宣言のヘッダの生成 (reg 型)
PtiIOHead*
CptFactory::new_RegIOHead(
  const FileRegion& file_region,
  VpiDir dir,
  bool sign,
  const PtExpr* left,
  const PtExpr* right
)
{
  if ( left == nullptr ) {
    ASSERT_COND( right == nullptr );
    ++ mNumIOH;
    void* p = mAlloc.get_memory(sizeof(CptIOH));
    auto obj = new (p) CptIOH{file_region, dir, VpiAuxType::Reg,
			      VpiNetType::None, VpiVarType::None, sign};
    return obj;
  }
  else {
    ++ mNumIOHV;
    void* p = mAlloc.get_memory(sizeof(CptIOHV));
    auto obj = new (p) CptIOHV{file_region, dir, VpiAuxType::Reg,
			       VpiNetType::None,
			       sign, left, right};
    return obj;
  }
}

// @brief 範囲付きの IO 宣言のヘッダの生成 (ネット型)
PtiIOHead*
CptFactory::new_NetIOHead(
  const FileRegion& file_region,
  VpiDir dir,
  VpiNetType net_type,
  bool sign,
  const PtExpr* left,
  const PtExpr* right
)
{
  if ( left == nullptr ) {
    ASSERT_COND( right == nullptr );
    ++ mNumIOH;
    void* p = mAlloc.get_memory(sizeof(CptIOH));
    auto obj = new (p) CptIOH{file_region, dir, VpiAuxType::Net,
			      net_type, VpiVarType::None, sign};
    return obj;
  }
  else {
    ++ mNumIOHV;
    void* p = mAlloc.get_memory(sizeof(CptIOHV));
    auto obj = new (p) CptIOHV{file_region, dir, VpiAuxType::Net,
			       net_type,
			       sign, left, right};
    return obj;
  }
}

// @brief IO 宣言のヘッダの生成 (変数型)
PtiIOHead*
CptFactory::new_VarIOHead(
  const FileRegion& file_region,
  VpiDir dir,
  VpiVarType var_type
)
{
  ++ mNumIOH;
  void* p = mAlloc.get_memory(sizeof(CptIOH));
  auto obj = new (p) CptIOH{file_region, dir, VpiAuxType::Var,
			    VpiNetType::None, var_type, false};
  return obj;
}

// @brief 初期値付き IO 宣言の要素の生成
PtIOItem*
CptFactory::new_IOItem(
  const FileRegion& file_region,
  const char* name,
  const PtExpr* init_value
)
{
  if ( init_value == nullptr ) {
    ++ mNumIOItem;
    void* p = mAlloc.get_memory(sizeof(CptIOItem));
    auto obj = new (p) CptIOItem{file_region, name};
    return obj;
  }
  else {
    ++ mNumIOItemI;
    void* p = mAlloc.get_memory(sizeof(CptIOItemI));
    auto obj = new (p) CptIOItemI{file_region, name, init_value};
    return obj;
  }
}

END_NAMESPACE_YM_VERILOG
