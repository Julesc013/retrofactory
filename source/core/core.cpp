#include "core/core.h"

#include "system/rng.h"
#include "world/world.h"
#include "schedule/tick.h"
#include "core/game_state.h"
#include "core/trans_net.h"
#include "core/travel_net.h"

namespace
{
    const u32 kEventResearchPulse = 1u;
    const u32 kEventEntityPulse = 2u;

    static GameState make_game_state(CoreState &state)
    {
        GameState gs;
        gs.world = &state.world;
        gs.entities = &state.entities;
        gs.trans_power = &state.world.trans_power;
        gs.trans_fluid = &state.world.trans_fluid;
        gs.trans_data = &state.world.trans_data;
        gs.travel_rail = &state.world.travel_rail;
        gs.travel_road = &state.world.travel_road;
        gs.travel_water = &state.world.travel_water;
        gs.travel_air = &state.world.travel_air;
        return gs;
    }

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
    recipies_init(state.recipes, state.prototypes);
    research_init(state.research, state.prototypes);
    scheduler_init(state.scheduler);
    if (!world_init(state.world, config.initial_seed, state.prototypes))
    {
        return false;
    }

    /* Seed baseline transmission graphs to keep deterministic hashes non-trivial. */
    if (state.world.trans_power.node_count == 0)
    {
        trans_node_add(&state.world.trans_power, 0, 0, 0);
    }
    if (state.world.trans_fluid.node_count == 0)
    {
        trans_node_add(&state.world.trans_fluid, 0, 0, 0);
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

    {
        GameState gs = make_game_state(state);
        tick_step_trans_networks(&gs);
    }

    if (!schedule_tick(state.scheduler, state.tick, handle_scheduled_event, &state))
    {
        return false;
    }

    {
        GameState gs = make_game_state(state);
        tick_step_travel_networks(&gs);
    }
    state.tick += 1u;
    return true;
}

void core_shutdown(CoreState &state)
{
    world_shutdown(state.world);
    recipies_shutdown(state.recipes);
    research_shutdown(state.research);
    entities_shutdown(state.entities);
    state.tick = 0u;
    state.prototypes = 0;
}
