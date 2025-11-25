#include <cstdio>

#include "core/core.h"
#include "saveload/saveload.h"
#include "saveload/hash.h"
#include "world/world.h"
#include "engine/snapshot.h"
#include "engine/replay.h"
#include "render/rend_api.h"

namespace
{
    const uint64 kTestSeed = 0xCAFEBABEULL;
    const uint32 kTestTicks = 500u;
    const char kTestPath[] = "test_state.rfs";
}

int main()
{
    CoreConfig config;
    config.initial_seed = kTestSeed;

    CoreState original_state;
    if (!core_init(original_state, config))
    {
        return 1;
    }

    uint32 tick_index;
    for (tick_index = 0u; tick_index < kTestTicks; ++tick_index)
    {
        if (!core_tick(original_state))
        {
            return 1;
        }
    }

    Tile *sample_tile = world_get_tile(original_state.world, 4, 4);
    if (sample_tile == 0)
    {
        core_shutdown(original_state);
        return 1;
    }
    sample_tile->terrain_type = 7u;

    const uint64 hash_before = compute_state_hash(original_state);

    if (!save_core_state(original_state, kTestPath))
    {
        return 1;
    }

    CoreState loaded_state;
    if (!core_init(loaded_state, config))
    {
        std::remove(kTestPath);
        core_shutdown(original_state);
        return 1;
    }

    if (!load_core_state(loaded_state, kTestPath))
    {
        std::remove(kTestPath);
        core_shutdown(original_state);
        core_shutdown(loaded_state);
        return 1;
    }

    const uint64 hash_after = compute_state_hash(loaded_state);
    std::remove(kTestPath);

    if (original_state.tick != loaded_state.tick)
    {
        return 1;
    }

    if (original_state.rng.state != loaded_state.rng.state)
    {
        core_shutdown(original_state);
        core_shutdown(loaded_state);
        return 1;
    }

    if (hash_before != hash_after)
    {
        core_shutdown(original_state);
        core_shutdown(loaded_state);
        return 1;
    }

    Tile *loaded_tile = world_get_tile(loaded_state.world, 4, 4);
    if (loaded_tile == 0 || loaded_tile->terrain_type != sample_tile->terrain_type)
    {
        core_shutdown(original_state);
        core_shutdown(loaded_state);
        return 1;
    }

    /* Build snapshot and record replay frame to ensure hashing path is stable. */
    SnapshotWorld snapshot;
    if (!snapshot_build(loaded_state, snapshot))
    {
        core_shutdown(original_state);
        core_shutdown(loaded_state);
        return 1;
    }

    ReplayFrame frame;
    if (!replay_record_frame(snapshot, frame) || frame.tick != loaded_state.tick)
    {
        core_shutdown(original_state);
        core_shutdown(loaded_state);
        return 1;
    }

    RenderBackbuffer buffer;
    render_backbuffer_init(&buffer, 64u, 64u);
    RenderContext rc = make_render_context(&snapshot, &buffer);
    if (!render_frame(&rc))
    {
        render_backbuffer_free(&buffer);
        core_shutdown(original_state);
        core_shutdown(loaded_state);
        return 1;
    }
    const u64 checksum = render_backbuffer_checksum(&buffer);
    render_backbuffer_free(&buffer);
    if (checksum == 0u)
    {
        core_shutdown(original_state);
        core_shutdown(loaded_state);
        return 1;
    }

    core_shutdown(original_state);
    core_shutdown(loaded_state);
    return 0;
}
