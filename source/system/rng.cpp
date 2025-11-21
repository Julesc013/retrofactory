#include "system/rng.h"

// Simple linear congruential generator; good enough for deterministic harness tests.
void rng_seed(Rng &rng, uint64 seed)
{
    rng.state = seed;
    // Avoid a zero state so streams always advance.
    if (rng.state == 0u)
    {
        rng.state = 1u;
    }
}

uint64 rng_next(Rng &rng)
{
    // LCG parameters from Numerical Recipes.
    rng.state = rng.state * 6364136223846793005ULL + 1ULL;
    return rng.state;
}
