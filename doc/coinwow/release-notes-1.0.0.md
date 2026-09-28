# COINWOW Core v1.0.0 — release notes (DRAFT)

> **Draft.** v1.0.0 is not tagged or published yet. The download location
> will be https://github.com/COINWOW/COINWOW/releases once the release is
> approved. Do not link to assets that do not exist yet.

COINWOW Core v1.0.0 is the first official, cleaned-up release of the COINWOW
reference node and wallet. Earlier public builds identified themselves as
29.99.0 (a Bitcoin Core development version number).

## Compatibility

- **Same network, same chain.** v1.0.0 follows exactly the same consensus
  rules as the 29.99.0 builds. Genesis, block validity, subsidy
  (25 CW, halving every 200,000 blocks, 10,000,000 CW cap), difficulty
  adjustment, ports (P2P 51445, RPC 51446), address formats (`C…`/`c…`
  Base58 and `cw1…` Bech32), message start and protocol version (70016) are
  unchanged. Old and new nodes interoperate: a transaction was sent from a
  29.99 node to a v1.0.0 wallet on mainnet, then relayed, mined and
  confirmed (txid
  `5bff01ddd1763a8894a7d1c93ba13f6dd2eda8df0234d78d718abe52bb1e5b49`).
- **Wallets.** Wallets created by 29.99 builds stored extended keys with
  testnet-style `tpub`/`tprv` prefixes. v1.0.0 writes new mainnet keys with
  standard `xpub`/`xprv` prefixes and **reads both**. Existing descriptors
  are preserved exactly as stored (same descriptor text and ID), so old
  wallets keep deriving the same addresses. Always back up `wallet.dat`
  before upgrading.
- **User agent** is now `/WOW:1.0.0/` (was `/WOW:29.99.0/`).

## Notable changes since 29.99.0

- Fixed: with the default configuration, `coinwowd` failed to start when
  listening for inbound connections ("Unable to bind to 127.0.0.1:51446 …
  Failed to listen on any port"). The automatic Tor onion listener now uses
  127.0.0.1:51447 on mainnet instead of colliding with the RPC port.
- Fixed: the first-run dialog and the startup disk check claimed that
  about 720 GB of data would be stored (Bitcoin's figure). The estimate is
  now 1 GB.
- Fixed: `getdeploymentinfo` reported Taproot as inactive, although Taproot
  rules have been enforced since the genesis block. It now reports it as
  active. Validation is unchanged.
- Legacy `tpub`/`tprv` wallet compatibility (see above).
- Removed Bitcoin mainnet/testnet/signet AssumeUTXO snapshots, Bitcoin
  fixed seeds and DNS seeds. Mainnet uses the COINWOW DNS seeds
  `seed.coinwow.ca` and `seed2.coinwow.ca` and four COINWOW fixed seeds.
- testnet3, testnet4 and signet now have their own message start and ports
  (51455, 51465, 51475) and no inherited Bitcoin seeds or chain data.
  COINWOW does not operate public test networks yet.
- Security reporting now goes through GitHub private vulnerability reports
  (see `SECURITY.md`).
- Provenance clean-up: the historical Bitcoin Core release notes, vendored
  libraries (libsecp256k1, minisketch, leveldb, ctaes, univalue) and
  upstream links carry their original Bitcoin attribution again. The
  inherited Bitcoin Core code-signing certificate was removed.

## Known limitations

- **Timelocks and strict DER signatures are not consensus-enforced yet.**
  BIP34, BIP66, BIP65 (CHECKLOCKTIMEVERIFY) and CSV (BIP68/112/113) activate
  at the inherited heights 227,931 / 363,725 / 388,381 / 419,328, which the
  chain is expected to reach during 2027. Until then these rules are
  enforced only as relay policy, so do not rely on CLTV/CSV timelocks for
  value you cannot afford to lose to a malicious miner. SegWit and Taproot
  are fully enforced. See `doc/coinwow/consensus-audit.md`.
- Binaries are **not code-signed** yet (Windows Authenticode, macOS
  notarization), so operating-system warnings are expected. Verify
  downloads against the published SHA256SUMS.

## Upgrading

1. Shut down the old node or wallet cleanly.
2. Back up `wallet.dat`, or the whole `wallets/` directory.
3. Install v1.0.0 and start it with the same data directory. No reindex is
   required.
