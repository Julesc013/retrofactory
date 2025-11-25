#include "world/worldgen.h"

#include "system/rng.h"
#include "utility/hash.h"
#include "world/tile.h"

namespace
{
    u8 pick_terrain_from_prototypes(const PrototypeStore *prototypes, u32 noise)
    {
        if (prototypes == 0 || prototypes->worldgen.size == 0u)
        {
            /* Default bands. */
            const u32 band = noise % 100u;
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

        u32 total_weight = 0u;
        u32 i;
        for (i = 0u; i < prototypes->worldgen.size; ++i)
        {
            total_weight += prototypes->worldgen.data[i].weight;
        }
        if (total_weight == 0u)
        {
            total_weight = prototypes->worldgen.size;
        }

        const u32 pick = noise % total_weight;
        u32 accum = 0u;
        for (i = 0u; i < prototypes->worldgen.size; ++i)
        {
            accum += prototypes->worldgen.data[i].weight;
            if (pick < accum)
            {
                return prototypes->worldgen.data[i].terrain & 0x0Fu;
            }
        }
        return static_cast<u8>(noise & 0x03u);
    }
}

bool worldgen_generate(World &world, u64 seed, const PrototypeStore *prototypes)
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
                tile->terrain_type = pick_terrain_from_prototypes(prototypes, noise);
            }
        }
    }

    return true;
}
