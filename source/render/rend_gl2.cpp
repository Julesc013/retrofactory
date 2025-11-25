#include "render/rend_gl2.h"

namespace
{
    RenderBackbuffer g_gl2_buffer;
}

bool rend_gl2_init(void)
{
    return render_backbuffer_init(&g_gl2_buffer, 400u, 300u);
}

bool rend_gl2_frame(RenderContext *ctx)
{
    if (ctx == 0)
    {
        return false;
    }
    if (ctx->target == 0)
    {
        ctx->target = &g_gl2_buffer;
    }
    return render_frame(ctx);
}

void rend_gl2_shutdown(void)
{
    render_backbuffer_free(&g_gl2_buffer);
}
