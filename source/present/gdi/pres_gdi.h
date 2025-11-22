#pragma once

#include "render/rend_api.h"

bool pres_gdi_init();
bool pres_gdi_present(RenderContext &ctx);
void pres_gdi_shutdown();
