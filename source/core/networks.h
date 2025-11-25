#pragma once

#include "system/types.h"
#include "system/ids.h"
#include "core/trans_net.h"

struct PrototypeStore;

struct NetworkNode
{
    NetworkId id;
    TransNodeId trans_id;
    u32 capacity;
};

struct NetworkState
{
    TransGraph *power_graph;
    TransGraph *fluid_graph;
    NetworkId next_id;
    const PrototypeStore *prototypes;
};

void networks_init(NetworkState &state, TransGraph *power_graph = 0, TransGraph *fluid_graph = 0, const PrototypeStore *prototypes = 0);
void networks_shutdown(NetworkState &state);
NetworkId networks_create_power(NetworkState &state, u32 capacity);
NetworkId networks_create_fluid(NetworkState &state, u32 capacity);
bool networks_tick(NetworkState &state, Tick tick);
