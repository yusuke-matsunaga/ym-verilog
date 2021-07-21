
/// @file CptPrimary.cc
/// @brief CptPrimary の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "CptExpr.h"
#include "parser/CptFactory.h"
#include "parser/PuHierName.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// PtPrimary のベース実装クラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptPrimaryBase::CptPrimaryBase(const char* name) :
  mName(name)
{
}

// デストラクタ
CptPrimaryBase::~CptPrimaryBase()
{
}

// クラスの型を返す．
PtExprType
CptPrimaryBase::type() const
{
  return PtExprType::Primary;
}

// 末尾の名前を取り出す．
const char*
CptPrimaryBase::name() const
{
  return mName;
}


//////////////////////////////////////////////////////////////////////
// PtPrimary の実装クラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptPrimary::CptPrimary(const FileRegion& file_region,
		       const char* name) :
  CptPrimaryBase(name),
  mFileRegion(file_region)
{
}

// デストラクタ
CptPrimary::~CptPrimary()
{
}

// ファイル位置を返す．
FileRegion
CptPrimary::file_region() const
{
  return mFileRegion;
}

// index_list も range も持たないとき true を返す．
bool
CptPrimary::is_simple() const
{
  return true;
}


//////////////////////////////////////////////////////////////////////
// インデックスつきの終端記号を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptPrimaryI::CptPrimaryI(const FileRegion& file_region,
			 const char* name,
			 PtiExprArray&& index_array) :
  CptPrimaryBase(name),
  mFileRegion(file_region),
  mIndexArray{move(index_array)}
{
}

// デストラクタ
CptPrimaryI::~CptPrimaryI()
{
}

// ファイル位置を返す．
FileRegion
CptPrimaryI::file_region() const
{
  return mFileRegion;
}

// @brief インデックスリストのサイズの取得
// @return インデックスリストのサイズ
SizeType
CptPrimaryI::index_num() const
{
  return mIndexArray.size();
}

// @brief インデックスの取得
// @param[in] pos 位置番号 ( 0 <= pos < index_num() )
const PtExpr*
CptPrimaryI::index(SizeType pos) const
{
  ASSERT_COND( 0 <= pos && pos < index_num() );
  return mIndexArray[pos];
}

// index_list も range も持たないとき true を返す．
bool
CptPrimaryI::is_simple() const
{
  return false;
}


//////////////////////////////////////////////////////////////////////
// 定数インデックスつきの終端記号を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptPrimaryCI::CptPrimaryCI(const FileRegion& file_region,
			   const char* name,
			   PtiExprArray&& index_array) :
  CptPrimaryI(file_region, name, move(index_array))
{
}

// デストラクタ
CptPrimaryCI::~CptPrimaryCI()
{
}

// インデックスもしくは範囲が定数にならなければならないとき true を返す．
bool
CptPrimaryCI::is_const_index() const
{
  // この関数こそがこのクラスの存在意義
  return true;
}


//////////////////////////////////////////////////////////////////////
// 範囲指定つきの終端記号を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptPrimaryR::CptPrimaryR(const FileRegion& file_region,
			 const char* name,
			 VpiRangeMode mode,
			 const PtExpr* left,
			 const PtExpr* right) :
  CptPrimaryBase(name),
  mFileRegion{file_region},
  mMode{mode},
  mLeftRange{left},
  mRightRange{right}
{
}

// デストラクタ
CptPrimaryR::~CptPrimaryR()
{
}

// ファイル位置を返す．
FileRegion
CptPrimaryR::file_region() const
{
  return mFileRegion;
}

// 範囲指定モードの取得
VpiRangeMode
CptPrimaryR::range_mode() const
{
  return mMode;
}

// range の MSB を取出す．
const PtExpr*
CptPrimaryR::left_range() const
{
  return mLeftRange;
}

// range の LSB を取出す．
const PtExpr*
CptPrimaryR::right_range() const
{
  return mRightRange;
}

// index_list も range も持たないとき true を返す．
bool
CptPrimaryR::is_simple() const
{
  return false;
}


