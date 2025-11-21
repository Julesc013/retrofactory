#include "saveload/saveload.h"

#include <stddef.h>
#include <cstdio>
#include <cstring>

#include "saveload/hash.h"
#include "config/version.h"
#include "world/world.h"

namespace
{
    const char kMagic[4] = {'R', 'F', 'S', '1'};
    const u32 kVersion = RF_SAVE_VERSION;

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
    const u32 dims[6] = {
        dim.chunk_size_x,
        dim.chunk_size_y,
        dim.chunk_count_x,
        dim.chunk_count_y,
        dim.tile_count_x,
        dim.tile_count_y};

    bool ok = true;
    ok = ok && write_all(kMagic, sizeof(kMagic), file);
    ok = ok && write_all(&kVersion, sizeof(u32), file);
    ok = ok && write_all(&state.tick, sizeof(u64), file);
    ok = ok && write_all(&state.rng.state, sizeof(u64), file);
    ok = ok && write_all(&state.rng.inc, sizeof(u64), file);
    ok = ok && write_all(dims, sizeof(dims), file);

    if (state.world.tiles != 0 && state.world.tile_count > 0u)
    {
        ok = ok && write_all(state.world.tiles, static_cast<size_t>(state.world.tile_count * sizeof(Tile)), file);
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
    u32 version = 0u;
    u32 dims[6] = {0u, 0u, 0u, 0u, 0u, 0u};
    u64 rng_state = 0u;
    u64 rng_inc = 0u;
    bool ok = true;

    ok = ok && read_all(magic, sizeof(magic), file);
    ok = ok && std::memcmp(magic, kMagic, sizeof(kMagic)) == 0;
    ok = ok && read_all(&version, sizeof(u32), file);
    ok = ok && version == kVersion;
    ok = ok && read_all(&state.tick, sizeof(u64), file);
    ok = ok && read_all(&rng_state, sizeof(u64), file);
    ok = ok && read_all(&rng_inc, sizeof(u64), file);
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
        const size_t tile_bytes = static_cast<size_t>(state.world.tile_count * sizeof(Tile));
        ok = tile_bytes == 0u || read_all(state.world.tiles, tile_bytes, file);
    }

    if (ok)
    {
        state.rng.state = rng_state;
        state.rng.inc = rng_inc;
    }

    std::fclose(file);
    return ok;
}
