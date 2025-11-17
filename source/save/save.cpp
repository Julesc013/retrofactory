#include "save/save.h"

#include <cstdio>
#include <cstddef>

#include "world/world_grid.h"
#include "world/world_space.h"

namespace
{
    const uint32 kSaveMagic = 0x52465331u; /* "RFS1" */
    const uint32 kSaveVersionWorld = 2u;

    bool write_bytes(std::FILE *file, const unsigned char *data, size_t size)
    {
        return std::fwrite(data, 1u, size, file) == size;
    }

    bool read_bytes(std::FILE *file, unsigned char *data, size_t size)
    {
        return std::fread(data, 1u, size, file) == size;
    }

    void encode_u32(uint32 value, unsigned char *out)
    {
        out[0] = static_cast<unsigned char>(value & 0xFFu);
        out[1] = static_cast<unsigned char>((value >> 8) & 0xFFu);
        out[2] = static_cast<unsigned char>((value >> 16) & 0xFFu);
        out[3] = static_cast<unsigned char>((value >> 24) & 0xFFu);
    }

    void encode_u64(uint64 value, unsigned char *out)
    {
        encode_u32(static_cast<uint32>(value & 0xFFFFFFFFULL), out);
        encode_u32(static_cast<uint32>(value >> 32), out + 4);
    }

    uint32 decode_u32(const unsigned char *data)
    {
        return static_cast<uint32>(data[0]) |
               (static_cast<uint32>(data[1]) << 8) |
               (static_cast<uint32>(data[2]) << 16) |
               (static_cast<uint32>(data[3]) << 24);
    }

    uint64 decode_u64(const unsigned char *data)
    {
        uint32 lo = decode_u32(data);
        uint32 hi = decode_u32(data + 4);
        return static_cast<uint64>(lo) | (static_cast<uint64>(hi) << 32);
    }

    bool write_u32(std::FILE *file, uint32 value)
    {
        unsigned char buffer[4];
        encode_u32(value, buffer);
        return write_bytes(file, buffer, sizeof(buffer));
    }

    bool write_u64(std::FILE *file, uint64 value)
    {
        unsigned char buffer[8];
        encode_u64(value, buffer);
        return write_bytes(file, buffer, sizeof(buffer));
    }

    bool read_u32(std::FILE *file, uint32 &value)
    {
        unsigned char buffer[4];
        if (!read_bytes(file, buffer, sizeof(buffer)))
        {
            return false;
        }
        value = decode_u32(buffer);
        return true;
    }

    bool read_u64(std::FILE *file, uint64 &value)
    {
        unsigned char buffer[8];
        if (!read_bytes(file, buffer, sizeof(buffer)))
        {
            return false;
        }
        value = decode_u64(buffer);
        return true;
    }
    bool write_world_section(std::FILE *file, const World &world)
    {
        const WorldDimensions &dim = world_get_dimensions(world);
        bool ok = true;
        ok = ok && write_u32(file, static_cast<uint32>(dim.chunk_count_x));
        ok = ok && write_u32(file, static_cast<uint32>(dim.chunk_count_y));
        ok = ok && write_u32(file, static_cast<uint32>(dim.chunk_size_x));
        ok = ok && write_u32(file, static_cast<uint32>(dim.chunk_size_y));

        const int32 chunk_total = dim.chunk_count_x * dim.chunk_count_y;
        const int32 tiles_per_chunk = dim.chunk_size_x * dim.chunk_size_y;

        int32 chunk_index;
        for (chunk_index = 0; chunk_index < chunk_total && ok; ++chunk_index)
        {
            const Chunk &chunk = world.chunks[chunk_index];
            int32 tile_index;
            for (tile_index = 0; tile_index < tiles_per_chunk; ++tile_index)
            {
                const unsigned char value = chunk.tiles[tile_index].terrain_type;
                ok = ok && write_bytes(file, &value, 1u);
            }
        }

        return ok;
    }

    bool read_world_section(std::FILE *file, World &world, uint32 version)
    {
        if (version < kSaveVersionWorld)
        {
            return true;
        }

        uint32 chunk_count_x_u = 0u;
        uint32 chunk_count_y_u = 0u;
        uint32 chunk_size_x_u = 0u;
        uint32 chunk_size_y_u = 0u;

        if (!read_u32(file, chunk_count_x_u) ||
            !read_u32(file, chunk_count_y_u) ||
            !read_u32(file, chunk_size_x_u) ||
            !read_u32(file, chunk_size_y_u))
        {
            return false;
        }

        const int32 chunk_count_x = static_cast<int32>(chunk_count_x_u);
        const int32 chunk_count_y = static_cast<int32>(chunk_count_y_u);
        const int32 chunk_size_x = static_cast<int32>(chunk_size_x_u);
        const int32 chunk_size_y = static_cast<int32>(chunk_size_y_u);

        if (!world_init_with_dimensions(world, chunk_count_x, chunk_count_y, chunk_size_x, chunk_size_y))
        {
            return false;
        }

        const int32 chunk_total = chunk_count_x * chunk_count_y;
        const int32 tiles_per_chunk = chunk_size_x * chunk_size_y;

        int32 chunk_index;
        for (chunk_index = 0; chunk_index < chunk_total; ++chunk_index)
        {
            Chunk &chunk = world.chunks[chunk_index];
            int32 tile_index;
            for (tile_index = 0; tile_index < tiles_per_chunk; ++tile_index)
            {
                unsigned char value = 0u;
                if (!read_bytes(file, &value, 1u))
                {
                    return false;
                }
                chunk.tiles[tile_index].terrain_type = value;
                chunk.tiles[tile_index].reserved[0] = 0u;
                chunk.tiles[tile_index].reserved[1] = 0u;
                chunk.tiles[tile_index].reserved[2] = 0u;
            }
        }

        return true;
    }
}

bool save_core_state(const CoreState &state, const char *path)
{
    if (path == 0)
    {
        return false;
    }

    std::FILE *file = std::fopen(path, "wb");
    if (file == 0)
    {
        return false;
    }

    bool ok = true;
    ok = ok && write_u32(file, kSaveMagic);
    ok = ok && write_u32(file, kSaveVersionWorld);
    ok = ok && write_u64(file, state.tick);
    ok = ok && write_u64(file, state.rng.state);
    ok = ok && write_world_section(file, state.world);

    if (std::fclose(file) != 0)
    {
        ok = false;
    }

    return ok;
}

bool load_core_state(CoreState &state, const char *path)
{
    if (path == 0)
    {
        return false;
    }

    std::FILE *file = std::fopen(path, "rb");
    if (file == 0)
    {
        return false;
    }

    if (state.world.chunks == 0)
    {
        if (!world_init(state.world))
        {
            std::fclose(file);
            return false;
        }
    }

    uint32 magic = 0u;
    uint32 version = 0u;
    uint64 tick = 0u;
    uint64 rng_state = 0u;

    bool ok = true;
    ok = ok && read_u32(file, magic);
    ok = ok && read_u32(file, version);

    if (!ok || magic != kSaveMagic || version < 1u || version > kSaveVersionWorld)
    {
        if (ok)
        {
            std::fclose(file);
        }
        return false;
    }

    ok = ok && read_u64(file, tick);
    ok = ok && read_u64(file, rng_state);
    ok = ok && read_world_section(file, state.world, version);

    if (std::fclose(file) != 0)
    {
        ok = false;
    }

    if (!ok)
    {
        return false;
    }

    state.tick = tick;
    state.rng.state = rng_state;
    return true;
}
