#include "world/world_grid.h"

#include <new>

namespace
{
    void clear_chunk(Chunk &chunk)
    {
        int32 tile_index;
        for (tile_index = 0; tile_index < WORLD_CHUNK_TILE_COUNT; ++tile_index)
        {
            chunk.tiles[tile_index].terrain_type = 0u;
            chunk.tiles[tile_index].reserved[0] = 0u;
            chunk.tiles[tile_index].reserved[1] = 0u;
            chunk.tiles[tile_index].reserved[2] = 0u;
        }
    }
}

bool world_init_grid(World &world)
{
    const int32 chunk_total = world.dimensions.chunk_count_x * world.dimensions.chunk_count_y;
    if (chunk_total <= 0)
    {
        return false;
    }

    if (world.chunks != 0)
    {
        delete[] world.chunks;
        world.chunks = 0;
    }

    world.chunks = new (std::nothrow) Chunk[chunk_total];
    if (world.chunks == 0)
    {
        return false;
    }

    int32 chunk_index;
    for (chunk_index = 0; chunk_index < chunk_total; ++chunk_index)
    {
        clear_chunk(world.chunks[chunk_index]);
    }

    return true;
}

Tile *world_get_tile(World &world, int32 x, int32 y)
{
    if (world.chunks == 0)
    {
        return 0;
    }

    if (x < 0 || y < 0)
    {
        return 0;
    }

    const WorldDimensions &dim = world.dimensions;
    if (x >= dim.tile_count_x || y >= dim.tile_count_y)
    {
        return 0;
    }

    const int32 chunk_x = x / dim.chunk_size_x;
    const int32 chunk_y = y / dim.chunk_size_y;
    const int32 local_x = x % dim.chunk_size_x;
    const int32 local_y = y % dim.chunk_size_y;

    const int32 chunk_index = chunk_y * dim.chunk_count_x + chunk_x;
    Chunk &chunk = world.chunks[chunk_index];

    const int32 tile_index = local_y * dim.chunk_size_x + local_x;
    return &chunk.tiles[tile_index];
}
