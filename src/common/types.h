// Copyright (C) 2009-2025 Bitcoin Core developers

// Copyright (C) 2026 COINWOW developers

// Distributed under the MIT software license

// Copyright (c) 2010-2021 The COINWOW Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

//! @file common/types.h is a home for simple enum and struct type definitions
//! that can be used internally by functions in the libcoinwow_common library,
//! but also used externally by node, wallet, and GUI code.
//!
//! This file is intended to define only simple types that do not have external
//! dependencies. More complicated types should be defined in dedicated header
//! files.

#ifndef COINWOW_COMMON_TYPES_H
#define COINWOW_COMMON_TYPES_H

namespace common {
enum class PSBTError {
    MISSING_INPUTS,
    SIGHASH_MISMATCH,
    EXTERNAL_SIGNER_NOT_FOUND,
    EXTERNAL_SIGNER_FAILED,
    UNSUPPORTED,
    INCOMPLETE,
    OK,
};
} // namespace common

#endif // COINWOW_COMMON_TYPES_H
