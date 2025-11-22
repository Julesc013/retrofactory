#include "mods/prototypes.h"

#include <cstring>

#include "utility/hash.h"

namespace
{
    template <typename T>
    bool upsert(Array<T> &arr, const char *id, const T &value, T &out_value)
    {
        if (id == 0)
        {
            return false;
        }
        u32 i;
        for (i = 0u; i < arr.size; ++i)
        {
            if (std::strncmp(arr.data[i].id, id, kProtoIdMax) == 0)
            {
                out_value = value;
                out_value.numeric_id = arr.data[i].numeric_id;
                arr.data[i] = out_value;
                return true;
            }
        }

        out_value = value;
        out_value.numeric_id = arr.size + 1u;
        return array_push(arr, out_value);
    }

    void clear_store(PrototypeStore &store)
    {
        array_clear(store.entities);
        array_clear(store.networks);
        array_clear(store.recipes);
        array_clear(store.research);
        array_clear(store.worldgen);
    }
}

void prototypes_init(PrototypeStore &store)
{
    array_init(store.entities);
    array_init(store.networks);
    array_init(store.recipes);
    array_init(store.research);
    array_init(store.worldgen);
}

void prototypes_free(PrototypeStore &store)
{
    array_free(store.entities);
    array_free(store.networks);
    array_free(store.recipes);
    array_free(store.research);
    array_free(store.worldgen);
}

const EntityPrototype *prototype_find_entity(const PrototypeStore &store, u32 numeric_id)
{
    if (numeric_id == 0u || numeric_id > store.entities.size)
    {
        return 0;
    }
    return &store.entities.data[numeric_id - 1u];
}

const EntityPrototype *prototype_find_entity_by_name(const PrototypeStore &store, const char *name)
{
    if (name == 0)
    {
        return 0;
    }
    u32 i;
    for (i = 0u; i < store.entities.size; ++i)
    {
        if (std::strncmp(store.entities.data[i].id, name, kProtoIdMax) == 0)
        {
            return &store.entities.data[i];
        }
    }
    return 0;
}

const NetworkPrototype *prototype_find_network(const PrototypeStore &store, u32 numeric_id)
{
    if (numeric_id == 0u || numeric_id > store.networks.size)
    {
        return 0;
    }
    return &store.networks.data[numeric_id - 1u];
}

const RecipePrototype *prototype_find_recipe(const PrototypeStore &store, u32 numeric_id)
{
    if (numeric_id == 0u || numeric_id > store.recipes.size)
    {
        return 0;
    }
    return &store.recipes.data[numeric_id - 1u];
}

const ResearchPrototype *prototype_find_research(const PrototypeStore &store, u32 numeric_id)
{
    if (numeric_id == 0u || numeric_id > store.research.size)
    {
        return 0;
    }
    return &store.research.data[numeric_id - 1u];
}

const WorldgenPrototype *prototype_find_worldgen(const PrototypeStore &store, u32 numeric_id)
{
    if (numeric_id == 0u || numeric_id > store.worldgen.size)
    {
        return 0;
    }
    return &store.worldgen.data[numeric_id - 1u];
}

bool prototype_upsert_entity(PrototypeStore &store, const EntityPrototype &proto)
{
    EntityPrototype stored = proto;
    if (stored.sprite_index == 0u)
    {
        stored.sprite_index = hash_string32(stored.id);
    }
    return upsert<EntityPrototype>(store.entities, stored.id, stored, stored);
}

bool prototype_upsert_network(PrototypeStore &store, const NetworkPrototype &proto)
{
    NetworkPrototype stored = proto;
    if (stored.capacity == 0u)
    {
        stored.capacity = 100u;
    }
    return upsert<NetworkPrototype>(store.networks, stored.id, stored, stored);
}

bool prototype_upsert_recipe(PrototypeStore &store, const RecipePrototype &proto)
{
    RecipePrototype stored = proto;
    if (stored.time == 0u)
    {
        stored.time = 1u;
    }
    if (stored.output_yield == 0u)
    {
        stored.output_yield = 1u;
    }
    return upsert<RecipePrototype>(store.recipes, stored.id, stored, stored);
}

bool prototype_upsert_research(PrototypeStore &store, const ResearchPrototype &proto)
{
    ResearchPrototype stored = proto;
    if (stored.cost == 0u)
    {
        stored.cost = 10u;
    }
    return upsert<ResearchPrototype>(store.research, stored.id, stored, stored);
}

bool prototype_upsert_worldgen(PrototypeStore &store, const WorldgenPrototype &proto)
{
    WorldgenPrototype stored = proto;
    if (stored.weight == 0u)
    {
        stored.weight = 1u;
    }
    stored.terrain = stored.terrain & 0x0Fu;
    return upsert<WorldgenPrototype>(store.worldgen, stored.biome, stored, stored);
}

void prototype_reset(PrototypeStore &store)
{
    clear_store(store);
}
