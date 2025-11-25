#include "render/rend_metal.h"

bool rend_metal_init(const RenderInitParams *params, PresentHandle *presenter)
{
    (void)params;
    (void)presenter;
    return false; /* Metal path stubbed. */
}

bool rend_metal_frame(RenderContext *ctx, PresentHandle *presenter)
{
    (void)ctx;
    (void)presenter;
    return false;
}

void rend_metal_shutdown(PresentHandle *presenter)
{
    (void)presenter;
}

const RenderAdapter *rend_metal_adapter(void)
{
    static RenderAdapter adapter = {rend_metal_init, rend_metal_frame, rend_metal_shutdown};
    return &adapter;
}
