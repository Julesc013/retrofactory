#include "world/world.h"
#include "utility/hash.h"

static u64 world_checksum(const World &world)
{
    if (world.tiles == 0 || world.tile_count == 0u)
    {
        return 0u;
    }
    return hash_bytes64(world.tiles, static_cast<u32>(world.tile_count * sizeof(Tile)));
}

int main()
{
    u64 baseline = 0u;
    u32 i;
    for (i = 0u; i < 5u; ++i)
    {
        const u64 seed = 5u + i;
        World world;
        if (!world_init(world, seed))
        {
            return 1;
        }
        const u64 hash = world_checksum(world);
        if (hash == 0u)
        {
            world_shutdown(world);
            return 1;
        }
        if (i == 0u)
        {
            baseline = hash;
        }
        else if (seed == 5u && hash != baseline)
        {
            world_shutdown(world);
            return 1;
        }
        if (seed != 5u && hash == baseline)
        {
            world_shutdown(world);
            return 1;
        }
        world_shutdown(world);
    }

    return 0;
}
