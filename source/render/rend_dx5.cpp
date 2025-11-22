#include "render/rend_dx5.h"

namespace
{
    RenderBackbuffer g_dx_buffer;
    const u32 kDxWidth = 640u;
    const u32 kDxHeight = 480u;
}

bool rend_dx5_init()
{
    return render_backbuffer_init(g_dx_buffer, kDxWidth, kDxHeight);
}

bool rend_dx5_frame(RenderContext &ctx)
{
    if (ctx.target == 0)
    {
        ctx.target = &g_dx_buffer;
    }
    return render_frame(ctx);
}

void rend_dx5_shutdown()
{
    render_backbuffer_free(g_dx_buffer);
}
