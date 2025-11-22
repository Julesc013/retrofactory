#include "render/rend_pick.h"
#include "render/rend_sw.h"
#include "render/rend_dx5.h"

bool rend_pick_init(RenderBackend backend)
{
    switch (backend)
    {
    case RenderBackend_Software:
        return rend_sw_init();
    case RenderBackend_DX5:
        return rend_dx5_init();
    default:
        return false;
    }
}

bool rend_pick_frame(RenderBackend backend, RenderContext &ctx)
{
    switch (backend)
    {
    case RenderBackend_Software:
        return rend_sw_frame(ctx);
    case RenderBackend_DX5:
        return rend_dx5_frame(ctx);
    default:
        return false;
    }
}

void rend_pick_shutdown(RenderBackend backend)
{
    switch (backend)
    {
    case RenderBackend_Software:
        rend_sw_shutdown();
        break;
    case RenderBackend_DX5:
        rend_dx5_shutdown();
        break;
    default:
        break;
    }
}
