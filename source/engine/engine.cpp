#include "engine/engine.h"
#include "render/rend_api.h"
#include "engine/snapshot.h"

namespace
{
    RenderBackendId pick_default_backend()
    {
        RenderBackendId id;
        id.family = REND_FAMILY_SDL;
        id.version = REND_VER_SDL2;
        return id;
    }
}

bool engine_init(EngineContext &ctx, const char *config_path)
{
    engine_state_init(ctx.state);
    if (!engine_config_init(ctx.config, config_path))
    {
        return false;
    }

    ctx.core_config.prototypes = &ctx.config.prototypes;
    ctx.core_config.initial_seed = 1u;
    if (!core_init(ctx.core_state, ctx.core_config))
    {
        return false;
    }

    RenderInitParams render_params;
    render_params.backend = pick_default_backend();
    render_params.width = 0;
    render_params.height = 0;
    render_params.fullscreen = 0;
    render_params.vsync = 0;
    render_params.native_window = 0;
    if (!render_init(&render_params))
    {
        render_params.backend.family = REND_FAMILY_SOFTWARE;
        render_params.backend.version = REND_VER_NONE;
        if (!render_init(&render_params))
        {
            return false;
        }
    }

    engine_state_set_phase(ctx.state, EnginePhase_Running);
    return true;
}

bool engine_tick(EngineContext &ctx)
{
    if (ctx.state.phase != EnginePhase_Running)
    {
        return false;
    }
    return core_tick(ctx.core_state);
}

bool engine_render(EngineContext &ctx, RenderContext &rc)
{
    if (ctx.state.phase != EnginePhase_Running)
    {
        return false;
    }

    SnapshotWorld snapshot;
    if (!snapshot_build(ctx.core_state, snapshot))
    {
        return false;
    }

    rc.snapshot = &snapshot;
    if (snapshot.world != 0)
    {
        const WorldDimensions &dim = snapshot.world->dimensions;
        if (rc.camera.width == 0u)
        {
            rc.camera.width = dim.tile_count_x > 48u ? 48u : dim.tile_count_x;
        }
        if (rc.camera.height == 0u)
        {
            rc.camera.height = dim.tile_count_y > 36u ? 36u : dim.tile_count_y;
        }
        if (snapshot.entities != 0 && snapshot.entities->entities.size > 0u)
        {
            const EntityInstance &focus = snapshot.entities->entities.data[0];
            const i32 half_w = static_cast<i32>(rc.camera.width / 2u);
            const i32 half_h = static_cast<i32>(rc.camera.height / 2u);
            rc.camera.origin_x = focus.x > static_cast<u32>(half_w) ? static_cast<i32>(focus.x) - half_w : 0;
            rc.camera.origin_y = focus.y > static_cast<u32>(half_h) ? static_cast<i32>(focus.y) - half_h : 0;
        }
    }
    return render_draw(&rc);
}

void engine_shutdown(EngineContext &ctx)
{
    render_shutdown();
    core_shutdown(ctx.core_state);
    prototypes_free(ctx.config.prototypes);
    engine_state_set_phase(ctx.state, EnginePhase_Shutdown);
}