//////////////////////////////////////////////////////////////////////
// 定数範囲指定つきの終端記号を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptPrimaryCR::CptPrimaryCR(const FileRegion& file_region,
			   const char* name,
			   VpiRangeMode mode,
			   const PtExpr* left,
			   const PtExpr* right) :
  CptPrimaryR(file_region, name, mode, left, right)
{
}

// デストラクタ
CptPrimaryCR::~CptPrimaryCR()
{
}

// インデックスもしくは範囲が定数にならなければならないとき true を返す．
bool
CptPrimaryCR::is_const_index() const
{
  return true;
}


//////////////////////////////////////////////////////////////////////
// インデックスと範囲指定つきの終端記号を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptPrimaryIR::CptPrimaryIR(const FileRegion& file_region,
			   const char* name,
			   PtiExprArray&& index_array,
			   VpiRangeMode mode,
			   const PtExpr* left,
			   const PtExpr* right) :
  CptPrimaryI(file_region, name, move(index_array)),
  mMode{mode},
  mLeftRange{left},
  mRightRange{right}
{
}

// デストラクタ
CptPrimaryIR::~CptPrimaryIR()
{
}

// 範囲指定モード
VpiRangeMode
CptPrimaryIR::range_mode() const
{
  return mMode;
}

// range の MSB を取出す．
const PtExpr*
CptPrimaryIR::left_range() const
{
  return mLeftRange;
}

// range の LSB を取出す．
const PtExpr*
CptPrimaryIR::right_range() const
{
  return mRightRange;
}


//////////////////////////////////////////////////////////////////////
// 階層名を持つ primary operator を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptPrimaryH::CptPrimaryH(const FileRegion& file_region,
			 PtiNameBranchArray&& nb_array,
			 const char* tail_name) :
  CptPrimaryBase(tail_name),
  mFileRegion{file_region},
  mNbArray{move(nb_array)}
{
}

// デストラクタ
CptPrimaryH::~CptPrimaryH()
{
}

// ファイル位置を返す．
FileRegion
CptPrimaryH::file_region() const
{
  return mFileRegion;
}

// @brief 階層ブランチの要素数の取得
SizeType
CptPrimaryH::namebranch_num() const
{
  return mNbArray.size();
}

// @brief 階層ブランチの取得
/// @param[in] pos 位置 ( 0 <= pos < namebranch_num() )
const PtNameBranch*
CptPrimaryH::namebranch(SizeType pos) const
{
  return mNbArray[pos];
}


//////////////////////////////////////////////////////////////////////
// 階層名を持つインデックスつき primary を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptPrimaryHI::CptPrimaryHI(const FileRegion& file_region,
			   PtiNameBranchArray&& nb_array,
			   const char* tail_name,
			   PtiExprArray&& index_array) :
  CptPrimaryI(file_region, tail_name, move(index_array)),
  mNbArray{move(nb_array)}
{
}

// デストラクタ
CptPrimaryHI::~CptPrimaryHI()
{
}

// @brief 階層ブランチの要素数の取得
SizeType
CptPrimaryHI::namebranch_num() const
{
  return mNbArray.size();
}

// @brief 階層ブランチの取得
/// @param[in] pos 位置 ( 0 <= pos < namebranch_num() )
const PtNameBranch*
CptPrimaryHI::namebranch(SizeType pos) const
{
  return mNbArray[pos];
}


//////////////////////////////////////////////////////////////////////
// 階層名を持つ定数インデックスつき primary を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptPrimaryHCI::CptPrimaryHCI(const FileRegion& file_region,
			     PtiNameBranchArray&& nb_array,
			     const char* tail_name,
			     PtiExprArray&& index_array) :
  CptPrimaryHI(file_region, move(nb_array), tail_name, move(index_array))
{
}

// デストラクタ
CptPrimaryHCI::~CptPrimaryHCI()
{
}

// インデックスもしくは範囲が定数にならなければならないとき true を返す．
bool
CptPrimaryHCI::is_const_index() const
{
  return true;
}


