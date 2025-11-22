#include "present/web/pres_web.h"

bool pres_web_init()
{
    return true;
}

bool pres_web_present(RenderContext &ctx)
{
    return render_frame(ctx);
}

void pres_web_shutdown()
{
}
