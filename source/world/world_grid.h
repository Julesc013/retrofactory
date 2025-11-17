#ifndef WORLD_WORLD_GRID_H
#define WORLD_WORLD_GRID_H

#include "system/system_base.h"
#include "world/world_space.h"

enum
{
    WORLD_CHUNK_SIZE_X = 8,
    WORLD_CHUNK_SIZE_Y = 8,
    WORLD_CHUNK_TILE_COUNT = WORLD_CHUNK_SIZE_X * WORLD_CHUNK_SIZE_Y
};

struct Tile
{
    uint8 terrain_type;
    uint8 reserved[3];
};

struct Chunk
{
    Tile tiles[WORLD_CHUNK_TILE_COUNT];
};

bool world_init_grid(World &world);
Tile *world_get_tile(World &world, int32 x, int32 y);

#endif /* WORLD_WORLD_GRID_H */
