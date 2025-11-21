#include "core/core.h"

#include "system/rng.h"
#include "world/world.h"
#include "schedule/tick.h"

bool core_init(CoreState &state, const CoreConfig &config)
{
    state.tick = 0u;
    rng_seed(&state.rng, config.initial_seed, 0u);
    entities_init(state.entities);
    networks_init(state.networks);
    recipies_init(state.recipes);
    research_init(state.research);
    scheduler_init(state.scheduler);
    return world_init(state.world, config.initial_seed);
}

bool core_tick(CoreState &state)
{
    rng_next_u32(&state.rng);
    if (!schedule_tick(state.scheduler, state.tick))
    {
        return false;
    }

    research_tick(state.research);
    state.tick += 1u;
    return true;
}

void core_shutdown(CoreState &state)
{
    world_shutdown(state.world);
    recipies_shutdown(state.recipes);
    research_shutdown(state.research);
    networks_shutdown(state.networks);
    entities_shutdown(state.entities);
    state.tick = 0u;
}
