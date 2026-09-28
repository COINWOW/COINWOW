// Copyright (c) 2026-present COINWOW Developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or https://opensource.org/license/mit/.

// Regression guard for COINWOW network identity and consensus parameters.
//
// COINWOW mainnet has been live since 2026-04-24. The values checked here
// define which blocks are valid and which peers/wallets are compatible, so
// any change to them is a hard fork, a network split or a wallet break.
// If this test fails, the change must be deliberate and reviewed as a
// consensus/network change, not treated as a routine test update.

#include <chainparams.h>
#include <chainparamsbase.h>
#include <consensus/amount.h>
#include <consensus/consensus.h>
#include <consensus/params.h>
#include <kernel/chainparams.h>
#include <uint256.h>
#include <util/chaintype.h>
#include <validation.h>
#include <versionbits.h>

#include <test/util/setup_common.h>

#include <boost/test/unit_test.hpp>

#include <string>
#include <vector>

BOOST_FIXTURE_TEST_SUITE(coinwow_params_tests, BasicTestingSetup)

BOOST_AUTO_TEST_CASE(mainnet_identity)
{
    const auto params = CreateChainParams(*m_node.args, ChainType::MAIN);
    const auto& consensus = params->GetConsensus();

    // Genesis block
    BOOST_CHECK_EQUAL(consensus.hashGenesisBlock, uint256{"0000000093674d270efbac62a914cf8e90417bd7f0033abaf47653e48c671038"});
    BOOST_CHECK_EQUAL(params->GenesisBlock().hashMerkleRoot, uint256{"6c18b005f11540a0484181be1e1011503bc38afccef34e22dbe871495de0075a"});
    BOOST_CHECK_EQUAL(params->GenesisBlock().nTime, 1777062967U);
    BOOST_CHECK_EQUAL(params->GenesisBlock().nNonce, 3426524174U);
    BOOST_CHECK_EQUAL(params->GenesisBlock().nBits, 0x1d00ffffU);
    BOOST_CHECK_EQUAL(params->GenesisBlock().nVersion, 1);

    // P2P identity
    const MessageStartChars expected_magic{0xc1, 0x0f, 0xe3, 0xa9};
    BOOST_CHECK(params->MessageStart() == expected_magic);
    BOOST_CHECK_EQUAL(params->GetDefaultPort(), 51445);
    BOOST_CHECK_EQUAL(CreateBaseChainParams(ChainType::MAIN)->RPCPort(), 51446);

    // Address and key encodings
    BOOST_CHECK(params->Base58Prefix(CChainParams::PUBKEY_ADDRESS) == std::vector<unsigned char>{28});
    BOOST_CHECK(params->Base58Prefix(CChainParams::SCRIPT_ADDRESS) == std::vector<unsigned char>{88});
    BOOST_CHECK(params->Base58Prefix(CChainParams::SECRET_KEY) == std::vector<unsigned char>{156});
    BOOST_CHECK((params->Base58Prefix(CChainParams::EXT_PUBLIC_KEY) == std::vector<unsigned char>{0x04, 0x88, 0xB2, 0x1E}));
    BOOST_CHECK((params->Base58Prefix(CChainParams::EXT_SECRET_KEY) == std::vector<unsigned char>{0x04, 0x88, 0xAD, 0xE4}));
    BOOST_CHECK_EQUAL(params->Bech32HRP(), "cw");
}

