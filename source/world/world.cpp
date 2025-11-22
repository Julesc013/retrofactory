#include "world/world.h"
#include "world/worldgen.h"

namespace
{
    const u32 kChunkSizeX = 16u;
    const u32 kChunkSizeY = 16u;
    const u32 kChunkCountX = 4u;
    const u32 kChunkCountY = 4u;

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
    const u64 total_tiles = static_cast<u64>(dimensions.tile_count_x) * static_cast<u64>(dimensions.tile_count_y);
    if (total_tiles == 0u)
    {
        return false;
    }

    if (world.tiles != 0)
    {
        delete[] world.tiles;
        world.tiles = 0;
        world.tile_count = 0u;
    }
    if (world.chunks != 0)
    {
        delete[] world.chunks;
        world.chunks = 0;
        world.chunk_count = 0u;
    }

    world.dimensions = dimensions;
    world.tile_count = static_cast<u32>(total_tiles);
    world.chunk_count = dimensions.chunk_count_x * dimensions.chunk_count_y;
    world.tiles = new Tile[world.tile_count];
    world.chunks = new Chunk[world.chunk_count];

    /* Zero-initialize terrain. */
    u32 index;
    for (index = 0u; index < world.tile_count; ++index)
    {
        world.tiles[index].terrain_type = 0u;
    }

    /* Build chunk metadata and tile pointers. */
    const u32 chunk_size_x = dimensions.chunk_size_x;
    const u32 chunk_size_y = dimensions.chunk_size_y;
    u32 chunk_y;
    u32 chunk_index = 0u;
    for (chunk_y = 0u; chunk_y < dimensions.chunk_count_y; ++chunk_y)
    {
        u32 chunk_x;
        for (chunk_x = 0u; chunk_x < dimensions.chunk_count_x; ++chunk_x)
        {
            const u32 origin_x = chunk_x * chunk_size_x;
            const u32 origin_y = chunk_y * chunk_size_y;
            const u32 tile_offset = origin_y * dimensions.tile_count_x + origin_x;
            chunk_init(world.chunks[chunk_index], origin_x, origin_y, chunk_size_x, chunk_size_y, world.tiles + tile_offset);
            chunk_index += 1u;
        }
    }

    return true;
}

bool world_init(World &world, u64 seed, const PrototypeStore *prototypes)
{
    const WorldDimensions dims = build_default_dimensions();
    if (!world_resize(world, dims))
    {
        return false;
    }
    return worldgen_generate(world, seed, prototypes);
}

void world_shutdown(World &world)
{
    if (world.tiles != 0)
    {
        delete[] world.tiles;
        world.tiles = 0;
        world.tile_count = 0u;
    }
    if (world.chunks != 0)
    {
        delete[] world.chunks;
        world.chunks = 0;
        world.chunk_count = 0u;
    }

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

Tile *world_get_tile(World &world, u32 x, u32 y)
{
    const u32 width = world.dimensions.tile_count_x;
    const u32 height = world.dimensions.tile_count_y;
    if (x >= width || y >= height || world.tiles == 0)
    {
        return 0;
    }

    const u64 index = static_cast<u64>(y) * static_cast<u64>(width) + static_cast<u64>(x);
    if (index >= world.tile_count)
    {
        return 0;
    }

    return &world.tiles[index];
}

const Tile *world_get_tile(const World &world, u32 x, u32 y)
{
    const u32 width = world.dimensions.tile_count_x;
    const u32 height = world.dimensions.tile_count_y;
    if (x >= width || y >= height || world.tiles == 0)
    {
        return 0;
    }

    const u64 index = static_cast<u64>(y) * static_cast<u64>(width) + static_cast<u64>(x);
    if (index >= world.tile_count)
    {
        return 0;
    }

    return &world.tiles[index];
}
