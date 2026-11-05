# Copyright (c) 2023-present The Bitcoin Core developers
# Copyright (c) 2024-present COINWOW Developers
# Distributed under the MIT software license, see the accompanying
# file COPYING or https://opensource.org/license/mit/.

# Wallet functionality used by coinwowd and coinwow-wallet executables.
add_library(coinwow_wallet STATIC EXCLUDE_FROM_ALL
  coincontrol.cpp
  coinselection.cpp
  context.cpp
  crypter.cpp
  db.cpp
  dump.cpp
  external_signer_scriptpubkeyman.cpp
  feebumper.cpp
  fees.cpp
  interfaces.cpp
  load.cpp
  migrate.cpp
  receive.cpp
  rpc/addresses.cpp
  rpc/backup.cpp
  rpc/coins.cpp
  rpc/encrypt.cpp
  rpc/signmessage.cpp
  rpc/spend.cpp
  rpc/transactions.cpp
  rpc/util.cpp
  rpc/wallet.cpp
  scriptpubkeyman.cpp
  spend.cpp
  sqlite.cpp
  transaction.cpp
  wallet.cpp
  walletdb.cpp
  walletutil.cpp
)
# CMake >= 4.x renamed the FindSQLite3 target to SQLite3::SQLite3 and
# deprecated SQLite::SQLite3. Prefer the new name, fall back to the old one.
set(coinwow_sqlite3_target "")
if(TARGET SQLite3::SQLite3)
  set(coinwow_sqlite3_target SQLite3::SQLite3)
elseif(TARGET SQLite::SQLite3)
  set(coinwow_sqlite3_target SQLite::SQLite3)
endif()

target_link_libraries(coinwow_wallet
  PRIVATE
    core_interface
    coinwow_common
    $<TARGET_NAME_IF_EXISTS:unofficial::sqlite3::sqlite3>
    ${coinwow_sqlite3_target}
    univalue
    Boost::headers
    $<TARGET_NAME_IF_EXISTS:USDT::headers>
)
