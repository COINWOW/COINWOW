// Copyright (c) 2022 The Bitcoin Core developers
// Copyright (c) 2024-present COINWOW Developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef COINWOW_NODE_COINS_VIEW_ARGS_H
#define COINWOW_NODE_COINS_VIEW_ARGS_H

class ArgsManager;
struct CoinsViewOptions;

namespace node {
void ReadCoinsViewArgs(const ArgsManager& args, CoinsViewOptions& options);
} // namespace node

#endif // COINWOW_NODE_COINS_VIEW_ARGS_H
