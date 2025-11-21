#include "world/world.h"

#include <cstddef>

namespace
{
    const uint32 kChunkSizeX = 16u;
    const uint32 kChunkSizeY = 16u;
    const uint32 kChunkCountX = 4u;
    const uint32 kChunkCountY = 4u;

    WorldDimensions build_default_dimensions()
    {
        WorldDimensions out;
        out.chunk_size_x = kChunkSizeX;
        out.chunk_size_y = kChunkSizeY;
        out.chunk_count_x = kChunkCountX;
        out.chunk_count_y = kChunkCountY;
        out.tile_count_x = out.chunk_size_x * out.chunk_count_x;
        out.tile_count_y = out.chunk_size_y * out.chunk_count_y;
        return out;
    }
}

bool world_resize(World &world, const WorldDimensions &dimensions)
{
    const uint64 total_tiles = static_cast<uint64>(dimensions.tile_count_x) * static_cast<uint64>(dimensions.tile_count_y);
    if (total_tiles == 0u)
    {
        return false;
    }

    world.dimensions = dimensions;
    world.tiles.clear();
    world.tiles.resize(static_cast<std::size_t>(total_tiles));
    return true;
}

bool world_init(World &world, uint64 /*seed*/)
{
    const WorldDimensions dims = build_default_dimensions();
    return world_resize(world, dims);
}

void world_shutdown(World &world)
{
    world.tiles.clear();
    world.dimensions.chunk_size_x = 0u;
    world.dimensions.chunk_size_y = 0u;
    world.dimensions.chunk_count_x = 0u;
    world.dimensions.chunk_count_y = 0u;
    world.dimensions.tile_count_x = 0u;
    world.dimensions.tile_count_y = 0u;
}

const WorldDimensions &world_get_dimensions(const World &world)
{
    return world.dimensions;
}

Tile *world_get_tile(World &world, uint32 x, uint32 y)
{
    const uint32 width = world.dimensions.tile_count_x;
    const uint32 height = world.dimensions.tile_count_y;
    if (x >= width || y >= height)
    {
        return 0;
    }

    const std::size_t index = static_cast<std::size_t>(y) * static_cast<std::size_t>(width) + static_cast<std::size_t>(x);
    return &world.tiles[index];
}

const Tile *world_get_tile(const World &world, uint32 x, uint32 y)
{
    const uint32 width = world.dimensions.tile_count_x;
    const uint32 height = world.dimensions.tile_count_y;
    if (x >= width || y >= height)
    {
        return 0;
    }

    const std::size_t index = static_cast<std::size_t>(y) * static_cast<std::size_t>(width) + static_cast<std::size_t>(x);
    return &world.tiles[index];
}
