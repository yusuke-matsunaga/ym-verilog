
/// @file VlDumperImpl_scope.cc
/// @brief VlDumperImpl の実装ファイル (scope担当)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2009, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "VlDumperImpl.h"

#include "ym/VlMgr.h"
#include "ym/vl/VlModule.h"
#include "ym/vl/VlScope.h"
#include "ym/vl/VlTaskFunc.h"


BEGIN_NAMESPACE_YM_VERILOG

// scope の内容を出力する関数
// IEEE1364-2001 p.634
void
VlDumperImpl::put_scope(const char* label,
			const VlMgr& mgr,
			const VlScope* scope)
{
  VlDumpHeader x(this, label, "Scope");

  put("mFileRegion", scope->file_region() );
  put("vpiFullName", scope->full_name() );

  put_scope_sub(mgr, scope);
}

void
VlDumperImpl::put_scope_sub(const VlMgr& mgr,
			    const VlScope* scope)
{
  put_decl_list("vpiParameter", mgr, mgr.find_decl_list(scope, vpiParameter));

  put_paramassign_list("vpiParamAssign", mgr, mgr.find_paramassign_list(scope));

  put_defparam_list("vpiDefParam", mgr, mgr.find_defparam_list(scope));

  put_decl_list("vpiSpecParam", mgr, mgr.find_decl_list(scope, vpiSpecParam));

  put_decl_list("vpiNet", mgr, mgr.find_decl_list(scope, vpiNet));

  put_declarray_list("vpiNetArray", mgr, mgr.find_declarray_list(scope, vpiNetArray));

  put_decl_list("vpiReg", mgr, mgr.find_decl_list(scope, vpiReg));

  put_declarray_list("vpiRegArray", mgr, mgr.find_declarray_list(scope, vpiRegArray));

  put_decl_list("vpiVariables", mgr, mgr.find_decl_list(scope, vpiVariables));

  put_declarray_list("vpiVariables", mgr, mgr.find_declarray_list(scope, vpiVariables));

  put_decl_list("vpiNamedEvent", mgr, mgr.find_decl_list(scope, vpiNamedEvent));

  put_declarray_list("vpiNamedEventArray", mgr, mgr.find_declarray_list(scope, vpiNamedEventArray));

  vector<const VlScope*> scope_list = mgr.find_internalscope_list(scope);
  VlDumpHeader x(this, "vpiInternalScope", "ScopeList");
  for ( auto obj: scope_list ) {
    put_scope("vpiInternalScope", mgr, obj);
  }

  vector<const VlTaskFunc*> task_list = mgr.find_task_list(scope);
  for ( auto task: task_list ) {
    put_task("vpiTask", mgr, task);
  }

  vector<const VlTaskFunc*> func_list = mgr.find_function_list(scope);
  for ( auto func: func_list ) {
    put_function("vpiFunction", mgr, func);
  }

  put_contassign_list("vpiContAssign", mgr, mgr.find_contassign_list(scope));

  vector<const VlModule*> module_list = mgr.find_module_list(scope);
  // module instance 名前だけ
  for ( auto module: module_list ) {
    put("vpiModule", module->full_name());
  }

  put_modulearray_list("vpiModuleArray", mgr, mgr.find_modulearray_list(scope));

  put_primitive_list("vpiPrimitive", mgr, mgr.find_primitive_list(scope));

  put_primarray_list("vpiPrimitiveArray", mgr, mgr.find_primarray_list(scope));
}

END_NAMESPACE_YM_VERILOG
