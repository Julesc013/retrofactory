#include "render/rnd_api.h"

#include "render/rnd_pick.h"
#include "render/rnd_sw.h"

namespace
{
    int g_render_backend = -1;
}

bool render_init(const RenderConfig &config)
{
    if (config.window == 0)
    {
        return false;
    }

    g_render_backend = pick_render_backend(config);

    switch (g_render_backend)
    {
    case RENDER_BACKEND_SW:
        return rnd_sw_init(config);
    default:
        break;
    }

    return false;
}

void render_frame()
{
    switch (g_render_backend)
    {
    case RENDER_BACKEND_SW:
        rnd_sw_render();
        break;
    default:
        break;
    }
}

void render_shutdown()
{
    switch (g_render_backend)
    {
    case RENDER_BACKEND_SW:
        rnd_sw_shutdown();
        break;
    default:
        break;
    }

    g_render_backend = -1;
}
