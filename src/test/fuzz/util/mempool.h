// Copyright (C) 2009-2025 Bitcoin Core developers

// Copyright (C) 2026 COINWOW developers

// Distributed under the MIT software license

// Copyright (c) 2022-present The COINWOW Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef COINWOW_TEST_FUZZ_UTIL_MEMPOOL_H
#define COINWOW_TEST_FUZZ_UTIL_MEMPOOL_H

#include <kernel/mempool_entry.h>
#include <validation.h>

class CTransaction;
class CTxMemPool;
class FuzzedDataProvider;

class DummyChainState final : public Chainstate
{
public:
    void SetMempool(CTxMemPool* mempool)
    {
        m_mempool = mempool;
    }
};

[[nodiscard]] CTxMemPoolEntry ConsumeTxMemPoolEntry(FuzzedDataProvider& fuzzed_data_provider, const CTransaction& tx, uint32_t max_height=std::numeric_limits<uint32_t>::max()) noexcept;

#endif // COINWOW_TEST_FUZZ_UTIL_MEMPOOL_H
