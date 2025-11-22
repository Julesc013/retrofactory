#include "core/core.h"

#include "system/rng.h"
#include "world/world.h"
#include "schedule/tick.h"

namespace
{
    const u32 kEventResearchPulse = 1u;
    const u32 kEventEntityPulse = 2u;

    void handle_scheduled_event(const ScheduledEvent &ev, void *user_data)
    {
        CoreState *state = static_cast<CoreState *>(user_data);
        if (state == 0)
        {
            return;
        }

        switch (ev.type)
        {
        case kEventResearchPulse:
            research_tick(state->research);
            break;
        case kEventEntityPulse:
            entities_tick(state->entities, state->world, ev.tick);
            break;
        default:
            break;
        }
    }
}

bool core_init(CoreState &state, const CoreConfig &config)
{
    state.tick = 0u;
    state.prototypes = config.prototypes;
    rng_seed(&state.rng, config.initial_seed, 0u);
    entities_init(state.entities, state.prototypes);
    networks_init(state.networks, state.prototypes);
    recipies_init(state.recipes, state.prototypes);
    research_init(state.research, state.prototypes);
    scheduler_init(state.scheduler);
    if (!world_init(state.world, config.initial_seed, state.prototypes))
    {
        return false;
    }

    /* Bring up a couple of default networks and entities so hashes are non-trivial. */
    if (state.networks.power.size == 0u && state.networks.fluid.size == 0u)
    {
        networks_create_power(state.networks, 100u);
        networks_create_fluid(state.networks, 40u);
    }

    const WorldDimensions &dim = world_get_dimensions(state.world);
    const u32 spawn_x = dim.tile_count_x / 2u;
    const u32 spawn_y = dim.tile_count_y / 2u;
    u32 proto_to_spawn = 1u;
    if (state.prototypes != 0 && state.prototypes->entities.size > 0u)
    {
        proto_to_spawn = state.prototypes->entities.data[0].numeric_id;
    }
    entities_spawn(state.entities, state.world, proto_to_spawn, spawn_x, spawn_y);

    /* Periodic maintenance events drive research and entity actions. */
    scheduler_push_interval(state.scheduler, 1u, 1u, 100000u, kEventEntityPulse, 0u);
    scheduler_push_interval(state.scheduler, 5u, 5u, 1000u, kEventResearchPulse, 0u);

    return true;
}

bool core_tick(CoreState &state)
{
    rng_next_u32(&state.rng);
    if (!schedule_tick(state.scheduler, state.tick, handle_scheduled_event, &state))
    {
        return false;
    }

    networks_tick(state.networks, state.tick);
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
    state.prototypes = 0;
}
