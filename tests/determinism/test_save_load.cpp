#include <cstdio>
#include <cstring>

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

    bool buffers_equal(const void *a, const void *b, size_t bytes)
    {
        if (a == 0 || b == 0)
        {
            return a == b;
        }
        return std::memcmp(a, b, bytes) == 0;
    }

    void apply_bytes(u64 &hash, const void *data, size_t size)
    {
        const unsigned char *bytes = static_cast<const unsigned char *>(data);
        size_t index;
        for (index = 0u; index < size; ++index)
        {
            hash ^= static_cast<u64>(bytes[index]);
            hash *= 1099511628211ULL; /* FNV prime */
        }
    }

    void apply_u32(u64 &hash, u32 value)
    {
        apply_bytes(hash, &value, sizeof(u32));
    }

    void apply_u64(u64 &hash, u64 value)
    {
        apply_bytes(hash, &value, sizeof(u64));
    }

    void trace_hash(const CoreState &state, const char *tag)
    {
        u64 hash = 1469598103934665603ULL; /* FNV offset basis */
#define TRACE(label) std::fprintf(stderr, "[%s] %s -> %llu\n", tag, label, static_cast<unsigned long long>(hash))

        apply_u64(hash, state.tick);
        apply_u64(hash, state.rng.state);
        apply_u64(hash, state.rng.inc);
        TRACE("tick+rng");

        const WorldDimensions &dimensions = state.world.dimensions;
        apply_u32(hash, dimensions.chunk_size_x);
        apply_u32(hash, dimensions.chunk_size_y);
        apply_u32(hash, dimensions.chunk_count_x);
        apply_u32(hash, dimensions.chunk_count_y);
        apply_u32(hash, dimensions.tile_count_x);
        apply_u32(hash, dimensions.tile_count_y);
        TRACE("dimensions");

        if (state.world.tiles != 0 && state.world.tile_count > 0u)
        {
            apply_bytes(hash, state.world.tiles, static_cast<size_t>(state.world.tile_count * sizeof(Tile)));
        }
        TRACE("tiles");

        /* Transmission networks */
        apply_u32(hash, static_cast<u32>(state.world.trans_power.node_count));
        apply_u32(hash, static_cast<u32>(state.world.trans_power.edge_count));
        if (state.world.trans_power.nodes != 0 && state.world.trans_power.node_count > 0)
        {
            apply_bytes(hash, state.world.trans_power.nodes, static_cast<size_t>(state.world.trans_power.node_count * sizeof(TransNode)));
        }
        if (state.world.trans_power.edges != 0 && state.world.trans_power.edge_count > 0)
        {
            apply_bytes(hash, state.world.trans_power.edges, static_cast<size_t>(state.world.trans_power.edge_count * sizeof(TransEdge)));
        }
        TRACE("trans_power");

        apply_u32(hash, static_cast<u32>(state.world.trans_fluid.node_count));
        apply_u32(hash, static_cast<u32>(state.world.trans_fluid.edge_count));
        if (state.world.trans_fluid.nodes != 0 && state.world.trans_fluid.node_count > 0)
        {
            apply_bytes(hash, state.world.trans_fluid.nodes, static_cast<size_t>(state.world.trans_fluid.node_count * sizeof(TransNode)));
        }
        if (state.world.trans_fluid.edges != 0 && state.world.trans_fluid.edge_count > 0)
        {
            apply_bytes(hash, state.world.trans_fluid.edges, static_cast<size_t>(state.world.trans_fluid.edge_count * sizeof(TransEdge)));
        }
        TRACE("trans_fluid");

        apply_u32(hash, static_cast<u32>(state.world.trans_data.node_count));
        apply_u32(hash, static_cast<u32>(state.world.trans_data.edge_count));
        if (state.world.trans_data.nodes != 0 && state.world.trans_data.node_count > 0)
        {
            apply_bytes(hash, state.world.trans_data.nodes, static_cast<size_t>(state.world.trans_data.node_count * sizeof(TransNode)));
        }
        if (state.world.trans_data.edges != 0 && state.world.trans_data.edge_count > 0)
        {
            apply_bytes(hash, state.world.trans_data.edges, static_cast<size_t>(state.world.trans_data.edge_count * sizeof(TransEdge)));
        }
        TRACE("trans_data");

        /* Travel networks */
        apply_u32(hash, static_cast<u32>(state.world.travel_rail.node_count));
        apply_u32(hash, static_cast<u32>(state.world.travel_rail.segment_count));
        if (state.world.travel_rail.nodes != 0 && state.world.travel_rail.node_count > 0)
        {
            apply_bytes(hash, state.world.travel_rail.nodes, static_cast<size_t>(state.world.travel_rail.node_count * sizeof(TravelNode)));
        }
        if (state.world.travel_rail.segments != 0 && state.world.travel_rail.segment_count > 0)
        {
            apply_bytes(hash, state.world.travel_rail.segments, static_cast<size_t>(state.world.travel_rail.segment_count * sizeof(TravelSegment)));
        }
        TRACE("travel_rail");

        apply_u32(hash, static_cast<u32>(state.world.travel_road.node_count));
        apply_u32(hash, static_cast<u32>(state.world.travel_road.segment_count));
        if (state.world.travel_road.nodes != 0 && state.world.travel_road.node_count > 0)
        {
            apply_bytes(hash, state.world.travel_road.nodes, static_cast<size_t>(state.world.travel_road.node_count * sizeof(TravelNode)));
        }
        if (state.world.travel_road.segments != 0 && state.world.travel_road.segment_count > 0)
        {
            apply_bytes(hash, state.world.travel_road.segments, static_cast<size_t>(state.world.travel_road.segment_count * sizeof(TravelSegment)));
        }
        TRACE("travel_road");

        apply_u32(hash, static_cast<u32>(state.world.travel_water.node_count));
        apply_u32(hash, static_cast<u32>(state.world.travel_water.segment_count));
        if (state.world.travel_water.nodes != 0 && state.world.travel_water.node_count > 0)
        {
            apply_bytes(hash, state.world.travel_water.nodes, static_cast<size_t>(state.world.travel_water.node_count * sizeof(TravelNode)));
        }
        if (state.world.travel_water.segments != 0 && state.world.travel_water.segment_count > 0)
        {
            apply_bytes(hash, state.world.travel_water.segments, static_cast<size_t>(state.world.travel_water.segment_count * sizeof(TravelSegment)));
        }
        TRACE("travel_water");

        apply_u32(hash, static_cast<u32>(state.world.travel_air.node_count));
        apply_u32(hash, static_cast<u32>(state.world.travel_air.segment_count));
        if (state.world.travel_air.nodes != 0 && state.world.travel_air.node_count > 0)
        {
            apply_bytes(hash, state.world.travel_air.nodes, static_cast<size_t>(state.world.travel_air.node_count * sizeof(TravelNode)));
        }
        if (state.world.travel_air.segments != 0 && state.world.travel_air.segment_count > 0)
        {
            apply_bytes(hash, state.world.travel_air.segments, static_cast<size_t>(state.world.travel_air.segment_count * sizeof(TravelSegment)));
        }
        TRACE("travel_air");

        apply_u32(hash, state.entities.next_id);
        apply_u32(hash, state.entities.entities.size);
        if (state.entities.entities.size > 0u)
        {
            apply_bytes(hash, state.entities.entities.data, static_cast<size_t>(state.entities.entities.size * sizeof(EntityInstance)));
        }
        TRACE("entities");

        apply_u32(hash, state.recipes.next_id);
        apply_u32(hash, state.recipes.recipes.size);
        if (state.recipes.recipes.size > 0u)
        {
            apply_bytes(hash, state.recipes.recipes.data, static_cast<size_t>(state.recipes.recipes.size * sizeof(Recipe)));
        }
        TRACE("recipes");

        apply_u32(hash, state.research.active);
        apply_u32(hash, state.research.progress);
        apply_u32(hash, state.research.topics.size);
        if (state.research.topics.size > 0u)
        {
            apply_bytes(hash, state.research.topics.data, static_cast<size_t>(state.research.topics.size * sizeof(ResearchTopic)));
        }
        TRACE("research");

        apply_u32(hash, state.scheduler.processed);
        apply_u32(hash, state.scheduler.count);
        if (state.scheduler.count > 0u)
        {
            apply_bytes(hash, state.scheduler.events, static_cast<size_t>(state.scheduler.count * sizeof(ScheduledEvent)));
        }
        TRACE("scheduler");
#undef TRACE
    }

    void debug_diff(const CoreState &before, const CoreState &after)
    {
        const World &wa = before.world;
        const World &wb = after.world;
        if (std::memcmp(&wa.dimensions, &wb.dimensions, sizeof(WorldDimensions)) != 0)
        {
            std::fprintf(stderr, "diff: dimensions\n");
        }
        if (wa.tile_count != wb.tile_count)
        {
            std::fprintf(stderr, "diff: tile_count %u vs %u\n", wa.tile_count, wb.tile_count);
        }
        else if (wa.tiles != 0 && wb.tiles != 0 && !buffers_equal(wa.tiles, wb.tiles, static_cast<size_t>(wa.tile_count * sizeof(Tile))))
        {
            std::fprintf(stderr, "diff: tiles content\n");
        }

        if (wa.trans_power.node_count != wb.trans_power.node_count || wa.trans_power.edge_count != wb.trans_power.edge_count)
        {
            std::fprintf(stderr, "diff: trans_power counts\n");
        }
        if (wa.trans_fluid.node_count != wb.trans_fluid.node_count || wa.trans_fluid.edge_count != wb.trans_fluid.edge_count)
        {
            std::fprintf(stderr, "diff: trans_fluid counts\n");
        }
        if (wa.trans_data.node_count != wb.trans_data.node_count || wa.trans_data.edge_count != wb.trans_data.edge_count)
        {
            std::fprintf(stderr, "diff: trans_data counts\n");
        }

        if (wa.travel_rail.node_count != wb.travel_rail.node_count || wa.travel_rail.segment_count != wb.travel_rail.segment_count)
        {
            std::fprintf(stderr, "diff: travel_rail counts\n");
        }
        if (wa.travel_road.node_count != wb.travel_road.node_count || wa.travel_road.segment_count != wb.travel_road.segment_count)
        {
            std::fprintf(stderr, "diff: travel_road counts\n");
        }
        if (wa.travel_water.node_count != wb.travel_water.node_count || wa.travel_water.segment_count != wb.travel_water.segment_count)
        {
            std::fprintf(stderr, "diff: travel_water counts\n");
        }
        if (wa.travel_air.node_count != wb.travel_air.node_count || wa.travel_air.segment_count != wb.travel_air.segment_count)
        {
            std::fprintf(stderr, "diff: travel_air counts\n");
        }

        const EntitiesState &ea = before.entities;
        const EntitiesState &eb = after.entities;
        if (ea.next_id != eb.next_id || ea.entities.size != eb.entities.size)
        {
            std::fprintf(stderr, "diff: entities meta\n");
        }
        else if (ea.entities.size > 0u && !buffers_equal(ea.entities.data, eb.entities.data, static_cast<size_t>(ea.entities.size * sizeof(EntityInstance))))
        {
            std::fprintf(stderr, "diff: entities data\n");
        }

        const RecipeRegistry &ra = before.recipes;
        const RecipeRegistry &rb = after.recipes;
        if (ra.next_id != rb.next_id || ra.recipes.size != rb.recipes.size)
        {
            std::fprintf(stderr, "diff: recipes meta\n");
        }
        else if (ra.recipes.size > 0u && !buffers_equal(ra.recipes.data, rb.recipes.data, static_cast<size_t>(ra.recipes.size * sizeof(Recipe))))
        {
            std::fprintf(stderr, "diff: recipes data\n");
        }

        const ResearchState &sa = before.research;
        const ResearchState &sb = after.research;
        if (sa.active != sb.active || sa.progress != sb.progress || sa.topics.size != sb.topics.size)
        {
            std::fprintf(stderr, "diff: research meta\n");
        }
        else if (sa.topics.size > 0u && !buffers_equal(sa.topics.data, sb.topics.data, static_cast<size_t>(sa.topics.size * sizeof(ResearchTopic))))
        {
            std::fprintf(stderr, "diff: research topics\n");
        }

        const Scheduler &pa = before.scheduler;
        const Scheduler &pb = after.scheduler;
        if (pa.processed != pb.processed || pa.count != pb.count)
        {
            std::fprintf(stderr, "diff: scheduler meta\n");
        }
        else if (pa.count > 0u && !buffers_equal(pa.events, pb.events, static_cast<size_t>(pa.count * sizeof(ScheduledEvent))))
        {
            std::fprintf(stderr, "diff: scheduler events\n");
        }
    }
}

