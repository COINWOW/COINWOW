#include <iostream>
#include "uint256.h"
#include "primitives/block.h"
#include "consensus/consensus.h"
#include "pow.h"
#include "chainparams.h"

// Tu devras adapter le CreateGenesisBlock selon ton code exact.
// Voici un exemple générique, adapte selon ta fonction CreateGenesisBlock.

// Si t'as une fonction dans chainparams.cpp comme:
// CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward);

extern CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward);

int main() {
    uint32_t nTime = 1690000000;    // Mets une date récente en timestamp UNIX (exemple)
    uint32_t nBits = 0x1d00ffff;    // Difficulté (exemple, adapte selon ton réseau)
    int32_t nVersion = 1;
    CAmount genesisReward = 50 * COIN;  // Ta récompense genesis (adapte)

    uint32_t nonce = 0;
    CBlock genesisBlock;

    uint256 target = UintToArith256(uint256S("00000000ffffffffffffffffffffffffffffffffffffffffffffffffffffffff")); // cible facile pour miner

    while(true) {
        genesisBlock = CreateGenesisBlock(nTime, nonce, nBits, nVersion, genesisReward);

        uint256 hash = genesisBlock.GetHash();

        if (UintToArith256(hash) <= UintToArith256(target)) {
            std::cout << "Nouveau Genesis trouvé!" << std::endl;
            std::cout << "Hash: " << hash.ToString() << std::endl;
            std::cout << "Nonce: " << nonce << std::endl;
            std::cout << "Merkle Root: " << genesisBlock.hashMerkleRoot.ToString() << std::endl;
            break;
        }
        nonce++;
        if (nonce == 0) {
            nTime++;  // si overflow nonce, avance le temps
        }
    }

    return 0;
}
