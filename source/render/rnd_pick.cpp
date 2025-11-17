#include "render/rnd_pick.h"

int pick_render_backend(const RenderConfig &config)
{
    (void)config;
    return RENDER_BACKEND_SW;
}
