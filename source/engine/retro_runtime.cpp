#include "engine/retro_runtime.h"

#include "engine/engine.h"
#include "engine/snapshot.h"
#include "render/rend_api.h"

struct RetroRuntime
{
    EngineContext engine;
    RenderContext render_ctx;
    RenderBackbuffer backbuffer;
    SnapshotWorld snapshot;
    RetroPresenter presenter;
    bool ready;
};

namespace
{
    const u32 kDefaultWidth = 320u;
    const u32 kDefaultHeight = 200u;

    void ensure_backbuffer(RetroRuntime *rt)
    {
        if (rt == 0 || rt->backbuffer.width != 0u || rt->backbuffer.height != 0u)
        {
            return;
        }
        const u32 w = rt->render_ctx.camera.width != 0u ? rt->render_ctx.camera.width : kDefaultWidth;
        const u32 h = rt->render_ctx.camera.height != 0u ? rt->render_ctx.camera.height : kDefaultHeight;
        render_backbuffer_init(&rt->backbuffer, w, h);
        rt->render_ctx.target = &rt->backbuffer;
    }
}

RetroRuntime *retro_runtime_create(u32 camera_width, u32 camera_height, RetroPresenter presenter)
{
    RetroRuntime *rt = new RetroRuntime();
    rt->presenter = presenter;
    rt->ready = false;
    rt->render_ctx = make_render_context(0, 0);
    rt->render_ctx.camera.width = camera_width;
    rt->render_ctx.camera.height = camera_height;
    rt->backbuffer.width = 0u;
    rt->backbuffer.height = 0u;
    rt->backbuffer.size = 0u;
    rt->backbuffer.capacity = 0u;
    rt->backbuffer.pixels = 0;
    return rt;
}

void retro_runtime_destroy(RetroRuntime *rt)
{
    if (rt == 0)
    {
        return;
    }
    if (rt->ready)
    {
        engine_shutdown(rt->engine);
    }
    render_backbuffer_free(&rt->backbuffer);
    delete rt;
}

bool retro_runtime_init(RetroRuntime *rt, const char *config_path)
{
    if (rt == 0)
    {
        return false;
    }
    if (rt->ready)
    {
        return true;
    }
    if (!engine_init(rt->engine, config_path))
    {
        return false;
    }
    ensure_backbuffer(rt);
    rt->ready = true;
    return true;
}

bool retro_runtime_step(RetroRuntime *rt)
{
    if (rt == 0 || !rt->ready)
    {
        return false;
    }
    if (!engine_tick(rt->engine))
    {
        return false;
    }
    if (!snapshot_build(rt->engine.core_state, rt->snapshot))
    {
        return false;
    }
    rt->render_ctx.snapshot = &rt->snapshot;
    ensure_backbuffer(rt);
    return render_frame(&rt->render_ctx);
}

void retro_runtime_set_camera(RetroRuntime *rt, u32 camera_width, u32 camera_height)
{
    if (rt == 0)
    {
        return;
    }
    rt->render_ctx.camera.width = camera_width;
    rt->render_ctx.camera.height = camera_height;
}
