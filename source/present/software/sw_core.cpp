#include "present/software/sw_core.h"

bool sw_core_init()
{
    return true;
}

bool sw_core_present(RenderContext &ctx)
{
    return render_frame(ctx);
}

void sw_core_shutdown()
{
}
