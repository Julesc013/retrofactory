#include "render/rend_sdl.h"

#include "present/present_api.h"

bool rend_sdl_init(const RenderInitParams *params, PresentHandle *presenter)
{
    (void)params;
    return presenter != 0;
}

bool rend_sdl_frame(RenderContext *ctx, PresentHandle *presenter)
{
    if (ctx == 0)
    {
        return false;
    }
    if (presenter != 0 && ctx->target == 0)
    {
        ctx->target = present_backbuffer(presenter);
    }
    return render_frame(ctx);
}

void rend_sdl_shutdown(PresentHandle *presenter)
{
    (void)presenter;
}

const RenderAdapter *rend_sdl_adapter(void)
{
    static RenderAdapter adapter = {rend_sdl_init, rend_sdl_frame, rend_sdl_shutdown};
    return &adapter;
}
