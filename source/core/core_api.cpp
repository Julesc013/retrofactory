#include "core/core_api.h"

bool core_init(CoreState &state, const CoreConfig &config)
{
    state.tick = 0ULL;
    rng_seed(state.rng, config.initial_seed);
    return true;
}

bool core_tick(CoreState &state)
{
    ++state.tick;
    rng_next_u32(state.rng);
    return true;
}

void core_shutdown(CoreState &state)
{
    (void)state;
}
