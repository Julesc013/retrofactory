#pragma once

#include "system/types.h"

// Minimal deterministic RNG helper used by the core tests.
struct Rng
{
    uint64 state;
};

void rng_seed(Rng &rng, uint64 seed);
uint64 rng_next(Rng &rng);
