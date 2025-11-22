#include "render/rend_gl1.h"

bool rend_gl1_init()
{
    return true;
}

bool rend_gl1_frame(RenderContext &ctx)
{
    return render_frame(ctx);
}

void rend_gl1_shutdown()
{
}
