#include "core/entities.h"
#include "utility/hash.h"

static EntityId allocate_id(EntitiesState &state)
{
    /* Avoid zero as a sentinel. */
    if (state.next_id == 0u)
    {
        state.next_id = 1u;
    }
    EntityId id = state.next_id;
    state.next_id += 1u;
    return id;
}

void entities_init(EntitiesState &state, const PrototypeStore *prototypes)
{
    array_init(state.entities);
    array_reserve(state.entities, 16u);
    state.next_id = 1u;
    state.prototypes = prototypes;
}

void entities_shutdown(EntitiesState &state)
{
    array_free(state.entities);
    state.next_id = 1u;
    state.prototypes = 0;
}

EntityId entities_spawn(EntitiesState &state, World &world, u32 proto_id, u32 x, u32 y)
{
    if (state.prototypes != 0 && prototype_find_entity(*state.prototypes, proto_id) == 0)
    {
        return 0u;
    }

    Tile *tile = world_get_tile(world, x, y);
    if (tile == 0)
    {
        return 0u;
    }

    EntityInstance inst;
    inst.id = allocate_id(state);
    inst.proto_id = proto_id;
    inst.x = x;
    inst.y = y;
    inst.state = 0u;

    if (!array_push(state.entities, inst))
    {
        return 0u;
    }

    /* Tag tile occupancy deterministically. */
    tile->terrain_type = static_cast<u8>((tile->terrain_type & 0x0Fu) | 0x10u);
    return inst.id;
}

bool entities_tick(EntitiesState &state, World &world, Tick tick)
{
    const u32 count = state.entities.size;
    u32 i;
    for (i = 0u; i < count; ++i)
    {
        EntityInstance &inst = state.entities.data[i];
        /* Pseudo behaviour: bounce state using tile terrain hash. */
        const Tile *tile = world_get_tile(world, inst.x, inst.y);
        const u8 terrain = tile ? tile->terrain_type : 0u;
        const u32 mix = hash_combine32(static_cast<u32>(inst.id), static_cast<u32>(terrain));
        u32 speed = 1u;
        if (state.prototypes != 0)
        {
            const EntityPrototype *proto = prototype_find_entity(*state.prototypes, inst.proto_id);
            if (proto != 0 && proto->speed > 0u)
            {
                speed = proto->speed;
            }
        }
        inst.state = static_cast<u8>((mix + tick * speed) & 0xFFu);
    }
    return true;
}
