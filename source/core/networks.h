#pragma once

#include "system/types.h"
#include "system/ids.h"
#include "utility/array.h"
#include "mods/prototypes.h"

struct NetworkNode
{
    NetworkId id;
    u32 load;
    u32 capacity;
};

struct NetworkState
{
    Array<NetworkNode> power;
    Array<NetworkNode> fluid;
    NetworkId next_id;
    const PrototypeStore *prototypes;
};

void networks_init(NetworkState &state, const PrototypeStore *prototypes = 0);
void networks_shutdown(NetworkState &state);
NetworkId networks_create_power(NetworkState &state, u32 capacity);
NetworkId networks_create_fluid(NetworkState &state, u32 capacity);
bool networks_tick(NetworkState &state, Tick tick);
