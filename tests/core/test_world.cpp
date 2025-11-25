#include <cstdio>

#include "core/core.h"
#include "saveload/hash.h"
#include "world/world.h"

namespace
{
    const uint64 kTestSeed = 0x11112222ULL;
}

int main()
{
    CoreConfig config;
    config.initial_seed = kTestSeed;

    CoreState state;
    if (!core_init(state, config))
    {
        std::printf("core_init failed\n");
        return 1;
    }

    const WorldDimensions &dim = world_get_dimensions(state.world);
    if (dim.tile_count_x != dim.chunk_count_x * dim.chunk_size_x ||
        dim.tile_count_y != dim.chunk_count_y * dim.chunk_size_y)
    {
        std::printf("dimension mismatch: tiles (%u,%u) chunks (%u,%u) chunk_size (%u,%u)\n",
            dim.tile_count_x, dim.tile_count_y, dim.chunk_count_x, dim.chunk_count_y, dim.chunk_size_x, dim.chunk_size_y);
        core_shutdown(state);
        return 1;
    }

    const uint64 initial_hash = compute_state_hash(state);

    Tile *tile_a = world_get_tile(state.world, 10, 10);
    Tile *tile_b = world_get_tile(state.world, 20, 30);
    if (tile_a == 0 || tile_b == 0)
    {
        std::printf("tile lookup failed (a=%p b=%p)\n", tile_a, tile_b);
        core_shutdown(state);
        return 1;
    }

    const u8 original_a = tile_a->terrain_type;
    const u8 original_b = tile_b->terrain_type;

    tile_a->terrain_type = 3u;
    tile_b->terrain_type = 5u;

    const uint64 modified_hash = compute_state_hash(state);
    if (modified_hash == initial_hash)
    {
        std::printf("hash unchanged after modification: initial=%llu modified=%llu\n",
            static_cast<unsigned long long>(initial_hash), static_cast<unsigned long long>(modified_hash));
        core_shutdown(state);
        return 1;
    }

    tile_a->terrain_type = original_a;
    tile_b->terrain_type = original_b;

    const uint64 restored_hash = compute_state_hash(state);
    if (restored_hash != initial_hash)
    {
        std::printf("hash did not restore: initial=%llu restored=%llu\n",
            static_cast<unsigned long long>(initial_hash), static_cast<unsigned long long>(restored_hash));
        core_shutdown(state);
        return 1;
    }

    core_shutdown(state);
    return 0;
}
