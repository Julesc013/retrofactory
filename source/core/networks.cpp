#include "core/networks.h"
#include "utility/hash.h"

static NetworkId next_network_id(NetworkState &state)
{
    if (state.next_id == 0u)
    {
        state.next_id = 1u;
    }
    NetworkId id = state.next_id;
    state.next_id += 1u;
    return id;
}

static NetworkNode make_node(NetworkId id, u32 capacity)
{
    NetworkNode node;
    node.id = id;
    node.capacity = capacity;
    node.load = 0u;
    return node;
}

void networks_init(NetworkState &state)
{
    array_init(state.power);
    array_init(state.fluid);
    state.next_id = 1u;
}

void networks_shutdown(NetworkState &state)
{
    array_free(state.power);
    array_free(state.fluid);
    state.next_id = 1u;
}

NetworkId networks_create_power(NetworkState &state, u32 capacity)
{
    NetworkId id = next_network_id(state);
    NetworkNode node = make_node(id, capacity);
    if (!array_push(state.power, node))
    {
        return 0u;
    }
    return id;
}

NetworkId networks_create_fluid(NetworkState &state, u32 capacity)
{
    NetworkId id = next_network_id(state);
    NetworkNode node = make_node(id, capacity);
    if (!array_push(state.fluid, node))
    {
        return 0u;
    }
    return id;
}

bool networks_tick(NetworkState &state, Tick tick)
{
    u32 i;
    for (i = 0u; i < state.power.size; ++i)
    {
        NetworkNode &node = state.power.data[i];
        const u32 hash = hash_combine32(node.id, tick);
        node.load = (hash % (node.capacity + 1u));
    }
    for (i = 0u; i < state.fluid.size; ++i)
    {
        NetworkNode &node = state.fluid.data[i];
        const u32 hash = hash_combine32(node.id ^ 0xA5A5u, tick + 3u);
        node.load = (hash % (node.capacity + 1u));
    }
    return true;
}
