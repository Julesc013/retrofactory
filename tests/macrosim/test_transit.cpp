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
    World a;
    World b;
    World c;
    if (!world_init(a, 5u) || !world_init(b, 5u) || !world_init(c, 6u))
    {
        return 1;
    }

    const u64 hash_a = world_checksum(a);
    const u64 hash_b = world_checksum(b);
    const u64 hash_c = world_checksum(c);

    world_shutdown(a);
    world_shutdown(b);
    world_shutdown(c);

    if (hash_a == 0u || hash_c == 0u)
    {
        return 1;
    }
    if (hash_a != hash_b)
    {
        return 1;
    }
    if (hash_a == hash_c)
    {
        return 1;
    }
    return 0;
}
