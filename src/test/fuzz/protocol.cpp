// Copyright (C) 2009-2025 Bitcoin Core developers

// Copyright (C) 2026 COINWOW developers

// Distributed under the MIT software license

// Copyright (c) 2020 The COINWOW Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <protocol.h>
#include <test/fuzz/FuzzedDataProvider.h>
#include <test/fuzz/fuzz.h>
#include <test/fuzz/util.h>

#include <cstdint>
#include <optional>
#include <stdexcept>
#include <vector>

FUZZ_TARGET(protocol)
{
    FuzzedDataProvider fuzzed_data_provider(buffer.data(), buffer.size());
    const std::optional<CInv> inv = ConsumeDeserializable<CInv>(fuzzed_data_provider);
    if (!inv) {
        return;
    }
    try {
        (void)inv->GetMessageType();
    } catch (const std::out_of_range&) {
    }
    (void)inv->ToString();
    const std::optional<CInv> another_inv = ConsumeDeserializable<CInv>(fuzzed_data_provider);
    if (!another_inv) {
        return;
    }
    (void)(*inv < *another_inv);
}
