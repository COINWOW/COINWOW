// Copyright (c) 2022 The Bitcoin Core developers
// Copyright (c) 2024-present COINWOW Developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#include <util/translation.h>

#include <functional>
#include <string>

// Define G_TRANSLATION_FUN symbol in libcoinwowkernel library so users of the
// library aren't required to export this symbol
extern const TranslateFn G_TRANSLATION_FUN{nullptr};
