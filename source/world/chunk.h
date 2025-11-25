#pragma once

#include "system/types.h"
#include "world/tile.h"

struct Chunk
{
    u32 origin_x;
    u32 origin_y;
    u32 size_x;
    u32 size_y;
    Tile *tiles;
};

void chunk_init(Chunk &chunk, u32 ox, u32 oy, u32 sx, u32 sy, Tile *tile_base);
Tile *chunk_get_tile(Chunk &chunk, u32 x, u32 y);
const Tile *chunk_get_tile(const Chunk &chunk, u32 x, u32 y);
