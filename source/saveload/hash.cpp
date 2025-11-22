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

    hash_u32(hash, state.entities.next_id);
    hash_u32(hash, state.entities.entities.size);
    if (state.entities.entities.size > 0u)
    {
        hash_bytes(hash, state.entities.entities.data, static_cast<size_t>(state.entities.entities.size * sizeof(EntityInstance)));
    }

    hash_u32(hash, state.networks.next_id);
    hash_u32(hash, state.networks.power.size);
    if (state.networks.power.size > 0u)
    {
        hash_bytes(hash, state.networks.power.data, static_cast<size_t>(state.networks.power.size * sizeof(NetworkNode)));
    }
    hash_u32(hash, state.networks.fluid.size);
    if (state.networks.fluid.size > 0u)
    {
        hash_bytes(hash, state.networks.fluid.data, static_cast<size_t>(state.networks.fluid.size * sizeof(NetworkNode)));
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
