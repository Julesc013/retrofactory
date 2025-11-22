#include "present/dx5/pres_dx5.h"
#include "render/rend_pick.h"
#include "render/rend_dx5.h"

namespace
{
    RenderBackbuffer g_dx_buffer;
    bool g_ready = false;
}

bool pres_dx5_init()
{
    g_ready = rend_dx5_init();
    if (g_ready)
    {
        g_ready = render_backbuffer_init(g_dx_buffer, 640u, 480u);
    }
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
    return rend_dx5_frame(ctx);
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
