# COINWOW consensus and soft-fork audit (September 2026)

Scope: COINWOW Core source at branch `release/v1.0.0-prep`, based on
`fix/coincontrol-fuzz-v2` @ `a6f9b42`. Mainnet has been live since the genesis
block of 2026-04-24; around height 51,093 on 2026-09-13 the chain was
producing roughly 1,440 blocks/day (the 48384–50399 retarget window took
118,350 s against a 120,960 s target).

Method: read the consensus code paths (`validation.cpp`, `pow.cpp`,
`consensus/*`, `script/interpreter.cpp`, `deploymentstatus.h`,
`versionbits.cpp`, `kernel/chainparams.cpp`) and diff them against upstream
Bitcoin Core v29.0 after normalising the Bitcoin→COINWOW rename. Apart from
the parameters listed below, the only COINWOW-specific consensus changes are
`MAX_MONEY` and the initial subsidy in `GetBlockSubsidy`. The other diffs
match upstream refactors made after 29.0 (`std::span`, removal of
checkpoints, walking back from invalid blocks, and so on).

Legend: **[test]** proven by an automated test in this repository,
**[code]** verified by reading the code, **[assumption]** needs
confirmation on the live network.

## 1. Supply, subsidy, halving

| Item | Value | Evidence |
|---|---|---|
| `MAX_MONEY` | 10,000,000 CW | [test] `coinwow_params_tests/mainnet_consensus_rules` |
| Initial subsidy | 25 CW | [test] same |
| Halving interval | 200,000 blocks | [test] same, `validation_tests/block_subsidy_test` |
| Exact asymptotic issuance | 999,999,997,800,000 sat = 9,999,999.978 CW | [test] `validation_tests/subsidy_limit_test` |
| Genesis output | 50 CW, never spendable (genesis coinbase is not added to the UTXO set) | [code] |
| First halving | height 200,000 (about late December 2026 at 1,440 blocks/day) | [code] |
| Coinbase maturity | 100 blocks (about 100 minutes) | [test] |

## 2. Proof of work

- SHA256d, `powLimit` = `00000000ffff…` (difficulty 1), target spacing 60 s,
  retarget every 2016 blocks (33.6 h), ±4× clamp: **[test]** values,
  **[code]** algorithm identical to Bitcoin Core (`pow.cpp` normalised diff
  is empty apart from includes and comments).
- `fPowAllowMinDifficultyBlocks = false`, `fPowNoRetargeting = false`.
- `enforce_BIP94 = false`: the timewarp fix is not enforced. Bitcoin mainnet
  is in the same situation. A miner with majority hash rate could
  manipulate timestamps to lower difficulty. This is a candidate for a
  future coordinated soft fork, not a v1.0.0 blocker.
- Sample check from the live network: retarget 48384→50399 took 118,350 s
  against 120,960 s, and difficulty moved from about 67,326.65 to 68,812.01. This is
  consistent with the formula. **[assumption]** based on data reported
  from Node1, not re-queried here.

## 3. Soft-fork deployment state

| Rule | Mainnet parameter | State today | Evidence |
|---|---|---|---|
| BIP16 P2SH | always on | active since genesis | [code] `GetBlockScriptFlags` starts from `P2SH\|WITNESS\|TAPROOT`; the two `script_flag_exceptions` hashes are Bitcoin blocks and cannot occur on COINWOW |
| SegWit (BIP141/143/147) | `SegwitHeight = 0` | active since genesis, including NULLDUMMY and witness commitment rules | [code] + [test] `getdeploymentinfo` on a fresh mainnet datadir: `segwit active, height 0` |
| Taproot (BIP340-342) | script flag always set | **enforced since genesis** | [code] `SCRIPT_VERIFY_TAPROOT` is set unconditionally. Before this branch `getdeploymentinfo` reported taproot as BIP9 `defined`/inactive (observed on the a6f9b42 build); fixed by commit "Report Taproot as always active on mainnet" |
| BIP34 (height in coinbase, `nVersion >= 2`) | 227,931 | **not active yet** | [code] `ContextualCheckBlock` / `ContextualCheckBlockHeader` |
| BIP66 (strict DER) | 363,725 | **not active yet** | [code] |
| BIP65 (CHECKLOCKTIMEVERIFY, `nVersion >= 4`) | 388,381 | **not active yet** | [code] |
| CSV (BIP68/112/113) | 419,328 | **not active yet** | [code] |
| `MinBIP9WarningHeight` | 483,840 | warning-only ("unknown new rules") | [code] `WarningBitsConditionChecker`; no effect on validity |

