#include "saveload/hash.h"

#include <cstddef>

namespace
{
    const uint64 kOffsetBasis = 1469598103934665603ULL;
    const uint64 kPrime = 1099511628211ULL;

    void hash_bytes(uint64 &hash, const void *data, std::size_t size)
    {
        const unsigned char *bytes = static_cast<const unsigned char *>(data);
        std::size_t index;
        for (index = 0u; index < size; ++index)
        {
            hash ^= static_cast<uint64>(bytes[index]);
            hash *= kPrime;
        }
    }

    void hash_u64(uint64 &hash, uint64 value)
    {
        hash_bytes(hash, &value, sizeof(uint64));
    }

    void hash_u32(uint64 &hash, uint32 value)
    {
        hash_bytes(hash, &value, sizeof(uint32));
    }
}

uint64 compute_state_hash(const CoreState &state)
{
    uint64 hash = kOffsetBasis;

    hash_u64(hash, state.tick);
    hash_u64(hash, state.rng.state);

    const WorldDimensions &dimensions = state.world.dimensions;
    hash_u32(hash, dimensions.chunk_size_x);
    hash_u32(hash, dimensions.chunk_size_y);
    hash_u32(hash, dimensions.chunk_count_x);
    hash_u32(hash, dimensions.chunk_count_y);
    hash_u32(hash, dimensions.tile_count_x);
    hash_u32(hash, dimensions.tile_count_y);

    const std::size_t tile_count = state.world.tiles.size();
    if (tile_count > 0u)
    {
        hash_bytes(hash, &state.world.tiles[0], tile_count * sizeof(Tile));
    }

    return hash;
}
