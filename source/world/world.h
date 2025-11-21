#pragma once

#include <vector>

#include "system/types.h"
#include "world/tile.h"

struct WorldDimensions
{
    uint32 chunk_size_x;
    uint32 chunk_size_y;
    uint32 chunk_count_x;
    uint32 chunk_count_y;
    uint32 tile_count_x;
    uint32 tile_count_y;
};

struct World
{
    WorldDimensions dimensions;
    std::vector<Tile> tiles;
};

bool world_init(World &world, uint64 seed);
bool world_resize(World &world, const WorldDimensions &dimensions);
void world_shutdown(World &world);

const WorldDimensions &world_get_dimensions(const World &world);
Tile *world_get_tile(World &world, uint32 x, uint32 y);
const Tile *world_get_tile(const World &world, uint32 x, uint32 y);
