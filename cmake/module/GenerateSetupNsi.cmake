# Copyright (c) 2023-present The COINWOW Core developers
# Distributed under the MIT software license, see the accompanying
# file COPYING or https://opensource.org/license/mit/.

function(generate_setup_nsi)
  set(abs_top_srcdir ${PROJECT_SOURCE_DIR})
  set(abs_top_builddir ${PROJECT_BINARY_DIR})
  set(CLIENT_URL ${PROJECT_HOMEPAGE_URL})
  set(CLIENT_TARNAME "coinwow")
  set(COINWOW_WRAPPER_NAME "coinwow")
  set(COINWOW_GUI_NAME "coinwow-qt")
  set(COINWOW_DAEMON_NAME "coinwowd")
  set(COINWOW_CLI_NAME "coinwow-cli")
  set(COINWOW_TX_NAME "coinwow-tx")
  set(COINWOW_WALLET_TOOL_NAME "coinwow-wallet")
  set(COINWOW_TEST_NAME "test_coinwow")
  set(EXEEXT ${CMAKE_EXECUTABLE_SUFFIX})
  configure_file(${PROJECT_SOURCE_DIR}/share/setup.nsi.in ${PROJECT_BINARY_DIR}/coinwow-win64-setup.nsi USE_SOURCE_PERMISSIONS @ONLY)
endfunction()