//////////////////////////////////////////////////////////////////////
// 階層名を持つ範囲指定つき primary を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptPrimaryHR::CptPrimaryHR(const FileRegion& file_region,
			   PtiNameBranchArray&& nb_array,
			   const char* tail_name,
			   VpiRangeMode mode,
			   const PtExpr* left,
			   const PtExpr* right) :
  CptPrimaryR(file_region, tail_name, mode, left, right),
  mNbArray{move(nb_array)}
{
}

// デストラクタ
CptPrimaryHR::~CptPrimaryHR()
{
}

// @brief 階層ブランチの要素数の取得
SizeType
CptPrimaryHR::namebranch_num() const
{
  return mNbArray.size();
}

// @brief 階層ブランチの取得
/// @param[in] pos 位置 ( 0 <= pos < namebranch_num() )
const PtNameBranch*
CptPrimaryHR::namebranch(SizeType pos) const
{
  return mNbArray[pos];
}


//////////////////////////////////////////////////////////////////////
// 階層名を持つインデックスと範囲指定つき primary を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptPrimaryHIR::CptPrimaryHIR(const FileRegion& file_region,
			     PtiNameBranchArray&& nb_array,
			     const char* tail_name,
			     PtiExprArray&& index_array,
			     VpiRangeMode mode,
			     const PtExpr* left,
			     const PtExpr* right) :
  CptPrimaryIR(file_region, tail_name, move(index_array), mode, left, right),
  mNbArray{move(nb_array)}
{
}

// デストラクタ
CptPrimaryHIR::~CptPrimaryHIR()
{
}

// @brief 階層ブランチの要素数の取得
SizeType
CptPrimaryHIR::namebranch_num() const
{
  return mNbArray.size();
}

// @brief 階層ブランチの取得
/// @param[in] pos 位置 ( 0 <= pos < namebranch_num() )
const PtNameBranch*
CptPrimaryHIR::namebranch(SizeType pos) const
{
  return mNbArray[pos];
}


//////////////////////////////////////////////////////////////////////
// primary 関係
//////////////////////////////////////////////////////////////////////

// primary を生成する．
const PtExpr*
CptFactory::new_Primary(const FileRegion& file_region,
			const char* name)
{
  ++ mNumPrimary;
  void* p{mAlloc.get_memory(sizeof(CptPrimary))};
  auto obj{new (p) CptPrimary(file_region, name)};
  return obj;
}

// primary を生成する．
const PtExpr*
CptFactory::new_Primary(const FileRegion& file_region,
			const char* name,
			const vector<const PtExpr*>& index_array)
{
  ++ mNumPrimaryI;
  void* p{mAlloc.get_memory(sizeof(CptPrimaryI))};
  auto obj{new (p) CptPrimaryI(file_region, name,
			       PtiArray<const PtExpr>(mAlloc, index_array))};
  return obj;
}

// primary を生成する．
const PtExpr*
CptFactory::new_Primary(const FileRegion& file_region,
			const char* name,
			VpiRangeMode mode,
			const PtExpr* left,
			const PtExpr* right)
{
  ++ mNumPrimaryR;
  void* p{mAlloc.get_memory(sizeof(CptPrimaryR))};
  auto obj{new (p) CptPrimaryR(file_region, name, mode, left, right)};
  return obj;
}

// primary を生成する．
const PtExpr*
CptFactory::new_Primary(const FileRegion& file_region,
			const char* name,
			const vector<const PtExpr*>& index_array,
			VpiRangeMode mode,
			const PtExpr* left,
			const PtExpr* right)
{
  ++ mNumPrimaryIR;
  void* p{mAlloc.get_memory(sizeof(CptPrimaryIR))};
  auto obj{new (p) CptPrimaryIR(file_region, name,
				PtiArray<const PtExpr>(mAlloc, index_array),
				mode, left, right)};
  return obj;
}

