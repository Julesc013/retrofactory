#include "system/rng.h"

namespace
{
    const uint32 kRngMultiplier = 1664525u;
    const uint32 kRngIncrement = 1013904223u;
}

void rng_init(rf_rng_state &state, uint32 seed)
{
    if (seed == 0u)
    {
        seed = 1u;
    }
    state.seed = seed;
}

uint32 rng_next(rf_rng_state &state)
{
    state.seed = kRngMultiplier * state.seed + kRngIncrement;
    return state.seed;
}
