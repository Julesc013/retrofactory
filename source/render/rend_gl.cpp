#include "render/rend_gl.h"

bool rend_gl_init(const RenderInitParams *params, PresentHandle *presenter)
{
    (void)params;
    (void)presenter;
    return false; /* OpenGL path stubbed. */
}

bool rend_gl_frame(RenderContext *ctx, PresentHandle *presenter)
{
    (void)ctx;
    (void)presenter;
    return false;
}

void rend_gl_shutdown(PresentHandle *presenter)
{
    (void)presenter;
}

const RenderAdapter *rend_gl_adapter(void)
{
    static RenderAdapter adapter = {rend_gl_init, rend_gl_frame, rend_gl_shutdown};
    return &adapter;
}
