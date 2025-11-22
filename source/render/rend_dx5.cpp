#include "render/rend_dx5.h"

bool rend_dx5_init()
{
    return true;
}

bool rend_dx5_frame(RenderContext &ctx)
{
    return render_frame(ctx);
}

void rend_dx5_shutdown()
{
}
