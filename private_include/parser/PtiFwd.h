#ifndef PTIFWD_H
#define PTIFWD_H

/// @file PtiFwd.h
/// @brief Pti クラスの名前の宣言のみおこなうヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym/verilog.h"
#include "ym/pt/PtP.h"
#include "PtrList.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス名の前方宣言
//////////////////////////////////////////////////////////////////////

// in PtiFactory.h
class PtiFactory;

// in PtiDecl.h
class PtiPort;
class PtiPortRef;
class PtiIOHead;
class PtiDeclHead;

// in PuHierName.h
class PuHierName;

// in PtiArray.h
template <typename T>
class PtiArray;

using PtiAttrInstArray    = PtiArray<const PtAttrInst>;
using PtiAttrSpecArray    = PtiArray<const PtAttrSpec>;
using PtiCaseItemArray    = PtiArray<const PtCaseItem>;
using PtiConnectionArray  = PtiArray<const PtConnection>;
using PtiContAssignArray  = PtiArray<const PtContAssign>;
using PtiDeclHeadArray    = PtiArray<const PtDeclHead>;
using PtiDeclItemArray    = PtiArray<const PtDeclItem>;
using PtiDefParamArray    = PtiArray<const PtDefParam>;
using PtiExprArray        = PtiArray<const PtExpr>;
using PtiGenCaseItemArray = PtiArray<const PtGenCaseItem>;
using PtiInstArray        = PtiArray<const PtInst>;
using PtiIOHeadArray      = PtiArray<const PtIOHead>;
using PtiIOItemArray      = PtiArray<const PtIOItem>;
using PtiItemArray        = PtiArray<const PtItem>;
using PtiNameBranchArray  = PtiArray<const PtNameBranch>;
using PtiRangeArray       = PtiArray<const PtRange>;
using PtiPortArray        = PtiArray<const PtPort>;
using PtiStmtArray        = PtiArray<const PtStmt>;
using PtiUdpEntryArray    = PtiArray<const PtUdpEntry>;
using PtiUdpValueArray    = PtiArray<const PtUdpValue>;

/// @relates PtiNameBranchArray
/// @brief 階層名を作り出す関数
/// @param[in] nb_array 階層ブランチリスト
/// @param[in] name 末尾の名前
/// @return 階層名を展開したものを返す．
string
expand_full_name(const PtiNameBranchArray* nb_array,
		 const char* name);

END_NAMESPACE_YM_VERILOG

#endif // PTIFWD_H
