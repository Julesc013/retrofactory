#pragma once

#include "system/types.h"
#include "utility/array.h"

enum
{
    kProtoIdMax = 64,
    kProtoPackMax = 64
};

struct EntityPrototype
{
    char id[kProtoIdMax];
    char sprite[kProtoIdMax];
    char pack[kProtoPackMax];
    u32 speed;
    u32 sprite_index;
    u32 numeric_id;
};

struct NetworkPrototype
{
    char id[kProtoIdMax];
    u32 capacity;
    u32 numeric_id;
};

struct RecipePrototype
{
    char id[kProtoIdMax];
    u32 time;
    u32 input_cost;
    u32 output_yield;
    u32 numeric_id;
};

struct ResearchPrototype
{
    char id[kProtoIdMax];
    u32 cost;
    u32 numeric_id;
};

struct WorldgenPrototype
{
    char biome[kProtoIdMax];
    u32 weight;
    u8 terrain;
    u32 numeric_id;
};

struct PrototypeStore
{
    Array<EntityPrototype> entities;
    Array<NetworkPrototype> networks;
    Array<RecipePrototype> recipes;
    Array<ResearchPrototype> research;
    Array<WorldgenPrototype> worldgen;
};

void prototypes_init(PrototypeStore &store);
void prototypes_free(PrototypeStore &store);

const EntityPrototype *prototype_find_entity(const PrototypeStore &store, u32 numeric_id);
const EntityPrototype *prototype_find_entity_by_name(const PrototypeStore &store, const char *name);
const NetworkPrototype *prototype_find_network(const PrototypeStore &store, u32 numeric_id);
const RecipePrototype *prototype_find_recipe(const PrototypeStore &store, u32 numeric_id);
const ResearchPrototype *prototype_find_research(const PrototypeStore &store, u32 numeric_id);
const WorldgenPrototype *prototype_find_worldgen(const PrototypeStore &store, u32 numeric_id);

bool prototype_upsert_entity(PrototypeStore &store, const EntityPrototype &proto);
bool prototype_upsert_network(PrototypeStore &store, const NetworkPrototype &proto);
bool prototype_upsert_recipe(PrototypeStore &store, const RecipePrototype &proto);
bool prototype_upsert_research(PrototypeStore &store, const ResearchPrototype &proto);
bool prototype_upsert_worldgen(PrototypeStore &store, const WorldgenPrototype &proto);

void prototype_reset(PrototypeStore &store);
