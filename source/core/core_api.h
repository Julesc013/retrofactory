#ifndef CORE_CORE_API_H
#define CORE_CORE_API_H

#include "system/system_base.h"
#include "system/rng.h"
#include "world/world_space.h"

struct CoreConfig
{
    uint64 initial_seed;
};

struct CoreState
{
    Tick tick;
    RngState rng;
    World world;
};

bool core_init(CoreState &state, const CoreConfig &config);
bool core_tick(CoreState &state);
void core_shutdown(CoreState &state);

#endif /* CORE_CORE_API_H */
