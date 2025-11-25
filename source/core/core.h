#pragma once

#include "system/rng.h"
#include "world/world.h"
#include "core/entities.h"
#include "core/recipies.h"
#include "core/research.h"
#include "schedule/events.h"
#include "mods/prototypes.h"

struct CoreConfig
{
    u64 initial_seed;
    const PrototypeStore *prototypes;

    CoreConfig() : initial_seed(1u), prototypes(0) {}
};

struct CoreState
{
    Tick tick;
    const PrototypeStore *prototypes;
    RngStream rng;
    World world;
    EntitiesState entities;
    RecipeRegistry recipes;
    ResearchState research;
    Scheduler scheduler;

    CoreState() : tick(0u), prototypes(0), rng(), world(), entities(), recipes(), research(), scheduler() {}
};

bool core_init(CoreState &state, const CoreConfig &config);
bool core_tick(CoreState &state);
void core_shutdown(CoreState &state);
