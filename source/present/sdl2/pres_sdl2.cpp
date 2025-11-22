#include "present/sdl2/pres_sdl2.h"

bool pres_sdl2_init()
{
    return true;
}

bool pres_sdl2_present(RenderContext &ctx)
{
    return render_frame(ctx);
}

void pres_sdl2_shutdown()
{
}
