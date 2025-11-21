#pragma once

#include "system/rng.h"
#include "world/world.h"
#include "core/entities.h"
#include "core/networks.h"
#include "core/recipies.h"
#include "core/research.h"
#include "schedule/events.h"

struct CoreConfig
{
    u64 initial_seed;

    CoreConfig() : initial_seed(1u) {}
};

struct CoreState
{
    Tick tick;
    RngStream rng;
    World world;
    EntitiesState entities;
    NetworkState networks;
    RecipeRegistry recipes;
    ResearchState research;
    Scheduler scheduler;

    CoreState() : tick(0u), rng(), world(), entities(), networks(), recipes(), research(), scheduler() {}
};

bool core_init(CoreState &state, const CoreConfig &config);
bool core_tick(CoreState &state);
void core_shutdown(CoreState &state);
