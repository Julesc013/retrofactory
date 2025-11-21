#include "saveload/saveload.h"

#include <cstddef>
#include <cstdio>
#include <cstring>

#include "saveload/hash.h"
#include "world/world.h"

namespace
{
    const char kMagic[4] = {'R', 'F', 'S', '1'};
    const uint32 kVersion = 1u;

    bool write_all(const void *data, std::size_t size, std::FILE *file)
    {
        return std::fwrite(data, 1u, size, file) == size;
    }

    bool read_all(void *data, std::size_t size, std::FILE *file)
    {
        return std::fread(data, 1u, size, file) == size;
    }
}

bool save_core_state(const CoreState &state, const char *path)
{
    std::FILE *file = std::fopen(path, "wb");
    if (file == 0)
    {
        return false;
    }

    const WorldDimensions &dim = state.world.dimensions;
    const uint32 dims[6] = {
        dim.chunk_size_x,
        dim.chunk_size_y,
        dim.chunk_count_x,
        dim.chunk_count_y,
        dim.tile_count_x,
        dim.tile_count_y};

    bool ok = true;
    ok = ok && write_all(kMagic, sizeof(kMagic), file);
    ok = ok && write_all(&kVersion, sizeof(uint32), file);
    ok = ok && write_all(&state.tick, sizeof(uint64), file);
    ok = ok && write_all(&state.rng.state, sizeof(uint64), file);
    ok = ok && write_all(dims, sizeof(dims), file);

    if (!state.world.tiles.empty())
    {
        ok = ok && write_all(&state.world.tiles[0], state.world.tiles.size() * sizeof(Tile), file);
    }

    std::fclose(file);
    return ok;
}

bool load_core_state(CoreState &state, const char *path)
{
    std::FILE *file = std::fopen(path, "rb");
    if (file == 0)
    {
        return false;
    }

    char magic[4] = {0, 0, 0, 0};
    uint32 version = 0u;
    uint32 dims[6] = {0u, 0u, 0u, 0u, 0u, 0u};
    bool ok = true;

    ok = ok && read_all(magic, sizeof(magic), file);
    ok = ok && std::memcmp(magic, kMagic, sizeof(kMagic)) == 0;
    ok = ok && read_all(&version, sizeof(uint32), file);
    ok = ok && version == kVersion;
    ok = ok && read_all(&state.tick, sizeof(uint64), file);
    ok = ok && read_all(&state.rng.state, sizeof(uint64), file);
    ok = ok && read_all(dims, sizeof(dims), file);

    WorldDimensions dimensions;
    dimensions.chunk_size_x = dims[0];
    dimensions.chunk_size_y = dims[1];
    dimensions.chunk_count_x = dims[2];
    dimensions.chunk_count_y = dims[3];
    dimensions.tile_count_x = dims[4];
    dimensions.tile_count_y = dims[5];

    if (ok)
    {
        ok = world_resize(state.world, dimensions);
    }

    if (ok)
    {
        const std::size_t tile_bytes = state.world.tiles.size() * sizeof(Tile);
        ok = tile_bytes == 0u || read_all(&state.world.tiles[0], tile_bytes, file);
    }

    std::fclose(file);
    return ok;
}
