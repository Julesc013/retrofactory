#pragma once

#include "system/types.h"
#include "world/tile.h"
#include "world/chunk.h"
#include "core/travel_net.h"
#include "core/trans_net.h"
#include "mods/prototypes.h"

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
    Chunk *chunks;
    u32 chunk_count;
    TransGraph trans_power;
    TransGraph trans_fluid;
    TransGraph trans_data;
    TravelGraph travel_rail;
    TravelGraph travel_road;
    TravelGraph travel_water;
    TravelGraph travel_air;

    World()
        : dimensions(),
          tiles(0),
          tile_count(0u),
          chunks(0),
          chunk_count(0u),
          trans_power(),
          trans_fluid(),
          trans_data(),
          travel_rail(),
          travel_road(),
          travel_water(),
          travel_air()
    {
    }
};

bool world_init(World &world, u64 seed, const PrototypeStore *prototypes = 0);
bool world_resize(World &world, const WorldDimensions &dimensions);
void world_shutdown(World &world);

const WorldDimensions &world_get_dimensions(const World &world);
Tile *world_get_tile(World &world, u32 x, u32 y);
const Tile *world_get_tile(const World &world, u32 x, u32 y);
