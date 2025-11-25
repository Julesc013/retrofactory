#include "render/rend_dx.h"

bool rend_dx_init(const RenderInitParams *params, PresentHandle *presenter)
{
    (void)params;
    (void)presenter;
    return false; /* DirectX render path is stubbed for now. */
}

bool rend_dx_frame(RenderContext *ctx, PresentHandle *presenter)
{
    (void)ctx;
    (void)presenter;
    return false;
}

void rend_dx_shutdown(PresentHandle *presenter)
{
    (void)presenter;
}

const RenderAdapter *rend_dx_adapter(void)
{
    static RenderAdapter adapter = {rend_dx_init, rend_dx_frame, rend_dx_shutdown};
    return &adapter;
}
