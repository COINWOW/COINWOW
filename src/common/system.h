// Copyright (C) 2009-2025 Bitcoin Core developers

// Copyright (C) 2026 COINWOW developers

// Distributed under the MIT software license

// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-present The COINWOW Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef COINWOW_COMMON_SYSTEM_H
#define COINWOW_COMMON_SYSTEM_H

#include <coinwow-build-config.h> // IWYU pragma: keep

#include <cstdint>
#include <string>

// Application startup time (used for uptime calculation)
int64_t GetStartupTime();

void SetupEnvironment();
[[nodiscard]] bool SetupNetworking();
#ifndef WIN32
std::string ShellEscape(const std::string& arg);
#endif
#if HAVE_SYSTEM
void runCommand(const std::string& strCommand);
#endif

/**
 * Return the number of cores available on the current system.
 * @note This does count virtual cores, such as those provided by HyperThreading.
 */
int GetNumCores();

#endif // COINWOW_COMMON_SYSTEM_H
