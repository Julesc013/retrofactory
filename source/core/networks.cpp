#include "core/networks.h"
#include "core/game_state.h"
#include "mods/prototypes.h"

#include <cstring>

namespace
{
    NetworkId next_network_id(NetworkState &state)
    {
        if (state.next_id == 0u)
        {
            state.next_id = 1u;
        }
        NetworkId id = state.next_id;
        state.next_id += 1u;
        return id;
    }

    TransNodeId spawn_node(TransGraph *graph)
    {
        if (graph == 0)
        {
            TransNodeId invalid;
            invalid.index = -1;
            return invalid;
        }
        return trans_node_add(graph, 0, 0, 0);
    }
}

void networks_init(NetworkState &state, TransGraph *power_graph, TransGraph *fluid_graph, const PrototypeStore *prototypes)
{
    state.power_graph = power_graph;
    state.fluid_graph = fluid_graph;
    state.next_id = 1u;
    state.prototypes = prototypes;

    if (prototypes != 0)
    {
        u32 i;
        for (i = 0u; i < prototypes->networks.size; ++i)
        {
            const NetworkPrototype &proto = prototypes->networks.data[i];
            if (std::strstr(proto.id, "fluid") != 0)
            {
                networks_create_fluid(state, proto.capacity);
            }
            else
            {
                networks_create_power(state, proto.capacity);
            }
        }
    }
}

void networks_shutdown(NetworkState &state)
{
    state.power_graph = 0;
    state.fluid_graph = 0;
    state.next_id = 1u;
    state.prototypes = 0;
}

NetworkId networks_create_power(NetworkState &state, u32 capacity)
{
    TransNodeId trans_id = spawn_node(state.power_graph);
    NetworkId id = next_network_id(state);
    (void)capacity;
    (void)trans_id;
    return id;
}

NetworkId networks_create_fluid(NetworkState &state, u32 capacity)
{
    TransNodeId trans_id = spawn_node(state.fluid_graph);
    NetworkId id = next_network_id(state);
    (void)capacity;
    (void)trans_id;
    return id;
}

bool networks_tick(NetworkState &state, Tick tick)
{
    GameState gs;
    (void)tick;
    gs.world = 0;
    gs.entities = 0;
    gs.trans_power = state.power_graph;
    gs.trans_fluid = state.fluid_graph;
    gs.trans_data = 0;
    gs.travel_rail = 0;
    gs.travel_road = 0;
    gs.travel_water = 0;
    gs.travel_air = 0;
    trans_step_power(&gs);
    trans_step_fluid(&gs);
    return true;
}
