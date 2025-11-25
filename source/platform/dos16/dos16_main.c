#include "engine/retro_runtime.h"

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    RetroRuntime *rt = retro_runtime_create(32u, 24u, RETRO_PRESENTER_SOFTWARE);
    if (rt == 0)
    {
        return 1;
    }

    if (!retro_runtime_init(rt, 0))
    {
        retro_runtime_destroy(rt);
        return 1;
    }

    u32 frame;
    for (frame = 0u; frame < 120u; ++frame)
    {
        if (!retro_runtime_step(rt))
        {
            break;
        }
    }

    retro_runtime_destroy(rt);
    return frame == 120u ? 0 : 1;
}
