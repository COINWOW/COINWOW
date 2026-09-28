#!/usr/bin/env python3
# Copyright (c) 2022-present The Bitcoin Core developers
# Copyright (c) 2024-present COINWOW Developers
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.

import subprocess

from test_framework.test_framework import COINWOWTestFramework

class COINWOWChainstateTest(COINWOWTestFramework):
    def skip_test_if_missing_module(self):
        self.skip_if_no_coinwow_chainstate()

    def set_test_params(self):
        self.setup_clean_chain = True
        self.chain = ""
        self.num_nodes = 1
        # Set prune to avoid disk space warning.
        self.extra_args = [["-prune=550"]]

    def add_block(self, datadir, input, expected_stderr):
        proc = subprocess.Popen(
            self.get_binaries().chainstate_argv() + [datadir],
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True
        )
        stdout, stderr = proc.communicate(input=input + "\n", timeout=5)
        self.log.debug("STDOUT: {0}".format(stdout.strip("\n")))
        self.log.info("STDERR: {0}".format(stderr.strip("\n")))

        if expected_stderr not in stderr:
            raise AssertionError(f"Expected stderr output {expected_stderr} does not partially match stderr:\n{stderr}")

    def run_test(self):
        node = self.nodes[0]
        datadir = node.cli.datadir
        node.stop_node()

        self.log.info(f"Testing coinwow-chainstate {self.get_binaries().chainstate_argv()} with datadir: {datadir}")
        # COINWOW mainnet block at height 1 built on the COINWOW genesis block
        # (valid proof of work at difficulty 1, coinbase paying 0 to OP_TRUE).
        # It is a test-only block, not the block 1 of the live chain.
        # The previous fixture was Bitcoin's block 1, whose parent (Bitcoin's
        # genesis) does not exist on COINWOW.
        block_one = "000000203810678ce45376f4ba3a03f0d77b41908ecf14a962acfb0e274d67930000000033d9596eac476d9f5cf157e5a4b19ddeb32c2874159a11e5e0877255a09699438fd6eb69ffff001d002e648c0101000000010000000000000000000000000000000000000000000000000000000000000000ffffffff33010130434f494e574f5720636f696e776f772d636861696e73746174652066756e6374696f6e616c207465737420626c6f636bffffffff010000000000000000015100000000"
        self.add_block(datadir, block_one, "Block has not yet been rejected")
        self.add_block(datadir, block_one, "duplicate")
        self.add_block(datadir, "00", "Block decode failed")
        self.add_block(datadir, "", "Empty line found")

if __name__ == "__main__":
    COINWOWChainstateTest(__file__).main()
