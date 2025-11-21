#include "saveload/hash.h"

#include <stddef.h>

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

    return hash;
}
