#include "render/rend_sw.h"

#include "present/present_api.h"

namespace
{
    RenderBackbuffer g_backbuffer;
    bool g_buffer_ready = false;
    const u32 kDefaultWidth = 320u;
    const u32 kDefaultHeight = 200u;

    RenderBackbuffer *select_target(RenderContext *ctx, PresentHandle *presenter)
    {
        if (ctx == 0)
        {
            return 0;
        }
        if (ctx->target != 0)
        {
            return ctx->target;
        }
        if (presenter != 0)
        {
            ctx->target = present_backbuffer(presenter);
            return ctx->target;
        }
        if (!g_buffer_ready)
        {
            g_buffer_ready = render_backbuffer_init(&g_backbuffer, kDefaultWidth, kDefaultHeight);
        }
        ctx->target = g_buffer_ready ? &g_backbuffer : 0;
        return ctx->target;
    }
}

bool rend_sw_init(const RenderInitParams *params, PresentHandle *presenter)
{
    (void)params;
    (void)presenter;
    if (presenter == 0 && !g_buffer_ready)
    {
        g_buffer_ready = render_backbuffer_init(&g_backbuffer, kDefaultWidth, kDefaultHeight);
    }
    return presenter != 0 || g_buffer_ready;
}

bool rend_sw_frame(RenderContext *ctx, PresentHandle *presenter)
{
    RenderBackbuffer *target = select_target(ctx, presenter);
    if (target == 0)
    {
        return false;
    }
    return render_frame(ctx);
}

void rend_sw_shutdown(PresentHandle *presenter)
{
    (void)presenter;
    if (g_buffer_ready)
    {
        render_backbuffer_free(&g_backbuffer);
        g_buffer_ready = false;
    }
}

const RenderBackbuffer *rend_sw_backbuffer(void)
{
    return &g_backbuffer;
}

const RenderAdapter *rend_sw_adapter(void)
{
    static RenderAdapter adapter = {rend_sw_init, rend_sw_frame, rend_sw_shutdown};
    return &adapter;
}
