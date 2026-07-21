// Copyright (C) 2009-2025 Bitcoin Core developers

// Copyright (C) 2026 COINWOW developers

// Distributed under the MIT software license

// Copyright (c) 2019-2022 The COINWOW Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef COINWOW_UTIL_BIP32_H
#define COINWOW_UTIL_BIP32_H

#include <cstdint>
#include <string>
#include <vector>

/** Parse an HD keypaths like "m/7/0'/2000". */
[[nodiscard]] bool ParseHDKeypath(const std::string& keypath_str, std::vector<uint32_t>& keypath);

/** Write HD keypaths as strings */
std::string WriteHDKeypath(const std::vector<uint32_t>& keypath, bool apostrophe = false);
std::string FormatHDKeypath(const std::vector<uint32_t>& path, bool apostrophe = false);

#endif // COINWOW_UTIL_BIP32_H
