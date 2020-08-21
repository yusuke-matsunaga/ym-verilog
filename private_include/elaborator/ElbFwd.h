#ifndef ELBFWD_H
#define ELBFWD_H

/// @file ElbFwd.h
/// @brief ElbObj の派生クラスの前方宣言ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym/vl/VlFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

// in ElbScope.h
class ElbScope;

// in ElbModule.h
class ElbModule;

// in ElbModuleArray.h
class ElbModuleArray;

// in ElbPort.h
class ElbPort;

// in ElbHead.h
class ElbHead;

// in ElbIODecl.h
class ElbIOHead;

// in ElbDecl.h
class ElbDeclHead;
class ElbDecl;

// in ElbParameter.h
class ElbParamHead;
class ElbParameter;

// in ElbContAssign.h
class ElbCaHead;

// in ElbTaskFunc.h
class ElbTaskFunc;

// in ElbPrimitive.h
class ElbPrimHead;
class ElbPrimitive;
class ElbPrimArray;
class ElbPrimTerm;

// in ElbProcess.h
class ElbProcess;

// in ElbExpr.h
class ElbExpr;

// in ElbRange.h
class ElbRange;
class ElbRangeSrc;

// in ElbUdp.h
class ElbUdpDefn;
class ElbTableEntry;

// in ElbUserSystf.h
class ElbUserSystf;

// in ElbGfRoot.h
class ElbGfRoot;

// in ElbGenvar.h
class ElbGenvar;

// in ElbAttribute.h
class ElbAttribute;
class ElbAttrList;
class ElbAttrIter;

END_NAMESPACE_YM_VERILOG

#endif // ELBFWD_H
