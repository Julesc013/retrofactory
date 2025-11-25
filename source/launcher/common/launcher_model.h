#pragma once

#include "system/types.h"
#include "render/rend_backend.h"

enum LauncherEdition
{
    Edition_1x,
    Edition_3x,
    Edition_9x,
    Edition_Nx,
    Edition_Lx,
    Edition_Mx,
    Edition_Cx,
    Edition_Ax,
    Edition_Wx
};

struct LauncherModel
{
    LauncherEdition edition;
    RenderBackendId renderer; /* selected render backend */
};

void launcher_model_init(LauncherModel &model, LauncherEdition edition);
