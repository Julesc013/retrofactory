#include "core/core.h"

#include "system/rng.h"
#include "world/world.h"

bool core_init(CoreState &state, const CoreConfig &config)
{
    state.tick = 0u;
    rng_seed(&state.rng, config.initial_seed, 0u);
    return world_init(state.world, config.initial_seed);
}

bool core_tick(CoreState &state)
{
    rng_next_u32(&state.rng);
    state.tick += 1u;
    return true;
}

void core_shutdown(CoreState &state)
{
    world_shutdown(state.world);
    state.tick = 0u;
}
