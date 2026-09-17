#include <iostream>    // Pour std::cout, std::endl
#include <string>      // Pour std::string
#include <cstdint>     // Pour uint32_t, int32_t etc

// Les includes spécifiques à COINWOW Core (selon ce que tu as dans ton projet)
#include <script/script.h>
#include <primitives/block.h>
#include "uint256.h"
#include <consensus/merkle.h>
#include <util/strencodings.h>
int main()
{
    const char* pszTimestamp = "WOW creates the unstoppable cryptocurrency: COINWOW.";
    CScript genesisOutputScript = CScript()
        << ParseHex("04678afdb0fe5548271967f1a67130b7105cd6a828e03909a67962e0ea1f61deb649f6bc3f4cef38c4f35504e51ec112de5c384df7ba0b8d578a4c702b6bf11d5f")
        << OP_CHECKSIG;

    uint32_t nTime = 1710000000; // timestamp fixe, tu peux changer
    uint32_t nNonce = 0;
    uint32_t nBits = 0x1d00ffff; // difficulté
    int32_t nVersion = 1;
    CAmount genesisReward = 50 * COIN;

    std::cout << "Début du minage du Genesis Block..." << std::endl;

    CBlock genesis;
    while (true) {
        genesis = CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);

        uint256 hash = genesis.GetHash();
        std::string hashStr = hash.ToString();

        // Exemple simple : on veut un hash qui commence par "0000"
        if (hashStr.substr(0, 4) == "0000") {
            std::cout << "Block miné !" << std::endl;
            std::cout << "Nonce trouvé : " << nNonce << std::endl;
            std::cout << "Hash : " << hashStr << std::endl;
            break;
        }

        if (nNonce % 100000 == 0) {
            std::cout << "Essai nonce : " << nNonce << " Hash actuel : " << hashStr << std::endl;
        }

        nNonce++;
    }

    return 0;
}

