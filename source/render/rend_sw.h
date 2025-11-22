#pragma once

#include "render/rend_api.h"

bool rend_sw_init();
bool rend_sw_frame(RenderContext &ctx);
void rend_sw_shutdown();
