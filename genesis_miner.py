import hashlib
import struct

# === À MODIFIER À CHAQUE RÉSEAU ===
pszTimestamp = "COINWOW - Mainnet"  # Change ce texte pour chaque réseau!
nTime = 1722666100                  # Change ce nombre pour chaque réseau!
nBits = 0x207fffff                  # Laisse comme ça pour test rapide.
nNonce = 0                          # Commence toujours à 0.
nVersion = 1
reward = 50 * 100000000             # 50 coins

script_pubkey = bytes.fromhex(
    "4104678afdb0fe5548271967f1a67130b7105cd6a828e03909a67962e0ea1f61deb649f6bc3f4cef38c4f35504e51ec112de5c384df7ba0b8d578a4c702b6bf11d5f"
    "ac"
)

def encode_target(nBits):
    exponent = nBits >> 24
    mantissa = nBits & 0xffffff
    target_hexstr = '%064x' % (mantissa * (1 << (8 * (exponent - 3))))
    return bytes.fromhex(target_hexstr)

target = encode_target(nBits)

print(f"Mining genesis block...")
while True:
    coinbase = (
        b'\x04' + len(pszTimestamp).to_bytes(1, 'little') +
        bytes(pszTimestamp, 'utf-8')
    )
    merkle_root = hashlib.sha256(hashlib.sha256(coinbase).digest()).digest()
    header = (
        struct.pack("<L", nVersion) +
        bytes(32) +
        merkle_root +
        struct.pack("<L", nTime) +
        struct.pack("<L", nBits) +
        struct.pack("<L", nNonce)
    )
    hash = hashlib.sha256(hashlib.sha256(header).digest()).digest()[::-1]
    if hash < target:
        print("GENESIS FOUND:")
        print("  nTime        :", nTime)
        print("  nNonce       :", nNonce)
        print("  Hash         :", hash.hex())
        print("  Merkle Root  :", merkle_root[::-1].hex())
        break
    nNonce += 1
    if nNonce % 100000 == 0:
        print(f"Searching... nNonce = {nNonce}")
