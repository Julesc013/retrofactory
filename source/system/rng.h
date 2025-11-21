#pragma once

#include "system/types.h"

/* Deterministic PCG-style RNG stream. */
struct RngStream
{
    u64 state;
    u64 inc;
};

void rng_seed(RngStream *rng, u64 seed, u64 sequence);
u32 rng_next_u32(RngStream *rng);
i32 rng_next_range(RngStream *rng, i32 min_inclusive, i32 max_inclusive);
float rng_next_float01(RngStream *rng);
