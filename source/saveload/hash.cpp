#include "saveload/hash.h"

#include <stddef.h>

#include "schedule/events.h"

namespace
{
    const u64 kOffsetBasis = 1469598103934665603ULL;
    const u64 kPrime = 1099511628211ULL;

    void hash_bytes(u64 &hash, const void *data, size_t size)
    {
        const unsigned char *bytes = static_cast<const unsigned char *>(data);
        size_t index;
        for (index = 0u; index < size; ++index)
        {
            hash ^= static_cast<u64>(bytes[index]);
            hash *= kPrime;
        }
    }

    void hash_u64(u64 &hash, u64 value)
    {
        hash_bytes(hash, &value, sizeof(u64));
    }

    void hash_u32(u64 &hash, u32 value)
    {
        hash_bytes(hash, &value, sizeof(u32));
    }
}

u64 compute_state_hash(const CoreState &state)
{
    u64 hash = kOffsetBasis;

    hash_u64(hash, state.tick);
    hash_u64(hash, state.rng.state);
    hash_u64(hash, state.rng.inc);

    const WorldDimensions &dimensions = state.world.dimensions;
    hash_u32(hash, dimensions.chunk_size_x);
    hash_u32(hash, dimensions.chunk_size_y);
    hash_u32(hash, dimensions.chunk_count_x);
    hash_u32(hash, dimensions.chunk_count_y);
    hash_u32(hash, dimensions.tile_count_x);
    hash_u32(hash, dimensions.tile_count_y);

    if (state.world.tiles != 0 && state.world.tile_count > 0u)
    {
        hash_bytes(hash, state.world.tiles, static_cast<size_t>(state.world.tile_count * sizeof(Tile)));
    }

    /* Transmission networks */
    hash_u32(hash, static_cast<u32>(state.world.trans_power.node_count));
    hash_u32(hash, static_cast<u32>(state.world.trans_power.edge_count));
    if (state.world.trans_power.nodes != 0 && state.world.trans_power.node_count > 0)
    {
        hash_bytes(hash, state.world.trans_power.nodes, static_cast<size_t>(state.world.trans_power.node_count * sizeof(TransNode)));
    }
    if (state.world.trans_power.edges != 0 && state.world.trans_power.edge_count > 0)
    {
        hash_bytes(hash, state.world.trans_power.edges, static_cast<size_t>(state.world.trans_power.edge_count * sizeof(TransEdge)));
    }

    hash_u32(hash, static_cast<u32>(state.world.trans_fluid.node_count));
    hash_u32(hash, static_cast<u32>(state.world.trans_fluid.edge_count));
    if (state.world.trans_fluid.nodes != 0 && state.world.trans_fluid.node_count > 0)
    {
        hash_bytes(hash, state.world.trans_fluid.nodes, static_cast<size_t>(state.world.trans_fluid.node_count * sizeof(TransNode)));
    }
    if (state.world.trans_fluid.edges != 0 && state.world.trans_fluid.edge_count > 0)
    {
        hash_bytes(hash, state.world.trans_fluid.edges, static_cast<size_t>(state.world.trans_fluid.edge_count * sizeof(TransEdge)));
    }

    hash_u32(hash, static_cast<u32>(state.world.trans_data.node_count));
    hash_u32(hash, static_cast<u32>(state.world.trans_data.edge_count));
    if (state.world.trans_data.nodes != 0 && state.world.trans_data.node_count > 0)
    {
        hash_bytes(hash, state.world.trans_data.nodes, static_cast<size_t>(state.world.trans_data.node_count * sizeof(TransNode)));
    }
    if (state.world.trans_data.edges != 0 && state.world.trans_data.edge_count > 0)
    {
        hash_bytes(hash, state.world.trans_data.edges, static_cast<size_t>(state.world.trans_data.edge_count * sizeof(TransEdge)));
    }

    /* Travel networks */
    hash_u32(hash, static_cast<u32>(state.world.travel_rail.node_count));
    hash_u32(hash, static_cast<u32>(state.world.travel_rail.segment_count));
    if (state.world.travel_rail.nodes != 0 && state.world.travel_rail.node_count > 0)
    {
        hash_bytes(hash, state.world.travel_rail.nodes, static_cast<size_t>(state.world.travel_rail.node_count * sizeof(TravelNode)));
    }
    if (state.world.travel_rail.segments != 0 && state.world.travel_rail.segment_count > 0)
    {
        hash_bytes(hash, state.world.travel_rail.segments, static_cast<size_t>(state.world.travel_rail.segment_count * sizeof(TravelSegment)));
    }

    hash_u32(hash, static_cast<u32>(state.world.travel_road.node_count));
    hash_u32(hash, static_cast<u32>(state.world.travel_road.segment_count));
    if (state.world.travel_road.nodes != 0 && state.world.travel_road.node_count > 0)
    {
        hash_bytes(hash, state.world.travel_road.nodes, static_cast<size_t>(state.world.travel_road.node_count * sizeof(TravelNode)));
    }
    if (state.world.travel_road.segments != 0 && state.world.travel_road.segment_count > 0)
    {
        hash_bytes(hash, state.world.travel_road.segments, static_cast<size_t>(state.world.travel_road.segment_count * sizeof(TravelSegment)));
    }

    hash_u32(hash, static_cast<u32>(state.world.travel_water.node_count));
    hash_u32(hash, static_cast<u32>(state.world.travel_water.segment_count));
    if (state.world.travel_water.nodes != 0 && state.world.travel_water.node_count > 0)
    {
        hash_bytes(hash, state.world.travel_water.nodes, static_cast<size_t>(state.world.travel_water.node_count * sizeof(TravelNode)));
    }
    if (state.world.travel_water.segments != 0 && state.world.travel_water.segment_count > 0)
    {
        hash_bytes(hash, state.world.travel_water.segments, static_cast<size_t>(state.world.travel_water.segment_count * sizeof(TravelSegment)));
    }

    hash_u32(hash, static_cast<u32>(state.world.travel_air.node_count));
    hash_u32(hash, static_cast<u32>(state.world.travel_air.segment_count));
    if (state.world.travel_air.nodes != 0 && state.world.travel_air.node_count > 0)
    {
        hash_bytes(hash, state.world.travel_air.nodes, static_cast<size_t>(state.world.travel_air.node_count * sizeof(TravelNode)));
    }
    if (state.world.travel_air.segments != 0 && state.world.travel_air.segment_count > 0)
    {
        hash_bytes(hash, state.world.travel_air.segments, static_cast<size_t>(state.world.travel_air.segment_count * sizeof(TravelSegment)));
    }

    hash_u32(hash, state.entities.next_id);
    hash_u32(hash, state.entities.entities.size);
    if (state.entities.entities.size > 0u)
    {
        hash_bytes(hash, state.entities.entities.data, static_cast<size_t>(state.entities.entities.size * sizeof(EntityInstance)));
    }

    hash_u32(hash, state.recipes.next_id);
    hash_u32(hash, state.recipes.recipes.size);
    if (state.recipes.recipes.size > 0u)
    {
        hash_bytes(hash, state.recipes.recipes.data, static_cast<size_t>(state.recipes.recipes.size * sizeof(Recipe)));
    }

    hash_u32(hash, state.research.active);
    hash_u32(hash, state.research.progress);
    hash_u32(hash, state.research.topics.size);
    if (state.research.topics.size > 0u)
    {
        hash_bytes(hash, state.research.topics.data, static_cast<size_t>(state.research.topics.size * sizeof(ResearchTopic)));
    }

    hash_u32(hash, state.scheduler.processed);
    hash_u32(hash, state.scheduler.count);
    if (state.scheduler.count > 0u)
    {
        hash_bytes(hash, state.scheduler.events, static_cast<size_t>(state.scheduler.count * sizeof(ScheduledEvent)));
    }

    return hash;
}
