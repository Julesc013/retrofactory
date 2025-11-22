#include "render/rend_gl1.h"

namespace
{
    RenderBackbuffer g_gl1_buffer;
}

bool rend_gl1_init()
{
    return render_backbuffer_init(g_gl1_buffer, 320u, 200u);
}

bool rend_gl1_frame(RenderContext &ctx)
{
    if (ctx.target == 0)
    {
        ctx.target = &g_gl1_buffer;
    }
    return render_frame(ctx);
}

void rend_gl1_shutdown()
{
    render_backbuffer_free(g_gl1_buffer);
}
