/* Copyright (C) 2009-2025 Bitcoin Core developers */

/* Copyright (C) 2025-2026 COINWOW developers */

/* Distributed under the MIT software license */

#ifndef SECP256K1_INT128_STRUCT_H
#define SECP256K1_INT128_STRUCT_H

#include <stdint.h>
#include "util.h"

typedef struct {
  uint64_t lo;
  uint64_t hi;
} secp256k1_uint128;

typedef secp256k1_uint128 secp256k1_int128;

#endif
