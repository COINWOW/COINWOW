# Libraries

| Name                     | Description |
|--------------------------|-------------|
| *libcoinwow_cli*         | RPC client functionality used by *coinwow-cli* executable |
| *libcoinwow_common*      | Home for common functionality shared by different executables and libraries. Similar to *libcoinwow_util*, but higher-level (see [Dependencies](#dependencies)). |
| *libcoinwow_consensus*   | Consensus functionality used by *libcoinwow_node* and *libcoinwow_wallet*. |
| *libcoinwow_crypto*      | Hardware-optimized functions for data encryption, hashing, message authentication, and key derivation. |
| *libcoinwow_kernel*      | Consensus engine and support library used for validation by *libcoinwow_node*. |
| *libcoinwowqt*           | GUI functionality used by *coinwow-qt* and *coinwow-gui* executables. |
| *libcoinwow_ipc*         | IPC functionality used by *coinwow-node*, *coinwow-wallet*, *coinwow-gui* executables to communicate when [`-DENABLE_IPC=ON`](multiprocess.md) is used. |
| *libcoinwow_node*        | P2P and RPC server functionality used by *coinwowd* and *coinwow-qt* executables. |
| *libcoinwow_util*        | Home for common functionality shared by different executables and libraries. Similar to *libcoinwow_common*, but lower-level (see [Dependencies](#dependencies)). |
| *libcoinwow_wallet*      | Wallet functionality used by *coinwowd* and *coinwow-wallet* executables. |
| *libcoinwow_wallet_tool* | Lower-level wallet functionality used by *coinwow-wallet* executable. |
| *libcoinwow_zmq*         | [ZeroMQ](../zmq.md) functionality used by *coinwowd* and *coinwow-qt* executables. |

## Conventions

- Most libraries are internal libraries and have APIs which are completely unstable! There are few or no restrictions on backwards compatibility or rules about external dependencies. An exception is *libcoinwow_kernel*, which, at some future point, will have a documented external interface.

- Generally each library should have a corresponding source directory and namespace. Source code organization is a work in progress, so it is true that some namespaces are applied inconsistently, and if you look at [`add_library(coinwow_* ...)`](../../src/CMakeLists.txt) lists you can see that many libraries pull in files from outside their source directory. But when working with libraries, it is good to follow a consistent pattern like:

  - *libcoinwow_node* code lives in `src/node/` in the `node::` namespace
  - *libcoinwow_wallet* code lives in `src/wallet/` in the `wallet::` namespace
  - *libcoinwow_ipc* code lives in `src/ipc/` in the `ipc::` namespace
  - *libcoinwow_util* code lives in `src/util/` in the `util::` namespace
  - *libcoinwow_consensus* code lives in `src/consensus/` in the `Consensus::` namespace

## Dependencies

- Libraries should minimize what other libraries they depend on, and only reference symbols following the arrows shown in the dependency graph below:

<table><tr><td>

```mermaid

%%{ init : { "flowchart" : { "curve" : "basis" }}}%%

graph TD;

coinwow-cli[coinwow-cli]-->libcoinwow_cli;

coinwowd[coinwowd]-->libcoinwow_node;
coinwowd[coinwowd]-->libcoinwow_wallet;

coinwow-qt[coinwow-qt]-->libcoinwow_node;
coinwow-qt[coinwow-qt]-->libcoinwowqt;
coinwow-qt[coinwow-qt]-->libcoinwow_wallet;

coinwow-wallet[coinwow-wallet]-->libcoinwow_wallet;
coinwow-wallet[coinwow-wallet]-->libcoinwow_wallet_tool;

libcoinwow_cli-->libcoinwow_util;
libcoinwow_cli-->libcoinwow_common;

libcoinwow_consensus-->libcoinwow_crypto;

libcoinwow_common-->libcoinwow_consensus;
libcoinwow_common-->libcoinwow_crypto;
libcoinwow_common-->libcoinwow_util;

libcoinwow_kernel-->libcoinwow_consensus;
libcoinwow_kernel-->libcoinwow_crypto;
libcoinwow_kernel-->libcoinwow_util;

libcoinwow_node-->libcoinwow_consensus;
libcoinwow_node-->libcoinwow_crypto;
libcoinwow_node-->libcoinwow_kernel;
libcoinwow_node-->libcoinwow_common;
libcoinwow_node-->libcoinwow_util;

libcoinwowqt-->libcoinwow_common;
libcoinwowqt-->libcoinwow_util;

libcoinwow_util-->libcoinwow_crypto;

libcoinwow_wallet-->libcoinwow_common;
libcoinwow_wallet-->libcoinwow_crypto;
libcoinwow_wallet-->libcoinwow_util;

libcoinwow_wallet_tool-->libcoinwow_wallet;
libcoinwow_wallet_tool-->libcoinwow_util;

classDef bold stroke-width:2px, font-weight:bold, font-size: smaller;
class coinwow-qt,coinwowd,coinwow-cli,coinwow-wallet bold
```
</td></tr><tr><td>

**Dependency graph**. Arrows show linker symbol dependencies. *Crypto* lib depends on nothing. *Util* lib is depended on by everything. *Kernel* lib depends only on consensus, crypto, and util.

</td></tr></table>

- The graph shows what _linker symbols_ (functions and variables) from each library other libraries can call and reference directly, but it is not a call graph. For example, there is no arrow connecting *libcoinwow_wallet* and *libcoinwow_node* libraries, because these libraries are intended to be modular and not depend on each other's internal implementation details. But wallet code is still able to call node code indirectly through the `interfaces::Chain` abstract class in [`interfaces/chain.h`](../../src/interfaces/chain.h) and node code calls wallet code through the `interfaces::ChainClient` and `interfaces::Chain::Notifications` abstract classes in the same file. In general, defining abstract classes in [`src/interfaces/`](../../src/interfaces/) can be a convenient way of avoiding unwanted direct dependencies or circular dependencies between libraries.

- *libcoinwow_crypto* should be a standalone dependency that any library can depend on, and it should not depend on any other libraries itself.

- *libcoinwow_consensus* should only depend on *libcoinwow_crypto*, and all other libraries besides *libcoinwow_crypto* should be allowed to depend on it.

- *libcoinwow_util* should be a standalone dependency that any library can depend on, and it should not depend on other libraries except *libcoinwow_crypto*. It provides basic utilities that fill in gaps in the C++ standard library and provide lightweight abstractions over platform-specific features. Since the util library is distributed with the kernel and is usable by kernel applications, it shouldn't contain functions that external code shouldn't call, like higher level code targeted at the node or wallet. (*libcoinwow_common* is a better place for higher level code, or code that is meant to be used by internal applications only.)

- *libcoinwow_common* is a home for miscellaneous shared code used by different COINWOW Core applications. It should not depend on anything other than *libcoinwow_util*, *libcoinwow_consensus*, and *libcoinwow_crypto*.

- *libcoinwow_kernel* should only depend on *libcoinwow_util*, *libcoinwow_consensus*, and *libcoinwow_crypto*.

- The only thing that should depend on *libcoinwow_kernel* internally should be *libcoinwow_node*. GUI and wallet libraries *libcoinwowqt* and *libcoinwow_wallet* in particular should not depend on *libcoinwow_kernel* and the unneeded functionality it would pull in, like block validation. To the extent that GUI and wallet code need scripting and signing functionality, they should be able to get it from *libcoinwow_consensus*, *libcoinwow_common*, *libcoinwow_crypto*, and *libcoinwow_util*, instead of *libcoinwow_kernel*.

- GUI, node, and wallet code internal implementations should all be independent of each other, and the *libcoinwowqt*, *libcoinwow_node*, *libcoinwow_wallet* libraries should never reference each other's symbols. They should only call each other through [`src/interfaces/`](../../src/interfaces/) abstract interfaces.

## Work in progress

- Validation code is moving from *libcoinwow_node* to *libcoinwow_kernel* as part of [The libcoinwowkernel Project #27587](https://github.com/coinwow/coinwow/issues/27587)
