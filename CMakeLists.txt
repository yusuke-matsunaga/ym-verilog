# ===================================================================
# CMAKE のおまじない
# ===================================================================


# ===================================================================
# プロジェクト名，バージョンの設定
# ===================================================================


# ===================================================================
# オプション
# ===================================================================


# ===================================================================
# パッケージの検査
# ===================================================================

find_package(BISON 3.0.4 REQUIRED)


# ===================================================================
# ヘッダファイルの生成
# ===================================================================


# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories(
  ${PROJECT_BINARY_DIR}/include
  ${PROJECT_SOURCE_DIR}/ym-common/include
  ${PROJECT_SOURCE_DIR}/ym-base/include
  ${PROJECT_SOURCE_DIR}/ym-logic/include
  ${PROJECT_SOURCE_DIR}/ym-cell/include
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/private_include
  ${CMAKE_CURRENT_BINARY_DIR}
  )


# ===================================================================
#  マクロの定義
# ===================================================================


# ===================================================================
# サブディレクトリの設定
# ===================================================================

#add_subdirectory ( cython-src )
add_subdirectory ( tests/programs )


# ===================================================================
#  ソースの設定
# ===================================================================

set (common_SOURCES
  c++-src/common/BitVector_constructor.cc
  c++-src/common/BitVector_misc.cc
  c++-src/common/BitVector_op1.cc
  c++-src/common/BitVector_op2.cc
  c++-src/common/VlMgr.cc
  c++-src/common/VlUdpVal.cc
  c++-src/common/VlValue.cc
  c++-src/common/VlValueRep.cc
  )

set (elaborator_SOURCES
  c++-src/elaborator/ei/EiAttribute.cc
  c++-src/elaborator/ei/EiContAssign.cc
  c++-src/elaborator/ei/EiControl.cc
  c++-src/elaborator/ei/EiDelay.cc
  c++-src/elaborator/ei/EiFactory.cc
  c++-src/elaborator/ei/EiGenBlockScope.cc
  c++-src/elaborator/ei/EiGfRoot.cc
  c++-src/elaborator/ei/EiModule.cc
  c++-src/elaborator/ei/EiParamAssign.cc
  c++-src/elaborator/ei/EiPort.cc
  c++-src/elaborator/ei/EiPrimitive.cc
  c++-src/elaborator/ei/EiProcess.cc
  c++-src/elaborator/ei/EiRange.cc
  c++-src/elaborator/ei/EiScope.cc
  c++-src/elaborator/ei/EiStmtBlockScope.cc
  c++-src/elaborator/ei/EiTaskFunc.cc
  c++-src/elaborator/ei/EiToplevel.cc
  c++-src/elaborator/ei/EiUdp.cc

  c++-src/elaborator/ei_decl/EiDecl.cc
  c++-src/elaborator/ei_decl/EiDeclArray.cc
  c++-src/elaborator/ei_decl/EiDeclHead.cc
  c++-src/elaborator/ei_decl/EiGenvar.cc
  c++-src/elaborator/ei_decl/EiImpNet.cc
  c++-src/elaborator/ei_decl/EiIODecl.cc
  c++-src/elaborator/ei_decl/EiParameter.cc

  c++-src/elaborator/ei_expr/EiBinaryOp.cc
  c++-src/elaborator/ei_expr/EiBitSelect.cc
  c++-src/elaborator/ei_expr/EiConcatOp.cc
  c++-src/elaborator/ei_expr/EiConstant.cc
  c++-src/elaborator/ei_expr/EiExpr.cc
  c++-src/elaborator/ei_expr/EiFuncCall.cc
  c++-src/elaborator/ei_expr/EiLhs.cc
  c++-src/elaborator/ei_expr/EiPartSelect.cc
  c++-src/elaborator/ei_expr/EiPrimary.cc
  c++-src/elaborator/ei_expr/EiTernaryOp.cc
  c++-src/elaborator/ei_expr/EiUnaryOp.cc

  c++-src/elaborator/ei_stmt/EiAssignment.cc
  c++-src/elaborator/ei_stmt/EiBlockStmt.cc
  c++-src/elaborator/ei_stmt/EiCondStmt.cc
  c++-src/elaborator/ei_stmt/EiMiscStmt.cc
  c++-src/elaborator/ei_stmt/EiStmt.cc

  c++-src/elaborator/elb_mgr/AttrHash.cc
  c++-src/elaborator/elb_mgr/ElbDecl.cc
  c++-src/elaborator/elb_mgr/ElbExpr.cc
  c++-src/elaborator/elb_mgr/ElbMgr.cc
  c++-src/elaborator/elb_mgr/ElbPrimitive.cc
  c++-src/elaborator/elb_mgr/TagDict.cc
  c++-src/elaborator/elb_mgr/VlNamedObj.cc

  c++-src/elaborator/main/AttrGen.cc
  c++-src/elaborator/main/DeclGen.cc
  c++-src/elaborator/main/Elaborator.cc
  c++-src/elaborator/main/ElbEnv.cc
  c++-src/elaborator/main/ElbError.cc
  c++-src/elaborator/main/ElbProxy.cc
  c++-src/elaborator/main/ElbStubList.cc
  c++-src/elaborator/main/ErrorGen.cc
  c++-src/elaborator/main/ExprGen_funccall.cc
  c++-src/elaborator/main/ExprGen_main.cc
  c++-src/elaborator/main/ExprGen_operation.cc
  c++-src/elaborator/main/ExprGen_primary.cc
  c++-src/elaborator/main/ExprEval.cc
  c++-src/elaborator/main/FuncEval.cc
  c++-src/elaborator/main/ItemGen_main.cc
  c++-src/elaborator/main/ItemGen_module_inst.cc
  c++-src/elaborator/main/ItemGen_primitive.cc
  c++-src/elaborator/main/ItemGen_taskfunc.cc
  c++-src/elaborator/main/ModuleGen.cc
  c++-src/elaborator/main/ObjDict.cc
  c++-src/elaborator/main/StmtGen_assign.cc
  c++-src/elaborator/main/StmtGen_block.cc
  c++-src/elaborator/main/StmtGen_cond.cc
  c++-src/elaborator/main/StmtGen_main.cc
  c++-src/elaborator/main/UdpGen.cc
  )

