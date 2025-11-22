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

void entities_init(EntitiesState &state)
{
    array_init(state.entities);
    array_reserve(state.entities, 16u);
    state.next_id = 1u;
}

void entities_shutdown(EntitiesState &state)
{
    array_free(state.entities);
    state.next_id = 1u;
}

EntityId entities_spawn(EntitiesState &state, World &world, u32 proto_id, u32 x, u32 y)
{
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
        inst.state = static_cast<u8>((mix + tick) & 0xFFu);
    }
    return true;
}
