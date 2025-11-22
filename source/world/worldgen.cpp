#include "world/worldgen.h"

#include "system/rng.h"
#include "utility/hash.h"
#include "world/tile.h"

namespace
{
    u8 classify(u32 mix)
    {
        const u32 band = mix % 100u;
        if (band < 10u)
        {
            return 3u; /* water */
        }
        if (band < 40u)
        {
            return 1u; /* foliage */
        }
        if (band < 70u)
        {
            return 2u; /* ore */
        }
        return 0u; /* plains */
    }
}

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
                const u32 coord_hash = hash_combine32(static_cast<u32>(seed), hash_combine32(x, y * 1664525u));
                const u32 noise = rng_next_u32(&rng) ^ coord_hash;
                tile->terrain_type = classify(noise);
            }
        }
    }

    return true;
}
