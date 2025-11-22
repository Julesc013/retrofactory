#pragma once

#include "system/types.h"
#include "system/ids.h"
#include "utility/array.h"

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
};

void networks_init(NetworkState &state);
void networks_shutdown(NetworkState &state);
NetworkId networks_create_power(NetworkState &state, u32 capacity);
NetworkId networks_create_fluid(NetworkState &state, u32 capacity);
bool networks_tick(NetworkState &state, Tick tick);