BOOST_AUTO_TEST_CASE(mainnet_consensus_rules)
{
    const auto params = CreateChainParams(*m_node.args, ChainType::MAIN);
    const auto& consensus = params->GetConsensus();

    // Money supply
    BOOST_CHECK_EQUAL(MAX_MONEY, 10'000'000 * COIN);
    BOOST_CHECK_EQUAL(consensus.nSubsidyHalvingInterval, 200'000);
    BOOST_CHECK_EQUAL(GetBlockSubsidy(0, consensus), 25 * COIN);
    BOOST_CHECK_EQUAL(GetBlockSubsidy(199'999, consensus), 25 * COIN);
    BOOST_CHECK_EQUAL(GetBlockSubsidy(200'000, consensus), 25 * COIN / 2);
    BOOST_CHECK_EQUAL(COINBASE_MATURITY, 100);

    // Proof of work
    BOOST_CHECK_EQUAL(consensus.powLimit, uint256{"00000000ffffffffffffffffffffffffffffffffffffffffffffffffffffffff"});
    BOOST_CHECK_EQUAL(consensus.nPowTargetSpacing, 60);
    BOOST_CHECK_EQUAL(consensus.nPowTargetTimespan, 2016 * 60);
    BOOST_CHECK_EQUAL(consensus.DifficultyAdjustmentInterval(), 2016);
    BOOST_CHECK(!consensus.fPowAllowMinDifficultyBlocks);
    BOOST_CHECK(!consensus.fPowNoRetargeting);
    BOOST_CHECK(!consensus.enforce_BIP94);

    // Buried deployments. These heights are inherited from Bitcoin and are
    // shared by every COINWOW release so far; the BIP65/BIP66/CSV/BIP34 rules
    // activate for everyone at these heights. Moving any of them is a
    // consensus change (see doc/coinwow/consensus-audit.md).
    BOOST_CHECK_EQUAL(consensus.BIP34Height, 227931);
    BOOST_CHECK_EQUAL(consensus.BIP66Height, 363725);
    BOOST_CHECK_EQUAL(consensus.BIP65Height, 388381);
    BOOST_CHECK_EQUAL(consensus.CSVHeight, 419328);
    BOOST_CHECK_EQUAL(consensus.SegwitHeight, 0);
    BOOST_CHECK_EQUAL(consensus.MinBIP9WarningHeight, 483840);

    // Taproot is enforced unconditionally by block script flags; the BIP9
    // entry only affects RPC reporting and must say "always active".
    const auto& taproot = consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT];
    BOOST_CHECK_EQUAL(taproot.nStartTime, Consensus::BIP9Deployment::ALWAYS_ACTIVE);
    BOOST_CHECK_EQUAL(taproot.nTimeout, Consensus::BIP9Deployment::NO_TIMEOUT);
}

BOOST_AUTO_TEST_CASE(testnets_do_not_collide_with_mainnet_or_bitcoin)
{
    const MessageStartChars bitcoin_main{0xf9, 0xbe, 0xb4, 0xd9};
    const MessageStartChars bitcoin_testnet3{0x0b, 0x11, 0x09, 0x07};
    const MessageStartChars bitcoin_testnet4{0x1c, 0x16, 0x3f, 0x28};
    const auto main = CreateChainParams(*m_node.args, ChainType::MAIN);

    for (const ChainType chain : {ChainType::TESTNET, ChainType::TESTNET4, ChainType::SIGNET}) {
        const auto p = CreateChainParams(*m_node.args, chain);
        BOOST_CHECK(p->MessageStart() != main->MessageStart());
        BOOST_CHECK(p->MessageStart() != bitcoin_main);
        BOOST_CHECK(p->GetDefaultPort() != main->GetDefaultPort());
        BOOST_CHECK(p->GetConsensus().hashGenesisBlock != main->GetConsensus().hashGenesisBlock);
        // No inherited Bitcoin test-network seeds or chain data.
        BOOST_CHECK(p->DNSSeeds().empty());
        BOOST_CHECK(p->FixedSeeds().empty());
        BOOST_CHECK(p->GetConsensus().nMinimumChainWork == uint256{});
        BOOST_CHECK(p->GetConsensus().defaultAssumeValid == uint256{});
    }
    const auto testnet3 = CreateChainParams(*m_node.args, ChainType::TESTNET);
    const auto testnet4 = CreateChainParams(*m_node.args, ChainType::TESTNET4);
    BOOST_CHECK(testnet3->MessageStart() != bitcoin_testnet3);
    BOOST_CHECK(testnet4->MessageStart() != bitcoin_testnet4);
    BOOST_CHECK(testnet3->MessageStart() != testnet4->MessageStart());
}

BOOST_AUTO_TEST_SUITE_END()
