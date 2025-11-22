#pragma once

#include "render/rend_api.h"

bool pres_sdl2_init();
bool pres_sdl2_present(RenderContext &ctx);
void pres_sdl2_shutdown();
