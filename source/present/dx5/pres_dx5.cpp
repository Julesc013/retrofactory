#include "present/dx5/pres_dx5.h"

bool pres_dx5_init()
{
    return true;
}

bool pres_dx5_present(RenderContext &ctx)
{
    return render_frame(ctx);
}

void pres_dx5_shutdown()
{
}
