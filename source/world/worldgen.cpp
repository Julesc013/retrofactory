#include "world/worldgen.h"

#include "system/rng.h"
#include "world/tile.h"

bool worldgen_generate(World &world, u64 seed)
{
    if (world.tiles == 0 || world.tile_count == 0u)
    {
        return false;
    }

    RngStream rng;
    rng_seed(&rng, seed, 1u);

    const u32 width = world.dimensions.tile_count_x;
    const u32 height = world.dimensions.tile_count_y;

    u32 y;
    for (y = 0u; y < height; ++y)
    {
        u32 x;
        for (x = 0u; x < width; ++x)
        {
            Tile *tile = world_get_tile(world, x, y);
            if (tile != 0)
            {
                /* Placeholder deterministic terrain: keep zeroed for stable tests. */
                (void)rng_next_u32(&rng);
                tile->terrain_type = 0u;
            }
        }
    }

    return true;
}
