#ifndef WORLD_WORLD_SPACE_H
#define WORLD_WORLD_SPACE_H

#include "system/system_base.h"

struct Chunk;

struct WorldDimensions
{
    int32 chunk_count_x;
    int32 chunk_count_y;
    int32 chunk_size_x;
    int32 chunk_size_y;
    int32 tile_count_x;
    int32 tile_count_y;
};

struct World
{
    WorldDimensions dimensions;
    Chunk *chunks;

    World();
};

bool world_init(World &world);
bool world_init_with_dimensions(World &world, int32 chunk_count_x, int32 chunk_count_y, int32 chunk_size_x, int32 chunk_size_y);
void world_shutdown(World &world);
const WorldDimensions &world_get_dimensions(const World &world);

#endif /* WORLD_WORLD_SPACE_H */
