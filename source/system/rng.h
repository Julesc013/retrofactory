#ifndef SYSTEM_RNG_H
#define SYSTEM_RNG_H

#include "system/system_base.h"

struct rf_rng_state
{
    uint32 seed;
};

void rng_init(rf_rng_state &state, uint32 seed);
uint32 rng_next(rf_rng_state &state);

#endif /* SYSTEM_RNG_H */
