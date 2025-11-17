#include "save/state_hash.h"

#include "world/world_grid.h"
#include "world/world_space.h"

namespace
{
    uint64 mix64(uint64 value)
    {
        value ^= value >> 33;
        value *= 0xff51afd7ed558ccdULL;
        value ^= value >> 33;
        value *= 0xc4ceb9fe1a85ec53ULL;
        value ^= value >> 33;
        return value;
    }

    uint64 hash_world_state(const World &world)
    {
        if (world.chunks == 0)
        {
            return 0ULL;
        }

        const WorldDimensions &dim = world.dimensions;
        uint64 combined = 0x9e3779b97f4a7c15ULL;
        combined ^= mix64(static_cast<uint64>(dim.chunk_count_x)) + (combined << 6) + (combined >> 2);
        combined ^= mix64(static_cast<uint64>(dim.chunk_count_y)) + (combined << 6) + (combined >> 2);
        combined ^= mix64(static_cast<uint64>(dim.chunk_size_x)) + (combined << 6) + (combined >> 2);
        combined ^= mix64(static_cast<uint64>(dim.chunk_size_y)) + (combined << 6) + (combined >> 2);

        const int32 chunk_total = dim.chunk_count_x * dim.chunk_count_y;
        const int32 tiles_per_chunk = dim.chunk_size_x * dim.chunk_size_y;

        int32 chunk_index;
        for (chunk_index = 0; chunk_index < chunk_total; ++chunk_index)
        {
            const Chunk &chunk = world.chunks[chunk_index];
            int32 tile_index;
            for (tile_index = 0; tile_index < tiles_per_chunk; ++tile_index)
            {
                const uint64 tile_value = static_cast<uint64>(chunk.tiles[tile_index].terrain_type);
                combined ^= mix64(tile_value + static_cast<uint64>(tile_index + 1)) + (combined << 6) + (combined >> 2);
            }
        }

        return combined;
    }
}

uint64 compute_state_hash(const CoreState &state)
{
    uint64 combined = mix64(state.tick);
    combined ^= mix64(state.rng.state) + (combined << 6) + (combined >> 2);
    combined ^= hash_world_state(state.world) + (combined << 6) + (combined >> 2);
    return mix64(combined);
}
