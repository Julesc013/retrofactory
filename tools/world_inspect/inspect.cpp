#include "inspect.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "world/world.h"
#include "utility/hash.h"

int world_inspect_main(int argc, char **argv)
{
    u64 seed = 1u;
    if (argc > 1)
    {
        seed = static_cast<u64>(std::strtoull(argv[1], 0, 10));
    }

    World world;
    if (!world_init(world, seed))
    {
        std::printf("Failed to init world\n");
        return 1;
    }

    u32 histogram[4] = {0u, 0u, 0u, 0u};
    u32 i;
    for (i = 0u; i < world.tile_count; ++i)
    {
        const u8 terrain = world.tiles[i].terrain_type & 0x03u;
        histogram[terrain % 4u] += 1u;
    }

    const u64 world_hash = hash_bytes64(world.tiles, static_cast<u32>(world.tile_count * sizeof(Tile)));
    std::printf("World seed: %llu\n", static_cast<unsigned long long>(seed));
    std::printf("Terrain histogram plains/foliage/ore/water: %u %u %u %u\n",
                histogram[0], histogram[1], histogram[2], histogram[3]);
    std::printf("Tile hash: %llu\n", static_cast<unsigned long long>(world_hash));

    world_shutdown(world);
    return 0;
}

int main(int argc, char **argv)
{
    return world_inspect_main(argc, argv);
}
