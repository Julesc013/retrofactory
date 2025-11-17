#ifndef SYSTEM_RNG_H
#define SYSTEM_RNG_H

#include "system/system_base.h"

struct RngState
{
    uint64 state;
};

void rng_seed(RngState &state, uint64 seed);
uint32 rng_next_u32(RngState &state);

#endif /* SYSTEM_RNG_H */
