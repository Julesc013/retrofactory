#include "present/qdraw/pres_qdraw.h"

bool pres_qdraw_init()
{
    return true;
}

bool pres_qdraw_present(RenderContext &ctx)
{
    return render_frame(ctx);
}

void pres_qdraw_shutdown()
{
}
