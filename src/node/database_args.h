// Copyright (c) 2022 The Bitcoin Core developers
// Copyright (c) 2024-present COINWOW Developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef COINWOW_NODE_DATABASE_ARGS_H
#define COINWOW_NODE_DATABASE_ARGS_H

class ArgsManager;
struct DBOptions;

namespace node {
void ReadDatabaseArgs(const ArgsManager& args, DBOptions& options);
} // namespace node

#endif // COINWOW_NODE_DATABASE_ARGS_H