int main()
{
    CoreConfig config;
    config.initial_seed = kTestSeed;

    CoreState original_state;
    if (!core_init(original_state, config))
    {
        std::fprintf(stderr, "fail: core_init(original_state)\n");
        return 1;
    }

    uint32 tick_index;
    for (tick_index = 0u; tick_index < kTestTicks; ++tick_index)
    {
        if (!core_tick(original_state))
        {
            std::fprintf(stderr, "fail: core_tick at %u\n", tick_index);
            return 2;
        }
    }

    Tile *sample_tile = world_get_tile(original_state.world, 4, 4);
    if (sample_tile == 0)
    {
        core_shutdown(original_state);
        std::fprintf(stderr, "fail: sample_tile null\n");
        return 3;
    }
    sample_tile->terrain_type = 7u;

    const uint64 hash_before = compute_state_hash(original_state);

    if (!save_core_state(original_state, kTestPath))
    {
        std::fprintf(stderr, "fail: save_core_state\n");
        return 4;
    }

    CoreState loaded_state;
    if (!core_init(loaded_state, config))
    {
        std::remove(kTestPath);
        core_shutdown(original_state);
        std::fprintf(stderr, "fail: core_init(loaded_state)\n");
        return 5;
    }

    if (!load_core_state(loaded_state, kTestPath))
    {
        std::remove(kTestPath);
        core_shutdown(original_state);
        core_shutdown(loaded_state);
        std::fprintf(stderr, "fail: load_core_state\n");
        return 6;
    }

    const uint64 hash_after = compute_state_hash(loaded_state);
    std::remove(kTestPath);

    if (original_state.tick != loaded_state.tick)
    {
        std::fprintf(stderr, "fail: tick mismatch\n");
        return 7;
    }

    if (original_state.rng.state != loaded_state.rng.state)
    {
        core_shutdown(original_state);
        core_shutdown(loaded_state);
        std::fprintf(stderr, "fail: rng mismatch\n");
        return 8;
    }

    if (hash_before != hash_after)
    {
        std::fprintf(stderr, "fail: hash mismatch before=%llu after=%llu\n",
                     static_cast<unsigned long long>(hash_before),
                     static_cast<unsigned long long>(hash_after));
        debug_diff(original_state, loaded_state);
        trace_hash(original_state, "before");
        trace_hash(loaded_state, "after");
        core_shutdown(original_state);
        core_shutdown(loaded_state);
        return 9;
    }

    Tile *loaded_tile = world_get_tile(loaded_state.world, 4, 4);
    if (loaded_tile == 0 || loaded_tile->terrain_type != sample_tile->terrain_type)
    {
        core_shutdown(original_state);
        core_shutdown(loaded_state);
        std::fprintf(stderr, "fail: loaded_tile mismatch\n");
        return 10;
    }

    /* Build snapshot and record replay frame to ensure hashing path is stable. */
    SnapshotWorld snapshot;
    if (!snapshot_build(loaded_state, snapshot))
    {
        core_shutdown(original_state);
        core_shutdown(loaded_state);
        std::fprintf(stderr, "fail: snapshot_build\n");
        return 11;
    }

    ReplayFrame frame;
    if (!replay_record_frame(snapshot, frame) || frame.tick != loaded_state.tick)
    {
        core_shutdown(original_state);
        core_shutdown(loaded_state);
        std::fprintf(stderr, "fail: replay_record_frame\n");
        return 12;
    }

    RenderBackbuffer buffer;
    render_backbuffer_init(&buffer, 64u, 64u);
    RenderContext rc = make_render_context(&snapshot, &buffer);
    if (!render_frame(&rc))
    {
        render_backbuffer_free(&buffer);
        core_shutdown(original_state);
        core_shutdown(loaded_state);
        std::fprintf(stderr, "fail: render_frame\n");
        return 13;
    }
    const u64 checksum = render_backbuffer_checksum(&buffer);
    render_backbuffer_free(&buffer);
    if (checksum == 0u)
    {
        core_shutdown(original_state);
        core_shutdown(loaded_state);
        std::fprintf(stderr, "fail: checksum zero\n");
        return 14;
    }

    core_shutdown(original_state);
    core_shutdown(loaded_state);
    return 0;
}