The same heights are present at tag `v1.0` (29.99.0) and in every later
commit, so **all released COINWOW binaries agree** on when these rules
activate. [code] `git show v1.0:src/kernel/chainparams.cpp`

### 3.1 What "not active yet" means in practice

Before each height, the corresponding rule is not a consensus rule, but it
is still enforced as **mempool/relay policy**. `MANDATORY_SCRIPT_VERIFY_FLAGS`
contains DERSIG, CHECKLOCKTIMEVERIFY and CHECKSEQUENCEVERIFY, and
`STANDARD_LOCKTIME_VERIFY_FLAGS` contains BIP68 sequence locks. **[code]**
Consequences:

- Nodes will not relay transactions that violate these rules, and
  `getblocktemplate` (used by the pool) will not include them.
- A miner who builds blocks outside Core could still include a transaction
  that spends a CLTV/CSV-timelocked output early, or uses a non-DER
  signature, and every node would accept that block.
- Plain P2PKH/P2WPKH/P2TR key-path spends are unaffected. The risk
  concerns only contracts that depend on timelocks (HTLCs, vaults, escrow,
  Lightning-style channels), which COINWOW does not use today.
- Before BIP34, the block version and coinbase height are not enforced.
  BIP30 (no overwriting unspent txids) stays enforced on every block,
  because `BIP34Hash` is Bitcoin's block 227,931 hash, which never matches.
  This costs a small amount of performance and is safe.

### 3.2 Projected activation dates

These are projections from the ~2026-09-13 snapshot (height 51,093). They are
**[assumption]**s that depend on hash rate.

| Height | Event | at 1,440 blocks/day | at 1,000 blocks/day |
|---|---|---|---|
| 200,000 | first halving | 2026-12-25 | 2027-02-09 |
| 227,931 | BIP34 | 2027-01-13 | 2027-03-08 |
| 363,725 | BIP66 | 2027-04-18 | 2027-07-22 |
| 388,381 | BIP65 | 2027-05-05 | 2027-08-16 |
| 419,328 | CSV | 2027-05-26 | 2027-09-16 |
| 483,840 | BIP9 unknown-version warnings start | 2027-07-10 | 2027-11-19 |

## 4. Decision memo: BIP34 / BIP66 / BIP65 / CSV

**Decision for v1.0.0: keep the inherited heights unchanged.**

Options considered:

1. **Keep the heights (chosen).** No consensus change. All old and new nodes
   activate each rule at the same height, so there is no split risk. The
   rules become enforced automatically in 2027.
   Cost: until then, timelocks and strict DER are policy-only (§3.1). This is
   documented in the release notes.
2. **Move activation to a single, nearer future height H.** This is a soft
   fork. It is safe for the chain history, because blocks below H are not
   re-validated under the new rules. However, it requires every miner (in
   practice the pool on Node1) to run the new version before H. Otherwise a
   non-upgraded miner could produce a block that upgraded nodes reject,
   splitting the network. Old 29.99/1.0.0 nodes would keep following the
   upgraded chain, because stricter blocks are valid for them. This is a
   reasonable follow-up for a v1.1 release with an announced height,
   provided it is shipped separately from the first clean release.
