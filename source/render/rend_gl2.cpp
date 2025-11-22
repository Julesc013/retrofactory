#include "render/rend_gl2.h"

bool rend_gl2_init()
{
    return true;
}

bool rend_gl2_frame(RenderContext &ctx)
{
    return render_frame(ctx);
}

void rend_gl2_shutdown()
{
}
