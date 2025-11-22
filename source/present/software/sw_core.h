#pragma once

#include "render/rend_api.h"

bool sw_core_init();
bool sw_core_present(RenderContext &ctx);
void sw_core_shutdown();
