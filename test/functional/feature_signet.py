#!/usr/bin/env python3
# Copyright (c) 2019-2022 The COINWOW Core developers
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.
"""Test basic signet functionality"""

from decimal import Decimal

from test_framework.test_framework import COINWOWTestFramework
from test_framework.util import assert_equal

class SignetParams:
    def __init__(self, challenge):
        # Prune to prevent disk space warning on CI systems with limited space,
        # when using networks other than regtest.
        self.challenge = challenge
        self.shared_args = ["-prune=550", f"-signetchallenge={challenge}"]

class SignetBasicTest(COINWOWTestFramework):
    def set_test_params(self):
        self.chain = "signet"
        self.num_nodes = 3
        self.setup_clean_chain = True
        self.signets = [
            SignetParams(challenge='51'), # OP_TRUE
            SignetParams(challenge='00'), # OP_FALSE: locally-defined, incompatible with OP_TRUE
        ]

        self.extra_args = [
            self.signets[0].shared_args, self.signets[0].shared_args,
            self.signets[1].shared_args,
        ]

    def setup_network(self):
        self.setup_nodes()
        # Nodes stay disconnected: each submitblock call below is a
        # deliberate, deterministic cold submission (no P2P relay races).

    def run_test(self):
        self.log.info("basic tests using OP_TRUE challenge")

        self.log.info('getblockchaininfo')
        def check_getblockchaininfo(node_idx, signet_idx):
            blockchain_info = self.nodes[node_idx].getblockchaininfo()
            assert_equal(blockchain_info['chain'], 'signet')
            assert_equal(blockchain_info['signet_challenge'], self.signets[signet_idx].challenge)
        check_getblockchaininfo(node_idx=1, signet_idx=0)
        check_getblockchaininfo(node_idx=2, signet_idx=1)

        self.log.info('getmininginfo')
        def check_getmininginfo(node_idx, signet_idx):
            mining_info = self.nodes[node_idx].getmininginfo()
            assert_equal(mining_info['blocks'], 0)
            assert_equal(mining_info['chain'], 'signet')
            assert 'currentblocktx' not in mining_info
            assert 'currentblockweight' not in mining_info
            assert_equal(mining_info['networkhashps'], Decimal('0'))
            assert_equal(mining_info['pooledtx'], 0)
            assert_equal(mining_info['signet_challenge'], self.signets[signet_idx].challenge)
        check_getmininginfo(node_idx=0, signet_idx=0)
        check_getmininginfo(node_idx=2, signet_idx=1)

        self.log.info("locally-generated signet block acceptance/rejection check")

        # Mine a block on the OP_TRUE-challenge network (node 0), then submit
        # the same raw block, unseen, to a compatible node (accepted) and to
        # a node whose challenge is incompatible (rejected). OP_TRUE requires
        # no signature at all, so block_hex needs no real signet key.
        block_hash = self.generate(self.nodes[0], 1, sync_fun=self.no_op)[0]
        block_hex = self.nodes[0].getblock(block_hash, 0)

        assert_equal(self.nodes[1].submitblock(block_hex), None)
        assert_equal(self.nodes[1].getblockcount(), 1)

        assert_equal(self.nodes[2].submitblock(block_hex), 'bad-signet-blksig')

        self.log.info("test that signet logs the network magic on node start")
        with self.nodes[0].assert_debug_log(["Signet derived magic (message start)"]):
            self.restart_node(0)
        self.stop_node(0)
        self.nodes[0].assert_start_raises_init_error(extra_args=["-signetchallenge=abc"], expected_msg="Error: -signetchallenge must be hex, not 'abc'.")
        self.nodes[0].assert_start_raises_init_error(extra_args=["-signetchallenge=abc"] * 2, expected_msg="Error: -signetchallenge cannot be multiple values.")


if __name__ == '__main__':
    SignetBasicTest(__file__).main()
