#include "render/rend_vk.h"

bool rend_vk_init(const RenderInitParams *params, PresentHandle *presenter)
{
    (void)params;
    (void)presenter;
    return false; /* Vulkan path stubbed. */
}

bool rend_vk_frame(RenderContext *ctx, PresentHandle *presenter)
{
    (void)ctx;
    (void)presenter;
    return false;
}

void rend_vk_shutdown(PresentHandle *presenter)
{
    (void)presenter;
}

const RenderAdapter *rend_vk_adapter(void)
{
    static RenderAdapter adapter = {rend_vk_init, rend_vk_frame, rend_vk_shutdown};
    return &adapter;
}
