#!/usr/bin/env python3
# Copyright (c) 2025 The COINWOW Core developers
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.
"""Test getmininginfo difficulty reporting on COINWOW mainnet parameters.

The functional test verifies current and next-block reporting from the
COINWOW mainnet genesis. The first retarget-period ancestor walk and the
maximum difficulty adjustment are covered by pow_tests.cpp without requiring
2015 genuine difficulty-1 proof-of-work blocks.
"""

from test_framework.blocktools import (
    DIFF_1_N_BITS,
    DIFF_1_TARGET,
    nbits_str,
    target_str,
)
from test_framework.test_framework import COINWOWTestFramework
from test_framework.util import assert_equal


class MiningMainnetTest(COINWOWTestFramework):
    def set_test_params(self):
        self.num_nodes = 1
        self.setup_clean_chain = True
        self.chain = ""

    def run_test(self):
        node = self.nodes[0]

        node.stderr.seek(0)
        node.stderr.truncate()

        assert_equal(node.getblockcount(), 0)

        self.log.info("Check genesis difficulty reporting")
        mining_info = node.getmininginfo()
        assert_equal(mining_info["blocks"], 0)
        assert_equal(mining_info["difficulty"], 1)
        assert_equal(mining_info["bits"], nbits_str(DIFF_1_N_BITS))
        assert_equal(mining_info["target"], target_str(DIFF_1_TARGET))

        self.log.info("Check next-block difficulty prediction")
        assert_equal(mining_info["next"]["height"], 1)
        assert_equal(mining_info["next"]["difficulty"], 1)
        assert_equal(mining_info["next"]["bits"], nbits_str(DIFF_1_N_BITS))
        assert_equal(mining_info["next"]["target"], target_str(DIFF_1_TARGET))


if __name__ == "__main__":
    MiningMainnetTest(__file__).main()
