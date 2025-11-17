#include "world/world_space.h"

#include "world/world_grid.h"

namespace
{
    const int32 kDefaultChunkCountX = 8;
    const int32 kDefaultChunkCountY = 8;
    const int32 kDefaultChunkSizeX = WORLD_CHUNK_SIZE_X;
    const int32 kDefaultChunkSizeY = WORLD_CHUNK_SIZE_Y;
}

World::World()
    : chunks(0)
{
    dimensions.chunk_count_x = 0;
    dimensions.chunk_count_y = 0;
    dimensions.chunk_size_x = 0;
    dimensions.chunk_size_y = 0;
    dimensions.tile_count_x = 0;
    dimensions.tile_count_y = 0;
}

bool world_init(World &world)
{
    return world_init_with_dimensions(world,
                                      kDefaultChunkCountX,
                                      kDefaultChunkCountY,
                                      kDefaultChunkSizeX,
                                      kDefaultChunkSizeY);
}

bool world_init_with_dimensions(World &world, int32 chunk_count_x, int32 chunk_count_y, int32 chunk_size_x, int32 chunk_size_y)
{
    if (world.chunks != 0)
    {
        world_shutdown(world);
    }

    if (chunk_count_x <= 0 || chunk_count_y <= 0 || chunk_size_x <= 0 || chunk_size_y <= 0)
    {
        return false;
    }

    world.dimensions.chunk_count_x = chunk_count_x;
    world.dimensions.chunk_count_y = chunk_count_y;
    world.dimensions.chunk_size_x = chunk_size_x;
    world.dimensions.chunk_size_y = chunk_size_y;
    world.dimensions.tile_count_x = world.dimensions.chunk_count_x * world.dimensions.chunk_size_x;
    world.dimensions.tile_count_y = world.dimensions.chunk_count_y * world.dimensions.chunk_size_y;

    return world_init_grid(world);
}

void world_shutdown(World &world)
{
    if (world.chunks != 0)
    {
        delete[] world.chunks;
        world.chunks = 0;
    }

    world.dimensions.chunk_count_x = 0;
    world.dimensions.chunk_count_y = 0;
    world.dimensions.chunk_size_x = 0;
    world.dimensions.chunk_size_y = 0;
    world.dimensions.tile_count_x = 0;
    world.dimensions.tile_count_y = 0;
}

const WorldDimensions &world_get_dimensions(const World &world)
{
    return world.dimensions;
}
