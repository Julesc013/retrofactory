#include "render/rend_api.h"

#include "utility/hash.h"
#include "world/world.h"
#include "mods/prototypes.h"

namespace
{
    RenderCamera resolve_camera(const RenderContext &ctx)
    {
        RenderCamera cam = ctx.camera;
        const SnapshotWorld *snapshot = ctx.snapshot;
        if ((cam.width == 0u || cam.height == 0u) && snapshot != 0 && snapshot->world != 0)
        {
            cam.width = snapshot->world->dimensions.tile_count_x;
            cam.height = snapshot->world->dimensions.tile_count_y;
        }
        if (cam.width == 0u)
        {
            cam.width = ctx.target ? ctx.target->width : 1u;
        }
        if (cam.height == 0u)
        {
            cam.height = ctx.target ? ctx.target->height : 1u;
        }
        if (snapshot != 0 && snapshot->world != 0)
        {
            const WorldDimensions &dim = snapshot->world->dimensions;
            if (cam.origin_x == 0 && cam.width < dim.tile_count_x)
            {
                cam.origin_x = static_cast<i32>((dim.tile_count_x - cam.width) / 2u);
            }
            if (cam.origin_y == 0 && cam.height < dim.tile_count_y)
            {
                cam.origin_y = static_cast<i32>((dim.tile_count_y - cam.height) / 2u);
            }
            if (cam.origin_x < 0)
            {
                cam.origin_x = 0;
            }
            if (cam.origin_y < 0)
            {
                cam.origin_y = 0;
            }
        }
        return cam;
    }

    u32 tile_color_from_proto(const PrototypeStore *store, u8 terrain)
    {
        if (store == 0 || store->worldgen.size == 0u)
        {
            const u32 palette[4] = {
                0xFF303030u, /* plains */
                0xFF2E7D32u, /* foliage */
                0xFF8D6E63u, /* ore/rock */
                0xFF1E88E5u  /* water */
            };
            return palette[terrain % 4u];
        }

        const WorldgenPrototype *match = 0;
        u32 i;
        for (i = 0u; i < store->worldgen.size; ++i)
        {
            const WorldgenPrototype &proto = store->worldgen.data[i];
            if ((proto.terrain & 0x0Fu) == terrain)
            {
                match = &proto;
                break;
            }
        }
        if (match == 0)
        {
            match = &store->worldgen.data[terrain % store->worldgen.size];
        }

        const u32 hash = hash_string32(match->biome);
        const u8 r = static_cast<u8>((hash >> 16) & 0x7Fu) + 64u;
        const u8 g = static_cast<u8>((hash >> 8) & 0x7Fu) + 64u;
        const u8 b = static_cast<u8>(hash & 0x7Fu) + 64u;
        return 0xFF000000u | (static_cast<u32>(r) << 16) | (static_cast<u32>(g) << 8) | b;
    }

    u32 entity_color_from_proto(const EntityInstance &inst, const SnapshotWorld *snapshot)
    {
        u32 hash = hash_combine32(static_cast<u32>(inst.id), inst.proto_id);
        if (snapshot != 0 && snapshot->prototypes != 0)
        {
            const EntityPrototype *proto = prototype_find_entity(*snapshot->prototypes, inst.proto_id);
            if (proto != 0)
            {
                hash = hash_combine32(proto->sprite_index, inst.state);
            }
        }
        const u8 r = static_cast<u8>((hash >> 16) & 0xFFu);
        const u8 g = static_cast<u8>((hash >> 8) & 0xFFu);
        const u8 b = static_cast<u8>(hash & 0xFFu);
        return 0xFF000000u | (static_cast<u32>(r) << 16) | (static_cast<u32>(g) << 8) | b;
    }
}

bool render_backbuffer_init(RenderBackbuffer &buffer, u32 width, u32 height)
{
    array_init(buffer.pixels);
    buffer.width = width;
    buffer.height = height;
    const u32 total = width * height;
    return array_resize(buffer.pixels, total);
}

void render_backbuffer_free(RenderBackbuffer &buffer)
{
    array_free(buffer.pixels);
    buffer.width = 0u;
    buffer.height = 0u;
}

u64 render_backbuffer_checksum(const RenderBackbuffer &buffer)
{
    if (buffer.pixels.data == 0 || buffer.pixels.size == 0u)
    {
        return 0u;
    }
    return hash_bytes64(buffer.pixels.data, buffer.pixels.size * sizeof(u32));
}

bool render_frame(RenderContext &ctx)
{
    if (ctx.target == 0 || ctx.target->pixels.data == 0)
    {
        return false;
    }

    RenderBackbuffer &buffer = *ctx.target;
    const u32 w = buffer.width;
    const u32 h = buffer.height;
    const u32 total = w * h;

    u32 i;
    for (i = 0u; i < total; ++i)
    {
        buffer.pixels.data[i] = 0xFF000000u; /* opaque black */
    }

    if (ctx.snapshot == 0 || ctx.snapshot->world == 0)
    {
        return true;
    }

    const World &world = *ctx.snapshot->world;
    const WorldDimensions &dim = world.dimensions;
    if (dim.tile_count_x == 0u || dim.tile_count_y == 0u)
    {
        return true;
    }

    const RenderCamera cam = resolve_camera(ctx);
    const PrototypeStore *store = ctx.snapshot->prototypes;

    u32 y;
    for (y = 0u; y < h; ++y)
    {
        const i32 ty = cam.origin_y + static_cast<i32>((y * cam.height) / (h == 0u ? 1u : h));
        u32 x;
        for (x = 0u; x < w; ++x)
        {
            const i32 tx = cam.origin_x + static_cast<i32>((x * cam.width) / (w == 0u ? 1u : w));
            if (tx < 0 || ty < 0 || static_cast<u32>(tx) >= dim.tile_count_x || static_cast<u32>(ty) >= dim.tile_count_y)
            {
                continue;
            }
            const Tile *tile = world_get_tile(world, static_cast<u32>(tx), static_cast<u32>(ty));
            const u8 terrain = tile ? (tile->terrain_type & 0x0Fu) : 0u;
            u32 color = tile_color_from_proto(store, terrain);

            const u8 tick_mod = static_cast<u8>(ctx.snapshot->tick & 0x0Fu);
            color ^= static_cast<u32>(tick_mod) << 20;
            buffer.pixels.data[y * w + x] = color;
        }
    }

    if (ctx.snapshot->entities != 0 && ctx.snapshot->entities->entities.data != 0)
    {
        const u32 count = ctx.snapshot->entities->entities.size;
        u32 idx;
        for (idx = 0u; idx < count; ++idx)
        {
            const EntityInstance &inst = ctx.snapshot->entities->entities.data[idx];
            const i32 ex = static_cast<i32>(inst.x) - cam.origin_x;
            const i32 ey = static_cast<i32>(inst.y) - cam.origin_y;
            if (ex < 0 || ey < 0 || static_cast<u32>(ex) >= cam.width || static_cast<u32>(ey) >= cam.height)
            {
                continue;
            }
            const u32 px = static_cast<u32>((ex * static_cast<i32>(w)) / (cam.width == 0u ? 1 : static_cast<i32>(cam.width)));
            const u32 py = static_cast<u32>((ey * static_cast<i32>(h)) / (cam.height == 0u ? 1 : static_cast<i32>(cam.height)));
            if (px < w && py < h)
            {
                buffer.pixels.data[py * w + px] = entity_color_from_proto(inst, ctx.snapshot);
            }
        }
    }

    return true;
}
