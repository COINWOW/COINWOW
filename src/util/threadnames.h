// Copyright (C) 2009-2025 Bitcoin Core developers

// Copyright (C) 2026 COINWOW developers

// Distributed under the MIT software license

// Copyright (c) 2018-2019 The COINWOW Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef COINWOW_UTIL_THREADNAMES_H
#define COINWOW_UTIL_THREADNAMES_H

#include <string>

namespace util {
//! Rename a thread both in terms of an internal (in-memory) name as well
//! as its system thread name.
//! @note Do not call this for the main thread, as this will interfere with
//! UNIX utilities such as top and killall. Use ThreadSetInternalName instead.
void ThreadRename(const std::string&);

//! Set the internal (in-memory) name of the current thread only.
void ThreadSetInternalName(const std::string&);

//! Get the thread's internal (in-memory) name; used e.g. for identification in
//! logging.
std::string ThreadGetInternalName();

} // namespace util

#endif // COINWOW_UTIL_THREADNAMES_H
