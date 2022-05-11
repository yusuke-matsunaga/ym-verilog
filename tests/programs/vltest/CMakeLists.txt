

# ===================================================================
# オプション
# ===================================================================


# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories (
  ../include
  )


# ===================================================================
#  マクロの定義
# ===================================================================


# ===================================================================
# サブディレクトリの設定
# ===================================================================



# ===================================================================
#  ソースファイルの設定
# ===================================================================

set ( vltest_SOURCES
  VlDumper.cc
  VlDumperImpl.cc
  VlDumperImpl_decl.cc
  VlDumperImpl_expr.cc
  VlDumperImpl_item.cc
  VlDumperImpl_module.cc
  VlDumperImpl_scope.cc
  VlDumperImpl_stmt.cc
  vltest_elaborate.cc
  vltest_main.cc
  vltest_parse.cc
  ../common/VlTestLineWatcher.cc
  )


# ===================================================================
#  テスト用のターゲットの設定
# ===================================================================

add_executable ( vltest
  ${vltest_SOURCES}
  $<TARGET_OBJECTS:ym_verilog_obj_d>
  $<TARGET_OBJECTS:ym_cell_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  $<TARGET_OBJECTS:ym_base_obj_d>
  )

target_compile_options ( vltest
  PRIVATE "-g"
  )

target_link_libraries ( vltest
  ${YM_LIB_DEPENDS}
  )
