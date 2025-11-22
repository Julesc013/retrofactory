#include "present/dx5/pres_dx5.h"
#include "render/rend_pick.h"
#include "render/rend_dx5.h"

namespace
{
    RenderBackbuffer g_dx_buffer;
    bool g_ready = false;
    u64 g_last_checksum = 0u;
    u32 g_presented = 0u;
}

bool pres_dx5_init()
{
    g_ready = rend_dx5_init();
    if (g_ready)
    {
        g_ready = render_backbuffer_init(g_dx_buffer, 640u, 480u);
    }
    g_last_checksum = 0u;
    g_presented = 0u;
    return g_ready;
}

bool pres_dx5_present(RenderContext &ctx)
{
    if (!g_ready && !pres_dx5_init())
    {
        return false;
    }
    if (ctx.target == 0)
    {
        ctx.target = &g_dx_buffer;
    }
    if (!rend_dx5_frame(ctx))
    {
        return false;
    }
    g_last_checksum = render_backbuffer_checksum(*ctx.target);
    g_presented += 1u;
    return true;
}

void pres_dx5_shutdown()
{
    if (g_ready)
    {
        rend_dx5_shutdown();
        render_backbuffer_free(g_dx_buffer);
        g_ready = false;
    }
}

u64 pres_dx5_last_checksum()
{
    return g_last_checksum;
}

u32 pres_dx5_frame_count()
{
    return g_presented;
}
