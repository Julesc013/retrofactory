#pragma once

#include "system/types.h"

typedef u32 PlanetId;
typedef u32 SurfaceId;
typedef u32 ColonyId;
typedef u32 RegionId;
typedef u32 ChunkId;
typedef u32 EntityId;
typedef u32 TileId;
typedef u32 ResearchId;
typedef u32 RecipeId;
typedef u32 NetworkId;

static inline bool planet_id_is_valid(PlanetId id) { return id != 0u; }
static inline bool surface_id_is_valid(SurfaceId id) { return id != 0u; }
static inline bool colony_id_is_valid(ColonyId id) { return id != 0u; }
static inline bool region_id_is_valid(RegionId id) { return id != 0u; }
static inline bool chunk_id_is_valid(ChunkId id) { return id != 0u; }
static inline bool entity_id_is_valid(EntityId id) { return id != 0u; }
static inline bool tile_id_is_valid(TileId id) { return id != 0u; }
