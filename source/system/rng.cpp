#include "system/rng.h"

/* PCG32; deterministic and simple enough for C++98. */
void rng_seed(RngStream *rng, u64 seed, u64 sequence)
{
    if (rng == 0)
    {
        return;
    }

    rng->state = 0u;
    rng->inc = (sequence << 1u) | 1u;
    rng_next_u32(rng);
    rng->state += seed;
    rng_next_u32(rng);
}

u32 rng_next_u32(RngStream *rng)
{
    const u64 oldstate = rng->state;
    rng->state = oldstate * 6364136223846793005ULL + rng->inc;
    const u32 xorshifted = static_cast<u32>(((oldstate >> 18u) ^ oldstate) >> 27u);
    const u32 rot = static_cast<u32>(oldstate >> 59u);
    return (xorshifted >> rot) | (xorshifted << ((-rot) & 31u));
}

i32 rng_next_range(RngStream *rng, i32 min_inclusive, i32 max_inclusive)
{
    if (max_inclusive <= min_inclusive)
    {
        return min_inclusive;
    }

    const u32 span = static_cast<u32>(max_inclusive - min_inclusive + 1);
    const u32 value = rng_next_u32(rng) % span;
    return static_cast<i32>(min_inclusive + static_cast<i32>(value));
}

float rng_next_float01(RngStream *rng)
{
    /* Scale 32-bit integer into [0,1) */
    const u32 value = rng_next_u32(rng) >> 8; /* 24 bits */
    return static_cast<float>(value) / static_cast<float>(0x01000000u);
}
