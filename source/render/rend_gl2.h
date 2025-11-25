#pragma once

#include "render/rend_api.h"

bool rend_gl2_init(void);
bool rend_gl2_frame(RenderContext *ctx);
void rend_gl2_shutdown(void);
