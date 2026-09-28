# COINWOW Core (CW)

COINWOW Core is the reference implementation of the COINWOW cryptocurrency:
a full node that connects to the COINWOW peer-to-peer network, validates
every block and transaction, and includes a wallet with a graphical
interface.

- Website: https://coinwow.ca
- Source code: https://github.com/COINWOW/COINWOW
- Releases: https://github.com/COINWOW/COINWOW/releases

COINWOW is an independent blockchain derived from
[Bitcoin Core](https://github.com/bitcoin/bitcoin). It keeps Bitcoin's
SHA256d proof of work, UTXO model and script system, with its own genesis
block, network identity and monetary parameters.

## Network parameters (mainnet)

| Parameter | Value |
|---|---|
| Ticker | CW |
| Proof of work | SHA256d |
| Target block time | 60 seconds |
| Difficulty adjustment | every 2016 blocks (≈ 33.6 hours), ±4× |
| Block reward | 25 CW, halving every 200,000 blocks |
| Maximum supply | 10,000,000 CW (asymptotically 9,999,999.978 CW) |
| Coinbase maturity | 100 blocks |
| Genesis block | `0000000093674d270efbac62a914cf8e90417bd7f0033abaf47653e48c671038` (2026-04-24) |
| P2P port | 51445 |
| RPC port | 51446 (bind to localhost only) |
| Addresses | Base58 `C…` (P2PKH) / `c…` (P2SH); Bech32 `cw1…` |
| Extended keys | `xpub` / `xprv` (legacy `tpub` / `tprv` still readable) |
| DNS seeds | `seed.coinwow.ca`, `seed2.coinwow.ca` |
| SegWit / Taproot | active since genesis |

Full details and the soft-fork schedule are in
[doc/coinwow/consensus-audit.md](doc/coinwow/consensus-audit.md).

## Components

- `coinwowd`: full node and RPC server
- `coinwow-qt`: graphical node and wallet
- `coinwow-cli`: RPC command-line client
- `coinwow-wallet`: offline wallet tool
- `coinwow-tx`, `coinwow-util`: transaction and utility tools

## Building

COINWOW Core uses CMake, like Bitcoin Core. On Linux:

```sh
cmake -B build
cmake --build build -j"$(nproc)"
ctest --test-dir build        # unit tests
build/test/functional/test_runner.py   # functional tests
```

See [doc/build-unix.md](doc/build-unix.md),
[doc/build-windows-msvc.md](doc/build-windows-msvc.md) and
[doc/build-osx.md](doc/build-osx.md) for dependencies and
platform-specific instructions. On macOS, the `.app` bundle is produced by the
`deploy` target (`cmake --build build --target deploy`), not by the default
build.

## Security

Please report vulnerabilities privately; see [SECURITY.md](SECURITY.md).

## License

COINWOW Core is released under the MIT license; see [COPYING](COPYING).
Portions are copyright The Bitcoin Core developers.

COINWOW Core is experimental software. Keep backups of your wallets and use
it at your own risk.
