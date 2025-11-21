#pragma once

#include "system/rng.h"
#include "world/world.h"

struct CoreConfig
{
    uint64 initial_seed;

    CoreConfig() : initial_seed(1u) {}
};

struct CoreState
{
    uint64 tick;
    Rng rng;
    World world;

    CoreState() : tick(0u), rng(), world() {}
};

bool core_init(CoreState &state, const CoreConfig &config);
bool core_tick(CoreState &state);
void core_shutdown(CoreState &state);
