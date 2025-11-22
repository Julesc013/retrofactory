#include "render/rend_api.h"

#include "utility/hash.h"
#include "world/world.h"

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

    const u32 palette[4] = {
        0xFF303030u, /* plains */
        0xFF2E7D32u, /* foliage */
        0xFF8D6E63u, /* ore/rock */
        0xFF1E88E5u  /* water */
    };

    u32 y;
    for (y = 0u; y < h; ++y)
    {
        const u32 ty = (y * dim.tile_count_y) / h;
        u32 x;
        for (x = 0u; x < w; ++x)
        {
            const u32 tx = (x * dim.tile_count_x) / w;
            const Tile *tile = world_get_tile(world, tx, ty);
            const u8 terrain = tile ? (tile->terrain_type & 0x0Fu) : 0u;
            u32 color = palette[terrain % 4u];

            /* Light animation based on tick for headless validation. */
            const u8 tick_mod = static_cast<u8>(ctx.snapshot->tick & 0x1Fu);
            color ^= static_cast<u32>(tick_mod) << 16;
            buffer.pixels.data[y * w + x] = color;
        }
    }

    return true;
}
