#include "system/rng.h"

namespace
{
    const uint64 kRngMultiplier = 6364136223846793005ULL;
    const uint64 kRngIncrement = 1442695040888963407ULL;
}

void rng_seed(RngState &state, uint64 seed)
{
    if (seed == 0ULL)
    {
        seed = 0x4d595df4d0f33173ULL;
    }
    state.state = seed;
}

uint32 rng_next_u32(RngState &state)
{
    state.state = state.state * kRngMultiplier + kRngIncrement;
    return static_cast<uint32>(state.state >> 32);
}
