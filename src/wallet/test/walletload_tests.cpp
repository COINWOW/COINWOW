// Copyright (c) 2022 The Bitcoin Core developers
// Copyright (c) 2024-present COINWOW Developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or https://www.opensource.org/licenses/mit-license.php.

#include <wallet/test/util.h>
#include <wallet/wallet.h>
#include <base58.h>
#include <crypto/sha256.h>
#include <key_io.h>
#include <script/descriptor.h>
#include <streams.h>
#include <test/util/logging.h>
#include <test/util/setup_common.h>

#include <boost/test/unit_test.hpp>

namespace wallet {

BOOST_AUTO_TEST_SUITE(walletload_tests)

class DummyDescriptor final : public Descriptor {
private:
    std::string desc;
public:
    explicit DummyDescriptor(const std::string& descriptor) : desc(descriptor) {};
    ~DummyDescriptor() = default;

    std::string ToString(bool compat_format) const override { return desc; }
    std::string ToLegacyExtCompatString() const override { return desc; }
    std::optional<OutputType> GetOutputType() const override { return OutputType::UNKNOWN; }

    bool IsRange() const override { return false; }
    bool IsSolvable() const override { return false; }
    bool IsSingleType() const override { return true; }
    bool ToPrivateString(const SigningProvider& provider, std::string& out) const override { return false; }
    bool ToNormalizedString(const SigningProvider& provider, std::string& out, const DescriptorCache* cache = nullptr) const override { return false; }
    bool Expand(int pos, const SigningProvider& provider, std::vector<CScript>& output_scripts, FlatSigningProvider& out, DescriptorCache* write_cache = nullptr) const override { return false; };
    bool ExpandFromCache(int pos, const DescriptorCache& read_cache, std::vector<CScript>& output_scripts, FlatSigningProvider& out) const override { return false; }
    void ExpandPrivate(int pos, const SigningProvider& provider, FlatSigningProvider& out) const override {}
    std::optional<int64_t> ScriptSize() const override { return {}; }
    std::optional<int64_t> MaxSatisfactionWeight(bool) const override { return {}; }
    std::optional<int64_t> MaxSatisfactionElems() const override { return {}; }
    void GetPubKeys(std::set<CPubKey>& pubkeys, std::set<CExtPubKey>& ext_pubs) const override {}
};

BOOST_FIXTURE_TEST_CASE(wallet_load_descriptors, TestingSetup)
{
    std::unique_ptr<WalletDatabase> database = CreateMockableWalletDatabase();
    {
        // Write unknown active descriptor
        WalletBatch batch(*database);
        std::string unknown_desc = "trx(tpubD6NzVbkrYhZ4Y4S7m6Y5s9GD8FqEMBy56AGphZXuagajudVZEnYyBahZMgHNCTJc2at82YX6s8JiL1Lohu5A3v1Ur76qguNH4QVQ7qYrBQx/86'/1'/0'/0/*)#8pn8tzdt";
        WalletDescriptor wallet_descriptor(std::make_shared<DummyDescriptor>(unknown_desc), 0, 0, 0, 0);
        BOOST_CHECK(batch.WriteDescriptor(uint256(), wallet_descriptor));
        BOOST_CHECK(batch.WriteActiveScriptPubKeyMan(static_cast<uint8_t>(OutputType::UNKNOWN), uint256(), false));
    }

    {
        // Now try to load the wallet and verify the error.
        const std::shared_ptr<CWallet> wallet(new CWallet(m_node.chain.get(), "", std::move(database)));
        BOOST_CHECK_EQUAL(wallet->LoadWallet(), DBErrors::UNKNOWN_DESCRIPTOR);
    }

    // Test 2
    // Now write a valid descriptor with an invalid ID.
    // As the software produces another ID for the descriptor, the loading process must be aborted.
    database = CreateMockableWalletDatabase();

    // Verify the error
    bool found = false;
    DebugLogHelper logHelper("The descriptor ID calculated by the wallet differs from the one in DB", [&](const std::string* s) {
        found = true;
        return false;
    });

    {
        // Write valid descriptor with invalid ID
        WalletBatch batch(*database);
        std::string desc = "wpkh([d34db33f/84h/0h/0h]xpub6DJ2dNUysrn5Vt36jH2KLBT2i1auw1tTSSomg8PhqNiUtx8QX2SvC9nrHu81fT41fvDUnhMjEzQgXnQjKEu3oaqMSzhSrHMxyyoEAmUHQbY/0/*)#cjjspncu";
        WalletDescriptor wallet_descriptor(std::make_shared<DummyDescriptor>(desc), 0, 0, 0, 0);
        BOOST_CHECK(batch.WriteDescriptor(uint256::ONE, wallet_descriptor));
    }

    {
        // Now try to load the wallet and verify the error.
        const std::shared_ptr<CWallet> wallet(new CWallet(m_node.chain.get(), "", std::move(database)));
        BOOST_CHECK_EQUAL(wallet->LoadWallet(), DBErrors::CORRUPT);
        BOOST_CHECK(found); // The error must be logged
    }
}

namespace {
// Public BIP32 test vector 1 master xpub (throwaway key, never a real wallet key).
const std::string TEST_XPUB{"xpub661MyMwAqRbcFtXgS5sYJABqqG9YLmC4Q1Rdap9gSE8NqtwybGhePY2gZ29ESFjqJoCu1Rupje8YtGqsefD265TMg7usUDFdp6W1EGMcet8"};

//! tpub encoding of an xpub, built from hardcoded version bytes independently of the code under test.
std::string MakeTpub(const std::string& xpub)
{
    const CExtPubKey pubkey = DecodeExtPubKey(xpub);
    std::vector<unsigned char> data{0x04, 0x35, 0x87, 0xCF};
    data.resize(data.size() + BIP32_EXTKEY_SIZE);
    pubkey.Encode(data.data() + 4);
    return EncodeBase58Check(data);
}

std::string WithChecksum(const std::string& desc) { return desc + "#" + GetDescriptorChecksum(desc); }

uint256 HashOf(const std::string& str)
{
    uint256 id;
    CSHA256().Write(reinterpret_cast<const unsigned char*>(str.data()), str.size()).Finalize(id.begin());
    return id;
}

//! Database record value of a WalletDescriptor: text, creation_time, next_index, range_start, range_end.
DataStream MakeDescriptorRecord(const std::string& text)
{
    DataStream ss;
    ss << text << uint64_t{1234} << int32_t{5} << int32_t{0} << int32_t{10};
    return ss;
}
} // namespace

BOOST_FIXTURE_TEST_CASE(walletdescriptor_legacy_tpub_id, BasicTestingSetup)
{
    const std::string legacy_text{WithChecksum("wpkh(" + MakeTpub(TEST_XPUB) + "/0/*)")};
    const std::string current_text{WithChecksum("wpkh(" + TEST_XPUB + "/0/*)")};
    // The historical ID is the hash of the tpub-encoded compat string, the current one of the xpub-encoded string.
    const uint256 legacy_id{HashOf(legacy_text)};
    const uint256 current_id{HashOf(current_text)};
    BOOST_REQUIRE(legacy_id != current_id);

    // A legacy (tpub) descriptor parses, and keeps its exact stored text.
    WalletDescriptor legacy;
    DataStream legacy_in = MakeDescriptorRecord(legacy_text);
    legacy_in >> legacy;
    BOOST_REQUIRE(legacy.descriptor);
    BOOST_CHECK(legacy.legacy_text == legacy_text);
    BOOST_CHECK(legacy.id == current_id);
    BOOST_CHECK(DescriptorID(*legacy.descriptor) == current_id);
    BOOST_CHECK(LegacyDescriptorID(*legacy.descriptor) == legacy_id);
    // Regular string forms are xpub-encoded.
    BOOST_CHECK(legacy.descriptor->ToString() == current_text);

    // Only the exact historical ID is adopted; anything else is refused and leaves the ID untouched.
    uint256 flipped{legacy_id};
    flipped.begin()[0] ^= 1;
    BOOST_CHECK(!legacy.AdoptLegacyId(current_id));
    BOOST_CHECK(!legacy.AdoptLegacyId(uint256::ONE));
    BOOST_CHECK(!legacy.AdoptLegacyId(flipped));
    BOOST_CHECK(legacy.id == current_id);
    BOOST_CHECK(legacy.AdoptLegacyId(legacy_id));
    BOOST_CHECK(legacy.id == legacy_id);

    // The stored representation is written back byte for byte (no silent tpub -> xpub rewrite), also after a copy.
    DataStream out;
    out << legacy;
    BOOST_CHECK(out.str() == MakeDescriptorRecord(legacy_text).str());
    const WalletDescriptor copy = legacy;
    DataStream copy_out;
    copy_out << copy;
    BOOST_CHECK(copy.id == legacy_id);
    BOOST_CHECK(copy_out.str() == out.str());

    // A regular xpub descriptor is unaffected: no legacy text, current ID, and the legacy ID is never adopted.
    WalletDescriptor current;
    DataStream current_in = MakeDescriptorRecord(current_text);
    current_in >> current;
    BOOST_REQUIRE(current.descriptor);
    BOOST_CHECK(current.legacy_text.empty());
    BOOST_CHECK(current.id == current_id);
    BOOST_CHECK(!current.AdoptLegacyId(legacy_id));
    BOOST_CHECK(current.id == current_id);
    DataStream current_out;
    current_out << current;
    BOOST_CHECK(current_out.str() == MakeDescriptorRecord(current_text).str());
}

BOOST_FIXTURE_TEST_CASE(wallet_load_legacy_tpub_descriptor, TestingSetup)
{
    const std::string legacy_text{WithChecksum("wpkh(" + MakeTpub(TEST_XPUB) + "/0/*)")};
    const uint256 legacy_id{HashOf(legacy_text)};

    WalletDescriptor stored;
    DataStream stored_in = MakeDescriptorRecord(legacy_text);
    stored_in >> stored;
    DataStream stored_bytes;
    stored_bytes << stored;
    const SerializeData expected_value(stored_bytes.begin(), stored_bytes.end());

    // Database as written by a version that encoded mainnet extended keys as tpub.
    std::unique_ptr<WalletDatabase> database = CreateMockableWalletDatabase();
    {
        WalletBatch batch(*database);
        BOOST_CHECK(batch.WriteDescriptor(legacy_id, stored));
    }
    MockableDatabase& mock_db = static_cast<MockableDatabase&>(*database);

    // The historical ID passes the (unchanged) ID verification and the descriptor loads.
    const std::shared_ptr<CWallet> wallet(new CWallet(m_node.chain.get(), "", std::move(database)));
    const DBErrors res = wallet->LoadWallet();
    BOOST_CHECK(res != DBErrors::CORRUPT);
    BOOST_CHECK(res != DBErrors::UNKNOWN_DESCRIPTOR);
    BOOST_CHECK(wallet->GetScriptPubKeyMan(legacy_id) != nullptr);

    // Loading did not rewrite the stored descriptor.
    bool found{false};
    for (const auto& [key, value] : mock_db.m_records) {
        if (value == expected_value) found = true;
    }
    BOOST_CHECK(found);
}

BOOST_AUTO_TEST_SUITE_END()
} // namespace wallet