3. **Retroactive activation (height 0 or any mined height).** Rejected. A
   syncing node would re-validate historical blocks under rules they never
   had to follow. Any historical block that contains a non-DER signature, a
   CLTV/CSV violation, a missing coinbase height or `nVersion < 4` would
   become invalid and fork new nodes off the live chain. Mining software is
   not guaranteed to have put the BIP34 height in early coinbases, so this
   would require a full-chain audit and would still gain almost nothing.

Before BIP34 activates (≈ January 2027) you must confirm on the pool
(NOMP on Node1) that:

- block templates use `version` from `getblocktemplate` (0x20000000 or
  higher; this also satisfies BIP65's `nVersion >= 4`), and
- the coinbase `scriptSig` starts with the serialized block height, as
  required by BIP34.

If either check fails, blocks from that height on will be rejected by every
node. To check, fetch any recent block with `getblock <hash> 2` on Node1 and
look at `tx[0].vin[0].coinbase`: the first bytes must encode the height.
**[assumption]** Not verified from here (no network access to the nodes).

## 5. Other consensus-relevant parameters

| Parameter | Value | Assessment |
|---|---|---|
| `nMinimumChainWork` | 0 | **Not a consensus rule.** Because it is 0, a freshly syncing node does not use the header pre-sync anti-DoS protection until it is close to the tip, and at difficulty 1 a fake header chain is cheap to produce with SHA256 ASICs. Recommendation: set it to the real chainwork at a height a few thousand blocks below the tip before the release (`getblockheader <hash>` → `chainwork`). This needs live data, so it is left for the release checklist. |
| `defaultAssumeValid` | 0 | Performance only (script checks are skipped below the assumed-valid block). Optional; same procedure as above. |
| `chainTxData` | height ≈ 51,093 | Progress-estimate only. |
| `m_assumeutxo_data` | empty | Correct (Bitcoin snapshots were removed). |
| Checkpoints | none | Same as upstream master. |
| `MAX_BLOCK_WEIGHT` / sigops / serialized size | 4,000,000 / 80,000 / 4,000,000 | Unchanged from Bitcoin. With 60-s blocks this gives 10× Bitcoin's throughput per unit of time; acceptable at current usage. |
| `MAX_FUTURE_BLOCK_TIME` | 2 h | Unchanged. It is large relative to 60-s blocks, but changing it is a consensus change. |
| `script_flag_exceptions` | two Bitcoin block hashes | Dead entries (the hashes cannot occur on COINWOW). They are harmless and left in place to avoid touching consensus code. |
| BIP30 exception heights 91722/91812 | Bitcoin hashes | Dead code on COINWOW and harmless. |

## 6. Version numbers

- `PROTOCOL_VERSION` stays 70016, identical to 29.99, so there is no
  P2P-level change. **[code]**
- `CLIENT_VERSION` went from 299900 (29.99.0) to 10000 (1.0.0). Checked
  uses **[code]**:
  - the user agent `/WOW:1.0.0/` (informational);
  - `getnetworkinfo.version` (informational);
  - the wallet `version` record, which is rewritten on load. A 1.0.0 wallet
    still opens in 29.99, because the `minversion` gate compares feature
    levels, not `CLIENT_VERSION`;
  - `OptionsModel::checkAndMigrate`: settings stamped 299900 by 29.99 are
    greater than 10000, so the old-GUI migration block is skipped. The only
    migration inside it (dbcache 100→300 for settings below 0.13) cannot
    apply to any COINWOW user. Note that any future migration keyed on
    `CLIENT_VERSION` must account for this regression.

  No consensus or P2P effect.

## 7. Items explicitly not changed

Genesis, message start, ports, address prefixes, BIP32 prefixes, HRP,
PoW, subsidy, halving, `MAX_MONEY`, the soft-fork heights and every other
consensus value are unchanged on this branch. `src/test/coinwow_params_tests.cpp`
now fails if any of them is modified.
