#include "world/chunk.h"

void chunk_init(Chunk &chunk, u32 ox, u32 oy, u32 sx, u32 sy, Tile *tile_base)
{
    chunk.origin_x = ox;
    chunk.origin_y = oy;
    chunk.size_x = sx;
    chunk.size_y = sy;
    chunk.tiles = tile_base;
}

Tile *chunk_get_tile(Chunk &chunk, u32 x, u32 y)
{
    if (x >= chunk.size_x || y >= chunk.size_y || chunk.tiles == 0)
    {
        return 0;
    }
    const u32 index = y * chunk.size_x + x;
    return &chunk.tiles[index];
}

const Tile *chunk_get_tile(const Chunk &chunk, u32 x, u32 y)
{
    if (x >= chunk.size_x || y >= chunk.size_y || chunk.tiles == 0)
    {
        return 0;
    }
    const u32 index = y * chunk.size_x + x;
    return &chunk.tiles[index];
}
