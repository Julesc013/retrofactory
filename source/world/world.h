#pragma once

#include "system/types.h"
#include "world/tile.h"

struct WorldDimensions
{
    u32 chunk_size_x;
    u32 chunk_size_y;
    u32 chunk_count_x;
    u32 chunk_count_y;
    u32 tile_count_x;
    u32 tile_count_y;
};

struct World
{
    WorldDimensions dimensions;
    Tile *tiles;
    u32 tile_count;

    World() : dimensions(), tiles(0), tile_count(0u) {}
};

bool world_init(World &world, u64 seed);
bool world_resize(World &world, const WorldDimensions &dimensions);
void world_shutdown(World &world);

const WorldDimensions &world_get_dimensions(const World &world);
Tile *world_get_tile(World &world, u32 x, u32 y);
const Tile *world_get_tile(const World &world, u32 x, u32 y);