set (parser_SOURCES
  c++-src/parser/cpt/CptDecl.cc
  c++-src/parser/cpt/CptExpr.cc
  c++-src/parser/cpt/CptFactory.cc
  c++-src/parser/cpt/CptGenerate.cc
  c++-src/parser/cpt/CptIO.cc
  c++-src/parser/cpt/CptInst.cc
  c++-src/parser/cpt/CptItem.cc
  c++-src/parser/cpt/CptMisc.cc
  c++-src/parser/cpt/CptModule.cc
  c++-src/parser/cpt/CptPrimary.cc
  c++-src/parser/cpt/CptSpecItem.cc
  c++-src/parser/cpt/CptStmt.cc
  c++-src/parser/cpt/CptUdp.cc

  c++-src/parser/main/Parser.cc
  c++-src/parser/main/Parser_decl.cc
  c++-src/parser/main/Parser_expr.cc
  c++-src/parser/main/Parser_generate.cc
  c++-src/parser/main/Parser_inst.cc
  c++-src/parser/main/Parser_item.cc
  c++-src/parser/main/Parser_misc.cc
  c++-src/parser/main/Parser_module.cc
  c++-src/parser/main/Parser_port.cc
  c++-src/parser/main/Parser_stmt.cc
  c++-src/parser/main/Parser_udp.cc

  c++-src/parser/pt_mgr/PtDumper.cc
  c++-src/parser/pt_mgr/PtMgr.cc
  c++-src/parser/pt_mgr/PtiBase.cc
  c++-src/parser/pt_mgr/PtiFactory.cc

  c++-src/parser/scanner/InputMgr.cc
  c++-src/parser/scanner/Lex.cc
  c++-src/parser/scanner/LexCondPlugin.cc
  c++-src/parser/scanner/LexMacroPlugin.cc
  c++-src/parser/scanner/LexModulePlugin.cc
  c++-src/parser/scanner/LexModuleState.cc
  c++-src/parser/scanner/LexPlugin.cc
  c++-src/parser/scanner/LexPluginDict.cc
  c++-src/parser/scanner/LexState.cc
  c++-src/parser/scanner/MacroSource.cc
  c++-src/parser/scanner/RawLex.cc
  c++-src/parser/scanner/RsrvWordDic.cc
  c++-src/parser/scanner/StdLexPlugin.cc
  c++-src/parser/scanner/TokenInfo.cc
  c++-src/parser/scanner/TokenList.cc

  c++-src/parser/spt/SptDecl.cc
  c++-src/parser/spt/SptExpr.cc
  c++-src/parser/spt/SptFactory.cc
  c++-src/parser/spt/SptGenerate.cc
  c++-src/parser/spt/SptItem.cc
  c++-src/parser/spt/SptMisc.cc
  c++-src/parser/spt/SptModule.cc
  c++-src/parser/spt/SptSpecItem.cc
  c++-src/parser/spt/SptStmt.cc
  c++-src/parser/spt/SptUdp.cc

  ${CMAKE_CURRENT_BINARY_DIR}/verilog_grammer.cc
  )

set (alloc_SOURCES
  c++-src/alloc/Alloc.cc
  c++-src/alloc/SimpleAlloc.cc
  )


# Create target for the parser
add_custom_target ( verilog_grammer ALL
  DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/verilog_grammer.hh"
  "${CMAKE_CURRENT_BINARY_DIR}/verilog_grammer.cc"
  )

# Create custom command for bison/yacc (note the DEPENDS)
add_custom_command (
  COMMAND "${BISON_EXECUTABLE}"
  "${CMAKE_CURRENT_SOURCE_DIR}/c++-src/parser/main/verilog_grammer.yy"
  -o "${CMAKE_CURRENT_BINARY_DIR}/verilog_grammer.cc"
  -d -v
  OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/verilog_grammer.cc"
  "${CMAKE_CURRENT_BINARY_DIR}/verilog_grammer.hh"
  "${CMAKE_CURRENT_BINARY_DIR}/verilog_grammer.output"
  DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/c++-src/parser/main/verilog_grammer.yy"
  )

# Since verilog_grammer.cc does not exists yet when cmake is run, mark
# it as generated
set_source_files_properties ( ${CMAKE_CURRENT_BINARY_DIR}/verilog_grammer.cc GENERATED )


# ===================================================================
#  ターゲットの設定
# ===================================================================
ym_add_object_library ( ym_verilog
  ${common_SOURCES}
  ${elaborator_SOURCES}
  ${parser_SOURCES}
  ${alloc_SOURCES}
  )