// primary を生成する．
const PtExpr*
CptFactory::new_Primary(const FileRegion& file_region,
			PuHierName* hname)
{
  ++ mNumPrimaryH;
  void* p{mAlloc.get_memory(sizeof(CptPrimaryH))};
  auto nb_array = hname->name_branch_to_vector();
  auto tail_name = hname->tail_name();
  auto obj{new (p) CptPrimaryH(file_region,
			       PtiArray<const PtNameBranch>(mAlloc, nb_array),
			       tail_name)};
  return obj;
}

// primary を生成する．
const PtExpr*
CptFactory::new_Primary(const FileRegion& file_region,
			PuHierName* hname,
			const vector<const PtExpr*>& index_array)
{
  ++ mNumPrimaryHI;
  void* p{mAlloc.get_memory(sizeof(CptPrimaryHI))};
  auto nb_array = hname->name_branch_to_vector();
  auto tail_name = hname->tail_name();
  auto obj{new (p) CptPrimaryHI(file_region,
				PtiArray<const PtNameBranch>(mAlloc, nb_array),
				tail_name,
				PtiArray<const PtExpr>(mAlloc, index_array))};
  return obj;
}

// primary を生成する．
const PtExpr*
CptFactory::new_Primary(const FileRegion& file_region,
			PuHierName* hname,
			VpiRangeMode mode,
			const PtExpr* left,
			const PtExpr* right)
{
  ++ mNumPrimaryHR;
  void* p{mAlloc.get_memory(sizeof(CptPrimaryHR))};
  auto nb_array = hname->name_branch_to_vector();
  auto tail_name = hname->tail_name();
  auto obj{new (p) CptPrimaryHR(file_region,
				PtiArray<const PtNameBranch>(mAlloc, nb_array),
				tail_name, mode,
				left, right)};
  return obj;
}

// primary を生成する．
const PtExpr*
CptFactory::new_Primary(const FileRegion& file_region,
			PuHierName* hname,
			const vector<const PtExpr*>& index_array,
			VpiRangeMode mode,
			const PtExpr* left,
			const PtExpr* right)
{
  ++ mNumPrimaryHIR;
  void* p{mAlloc.get_memory(sizeof(CptPrimaryHIR))};
  auto nb_array = hname->name_branch_to_vector();
  auto tail_name = hname->tail_name();
  auto obj{new (p) CptPrimaryHIR(file_region,
				 PtiArray<const PtNameBranch>(mAlloc, nb_array),
				 tail_name,
				 PtiArray<const PtExpr>(mAlloc, index_array),
				 mode, left, right)};
  return obj;
}

// primary を生成する．
const PtExpr*
CptFactory::new_CPrimary(const FileRegion& file_region,
			 const char* name,
			 const vector<const PtExpr*>& index_array)
{
  ++ mNumPrimaryCI;
  void* p{mAlloc.get_memory(sizeof(CptPrimaryCI))};
  auto obj{new (p) CptPrimaryCI(file_region, name,
				PtiArray<const PtExpr>(mAlloc, index_array))};
  return obj;
}

// primary を生成する．
const PtExpr*
CptFactory::new_CPrimary(const FileRegion& file_region,
			 const char* name,
			 VpiRangeMode mode,
			 const PtExpr* left,
			 const PtExpr* right)
{
  ++ mNumPrimaryCR;
  void* p{mAlloc.get_memory(sizeof(CptPrimaryCR))};
  auto obj{new (p) CptPrimaryCR(file_region, name, mode, left, right)};
  return obj;
}

// primary を生成する．
const PtExpr*
CptFactory::new_CPrimary(const FileRegion& file_region,
			 PuHierName* hname,
			 const vector<const PtExpr*>& index_array)
{
  ++ mNumPrimaryHCI;
  void* p{mAlloc.get_memory(sizeof(CptPrimaryCI))};
  auto nb_array = hname->name_branch_to_vector();
  auto tail_name = hname->tail_name();
  auto obj{new (p) CptPrimaryHCI(file_region,
				 PtiArray<const PtNameBranch>(mAlloc, nb_array),
				 tail_name,
				 PtiArray<const PtExpr>(mAlloc, index_array))};
  return obj;
}

END_NAMESPACE_YM_VERILOG
