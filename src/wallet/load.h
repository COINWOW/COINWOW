// Copyright (C) 2009-2025 Bitcoin Core developers

// Copyright (C) 2026 COINWOW developers

// Distributed under the MIT software license

// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2021 The COINWOW Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef COINWOW_WALLET_LOAD_H
#define COINWOW_WALLET_LOAD_H

#include <string>
#include <vector>

class ArgsManager;
class CScheduler;

namespace interfaces {
class Chain;
} // namespace interfaces

namespace wallet {
struct WalletContext;

//! Responsible for reading and validating the -wallet arguments and verifying the wallet database.
bool VerifyWallets(WalletContext& context);

//! Load wallet databases.
bool LoadWallets(WalletContext& context);

//! Complete startup of wallets.
void StartWallets(WalletContext& context);

void UnloadWallets(WalletContext& context);
} // namespace wallet

#endif // COINWOW_WALLET_LOAD_H
