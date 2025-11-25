#pragma once

#include "system/types.h"
#include "system/ids.h"
#include "utility/array.h"
#include "world/world.h"
#include "mods/prototypes.h"
#include "core/travel_net.h"
#include "core/trans_net.h"

struct EntityInstance
{
    EntityId id;
    u32 proto_id;
    u32 x;
    u32 y;
    u8 state;
};

struct StationComponent
{
    TravelNodeId travel_node;
    TransNodeId power_node;
    TransNodeId data_node;
};

struct EntitiesState
{
    Array<EntityInstance> entities;
    EntityId next_id;
    const PrototypeStore *prototypes;
};

void entities_init(EntitiesState &state, const PrototypeStore *prototypes = 0);
void entities_shutdown(EntitiesState &state);

EntityId entities_spawn(EntitiesState &state, World &world, u32 proto_id, u32 x, u32 y);
bool entities_tick(EntitiesState &state, World &world, Tick tick);
