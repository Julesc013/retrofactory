#include "present/sdl2/pres_sdl2.h"
#include "render/rend_pick.h"
#include "render/rend_sw.h"

namespace
{
    RenderBackend g_backend = RenderBackend_Software;
    RenderBackbuffer g_buffer;
    bool g_ready = false;
}

bool pres_sdl2_init()
{
    g_ready = rend_pick_init(g_backend);
    if (g_ready)
    {
        g_ready = render_backbuffer_init(g_buffer, 320u, 200u);
    }
    return g_ready;
}

bool pres_sdl2_present(RenderContext &ctx)
{
    if (!g_ready && !pres_sdl2_init())
    {
        return false;
    }
    if (ctx.target == 0)
    {
        ctx.target = &g_buffer;
    }
    return rend_pick_frame(g_backend, ctx);
}

void pres_sdl2_shutdown()
{
    if (g_ready)
    {
        rend_pick_shutdown(g_backend);
        render_backbuffer_free(g_buffer);
        g_ready = false;
    }
}
