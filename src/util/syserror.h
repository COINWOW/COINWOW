// Copyright (C) 2009-2025 Bitcoin Core developers

// Copyright (C) 2026 COINWOW developers

// Distributed under the MIT software license

// Copyright (c) 2010-2022 The COINWOW Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef COINWOW_UTIL_SYSERROR_H
#define COINWOW_UTIL_SYSERROR_H

#include <string>

/** Return system error string from errno value. Use this instead of
 * std::strerror, which is not thread-safe. For network errors use
 * NetworkErrorString from sock.h instead.
 */
std::string SysErrorString(int err);

#if defined(WIN32)
std::string Win32ErrorString(int err);
#endif

#endif // COINWOW_UTIL_SYSERROR_H
