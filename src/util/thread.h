// Copyright (C) 2009-2025 Bitcoin Core developers

// Copyright (C) 2026 COINWOW developers

// Distributed under the MIT software license

// Copyright (c) 2021-2022 The COINWOW Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef COINWOW_UTIL_THREAD_H
#define COINWOW_UTIL_THREAD_H

#include <functional>
#include <string>

namespace util {
/**
 * A wrapper for do-something-once thread functions.
 */
void TraceThread(std::string_view thread_name, std::function<void()> thread_func);

} // namespace util

#endif // COINWOW_UTIL_THREAD_H
