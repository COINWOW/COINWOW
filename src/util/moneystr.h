// Copyright (C) 2009-2025 Bitcoin Core developers

// Copyright (C) 2026 COINWOW developers

// Distributed under the MIT software license

// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2022 The COINWOW Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

/**
 * Money parsing/formatting utilities.
 */
#ifndef COINWOW_UTIL_MONEYSTR_H
#define COINWOW_UTIL_MONEYSTR_H

#include <consensus/amount.h>

#include <optional>
#include <string>

/* Do not use these functions to represent or parse monetary amounts to or from
 * JSON but use AmountFromValue and ValueFromAmount for that.
 */
std::string FormatMoney(const CAmount n);
/** Parse an amount denoted in full coins. E.g. "0.0034" supplied on the command line. **/
std::optional<CAmount> ParseMoney(const std::string& str);

#endif // COINWOW_UTIL_MONEYSTR_H
