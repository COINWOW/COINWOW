# Mine COINWOW

Français : [mining.fr.md](mining.fr.md)

Miners add new blocks to the COINWOW chain. Each time you find a block, you
earn 25 CW. This guide shows you how to mine on your own, with your own node
and your own payout address. Nobody else ever holds your coins.

> **Note:** There is no public community pool yet. A pool holds and pays out
> other people's coins, so it needs careful testing and a legal review first.

## COINWOW at a glance

| | |
|---|---|
| Algorithm | SHA-256d (the same as Bitcoin) |
| Block time | 60 seconds |
| Block reward | 25 CW, halved every 200,000 blocks |
| Maximum supply | 10,000,000 CW |
| Difficulty adjustment | Every 2016 blocks |
| Mined coins can be spent after | 100 blocks (about 100 minutes) |
| Payout address | Starts with `cw1` |

## What you need

- **A SHA-256 ASIC miner.** For example, an Antminer S9.
- **A computer running your own COINWOW node**, fully synchronized.
- **A Stratum server.** ASIC miners speak Stratum, not the node's RPC.
  You set one up in step 4.
- **A wired network connection** to your miner.

## Can I mine with my PC?

In theory, yes. In practice, no.

- A computer processor reaches tens of millions of hashes per second.
- A graphics card reaches about a billion.
- An ASIC like the Antminer S9 reaches several trillion.

That makes a PC thousands of times (or more) too slow to find blocks at the
current difficulty. COINWOW Core also has no built-in miner, and the project has not
tested any PC mining software.

The difficulty adjusts every 2016 blocks, so this can change if ASIC miners
leave the network.

## How long does it take to find a block?

If you were the only miner:

```
seconds per block = difficulty × 4,294,967,296 ÷ hashes per second
```

For example, at a difficulty of 68,812 and 1 TH/s (one trillion hashes per
second), that is about 295 seconds.

The difficulty changes over time. To see the current value, run:

```
coinwow-cli getmininginfo
```

> **Important:** Mining never guarantees income, and CW has no guaranteed
> value.

## Set up mining

### Step 1. Synchronize your node

Open `coinwow-qt`, or start `coinwowd`. Then run:

```
coinwow-cli getblockchaininfo
```

Wait until you see `"initialblockdownload": false`.

> **Note:** The chain only moves forward when someone mines. If nobody is
> mining, the network pauses at the last block, and your wallet may show a
> synchronization percentage that never reaches 100 %. This is normal.

### Step 2. Get a payout address

Run:

```
coinwow-cli getnewaddress "mining" bech32
```

The address starts with `cw1`. Back up your wallet **before** you start mining.
See [managing-wallets.md](../managing-wallets.md).

> **Important:** Never share your wallet file, your recovery data or your RPC
> credentials with anyone, even someone who says they work for COINWOW.

### Step 3. Let your node talk to a Stratum server

Add these lines to `coinwow.conf`
(see [coinwow-conf.md](../coinwow-conf.md)):

```
server=1
rpcbind=127.0.0.1
rpcallowip=127.0.0.1
```

Then restart your node. Use hashed credentials (`rpcauth`, see
`share/rpcauth/README.md`) or the cookie file. Avoid a plain-text
`rpcuser` and `rpcpassword`.

> **Important:** Never open the RPC port (51446) to the internet.

### Step 4. Set up a Stratum server

Install a Stratum server that builds work from your node's
`getblocktemplate` RPC. Then:

1. Point it at `127.0.0.1:51446`.
2. Enter your `cw1…` address as the payout address.
3. Make the Stratum port reachable by your miner, ideally only on your home
   network.

The COINWOW project's own test setup uses
[NOMP](https://github.com/zone117x/node-open-mining-portal) with an
Antminer S9. Other Stratum servers that work with Bitcoin Core's
`getblocktemplate` should also work, but the project has not tested them.

### Step 5. Connect your miner

Open your miner's web page. In the pool settings, enter:

```
stratum+tcp://<address of your Stratum server>:<port>
```

The worker name and password depend on your Stratum software. Many ignore the
password.

### Step 6. Check that it works

- Your miner's page shows **accepted** shares.
- `coinwow-cli getmininginfo` shows the difficulty and block height.
- When you find a block, 25 CW appears in your wallet as **immature**. It
  becomes spendable after 100 more blocks.

## Stay safe

- **Power.** An Antminer S9 draws about 1.3 kW. Use a circuit that can carry
  the load, and do not use a power bar that is not rated for it.
- **Heat and noise.** An ASIC runs hot and loud. Keep it ventilated, away from
  anything that can burn, and never leave it unattended.
- **Under 18?** Ask an adult to supervise the hardware.
- **Scams.** Nobody from COINWOW will ask you to type commands, share your
  wallet or send coins to receive coins.
- **Downloads.** COINWOW Core v1.0.0 is not code-signed yet, so Windows and
  macOS show warnings. Check your download against the published
  `SHA256SUMS`.

## For developers

If you write or run mining software:

- Use the `version` returned by `getblocktemplate`.
- The coinbase `scriptSig` must start with the serialized block height
  (BIP34). BIP34 starts to be enforced at height 227,931, which the chain is
  expected to reach during 2027.
  See [consensus-audit.md](consensus-audit.md).
