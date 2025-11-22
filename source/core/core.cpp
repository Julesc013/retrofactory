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
    if (!world_init(state.world, config.initial_seed))
    {
        return false;
    }

    /* Bring up a couple of default networks and entities so hashes are non-trivial. */
    networks_create_power(state.networks, 100u);
    networks_create_fluid(state.networks, 40u);

    const WorldDimensions &dim = world_get_dimensions(state.world);
    const u32 spawn_x = dim.tile_count_x / 2u;
    const u32 spawn_y = dim.tile_count_y / 2u;
    entities_spawn(state.entities, state.world, 1u, spawn_x, spawn_y);

    /* Periodic maintenance event every 60 ticks. */
    scheduler_push_interval(state.scheduler, 30u, 60u, 100u, 1u, 0u);

    return true;
}

bool core_tick(CoreState &state)
{
    rng_next_u32(&state.rng);
    if (!schedule_tick(state.scheduler, state.tick))
    {
        return false;
    }

    networks_tick(state.networks, state.tick);
    entities_tick(state.entities, state.world, state.tick);
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
