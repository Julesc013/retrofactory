#include "render/rend_sw.h"

bool rend_sw_init()
{
    return true;
}

bool rend_sw_frame(RenderContext &ctx)
{
    return render_frame(ctx);
}

void rend_sw_shutdown()
{
}
