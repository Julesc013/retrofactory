#include "present/gdi/pres_gdi.h"

bool pres_gdi_init()
{
    return true;
}

bool pres_gdi_present(RenderContext &ctx)
{
    return render_frame(ctx);
}

void pres_gdi_shutdown()
{
}
