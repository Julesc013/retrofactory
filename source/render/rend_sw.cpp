#include "render/rend_sw.h"

namespace
{
    RenderBackbuffer g_backbuffer;
    const u32 kDefaultWidth = 320u;
    const u32 kDefaultHeight = 200u;
}

bool rend_sw_init()
{
    return render_backbuffer_init(g_backbuffer, kDefaultWidth, kDefaultHeight);
}

bool rend_sw_frame(RenderContext &ctx)
{
    if (ctx.target == 0)
    {
        ctx.target = &g_backbuffer;
    }
    return render_frame(ctx);
}

void rend_sw_shutdown()
{
    render_backbuffer_free(g_backbuffer);
}

const RenderBackbuffer &rend_sw_backbuffer()
{
    return g_backbuffer;
}
