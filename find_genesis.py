import hashlib
import struct
import time

psz_timestamp = b"COINWOW - WOW creates the unstoppable cryptocurrency"
time_value = int(time.time())
bits = 0x1f00ffff
version = 1

def sha256d(data):
    return hashlib.sha256(hashlib.sha256(data).digest()).digest()

def merkle_root(tx):
    return sha256d(tx)

def create_coinbase_tx():
    tx = b""
    tx += struct.pack("<L", 1)
    tx += b"\x01"
    tx += b"\x00" * 32
    tx += struct.pack("<L", 0xffffffff)
    script = psz_timestamp
    tx += bytes([len(script)]) + script
    tx += struct.pack("<L", 0xffffffff)
    tx += b"\x01"
    tx += struct.pack("<Q", 50 * 100000000)
    tx += b"\x00"
    tx += struct.pack("<L", 0)
    return tx

tx = create_coinbase_tx()
merkle = merkle_root(tx)

print("Mining genesis...")

nonce = 0
while True:
    header = struct.pack("<L", version)
    header += b"\x00" * 32
    header += merkle
    header += struct.pack("<L", time_value)
    header += struct.pack("<L", bits)
    header += struct.pack("<L", nonce)

    hash_result = sha256d(header)[::-1].hex()

    hash_int = int(hash_result, 16)
    target = 0x0000ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff

    if hash_int < target:
        print("FOUND!")
        print("Nonce:", nonce)
        print("Time:", time_value)
        print("Hash:", hash_result)
        print("Merkle:", merkle[::-1].hex())
        break

    nonce += 1
