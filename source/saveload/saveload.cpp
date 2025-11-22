#include "saveload/saveload.h"

#include <stddef.h>
#include <cstdio>
#include <cstring>

#include "saveload/hash.h"
#include "config/version.h"
#include "world/world.h"
#include "schedule/events.h"
#include "utility/array.h"

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

    /* Core subsystems */
    const u32 entity_count = state.entities.entities.size;
    ok = ok && write_all(&state.entities.next_id, sizeof(EntityId), file);
    ok = ok && write_all(&entity_count, sizeof(u32), file);
    if (ok && entity_count > 0u)
    {
        const size_t bytes = static_cast<size_t>(entity_count * sizeof(EntityInstance));
        ok = ok && write_all(state.entities.entities.data, bytes, file);
    }

    const u32 power_count = state.networks.power.size;
    const u32 fluid_count = state.networks.fluid.size;
    ok = ok && write_all(&state.networks.next_id, sizeof(NetworkId), file);
    ok = ok && write_all(&power_count, sizeof(u32), file);
    if (ok && power_count > 0u)
    {
        const size_t bytes = static_cast<size_t>(power_count * sizeof(NetworkNode));
        ok = ok && write_all(state.networks.power.data, bytes, file);
    }
    ok = ok && write_all(&fluid_count, sizeof(u32), file);
    if (ok && fluid_count > 0u)
    {
        const size_t bytes = static_cast<size_t>(fluid_count * sizeof(NetworkNode));
        ok = ok && write_all(state.networks.fluid.data, bytes, file);
    }

    const u32 recipe_count = state.recipes.recipes.size;
    ok = ok && write_all(&state.recipes.next_id, sizeof(RecipeId), file);
    ok = ok && write_all(&recipe_count, sizeof(u32), file);
    if (ok && recipe_count > 0u)
    {
        const size_t bytes = static_cast<size_t>(recipe_count * sizeof(Recipe));
        ok = ok && write_all(state.recipes.recipes.data, bytes, file);
    }

    const u32 topic_count = state.research.topics.size;
    ok = ok && write_all(&state.research.active, sizeof(ResearchId), file);
    ok = ok && write_all(&state.research.progress, sizeof(u32), file);
    ok = ok && write_all(&topic_count, sizeof(u32), file);
    if (ok && topic_count > 0u)
    {
        const size_t bytes = static_cast<size_t>(topic_count * sizeof(ResearchTopic));
        ok = ok && write_all(state.research.topics.data, bytes, file);
    }

    ok = ok && write_all(&state.scheduler.processed, sizeof(u32), file);
    ok = ok && write_all(&state.scheduler.count, sizeof(u32), file);
    if (ok && state.scheduler.count > 0u)
    {
        const size_t ev_bytes = static_cast<size_t>(state.scheduler.count * sizeof(ScheduledEvent));
        ok = ok && write_all(state.scheduler.events, ev_bytes, file);
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
        u32 entity_count = 0u;
        ok = ok && read_all(&state.entities.next_id, sizeof(EntityId), file);
        ok = ok && read_all(&entity_count, sizeof(u32), file);
        if (ok)
        {
            array_clear(state.entities.entities);
            ok = array_resize(state.entities.entities, entity_count);
            if (ok && entity_count > 0u)
            {
                const size_t bytes = static_cast<size_t>(entity_count * sizeof(EntityInstance));
                ok = read_all(state.entities.entities.data, bytes, file);
            }
        }

        u32 power_count = 0u;
        u32 fluid_count = 0u;
        ok = ok && read_all(&state.networks.next_id, sizeof(NetworkId), file);
        ok = ok && read_all(&power_count, sizeof(u32), file);
        if (ok)
        {
            array_clear(state.networks.power);
            ok = array_resize(state.networks.power, power_count);
            if (ok && power_count > 0u)
            {
                const size_t bytes = static_cast<size_t>(power_count * sizeof(NetworkNode));
                ok = read_all(state.networks.power.data, bytes, file);
            }
        }
        ok = ok && read_all(&fluid_count, sizeof(u32), file);
        if (ok)
        {
            array_clear(state.networks.fluid);
            ok = array_resize(state.networks.fluid, fluid_count);
            if (ok && fluid_count > 0u)
            {
                const size_t bytes = static_cast<size_t>(fluid_count * sizeof(NetworkNode));
                ok = read_all(state.networks.fluid.data, bytes, file);
            }
        }

        u32 recipe_count = 0u;
        ok = ok && read_all(&state.recipes.next_id, sizeof(RecipeId), file);
        ok = ok && read_all(&recipe_count, sizeof(u32), file);
        if (ok)
        {
            array_clear(state.recipes.recipes);
            ok = array_resize(state.recipes.recipes, recipe_count);
            if (ok && recipe_count > 0u)
            {
                const size_t bytes = static_cast<size_t>(recipe_count * sizeof(Recipe));
                ok = read_all(state.recipes.recipes.data, bytes, file);
            }
        }

        u32 topic_count = 0u;
        ok = ok && read_all(&state.research.active, sizeof(ResearchId), file);
        ok = ok && read_all(&state.research.progress, sizeof(u32), file);
        ok = ok && read_all(&topic_count, sizeof(u32), file);
        if (ok)
        {
            array_clear(state.research.topics);
            ok = array_resize(state.research.topics, topic_count);
            if (ok && topic_count > 0u)
            {
                const size_t bytes = static_cast<size_t>(topic_count * sizeof(ResearchTopic));
                ok = read_all(state.research.topics.data, bytes, file);
            }
        }

        ok = ok && read_all(&state.scheduler.processed, sizeof(u32), file);
        ok = ok && read_all(&state.scheduler.count, sizeof(u32), file);
        if (ok && state.scheduler.count > 0u)
        {
            if (state.scheduler.count > kMaxScheduledEvents)
            {
                ok = false;
            }
            else
            {
                const size_t ev_bytes = static_cast<size_t>(state.scheduler.count * sizeof(ScheduledEvent));
                ok = ev_bytes == 0u || read_all(state.scheduler.events, ev_bytes, file);
            }
        }
    }

    if (ok)
    {
        state.rng.state = rng_state;
        state.rng.inc = rng_inc;
    }

    std::fclose(file);
    return ok;
}
