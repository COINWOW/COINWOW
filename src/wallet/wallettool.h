// Copyright (C) 2009-2025 Bitcoin Core developers

// Copyright (C) 2026 COINWOW developers

// Distributed under the MIT software license

// Copyright (c) 2016-2021 The COINWOW Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef COINWOW_WALLET_WALLETTOOL_H
#define COINWOW_WALLET_WALLETTOOL_H

#include <string>

class ArgsManager;

namespace wallet {
namespace WalletTool {

bool ExecuteWalletToolFunc(const ArgsManager& args, const std::string& command);

} // namespace WalletTool
} // namespace wallet

#endif // COINWOW_WALLET_WALLETTOOL_H
