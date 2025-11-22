#pragma once

#include "system/types.h"
#include "system/ids.h"
#include "utility/array.h"
#include "world/world.h"

struct EntityInstance
{
    EntityId id;
    u32 proto_id;
    u32 x;
    u32 y;
    u8 state;
};

struct EntitiesState
{
    Array<EntityInstance> entities;
    EntityId next_id;
};

void entities_init(EntitiesState &state);
void entities_shutdown(EntitiesState &state);

EntityId entities_spawn(EntitiesState &state, World &world, u32 proto_id, u32 x, u32 y);
bool entities_tick(EntitiesState &state, World &world, Tick tick);
